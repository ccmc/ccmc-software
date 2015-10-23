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
#ifdef HAVE_PYTHON
#include <boost/python.hpp> 
#endif

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

	{
		long status = kameleon.open(filename);
		std::cout << "main: opened file: " << filename << " with status: " << status << std::endl;
		if (status == ccmc::FileReader::OK)
		{
			std::cout <<"main: kameleon file was opened successfully" << std::endl;
			bool successLoading = kameleon.loadVariable(variable);
			if (successLoading)
			{
				ccmc::Interpolator * interpolator = kameleon.createNewInterpolator();
				std::cout << "starting interpolations" << std::endl;
				float value = 0.f;
				{
					value = interpolator->interpolate(variable, c0, c1, c2);
					std::cout << "value: " << value << std::endl;
				}
				std::cout << "deleting interpolator"<<std::endl;
				delete interpolator;				
			}
			else{
				std::cout << "Could not load variable" << variable << std::endl;
			}
			kameleon.close();
		}
	}
	return 0;
}
