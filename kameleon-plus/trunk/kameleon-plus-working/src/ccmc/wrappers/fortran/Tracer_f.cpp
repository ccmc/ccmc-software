/*
 * Tracer_f.cpp
 *
 *  Created on: Jun 9, 2010
 *      Author: dberrios
 */
#include "Tracer_f.h"
#include <ccmc/wrappers/c/Tracer_c.h>
#include <ccmc/Tracer.h>
// #include <ccmc/Kameleon.h>
#include <string>
#include <string.h>

using namespace ccmc;
// extern _C_ boost::unordered_map<int, ccmc::Kameleon *> ccmc::kameleonObjects;

void f_tracer_create_(int * id, int * kid)
{
	// std::cout << "f_tracer_create_ number of kameleon objects: " << kameleonObjects.size() << std::endl;
	*id = Tracer_create(*kid);
}
void f_tracer_bidirectionaltrace_(int * id, const char * variable, const float * startComponent1, const float * startComponent2,
		const float * startComponent3, const int * step_max, const float * dn, int * actual_steps, float * x_array, float * y_array, float * z_array)
{
	std::string variable_str = variable;
	int length = variable_str.size();
	char variable_corrected[length+1];
	strcpy(variable_corrected, variable_str.c_str());
	for (int i = 0; i < length+1; i++)
	{
		if (variable_corrected[i] == ' ')
		{
			variable_corrected[i] = '\0';
		}
	}
	// std::cout << "corrected variable: " << variable_corrected << std::endl;
	Tracer_bidirectionalTrace(*id, variable_corrected, startComponent1, startComponent2,
			startComponent3, step_max, dn, actual_steps, x_array, y_array, z_array);
	// std::cout << " finished Tracer_bidirectionalTrace" << std::endl;
}

void f_tracer_delete_(int * id)
{
	Tracer_delete(*id);
}
