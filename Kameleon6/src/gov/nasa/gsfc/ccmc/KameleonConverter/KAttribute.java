package gov.nasa.gsfc.ccmc.KameleonConverter;
public class KAttribute {

	String name;
	String description; 
	DataType dataType;
	KAttributeType attType;
	Object value;

	/**Enumerator specifying the Data type of the Attribute*/
	public enum DataType {
		INT, FLOAT, DOUBLE, STRING, BOOLEAN
	}

	/**Enumerator specifying whether the Attribute is a CCMC one, which all model types will have, or one that is specific to only this type of model.*/
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

	public String toString(){
		return "\n[Attribute Name : " + name + 
		"\n Description    : " + description + 
		"\n DataType       : " + dataType + 
		"\n Value          : " + value + 
		"\n Attribute Type : " + attType + "]";
	}

	/**
	 * Sets the specific value of this Attribute object.
	 * 
	 * @param val The value of this attribute object, in any form (String, int, object, array, etc.)
	 */
	public void setValue(Object val){
		value=val;
	}


}
