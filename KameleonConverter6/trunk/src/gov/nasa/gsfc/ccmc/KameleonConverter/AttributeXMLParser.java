/*
 * 
 * Name: AttributeXMLParser.java
 * 
 * Version: 6.0
 * 
 * Author: Nitesh Donti
 * 		   NASA-GSFC-CCMC (Code 587)
 * 		   Intern
 * 
 * Purpose: This parses two XML files: GlobalCCMCAttributeNames.xml
 * 			and VariableCCMCAttributeNames.xml. It takes the name and
 * 			description of each of the attributes in the files and puts  
 * 			them in a KAttribute Object. Then, this parser class can 
 * 			return an array of CCMC KAttribute Objects. 
 * 			The AttributeXMLParser Object also has a Hashmap that can 
 * 			be accessed. The Hashmap maps the name of the KAttribute
 * 			to the actual KAttribute Object. 
 * 	
 *  
 * Modification History:
 *  
 * Summer 2011 	Donti, Nitesh
 * 				Initial Development Started
 * 				All tasks complete
 * 				Ready to use
 *  			  
 *  			 
 * 
 */


package gov.nasa.gsfc.ccmc.KameleonConverter;
import java.io.File;
import java.util.HashMap;

import javax.xml.parsers.DocumentBuilder;
import javax.xml.parsers.DocumentBuilderFactory;

import org.apache.log4j.Logger;
import org.w3c.dom.Document;
import org.w3c.dom.Element;
import org.w3c.dom.Node;
import org.w3c.dom.NodeList;

public class AttributeXMLParser {

	/**
	 * Logger for at AttributeXMLParser.java class. 
	 * Use Logger.info(), Logger.error(), and Logger.debug() to 
	 * print statements within this class. 
	 */
	static Logger logger = Logger.getLogger(AttributeXMLParser.class);

	/**
	 * An Array of CCMC Attributes that have names, descriptions, and types, but not Values. 
	 */
	private KAttribute[] CCMCattrs=null;

	/**
	 * Key = name of the KAttribute
	 * Value = the corresponding KAttribute Object
	 */
	HashMap<String, KAttribute> name2attr = new HashMap<String, KAttribute>();

	/**
	 * Constructor for this class calls the read method
	 * @param xmlName The name of the XML document that stores the information about 
	 * the CCMC Attributes typically GlobalCCMCAttributeNames.xml or VariableCCMCAttributeNames.xml . 
	 */
	AttributeXMLParser(String xmlName){ 
		read(xmlName);	
	}

	/**
	 * Reads and parses the XML File about the Attributes. 
	 * Also creates an array of KAttribute objects and a HashMap mapping names to KAttribute objects. 
	 * @param xmlName The name of the XML document that stores the information about the CCMC Attributes. 
	 */
	private void read(String xmlName){

		try {

			File file = new File(xmlName);
			DocumentBuilderFactory dbf = DocumentBuilderFactory.newInstance();
			DocumentBuilder db = dbf.newDocumentBuilder();
			Document doc = db.parse(file);
			doc.getDocumentElement().normalize();

			//logger.debug("-------------------------------------------\n"  + "CCMC Attribute Type: " + doc.getDocumentElement().getAttribute("name") + "\n-------------------------------------------");

			NodeList nodeAttrLst = doc.getElementsByTagName("attribute");

			CCMCattrs = new KAttribute[nodeAttrLst.getLength()];

			Element fstElmnt=null;
			NodeList fstNmElmntLst=null;
			Element fstNmElmnt=null;
			NodeList fstNm=null;

			String name=null;
			String description=null;
			String datatype=null;

			//if(CommandLineInterface.verboseFlag && f.indexOf("Global")!=-1)logger.debug("\nGlobal, Model-Specific Attributes  and   Descriptions: \n");
			//else if(CommandLineInterface.verboseFlag && f.indexOf("Variable")!=-1) logger.debug("\nVariable, CCMC Attributes  and   Descriptions: \n");

			//this for-loop parses children with 2 types of children
			for (int s = 0; s < nodeAttrLst.getLength(); s++) {

				Node fstNode = nodeAttrLst.item(s);


				if (fstNode.getNodeType() == Node.ELEMENT_NODE) {

					fstElmnt = (Element) fstNode;
					
					//reading in the name
					fstNmElmntLst = fstElmnt.getElementsByTagName("name");
					fstNmElmnt = (Element) fstNmElmntLst.item(0);
					fstNm = fstNmElmnt.getChildNodes();

					name = ((Node) fstNm.item(0)).getNodeValue();

					//logger.debug("Attribute Name"  +num+":"+ name);
					
					//reading in the description
					fstNmElmntLst = fstElmnt.getElementsByTagName("description");
					fstNmElmnt = (Element) fstNmElmntLst.item(0);
					fstNm = fstNmElmnt.getChildNodes();

					try {
						description=((Node) fstNm.item(0)).getNodeValue();
					} catch (NullPointerException e) {
						description=null;
					}


					//logger.debug("Description" +num+":"+ description);

					//reading in the datatype
					fstNmElmntLst = fstElmnt.getElementsByTagName("datatype");
					fstNmElmnt = (Element) fstNmElmntLst.item(0);

					try {
						fstNm = fstNmElmnt.getChildNodes();
						datatype=((Node) fstNm.item(0)).getNodeValue();
					} catch (NullPointerException e) {
						datatype=null;
					}

					//logger.debug("DataType" +num+":"+ datatype);
					CCMCattrs[s]= new KAttribute(name,null,description, "ccmc", datatype);

					//placing the Attribute's Name and the KAttribute object in the hashmap
					name2attr.put(name, CCMCattrs[s]);
				}
			}
		} catch (Exception e) {
			e.printStackTrace();
		}

	}

	/**
	 * 
	 * @return  An array of CCMC Attributes that have names, descriptions, and types, but not Values. 
	 */
	KAttribute[] getAttributes(){
		return CCMCattrs;
	}

}
