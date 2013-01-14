/*
 * 
 * Name: MAS.java
 * 
 * Version: 6.0
 * 
 * Author: Nitesh Donti
 * 		   NASA-GSFC-CCMC (Code 587)
 * 		   Intern
 * 
 * Purpose: Custom read routine for the
 * 			MAS input model file.
 * 	
 *  
 * Modification History:
 *  
 * Summer 2011 	Donti, Nitesh
 *  			Extended the Model superclass  	
 *  			 
 * Tasks: 	Need to create the read() method, etc. 
 * 
 * Notes:   mas.xml is in the directory entitled "Unused XML Files" 
 * 			because it has yet to be developed
 */

package gov.nasa.gsfc.ccmc.KameleonConverter;

import org.apache.log4j.Logger;

public class MAS extends Model {
	
	/**
	 * Logger for the MAS.java class. 
	 * Use Logger.info(), Logger.debug(), and Logger.error() to print
	 * statements out within the class. 
	 */
	static Logger logger = Logger.getLogger(MAS.class);
	

	/**
	 * Constructor for the MAS Object
	 * Passes in the default XML file for MAS (mas.xml)
	 * into the constructor for the superclass Model.java
	 */
	public MAS() {

		super("MAS.xml");
	}
	
	/**
	 * Returns "MAS Object" to describe, briefly, 
	 * what this Object is
	 */
	public String toString(){
		return "MAS Object";
	}

	/**
	 * Reads in the values and information from the 
	 * input MAS file
	 */
	public void read() {
		
	}

}


