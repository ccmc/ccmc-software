/*
 * FileReader.cpp
 *
 *  Created on: Apr 22, 2009
 *      Author: David Berrios
 */
// #include "config.h"
#include "Kameleon-plus-Config.h"
#include "FileReader.h"
#include "CDFFileReader.h"
#include "HDF5FileReader.h"
#include "GeneralFileReader.h"
#include <string>
#include <vector>
#include <deque>
#include <iostream>
#include <fstream>
#include <queue>


namespace ccmc
{

	GeneralFileReader::GeneralFileReader()
	{
		this->fileReader = NULL;
	}

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
		else 
		{
			// std::cout << "Checking if the file is an HDF5 file" << std::endl;
			delete fileReader;

			this->fileReader = new HDF5FileReader::HDF5FileReader();

			long status = fileReader->open(filename);
			// std::cerr << "opened HDF5 file. status: " << status << std::endl;
			if (status == FileReader::OK)
			{
				// std::cerr << "Initialized an HDF5 file reader" << std::endl;
				return status;
			} else
				return FileReader::OPEN_ERROR;

		}
#endif /* HAVE_HDF5 */

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
			close();
	}

	void GeneralFileReader::initializeVariableIDs()
	{
		fileReader->initializeVariableIDs();
	}

	void GeneralFileReader::initializeVariableNames()
	{
		fileReader->initializeVariableNames();
	}




}
