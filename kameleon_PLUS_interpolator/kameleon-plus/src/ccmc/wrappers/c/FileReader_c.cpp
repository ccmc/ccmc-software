/*
 * FileReader_compatibility.h
 *
 *  Created on: Mar 16, 2010
 *      Author: dberrios
 */
#include "FileReader_c.h"
#include <ccmc/FileReader.h>
#include <ccmc/Kameleon.h>
#include <string>
#include <string.h>
#include <iostream>

using namespace ccmc;


typedef boost::unordered_map<int, FileReader*> map_i_F;

/**
 * Not sure why I return the id again.  Eventually this should return a status
 */
int FileReader_create()
{
	//create a new FileReader object and put it into the map
	int maxObjects = 4096;
	int id = 0;
	while (fileReaderObjects.find(id) != fileReaderObjects.end())
	{
		id++;
		if (id > maxObjects)
			return -1;
	}
	fileReaderObjects[id] = new FileReader();
	return id;
}

/**
 * @return the CDFid returned by the open call
 */
long FileReader_open(int id, const char * filename)
{
	map_i_F::iterator iter = fileReaderObjects.find(id);
	if (iter != fileReaderObjects.end())
	{
		return (*iter).second->open(filename);
	} else
		return -1L;
}

/**
 * Make sure the variableData pointer has already been allocated to the correct size.  Query the file
 * to see how many records are in the variable array.
 */
void FileReader_getVariable(int id, const char * variable, float * variableData)
{
	map_i_F::iterator iter = fileReaderObjects.find(id);
	if (iter != fileReaderObjects.end())
	{
		std::vector<float>* vData = (*iter).second->getVariable(variable);
		for (int i = 0; i < vData->size(); i++)
		{
			variableData[i] = (*vData)[i];
		}
		delete vData;
	}
	//else do nothing.  Should return some type of error
}

/**
 * Make sure the variableData pointer has already been allocated to the correct size.  Query the file
 * to see how many records are in the variable array.
 */
void FileReader_getVariableByID(int id, long variableID, float * variableData)
{
	map_i_F::iterator iter = fileReaderObjects.find(id);
	if (iter != fileReaderObjects.end())
	{
		std::vector<float>* vData = (*iter).second->getVariableByID(variableID);
		for (int i = 0; i < vData->size(); i++)
		{
			variableData[i] = (*vData)[i];
		}
		delete vData;
	}
}

/**
 * Make sure the variableData pointer has already been allocated to the correct size.  This should have space
 * for count elements
 */
void FileReader_getVariableSubRange(int id, const char * variable, long startIndex, long count, float * variableData)
{
	map_i_F::iterator iter = fileReaderObjects.find(id);
	if (iter != fileReaderObjects.end())
	{
		std::vector<float>* vData = (*iter).second->getVariable(variable, startIndex, count);
		for (int i = 0; i < vData->size(); i++)
		{
			variableData[i] = (*vData)[i];
		}
		delete vData;
	}
}

/**
 * Make sure the variableData pointer has already been allocated to the correct size.  This should have space
 * for count elements
 */
void FileReader_getVariableByIDSubRange(int id, long variableID, long startIndex, long count, float * variableData)
{
	map_i_F::iterator iter = fileReaderObjects.find(id);
	if (iter != fileReaderObjects.end())
	{
		std::vector<float>* vData = (*iter).second->getVariableByID(variableID, startIndex, count);
		for (int i = 0; i < vData->size(); i++)
		{
			variableData[i] = (*vData)[i];
		}
		delete vData;
	}
}

long FileReader_getVariableID(int id, const char * variable)
{
	map_i_F::iterator iter = fileReaderObjects.find(id);
	if (iter != fileReaderObjects.end())
	{
		return (*iter).second->getVariableID(variable);
	} else
		return -1L;
}

void FileReader_getVariableInt(int id, const char * variable, int * variableData)
{
	map_i_F::iterator iter = fileReaderObjects.find(id);
	if (iter != fileReaderObjects.end())
	{
		std::vector<int>* vData = (*iter).second->getVariableInt(variable);
		for (int i = 0; i < vData->size(); i++)
		{
			variableData[i] = (*vData)[i];
		}
		delete vData;
	}
}

int FileReader_getNumberOfGlobalAttributes(int id)
{
	map_i_F::iterator iter = fileReaderObjects.find(id);
	if (iter != fileReaderObjects.end())
	{
		return (*iter).second->getNumberOfGlobalAttributes();
	} else
		return -1;
}

int FileReader_getNumberOfVariables(int id)
{
	map_i_F::iterator iter = fileReaderObjects.find(id);
	if (iter != fileReaderObjects.end())
	{
		return (*iter).second->getNumberOfVariables();
	} else
		return -1;
}

int FileReader_getNumberOfVariableAttributes(int id)
{
	map_i_F::iterator iter = fileReaderObjects.find(id);
	if (iter != fileReaderObjects.end())
	{
		return (*iter).second->getNumberOfVariableAttributes();
	} else
		return -1;
}

long FileReader_getNumberOfRecords(int id, const char * variable)
{
	map_i_F::iterator iter = fileReaderObjects.find(id);
	if (iter != fileReaderObjects.end())
	{
		return (*iter).second->getNumberOfRecords(variable);
	}
}

long FileReader_getNumberOfRecordsByID(int id, long variable_id)
{
	map_i_F::iterator iter = fileReaderObjects.find(id);
	if (iter != fileReaderObjects.end())
	{
		return (*iter).second->getNumberOfRecords(variable_id);
	}
}

long FileReader_close(int id)
{
	map_i_F::iterator iter = fileReaderObjects.find(id);
	if (iter != fileReaderObjects.end())
	{
		return (*iter).second->close();
	} else
		-1L;
}

/**
 * returns 1 if success
 * -1 otherwise
 */
long FileReader_delete(int id)
{
	//TODO: error checking
	map_i_F::iterator iter = fileReaderObjects.find(id);
	if (iter != fileReaderObjects.end())
	{
		delete (*iter).second;
		fileReaderObjects.erase(iter);
		return 1L;
	} else
		-1L;
}

