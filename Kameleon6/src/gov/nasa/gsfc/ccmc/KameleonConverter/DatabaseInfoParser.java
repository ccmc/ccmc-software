package gov.nasa.gsfc.ccmc.KameleonConverter;

import java.io.File;
import java.io.FileNotFoundException;
import java.util.ArrayList;
import java.util.Scanner;
import org.apache.log4j.Logger;


public class DatabaseInfoParser {

	static Logger logger = Logger.getLogger(DatabaseInfoParser.class);

	File file;


	DatabaseInfoParser(String f, Model m) throws NoAttributeException, FileNotFoundException{
		file= new File (f);
		parse(m);
	}


	public void parse(Model m) throws NoAttributeException, FileNotFoundException{

		System.out.println("\n-----\n...Now Parsing Database File");

			Scanner s = new Scanner(file);
			KAttribute temp;


			ArrayList<String> entries = new ArrayList<String>();
			while(s.hasNextLine())
				entries.add(s.nextLine());

			String[] keywords ={"run_number", "last_name", "request_date", "start_", "end_"};
			String[] keys ={"run_registration_number", "generated_by", "generation_date", "start_time", "end_time"};
			//What should we do about all of the other information that we find in the DatabaseInfo file?
			//Should we just collect all of the extra information without any mapping?
			//Or should we implement more XML documents?
			
			int numOfKeysFound=0;


			for(int i=0; i<keys.length; i++){

				int linenum=0;
				String line=entries.get(linenum);

				//looks for the keyword
				while(line.indexOf(keywords[i])==-1 && linenum<entries.size()-1){
					linenum++;
					line=entries.get(linenum);
				}

				//if the keyword is found, assigns it as a value to the corresponding key in the hashmap
				if(line.indexOf(keywords[i])!=-1){
					
					numOfKeysFound++;

					String val = line.substring(0, line.indexOf(" "));
					
					
					//In the case of the model name, we convert it to lower case letters, so that programs like the interpolator can recognize it more easily.
					//As soon as the interpolator is updated, we can erase this workaround. 
					if(keywords[i].equals("model_")){
						val=val.toLowerCase();}
					
					temp=m.getGlobalAttribute(keys[i]);
					temp.setValue(val);

					logger.info("setting " + temp.name + " to " + temp.value);
					
				}
				linenum=0;
			}

			m.getGlobalAttribute("terms_of_usage").setValue(m.getGlobalAttribute("terms_of_usage").description);
			m.getGlobalAttribute("kameleon_version").setValue(CommandLineInterface.KameleonVersion);
			
	
			if(numOfKeysFound==0){
				logger.error("**ERROR**No Information found from Database Info File.");
			}
			

		System.out.println("-----");


	}



}
