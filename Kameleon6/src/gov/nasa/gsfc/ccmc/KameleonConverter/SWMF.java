/*
 * 
 * Name: SWMF.java
 * 
 * Version: 6.0
 * 
 * Author: Nitesh Donti
 * 		   NASA-GSFC-CCMC (Code 587)
 * 		   Intern
 * 
 * Purpose: Custom read routine for the 
 * 			SWMF input model file. 
 *  
 * Modification History:
 *
 * January 2012 Donti, Nitesh
 * 				Fixed a couple of bugs that did not retain original variable attribute information from the input file
 *
 * Summer 2011 	Donti, Nitesh
 * 				Initial Development Started
 * 				All tasks complete
 * 				Ready to use
 *  			  
 * Tasks:	Need to put info into the README among
 * 			other Global Attributes. 	 
 * 
 */

package gov.nasa.gsfc.ccmc.KameleonConverter;

import java.io.File;
import java.io.FileNotFoundException;
import java.io.IOException;
import java.util.ArrayList;
import java.util.Collections;
import java.util.List;
import java.util.Scanner;
import java.util.StringTokenizer;

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
		String dateTokens[] = simulatedtime.split("-");
		String date = dateTokens[0] + "-" + dateTokens[1] + "-" + dateTokens[2];
		String time = dateTokens[3] + ":" + dateTokens[4] + ":" + dateTokens[5] + "." + dateTokens[6];
		this.addGlobalAttribute(new KAttribute("timestep_time", date+"T"+time+"Z", "Current time", "model", "String"));
		this.addGlobalAttribute(new KAttribute("B Tilt Degrees", tiltdegrees.substring(tiltdegrees.indexOf("=")+1), "Magnetic Field (B) Tilt Degrees", "model", "String"));

		String varsLine = eyes.next();
		String tempholder = eyes.next();
		while(tempholder.indexOf("ZONE")==-1){
			varsLine+=tempholder; 
			tempholder= eyes.next();
			logger.info("tempholder: " + tempholder);
		}
		//getting dimension info from the file using Scanner
		int[] dims = new int[2];
		while (!eyes.hasNextInt())
			eyes.next();
		// number refers to northern hemisphere only - add south and subtract equator which is duplicated
		//dims[0]=2*eyes.nextInt()-1;
		dims[0] = eyes.nextInt();
		while (!eyes.hasNextInt())
			eyes.next();
		dims[1]=eyes.nextInt();
		//end - getting dimension info

		String varName;
		String units;
		int variable = 1;


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
			this.getVariableObject(getNumVariables()-1).addAttribute(new KAttribute("units", units, "The units that the variable is in", "model", "string"));

			int sizeOfVariables = (dims[0]*2-1)*dims[1];
			this.getVariableObject(getNumVariables()-1).dataValues= new float[sizeOfVariables];
			//this.getVariableObject(getNumVariables()-1).getAttributeObject("grid_system");
			varsLine = varsLine.substring(varsLine.indexOf("\"")+1);

		}


		tempholder = eyes.nextLine(); //should be F=POINT
		//logger.info("next line: " + tempholder);


		//This adds the data to the each variable.
		//tempholder = eyes.next();
		List<SWMFIonoData> ionoData = new ArrayList<SWMFIonoData>();
		for (int i = 0; i < dims[0]*dims[1]; i++)
		{
			String nextLine = eyes.nextLine();
			StringTokenizer st = new StringTokenizer(nextLine);
			if (st.countTokens() != this.getNumVariables())
				nextLine += eyes.nextLine();
			st = new StringTokenizer(nextLine);
			st.countTokens();

			ionoData.add(new SWMFIonoData(nextLine));
			//((float[])(this.getVariableObject(variableNum).dataValues))[i]=nextFloat;
		}

		// read southern hemisphere
		tempholder = eyes.nextLine(); //should be ZONE line
		tempholder = eyes.nextLine();//should be I= ... F=POINT line
		logger.info("next line: " + tempholder);

		//		tempholder = eyes.next();
		//		while(tempholder.indexOf("F=")==-1)
		//			tempholder = eyes.next();
		for (int i = 0; i < (dims[0])*dims[1]; i++)
		{
			ionoData.add(new SWMFIonoData(eyes.nextLine()));
			//		((float[])(this.getVariableObject(variableNum).dataValues))[i+(dims[0]*dims[1]-1]=nextFloat;
			//				} catch (Exception e)
			//				{
			//					logger.info("i: " + i + " +dims: " + (i+dims[0]*dims[1]-1) + "dims*dims: " + dims[0]*dims[1]);
			//					System.exit(0);
			//				}
			//}	
		}

		IonoComparatorLatitudeFirstLongitudeSecond LatLonSorter = new IonoComparatorLatitudeFirstLongitudeSecond();
		Collections.sort(ionoData, LatLonSorter);

		//get the longitude values so we can store unique values. this will make it easier to search in the interpolator.
		float longitudeArray[] = new float[dims[1]];
		for (int i = 0; i < dims[1]; i++)
		{
			longitudeArray[i] = ionoData.get(i).getVariable(SWMFIonoData.PSI);
		}

		//remove duplicates
		List<SWMFIonoData> duplicatesRemoved = new ArrayList<SWMFIonoData>();
		duplicatesRemoved.add(ionoData.remove(0));
		while (ionoData.size() > 0)
		{
			SWMFIonoData current = ionoData.remove(0);
			if (LatLonSorter.compare(current, duplicatesRemoved.get(duplicatesRemoved.size()-1)) != 0)
				duplicatesRemoved.add(current);
			else
				System.out.println("duplicate removed");
		}

		//Sort the records longitude first, then latitude, but sweep from the south pole to the north pole
		IonoComparatorLongitudeFirstLatitudeSecond lonLatSorter = new IonoComparatorLongitudeFirstLatitudeSecond();
		Collections.sort(duplicatesRemoved, lonLatSorter);
		float latitudeArray[] = new float[dims[0]*2-1];
		for (int i = 0; i < dims[0]*2-1; i++)
		{
			latitudeArray[i] = 90.0f - duplicatesRemoved.get(i).getVariable(SWMFIonoData.THETA);
		}
		for (int i = 0; i < duplicatesRemoved.size(); i++)
		{

			SWMFIonoData currentDataLine = duplicatesRemoved.get(i);
			for (int j = 0; j < this.getNumVariables(); j++)
			{
				((float[])(this.getVariableObject(j).dataValues))[i] = currentDataLine.getVariable(j);
			}
		}

		this.getVariableObject(SWMFIonoData.PSI).dataValues = new float[dims[1]];
		for (int i = 0; i < longitudeArray.length; i++)
		{
			((float[])(this.getVariableObject(SWMFIonoData.PSI).dataValues))[i] = longitudeArray[i];
		}
		this.getVariableObject(SWMFIonoData.THETA).dataValues = new float[dims[0]*2-1];
		for (int i = 0; i < latitudeArray.length; i++)
		{
			((float[])(this.getVariableObject(SWMFIonoData.THETA).dataValues))[i] = latitudeArray[i];
		}

		//		for(int i=((dims[0]-1)/2)*dims[1]; eyes.hasNextFloat(); i++){
		//			((float[])(this.getVariableObject(i%this.getNumVariables()).dataValues))[(int)i/this.getNumVariables()]=eyes.nextFloat();
		//		}

		//---------------setting various fields for each variable---------------

		for(int i=0; i<this.getNumVariables(); i++){

			Variable tempV= this.getVariableObject(i);

			//----this is where we assign variable attributes (ccmc standard)----
			//Adds the Attribute Values to the CCMC Standard Variables. These attributes and attribute values come from Variables.xml.
			getAttrValues(tempV);

			//setting the data type
			tempV.dt= "float";
			//setting number of elements
			tempV.numElem = ((float[])(tempV.dataValues)).length;
			//setting number of dimensions
			tempV.numDim = 2;
			//setting the dimension Sizes field for the Enlil Variable Object
			tempV.dimSizes= new int[] {dims[0]*2-1, dims[1]};
			//setting min and mix values
		}

		for (int i = 0; i < this.getNumVariables(); i++)
		{
			if(!CommandLineInterface.nominmaxFlag){
				Variable var = this.getVariableObject(i);
				super.minmaxCalculator(var, ((float[])var.dataValues));	
			}	
		}
		//--------------------------end setting various fields--------------------
	}
	public String toString(){
		return "SWMF Object";
	}

}
