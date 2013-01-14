/*
 * 
 * Name: Parser3.java
 * 
 * Version: 6.0
 * 
 * Author: Nitesh Donti
 * 		   NASA-GSFC-CCMC (Code 587)
 * 		   Intern
 * 
 * Purpose: *No Longer Necessary. Used Previously to Obtain Information.*
 * 			 
 * 			//Deprecated
 * 			//[Model]attrs.xml NOT currently used
 * 
 * 			This parses a C file from the original 
 * 			Kameleon Converter and helps convert it
 * 			to a format that works well with some
 * 			XML files that include and Model name 
 * 			and "Attributes" in their titles. 
 * 
 *   		Copy everything from the C file and paste it into
 * 			the console. After conversion, copy the resulting 
 * 			text and paste into appropriate XML document.
 * 
 *  		So far, this parser has converted from 
 *  		read_[model].h files to [Model]attrs.xml. 
 * 
 * Modification History:
 *  
 * Summer 2011 	Donti, Nitesh
 * 				Parser Complete
 * 				Parser used to convert to XML
 * 
 * Tasks:
 * 
 */


package gov.nasa.gsfc.ccmc.KameleonConverter;
import java.util.Scanner;


public class Parser3 {

	public static void main(String[] args) {
		
		Scanner s = new Scanner(System.in);

		String category = "object";
		String type = s.next();
		String spec = "";
		
		
		String line=" ";
		
		while(s.hasNext()){
			
			while (line.length()<7 && line.indexOf('*')==-1)
				line = s.next();
			
			if(line.indexOf('*')!=-1)
				line=line.substring(1);
			line=line.substring(0,line.length()-1);
			
			System.out.println("<Variable type=\""+category+"\" specification=\""+spec+"\">");
			System.out.println("      <name>"+line+"</name>");
			System.out.println("      <datatype>"+type+"</datatype>");
			System.out.println("</Variable>");
			System.out.println("");
			
			line= s.next();
		
		}
		
		
		
		
	}
}
