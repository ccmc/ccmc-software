/*
 * GeneralFileReader_compatibility.h
 *
 *  Created on: Mar 16, 2010
 *      Author: dberrios
 */
#include "GeneralFileReader_c.h"
#include <ccmc/GeneralFileReader.h>
#include <ccmc/Kameleon.h>
#include <string>
#include <string.h>
#include <iostream>

using namespace ccmc;


typedef boost::unordered_map<int, GeneralFileReader*> map_i_F;

/**
 * Not sure why I return the id again.  Eventually this should return a status
 */
int GeneralFileReader_create()
{
	//create a new GeneralFileReader object and put it into the map
	int id = 0;
	while (generalFileReaderObjects.find(id) != generalFileReaderObjects.end())
	{
		id++;
	}
	generalFileReaderObjects[id] = new GeneralFileReader();
	return id;
}

/**
 * @return the CDFid returned by the open call
 */
long GeneralFileReader_open(int id, const char * filename)
{
	map_i_F::iterator iter = generalFileReaderObjects.find(id);
	if (iter != generalFileReaderObjects.end())
	{
		return (*iter).second->open(filename);
	} else
		return -1L;
}


/**
 * Make sure the variableData pointer has already been allocated to the correct size.  Query the file
 * to see how many records are in the variable array.
 */
void GeneralFileReader_getVariableByID(int id, long variableID, float * variableData)
{
	map_i_F::iterator iter = generalFileReaderObjects.find(id);
	if (iter != generalFileReaderObjects.end())
	{
		std::vector<float>* vData = (*iter).second->getVariable(variableID);
		for (int i = 0; i < vData->size(); i++)
		{
			variableData[i] = (*vData)[i];
		}
		delete vData;
	}
}

/**
 * Make sure the variableData pointer has already been allocated to the correct size.  Query the file
 * to see how many records are in the variable array.
 */
void GeneralFileReader_getVariable(int id, const char * variable, float * variableData)
{
	map_i_F::iterator iter = generalFileReaderObjects.find(id);
	if (iter != generalFileReaderObjects.end())
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
 * Make sure the variableData pointer has already been allocated to the correct size.  This should have space
 * for count elements
 */
void GeneralFileReader_getVariableSubRange(int id, const char * variable, long startIndex, long count, float * variableData)
{
	map_i_F::iterator iter = generalFileReaderObjects.find(id);
	if (iter != generalFileReaderObjects.end())
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
void GeneralFileReader_getVariableByIDSubRange(int id, long variableID, long startIndex, long count, float * variableData)
{
	map_i_F::iterator iter = generalFileReaderObjects.find(id);
	if (iter != generalFileReaderObjects.end())
	{
		std::vector<float>* vData = (*iter).second->getVariable(variableID, startIndex, count);
		for (int i = 0; i < vData->size(); i++)
		{
			variableData[i] = (*vData)[i];
		}
		delete vData;
	}
}
float GeneralFileReader_getVariableAtIndex(int id, const char * variable, long index)
{
	map_i_F::iterator iter = generalFileReaderObjects.find(id);
	if (iter != generalFileReaderObjects.end())
	{
		return (*iter).second->getVariableAtIndex(variable, index);
	} else
		return 0.f;
}

float GeneralFileReader_getVariableAtIndexByID(long variable_id, long index)
{

}

void GeneralFileReader_getVariableInt(int id, const char * variable, int * variableData)
{
	map_i_F::iterator iter = generalFileReaderObjects.find(id);
	if (iter != generalFileReaderObjects.end())
	{
		std::vector<int>* vData = (*iter).second->getVariableInt(variable);
		for (int i = 0; i < vData->size(); i++)
		{
			variableData[i] = (*vData)[i];
		}
		delete vData;
	}
}

int GeneralFileReader_getVariableIntAtIndex(int id, const char * variable, long index)
{

}

int GeneralFileReader_getNumberOfGlobalAttributes(int id)
{
	map_i_F::iterator iter = generalFileReaderObjects.find(id);
	if (iter != generalFileReaderObjects.end())
	{
		return (*iter).second->getNumberOfGlobalAttributes();
	} else
		return -1;
}

int GeneralFileReader_getNumberOfVariables(int id)
{
	map_i_F::iterator iter = generalFileReaderObjects.find(id);
	if (iter != generalFileReaderObjects.end())
	{
		return (*iter).second->getNumberOfVariables();
	} else
		return -1;
}


int GeneralFileReader_getNumberOfVariableAttributes(int id)
{
	map_i_F::iterator iter = generalFileReaderObjects.find(id);
	if (iter != generalFileReaderObjects.end())
	{
		return (*iter).second->getNumberOfVariableAttributes();
	} else
		return -1;
}

/*
 *

extern _C_ struct Attribute FileReader_getGlobalAttributeByID(int id, long i);
extern _C_ long GeneralFileReader_close(int id);
extern _C_ long GeneralFileReader_delete(int id);
*/





long GeneralFileReader_getNumberOfRecords(int id, const char * variable)
{
	map_i_F::iterator iter = generalFileReaderObjects.find(id);
	if (iter != generalFileReaderObjects.end())
	{
		return (*iter).second->getNumberOfRecords(variable);
	}
}

long GeneralFileReader_getNumberOfRecordsByID(int id, long variable_id)
{
	map_i_F::iterator iter = generalFileReaderObjects.find(id);
	if (iter != generalFileReaderObjects.end())
	{
		return (*iter).second->getNumberOfRecords(variable_id);
	}
}

long GeneralFileReader_getVariableID(int id, const char * variable)
{
	map_i_F::iterator iter = generalFileReaderObjects.find(id);
	if (iter != generalFileReaderObjects.end())
	{
		return (*iter).second->getVariableID(variable);
	} else
		return -1L;
}

void GeneralFileReader_getVariableName (int id, long variable_id, char * variableName);

void GeneralFileReader_getGlobalAttributeName(int id, long attribute_id, char * attributeName);
void GeneralFileReader_getVariableAttributeName(int id, long attribute_id, char * vAttributeName);
Attribute GeneralFileReader_getGlobalAttribute (int id, const char * attribute);
Attribute GeneralFileReader_getVariableAttribute (int id, const char * variable, const char * attribute);
bool GeneralFileReader_doesAttributeExist (int id,  const char * attribute);
bool GeneralFileReader_doesVariableExist (int id, const char * variable);
void GeneralFileReader_getCurrentFilename (int id, char * filename);
long GeneralFileReader_close(int id)
{
	map_i_F::iterator iter = generalFileReaderObjects.find(id);
	if (iter != generalFileReaderObjects.end())
	{
		return (*iter).second->close();
	} else
		return -1L;
}

/**
 * returns 1 if success
 * -1 otherwise
 */
long GeneralFileReader_delete(int id)
{
	//TODO: error checking
	map_i_F::iterator iter = generalFileReaderObjects.find(id);
	if (iter != generalFileReaderObjects.end())
	{
		delete (*iter).second;
		generalFileReaderObjects.erase(iter);
		return 1L;
	} else
		return -1L;
}

