/*
 * Kameleon_compatibility.cpp
 *
 *  Created on: Jul 8, 2009
 *      Author: dberrios
 */
#include "Kameleon_c.h"
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
	//create new kameleon object
	kameleonObjects[id] = new Kameleon();
	// std::cout<<"Kameleon_create: created new kameleon object with id: " << id << std::endl;
	// std::cout<<"Kameleon_create: new kameleonObject pointer: "<< kameleonObjects[id]<<std::endl; 
	if (kameleonObjects.find(id) == kameleonObjects.end()){
		std::cout<<"kameleon object was not found" << std::endl;
	}
	else{
		// std::cout<<"kameleon object was found" << std::endl;
		// std::cout << "number of kameleon objects: " << kameleonObjects.size() << std::endl;
		// std::cout<< "kameleonObjects address:" << & kameleonObjects << std::endl;
	}
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
	std::string filename_str = filename;
	int status;
	map_i_K::iterator iter = kameleonObjects.find(id);

	if (iter != kameleonObjects.end())
	{
		//first, open the file

		ccmc::Kameleon * kameleon = (*iter).second;
//		std::cout << "before calling kameleon->open" << std::endl;
		status = kameleon->open(filename_str);
//		std::cout << "filename: " << filename << " status: " << status << " after" << std::endl;
		//kameleonObjects.erase(iter);
	} else
	{

		//should never happen
		std::cerr << "this should never happen!" << std::endl;

	}
//	std::cout << "new size of kameleonObjects: " << kameleonObjects.size() << std::endl;
	return status;
}

int Kameleon_close(int id)
{
	//TODO: error checking
	ccmc::Kameleon * kameleon = kameleonObjects[id];
	int status = kameleon->close();
	return status;
//	std::cout << "new size of kameleonObjects: " << kameleonObjects.size() << std::endl;
}

int Kameleon_delete(int id)
{
	//TODO: error checking
	map_i_K::iterator iter = kameleonObjects.find(id);
	if (iter != kameleonObjects.end())
	{
		delete (*iter).second;
		kameleonObjects.erase(iter);
		return 0;
	} else
		return 1;
}

void Kameleon_get_model_name(int id, char * model_name)
{
	//TODO: error checking
	ccmc::Kameleon * kameleon = kameleonObjects[id];
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
	std::string variable_str = variable;
//	std::cout << "variable: " << variable << std::endl;
	ccmc::Kameleon * kameleon = kameleonObjects[id];

	int status = kameleon->loadVariable(variable_str);
	return status;
}

int Kameleon_unload_variable(int id, char * variable)
{
	//TODO: error checking
	std::string variable_str = variable;
//	std::cout << "variable: " << variable << std::endl;
	ccmc::Kameleon * kameleon = kameleonObjects[id];

	int status = kameleon->unloadVariable(variable_str);
	return status;
}

int Kameleon_load_vector_variable(int id, const char * variable)
{
	//TODO: error checking
	// std::cout << "Kameleon_load_vector_variable number of kameleon objects: " << kameleonObjects.size() << std::endl;
	std::string variable_str = variable;
	ccmc::Kameleon * kameleon = kameleonObjects[id];
	int status = kameleon->loadVectorVariable(variable_str);
	return status;
}

void Kameleon_get_global_attribute_string(int id, const char * gAttribute, char * destbuffer)
{
	//TODO: error checking
	std::string gAttribute_str = gAttribute;
	ccmc::Kameleon * kameleon = kameleonObjects[id];
	std::string attribute = (kameleon->getGlobalAttribute(gAttribute_str)).getAttributeString();
	strncpy(destbuffer, attribute.c_str(), attribute.size());
	destbuffer[attribute.size()] = '\0';
}

float Kameleon_get_missing_value(int id)
{
	//TODO: error checking
	ccmc::Kameleon * kameleon = kameleonObjects[id];
	return kameleon->getMissingValue();
}

void Kameleon_get_native_unit(int id, const char * variable, char * destbuffer)
{
	//TODO: error checking
	std::string variable_str = variable;
	ccmc::Kameleon * kameleon = kameleonObjects[id];
	std::string units = kameleon->getNativeUnit(variable_str);
	strncpy(destbuffer, units.c_str(), units.size());
}

void Kameleon_get_vis_unit(int id, const char * variable, char * destbuffer)
{
	std::string variable_str = variable;
	ccmc::Kameleon * kameleon = kameleonObjects[id];
	std::string units = kameleon->getVisUnit(variable_str);
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

int Kameleon_get_variable_id(int id, const char * variable)
{
	std::string variable_str = variable;
	ccmc::Kameleon * kameleon = kameleonObjects[id];
	int variable_id = kameleon->getVariableID(variable_str);
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


