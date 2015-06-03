/*
 * pyModel_prog.cpp
 *
 *  Created on: Jun 2, 2015
 *      Author: Asher Pembroke
 *
 *  This program illustrates use of python model embedding
 */

#include <boost/lexical_cast.hpp>
#include <iostream>
#include <ccmc/PythonModel.h>
#include <ccmc/PythonInterpolator.h>
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
		cout << "pyModel_prog <config_file> variable p_component1 p_component2 p_component3" << endl;
		exit(1);
	}

	string filename = argv[1];
	string variable = argv[2];
	float p_component1 = boost::lexical_cast<float>(argv[3]);
	float p_component2 = boost::lexical_cast<float>(argv[4]);
	float p_component3 = boost::lexical_cast<float>(argv[5]);


	// std::cerr << "creating PythonModel instance" << std::endl;
	ccmc::PythonModel pymodel;
	pymodel.open(filename);
	// std::cerr << "opened file?" << std::endl;
	
	ccmc::Interpolator * interpolator = pymodel.createNewInterpolator();

	// std::cerr << "created interpolator" << std::endl;

	float value = interpolator->interpolate(variable, p_component1, p_component2, p_component3);

	delete interpolator;

	string units = pymodel.getNativeUnit(variable);

	cout << "interpolated value: " << value << " " << units << endl;
	// pymodel.close();

}
