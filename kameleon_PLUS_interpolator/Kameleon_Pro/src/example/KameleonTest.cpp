#include <iostream>
#include <iomanip>
#include <sstream>
#include <string>
#include <ccmc/Kameleon.h>
#include <kameleon_old.h>
#include <ccmc/FileReader.h>
#include <ccmc/Model.h>
#include <ccmc/OpenGGCM.h>
#include <ccmc/BATSRUS.h>
#include <ccmc/Attribute.h>
#include <ctime>
#include <google/profiler.h>
#include <boost/program_options.hpp>

namespace po = boost::program_options;
int main(int argc, char * argv[])
{
	std::string filename;
	int iterations = 10;
	po::options_description desc("Test program");
	desc.add_options()("help", "produce help message")("input-file", po::value<
			std::string>(&filename), "input file")("iterations",
			po::value<int>(&iterations),
			"Number of iterations to interpolate position");

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

	/*Derived derived;

	 derived.open((char*)filename.c_str());
	 float value2;
	 //float value2 = derived.interpolate("bx", -10.f, -2.f, 0.0f);
	 value2 = derived.interpolate("bx", -10.f, -2.f, 0.0f);

	 CDFid current_cdf_id = get_current_CDFid();

	 cout << "current_cdf_id: " << current_cdf_id << "." << endl;

	 derived.close();
	 FileReader fileReader1;
	 FileReader fileReader2;
	 //for (int i = 0; i < 1; i++)
	 //{
	 fileReader1.open(filename);
	 fileReader2.open(filename);
	 Attribute attribute = fileReader2.getGlobalAttribute("model_name"); //string
	 std::cout << "attribute value: " << attribute.getAttributeString() << std::endl;


	 attribute = fileReader2.getGlobalAttribute("grid_system_count"); //int
	 std::cout << "attribute value: " << attribute.getAttributeInt() << std::endl;

	 fileReader2.close();
	 fileReader1.close();

	 //}
	 //derived.selectCDF(current_cdf_id);
	 //std::cout << "Units: " << derived.getUnits("bx") << std::endl;
	 */

	Model * model = NULL;

	FileReader fReader;
	fReader.open(filename);
	string model_name =
			(fReader.getGlobalAttribute("model_name")).getAttributeString();
	if (model_name == "open_ggcm" || model_name == "ucla_ggcm")
	{
		std::cout << "created OpenGGCM object" << std::endl;
		model = new OpenGGCM();
	} else if (model_name == "batsrus")
	{
		std::cout << "created BATSRUS object" << std::endl;
		model = new BATSRUS();
	}
	model->open(filename);
	Attribute aattribute = model->getGlobalAttribute("model_name");
	model->loadVariable("bx");
	long variable_id = model->getVariableID("bx");
	std::vector<float> newTimesSameBlock;
	std::vector<float> newValuesSameBlock;
	std::vector<float> oldTimesSameBlock;
	std::vector<float> oldValuesSameBlock;

	std::vector<float> newTimesDiffBlock;
	std::vector<float> newValuesDiffBlock;
	std::vector<float> oldTimesDiffBlock;
	std::vector<float> oldValuesDiffBlock;

	std::string variable = "bx";
	clock_t start, finish;
	float value;
	double elapsed_time;
	std::string open_ggcm = "open_ggcm";
	std::string batsrus = "batsrus";
	std::string ucla_ggcm = "ucla_ggcm";
	Interpolator * interpolator = model->createNewInterpolator();


	//calculate new times for same block

	ProfilerStart("/tmp/reader2.prof");
	for (int i = 0; i < 10; i++)
	{
		start = clock();
		int iter = iterations * (i+1);
		for (int j = 0; j < iter; j++)
		{
			value = interpolator->interpolate(variable,-10.0f/* + 6.f * (float) (j % 2)*/, -2.0f, 0.0f);

			//total += value;
			//std::cout << "New value: " << value << std::endl;
			//std::cout << "model_name: " << aattribute.getAttributeString() << std::endl;
		}

		finish = clock();
		elapsed_time = ((double) finish - (double) start) / CLOCKS_PER_SEC;
		newTimesSameBlock.push_back(elapsed_time);
		newValuesSameBlock.push_back(value);
	}
	ProfilerStop();
	model->close();
	//delete model;

	//calculate old times for same block
	open_cdf((char *) filename.c_str(), 1, "bx");
	const char * bx = variable.c_str();
	for (int i = 0; i < 10; i++)
	{
		start = clock();
		int iter = iterations * (i+1);
		if (model_name == open_ggcm || model_name == ucla_ggcm)
		{
			for (int j = 0; j < iter; j++)
			{
				value = interpolate_ucla_ggcm_cdf((char *) bx, -10.0f/* + 6.f
						* (float) (j % 2)*/, -2.0f, 0.0f, 1, 0);
			}

		} else if (model_name == batsrus)
		{
			for (int j = 0; j < iter; j++)
			{

				value = interpolate_batsrus_cdf((char *) bx, -10.0f/* + 6.f
						* (float) (j % 2)*/, -2.f, 0.f, 1, 0);
			}
		}
		finish = clock();
		elapsed_time = ((double) finish - (double) start) / CLOCKS_PER_SEC;
		oldTimesSameBlock.push_back(elapsed_time);
		oldValuesSameBlock.push_back(value);
		//total += value;
		//std::cout << "New value: " << value << std::endl;
		//std::cout << "model_name: " << aattribute.getAttributeString() << std::endl;
	}

	close_cdf();

	/*** different block ***/
	model->open(filename);
		//aattribute = model->getGlobalAttribute("model_name");
		model->loadVariable("bx");
		interpolator = model->createNewInterpolator();

	for (int i = 0; i < 10; i++)
	{

		int iter = iterations * (i+1);
		start = clock();
		for (int j = 0; j < iter; j++)
		{
			value = interpolator->interpolate(variable_id,-10.0f + 6.f * (float) (j % 2), -2.0f, 0.0f);

			//total += value;
			//std::cout << "New value: " << value << std::endl;
			//std::cout << "model_name: " << aattribute.getAttributeString() << std::endl;
		}

		finish = clock();
		elapsed_time = ((double) finish - (double) start) / CLOCKS_PER_SEC;
		newTimesDiffBlock.push_back(elapsed_time);
		newValuesDiffBlock.push_back(value);
	}
	//ProfilerStop();
	model->close();
	delete model;

	//calculate old times for same block
	open_cdf((char *) filename.c_str(), 1, "bx");
	//const char * bx = variable.c_str();
	for (int i = 0; i < 10; i++)
	{

		int iter = iterations * (i+1);
		start = clock();
		if (model_name == open_ggcm || model_name == ucla_ggcm)
		{
			for (int j = 0; j < iter; j++)
			{
				value = interpolate_ucla_ggcm_cdf((char *) bx, -10.0f + 6.f
						* (float) (j % 2), -2.0f, 0.0f, 1, 0);
			}

		} else if (model_name == batsrus)
		{
			for (int j = 0; j < iter; j++)
			{

				value = interpolate_batsrus_cdf((char *) bx, -10.0f + 6.f
						* (float) (j % 2), -2.f, 0.f, 1, 0);
			}
		}
		finish = clock();
		elapsed_time = ((double) finish - (double) start) / CLOCKS_PER_SEC;
		oldTimesDiffBlock.push_back(elapsed_time);
		oldValuesDiffBlock.push_back(value);
		//total += value;
		//std::cout << "New value: " << value << std::endl;
		//std::cout << "model_name: " << aattribute.getAttributeString() << std::endl;
	}

	close_cdf();
	/*** end different block ***/

	std::cout << "Same block tests" << std::endl;
	std::cout << "----------------" << std::endl;
	for (int i = 0; i < 10; i++)
	{
		std::cout << "old_time: " << oldTimesSameBlock[i] << " new_time: " << newTimesSameBlock[i] << " speedup: ";
		std::cout << oldTimesSameBlock[i]/newTimesSameBlock[i] << " old_value: " << oldValuesSameBlock[i];
		std::cout << " new_value: " << newValuesSameBlock[i] << std::endl;
	}

	std::cout << "\nDifferent block tests" << std::endl;
	std::cout << "----------------" << std::endl;
	for (int i = 0; i < 10; i++)
	{
		std::cout << "old_time: " << oldTimesDiffBlock[i] << " new_time: " << newTimesDiffBlock[i] << " speedup: ";
		std::cout << oldTimesDiffBlock[i]/newTimesDiffBlock[i] << " old_value: " << oldValuesDiffBlock[i];
		std::cout << " new_value: " << newValuesDiffBlock[i] << std::endl;
	}
	return 0;
}
