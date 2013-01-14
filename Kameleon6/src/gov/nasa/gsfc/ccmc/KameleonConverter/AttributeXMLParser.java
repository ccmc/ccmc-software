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

//This parses GlobalCCMCAttributesNames.xml and VariableCCMCAttibuteNames.xml.
//It takes the name and description of each of the attributes and puts them in an Attribute. 
//Then, the parser can return an array of hashmap of those Attribute objects. 

public class AttributeXMLParser {
	
	static Logger logger = Logger.getLogger(AttributeXMLParser.class);

	/**
	 * An Array of CCMC Attributes
	 */
	KAttribute[] CCMCattrs=null;

	/**
	 * Key = name of the Attribute
	 * Value = the corresponding Attribute Object
	 */
	HashMap<String, KAttribute> name2attr = new HashMap<String, KAttribute>();

	AttributeXMLParser(String f){ 
		read(f);	
	}

/**
 * Reads and parses the XML File about the Attributes. 
 * Also creates an array of KAttribute objects and a HashMap mapping names to KAttribute objects. 
 * @param f The name of the XML document that stores the information about the CCMC Attributes. 
 */
	private void read(String f){

		try {

			File file = new File(f);
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
					fstNmElmntLst = fstElmnt.getElementsByTagName("name");
					fstNmElmnt = (Element) fstNmElmntLst.item(0);
					fstNm = fstNmElmnt.getChildNodes();
					
					name = ((Node) fstNm.item(0)).getNodeValue();

					//logger.debug("Attribute Name"  +num+":"+ name);

					fstNmElmntLst = fstElmnt.getElementsByTagName("description");
					fstNmElmnt = (Element) fstNmElmntLst.item(0);
					fstNm = fstNmElmnt.getChildNodes();

					try {
						description=((Node) fstNm.item(0)).getNodeValue();
					} catch (NullPointerException e) {
						description=null;
					}


					//logger.debug("Description" +num+":"+ description);

					if(f.indexOf("Global")!=-1){
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
					}else
						CCMCattrs[s]= new KAttribute(name,null,description, "ccmc" , null);

					name2attr.put(name, CCMCattrs[s]);

				}

			}


		} catch (Exception e) {
			e.printStackTrace();

		}

	}

	/**
	 * 
	 * @return  An array of CCMC Attributes. 
	 */
	KAttribute[] getAttributes(){
		return CCMCattrs;
	}

}
