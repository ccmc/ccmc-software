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
	/**
	 *
	 */
	MAS::MAS()
	{
		// TODO Auto-generated constructor stub

	}

	/**
	 * @copydoc Model::open
	 */
	long MAS::open(const std::string&)
	{
		long status;
		status = openFile(filename);

		long success = loadVariable(r_string);
		if (success != FileReader::OK)
			return success;
		success = loadVariable(lat_string);
		if (success != FileReader::OK)
			return success;
		success = loadVariable(lon_string);
		if (success != FileReader::OK)
			return success;
		initializeSIUnits();
		initializeConversionFactorsToSI();
		return status;
	}

	/**
	 * @copydoc Model::createNewInterpolator()
	 */
	Interpolator* MAS::createNewInterpolator()
	{
		Interpolator * interpolator = new MASInterpolator(this);
		return interpolator;
	}

	/**
	 * @copydoc Model::initializeConversionFactorsToSI()
	 */
	void MAS::initializeConversionFactorsToSI()
	{

	}

	/**
	 * @copydoc Model::initializeSIUnits()
	 */
	void MAS::initializeSIUnits()
	{

	}

	/**
	 *
	 */
	MAS::~MAS()
	{
		// TODO Auto-generated destructor stub
	}
}
