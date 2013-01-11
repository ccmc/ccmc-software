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

	protected VariableXMLParser varxmlparser = new VariableXMLParser("Variables.xml");


	/**
	 * Constructor for any model type. 
	 * Calls the ModelXMLParser. 
	 * @param xmlmodelfile The name of the xml file for the model, which should look like [modelname].xml (e.g. enlil.xml). 
	 */
	Model(String xmlmodelfile){
		ModelXMLParser modelparser = new ModelXMLParser(xmlmodelfile);

		for(int i=0; i< modelparser.getModelSpecGlbAttrs().length; i++){
			this.addGlobalAttribute(modelparser.getModelSpecGlbAttrs()[i]);
		}
		orig2kamel=modelparser.getNamesMap();
	}

	/**
	 * Adds to a CCMC standard variable the attribute values that have already been registered 
	 * in the XML document entitled "Variables.xml".
	 * @param var A variable that is in the XML document "Variables.xml".
	 */
	public void getCCMCstandardattrs(Variable var){

		var.cal=varxmlparser.Kname2var.get(var.KameleonName).cal;
		for(int p=0; p<var.cal.length; p++)
			var.nameToAttribute.put(var.cal[p].name, var.cal[p]);


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

	public int VarObjsSize(){
		return variableObjects.size();
	}

	public int GloAttrsSize(){
		return globalattributes.size();
	}

	public boolean doesVarExist(String KamelName){
		return !(nameToVariable.get(KamelName)==null);
	}

	///////////////////////////////////////////////////////////////////


	String FilePathname;
	String OutputDirectory;
	String DatabaseInfoFile;
	String AuxiliaryFile;


	public void setFilePathname(String path){
		FilePathname = path;
	}
	public void setOutputDirectory(String od){
		OutputDirectory=od;
	}
	public void setDatabaseInfoFile(String file){
		DatabaseInfoFile = file;
	}
	public void setAuxiliaryFile(String af){
		AuxiliaryFile = af;
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
	 */
	public void read() throws NoAttributeException, FileNotFoundException, NoVariableException, IOException {

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


	public void minmaxCalculator(Variable var, float[] f){
		float min_value = Float.MAX_VALUE;
		float max_value = Float.MIN_VALUE;
		for (int t = 0; t < f.length; t++){
			if (f[t] < min_value)
			{
				min_value =f[t];
			} else if (f[t] > max_value)
			{
				max_value = f[t];
			}
		}
		var.addAttribute(new KAttribute("actual_min", min_value, "Smallest value in the data for a particular variable.","model", "float"));
		var.addAttribute(new KAttribute("actual_max", max_value, "Largest value in the data for a particular variable.", "model", "float"));
	}


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
		var.addAttribute(new KAttribute("actual_min", min_value, "Smallest value in the data for a particular variable.", "model", "int"));
		var.addAttribute(new KAttribute("actual_max", max_value, "Largest value in the data for a particular variable.","model", "int"));
	}




}
