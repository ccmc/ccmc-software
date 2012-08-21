/*
 * BATSRUSInterpolator.cpp
 *
 *  Created on: Jun 30, 2009
 *      Author: dberrios
 */

#include "BATSRUSInterpolator.h"
#include "StringConstants.h"
#include <limits>
#include <cmath>
#include <iostream>


namespace ccmc
{
	/**
	 * @param modelReader Pointer to the Model object containing the appropriate variable maps.  BATSRUSInterpolator
	 * should be returned by a BATSRUS::createNewInterpolator() call.
	 */
	BATSRUSInterpolator::BATSRUSInterpolator(Model * modelReader)
	{
		// TODO Auto-generated constructor stub
		this->modelReader = modelReader;
		block_x_min_array = modelReader->getVariableFromMap(ccmc::strings::variables::block_x_min_);
		block_y_min_array = modelReader->getVariableFromMap(ccmc::strings::variables::block_y_min_);
		block_z_min_array = modelReader->getVariableFromMap(ccmc::strings::variables::block_z_min_);
		block_x_max_array = modelReader->getVariableFromMap(ccmc::strings::variables::block_x_max_);
		block_y_max_array = modelReader->getVariableFromMap(ccmc::strings::variables::block_y_max_);
		block_z_max_array = modelReader->getVariableFromMap(ccmc::strings::variables::block_z_max_);
		block_x_center_array = modelReader->getVariableFromMap(ccmc::strings::variables::block_x_center_);
		block_y_center_array = modelReader->getVariableFromMap(ccmc::strings::variables::block_y_center_);
		block_z_center_array = modelReader->getVariableFromMap(ccmc::strings::variables::block_z_center_);
		block_child_id_1_array = modelReader->getIntVariableFromMap(ccmc::strings::variables::block_child_id_1_);
		block_child_id_2_array = modelReader->getIntVariableFromMap(ccmc::strings::variables::block_child_id_2_);
		block_child_id_3_array = modelReader->getIntVariableFromMap(ccmc::strings::variables::block_child_id_3_);
		block_child_id_4_array = modelReader->getIntVariableFromMap(ccmc::strings::variables::block_child_id_4_);
		block_child_id_5_array = modelReader->getIntVariableFromMap(ccmc::strings::variables::block_child_id_5_);
		block_child_id_6_array = modelReader->getIntVariableFromMap(ccmc::strings::variables::block_child_id_6_);
		block_child_id_7_array = modelReader->getIntVariableFromMap(ccmc::strings::variables::block_child_id_7_);
		block_child_id_8_array = modelReader->getIntVariableFromMap(ccmc::strings::variables::block_child_id_8_);
		block_child_count_array = modelReader->getIntVariableFromMap(ccmc::strings::variables::block_child_count_);
		block_at_amr_level_array = modelReader->getIntVariableFromMap(ccmc::strings::variables::block_at_amr_level_);

		callCount = 0;
		old_x = std::numeric_limits<float>::min( );
		old_y = std::numeric_limits<float>::min( );
		old_z = std::numeric_limits<float>::min( );

		previousWasValid = false;

		/********* get values for NX, NY, NZ **********/
		/** They are stored as floats.  Need to fetch them, and convert to int **/
		nx = (int) (modelReader->getGlobalAttribute(strings::attributes::special_parameter_NX_)).getAttributeFloat();
		ny = (int) (modelReader->getGlobalAttribute(strings::attributes::special_parameter_NY_)).getAttributeFloat();
		nz = (int) (modelReader->getGlobalAttribute(strings::attributes::special_parameter_NZ_)).getAttributeFloat();

		//std::cout << "finished reading nx, ny, nz" << std::endl;
		/********* get value for number_of_blocks **********/
		int number_of_blocks = (modelReader->getGlobalAttribute(strings::attributes::number_of_blocks_)).getAttributeInt();

		global_x_max = (modelReader->getGlobalAttribute(strings::attributes::global_x_max_)).getAttributeFloat();
		global_y_max = (modelReader->getGlobalAttribute(strings::attributes::global_y_max_)).getAttributeFloat();
		global_z_max = (modelReader->getGlobalAttribute(strings::attributes::global_z_max_)).getAttributeFloat();
		global_x_min = (modelReader->getGlobalAttribute(strings::attributes::global_x_min_)).getAttributeFloat();
		global_y_min = (modelReader->getGlobalAttribute(strings::attributes::global_y_min_)).getAttributeFloat();
		global_z_min = (modelReader->getGlobalAttribute(strings::attributes::global_z_min_)).getAttributeFloat();
	}

