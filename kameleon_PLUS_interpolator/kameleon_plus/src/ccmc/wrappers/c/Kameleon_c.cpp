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
#include <ccmc/Tracer.h>
#include <string>
#include <string.h>
#include <iostream>


using namespace ccmc;
//TODO: Add appropriate error checking.  These functions WILL segfault if the id does not exist and you attempt to access the member functions of a null pointer.
typedef boost::unordered_map<int, ccmc::Kameleon*> map_i_K;
typedef boost::unordered_map<int, ccmc::Interpolator*> map_i_I;
typedef boost::unordered_map<int, ccmc::Tracer*> map_i_T;

int Kameleon_create()
{




	int maxObjects = 4096;
	int id = 0;
	while (kameleonObjects.find(id) != kameleonObjects.end())
	{
		id++;
		if (id > maxObjects)
			return -1;
	}
	//first check if the id exists.  if so, delete it and create a new one.
	kameleonObjects[id] = new Kameleon();
	return id;
}

int Interpolator_create(int kid)
{

	Interpolator * interpolator = NULL;
	int id = 0;
	if (kameleonObjects.find(kid) != kameleonObjects.end())
	{
		int maxObjects = 4096;
		while (interpolatorObjects.find(id) != interpolatorObjects.end())
		{
			id++;
			if (id > maxObjects)
				return -1;
		}
		//first check if the id exists.  if so, delete it and create a new one.
		interpolatorObjects[id] = kameleonObjects[kid]->createNewInterpolator();
		return id;
	} else
		return -1;

}

int Interpolator_delete(int id)
{
	//TODO: error checking
	map_i_I::iterator iter = interpolatorObjects.find(id);
	if (iter != interpolatorObjects.end())
	{
		delete (*iter).second;
		interpolatorObjects.erase(iter);
		return 0;
	} else
		return 1;
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
//		std::cout << "before calling kameleon->open" << std::endl;
		status = kameleon->open(filename);
//		std::cout << "filename: " << filename << " status: " << status << " after" << std::endl;
		//kameleonObjects.erase(iter);
	} else
	{

		//should never happen
		std::cerr << "this should never happen!" << std::endl;

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
		return 1;
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
	float value = interpolator->interpolate(variable, *c0,*c1,*c2,*dc0,*dc1,*dc2);
//	std::cout << "inside c. returning value: " << value << " for variable " << variable << std::endl;
	return value;
}

float Kameleon_interpolate_by_id(int id, int variable, const float * c0, const float * c1, const float * c2, float * dc0, float * dc1, float * dc2)
{
	//TODO: error checking
	//first, fetch the interpolator
	ccmc::Interpolator * interpolator = ccmc::interpolatorObjects[id];
	float value = interpolator->interpolate(variable, *c0,*c1,*c2,*dc0,*dc1,*dc2);
//	std::cout << "inside c. returning value: " << value << " for variable " << variable << std::endl;
	return value;
}

int Kameleon_load_variable(int id, char * variable)
{
	//TODO: error checking
//	std::cout << "variable: " << variable << std::endl;
	ccmc::Kameleon * kameleon = ccmc::kameleonObjects[id];

	int status = kameleon->loadVariable(variable);
	return status;
}

int Kameleon_unload_variable(int id, char * variable)
{
	//TODO: error checking
//	std::cout << "variable: " << variable << std::endl;
	ccmc::Kameleon * kameleon = ccmc::kameleonObjects[id];

	int status = kameleon->unloadVariable(variable);
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

void Kameleon_create_c_string(const char * t_string, char * destbuffer)
{
	std::string t_string_str = t_string;
	//get string length
	int length = t_string_str.size();
	//char t_string_corrected[length+1];
	strcpy(destbuffer, t_string_str.c_str());
	//std::cout << "string length: " << length << std::endl;
	for (int i = length; i> 1; i--)
	{
		if (destbuffer[i-1] != ' ')
		{
	//		std::cout << "add the character at index: " << i << std::endl;
			destbuffer[i] = '\0';
			break;
		}

	}
}

int Tracer_create(int kid)
{
	if (kameleonObjects.find(kid) != kameleonObjects.end())
	{
		int maxObjects = 4096;
		int id = 0;
		while (tracerObjects.find(id) != tracerObjects.end())
		{
			id++;
			if (id > maxObjects)
				return -1;
		}
		//first check if the id exists.  if so, delete it and create a new one.
		Kameleon * kameleon = kameleonObjects[kid];
		tracerObjects[id] = new Tracer(kameleon);
		return id;
	} else
		return -1;

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

void Tracer_unidirectionalTrace(int id, const char * variable, const float * startComponent1, const float * startComponent2,
		const float * startComponent3, const int * step_max, const float * dn, int * actual_steps, float * x_array, float * y_array, float * z_array)
{
	ccmc::Tracer * tracer = tracerObjects[id];
	tracer->setMaxIterations(*step_max);
	if (dn < 0)
	{
		tracer->setDn(-*dn);
		Fieldline fieldline = tracer->unidirectionalTrace(variable, *startComponent1, *startComponent2, *startComponent3, ccmc::Tracer::REVERSE);
		*actual_steps = fieldline.size();
		for (int i = 0; i < fieldline.size(); i++)
		{
			x_array[i] = fieldline.getPositions()[i].component1;
			y_array[i] = fieldline.getPositions()[i].component2;
			z_array[i] = fieldline.getPositions()[i].component3;
		}
	}else
	{
		tracer->setDn(*dn);
		Fieldline fieldline = tracer->unidirectionalTrace(variable, *startComponent1, *startComponent2, *startComponent3, ccmc::Tracer::FOWARD);
		*actual_steps = fieldline.size();
		for (int i = 0; i < fieldline.size(); i++)
		{
			x_array[i] = fieldline.getPositions()[i].component1;
			y_array[i] = fieldline.getPositions()[i].component2;
			z_array[i] = fieldline.getPositions()[i].component3;
		}
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
		return 1;
}

int Kameleon_get_variable_id(int id, const char * variable)
{
	ccmc::Kameleon * kameleon = ccmc::kameleonObjects[id];
	int variable_id = kameleon->getVariableID(variable);
//	std::cout << "from c. kid: " << id << " variable_id: " << variable_id << std::endl;
	return variable_id;
}

int Tracer_setInnerBoundary(int tid, float innerBoundary)
{
	//TODO: error checking
	map_i_T::iterator iter = ccmc::tracerObjects.find(tid);
	if (iter != ccmc::tracerObjects.end())
	{
		(*iter).second->setInnerBoundary(innerBoundary);
		return 0;
	} else
		return 1;
}


