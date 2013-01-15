package gov.nasa.gsfc.ccmc.KameleonConverter;

public class AMIEDataObject {

	private float[] data;
	
	public static int Longi = -1;
	public static int Lati = 0; 
	public static int Potential = 1; //potential (kV)
	public static int AvgEnergy = 2; //average energy (keV)
	public static int EFlux = 3; //e_flux (W/m^2)
	
	public AMIEDataObject(float a, float b, float c, float d, float e){
		float[] data1 = {a,b,c,d,e};
		this.data  = data1; 
	}
	
	public AMIEDataObject(float[] nums){
		this.data = nums;
	}
	
	float getVariable (int index){
		return data[index];
	}
}
