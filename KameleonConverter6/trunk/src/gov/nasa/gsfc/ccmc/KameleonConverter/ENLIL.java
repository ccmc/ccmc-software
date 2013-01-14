/*
 * 
 * Name: ENLIL.java
 * 
 * Version: 6.0
 * 
 * Author: Nitesh Donti
 * 		   NASA-GSFC-CCMC (Code 587)
 * 		   Intern
 * 
 * Purpose: Custom read routine for the 
 * 			Enlil input model file. Supports those
 * 			with and without extra CME data. 
 *  
 * Modification History:
 *  
 * Summer 2011 	Donti, Nitesh
 * 				Initial Development Started
 * 				All tasks complete
 * 				Ready to use
 * 
 * Tasks:	Need to update the README because developers
 * 	 		no longer use the EVP files. 	 
 * 
 * Winter 2012: Starting to build support for Enlil files with CMEs
 *  			Bug fixes and adapting to other changes that occurred over the year  
 * 
 */


package gov.nasa.gsfc.ccmc.KameleonConverter;
import java.io.IOException;
import org.apache.log4j.Logger;
import ucar.ma2.Array;
import ucar.nc2.NetcdfFile;


public class ENLIL extends Model{
	boolean has_CME;

	/**
	 * Logger for the ENLIL.java class. 
	 * Use Logger.info(), Logger.debug(), and Logger.error() to print
	 * statements out within the class. 
	 */
	static Logger logger = Logger.getLogger(ENLIL.class);

	/**The Netcdf/Enlil file from which to read.*/
	NetcdfFile file;          		

	/**
	 * The List of Variables from the NetCDF File. Note that these Variables are
	 * different from the CDF Variables and from the KameleonConverter Variables. 
	 */
	java.util.List<ucar.nc2.Variable> ncVariables;

	ENLIL(boolean has_CME){
		super(has_CME? "enlilcme.xml":"enlil.xml");
		this.has_CME = has_CME;
	}

	/**
	 * @throws NoAttributeException
	 * @throws IOException 
	 * @throws NoVariableException 
	 */
	public void read() throws NoAttributeException, NoVariableException, IOException{
		super.read();

		//opening the enlil time file to get all of the information
		logger.info("Now OPENING " + FilePathname);
		//if opening this file does not work, the Command Line Interface will handle the exception and alert the user
		file=NetcdfFile.open(FilePathname);
		setGlobalAttrs();
		setVariableAttrs();
		
		this.getGlobalAttribute("original_output_file_name").value=
			this.FilePathname.substring(this.FilePathname.lastIndexOf('/'));
		logger.info("\n\nThe information for your new java "+ this 
				+" has just been read from the original file!");
		logger.debug(super.getVariableObject(6).cal[9]);
	}


