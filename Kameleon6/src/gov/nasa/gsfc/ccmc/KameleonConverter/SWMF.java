package gov.nasa.gsfc.ccmc.KameleonConverter;

import java.io.File;
import java.io.FileNotFoundException;
import java.io.IOException;
import java.util.Scanner;
import org.apache.log4j.Logger;

public class SWMF extends Model {

	static Logger logger = Logger.getLogger(SWMF.class);


	public SWMF() {
		super("swmf.xml");
	}
	public void read() throws NoAttributeException, NoVariableException, IOException{
		super.read();
		parseFile();
		logger.info("\n\n\nThe information for your new java "+ this +" has just been read from the original file!");

	}
	public void parseFile() throws FileNotFoundException, NoAttributeException{

		logger.info("now looking within "+FilePathname);

		Scanner eyes = new Scanner (new File(FilePathname));
		
		String firstLine= eyes.nextLine();
		String title = (firstLine.split(","))[0].trim();
		String simulatedtime = (firstLine.split(","))[1].trim();
		String tiltdegrees= (firstLine.split(","))[2].trim();
		this.addGlobalAttribute(new KAttribute("Title", title.substring(title.indexOf("\"")+1), "Title", "model", "String"));
		this.getGlobalAttribute("start_time").value=simulatedtime;
		this.addGlobalAttribute(new KAttribute("B Tilt Degrees", tiltdegrees.substring(tiltdegrees.indexOf("=")+1), "Magnetic Field (B) Tilt Degrees", "model", "String"));
		
		String varsLine = eyes.next();
		String tempholder = eyes.next();
		while(tempholder.indexOf("ZONE")==-1){
			varsLine+=tempholder; 
			tempholder= eyes.next();
		}
		
		int[] dims = new int[2];
		while (!eyes.hasNextInt())
			eyes.next();
		dims[0]=eyes.nextInt();
		while (!eyes.hasNextInt())
			eyes.next();
		dims[1]=eyes.nextInt();
		
		
		String varName;
		String units;
		int variable = 0;


		//This gets the Variable Names and the units by parsing the file. 
		//Adding Variable Objects
		while(varsLine.indexOf("\"")!=-1){
			varsLine = varsLine.substring(varsLine.indexOf("\"")+1);
			varName = varsLine.substring(0,varsLine.indexOf("["));
			varsLine = varsLine.substring(varsLine.indexOf("[")+1);

			//if we know what to map the original name to, we will
			//if not, we will give a generic kameleon name
			if(orig2kamel.get(varName)!=null)
				this.addVariableObject(new Variable(varName,orig2kamel.get(varName)));
			else{
				this.addVariableObject(new Variable(varName,"kameleon_identity_unknown"+variable++));
				}
			
			units = varsLine.substring(0, varsLine.indexOf("]"));
			this.getVariableObject(VarObjsSize()-1).addAttribute(new KAttribute("units", units, "The units that the varaible is in", "model", "String"));
			this.getVariableObject(VarObjsSize()-1).dataValues= new float[dims[0]*dims[1]];
			varsLine = varsLine.substring(varsLine.indexOf("\"")+1);
		}


		//This adds the data to the each variable.
		tempholder = eyes.next();
		while(tempholder.indexOf("F=")==-1)
			tempholder = eyes.next();
		for(int i=0; eyes.hasNextFloat(); i++){
			((float[])(this.getVariableObject(i%this.VarObjsSize()).dataValues))[(int)i/20]=eyes.nextFloat();
		}




		//---------------setting various fields for each attribute---------------

		for(int i=0; i<this.VarObjsSize(); i++){

			Variable tempV= this.getVariableObject(i);

			//setting the data type
			tempV.dt= "float";
			//setting number of elements
			tempV.numElem = ((float[])(tempV.dataValues)).length;
			//setting number of dimensions
			tempV.numDim = 2;
			//setting the dimension Sizes field for the Enlil Variable Object
			tempV.dimSizes= new int[] {dims[0], dims[1]};
			//setting min and mix values
			if(!CommandLineInterface.nominmaxFlag){
				super.minmaxCalculator(tempV, ((float[])tempV.dataValues));			
			}


			//----this is where we assign variable attributes (ccmc standard)----
			//Adds the Attribute Values to the CCMC Standard Variables. These attributes and attribute values come from Variables.xml. 
			getCCMCstandardattrs(tempV);
			
			
		}
		//--------------------------end setting various fields--------------------




	}
	public String toString(){
		return "SWMF Object";
	}

}
