package gov.nasa.gsfc.ccmc.KameleonConverter;

import java.io.File;
import java.io.FileNotFoundException;
import java.io.IOException;
import java.util.ArrayList;
import java.util.Scanner;

import org.apache.log4j.Logger;

public class OPEN_GGCM extends Model {

	/**
	 * Options for logger output include .info(), .error(), and .debug(). 
	 */
	static Logger logger = Logger.getLogger(OPEN_GGCM.class);

	/**
	 * Constructor for Open GGCM Model Objects. 
	 * Calls the constructor for Model.java and parses the xml file open_ggcm.xml. 
	 */
	public OPEN_GGCM() {
		super("open_ggcm.xml");
	}

	/**
	 * @return The name of object as its model type ("OPEN_GGCM Object").
	 */
	public String toString(){
		return "OPEN_GGCM Object";
	}

	/**
	 * Holds the file, which is created via the individual file pathname taken from the input directory.
	 * Each model object will have a unique pathname from which its data come.  
	 */
	private File file;

	/**
	 * An ArrayList that stores sets of ASCII Text. 
	 * Each Variable has ASCII Text, from which the data values are gathered, associated with it. 
	 * We make this an ArrayList of StringBuilder instead of String so that we can easily add on to 
	 * each string of text without creating a new String each time. 
	 */
	private ArrayList<StringBuilder> asciiStorer = new ArrayList<StringBuilder>();

	/**
	 * A scanner used to read in from the file from the input directory. 
	 */
	private Scanner filereader;


	//this arraylist holds the many lines of ASCII that belong to one individual variable
	//when finished, this arraylist will be converted to a String[] and put into the setholder
	//and then reset to a new blank array
	ArrayList<String>lineholder= new ArrayList<String>();


	//number of elements within this arraylist = number of variables
	//one String[] for each arraylist
	ArrayList<String[]> setholder = new ArrayList<String[]>();



	/**
	 * Opens the associated file. 
	 * Creates the variables. 
	 * Stores the header information for each variable within the Variable object. 
	 * Stores the ASCII data associated with each variable in the ArrayList asciiStorer. 
	 * 
	 */
	public void read() throws FileNotFoundException, NoAttributeException, NoVariableException, IOException {
		super.read();

		logger.info("Now OPENING " + FilePathname);
		file= new File(FilePathname);
		filereader = new Scanner(file);



		String line = "";


		while(filereader.hasNextLine())
		{
			line = filereader.nextLine();
			if (line.startsWith("FIELD"))//signifies a new variable's arrival
			{	
				if(VarObjsSize()!=0){
					String [] linesArray = new String[lineholder.size()];

					setholder.add((String[])(lineholder.toArray(linesArray)));
					lineholder = new ArrayList<String>();
				}

				//getting the variable name
				String varName = filereader.next();

				Variable tempV;

				if(orig2kamel.get(varName)!=null)
					tempV=new Variable(varName,orig2kamel.get(varName)); 
				else
					tempV= new Variable(varName,"kameleon_identity_unknown"+VarObjsSize());


				logger.debug(tempV.originalName + " = " + tempV.KameleonName);



				//setting number of dimensions
				if(line.charAt(6)=='3'){
					tempV.numDim=3;
				}

				/*
				 * This can just be added on to the previous if-statement to 
				 * test for variables that may have dimensions other than 3
				 * 
				else if (line.charAt(6)=='2')
					tempV.numDim=2;
				else if (line.charAt(6)=='1')
					tempV.numDim=1;
				else if (line.charAt(6)=='4')
					tempV.numDim=4;
				else{
					logger.error("**ERROR**Invalid Number of Dimensions.\nEXITING...");
					System.exit(0);
				}
				 */


				/*
				 * this gets the start time value but it's already in the databaseinfo file...hmmm..why the discrepancy?
				 * 
				//getting the start time
				if(this.VarObjsSize()==1){
				line=n.next();
				line=n.next();
				line=n.next();
				line=n.next();
				this.getGlobalAttribute("start_time").value=line;
				}
				 */


				//skipping over some items and 
				//getting to the information that we need...
				filereader.nextLine();
				filereader.nextLine();
				filereader.next();

				//setting the dimension sizes, assuming that there are only 3
				int [] dimensionality = {filereader.nextInt(), filereader.nextInt(), filereader.nextInt()};
				tempV.dimSizes = dimensionality;
				//setting number of elements
				tempV.numElem=dimensionality[0]*dimensionality[1]*dimensionality[2];
				//setting the data type
				tempV.dt="float";


				//this reads in from Line 5 how the data is encoded - always "WRN2"
				filereader.next();

				//getting to the information that we need...
				//the number of elements in Line 5 is skipped because a) it is hard to read in  b) we have already calculated it by multiplying the different dimension numbers
				String minNum = filereader.next().trim();
				int start = new Integer(tempV.numElem).toString().length();

				//the next two numbers include the actual min and actual max values
				tempV.addAttribute(new KAttribute("actual_min", (minNum.substring(start,minNum.indexOf("E")+4)), "Smallest value in the data for a particular variable.","model", "float"));
				tempV.addAttribute(new KAttribute("actual_max", (filereader.nextFloat()), "Largest value in the data for a particular variable.","model", "float"));
				//the final values included deprecated and unnecessary information

				filereader.nextLine();

				this.addVariableObject(tempV);				

				//Adds the Attribute Values to the CCMC Standard Variables. These attributes and attribute values come from Variables.xml. 
				getCCMCstandardattrs(tempV);

				tempV.printAllAttrs();


			}else if(this.VarObjsSize()!=0){

				lineholder.add(line);
			}

		}
		//after all lines have been read from the file, the last set of lines has to be assigned to the last variable. 
		if(VarObjsSize()!=0){
			String [] linesArray = new String[lineholder.size()];	
			setholder.add((String[])(lineholder.toArray(linesArray)));
		}

		textParser();

	}

