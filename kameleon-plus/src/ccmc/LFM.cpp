/*
 * LFM.cpp
 *
 *  Created on: Dec 4, 2012
 *      Author: Brian Curtis
 */

#include "LFM.h"
#include "Point.h"
#include "Cell3D.h"
#include "Vector.h"
#include "LFMInterpolator.h"
#include "StringConstants.h"
#include "GeneralFileReader.h"

namespace ccmc
{
	/**
	 * Default constructor
	 */
	LFM::LFM()
	{
		// TODO Auto-generated constructor stub

		previous_x = missingValue;
		previous_y = missingValue;
		previous_z = missingValue;
		this->busyStatus = Model::OK;

	}

	/**
	 * @copydoc Model::open()
	 */
	long LFM::open(const std::string& filename)
	{
		this->setBusyStatus(Model::BUSY);
		long status;
		status = GeneralFileReader::open(filename);
		this->progress = 20;
		loadVariable(ccmc::strings::variables::x_);
		this->progress = 40;
		loadVariable(ccmc::strings::variables::y_);
		this->progress = 60;
		loadVariable(ccmc::strings::variables::z_);
		this->progress = 80;
		initializeSIUnits();
		initializeConversionFactorsToSI();
		this->progress = 100;
//		cout << "testing open in OpenGGCM class" << endl;
		this->setBusyStatus(Model::OK);
		return status;
	}

