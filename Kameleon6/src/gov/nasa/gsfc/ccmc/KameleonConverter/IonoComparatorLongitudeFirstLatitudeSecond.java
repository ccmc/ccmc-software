package gov.nasa.gsfc.ccmc.KameleonConverter;

import java.util.Comparator;

public class IonoComparatorLongitudeFirstLatitudeSecond implements Comparator<SWMFIonoData>
{

	//sort by longitude first, then latitude. Also sort the latitude starting at 180 and going down to 0 (south to north)
	@Override
	public int compare(SWMFIonoData s1, SWMFIonoData s2)
	{
		if (s1.getVariable(SWMFIonoData.PSI) < s2.getVariable(SWMFIonoData.PSI))
			return -1;
		else if (s1.getVariable(SWMFIonoData.PSI) > s2.getVariable(SWMFIonoData.PSI))
			return 1;
		if (s1.getVariable(SWMFIonoData.THETA) > s2.getVariable(SWMFIonoData.THETA))
			return -1;
		else if (s1.getVariable(SWMFIonoData.THETA) < s2.getVariable(SWMFIonoData.THETA))
			return 1;
		else 
			return 0;
	}

}
