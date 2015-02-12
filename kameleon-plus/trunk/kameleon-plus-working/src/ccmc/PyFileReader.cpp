/*
 * PyFileReader.cpp
 *
 *  Created on: March 5, 2015
 *      Author: Asher Pembroke
 */
// #include "config.h"
#ifndef HAVE_PYTHON
#define HAVE_PYTHON
#endif /* HAVE_PYTHON */
#ifdef PYFILEREADER_
#include <boost/python.hpp>
#include "Kameleon-plus-Config.h"
#include "PyFileReader.h"
#include <string>
#include <vector>
#include <deque>
#include <iostream>
#include <fstream>
#include <queue>

using namespace boost::python;

namespace ccmc
{

	PyFileReader::PyFileReader()
	{
		this->fileReader = NULL;
	}

	long PyFileReader::open(const std::string& filename)
	{

		//check the file
		//std::cout << "First, checking if the file is a CDF file" << std::endl;
		if (this->fileReader == NULL)
			this->fileReader = new CDFFileReader();
		else
		{
			this->close();
			//delete this->fileReader;
			this->fileReader = new CDFFileReader();
		}
		long status = fileReader->open(filename);
		if (status == FileReader::OK)
		{
//			std::cerr << "Initialized a CDF file reader" << std::endl;
			//this->fileReader = fileReader;
			return status;
		}

#ifdef HAVE_HDF5 

		// std::cout << "Checking if the file is an HDF5 file" << std::endl;
		delete fileReader;

		this->fileReader = new HDF5FileReader::HDF5FileReader();

		status = fileReader->open(filename);
		// std::cerr << "opened HDF5 file. status: " << status << std::endl;
		if (status == FileReader::OK)
		{
			// std::cerr << "Initialized an HDF5 file reader" << std::endl;
			return status;
		} 

#endif /* HAVE_HDF5 */

#ifdef HAVE_PYTHON
		std::cout << "Checking if the file can be read by a python embedded reader" << std::endl;
		delete fileReader;
		
		Py_Initialize();

		try {
			// start here

			// put pyreader build path in Kameleon-plus-Config.h
			PyRun_SimpleString("import os,sys\nsys.path.append('/Users/apembrok/git/ccmc-software/kameleon-plus/trunk/kameleon-plus-working/src/ccmc/pyreaders')\n");
			PyRun_SimpleString("sys.path.append('/Users/apembrok/git/ccmc-software/kameleon-plus/trunk/kameleon-plus-working/src/ccmc/pyreaders/build')\n");
			PyRun_SimpleString("import pyKameleon\n");
			PyRun_SimpleString("print os.getcwd()\n");
			std::cout <<"Importing testReader" << std::endl;
		    object testReader_module = import("testReader");
		    std::cout <<"retrieving pyReader object"<< std::endl;
		    object py_reader_class = testReader_module.attr("pyFileReader");
		    object py_reader_obj = py_reader_class();
		    std::cout <<"Extracting and assigning ccmc::FileReader pointer from pyFileReader" << std::endl;
		    this->fileReader = extract< ccmc::FileReader* >(py_reader_obj);
		    std::cout <<"opening file"<< std::endl;
		    status = fileReader->open(filename);
		    if (status == FileReader::OK)
		    {
		    	std::cout <<"Successful read!"<< std::endl;
		    	return status;
		    }
		} catch (error_already_set) {
			PyErr_Print();
		}

#endif /* HAVE_PYTHON */

		return status;
	}



	std::vector<float>* PyFileReader::getVariable(const std::string& variable)
	{
		return fileReader->getVariable(variable);
	}

	std::vector<float>* PyFileReader::getVariable(long variable)
	{
		return fileReader->getVariable(variable);
	}

	std::vector<float>* PyFileReader::getVariable(const std::string& variable, long startIndex, long count)
	{
		return fileReader->getVariable(variable, startIndex, count);
	}

