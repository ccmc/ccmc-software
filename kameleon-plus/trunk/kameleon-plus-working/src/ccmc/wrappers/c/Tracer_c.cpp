/*
 * Tracer_c.cpp
 *
 *  Created on: Jun 9, 2010
 *      Author: dberrios
 */

#include "Tracer_c.h"
#include <ccmc/Tracer.h>
#include <ccmc/Kameleon.h>
#include "Kameleon_c.h"
#include <ccmc/Point3f.h>
#include <ccmc/Fieldline.h>

using namespace ccmc;
typedef boost::unordered_map<int, ccmc::Tracer*> map_i_T;
// extern _C_ boost::unordered_map<int, ccmc::Kameleon *> ccmc::kameleonObjects;

int Tracer_create(int kid)
{
	// std::cout <<"Tracer_create number of kameleon objects: " << ccmc::kameleonObjects.size() << std::endl;
	// std::cout<< "kameleonObjects address:" << & kameleonObjects << std::endl;

	// std::cout <<"Tracer_create looking for kameleon object with id: " << kid << std::endl;
	if (ccmc::kameleonObjects.find(kid) != ccmc::kameleonObjects.end())
	{
		// std::cout<<"kameleon object was found in Tracer_create" << std::endl;
		int maxObjects = 4096;
		int id = 0;
		while (tracerObjects.find(id) != tracerObjects.end())
		{
			id++;
			if (id > maxObjects)
				return -1;
		}
		//first check if the id exists.  if so, delete it and create a new one.
		Kameleon * kameleon = ccmc::kameleonObjects[kid];
		ccmc::tracerObjects[id] = new Tracer(kameleon);
		return id;
	} 
	else
	{
		// std::cout << "Tracer_create could not find kameleon object" << std::endl;
		// std::cout << "number of kameleon objects: " << ccmc::kameleonObjects.size() << std::endl;
		// std::cout<< "kameleonObjects address:" << & kameleonObjects << std::endl;
		return -1;
	}


}

void Tracer_bidirectionalTrace(int id, const char * variable, const float* startComponent1, const float* startComponent2,
		const float* startComponent3, const int* step_max, const float* dn, int * actual_steps, float * x_array, float * y_array, float * z_array)
{
	
	// std::cout << "tracerObject id:" << id << std::endl;
	// std::cout << "before asigning to std::string" << std::endl;
	std::string variable_str = variable;
	// std::cout << "after asigning to std::string" << std::endl;
	// std::cout << "finding tracerObject" << std::endl;
	map_i_T::iterator iter = ccmc::tracerObjects.find(id);
	
	if (iter != ccmc::tracerObjects.end())
	{
		// std::cout << "setting dn:" << *dn << std::endl;
		(*iter).second->setDn(*dn);
		// std::cout << "setting max iterations: " << *step_max << std::endl;
		(*iter).second->setMaxIterations(*step_max);
		// std::cout << "before tracing with variable: '" << variable_str << "'" << std::endl;
		ccmc::Fieldline fieldline = ((*iter).second)->bidirectionalTrace(variable_str, *startComponent1, *startComponent2, *startComponent3);
		// std::cout << "after tracing" << std::endl;
		*actual_steps = fieldline.size();
		std::vector<ccmc::Point3f> positions = fieldline.getPositions();
		// std::cout <<"positions length: " << positions.size(); 
		for (int i = 0; i < positions.size(); i++)
		{
			x_array[i] = positions[i].component1;
			y_array[i] = positions[i].component2;
			z_array[i] = positions[i].component3;
		}
	}
	else{
		std::cout <<"could not find tracer object!"<< std::endl;
	}

}

void Tracer_setInnerBoundary(int id, float innerBoundary){
  ccmc::Tracer * tracer = tracerObjects[id];
  tracer->setInnerBoundary(innerBoundary);
}

void Tracer_unidirectionalTrace(int id, const char * variable, const float * startComponent1, const float * startComponent2,
		const float * startComponent3, const int * step_max, const float * dn, int * actual_steps, float * x_array, float * y_array, float * z_array)
{
	std::string variable_str = variable;
	ccmc::Tracer * tracer = tracerObjects[id];
	tracer->setMaxIterations(*step_max);
	if (dn < 0)
	{
		tracer->setDn(-*dn);
		Fieldline fieldline = tracer->unidirectionalTrace(variable_str, *startComponent1, *startComponent2, *startComponent3, ccmc::Tracer::REVERSE);
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
		Fieldline fieldline = tracer->unidirectionalTrace(variable_str, *startComponent1, *startComponent2, *startComponent3, ccmc::Tracer::FOWARD);
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
