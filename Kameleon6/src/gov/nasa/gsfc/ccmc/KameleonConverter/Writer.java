package gov.nasa.gsfc.ccmc.KameleonConverter;

import java.util.ArrayList;

import org.apache.log4j.Logger;



public abstract class Writer {
	
	Logger logger = Logger.getLogger(Writer.class);
	
	public ArrayList<String> namesOfWrittenFiles = new ArrayList<String>();
	
	
	
	/**
	 * The name of the new CDF File that you are going to create. 
	 */
	String newfilename;
	

	
	public void convert (Model m) throws NoAttributeException{
		//just marking the beginning of the conversion
		logger.info("...now beginning conversion of "+m+" to a " +this);
		logger.info("...writing");
		
		if(m.VarObjsSize()==0){
			logger.error("***ERROR***There are no variables. \nEXITING...");
			System.exit(0);
		}

		
		
		newfilename= m.OutputDirectory+"/"+m.getGlobalAttribute("run_registration_number").value+".";
		newfilename+= m.getGlobalAttribute("model_name").value+"."+m.getTimestep()+this.extension();
		this.namesOfWrittenFiles.add(newfilename);
		
		
	}



	abstract protected String extension();
	
	
	
}
