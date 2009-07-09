/*
 * Kameleon_compatibility.cpp
 *
 *  Created on: Jul 8, 2009
 *      Author: dberrios
 */
#include "Kameleon_compatibility.h"
#include "Kameleon.h"
#include <string>
#include <iostream>

//TODO: Add appropriate error checking.  These functions WILL segfault if the id does not exist and you attempt to access the member functions of a null pointer.
typedef boost::unordered_map<int, Kameleon*> map_i_K;
typedef boost::unordered_map<int, Interpolator*> map_i_I;
int Kameleon_create(int id)
{
	//first check if the id exists.  if so, delete it and create a new one.
	map_i_K::iterator iter = kameleonObjects.find(id);
	if (iter != kameleonObjects.end())
	{
		//Doh! a Kameleon object already exists.
		//Not sure what to do here, so we delete the current object first
		delete (*iter).second;

		map_i_I::iterator iter2 = interpolatorObjects.find(id);
		if (iter2 != interpolatorObjects.end())
		{
			delete (*iter2).second;
		} else
		{
			//should never happen, but insert a new entry anyway
			std::cout << "Inserting a new Interpolator.  Should never be here." << std::endl;
			interpolatorObjects[id];
		}

		//kameleonObjects.erase(iter);
		(*iter).second = new Kameleon();
	} else
	{
std::cout << "creating kameleon object" << std::endl;
		kameleonObjects[id] = new Kameleon();
		interpolatorObjects[id];

	}
	return id;
}

int Kameleon_open(int id, const char * filename)
{
	int status;
	map_i_K::iterator iter = kameleonObjects.find(id);

	if (iter != kameleonObjects.end())
	{
		//first, open the file

		Kameleon * kameleon = (*iter).second;

		status = kameleon->open(filename);
		std::cout << "after" << std::endl;
		map_i_I::iterator iter2 = interpolatorObjects.find(id);
		if (iter2 != interpolatorObjects.end())
		{
			delete (*iter2).second;
		}
		(*iter2).second = kameleon->createNewInterpolator();

		//kameleonObjects.erase(iter);
	} else
	{

		//should never happen

	}

	return status;
}

int Kameleon_close(int id)
{
	Kameleon * kameleon = kameleonObjects[id];
	int status = kameleon->close();
	return status;
}

int Kameleon_delete(int id)
{
	map_i_K::iterator iter = kameleonObjects.find(id);
	if (iter != kameleonObjects.end())
	{
		delete (*iter).second;
		kameleonObjects.erase(iter);
		return 0;
	} else
		1;
}

void Kameleon_get_model_name(int id, char * model_name)
{
	Kameleon * kameleon = kameleonObjects[id];
	std::string model_name_string = kameleon->getModelName();
	std::cout << "model_name_string: '" << model_name_string << "'" << std::endl;
	strncpy(model_name, model_name_string.c_str(), sizeof(char) * model_name_string.size());
	model_name[model_name_string.size()] = '\0';

}

float Kameleon_interpolate(int id, const char * variable, const float * c0, const float * c1, const float * c2, float * dc0, float * dc1, float * dc2)
{
	//first, fetch the interpolator
	Interpolator * interpolator = interpolatorObjects[id];
	return interpolator->interpolate(variable, *c0,*c1,*c2,*dc0,*dc1,*dc2);
}

int Kameleon_load_variable(int id, const char * variable)
{
	Kameleon * kameleon = kameleonObjects[id];
	int status = kameleon->loadVariable(variable);
	return status;
}

void Kameleon_get_global_attribute_string(int id, const char * gAttribute, char * destbuffer)
{
	Kameleon * kameleon = kameleonObjects[id];
	std::string attribute = (kameleon->getGlobalAttribute(destbuffer)).getAttributeString();
	strncpy(destbuffer, attribute.c_str(), attribute.size());
	destbuffer[attribute.size()] = '\0';
}
