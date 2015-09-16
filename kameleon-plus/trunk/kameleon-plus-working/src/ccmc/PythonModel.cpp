#include "PythonModel.h"
#include "Point.h"
#include "Vector.h"
#include "PythonInterpolator.h"
#include "StringConstants.h"
#include "GeneralFileReader.h"
#include <iostream>


namespace ccmc
{
	/**
	 * Default constructor
	 */
	PythonModel::PythonModel()
	{
		// default constructor
		this->busyStatus = Model::OK;
		// std::cout <<"python model constructor called" << std::endl;

	}

	/**
	 * @copydoc Model::open()
	 */
	long PythonModel::open(const std::string& filename)
	{
		this->setBusyStatus(Model::BUSY);
		long status;
		// std::cout << "calling GeneralFileReader open function"<<std::endl;
		status = GeneralFileReader::open(filename);

		initializeSIUnits();
		initializeConversionFactorsToSI();
		this->setBusyStatus(Model::OK);
		return status;
	}


	Interpolator * PythonModel::createNewInterpolator()
	{
		// std::cout <<"creating new PythonModel interpolator\n";
		Interpolator * interpolator = new PythonInterpolator(this);

		return interpolator;
	}

	/**
	 * @copydoc Model::initializeConversionFactorsToSI()
	 */
	void PythonModel::initializeConversionFactorsToSI()
	{
//		std::cout << "PythonModel::initializeConversionFactorsToSI()" << std::endl;
#ifdef DEBUG
		std::cout << "PythonModel::initializeConversionFactorsToSI need to fix" << std::endl;
#endif
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
	void PythonModel::initializeSIUnits()
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
		variableSIUnits["ex"] = "v/m";
		variableSIUnits["ey"] = "v/m";
		variableSIUnits["ez"] = "v/m";
	}


	/**
	 *
	 */
	PythonModel::~PythonModel()
	{
		// TODO Auto-generated destructor stub
		// std::cout << "deleting PythonModel object" << std::endl;
	}

}
