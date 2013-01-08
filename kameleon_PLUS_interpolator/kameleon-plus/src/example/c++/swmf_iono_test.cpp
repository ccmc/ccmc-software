/*
 * magnetogram_test.cpp
 *
 *  Created on: Sep 23, 2009
 *      Author: dberrios
 */


#include <boost/lexical_cast.hpp>
#include <iostream>
#include <ccmc/SWMFIono.h>
#include <ccmc/Interpolator.h>
#include <string>
#include <math.h>

using namespace std;

/**
 * I have no idea what's contained in a magnetogram. Do they contain more than one variable?
 * magnetogram <inputfile> variable lat lon
 */
int main (int argc, char * argv[])
{
	if (argc != 5)
	{
		cout << "swmf_iono <inputfile> variable lat mlt" << endl;
		exit(1);
	}

	string filename = argv[1];
	string variable = argv[2];
	float lat = boost::lexical_cast<float>(argv[3]);
	float mlt = boost::lexical_cast<float>(argv[4]);
	ccmc::SWMFIono swmfIono;
	long status = swmfIono.open(filename);
	std::cout << "File opened with status: " << status << std::endl;
	swmfIono.loadVariable(variable);

	ccmc::Interpolator * interpolator = swmfIono.createNewInterpolator();

	//needs three position components. Just ignore the first one.
	float lon = mlt;//fmod((mlt * 15. + 180.), 360.);
	std::cout << "mlt: " << mlt << " lon: " << lon << std::endl;
	float value = interpolator->interpolate(variable, 0, lat, lon);
	delete interpolator;

	string units = swmfIono.getNativeUnit(variable);

	cout << "interpolated value: " << value << " " << units << endl;
	swmfIono.close();

}
