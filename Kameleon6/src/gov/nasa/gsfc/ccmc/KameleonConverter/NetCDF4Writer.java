package gov.nasa.gsfc.ccmc.KameleonConverter;

import edu.ucar.ral.nujan.netcdf.NhDimension;
import edu.ucar.ral.nujan.netcdf.NhFileWriter;
import edu.ucar.ral.nujan.netcdf.NhGroup;
import edu.ucar.ral.nujan.netcdf.NhVariable;

public class NetCDF4Writer extends Writer {

	public void convert(Model m){


		try{
			super.convert(m);

			NhFileWriter hfile = new NhFileWriter(newfilename,NhFileWriter.OPT_OVERWRITE);
			NhGroup grp = hfile.getRootGroup();


			//------------------------------adding Variables and Variable Attributes----------------------------
			logger.info("Writing Variables...");
			NhGroup folder = grp.addGroup("Variables");
			NhVariable [] vars = new NhVariable[m.VarObjsSize()];
			Variable var; 
			int dtype=0;
			NhDimension[] nhDims = new NhDimension[1];
			int[] chunkLengths = new int[1];

			//creating variables
			for (int i=0; i<m.VarObjsSize(); i++){
				var=m.getVariableObject(i);
				
				//checking to make sure all required information about the variable exists
				if(var.dataValues == null || var.dt == null || var.numDim == 0 || var.numElem == 0 || var.dimSizes == null){
					logger.error("**ERROR**Missing required information about Variable "+ var.originalName);
					logger.error("Cannot convert. \nEXITING...");
					System.exit(0);
				}

				nhDims[0]=grp.addDimension("numberOfElements"+i, var.numElem);
				chunkLengths[0]=var.numElem;

				if(var.dt.equalsIgnoreCase("float")){
					dtype=NhVariable.TP_FLOAT;
				}
				else if(var.dt.equalsIgnoreCase("int")){
					dtype=NhVariable.TP_INT;
				}
				else if(var.dt.equalsIgnoreCase("double")){
					dtype=NhVariable.TP_DOUBLE;
				}
				else{
					logger.error("Trouble creating a variable object: "+ var.KameleonName);
					System.exit(0);
				}
				vars[i]=folder.addVariable(var.KameleonName, dtype, nhDims, chunkLengths, null, 0);

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
						if(!vars[i].attributeExists(attr.name)){
							if(attr.dataType.toString().toLowerCase().indexOf("flo")!=-1){
								dtype=NhVariable.TP_FLOAT;
								vars[i].addAttribute(attr.name, dtype,(Float)attr.value);
							}
							else if(attr.dataType.toString().toLowerCase().indexOf("str")!=-1){
								dtype=NhVariable.TP_STRING_VAR;
								vars[i].addAttribute(attr.name, dtype, (String) attr.value);
							}
							else  if(attr.dataType.toString().toLowerCase().indexOf("double")!=-1){
								dtype=NhVariable.TP_DOUBLE;
								vars[i].addAttribute(attr.name, dtype, ((Double) attr.value));
							}
							else if(attr.dataType.toString().toLowerCase().indexOf("int")!=-1){
								dtype=NhVariable.TP_INT;
								vars[i].addAttribute(attr.name, dtype,((Integer)attr.value));
							}
							else
								logger.error("Data Type of Attribute Unknown.");

						}else{
							if(CommandLineInterface.verboseFlag)
								logger.debug("'Twas a duplicate and not added:"+attr.name+" "+ var.KameleonName);
						}

					} catch(ClassCastException cc){
						//this is a workaround for a class cast problem
						dtype=NhVariable.TP_STRING_VAR;
						vars[i].addAttribute(attr.name, dtype, (String) attr.value);
					}
				}
			}
			//-----------------end adding Variables and Variable Attributes---------------
			hfile.endDefine();

			

			//------------------------adding Global Attributes---------------------
			logger.info("Writing Global Attributes...");
			dtype=0;

			KAttribute ga;
			for(int i=0; i<m.GloAttrsSize();i++){
				ga=m.getGlobalAttribute(i);
				if(ga.dataType.toString().toLowerCase().indexOf("flo")!=-1){
					dtype=NhVariable.TP_FLOAT;
					grp.addAttribute(ga.name, dtype, (Float) ga.value);
				}
				else if(ga.dataType.toString().toLowerCase().indexOf("str")!=-1){
					dtype=NhVariable.TP_STRING_VAR;
					grp.addAttribute(ga.name, dtype, (String)ga.value);
				}
				else if(ga.dataType.toString().toLowerCase().indexOf("int")!=-1){
					dtype=NhVariable.TP_INT;
					grp.addAttribute(ga.name, dtype, (Integer) ga.value);
				}
				else if(ga.dataType.toString().toLowerCase().indexOf("long")!=-1){
					dtype=NhVariable.TP_LONG;
					grp.addAttribute(ga.name, dtype, (Long)ga.value);
				}
				else if(ga.dataType.toString().toLowerCase().indexOf("double")!=-1){
					dtype=NhVariable.TP_DOUBLE;
					grp.addAttribute(ga.name, dtype, (Double) ga.value);
				}



			}
			//-------------------end adding Global Attributes----------------------

			
			
			//------------writing Data to Variables-----------
			logger.info("Writing data to Variables...");
			for(int j=0; j<m.VarObjsSize(); j++){
				var=m.getVariableObject(j);

				int[] indices= {0};

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
			hfile.close();

		}catch (Exception e){
			e.printStackTrace();
		}

	}


	public String toString(){
		return "NetCDF4 File";
	}

	public String extension(){
		return ".nc";
	}

}
