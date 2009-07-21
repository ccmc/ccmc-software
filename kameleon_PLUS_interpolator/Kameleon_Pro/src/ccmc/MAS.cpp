/*
 * MAS.cpp
 *
 *  Created on: Jul 9, 2009
 *      Author: dberrios
 */

#include "MAS.h"
#include "Interpolator.h"
#include "MASInterpolator.h"

namespace ccmc
{
	MAS::MAS()
	{
		// TODO Auto-generated constructor stub

	}

	long MAS::open(const std::string&)
	{
		long status;
		status = openFile(filename);

		loadVariable(r_string);
		loadVariable(lat_string);
		loadVariable(lon_string);
		initializeSIUnits();
		initializeConversionFactorsToSI();
	}

	Interpolator* MAS::createNewInterpolator()
	{
		Interpolator * interpolator = new MASInterpolator(this);
		return interpolator;
	}

	void MAS::initializeConversionFactorsToSI()
	{

	}

	void MAS::initializeSIUnits()
	{

	}

	MAS::~MAS()
	{
		// TODO Auto-generated destructor stub
	}
}
