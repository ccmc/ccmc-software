/*
 * 
 * Name: KPVT.java
 * 
 * Version: 6.0
 * 
 * Author: Nitesh Donti
 * 		   NASA-GSFC-CCMC (Code 587)
 * 		   Intern
 * 
 * Purpose: Custom read routine for the
 * 			KPVT input model file.
 * 	
 *  
 * Modification History:
 *  
 * Summer 2011 	Donti, Nitesh
 *  			Extension of the Model superclass  	
 *  			 
 * Tasks: 	Need to create the read() method, etc. 
 * 
 * Notes:   kpvt.xml is in the directory entitled "Unused XML Files" 
 * 			because it has yet to be developed
 * 
 */
package gov.nasa.gsfc.ccmc.KameleonConverter;

import org.apache.log4j.Logger;

public class KPVT extends Model {
	
	/**
	 * Logger for the KPVT.java class. 
	 * Use Logger.info(), Logger.debug(), and Logger.error() to print
	 * statements out within the class. 
	 */
	static Logger logger = Logger.getLogger(KPVT.class);
	

	/**
	 * Constructor for the KPVT Object
	 * Passes in the default XML file for KPVT (kpvt.xml)
	 * into the constructor for the superclass Model.java
	 */
	public KPVT() {

		super("kpvt.xml");

	}
	
	
	/**
	 * Reads in the values and information from the 
	 * input KPVT file
	 */
	public void read() {
		
	}
	
	
	/**
	 * Returns "KPVT Object" to describe, briefly, 
	 * what this Object is
	 */
	public String toString(){
		return "KPVT Object";
	}

}
