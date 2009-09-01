/**
*	attempts
*/

#include <iostream>
#include <iomanip>
#include <sstream>
#include <string>
#include <ctime>
#include <ccmc/Kameleon.h>
//#include <ccmc/Kameleon_compatibility.h>
#include <boost/program_options.hpp>
//#include <google/profiler.h>
//#include <ccmc/kameleon_adjusted.h>
#define LENGTH 500;
namespace po = boost::program_options;
int main (int argc, char * argv[])
{

	Kameleon kameleon;
	std::string filename;
	std::string variable;
	int iterations = 10;
	po::options_description desc("Test program");
	desc.add_options()("help", "produce help message")
		("input-file", po::value<std::string>(&filename), "input file")
		("variable", po::value<std::string>(&variable), "The variable");

	po::positional_options_description p;
	p.add("input-file", -1);

	po::variables_map vm;
	po::store(
			po::command_line_parser(argc, argv).options(desc).positional(p).run(),
			vm);
	po::notify(vm);

	if (vm.count("help"))
	{
		cout << desc << endl;
	}

	if (vm.count("input-file"))
	{
		std::cout << "Input file: " << filename << std::endl;
	} else
	{
		std::cout << "Input file was not set." << std::endl;
		std::cout << desc << std::endl;
		exit(1);
	}
	std::cout << "after po options" << std::endl;

	kameleon.open(filename);
	std::string rho_ = "rho";
	std::string n_ = "n";
	std::string pram_ = "pram";
	kameleon.loadVariable(variable);

	std::cout << "Creating new interpolator" << std::endl;
	Interpolator * interpolator = kameleon.createNewInterpolator();
	clock_t start, finish;

	start = clock();

	float conversion = kameleon.getConversionFactorToVis(variable);
	float value;
	std::string bx_ = "bx";
	float convertedValue;
//	ProfilerStart("/tmp/reader2.prof");
	for (int i = 0; i < 1000000; i++)
	{

		value = interpolator->interpolate(variable, -10.0f + 6.f * (float) (i % 2), -2.0f, 0.0f);
		convertedValue = value * conversion;
		//std::cout << "Destroying new interpolator" << std::endl;
		//std::cout << "value: " << value << std::endl;
	}
	std::cout << "conversionFactor: " << conversion << std::endl;
	std::cout << "value: " << value << " " << kameleon.getNativeUnit(variable) << " convertedValue: " <<
		convertedValue << " " << kameleon.getVisUnit(variable) << std::endl;

//	ProfilerStop();
	finish = clock();
	float elapsed_time = ((double) finish - (double) start) / CLOCKS_PER_SEC;
	delete interpolator;
	std::cout << "Closing file" << std::endl;
	//kameleon.close();
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