	/**
	 * Parses the ASCII text associated with each variable. 
	 * Assigns the data values to each variable. 
	 * @throws NoAttributeException 
	 */
	public void textParser() throws NoAttributeException{


		for(int i=0; i<VarObjsSize(); i++){

			Variable var= getVariableObject(i);



			float[] values = new float[var.numElem];

			//if actual max = actual min, then all values are the same
			if(var.getAttributeObject("actual_min").value.equals(var.getAttributeObject("actual_max").value)){
				for(int j=0; j<var.numElem; j++){
					values[j]=(Float)(var.getAttributeObject("actual_min").value);
				}
			}


			logger.debug("-- for "+var.KameleonName+", the setholder holds "+setholder.get(i).length + " sets of lines.");
			logger.debug("   its first set includes these lines to begin with:");
			logger.debug(setholder.get(i)[0]);
			logger.debug(setholder.get(i)[1]);
			logger.debug(setholder.get(i)[2]);
			logger.debug(setholder.get(i)[3]);


			for(int block64=0; block64<var.numElem; block64+=64){
				String buffer = setholder.get(i)[block64];
				int bytes = buffer.length();
				int[] msbits = new int[72];
				int numdecoded;
				int blocksize= Math.min(64,var.numElem-block64);
				float dzi;
				
				numdecoded=decode(buffer,msbits);
				if(numdecoded!=blocksize)
					logger.error("FAIL");

				buffer=setholder.get(i)[block64+1];
				bytes = buffer.length();
				int[] lsbits = new int[72];

				numdecoded=decode(buffer, lsbits);
				if(numdecoded!=blocksize)
					logger.error("FAIL");
				
				dzi=((Float)(var.getAttributeObject("actual_max").value) - (Float)(var.getAttributeObject("actual_min").value))/4410;
				
				for(int j=0;j<blocksize; j++){
					msbits[j]-=33;
					lsbits[j]-=33;
					
					float sign =(float) 1.0;
					
					if(msbits[j]>=47){
						sign=(float)-1.0;
						msbits[j]-=47;
					}
					
					int intvalue= (int) (lsbits[j] +94.0*msbits[j]);
					
					values[block64+j]= sign* (float)Math.exp((dzi*intvalue+(Float)(var.getAttributeObject("actual_min").value)));
				
				}	
			}
			var.dataValues=values;
		}
	}


	/**
	 * Goes from the ASCII step to the integers step.
	 * Takes the empty array of ints and fills it in with the appropriate values.
	 * Returns the number of values in the line. 
	 * 
	 * @param line One String line read in from a variable's ASCII text
	 * @param outarray A reference to an empty array of ints (either msbits[72] or lsbits[72])
	 * @return the number of values in the line
	 */
	public int decode(String line, int[] outarray){

		//for error checking we will make all of the values in the int[] equal to 34
		for(int i=0; i<64; i++)
			outarray[i]=34;

		int runvalue;
		int runcount;
		int inputchecksum = line.charAt(0);
		int checksum=0;
		int indexin=1;
		int indexout=0;

		while( (runvalue=(int)(line.charAt(indexin))) != 0){

			if(runvalue<0)
				runvalue+=256;

			if(runvalue>127){
				runcount=runvalue-170;
				runvalue= line.charAt(++indexin);
				if(runvalue<0){
					runvalue+=256;
				}
				for(int count =0; count<runcount; count++){
					if(indexout<67)
						outarray[indexout++]=runvalue;
					else
						checksum+=runvalue;
				}
			}
			else{
				if(indexout<67)
					outarray[indexout++]=runvalue;
				else
					checksum+=runvalue;
			}
			indexin++;	
			if(indexin>67)
				return indexout;
		}
		
		

		if(indexout>64)
			return 64;
		if(inputchecksum!=33 + checksum%92)
			return 0;

		return indexout;
	}



}


//look at RLED