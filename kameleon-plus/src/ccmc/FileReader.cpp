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

using namespace std;

namespace ccmc
{
	/**
	 * Default constructor. Does nothing.
	 */
	FileReader::FileReader()
	{
		current_file_id = NULL;
	}

	/**
	 * @param filename
	 * @return The CDF status of the open call.  CDF_OK is the standard successful status.
	 */
	long FileReader::open(const std::string& filename)
	{
		return openFile(filename);
	}



	/**
	 * Returns the CDFid value of the currently opened CDF file
	 * @return CDFid returned from the CDF library for currently selected file.
	 */
	long FileReader::getCurrentFileID()
	{
		return *current_file_id;
	}

	/**
	 * Closes the currently selected file.
	 * @return Status of close operation.
	 */
	long FileReader::close()
	{
		long status = closeFile();
		current_file_id = NULL;
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
	 * Returns the string representation of the variable referred to by variable_id
	 * @return String representation of the variable.
	 */
	std::string FileReader::getVariableName(long variable_id)
	{
		//first, check the current variableNames map.  fetching the variableNames from the file is expensive
		boost::unordered_map<long, std::string>::iterator iter = variableNames.find(variable_id);
		if (iter != variableNames.end())
			return (*iter).second;

		char variableNameBuffer[512];
		CDFstatus status = CDFgetzVarName((void *) current_file_id, variable_id, variableNameBuffer);
		long numElements;
		status = CDFgetzVarNumElements((void *) current_file_id, variable_id, &numElements);
		variableNameBuffer[numElements] = '\0';
		std::string variableName = variableNameBuffer;
		if (variableName != "")
		{
			variableNames[variable_id] = variableName;
		}
		return variableName;
	}

	/**
	 * @param variable
	 * @return
	 */
	bool FileReader::doesVariableExist(const std::string& variable)
	{

		//first, check the current variableIDs map.  fetching the variableID from the file is expensive
		boost::unordered_map<std::string, long>::iterator iter = variableIDs.find(variable);
		if (iter != variableIDs.end())
			return true;

		long variableNumber = CDFgetVarNum((void *) current_file_id, (char *) variable.c_str());
		//std::cout << "variableNumber: " << variableNumber << std::endl;
		if (variableNumber >= 0)
		{
			variableIDs[variable] = variableNumber;
			variableNames[variableNumber] = variable;
			return true;
		}

		return false;
	}

	/**
	 * Returns the current filename.
	 * @return The current filename.
	 */
	const std::string& FileReader::getCurrentFilename()
	{
		return this->current_filename;
	}

	/**
	 *@param attribute_id
	 *@return
	 */
	std::string FileReader::getGlobalAttributeName(long attribute_id)
	{
		char buffer[256];
		CDFgetAttrName((void *) current_file_id, attribute_id, buffer);
		std::string buffer_string = buffer;
		//cout << "Attribute Name: '" << buffer_string << "'" << endl;


		return buffer_string;
	}

	/**
	 *@return
	 */
	int FileReader::getNumberOfVariables()
	{
		long numVars;
		CDFgetNumzVars((void *) current_file_id, &numVars);
		return (int)numVars;
	}

	/**
	 * Helper method to store the global attributes in a map. This solves some issues with
	 * threaded operations on CDF files.
	 */
	void FileReader::initializeGlobalAttributes()
	{
		int numGAttributes = this->getNumberOfGlobalAttributes();
		//cout << "numGAttributes: " << numGAttributes << endl;
		for (int i = 0; i < numGAttributes; i++)
		{

			std::string gAttributeName = this->getGlobalAttributeName((long)i);
			//std::cout << "fetched: " << gAttributeName << std::endl;
			this->getGlobalAttribute(gAttributeName);
			this->getGlobalAttribute(i);
			//model->getGlobalAttribute(gAttributeName);
			//model->getGlobalAttribute(i);
		}
	}

	/**
	 * Helper method to initialize a map containing variable IDs.  This solves some issues
	 * with threaded operations on CDF files.
	 */
	void FileReader::initializeVariableIDs()
	{
		int numVariables = this->getNumberOfVariables();
		//cout << "numVariables: " << numVariables << endl;
		char variableName[512];
		for (int i = 0; i < numVariables; i++)
		{
			CDFgetzVarName((void *) current_file_id, i, variableName);
			std::string variableNameString = variableName;
			variableIDs[variableNameString] = i;

		}
	}

	/**
	 * Helper method to initialize a variable names map. This solves some issues with
	 * threaded operations on CDF files.
	 */
	void FileReader::initializeVariableNames()
	{
		int numVariables = this->getNumberOfVariables();
		//cout << "numVariables: " << numVariables << endl;
		char variableName[512];
		for (int i = 0; i < numVariables; i++)
		{
			CDFgetzVarName((void *) current_file_id, i, variableName);
			std::string variableNameString = variableName;
			variableNames[(long)i] = variableNameString;

		}
	}

	/**
	 * Gets the number of variable attributes.
	 * @return The number of variable attributes in the opened file.
	 */
	int FileReader::getNumberOfVariableAttributes()
	{
		long numVAttributes;

		CDFgetNumvAttributes((void *) current_file_id, &numVAttributes);
		return (int)numVAttributes;
	}

	/**
	 * @param attribute_id
	 * @return String representing the name of the attribute specified by attribute_id
	 */
	std::string FileReader::getVariableAttributeName(long attribute_id)
	{
		char buffer[256];
		CDFgetAttrName((void *) current_file_id, attribute_id, buffer);
		std::string buffer_string = buffer;
		//cout << "Attribute Name: '" << buffer_string << "'" << endl;
		return buffer_string;
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

	/**
	 * @param variable
	 * @return
	 */
	long FileReader::getNumberOfRecords(const std::string& variable)
	{
		//std::cout << "reading " << variable << std::endl;
		//get variable number
		long variableNum = CDFgetVarNum((void *) current_file_id, (char *) variable.c_str());

		return getNumberOfRecords(variableNum);
	}

	/**
	 * @param variable_id
	 * @return
	 */
	long FileReader::getNumberOfRecords(long variable_id)
	{
		long counts[1];
		//get dim sizes
		CDFgetzVarDimSizes((void *) current_file_id, variable_id, counts);
		return counts[0];
	}

	/**
	 * @return
	 */
	std::vector<std::string> FileReader::getVariableAttributeNames()
	{
		std::vector<std::string> attributeNames;
		long numAttributes;
		CDFgetNumAttributes((void *) current_file_id, &numAttributes);
		char name[512];
		long attrScope;
		long maxgEntry;
		long maxrEntry;
		long maxzEntry;

		for (int i = 0; i < numAttributes; i++)
		{
			std::string value = "";
			CDFinquireAttr((void *) current_file_id, i,name, &attrScope, &maxgEntry, &maxrEntry, &maxzEntry);
			//CDFgetAttrName((void *) current_file_id, i, buffer);
			if (attrScope == VARIABLE_SCOPE)
			{
				value = name;
				attributeNames.push_back(value);
			}
		}
		return attributeNames;
	}

	/**
	 * Destructor
	 */
	FileReader::~FileReader()
	{
		if ((void *) current_file_id != NULL)
			close();
	}
}
