/*
 * Tracer_c.cpp
 *
 *  Created on: Jun 9, 2010
 *      Author: dberrios
 */

#include "Tracer_c.h"
#include <ccmc/Kameleon.h>
#include <ccmc/Kameleon-Tracer.h>
#include <ccmc/Point3f.h>

using namespace ccmc;
typedef boost::unordered_map<int, ccmc::Tracer*> map_i_T;
typedef boost::unordered_map<int, ccmc::Kameleon*> map_i_K;
int Tracer_create(int id, int kid)
{
	//first check if the id exists.  if so, delete it and create a new one.
	map_i_T::iterator iter = tracerObjects.find(id);
	if (iter != tracerObjects.end())
	{
		//Doh! a Kameleon object already exists.
		//Not sure what to do here, so we delete the current object first
		delete (*iter).second;
		map_i_K::iterator iter2 = kameleonObjects.find(kid);
		if (iter2 != kameleonObjects.end())
			(*iter).second = new Tracer((*iter2).second);
		else
		{
			kameleonObjects[kid] = new Kameleon();
			(*iter).second = new Tracer(kameleonObjects[kid]);
		}
	} else
	{
std::cout << "creating kameleon object" << std::endl;
		map_i_K::iterator iter2 = kameleonObjects.find(kid);
		if (iter2 != kameleonObjects.end())
		{
			tracerObjects[id] = new Tracer((*iter2).second);
		}
		else
		{
			kameleonObjects[kid] = new Kameleon();
			tracerObjects[id] = new Tracer(kameleonObjects[kid]);
		}


	}
	return id;
}

void Tracer_bidirectionalTrace(int id, const char * variable, const float& startComponent1, const float& startComponent2,
		const float& startComponent3, const int& step_max, const float& dn, int * actual_steps, float * x_array, float * y_array, float * z_array)
{
	std::string variable_str = variable;
	map_i_T::iterator iter = tracerObjects.find(id);
	((*iter).second)->setMaxIterations(step_max);
	((*iter).second)->setDn(dn);
	Fieldline fieldline = ((*iter).second)->bidirectionalTrace(variable_str, startComponent1, startComponent2, startComponent3);
	*actual_steps = fieldline.size();
	std::vector<Point3f> positions = fieldline.getPositions();
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
	map_i_T::iterator iter = tracerObjects.find(id);
	if (iter != tracerObjects.end())
	{
		delete (*iter).second;
		tracerObjects.erase(iter);
		return 0;
	} else
		1;
}
