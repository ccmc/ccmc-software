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


void f_kameleon_create_(int * id)
{
	Kameleon_create(*id);
}

void f_kameleon_open_(int * id, const char * filename, long * status)
{
	*status = Kameleon_open(*id, filename);
}
//extern _C_ void f_kameleon_get_model_name(int id, char * model_name);
//extern _C_ float Kameleon_interpolate(int id, const char * variable, const float * c0,
//		const float * c1, const float *c2, float * dc0, float * dc1, float * dc2);
//extern _C_ void f_kameleon_load_variable(int *id, const char * variable);
void f_kameleon_load_vector_variable_(int *id, const char * variable)
{
	Kameleon_load_vector_variable(*id, variable);
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
