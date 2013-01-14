/*
 * 
 * Name: NoAttributeException.java
 * 
 * Version: 6.0
 * 
 * Author: Nitesh Donti
 * 		   NASA-GSFC-CCMC (Code 587)
 * 		   Intern
 * 
 * Purpose: Exception is thrown when an Attribute is requested 
 * 			but no such attribute exists. 
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

@SuppressWarnings("serial")
public class NoAttributeException extends Exception {

	public NoAttributeException(String s){
		super(s);
	}
	
	public NoAttributeException(){}
	
}
