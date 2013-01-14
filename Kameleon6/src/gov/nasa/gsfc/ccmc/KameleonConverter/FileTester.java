/*
 * 
 * Name: FileTester.java
 * 
 * Version: 6.0
 * 
 * Author: Nitesh Donti
 * 		   NASA-GSFC-CCMC (Code 587)
 * 		   Intern
 * 
 * Purpose: Blank class (with main) used to test
 * 			and debug other parts of the software. 
 *  
 * 
 */
package gov.nasa.gsfc.ccmc.KameleonConverter;


import ncsa.hdf.object.*;
import ncsa.hdf.object.h5.*;

public class FileTester {

	//private static long[] dims2D = {20, 10};
	//private static String cname = "/Applications/TheHDFGroup/HDFView/samples/hdf5_testcopy.h5";
	private static String cname = "/Users/Nitesh/Desktop/ConversionTests/conversionResultsFiles/ionosphere.swmf.it_20001126_000000_0000.h5"; 
	

	/**
	 * @param args
	 * @throws Exception 
	 */
	public static void main(String[] args) throws Exception {


		System.out.println("*FILE TESTER*\n");

		// retrieve an instance of H5File
		FileFormat fileFormat = FileFormat.getFileFormat(FileFormat.FILE_TYPE_HDF5);
		H5File h = (H5File) fileFormat.createFile(cname,FileFormat.FILE_CREATE_OPEN);

		if (h.canRead()) 
		{
			System.out.println("Okay so the testfile "+cname+" is not null and can be read from.");
		}
		else 
		{
			System.out.print("bleh");
		}

		//datatypes are in HDF5Constants
		h.open();
		Group root = (Group)((javax.swing.tree.DefaultMutableTreeNode)h.getRootNode()).getUserObject();
		H5Group group = new H5Group(h, "Variables", "/", root);
		p("num of mems in file = " + group.getNumberOfMembersInFile());
		HObject var;

		for(int j = 0; j<group.getNumberOfMembersInFile(); j++){

			System.out.println("------Variable------");

			var= group.getMemberList().get(j);

			//----this prints out all of the attributes for one variable----
			Attribute attribute; 


			H5ScalarDS dataset = new H5ScalarDS(h,var.getName(), "/Variables/");
			dataset.open();


			for (int i =0; i<12; i++) {

				if(true){
					attribute = (Attribute)dataset.getMetadata().get(i);
					switch(attribute.getType().getDatatypeClass()) {
						case Datatype.CLASS_STRING:
							System.out.println( attribute.getName()+ ": " + ((String[])(attribute).getValue())[0]);
							break;
						case Datatype.CLASS_FLOAT:
							System.out.println(attribute.getName()+ ": " +((float[])(attribute).getValue())[0]);
							break;
						case Datatype.CLASS_INTEGER:
							System.out.println(attribute.getName()+ ": " + ((int[])(attribute).getValue())[0]);
							break;
					}
				}
			}
			//------end printing out attributes for variable-----

		}

		root.open();
	}



	static void p(Object o){
		System.out.println(o);
	}

}
