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
	 * Opens a new file. If the previous file has been opened successfuly, and has the
	 * same filename as the requested filename, nothing will be done.
	 * @param filename
	 * @return The status of the open call.  This method should be called from open().
	 */
	long FileReader::openFile(const std::string& filename)
	{
		long status;
		if (current_file_id != NULL && filename != current_filename)
		{
			close();
		}
		status = CDFopenCDF((char *)filename.c_str(), &current_file_id);

		if (status == CDF_OK)
		{
			current_filename = filename;
			this->initializeGlobalAttributes();
			this->initializeVariableIDs();
		}


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
			current_filename = "";

		}
		variableIDs.clear();
		variableNames.clear();
		gAttributes.clear();
		vAttributes.clear();
		gAttributeByID.clear();
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
	Attribute FileReader::getGlobalAttribute(long i)
	{

		boost::unordered_map<long, Attribute>::iterator iter = gAttributeByID.find(i);
		if (iter != gAttributeByID.end())
			return (*iter).second;

		//cout << "after search" << endl;


		long attrNum = i;
		//cout << "i: " << i << " attrNum: " << attrNum << endl;
		long numElements;
		long dataType;

		//std::cout << "dataType: " << dataType << std::endl;


		CDFstatus status =  CDFgetAttrgEntryDataType (current_file_id, attrNum, 0, &dataType);
		status = CDFgetAttrgEntryNumElements (current_file_id, attrNum, 0, &numElements);

		Attribute attribute;
		if (dataType == CDF_CHAR)
		{
			std::string attributeValue = "NULL";
			char attributeBuffer[numElements+1];
			CDFgetAttrgEntry(current_file_id, attrNum, 0, attributeBuffer);
			//std::cout << "attrNum: " << attrNum << " i: " << i << " numElements: " << numElements << std::endl;
			//modelName[numElements] = '\0';
			attributeBuffer[numElements] = '\0';
			//std::cout << "status: " << status << std::endl;
			//if (status == CDF_OK)
			{
				attributeValue = attributeBuffer;
				attributeValue = attributeValue.substr(0, numElements); //only use valid parts of char string
			}

			char attributeNameBuffer[512];

			//char * ctemp = new char[512];
			//strcpy(ctemp, attributeValue.c_str());
			//void * vtemp = (void *)ctemp;
			CDFgetAttrName(current_file_id, attrNum, attributeNameBuffer);
			//std::cout << "attrNum: " << attrNum << " i: " << i << " numElements: " << numElements << std::endl;
			//Attribute attribute;
			attribute.setAttributeName(attributeNameBuffer);
			//std::cout << "attributeBuffer: " << attributeBuffer << endl;
			attribute.setAttributeValue(attributeValue);
			//return attribute;
		} else if (dataType == CDF_INT4)
		{
			//int attributeValue = 0.f;
			int attributeBuffer;// = new int[1];

			CDFgetAttrgEntry(current_file_id, attrNum, 0, (void*) &attributeBuffer);
			//std::cout << "attrNum: " << attrNum << " i: " << i << " numElements: " << numElements << std::endl;
			//std::cout << "numElements: " << numElements << std::endl;
			//modelName[numElements] = '\0';
			//std::cout << "status: " << status << std::endl;
			//if (status == CDF_OK)
			{
				//do nothing.  defaults to zero.
				//attributeValue = (int) attributeBuffer[0];
			}

			char attributeNameBuffer[512];

			CDFgetAttrName(current_file_id, attrNum, attributeNameBuffer);
			//Attribute attribute;
			attribute.setAttributeName(attributeNameBuffer);
			attribute.setAttributeValue(attributeBuffer);
			//return attribute;
		} else //CDF_FLOAT
		{
			//float attributeValue = 0.f;
			float attributeBuffer;// = new float[1];

			CDFgetAttrgEntry(current_file_id, attrNum, 0, (void *) &attributeBuffer);
			//std::cout << "numElements: " << numElements << std::endl;
			//modelName[numElements] = '\0';
			//std::cout << "status: " << status << std::endl;


			char attributeNameBuffer[1024];
			CDFgetAttrName(current_file_id, attrNum, attributeNameBuffer);
			//Attribute attribute;
			attribute.setAttributeName(attributeNameBuffer);
			attribute.setAttributeValue(attributeBuffer);
			//return attribute;
		}
		gAttributeByID[i] = attribute;
		cout << "added: " << i << " name: " << attribute.getAttributeName() << endl;
		return attribute;

	}

	/**
	 * @param attribute
	 * @return
	 */
	Attribute FileReader::getGlobalAttribute(const std::string& attribute)
	{
		//first, check if the attribute has already been requested. If so, return stored value
		boost::unordered_map<std::string, Attribute>::iterator iter = gAttributes.find(attribute);
		if (iter != gAttributes.end())
			return (*iter).second;



		//cout << "attribute: " << attribute;
		long attrNum = CDFgetAttrNum(current_file_id, (char *) attribute.c_str());
		//cout << "attrNum after attribute: " << attrNum << endl;
		if (attrNum < 0)
			std::cout << "attrNum: " << attrNum << " returned for " << attribute << std::endl;
		Attribute current_attribute = getGlobalAttribute(attrNum);
		gAttributes[attribute] = current_attribute;
		return current_attribute;
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
		long num_attributes;
		CDFgetNumgAttributes(current_file_id, &num_attributes);
		return (int)num_attributes;
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

		std::cout << "getting variable id for: " << variable << std::endl;
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

	const std::string& FileReader::getCurrentFilename()
	{
		return this->current_filename;
	}

	std::string FileReader::getGlobalAttributeName(long attribute_id)
	{
		char buffer[256];
		CDFgetAttrName(current_file_id, attribute_id, buffer);
		std::string buffer_string = buffer;
		cout << "Attribute Name: '" << buffer_string << "'" << endl;


		return buffer_string;
	}

	int FileReader::getNumberOfVariables()
	{
		long numVars;
		CDFgetNumzVars(current_file_id, &numVars);
		return (int)numVars;
	}

	/**
	 * Inefficient.  For now, we prepopulate both this classes attributes, and the model objects attributes.
	 * I should merge the two eventually.
	 */
	void FileReader::initializeGlobalAttributes()
	{
		int numGAttributes = this->getNumberOfGlobalAttributes();
		cout << "numGAttributes: " << numGAttributes << endl;
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

	void FileReader::initializeVariableIDs()
	{
		int numVariables = this->getNumberOfVariables();
		cout << "numVariables: " << numVariables << endl;
		char variableName[512];
		for (int i = 0; i < numVariables; i++)
		{
			CDFgetzVarName(current_file_id, i, variableName);
			std::string variableNameString = variableName;
			variableIDs[variableNameString] = i;

		}
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
