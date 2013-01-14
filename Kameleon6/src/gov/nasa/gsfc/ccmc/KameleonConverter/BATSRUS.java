/*
 * 
 * Name: BATSRUS.java
 * 
 * Version: 6.0
 * 
 * Author: Nitesh Donti
 * 		   NASA-GSFC-CCMC (Code 587)
 * 		   Intern
 * 
 * Purpose: Custom read routine for the 
 * 			BATS-R-US input model file. 
 *  
 * Modification History:
 *  
 * Summer 2011 	Donti, Nitesh
 * 				Extended the Model superclass
 *  			  
 * Tasks: 	Need to create the read() method, etc. 
 * 
 * Notes:   batsrus.xml is in the directory entitled "Unused XML Files" 
 * 			because it has yet to be developed
 * 
 */


package gov.nasa.gsfc.ccmc.KameleonConverter;

import org.apache.log4j.Logger;

public class BATSRUS extends Model {

	/**
	 * Logger for the BATSRUS.java class. 
	 * Use Logger.info(), Logger.debug(), and Logger.error() to print
	 * statements out within the class. 
	 */
	static Logger logger = Logger.getLogger(BATSRUS.class);
	
	/**
	 * Constructor for the BATSRUS Object
	 * Passes in the default XML file for BATSRUS (batsrus.xml)
	 * into the constructor for the superclass Model.java
	 */
	public BATSRUS() {
		super("batsrus.xml");
	}

	/**
	 * Returns "BATSRUS Object" to describe, briefly, 
	 * what this Object is
	 */
	public String toString(){
		return "BATSRUS Object";
	}
	
	/**
	 * Reads in the values and information from the 
	 * input BATSRUS file
	 */
	public void read() {
		
	}
	
	
}
