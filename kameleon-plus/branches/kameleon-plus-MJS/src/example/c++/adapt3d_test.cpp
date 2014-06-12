/*
 * adapt3d_test.cpp
 *
 *  Created on: Sep 23, 2009
 *      Author: dberrios
 */

#include <boost/lexical_cast.hpp>
#include <boost/random/linear_congruential.hpp>
#include <boost/random/uniform_real.hpp>
#include <boost/random/variate_generator.hpp>
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
	boost::minstd_rand generator(42u);
	boost::uniform_real<> xrandom(-30.0,-27);
	boost::uniform_real<> yrandom(-3.0, 3.0);
	boost::uniform_real<> zrandom(-3.0, 3.0);
	boost::variate_generator<boost::minstd_rand&, boost::uniform_real<> > xrandomGenerator(generator, xrandom);
	boost::variate_generator<boost::minstd_rand&, boost::uniform_real<> > yrandomGenerator(generator, yrandom);
	boost::variate_generator<boost::minstd_rand&, boost::uniform_real<> > zrandomGenerator(generator, zrandom);
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
	float upper = 30.0;
	int numValues = 300;
	int numPositions = 1000;
	std::vector<float> xvalues(numPositions);
	std::vector<float> yvalues(numPositions);
	std::vector<float> zvalues(numPositions);
	for (int i = 0; i < numPositions; i++)
	{
		float c0,c1,c2;

		c0 = xrandomGenerator();
		c1 = yrandomGenerator();
		c2 = zrandomGenerator();

		xvalues[i] = c0;
		yvalues[i] = c1;
		zvalues[i] = c2;
		//if (randomGenerator() > .5) { c0 *= -1.0f;};
		//if (randomGenerator() > .5) { c1 *= -1.0f;};
		//if (randomGenerator() > .5) { c2 *= -1.0f;};
		//float value = interpolator->interpolate(variable, c0, c1, c2);
	}
	std::vector<float> previousResults(numPositions);
	for (int num = 0; num < numValues; num++)
	{
		for (int i = 0; i <numPositions; i++)
		{
			float value = interpolator->interpolate(variable, xvalues[i], yvalues[i], zvalues[i]);
			if (num != 0 && previousResults[i] != value)
			{
				std::cout << "Current result (" << value << ") is different from previous result (";
				std::cout << previousResults[i] << ") for position(" << xvalues[i] << "," << yvalues[i] ;
				std::cout << "," << zvalues[i] << ")" << std::endl;
			}
			previousResults[i] = value;

		}
	}
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
