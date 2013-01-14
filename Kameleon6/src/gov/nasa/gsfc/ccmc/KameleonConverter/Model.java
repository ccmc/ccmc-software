/*
 * 
 * Name: Model.java
 * 
 * Version: 6.0
 * 
 * Author: Nitesh Donti
 * 		   NASA-GSFC-CCMC (Code 587)
 * 		   Intern
 * 
 * Purpose: The object that stores all of the 
 * 			Variables (Objects) and Global Attributes 
 * 			(KAttribute Objects) for each Model. 
 *  
 * Modification History:
 *  
 * Summer 2011 	Donti, Nitesh
 * 				Initial Development Started
 * 				All tasks complete
 * 				Ready to use
 * 
 * 
 */

package gov.nasa.gsfc.ccmc.KameleonConverter;
import java.io.FileNotFoundException;
import java.io.IOException;
import java.util.ArrayList;
import java.util.HashMap;

import org.apache.log4j.Logger;

public abstract class Model {

	static Logger logger = Logger.getLogger(Model.class);

	/** Key = KameleonName,Value = Variable Object */
	private HashMap <String, Variable> nameToVariable = new HashMap<String, Variable>();

	/** Key = AttributeName,Value = Attribute Object */
	private HashMap <String, KAttribute> nameToAttribute = new HashMap<String, KAttribute>();

	/** The timestep number (if there are multiple). The converter will convert one full timestep at a time.*/
	private String instanceNumber; 

	/** The collection of Variables for the Model.**/
	private ArrayList<Variable> variableObjects = new ArrayList<Variable>();

	/** The collection of Global Attributes for the Model.*/
	private ArrayList<KAttribute> globalattributes = new ArrayList<KAttribute>();

	/**
	 * Maps the Original Name of the variable from the input file to the Kameleon Name that the CCMC uses. 
	 */
	protected HashMap<String, String> orig2kamel;

	protected VariableXMLParser varxmlparser;


	/**
	 * Constructor for any model type. 
	 * Calls the ModelXMLParser. 
	 * @param xmlmodelfile The name of the xml file for the model, which should look like [modelname].xml (e.g. enlil.xml). 
	 */
	Model(String xmlmodelfile){
		ModelXMLParser modelparser = new ModelXMLParser(xmlmodelfile);
		varxmlparser = new VariableXMLParser("Variables.xml");

		for(int i=0; i< modelparser.getModelSpecGlbAttrs().length; i++){
			this.addGlobalAttribute(modelparser.getModelSpecGlbAttrs()[i]);
		}
		orig2kamel=modelparser.getNamesMap();
	}

	/**
	 * Adds to a CCMC standard variable the attribute values that have already been registered 
	 * in the XML document entitled "Variables.xml".
	 * Needs to make sure to not overwrite anything that has already been written. 
	 * @param var A variable that is in the XML document "Variables.xml".
	 * @throws NoAttributeException 
	 */
	public void getAttrValues(Variable var) throws NoAttributeException{


		try
		{
			var.cal=varxmlparser.Kname2var.get(var.KameleonName).cal;
		}
		catch (NullPointerException n){
			logger.info("**Problem with finding the CCMC attributes for this variable: "+var.KameleonName);
		}


		for(int p=0; p<var.cal.length-2; p++) {

			try{
				if(CommandLineInterface.verboseFlag)
					logger.info("variable attribute: " + var.cal[p].name + " attribute value: " + var.cal[p].value);
				if(var.getAttributeObject(var.cal[p].name).value==null){
					var.nameToAttribute.put(var.cal[p].name, var.cal[p]);
				}
			}
			catch(NullPointerException e){
				e.printStackTrace();
			}

		}
	}

	/**Adds a variable object to the model and to the model's hashmap.
	 * 
	 * @param v Variable Object to be added to this Model
	 */
	public void addVariableObject(Variable v){
		variableObjects.add(v);
		nameToVariable.put(v.KameleonName, v);
	}

	public Variable getVariableObject(int u){
		return variableObjects.get(u);
	}

	/**
	 * 
	 * @param s The Kameleon Name of the Variable. 
	 * @return
	 */
	public Variable getVariableObject(String KamelS) throws NoVariableException{
		if(nameToVariable.get(KamelS)==null)
			throw new NoVariableException();

		return nameToVariable.get(KamelS);
	}


	/**Adds a global attribute to the model's ArrayList and to the model's HashMap.
	 * 
	 * @param k   Global Attribute to be added to this Model
	 */
	public void addGlobalAttribute(KAttribute k){
		globalattributes.add(k);
		nameToAttribute.put(k.name, k);
	}

	/**
	 * Gets the Global Attribute from the ArrayList. 
	 * @param s  The index of the ArrayList from which to grab the Global Attribute. 
	 * @return The requested Global Attribute.
	 */

	public KAttribute getGlobalAttribute(int g) throws NoAttributeException{
		if(g>=globalattributes.size() || g<0)
			throw new NoAttributeException("No Global Attribute available at index requested.");

		return globalattributes.get(g);
	}

	/**
	 * Gets the Global Attribute from the HashMap. 
	 * @param s  The key to look for in the HashMap. 
	 * @return The requested Global Attribute.
	 */
	public KAttribute getGlobalAttribute(String s) throws NoAttributeException{
		if(nameToAttribute.get(s)==null)
			throw new NoAttributeException("No Global Attribute exists by that name.");
		return nameToAttribute.get(s);
	}

