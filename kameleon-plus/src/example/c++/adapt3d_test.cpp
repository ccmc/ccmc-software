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
#include <ctime>

/**
 * adapt3d <inputfile> variable x y z
 */
int main (int argc, char * argv[])
{
	if (argc != 6)
	{
		std::cout << "adapt3d <inputfile> variable x y z" << std::endl;
		exit(1);
	}
	ccmc::Adapt3D adapt3d;
	//adapt3d = new ccmc::Adapt3D();

	std::string filename = argv[1];
	std::string variable = argv[2];
	float x = boost::lexical_cast<float>(argv[3]);
	float y = boost::lexical_cast<float>(argv[4]);
	float z = boost::lexical_cast<float>(argv[5]);

	adapt3d.open(filename);
	adapt3d.loadVariable(variable);

	clock_t begin = clock();
	ccmc::Interpolator * interpolator = adapt3d.createNewInterpolator();
//	int upper = 5;
//	float delta = 10.f/(float)(upper -1);
//	for (int i = 0; i < upper; i++)
//	{
//		for (int j = 0; j < upper; j++)
//		{
//			for (int k = 0; k < upper; k++)
//			{
//				float x = -5.f+delta*(float)i;
//				float y = -5.f+delta*(float)j;
//				float z = -5.f+delta*(float)k;
//				//std::cout << "x: " << x << " y: " << y << " z: " << z << std::endl;
//				float value = interpolator->interpolate(variable, x,y,z);
//
//			}
//		}
//	}
	clock_t end = clock();
	float final = (end-begin)/CLOCKS_PER_SEC;
	std::cout << "elapsed time: " << final << " seconds" << std::endl;
	float value = interpolator->interpolate(variable, x, y, z);
	//float density = interpolator->interpolate("rho", x, y, z);
	delete interpolator;

	std::string units = adapt3d.getNativeUnit(variable);

	std::cout << "interpolated value: " << value << " " << units << std::endl;
	adapt3d.close();
	return 0;

}
