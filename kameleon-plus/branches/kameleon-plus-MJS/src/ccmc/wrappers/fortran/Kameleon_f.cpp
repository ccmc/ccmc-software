/*
 * Kameleon_compatibility.cpp
 *
 *  Created on: Jul 8, 2009
 *      Author: dberrios
 */
#include "Kameleon_f.h"
#include <ccmc/wrappers/c/Kameleon_c.h>
#include <string>
#include <string.h>
#include <iostream>

void f_kameleon_create_(int * id)
{
//	std::cout << "calling C create function" << std::endl;
	*id = Kameleon_create();
//	std::cout << "after C create function" << std::endl;
}

void f_interpolator_create_(int * kid, int * id)
{
	*id = Interpolator_create(*kid);

}

void f_interpolator_delete_(int * id)
{
	Interpolator_delete(*id);

}

void f_kameleon_open_(int * id, const char * filename, int * status)
{

	std::string filename_str = filename;
	//get string length
	int length = filename_str.size();
	char filename_corrected[length+1];

	strcpy(filename_corrected, filename_str.c_str());
	//std::cout << "string length: " << length << std::endl;
	for (int i = 0; i< length; i++)
	{
		if (filename_corrected[i] == ' ')
		{
	//		std::cout << "add the character at index: " << i << std::endl;
			filename_corrected[i] = '\0';
			break;
		}

	}
//	std::cout << "filename: '" << filename_corrected << "'" << std::endl;
	*status = Kameleon_open(*id, filename_corrected);
}

void f_kameleon_interpolate_(int * id, const char * variable, float * c0,
		float * c1, float *c2, float * dc0, float * dc1, float * dc2, float * returnValue)
{
	std::string variable_str = variable;
	//get string length
	int length = variable_str.size();
	char variable_corrected[length+1];
	strcpy(variable_corrected, variable_str.c_str());
	//std::cout << "string length: " << length << std::endl;
	for (int i = 0; i< length; i++)
	{
		if (variable_corrected[i] == ' ')
		{
	//		std::cout << "add the character at index: " << i << std::endl;
			variable_corrected[i] = '\0';
			break;
		}

	}
//	std::cout << "inside f_kameleon_interpolate. lat: " << *c1 << " mlt: " << *c2 << std::endl;
	*returnValue = Kameleon_interpolate(*id, variable_corrected, c0, c1, c2, dc0, dc1, dc2);
//	std::cout << "returning " << *returnValue << std::endl;
	//*returnValue = Kameleon_interpolate(*id, variable_corrected, c0, c1, c2, dc0, dc1, dc2);

}

void f_kameleon_interpolate_by_id_(int * id, int * variable, float * c0,
		float * c1, float *c2, float * dc0, float * dc1, float * dc2, float * returnValue)
{

//	std::cout << "inside f_kameleon_interpolate. id: " << *id << " variable: " << *variable << std::endl;
	*returnValue = Kameleon_interpolate_by_id(*id, *variable, c0, c1, c2, dc0, dc1, dc2);
//	std::cout << "returning " << *returnValue << std::endl;
	//*returnValue = Kameleon_interpolate(*id, variable_corrected, c0, c1, c2, dc0, dc1, dc2);

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
void f_kameleon_load_variable_(int *id, const char * variable)
{
	std::string variable_str = variable;
	int length = variable_str.size();
	char variable_corrected[length+1];
	strcpy(variable_corrected, variable_str.c_str());
	for (int i = 0; i< length; i++)
	{
		if (variable_corrected[i] == ' ')
		{
			variable_corrected[i] = '\0';
			break;
		}
	}

//	std::cout << "variable_corrected: '" << variable_corrected << "'" << std::endl;
//	std::cout << "calling Kameleon_load_variable" << std::endl;
	Kameleon_load_variable(*id, variable_corrected);
//	std::cout << "finished calling Kameleon_load_variable" << std::endl;
}

/**
 * This creates a copy of the string to make it compatible with C. The better way would be to
 * pass the string to this function with a null terminal at the correct position. Not sure how
 * to do that in fortran
 */
void f_kameleon_unload_variable_(int *id, const char * variable)
{
	std::string variable_str = variable;
	int length = variable_str.size();
	char variable_corrected[length+1];
	strcpy(variable_corrected, variable_str.c_str());
	for (int i = 0; i< length; i++)
	{
		if (variable_corrected[i] == ' ')
		{
			variable_corrected[i] = '\0';
			break;
		}
	}

//	std::cout << "variable_corrected: '" << variable_corrected << "'" << std::endl;
//	std::cout << "calling Kameleon_load_variable" << std::endl;
	Kameleon_unload_variable(*id, variable_corrected);
//	std::cout << "finished calling Kameleon_load_variable" << std::endl;
}

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
	for (int i = 0; i< length; i++)
	{
		if (variable_corrected[i] == ' ')
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

void f_kameleon_delete_(int * id, int * status)
{
	*status = Kameleon_delete(*id);
}

void f_kameleon_create_c_string_(const char * t_string, char * destbuffer)
{
	Kameleon_create_c_string(t_string, destbuffer);
}

void f_kameleon_get_variable_id_(int * kid, const char * variable, int * vid)
{
	std::string variable_str = variable;
	int length = variable_str.size();
	char variable_corrected[length+1];
	strcpy(variable_corrected, variable_str.c_str());
	for (int i = 0; i< length; i++)
	{
		if (variable_corrected[i] == ' ')
		{
			variable_corrected[i] = '\0';
			break;
		}
	}
	*vid = Kameleon_get_variable_id(*kid, variable_corrected);
}
