/*
 * 
 * Name: OPEN_GGCM.java
 * 
 * Version: 6.0
 * 
 * Author: Nitesh Donti
 * 		   NASA-GSFC-CCMC (Code 587)
 * 		   Intern
 * 
 * Purpose: Custom read routine for the Open 
 * 			GGCM input model. 
 * 			This parses the header information 
 * 			and ASCII text from the input file. 
 *  
 * Modification History:
 *  
 * Summer 2011	Donti, Nitesh
 *  			Initial Development Started
 *  			Most information retrieved from files 
 *  			Problems occurring with reading ASCII (data values) properly			  
 *  
 * Tasks: 		Read in the data values properly and we should be good to go
 * 
 */


package gov.nasa.gsfc.ccmc.KameleonConverter;

import java.io.File;
import java.io.FileNotFoundException;
import java.io.IOException;
import java.io.UnsupportedEncodingException;
import java.util.ArrayList;
import java.util.Scanner;

import org.apache.log4j.Logger;

public class OPEN_GGCM extends Model
{

	/**
	 * Options for logger output include .info(), .error(), and .debug().
	 */
	static Logger logger = Logger.getLogger(OPEN_GGCM.class);

	/**
	 * Constructor for Open GGCM Model Objects. Calls the constructor for
	 * Model.java and parses the xml file open_ggcm.xml.
	 */
	public OPEN_GGCM()
	{
		super("open_ggcm.xml");
	}

	/**
	 * @return The name of object as its model type ("OPEN_GGCM Object").
	 */
	public String toString()
	{
		return "OPEN_GGCM Object";
	}

	/**
	 * Holds the file, which is created via the individual file pathname taken
	 * from the input directory. Each model object will have a unique pathname
	 * from which its data come.
	 */
	private File file;

	/**
	 * A scanner used to read in from the file from the input directory.
	 */
	private Scanner filereader;

	// this arraylist holds the many lines of ASCII that belong to one
	// individual variable
	// when finished, this arraylist will be converted to a String[] and put
	// into the setholder
	// and then reset to a new blank array
	ArrayList<String> lineholder = new ArrayList<String>();

	// number of elements within this arraylist = number of variables
	// one String[] for each arraylist
	ArrayList<String[]> setholder = new ArrayList<String[]>();

