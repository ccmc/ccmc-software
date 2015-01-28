/**
* Minimal test of ccmc library
*/

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
#include <vector>

using namespace std;
using namespace ccmc;

/**
 * main_test <inputfile> 
 */
int main (int argc, char * argv[])
{

	if ((argc != 2))
	{
		cout << "You must point me to a kameleon-compatible cdf or hdf5 file\n"
				<<"\t ./main_test path/to/kameleon/converterd/file.cdf\n"
				<< endl;
		exit(1);
	}
	else{
		string filename = argv[1];
		cout << "input file: "<< filename;
	
	

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

		cout<<"initializing interpolator (takes a few seconds)... " << endl;
		LFMInterpolator interpolator(lfm);
		cout<<"done.\n";


		cout <<" Finished. Deleting lfm\n";
		delete lfm;

		
		}
return 0;
}


