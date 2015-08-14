/*
 * FileReader.cpp
 *
 *  Created on: Apr 22, 2009
 *      Author: David Berrios
 */
// #include "config.h"

#include "kameleon-plus-Config.h"
#include "Constants.h"
#include "FileReader.h"
#include "CDFFileReader.h"
#include "HDF5FileReader.h"
#include "GeneralFileReader.h"
#include <boost/python.hpp> //Todo:put ifdef here
#include <string>
#include <vector>
#include <deque>
#include <iostream>
#include <fstream>
#include <queue>
#include <sstream>

namespace bp = boost::python; //Todo:put ifdef here

namespace ccmc
{
	namespace pyglobals{
		bool PYTHON_IS_INITIALIZED = false;
	}

	/**
	 * sets fileReader pointer to NULL
	 */

	GeneralFileReader::GeneralFileReader()
	{
		this->fileReader = NULL;
	}

	/**
	 * @brief Attempts to open using CDFFileReader, falls back to HDF5FileReader (if HAVE_HDF5 is defined)
	 * 
	 * TODO: Add support for embedded python FileReader as third option.
	 * @param filename Name of the file to be read. Note: no extension checking is done at this time.
	 * @return status
	 */ 

	long GeneralFileReader::open(const std::string& filename)
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
		// std::cout << "Checking if the file can be read by a python embedded reader" << std::endl;
		delete fileReader;
		// std::cout <<"deleted filereader"<< std::endl;
		// std::cout <<"ccmc directory:"<< CCMC_DIR << std::endl;

		if (ccmc::pyglobals::PYTHON_IS_INITIALIZED != true)
			{
				std::cout <<"python initializing.." << std::endl;
				Py_Initialize();
				ccmc::pyglobals::PYTHON_IS_INITIALIZED = true;
			}
		
		bp::object main = bp::import("__main__");
		this->python_namespace = main.attr("__dict__");

		try {
			//set python variable ccmc_path = ${KAMELEON_SRC_DIR}/ccmc/
			std::string path_string("ccmc_path = \'");
			path_string += CCMC_DIR; //set by CMake via kameleon-plus-Config 
			path_string += "\'\n";
			bp::exec(path_string.c_str(),this->python_namespace); 

			//set paths to find pyreaders and pyKameleon module
			bp::exec(
				"import os,sys\n"
				"sys.path.append(ccmc_path)\n"
				"sys.path.append(ccmc_path +\'../../lib/ccmc/\')\n"
				"sys.path.append(ccmc_path +\'pyreaders/\')\n"
				,this->python_namespace
			);

			//create factory object that generates a new FileReader subclass from filename
			std::string run_string("from pyreaders import testReader\n");
			run_string+= "factory = testReader.FileReaderFactory(\'"; 
			run_string+= filename; 
			run_string+= "\')\n";
			bp::exec(run_string.c_str(),this->python_namespace);
			bp::exec("python_reader = factory.createPyReader()\n",this->python_namespace);


			// retrieve file_reader object from python and cast to FileReader pointer
		    bp::object file_reader_obj = this->python_namespace["python_reader"];
		    this->fileReader = bp::extract< ccmc::FileReader* >(file_reader_obj);
		   
		   	// open the file
		    status = fileReader->open(filename);

		    if (status == FileReader::OK)
		    {
		    	// std::cout <<"Successful read!"<< std::endl;
		    	return status;
		    }
		} catch (bp::error_already_set) {
			PyErr_Print();
		}

#endif /* HAVE_PYTHON */

