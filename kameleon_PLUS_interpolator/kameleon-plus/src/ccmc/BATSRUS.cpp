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


namespace ccmc
{
	/**
	 * Default constructor
	 */
	BATSRUS::BATSRUS()
	{
		// TODO Auto-generated constructor stub

		//variableNames = {"bx", "by", "bz", "b1x", "b1y", "b1z", "ux", "uy", "uz",
		//					 "jx","jy","jz","rho","p","e"};


	}

	/**
	 * @copydoc Model::open()
	 */
	long BATSRUS::open(const std::string& filename)
	{
		long status;
		status = openFile(filename);




		/*printf("DEBUG\t main_memory_flag = %d\n", main_memory_flag);*/

		/*
		 if ((x_pos = (float *) calloc(nx * number_of_blocks, sizeof(float))) == NULL)
		 {
		 printf("\ncalloc failed for x_pos buffer!\n");
		 exit(EXIT_FAILURE);
		 }
		 if ((y_pos = (float *) calloc(ny * number_of_blocks, sizeof(float))) == NULL)
		 {
		 printf("\ncalloc failed for y_pos buffer!\n");
		 exit(EXIT_FAILURE);
		 }
		 if ((z_pos = (float *) calloc(nz * number_of_blocks, sizeof(float))) == NULL)
		 {
		 printf("\ncalloc failed for z_pos buffer!\n");
		 exit(EXIT_FAILURE);
		 }
		 */
		/********** get unique x,y,z values for every block *********************/
		/*
		 for (i = 0; i < number_of_blocks; i++)
		 {
		 for (ix = 0; ix < nx; ix++)
		 {
		 indices[0] = (i * nx * ny * nz) + ix;
		 status = CDFlib(SELECT_, zVAR_, x_cdf_varNum, SELECT_, zVAR_RECNUMBER_,
		 recordStart, SELECT_, zVAR_DIMINDICES_, indices, GET_, zVAR_DATA_,
		 &current_position, NULL_);
		 if (status != CDF_OK)
		 StatusHandler(status);

		 x_pos[ix + nx * i] = current_position;
		 }
		 for (iy = 0; iy < ny; iy++)
		 {
		 indices[0] = (i * nx * ny * nz) + iy * nx;
		 status = CDFlib(SELECT_, zVAR_, y_cdf_varNum, SELECT_, zVAR_RECNUMBER_,
		 recordStart, SELECT_, zVAR_DIMINDICES_, indices, GET_, zVAR_DATA_,
		 &current_position, NULL_);
		 if (status != CDF_OK)
		 StatusHandler(status);

		 y_pos[iy + ny * i] = current_position;
		 }
		 for (iz = 0; iz < nz; iz++)
		 {
		 indices[0] = (i * nx * ny * nz) + iz * nx * ny;
		 status = CDFlib(SELECT_, zVAR_, z_cdf_varNum, SELECT_, zVAR_RECNUMBER_,
		 recordStart, SELECT_, zVAR_DIMINDICES_, indices, GET_, zVAR_DATA_,
		 &current_position, NULL_);
		 if (status != CDF_OK)
		 StatusHandler(status);

		 z_pos[iz + nz * i] = current_position;
		 }
		 }

		 }
		 */
		//int number_of_blocks_plus_virtual = (8 * number_of_blocks) / 7; /*  add ceil or floor later for consistency */

		/********** get block_*_min/max values ************/
		bool success = loadVariable(ccmc::strings::variables::block_x_min_);
//		cout << "block_x_min: " << success << endl;

		success = loadVariable(ccmc::strings::variables::block_x_max_);
//		cout << "block_x_max: " << success << endl;

		success = loadVariable(ccmc::strings::variables::block_y_min_);
//		cout << "block_y_min: " << success << endl;

		success = loadVariable(ccmc::strings::variables::block_y_max_);
//		cout << "block_x_max: " << success << endl;

		success = loadVariable(ccmc::strings::variables::block_z_min_);
//		cout << "block_z_min: " << success << endl;

		success = loadVariable(ccmc::strings::variables::block_z_max_);
//		cout << "block_z_max: " << success << endl;

		/********** get block_child_count_array, block_*_center_array, & block_child_id_*_array values ************/
		success = loadVariableInt(ccmc::strings::variables::block_child_count_);


		success = loadVariable(ccmc::strings::variables::block_x_center_);
//		cout << "block_x_center: " << success << endl;

		success = loadVariable(ccmc::strings::variables::block_y_center_);
//		cout << "block_y_center: " << success << endl;

		success = loadVariable(ccmc::strings::variables::block_z_center_);
//		cout << "block_z_center: " << success << endl;

		success = loadVariableInt(ccmc::strings::variables::block_child_id_1_);
//		cout << "block_child_id_1: " << success << endl;

		success = loadVariableInt(ccmc::strings::variables::block_child_id_2_);
//		cout << "block_child_id_2: " << success << endl;

		success = loadVariableInt(ccmc::strings::variables::block_child_id_3_);
//		cout << "block_child_id_3: " << success << endl;

		success = loadVariableInt(ccmc::strings::variables::block_child_id_4_);
//		cout << "block_child_id_4: " << success << endl;

		success = loadVariableInt(ccmc::strings::variables::block_child_id_5_);
//		cout << "block_child_id_5: " << success << endl;

		success = loadVariableInt(ccmc::strings::variables::block_child_id_6_);
//		cout << "block_child_id_6: " << success << endl;

		success = loadVariableInt(ccmc::strings::variables::block_child_id_7_);
//		cout << "block_child_id_7: " << success << endl;

		success = loadVariableInt(ccmc::strings::variables::block_child_id_8_);
//		cout << "block_child_id_8: " << success << endl;


		/******** get values for block_at_amr_level *******/
		success = loadVariableInt(ccmc::strings::variables::block_at_amr_level_);
//		cout << "block_at_amr_level: " << success << endl;
		//std::cout << "block_at_amr_level: " << variableDataInt["block_at_amr_level"][0] << std::endl;
		initializeVariableNames();
		initializeConversionFactorsToSI();
		initializeSIUnits();

//		initializeGlobalAttributes();
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

		/*conversionFactorsToSI["t"] = 1.e-12/Boltzmann;
		 conversionFactorsToSI["s"] = 1.e-4;
		 conversionFactorsToSI["pram"] = 1.627e-6;
		 conversionFactorsToSI["nv"] = 1.e6;
		 conversionFactorsToSI["nvx"] = 1.e6;
		 conversionFactorsToSI["nvy"] = 1.e6;
		 conversionFactorsToSI["nvz"] = 1.e6;
		 conversionFactorsToSI["beta"] = 1.e21;*/
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
