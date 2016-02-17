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
#include <boost/python.hpp>

using namespace std;
namespace bp = boost::python;
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
	float dc0, dc1, dc2;

	ccmc::PythonModel pymodel;
	pymodel.open(filename);
	
	if (pymodel.doesVariableExist(variable))
	{
		// no variable loading necessary
		ccmc::Interpolator * interpolator = pymodel.createNewInterpolator();
		float value = interpolator->interpolate(variable, p_component1, p_component2, p_component3, dc0, dc1, dc2);
		string units = pymodel.getNativeUnit(variable);
		cout << "interpolated value: " << value << " " << units << endl;
		cout << "resolution: " << dc0 << " "<< dc1 <<" " << dc2 << endl;
		delete interpolator;
	} else{
		cout << variable << " does not exist!"<<endl;
	}
	

	


}
