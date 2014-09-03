// Hello.cpp

#include <boost/python.hpp>
#include <../FileReader.h>
#include <vector>
#include "testVirtual.cpp"
#include <string>


using namespace boost::python;
// using namespace ccmc;

int main(int, char **) {
	

	Py_Initialize();

	// try accessing baz module
	std::cout << "accessing a c++ class via python module..." << std::endl;
	try {
		PyRun_SimpleString("import sys\nsys.path.append('../')");
		PyRun_SimpleString("import sys\nsys.path.append('.')");

	    object py_readers = import("testReader");
	    object testSampleReader = py_readers.attr("testSampleReader");
	    ccmc::FileReader* testSampleReaderPtr = extract<ccmc::FileReader*>(testSampleReader); 
	    testSampleReaderPtr->open("Example_data_file");
	    std::cout <<"accessing python-derived getVariableAtIndex" << std::endl;
	    float val2 = testSampleReaderPtr->getVariableAtIndex("rho", 10);
	    std::cout << "c++ extracted V[10]:" << val2 << std::endl; 
	    string filename = testSampleReaderPtr->getCurrentFilename();
	    std::cout << "current file name = "<< filename << std::endl;

	} catch (error_already_set) {
		PyErr_Print();
	}

	Py_Finalize();
	return 0;
}