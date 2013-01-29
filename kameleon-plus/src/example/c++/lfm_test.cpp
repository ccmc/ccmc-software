/*
 * LFM_test.cpp
 *
 *  Created on: Dec 5, 2012
 *      Author: Brian Curtis
 */

#include <boost/lexical_cast.hpp>
#include <iostream>
#include <ccmc/LFM.h>
#include <ccmc/Interpolator.h>
#include <string>

using namespace std;
using namespace ccmc;

/**
 * LFM <inputfile> variable x y z
 */
int main (int argc, char * argv[])
{
	if (argc != 6)
	{
		cout << "LFM <inputfile> variable x y z" << endl;
		exit(1);
	}

	string filename = argv[1];
	string variable = argv[2];
	float x = boost::lexical_cast<float>(argv[3]);
	float y = boost::lexical_cast<float>(argv[4]);
	float z = boost::lexical_cast<float>(argv[5]);
	LFM lfm;
	lfm.open(filename);
	lfm.loadVariable(variable);

	Interpolator * interpolator = lfm.createNewInterpolator();
	float value = interpolator->interpolate(variable, x, y, z);
	delete interpolator;

	string units = lfm.getNativeUnit(variable);

	cout << "interpolated value: " << value << " " << units << endl;
	lfm.close();

}