		return status;
	}



	std::vector<float>* GeneralFileReader::getVariable(const std::string& variable)
	{
		return fileReader->getVariable(variable);
	}

	std::vector<float>* GeneralFileReader::getVariable(long variable)
	{
		return fileReader->getVariable(variable);
	}

	std::vector<float>* GeneralFileReader::getVariable(const std::string& variable, long startIndex, long count)
	{
		return fileReader->getVariable(variable, startIndex, count);
	}

	std::vector<float>* GeneralFileReader::getVariable(long variable, long startIndex, long count)
	{
		return fileReader->getVariable(variable, startIndex, count);
	}

	float GeneralFileReader::getVariableAtIndex(const std::string& variable, long index)
	{
		return fileReader->getVariableAtIndex(variable, index);
	}

	float GeneralFileReader::getVariableAtIndex(long variable_id, long index)
	{
		return fileReader->getVariableAtIndex(variable_id, index);
	}

	std::vector<int>* GeneralFileReader::getVariableInt(const std::string& variable)
	{
		return fileReader->getVariableInt(variable);
	}

	int GeneralFileReader:: getVariableIntAtIndex(const std::string& variable, long index)
	{
		return fileReader->getVariableIntAtIndex(variable, index);
	}

	int GeneralFileReader:: getNumberOfGlobalAttributes()
	{
		return fileReader->getNumberOfGlobalAttributes();
	}

	int GeneralFileReader:: getNumberOfVariables()
	{
		return fileReader->getNumberOfVariables();
	}

	int GeneralFileReader:: getNumberOfVariableAttributes()
	{
		return fileReader->getNumberOfVariableAttributes();
	}

	long GeneralFileReader:: getNumberOfRecords(const std::string& variable)
	{
		return fileReader->getNumberOfRecords(variable);
	}

	long GeneralFileReader:: getNumberOfRecords(long variable_id)
	{
		return fileReader->getNumberOfRecords(variable_id);
	}

	long GeneralFileReader:: getVariableID(const std::string& variable)
	{
		return fileReader->getVariableID(variable);
	}

	std::string GeneralFileReader::getVariableName(long variable_id)
	{
		return fileReader->getVariableName(variable_id);
	}

	Attribute GeneralFileReader::getGlobalAttribute(long i)
	{
		return fileReader->getGlobalAttribute(i);
	}

	std::string GeneralFileReader::getGlobalAttributeName(long attribute_id)
	{
		return fileReader->getGlobalAttributeName(attribute_id);
	}

	std::string GeneralFileReader::getVariableAttributeName(long attribute_id)
	{
		return fileReader->getVariableAttributeName(attribute_id);
	}

	Attribute GeneralFileReader::getGlobalAttribute(const std::string& attribute)
	{
		return fileReader->getGlobalAttribute(attribute);
	}

	Attribute GeneralFileReader::getVariableAttribute(const std::string& variable, const std::string& attribute)
	{
		return fileReader->getVariableAttribute(variable, attribute);
	}

	std::vector<std::string> GeneralFileReader::getVariableAttributeNames()
	{
		return fileReader->getVariableAttributeNames();
	}

	bool GeneralFileReader::doesAttributeExist(const std::string& attribute)
	{

		return fileReader->doesAttributeExist(attribute);
		
	}

	bool GeneralFileReader::doesVariableExist(const std::string& variable)
	{
		return fileReader->doesVariableExist(variable);
	}

	long GeneralFileReader:: close()
	{
#ifdef HAVE_PYTHON
		// ToDo: Find out what happens if multiple pyReaders are used
		if (Py_IsInitialized())
		{
			// std::cout << "GeneralFileReader:: close()" << std::endl;
			long status; 
			try{
				status = fileReader->close();
			} catch (bp::error_already_set) {
				PyErr_Print();
			}
			
			fileReader = NULL;
			return status;
		}
		
#endif /* HAVE_PYTHON */


		long status = fileReader->close();
		delete fileReader;
		fileReader = NULL;
		return status;
	}

	const std::string& GeneralFileReader::getCurrentFilename()
	{
		return fileReader->getCurrentFilename();
	}

	GeneralFileReader::~GeneralFileReader()
	{
		if (fileReader != NULL)
			// std::cout << "~GeneralFileReader() calling GeneralFileReader::close()" << std::endl;
			close();

#ifdef HAVE_PYTHON
		// ToDo: Find out what happens if multiple pyReaders are used..
		// if (ccmc::pyglobals::PYTHON_IS_INITIALIZED == true)
		// 	{
		// 		if (Py_IsInitialized())
		// 		{
		// 			std::cout << "python finalizing..." << std::endl;
		// 			Py_Finalize();
		// 			ccmc::pyglobals::PYTHON_IS_INITIALIZED = false;
		// 		}
		// 	}
		
#endif /* HAVE_PYTHON */
	}

	/**
	* calls fileReader's initializeVariableIDs()
	*/
	void GeneralFileReader::initializeVariableIDs()
	{
		fileReader->initializeVariableIDs();
	}

	/**
	* calls fileReader's initializeVariableNames()
	*/
	void GeneralFileReader::initializeVariableNames()
	{
		fileReader->initializeVariableNames();
	}




}