	/**
	 * Opens the associated file. Creates the variables. Stores the header
	 * information for each variable within the Variable object. Stores the
	 * ASCII data associated with each variable in the ArrayList asciiStorer.
	 * 
	 */
	public void read() throws FileNotFoundException, NoAttributeException, NoVariableException, IOException
	{
		super.read();

		logger.info("Now OPENING " + FilePathname);
		file = new File(FilePathname);
		filereader = new Scanner(file);

		String line = "";
		StringBuffer charCollection = new StringBuffer();

		Variable tempV = null;

		// the second part of this condition just allows us to do one variable
		// in order to save time
		while (filereader.hasNextLine() && lineholder.size() != 98438000)
		{
			line = filereader.nextLine();
			if (line.startsWith("FIELD"))// signifies a new variable's arrival
			{
				if (getNumVariables() != 0)
				{
					String[] linesArray = new String[lineholder.size()];

					setholder.add((String[]) (lineholder.toArray(linesArray)));
					lineholder = new ArrayList<String>();
				}

				/*if (VarObjsSize() == 1)
				{
					method(charCollection, tempV);

					System.exit(2);
				}*/

				// getting the variable name
				String varName = filereader.next();

				if (orig2kamel.get(varName) != null)
					tempV = new Variable(varName, orig2kamel.get(varName));
				else
					tempV = new Variable(varName, "kameleon_identity_unknown" + getNumVariables());

				logger.debug(tempV.originalName + " = " + tempV.KameleonName);

				// setting number of dimensions
				if (line.charAt(6) == '3')
				{
					tempV.numDim = 3;
				}

				/*
				 * This can just be added on to the previous if-statement to
				 * test for variables that may have dimensions other than 3
				 * 
				 * else if (line.charAt(6)=='2') tempV.numDim=2; else if
				 * (line.charAt(6)=='1') tempV.numDim=1; else if
				 * (line.charAt(6)=='4') tempV.numDim=4; else{
				 * logger.error("**ERROR**Invalid Number of Dimensions.\nEXITING..."
				 * ); System.exit(0); }
				 */

				/*
				 * this gets the start time value but it's already in the
				 * databaseinfo file...but there is a discrepancy
				 * 
				 * //getting the start time if(this.VarObjsSize()==1){
				 * line=n.next(); line=n.next(); line=n.next(); line=n.next();
				 * this.getGlobalAttribute("start_time").value=line; }
				 */

				// skipping over some items and
				// getting to the information that we need...
				filereader.nextLine();
				filereader.nextLine();
				filereader.next();

				// setting the dimension sizes, assuming that there are only 3
				int[] dimensionality = { filereader.nextInt(), filereader.nextInt(), filereader.nextInt() };
				tempV.dimSizes = dimensionality;
				// setting number of elements
				tempV.numElem = dimensionality[0] * dimensionality[1] * dimensionality[2];
				// setting the data type
				tempV.dt = "float";

				// this reads in from Line 5 how the data is encoded - always
				// "WRN2"
				filereader.next();

				// getting to the information that we need...
				// the number of elements in Line 5 is skipped because a) it is
				// hard to read in b) we have already calculated it by
				// multiplying the different dimension numbers
				String minNum = filereader.next().trim();
				int start = new Integer(tempV.numElem).toString().length();

				String minString = (minNum.substring(start, minNum.indexOf("E") + 4));
				Scanner reader = new Scanner(minString);

				// the next two numbers (reader.nextFloat()) include the actual min and actual max values
				
				try
				{
					tempV.getAttributeObject("actual_min").value=reader.nextFloat();
					tempV.getAttributeObject("actual_max").value=reader.nextFloat();
				} catch (NoAttributeException e)
				{
					e.printStackTrace();
				}
				
				// the final values from filereader include deprecated and unnecessary information
				filereader.nextLine();

				this.addVariableObject(tempV);

				// Adds the Attribute Values to the CCMC Standard Variables.
				// These attributes and attribute values come from
				// Variables.xml.
				getAttrValues(tempV);

				tempV.printAllAttrs();

			} else if (this.getNumVariables() != 0)
			{

				lineholder.add(line);

				if (getNumVariables() == 1)
				{

					if (line.indexOf("\n") != -1)
						line = line.substring(0, line.indexOf("\n"));
					charCollection.append(line);
				}

			}

		}
		// after all lines have been read from the file, the last set of lines
		// has to be assigned to the last variable.
		if (getNumVariables() != 0)
		{
			String[] linesArray = new String[lineholder.size()];
			setholder.add((String[]) (lineholder.toArray(linesArray)));
		}

		textParser();

		gridParser();

	}

