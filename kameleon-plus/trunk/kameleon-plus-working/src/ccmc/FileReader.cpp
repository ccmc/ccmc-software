/*
 * FileReader.cpp
 *
 *  Created on: Apr 22, 2009
 *      Author: David Berrios
 */

#include "FileReader.h"
#include <string>
#include <vector>
#include <deque>
#include <iostream>
#include <fstream>
#include <queue>
#ifdef HAVE_PYTHON
#include <boost/python.hpp> 
#endif
using namespace std;

namespace ccmc
{
	// A.Pembroke added these to avoid compiler linker errors
	const long FileReader::OK;
	const long FileReader::OPEN_ERROR;
	const long FileReader::FILE_DOES_NOT_EXIST;
	const long FileReader::VARIABLE_DOES_NOT_EXIST;
	const long FileReader::ATTRIBUTE_DOES_NOT_EXIST;
	const long FileReader::LOAD_FAILED;
	const long FileReader::UNABLE_TO_ALLOCATE_MEMORY;
	const long FileReader::VARIABLE_NOT_IN_MEMORY;
	const long FileReader::MODEL_NOT_SUPPORTED;
	const long FileReader::NOT_A_VALID_KAMELEON_FILE;

	/**
	 * Default constructor. Does nothing.
	 */
	FileReader::FileReader()
	{
		numGAttributes = 0;
		numVAttributes = 0;
	}


	/**
	 * @param filename
	 * @return The status of the open call.  OK is the standard successful status.
	 */
	long FileReader::open(const std::string& filename, bool readonly)
	{
#ifdef HAVE_PYTHON
		// std::cout << "c++ open() calling openFile()" << std::endl;
		namespace bp = boost::python;
		try {
			return openFile(filename, readonly);
		} catch (bp::error_already_set) {
			PyErr_Print();
			return FileReader::OPEN_ERROR;
		}
#else 
		return openFile(filename, readonly);
#endif /* HAVE_PYTHON */
		
	}

	void FileReader::setCurrentFilename(const std::string& filename)
	{
		// std::cout << "c++ setting current_filename to " << filename << std::endl;
		this->current_filename = filename;
	}

	/**
	 * Closes the currently selected file.
	 * @return Status of close operation.
	 */
	long FileReader::close()
	{
		// std::cout << "c++ close() calling closeFile()" << std::endl;
		long status = closeFile();
		current_filename = "";
		variableIDs.clear();
		variableNames.clear();
		gAttributes.clear();
		vAttributes.clear();
		gAttributeByID.clear();
		return status;
	}


	/**
	 * Returns the variable ID as a long. Using the variable ID wherever possible is significantly faster than the
	 * equivalent methods accepting the variable string.
	 * @return Status of the file operation.
	 */
	long FileReader::getVariableID(const std::string& variable)
	{
		//first, check the current variableIDs map.  fetching the variableID from the file is expensive
		boost::unordered_map<std::string, long>::iterator iter = variableIDs.find(variable);
		if (iter != variableIDs.end())
			return (*iter).second;
		else
		{
			//since we prefetched all of the variables on open, it shouldn't exist
			return -1;
		}

		/*std::cout << "getting variable id for: " << variable << std::endl;
		long variableNumber = CDFgetVarNum((void *) current_file_id, (char *) variable.c_str());
		//std::cout << "variableNumber: " << variableNumber << std::endl;
		if (variableNumber >= 0) //we want to keep the crappy negative ids
			variableIDs[variable] = variableNumber;

		return variableNumber;*/
	}
	/**
	 * Helper method to store the global attributes in a map. This solves some issues with
	 * threaded operations on CDF files.
	 */
	void FileReader::initializeGlobalAttributes()
	{
		//cout << "numGAttributes: " << numGAttributes << endl;
		for (int i = 0; i < numGAttributes; i++)
		{

			std::string gAttributeName = this->getGlobalAttributeName((long)i);
			//std::cout << "fetched: " << gAttributeName << std::endl;

			this->getGlobalAttribute(i);
			this->getGlobalAttribute(gAttributeName);
			//model->getGlobalAttribute(gAttributeName);
			//model->getGlobalAttribute(i);
		}
	}

	//need to update this to do this programatically. for now, we just
	//preload the most used attributes
	void FileReader::initializeVariableAttributes()
	{
		Attribute model_name = this->getGlobalAttribute("model_name");
		string model_name_string = model_name.getAttributeString();
		if (model_name_string == "open_ggcm" ||
				model_name_string == "ucla_ggcm")
		{
			this->getVariableAttribute("x","actual_min");
			this->getVariableAttribute("y","actual_min");
			this->getVariableAttribute("z","actual_min");
			this->getVariableAttribute("x","actual_max");
			this->getVariableAttribute("y","actual_max");
			this->getVariableAttribute("z","actual_max");
		}

	}

	void FileReader::addVariableName(const std::string& variable, long id){
		variableIDs[variable] = id;
		variableNames[id] = variable;
		return;
	}


	/**
	 * Destructor
	 */
	FileReader::~FileReader()
	{

	}
}
