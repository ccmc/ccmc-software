/*
 * 
 * Name: CTIP.java
 * 
 * Version: 6.0
 * 
 * Author: Nitesh Donti
 * 		   NASA-GSFC-CCMC (Code 587)
 * 		   Intern
 * 
 * Purpose: Custom read routine for the
 * 			CTIP input model file.
 * 	
 *  
 * Modification History:
 *  
 * Summer 2011 	Donti, Nitesh
 *  			Extended the Model superclass  	
 *  			 
 * Tasks: 	Need to create the read() method, etc. 
 * 
 * Notes:   ctip.xml is in the directory entitled "Unused XML Files" 
 * 			because it has yet to be developed
 * 
 */

package gov.nasa.gsfc.ccmc.KameleonConverter;

import org.apache.log4j.Logger;

public class CTIP extends Model {
	
	/**
	 * Logger for the CTIP.java class. 
	 * Use Logger.info(), Logger.debug(), and Logger.error() to print
	 * statements out within the class. 
	 */
	static Logger logger = Logger.getLogger(CTIP.class);
	
	/**
	 * Constructor for the CTIP Object
	 * Passes in the default XML file for CTIP (ctip.xml)
	 * into the constructor for the superclass Model.java
	 */
	public CTIP() {
		super("ctip.xml");
	}

	/**
	 * Returns "CTIP Object" to describe, briefly, 
	 * what this Object is
	 */
	public String toString(){
		return "CTIP Object";
	}
	
	/**
	 * Reads in the values and information from the 
	 * input CTIP file
	 */
	public void read() {
		
	}
	
}
