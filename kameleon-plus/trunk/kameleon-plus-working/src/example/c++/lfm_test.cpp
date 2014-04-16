/*
 * LFM_test.cpp
 *
 *  Created on: Aug 5, 2013
 *      Author: Asher Pembroke
 *
 *  This program illustrates some simple ways of using the LFM interpolator.
 *  Note: The initialization phase is somewhat long - it sets up millions of polyhedral cells,
 *  then primes all of them for interpolation. The time spent actually interpolating is printed
 *  whenever the LFMInterpolator is destroyed.
 *
 */

#include <boost/lexical_cast.hpp>
#include <boost/tokenizer.hpp>
#include <boost/foreach.hpp>
#include <iostream>
#include <ccmc/LFM.h>
#include <ccmc/LFMInterpolator.h>
#include <ccmc/Interpolator.h>
#include <ccmc/Point3f.h>
#include <ccmc/Kameleon.h>
#include <ccmc/Tracer.h>
#include <string>
#include <fstream>
#include <iomanip>
#include "time.h"
#include <vector>

using namespace std;
using namespace ccmc;

/**
 * LFM <inputfile> variable x y z
 */
int main (int argc, char * argv[])
{
	std::string variables[] = {"bx", "by", "bz", "ex","ey", "ez", "ux", "uy","uz", "rho", "p"};


	if ((argc != 6) && (argc != 4))
	{
		cout << "You must use the following format to test LFM interpolation at a point in GSE coordinates:\n"
				<<"\t ./lfm path/to/kameleon/converterd/LFMfile.h5 variable x y z \n"
				<<"with variable set to: bx by bz ex ey ez ux uy uz rho p\n"
				<<"or, to interpolate all available lfm variables onto a list of input (single spaced) positions, use\n"
				<<"\t ./lfm path/to/kameleon/converterd/LFMfile.h5 path/to/positions.txt path/to/output/file.txt"
				<< endl;
		exit(1);
	}

	string filename = argv[1];

	cout<<"Creating LFM object... ";
	LFM * lfm = new LFM;

	cout<<"Opening LFM.\n";
	lfm->open(filename);

	/*
	 * Load variables (pressure,density and e-field already loaded)
	 * It is necessary to load the variables prior to interpolating
	 */
	lfm->loadVariable("bx");
	lfm->loadVariable("by");
	lfm->loadVariable("bz");
	lfm->loadVariable("ux");
	lfm->loadVariable("uy");
	lfm->loadVariable("uz");

	cout<<"initializing interpolator (takes a few seconds)... ";
	LFMInterpolator interpolator(lfm);
	cout<<"done.\n";

	float px,py,pz;
	float dx,dy,dz;

	if (argc == 6){

		string variable = argv[2];
		px = boost::lexical_cast<float>(argv[3]);
		py = boost::lexical_cast<float>(argv[4]);
		pz = boost::lexical_cast<float>(argv[5]);

		cout<<"Interpolating variable "<< variable <<" at ("<< px <<","<<py<<","<<pz<<")...\n";

		float variableOut = interpolator.interpolate(variable,px,py,pz, dx,dy,dz);

		std::cout<<variable << "("<< px <<","<<py<<","<<pz<<")="<<variableOut<<endl;
		std::cout<<"resolution at query point: "<<dx<<", "<<dy<<", "<<dz<<endl;
	}

	if (argc == 4){

		string positionsFile = argv[2];
		std::cout<<"Reading positions from file "<<positionsFile<<endl;

		std::vector<Point3f> seedPositions;

		std::ifstream seedsFileStream;
		seedsFileStream.open(positionsFile.c_str());

		if (!seedsFileStream.is_open())
		{
				std::cerr << "Error opening seeds position file " << positionsFile << "." << std::endl;
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
				Point3f p;
				std::string t;
				BOOST_FOREACH(t, tokens){
					if (count == 0){
						p.component1 = boost::lexical_cast<float>(t);
						count++;
					} else if (count == 1){
						p.component2 = boost::lexical_cast<float>(t);
						count++;
					} else {
						p.component3 = boost::lexical_cast<float>(t);
					}
				}

				if (count > 0){
					seedPositions.push_back(p);
				}
			}
		}

		string outputFileName = argv[3];
		std::ofstream outFile(outputFileName.c_str());
		outFile <<"bx by bz[T] ex ey ez[V/m] ux uy uz [m/s] rho [g/cc] p [Pa]\n";

		for (size_t i=0; i<seedPositions.size();i++){
			px = seedPositions[i].component1;
			py = seedPositions[i].component2;
			pz = seedPositions[i].component3;
			for (size_t j = 0; j<11; j++){
				outFile << std::scientific << interpolator.interpolate(variables[j],px,py,pz) <<" ";
			}
			outFile << endl;
		}
	}


	cout <<" Finished. Deleting lfm\n";
	delete lfm;

	return 0;

}

