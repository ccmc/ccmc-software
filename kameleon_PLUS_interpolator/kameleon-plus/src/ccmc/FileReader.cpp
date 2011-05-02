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

		if(!(ifstream(filename.c_str())))
		{
			status = FILE_DOES_NOT_EXIST;
			std::cout << "filename: \"" << filename << "\" does not exist." << std::endl;
		}
		else{
			status = CDFopenCDF((char *)filename.c_str(), &current_file_id);


			if (status == CDF_OK)
			{
				long readOnlyMode = READONLYon;
				CDFsetReadOnlyMode(current_file_id, readOnlyMode);

				//check if this is a valid Kameleon converted file

				current_filename = filename;
				this->initializeGlobalAttributes();
				this->initializeVariableAttributes();
				this->initializeVariableIDs();
				this->initializeVariableNames();
				status = OK;
			} else
			{
				status = OPEN_ERROR;
			}

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
	 * Returns a pointer to a std::vector<float> containing the values of the selected variable
	 * stored in the selected file.  This allocates a new std::vector<float> pointer.  Make sure you
	 * delete the contents when you done using it, or you will have a memory leak.
	 * @param variable
	 * @return std::vector<float> containing the values of the selected variable.
	 */
	std::vector<float>* FileReader::getVariable(const std::string& variable)
	{
		long counts[1];
		std::vector<float>* variableData = new std::vector<float>();

		if (this->doesVariableExist(variable))
		{
			//std::cout << "reading " << variable << std::endl;
			//get variable number
			long variableNum = this->getVariableID(variable);

			//check if variable exists


			long recStart = 0L;
			long recCount = 1L;
			long recInterval = 1L;
			long dimIndices[] = { 0 };
			long dimIntervals[] = { 1 };

			//get dim sizes

			CDFgetzVarDimSizes(current_file_id, variableNum, counts);
			//cout << "variable: " << variable << ": " << counts[0] << endl;
			float * buffer = new float[counts[0]];
			CDFhyperGetzVarData(current_file_id, variableNum, recStart, recCount, recInterval, dimIndices, counts,
					dimIntervals, buffer);
			//add data to vector type, and delete original array
			variableData->reserve(counts[0]);
			for (int i = 0; i < counts[0]; i++)
			{
				variableData->push_back(buffer[i]);
			}

			delete[] buffer;
			//std::cout << "finished reading " << variable << std::endl;
			//std::cout << "size of variable: " << variableData.size() << std::endl;
			//std::cout << "dimSizes[0]: " << dimSizes[0] << std::endl;

		}

		return variableData;
	}

	/**
	 * Returns a pointer to a std::vector<float> containing the values of the selected variable
	 * in the range specified by the startIndex and count (the number of records to read) stored in the selected file.
	 * This allocates a new std::vector<float> pointer.  Make sure you
	 * delete the contents when you done using it, or you will have a memory leak.
	 * @param variable
	 * @param startIndex
	 * @param count
	 * @return std::vector<float> containing the values of the selected variable.
	 */
	std::vector<float>* FileReader::getVariable(const std::string& variable, long startIndex, long count)
	{
		//std::cout << "reading " << variable << std::endl;
		//get variable number
		long variableNum = CDFgetVarNum(current_file_id, (char *) variable.c_str());

		long recStart = 0L;
		long recCount = 1L;
		long recInterval = 1L;
		long dimIndices[] = { 0 };
		long dimIntervals[] = { 1 };

		long counts[1] = {count};
		//get dim sizes
		//CDFgetzVarDimSizes(current_file_id, variableNum, counts);
		float * buffer = new float[counts[0]];
		CDFhyperGetzVarData(current_file_id, variableNum, recStart, recCount, recInterval, dimIndices, counts,
				dimIntervals, buffer);
		//add data to vector type, and delete original array
		std::vector<float>* variableData = new std::vector<float>();
		variableData->reserve(counts[0]);
		for (int i = 0; i < counts[0]; i++)
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
	 * Returns a pointer to a std::vector<float> containing the values of the selected variable
	 * in the range specified by the startIndex and count (the number of records to read) stored
	 * in the selected file.  This allocates a new std::vector<float> pointer.  Make sure you
	 * delete the contents when you done using it, or you will have a memory leak.
	 * @param variableID
	 * @param startIndex
	 * @param count
	 * @return std::vector<float> containing the values of the selected variable.
	 */
	std::vector<float>* FileReader::getVariableByID(long variableID, long startIndex, long count)
	{
		//std::cout << "reading " << variable << std::endl;
		//get variable number
		long recStart = 0L;
		long recCount = 1L;
		long recInterval = 1L;
		long dimIndices[] = { 0 };
		long dimIntervals[] = { 1 };

		long counts[1] = {count};
		//get dim sizes
		//CDFgetzVarDimSizes(current_file_id, variableNum, counts);
		float * buffer = new float[counts[0]];
		CDFhyperGetzVarData(current_file_id, variableID, recStart, recCount, recInterval, dimIndices, counts,
				dimIntervals, buffer);
		//add data to vector type, and delete original array
		std::vector<float>* variableData = new std::vector<float>();
		variableData->reserve(counts[0]);
		for (long i = 0; i < counts[0]; i++)
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
	 * Returns a pointer to a std::vector<float> containing the values of the selected variable
	 * stored in the selected file.  This allocates a new std::vector<float> pointer.  Make sure you
	 * delete the contents when you done using it, or you will have a memory leak.
	 * @param variable
	 * @return std::vector<float> containing the values of the selected variable.
	 */
	std::vector<float>* FileReader::getVariableByID(long variable)
	{
		//std::cout << "reading " << variable << std::endl;
		//get variable number
		long recStart = 0L;
		long recCount = 1L;
		long recInterval = 1L;
		long dimIndices[] = { 0 };
		long dimIntervals[] = { 1 };

		long counts[1];
		//get dim sizes
		CDFgetzVarDimSizes(current_file_id, variable, counts);
		float * buffer = new float[counts[0]];
		CDFhyperGetzVarData(current_file_id, variable, recStart, recCount, recInterval, dimIndices, counts,
				dimIntervals, buffer);
		//add data to vector type, and delete original array
		std::vector<float>* variableData = new std::vector<float>();
		variableData->reserve(counts[0]);
		for (long i = 0; i < counts[0]; i++)
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
	 * @brief Returns a value in the flat array of the variable and index requested.
	 *
	 * Use this method on variables that have a type of float
	 *
	 * @param variable The variable in the file
	 * @param index The index in the variable's array in the file
	 *
	 * @return float of the value in the array.
	 */
	float FileReader::getVariableAtIndex(const std::string& variable, long index)
	{
		//std::cout << "index " << index << std::endl;
		//get variable number
		long variableNum = CDFgetVarNum(current_file_id, (char *) variable.c_str());

		long recStart = 0L;
		long recCount = 1L;
		long recInterval = 1L;
		long dimIndices[] = { index };
		long dimIntervals[] = { 1 };

		long count[1] = {1};
		//get dim sizes
		//CDFgetzVarDimSizes(current_file_id, variableNum, dimSizes);
		float * buffer = new float[1];

		CDFhyperGetzVarData(current_file_id, variableNum, recStart, recCount, recInterval, dimIndices, count,
				dimIntervals, buffer);
		float value = buffer[0];
		delete[] buffer;
		//std::cout << "finished reading " << variable << std::endl;
		//std::cout << "size of variable: " << variableData.size() << std::endl;
		//std::cout << "dimSizes[0]: " << dimSizes[0] << std::endl;
		return value;
	}

	/**
	 * This allocates a new std::vector<int> pointer.  Make sure you
	 * delete the contents when you done using it, or you will have a memory leak.
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

		long count[1];
		//get dim sizes
		CDFgetzVarDimSizes(current_file_id, variableNum, count);
		int * buffer = new int[count[0]];
		CDFhyperGetzVarData(current_file_id, variableNum, recStart, recCount, recInterval, dimIndices, count,
				dimIntervals, buffer);
		//add data to vector type, and delete original array
		std::vector<int> * variableData = new std::vector<int>();
		variableData->reserve(count[0]);
		for (int i = 0; i < count[0]; i++)
		{
			variableData->push_back(buffer[i]);
		}

		delete[] buffer;
		//std::cout << "finished reading " << variable << std::endl;
		return variableData;
	}

	/**
	 * @param variableNum
	 * @param index
	 * @return
	 */
	float FileReader::getVariableAtIndexByID(long variableNum, long index)
	{
		//std::cout << "reading " << variable << std::endl;
		//get variable number
		//long variableNum = CDFgetVarNum(current_file_id, (char *) variable.c_str());

		long recStart = 0L;
		long recCount = 1L;
		long recInterval = 1L;
		long dimIndices[] = { index };
		long dimIntervals[] = { 1 };

		long count[1] = {1};
		//get dim sizes
		//CDFgetzVarDimSizes(current_file_id, variableNum, dimSizes);
		float buffer[1];
		CDFgetzVarData(current_file_id, variableNum, recStart, dimIndices, buffer);
		//add data to vector type, and delete original array
		return buffer[0];
		//delete[] buffer;
		//std::cout << "finished reading " << variable << std::endl;

	}

	/**
	 * @brief Returns a value in the flat array of the variable and index requested.
	 *
	 * Use this method on variables that have a type of int
	 *
	 * @param variable The variable in the file
	 * @param index The index in the variable's array in the file
	 *
	 * @return int of the value in the array.
	 */
	int FileReader::getVariableIntAtIndex(const std::string& variable, long index)
	{
		//std::cout << "reading " << variable << std::endl;
		//get variable number
		long variableNum = CDFgetVarNum(current_file_id, (char *) variable.c_str());

		long recStart = 0L;
		long recCount = 1L;
		long recInterval = 1L;
		long dimIndices[] = { index };
		long dimIntervals[] = { 1 };

		long dimSizes[1] = {1};
		//get dim sizes
		//CDFgetzVarDimSizes(current_file_id, variableNum, dimSizes);
		int * buffer = new int[1];
		CDFgetzVarData(current_file_id, variableNum, recStart, dimIndices, buffer);
		//add data to vector type, and delete original array
		int value = buffer[0];
		delete[] buffer;
		//std::cout << "finished reading " << variable << std::endl;
		return value;
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
			gAttributeByID[i] = attribute;
			gAttributes[attribute.getAttributeName()] = attribute;
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
			gAttributeByID[i] = attribute;
			gAttributes[attribute.getAttributeName()] = attribute;
			//return attribute;
		} else if (dataType == CDF_FLOAT)//CDF_FLOAT
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
			gAttributeByID[i] = attribute;
			gAttributes[attribute.getAttributeName()] = attribute;
			//return attribute;
		}

		//cout << "added: " << i << " name: " << attribute.getAttributeName() << endl;
		//std::cout << "Attribute: " << attribute.toString() << std::endl;
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

		//std::cout << "after search in getGlobalAttribute(const std::string& attribute" << std::endl;

		//cout << "attribute: " << attribute;
		long attrNum = CDFgetAttrNum(current_file_id, (char *) attribute.c_str());
		//cout << "attrNum after attribute: " << attrNum << endl;
		Attribute current_attribute;
		if (attrNum < 0)
		{
			//std::cout << "attrNum: " << attrNum << " returned for " << attribute << std::endl;
		}
		else
		{
			current_attribute = getGlobalAttribute(attrNum);
		}//gAttributes[attribute] = current_attribute;
		return current_attribute;
	}

	/**
	 * @param variable
	 * @param vattribute
	 * @return
	 */
	Attribute FileReader::getVariableAttribute(const std::string& variable, const std::string& vattribute)
	{


		//first, check the vAttributes map
		boost::unordered_map<std::string, boost::unordered_map< std::string, Attribute> >::iterator iter =
				vAttributes.find(variable);
		if (iter != vAttributes.end())
		{
			boost::unordered_map< std::string, Attribute>::iterator iter2 = vAttributes[variable].find(vattribute);
			if (iter2 != vAttributes[variable].end())
			{
				return (*iter2).second;
			}
		}
		long variableNumber = CDFgetVarNum(current_file_id, (char *) variable.c_str());
		long attributeNumber = CDFgetAttrNum(current_file_id, (char *) vattribute.c_str());
		long dataType;
		CDFstatus status = CDFgetAttrzEntryDataType(current_file_id, attributeNumber, variableNumber, &dataType);
		Attribute attribute;
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

			attribute.setAttributeName(vattribute);
			attribute.setAttributeValue(valueString);


		} else if (dataType == CDF_INT4)
		{

			int value;
			long status = CDFgetAttrzEntry(current_file_id, attributeNumber, variableNumber, &value);
			//std::cout << "I: attributeValue (" << vattribute << "): " << value << std::endl;

			attribute.setAttributeName(vattribute);
			attribute.setAttributeValue(value);


		} else if (dataType == CDF_FLOAT) //CDF_FLOAT
		{
			float value;
			long status = CDFgetAttrzEntry(current_file_id, attributeNumber, variableNumber, &value);
			//std::cout << "F: attributeValue (" << vattribute << "): " << value << std::endl;

			attribute.setAttributeName(vattribute);
			attribute.setAttributeValue(value);

		}

		(vAttributes[variable])[vattribute] = attribute;
		return attribute;

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
		if (status == CDF_OK)
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
		else
		{
			//since we prefetched all of the variables on open, it shouldn't exist
			return -1;
		}

		/*std::cout << "getting variable id for: " << variable << std::endl;
		long variableNumber = CDFgetVarNum(current_file_id, (char *) variable.c_str());
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
		CDFgetAttrName(current_file_id, attribute_id, buffer);
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
		CDFgetNumzVars(current_file_id, &numVars);
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
			CDFgetzVarName(current_file_id, i, variableName);
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
			CDFgetzVarName(current_file_id, i, variableName);
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

		CDFgetNumvAttributes(current_file_id, &numVAttributes);
		return (int)numVAttributes;
	}

	/**
	 * @param attribute_id
	 * @return String representing the name of the attribute specified by attribute_id
	 */
	std::string FileReader::getVariableAttributeName(long attribute_id)
	{
		char buffer[256];
		CDFgetAttrName(current_file_id, attribute_id, buffer);
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
		long variableNum = CDFgetVarNum(current_file_id, (char *) variable.c_str());

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
		CDFgetzVarDimSizes(current_file_id, variable_id, counts);
		return counts[0];
	}

	/**
	 * @return
	 */
	std::vector<std::string> FileReader::getVariableAttributeNames()
	{
		std::vector<std::string> attributeNames;
		long numAttributes;
		CDFgetNumAttributes(current_file_id, &numAttributes);
		char name[512];
		long attrScope;
		long maxgEntry;
		long maxrEntry;
		long maxzEntry;

		for (int i = 0; i < numAttributes; i++)
		{
			std::string value = "";
			CDFinquireAttr(current_file_id, i,name, &attrScope, &maxgEntry, &maxrEntry, &maxzEntry);
			//CDFgetAttrName(current_file_id, i, buffer);
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
		if (current_file_id != NULL)
			close();
	}
}
