/*
 * Kameleon_compatibility.cpp
 *
 *  Created on: Jul 8, 2009
 *      Author: dberrios
 */
#include "Kameleon_c.h"
#include "Tracer_c.h"
#include <ccmc/Kameleon.h>
#include <ccmc/Interpolator.h>
#include <string>
#include <string.h>
#include <iostream>


using namespace ccmc;
//TODO: Add appropriate error checking.  These functions WILL segfault if the id does not exist and you attempt to access the member functions of a null pointer.
typedef boost::unordered_map<int, ccmc::Kameleon*> map_i_K;
typedef boost::unordered_map<int, ccmc::Interpolator*> map_i_I;
typedef boost::unordered_map<int, ccmc::Tracer*> map_i_T;
int Kameleon_create(int id)
{




//	std::cout << "inside C create function" << std::endl;


	//first check if the id exists.  if so, delete it and create a new one.
	map_i_K::iterator iter = ccmc::kameleonObjects.find(id);
	if (iter != ccmc::kameleonObjects.end())
	{
		//Doh! a Kameleon object already exists.
		//Not sure what to do here, so we delete the current object first
		delete (*iter).second;

		map_i_I::iterator iter2 = ccmc::interpolatorObjects.find(id);
		if (iter2 != ccmc::interpolatorObjects.end())
		{
			delete (*iter2).second;
		} else
		{
			//should never happen, but insert a new entry anyway
//			std::cout << "Inserting a new Interpolator.  Should never be here." << std::endl;
			ccmc::interpolatorObjects[id];
		}

		//kameleonObjects.erase(iter);
		(*iter).second = new ccmc::Kameleon();
	} else
	{
//std::cout << "creating kameleon object" << std::endl;
		ccmc::kameleonObjects[id] = new Kameleon();
		ccmc::interpolatorObjects[id];
//		std::cout << "new size of kameleonObjects: " << ccmc::kameleonObjects.size() << std::endl;


	}
//	std::cout << "end of C create function" << std::endl;
	return id;
}

int Kameleon_open(int id, const char * filename)
{
	//TODO: error checking
	int status;
	map_i_K::iterator iter = ccmc::kameleonObjects.find(id);

	if (iter != ccmc::kameleonObjects.end())
	{
		//first, open the file

		ccmc::Kameleon * kameleon = (*iter).second;

		status = kameleon->open(filename);
//		std::cout << "filename: " << filename << " status: " << status << " after" << std::endl;
		map_i_I::iterator iter2 = ccmc::interpolatorObjects.find(id);
		if (iter2 != ccmc::interpolatorObjects.end())
		{
			delete (*iter2).second;
		}
		(*iter2).second = kameleon->createNewInterpolator();

		//kameleonObjects.erase(iter);
	} else
	{

		//should never happen

	}
//	std::cout << "new size of kameleonObjects: " << ccmc::kameleonObjects.size() << std::endl;
	return status;
}

int Kameleon_close(int id)
{
	//TODO: error checking
	ccmc::Kameleon * kameleon = ccmc::kameleonObjects[id];
	int status = kameleon->close();
	return status;
//	std::cout << "new size of kameleonObjects: " << ccmc::kameleonObjects.size() << std::endl;
}

int Kameleon_delete(int id)
{
	//TODO: error checking
	map_i_K::iterator iter = ccmc::kameleonObjects.find(id);
	if (iter != ccmc::kameleonObjects.end())
	{
		delete (*iter).second;
		ccmc::kameleonObjects.erase(iter);
		return 0;
	} else
		1;
}

void Kameleon_get_model_name(int id, char * model_name)
{
	//TODO: error checking
	ccmc::Kameleon * kameleon = ccmc::kameleonObjects[id];
	std::string model_name_string = kameleon->getModelName();
//	std::cout << "model_name_string: '" << model_name_string << "'" << std::endl;
	strncpy(model_name, model_name_string.c_str(), sizeof(char) * model_name_string.size());
	model_name[model_name_string.size()] = '\0';

}

float Kameleon_interpolate(int id, const char * variable, const float * c0, const float * c1, const float * c2, float * dc0, float * dc1, float * dc2)
{
	//TODO: error checking
	//first, fetch the interpolator
	ccmc::Interpolator * interpolator = ccmc::interpolatorObjects[id];
	return interpolator->interpolate(variable, *c0,*c1,*c2,*dc0,*dc1,*dc2);
}

