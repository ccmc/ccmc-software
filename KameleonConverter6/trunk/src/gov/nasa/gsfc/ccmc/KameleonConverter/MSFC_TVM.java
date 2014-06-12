/*
 * 
 * Name: MSFC_TVM.java
 * 
 * Version: 6.0
 * 
 * Author: Nitesh Donti
 * 		   NASA-GSFC-CCMC (Code 587)
 * 		   Intern
 * 
 * Purpose: Custom read routine for the MSFC TVM 
 * 			input model type
 *  
 * Modification History:
 *  
 * Summer 2011 	Donti, Nitesh
 * 				Initial Development Started
 * 
 * Tasks:	Read routine for this model type
 * 
 * Notes:   msfc_tvm.xml is in the directory entitled "Unused XML Files" 
 * 			because it has yet to be developed
 */

package gov.nasa.gsfc.ccmc.KameleonConverter;

public class MSFC_TVM extends Model {

	/**
	 * Constructor for the MSFC_TVM Object
	 * Passes in the default XML file for MSFC_TVM (msfc_tvm.xml)
	 * into the constructor for the superclass Model.java
	 */
	public MSFC_TVM() {
		super("msfc_tvm.xml");
	}

	/**
	 * Returns "MSFC_TVM Object" to describe, briefly, 
	 * what this Object is
	 */
	public String toString(){
		return "MSFC_TVM Object";
	}
	
	
	/**
	 * Reads in the values and information from the 
	 * input MSFC_TVM file
	 */
	public void read() {
		
	}
	
}
