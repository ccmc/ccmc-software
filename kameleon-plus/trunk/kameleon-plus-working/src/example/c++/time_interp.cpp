/*
 * magnetogram_test.cpp
 *
 *  Created on: Sep 23, 2009
 *      Author: dberrios
 */

#include <boost/lexical_cast.hpp>
#include <ccmc/TimeInterpolator.h>
#include <ccmc/CCMCTime.h>
#include <iostream>
#include <string>
#include <vector>

using namespace std;

int main (int argc, char * argv[])
{
	if (argc != 5)
	{
		cout << "time_interp variable c0 c1 c2" << endl;
		exit(1);
	}

	//"timestep_time"       1:    CDF_CHAR     { "2006-12-13T00:05:00.000Z" } .
	std::string filename1 = "/Volumes/Drobo/CCMC_CCMC_052312_1_CDF/conversionResultsFiles/CCMC_CCMC_052312_1.swmf.i_e20061213-000500-000.cdf";

	//timestep_time"       1:    CDF_CHAR     { "2006-12-13T00:10:00.000Z" } .
	std::string filename2 = "/Volumes/Drobo/CCMC_CCMC_052312_1_CDF/conversionResultsFiles/CCMC_CCMC_052312_1.swmf.i_e20061213-001000-000.cdf";

	ccmc::Time time = ccmc::TimeInterpolator::parseTime("2006/12/13T00:10:00.000Z");
	std::cout << "Example Time: " << ccmc::TimeInterpolator::toString(time) << std::endl;
	ccmc::TimeInterpolator timeInterp;
	timeInterp.addTimestep(filename1);
	timeInterp.addTimestep(filename2);
	std::vector<std::string> variables;
	variables.push_back("ep");

	timeInterp.manageMemory(time, variables);

	float value = timeInterp.interpolate(time, "ep", 0.0, 89, 12);
	std::cout << "interpolated value: " << value << " at time: " << time << std::endl;

	//need to close and clean up

}