	public void setGlobalAttrs() throws NoAttributeException{
		logger.info("\nsetting Global Attributes...");

		for (int i=0; i <file.getGlobalAttributes().size(); i++){
			if(CommandLineInterface.verboseFlag)
				logger.debug(file.getGlobalAttributes().get(i));

			String prefix="";

			//this keyword is the name of the global attribute from the enlil 
			//file that tells us what time the file is simulating
			String keyword = "refdate_cal";

			if(file.getGlobalAttributes().get(0).getName().toLowerCase().indexOf("type")!=-1){
				prefix+=file.getGlobalAttributes().get(0).getStringValue().toLowerCase()+"_";
			}
			if(file.getGlobalAttributes().get(2).getName().toLowerCase().indexOf("label")!=-1 && !file.getGlobalAttributes().get(2).getStringValue().equals("")){
				prefix+=file.getGlobalAttributes().get(2).getStringValue().toLowerCase()+"_";
			}

			//adding the new global attributes with prefix "time_"
			if(file.getGlobalAttributes().get(i).getValue(0)!=null && !file.getGlobalAttributes().get(i).getValue(0).equals("")){
				if(file.getGlobalAttributes().get(i).getName().equals(keyword))
					this.getGlobalAttribute("start_time").value=file.getGlobalAttributes().get(i).getValue(0);
				else
					this.addGlobalAttribute(new KAttribute (prefix+file.getGlobalAttributes().get(i).getName(), file.getGlobalAttributes().get(i).getValue(0), null, "model", file.getGlobalAttributes().get(i).getDataType().name()));
			}
		}

		int grid_system_1_number_of_dimensions=0;
		int dimension_num=0;
		for(int u=0; u<file.getDimensions().size(); u++){
			if(file.getDimensions().get(u).getLength()>1){
				grid_system_1_number_of_dimensions++;
				this.addGlobalAttribute(new KAttribute("grid_system_1_dimension_" + (++dimension_num) + "_size",file.getDimensions().get(u).getLength(),"Size of dimension m for grid n (e.g. grid_system_1_dimension_2_size)", "model", "int"));
			}
		}

		//i'm not sure if this is needed. I have to test.
		grid_system_1_number_of_dimensions=3;
		this.addGlobalAttribute(new KAttribute("grid_system_1_number_of_dimensions",grid_system_1_number_of_dimensions,"The number m of how many dimensions are in grid n. So for every grid there will be a corresponding grid_system_n_number_of_dimensions attribute (e.g. The first grid will have an attribute grid_system_1_number_of_dimensions)" ,"model", "int"));
		if(grid_system_1_number_of_dimensions>3){
			logger.error("The Grid System has greater than 3 dimensions. There has been a change to the ENLIL structure. Please check the standards and the input files.");
			logger.error("EXITING...");
			System.exit(0);
		}

	}

