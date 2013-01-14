package gov.nasa.gsfc.ccmc.KameleonConverter;
import gov.nasa.gsfc.ccmc.KameleonConverter.KAttribute.KAttributeType;

import java.util.ArrayList;
import java.util.HashMap;

import org.apache.log4j.Logger;

public class Variable{
	
	Logger logger = Logger.getLogger(Variable.class);
	
	public String originalName;
	public String KameleonName;

	public Object dataValues;

	/**
	 * The data type of the elements in the variable's array of values.
	 * Choices: "int" "double" "float" "string"
	 */
	public String dt;

	/**
	 * The number of dimensions
	 */
	public int numDim;

	/**
	 * The number of elements in the variable's array of values.
	 */
	public int numElem;
	
	/**
	 *A long array, with dimSizes.length == numDim. List the number of elements in each dimension. Example: {10, 20} 
	 */
	public int[] dimSizes;

	/**
	 * An array for Variable Attributes that are standard across all model. 
	 * Since these are all CCMC Attributes, we know which ones to expect and look for,
	 * and therefore know the size of the array (so no need for an ArrayList).
	 */
	KAttribute[] cal = null;

	/**
	 * An Array List for Variable Attributes that vary across different model types. 
	 * Since they are Model Specific Variable Attributes, we do not know how
	 * many to expect so an ArrayList is more ideal so that we can add on attributes 
	 * as we go along. 
	 */
	ArrayList<KAttribute> msaal = new ArrayList<KAttribute>();

	/**
	 * Key = Name of the Attribute
	 * Value = Attribute Object
	 */
	HashMap<String, KAttribute> nameToAttribute = new HashMap <String, KAttribute>();

	/**
	 * Constructor for Variable Object. 
	 * 
	 * Also, parses the XML file "VariableCCMCAttributeNames.xml" in order to 
	 * assign the Variable Object its 9 CCMC Standard Variable Attributes. 
	 * 
	 * Puts those Variable Attributes in the array and in the hashmap.
	 * 
	 * Also, adds a Model Specific Variable Attribute that states what the original name of the variable is. 
	 * 
	 * @param orig    The variables original name. 
	 * @param kamel	  The new Kameleon standard name for the variable. 
	 */
	Variable(String orig, String kamel) {

		originalName = orig; 
		KameleonName = kamel;
		addAttribute(new KAttribute("Original Name", originalName, "The original name of the variable, as stated in the Input File" , "model", "String"));
		

		AttributeXMLParser attrxmlparser = new AttributeXMLParser("VariableCCMCAttributeNames.xml");
		cal=attrxmlparser.getAttributes();

		for(int i=0; i<cal.length; i++)
			nameToAttribute.put(cal[i].name, cal[i]);
	}

	/** 
	 * @return the Attribute that has the name of the String that is passed in 
	 * **/
	public KAttribute getAttributeObject (String attributeName) throws NoAttributeException{

		if(nameToAttribute.get(attributeName)==null)
			throw new NoAttributeException();
		KAttribute b = nameToAttribute.get(attributeName);

		return b;
	}

	/**
	 * Adds a ModelSpecific Variable Attribute to this Variable Object. 
	 * Do not use for a CCMC Variable Attribute. 
	 * 
	 * @param k ModelSpecificAttribute (only) to be added to the Variable Object's collection of attributes. 
	 */
	public void addAttribute(KAttribute k){
		if(k.attType==KAttributeType.MODELSPEC){
			nameToAttribute.put(k.name, k);
			msaal.add(k);
		}
	}

	/** 
	 * @return a String array of names of this variable object's CCMC Variable Attributes
	 */
	public String[] getCCMCAttributeNames(){
		String[] x = new String[cal.length];
		for (int i=0; i<cal.length; i++)
			x[i]=cal[i].name;

		return x;}

	/** 
	 * @return a String array of names of this variable object's current list of ModelSpecific Variable Attributes
	 */
	public String[] getModelSpecAttributeNames(){
		String[] x = new String[msaal.size()];
		for (int i=0; i<msaal.size(); i++)
			x[i]=msaal.get(i).name;

		return x;}

	/**
	 * @return a String array of name of this variable object's current list of CCMC Variable Attributes and ModelSpecific Variable Attributes
	 */
	public String [] getAllAttributeNames(){
		String[] x = new String[cal.length + msaal.size()];
		for (int i=0; i<cal.length; i++)
			x[i]=cal[i].name;
		for (int i=cal.length; i<cal.length+msaal.size(); i++)
			x[i]=msaal.get(i-cal.length).name;
		return x;
	}

	/**
	 * @return this Variable Object's original name (from the input file) and Kameleon name (standard to CDF files)
	 */
	public String toString(){
		return 
		"\n[Original Name : " + originalName + 
		"\n Kameleon Name : " + KameleonName;

	}
	
	/**
	 * Prints out a String representation of the Variable's Attribute names and their corresponding values
	 * @throws NoAttributeException
	 */
	public void printAllAttrs() throws NoAttributeException{
		
		StringBuffer line = new StringBuffer().append("[\n");
		for(int i = 0; i<getAllAttributeNames().length; i++){
			line.append(getAttributeObject(getAllAttributeNames()[i]).name + " = " + getAttributeObject(getAllAttributeNames()[i]).value+"\n");
		}
		line.append("]");
		logger.debug(line.toString());
	}

	
}