	/**
	 * This is just the method that I am going to use with a different variation in 
	 * how we read in the file. It combines all of the characters into one 
	 * really large String. 
	 * 
	 * @param line
	 * @param var
	 * @throws NoAttributeException
	 */
	/*
	private void method(StringBuffer line, Variable var) throws NoAttributeException
	{

		float[] values = new float[var.numElem];

		int numMSfails = 0;
		int numLSfails = 0;

		int start = 0;
		int next = 64;

		// this parses through each Line of ASCII text
		for (int block64 = 0; block64 < var.numElem; block64 += 64)
		{
			int[] msbits = new int[72];
			int numdecoded;
			int blocksize = Math.min(64, var.numElem - block64);
			float dzi;

			String buffer = line.substring(start, next);
			start += 64;
			next += 64;
			// int bytes = buffer.length();

			numdecoded = decode(buffer, msbits);
			if (numdecoded != blocksize)
			{// if(CommandLineInterface.verboseFlag)
				logger.error("FAIL msbits " + numdecoded + " " + blocksize);
				numMSfails++;
			}

			buffer = line.substring(start, next);
			start += 64;
			next += 64;
			// bytes = buffer.length();
			int[] lsbits = new int[72];

			numdecoded = decode(buffer, lsbits);
			if (numdecoded != blocksize)
			{
				// if(CommandLineInterface.verboseFlag)
				logger.error("FAIL lsbits " + numdecoded + " " + blocksize);
				numLSfails++;
			}

			dzi = ((Float) (var.getAttributeObject("actual_max").value) - (Float) (var.getAttributeObject("actual_min").value)) / 4410;

			for (int j = 0; j < blocksize; j++)
			{
				msbits[j] -= 33;
				lsbits[j] -= 33;

				float sign = 1.0f;

				if (msbits[j] >= 47)
				{
					sign = -1.0f;
					msbits[j] -= 47;
				}

				int intvalue = (int) (lsbits[j] + 94.0 * msbits[j]);

				values[block64 + j] = sign * (float) Math.exp((dzi * intvalue + (Float) (var.getAttributeObject("actual_min").value)));

			}
		}
		if (numMSfails > 0 || numLSfails > 0)
		{
			logger.error("FAIL ls bits:" + numLSfails + " occurrences for " + var.KameleonName);
			logger.error("FAIL ms bits:" + numMSfails + " occurrences for " + var.KameleonName);
		}

		var.dataValues = values;
	}
	*/
	
	/**
	 * This parses the auxiliary files for Open GGCM, which are 3 text files
	 * entitled "gridx.txt", "gridy.txt", and "gridz.txt". 
	 * 
	 * It then creates 12 variables that work with the grid. 
	 * 
	 * @throws NoVariableException
	 * @throws FileNotFoundException
	 * @throws NoAttributeException 
	 */
	private void gridParser() throws NoVariableException, FileNotFoundException, NoAttributeException
	{

		Variable x = (new Variable("x", "x"));
		this.addVariableObject(x);
		Variable y = (new Variable("y", "y"));
		this.addVariableObject(y);
		Variable z = (new Variable("z", "z"));
		this.addVariableObject(z);

		Variable xbx = (new Variable("gridx_bx", "x_bx"));
		this.addVariableObject(xbx);
		Variable ybx = (new Variable("gridy_bx", "y_bx"));
		this.addVariableObject(ybx);
		Variable zbx = (new Variable("gridz_bx", "z_bx"));
		this.addVariableObject(zbx);

		Variable xby = (new Variable("gridx_by", "x_by"));
		this.addVariableObject(xby);
		Variable yby = (new Variable("gridy_by", "y_by"));
		this.addVariableObject(yby);
		Variable zby = (new Variable("gridz_by", "z_by"));
		this.addVariableObject(zby);

		Variable xbz = (new Variable("gridx_bz", "x_bz"));
		this.addVariableObject(xbz);
		Variable ybz = (new Variable("gridy_bz", "y_bz"));
		this.addVariableObject(ybz);
		Variable zbz = (new Variable("gridz_bz", "z_bz"));
		this.addVariableObject(zbz);

		Variable a;
		Variable b;
		Variable c;

		// loops through the 12 most recently added variables
		for (int u = this.getNumVariables() - 1; u > this.getNumVariables() - 13; u--)
		{
			a = this.getVariableObject(u);
			a.dataValues = new float[100];
			a.dt = "float";
			a.numDim = 1;
			a.numElem = 100;
			a.dimSizes = new int[] { 1 };
			// puts in the values for the standard CCMC attributes
			getAttrValues(a);

			b = this.getVariableObject(--u);
			b.dataValues = new float[100];
			b.dt = "float";
			b.numDim = 1;
			b.numElem = 100;
			b.dimSizes = new int[] { 1 };
			// puts in the values for the standard CCMC attributes
			getAttrValues(b);

			c = this.getVariableObject(--u);
			c.dataValues = new float[315];
			c.dt = "float";
			c.numDim = 1;
			c.numElem = 315;
			c.dimSizes = new int[] { 1 };
			// puts in the values for the standard CCMC attributes
			getAttrValues(c);

		}

		Scanner xreader = new Scanner(new File(AuxiliaryFiles[0]));
		xreader.nextLine();

		for (int i = 0; xreader.hasNextLine() && xreader.hasNextFloat(); i++)
		{
			((float[]) (xbz.dataValues))[i] = ((float[]) (xby.dataValues))[i] = ((float[]) (x.dataValues))[i] = xreader.nextFloat();
			xreader.nextFloat();
			((float[]) (xbx.dataValues))[i] = xreader.nextFloat();
		}

		Scanner yreader = new Scanner(new File(AuxiliaryFiles[1]));
		yreader.nextLine();

		for (int i = 0; yreader.hasNextLine() && yreader.hasNextFloat(); i++)
		{
			((float[]) (ybz.dataValues))[i] = ((float[]) (ybx.dataValues))[i] = ((float[]) (y.dataValues))[i] = yreader.nextFloat();
			yreader.nextFloat();
			((float[]) (yby.dataValues))[i] = yreader.nextFloat();
		}

		Scanner zreader = new Scanner(new File(AuxiliaryFiles[2]));
		zreader.nextLine();

		for (int i = 0; zreader.hasNextLine() && zreader.hasNextFloat(); i++)
		{
			((float[]) (zby.dataValues))[i] = ((float[]) (zbx.dataValues))[i] = ((float[]) (z.dataValues))[i] = zreader.nextFloat();
			zreader.nextFloat();
			((float[]) (zbz.dataValues))[i] = zreader.nextFloat();
		}

		for (int u = this.getNumVariables() - 1; u > this.getNumVariables() - 13; u--)
		{
			a = this.getVariableObject(u);
			this.minmaxCalculator(a, (float[]) a.dataValues);
		}

	}

