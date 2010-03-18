/*
 * FileReader_compatibility.h
 *
 *  Created on: Mar 16, 2010
 *      Author: dberrios
 */
#include "FileReader_compatibility.h"
#include "FileReader.h"
#include "Kameleon.h"
#include <string>
#include <string.h>
#include <iostream>

using namespace ccmc;


typedef boost::unordered_map<int, FileReader*> map_i_F;

/**
 * Not sure why I return the id again.  Eventually this should return a status
 */
int FileReader_create(int id)
{
	//first check if the id exists.  if so, delete it and create a new one.
	map_i_F::iterator iter = fileReaderObjects.find(id);
	if (iter != fileReaderObjects.end())
	{
		//Doh! a FileReader object already exists.
		//Not sure what to do here, so we delete the current object first
		delete (*iter).second;

		(*iter).second = new FileReader();
	} else
	{
std::cout << "creating FileReader object" << std::endl;
		fileReaderObjects[id] = new FileReader();
	}
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

long FileReader_getNumberOfRecords(int id, const std::string& variable)
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

void f_filereader_create_(int * id)
{
	FileReader_create(*id);
}

void f_filereader_open_(int * id, const char * filename, long * status)
{
	*status = FileReader_open(*id, filename);
}

void f_filereader_getvariable_(int * id, const char * variable, float * variableData)
{
	FileReader_getVariable(*id, variable, variableData);
}

void f_filereader_getvariablebyid_(int * id, long * variableID, float * variableData)
{
	FileReader_getVariableByID(*id, *variableID, variableData);
}

void f_filereader_getvariablesubrange_(int * id, const char * variable, long * startIndex, long * count, float * variableData)
{
	FileReader_getVariableSubRange(*id, variable, *startIndex, *count, variableData);
}

void f_filereader_getvariablebyidsubrange_(int * id, long * variableID, long * startIndex, long * count, float * variableData)
{
	FileReader_getVariableByIDSubRange(*id, *variableID, *startIndex, *count, variableData);
}

void f_filereader_getvariableid_(int * id, const char * variable, long * status)
{
	*status = FileReader_getVariableID(*id, variable);
}

void f_filereader_getvariableint(int id, const char * variable, int * variableData)
{

}

void f_filereader_getnumberofglobalattributes(long * num)
{

}

void f_filereader_getnumberofvariables(long * num)
{

}

void f_filereader_getnumberofvariableattributes(long * num)
{

}

void f_filereader_getnumberofrecords(int * id, const char * variable, long * num)
{

}

void f_filereader_getnumberofrecordsbyid(int * id, long * variable_id, long * num)
{

}

void f_filereader_close_(int * id, long * status)
{
	*status = FileReader_close(*id);
}

void f_filereader_delete_(int * id, long * status)
{
	*status = FileReader_delete(*id);
}


