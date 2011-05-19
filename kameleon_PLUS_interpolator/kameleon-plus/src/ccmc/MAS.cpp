/*
 * MAS.cpp
 *
 *  Created on: Jul 9, 2009
 *      Author: dberrios
 */

#include "MAS.h"
#include "Interpolator.h"
#include "MASInterpolator.h"
#include "StringConstants.h"

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
	long MAS::open(const std::string& filename)
	{
		long status;
		status = openFile(filename);
		initializeMaps();
		long success = loadVariable(ccmc::strings::variables::r_);
		if (success != FileReader::OK)
			return success;
		success = loadVariable(ccmc::strings::variables::r1_);
		if (success != FileReader::OK)
					return success;
		success = loadVariable(ccmc::strings::variables::theta_);
		if (success != FileReader::OK)
			return success;
		success = loadVariable(ccmc::strings::variables::phi_);
		if (success != FileReader::OK)
			return success;
		success = loadVariable(ccmc::strings::variables::theta1_);
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

	const std::vector<float>* MAS::getRPosGrid(std::string variable)
	{
		boost::unordered_map<std::string, std::string>::iterator iter = this->rPosGrid.find(variable);
		if (iter != rPosGrid.end())
			return this->getVariableData((*iter).second);
		else
			return this->getVariableData(ccmc::strings::variables::r_);
	}

	const std::vector<float>* MAS::getLatPosGrid(std::string variable)
	{
		boost::unordered_map<std::string, std::string>::iterator iter = this->latPosGrid.find(variable);
		if (iter != latPosGrid.end())
			return this->getVariableData((*iter).second);
		else
			return this->getVariableData(ccmc::strings::variables::theta_);
	}

	const std::vector<float>* MAS::getRPosGridByID(long variable_id)
	{
		boost::unordered_map<long, std::string>::iterator iter = this->rPosGridByID.find(variable_id);
		if (iter != rPosGridByID.end())
			return this->getVariableData((*iter).second);
		else
			return this->getVariableData(ccmc::strings::variables::r_);
	}

	const std::vector<float>* MAS::getLatPosGridByID(long variable_id)
	{
		boost::unordered_map<long, std::string>::iterator iter = this->latPosGridByID.find(variable_id);
		if (iter != latPosGridByID.end())
			return this->getVariableData((*iter).second);
		else
			return this->getVariableData(ccmc::strings::variables::theta_);
	}

	void MAS::initializeMaps()
	{

		//rPosGrid for the different variables
		rPosGrid["br"] = ccmc::strings::variables::r1_;
		rPosGrid["jphi"] = ccmc::strings::variables::r1_;
		rPosGrid["jtheta"] = ccmc::strings::variables::r1_;
		rPosGrid["uphi"] = ccmc::strings::variables::r1_;
		rPosGrid["utheta"] = ccmc::strings::variables::r1_;
		rPosGrid["rho"] = ccmc::strings::variables::r1_;
		rPosGrid["p"] = ccmc::strings::variables::r1_;
		rPosGrid["T"] = ccmc::strings::variables::r1_;

		rPosGridByID[this->getVariableID("br")] = ccmc::strings::variables::r1_;
		rPosGridByID[this->getVariableID("jphi")] = ccmc::strings::variables::r1_;
		rPosGridByID[this->getVariableID("jtheta")] = ccmc::strings::variables::r1_;
		rPosGridByID[this->getVariableID("uphi")] = ccmc::strings::variables::r1_;
		rPosGridByID[this->getVariableID("utheta")] = ccmc::strings::variables::r1_;
		rPosGridByID[this->getVariableID("rho")] = ccmc::strings::variables::r1_;
		rPosGridByID[this->getVariableID("p")] = ccmc::strings::variables::r1_;
		rPosGridByID[this->getVariableID("T")] = ccmc::strings::variables::r1_;

		//latPosGrid for the different variables
		latPosGrid["btheta"] = ccmc::strings::variables::theta1_;
		latPosGrid["jr"] = ccmc::strings::variables::theta1_;
		latPosGrid["jphi"] = ccmc::strings::variables::theta1_;
		latPosGrid["ur"] = ccmc::strings::variables::theta1_;
		latPosGrid["uphi"] = ccmc::strings::variables::theta1_;
		latPosGrid["rho"] = ccmc::strings::variables::theta1_;
		latPosGrid["p"] = ccmc::strings::variables::theta1_;
		latPosGrid["T"] = ccmc::strings::variables::theta1_;

		latPosGridByID[this->getVariableID("btheta")] = ccmc::strings::variables::theta1_;
		latPosGridByID[this->getVariableID("jr")] = ccmc::strings::variables::theta1_;
		latPosGridByID[this->getVariableID("jphi")] = ccmc::strings::variables::theta1_;
		latPosGridByID[this->getVariableID("ur")] = ccmc::strings::variables::theta1_;
		latPosGridByID[this->getVariableID("uphi")] = ccmc::strings::variables::theta1_;
		latPosGridByID[this->getVariableID("rho")] = ccmc::strings::variables::theta1_;
		latPosGridByID[this->getVariableID("p")] = ccmc::strings::variables::theta1_;
		latPosGridByID[this->getVariableID("T")] = ccmc::strings::variables::theta1_;

		//change sign flag
		changeSignFlag[ccmc::strings::variables::btheta_] = true;
		changeSignFlag[ccmc::strings::variables::jtheta_] = true;
		changeSignFlag[ccmc::strings::variables::utheta_] = true;

		changeSignFlagByID[this->getVariableID(ccmc::strings::variables::btheta_)] = true;
		changeSignFlagByID[this->getVariableID(ccmc::strings::variables::jtheta_)] = true;
		changeSignFlagByID[this->getVariableID(ccmc::strings::variables::utheta_)] = true;

	}

	bool MAS::getChangeSignFlag(std::string variable)
	{
		boost::unordered_map<std::string, bool>::iterator iter = this->changeSignFlag.find(variable);
		if (iter != changeSignFlag.end())
			return (*iter).second;
		else
			return false;

	}

	bool MAS::getChangeSignFlagByID(long variable_id)
	{
		boost::unordered_map<long, bool>::iterator iter = this->changeSignFlagByID.find(variable_id);
		if (iter != changeSignFlagByID.end())
			return (*iter).second;
		else
			return false;
	}

	/**
	 *
	 */
	MAS::~MAS()
	{
		// TODO Auto-generated destructor stub
	}
}
