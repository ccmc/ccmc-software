/*
 * FileReader.cpp
 *
 *  Created on: Apr 22, 2009
 *      Author: David Berrios
 */

#include "FileReader.h"
#include <cdf.h>
#include <cdfdist.h>
#include <string>
#include <vector>
#include <deque>
#include <iostream>
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
	 * @param filename
	 * @return The status of the open call.  This method should be called from open().
	 */
	long FileReader::openFile(const std::string& filename)
	{
		long status;
		if (current_file_id != NULL)
		{
			close();
		}
		status = CDFopenCDF(filename.c_str(), &current_file_id);

		//cout << "current_file_id: " << current_file_id << endl;
		//cout << "testing open in FileReader class" << endl;
		return status;
	}

	/**
	 * Returns the CDFid value of the currently opened CDF file
	 * @return CDFid returned from the CDF library for currently selected file.
	 */
	CDFid FileReader::getCurrentFileID()
	{
		return current_file_id;
	}

	/**
	 * Closes the currently selected file.
	 * @return Status of close operation.
	 */
	long FileReader::close()
	{
		return closeFile();
	}

	/**
	 * Closes the currently selected file.  Call this from the close() method.
	 * @return Status of close operation.
	 */
	long FileReader::closeFile()
	{
		long status;
		if (current_file_id != NULL)
		{
			status = CDFcloseCDF(current_file_id);
			current_file_id = NULL;

		}
		variableIDs.clear();
		variableNames.clear();
		return status;
	}

	/**
	 * Returns a std::vector<float> containing the values of the selected variable
	 * stored in the selected file.
	 * @param variable
	 * @return std::vector<float> containing the values of the selected variable.
	 */
	std::vector<float>* FileReader::getVariable(const std::string& variable)
	{
		//std::cout << "reading " << variable << std::endl;
		//get variable number
		long variableNum = CDFgetVarNum(current_file_id, (char *) variable.c_str());

		long recStart = 0L;
		long recCount = 1L;
		long recInterval = 1L;
		long dimIndices[] = { 0 };
		long dimIntervals[] = { 1 };

		long dimSizes[1];
		//get dim sizes
		CDFgetzVarDimSizes(current_file_id, variableNum, dimSizes);
		float * buffer = new float[dimSizes[0]];
		CDFhyperGetzVarData(current_file_id, variableNum, recStart, recCount, recInterval, dimIndices, dimSizes,
				dimIntervals, buffer);
		//add data to vector type, and delete original array
		std::vector<float>* variableData = new std::vector<float>();
		variableData->reserve(dimSizes[0]);
		for (int i = 0; i < dimSizes[0]; i++)
		{
			variableData->push_back(buffer[i]);
		}

		delete[] buffer;
		//std::cout << "finished reading " << variable << std::endl;
		//std::cout << "size of variable: " << variableData.size() << std::endl;
		//std::cout << "dimSizes[0]: " << dimSizes[0] << std::endl;
		return variableData;
	}

	/**
	 * @param variable
	 * @return vector<int> containing the integer values of the variable
	 */
	std::vector<int>* FileReader::getVariableInt(const std::string& variable)
	{
		//std::cout << "reading " << variable << std::endl;
		//get variable number
		long variableNum = CDFgetVarNum(current_file_id, (char *) variable.c_str());

		long recStart = 0L;
		long recCount = 1L;
		long recInterval = 1L;
		long dimIndices[] = { 0 };
		long dimIntervals[] = { 1 };

		long dimSizes[1];
		//get dim sizes
		CDFgetzVarDimSizes(current_file_id, variableNum, dimSizes);
		int * buffer = new int[dimSizes[0]];
		CDFhyperGetzVarData(current_file_id, variableNum, recStart, recCount, recInterval, dimIndices, dimSizes,
				dimIntervals, buffer);
		//add data to vector type, and delete original array
		std::vector<int> * variableData = new std::vector<int>();
		variableData->reserve(dimSizes[0]);
		for (int i = 0; i < dimSizes[0]; i++)
		{
			variableData->push_back(buffer[i]);
		}

		delete[] buffer;
		//std::cout << "finished reading " << variable << std::endl;
		return variableData;
	}

	/**
	 * @param i The attribute number
	 * @return
	 */
	Attribute FileReader::getGlobalAttribute(int i)
	{

		long attrNum = i;
		long numElements;
		long dataType;

		//std::cout << "dataType: " << dataType << std::endl;


		CDFstatus status = CDFinquireAttrgEntry(current_file_id, attrNum, 0, &dataType, &numElements);
		if (dataType == CDF_CHAR)
		{
			std::string attributeValue = "NULL";
			char attributeBuffer[512];
			CDFgetAttrgEntry(current_file_id, attrNum, 0, attributeBuffer);
			//std::cout << "numElements: " << numElements << std::endl;
			//modelName[numElements] = '\0';
			attributeBuffer[numElements] = '\0';
			//std::cout << "status: " << status << std::endl;
			if (status == CDF_OK)
			{
				attributeValue = attributeBuffer;
				attributeValue = attributeValue.substr(0, numElements); //only use valid parts of char string
			}

			char attributeNameBuffer[512];

			//char * ctemp = new char[512];
			//strcpy(ctemp, attributeValue.c_str());
			//void * vtemp = (void *)ctemp;
			CDFgetAttrName(current_file_id, attrNum, attributeNameBuffer);
			Attribute attribute;
			attribute.setAttributeName(attributeNameBuffer);
			//std::cout << "attributeBuffer: " << attributeBuffer << endl;
			attribute.setAttributeValue(attributeValue);
			return attribute;
		} else if (dataType == CDF_INT4)
		{
			//int attributeValue = 0.f;
			int attributeBuffer;// = new int[1];

			CDFgetAttrgEntry(current_file_id, attrNum, 0, (void*) &attributeBuffer);
			//std::cout << "numElements: " << numElements << std::endl;
			//modelName[numElements] = '\0';
			//std::cout << "status: " << status << std::endl;
			if (status == CDF_OK)
			{
				//do nothing.  defaults to zero.
				//attributeValue = (int) attributeBuffer[0];
			}

			char attributeNameBuffer[512];

			CDFgetAttrName(current_file_id, attrNum, attributeNameBuffer);
			Attribute attribute;
			attribute.setAttributeName(attributeNameBuffer);
			attribute.setAttributeValue(attributeBuffer);
			return attribute;
		} else //CDF_FLOAT
		{
			//float attributeValue = 0.f;
			float attributeBuffer;// = new float[1];

			CDFgetAttrgEntry(current_file_id, attrNum, 0, (void *) &attributeBuffer);
			//std::cout << "numElements: " << numElements << std::endl;
			//modelName[numElements] = '\0';
			//std::cout << "status: " << status << std::endl;
			if (status == CDF_OK)
			{
				//do nothing.
				//attributeValue = (float) attributeBuffer[0];
			} else
			{
				std::cout << "attrNum: " << attrNum << " failed to read correctly." << std::endl;
			}

			char attributeNameBuffer[1024];
			CDFgetAttrName(current_file_id, attrNum, attributeNameBuffer);
			Attribute attribute;
			attribute.setAttributeName(attributeNameBuffer);
			attribute.setAttributeValue(attributeBuffer);
			return attribute;
		}

	}

	/**
	 * @param attribute
	 * @return
	 */
	Attribute FileReader::getGlobalAttribute(const std::string& attribute)
	{
		long attrNum = CDFgetAttrNum(current_file_id, (char *) attribute.c_str());
		return FileReader::getGlobalAttribute(attrNum);
	}

	/**
	 * @param variable
	 * @param vattribute
	 * @return
	 */
	Attribute FileReader::getVariableAttribute(const std::string& variable, const std::string& vattribute)
	{

		long variableNumber = CDFgetVarNum(current_file_id, (char *) variable.c_str());
		long attributeNumber = CDFgetAttrNum(current_file_id, (char *) vattribute.c_str());
		long dataType;
		CDFstatus status = CDFgetAttrzEntryDataType(current_file_id, attributeNumber, variableNumber, &dataType);

		//	std::cout << "FileReader::getVariableAttribute - datatype: " << dataType << " CDF_CHAR: " << CDF_CHAR << std::endl;
		if (dataType == CDF_CHAR)
		{
			char value[1024];
			long status = CDFgetAttrzEntry(current_file_id, attributeNumber, variableNumber, value);
			long numElements;
			status = CDFgetAttrzEntryNumElements(current_file_id, attributeNumber, variableNumber, &numElements);

			value[numElements] = '\0';
			//std::cout << "C: attributeValue (" << vattribute << "): " << value << std::endl;
			std::string valueString = value;
			Attribute attribute;
			attribute.setAttributeName(vattribute);
			attribute.setAttributeValue(valueString);
			return attribute;

		} else if (dataType == CDF_INT4)
		{

			int value;
			long status = CDFgetAttrzEntry(current_file_id, attributeNumber, variableNumber, &value);
			//std::cout << "I: attributeValue (" << vattribute << "): " << value << std::endl;
			Attribute attribute;
			attribute.setAttributeName(vattribute);
			attribute.setAttributeValue(value);
			return attribute;

		} else //CDF_FLOAT
		{
			float value;
			long status = CDFgetAttrzEntry(current_file_id, attributeNumber, variableNumber, &value);
			//std::cout << "F: attributeValue (" << vattribute << "): " << value << std::endl;
			Attribute attribute;
			attribute.setAttributeName(vattribute);
			attribute.setAttributeValue(value);
			return attribute;
		}

	}

	/**
	 * TODO
	 * Retrieves the number of global attributes in the selected file.  This is useful for iterating over all
	 * available global attributes.
	 * @return The number of global attributes stored in the selected file.
	 */
	int FileReader::getNumberOfGlobalAttributes()
	{
		return 0;
	}

	/**
	 * @param attribute
	 * @return
	 */
	bool FileReader::doesAttributeExist(const std::string& attribute)
	{
		bool exists = false;
		CDFstatus status = CDFconfirmAttrExistence(current_file_id, (char*) attribute.c_str());
		if (status != NO_SUCH_ENTRY)
			exists = true;

		return exists;

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

		long variableNumber = CDFgetVarNum(current_file_id, (char *) variable.c_str());
		//std::cout << "variableNumber: " << variableNumber << std::endl;
		if (variableNumber >= 0)
			variableIDs[variable] = variableNumber;

		return variableNumber;
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
		CDFstatus status = CDFgetzVarName(current_file_id, variable_id, variableNameBuffer);
		long numElements;
		status = CDFgetzVarNumElements(current_file_id, variable_id, &numElements);
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

		long variableNumber = CDFgetVarNum(current_file_id, (char *) variable.c_str());
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
	 * Destructor
	 */
	FileReader::~FileReader()
	{
		if (current_file_id != NULL)
			close();
	}
}
