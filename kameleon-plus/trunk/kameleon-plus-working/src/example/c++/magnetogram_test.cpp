/*
 * magnetogram_test.cpp
 *
 *  Created on: Sep 23, 2009
 *      Author: dberrios
 */


#include <boost/lexical_cast.hpp>
#include <iostream>
#include <ccmc/Magnetogram.h>
#include <ccmc/Interpolator.h>
#include <string>

using namespace std;

/**
 * I have no idea what's contained in a magnetogram. Do they contain more than one variable?
 * magnetogram <inputfile> variable lat lon
 */
int main (int argc, char * argv[])
{
	if (argc != 5)
	{
		cout << "magnetogram <inputfile> variable lat lon" << endl;
		exit(1);
	}

	string filename = argv[1];
	string variable = argv[2];
	float lat = boost::lexical_cast<float>(argv[3]);
	float lon = boost::lexical_cast<float>(argv[4]);
	ccmc::Magnetogram magnetogram;
	magnetogram.open(filename);
	magnetogram.loadVariable(variable);

	ccmc::Interpolator * interpolator = magnetogram.createNewInterpolator();

	//needs three position components. Just ignore the last one.
	float value = interpolator->interpolate(variable, lat, lon, 0);
	delete interpolator;

	string units = magnetogram.getNativeUnit(variable);

	cout << "interpolated value: " << value << " " << units << endl;
	magnetogram.close();

}
