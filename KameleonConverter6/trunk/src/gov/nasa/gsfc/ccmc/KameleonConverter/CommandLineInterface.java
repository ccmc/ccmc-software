/*
 * 
 * Name: CommandLineInterface.java
 * 
 * Version: 6.0
 * 
 * Author: Nitesh Donti
 * 		   NASA-GSFC-CCMC (Code 587)
 * 		   Intern
 *  
 * Modification History:
 *  
 * January 2012 Donti, Nitesh
 * 				Added support for the HDF5 input model
 * 				Added a check to see if the input directory was a file rather than a directory
 *  
 * Summer 2011 	Donti, Nitesh
 *  			Initial Development Started
 *  			All tasks complete 
 *  			Ready to use 
 *  
 *  *Any changes will only be necessitated pending any new models or formats.
 * 
 * 
 */

package gov.nasa.gsfc.ccmc.KameleonConverter;
import jargs.gnu.CmdLineParser;
import java.io.File;
import java.io.FileNotFoundException;
import java.io.IOException;
import java.util.ArrayList;
import java.util.Date;
import java.util.Properties;
import java.util.Scanner;
import java.text.DateFormat;
import java.text.SimpleDateFormat;
import org.apache.log4j.Logger;
import org.apache.log4j.PropertyConfigurator;


/**
 * 
 * @author Nitesh Donti
 * 		   Intern, National Aeronautics and Space Administration
 * 		   Goddard Space Flight Center
 * 		   Community Coordinated Modeling Center (CCMC)
 * @version 6.0
 * 
 * Date: Summer 2011 (June 1st, 2011 - August 5th, 2011)
 * 
 *
 */

public class CommandLineInterface {

	/**
	 * A logger for the class. 
	 * Options for printing out statements include .info(), .debug(), and .error().
	 */
	static Logger logger = Logger.getLogger(CommandLineInterface.class);

	/**
	 * Toggled ON if user wants verbose output and extra details when running the program. 
	 * User can turn this to TRUE by entering "-v" as an argument in the CLI.
	 */
	public static boolean verboseFlag;

	/**
	 * Toggled ON if user does not want Actual Min/Max calculations.
	 * Turning this ON will save a little time.  
	 * User can turn this to TRUE by entering "-n" as an argument in the CLI.
	 */
	public static boolean nominmaxFlag;	

	public static int num2run;

	/**
	 * The current Version number of the Kameleon Conversion Software. 
	 */
	public static String KameleonVersion = "6.01";


	/**Prints the options for a proficient command line user.  **/
	private static void printUsage() {
		System.err.println(
				"\n[-h|--help]      [-n|,--nominmax]	  [-i|--indir=string]      [-m|--model=string]\n" +
						"[-v|--verbose]                            [-o|--outdir=string]     [-a|--aux_file=string]\n" +
				"[--version]      [-c|--compress=string]	  [-d|--database=string]   [-f|--format=string]");
	}

	/**Prints the options and also extended descriptions for the
	 * novice or unfamiliar command line user in the case that he 
	 * or she requests help. 
	 */
	private static void printExtendedUsage(){

		System.out.println(
				"**Always Required\n"+
						"*Usually Required\n"+
						"[-h|--help]                 print this help information\n" +
						"[-v|--verbose]              turn verbose output on\n" +
						"[--version]                 print software version number\n" +
						"[-n|--nominmax]             turn OFF actual min/max calculations for each variable\n" +
						"[-c|--compress=string]      use specified compression algorithm\n" +
						"[-i|--indir=string]**       directory containing input files to be converted\n" +
						"[-o|--outdir=string]*       output directory where converted files will be placed (input directory is the default)\n" +
						"[-d|--database=string]*     ccmc DatabaseInfo file location - for CCMC use only\n" +
						"[-m|--model=string]**       model name\n" +
						"[-a|--aux_file=string]*     auxiliary input file - sometimes required (e.g. a grid file seperate from general data files)\n" +
				"[-f|--format=string]**      data format to convert to");	
	}

