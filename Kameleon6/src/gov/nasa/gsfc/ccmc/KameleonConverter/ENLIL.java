package gov.nasa.gsfc.ccmc.KameleonConverter;
import java.io.IOException;
import org.apache.log4j.Logger;
import ucar.ma2.Array;
/*import ucar.ma2.ArrayFloat;
import ucar.ma2.ArrayFloat.D4;


					if(tempV.numDim==4){

						System.out.println("hi");

						ArrayFloat.D4 a = (D4) ncVariables.get(i).read();
						tempV.dataValues=new float[(int) tempV.numElem];
						int num=0;

						float b;	

						float min_value = Float.MAX_VALUE;
						float max_value = Float.MIN_VALUE;

						for(int m=0;m<a.getShape()[0];m++)
							for(int j=0;j<a.getShape()[1];j++)
								for(int k=0;k<a.getShape()[2];k++)
									for(int l=0;l<a.getShape()[3];l++){
										b= (float) a.get(m, j, k, l);

										((float[])(tempV.dataValues))[num]= b;



										if ( b< min_value)
										{
											min_value = b;
										} else if (b > max_value)
										{
											max_value = b;
										}	

										num++;

									}


						tempV.addAttribute(new KAttribute("actual_min", min_value, "Smallest value in the data for a particular variable.", "model", "float"));
						tempV.addAttribute(new KAttribute("actual_max", max_value, "Largest value in the data for a particular variable.","model", "float"));

				}

				else{

 */
import ucar.nc2.NetcdfFile;

//Task?: Change the valid_min and valid_max to String values
// Update the README because we do not use the evp files anymore. 

public class ENLIL extends Model{

	static Logger logger = Logger.getLogger(ENLIL.class);

	/**The Netcdf/Enlil file from which to read.*/
	NetcdfFile file;          		

	/**
	 * The List of Variables from the NetCDF File. Note that these Variables are different from the CDF Variables and from the KameleonConverter Variables. 
	 */
	java.util.List<ucar.nc2.Variable> ncVariables;

	ENLIL(){
		super("enlil.xml");
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

		logger.info("\nsetting Global Attributes...");
		setGlobalAttrs();

		logger.info("\nsetting Variable Attributes...");
		setVariableAttrs();

		this.getGlobalAttribute("original_output_file_name").value=this.FilePathname.substring(this.FilePathname.lastIndexOf('/'));

		System.out.println("\n\nThe information for your new java "+ this +" has just been read from the original file!");
	}


