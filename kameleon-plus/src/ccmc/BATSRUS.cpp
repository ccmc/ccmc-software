/*
 * BATSRUS.cpp
 *
 *  Created on: Jun 2, 2009
 *      Author: David Berrios
 */

#include "BATSRUS.h"
#include "Point.h"
#include "Cell3D.h"
#include "Vector.h"
#include "BATSRUSInterpolator.h"
#include "StringConstants.h"
#include "GeneralFileReader.h"


namespace ccmc
{
	/**
	 * Default constructor
	 */
	BATSRUS::BATSRUS()
	{

	}

	/**
	 * @copydoc Model::open()
	 */
	long BATSRUS::open(const std::string& filename)
	{
		long status;
		status = GeneralFileReader::open(filename);

		/********** get block_*_min/max values ************/
		bool success = false;
		success = loadVariable(ccmc::strings::variables::block_x_min_);
		if (success != FileReader::OK)
			return success;
		success = loadVariable(ccmc::strings::variables::block_x_max_);
		if (success != FileReader::OK)
			return success;
		success = loadVariable(ccmc::strings::variables::block_y_min_);
		if (success != FileReader::OK)
			return success;
		success = loadVariable(ccmc::strings::variables::block_y_max_);
		if (success != FileReader::OK)
			return success;
		success = loadVariable(ccmc::strings::variables::block_z_min_);
		if (success != FileReader::OK)
			return success;
		success = loadVariable(ccmc::strings::variables::block_z_max_);
		if (success != FileReader::OK)
			return success;

		/********** get block_child_count_array, block_*_center_array, & block_child_id_*_array values ************/
		success = loadVariableInt(ccmc::strings::variables::block_child_count_);
		if (success != FileReader::OK)
			return success;
		success = loadVariable(ccmc::strings::variables::block_x_center_);
		if (success != FileReader::OK)
			return success;
		success = loadVariable(ccmc::strings::variables::block_y_center_);
		if (success != FileReader::OK)
			return success;
		success = loadVariable(ccmc::strings::variables::block_z_center_);
		if (success != FileReader::OK)
			return success;
		success = loadVariableInt(ccmc::strings::variables::block_child_id_1_);
		if (success != FileReader::OK)
			return success;
		success = loadVariableInt(ccmc::strings::variables::block_child_id_2_);
		if (success != FileReader::OK)
			return success;
		success = loadVariableInt(ccmc::strings::variables::block_child_id_3_);
		if (success != FileReader::OK)
			return success;
		success = loadVariableInt(ccmc::strings::variables::block_child_id_4_);
		if (success != FileReader::OK)
			return success;
		success = loadVariableInt(ccmc::strings::variables::block_child_id_5_);
		if (success != FileReader::OK)
			return success;
		success = loadVariableInt(ccmc::strings::variables::block_child_id_6_);
		if (success != FileReader::OK)
			return success;
		success = loadVariableInt(ccmc::strings::variables::block_child_id_7_);
		if (success != FileReader::OK)
			return success;
		success = loadVariableInt(ccmc::strings::variables::block_child_id_8_);
		if (success != FileReader::OK)
			return success;

		/******** get values for block_at_amr_level *******/
		success = loadVariableInt(ccmc::strings::variables::block_at_amr_level_);
		if (success != FileReader::OK)
			return success;
		GeneralFileReader::initializeVariableNames();
		initializeConversionFactorsToSI();
		initializeSIUnits();

		return status;
	}

	/**
	 *
	 * @copydoc Model::initializeSIUnits()
	 *
	 */
	void BATSRUS::initializeSIUnits()
	{
		variableSIUnits[ccmc::strings::variables::bx_] = "T";
		variableSIUnits[ccmc::strings::variables::by_] = "T";
		variableSIUnits[ccmc::strings::variables::bz_] = "T";
		variableSIUnits[ccmc::strings::variables::ux_] = "m/s";
		variableSIUnits[ccmc::strings::variables::uy_] = "m/s";
		variableSIUnits[ccmc::strings::variables::uz_] = "m/s";
		variableSIUnits[ccmc::strings::variables::jx_] = "A/m^2";
		variableSIUnits[ccmc::strings::variables::jy_] = "A/m^2";
		variableSIUnits[ccmc::strings::variables::jz_] = "A/m^2";
		variableSIUnits[ccmc::strings::variables::b1x_] = "T";
		variableSIUnits[ccmc::strings::variables::b1y_] = "T";
		variableSIUnits[ccmc::strings::variables::b1z_] = "T";
		variableSIUnits[ccmc::strings::variables::rho_] = "m^-3";
		variableSIUnits[ccmc::strings::variables::p_] = "Pa";
		variableSIUnits[ccmc::strings::variables::e_] = "J/m^3";
	}


	/**
	 * @copydoc Model::initializeConversionFactorsToSI()
	 */
	void BATSRUS::initializeConversionFactorsToSI()
	{

		/**
		 * TODO: fix these conversion factors
		 */
		conversionFactorsToSI[ccmc::strings::variables::bx_] = 1e-9f;
		conversionFactorsToSI[ccmc::strings::variables::by_] = 1e-9f;
		conversionFactorsToSI[ccmc::strings::variables::bz_] = 1e-9f;
		conversionFactorsToSI[ccmc::strings::variables::ux_] = 1e3f;
		conversionFactorsToSI[ccmc::strings::variables::uy_] = 1e3f;
		conversionFactorsToSI[ccmc::strings::variables::uz_] = 1e3f;
		conversionFactorsToSI[ccmc::strings::variables::jx_] = 1e-6f;
		conversionFactorsToSI[ccmc::strings::variables::jy_] = 1e-6f;
		conversionFactorsToSI[ccmc::strings::variables::jz_] = 1e-6f;
		conversionFactorsToSI[ccmc::strings::variables::b1x_] = 1e-9f;
		conversionFactorsToSI[ccmc::strings::variables::b1y_] = 1e-9f;
		conversionFactorsToSI[ccmc::strings::variables::b1z_] = 1e-9f;
		conversionFactorsToSI[ccmc::strings::variables::rho_] = 1e6;
		conversionFactorsToSI[ccmc::strings::variables::p_] = 1e-9f;
		conversionFactorsToSI[ccmc::strings::variables::e_] = 1e-15f;

	}


	/**
	 * @copydoc Model::createNewInterpolator()
	 */
	Interpolator * BATSRUS::createNewInterpolator()
	{
		Interpolator * interpolator = new BATSRUSInterpolator(this);
		return interpolator;
	}

	/**
	 * Destructor
	 */
	BATSRUS::~BATSRUS()
	{
		// TODO Auto-generated destructor stub
//		std::cout << "deleting BATSRUS object" << std::endl;
	}
}
