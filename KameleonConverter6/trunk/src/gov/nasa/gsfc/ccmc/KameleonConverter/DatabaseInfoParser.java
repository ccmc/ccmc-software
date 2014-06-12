/*
 * Name: DatabaseInfoParser.java
 * 		 DatabaseInfoLine.java
 * 
 * Version: 6.0
 * 
 * Author: Nitesh Donti
 * 		   NASA-GSFC-CCMC (Code 587)
 * 		   Intern
 * 
 * Purpose: Parses the DatabaseInfo file, and calls 
 * 			DatabaseInfoLine.java (a class within this
 * 			java file) for further assistance. 
 * 			Checks for some specific keywords and stores
 * 			them as CCMC names/values. 
 * 			Finds all other information in the DatabaseInfo
 * 			file and stores them as global variables. 
 *  
 * Modification History:
 *  
 * Summer 2011 	Donti, Nitesh
 * 				Initial Development Started
 * 				All tasks complete
 * 				Ready to use
 *  			  
 * 08.01.2011 	Berrios, David
 * 				New class made (DatbaseInfoLine.java)
 * 				Line Parser complete
 * 				Ready to use
 *  			 
 * 
 */

package gov.nasa.gsfc.ccmc.KameleonConverter;

import java.io.File;
import java.io.FileNotFoundException;
import java.util.Scanner;
import org.apache.log4j.Logger;

public class DatabaseInfoParser {

	/**
	 * Logger for the DatabaseInfoParser.java class. 
	 * Use Logger.info(), Logger.debug(), and Logger.error() to print
	 * statements out within the class. 
	 */
	static Logger logger = Logger.getLogger(DatabaseInfoParser.class);

	/**
	 * Holds the DatabaseInfo text file
	 */
	File file;

	/**
	 * Constructor for the DatabaseInfoParser class
	 * Adds information (mostly global attributes) from the 
	 * DatabaseInfo file to the Model that is passed in
	 * @param filename The path of the DatabaseInfo file
	 * @param m The model object to which to add global attributes and other information 
	 * 			from the database info file
	 * @throws NoAttributeException 
	 * @throws FileNotFoundException
	 */
	DatabaseInfoParser(String filename, Model m) throws NoAttributeException, FileNotFoundException{
		file= new File (filename);
		parse(m);
	}

	/**
	 * Parses the DatabaseInfo file and adds info to the Model
	 * @param m
	 * @throws NoAttributeException
	 * @throws FileNotFoundException
	 */
	public void parse(Model m) throws NoAttributeException, FileNotFoundException{

		logger.info("\n-----\n...Now Parsing Database File");

		Scanner s = new Scanner(file);
		KAttribute temp;

		String[] keywords ={"run_number", "last_name", "request_date", "start_", "end_" };
		String[] keys ={"run_registration_number", "generated_by", "generation_date", "start_time", "end_time"};

		String line = null;


		while(s.hasNextLine()){
			line = s.nextLine();

			DatabaseInfoLine dil = new DatabaseInfoLine(line);
			if (dil.getName() != null)
			{
				boolean special = false;
				for(int i=0; i<keywords.length; i++){
					if(dil.getName().startsWith(keywords[i]) && dil.getValue() !=null){
						special = true;
						temp=m.getGlobalAttribute(keys[i]);
						temp.value = (dil.getValue());
					}
				}
				if(!special && dil.getValue()!=null ){
					//this checks to see if there is already a global attribute by that name. if not, then it will create the new global attribute. 
					try{
						m.getGlobalAttribute(dil.getName());
					}catch(NoAttributeException e){
						m.addGlobalAttribute(new KAttribute(dil.getName(), dil.getValue(), "", "model", "String"));
					}
				}
			}
		}

		m.getGlobalAttribute("terms_of_usage").value = (m.getGlobalAttribute("terms_of_usage").description);
		m.getGlobalAttribute("kameleon_version").value = (CommandLineInterface.KameleonVersion);
		System.out.println("-----");
	}

	public class DatabaseInfoLine {
		
		/**
		 * The name of the Global Attribute as taken 
		 * from the left side of the file
		 */
		private String name;
		
		/**
		 * The value of the Global Attribute as taken
		 * from the right side of the file
		 */
		private String value;

		public DatabaseInfoLine(String line)
		{
			String tokens[] = line.split("#");
			if (tokens.length != 2)
			{
				name = null;
				value = null;
			} else
			{
				value = tokens[0].trim();
				name = tokens[1].trim();
			}
		}
		
		public String getName()
		{
			return name;
		}

		public String getValue()
		{
			return value;
		}

		//used for testing purposes
		public String toString()
		{
			String out = "name: " + name + " value: " + value;
			return out;
		}	
	}
}


