package gov.nasa.gsfc.ccmc.KameleonConverter;

import java.util.Comparator;

public class IonoComparatorLatitudeFirstLongitudeSecond implements Comparator<SWMFIonoData>
{

	//sort by latitude first, then longitude
	@Override
	public int compare(SWMFIonoData s1, SWMFIonoData s2)
	{
		if (s1.getVariable(SWMFIonoData.THETA) < s2.getVariable(SWMFIonoData.THETA))
			return -1;
		else if (s1.getVariable(SWMFIonoData.THETA) > s2.getVariable(SWMFIonoData.THETA))
			return 1;
		else if (s1.getVariable(SWMFIonoData.PSI) < s2.getVariable(SWMFIonoData.PSI))
			return -1;
		else if (s1.getVariable(SWMFIonoData.PSI) > s2.getVariable(SWMFIonoData.PSI))
			return 1;
		else 
			return 0;
	}

}