	/**
	 * @param variable
	 * @param c0 X component of the position
	 * @param c1 Y component of the position
	 * @param c2 Z component of the position
	 * @return
	 */
	float BATSRUSInterpolator::interpolate(const std::string& variable, const float& c0, const float& c1,
			const float& c2)
	{

		float dc0, dc1, dc2;
		long variable_id = modelReader->getVariableID(variable);
		return interpolate(variable_id, c0, c1, c2, dc0, dc1, dc2);

	}

	/**
	 * @param variable_id
	 * @param c0 X component of the position
	 * @param c1 Y component of the position
	 * @param c2 Z component of the position
	 * @return
	 */
	float BATSRUSInterpolator::interpolate(const long& variable_id, const float& c0, const float& c1, const float& c2)
	{

		float dc0, dc1, dc2;
		return interpolate(variable_id, c0, c1, c2, dc0, dc1, dc2);

	}

	/**
	 * Interpolation method. Note that using the variable ID is significantly faster than using the variable string.
	 * @param variable_id A long representing the variable ID.
	 * @param c0 X component of the position
	 * @param c1 Y component of the position
	 * @param c2 Z component of the position
	 * @param dc0 Reference to a variable to store the delta for component 0
	 * @param dc1 Reference to a variable to store the delta for component 1
	 * @param dc2 Reference to a variable to store the delta for component 2
	 * @return The interpolated value at position (c0,c1,c2) with deltas (dc0,dc1,dc2)
	 */
	float BATSRUSInterpolator::interpolate(const long& variable_id, const float& c0, const float& c1, const float& c2,
			float& dc0, float& dc1, float& dc2)
	{
		float missingValue = this->modelReader->getMissingValue();
		bool main_memory_flag = true;
		if (this->modelReader->getVariableFromMap(variable_id) == NULL)
			main_memory_flag = false;
		long status;

		/********* interpolate_amr_data variables for new interpolation routine/upgrade *******/
		int ic, new_blk[8], valid;
		float ixx, iyy, izz, dx1, dy1, dz1, dx2, dy2, dz2, data_c[8], d_m1, d_m2, yy_c2[4], zz_c2[4];

		float XMIN, XMAX, YMIN, YMAX, ZMIN, ZMAX;


		/**********************************************************************************************/

		//	counts[0] = 0; /*reset values after once through */
		int intervals[] = { 1 };

		/* for field line tracing,etc..., select appropriate variable number ie. bx_cdfNum|by_cdfNum|bz_cdfNum based on *variable_string */

		/************** NEW INTERPOLATION ROUTINE & MODIFICATION ***************/
		bool new_position = true;
		if (old_x == c0 && old_y == c1 && old_z == c2 && previousWasValid)
		{
			new_position = false;
		} else
		{
		}
		if (new_position)
		{
			ib = find_octree_block(c0, c1, c2, -1);

			if (ib == -1)
			{
				previousWasValid = false;
				return missingValue;
			}

			XMIN = ((*block_x_min_array)[ib]);
			XMAX = ((*block_x_max_array)[ib]);
			YMIN = ((*block_y_min_array)[ib]);
			YMAX = ((*block_y_max_array)[ib]);
			ZMIN = ((*block_z_min_array)[ib]);
			ZMAX = ((*block_z_max_array)[ib]);

			/*** set delta's and sample xyz positions ***/

			dx1 = (XMAX - XMIN) / (float) nx;
			dy1 = (YMAX - YMIN) / (float) ny;
			dz1 = (ZMAX - ZMIN) / (float) nz;

			ixx = (c0 - XMIN) / dx1 - 0.5;
			iyy = (c1 - YMIN) / dy1 - 0.5;
			izz = (c2 - ZMIN) / dz1 - 0.5;

			/* return delta values to calling program */
			dc0 = dx1;
			dc1 = dy1;
			dc2 = dz1;

			/*** indices of grid positions around sample locations ***/

			ix_c[0] = ix_c[2] = ix_c[4] = ix_c[6] = (int)std::floor(ixx);
			ix_c[1] = ix_c[3] = ix_c[5] = ix_c[7] = (int)std::floor(ixx + 1);
			iy_c[0] = iy_c[1] = iy_c[4] = iy_c[5] = (int)std::floor(iyy);
			iy_c[2] = iy_c[3] = iy_c[6] = iy_c[7] = (int)std::floor(iyy + 1);
			iz_c[0] = iz_c[1] = iz_c[2] = iz_c[3] = (int)std::floor(izz);
			iz_c[4] = iz_c[5] = iz_c[6] = iz_c[7] = (int)std::floor(izz + 1);

			for (ic = 0; ic < 8; ic++)
			{

				ib_c[ic] = ib;

				xx_c[ic] = XMIN + (0.5 + ix_c[ic]) * dx1;
				yy_c[ic] = YMIN + (0.5 + iy_c[ic]) * dy1;
				zz_c[ic] = ZMIN + (0.5 + iz_c[ic]) * dz1;

				new_blk[ic] = 0;

				/* validate positions in block and find new block as necessary */

				if (ix_c[ic] < 0)
				{
					new_blk[ic] = 1;
					xx_c[ic] = XMIN - 0.5 * dx1;
				} else
				{
					if (ix_c[ic] > (nx - 0.5))
					{
						new_blk[ic] = 1;
						xx_c[ic] = XMAX + 0.5 * dx1;
					} else
					{
						xx_c[ic] = XMIN + (0.5 + ix_c[ic]) * dx1;
					}

				}

				if (iy_c[ic] < 0)
				{
					new_blk[ic] = 1;
					yy_c[ic] = YMIN - 0.5 * dy1;
				} else
				{
					if (iy_c[ic] > (ny - 1))
					{
						new_blk[ic] = 1;
						yy_c[ic] = YMAX + 0.5 * dy1;
					} else
					{
						yy_c[ic] = YMIN + (0.5 + iy_c[ic]) * dy1;
					}
				}

				if (iz_c[ic] < 0)
				{
					new_blk[ic] = 1;
					zz_c[ic] = ZMIN - 0.5 * dz1;
				} else
				{
					if (iz_c[ic] > (nz - 1))
					{
						new_blk[ic] = 1;
						zz_c[ic] = ZMAX + 0.5 * dz1;
					} else
					{
						zz_c[ic] = ZMIN + (0.5 + iz_c[ic]) * dz1;
					}
				}

			} /*** end for ( ic = 0; ic < 8; ic++ ) loop ***/

			for (ic = 0; ic < 8; ic++)
			{
				valid_c[ic] = 1;
				if (new_blk[ic])
				{
					long ibc;
					ibc = find_octree_block(xx_c[ic], yy_c[ic], zz_c[ic], -1L);

					/* ; now snap into position at new block cell */

					if (ibc != -1)
					{
						ib_c[ic] = ibc; /* change block number */

						/*** set the BLOCK MIN/MAX values depending on main_memory flag ***/

						XMIN = (*block_x_min_array)[ibc];
						XMAX = (*block_x_max_array)[ibc];
						YMIN = (*block_y_min_array)[ibc];
						YMAX = (*block_y_max_array)[ibc];
						ZMIN = (*block_z_min_array)[ibc];
						ZMAX = (*block_z_max_array)[ibc];
						dx2 = (XMAX - XMIN) / (float)nx;
						dy2 = (YMAX - YMIN) / (float)ny;
						dz2 = (ZMAX - ZMIN) / (float)nz;

						xx_c[ic] = c0 + (ic % 2) * dx2; /* adjust stencil with */
						yy_c[ic] = c1 + ((ic % 4) / 2) * dy2; /* resolution change between */
						zz_c[ic] = c2 + (ic / 4) * dz2; /* neighboring blocks */

						ix_c[ic] = std::min(nx - 1., std::max(0., std::floor((xx_c[ic] - XMIN) / dx2 - 0.5)));
						iy_c[ic] = std::min(ny - 1., std::max(0., std::floor((yy_c[ic] - YMIN) / dy2 - 0.5)));
						iz_c[ic] = std::min(nz - 1., std::max(0., std::floor((zz_c[ic] - ZMIN) / dz2 - 0.5)));
						xx_c[ic] = XMIN + dx2 * (0.5 + ix_c[ic]);
						yy_c[ic] = YMIN + dx2 * (0.5 + iy_c[ic]);
						zz_c[ic] = ZMIN + dx2 * (0.5 + iz_c[ic]);

					} else
					{
						valid_c[ic] = 0;
						ib_c[ic] = ib;

						/* point to some grid point near the position (xx,yy,zz) */
						ix_c[ic] = nx / 2;
						iy_c[ic] = ny / 2;
						iz_c[ic] = nz / 2;

						/* throw out those points by moving them off */
						xx_c[ic] = std::fabs(missingValue); /*x_blk[ib*NX+ix_c[ic]]; */
						yy_c[ic] = std::fabs(missingValue); /*y_blk[ib*NY+iy_c[ic]]; */
						zz_c[ic] = std::fabs(missingValue); /*z_blk[ib*NZ+iz_c[ic]]; */
					}
				}
			}

		} /** end of if new position loop **/
		else
		{
			if (ib == -1L) /*** outside of range ***/
			{
				dc0 = 0.;
				dc1 = 0.;
				dc2 = 0.;
				previousWasValid = false;
				return missingValue;
			}

			/*** if it is not a new position, still calculate the delta as necessary ***/

			/*** set the BLOCK MIN/MAX values depending on main_memory flag ***/
			XMIN = (*block_x_min_array)[ib];
			XMAX = (*block_x_max_array)[ib];
			YMIN = (*block_y_min_array)[ib];
			YMAX = (*block_y_max_array)[ib];
			ZMIN = (*block_z_min_array)[ib];
			ZMAX = (*block_z_max_array)[ib];

			/*** set delta's and sample xyz positions ***/

			dx1 = (XMAX - XMIN) / (float) nx;
			dy1 = (YMAX - YMIN) / (float) ny;
			dz1 = (ZMAX - ZMIN) / (float) nz;

			/* set delta values from calling program */
			dc0 = dx1;
			dc1 = dy1;
			dc2 = dz1;

		}

		/*********************************************************************

		 *                            retreive the data                        **

		 ********************************************************************/


		const std::vector<float>* vData = NULL;
		if (main_memory_flag == true)
			vData = modelReader->getVariableFromMap(variable_id);
		for (ic = 0; ic < 8; ic++)
		{

			float temp_space;

			/********* NOTE to SELF:  Put variable name in an array or something next time, but since this works.... ***********/

			int index = ix_c[ic] + nx * (iy_c[ic] + ny * (iz_c[ic] + nz * ib_c[ic]));

			if (vData != NULL)
				data_c[ic] = (*vData)[index];
			else
			{
				data_c[ic] = modelReader->getVariableAtIndex(variable_id, index);
			}

		} /* end of for( ic = 0; ic < 8; ic++) loop */

		/************** per Lutz do pair-wise interpolations in x,y, & z directions *********/

		for (ic = 0; ic < 4; ic++)
		{
			int ic2, ic2_1;
			ic2 = 2 * ic;
			ic2_1 = ic2 + 1;
			d_m1 = (c0 - xx_c[ic2]);
			d_m2 = (xx_c[ic2_1] - c0);

			if (valid_c[ic2] && valid_c[ic2_1] && (std::fabs(d_m1 + d_m2) > (dx1 / 4.)))
			{

				data_c[ic] = (d_m2 * data_c[ic2] + d_m1 * data_c[ic2_1]) / (d_m1 + d_m2);

				yy_c2[ic] = (d_m2 * yy_c[ic2] + d_m1 * yy_c[ic2_1]) / (d_m1 + d_m2);

				zz_c2[ic] = (d_m2 * zz_c[ic2] + d_m1 * zz_c[ic2_1]) / (d_m1 + d_m2);
				valid = 1;
			} else
			{
				valid = 0;
				if (valid_c[ic2_1])
				{
					data_c[ic] = data_c[ic2_1];

					yy_c2[ic] = yy_c[ic2_1];

					zz_c2[ic] = zz_c[ic2_1];
					valid = 1;
				}
				if (valid_c[ic2])
				{
					data_c[ic] = data_c[ic2];

					yy_c2[ic] = yy_c[ic2];

					zz_c2[ic] = zz_c[ic2];
					valid = 1;
				}
			}
			valid_c[ic] = valid;
		} /* end of for( ic = 0; ic < 4; ic++ ); loop*/

		for (ic = 0; ic < 2; ic++)
		{
			int ic2, ic2_1;
			ic2 = 2 * ic;
			ic2_1 = ic2 + 1;
			d_m1 = (c1 - yy_c2[ic2]);
			d_m2 = (yy_c2[ic2_1] - c1);

			if (valid_c[ic2] && valid_c[ic2_1] && (std::fabs(d_m1 + d_m2) >= (dy1 / 4.)))
			{
				zz_c2[ic] = (d_m2 * zz_c2[ic2] + d_m1 * zz_c2[ic2_1]) / (d_m1 + d_m2);
				data_c[ic] = (d_m2 * data_c[ic2] + d_m1 * data_c[ic2_1]) / (d_m1 + d_m2);
				valid = 1;
			} else
			{
				valid = 0;
				if (valid_c[ic2_1])
				{
					data_c[ic] = data_c[ic2_1];
					zz_c2[ic] = zz_c2[ic2_1];
					valid = 1;
				}
				if (valid_c[ic2])
				{
					data_c[ic] = data_c[ic2];
					zz_c2[ic] = zz_c2[ic2];
					valid = 1;
				}
			}
			valid_c[ic] = valid;
		}

		d_m1 = (c2 - zz_c2[0]);
		d_m2 = (zz_c2[1] - c2);

		/************************************************************************************

		 ***************** return result of interpolation into data_c[0] ********************

		 ***********************************************************************************/

		if (valid_c[0] && valid_c[1] && (std::fabs(d_m1 + d_m2) >= (dz1 / 4.)))
		{
			data_c[0] = ((d_m2 * data_c[0] + d_m1 * data_c[1]) / (d_m1 + d_m2));
			valid = 1;
		} else
		{
			valid = 0;
			if (valid_c[1])
			{
				data_c[0] = data_c[1];
				valid = 1;
			}
			if (valid_c[0])
			{
				valid = 1;
			}
		}
		if (callCount == 0)
			callCount++;
		old_x = c0;
		old_y = c1;
		old_z = c2;
		if (valid)
		{
			previousWasValid = true;
			return (data_c[0]);
		} else
		{
			previousWasValid = false;
			return missingValue;
		}
	}