	std::vector<float>* PyFileReader::getVariable(long variable, long startIndex, long count)
	{
		return fileReader->getVariable(variable, startIndex, count);
	}

	float PyFileReader::getVariableAtIndex(const std::string& variable, long index)
	{
		return fileReader->getVariableAtIndex(variable, index);
	}

	float PyFileReader::getVariableAtIndex(long variable_id, long index)
	{
		return fileReader->getVariableAtIndex(variable_id, index);
	}

	std::vector<int>* PyFileReader::getVariableInt(const std::string& variable)
	{
		return fileReader->getVariableInt(variable);
	}

	int PyFileReader:: getVariableIntAtIndex(const std::string& variable, long index)
	{
		return fileReader->getVariableIntAtIndex(variable, index);
	}

	int PyFileReader:: getNumberOfGlobalAttributes()
	{
		return fileReader->getNumberOfGlobalAttributes();
	}

	int PyFileReader:: getNumberOfVariables()
	{
		return fileReader->getNumberOfVariables();
	}

	int PyFileReader:: getNumberOfVariableAttributes()
	{
		return fileReader->getNumberOfVariableAttributes();
	}

	long PyFileReader:: getNumberOfRecords(const std::string& variable)
	{
		return fileReader->getNumberOfRecords(variable);
	}

	long PyFileReader:: getNumberOfRecords(long variable_id)
	{
		return fileReader->getNumberOfRecords(variable_id);
	}

	long PyFileReader:: getVariableID(const std::string& variable)
	{
		return fileReader->getVariableID(variable);
	}

	std::string PyFileReader::getVariableName(long variable_id)
	{
		return fileReader->getVariableName(variable_id);
	}

	Attribute PyFileReader::getGlobalAttribute(long i)
	{
		return fileReader->getGlobalAttribute(i);
	}

	std::string PyFileReader::getGlobalAttributeName(long attribute_id)
	{
		return fileReader->getGlobalAttributeName(attribute_id);
	}

	std::string PyFileReader::getVariableAttributeName(long attribute_id)
	{
		return fileReader->getVariableAttributeName(attribute_id);
	}

	Attribute PyFileReader::getGlobalAttribute(const std::string& attribute)
	{
		return fileReader->getGlobalAttribute(attribute);
	}

	Attribute PyFileReader::getVariableAttribute(const std::string& variable, const std::string& attribute)
	{
		return fileReader->getVariableAttribute(variable, attribute);
	}

	std::vector<std::string> PyFileReader::getVariableAttributeNames()
	{
		return fileReader->getVariableAttributeNames();
	}

	bool PyFileReader::doesAttributeExist(const std::string& attribute)
	{

#ifdef HAVE_PYTHON
		try{
			std::cout <<" c++ seeing if attribute exists" << std::endl;
			return fileReader->doesAttributeExist(attribute);
			} catch (error_already_set) {
			PyErr_Print();
		}
#endif HAVE_PYTHON

		return fileReader->doesAttributeExist(attribute);
		
	}

	bool PyFileReader::doesVariableExist(const std::string& variable)
	{
		return fileReader->doesVariableExist(variable);
	}

	long PyFileReader:: close()
	{
		long status = fileReader->close();
		delete fileReader;
		fileReader = NULL;
		return status;
	}

	const std::string& PyFileReader::getCurrentFilename()
	{
		return fileReader->getCurrentFilename();
	}

	PyFileReader::~PyFileReader()
	{
		if (fileReader != NULL)
			close();

#ifdef HAVE_PYTHON
		// ToDo: Find out what happens if multiple pyReaders are used
		if (Py_IsInitialized())
		{
			std::cout << "Closing Python interpreters" << std::endl;
			Py_Finalize();
		}
		
#endif /* HAVE_PYTHON */
	}

	void PyFileReader::initializeVariableIDs()
	{
		fileReader->initializeVariableIDs();
	}

	void PyFileReader::initializeVariableNames()
	{
		fileReader->initializeVariableNames();
	}

}
#endif
