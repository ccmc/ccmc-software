/*
 * open_ggcm_test.cpp
 *
 *  Created on: Sep 23, 2009
 *      Author: dberrios
 */

#include <boost/lexical_cast.hpp>
#include <iostream>
#include <ccmc/OpenGGCM.h>
#include <ccmc/Interpolator.h>
#include <string>

using namespace std;
using namespace ccmc;

/**
 * open_ggcm <inputfile> variable x y z
 */
int main (int argc, char * argv[])
{
	if (argc != 6)
	{
		cout << "open_ggcm <inputfile> variable x y z" << endl;
		exit(1);
	}

	string filename = argv[1];
	string variable = argv[2];
	float x = boost::lexical_cast<float>(argv[3]);
	float y = boost::lexical_cast<float>(argv[4]);
	float z = boost::lexical_cast<float>(argv[5]);
	OpenGGCM open_ggcm;
	open_ggcm.open(filename);
	open_ggcm.loadVariable(variable);

	Interpolator * interpolator = open_ggcm.createNewInterpolator();
	float value = interpolator->interpolate(variable, x, y, z);
	delete interpolator;

	string units = open_ggcm.getNativeUnit(variable);

	cout << "interpolated value: " << value << " " << units << endl;
	open_ggcm.close();

}
