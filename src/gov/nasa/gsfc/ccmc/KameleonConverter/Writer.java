/*
 * 
 * Name: Writer.java
 * 
 * Version: 6.0
 * 
 * Author: Nitesh Donti
 * 		   NASA-GSFC-CCMC (Code 587)
 * 		   Intern
 * 
 * Purpose: The superclass for all of the writer classes. 
 *  
 * Modification History:
 *  
 * Summer 2011 	Donti, Nitesh
 * 				Initial Development Started
 * 				All tasks complete
 * 				Ready to use
 *  			  
 * Tasks: 	
 * 	 
 * 
 */

package gov.nasa.gsfc.ccmc.KameleonConverter;
import java.util.ArrayList;

import org.apache.log4j.Logger;

public abstract class Writer {

	Logger logger = Logger.getLogger(Writer.class);

	/**
	 * The complete filepaths of the new files that have already been written
	 * to the computer.
	 * 
	 */
	public ArrayList<String> namesOfWrittenFiles = new ArrayList<String>();

	/**
	 * The name of the new File that you are going to create. 
	 */
	String newfilename;

	public void convert (Model m) throws NoAttributeException{
		//just marking the beginning of the conversion
		logger.info("...now beginning conversion of "+m+" to a " +this);
		logger.info("...writing");

		if(m.getNumVariables()==0){
			logger.error("***ERROR***There are no variables. \nEXITING...");
			System.exit(0);
		}

		newfilename= m.OutputDirectory+"/"+m.getGlobalAttribute("run_registration_number").value+".";
		newfilename+= m.getGlobalAttribute("model_name").value+"."+m.getTimestep()+this.extension();
		this.namesOfWrittenFiles.add(newfilename);
	}

	/**
	 * 
	 * @return the extension of the file (e.g.  .nc, .hdf, .cdf)
	 */
	abstract protected String extension();

}