	/**
	 * Parses the ASCII text associated with each variable. Assigns the data
	 * values to each variable.
	 * 
	 * @throws NoAttributeException
	 */
	private void textParser() throws NoAttributeException
	{

		for (int i = 0; i < getNumVariables(); i++)
		{

			Variable var = getVariableObject(i);

			float[] values = new float[var.numElem];

			// if actual max = actual min, then all values are the same
			if (var.getAttributeObject("actual_min").value.equals(var.getAttributeObject("actual_max").value))
			{
				for (int j = 0; j < var.numElem; j++)
				{
					values[j] = (Float) (var.getAttributeObject("actual_min").value);
				}
			}

			if (CommandLineInterface.verboseFlag)
			{
				logger.debug("-- for " + var.KameleonName + ", the setholder holds " + setholder.get(i).length + " sets of lines.");
				logger.debug("   its first set includes these lines to begin with:");
				logger.debug(setholder.get(i)[0]);
				logger.debug(setholder.get(i)[1]);
				logger.debug(setholder.get(i)[2]);
				logger.debug(setholder.get(i)[98437]);
			}

			int linenumber = 0;
			int numMSfails = 0;
			int numLSfails = 0;

			// this parses through each Line of ASCII text
			for (int block64 = 0; block64 < var.numElem; block64 += 64)
			{
				int[] msbits = new int[72];
				int numdecoded;
				int blocksize = Math.min(64, var.numElem - block64);
				float dzi;

				String buffer = setholder.get(i)[linenumber++];
				// int bytes = buffer.length();

				numdecoded = decode(buffer, msbits);
				if (numdecoded != blocksize)
				{
					// if(CommandLineInterface.verboseFlag)
					logger.error("FAIL msbits " + numdecoded + " " + blocksize);
					numMSfails++;
				}

				buffer = setholder.get(i)[linenumber++];
				// bytes = buffer.length();
				int[] lsbits = new int[72];

				numdecoded = decode(buffer, lsbits);
				if (numdecoded != blocksize)
				{
					// if(CommandLineInterface.verboseFlag)
					logger.error("FAIL lsbits " + numdecoded + " " + blocksize);
					numLSfails++;
				}

				dzi = ((Float) (var.getAttributeObject("actual_max").value) - (Float) (var.getAttributeObject("actual_min").value)) / 4410;

				for (int j = 0; j < blocksize; j++)
				{
					msbits[j] -= 33;
					lsbits[j] -= 33;

					float sign = 1.0f;

					if (msbits[j] >= 47)
					{
						sign = -1.0f;
						msbits[j] -= 47;
					}

					int intvalue = (int) (lsbits[j] + 94.0 * msbits[j]);

					values[block64 + j] = sign * (float) Math.exp((dzi * intvalue + (Float) (var.getAttributeObject("actual_min").value)));

				}
			}
			if (numMSfails > 0 || numLSfails > 0)
			{
				logger.error("FAIL ls bits:" + numLSfails + " occurrences for " + var.KameleonName);
				logger.error("FAIL ms bits:" + numMSfails + " occurrences for " + var.KameleonName);
			}

			var.dataValues = values;
		}

	}