	/**
	 * Interpolation method. Note that using the variable ID is significantly faster than using the variable string.
	 * @param variable The input variable.
	 * @param c0 X component of the position
	 * @param c1 Y component of the position
	 * @param c2 Z component of the position
	 * @param dc0 Reference to a variable to store the delta for component 0
	 * @param dc1 Reference to a variable to store the delta for component 1
	 * @param dc2 Reference to a variable to store the delta for component 2
	 * @return The interpolated value at position (c0,c1,c2) with deltas (dc0,dc1,dc2)
	 */
	float BATSRUSInterpolator::interpolate(const std::string& variable, const float& c0, const float& c1,
			const float& c2, float& dc0, float& dc1, float& dc2)
	{
		float missingValue = this->modelReader->getMissingValue();
		//std::cout << "BATSRUSInterpolator::interpolate. variable: " << variable << std::endl;
		bool main_memory_flag = true;
		if (this->modelReader->getVariableFromMap(variable) == NULL)
			main_memory_flag = false;


		long status;

		/********* interpolate_amr_data variables for new interpolation routine/upgrade *******/

		int ic, new_blk[8], valid;
		float ixx, iyy, izz, dx1, dy1, dz1, dx2, dy2, dz2, data_c[8], d_m1, d_m2, yy_c2[4], zz_c2[4];

		float XMIN, XMAX, YMIN, YMAX, ZMIN, ZMAX;

		int intervals[] = { 1 };

		/* for field line tracing,etc..., select appropriate variable number ie. bx_cdfNum|by_cdfNum|bz_cdfNum based on *variable_string */

		/************** NEW INTERPOLATION ROUTINE & MODIFICATION ***************/
		bool new_position = true;
		if (old_x == c0 && old_y == c1 && old_z == c2 && previousWasValid)
		{
			new_position = false;
		} else
		{
		}
		if (new_position)
		{
			ib = find_octree_block(c0, c1, c2, -1);

			if (ib == -1)
			{
				previousWasValid = false;
				return missingValue;
			}

			/*** set the BLOCK MIN/MAX values depending on main_memory flag ***/

			XMIN = ((*block_x_min_array)[ib]);
			XMAX = ((*block_x_max_array)[ib]);
			YMIN = ((*block_y_min_array)[ib]);
			YMAX = ((*block_y_max_array)[ib]);
			ZMIN = ((*block_z_min_array)[ib]);
			ZMAX = ((*block_z_max_array)[ib]);

			/*** set delta's and sample xyz positions ***/

			dx1 = (XMAX - XMIN) / (float) nx;
			dy1 = (YMAX - YMIN) / (float) ny;
			dz1 = (ZMAX - ZMIN) / (float) nz;

			ixx = (c0 - XMIN) / dx1 - 0.5;
			iyy = (c1 - YMIN) / dy1 - 0.5;
			izz = (c2 - ZMIN) / dz1 - 0.5;

			/* return delta values to calling program */
			dc0 = dx1;
			dc1 = dy1;
			dc2 = dz1;

			/*** indices of grid positions around sample locations ***/

			ix_c[0] = ix_c[2] = ix_c[4] = ix_c[6] = std::floor(ixx);
			ix_c[1] = ix_c[3] = ix_c[5] = ix_c[7] = std::floor(ixx + 1);
			iy_c[0] = iy_c[1] = iy_c[4] = iy_c[5] = std::floor(iyy);
			iy_c[2] = iy_c[3] = iy_c[6] = iy_c[7] = std::floor(iyy + 1);
			iz_c[0] = iz_c[1] = iz_c[2] = iz_c[3] = std::floor(izz);
			iz_c[4] = iz_c[5] = iz_c[6] = iz_c[7] = std::floor(izz + 1);

			for (ic = 0; ic < 8; ic++)
			{

				ib_c[ic] = ib;

				xx_c[ic] = XMIN + (0.5 + ix_c[ic]) * dx1;
				yy_c[ic] = YMIN + (0.5 + iy_c[ic]) * dy1;
				zz_c[ic] = ZMIN + (0.5 + iz_c[ic]) * dz1;

				new_blk[ic] = 0;

				/* validate positions in block and find new block as necessary */

				if (ix_c[ic] < 0.0)
				{
					new_blk[ic] = 1;
					xx_c[ic] = XMIN - 0.5 * dx1;
				} else
				{
					if (ix_c[ic] > (nx - 0.5))
					{
						new_blk[ic] = 1;
						xx_c[ic] = XMAX + 0.5 * dx1;
					} else
					{
						xx_c[ic] = XMIN + (0.5 + ix_c[ic]) * dx1;
					}

				}

				if (iy_c[ic] < 0)
				{
					new_blk[ic] = 1;
					yy_c[ic] = YMIN - 0.5 * dy1;
				} else
				{
					if (iy_c[ic] > (ny - 1))
					{
						new_blk[ic] = 1;
						yy_c[ic] = YMAX + 0.5 * dy1;
					} else
					{
						yy_c[ic] = YMIN + (0.5 + iy_c[ic]) * dy1;
					}
				}

				if (iz_c[ic] < 0)
				{
					new_blk[ic] = 1;
					zz_c[ic] = ZMIN - 0.5 * dz1;
				} else
				{
					if (iz_c[ic] > (nz - 1))
					{
						new_blk[ic] = 1;
						zz_c[ic] = ZMAX + 0.5 * dz1;
					} else
					{
						zz_c[ic] = ZMIN + (0.5 + iz_c[ic]) * dz1;
					}
				}

			} /*** end for ( ic = 0; ic < 8; ic++ ) loop ***/

			for (ic = 0; ic < 8; ic++)
			{
				valid_c[ic] = 1;
				if (new_blk[ic])
				{
					long ibc;
					ibc = find_octree_block(xx_c[ic], yy_c[ic], zz_c[ic], -1L);

					/* ; now snap into position at new block cell */

					if (ibc != -1)
					{
						ib_c[ic] = ibc; /* change block number */

						/*** set the BLOCK MIN/MAX values depending on main_memory flag ***/

						XMIN = (*block_x_min_array)[ibc];
						XMAX = (*block_x_max_array)[ibc];
						YMIN = (*block_y_min_array)[ibc];
						YMAX = (*block_y_max_array)[ibc];
						ZMIN = (*block_z_min_array)[ibc];
						ZMAX = (*block_z_max_array)[ibc];
						dx2 = (XMAX - XMIN) / (float)nx;
						dy2 = (YMAX - YMIN) / (float)ny;
						dz2 = (ZMAX - ZMIN) / (float)nz;

						xx_c[ic] = c0 + (ic % 2) * dx2; /* adjust stencil with */
						yy_c[ic] = c1 + ((ic % 4) / 2) * dy2; /* resolution change between */
						zz_c[ic] = c2 + (ic / 4) * dz2; /* neighboring blocks */

						ix_c[ic] = std::min(nx - 1., std::max(0., std::floor((xx_c[ic] - XMIN) / dx2 - 0.5)));
						iy_c[ic] = std::min(ny - 1., std::max(0., std::floor((yy_c[ic] - YMIN) / dy2 - 0.5)));
						iz_c[ic] = std::min(nz - 1., std::max(0., std::floor((zz_c[ic] - ZMIN) / dz2 - 0.5)));
						xx_c[ic] = XMIN + dx2 * (0.5 + ix_c[ic]);
						yy_c[ic] = YMIN + dx2 * (0.5 + iy_c[ic]);
						zz_c[ic] = ZMIN + dx2 * (0.5 + iz_c[ic]);

					} else
					{
						valid_c[ic] = 0;
						ib_c[ic] = ib;

						/* point to some grid point near the position (xx,yy,zz) */
						ix_c[ic] = nx / 2;
						iy_c[ic] = ny / 2;
						iz_c[ic] = nz / 2;

						/* throw out those points by moving them off */
						xx_c[ic] = std::fabs(missingValue); /*x_blk[ib*NX+ix_c[ic]]; */
						yy_c[ic] = std::fabs(missingValue); /*y_blk[ib*NY+iy_c[ic]]; */
						zz_c[ic] = std::fabs(missingValue); /*z_blk[ib*NZ+iz_c[ic]]; */
					}
				}
			}

		} /** end of if new position loop **/
		else
		{
			if (ib == -1L) /*** outside of range ***/
			{
				dc0 = 0.;
				dc1 = 0.;
				dc2 = 0.;
				previousWasValid = false;
				return missingValue;
			}

			/*** if it is not a new position, still calculate the delta as necessary ***/

			/*** set the BLOCK MIN/MAX values depending on main_memory flag ***/
			XMIN = (*block_x_min_array)[ib];
			XMAX = (*block_x_max_array)[ib];
			YMIN = (*block_y_min_array)[ib];
			YMAX = (*block_y_max_array)[ib];
			ZMIN = (*block_z_min_array)[ib];
			ZMAX = (*block_z_max_array)[ib];

			/*** set delta's and sample xyz positions ***/

			dx1 = (XMAX - XMIN) / (float) nx;
			dy1 = (YMAX - YMIN) / (float) ny;
			dz1 = (ZMAX - ZMIN) / (float) nz;

			/* set delta values from calling program */
			dc0 = dx1;
			dc1 = dy1;
			dc2 = dz1;

		}

		/*********************************************************************

		 *                            retreive the data                        **

		 ********************************************************************/

		const std::vector<float>* vData = NULL;
		if (main_memory_flag == true)
			vData = modelReader->getVariableFromMap(variable);
		for (ic = 0; ic < 8; ic++)
		{

			float temp_space;

			/********* NOTE to SELF:  Put variable name in an array or something next time, but since this works.... ***********/

			int index = ix_c[ic] + nx * (iy_c[ic] + ny * (iz_c[ic] + nz * ib_c[ic]));

			if (vData != NULL)
				data_c[ic] = (*vData)[index];
			else
			{
				data_c[ic] = modelReader->getVariableAtIndex(variable, index);
			}

		} /* end of for( ic = 0; ic < 8; ic++) loop */

		/************** per Lutz do pair-wise interpolations in x,y, & z directions *********/

		for (ic = 0; ic < 4; ic++)
		{
			int ic2, ic2_1;
			ic2 = 2 * ic;
			ic2_1 = ic2 + 1;
			d_m1 = (c0 - xx_c[ic2]);
			d_m2 = (xx_c[ic2_1] - c0);

			if (valid_c[ic2] && valid_c[ic2_1] && (std::fabs(d_m1 + d_m2) > (dx1 / 4.)))
			{

				data_c[ic] = (d_m2 * data_c[ic2] + d_m1 * data_c[ic2_1]) / (d_m1 + d_m2);

				yy_c2[ic] = (d_m2 * yy_c[ic2] + d_m1 * yy_c[ic2_1]) / (d_m1 + d_m2);

				zz_c2[ic] = (d_m2 * zz_c[ic2] + d_m1 * zz_c[ic2_1]) / (d_m1 + d_m2);
				valid = 1;
			} else
			{
				valid = 0;
				if (valid_c[ic2_1])
				{
					data_c[ic] = data_c[ic2_1];

					yy_c2[ic] = yy_c[ic2_1];

					zz_c2[ic] = zz_c[ic2_1];
					valid = 1;
				}
				if (valid_c[ic2])
				{
					data_c[ic] = data_c[ic2];

					yy_c2[ic] = yy_c[ic2];

					zz_c2[ic] = zz_c[ic2];
					valid = 1;
				}
			}
			valid_c[ic] = valid;
		} /* end of for( ic = 0; ic < 4; ic++ ); loop*/

		for (ic = 0; ic < 2; ic++)
		{
			int ic2, ic2_1;
			ic2 = 2 * ic;
			ic2_1 = ic2 + 1;
			d_m1 = (c1 - yy_c2[ic2]);
			d_m2 = (yy_c2[ic2_1] - c1);

			if (valid_c[ic2] && valid_c[ic2_1] && (std::fabs(d_m1 + d_m2) >= (dy1 / 4.)))
			{
				zz_c2[ic] = (d_m2 * zz_c2[ic2] + d_m1 * zz_c2[ic2_1]) / (d_m1 + d_m2);
				data_c[ic] = (d_m2 * data_c[ic2] + d_m1 * data_c[ic2_1]) / (d_m1 + d_m2);
				valid = 1;
			} else
			{
				valid = 0;
				if (valid_c[ic2_1])
				{
					data_c[ic] = data_c[ic2_1];
					zz_c2[ic] = zz_c2[ic2_1];
					valid = 1;
				}
				if (valid_c[ic2])
				{
					data_c[ic] = data_c[ic2];
					zz_c2[ic] = zz_c2[ic2];
					valid = 1;
				}
			}
			valid_c[ic] = valid;
		}

		d_m1 = (c2 - zz_c2[0]);
		d_m2 = (zz_c2[1] - c2);

		/************************************************************************************

		 ***************** return result of interpolation into data_c[0] ********************

		 ***********************************************************************************/

		if (valid_c[0] && valid_c[1] && (std::fabs(d_m1 + d_m2) >= (dz1 / 4.)))
		{
			data_c[0] = ((d_m2 * data_c[0] + d_m1 * data_c[1]) / (d_m1 + d_m2));
			valid = 1;
		} else
		{
			valid = 0;
			if (valid_c[1])
			{
				data_c[0] = data_c[1];
				valid = 1;
			}
			if (valid_c[0])
			{
				valid = 1;
			}
		}
		if (callCount == 0)
			callCount++;
		old_x = c0;
		old_y = c1;
		old_z = c2;
		if (valid)
		{
			previousWasValid = true;
			return (data_c[0]);
		} else
		{
			previousWasValid = false;
			return missingValue;
		}
	}

