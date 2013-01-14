package gov.nasa.gsfc.ccmc.KameleonConverter;



public class SWMFIonoData
{
	private float[] data;
	
	public static int X = 0;
	public static int Y = 1;
	public static int Z = 2;
	public static int THETA = 3;
	public static int PSI = 4;
	public static int SIGMAH = 5;
	public static int SIGMAP = 6;
	public static int EFLUX = 7;
	public static int EAVE = 8;
	public static int JR = 9;
	public static int PHI_EP = 10;
	public static int EX = 11;
	public static int EY = 12;
	public static int EZ = 13;
	public static int JX = 14;
	public static int JY = 15;
	public static int JZ = 16;
	public static int UX = 17;
	public static int UY = 18;
	public static int UZ = 19;
	
//	private float x, y, z;
//	private float theta, psi;
//	private float sigmah;
//	private float sigmap;
//	private float eflux;
//	private float eave;
//	private float jr;
//	private float phiep;
//	private float ex, ey, ez;
//	private float jx, jy, jz;
//	private float ux, uy, uz;
	
	public SWMFIonoData(String line) throws NumberFormatException
	{
		
		data = new float[22];
		String tokens[] = line.split("\\s");
		//System.out.println("tokens: " + tokens.length);
		int count = -1;
		int index = 0;
		while (count < tokens.length){
			try
			{
				float nextFloat = Float.parseFloat(tokens[++count]);
			
				//System.out.println("tokens[" + index + "]: " + tokens[count]);
				
				data[index++]=nextFloat;
			} catch (Exception e)
			{
				//System.out.println("line: " + line);
				//e.printStackTrace();
				
			}
		}	
		if (index > 22)
		{
			System.out.println("> 22!: " + index);
			throw new NumberFormatException();
		}
		//System.exit(0);
		//System.out.println("count: " + index);
	}
	
	public float getVariable(int variable_id)
	{
		return data[variable_id];
	}

}
