/*
 * 
 * Name: HDF5Writer.java
 * 
 * Version: 6.0
 * 
 * Author: Nitesh Donti
 * 		   NASA-GSFC-CCMC (Code 587)
 * 		   Intern
 * 
 * Purpose: A class for Attribute information.
 * 			This includes both Global attributes 
 * 			and Variable attributes.  
 * 			Name, Value, Description, Data type, and Attribute type (CCMC vs Model Specific)
 * 			are all included in each KAttribute Object. 
 * 
 * Modification History:
 *  
 * Summer 2011 	Donti, Nitesh
 * 				Initial development started
 * 				Removed Subclasses of KAttribute 
 * 				Added enums for KAttribute type
 * 				All tasks complete
 * 
 * Tasks: 	
 * 
 */

package gov.nasa.gsfc.ccmc.KameleonConverter;
public class KAttribute {

	/**
	 * Name of the Attribute
	 */
	String name;
	
	/**
	 * What the attribute means or represents
	 */
	String description; 
	
	/**
	 * Specifying the Data type of the Attribute
	 */
	private DataType dataType;
	
	/**
	 * Specifying whether the Attribute is a CCMC one, which all model types will have, or one that is specific to only this type of model or this exact model.
	 */
	private KAttributeType attType;
	
	/**
	 * The value of the attribute
	 */
	Object value;

	/**Enumerator specifying the Data type of the Attribute*/
	public enum DataType {
		INT, FLOAT, DOUBLE, STRING, BOOLEAN
	}

	/**Enumerator specifying whether the Attribute is a CCMC one, which all model types will have, or one that is specific to only this type of model or this exact model.*/
	public enum KAttributeType{
		CCMC, MODELSPEC
	}


	/**
	 * Constructor with the name, value, and description
	 * Put "null" for value and description if you only have the name and AttributeType
	 * 
	 * @param  kAttrType  A String - either "model" or "ccmc" to define what kind of variable this is. 
	 * @param data_type   A String denoting what data type the value of the attribute is ("string","int", etc.)
	 * @param description A Description of the Attribute
	 * @param name The name of the Attribute. 
	 * @param value The value of the Attribute. 
	 * 
	 **/
	KAttribute(String name, Object value, String description, String kAttrType, String data_type){
		this.name = name;  
		this.value = value;
		this.description = description;
		
		
		if(kAttrType.toLowerCase().indexOf("ccmc")!=-1)
			this.attType = KAttributeType.CCMC;
		else if (kAttrType.toLowerCase().indexOf("model")!=-1)
			this.attType=KAttributeType.MODELSPEC;

		if(data_type!=null){
			if(data_type.toLowerCase().indexOf("boo")!=-1)
				dataType=DataType.BOOLEAN;
			else if(data_type.toLowerCase().indexOf("dou")!=-1)
				dataType=DataType.DOUBLE;
			else if(data_type.toLowerCase().indexOf("str")!=-1)
				dataType=DataType.STRING;
			else if(data_type.toLowerCase().indexOf("int")!=-1)
				dataType=DataType.INT;
			else if(data_type.toLowerCase().indexOf("flo")!=-1)
				dataType=DataType.FLOAT;
		}
	}

	/**
	 * Returns all five components of an Attribute in text form. 
	 * - Name, Description, DataType, Value, Kameleon Attribute Type
	 */
	public String toString(){
		return "\n[Attribute Name : " + name + 
		"\n Description    : " + description + 
		"\n DataType       : " + dataType + 
		"\n Value          : " + value + 
		"\n Attribute Type : " + attType + "]";
	}

	

	/**
	 * Sets the data type for this Attribute Object. 
	 * @param data_type A string stating what type of data the value of this
	 * Attribute is (e.g. String, boolean, float, etc.)
	 */
	public void setDataType(String data_type){
		if(data_type.toLowerCase().indexOf("boo")!=-1)
			dataType=DataType.BOOLEAN;
		else if(data_type.toLowerCase().indexOf("dou")!=-1)
			dataType=DataType.DOUBLE;
		else if(data_type.toLowerCase().indexOf("str")!=-1)
			dataType=DataType.STRING;
		else if(data_type.toLowerCase().indexOf("int")!=-1)
			dataType=DataType.INT;
		else if(data_type.toLowerCase().indexOf("flo")!=-1)
			dataType=DataType.FLOAT;
	}

	/**
	 * 
	 * @return An enum stating the data type (String, int, float, etc.) of the
	 * value of this Attribute
	 */
	public DataType getDataType(){
		return dataType;
	}
	
	/**
	 * 
	 * @return An enum stating whether the Attribute is one that is standard (CCMC) or 
	 * if it is Specific to each unique model
	 */
	public KAttributeType getKType(){
		return attType;
	}
	
}
