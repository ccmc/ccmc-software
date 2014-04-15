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
#include <ccmc/Point3f.h>
#include <ccmc/Vector.h>
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
		cout << "integrator <filename> <variable> c0 c1 c2" << endl;
		exit(1);
	}

	filename = argv[1];
	variable = argv[2];
	c0 = boost::lexical_cast<float>(argv[3]);
	c1 = boost::lexical_cast<float>(argv[4]);
	c2 = boost::lexical_cast<float>(argv[5]);

	cout << "This program will compute the integral for a fieldline of the specified variable" << std::endl;
	Kameleon kameleon; //creates a kameleon object capable of working with any of the ccmc-supported models
	kameleon.open(filename);
	kameleon.loadVectorVariable("b"); //see the namespace ccmc::strings::variables for possible inputs
	kameleon.loadVectorVariable("e");

	Tracer tracer(&kameleon); // Sets the interpolator (based on the kameleon object) to be used for tracing
	tracer.setMaxIterations(20000); //Maximum number of points in the fieldline
	tracer.setInnerBoundary(2.5f); //radius where fieldlines should stop (usually larger than the model's inner boundary)
	tracer.setDn(.2f); //step size relative to the cell size

	clock_t start, finish;

	cout << "Initializing field line\n";
	Fieldline fieldline;

	/*
	 * Supported tracers: input position is cartesian (x,y,z) unless the model is Enlil (r,theta,phi)
	 * 	bidirectionalTrace(string variable, float p0, float p1, float p2)
	 * 	bidirectionalTraceWithDipole(string variable, float p0, float p1, float p2)
	 * 	unidirectionalTrace(string variable, float p0, float p1, float p2, Direction) where Direction must be keyword FORWARD or REVERSE
	 * 	unidirectionalTraceWithDipole(string variable, float p0, float p1, float p2, Direction)
	 */
	cout << "Bidirectional trace beginning at ("<< c0 << ","<<c1<<","<<c2<<")"<<endl;
	start = clock();
	fieldline = tracer.bidirectionalTrace("b",c0,c1,c2);
	finish = clock();
	float elapsed_time = ((double) finish - (double) start) / CLOCKS_PER_SEC;
	cout << "Fieldline trace took "<< elapsed_time << " seconds\n"<<endl;
	cout << "After bidrectional trace\n";
	cout << "Points in fieldline:"<<fieldline.size()<<endl;


	//You can retrieve the kameleon object's interpolator from the tracer, thusly:
	Interpolator * interpolator = tracer.interpolator; //interpolator will be deleted via the Tracer's destructor
	//Or you can make a new one, which is useful for doing interpolations in parallel:
	//Interpolator * interpolator = kameleon.createNewInterpolator(); //but make sure you delete it when done

	Fieldline fieldlineWithVariable;
	Point3f p, eField;
	float datum, pEx, pEy, pEz;

	for (int i = 0; i < fieldline.size(); i++)
	{
		p = fieldline.getPosition(i);
		datum = interpolator->interpolate(variable,p.component1,p.component2,p.component3);
		fieldlineWithVariable.insertPointData(p,datum);
		eField.component1 = interpolator->interpolate("ex",p.component1,p.component2,p.component3);
		eField.component2 = interpolator->interpolate("ey",p.component1,p.component2,p.component3);
		eField.component3 = interpolator->interpolate("ez",p.component1,p.component2,p.component3);
		fieldlineWithVariable.insertVectorData(eField); // eField in [mV/m]
	}

	vector<float> segmentLengths = fieldlineWithVariable.getDs(); //computes line segments of fieldline [Re]
	vector<float> integral = fieldlineWithVariable.integrate(); //sum of individual segmentLengths * variable
	vector<float> fieldlinePotential = fieldlineWithVariable.integrateVector(); // integral(E dot dl)
	std::cout<<"integral result:"<< integral.back()<< "[" << variable << "* Re]"<<endl;
	std::cout<<"Fieldline potential:"<< fieldlinePotential.back()*(6.3781e3)<<"[V]\n"; //[V] = [Re*mV/m]*[1V/1000mV]*[6.3781e6 m/Re]

	kameleon.close();

	//delete interpolator; //required if you used kameleon.createNewInterpolator above

	std::cout << "finished" << std::endl;

	return 0;
}
