/*
 * Kameleon_compatibility.cpp
 *
 *  Created on: Jul 8, 2009
 *      Author: dberrios
 */
#include "Kameleon_f.h"
#include <ccmc/Kameleon.h>
#include <ccmc/wrappers/c/Kameleon_c.h>
#include <string>
#include <string.h>
#include <iostream>

using namespace ccmc;

void f_kameleon_create_(int * id)
{
//	std::cout << "calling C create function" << std::endl;
	Kameleon_create(*id);
//	std::cout << "after C create function" << std::endl;
}

void f_kameleon_open_(int * id, const char * filename, long * status)
{
	std::string filename_str = filename;
	//get string length
	int length = filename_str.size();
	char filename_corrected[length+1];
	strcpy(filename_corrected, filename_str.c_str());
	//std::cout << "string length: " << length << std::endl;
	for (int i = length; i> 1; i--)
	{
		if (filename_corrected[i-1] != ' ')
		{
	//		std::cout << "add the character at index: " << i << std::endl;
			filename_corrected[i] = '\0';
			break;
		}

	}
	*status = Kameleon_open(*id, filename_corrected);
}
//extern _C_ void f_kameleon_get_model_name(int id, char * model_name);
//extern _C_ float Kameleon_interpolate(int id, const char * variable, const float * c0,
//		const float * c1, const float *c2, float * dc0, float * dc1, float * dc2);
//extern _C_ void f_kameleon_load_variable(int *id, const char * variable);

/**
 * This creates a copy of the string to make it compatible with C. The better way would be to
 * pass the string to this function with a null terminal at the correct position. Not sure how
 * to do that in fortran
 */
void f_kameleon_load_vector_variable_(int *id, const char * variable)
{
	std::string variable_str = variable;
	int length = variable_str.size();
	char variable_corrected[length+1];
	strcpy(variable_corrected, variable_str.c_str());
	for (int i = length; i>1; i--)
	{
		if (variable_corrected[i-1] != ' ')
		{
			variable_corrected[i] = '\0';
			break;
		}
	}
	Kameleon_load_vector_variable(*id, variable_corrected);
}

//extern _C_ void Kameleon_get_global_attribute_string(int id, const char * gAttribute, char * destbuffer);
void f_kameleon_close_(int * id)
{
	Kameleon_close(*id);
}

void f_kameleon_delete_(int * id, long * status)
{
	*status = Kameleon_delete(*id);
}
