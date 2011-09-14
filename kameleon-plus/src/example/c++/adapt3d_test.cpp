/*
 * adapt3d_test.cpp
 *
 *  Created on: Sep 23, 2009
 *      Author: dberrios
 */

#include <boost/lexical_cast.hpp>
#include <iostream>
#include <ccmc/Adapt3D.h>
#include <ccmc/Interpolator.h>
#include <string>

using namespace std;

/**
 * adapt3d <inputfile> variable x y z
 */
int main (int argc, char * argv[])
{
	if (argc != 6)
	{
		cout << "adapt3d <inputfile> variable x y z" << endl;
		exit(1);
	}

	string filename = argv[1];
	string variable = argv[2];
	float x = boost::lexical_cast<float>(argv[3]);
	float y = boost::lexical_cast<float>(argv[4]);
	float z = boost::lexical_cast<float>(argv[5]);
	ccmc::Adapt3D adapt3d;
	adapt3d.open(filename);
	adapt3d.loadVariable(variable);

	ccmc::Interpolator * interpolator = adapt3d.createNewInterpolator();
	float value = interpolator->interpolate(variable, x, y, z);
	float density = interpolator->interpolate("rho", x, y, z);
	delete interpolator;

	string units = adapt3d.getNativeUnit(variable);

	cout << "interpolated value: " << value/density << " " << units << endl;
	adapt3d.close();
	return 0;

}
