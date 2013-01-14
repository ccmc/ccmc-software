package gov.nasa.gsfc.ccmc.KameleonConverter;
import java.util.Scanner;


public class Parser1 {

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
				System.out.println("                              <attribute>");
				System.out.print("                                   <name>" +name+"</name>");
				}
			}
			
			String h2 =s.nextLine();
			
			if(h2.length()>3){
				int f2 =h2.indexOf("\"");
				int n2= h2.substring(f2+1).indexOf("\"");
				
				if(f2!=-1 && n2!=-1){
				String datatype = h2.substring(f2+1, f2+1+n2);
				System.out.println("\n                                   <dataType> " +datatype + " </dataType> ");
				
				}
			}
			
			System.out.println("                              </attribute>");
			
			
			
		}
	}

}
