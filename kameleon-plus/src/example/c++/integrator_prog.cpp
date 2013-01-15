#include <iostream>
#include <iomanip>
#include <sstream>
#include <string>
#include <vector>
#include <ccmc/Kameleon.h>
#include <ccmc/Tracer.h>
#include <ccmc/Fieldline.h>
#include <ccmc/StringConstants.h>
#include <ccmc/Interpolator.h>
//#include <ccmc/old-tracer.h>
#include <ccmc/Point3f.h>
#include <boost/lexical_cast.hpp>
//#include <boost/program_options.hpp>
//#include <google/profiler.h>
#include <ctime>
#define LENGTH 500;

using namespace std;
using namespace ccmc;

//namespace po = boost::program_options;

int main (int argc, char * argv[])
{
	std::string filename;
	std::string variable;
	float c0;
	float c1;
	float c2;
	int iterations = 10;

	if (argc != 6)
	{
		cout << "tracer <filename> <variable> c0 c1 c2" << endl;
		exit(1);
	}

	filename = argv[1];
	variable = argv[2];
	c0 = boost::lexical_cast<float>(argv[3]);
	c1 = boost::lexical_cast<float>(argv[4]);
	c2 = boost::lexical_cast<float>(argv[5]);
	/*po::options_description desc("Test program");
	desc.add_options()("help", "produce help message")("input-file", po::value<std::string>(&filename), "input file")
					  ("variable", po::value<std::string>(&variable), "Variable to trace")
					  ("c0", po::value<float>(&c0), "First position component")
					  ("c1", po::value<float>(&c1), "Second position component")
					  ("c2", po::value<float>(&c2), "Third position component");

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

*/

	cout << "hello from integrator!" << std::endl;

	Kameleon kameleon;
	kameleon.open(filename);
	kameleon.loadVectorVariable(variable);
	Tracer tracer(&kameleon);
	tracer.setMaxIterations(20000);
	//tracer.setInnerBoundary(2.5f);
	tracer.setDn(.2f);


	clock_t start, finish;
	start = clock();
	Fieldline f1;

	f1 = tracer.bidirectionalTrace(variable,c0,c1,c2);

	finish = clock();
	float elapsed_time = ((double) finish - (double) start) / CLOCKS_PER_SEC;

	int vnum = 1;
	std::string variables[] = {ccmc::strings::variables::b_};
	Fieldline f2;


	Point3f p;
	ccmc::Interpolator * interpolator = kameleon.createNewInterpolator();
	for (int f = 0; f < f1.size(); f++)
	{
		p = f1.getPosition(f);
		for (int i = 0; i < vnum; i++)
		{
			f2.insertPointData(p,interpolator->interpolate(variables[i],
					p.component1,p.component2,p.component3));
		}
	}
	delete interpolator;


	kameleon.close();

	float dn = .2f;
	int step_max = 20000;	
	float r_end = 2.5f;

	finish = clock();
	float elapsed_time2 = ((double) finish - (double) start) / CLOCKS_PER_SEC;

	vector<float> elengths = f2.getDs();
	vector<float> length = f2.measure();

	std:: cout << "total points: "<< f2.size() << endl;
	std:: cout << "points in f1: " << f1.size() << endl;
	std::cout << "New elapsed time: " << elapsed_time << " with " << f1.getPositions().size() << " positions." << std::endl;


		for (int i = 0; i < f2.size(); i++)
		{
			if (((i % 100) == 0)||i==f2.size()-1)
			{
			std::cout << "New[" << i << "]: " << f2.getPositions()[i];
			cout << "  data at point: "  << f2.getData()[i];
				if(i > 0)
				{
					cout << "  ds vector: " << f2.getElements()[i];
					cout << "  element length: " << f2.getDs()[i];
				}
				cout << " total length: " << f2.measure()[i];
				cout << " integral: "<< f2.integrate()[i];
				cout << std::endl;
			}
		}
		Fieldline f3 = f2.interpolate(1,100);
		for(int i = 0; i < f3.size(); i++)
		{
			std::cout << "New[" << i << "]: " << f3.getPositions()[i];
					cout << "  data at point: "  << f3.getData()[i];
					cout << " nearest lower vertex: " << f3.getNearest()[i];
					cout << " data at nearest: " << f2.getData()[f3.getNearest()[i]] << " "<< f2.getData()[f3.getNearest()[i]+1];
					cout << std::endl;
		}


	return 0;
}
