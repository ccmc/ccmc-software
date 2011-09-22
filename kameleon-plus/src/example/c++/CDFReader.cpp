/*
 * HDF5Reader.cpp
 *
 *  Created on: Jul 22, 2011
 *      Author: dberrios
 */

#include <ccmc/CDFFileReader.h>
#include <vector>
#include <string>
#include <iostream>

int main (int argc, char * argv[])
{

	ccmc::CDFFileReader fileReader;
	fileReader.open(argv[1]);
	//std::vector<int> * intmat = fileReader.getVariableInt("intmat");
	//std::vector<float> * coord = fileReader.getVariable("coord");
	std::vector<float> * x = fileReader.getVariable("x");
	std::vector<float> * y = fileReader.getVariable("y");

	fileReader.close();



	return 0;
}