	/**
	 * 1)Checks to see that all required data are present.
	 * 2)Checks the validity of all of the arguments passed in by the 
	 * command line user (or the default values).
	 * 3)Creates new model and writer objects according to the 
	 * modelValue and formatValue passed in by the user. 
	 * @throws NoAttributeException 
	 * @throws FileNotFoundException 
	 * 
	 **/
	private static Writer createModel(String compressValue, 
			String inValue, String outValue, String databaseValue, String modelValue, 
			String auxValue, String formatValue) throws NoAttributeException, FileNotFoundException{

		Writer writer = null;
		Object[] Xfiles={};

		/**
		 * Many runs will require numerous files to be converted, possibly
		 * due to a large number of timesteps. This Model object represents one 
		 * instance/timestep. It will be created and then converted to the output format. Then, 
		 * it will take on the form of the next timestep.
		 */
		Model instance;




		//---------------------checking validity of MODEL VALUE-------------------
		/**
		 * 
		 * Model_key assigns an int value to each type of model:
		 * 
		 ****************************************************************************
		 *                                                                          *
		 *  Builds a key value reference table for models to use switch statements  *
		 *                                                                          *
		 *    batsrus        = 1                                                    *
		 *    ucla_ggcm      = 2   ---->                                            *
		 *    open_ggcm      = 2   ----> both strings will point to same case       *
		 *    ctip           = 3                                                    *
		 *    enlil          = 4                                                    *
		 *    kpvt           = 5                                                    *
		 *    msfc           = 6                                                    *
		 *    mas            = 7                  									*
		 *    swmf_ionosphere= 8                                                    *
		 *    HDF5           = 9                   									*
		 *    EnlilCME       = 10                                                   *
		 *                                                                          *
		 ****************************************************************************
		 *
		 *
		 **/
		int model_key = 0;

		if(modelValue.toLowerCase().indexOf("enlil")!=-1){
			if(modelValue.toLowerCase().indexOf("cme")!=-1){
				model_key=10;
				modelValue="EnlilCME";
				}
			else{
				model_key=4;
				modelValue="Enlil";
			}
		}
		else if(modelValue.toLowerCase().indexOf("ctip")!=-1){
			model_key=3;
			modelValue="CTIP";
		}
		else if(modelValue.toLowerCase().indexOf("mas")!=-1){
			model_key=7;
			modelValue="MAS";
		}
		else if(modelValue.toLowerCase().indexOf("kpvt")!=-1){
			model_key=5;
			modelValue="KPVT";
		}
		else if(modelValue.toLowerCase().indexOf("ggcm") !=-1 || modelValue.toLowerCase().indexOf("ucla") !=-1|| 
				modelValue.toLowerCase().indexOf("open") !=-1){
			model_key=2;
			modelValue="Open GGCM";
		}
		else if(modelValue.toLowerCase().indexOf("msfc") !=-1 || modelValue.toLowerCase().indexOf("tvm") !=-1){
			model_key=6;
			modelValue="MSFC TVM";
		}
		else if(modelValue.toLowerCase().indexOf("bats")!=-1){
			model_key=1;
			modelValue="BATS-R-US";
		}
		else if(modelValue.toLowerCase().indexOf("swmf")!=-1 || modelValue.toLowerCase().indexOf("ion")!=-1){
			model_key=8;
			modelValue="SWMF";
		}
		else if(modelValue.toLowerCase().indexOf("hdf")!=-1){
			model_key=9;
			modelValue="HDF5";
		} 
		else {
			logger.error("***ERROR***Model Type not found.\nPlease check the name/spelling.");
			System.exit(0);
		}

		logger.info("...selected "+ modelValue+" as Input Model Type");
		//---------------------END checking validity of MODEL VALUE-------------------




		//---------------------checking validity of OUTPUT DIRECTORY VALUE-------------------
		outValue+="/conversionResultsFiles";
		if(verboseFlag)
			System.out.println("checking --outdir argument for validity...");

		File outputdirectoryfile = new File(outValue);
		//this if-statement checks if the directory was made. if not, it checks if it ever existed in the first place. if not, the program ends. 
		//this is to check if the output directory given is valid. 
		if(!outputdirectoryfile.mkdir() && !outputdirectoryfile.exists()){
			logger.error("**ERROR**Could not locate Output Directory. Please check it and try again.");
			System.exit(2);
		}
		else{
			logger.info("...setting "+ outValue +" as location to store converted files");
		}
		//---------------------END checking validity of OUTPUT DIRECTORY VALUE-------------------



		//---------------------checking validity of DATABASE FILE VALUE-------------------
		if(databaseValue!=null){
			if(verboseFlag)
				logger.debug("checking --database argument for validity...");
			File databasefile = new File(databaseValue);
			logger.info(databaseValue);
			if(!databasefile.exists()){
				logger.error("**ERROR**Could not locate Database Info file.");
				databaseValue=null;
			}
		}
		else{
			logger.error("*********\n**ERROR** >> No Database Info File selected. Some information will be missing. \n*********");
		}
		//---------------------END checking validity of DATABASE FILE VALUE-------------------


		//---------------------checking validity of AUXILIARY FILE VALUE-------------------
		String[] auxFiles = null;

		if(auxValue==null){
			if(verboseFlag)
				logger.debug("no --aux file argument specified...\n...checking dependencies and requirements");

			switch(model_key){


				case 2: 
				case 3:
				case 5:
				case 6:
				case 7:
					logger.error("***ERROR***Auxiliary file required for a " + modelValue);
					System.exit(2);
					break;

				default: 
					logger.info("...no auxiliary file selected");
					break;

			}
		}else{

			File auxDir = new File(auxValue);
			Scanner auxReader;
			ArrayList<String> auxFilesList = null;

			switch(model_key){

				case 1:
				case 4:
				case 10:
				case 8:
				case 9:
					logger.info("No auxiliary file required for "+ modelValue +"... Disregarding entered value ("+auxValue+").");
					break;
				case 2:

					if(auxDir.isDirectory()){
						File[] files = auxDir.listFiles();
						auxFilesList = new ArrayList<String>();
						for(int k=0; k<files.length; k++){
							if(files[k].getName().startsWith("grid") && files[k].getName().endsWith(".txt")){
								auxFilesList.add(files[k].getAbsolutePath());
								auxReader= new Scanner(new File(files[k].getAbsolutePath()));
								if (auxReader.nextLine().indexOf("#") == -1){
									logger.error("***ERROR***Auxiliary file"+ files[k].getAbsolutePath() + " for " + modelValue + " is invalid.");
								}
							}
						}
					}

					if(auxFilesList == null || auxFilesList.size()!=3){
						logger.error("**ERROR**Could not locate the 3 necessary grid files (auxiliary files)");
						logger.error("Please place these files in the auxiliary directory and try again.");
						logger.error("EXITING...");
						System.exit(2);
					}
					else{
						auxFiles = auxFilesList.toArray(new String[3]);
						logger.info("Located the following as auxiliary grid files:");
						for(int k=0; k<auxFilesList.size(); k++)
							logger.info("-- " + auxFilesList.get(k));
					}

					break;

				default: 
					logger.info("...setting Auxiliary File to "+auxValue);
					break;	

			}

		}
		//---------------------END checking validity of AUXILIARY FILE VALUE-------


		//---------------------checking validity of INPUT DIRECTORY VALUE-------------------
		if(verboseFlag)
			System.out.println("checking --indir argument for validity...");

		ArrayList<String> pathholder = new ArrayList<String>();
		File bigfolder = new File (inValue);

		if(!bigfolder.exists()){
			System.err.println("\n***ERROR*** Input Directory:" + inValue + " does not exist.\nPlease check the location/spelling and try again.");
			System.exit(0);
		}

		if(bigfolder.isFile())
		{
			logger.error("**ERROR**You have listed a file. Please set the input as a directory through which we may search.\nEXITING...");
			System.exit(2);
		}

		File[] files = bigfolder.listFiles();

		logger.info("Gathering files from input directory: "+ inValue);

		//this is now going to read in the appropriate files from the Input Directory
		switch(model_key){
			case 1:
			case 3:
			case 5:
			case 6:
			case 7: 
				logger.error("***"+modelValue.toUpperCase() + " not supported as an input model at this time. " +
						"Please try again some other day. \n");
				System.exit(0);
				break;

			case 2:
				if(verboseFlag)
					logger.debug("...Searching for \"3df\" files...");

				for(int i=0; i<files.length; i++){
					if (files[i].getName().toLowerCase().indexOf("3df")!=-1){
						if(files[i].isFile()){
							pathholder.add(files[i].getPath());
							logger.info("...storing "+ files[i].getPath());
						}
						else if(files[i].isDirectory()){
							File folder = new File(files[i].getPath());
							File[] innerfiles = folder.listFiles();

							for(int k=1; k<innerfiles.length; k++){
								//this is looking for open ggcm files that have "3df.", but not topological files or header files
								//that contain "topo" or ".h", respectively. 
								if (innerfiles[k].getName().toLowerCase().indexOf("3df.")!=-1 && innerfiles[k].getName().toLowerCase().indexOf("topo")==-1 && innerfiles[k].getName().toLowerCase().indexOf(".h")==-1 ){
									pathholder.add(innerfiles[k].getPath());
									if(pathholder.size()<10)
										logger.info("...storing "+ innerfiles[k].getPath());
									else if(pathholder.size()==10){
										logger.info("...storing more files...");
									}
								} else{
									if(pathholder.size()<10 && CommandLineInterface.verboseFlag)
										logger.debug("*not adding " + innerfiles[k].getPath());
								}
							}
						}
					}
				}
				break;

			case 4:
			case 10:
				if(verboseFlag)
					logger.debug("...Searching for \"tim.\" files...");
				for(int i=0; i<files.length; i++){
					if (files[i].getName().startsWith("tim.") && files[i].getName().endsWith(".nc")){
						pathholder.add(files[i].getPath());
						if(pathholder.size()<10)
							logger.info("...storing "+ files[i].getPath());
						else if(pathholder.size()==10){
							logger.info("...storing more files...");
						}
					}
				}
				break;

			case 8:	
				if(verboseFlag)
					logger.debug("...Searching for \".tec\" files...");
				for(int i=0; i<files.length; i++){
					if (files[i].getName().endsWith(".tec")){
						pathholder.add(files[i].getPath());
						if(pathholder.size()<10)
							logger.info("...storing "+ files[i].getPath());
						else if(pathholder.size()==10){
							logger.info("...storing more files...");
						}
					}
				}
				break;

			case 9:	
				if(verboseFlag)
					logger.debug("...Searching for \".h5\" files...");
				for(int i=0; i<files.length; i++){
					if (files[i].getName().endsWith(".h5")){
						pathholder.add(files[i].getPath());
						if(pathholder.size()<10)
							logger.info("...storing "+ files[i].getPath());
						else if(pathholder.size()==10){
							logger.info("...storing more files...");
						}
					}
				}
				break;
			default: System.err.println("***UNKNOWN ERROR***(input directory validation)");
		}

		Xfiles = pathholder.toArray();
		if (Xfiles.length==0){
			logger.error("***ERROR***Invalid Input Directory. No usable files.");
			System.exit(0);
		}else{
			logger.info("Stored " + Xfiles.length + " files.");
		}
		//---------------------END checking validity of INPUT DIRECTORY VALUE--------------



		//---------------------checking validity of FORMAT VALUE-------------------
		if(verboseFlag)
			logger.info("checking --format argument for validity...");
		if (formatValue.toLowerCase().indexOf("net")!=-1)
			writer = new NetCDF4Writer();
		else if(formatValue.toLowerCase().indexOf("cdf")!=-1)
			writer = new CDFWriter();
		else if(formatValue.toLowerCase().indexOf("hdf")!=-1)
			writer = new HDF5Writer();
		else{
			logger.error("***ERROR***Output Format Type -" +formatValue+ "-not found.\nPlease check the name/spelling.");
			System.exit(0);
		}
		logger.info("...selected " + writer);
		//---------------------END checking validity of FORMAT VALUE-------------------


		//---------------------checking validity of COMPRESSION ALGORITHM VALUE-------------------
		if(compressValue==null){
			logger.info("...no compresion algorithm selected.");
		}
		else{
			logger.info("...selected "+compressValue+" as compression algorithm. Unfortunately, there is no support for compression algorithms at this time.");
		}
		//---------------------END checking validity of COMPRESSION ALGORITHM VALUE-------------------


		int start; 
		int end;

		//The for-loop below will execute all of the conversions unless 
		//the -t argument from the command line specified a lower number 
		//of runs to execute
		int runs = Math.min(CommandLineInterface.num2run, Xfiles.length);


		//Assigning the command line arguments to the new model object
		for(int i=0; i<runs; i++){

			logger.info("\n*Beginning Conversion Number " + (i+1) +" of "+ runs+".*");
			switch(model_key){

				case 1:
					instance = new BATSRUS(); break;
				case 2:
					instance = new OPEN_GGCM();
					start = Xfiles[i].toString().lastIndexOf("3df");
					instance.setTimestep(Xfiles[i].toString().substring(start+4));
					break;
				case 3:
					instance = new CTIP(); break;
				case 4:
				case 10:
					instance = new ENLIL(model_key==10);
					start = Xfiles[i].toString().indexOf(".");
					end = Xfiles[i].toString().lastIndexOf(".");
					instance.setTimestep(Xfiles[i].toString().substring(start+1,end));
					break;
				case 5:
					instance = new KPVT(); break;
				case 6:
					instance = new MSFC_TVM(); break;
				case 7:
					instance = new MAS(); break;
				case 8: 
					instance = new SWMF(); 
					start = Xfiles[i].toString().lastIndexOf("/");
					end = Xfiles[i].toString().lastIndexOf(".");
					instance.setTimestep(Xfiles[i].toString().substring(start+1,end)); break;
				case 9:
					instance = new HDF5(); 
    				start = Xfiles[i].toString().lastIndexOf("/");
					end = Xfiles[i].toString().lastIndexOf(".");
					instance.setTimestep(Xfiles[i].toString().substring(start+1,end)); break;
				case 11:
					instance = new AMIE();break;
					
				default: {
					instance = null;
					System.err.println("***ERROR***Input Model Type -" + modelValue  +"-not found.\nPlease check the name/spelling.");
					System.exit(0);
				}

			}

			instance.setAuxiliaryFiles(auxFiles); //sometimes needed
			instance.setFilePathname((String) Xfiles[i]); //necessary
			instance.setOutputDirectory(outValue); //necessary
			instance.setDatabaseInfoFile(databaseValue);//would be nice to have. if null, will take care of it in Model.java. 

			//End - Assigning Command Line arguments

			try {
				instance.read();
				//adds a global attribute with the current time as of right now
				DateFormat dateFormat = new SimpleDateFormat("yyyy/MM/dd HH:mm:ss");
				Date date = new Date();
				instance.addGlobalAttribute(new KAttribute("Conversion Time", dateFormat.format(date), "Current Time after file is read in", "model", "string"));

			} catch (NoVariableException e) {
				e.printStackTrace();
			} catch (IOException e) {
				logger.error("***ERROR***Could not open " + instance.FilePathname);
				e.printStackTrace();
			}

			writer.convert(instance);

			//conversion finished successfully
			logger.info("\n"+instance+" "+ (i+1) + " of "+ Xfiles.length+ " was successfully converted.\n----------");


		}
		return writer;

	}

