/*
 * time_interpolator_test.c
 *
 *  Created on: Oct 09, 2012
 *      Author: dberrios
 */

#include <ccmc/wrappers/c/TimeInterpolator_c.h>
#include <stdio.h>
#include <string.h>

int main( int argc, char * argv[])
{
	/*if (argc != 2)
	{
		printf("Must enter 1 filename1\n");
		return 1;
	}*/

	printf("creating time interpolator object\n");
	int tid = TimeInterp_create();
	printf("created time interpolator object\n");

	const char * filename1 = "/Volumes/Drobo/CCMC_CCMC_052312_1_CDF/conversionResultsFiles/CCMC_CCMC_052312_1.swmf.i_e20061213-000500-000.cdf";
	const char * filename2 = "/Volumes/Drobo/CCMC_CCMC_052312_1_CDF/conversionResultsFiles/CCMC_CCMC_052312_1.swmf.i_e20061213-001000-000.cdf";

	double epoch = TimeInterp_TimeStrToEpoch("2006-12-13T00:10:00.000Z");


	TimeInterp_addTimestep(tid, filename1);
	TimeInterp_addTimestep(tid, filename2);

	const char * variables[] = {"ep", "theta", "phi"};
	TimeInterp_manageMem(tid, epoch, variables, 3);

	float value = TimeInterp_interpolate(tid, epoch, "ep", 0.0, 89.0, 12.0);
	printf("interpolated value: %g.\n", value);
	TimeInterp_closeAndCleanupMemory(tid);
	printf("deleted TimeInterpolator object\n");
	return 0;

}