	/**
	 * This CREATES the VARIABLES and sets the ModelSpecificVariableAttributes for the Enlil Variables. 
	 * For the CCMC Variable Attributes, the Writer Object will set the values, because those are hardcoded in an xml file entitled Variables.xml. 
	 * 
	 * This Also Writes the Data to the Variable Objects.
	 * 
	 * @throws NoAttributeException
	 * @throws NoVariableException 
	 * @throws IOException 
	 */
	public void setVariableAttrs() throws NoAttributeException, NoVariableException, IOException{
		logger.info("\nsetting Variable Attributes...");
		ncVariables = file.getVariables();

		/**
		 * A temporary model specific attribute that holds the place of a model specific attribute that was just added to the variable.
		 */
		KAttribute tempMSA; 

		for(int i=0; i<ncVariables.size() ; i++){
			/**
			 * A temporary variable that holds the place of a variable that was just added to the object. 
			 */
			Variable tempV = null;

			//if we know what to map the original name to, we will
			//if not, we will add it as an unknown kameleon variable
			if(!ncVariables.get(i).getName().equals("TIME") && !ncVariables.get(i).getName().equals("DT") && !ncVariables.get(i).getName().equals("NSTEP")){

				logger.info("Starting to read full Variable Data for Variable #" + (i+1) + " of "+ ncVariables.size());

				//mapping original name to kameleon name
				if(orig2kamel.get(ncVariables.get(i).getName())!=null){
					tempV=new Variable(ncVariables.get(i).getName(),orig2kamel.get(ncVariables.get(i).getName()));}
				else{
					logger.info("**No Kameleon name known for this input variable: "+ncVariables.get(i).getName());
					tempV= new Variable(ncVariables.get(i).getName(),"kameleon_identity_unknown"+i);
				}

				//setting the number of elements field of an Enlil Variable Object
				tempV.numElem=(int)(ncVariables.get(i).getSize());
				//setting the number of dimensions field for an Enlil Variable Object
				tempV.numDim=ncVariables.get(i).getRank();
				//setting the dimension Sizes field for the Enlil Variable Object
				tempV.dimSizes= new int[ncVariables.get(i).getRank()];

				// both versions have 2+ dimensions here, depending on the variable. The 1st is always either 1 (no CME) or 2 (CME).

				//Adds the Attribute Values to the CCMC Standard Variables. These attributes and attribute values come from Variables.xml. 
				//This must be added before the Actual Min/Max are converted and before the Model Specific Attributes are added. 
				getAttrValues(tempV);
				

				//Adds Model Specific Attributes to Variables
				for(int j=0; j<ncVariables.get(i).getAttributes().size();j++){
					tempMSA=new KAttribute(ncVariables.get(i).getAttributes().get(j).getName(), 
							ncVariables.get(i).getAttributes().get(j).getValue(0), null, "model",
							ncVariables.get(i).getAttributes().get(j).getDataType().toString());
					tempV.addAttribute(tempMSA);
				}

				//The reason we add the CCMC Variable Attributes first is so that the actual data from the NetCDF File can overwrite the standard data if need be. 
				//lutz says to extract the data in two parts, one for each layer. then, if it's CME, make a new variable with an appended name. so EnlilCMEs have twice the (significant) variables.



				//-------------------------------setting the DATA for the Enlil Variable  AND CALCULATING Actual Min/Max-----------------
				tempV.dt = ncVariables.get(i).getDataType().toString().toLowerCase();
				if (tempV.dt.equalsIgnoreCase("double") || tempV.dt.equalsIgnoreCase("boolean")){
					if(CommandLineInterface.verboseFlag)
						logger.debug("TYPE: "+tempV.dt.toUpperCase());
				}
				else if (tempV.dt.equalsIgnoreCase("int")){

					if(CommandLineInterface.verboseFlag)
						logger.debug("TYPE: INT");
					try {
						Array a = ncVariables.get(i).read();
						tempV.dataValues=(int[]) a.copyTo1DJavaArray();

						//--
						if (this.has_CME){
							int[] vals = ((int []) tempV.dataValues);
							int val_indexer =0;
							int[] values_set1 = new int[vals.length/2]; 
							int[] values_set2 = new int[vals.length/2];

							for (int f : vals){
								if (val_indexer<vals.length/2){
									values_set1[val_indexer]=f;
								}
								else{
									values_set2[val_indexer%(vals.length/2)]=f;								
								}
								val_indexer++;
							}

							tempV.dataValues = values_set1;      					//what I'm doing here is only assigning the first layer to this variable (the one with the full output including the CME cone model)
							tempV.dimSizes[0] = 1;
							tempV.numElem/=2;
						

						//----creating a background variable for the EnlilCME case...--
					
							Variable background_var;
							if(orig2kamel.get(ncVariables.get(i).getName())!=null){
								logger.info(ncVariables.get(i).getName());
								background_var=new Variable(ncVariables.get(i).getName()+"'",orig2kamel.get(ncVariables.get(i).getName()+"'"));
								background_var.dataValues = values_set2;
								background_var.numElem=tempV.numElem;
								background_var.numDim = tempV.numDim;
								background_var.dimSizes = tempV.dimSizes;
								background_var.dt = tempV.dt;

								getAttrValues(background_var);

								//setting min and max values
								if(!CommandLineInterface.nominmaxFlag){
									minmaxCalculator(background_var, (int[]) background_var.dataValues);	
								}

								this.addVariableObject(background_var);
							}
							else{
								logger.error("**"+ncVariables.get(i).getName()+" has no Kameleon name and was not added as a variable.");
							}
						}
						//--
						
						if(!CommandLineInterface.nominmaxFlag){
							super.minmaxCalculator(tempV, ((int[])tempV.dataValues));
						}

					} catch (IOException e) {
						e.printStackTrace();
					}
				}
				else if (tempV.dt.equalsIgnoreCase("float")){
					if(CommandLineInterface.verboseFlag)
						logger.debug("TYPE: FLOAT");
					try {

						Array a = ncVariables.get(i).read();
						if(CommandLineInterface.verboseFlag){
							logger.info("info: "+ a.shapeToString());
						}
						tempV.dataValues = (float[]) a.copyTo1DJavaArray();

						if (this.has_CME){
							float[] vals = ((float []) tempV.dataValues);
							int val_indexer =0;
							float[] values_set1 = new float[vals.length/2]; 
							float[] values_set2 = new float[vals.length/2];

							for (float f : vals){
								if (val_indexer<vals.length/2){
									values_set1[val_indexer]=f;
								}
								else{
									values_set2[val_indexer%(vals.length/2)]=f;								
								}
								val_indexer++;
							}

							tempV.dataValues = values_set1;      					//what I'm doing here is only assigning the first layer to this variable (the one with the full output including the CME cone model)
							tempV.dimSizes[0] = 1;
							tempV.numElem/=2;
						

						

						//----creating a background variable for the EnlilCME case...--
					
							Variable background_var;
							if(orig2kamel.get(ncVariables.get(i).getName())!=null){
								logger.info(ncVariables.get(i).getName());
								background_var=new Variable(ncVariables.get(i).getName()+"'",orig2kamel.get(ncVariables.get(i).getName()+"'"));
								background_var.dataValues = values_set2;
								background_var.numElem=tempV.numElem;
								background_var.numDim = tempV.numDim;
								background_var.dimSizes = tempV.dimSizes;
								background_var.dt = tempV.dt;

								getAttrValues(background_var);

								//setting min and max values
								if(!CommandLineInterface.nominmaxFlag){
									minmaxCalculator(background_var, (float[]) background_var.dataValues);	
								}

								this.addVariableObject(background_var);
							}
							else{
								logger.error("**"+ncVariables.get(i).getName()+" has no Kameleon name and was not added as a variable.");
							}
						}
						
						
						//setting min and max values
						if(!CommandLineInterface.nominmaxFlag){
							super.minmaxCalculator(tempV, (float[]) tempV.dataValues);				
						}

						//---------------------

					} catch (IOException e) {
						e.printStackTrace();
					}
				}
				else {
					logger.error("\n***ERROR***Data type of variable"+tempV.originalName+" is unknown. Adding anyway with little information...");
				}
				//---------------------------------------end setting data for enlil variable-------------------------------------
				this.addVariableObject(tempV);
			}
			else{
				logger.info("Skipping Variable #"+(i+1)+" because it is not required as a Variable Object.");
				if(ncVariables.get(i).getSize()==1){
					String name=ncVariables.get(i).getName();

					if(name.equalsIgnoreCase("NSTEP"))
						name="time_numerical_time_step";
					else if(name.equalsIgnoreCase("DT"))
						name="time_physical_time_step";
					else if(name.equalsIgnoreCase("TIME"))
						name="time_physical_time";
					logger.info("...Adding its value as a Global Attribute instead.");

					if(ncVariables.get(i).getDataType().toString().toLowerCase().indexOf("float")!=-1){
						this.addGlobalAttribute(new KAttribute(name,ncVariables.get(i).readScalarFloat(), null,"model", "float"));
					}
					else if(ncVariables.get(i).getDataType().toString().toLowerCase().indexOf("int")!=-1){
						this.addGlobalAttribute(new KAttribute(name,ncVariables.get(i).readScalarInt(), null,"model","int"));
					}
					else if(ncVariables.get(i).getDataType().toString().toLowerCase().indexOf("string")!=-1){
						this.addGlobalAttribute(new KAttribute(name,ncVariables.get(i).readScalarString(), null,"model", "string"));
					}

				}else{
					logger.error("//Purpose of Variable #"+ (i+1) + " unknown.");
				}
			}
			
			/*
			if(tempV!=null){
				logger.debug(tempV);

				for(KAttribute e: tempV.cal){
					logger.debug(e);
				}
			}
			*/
			
			
		}

		
		if(doesVarExist("r1") && doesVarExist("theta1") && doesVarExist("phi1")){
			this.getGlobalAttribute("grid_system_count").value=2; 
			this.addGlobalAttribute(new KAttribute("grid_system_2","[r1,theta1,phi1]" , "Outline how a particular grid system is defined by showing coordinates used (e.g. [X,Y,Z] where X,Y,Z are position variables defined in current file)","model","String" ));
			this.addGlobalAttribute(new KAttribute("grid_2_type","HNM" ,"Keywords identifying all grids used in current model output. Grid types will be registered in external coordinate transformation package. (e.g. grid_1_type=GSM)","model", "String"));

			Variable v = this.getVariableObject("r1");

			int grid_system_2_number_of_dimensions=0;
			int dimension_num=0;
			for(int u=0; u<v.numDim; u++){
				if(v.dimSizes[u]>1){
					grid_system_2_number_of_dimensions++;
					this.addGlobalAttribute(new KAttribute("grid_system_2_dimension_" + (++dimension_num) + "_size",(int)(v.dimSizes[u]),"Size of dimension m for grid n (e.g. grid_system_1_dimension_2_size)","model", "int"));
				}
			}
			this.addGlobalAttribute(new KAttribute("grid_system_2_number_of_dimensions",grid_system_2_number_of_dimensions,"The number m of how many dimensions are in grid n. So for every grid there will be a corresponding grid_system_n_number_of_dimensions attribute (e.g. The first grid will have an attribute grid_system_1_number_of_dimensions)" ,"model","int"));
			if(grid_system_2_number_of_dimensions>3){
				logger.error("The Grid System 2 has greater than 3 dimensions. There has been a change to the ENLIL structure. Please check the standards and the input files.");
				logger.error("EXITING...");
				System.exit(0);
			}

		}


		if(doesVarExist("bp")){
			this.addPolarityCorrectionVariable("VARIABLE CREATED FROM BP/bp + B1/br", "b1r", "br");
			this.addPolarityCorrectionVariable("VARIABLE CREATED FROM BP/bp + B2/btheta","b1theta", "btheta");
			this.addPolarityCorrectionVariable("VARIABLE CREATED FROM BP/bp + B3/bphi", "b1phi", "bphi");
		}

		logger.info("\n Created "+ this.getNumVariables() + " variable(s).");
	}


