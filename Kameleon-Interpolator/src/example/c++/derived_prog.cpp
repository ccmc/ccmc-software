/**
*	attempts
*/

#include <iostream>
#include <iomanip>
#include <sstream>
#include <string>
#include <ctime>
#include <ccmc/Kameleon.h>
#include <boost/lexical_cast.hpp>
//#include <ccmc/Kameleon_compatibility.h>
//#include <google/profiler.h>
//#include <ccmc/kameleon_adjusted.h>
#define LENGTH 500;
int main (int argc, char * argv[])
{

	Kameleon kameleon;
	std::string filename;
	std::string variable;
	int iterations = 10;
	//po::options_description desc("Test program");
	//desc.add_options()("help", "produce help message")
	//	("input-file", po::value<std::string>(&filename), "input file")
	//	("variable", po::value<std::string>(&variable), "The variable");

	//po::positional_options_description p;
	//p.add("input-file", -1);

	//po::variables_map vm;
	//po::store(
	//		po::command_line_parser(argc, argv).options(desc).positional(p).run(),
	//		vm);
	//po::notify(vm);

	//if (vm.count("help"))
	//{
	//	cout << desc << endl;
	//}

	//if (vm.count("input-file"))
	//{
	//	std::cout << "Input file: " << filename << std::endl;
	//} else
	//{
	//	std::cout << "Input file was not set." << std::endl;
	//	std::cout << desc << std::endl;
	//	exit(1);
	//}
	//std::cout << "after po options" << std::endl;

	float c0,c1,c2;
	if (argc != 6)
	{
		cout << "derived <filename> variable c0 c1 c2\n";
		cout << "\tOpenGGCM, BATSRUS: x y z\n";
		cout << "\tENLIL, MAS: r phi(latitude) theta(longitude)" << endl;
		exit(1);
	}
	filename = argv[1];
	variable = argv[2];
	c0 = boost::lexical_cast<float>(argv[3]);
	c1 = boost::lexical_cast<float>(argv[4]);
	c2 = boost::lexical_cast<float>(argv[5]);
	kameleon.open(filename);
	std::cout << "Opened file: " << filename << std::endl;
	std::string rho_ = "rho";
	std::string n_ = "n";
	std::string pram_ = "pram";
	std::cout << "Loading variables" << std::endl;
	kameleon.loadVariable(variable);

	std::cout << "Creating new interpolator" << std::endl;
	Interpolator * interpolator = kameleon.createNewInterpolator();
	clock_t start, finish;

	start = clock();

	float conversion = 1.0f;
	//float conversion = interpolator->getConversionFactorToVis(variable);
	float value;
	//std::string bx_ = "br";
	float convertedValue;
//	ProfilerStart("derived.prof");
	//Interpolations.  The i%2 ensures the worst case scenario (previous block differe88nt than current block
	for (int i = 0; i < 1000; i++)
	{
		if (i % 100 == 0)
		{
			//std::cout << "i: " << i << std::endl;
		}
		value = interpolator->interpolate(variable, c0,c1,c2 );
		convertedValue = value * conversion;

	}
//	ProfilerStop();
	std::cout << "conversionFactor: " << conversion << std::endl;
	std::cout << "value: " << value << " " << kameleon.getNativeUnit(variable) << " convertedValue: " <<
		convertedValue << " " << kameleon.getVisUnit(variable) << std::endl;


	finish = clock();
	float elapsed_time = ((double) finish - (double) start) / CLOCKS_PER_SEC;

	std::cout << "test for disk access: " << interpolator->interpolate(variable, -10.f, -2.f, -0.f) << std::endl;
	delete interpolator;

	std::cout << "Elapsed time for interpolation: " << elapsed_time << std::endl;


	//test units
	std::vector<std::string> variables;
	variables.push_back("bx");
	variables.push_back("by");
	variables.push_back("bz");


	for (int i = 0; i < variables.size(); i++)
	{
		std::cout << "variable: " << variables[i] << " units: " << kameleon.getSIUnit(variables[i]) << std::endl;
	}

	kameleon.close();
	return 0;
}
