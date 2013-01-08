#include <iostream>
#include <fstream>
#include <iomanip>
#include <sstream>
#include <string>
#include <vector>
#include <ccmc/Kameleon.h>
#include <ccmc/Tracer.h>
#include <ccmc/Fieldline.h>
//#include <ccmc/old-tracer.h>
#include <ccmc/Point3f.h>
#include <boost/lexical_cast.hpp>
#include <boost/program_options.hpp>
//#include <google/profiler.h>
#include <ctime>
#include <boost/foreach.hpp>
#include <boost/tokenizer.hpp>
#define foreach BOOST_FOREACH
#define LENGTH 500;


using namespace ccmc;
//namespace po = boost::program_options;

int main (int argc, char * argv[])
{
	std::string filename;
	std::string variable;
	std::string seedfile;
	std::string additional;
	float c0;
	float c1;
	float c2;
	int iterations = 10;

	if (argc != 6)
	{
		std::cout << "tracer <filename> <variable> c0 c1 c2" << std::endl;
		exit(1);
	}

	filename = argv[1];
	variable = argv[2];
	c0 = boost::lexical_cast<float>(argv[3]);
	c1 = boost::lexical_cast<float>(argv[4]);
	c2 = boost::lexical_cast<float>(argv[5]);
//	boost::program_options::options_description desc("Fieldline tracer example program.");
//	desc.add_options()("help", "produce help message")
//			          ("input-file", boost::program_options::value<std::string>(&filename), "input file")
//					  ("variable", boost::program_options::value<std::string>(&variable), "Variable to trace")
//					  ("c0", boost::program_options::value<float>(&c0), "First position component")
//					  ("c1", boost::program_options::value<float>(&c1), "Second position component")
//					  ("c2", boost::program_options::value<float>(&c2), "Third position component")
//					  ("additional-variables", boost::program_options::value<std::string>(&additional),
//							  "Additional variables to interpolate at each position of the returned fieldline")
//					  ("seeds", boost::program_options::value<std::string>(&seedfile), "seed positions file");
//
//	boost::program_options::positional_options_description p;
//	p.add("input-file", -1);
//
//	boost::program_options::variables_map vm;
//	boost::program_options::store(
//			boost::program_options::command_line_parser(argc, argv).options(desc).positional(p).run(),
//			vm);
//	boost::program_options::notify(vm);
//
//	if (vm.count("help"))
//	{
//		std::cout << desc << std::endl;
//		exit(0);
//	}
//
//	bool useSeedPosition = false;
//	bool useSeedFile = false;
//	if (vm.count("c0") && vm.count("c1") && vm.count("c2"))
//	{
//		useSeedPosition = true;
//	} else if (vm.count("seeds"))
//	{
//		useSeedFile = true;
//	} else
//	{
//
//		std::cout << "Error: must specify either a seed position with c0, c1, and c2 or a seed positions file with seeds\n" << std::endl;
//		std::cout << desc << std::endl;
//
//		exit(0);
//	}
//
//	if (vm.count("input-file"))
//	{
//		std::cout << "Input file: " << filename << std::endl;
//	} else
//	{
//		std::cout << "Input file was not set." << std::endl;
//		std::cout << desc << std::endl;
//		exit(1);
//	}
//


	Kameleon kameleon;
	kameleon.open(filename);
	std::cout << "about to load vector variable" << std::endl;

	for (int i = 0; i < 1000; i++)
		kameleon.loadVectorVariable(variable);

	std::cout << "finished loading variable 1000 times" << std::endl;
	Tracer tracer(&kameleon);

	tracer.setMaxIterations(20000);
	if (kameleon.getModelName() == "ADAPT3D")
	{
		tracer.setInnerBoundary(1.f);
	} else if (kameleon.getModelName() == "enlil")
	{
		tracer.setInnerBoundary(kameleon.getVariableAttribute("r","actual_min").getAttributeFloat()/ccmc::constants::AU_in_meters);
	}
	//tracer.setInnerBoundary(2.5f);
	tracer.setDn(.2f);

	clock_t start, finish;
//	ProfilerStart("/tmp/tracer.prof");
	boost::char_separator<char> sep(",");
	std::string variables_selected = additional;
	boost::tokenizer<boost::char_separator<char> > tokens(variables_selected, sep);
	std::vector<std::string> variables;
	foreach(std::string t, tokens)
	{
		variables.push_back(t);
	}
	std::cout << "finished pushing variables" << std::endl;

	std::vector<Point3f> seedsVector;
	if (true)
	{

		seedsVector.push_back(Point3f(c0,c1,c2));
		std::cout << "added position" << std::endl;
	} else
	{
		std::ifstream seedsFileStream;
		seedsFileStream.open(seedfile.c_str());
		if (!seedsFileStream.is_open())
		{
			std::cerr << "Error opening seeds position file " << seedfile << "." << std::endl;
			exit(1);
		}else
		{
			std::string line;
			while(seedsFileStream.good())
			{
				getline(seedsFileStream, line);
				//std::cout << line << std::endl;
				boost::char_separator<char> sep(" ");
				std::string variables_selected = additional;
				boost::tokenizer<boost::char_separator<char> > tokens(line, sep);
				std::vector<std::string> variables;
				int count = 0;
				Point3f position;
				foreach(std::string t, tokens)
				{
					if (count == 0)
					{
						position.component1 = boost::lexical_cast<float>(t);
						count++;
					} else if (count == 1)
					{
						position.component2 = boost::lexical_cast<float>(t);
						count++;
					} else
					{
						position.component3 = boost::lexical_cast<float>(t);
					}
				}
				if (count > 0)
					seedsVector.push_back(position);
			}
		}


		//ifstream
	}

	for (int i = 0; i < seedsVector.size(); i++)
	{
		Point3f curr = seedsVector[i];
		std::cout << "--------------------------------------------------" << std::endl;
		std::cout << "Tracing using seed position (" << curr.component1 << ",";
		std::cout << curr.component2 << "," << curr.component3 << ") in timestep: ";
		std::cout << kameleon.getCurrentTime() << " for run: " << kameleon.getGlobalAttribute("run_registration_number").getAttributeString() << std::endl;
		start = clock();
		Fieldline f1;
		for (int i = 0; i < 1000; i++)
		{

			f1 = tracer.bidirectionalTrace(variable,curr.component1, curr.component2, curr.component3);
		}
		finish = clock();
		float elapsed_time = ((double) finish - (double) start) / CLOCKS_PER_SEC;


		float dn = .2f;
		int step_max = 20000;
		float r_end = 2.5f;

		std::cout << "Position";
		for (int j = 0; j < variables.size(); j++)
		{
			std::cout << " " << variables[j];
			kameleon.loadVariable(variables[j]);
		}
		std::cout << std::endl;
		Interpolator * interpolator = kameleon.createNewInterpolator();
		for (int i = 0; i < f1.size(); i++)
		{
			Point3f position = f1.getPosition(i);
			std::cout << position;
			for (int j = 0; j < variables.size(); j++)
			{

				std::cout << " " << interpolator->interpolate(variables[j], position.component1, position.component2, position.component3);
			}
			std::cout << std::endl;



		}

		std::cout << "Elapsed time: " << elapsed_time << " with " << f1.getPositions().size() << " positions." << std::endl;
		std::cout << "End Tracing for seed position (" << curr.component1 << "," << curr.component2 << "," << curr.component3 << ") in timestep: " << kameleon.getCurrentTime() << " for run: " << kameleon.getGlobalAttribute("run_registration_number").getAttributeString() << std::endl;
		std::cout << "--------------------------------------------------" << std::endl;
	}
	kameleon.close();


	return 0;
}