	public void setGlobalAttrs() throws NoAttributeException{

		for (int i=0; i <file.getGlobalAttributes().size(); i++){
			if(CommandLineInterface.verboseFlag)
				logger.debug(file.getGlobalAttributes().get(i));

			String prefix="";

			//this keyword is the name of the global attribute from the enlil file that tells us what time the file is simulating
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
	 * 
	 * @throws NoAttributeException
	 * @throws NoVariableException 
	 * @throws IOException 
	 */
	public void setVariableAttrs() throws NoAttributeException, NoVariableException, IOException{

		ncVariables = file.getVariables();
		/**
		 * A temporary variable that holds the place of a variable that was just added to the object. 
		 */
		Variable tempV;

		/**
		 * A temporary model specific attribute that holds the place of a model specific attribute that was just added to the variable.
		 */
		KAttribute tempMSA; 


		if(CommandLineInterface.testing){

			logger.info("[**Testing Mode: Variables at indices 0,2,4... will be skipped to save time.]");

		}


		for(int i=0; i<ncVariables.size() ; i++){


			//if we know what to map the original name to, we will
			//if not, we will add it as an unknown kameleon variable
			if(!ncVariables.get(i).getName().equals("TIME") && !ncVariables.get(i).getName().equals("DT") && !ncVariables.get(i).getName().equals("NSTEP")){

				//facilitates skipping and saving time when in testing mode
				if(CommandLineInterface.testing && i!=ncVariables.size()-1){
					i++; }

				logger.info("Starting to read full Enlil Variable Data for Variable #" + (i+1)+ " of "+ ncVariables.size());

				//mapping original name to kameleon name
				if(orig2kamel.get(ncVariables.get(i).getName())!=null)
					tempV=new Variable(ncVariables.get(i).getName(),orig2kamel.get(ncVariables.get(i).getName()));
				else
					tempV= new Variable(ncVariables.get(i).getName(),"kameleon_identity_unknown"+i);


				//setting the number of elements field of an Enlil Variable Object
				tempV.numElem=(int)(ncVariables.get(i).getSize());


				//setting the number of dimensions field for an Enlil Variable Object
				tempV.numDim=ncVariables.get(i).getRank();
				//setting the dimension Sizes field for the Enlil Variable Object
				tempV.dimSizes= new int[ncVariables.get(i).getRank()];
				for(int k=0; k<tempV.dimSizes.length; k++)
					tempV.dimSizes[k]=ncVariables.get(i).getShape(k);


				//setting the DATA for the Enlil Variable  AND CALCULATING Actual Min/Max
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
						tempV.dataValues= (int[]) a.copyTo1DJavaArray();

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
						tempV.dataValues = (float[]) a.copyTo1DJavaArray();


						//setting min and max values
						if(!CommandLineInterface.nominmaxFlag){
							super.minmaxCalculator(tempV, ((float[])tempV.dataValues));			
						}
						//}


					} catch (IOException e) {
						e.printStackTrace();
					}
				}
				else {
					System.err.println("\n***ERROR***Data type of variable attribute "+tempV.originalName+" is unknown.");
				}
				//end setting data for enlil variable






				//Adds the Attribute Values to the CCMC Standard Variables. These attributes and attribute values come from Variables.xml. 
				getCCMCstandardattrs(tempV);
				

				//Adds Model Specific Attributes to Variables
				for(int j=0; j<ncVariables.get(i).getAttributes().size();j++){



					tempMSA=new KAttribute(ncVariables.get(i).getAttributes().get(j).getName(), 
							ncVariables.get(i).getAttributes().get(j).getValue(0), null, "model",
							ncVariables.get(i).getAttributes().get(j).getDataType().toString());

					tempV.addAttribute(tempMSA);
				}

				//The reason we add the CCMC Variable Attributes first is so that the actual data from the NetCDF File can overwrite the standard data if need be. 
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
				System.err.println("The Grid System 2 has greater than 3 dimensions. There has been a change to the ENLIL structure. Please check the standards and the input files.");
				System.err.println("EXITING...");
				System.exit(0);
			}

		}


		if(doesVarExist("bp")){
			this.addPolarityCorrectionVariable("VARIABLE CREATED FROM BP/bp + B1/br", "b1r", "br");
			this.addPolarityCorrectionVariable("VARIABLE CREATED FROM BP/bp + B2/btheta","b1theta", "btheta");
			this.addPolarityCorrectionVariable("VARIABLE CREATED FROM BP/bp + B3/bphi", "b1phi", "bphi");
		}

		logger.info("\n Created "+ this.VarObjsSize() + " variable(s).");
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
			Variable tempBPVar= this.getVariableObject("bp");

			try {
				tempNewVar.dataValues=((float[])tempOrigVar.dataValues).clone();
			} catch (ClassCastException e) {
				tempNewVar.dataValues=((int[])tempOrigVar.dataValues).clone();
			}

			for(int i=0; i<tempOrigVar.numElem; i++){
				if(((float[])tempBPVar.dataValues)[i]<0)
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

				if(tempOrigVar.dt.equalsIgnoreCase("float"))
					super.minmaxCalculator(tempNewVar, ((float[])tempNewVar.dataValues));
				else if(tempOrigVar.dt.equalsIgnoreCase("int"))
					super.minmaxCalculator(tempNewVar, ((int[])tempNewVar.dataValues));

			}

			this.addVariableObject(tempNewVar);
			logger.info("Adding new variable - "+KamelName+" - in order to correct polarity of variable "+ origVarKamelName+".");

		}

	}


	public String toString(){
		return "Enlil Object";
	}

}