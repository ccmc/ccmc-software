/*
 * Kameleon_compatibility.cpp
 *
 *  Created on: Jul 8, 2009
 *      Author: dberrios
 */
#include "Kameleon_compatibility.h"
#include "Kameleon.h"

int Kameleon_create(int id)
{
	Kameleon * kameleon = new Kameleon();
	kameleonObjects[id] = kameleon;
	return id;
}

int Kameleon_open(int id, const char * filename)
{
	Kameleon * kameleon = kameleonObjects[id];
	int status = kameleon->open(filename);
	return status;
}

int Kameleon_close(int id)
{
	return 0;
}

int Kameleon_delete(int id)
{
	boost::unordered_map<int,Kameleon*>::iterator iter = kameleonObjects.find(id);
	if (iter != kameleonObjects.end())
	{
		delete (*iter).second;
		kameleonObjects.erase(iter);
		return 0;
	} else
		1;
}
