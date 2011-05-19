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
		initializeMaps();
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

	bool ENLIL::getChangeSignFlag(std::string variable)
	{
		boost::unordered_map<std::string, bool>::iterator iter = this->changeSignFlag.find(variable);
		if (iter != changeSignFlag.end())
			return (*iter).second;
		else
			return false;

	}

	bool ENLIL::getChangeSignFlagByID(long variable_id)
	{
		boost::unordered_map<long, bool>::iterator iter = this->changeSignFlagByID.find(variable_id);
		if (iter != changeSignFlagByID.end())
			return (*iter).second;
		else
			return false;
	}

	void ENLIL::initializeMaps()
	{
		//change sign flag
		changeSignFlag["btheta"] = true;
		changeSignFlag["b1theta"] = true;
		changeSignFlag["jtheta"] = true;
		changeSignFlag["utheta"] = true;

		changeSignFlagByID[this->getVariableID("btheta")] = true;
		changeSignFlagByID[this->getVariableID("b1theta")] = true;
		changeSignFlagByID[this->getVariableID("jtetha")] = true;
		changeSignFlagByID[this->getVariableID("utheta")] = true;
	}

	/**
	 *
	 */
	ENLIL::~ENLIL()
	{
		// TODO Auto-generated destructor stub
	}
}
