package gov.nasa.gsfc.ccmc.KameleonConverter;

import org.apache.log4j.Logger;
import edu.ucar.ral.nujan.hdf.HdfException;
import edu.ucar.ral.nujan.hdf.HdfFileWriter;
import edu.ucar.ral.nujan.hdf.HdfGroup;

public class HDF5Writer extends Writer {
	static Logger logger = Logger.getLogger(HDF5Writer.class);

	public void convert(Model m) throws NoAttributeException {



		try {
			super.convert(m);

			HdfFileWriter w = new HdfFileWriter(newfilename,HdfFileWriter.OPT_ALLOW_OVERWRITE);
			int dtype = 0;

			//------------------------------adding Variables----------------------------
			logger.info("Writing Variables...");
			HdfGroup grp = w.getRootGroup().addGroup("Variables");
			HdfGroup[] vars = new HdfGroup[m.VarObjsSize()];
			Variable var; 


			for (int i=0; i<m.VarObjsSize(); i++){


				var=m.getVariableObject(i);

				//checking to make sure all required information about the variable exists
				if(var.dataValues == null || var.dt == null || var.numDim == 0 || var.numElem == 0 || var.dimSizes == null){
					logger.error("**ERROR**Missing required information about Variable "+ var.originalName);
					if(var.dataValues == null)
						logger.error("Data Values are Missing.");
					if(var.dt == null)
						logger.error("Data Type is Missing.");
					if(var.numDim == 0)
						logger.error("Number of Elements = 0.");
					if(var.numElem == 0)
						logger.error("Number of Elements = 0.");
					if(var.dimSizes == null)
						logger.error("Dimension Sizes are Missing.");
					logger.error("Cannot convert. \nEXITING...");
					System.exit(0);
				}

				if(var.dt.equalsIgnoreCase("float"))
					dtype=HdfGroup.DTYPE_FLOAT32;
				else if(var.dt.equalsIgnoreCase("int"))
					dtype=HdfGroup.DTYPE_FIXED32;
				else if(var.dt.equalsIgnoreCase("double"))
					dtype=HdfGroup.DTYPE_FLOAT64;
				else{
					logger.error("Data type of Variable is unknown. \nEXITING...");
					System.exit(0);
				}

				if(var.numElem==0){
					logger.error("The number of elements for variable " + var.KameleonName + " is not set. \nEXITING...");
					System.exit(0);
				}


				int[]dimSizesInts= {var.numElem};

				vars[i] = grp.addVariable(var.KameleonName, dtype, 0, dimSizesInts, dimSizesInts, null, 0);
				dtype=0;

				KAttribute attr=null;
				//-------adding ModelSpecific Attributes to the new Variable------
				//-------adding CCMC Attributes to the new Variable (does NOT overwrite any repeats)------
				for(int y= 0; y<var.getModelSpecAttributeNames().length + var.getCCMCAttributeNames().length; y++){
					try {
						if(y<var.getModelSpecAttributeNames().length)
							attr=var.getAttributeObject(var.getModelSpecAttributeNames()[y]);
						else
							attr=var.getAttributeObject(var.getCCMCAttributeNames()[y-var.getModelSpecAttributeNames().length]);

						//if the attribute does not already exist
						if(vars[i].findAttribute(attr.name)==null){
							if (attr.dataType==null){
								logger.error(attr.name + " for "+var.KameleonName+" has no listed data type. CHECK THIS.");
							} 
							else if(attr.dataType.toString().toLowerCase().indexOf("flo")!=-1){

								dtype=HdfGroup.DTYPE_FLOAT32;
								vars[i].addAttribute(attr.name, dtype,0, ((Float) attr.value), false);
							}
							else if(attr.dataType.toString().toLowerCase().indexOf("str")!=-1){
								dtype=HdfGroup.DTYPE_STRING_FIX;
								vars[i].addAttribute(attr.name, dtype, ((String) attr.value).length(), (String) attr.value, false);
							}
							else  if(attr.dataType.toString().toLowerCase().indexOf("double")!=-1){
								dtype=HdfGroup.DTYPE_FLOAT64;
								vars[i].addAttribute(attr.name, dtype,0, ((Double) attr.value), false);
							}
							else if(attr.dataType.toString().toLowerCase().indexOf("int")!=-1){
								dtype=HdfGroup.DTYPE_FIXED32;
								vars[i].addAttribute(attr.name, dtype,0, ((Integer)attr.value), false);
							}
							else
								logger.error("Data Type of Attribute Unknown.");

						}else{
							if(CommandLineInterface.verboseFlag)
								logger.debug("'Twas a duplicate and not added:"+attr.name+" "+ var.KameleonName);
						}

					} catch (HdfException e) {
						logger.error("Some Error.");
						e.printStackTrace();

					} catch(ClassCastException cc){
						//this is a workaround for a class cast problem
						dtype=HdfGroup.DTYPE_STRING_FIX;
						vars[i].addAttribute(attr.name, dtype, ((String) attr.value).length() , (String) attr.value, false);
					}
				}

			}





			//-----------------end adding Variables and Variable Attributes---------------



			//------------------------adding Global Attributes---------------------
			logger.info("Writing Global Attributes...");

			HdfGroup global = w.getRootGroup();
			KAttribute ga;

			if(m.GloAttrsSize()==0){
				logger.error("There are no global attributes. \nEXITING...");
				System.exit(0);
			}


			for(int i=0; i<m.GloAttrsSize();i++){
				ga=m.getGlobalAttribute(i);
				if(ga.value!=null){
					if(ga.dataType.toString().toLowerCase().indexOf("flo")!=-1){
						dtype=HdfGroup.DTYPE_FLOAT32;
						global.addAttribute(ga.name, dtype, 0, ((Float) ga.value).floatValue(), false);
					}
					else if(ga.dataType.toString().toLowerCase().indexOf("str")!=-1){
						dtype=HdfGroup.DTYPE_STRING_VAR;
						global.addAttribute(ga.name, dtype, 0, ((String) ga.value).toString(), false);
					}
					else if(ga.dataType.toString().toLowerCase().indexOf("int")!=-1){
						dtype=HdfGroup.DTYPE_FIXED32;
						global.addAttribute(ga.name, dtype, 0, ((Integer)ga.value).intValue(), false);
					}
					else if(ga.dataType.toString().toLowerCase().indexOf("long")!=-1){
						dtype=HdfGroup.DTYPE_FIXED64;
						global.addAttribute(ga.name, dtype, 0, ((Long)ga.value).longValue(), false);
					}
					else if(ga.dataType.toString().toLowerCase().indexOf("double")!=-1){
						dtype=HdfGroup.DTYPE_FLOAT64;
						global.addAttribute(ga.name, dtype, 0, ((Double) ga.value).doubleValue(), false);
					}else{
						logger.error("Problem with writing Global Attribute");
						System.exit(0);}
				}else if (CommandLineInterface.verboseFlag)
					logger.debug(ga.name + " has a value of " + ga.value +" and was not added an a global attribute.");

			}
			//-------------------end adding Global Attributes----------------------
			w.endDefine();


			int[] indices= {0};

			//------------writing Data to Variables-----------
			logger.info("Writing data to Variables...");
			for(int j=0; j<m.VarObjsSize(); j++){
				var=m.getVariableObject(j);

				if(var.dt.equalsIgnoreCase("float"))
					vars[j].writeData(indices,(float[])var.dataValues, false);
				else if (var.dt.equalsIgnoreCase("double"))
					vars[j].writeData(indices,(double[])var.dataValues, false);
				else if (var.dt.equalsIgnoreCase("int"))
					vars[j].writeData(indices,(int[])var.dataValues, false);
				else{
					logger.error("Unsupported data type of variable data values.");
					System.exit(0);
				}	
			}
			//-----------end writing Data-------------------
			w.close();


		} catch (HdfException e) {
			e.printStackTrace();
		}
	}

	public String toString(){
		return "HDF5 File";
	}

	public String extension(){
		return ".h5";
	}

}
