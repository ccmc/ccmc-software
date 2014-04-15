#include <iostream>
#include <iomanip>
#include <sstream>
#include <string>
#include <ctime>
#include <cmath>
#include <cstdlib>
#include <ccmc/Kameleon.h>
#include <ccmc/FileReader.h>
#include <boost/lexical_cast.hpp>
#include <boost/random/linear_congruential.hpp>
#include <boost/random/uniform_real.hpp>
#include <boost/random/variate_generator.hpp>

int main (int argc, char * argv[])
{

	ccmc::Kameleon kameleon;
	std::string filename;
	std::string variable;
	int iterations = 10;
	float c0,c1,c2;
	if (argc != 6)
	{
		std::cout << "kameleon <filename> variable c0 c1 c2\n";
		std::cout << "\tAdapt3D, OpenGGCM, BATSRUS: x y z\n";
		std::cout << "\tENLIL, MAS: r theta(latitude) phi(longitude)" << std::endl;
		exit(1);
	}
	filename = argv[1];
	variable = argv[2];
	c0 = boost::lexical_cast<float>(argv[3]);
	c1 = boost::lexical_cast<float>(argv[4]);
	c2 = boost::lexical_cast<float>(argv[5]);
	//for (int o = 0; o < 5; o++)
	{
		long status = kameleon.open(filename);
		std::cout << "Opened file: " << filename << " with status: " << status << std::endl;
		std::cout << "FileReader::OK = " << ccmc::FileReader::OK << std::endl;
		if (status == ccmc::FileReader::OK)
		{

			bool successLoading = kameleon.loadVariable(variable);
			ccmc::Interpolator * interpolator = kameleon.createNewInterpolator();
			clock_t start, finish;

			std::cout << "starting interpolations" << std::endl;
			//start = clock();
			float value = 0.f;
			//for (int i = 0; i < 1; i++)
			{
				value = interpolator->interpolate(variable, c0, c1, c2);
				std::cout << "value: " << value << std::endl;
			}
			finish = clock();
			delete interpolator;
			kameleon.close();
		}
	}
	return 0;
}
