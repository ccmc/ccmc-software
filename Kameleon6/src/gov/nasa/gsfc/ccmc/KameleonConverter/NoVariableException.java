/*
 * 
 * Name: NoVariableException.java
 * 
 * Version: 6.0
 * 
 * Author: Nitesh Donti
 * 		   NASA-GSFC-CCMC (Code 587)
 * 		   Intern
 * 
 * Purpose: Exception is thrown when a Variable is requested 
 * 			but no such variable exists. 
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
 */
package gov.nasa.gsfc.ccmc.KameleonConverter;

public class NoVariableException extends Exception {

	/**
	 * 
	 */
	private static final long serialVersionUID = 1L;

	public NoVariableException() {
	}

	public NoVariableException(String arg0) {
		super(arg0);
	}

}