	const std::vector<std::string> LFM::getLoadedVariables()
	{
		std::vector<std::string> requiredVariables;
		requiredVariables.push_back(ccmc::strings::variables::x_bx_);
		requiredVariables.push_back(ccmc::strings::variables::x_by_);
		requiredVariables.push_back(ccmc::strings::variables::x_bz_);
		requiredVariables.push_back(ccmc::strings::variables::y_bx_);
		requiredVariables.push_back(ccmc::strings::variables::y_by_);
		requiredVariables.push_back(ccmc::strings::variables::y_bz_);
		requiredVariables.push_back(ccmc::strings::variables::z_bx_);
		requiredVariables.push_back(ccmc::strings::variables::z_by_);
		requiredVariables.push_back(ccmc::strings::variables::z_bz_);

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
	Interpolator * LFM::createNewInterpolator()
	{
		Interpolator * interpolator = new LFMInterpolator(this);

		return interpolator;
	}

	/**
	 * @copydoc Model::initializeConversionFactorsToSI()
	 */
	void LFM::initializeConversionFactorsToSI()
	{
//		std::cout << "OpenGGCM::initializeConversionFactorsToSI()" << std::endl;
		conversionFactorsToSI["bx1"] = 1e-9f;
		conversionFactorsToSI["by1"] = 1e-9f;
		conversionFactorsToSI["bz1"] = 1e-9f;
		conversionFactorsToSI["bx"] = 1e-9f;
		conversionFactorsToSI["by"] = 1e-9f;
		conversionFactorsToSI["bz"] = 1e-9f;
		conversionFactorsToSI["ux"] = 1e3f;
		conversionFactorsToSI["uy"] = 1e3f;
		conversionFactorsToSI["uz"] = 1e3f;
		conversionFactorsToSI["jx"] = 1e3f;
		conversionFactorsToSI["jy"] = 1e3f;
		conversionFactorsToSI["jz"] = 1e3f;
		conversionFactorsToSI["rho"] = 1e6f;
		conversionFactorsToSI["p"] = 1e-12f;

	}

	/**
	 * @copydoc Model::initializeSIUnits()
	 */
	void LFM::initializeSIUnits()
	{

		variableSIUnits["bx1"] = "T";
		variableSIUnits["by1"] = "T";
		variableSIUnits["bz1"] = "T";
		variableSIUnits["bx"] = "T";
		variableSIUnits["by"] = "T";
		variableSIUnits["bz"] = "T";
		variableSIUnits["ux"] = "m/s";
		variableSIUnits["uy"] = "m/s";
		variableSIUnits["uz"] = "m/s";
		variableSIUnits["jx"] = "A/m^2";
		variableSIUnits["jy"] = "A/m^2";
		variableSIUnits["jz"] = "A/m^2";
		variableSIUnits["rho"] = "m^-3";
		variableSIUnits["p"] = "Pa";
	}

	void LFM::initializeMaps()
	{
		//Staggared grid
		xGrid["bx1"] = ccmc::strings::variables::x_bx_;
		xGrid["by1"] = ccmc::strings::variables::x_by_;
		xGrid["bz1"] = ccmc::strings::variables::x_bz_;

		yGrid["bx1"] = ccmc::strings::variables::y_bx_;
		yGrid["by1"] = ccmc::strings::variables::y_by_;
		yGrid["bz1"] = ccmc::strings::variables::y_bz_;

		zGrid["bx1"] = ccmc::strings::variables::z_bx_;
		zGrid["by1"] = ccmc::strings::variables::z_by_;
		zGrid["bz1"] = ccmc::strings::variables::z_bz_;

		xGridByID[this->getVariableID("bx1")] = ccmc::strings::variables::x_bx_;
		xGridByID[this->getVariableID("by1")] = ccmc::strings::variables::x_by_;
		xGridByID[this->getVariableID("bz1")] = ccmc::strings::variables::x_bz_;

		yGridByID[this->getVariableID("bx1")] = ccmc::strings::variables::y_bx_;
		yGridByID[this->getVariableID("by1")] = ccmc::strings::variables::y_by_;
		yGridByID[this->getVariableID("bz1")] = ccmc::strings::variables::y_bz_;

		zGridByID[this->getVariableID("bx1")] = ccmc::strings::variables::z_bx_;
		zGridByID[this->getVariableID("by1")] = ccmc::strings::variables::z_by_;
		zGridByID[this->getVariableID("bz1")] = ccmc::strings::variables::z_bz_;

	}

	std::string LFM::getXGridName(const std::string& variable)
	{
		boost::unordered_map<std::string, std::string>::iterator iter = this->xGrid.find(variable);
		if (iter != xGrid.end())
		{
			//std::cerr << "grid: " << (*iter).second << std::endl;
			return (*iter).second;
		}
		else
		{
			//std::cerr << "grid: " << ccmc::strings::variables::theta_ << std::endl;
			return ccmc::strings::variables::x_;
		}
	}


	std::string LFM::getXGridName(long variable)
	{
		boost::unordered_map<long, std::string>::iterator iter = this->xGridByID.find(variable);
		if (iter != xGridByID.end())
		{
			//std::cerr << "grid: " << (*iter).second << std::endl;
			return (*iter).second;
		}
		else
		{
			//std::cerr << "grid: " << ccmc::strings::variables::theta_ << std::endl;
			return ccmc::strings::variables::x_;
		}
	}

	std::string LFM::getYGridName(const std::string& variable)
	{
		boost::unordered_map<std::string, std::string>::iterator iter = this->yGrid.find(variable);
		if (iter != yGrid.end())
		{
			//std::cerr << "grid: " << (*iter).second << std::endl;
			return (*iter).second;
		}
		else
		{
			//std::cerr << "grid: " << ccmc::strings::variables::theta_ << std::endl;
			return ccmc::strings::variables::y_;
		}
	}


	std::string LFM::getYGridName(long variable)
	{
		boost::unordered_map<long, std::string>::iterator iter = this->yGridByID.find(variable);
		if (iter != yGridByID.end())
		{
			//std::cerr << "grid: " << (*iter).second << std::endl;
			return (*iter).second;
		}
		else
		{
			//std::cerr << "grid: " << ccmc::strings::variables::theta_ << std::endl;
			return ccmc::strings::variables::y_;
		}
	}

	std::string LFM::getZGridName(const std::string& variable)
	{
		boost::unordered_map<std::string, std::string>::iterator iter = this->zGrid.find(variable);
		if (iter != zGrid.end())
		{
			//std::cerr << "grid: " << (*iter).second << std::endl;
			return (*iter).second;
		}
		else
		{
			//std::cerr << "grid: " << ccmc::strings::variables::theta_ << std::endl;
			return ccmc::strings::variables::z_;
		}
	}


	std::string LFM::getZGridName(long variable)
	{
		boost::unordered_map<long, std::string>::iterator iter = this->zGridByID.find(variable);
		if (iter != zGridByID.end())
		{
			//std::cerr << "grid: " << (*iter).second << std::endl;
			return (*iter).second;
		}
		else
		{
			//std::cerr << "grid: " << ccmc::strings::variables::theta_ << std::endl;
			return ccmc::strings::variables::z_;
		}
	}


	const std::vector<float>* const LFM::getXGrid(const std::string& variable)
	{
		return this->getVariableFromMap(getXGridName(variable));
	}

	const std::vector<float>* const LFM::getXGrid(long variable)
	{
		return this->getVariableFromMap(getXGridName(variable));
	}

	const std::vector<float>* const LFM::getYGrid(const std::string& variable)
	{
		return this->getVariableFromMap(getYGridName(variable));
	}

	const std::vector<float>* const LFM::getYGrid(long variable)
	{
		return this->getVariableFromMap(getYGridName(variable));
	}

	const std::vector<float>* const LFM::getZGrid(const std::string& variable)
	{
		return this->getVariableFromMap(getZGridName(variable));
	}

	const std::vector<float>* const LFM::getZGrid(long variable)
	{
		return this->getVariableFromMap(getZGridName(variable));
	}

	/**
	 *
	 */
	LFM::~LFM()
	{
		// TODO Auto-generated destructor stub
//		std::cout << "deleting OpenGGCM object" << std::endl;
	}

}
