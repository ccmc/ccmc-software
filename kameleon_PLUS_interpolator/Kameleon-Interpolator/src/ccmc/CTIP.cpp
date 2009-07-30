/*
 * CTIP.cpp
 *
 *  Created on: Jul 24, 2009
 *      Author: dberrios
 */

#include "CTIP.h"

namespace ccmc
{

	CTIP::CTIP()
	{
		// TODO Auto-generated constructor stub

	}

	long CTIP::open(const std::string&)
	{
		return 0;
	}

	/**
	 * @copydoc Model::createNewInterpolator()
	 */
	Interpolator* CTIP::createNewInterpolator()
	{
		return NULL;
	}

	/**
	 * @copydoc Model::initializeConversionFactorsToSI()
	 */
	void CTIP::initializeConversionFactorsToSI()
	{

	}

	/**
	 * @copydoc Model::initializeSIUnits()
	 */
	void CTIP::initializeSIUnits()
	{

	}

	CTIP::~CTIP()
	{
		// TODO Auto-generated destructor stub
	}

}
