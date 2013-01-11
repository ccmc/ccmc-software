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

/**
 * This parses the file called "Variables.xml", which stores all of the variables for all of the model types, and the accompanying CCMC Attributes with their values. 
 * This parser can return, in an array, the variables that were just created. 
 * @author Nitesh
 *
 */
public class VariableXMLParser {
	
	Logger logger = Logger.getLogger(VariableXMLParser.class);



	Variable[] vars=null;
	AttributeXMLParser axmlp=null;

	VariableXMLParser(String f ){

		axmlp = new AttributeXMLParser("VariableCCMCAttributeNames.xml");
		read(f);
	}

	/**
	 * Key = Kameleon name of the Variable
	 * Value = the corresponding Variable Object
	 */
	HashMap<String, Variable> Kname2var = new HashMap<String, Variable>();

	private void read(String f){

		try {

			String[] tags ={"category","name","dataType","classification","valid_min","valid_max",
					"units","grid_system","mask","description","is_vector","position_grid_system",
			"data_grid_system"};

			File file = new File(f);
			DocumentBuilderFactory dbf = DocumentBuilderFactory.newInstance();
			DocumentBuilder db = dbf.newDocumentBuilder();
			Document doc = db.parse(file);
			doc.getDocumentElement().normalize();
			NodeList nodeVarLst = doc.getElementsByTagName("Variable");


			vars = new Variable[nodeVarLst.getLength()];
			Element fstElmnt;
			NodeList fstNmElmntLst;
			Element fstNmElmnt; 
			NodeList fstNm;
			String data=null;
			
			//if(CommandLineInterface.verboseFlag) logger.debug("\nVariables: \n");

			//loop executes once for each distinct variable in the xml file
			for (int s = 0; s < nodeVarLst.getLength(); s++) {


				KAttribute[] atts = new KAttribute[tags.length-4];
				Node fstNode = nodeVarLst.item(s);

				if (fstNode.getNodeType() == Node.ELEMENT_NODE) {

					fstElmnt = (Element) fstNode; 
					//int num =s+1;

					//this loops through all of the attributes of each distinct variable
					for (int i=0; i<tags.length; i++){
						fstNmElmntLst = fstElmnt.getElementsByTagName(tags[i]);
						fstNmElmnt = (Element) fstNmElmntLst.item(0);
						fstNm = fstNmElmnt.getChildNodes();
						data=((Node) fstNm.item(0)).getNodeValue();

						
						//logger.debug(tags[i] +num+":"+ data);


					
						if(data==null)
						{//no variable attribute is added because there is no such element for this variable in Variables.xml
						}
						else{
							switch(i){

							case 1: vars[s]= new Variable(null,data); 
									Kname2var.put(data, vars[s]);
									break;
							case 4: 
							case 5: atts[i-4]=new KAttribute(tags[i], data, axmlp.name2attr.get(tags[i]).description, "ccmc", "float"); break;
							case 6: 
							case 7: 
							case 8: 
							case 11:
							case 12:
							case 9: atts[i-4]=new KAttribute(tags[i], data, axmlp.name2attr.get(tags[i]).description, "ccmc","string"); break;
							case 10: 
								if(data.indexOf("t")!=-1)
									atts[i-4]=new KAttribute(tags[i], 1, axmlp.name2attr.get(tags[i]).description,"ccmc", "int");
								else
									atts[i-4]=new KAttribute(tags[i], 0, axmlp.name2attr.get(tags[i]).description,"ccmc", "int");	
								break;
							default: break;

							}

							//the  reason we start with 4 is because the first 3 tags are not set as variable attributes for the variable
							if(i>3)
								vars[s].nameToAttribute.put(atts[i-4].name, atts[i-4]);
						}




					}
					
					//logger.debug("");

					vars[s].cal=atts;

				}

			

			}
		} catch (Exception e) {
			e.printStackTrace();
		}


	}




	/**Returns an array of Variable Objects that were just created by the parser**/
	Variable[] getVariables(){
		return vars;
	}



}
