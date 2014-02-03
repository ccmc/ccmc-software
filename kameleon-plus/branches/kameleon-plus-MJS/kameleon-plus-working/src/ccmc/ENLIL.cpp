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
#include "GeneralFileReader.h"

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
		this->setBusyStatus(Model::BUSY);
		//open the file
		long status;
		status = GeneralFileReader::open(filename);
		initializeMaps();
		//check the names of the components
		if (this->doesVariableExist(ccmc::strings::variables::r_))
		{
			std::cout << "r exists" << std::endl;
			r_string = ccmc::strings::variables::r_;
			lat_string = ccmc::strings::variables::theta_;
			lon_string = ccmc::strings::variables::phi_;
		} else
		{
			return FileReader::VARIABLE_DOES_NOT_EXIST;
		}

		long success = loadVariable(r_string);
		std::cout << "loaded r" << std::endl;
		if (success != FileReader::OK)
			return success;
		success = loadVariable(lat_string);
		std::cout << "loaded lat" << std::endl;
		if (success != FileReader::OK)
			return success;
		loadVariable(lon_string);
		if (success != FileReader::OK)
			return success;
		initializeSIUnits();
		initializeConversionFactorsToSI();
		this->setBusyStatus(Model::OK);
		return status;
	}

	const std::vector<std::string> ENLIL::getLoadedVariables()
	{
		std::vector<std::string> requiredVariables;
		requiredVariables.push_back(r_string);
		requiredVariables.push_back(lat_string);
		requiredVariables.push_back(lon_string);

		std::vector<std::string> variablesLoaded = Model::getLoadedVariables();

		for (int i = 0; i < requiredVariables.size(); i++)
		{
			int size = variablesLoaded.size();
			for (int j = 0; j < size; j++)
			{
				if (variablesLoaded[j] == requiredVariables[i])
				{
					variablesLoaded.erase(variablesLoaded.begin() + j);
					j = size;
				}


			}
		}
		return variablesLoaded;
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
