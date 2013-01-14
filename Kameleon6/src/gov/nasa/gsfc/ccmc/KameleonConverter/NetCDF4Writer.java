/*
 * 
 * Name: NetCDF4Writer.java
 * 
 * Version: 6.0
 * 
 * Author: Nitesh Donti
 * 		   NASA-GSFC-CCMC (Code 587)
 * 		   Intern
 * 
 * Purpose: Custom writer for the NetCDF4 Standardized
 * 			Output Data Format. 
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

import java.util.HashMap;

import org.apache.log4j.Logger;

import edu.ucar.ral.nujan.netcdf.NhDimension;
import edu.ucar.ral.nujan.netcdf.NhFileWriter;
import edu.ucar.ral.nujan.netcdf.NhGroup;
import edu.ucar.ral.nujan.netcdf.NhVariable;

public class NetCDF4Writer extends Writer {

	
	/**
	 * Logger for the NetCDF4Writer.java class. 
	 * Use Logger.info(), Logger.debug(), and Logger.error() to print
	 * statements out within the class. 
	 */
	static Logger logger = Logger.getLogger(NetCDF4Writer.class);
	
	/**
	 * Converts a Model Object (subclass Object) to NetCDF4 and creates the new file
	 * @param m The completed Model Object (subclass Object) to convert to NetCDF4
	 */
	public void convert(Model m){


		try{
			super.convert(m);

			NhFileWriter hfile = new NhFileWriter(newfilename,NhFileWriter.OPT_OVERWRITE);
			NhGroup rootGroup = hfile.getRootGroup();

			//------------------------------adding Variables and Variable Attributes----------------------------
			logger.info("Writing Variables...");
			NhGroup variablesGroup = rootGroup.addGroup("Variables");
			NhVariable [] vars = new NhVariable[m.getNumVariables()];
			Variable var; 
			int dtype=0;
			NhDimension[] nhDims = new NhDimension[1];
			HashMap<String, NhDimension> dimensions = new HashMap<String, NhDimension>();
			int[] chunkLengths = new int[1];

			//creating variables
			for (int i=0; i<vars.length; i++){
				var=m.getVariableObject(i);
				
				//checking to make sure all required information about the variable exists
				if(var.dataValues == null || var.dt == null || var.numDim == 0 || var.numElem == 0 || var.dimSizes == null){
					logger.error("**ERROR**Missing required information about Variable "+ var.originalName);
					logger.error("Cannot convert. \nEXITING...");
					System.exit(0);
				}

				String numElements = Integer.toString(var.numElem);
				if (dimensions.containsKey(numElements))
				{
					nhDims[0]= dimensions.get(numElements);
				} else
				{
					logger.error("numElements: " + numElements);
					nhDims[0] = rootGroup.addDimension("d" + numElements, var.numElem);
					dimensions.put(numElements, nhDims[0]);
				}
				//chunkLengths[0]=var.numElem;
				chunkLengths = null;
				//logger.debug("variable: " + var.KameleonName + " has " + var.numElem + " elements");

				if(var.dt.equalsIgnoreCase("float")){
					//logger.debug("dtype: float");
					dtype=NhVariable.TP_FLOAT;
				}
				else if(var.dt.equalsIgnoreCase("int")){
					//logger.debug("dtype: int");
					dtype=NhVariable.TP_INT;
				}
				else if(var.dt.equalsIgnoreCase("double")){
					//logger.debug("dtype: double");
					dtype=NhVariable.TP_DOUBLE;
				}
				else{
					logger.error("Trouble creating a variable object: "+ var.KameleonName);
					System.exit(0);
				}
				vars[i]=variablesGroup.addVariable(var.KameleonName, dtype, nhDims, chunkLengths, null, 0);

				KAttribute attr=null;
				//-------adding ModelSpecific Attributes to the new Variable------
				//-------adding CCMC Attributes to the new Variable (does NOT overwrite any repeats)------
				for(int y= 0; y<var.getModelSpecAttributeNames().length + var.getCCMCAttributeNames().length; y++){
					try {
						if(y<var.getModelSpecAttributeNames().length)
							attr=var.getAttributeObject(var.getModelSpecAttributeNames()[y]);
						else
							attr=var.getAttributeObject(var.getCCMCAttributeNames()[y-var.getModelSpecAttributeNames().length]);
						
						//if the attribute does not already exist
						if(!vars[i].attributeExists(attr.name)){
							
							String type = attr.getDataType().toString().toLowerCase();
							
							if(type.indexOf("flo")!=-1){
								dtype=NhVariable.TP_FLOAT;
								vars[i].addAttribute(attr.name, dtype,(Float)attr.value);
							}
							else if(type.indexOf("str")!=-1){
								dtype=NhVariable.TP_STRING_VAR;
								vars[i].addAttribute(attr.name, dtype, (String) attr.value);
							}
							else  if(type.indexOf("double")!=-1){
								dtype=NhVariable.TP_DOUBLE;
								vars[i].addAttribute(attr.name, dtype, ((Double) attr.value));
							}
							else if(type.indexOf("int")!=-1){
								dtype=NhVariable.TP_INT;
								vars[i].addAttribute(attr.name, dtype,((Integer)attr.value));
							}
							else
								logger.error("Data Type of Attribute Unknown.");

						}else{
							if(CommandLineInterface.verboseFlag)
								logger.debug("'Twas a duplicate and not added:"+attr.name+" "+ var.KameleonName);
						}

					} catch(ClassCastException cc){
						//this is a workaround for a class cast problem
						dtype=NhVariable.TP_STRING_VAR;
						vars[i].addAttribute(attr.name, dtype, (String) attr.value);
					}
				}
			}
			//-----------------end adding Variables and Variable Attributes---------------


			

			//------------------------adding Global Attributes---------------------
			logger.info("Writing Global Attributes...");
			dtype=0;

			KAttribute ga;
			for(int i=0; i<m.getGlobAttrsSize();i++){
				
				ga=m.getGlobalAttribute(i);
				String type= ga.getDataType().toString().toLowerCase();
				
				if(type.indexOf("flo")!=-1){
					dtype=NhVariable.TP_FLOAT;
					rootGroup.addAttribute(ga.name, dtype, (Float) ga.value);
				}
				else if(type.indexOf("int")!=-1){
					dtype=NhVariable.TP_INT;
					rootGroup.addAttribute(ga.name, dtype, (Integer) ga.value);
				}
				else if(type.indexOf("long")!=-1){
					dtype=NhVariable.TP_LONG;
					rootGroup.addAttribute(ga.name, dtype, (Long)ga.value);
				}
				else if(type.indexOf("double")!=-1){
					dtype=NhVariable.TP_DOUBLE;
					rootGroup.addAttribute(ga.name, dtype, (Double) ga.value);
				}else if(type.indexOf("str")!=-1){
					dtype=NhVariable.TP_STRING_VAR;
					String value = (String)ga.value;
					if (value == null)
						value = "";
					rootGroup.addAttribute(ga.name, dtype, value);
				}



			}

			//-------------------end adding Global Attributes----------------------

			hfile.endDefine();
			
			//------------writing Data to Variables-----------
			logger.info("Writing data to Variables...");
			for(int j=0; j<m.getNumVariables(); j++){
				var=m.getVariableObject(j);

				if(var.dt.equalsIgnoreCase("float"))
				{
					vars[j].writeData(null,(float[])var.dataValues, false);
				}
				else if (var.dt.equalsIgnoreCase("double"))
				{
					vars[j].writeData(null,(double[])var.dataValues, false);
				}
				else if (var.dt.equalsIgnoreCase("int"))
					vars[j].writeData(null,(int[])var.dataValues, false);
				else{
					logger.error("Unsupported data type of variable data values.");
					System.exit(0);
				}	
			}
			//-----------end writing Data-------------------
			hfile.close();

		}catch (Exception e){
			e.printStackTrace();
		}

	}

	/**
	 * @return The type of file this class will create, in String form ("NetCDF4 File")
	 */
	public String toString(){
		return "NetCDF4 File";
	}

	/**
	 * @return the string extension of the new file that will be created. [".nc"]
	 */
	public String extension(){
		return ".nc";
	}

}
