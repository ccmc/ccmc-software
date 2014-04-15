#include <iostream>
#include <fstream>
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
#include <boost/tokenizer.hpp>
//#include <boost/program_options.hpp>
//#include <google/profiler.h>
#include <boost/foreach.hpp>
#define foreach BOOST_FOREACH
#include <ctime>
#define LENGTH 500;

using namespace std;
using namespace ccmc;

//namespace po = boost::program_options;

int main (int argc, char * argv[])
{
// Output file names-----------------
	ofstream File("data_out.txt");
	ofstream File2("flineout.txt");
	ofstream File3("integrals.txt");
	ofstream File4("minima.txt");
	ofstream File5("maxima.txt");
	ofstream File6("polymin.txt");
	ofstream File7("polymax.txt");
    ofstream FileE("flineExEyEz.txt");
//------------------------------------
	std::string filename;
	std::string variable;
	std::string positions;
    std::vector<Point3f> seedsVector;
	float c0;
	float c1;
	float c2;
	int iterations = 10;

	if (argc != 6)
	{
		cout << "integrator <filename> <variable> <positions>" << endl;
//		exit(1);
	}

	filename = argv[1];
	variable = argv[2];
	positions = argv[3];
	/*po::options_description desc("Test program");
	desc.add_options()("help", "produce help message")("input-file", po::value<std::string>(&filename), "input file")
					  ("variable", po::value<std::string>(&variable), "Variable to trace")
					  ("positions",po::value<std::string>(&positions), "Positions file");	
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
		cout << "Input file: " << filename << std::endl;
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
	cout <<"file open\n";
	kameleon.loadVectorVariable(variable);
	cout <<"integrator: variables loaded\n";
	Tracer tracer(&kameleon);
	tracer.setMaxIterations(20000);
	//tracer.setInnerBoundary(2.5f);
	tracer.setDn(.2f);
	cout<<"integrator: tracer on\n";


	clock_t start, finish;
	start = clock();


	std::ifstream seedsFileStream;
	seedsFileStream.open(positions.c_str());
	if (!seedsFileStream.is_open())
	{
			std::cerr << "Error opening seeds position file " << positions << "." << std::endl;
			exit(1);
	}else
	{
		std::string line;
		while(seedsFileStream.good())
		{
			getline(seedsFileStream, line);
			//std::cout << line << std::endl;
			boost::char_separator<char> sep(" ");
			boost::tokenizer<boost::char_separator<char> > tokens(line, sep);
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
	File2 << "f3index px py pz f3data(px,py,pz)  f3int(ds*data)\n";
	FileE << "flinenum flinedex px py pz Ex Ey Ez\n";
	int polymin = 0;
	int polymax = 0;

    for (int i = 0; i < seedsVector.size(); i++)
    {
	cout <<"seed number "<< i << "\n";
    Point3f curr = seedsVector[i];
	c0 = curr.component1;
	c1 = curr.component2;
	c2 = curr.component3;
	// File2<<"seed: "<< c0 <<" "<< c1 <<" "<< c2 << "\n";
 	cout<<"seed: "<< c0 <<" "<< c1 <<" "<< c2 << "\n";
	Fieldline f1; //create new field line
	cout<<"Before bidirectional Trace\n";
	f1 = tracer.bidirectionalTrace(variable,c0,c1,c2); 
	cout <<"After bidirectional Trace\n";
	finish = clock();
	float elapsed_time = ((double) finish - (double) start) / CLOCKS_PER_SEC;

	int vnum = 1;
	std::string variables[] = {ccmc::strings::variables::b_};
	Fieldline f2;  //create new field line
    Fieldline fEx; //create field line for storing Efield
	Fieldline fEy;
	Fieldline fEz;
	float binv = 0.0; //create variable to hold partial integral ds/B
	Point3f p;  // create point to store sampled positions
    Point3f E; // create point to store Efield
    Point3f B; // create point to store Bfield
	ccmc::Interpolator * interpolator = kameleon.createNewInterpolator();
	for (int f = 0; f < f1.size(); f++)  //march along field line
	{
		p = f1.getPosition(f); // store current position in p 
		for (int g = 0; g < vnum; g++) // for each variables
		{
			binv = interpolator->interpolate(variables[g],p.component1,p.component2,p.component3);
			binv = 1.0/binv; // compute 1/B
			f2.insertPointData(p,binv); // add data at position into f2
			E.component1 =interpolator->interpolate(ccmc::strings::variables::ex_,
				p.component1,p.component2,p.component3);
                        E.component2 =interpolator->interpolate(ccmc::strings::variables::ey_,
                                p.component1,p.component2,p.component3);
                        E.component3 =interpolator->interpolate(ccmc::strings::variables::ez_,
                                p.component1,p.component2,p.component3);
			fEx.insertPointData(p,E.component1);
                        fEy.insertPointData(p,E.component2);
                        fEz.insertPointData(p,E.component3);
		}
	}
	delete interpolator;
	f2.minmax();

	float dn = .2f;
	int step_max = 20000;	
	float r_end = 2.5f;

	finish = clock();
	float elapsed_time2 = ((double) finish - (double) start) / CLOCKS_PER_SEC;

	vector<float> elengths = f2.getDs(); //creates elements and elementsMagnitude
	vector<float> length = f2.measure(); //uses elementsMagnitude

	std::cout << "total points: "<< f2.size()<<"\n" ;
	std::cout << "points in f1: " << f1.size() << "\n";
	std::cout << "New elapsed time: " << elapsed_time << " with " << f1.getPositions().size() << " positions." << "\n";
	std::cout << "f2index px py pz data int(ds*data)"<< "\n";
	f2.getElements(); //returns elements
	f2.getDs(); 
	f2.measure();
	f2.integrate(); //uses elementsMagnitude and values
	// f2.minmax(); //moved above, just after b inverse calculation, else ds/B gives more max than min!
	int extrema;
	cout << "f2 has "<<f2.mincount<<" minima and "<< f2.maxcount << " maxima\n";
	polymin = polymin + f2.mincount;
	polymax = polymax + f2.maxcount;
	File6 << polymin - f2.mincount<<"\n";
	File7 << polymax - f2.maxcount<<"\n";

	cout <<" minima: "<< f2.minima.size() <<"\n";
	 for (int m = 0; m < f2.minima.size(); m++)
	 {
	 extrema = f2.minima[m];
	 if ((extrema > 0)&&(extrema < f2.size()-1))
	 {
         File4 << f2.getPositions()[extrema].component1 << " ";
         File4 << f2.getPositions()[extrema].component2 << " ";
         File4 << f2.getPositions()[extrema].component3;
	 File4 << " "<<f2.getData()[extrema-1]<<" "<<f2.getData()[extrema]<<" "<<f2.getData()[extrema+1];
	 File4 << " "<<2*(f2.getData()[extrema] - f2.getData()[extrema-1])/(f2.getData()[extrema]+f2.getData()[extrema-1]);
	 File4 << " "<<2*(f2.getData()[extrema] - f2.getData()[extrema+1])/(f2.getData()[extrema]+f2.getData()[extrema+1]);
	 }
	 else
	 {
         File4 << f2.getPositions()[extrema].component1 << " ";
         File4 << f2.getPositions()[extrema].component2 << " ";
         File4 << f2.getPositions()[extrema].component3;
	 File4 << " "<<f2.getData()[extrema]<<" "<<f2.getData()[extrema]<<" "<<f2.getData()[extrema];
	 File4 << " 0.0 0.0";
	 }
	 File4 <<" "<<f2.getData()[extrema]<<" "<< m-f2.GlobMinIndex <<"\n";
	 }


	 cout <<" maxima: "<< f2.maxima.size() <<"\n";
         for (int m = 0; m < f2.maxima.size(); m++)
         {
	 extrema = f2.maxima[m];
         if ((extrema > 0)&&(extrema < f2.size()-1))
         {
         File5 << f2.getPositions()[extrema].component1 << " ";
         File5 << f2.getPositions()[extrema].component2 << " ";
         File5 << f2.getPositions()[extrema].component3;
         File5 << " "<< f2.getData()[extrema-1]<<" "<<f2.getData()[extrema]<<" "<<f2.getData()[extrema+1];
	 File5 << " "<<2*(f2.getData()[extrema] - f2.getData()[extrema-1])/(f2.getData()[extrema]+f2.getData()[extrema-1]);
         File5 << " "<<2*(f2.getData()[extrema] - f2.getData()[extrema+1])/(f2.getData()[extrema]+f2.getData()[extrema+1]);
	 }
         else
         {
         File5 << f2.getPositions()[extrema].component1 << " ";
         File5 << f2.getPositions()[extrema].component2 << " ";
         File5 << f2.getPositions()[extrema].component3;
         File5 << " "<<f2.getData()[extrema]<<" "<<f2.getData()[extrema]<<" "<<f2.getData()[extrema];
         File5 << " 0.0 0.0";
         }
	 File5 << " "<<f2.getData()[extrema]<<" "<< m - f2.GlobMaxIndex <<"\n";
         }

		for (int l = 0; l < f2.size(); l++)
		{
			if ((l>0)&&(l<f2.size()))
			{
		//	File << l << " " << f2.getPositions()[l].component1 << " ";
		//	File << f2.getPositions()[l].component2 << " " << f2.getPositions()[l].component3;
		//	File << " "  << f2.getData()[l];
		//	File << " " << f2.getElements()[l];
		//	File << " " << f2.getDs()[l];
		//	File << " " << f2.measure()[l];
		//	File << " " << f2.integrate()[l];
			}
		}
	Fieldline f3 = f2.interpolate(3,100); // Interpolate f2 to fixed ds
	fEx.getDs(); // creates differential elements 
	fEx.measure(); // integrates over elements
	fEy.getDs();
	fEy.measure();
	fEz.getDs();
	fEz.measure();
	Fieldline fExint = fEx.interpolate(3,100); // Interpolate fEx to fixed ds
    Fieldline fEyint = fEy.interpolate(3,100); // Interpolate fEy to fixed ds
    Fieldline fEzint = fEz.interpolate(3,100); // Interpolate fEz to fixed ds
	std::cout << "Interpolated Field line size: "<<f3.size() << "\n";
	if (f3.size() != 100)
	 {cout<<"Findme:interpolated field line not right!";}
	//File2 << "f3index px py pz f3data(px,py,pz)  f3int(ds*data)"<< "\n";
	int j = 0; // locates index within f2
	float t=0.0; // locates t between points in f2
	int k = 0;
	cout<<"Testing f3"<<"\n";
	cout<<"f3.getPositions()[k].component1= "<<f3.getPositions()[k].component1<<"\n";
	std::cout <<"Before print loop, f3.size()= "<<f3.size()<<"\n";
	for(k = 0; k < f3.size(); k++)
	{
		File2 << k << " " << f3.getPositions()[k].component1;
		File2 <<      " " << f3.getPositions()[k].component2;
		File2 <<      " " << f3.getPositions()[k].component3;
		File2 <<      " " << f3.getData()[k];
		j = f3.getNearest()[k]; //nearest lower vertex in f2
		t = f3.getTlocal()[k]; //tlocal between nearest and nearest+1 in f2
		File2 <<      " "<< f2.getIntegral(j)*(1.0-t)+t*f2.getIntegral(j) << "\n";
	}
	std::cout<<"\n"<<"Wrote field line to file"<<"\n";
	std::cout<< "Writing out integral \n";
	File3 << c0<<" "<<c1<<" "<<c2<< " "<< f2.integrate()[f2.size()-1]<<" "<<f2.mincount<< " "<< f2.maxcount<<"\n";
	
	int sze = fEx.size();
	std::cout<<"Writing electric field Ex to file\n";
        for(k = 0; k < sze; k++)
        {
		FileE << i << " "; // fline number
                FileE << float(k)/sze << " " << fEx.getPositions()[k].component1;
                FileE <<      " " << fEx.getPositions()[k].component2;
                FileE <<      " " << fEx.getPositions()[k].component3;
                FileE <<      " " << fEx.getData()[k]; //components of E
                FileE <<      " " << fEy.getData()[k];
                FileE <<      " " << fEz.getData()[k];
	        FileE << "\n";
        }

	}
	kameleon.close();
	return 0;
}
