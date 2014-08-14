/*
 * 
 * Name: CDFWriter.java
 * 
 * Version: 6.0
 * 
 * Author: Nitesh Donti
 * 		   NASA-GSFC-CCMC (Code 587)
 * 		   Intern
 * 
 * Purpose: Custom write routine for the CDF
 * 			standardized output format. 
 * 	
 *  
 * Modification History:
 *  
 * Summer 2011 	Donti, Nitesh
 * 				Initial Development Started
 * 				All tasks complete
 * 				Ready to use
 *  			  
 *  			 
 * 
 */


package gov.nasa.gsfc.ccmc.KameleonConverter;

import gsfc.nssdc.cdf.Attribute;
import gsfc.nssdc.cdf.CDF;
import gsfc.nssdc.cdf.CDFConstants;
import gsfc.nssdc.cdf.CDFException;
import gsfc.nssdc.cdf.Entry;

import java.io.File;

import org.apache.log4j.Logger;

public class CDFWriter extends Writer {

	/**
	 * Logger for the CDFWriter.java class. 
	 * Use Logger.info(), Logger.debug(), and Logger.error() to print
	 * statements out within the class. 
	 */
	static Logger logger = Logger.getLogger(CDFWriter.class);


	/**
	 * cdfFile that holds the CDF that we are creating
	 * 
	 * We can also specify the filename that we want the final, converted CDF to have. 
	 * 
	 * For testing purposes, we delete this file initially (if it already exists) so that we 
	 * can run the program and create the file again. 
	 */
	private File cdfFile;

	/**
	 * An array of Attribute objects. Note that this Attribute class comes from the gsfc.nssdc.cdf package, 
	 * and is NOT the same thing as the Kameleon software's KAttribute object. 
	 * 
	 * This array will store all of the model's global attributes after reading the information 
	 * from the model object. 
	 */
	private Attribute[] globalCDFattrs;

	/**
	 * An array of Attribute objects. Note that this Attribute class comes from the gsfc.nssdc.cdf package, 
	 * and is NOT the same thing as the Kameleon software's KAttribute object. 
	 * 
	 * This array will store all of the model's CCMC standard variable attributes that each of 
	 * the model's Variable objects will contain.  Additionally, it stores the model's Model-Specific
	 * variable attributes as well. 
	 * 
	 * This array is rewritten each time a new Variable is created. 
	 */
	private Attribute[] variableCDFattrs;
	
	/**
	 * The number of Attributes that a Variable (from the gsfc.nssdc.cdf package) contains. 
	 * This number is updated every time a new Variable is read in. 
	 * This number decrements when there are multiple variables with the same name. 
	 */
	private int variableCDFattrs_size; 

	/**
	 * An array of Variable objects. Note that this Variable class comes from the gsfc.nssdc.cdf package, 
	 * and is NOT the same thing as the Kameleon software's VariableObj class. 
	 * 
	 * This array will store all of the model's variables.
	 * Then, each variable, which takes on all of the contents of Attribute[] variableCDFattrs, will have
	 * its variable attributes filled to make the variables complete. 
	 */
	private gsfc.nssdc.cdf.Variable[] CDFvariables;

