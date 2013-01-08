/*
 * GeneralFileReader_compatibility.h
 *
 *  Created on: Mar 16, 2010
 *      Author: dberrios
 */
#include "GeneralFileReader_f.h"
#include <ccmc/wrappers/c/GeneralFileReader_c.h>
#include <ccmc/GeneralFileReader.h>
#include <ccmc/Kameleon.h>
#include <string>
#include <string.h>
#include <iostream>

using namespace ccmc;




void f_generalfilereader_create_(int * id)
{
	*id = GeneralFileReader_create();
}

void f_generalfilereader_open_(int * id, const char * filename, long * status)
{
	*status = GeneralFileReader_open(*id, filename);
}

void f_generalfilereader_getvariable_(int * id, const char * variable, float * variableData)
{
	GeneralFileReader_getVariable(*id, variable, variableData);
}

void f_generalfilereader_getvariablebyid_(int * id, long * variableID, float * variableData)
{
	GeneralFileReader_getVariableByID(*id, *variableID, variableData);
}

void f_generalfilereader_getvariablesubrange_(int * id, const char * variable, long * startIndex, long * count, float * variableData)
{
	GeneralFileReader_getVariableSubRange(*id, variable, *startIndex, *count, variableData);
}

void f_generalfilereader_getvariablebyidsubrange_(int * id, long * variableID, long * startIndex, long * count, float * variableData)
{
	GeneralFileReader_getVariableByIDSubRange(*id, *variableID, *startIndex, *count, variableData);
}

void f_generalfilereader_getvariableid_(int * id, const char * variable, long * status)
{
	*status = GeneralFileReader_getVariableID(*id, variable);
}

void f_generalfilereader_getvariableint(int id, const char * variable, int * variableData)
{

}

void f_generalfilereader_getnumberofglobalattributes(long * num)
{

}

void f_generalfilereader_getnumberofvariables(long * num)
{

}

void f_generalfilereader_getnumberofvariableattributes(long * num)
{

}

void f_generalfilereader_getnumberofrecords(int * id, const char * variable, long * num)
{

}

void f_generalfilereader_getnumberofrecordsbyid(int * id, long * variable_id, long * num)
{

}

void f_generalfilereader_close_(int * id, long * status)
{
	*status = GeneralFileReader_close(*id);
}

void f_generalfilereader_delete_(int * id, long * status)
{
	*status = GeneralFileReader_delete(*id);
}


