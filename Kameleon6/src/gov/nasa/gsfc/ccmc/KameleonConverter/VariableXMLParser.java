/*
 * 
 * Name: VariableXMLParser.java
 * 
 * Version: 6.0
 * 
 * Author: Nitesh Donti
 * 		   NASA-GSFC-CCMC (Code 587)
 * 		   Intern
 * 
 * Purpose: This parses the file called "Variables.xml", which 
 * 			stores all of the variables for all of the model types, 
 * 			and the accompanying CCMC Attributes with their values. 
 * 			This parser can return, in an array, the variables that were just created. 
 * 	
 *  
 * Modification History:
 *  
 * Summer 2011 	Donti, Nitesh
 * 				Initial Development Started
 * 				All tasks complete
 * 				Ready to use
 *  			  
 * Tasks: 	Update and add on new variables to Variables.xml as more models are added. 
 *  			 
 * Winter 2012: Lots of bugs appearing with "actual min" and "actual max".
 *  			Added some try-catches to hopefully remedy issue
 *  			but not sure if it's the right move.
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
					"data_grid_system","actual_min","actual_max"};

			File file = new File(f);
			DocumentBuilderFactory dbf = DocumentBuilderFactory.newInstance();
			dbf.setIgnoringElementContentWhitespace(true);
			DocumentBuilder db = dbf.newDocumentBuilder();
			Document doc = db.parse(file);
			doc.getDocumentElement().normalize();
			doc.getDocumentElement().getNodeName();
			NodeList nodeVarLst = doc.getElementsByTagName("Variable");

			//Node node = nodeVarLst.item(0);
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

						//logger.debug(tags[i]);
						fstNmElmntLst = fstElmnt.getElementsByTagName(tags[i]);
						fstNmElmnt = (Element) fstNmElmntLst.item(0);
						axmlp = new AttributeXMLParser("VariableCCMCAttributeNames.xml");
						try{ //if there is an attribute with the name of the tag
							fstNm = fstNmElmnt.getChildNodes();
							data=((Node) fstNm.item(0)).getNodeValue();
						}
						catch(NullPointerException e){ //if there is not an attribute with the name of the tag 
							//mainly for the "actual_min" and "actual_max" attributes
							atts[i-4]=axmlp.name2attr.get(tags[i]); 	
						}
						//logger.debug(tags[i] +num+":"+ data);

						//name
						if(i==1){
							vars[s]= new Variable(null,data); 
							Kname2var.put(data, vars[s]);
						} else if (i==10){
							if(data.indexOf("t")!=-1)
							{//if the value for is_vector == true
								axmlp.name2attr.get(tags[i]).value = (1);
								atts[i-4]=axmlp.name2attr.get(tags[i]);
							}
							else
							{//if the value for is_vector == false
								axmlp.name2attr.get(tags[i]).value = (0);
								atts[i-4]=axmlp.name2attr.get(tags[i]);
							}
						}else if (i>3)
						{
							try{
								axmlp.name2attr.get(tags[i]).value = (data);
								atts[i-4]=axmlp.name2attr.get(tags[i]);
							}
							catch (NullPointerException e){
								if(CommandLineInterface.verboseFlag)
									logger.debug("**This isn't here: "+tags[i]);
							}
						}
						//the  reason we start with 4 is because the first 3 tags are not set as variable attributes for the variable
						if(i>3){
							try{
								vars[s].nameToAttribute.put(atts[i-4].name, atts[i-4]);
							}
							catch (NullPointerException e){
								vars[s].nameToAttribute.put("unknown_name", atts[i-4]);
							}
						}
					}
					vars[s].cal=atts;
				}
			}
		} catch (Exception e) {
			e.printStackTrace();
		}
	}
	
	/**@return An array of complete Variable Objects that were just created by the parser**/
	public Variable[] getVariables(){
		return vars;
	}

}
