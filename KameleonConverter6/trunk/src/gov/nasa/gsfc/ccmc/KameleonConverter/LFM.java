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

    public double[] WhatInCalendar(double ModifiedJulianDate)
    //    Programmed by Urs Zimmerli, Birkenhof, 8872 Weesen, Switzerland 
    //    mail:                       urs@zimmer.li                       
    {   /* convert the julian date back to a normal date*/
    
        double JD = ModifiedJulianDate + 2400000.5;     //convert to Julian     
        int JDO= (int)(JD+0.5);
        int C;
        if (JDO<2299161.0)    //check whether befor or after switch to gregorian...which introduced the whole mess
        {   C=(JDO+1524); 
        }
        else
        {   int B = (int)((JDO - 1867216.25)/36524.25);
            C = JDO + B - ((int)(B/4)) + 1525;
        }
        int D = (int)((C - 122.1)/365.25);
        int E = (365 * D) + ((int)(D/4));
        int F = (int)((C-E)/30.6001);
        int DAY = ((int)(C-E+0.5))-((int)(30.6001*F));
        int MONTH = F-1-(12*((int)(F/14)));
        int YEAR = D - 4715 - ((int)((7+MONTH)/10));
        double HOUR = 24.0*(JD+0.5-JDO);
        
        double[] TheDate = {YEAR,MONTH,DAY,HOUR}  ;
        return TheDate;
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

            // Add global attributes
            // read the attribute into memory
            
            System.out.println("Printing H4 root attributes...");
            try {
                Group groot = (Group) root.getUserObject();
                List<Attribute> attrList = groot.getMetadata();
                // get mjd value (double?)
                // convert mjd to y,m,d;h,m,s;
                for ( Attribute attr : attrList ){
                    if ((attr.toString()).equals("mjd")){
                        // System.out.println( attr.toString() ); //prints name of attribute
                        // System.out.println( attr.toString("  ") ); //prints value of attribute
                        double [] mjd = (double[])(attr.getValue());
                        System.out.println(mjd[0]);
                        double [] dateTime = WhatInCalendar (mjd[0]);
                        String year = String.format("%04d", (int) dateTime[0]);
                        String month = String.format("%02d", (int) dateTime[1]);
                        String day =  String.format("%02d", (int) dateTime[2]);
                        Double hour = dateTime[3]; //13.3891 hrs
                        Double minute = 60*(hour-Math.floor(hour)); //60*.3891 min
                        String hr = String.format("%02d",(int) Math.floor(hour));
                        Double second = 60*(minute - Math.floor(minute));
                        String min = String.format("%02d", (int) Math.floor(minute));
                        Double msecond = 1000*(second - Math.floor(second));
                        String sec = String.format("%02d", (int) Math.floor(second));
                        String msec = String.format("%03d",(int) Math.floor(msecond));
                        System.out.println(year+"-"+month+"-"+day+":"+hr+":"+min+":"+sec+"."+msec);
                        String date = year+"-"+month+"-"+day;
                        String time = hr+":"+min+":"+sec+"."+msec;
                        this.addGlobalAttribute(new KAttribute("timestep_time", date+"T"+time+"Z", "Current time", "model", "String"));
                    	}
                    // Extract and add other global attributes here
                    }
                } catch (Exception ex)
                {
                    System.out.println("Cannot access attributes with exception: "+ex);
                }


            while (nodes.hasMoreElements())
            {
                node = (DefaultMutableTreeNode)nodes.nextElement();
                Object obj = node.getUserObject();
                //System.out.println("obj: "+obj);
                if (orig2kamel.get(obj.toString()) != null){
                	//this.addVariableObject(new Variable(obj.toString(), orig2kamel.get(obj.toString()) ));
					tempV = new Variable(obj.toString(), orig2kamel.get(obj.toString()) );
                }
                else{
					//this.addVariableObject(new Variable(obj.toString(), "kameleon_identity_unknown_"+ii) );
                    logger.info("unknown kameleon variable: " + obj.toString());
					tempV = new Variable(obj.toString(), "kameleon_identity_unknown_"+ii );
					ii = ii +1;
				}
                // if(obj.toString().matches("old run name|old run info|old go namelist|old rings namelist|old swind namelist|old const namelist|old ion namelist|new run name|new run info|new go namelist|new rings namelist|new swind namelist|new const namelist|new const namelist|new ion namelist|run_params|variable list")){
                //     logger.info("detected non-variable string in node list " + obj.toString());                   
                // 	logger.info("adding " + obj.toString() + " as a global attribute");              	
                // } //if (obj string matches one from list)
                
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
                    // setting tempV.dt
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
                        System.out.println("\t" + obj.toString() + " has numDim = " + tempV.numDim);
                        int Prod = 1;
                        long[] AA = sds.getDims(); //array of var dimensions
                        int[] BB = new int[tempV.numDim]; //array matching var dimensions
                        for (int i=0; i<tempV.numDim; i++){
                        	BB[i] = (int) AA[i]; //cast dimensions into integer array
                        	System.out.println("\tBB["+i+"]: "+BB[i]);
                        	Prod = Prod * BB[i];
                        }
                        tempV.dimSizes = BB;
                        //tempV.numElem  = Prod;
                        System.out.println("\tdimSizes: "+Integer.toString(tempV.dimSizes[0]));
                        long[] selected = sds.getSelectedDims(); // the selected size of the dataset
                        if(tempV.numDim == 3){
                        	selected[2] = BB[2];
                        }
                        tempV.dataValues = sds.getData();
                        
                        if ((tempV.dt == "string") && (tempV.numDim == 1)){
                            this.addGlobalAttribute(new KAttribute(obj.toString().replaceAll(" ", "_"),((String[]) tempV.dataValues)[0], "", "model", "String"));
                        }
                        else {
                            tempV.numElem = Array.getLength(tempV.dataValues);
                            if (tempV.dt == "float") {
                            	this.minmaxCalculator(tempV, (float[])tempV.dataValues);
                            	logger.debug("\tActual Min: "+tempV.getAttributeObject("actual_min").value);
                            	logger.debug("\tActual Max: "+tempV.getAttributeObject("actual_max").value);
                            }
                            System.out.println("\tnumElem: "+tempV.numElem);
                            
                            this.addVariableObject(tempV);
                            getAttrValues(tempV);
                            //tempV.printAllAttrs();
                            logger.debug("\tVariable "+tempV.KameleonName+" Added to Kameleon");
                        }
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

