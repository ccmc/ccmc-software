/*
 * ENLIL.cpp
 *
 *  Created on: Jul 9, 2009
 *      Author: dberrios
 */

#include "ENLIL.h"
#include "ENLILInterpolator.h"

namespace ccmc
{

	/**
	 *
	 */
	ENLIL::ENLIL()
	{
		// TODO Auto-generated constructor stub

	}

	/**
	 * @copybrief Model::open
	 *
	 * @copydetails Model::open
	 */
	long ENLIL::open(const std::string& filename)
	{
		long status;
		status = openFile(filename);
		long success = loadVariable("r");
		if (success != FileReader::OK)
			return success;
		success = loadVariable("phi");
		if (success != FileReader::OK)
			return success;
		loadVariable("theta");
		if (success != FileReader::OK)
			return success;
		initializeSIUnits();
		initializeConversionFactorsToSI();
		return status;
	}

	/**
	 * @copydoc Model::createNewInterpolator()
	 */
	Interpolator* ENLIL::createNewInterpolator()
	{
		Interpolator * interpolator = new ENLILInterpolator(this);
		return interpolator;
	}

	/**
	 * @copydoc Model::initializeConversionFactorsToSI()
	 */
	void ENLIL::initializeConversionFactorsToSI()
	{

	}

	/**
	 * @copydoc Model::initializeSIUnits()
	 */
	void ENLIL::initializeSIUnits()
	{

	}

	/**
	 *
	 */
	ENLIL::~ENLIL()
	{
		// TODO Auto-generated destructor stub
	}
}
