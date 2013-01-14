/*
 * 
 * Name: Parser2.java
 * 
 * Version: 6.0
 * 
 * Author: Nitesh Donti
 * 		   NASA-GSFC-CCMC (Code 587)
 * 		   Intern
 * 
 * Purpose: *No Longer Necessary. Used Previously to Obtain Information.*
 * 			
 * 	
 * 			This parses a C file from the original 
 * 			Kameleon Converter and helps convert it
 * 			to a format that works well with the 
 * 			Variables.xml file. 
 * 
 * 			Copy everything from the C file and paste it into
 * 			the console. After conversion, copy the resulting 
 * 			text and paste into Variables.xml. 
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


public class Parser2 {

	/**
	 * @param args
	 */
	public static void main(String[] args) {
		
		Scanner s = new Scanner(System.in);

		while (s.hasNextLine()){

			String h = s.nextLine();
			String name=null;
			
			while(h.length()<3 || h.indexOf("\"")==-1)
				h= s.nextLine();
			
			if(h.length()>3){
				int f =h.indexOf("\"");
				int n= h.substring(f+1).indexOf("\"");
				
				if(f!=-1 && n!=-1){
				name = h.substring(f+1, f+1+n);
				System.out.println("\n<Variable>");
				System.out.println("   <category>" +"time-related"+"</category>");
				System.out.println("   <name>"+name+"</name>");
				}
			}
		
			
			String h2 =s.nextLine();
			
			if(h2.length()>3){
				int f2 =h2.indexOf("\"");
				int n2= h2.substring(f2+1).indexOf("\"");
				
				if(f2!=-1 && n2!=-1){
				String datatype = h2.substring(f2+1, f2+1+n2);
				System.out.println("   <dataType>" +datatype + "</dataType> ");
				
				}
			}
			
			
			h2 =s.nextLine();
			
			if(h2.length()>3){
				int f2 =h2.indexOf("\"");
				int n2= h2.substring(f2+1).indexOf("\"");
				
				if(f2!=-1 && n2!=-1){
				String classification = h2.substring(f2+1, f2+1+n2);
				System.out.println("   <classification>" +classification + "</classification> ");
				
				}
			}
			
			
			s.next();s.next();
			h2=s.next();
			String validMin=h2;
			System.out.println("   <valid_min>" +validMin + "</valid_min> ");

			s.next();s.next();
			h2=s.next();
			String validMax=h2;
			System.out.println("   <valid_max>" +validMax + "</valid_max> ");
			
			s.next();
			h2 =s.nextLine(); 
			
				
			if(h2.length()>3){
				int f2 =h2.indexOf("\"");
				int n2= h2.substring(f2+1).indexOf("\"");
				
				if(f2!=-1 && n2!=-1){
				String units = h2.substring(f2+1, f2+1+n2);
				System.out.println("   <units>" +units+ "</units> ");
				
				}
			}
			
			h2 =s.nextLine();
			
			if(h2.length()>3){
				int f2 =h2.indexOf("\"");
				int n2= h2.substring(f2+1).indexOf("\"");
				
				if(f2!=-1 && n2!=-1){
				String gridSystem = h2.substring(f2+1, f2+1+n2);
				System.out.println("   <grid_system>" +gridSystem + "</grid_system> ");
				
				}
			}
			
			s.next();s.next();
			h2 =s.next();
			String mask =h2;
			
				System.out.println("   <mask>" +mask+ "</mask> ");
				
			
			s.next();
			h2 =s.nextLine();
			
			if(h2.length()>3){
				int f2 =h2.indexOf("\"");
				int n2= h2.substring(f2+1).indexOf("\"");
				
				if(f2!=-1 && n2!=-1){
				String description = h2.substring(f2+1, f2+1+n2);
				System.out.println("   <description>" +description + "</description> ");
				
				}
			}
			
			
			s.next();s.next();
			h2=s.next();
			
			System.out.println("   <is_vector>" + (h2.equals("1")) + "</is_vector> ");
			
			
			
			s.next();
			h2 =s.nextLine();
			
			if(h2.length()>3){
				int f2 =h2.indexOf("\"");
				int n2= h2.substring(f2+1).indexOf("\"");
				
				if(f2!=-1 && n2!=-1){
				String posGridSys = h2.substring(f2+1, f2+1+n2);
				System.out.println("   <position_grid_system>" +posGridSys + "</position_grid_system> ");
				
				}
			}
			
			
			
			h2 =s.nextLine();

			
			if(h2.length()>3){
				int f2 =h2.indexOf("\"");
				int n2= h2.substring(f2+1).indexOf("\"");
				
				if(f2!=-1 && n2!=-1){
				String classification = h2.substring(f2+1, f2+1+n2);
				System.out.println("   <data_grid_system>" +classification + "</data_grid_system> ");
				
				}
			}
			
			System.out.println("</Variable>");
			
		}

	}

}
