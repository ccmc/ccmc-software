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

int main (int argc, char * argv[])
{

	Kameleon kameleon;
	std::string filename;
	std::string variable;
	int iterations = 10;
	float c0,c1,c2;
	if (argc != 6)
	{
		cout << "kameleon <filename> variable c0 c1 c2\n";
		cout << "\tOpenGGCM, BATSRUS: x y z\n";
		cout << "\tENLIL, MAS: r theta(latitude) phi(longitude)" << endl;
		exit(1);
	}
	filename = argv[1];
	variable = argv[2];
	c0 = boost::lexical_cast<float>(argv[3]);
	c1 = boost::lexical_cast<float>(argv[4]);
	c2 = boost::lexical_cast<float>(argv[5]);
	long status = kameleon.open(filename);
	if (status == FileReader::OK)
	{
		std::cout << "Opened file: " << filename << " with status: " << status << std::endl;
		std::string rho_ = "rho";
		std::string n_ = "n";
		std::string pram_ = "pram";
		std::cout << "Model name: " << kameleon.getGlobalAttribute("model_name").getAttributeString() << std::endl;
		std::cout << "Min: " << kameleon.getVariableAttribute("x", "actual_min").getAttributeFloat() << std::endl;
		std::cout << "Loading variables" << std::endl;
		bool successLoading = kameleon.loadVariable(variable);
		bool success = kameleon.doesVariableExist(variable);
		//std::cout << "Creating new interpolator" << std::endl;
		Interpolator * interpolator = kameleon.createNewInterpolator();
		clock_t start, finish;

		start = clock();

		float conversion = 1.0f;
		//float conversion = interpolator->getConversionFactorToVis(variable);
		float value;
		//std::string bx_ = "br";
		float convertedValue;
		//generate a random number between -5 and 5;
		float x,y,z;
		srand((unsigned)time(0));
		int range = 10;
		if (success)
		{
			value = interpolator->interpolate(variable, c0,c1,c2 );
		} else
			value = kameleon.getMissingValue();

		std::cout << "value before random interpolations: " << value << std::endl;
		for (int i = 0; i < 1; i++)
		{
			for (int j = 0; j < 1; j++)
			{
				for (int k = 0; k < 1; k++)
				{

					x = ((float)(rand())/(float)(RAND_MAX)) * range - 5.f;
					y = ((float)(rand())/(float)(RAND_MAX)) * range - 5.f;
					z = ((float)(rand())/(float)(RAND_MAX)) * range - 5.f;
					float radius = sqrt(x*x+y*y+z*z);

					while(radius > 5.0f || radius < 1.0f)
					{

						x = ((float)(rand())/(float)(RAND_MAX)) * range - 5.f;
						y = ((float)(rand())/(float)(RAND_MAX)) * range - 5.f;
						z = ((float)(rand())/(float)(RAND_MAX)) * range - 5.f;
						radius = sqrt(x*x+y*y+z*z);
					}
					value = interpolator->interpolate(variable, x, y, z);
					//std::cerr << x << "," << y << "," << z << ": " << value << std::endl;
				}
			}
		}
		if (success)
		{
			value = interpolator->interpolate(variable, c0,c1,c2 );
		} else
			value = kameleon.getMissingValue();
		convertedValue = value * conversion;

		std::cout << "value after random interpolations: " << value << std::endl;
		if (success)
		{
			value = interpolator->interpolate(variable, c0,c1,c2 );
		} else
			value = kameleon.getMissingValue();
		convertedValue = value * conversion;

		std::cout << "value after random interpolations: " << value << std::endl;
		//std::cout << "conversionFactor: " << conversion << std::endl;
		//std::cout << "value: " << value << " " << kameleon.getNativeUnit(variable) << " convertedValue: " <<
		//	convertedValue << " " << kameleon.getVisUnit(variable) << std::endl;


//		float elapsed_time = ((double) finish - (double) start) / CLOCKS_PER_SEC;

		delete interpolator;
		kameleon.close();
	}
	return 0;
}