	/**
	 * Goes from the ASCII step to the integers step. Takes the empty array of
	 * ints and fills it in with the appropriate values. Returns the number of
	 * values in the line.
	 * 
	 * @param line
	 *            One String line read in from a variable's ASCII text
	 * @param outarray
	 *            A reference to an empty array of ints (either msbits[72] or
	 *            lsbits[72])
	 * @return the number of values in the line
	 */
	public static int decode(String line, int[] outarray)
	{

		char[] chars = new char[128];

		try
		{
			line.getBytes("US-ASCII");
		} catch (UnsupportedEncodingException e)
		{
			logger.error("US-ASCII unsupported");
		}
		for(int i=0; i<line.length(); i++){
			chars[i]=line.charAt(i);
		}
		
		
		int runvalue;
		int runcount;
		int inputchecksum = chars[0];
		int checksum = 0;
		int indexin = 1;
		int indexout = 0;

		// for error checking we will make all of the values in the int[] equal
		// to 34
		for (int i = 0; i < 64; i++)
			outarray[i] = 34;

		while ((runvalue =  (chars[indexin])) != 0)
		{
			logger.error("char: " + chars[indexin] + " (" + (int)chars[indexin] + ") index_in: " + (indexin) + " run value: " + runvalue + " checksum: " + checksum);
			if (runvalue < 0)
			{
				//logger.debug("Should never get here!!!");
				//System.exit(1);
				runvalue += 256;
			}
			if (runvalue > 127)
			{
				runcount = runvalue - 170;
				runvalue = (int) chars[++indexin];
				if (runvalue < 0)
				{
					runvalue += 256;
				}
				for (int count = 0; count < runcount; count++)
				{
					if (indexout < 67)
						outarray[indexout++] = runvalue;
					else
					{
						// logger.debug("decode: catastrophic error -- trying to write outside the output array");
					}
					checksum += runvalue;
				}
			} else
			{
				if (indexout < 67)
					outarray[indexout++] = runvalue;
				else
				{
					// logger.debug("decode: catastrophic error -- trying to write outside the output array");
				}
				checksum += runvalue;
			}
			indexin++;
			if (indexin > 67)
			{
				logger.error("Can't find error");
				return indexout;
			}
			
		}

		if (indexout > 64){
			logger.error("Range error");
			return 64;}
		if (inputchecksum != 33 + (checksum % 92))
		{
			logger.error("Checksum error");
			return 0;
		}
		//logger.error("checksum: " + checksum + " input_checksum: " + inputchecksum + " other: " + (33 + (checksum % 92)));

		return indexout;
	}

}

// look at RLED