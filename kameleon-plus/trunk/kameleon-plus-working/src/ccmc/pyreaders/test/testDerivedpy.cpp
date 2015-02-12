// Hello.cpp
// #include "testVirtual.cpp"

#include <boost/python.hpp>
#include <FileReader.h>
#include <vector>
#include <string>


using namespace boost::python;
// using namespace ccmc;

int main(int, char **) {
	

	Py_Initialize();

	std::cout << "accessing a c++ class via python module..." << std::endl;
	try {
		PyRun_SimpleString("import os,sys\nsys.path.append('/Users/apembrok/git/ccmc-software/kameleon-plus/trunk/kameleon-plus-working/src/ccmc/pyreaders')\n");
		PyRun_SimpleString("import os,sys\nsys.path.append('/Users/apembrok/git/ccmc-software/kameleon-plus/trunk/kameleon-plus-working/src/ccmc/pyreaders/build')\n");
		PyRun_SimpleString("sys.path.append('.')\n");
		PyRun_SimpleString("print os.getcwd()\n");
		std::cout <<"Importing testReader" << std::endl;
	    object testReader_module = import("testReader");
	    std::cout <<"Extracting pyFileReader object" << std::endl;
	    object testSampleReader = testReader_module.attr("pyFileReader");
	    object newObject = testSampleReader();
	    std::cout <<"Extracting ccmc::FileReader pointer from pyFileReader" << std::endl;
	    ccmc::FileReader* testSampleReaderPtr = extract<ccmc::FileReader*>(newObject); 
	    testSampleReaderPtr->open("Example_data_file");
	    std::cout <<"accessing python-derived getVariableAtIndex" << std::endl;
	    float val2 = testSampleReaderPtr->getVariableAtIndex("rho", 10);
	    std::cout << "c++ extracted V[10]:" << val2 << std::endl; 
	    std::string filename = testSampleReaderPtr->getCurrentFilename();
	    std::cout << "current file name = "<< filename << std::endl;

	} catch (error_already_set) {
		PyErr_Print();
	}

	Py_Finalize();
	return 0;
}