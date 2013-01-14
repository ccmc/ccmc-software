/**
 * 
 */
package gov.nasa.gsfc.ccmc.KameleonConverter;

import java.io.File;
import java.io.FileNotFoundException;
import java.io.IOException;
import java.util.Scanner;

/**
 * @author ccmc
 *
 */
public class AMIE extends Model {

	/**
	 * @param xmlmodelfile
	 */
	public AMIE() {
		super("amie.xml");

	}

	public void read() throws FileNotFoundException, NoAttributeException, NoVariableException, IOException{
		super.read();
		parseFile();
	}

	public static void parseFile() throws FileNotFoundException{

		String FilePathname = "/Users/ccmc/Desktop/amie.2006.347-350.dat";

		Scanner eyes = new Scanner (new File(FilePathname));
		eyes.nextLine(); //the first line with titles

		String currenttime=null;
		String time="";
		String date="";
		StringBuilder nums = new StringBuilder("");

		int MINUTES_TO_PROCESS =1;
		for(int minutenumber=0;minutenumber<MINUTES_TO_PROCESS; minutenumber++){

			//--this gets all of the lines for one single minute--
			do{
				if(minutenumber>1){
					System.out.println(nums.toString());
				}
				nums= new StringBuilder("");
				//--parsing a line
				String nextline = eyes.nextLine();
				String[] nextlinetokens = nextline.split(" ",0);
				String[] datetime = new String[6];
				float[] nextlinenums = new float[5];
				for(int i=0,j=0; i<nextlinetokens.length; i++){
					if (nextlinetokens[i].length()>0){
						if (j<6){
							datetime[j++]=nextlinetokens[i];
						}
						else{
							nextlinenums[j-6]=Float.valueOf(nextlinetokens[i]);
							j++;
							nums.append(nextlinenums[j-7] + " ");
						}

					}

				}
				date = datetime[0] + "-" + datetime[1] + "-"+ datetime[2];
				time = datetime[3] + ":" + datetime[4] + ":" + datetime[5];
				if (currenttime==null)
					currenttime = time;
				nums.append(date +","+time);
				if (!currenttime.equals(time)){
					currenttime=time;
					break;
				}
				System.out.println(nums.toString());
				//--finished parsing the line	
			}while(time.equals(currenttime) && eyes.hasNextLine());
			//--ends all of the data for the current minute
			
			
			//get the longitude values so we can store unique values. 
			//this will make it easier to search in the interpolator.
			//float[] latitudes = new float[48];
			
			
			
			
		}
		

	}
	public String toString(){
		return "AMIE Object";
	}

}