int Kameleon_load_variable(int id, const char * variable)
{
	//TODO: error checking
	ccmc::Kameleon * kameleon = ccmc::kameleonObjects[id];
	int status = kameleon->loadVariable(variable);
	return status;
}

int Kameleon_load_vector_variable(int id, const char * variable)
{
	//TODO: error checking
	ccmc::Kameleon * kameleon = ccmc::kameleonObjects[id];
	int status = kameleon->loadVectorVariable(variable);
	return status;
}

void Kameleon_get_global_attribute_string(int id, const char * gAttribute, char * destbuffer)
{
	//TODO: error checking
	ccmc::Kameleon * kameleon = ccmc::kameleonObjects[id];
	std::string attribute = (kameleon->getGlobalAttribute(gAttribute)).getAttributeString();
	strncpy(destbuffer, attribute.c_str(), attribute.size());
	destbuffer[attribute.size()] = '\0';
}

float Kameleon_get_missing_value(int id)
{
	//TODO: error checking
	ccmc::Kameleon * kameleon = ccmc::kameleonObjects[id];
	return kameleon->getMissingValue();
}

void Kameleon_get_native_unit(int id, const char * variable, char * destbuffer)
{
	//TODO: error checking
	ccmc::Kameleon * kameleon = ccmc::kameleonObjects[id];
	std::string units = kameleon->getNativeUnit(variable);
	strncpy(destbuffer, units.c_str(), units.size());
}

void Kameleon_get_vis_unit(int id, const char * variable, char * destbuffer)
{
	ccmc::Kameleon * kameleon = ccmc::kameleonObjects[id];
	std::string units = kameleon->getVisUnit(variable);
	strncpy(destbuffer, units.c_str(), units.size());
}

int Tracer_create(int id, int kid)
{
	//first check if the id exists.  if so, delete it and create a new one.
	map_i_T::iterator iter = ccmc::tracerObjects.find(id);

	if (iter != ccmc::tracerObjects.end())
	{
		//Doh! a Tracer object already exists.
		//Not sure what to do here, so we delete the current object first
		delete (*iter).second;

	}
//	std::cout << "size of kameleonObjects: " << ccmc::kameleonObjects.size() << std::endl;
	ccmc::kameleonObjects.size();
	map_i_K::iterator iter2 = ccmc::kameleonObjects.find(kid);
	if (iter2 != ccmc::kameleonObjects.end())
	{
//		std::cout << "kameleonObjects[kid] was found" << std::endl;
	}
	ccmc::Kameleon * kameleon = (*iter2).second;

	tracerObjects[id] = new ccmc::Tracer(kameleon);
//	std::cout << "end of C create function" << std::endl;
	return id;
}

void Tracer_bidirectionalTrace(int id, const char * variable, const float& startComponent1, const float& startComponent2,
		const float& startComponent3, const int& step_max, const float& dn, int * actual_steps, float * x_array, float * y_array, float * z_array)
{
//	std::cout << "before asigning to std::string" << std::endl;
	std::string variable_str = variable;
//	std::cout << "after asigning to std::string" << std::endl;
	map_i_T::iterator iter = ccmc::tracerObjects.find(id);
	((*iter).second)->setMaxIterations(step_max);
	((*iter).second)->setDn(dn);
//	std::cout << "before tracing with variable: '" << variable_str << "'" << std::endl;
	ccmc::Fieldline fieldline = ((*iter).second)->bidirectionalTrace(variable_str, startComponent1, startComponent2, startComponent3);
//	std::cout << "after tracing" << std::endl;
	*actual_steps = fieldline.size();
	std::vector<ccmc::Point3f> positions = fieldline.getPositions();
	for (int i = 0; i < positions.size(); i++)
	{
		x_array[i] = positions[i].component1;
		y_array[i] = positions[i].component2;
		z_array[i] = positions[i].component3;
	}
}

int Tracer_delete(int id)
{
	//TODO: error checking
	map_i_T::iterator iter = ccmc::tracerObjects.find(id);
	if (iter != ccmc::tracerObjects.end())
	{
		delete (*iter).second;
		ccmc::tracerObjects.erase(iter);
		return 0;
	} else
		1;
}


