/*
 * MAS.cpp
 *
 *  Created on: Jul 9, 2009
 *      Author: dberrios
 */

#include "MAS.h"
#include "Interpolator.h"
#include "GeneralFileReader.h"
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
		this->setBusyStatus(Model::BUSY);
		long status;
		status = GeneralFileReader::open(filename);
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
		success = loadVariable(ccmc::strings::variables::theta1_);
		if (success != FileReader::OK)
			return success;
		success = loadVariable(ccmc::strings::variables::phi_);
		if (success != FileReader::OK)
			return success;
		success = loadVariable(ccmc::strings::variables::phi1_);
		if (success != FileReader::OK)
			return success;

		initializeSIUnits();
		initializeConversionFactorsToSI();
		this->setBusyStatus(Model::OK);
		return status;
	}

	const std::vector<std::string> MAS::getLoadedVariables()
	{
		std::vector<std::string> requiredVariables;
		requiredVariables.push_back(ccmc::strings::variables::r_);
		requiredVariables.push_back(ccmc::strings::variables::r1_);
		requiredVariables.push_back(ccmc::strings::variables::theta_);
		requiredVariables.push_back(ccmc::strings::variables::theta1_);
		requiredVariables.push_back(ccmc::strings::variables::phi_);
		requiredVariables.push_back(ccmc::strings::variables::phi1_);

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

	const std::vector<float>* const MAS::getRPosGrid(const std::string& variable)
	{
		return this->getVariableFromMap(getRPosGridName(variable));
	}

	const std::vector<float>* const MAS::getRPosGrid(long variable_id)
	{
		return this->getVariableFromMap(getRPosGridName(variable_id));
	}

	std::string MAS::getRPosGridName(const std::string& variable)
	{
		boost::unordered_map<std::string, std::string>::iterator iter = this->rPosGrid.find(variable);
		if (iter != rPosGrid.end())
		{
			//std::cerr << "grid: " << (*iter).second << std::endl;
			return (*iter).second;
		} else
		{
			//std::cerr << "grid: " << ccmc::strings::variables::r_ << std::endl;
			return ccmc::strings::variables::r_;
		}
	}

	std::string MAS::getRPosGridName(long variable)
	{
		boost::unordered_map<long, std::string>::iterator iter = this->rPosGridByID.find(variable);
		if (iter != rPosGridByID.end())
		{
			//std::cerr << "grid: " << (*iter).second << std::endl;
			return (*iter).second;
		}
		else
		{
			//std::cerr << "grid: " << ccmc::strings::variables::r_ << std::endl;
			return ccmc::strings::variables::r_;
		}
	}

	const std::vector<float>* const MAS::getLatPosGrid(const std::string& variable)
	{
		return this->getVariableFromMap(this->getLatPosGridName(variable));
	}

	const std::vector<float>* const MAS::getLatPosGrid(long variable_id)
	{
		return this->getVariableFromMap(this->getLatPosGridName(variable_id));
	}

	std::string MAS::getLatPosGridName(const std::string& variable)
	{
		boost::unordered_map<std::string, std::string>::iterator iter = this->latPosGrid.find(variable);
		if (iter != latPosGrid.end())
		{
			//std::cerr << "grid: " << (*iter).second << std::endl;
			return (*iter).second;
		}
		else
		{
			//std::cerr << "grid: " << ccmc::strings::variables::theta_ << std::endl;
			return ccmc::strings::variables::theta_;
		}
	}
	std::string MAS::getLatPosGridName(long variable_id)
	{
		boost::unordered_map<long, std::string>::iterator iter = this->latPosGridByID.find(variable_id);
		if (iter != latPosGridByID.end())
		{
			//std::cerr << "grid: " << (*iter).second << std::endl;
			return (*iter).second;
		}
		else
		{
			//std::cerr << "grid: " << ccmc::strings::variables::theta_ << std::endl;
			return ccmc::strings::variables::theta_;
		}
	}

	const std::vector<float>* const MAS::getlonPosGrid(const std::string& variable)
	{
		return this->getVariableFromMap(this->getLonPosGridName(variable));
	}





	const std::vector<float>* const MAS::getLonPosGrid(long variable_id)
	{
		return this->getVariableFromMap(this->getLonPosGridName(variable_id));
	}

	std::string MAS::getLonPosGridName(const std::string& variable)
	{
		boost::unordered_map<std::string, std::string>::iterator iter = this->lonPosGrid.find(variable);
		if (iter != lonPosGrid.end())
		{
			//std::cerr << "grid: " << (*iter).second << std::endl;
			return (*iter).second;
		}
		else
		{
			//std::cerr << "grid: " << ccmc::strings::variables::theta_ << std::endl;
			return ccmc::strings::variables::theta_;
		}
	}

	std::string MAS::getLonPosGridName(long variable_id)
	{
		boost::unordered_map<long, std::string>::iterator iter = this->lonPosGridByID.find(variable_id);
		if (iter != lonPosGridByID.end())
		{
			//std::cerr << "grid: " << (*iter).second << std::endl;
			return (*iter).second;
		}
		else
		{
			//std::cerr << "grid: " << ccmc::strings::variables::theta_ << std::endl;
			return ccmc::strings::variables::phi_;
		}
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

		lonPosGrid["T"] = ccmc::strings::variables::phi1_;
		lonPosGrid["p"] = ccmc::strings::variables::phi1_;
		lonPosGrid["rho"] = ccmc::strings::variables::phi1_;
		lonPosGrid["bphi"] = ccmc::strings::variables::phi1_;
		lonPosGrid["ur"] = ccmc::strings::variables::phi1_;
		lonPosGrid["utheta"] = ccmc::strings::variables::phi1_;
		lonPosGrid["jr"] = ccmc::strings::variables::phi1_;
		lonPosGrid["jtheta"] = ccmc::strings::variables::phi1_;

		lonPosGridByID[this->getVariableID("T")] = ccmc::strings::variables::phi1_;
		lonPosGridByID[this->getVariableID("p")] = ccmc::strings::variables::phi1_;
		lonPosGridByID[this->getVariableID("rho")] = ccmc::strings::variables::phi1_;
		lonPosGridByID[this->getVariableID("bphi")] = ccmc::strings::variables::phi1_;
		lonPosGridByID[this->getVariableID("ur")] = ccmc::strings::variables::phi1_;
		lonPosGridByID[this->getVariableID("utheta")] = ccmc::strings::variables::phi1_;
		lonPosGridByID[this->getVariableID("jr")] = ccmc::strings::variables::phi1_;
		lonPosGridByID[this->getVariableID("jtheta")] = ccmc::strings::variables::phi1_;


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

	bool MAS::getChangeSignFlag(long variable_id)
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