	/**
	 * @param x
	 * @param y
	 * @param z
	 * @param old_block_number
	 * @return Block index
	 */
	int BATSRUSInterpolator::find_octree_block(float x, float y, float z, int old_block_number)
	{


		//first, check to see if the position is valid!!
		if (
				x < global_x_min || x > global_x_max ||
				y < global_y_min || y > global_y_max ||
				z < global_z_min || z > global_z_max)
			return -1;

		int block_index_1, block_index_2, root_index;

		block_index_1 = old_block_number;

		block_index_2 = -1;
		root_index = 0;

		/****** when main memory flag is set, retreive all required data values from main memory

		 data loaded with open_cdf routine ********/

		bool main_memory_flag = true;
		if (main_memory_flag)
		{
			/*** we may be in the same block used previously, if so check first to increase performance ***/

			if (old_block_number != -1)
			{
				if (((*block_x_min_array)[block_index_1] <= x) && ((*block_x_max_array)[block_index_1] >= x)
						&& ((*block_y_min_array)[block_index_1] <= y) && ((*block_y_max_array)[block_index_1] >= y)
						&& ((*block_z_min_array)[block_index_1] <= z) && ((*block_z_max_array)[block_index_1] >= z))
				{

					return (block_index_1);
				}
			}

			while ((root_index < (*block_at_amr_level_array)[0]) && (block_index_2 == -1))
			{
				block_index_1 = (*block_at_amr_level_array)[root_index];

				if (block_index_1 < (*block_x_min_array).size() && ((*block_x_min_array)[block_index_1] <= x)
						&& ((*block_x_max_array)[block_index_1] >= x) && ((*block_y_min_array)[block_index_1] <= y)
						&& ((*block_y_max_array)[block_index_1] >= y) && ((*block_z_min_array)[block_index_1] <= z)
						&& ((*block_z_max_array)[block_index_1] >= z))
				{

					block_index_2 = climb_octree(block_index_1, x, y, z);
				} else
				{
					root_index++;
				}
			}
		}

		return (block_index_2);

	}