	/**
	 * Converts a Model Object (subclass Object) to CDF and creates the new file
	 * @param m The completed Model Object (subclass Object) to convert to CDF
	 */
	public void convert (Model m) throws NoAttributeException{
		super.convert(m);

		cdfFile = new File(newfilename);
		if (cdfFile.exists()) 
			cdfFile.delete();

		globalCDFattrs = new Attribute[m.getGlobAttrsSize()];
		variableCDFattrs = new Attribute[m.getVariableObject(0).getCCMCAttributeNames().length + m.getVariableObject(0).msaal.size()];
		variableCDFattrs_size=variableCDFattrs.length;
		CDFvariables = new gsfc.nssdc.cdf.Variable[m.getNumVariables()];

		CDF cdf = null;

		try {
			//creates the new CDF file and sets some settings
			logger.info("creating file. " + newfilename);
			cdf = CDF.create(newfilename);
			cdf.setChecksum(CDFConstants.MD5_CHECKSUM);
			cdf.setFormat(CDFConstants.SINGLE_FILE);

			//creating the Global Attributes
			logger.info("creating Global Attributes");
			for(int i=0; i<m.getGlobAttrsSize(); i++)
			{
				if(CommandLineInterface.verboseFlag)
					logger.debug("\n"+m.getGlobalAttribute(i).name);
				try {
					if(m.getGlobalAttribute(i).value!=null || m.getGlobalAttribute(i).value==""){
						//creating the gsfc.nssdc.cdf.Attribute objects with GLOBAL scope
						globalCDFattrs[i]=Attribute.create(cdf,m.getGlobalAttribute(i).name, CDFConstants.GLOBAL_SCOPE);
						String dType = m.getGlobalAttribute(i).getDataType().toString();
						//making and assigning ENTRIES, each of which STORES the data/VALUE of one GLOBAL attribute
						if(dType.equalsIgnoreCase("double"))
							Entry.create(globalCDFattrs[i], 0, CDFConstants.CDF_DOUBLE, m.getGlobalAttribute(i).value);
						else if(dType.equalsIgnoreCase("string"))
							Entry.create(globalCDFattrs[i], 0, CDFConstants.CDF_CHAR, m.getGlobalAttribute(i).value.toString());
						else if(dType.equalsIgnoreCase("float"))
							Entry.create(globalCDFattrs[i], 0, CDFConstants.CDF_FLOAT, m.getGlobalAttribute(i).value);
						else if(dType.equalsIgnoreCase("boolean"))
							Entry.create(globalCDFattrs[i], 0, CDFConstants.CDF_BYTE, m.getGlobalAttribute(i).value);
						else if(dType.equalsIgnoreCase("int"))
							Entry.create(globalCDFattrs[i], 0, CDFConstants.CDF_INT4, m.getGlobalAttribute(i).value);
						else 
							logger.error("***ERROR***Data type of global attribute "+ m.getGlobalAttribute(i).name +" is unknown.");
					}else if(CommandLineInterface.verboseFlag)
						logger.debug(" // null");

				} catch (NullPointerException e){
					if(CommandLineInterface.verboseFlag)
						logger.debug(" // null");
				} catch (CDFException ce){
					if(CommandLineInterface.verboseFlag)
						logger.debug(" //CDFWriter92 error.");
				}
			}

			//Creating Variable Attribute CONTAINERS and storing them in an array to be used momentarily (NO VALUES YET, JUST NAMES)
			//creating the gsfc.nssdc.cdf.Attribute objects with VARIABLE scope (only the CCMC ones)
			//based on the CCMC variable attributes of this model's first recorded variable
			logger.info("Creating Variable Attribute objects with VARIABLE scope (CCMC only)");
			for(int i=0; i<m.getVariableObject(0).getCCMCAttributeNames().length; i++){
				try {
					variableCDFattrs[i]=Attribute.create(cdf,m.getVariableObject(0).getCCMCAttributeNames()[i], CDFConstants.VARIABLE_SCOPE);
				} catch (CDFException e) { 
					logger.error(e.getMessage() + m.getVariableObject(0).msaal.get(i).name);
				}
			}

			//Creating Variable Attribute containers and storing them in an array to be used momentarily (no values yet, just names)
			//creating the gsfc.nssdc.cdf.Attribute objects with VARIABLE scope (only the Model-Specific ones)
			//based on the CCMC variable attributes of this model's first recorded variable
			logger.info("Creating Variable Attribute objects with VARIABLE scope (Model-Specific only)");
			for(int i=0, j=0; i<m.getVariableObject(0).msaal.size(); i++, j++){
				try {
					variableCDFattrs[j+m.getVariableObject(0).getCCMCAttributeNames().length]=Attribute.create(cdf,m.getVariableObject(0).msaal.get(i).name, CDFConstants.VARIABLE_SCOPE);
				} catch (CDFException e) {
					if(e.getMessage().indexOf("Named attribute already exists")!=-1){
						variableCDFattrs_size--;
						j--;
					}
					else
						logger.error(e.getMessage() + m.getVariableObject(0).msaal.get(i).name);
				}
			}

			
			long cdfconstant = CDFConstants.CDF_CHAR;

			//Creating the gsfc.nssdc.cdf.Variable objects
			logger.info("Creating the gsfc.nssdc.cdf.Variable objects");
			for(int i=0; i<m.getNumVariables(); i++){
				gov.nasa.gsfc.ccmc.KameleonConverter.Variable var = m.getVariableObject(i);
				
				//checking to make sure all required information about the variable exists
				if(var.dataValues == null || var.dt == null || var.numDim == 0 || var.numElem == 0 || var.dimSizes == null){
					logger.error("**ERROR**Missing required information about Variable "+ var.originalName);
					logger.error("Cannot convert. \nEXITING...");
					System.exit(0);
				}
				
				
				long varDataType = 0;
				
				if(var.dt.equalsIgnoreCase("float"))
					varDataType=CDFConstants.CDF_FLOAT;
				else if (var.dt.equalsIgnoreCase("int"))
					varDataType=CDFConstants.CDF_INT4;
				else{
					logger.error("***ERROR***Unsupported Data Type " + var.dt + " for Variable " + var.originalName + " \nEXITING...");
					System.exit(0);
				}
				
				try{
				logger.info("exporting kameleon variable:" + var.KameleonName);
				CDFvariables[i]=gsfc.nssdc.cdf.Variable.create(cdf, var.KameleonName, varDataType, 1, 1, new long [] {var.numElem}, CDFConstants.VARY, new long [] {CDFConstants.VARY});
				}
				catch (CDFException c){
					
					c.printStackTrace();
				}
				//parsing all variable attributes so that we can assign them values and give them to the Variable object
				for(int j=0; j<variableCDFattrs_size; j++){
					try {

						//finds the data type of the variable attribute if the variable attribute actually has a value
						String datatype = m.getVariableObject(i).getAttributeObject(variableCDFattrs[j].getName()).getDataType().toString();

						if(datatype.equalsIgnoreCase("string"))
							cdfconstant=CDFConstants.CDF_CHAR;
						else if (datatype.equalsIgnoreCase("double"))
						{
							cdfconstant=CDFConstants.CDF_DOUBLE;
							var.getAttributeObject(variableCDFattrs[j].getName()).value = Double.parseDouble(var.getAttributeObject(variableCDFattrs[j].getName()).value.toString());
						}
						else if (datatype.equalsIgnoreCase("int"))
						{
							cdfconstant=CDFConstants.CDF_INT4;
							var.getAttributeObject(variableCDFattrs[j].getName()).value = Integer.parseInt(var.getAttributeObject(variableCDFattrs[j].getName()).value.toString());
						}
						else if (datatype.equalsIgnoreCase("float"))
						{
							cdfconstant=CDFConstants.CDF_FLOAT;
							var.getAttributeObject(variableCDFattrs[j].getName()).value = Float.parseFloat(var.getAttributeObject(variableCDFattrs[j].getName()).value.toString());
						}
						else if (datatype.equalsIgnoreCase("boolean"))
							cdfconstant=CDFConstants.CDF_BYTE;
						else 
							logger.error("***ERROR***Data type of variable attribute "+variableCDFattrs[j].getName() +" is unknown.");

						//making and assigning ENTRIES, each of which STORES a PIECE OF DATA of one VARIABLE attribute and belongs to one variable object
						CDFvariables[i].putEntry(variableCDFattrs[j], cdfconstant, var.getAttributeObject(variableCDFattrs[j].getName()).value);
						if(CommandLineInterface.verboseFlag)
							logger.debug("attribute: " + variableCDFattrs[j].getName() + " value: " + var.getAttributeObject(variableCDFattrs[j].getName()).value);
					} catch(NullPointerException e){
						e.printStackTrace();
						//CDFvariables[i].putEntry(variableCDFattrs[j], cdfconstant, "*no value*");
					} catch(NoAttributeException e){
						logger.debug("//Some Attributes Missing for Variable "+m.getVariableObject(i).KameleonName);
					} catch (Exception e) {
						logger.error("//some error ");
						e.printStackTrace();
					} 
				}

				if(CommandLineInterface.verboseFlag)
					logger.info("Beginning to put data for Variable# "+ (i+1));

				if(var.dt.equalsIgnoreCase("int")){

					if(CommandLineInterface.verboseFlag)
						for( int x=0; x<var.numElem;x+=1000000)
						{
							logger.debug("Array[" + x + "] = " + ((int[])(var.dataValues))[x]  );
						}
					//This is where we put all of the data into the final CDF variable. 
					CDFvariables[i].putHyperData(0L, 1L, 1L, new long[]{0}, new long [] {var.numElem,1}, new long[]{1}, (int[]) var.dataValues);
				}

				else if(var.dt.equalsIgnoreCase("float")){

					if(CommandLineInterface.verboseFlag)
						for( int x=0; x<var.numElem;x+=1000000)
						{
							logger.debug("Array[" + x + "] = " + ((float[])(var.dataValues))[x]  );
						}
					//This is where we put all of the data into the final CDF variable. 
					CDFvariables[i].putHyperData(0L, 1L, 1L, new long[]{0}, new long [] {var.numElem,1}, new long[]{1}, (float[]) var.dataValues);
				}
				else 
					throw new CDFException("The values of the data in this variable's array are neither ints nor floats. No other datatypes are currently accepted.");

				logger.info("Entered data values for Variable# " + (i+1));
			}	
			cdf.save(); 
			cdf.close();

		} catch (CDFException e) {
			e.printStackTrace();
			long status = cdf.getStatus();
			logger.error("CDFException: " + CDF.getStatusText(status));
		}
	}
	
	/**
	 * @return The type of file this class will create, in String form ("CDF File")
	 */
	public String toString(){
		return "CDF File";
	}

	/**
	 * @return the string extension of the new file that will be created. [".cdf"]
	 */
	public String extension(){
		return ".cdf";
	}
}