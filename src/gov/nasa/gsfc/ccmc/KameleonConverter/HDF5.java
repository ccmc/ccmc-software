/*
 * 
 * Name: HDF5.java
 * 
 * Version: 6.0
 * 
 * Author: Nitesh Donti
 * 		   NASA-GSFC-CCMC (Code 587)
 * 		   Intern
 * 
 * Purpose: Custom read routine for the 
 * 			HDF5 input model file. 
 *  
 * Modification History:
 *  
 * Dec 2011 	Donti, Nitesh
 * 				Initial Development Started
 * 				All tasks complete
 * 				Ready to use
 *  			  
 * Tasks: 	Figure out what needs to go in the XML document for HDF5. Then, make an "HDF5.xml" file for this class to use and access.
 *		 	Test with various files. 
 *       	Find the correct number of dimensions. 
 *		 	Find the correct values for each of the dimensions. 
 *			GET EVERYTHING WORKING FOR THE HDF5 FILES THAT COME FROM OPEN GGCM. Right now they work for regular datasets within folders. Make it work for datasets that are within folder that are in folders. 
 *
 *			Thanks, and good luck! It was great working on this!
 */

package gov.nasa.gsfc.ccmc.KameleonConverter;

import java.io.IOException;
import java.util.List;

import ncsa.hdf.object.*;
import ncsa.hdf.object.h5.*;


import org.apache.log4j.Logger;



public class HDF5 extends Model
{

	static Logger logger = Logger.getLogger(HDF5.class);
	static boolean variablesfolderexists = false; 

	HDF5()
	{
		super("swmf.xml");
	}



	public void read() throws NoAttributeException, NoVariableException, IOException{
		super.read();
		try
		{
			readfile();
		} catch (Exception e)
		{
			e.printStackTrace();
		} 
		System.out.println("\n\n\nThe information for your new java "+ this +" has just been read from the original file!");
	}

