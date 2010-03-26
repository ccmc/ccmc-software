#include <iostream>
#include <iomanip>
#include <sstream>
#include <string>
#include <vector>
#include <ccmc/Kameleon.h>
#include <ccmc/Kameleon-Tracer.h>
#include <ccmc/Fieldline.h>
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
	Kameleon kameleon;
	kameleon.open(filename);
	kameleon.loadVectorVariable(variable);
	Tracer tracer(&kameleon);
	tracer.setMaxIterations(20000);
	//tracer.setInnerBoundary(2.5f);
	tracer.setDn(.2f);

	clock_t start, finish;
//	ProfilerStart("/tmp/tracer.prof");
	start = clock();
	Fieldline f1;
	//for (int i = 0; i < 25; i++)
	{

		f1 = tracer.bidirectionalTrace(variable,c0,c1,c2);
	}
	finish = clock();
//	ProfilerStop();
	float elapsed_time = ((double) finish - (double) start) / CLOCKS_PER_SEC;
	kameleon.close();

	float dn = .2f;
	int step_max = 20000;
	cout << "Before btrace" << endl;
	float r_end = 2.5f;

	/*old::Tracer oldTracer;
	oldTracer.open((char *)filename.c_str());
	oldTracer.loadVariable((char * )variable.c_str());
	start = clock();
	old::Fieldline f2;
	for (int i = 0; i < 25; i++)
	{
		 f2 = oldTracer.bidirectionalTrace((char *)variable.c_str(), c0, c1, c2, r_end, step_max, dn);
	}*/
	finish = clock();
	float elapsed_time2 = ((double) finish - (double) start) / CLOCKS_PER_SEC;



	for (int i = 0; i < f1.size(); i++)
	{
		std::cout << "New[" << i << "]: " << f1.getPosition(i) << std::endl;
	}

	/*for (int i = 0; i < f2.size(); i++)
	{
		std::cout << "Old[" << i << "]: " << f2.getPosition(i) << std::endl;
	}*/

	std::cout << "New elapsed time: " << elapsed_time << " with " << f1.getPositions().size() << " positions." << std::endl;
	//std::cout << "Old elapsed time: " << elapsed_time2 << " with " << f2.getPositions().size() << " positions." << std::endl;
	//std::cout << "Speedup: " << elapsed_time2 / elapsed_time << std::endl;


	return 0;
}
