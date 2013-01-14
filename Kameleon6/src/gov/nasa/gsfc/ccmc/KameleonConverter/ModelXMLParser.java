package gov.nasa.gsfc.ccmc.KameleonConverter;
import javax.xml.parsers.DocumentBuilderFactory;
import javax.xml.parsers.DocumentBuilder;

import org.apache.log4j.Logger;
import org.w3c.dom.Document;
import org.w3c.dom.NodeList;
import org.w3c.dom.Node;
import org.w3c.dom.Element;
import java.io.File;
import java.util.HashMap;

//This parses the [model].xml files and gets all of the hardcoded Global Attributes
public class ModelXMLParser {

	static Logger logger = Logger.getLogger(ModelXMLParser.class);

	private KAttribute[] attrs=null;

	private HashMap<String, String>namesMap= new HashMap<String, String>();

	ModelXMLParser(String f){
		read(f);
	}

	public KAttribute[] getModelSpecGlbAttrs(){
		return attrs;
	}
	public void read (String f) {

		try {

			File file = new File(f);
			DocumentBuilderFactory dbf = DocumentBuilderFactory.newInstance();
			DocumentBuilder db = dbf.newDocumentBuilder();
			Document doc = db.parse(file);
			doc.getDocumentElement().normalize();
			//if(CommandLineInterface.verboseFlag)logger.debug("---------------------------------\n"  + "Model Type: " + doc.getDocumentElement().getAttribute("name") + "\n---------------------------------");

			NodeList nodeAttrLst = doc.getElementsByTagName("attribute");


			attrs = new KAttribute[nodeAttrLst.getLength()];

			//if(CommandLineInterface.verboseFlag) System.out.println("\nGlobal Attributes (Model-Specific) and   DataTypes: \n");

			//this for-loop parses children with 4 types of children
			for (int s = 0; s < nodeAttrLst.getLength(); s++) {

				Node fstNode = nodeAttrLst.item(s);

				if (fstNode.getNodeType() == Node.ELEMENT_NODE) {

					Element fstElmnt = (Element) fstNode;
					//int num =s+1;

					NodeList aList = fstElmnt.getElementsByTagName("name");
					Element aElement = (Element) aList.item(0);
					NodeList aName = aElement.getChildNodes();
					String name= ((Node) aName.item(0)).getNodeValue();
					//if(CommandLineInterface.verboseFlag) logger.debug("Attribute Name"  +num+":"+ ((Node) aName.item(0)).getNodeValue());

					aList = fstElmnt.getElementsByTagName("dataType");
					aElement = (Element) aList.item(0);
					NodeList bName = aElement.getChildNodes();
					String datatype = ((Node) bName.item(0)).getNodeValue();
					//if(CommandLineInterface.verboseFlag) logger.debug("Data Type" +num+":"+ ((Node) bName.item(0)).getNodeValue());

					aList = fstElmnt.getElementsByTagName("description");
					aElement = (Element) aList.item(0);
					String description=null;
					if(aElement!=null){
						description=((Node) aElement.getChildNodes().item(0)).getNodeValue();
					}
					//if(CommandLineInterface.verboseFlag) logger.debug("Description" +num+":"+ ((Node) cName.item(0)).getNodeValue());

					aList = fstElmnt.getElementsByTagName("value");
					aElement = (Element) aList.item(0);
					Object value=null;
					if(aElement!=null){
						value= ((Node) aElement.getChildNodes().item(0)).getNodeValue();
						}
					//if(CommandLineInterface.verboseFlag) logger.debug("Value" +num+":"+ ((Node) dName.item(0)).getNodeValue());


					attrs[s]= new KAttribute(name, value, description, "model", datatype);

				}

			}


			//if(CommandLineInterface.verboseFlag) logger.debug("Attributes: " + attrs.length);



			NodeList nodeNamesList = doc.getElementsByTagName("variable");

			for(int s=0; s<nodeNamesList.getLength(); s++){
				Node node1 = nodeNamesList.item(s);

				if(node1.getNodeType() == Node.ELEMENT_NODE){

					Element ment1 = (Element) node1;

					NodeList aList = ment1.getElementsByTagName("OriginalName");
					Element aElement = (Element) aList.item(0);
					NodeList aName = aElement.getChildNodes();

					NodeList bList = ment1.getElementsByTagName("KameleonName");
					Element bElement = (Element) bList.item(0);
					NodeList bName = bElement.getChildNodes();

					//if both names exist...
					if(aName.getLength()!=0 && bName.getLength()!=0){
						//mapping the original name to the kameleon name
						namesMap.put(((Node) aName.item(0)).getNodeValue(), ((Node) bName.item(0)).getNodeValue());

						if(CommandLineInterface.verboseFlag)
							logger.debug("Mapping Original Name \""+ ((Node) aName.item(0)).getNodeValue() +"\" to Kameleon Name \"" + ((Node) bName.item(0)).getNodeValue() + "\"") ;
					}

				}

			}





		} catch (Exception e) {
			e.printStackTrace();

		}
	}

	public HashMap<String, String> getNamesMap() {
		return namesMap;
	}
}
