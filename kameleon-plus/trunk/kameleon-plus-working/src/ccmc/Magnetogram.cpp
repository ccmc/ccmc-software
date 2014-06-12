/*
 * Magnetogram.cpp
 *
 *  Created on: Sep 23, 2009
 *      Author: dberrios
 */

#include "Magnetogram.h"
#include "MagnetogramInterpolator.h"
#include "GeneralFileReader.h"

namespace ccmc
{

/**
 *
 */
	Magnetogram::Magnetogram()
	{
		// TODO Auto-generated constructor stub

	}

	/**
	 * @copybrief Model::open
	 *
	 * @copydetails Model::open
	 */
	long Magnetogram::open(const std::string& filename)
	{
		long status;
		status = GeneralFileReader::open(filename);

		//loadVariable(r_string);
		loadVariable(lat_string);//position components? not sure how they are stored
		loadVariable(lon_string);
		initializeSIUnits();
		initializeConversionFactorsToSI();
        return status;
	}

	/**
	 * @copydoc Model::createNewInterpolator()
	 */
	Interpolator* Magnetogram::createNewInterpolator()
	{
		Interpolator * interpolator = new MagnetogramInterpolator(this);
		return interpolator;
	}

	/**
	 * @copydoc Model::initializeConversionFactorsToSI()
	 */
	void Magnetogram::initializeConversionFactorsToSI()
	{

	}

	/**
	 * @copydoc Model::initializeSIUnits()
	 */
	void Magnetogram::initializeSIUnits()
	{

	}

	/**
	 *
	 */
	Magnetogram::~Magnetogram()
	{
		// TODO Auto-generated destructor stub
	}
}
