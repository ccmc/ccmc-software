#include <iostream>
#include <boost/python.hpp>
#include <Python.h>

namespace py = boost::python;

int main(int, char **) {
    

    Py_Initialize();

    try {

        object main_module(handle<>(borrowed(PyImport_AddModule("__main__"))));
        object main_dictionary = main_module.attr("__dict__");
        object result = main_dictionary["result"];
        int result_value = extract<int>(result); //casts the result value as int

        std::cout << result_value << std::endl;

        main_dictionary["result"] = 20;

        PyRun_SimpleString("print result");
    }   catch (error_already_set) {
        PyErr_Print();
    }

    // try accessing module
    std::cout << "accessing a c++ class via python module..." << std::endl;
    try {
        initPointless(); // crucial! initializes Pointless
        PyRun_SimpleString("import Pointless");
        PyRun_SimpleString("print '4 + 5 = ' + str(Pointless.add_five(4))");

        // //importing matplotlib!
        // PyRun_SimpleString("import matplotlib as mpl");
        // PyRun_SimpleString("print mpl.__doc__");
    } catch (error_already_set) {
        PyErr_Print();
    }

    Py_Finalize();
    return 0;
}