	public int getNumVariables(){
		return variableObjects.size();
	}

	public int getGlobAttrsSize(){
		return globalattributes.size();
	}

	public boolean doesVarExist(String KamelName){
		return !(nameToVariable.get(KamelName)==null);
	}

	///////////////////////////////////////////////////////////////////


	String FilePathname;
	String OutputDirectory;
	String DatabaseInfoFile;
	String[] AuxiliaryFiles;


	public void setFilePathname(String path){
		FilePathname = path;
	}
	public void setOutputDirectory(String od){
		OutputDirectory=od;
	}
	public void setDatabaseInfoFile(String file){
		DatabaseInfoFile = file;
	}
	public void setAuxiliaryFiles(String[] af){
		AuxiliaryFiles = af;
	}
	public void setTimestep(String t){
		instanceNumber=t;
	}

	public String getTimestep(){
		return instanceNumber;
	}

	/**
	 * Parses an XML file with the standard CCMC Global Attributes for all Models.
	 * Puts these Global Attributes in the Model and into its HashMap.
	 * @throws NoAttributeException 
	 * @throws FileNotFoundException 
	 * @throws IOException 
	 * @throws NoVariableException 
	 * @throws Exception 
	 */
	public void read() throws NoAttributeException, FileNotFoundException, NoVariableException, IOException{

		//Parses an XML file with the standard CCMC Global Attributes for each Model
		String f = "GlobalCCMCAttributeNames.xml";
		AttributeXMLParser a = new AttributeXMLParser(f);
		KAttribute[] cal = a.getAttributes();

		//Puts these global attributes in the Model and its HashMap
		for(int i=0; i<cal.length; i++){
			this.addGlobalAttribute(cal[i]);	
		}

		//This global CCMC attribute is standard for all models. It simply states the terms of usage of this software. 
		this.addGlobalAttribute(new KAttribute("terms_of_usage", "***For tracking purposes of our government sponsors, we ask that you notify the " +
				"CCMC \nwhenever you use CCMC results in scientific publications or presentations: ccmc@ccmc.gsfc.nasa.gov", "CCMC Disclaimer Text: " +
				"For tracking purposes for our government sponsors, we ask that you notify the CCMC whenever you use CCMC results in a scientific " +
				"publication or presentation.", "ccmc", "string"));

		if(DatabaseInfoFile!=null)
			new DatabaseInfoParser(DatabaseInfoFile, this);
		else{
			//gets the name of the directory in which all of the input files are
			String directoryPath= FilePathname.substring(0,FilePathname.lastIndexOf("/"));
			getGlobalAttribute("run_registration_number").value=directoryPath.substring(directoryPath.lastIndexOf("/")+1);
		}

	}


	/**
	 * Calculates the Actual Minimum Value and the Actual Maximum Value
	 * of the data array of the Variable Object that is passed in. 
	 * @param var A Variable Object
	 * @param f The float[] that holds the data of the Variable Object that is passed in. This float[] should be the Variable's dataValues Object, but cast to a float[].
	 * @throws NoAttributeException 
	 */
	public void minmaxCalculator(Variable var, float[] f){
		float min_value = Float.MAX_VALUE;
		float max_value = -Float.MAX_VALUE;
		for (int t = 0; t < f.length; t++){
			if (f[t] < min_value)
			{
				min_value =f[t];
			} else if (f[t] > max_value)
			{
				max_value = f[t];
			}
		}

		try
		{
			var.getAttributeObject("actual_min").value=min_value;
			var.getAttributeObject("actual_max").value=max_value;
			int min_location=-1;
			int max_location=-1;
			for (int u = 0; u<var.cal.length; u++){
				KAttribute k = var.cal[u];
				if (k.name.equals("actual_min")){
					min_location = u;
				}
				else if (k.name.equals("actual_max")){
					max_location = u;
				}
			}
			var.cal[min_location]=var.getAttributeObject("actual_min");
			var.cal[max_location]=var.getAttributeObject("actual_max");
		} catch (Exception e)
		{
			e.printStackTrace();
		}

	}

	/**
	 * Calculates the Actual Minimum Value and the Actual Maximum Value
	 * of the data array of the Variable Object that is passed in. 
	 * @param var A Variable Object
	 * @param f The int[] that holds the data of the Variable Object that is passed in. This int[] should be the Variable's dataValues Object, but cast to a float[].
	 * @throws NoAttributeException 
	 */
	public void minmaxCalculator(Variable var, int[] f){
		int min_value = Integer.MAX_VALUE;
		int max_value = Integer.MIN_VALUE;
		for (int t = 0; t < f.length; t++){
			if (f[t] <= min_value)
			{
				min_value =f[t];
			}
			if (f[t] >= max_value)
			{
				max_value = f[t];
			}
		}

		try
		{
			var.getAttributeObject("actual_min").value=min_value;
			logger.debug(var.getAttributeObject("actual_min").value);
			var.getAttributeObject("actual_max").value=max_value;
			logger.debug(var.getAttributeObject("actual_max").value);
		} catch (NoAttributeException e)
		{
			e.printStackTrace();
		}
	}




}
