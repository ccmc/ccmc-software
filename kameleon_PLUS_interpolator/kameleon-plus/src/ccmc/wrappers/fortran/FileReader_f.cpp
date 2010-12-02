/*
 * FileReader_compatibility.h
 *
 *  Created on: Mar 16, 2010
 *      Author: dberrios
 */
#include "FileReader_f.h"
#include <ccmc/wrappers/c/FileReader_c.h>
#include <ccmc/FileReader.h>
#include <ccmc/Kameleon.h>
#include <string>
#include <string.h>
#include <iostream>

using namespace ccmc;




void f_filereader_create_(int * id)
{
	*id = FileReader_create();
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