	/**
	 * This creates a new variable that is a copy of an old variable except with possible changes in its data. 
	 * If the value of the BP variable is negative at some index, then this new variable's value will be the original variable's data times negative one. 
	 * 
	 * E.g.   BP variable = "bp"       new variable = "b1r"     original variable = "br"
	 * 
	 * if BP Variable has a value of -4 at its index#8 in its data array, then new variable will have the opposite value at index#8 than does original variable. 
	 * 
	 * @param origName For the new variable, the original name that you want it to have (briefly explaining how it was created)
	 * @param KamelName For the new variable, the Kameleon name that you want it to have. 
	 * @param origVarKamelName The Kameleon name of the Variable of which you are correcting the polarity. 
	 * @throws NoVariableException In the case that some variable does not exist (even though this is already checked).
	 */
	public void addPolarityCorrectionVariable(String origName, String KamelName, String origVarKamelName) throws NoVariableException{

		if(doesVarExist(origVarKamelName)){
			Variable tempNewVar = new Variable(origName, KamelName);
			Variable tempOrigVar= this.getVariableObject(origVarKamelName);

			try {
				tempNewVar.dataValues=((float[])tempOrigVar.dataValues).clone();
			} catch (ClassCastException e) {
				tempNewVar.dataValues=((int[])tempOrigVar.dataValues).clone();
			}

			for(int i=0; i<tempOrigVar.numElem; i++){
				((float[])tempNewVar.dataValues)[i]*=-1;
			}

			tempNewVar.dimSizes=tempOrigVar.dimSizes.clone();
			tempNewVar.dt=tempOrigVar.dt;
			tempNewVar.numDim=tempOrigVar.numDim;
			tempNewVar.numElem=tempOrigVar.numElem;

			//Adds the Attribute Values to the CCMC Standard Variables. These attributes and attribute values come from Variables.xml. 
			try {
				tempNewVar.cal=varxmlparser.Kname2var.get(tempNewVar.KameleonName).cal;
				for(int p=0; p<tempNewVar.cal.length; p++)
					tempNewVar.nameToAttribute.put(tempNewVar.cal[p].name, tempNewVar.cal[p]);
			} catch (Exception e) {
				e.printStackTrace();
			}


			if(!CommandLineInterface.nominmaxFlag){

				if(tempOrigVar.dt.equalsIgnoreCase("float")){
					super.minmaxCalculator(tempNewVar, ((float[])tempNewVar.dataValues));}
				else if(tempOrigVar.dt.equalsIgnoreCase("int")){
					super.minmaxCalculator(tempNewVar, ((int[])tempNewVar.dataValues));}

			}

			this.addVariableObject(tempNewVar);
			logger.info("Adding new variable - "+KamelName+" - in order to correct polarity of variable "+ origVarKamelName+".");

		}

	}


	public String toString(){
		return has_CME? "EnlilCME Object" : "Enlil Object";
	}

}