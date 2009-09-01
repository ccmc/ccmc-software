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


namespace ccmc
{
	/**
	 * Default constructor
	 */
	BATSRUS::BATSRUS()
	{
		// TODO Auto-generated constructor stub
		block_x_min = "block_x_min";
		block_y_min = "block_y_min";
		block_z_min = "block_z_min";
		block_x_max = "block_x_max";
		block_y_max = "block_y_max";
		block_z_max = "block_z_max";
		block_child_count = "block_child_count";
		block_x_center = "block_x_center";
		block_y_center = "block_y_center";
		block_z_center = "block_z_center";
		block_child_id_1 = "block_child_id_1";
		block_child_id_2 = "block_child_id_2";
		block_child_id_3 = "block_child_id_3";
		block_child_id_4 = "block_child_id_4";
		block_child_id_5 = "block_child_id_5";
		block_child_id_6 = "block_child_id_6";
		block_child_id_7 = "block_child_id_7";
		block_child_id_8 = "block_child_id_8";
		block_at_amr_level = "block_at_amr_level";
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

		//grab all variables and store them into the conversionFactors hashtable
		//we do this so the variableIDs remain consistent, even if some variables are missing


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
		loadVariable(block_x_min);
		loadVariable(block_x_max);
		loadVariable(block_y_min);
		loadVariable(block_y_max);
		loadVariable(block_z_min);
		loadVariable(block_z_max);

		/********** get block_child_count_array, block_*_center_array, & block_child_id_*_array values ************/
		loadVariableInt(block_child_count);
		loadVariable(block_x_center);
		loadVariable(block_y_center);
		loadVariable(block_z_center);
		loadVariableInt(block_child_id_1);
		loadVariableInt(block_child_id_2);
		loadVariableInt(block_child_id_3);
		loadVariableInt(block_child_id_4);
		loadVariableInt(block_child_id_5);
		loadVariableInt(block_child_id_6);
		loadVariableInt(block_child_id_7);
		loadVariableInt(block_child_id_8);

		/******** get values for block_at_amr_level *******/
		loadVariableInt(block_at_amr_level);
		//std::cout << "block_at_amr_level: " << variableDataInt["block_at_amr_level"][0] << std::endl;
		initializeVariableNames();
		initializeConversionFactorsToSI();
		initializeSIUnits();

		return status;
	}

	/**
	 * @brief Initializes the variable names that will be used in string comparisons.
	 *
	 * This significantly improves the performance of the library by avoiding
	 * unnecessary string creations and destructions.
	 */
	void BATSRUS::initializeVariableNames()
	{
		bx_ = "bx";
		by_ = "by";
		bz_ = "bz";
		ux_ = "ux";
		uy_ = "uy";
		uz_ = "uz";
		jx_ = "jx";
		jy_ = "jy";
		jz_ = "jz";
		b1x_ = "b1x";
		b1y_ = "b1y";
		b1z_ = "b1z";
		rho_ = "rho";
		p_ = "p";
		e_ = "e";
	}

	/**
	 *
	 * @copydoc Model::initializeSIUnits()
	 *
	 */
	void BATSRUS::initializeSIUnits()
	{
		variableSIUnits[bx_] = "T";
		variableSIUnits[by_] = "T";
		variableSIUnits[bz_] = "T";
		variableSIUnits[ux_] = "m/s";
		variableSIUnits[uy_] = "m/s";
		variableSIUnits[uz_] = "m/s";
		variableSIUnits[jx_] = "A/m^2";
		variableSIUnits[jy_] = "A/m^2";
		variableSIUnits[jz_] = "A/m^2";
		variableSIUnits[b1x_] = "T";
		variableSIUnits[b1y_] = "T";
		variableSIUnits[b1z_] = "T";
		variableSIUnits[rho_] = "m^-3";
		variableSIUnits[p_] = "Pa";
		variableSIUnits[e_] = "J/m^3";
	}


	/**
	 * @copydoc Model::initializeConversionFactorsToSI()
	 */
	void BATSRUS::initializeConversionFactorsToSI()
	{

		/**
		 * TODO: fix these conversion factors
		 */
		conversionFactorsToSI[bx_] = 1e-9f;
		conversionFactorsToSI[by_] = 1e-9f;
		conversionFactorsToSI[bz_] = 1e-9f;
		conversionFactorsToSI[ux_] = 1e3f;
		conversionFactorsToSI[uy_] = 1e3f;
		conversionFactorsToSI[uz_] = 1e3f;
		conversionFactorsToSI[jx_] = 1e-6f;
		conversionFactorsToSI[jy_] = 1e-6f;
		conversionFactorsToSI[jz_] = 1e-6f;
		conversionFactorsToSI[b1x_] = 1e-9f;
		conversionFactorsToSI[b1y_] = 1e-9f;
		conversionFactorsToSI[b1z_] = 1e-9f;
		conversionFactorsToSI[rho_] = 1e6;
		conversionFactorsToSI[p_] = 1e-9f;
		conversionFactorsToSI[e_] = 1e-15f;

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
		std::cout << "deleting BATSRUS object" << std::endl;
	}
}
