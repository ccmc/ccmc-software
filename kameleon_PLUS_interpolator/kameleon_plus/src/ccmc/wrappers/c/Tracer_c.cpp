/*
 * Tracer_c.cpp
 *
 *  Created on: Jun 9, 2010
 *      Author: dberrios
 */

#include "Tracer_c.h"
#include <ccmc/Kameleon.h>
#include <ccmc/Tracer.h>
#include "Kameleon_c.h"
#include <ccmc/Point3f.h>
#include <ccmc/Fieldline.h>

using namespace ccmc;



//int Tracer_create(int kid)
//{
//	Kameleon * kameleon = NULL;
//
//	if (kameleonObjects.find(kid))
//	{
//		kameleon = kameleonObjects[kid];
//	}
//	else
//		return -1;
//
//	int maxObjects = 4096;
//	int tid = 0;
//	while (tracerObjects.find(tid) != tracerObjects.end())
//	{
//		tid++;
//		if (tid > maxObjects)
//		{
//			return -1;
//		}
//
//	}
//	tracerObjects[tid] = new Tracer(kameleon);
//	return tid;
//}
////extern _C_ void Tracer_open(int id, const char * filename);
////extern _C_ void Tracer_loadVariable(int id, const char * variable);
//
//void Tracer_bidirectionalTrace(int id, const char * variable, const float& startComponent1, const float& startComponent2,
//		const float& startComponent3, const int& step_max, const float& dn, int * actual_steps, float * x_array, float * y_array, float * z_array)
//{
//	Tracer * tracer = tracerObjects[id];
//	Tracer::Direction direction;
//
//	tracer->setDn(dn);
//	Fieldline fieldline = tracer->bidirectionalTrace(variable, startComponent1, startComponent2, startComponent3);
//	*actual_steps = fieldline.size();
//	for (int i = 0; i < fieldline.size(); i++)
//	{
//		x_array[i] = fieldline.getPositions()[i];
//		y_array[i] = fieldline.getPositions()[i];
//		z_array[i] = fieldline.getPositions()[i];
//	}
//}
//
//void Tracer_unidirectionalTrace(int id, const char * variable, const float& startComponent1, const float& startComponent2,
//		const float& startComponent3, const int& step_max, const float& dn, int * actual_steps, float * x_array, float * y_array, float * z_array)
//{
//	Tracer * tracer = tracerObjects[id];
//	Tracer::Direction direction;
//	if (dn < 0)
//	{
//		tracer->setDn(-dn);
//		direction = Tracer::Direction::REVERSE;
//	}else
//	{
//		tracer->setDn(dn);
//		direction = Tracer::Direction::FOWARD;
//	}
//
//	Fieldline fieldline = tracer->unidirectionalTrace(variable, startComponent1, startComponent2, startComponent3, direction);
//	*actual_steps = fieldline.size();
//	for (int i = 0; i < fieldline.size(); i++)
//	{
//		x_array[i] = fieldline.getPositions()[i];
//		y_array[i] = fieldline.getPositions()[i];
//		z_array[i] = fieldline.getPositions()[i];
//	}
//}
//
//int Tracer_delete(int id)
//{
//
//}
