/*
 * ENLIL.cpp
 *
 *  Created on: Jul 9, 2009
 *      Author: dberrios
 */

#include "ENLIL.h"
#include "ENLILInterpolator.h"

ENLIL::ENLIL()
{
	// TODO Auto-generated constructor stub

}

long ENLIL::open(const std::string&)
{
	long status;
	status = openFile(filename);

	loadVariable(r_string);
	loadVariable(lat_string);
	loadVariable(lon_string);
	initializeSIUnits();
	initializeConversionFactorsToSI();
}

Interpolator* ENLIL::createNewInterpolator()
{
	Interpolator * interpolator = new ENLILInterpolator(this);
	return interpolator;
}


void ENLIL::initializeConversionFactorsToSI()
{

}


void ENLIL::initializeSIUnits()
{

}

ENLIL::~ENLIL()
{
	// TODO Auto-generated destructor stub
}
