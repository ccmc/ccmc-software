/*
 * 
 * Name: HDF.java
 * 
 * Version: 1.0
 * 
 * Author: Brian Curtis
 * 		   NASA-GSFC-CCMC (Code 674)
 * 		   Science Collaborator - George Mason University
 * 
 * Purpose: Custom read routine for the 
 * 			HDF input model file. 
 *  
 * Modification History:
 *  
 * Oct 2012 	Curtis, Brian
 * 				Initial Development Started
 * 				All tasks complete
 * 				Ready to use
 *  			  
 * Tasks: 	Figure out what needs to go in the XML document for HDF. Then, make an "HDF.xml" file for this class to use and access.
 *		 	Test with various files. 
 *       	Find the correct number of dimensions. 
 *		 	Find the correct values for each of the dimensions. 
 *			GET EVERYTHING WORKING FOR THE HDF FILES THAT COME FROM LFM. Right now they work for regular datasets within folders. Make it work for datasets that are within folder that are in folders. 
 *
 *			Thanks, and good luck! It was great working on this!
 */

package gov.nasa.gsfc.ccmc.KameleonConverter;

import java.io.IOException;
import java.util.Enumeration;
import java.util.List;
import java.lang.reflect.Array;
import java.lang.Class.*;

import javax.swing.tree.*;

import ncsa.hdf.hdflib.HDFConstants;
import ncsa.hdf.object.*;
import ncsa.hdf.object.h4.*;

import org.apache.log4j.Logger;



public class LFM extends Model
{

	static Logger logger = Logger.getLogger(LFM.class);
	static boolean variablesfolderexists = false;

	public LFM() {
		super("LFM.xml");
	}

	public void read() throws NoAttributeException, NoVariableException, IOException {
		super.read();
		try {
			readfile();
		} catch (Exception e) {
			e.printStackTrace();
		} 
		System.out.println("\n\n\nThe information for your new java "+ this +" has just been read from the original file!");
	}

	public void readfile() throws Exception{
		
		H4File h4file = new H4File(FilePathname, FileFormat.READ);
		
        try {
            h4file.open();
        } catch (Exception ex)
        {
            System.out.println(ex);
        }
        System.out.println("Opened Successfully");

        DefaultMutableTreeNode root = (DefaultMutableTreeNode)h4file.getRootNode();
        H4SDS sds = null;
        DefaultMutableTreeNode node = null;
        Variable tempV = null;
        if (root != null)
        {
            Enumeration nodes = root.depthFirstEnumeration();
            int ii = 1;
            while (nodes.hasMoreElements())
            {
                node = (DefaultMutableTreeNode)nodes.nextElement();
                Object obj = node.getUserObject();
                //System.out.println("obj: "+obj);
                if (orig2kamel.get(obj.toString()) != null){
                	//this.addVariableObject(new Variable(obj.toString(), orig2kamel.get(obj.toString()) ));
					tempV = new Variable(obj.toString(), orig2kamel.get(obj.toString()) );
                }
				else {
					//this.addVariableObject(new Variable(obj.toString(), "kameleon_identity_unknown_"+ii) );
					tempV = new Variable(obj.toString(), "kameleon_identity_unknown_"+ii );
					ii = ii +1;
				}
                logger.debug(tempV.originalName + " = " + tempV.KameleonName);
                if (obj instanceof H4SDS)
                {
                    sds = (H4SDS)obj;

                    // test H4CompoundDS attributes
                    Attribute attr = null;
                    List info = null;
                    try {
                        info = sds.getMetadata();
                        //System.out.println("ATTR info: "+info);
                    } catch (Exception ex)
                    { System.out.println(ex); }

                    
                    
                    
                    int n = 0;
                    if (info != null)
                    {
                        n = info.size();
                        for (int i=0; i<n; i++)
                        {
                            attr = (Attribute)info.get(i);
                        }
                    }

                    // data
                    Object data = null;
                    try
                    {
                    	data = sds.read();
                    } catch (Exception ex) {System.out.println(ex);}
                    if ((data != null) && data.getClass().isArray())
                    {
                        // setting tempV.dt
                        if(sds.getDatatype().getDatatypeDescription() == "8-bit character"){
                        	tempV.dt = "string";
                        }
                        else if(sds.getDatatype().getDatatypeDescription() == "32-bit floating-point"){
                        	tempV.dt = "float";
                        }
                        tempV.numDim = sds.getDims().length;
                        System.out.println("numDim "+tempV.numDim);
                        int Prod = 1;
                        long[] AA = sds.getDims();
                        int[] BB = new int[tempV.numDim];
                        for (int i=0; i<tempV.numDim; i++){
                        	BB[i] = (int) AA[i];
                        	System.out.println("BB["+i+"]: "+BB[i]);
                        	Prod = Prod * BB[i];
                        }
                        tempV.dimSizes = BB;
                        //tempV.numElem  = Prod;
                        System.out.println("dimSizes: "+tempV.dimSizes);
                        long[] selected = sds.getSelectedDims(); // the selected size of the dataet
                        if(tempV.numDim == 3){
                        	selected[2] = BB[2];
                        }
                        tempV.dataValues = sds.getData();
                        tempV.numElem = Array.getLength(tempV.dataValues);
                        System.out.println("numElem: "+tempV.numElem);
                        
                        this.addVariableObject(tempV);
                        getAttrValues(tempV);
                        //tempV.printAllAttrs();
                        logger.debug("Variable "+tempV.KameleonName+" Added to Kameleon");
                    }
                } //if (obj instanceof H4Group
            }//while (nodes.hasMoreElements())
        } //if (root != null)
        else {
        	System.out.println("ROOT WAS NULL");
        }

	try {
        h4file.close();
        } catch (Exception ex)
        {
            System.out.println("Cannot close file with exception: "+ex);
        }
		
}



public String toString(){
		return "HDF Object";
	}

}

