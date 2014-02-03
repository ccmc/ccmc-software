/*
 * open_ggcm_test.cpp
 *
 *  Created on: Sep 23, 2009
 *      Author: dberrios
 */

#include <boost/lexical_cast.hpp>
#include <iostream>
#include <ccmc/MAS.h>
#include <ccmc/Interpolator.h>
#include <string>

using namespace std;

/**
 * enlil <inputfile> variable r theta(longitude) phi(latitude)
 */
int main (int argc, char * argv[])
{
	if (argc != 6)
	{
		cout << "mas <inputfile> variable r theta(latitude) phi(longitude)" << endl;
		exit(1);
	}

	string filename = argv[1];
	string variable = argv[2];
	float r = boost::lexical_cast<float>(argv[3]);
	float lat = boost::lexical_cast<float>(argv[4]);
	float lon = boost::lexical_cast<float>(argv[5]);
	ccmc::MAS mas;
	mas.open(filename);
	std::cerr << "opened file" << std::endl;
	mas.loadVariable(variable);

	ccmc::Interpolator * interpolator = mas.createNewInterpolator();
	float value = interpolator->interpolate(variable, r, lat, lon);
	delete interpolator;

	string units = mas.getNativeUnit(variable);

	cout << "interpolated value: " << value << " " << units << endl;
	mas.close();

}