	/**
	 * @param args Should include -i, -o, -m, -f, etc. See CommandLineInterface.printUsage() for details. 
	 * @throws NoAttributeException 
	 * @throws FileNotFoundException 
	 */
	public static void main(String[] args) throws NoAttributeException, FileNotFoundException {
		//AMIE.parseFile();
		//System.exit(0);
		
		
		/**
		 * The time at which Conversion begins. 
		 */
		double epochtime1 = System.currentTimeMillis()/1000;


		Properties properties = new Properties();
		properties.put("log4j.rootLogger", "DEBUG, A1");
		properties.put("log4j.appender.A1","org.apache.log4j.ConsoleAppender");
		properties.put("log4j.appender.A1.layout","org.apache.log4j.PatternLayout");
		//# Print the date in ISO 8601 format


		System.out.println("--------------------------KAMELEON CONVERTER----------------------------");
		System.out.println("  Kameleon is a software suite that is being developed at the CCMC to\n"+
				"  address the difficulty in analyzing and disseminating the varying\n"+"  output formats of space weather model data.");
		System.out.println("------------------------------------------------------------------------");

		//if no arguments were ever passed in, shows directions and then exits
		if(args.length==0){
			printUsage();
			System.exit(2);}

		//creates the Command Line Parser and adds all of the options for the user
		CmdLineParser parser = new CmdLineParser();
		CmdLineParser.Option help = parser.addBooleanOption('h',"help");
		CmdLineParser.Option verbose = parser.addBooleanOption('v', "verbose");
		CmdLineParser.Option version = parser.addBooleanOption("version");
		CmdLineParser.Option nominmax = parser.addBooleanOption('n',"nominmax");
		CmdLineParser.Option compress = parser.addStringOption('c', "compress");
		CmdLineParser.Option in = parser.addStringOption('i', "indir");
		CmdLineParser.Option out = parser.addStringOption('o', "outdir");
		CmdLineParser.Option database = parser.addStringOption('d', "database");
		CmdLineParser.Option modeltype = parser.addStringOption('m', "model");
		CmdLineParser.Option aux = parser.addStringOption('a',"aux_file");
		CmdLineParser.Option format = parser.addStringOption('f',"format");
		CmdLineParser.Option quicktesting = parser.addIntegerOption('t',"testing");

		//parses command line arguments
		try {
			parser.parse(args);
		}
		catch ( CmdLineParser.OptionException e ) {
			System.err.println(e.getMessage());
			printUsage();
			System.exit(2);
		}

		//parses and assigns values 
		//Default values will be returned (instead of null) when the command line argument is missing.
		Boolean helpFlag = (Boolean)parser.getOptionValue(help, Boolean.FALSE);
		verboseFlag = (Boolean)parser.getOptionValue(verbose, Boolean.FALSE);
		Boolean versionFlag = (Boolean)parser.getOptionValue(version, Boolean.FALSE);
		nominmaxFlag = (Boolean)parser.getOptionValue(nominmax, Boolean.FALSE);
		num2run = (Integer)parser.getOptionValue(quicktesting,Integer.MAX_VALUE);
		String compressValue = (String)parser.getOptionValue(compress);
		String databaseValue = (String)parser.getOptionValue(database);
		String modelValue = (String)parser.getOptionValue(modeltype);
		String auxValue = (String)parser.getOptionValue(aux);
		String inValue = (String)parser.getOptionValue(in);
		String outValue = (String)parser.getOptionValue(out);
		String formatValue = (String)parser.getOptionValue(format);

		if(verboseFlag){
			properties.put("log4j.appender.A1.layout.ConversionPattern","%m |||| %d [%t] %-5p %c%n");
		}else{
			properties.put("log4j.appender.A1.layout.ConversionPattern","%m%n");
		}
		PropertyConfigurator.configure(properties);

		// The remaining command-line arguments -- those that do not start
		// with a minus sign -- can be captured with this:
		String[] otherArgs = parser.getRemainingArgs();
		
		if(verboseFlag)
			logger.info("\n----------------VALIDITY CHECKS----------------");

		//---------------------checking  that all required arguments are present-------------------
		if(modelValue==null){
			logger.error("**ERROR**Please specify the type of input model you would like to convert.\nBATS-R-US, CTIP, Enlil, EnlilCME, SWMF, HDF5, KPVT, MAS, MSFC TVM and OPEN GGCM (UCLA GGCM)\n(Use \"-m\")");
			printExtendedUsage();
			System.exit(2);
		}
		if(formatValue==null){
			logger.error("**ERROR**Please specify the format to which you would like to convert your input file. \nCDF, HDF5, NetCDF4 \n(Use \"-f\")");
			printExtendedUsage();
			System.exit(2);
		}
		if(inValue==null){
			logger.error("**ERROR**Please specify the input directory to process.(Use \"-i\")");
			printExtendedUsage();
			System.exit(2);
		}
		if(outValue==null){
			logger.error("**No Output Directory selected. Setting Output Directory to Input Directory..."+inValue);
			outValue=inValue;
		}
		//---------------------END checking  that all required arguments are present-------------------
		

		if(verboseFlag){
			logger.info("\nVerbose Output: ON");
			if(nominmaxFlag) 
				logger.info("Min/Max Calculations: OFF");
			logger.info("Input Model: " + modelValue);
			logger.info("Output Format: " + formatValue);
			logger.info("Setting Input Directory to " + inValue);
			logger.info("Setting Output Directory to " + outValue);
			logger.info("Setting DatabaseInfo File to " + databaseValue);
			if(auxValue!=null)
				logger.info("Setting Auxiliary File/Directory to " + auxValue);
			if(compressValue!=null)
				logger.info("Setting Compression Algorithm to " + compressValue);
		}

		//if help is requested with the --help argument
		if (helpFlag){
			logger.info("-------------------------------------------------------------");
			logger.info("Thanks for requesting some further help! \nThis program converts input models to a standardized format. Here are the directions for using our CLI.");
			logger.info("First, enter in a minus sign (-) followed immediately by the letter that corresponds with the field for which you want to specify data.");
			logger.info("Look at the categories shown below that correspond with these 1-character letters:");
			printExtendedUsage();
			logger.info("For example, you can type in following: \"-m mas\" if you want to specify the type of model as \"mas\".");
			logger.info("Or, for the -d and -v and --help options, simply type in \"-d\" or \"-v\" or \"--help\" to request debugging, verbosity, or help, respectively.");        	
			logger.info("-------------------------------------------------------------");
			logger.info("Currently, we accept the following input model types:\n    -BATS-R-US: Block-Adaptive-Tree-Solarwind-Roe-Upwind-Scheme\n    " +
					"-CTIP: Coupled Thermosphere Ionosphere Plasmasphere\n    -Enlil\n   -HDF5 (Hierarchical Data Format)\n -SWMF: Space Weather Modeling Framework\n    -KPVT: Key Parameter" +
					" Visualization Tool\n    -MAS\n    -MSFC TVM: Marshall Space Flight Center Tower Vector Magnetograph\n    -Open (or UCLA) GGCM: Open" +
					" Geospace General Circulation Model");
			logger.info("We can convert these input files into the following output files:\n    -CDF\n    -HDF5\n    -NetCDF4");
			logger.info("If your model type requires an auxiliary file, please specify its location.");
		}

		//if version is requested with the --version argument
		if(versionFlag){
			logger.info("This is KAMELEON CONVERTER SOFTWARE VERSION " + KameleonVersion);
		}

		//Prints arguments that did not fit any of the 12 options
		if(otherArgs.length!=0){
			logger.error("Unused arguments: ");
			for ( int i = 0; i < otherArgs.length; ++i ) {
				logger.error(otherArgs[i]);
			}
		}

		//checks the validity of each argument
		//and creates new model and writer objects accordingly
		Writer w= createModel(compressValue, 
				inValue, outValue, databaseValue, modelValue, auxValue, formatValue);
		
		outValue+="/conversionResultsFiles";

		//keeping track of how long everything took and closing remarks
		logger.info("\n\n-------------\nConversion Complete.");
		logger.info("-Total Time: " + (System.currentTimeMillis()/1000-epochtime1) + " seconds");
		logger.info("\nThank you for using the Kameleon Conversion Software.");
		logger.info("Please check " + outValue + " for your new " + formatValue + " files:\n");
		for(String newfilename: w.namesOfWrittenFiles){
			if(newfilename!=null)
				logger.info("-- "+newfilename);}
		logger.info("\n***For tracking purposes of our government sponsors, we ask that you notify the CCMC \nwhenever you use CCMC results in scientific publications or presentations: ccmc@ccmc.gsfc.nasa.gov");
		System.exit(0);

	}
}
