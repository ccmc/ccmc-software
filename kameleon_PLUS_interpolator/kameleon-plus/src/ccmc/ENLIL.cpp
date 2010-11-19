/*
 * ENLIL.cpp
 *
 *  Created on: Jul 9, 2009
 *      Author: dberrios
 */

#include "ENLIL.h"
#include "ENLILInterpolator.h"
#include "Constants.h"
#include "StringConstants.h"

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
		//open the file
		long status;
		status = openFile(filename);

		//check the names of the components
		if (this->doesVariableExist(ccmc::strings::variables::r_))
		{
			r_string = ccmc::strings::variables::r_;
			lat_string = ccmc::strings::variables::phi_;
			lon_string = ccmc::strings::variables::theta_;
		} else
		{
			r_string = ccmc::strings::variables::x_;
			lat_string = ccmc::strings::variables::y_;
			lon_string = ccmc::strings::variables::z_;
		}

		long success = loadVariable(r_string);
		if (success != FileReader::OK)
			return success;
		success = loadVariable(lat_string);
		if (success != FileReader::OK)
			return success;
		loadVariable(lon_string);
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