	public void readfile() throws Exception{
		String cname = FilePathname; 

		// retrieve an instance of H5File
		FileFormat fileFormat = FileFormat.getFileFormat(FileFormat.FILE_TYPE_HDF5);
		H5File h = (H5File) fileFormat.createFile(cname,FileFormat.FILE_CREATE_OPEN);

		if (!h.canRead()) 
		{
			logger.error("**ERROR**File ("+FilePathname+") cannot be read. EXITING...");
			System.exit(2);
		}

		//datatypes are in HDF5Constants
		try{
			h.open();
		}
		catch(Exception e){
			e.printStackTrace();
			logger.error("**ERROR**Unable to open file. EXITING..."); 
			System.exit(2);
		}
		Group root = (Group)((javax.swing.tree.DefaultMutableTreeNode)h.getRootNode()).getUserObject();


		//----getting global attributes-----
		List<?> globvars = root.getMetadata();
		Attribute globspec;

		for(int u=0; u<globvars.size(); u++){
			globspec = (Attribute)globvars.get(u);

			switch(globspec.getType().getDatatypeClass()){

				case Datatype.CLASS_STRING:
					this.addGlobalAttribute(new KAttribute(globspec.getName(),((String[])(globspec).getValue())[0],"","model","string"));
					break;
				case Datatype.CLASS_FLOAT:
					this.addGlobalAttribute(new KAttribute(globspec.getName(),((float[])(globspec).getValue())[0],"","model","float"));
					break;
				case Datatype.CLASS_INTEGER:
					this.addGlobalAttribute(new KAttribute(globspec.getName(),((int[])(globspec).getValue())[0],"","model","int"));
					break;

			}

		}


		for(int y=0; y<root.getNumberOfMembersInFile(); y++){

			H5Group group = new H5Group(h,root.getMemberList().get(y).getName(), "/", root);

			HObject var;

			int variable = 1;
			String varName; 
			Attribute attribute; 
			H5ScalarDS dataset;

			if(0<group.getNumberOfMembersInFile())
			{
				for(int j = 0; j<group.getNumberOfMembersInFile(); j++){
					//in here is where we want to check if the member in the Variables folder is itself a directory, or if it's a dataset. 

					var= group.getMemberList().get(j);
					varName = var.getName();
					System.out.println(var.getName());

					//if we know what to map the original name to, we will
					//if not, we will give a generic kameleon name
					if(orig2kamel.get(varName)!=null)
						this.addVariableObject(new Variable(varName,orig2kamel.get(varName)));
					else{
						this.addVariableObject(new Variable(varName,"kameleon_identity_unknown_"+variable++));
					}

					dataset = new H5ScalarDS(h,var.getName(), "/"+root.getMemberList().get(y).getName()+"/");
					dataset.open();

					try{

						System.out.println(dataset.getDatatype().getDatatypeDescription());


					}catch(NullPointerException e){
						H5Group subfolder = new H5Group(group.getFileFormat(), group.getMemberList().get(0).getName(), group.getPath(), group);
						
						System.out.println("Not a dataset.\n This is the dataset: "+subfolder.getName());
						System.exit(3);

						////CONTINUE WORK RIGHT HERE. WE WANT TO BE ABLE TO ACCESS THE DATASETS THAT ARE A COUPLE OF FOLDERS BELOW THE ROOT GROUP. IT'S DEFINITELY POSSIBLE.
					}

					//setting the number of dimensions
					this.getVariableObject(j).numDim = 1;// dataset.getRank();
					//THIS IS WRONG AND DOES NOT WORK PROPERLY

					//setting the data type
					this.getVariableObject(j).dt= dataset.getDatatype().getDatatypeDescription();

					//setting the actual dims
					int[] intversion = {1};
					this.getVariableObject(j).dimSizes = intversion; 
					//THIS IS WRONG AND DOES NOT WORK PROPERLY

					switch(dataset.getDatatype().getDatatypeClass()) {
						case Datatype.CLASS_STRING:
							//setting the data values
							String[] strvalues = (String[]) dataset.read();
							this.getVariableObject(j).dataValues=strvalues;

							//setting the number of elements
							this.getVariableObject(j).numElem = strvalues.length;

							break;
						case Datatype.CLASS_FLOAT:
							//setting the data values
							float[] floatvalues = (float[]) dataset.read();
							this.getVariableObject(j).dataValues=floatvalues;

							//setting the number of elements
							this.getVariableObject(j).numElem = floatvalues.length;

							break;
						case Datatype.CLASS_INTEGER:
							//setting the data values
							int[] intvalues = (int[]) dataset.read();
							this.getVariableObject(j).dataValues=intvalues;

							//setting the number of elements
							this.getVariableObject(j).numElem = intvalues.length;

							break;
						case Datatype.CLASS_CHAR:
							//setting the data values
							char[] charvalues = (char[]) dataset.read();
							this.getVariableObject(j).dataValues = charvalues;

							//setting the number of elements
							this.getVariableObject(j).numElem = charvalues.length;
							break;
					}


					//----this prints out all of the attributes for one variable---
					for (int i =0; i<dataset.getMetadata().size(); i++) {


						attribute = (Attribute)dataset.getMetadata().get(i);
						switch(attribute.getType().getDatatypeClass()) {
							case Datatype.CLASS_COMPOUND:
								System.out.print("It's a compound data array. That means that the datatypes of the contents are not all the same. This shouldn't really happen.");
								break; 
							case Datatype.CLASS_STRING:
								this.getVariableObject(getNumVariables()-1).addAttribute(new KAttribute(attribute.getName(), ((String[])(attribute).getValue())[0], "", "model", "String"));
								break;

							case Datatype.CLASS_FLOAT:
								this.getVariableObject(getNumVariables()-1).addAttribute(new KAttribute(attribute.getName(), ((float[])(attribute).getValue())[0], "", "model", "float"));
								break;

							case Datatype.CLASS_INTEGER:
								this.getVariableObject(getNumVariables()-1).addAttribute(new KAttribute(attribute.getName(), ((int[])(attribute).getValue())[0], "", "model", "int"));
								break;

						}
					}
					//------end printing out attributes for variable-----
				}
			}
		}
	}



	public String toString(){
		return "HDF5 Object";
	}

}