	/**
	 * @param root
	 * @param x
	 * @param y
	 * @param z
	 * @return
	 */
	int BATSRUSInterpolator::climb_octree(int root, float x, float y, float z)
	{
		long recordStart = 0;
		long indices[1];

		int ix, iy, iz;

		int child_id, child_key;

		indices[0] = root;


		/******** if main memory flag is NOT set, read all required data directly from cdf file **********/

		if ((*block_child_count_array)[root] == 0)
		{
			return (root);
		}

		ix = x > (*block_x_center_array)[root];
		iy = y > (*block_y_center_array)[root];
		iz = z > (*block_z_center_array)[root];

		/********** calculate & retrieve child ID **************/

		child_key = iz * 4 + 2 * iy + ix;

		indices[0] = root;

		switch (child_key)
		{
			case 0:
				child_id = (*block_child_id_1_array)[root];
				break;
			case 1:
				child_id = (*block_child_id_2_array)[root];
				break;
			case 2:
				child_id = (*block_child_id_3_array)[root];
				break;
			case 3:
				child_id = (*block_child_id_4_array)[root];
				break;
			case 4:
				child_id = (*block_child_id_5_array)[root];
				break;
			case 5:
				child_id = (*block_child_id_6_array)[root];
				break;
			case 6:
				child_id = (*block_child_id_7_array)[root];
				break;
			case 7:
				child_id = (*block_child_id_8_array)[root];
				break;

			default:
				/* do something constructive */
				std::cout << "default case!!" << std::endl;
				return 0;

		}

		return climb_octree(child_id, x, y, z);
	}

	/**
	 * Destructor
	 */
	BATSRUSInterpolator::~BATSRUSInterpolator()
	{
		// TODO Auto-generated destructor stub
	}
}
