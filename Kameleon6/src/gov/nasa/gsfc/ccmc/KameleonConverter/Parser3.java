package gov.nasa.gsfc.ccmc.KameleonConverter;
import java.util.Scanner;

//This parser is for turning the read_[model].h files into XML entries under the title [Model]attrs.xml. 
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
