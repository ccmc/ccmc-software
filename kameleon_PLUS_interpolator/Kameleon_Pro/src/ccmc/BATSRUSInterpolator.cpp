/*
 * BATSRUSInterpolator.cpp
 *
 *  Created on: Jun 30, 2009
 *      Author: dberrios
 */

#include "BATSRUSInterpolator.h"

/**
 * @param modelReader Pointer to the Model object containing the appropriate variable maps.  BATSRUSInterpolator
 * should be returned by a BATSRUS::createNewInterpolator() call.
 */
BATSRUSInterpolator::BATSRUSInterpolator(Model * modelReader)
{
	// TODO Auto-generated constructor stub
	this->modelReader = modelReader;
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

	block_x_min_array = modelReader->getVariableData(block_x_min);
	block_y_min_array = modelReader->getVariableData(block_y_min);
	block_z_min_array = modelReader->getVariableData(block_z_min);
	block_x_max_array = modelReader->getVariableData(block_x_max);
	block_y_max_array = modelReader->getVariableData(block_y_max);
	block_z_max_array = modelReader->getVariableData(block_z_max);
	block_x_center_array = modelReader->getVariableData(block_x_center);
	block_y_center_array = modelReader->getVariableData(block_y_center);
	block_z_center_array = modelReader->getVariableData(block_z_center);
	block_child_id_1_array = modelReader->getVariableDataInt(block_child_id_1);
	block_child_id_2_array = modelReader->getVariableDataInt(block_child_id_2);
	block_child_id_3_array = modelReader->getVariableDataInt(block_child_id_3);
	block_child_id_4_array = modelReader->getVariableDataInt(block_child_id_4);
	block_child_id_5_array = modelReader->getVariableDataInt(block_child_id_5);
	block_child_id_6_array = modelReader->getVariableDataInt(block_child_id_6);
	block_child_id_7_array = modelReader->getVariableDataInt(block_child_id_7);
	block_child_id_8_array = modelReader->getVariableDataInt(block_child_id_8);
	block_child_count_array = modelReader->getVariableDataInt(block_child_count);
	block_at_amr_level_array = modelReader->getVariableDataInt(block_at_amr_level);

	callCount = 0;
	old_x = -1000000000.f;
	old_y = -1000000000.f;
	old_z = -1000000000.f;

	previousWasValid = false;

	/********* get values for NX, NY, NZ **********/
	/** They are stored as floats.  Need to fetch them, and convert to int **/
	nx = (int)(modelReader->getGlobalAttribute("special_parameter_NX")).getAttributeFloat();
	ny = (int)(modelReader->getGlobalAttribute("special_parameter_NY")).getAttributeFloat();
	nz = (int)(modelReader->getGlobalAttribute("special_parameter_NZ")).getAttributeFloat();

	std::cout << "finished reading nx, ny, nz" << std::endl;
	/********* get value for number_of_blocks **********/
	int number_of_blocks =
			(modelReader->getGlobalAttribute("number_of_blocks")).getAttributeInt();
}

/**
 * @param variable
 * @param c0 X component of the position
 * @param c1 Y component of the position
 * @param c2 Z component of the position
 * @return
 */
float BATSRUSInterpolator::interpolate(const std::string& variable, const float& c0, const float& c1, const float& c2)
{

	float dc0, dc1, dc2;
	long variable_id = modelReader->getVariableID(variable);
	return interpolate(variable_id, c0,c1,c2,dc0,dc1,dc2);



}

/**
 * @param variable_id
 * @param c0 X component of the position
 * @param c1 Y component of the position
 * @param c2 Z component of the position
 * @return
 */
float BATSRUSInterpolator::interpolate(long variable_id, const float& c0, const float& c1, const float& c2)
{

	float dc0, dc1, dc2;
	return interpolate(variable_id, c0,c1,c2,dc0,dc1,dc2);



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
float BATSRUSInterpolator::interpolate(long variable_id, const float& c0, const float& c1, const float& c2,
		float& dc0, float& dc1, float& dc2)
{
	//std::cout << "point: " << c0 << "," << c1 << "," << c2 << std::endl;
	long status;

	/********* interpolate_amr_data variables for new interpolation routine/upgrade *******/



	int ic, new_blk[8], valid;
	float ixx, iyy, izz, dx1, dy1, dz1, dx2, dy2, dz2, data_c[8], d_m1, d_m2,
			yy_c2[4], zz_c2[4];

	float XMIN, XMAX, YMIN, YMAX, ZMIN, ZMAX;

	/*extern int is_var_in_memory( char * );*//*Previously Defined */

	/** lets see if requested variable is in memory **/

	/*************************************************************************************************

	 * LOGIC BLOCK to determine if interpolator should search for new block or use previosuly stored

	 * position and block

	 *************************************************************************************************/
	/*
	 if (call_count == 0) /*** first time this routine has been called ***/
	/*	{
	 previous_x = X;
	 previous_y = Y;
	 previous_z = Z;
	 } else if (previous_x == X && previous_y == Y && previous_z == Z
	 && !new_cdf_file) /*** not first time routine has been called, see if X, y, z has changed ***/
	/*	{ /** also check to see if this is a new file or not. **/
	//new_position = 0;
	/*	} else /** positions have changed or file has changed, store for next iteration **/
	/*	{
	 previous_x = X;
	 previous_y = Y;
	 previous_z = Z;
	 }*/

	/** set new_cdf_file flag to false until open_cdf is called again **/

	//new_cdf_file = 0;

	/*printf("%s \tDEBUG\tnew_position = %d\n", __FILE__, new_position );*/

	/**********************************************************************************************/

	//	counts[0] = 0; /*reset values after once through */
	int intervals[] = { 1 };

	//opt_arg_flag = optional_argument_flag;

	//va_start(argptr, optional_argument_flag);
	/* make argptr point to first UNAMED arguments which should be the missing variable */

	/*if( opt_arg_flag )*/
	//if (optional_argument_flag) {

	/*printf("DEBUG:\toptional arguments are present\n");*/

	/* there are optional arguments 1 - 4, therofore set missing, dx, dy, dz */

	/*hack = va_arg( argptr, double );*/
	/*missing	= va_arg( argptr, double );
	 dx = va_arg( argptr, float * );
	 dy = va_arg( argptr, float * );
	 dz = va_arg( argptr, float * );
	 */
	/*

	 printf("DEBUG:\toptional arguments are present setting missing value to -->%f<-- and also copying pointers...hack = %f\n", missing, hack );

	 printf("DEBUG:\t******** value of copied pointers ****** dx = %p, dy = %p,dz = %p\n", dx, dy, dz );

	 */

	/* for field line tracing,etc..., select appropriate variable number ie. bx_cdfNum|by_cdfNum|bz_cdfNum based on *variable_string */

	/************** NEW INTERPOLATION ROUTINE & MODIFICATION ***************/
	int new_position = 1;
	if (old_x == c0 && old_y == c1 && old_z == c2 && previousWasValid)
	{
		new_position = 0;
	} else
	{
		//callCount = 0;
		//new_position = 1;
	}
	if (new_position)
	{
		//std::cout << "new position!!" << std::endl;
		ib = find_octree_block(c0, c1, c2, -1);

		if (ib == -1)
		{
			std::cout << "returning missing value. ib = -1" << std::endl;
			return missingValue;
		}

		/*** set the BLOCK MIN/MAX values depending on main_memory flag ***/

		//if (main_memory_flag) {
		XMIN = ((*block_x_min_array)[ib]);
		XMAX = ((*block_x_max_array)[ib]);
		YMIN = ((*block_y_min_array)[ib]);
		YMAX = ((*block_y_max_array)[ib]);
		ZMIN = ((*block_z_min_array)[ib]);
		ZMAX = ((*block_z_max_array)[ib]);

		/*} else {
		 set_block_min_max(ib);
		 XMIN = block_x_min;
		 XMAX = block_x_max;
		 YMIN = block_y_min;
		 YMAX = block_y_max;
		 ZMIN = block_z_min;
		 ZMAX = block_z_max;
		 }*/

		/********** DEBUG

		 rintf("DEBUG:\tBLOCK X MIN = %f\n", XMIN );

		 rintf("DEBUG:\tBLOCK X MAX = %f\n", XMAX );

		 rintf("DEBUG:\tBLOCK Y MIN = %f\n", YMIN );

		 rintf("DEBUG:\tBLOCK Y MAX = %f\n", YMAX );

		 rintf("DEBUG:\tBLOCK Z MIN = %f\n", ZMIN );

		 rintf("DEBUG:\tBLOCK Z MAX = %f\n", ZMAX );

		 ****************/

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

		/********** DEBUG

		 rintf("nx = %d,ny = %d, nz = %d\n", nx,ny,nz );

		 rintf("NX = %f,NY = %f, NZ = %f\n", NX,NY,NZ );

		 rintf("dx1 = %f,dy1 = %f, dz1 = %f\n", dx1,dy1,dz1 );

		 rintf("ixx = %f,iyy = %f, izz = %f\n", ixx,iyy,izz );

		 ************/

		/*** indices of grid positions around sample locations ***/

		ix_c[0] = ix_c[2] = ix_c[4] = ix_c[6] = floor(ixx);
		ix_c[1] = ix_c[3] = ix_c[5] = ix_c[7] = floor(ixx + 1);
		iy_c[0] = iy_c[1] = iy_c[4] = iy_c[5] = floor(iyy);
		iy_c[2] = iy_c[3] = iy_c[6] = iy_c[7] = floor(iyy + 1);
		iz_c[0] = iz_c[1] = iz_c[2] = iz_c[3] = floor(izz);
		iz_c[4] = iz_c[5] = iz_c[6] = iz_c[7] = floor(izz + 1);

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

					//					if (main_memory_flag) {
					XMIN = (*block_x_min_array)[ibc];
					XMAX = (*block_x_max_array)[ibc];
					YMIN = (*block_y_min_array)[ibc];
					YMAX = (*block_y_max_array)[ibc];
					ZMIN = (*block_z_min_array)[ibc];
					ZMAX = (*block_z_max_array)[ibc];
					/*				} else {
					 set_block_min_max(ibc);
					 XMIN = block_x_min;
					 XMAX = block_x_max;
					 YMIN = block_y_min;
					 YMAX = block_y_max;
					 ZMIN = block_z_min;
					 ZMAX = block_z_max;
					 }
					 */
					dx2 = (XMAX - XMIN) / nx;
					dy2 = (YMAX - YMIN) / ny;
					dz2 = (ZMAX - ZMIN) / nz;

					xx_c[ic] = c0 + (ic % 2) * dx2; /* adjust stencil with */
					yy_c[ic] = c1 + ((ic % 4) / 2) * dy2; /* resolution change between */
					zz_c[ic] = c2 + (ic / 4) * dz2; /* neighboring blocks */

					ix_c[ic] = min(nx - 1., max(0., floor((xx_c[ic] - XMIN) / dx2 - 0.5)));
					iy_c[ic] = min(ny - 1., max(0., floor((yy_c[ic] - YMIN) / dy2 - 0.5)));
					iz_c[ic] = min(nz - 1., max(0., floor((zz_c[ic] - ZMIN) / dz2 - 0.5)));
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
					xx_c[ic] = fabs(missingValue); /*x_blk[ib*NX+ix_c[ic]]; */
					yy_c[ic] = fabs(missingValue); /*y_blk[ib*NY+iy_c[ic]]; */
					zz_c[ic] = fabs(missingValue); /*z_blk[ib*NZ+iz_c[ic]]; */
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
//std::cout << "ib: " << ib << std::endl;
		//if (main_memory_flag) {
		XMIN = (*block_x_min_array)[ib];
		XMAX = (*block_x_max_array)[ib];
		YMIN = (*block_y_min_array)[ib];
		YMAX = (*block_y_max_array)[ib];
		ZMIN = (*block_z_min_array)[ib];
		ZMAX = (*block_z_max_array)[ib];

		//} else {
		/*set_block_min_max(ib);
		 XMIN = block_x_min;
		 XMAX = block_x_max;
		 YMIN = block_y_min;
		 YMAX = block_y_max;
		 ZMIN = block_z_min;
		 ZMAX = block_z_max;*/
		//}

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

	const std::vector<float>* vData = modelReader->getVariableDataByID(variable_id);
	for (ic = 0; ic < 8; ic++)
	{

		float temp_space;

		/********* NOTE to SELF:  Put variable name in an array or something next time, but since this works.... ***********/

		int index = ix_c[ic] + nx * (iy_c[ic] + ny
				* (iz_c[ic] + nz * ib_c[ic]));

//cout << "variable: " << variable << " value: " << (*variableData[variable])[index] << " index: " << index << " length of variable: " << (*variableData[variable]).size()<< std::endl;
		data_c[ic] = (*vData)[index];

	} /* end of for( ic = 0; ic < 8; ic++) loop */

	/************** per Lutz do pair-wise interpolations in x,y, & z directions *********/

	for (ic = 0; ic < 4; ic++)
	{
		int ic2, ic2_1;
		ic2 = 2 * ic;
		ic2_1 = ic2 + 1;
		d_m1 = (c0 - xx_c[ic2]);
		d_m2 = (xx_c[ic2_1] - c0);

		if (valid_c[ic2] && valid_c[ic2_1] && (fabs(d_m1 + d_m2) > (dx1 / 4.)))
		/*      if( d_m1 * d_m2 > 0 ) */
		{

			data_c[ic] = (d_m2 * data_c[ic2] + d_m1 * data_c[ic2_1]) / (d_m1
					+ d_m2);

			yy_c2[ic] = (d_m2 * yy_c[ic2] + d_m1 * yy_c[ic2_1]) / (d_m1 + d_m2);

			zz_c2[ic] = (d_m2 * zz_c[ic2] + d_m1 * zz_c[ic2_1]) / (d_m1 + d_m2);
			valid = 1;
		} else
		{
			valid = 0;
			/*            if( fabs( d_m1 ) > fabs( d_m2 ) ) */
			if (valid_c[ic2_1])
			{
				data_c[ic] = data_c[ic2_1];

				yy_c2[ic] = yy_c[ic2_1];

				zz_c2[ic] = zz_c[ic2_1];
				valid = 1;
			}
			/*            }

			 else

			 { */
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

		/*        if( d_m1 * d_m2 > 0 ) */
		if (valid_c[ic2] && valid_c[ic2_1] && (fabs(d_m1 + d_m2) >= (dy1 / 4.)))
		{
			zz_c2[ic] = (d_m2 * zz_c2[ic2] + d_m1 * zz_c2[ic2_1]) / (d_m1
					+ d_m2);
			data_c[ic] = (d_m2 * data_c[ic2] + d_m1 * data_c[ic2_1]) / (d_m1
					+ d_m2);
			valid = 1;
		} else
		{
			valid = 0;
			/*            if( fabs( d_m1 ) > fabs( d_m2 ) ) */
			if (valid_c[ic2_1])
			{
				data_c[ic] = data_c[ic2_1];
				zz_c2[ic] = zz_c2[ic2_1];
				valid = 1;
			}
			/*            }

			 else

			 { */
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

	/*    if( d_m1 * d_m2 > 0 ) */
	if (valid_c[0] && valid_c[1] && (fabs(d_m1 + d_m2) >= (dz1 / 4.)))
	{
		data_c[0] = ((d_m2 * data_c[0] + d_m1 * data_c[1]) / (d_m1 + d_m2));
		valid = 1;
	} else
	{
		/*        if( fabs( d_m1 ) > fabs( d_m2 ) ) */
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
	/* return( data_c[ 0 ] ); */

	/*Point<float> point;

	Cell3D<float, float> cell = getCell(variable, -c0, -c1, c2);

	dc0 = cell.getPositions()[1].c0() - cell.getPositions()[0].c0();
	dc1 = cell.getPositions()[4].c1() - cell.getPositions()[0].c1();
	dc2 = cell.getPositions()[2].c2() - cell.getPositions()[0].c2();
	Point<float> p(-c0, -c1, c2);

	float value = cell.interpolateData(p);// * getConversionFactor(variable);
	return value;*/
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
float BATSRUSInterpolator::interpolate(const std::string& variable, const float& c0, const float& c1, const float& c2,
		float& dc0, float& dc1, float& dc2)
{
//	std::cout << "BATSRUSInterpolator::interpolate. variable: " << variable << std::endl;
	//long variable_id = modelReader->getVariableID(variable);
	//return interpolate(variable_id, c0, c1, c2, dc0, dc1, dc2);

	//std::cout << "point: " << c0 << "," << c1 << "," << c2 << std::endl;
		long status;

		/********* interpolate_amr_data variables for new interpolation routine/upgrade *******/



		int ic, new_blk[8], valid;
		float ixx, iyy, izz, dx1, dy1, dz1, dx2, dy2, dz2, data_c[8], d_m1, d_m2,
				yy_c2[4], zz_c2[4];

		float XMIN, XMAX, YMIN, YMAX, ZMIN, ZMAX;

		/*extern int is_var_in_memory( char * );*//*Previously Defined */

		/** lets see if requested variable is in memory **/

		/*************************************************************************************************

		 * LOGIC BLOCK to determine if interpolator should search for new block or use previosuly stored

		 * position and block

		 *************************************************************************************************/
		/*
		 if (call_count == 0) /*** first time this routine has been called ***/
		/*	{
		 previous_x = X;
		 previous_y = Y;
		 previous_z = Z;
		 } else if (previous_x == X && previous_y == Y && previous_z == Z
		 && !new_cdf_file) /*** not first time routine has been called, see if X, y, z has changed ***/
		/*	{ /** also check to see if this is a new file or not. **/
		//new_position = 0;
		/*	} else /** positions have changed or file has changed, store for next iteration **/
		/*	{
		 previous_x = X;
		 previous_y = Y;
		 previous_z = Z;
		 }*/

		/** set new_cdf_file flag to false until open_cdf is called again **/

		//new_cdf_file = 0;

		/*printf("%s \tDEBUG\tnew_position = %d\n", __FILE__, new_position );*/

		/**********************************************************************************************/

		//	counts[0] = 0; /*reset values after once through */
		int intervals[] = { 1 };

		//opt_arg_flag = optional_argument_flag;

		//va_start(argptr, optional_argument_flag);
		/* make argptr point to first UNAMED arguments which should be the missing variable */

		/*if( opt_arg_flag )*/
		//if (optional_argument_flag) {

		/*printf("DEBUG:\toptional arguments are present\n");*/

		/* there are optional arguments 1 - 4, therofore set missing, dx, dy, dz */

		/*hack = va_arg( argptr, double );*/
		/*missing	= va_arg( argptr, double );
		 dx = va_arg( argptr, float * );
		 dy = va_arg( argptr, float * );
		 dz = va_arg( argptr, float * );
		 */
		/*

		 printf("DEBUG:\toptional arguments are present setting missing value to -->%f<-- and also copying pointers...hack = %f\n", missing, hack );

		 printf("DEBUG:\t******** value of copied pointers ****** dx = %p, dy = %p,dz = %p\n", dx, dy, dz );

		 */

		/* for field line tracing,etc..., select appropriate variable number ie. bx_cdfNum|by_cdfNum|bz_cdfNum based on *variable_string */

		/************** NEW INTERPOLATION ROUTINE & MODIFICATION ***************/
		int new_position = 1;
		if (old_x == c0 && old_y == c1 && old_z == c2 && previousWasValid)
		{
			new_position = 0;
		} else
		{
			//callCount = 0;
			//new_position = 1;
		}
		if (new_position)
		{
			//std::cout << "new position!!" << std::endl;
			ib = find_octree_block(c0, c1, c2, -1);

			if (ib == -1)
			{
				std::cout << "returning missing value. ib = -1" << std::endl;
				return missingValue;
			}

			/*** set the BLOCK MIN/MAX values depending on main_memory flag ***/

			//if (main_memory_flag) {
			XMIN = ((*block_x_min_array)[ib]);
			XMAX = ((*block_x_max_array)[ib]);
			YMIN = ((*block_y_min_array)[ib]);
			YMAX = ((*block_y_max_array)[ib]);
			ZMIN = ((*block_z_min_array)[ib]);
			ZMAX = ((*block_z_max_array)[ib]);

			/*} else {
			 set_block_min_max(ib);
			 XMIN = block_x_min;
			 XMAX = block_x_max;
			 YMIN = block_y_min;
			 YMAX = block_y_max;
			 ZMIN = block_z_min;
			 ZMAX = block_z_max;
			 }*/

			/********** DEBUG

			 rintf("DEBUG:\tBLOCK X MIN = %f\n", XMIN );

			 rintf("DEBUG:\tBLOCK X MAX = %f\n", XMAX );

			 rintf("DEBUG:\tBLOCK Y MIN = %f\n", YMIN );

			 rintf("DEBUG:\tBLOCK Y MAX = %f\n", YMAX );

			 rintf("DEBUG:\tBLOCK Z MIN = %f\n", ZMIN );

			 rintf("DEBUG:\tBLOCK Z MAX = %f\n", ZMAX );

			 ****************/

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

			/********** DEBUG

			 rintf("nx = %d,ny = %d, nz = %d\n", nx,ny,nz );

			 rintf("NX = %f,NY = %f, NZ = %f\n", NX,NY,NZ );

			 rintf("dx1 = %f,dy1 = %f, dz1 = %f\n", dx1,dy1,dz1 );

			 rintf("ixx = %f,iyy = %f, izz = %f\n", ixx,iyy,izz );

			 ************/

			/*** indices of grid positions around sample locations ***/

			ix_c[0] = ix_c[2] = ix_c[4] = ix_c[6] = floor(ixx);
			ix_c[1] = ix_c[3] = ix_c[5] = ix_c[7] = floor(ixx + 1);
			iy_c[0] = iy_c[1] = iy_c[4] = iy_c[5] = floor(iyy);
			iy_c[2] = iy_c[3] = iy_c[6] = iy_c[7] = floor(iyy + 1);
			iz_c[0] = iz_c[1] = iz_c[2] = iz_c[3] = floor(izz);
			iz_c[4] = iz_c[5] = iz_c[6] = iz_c[7] = floor(izz + 1);

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

						//					if (main_memory_flag) {
						XMIN = (*block_x_min_array)[ibc];
						XMAX = (*block_x_max_array)[ibc];
						YMIN = (*block_y_min_array)[ibc];
						YMAX = (*block_y_max_array)[ibc];
						ZMIN = (*block_z_min_array)[ibc];
						ZMAX = (*block_z_max_array)[ibc];
						/*				} else {
						 set_block_min_max(ibc);
						 XMIN = block_x_min;
						 XMAX = block_x_max;
						 YMIN = block_y_min;
						 YMAX = block_y_max;
						 ZMIN = block_z_min;
						 ZMAX = block_z_max;
						 }
						 */
						dx2 = (XMAX - XMIN) / nx;
						dy2 = (YMAX - YMIN) / ny;
						dz2 = (ZMAX - ZMIN) / nz;

						xx_c[ic] = c0 + (ic % 2) * dx2; /* adjust stencil with */
						yy_c[ic] = c1 + ((ic % 4) / 2) * dy2; /* resolution change between */
						zz_c[ic] = c2 + (ic / 4) * dz2; /* neighboring blocks */

						ix_c[ic] = min(nx - 1., max(0., floor((xx_c[ic] - XMIN) / dx2 - 0.5)));
						iy_c[ic] = min(ny - 1., max(0., floor((yy_c[ic] - YMIN) / dy2 - 0.5)));
						iz_c[ic] = min(nz - 1., max(0., floor((zz_c[ic] - ZMIN) / dz2 - 0.5)));
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
						xx_c[ic] = fabs(missingValue); /*x_blk[ib*NX+ix_c[ic]]; */
						yy_c[ic] = fabs(missingValue); /*y_blk[ib*NY+iy_c[ic]]; */
						zz_c[ic] = fabs(missingValue); /*z_blk[ib*NZ+iz_c[ic]]; */
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
	//std::cout << "ib: " << ib << std::endl;
			//if (main_memory_flag) {
			XMIN = (*block_x_min_array)[ib];
			XMAX = (*block_x_max_array)[ib];
			YMIN = (*block_y_min_array)[ib];
			YMAX = (*block_y_max_array)[ib];
			ZMIN = (*block_z_min_array)[ib];
			ZMAX = (*block_z_max_array)[ib];

			//} else {
			/*set_block_min_max(ib);
			 XMIN = block_x_min;
			 XMAX = block_x_max;
			 YMIN = block_y_min;
			 YMAX = block_y_max;
			 ZMIN = block_z_min;
			 ZMAX = block_z_max;*/
			//}

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

		const std::vector<float>* vData = modelReader->getVariableData(variable);
		for (ic = 0; ic < 8; ic++)
		{

			float temp_space;

			/********* NOTE to SELF:  Put variable name in an array or something next time, but since this works.... ***********/

			int index = ix_c[ic] + nx * (iy_c[ic] + ny
					* (iz_c[ic] + nz * ib_c[ic]));

	//cout << "variable: " << variable << " value: " << (*variableData[variable])[index] << " index: " << index << " length of variable: " << (*variableData[variable]).size()<< std::endl;
			data_c[ic] = (*vData)[index];

		} /* end of for( ic = 0; ic < 8; ic++) loop */

		/************** per Lutz do pair-wise interpolations in x,y, & z directions *********/

		for (ic = 0; ic < 4; ic++)
		{
			int ic2, ic2_1;
			ic2 = 2 * ic;
			ic2_1 = ic2 + 1;
			d_m1 = (c0 - xx_c[ic2]);
			d_m2 = (xx_c[ic2_1] - c0);

			if (valid_c[ic2] && valid_c[ic2_1] && (fabs(d_m1 + d_m2) > (dx1 / 4.)))
			/*      if( d_m1 * d_m2 > 0 ) */
			{

				data_c[ic] = (d_m2 * data_c[ic2] + d_m1 * data_c[ic2_1]) / (d_m1
						+ d_m2);

				yy_c2[ic] = (d_m2 * yy_c[ic2] + d_m1 * yy_c[ic2_1]) / (d_m1 + d_m2);

				zz_c2[ic] = (d_m2 * zz_c[ic2] + d_m1 * zz_c[ic2_1]) / (d_m1 + d_m2);
				valid = 1;
			} else
			{
				valid = 0;
				/*            if( fabs( d_m1 ) > fabs( d_m2 ) ) */
				if (valid_c[ic2_1])
				{
					data_c[ic] = data_c[ic2_1];

					yy_c2[ic] = yy_c[ic2_1];

					zz_c2[ic] = zz_c[ic2_1];
					valid = 1;
				}
				/*            }

				 else

				 { */
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

			/*        if( d_m1 * d_m2 > 0 ) */
			if (valid_c[ic2] && valid_c[ic2_1] && (fabs(d_m1 + d_m2) >= (dy1 / 4.)))
			{
				zz_c2[ic] = (d_m2 * zz_c2[ic2] + d_m1 * zz_c2[ic2_1]) / (d_m1
						+ d_m2);
				data_c[ic] = (d_m2 * data_c[ic2] + d_m1 * data_c[ic2_1]) / (d_m1
						+ d_m2);
				valid = 1;
			} else
			{
				valid = 0;
				/*            if( fabs( d_m1 ) > fabs( d_m2 ) ) */
				if (valid_c[ic2_1])
				{
					data_c[ic] = data_c[ic2_1];
					zz_c2[ic] = zz_c2[ic2_1];
					valid = 1;
				}
				/*            }

				 else

				 { */
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

		/*    if( d_m1 * d_m2 > 0 ) */
		if (valid_c[0] && valid_c[1] && (fabs(d_m1 + d_m2) >= (dz1 / 4.)))
		{
			data_c[0] = ((d_m2 * data_c[0] + d_m1 * data_c[1]) / (d_m1 + d_m2));
			valid = 1;
		} else
		{
			/*        if( fabs( d_m1 ) > fabs( d_m2 ) ) */
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
			if (((*block_x_min_array)[block_index_1] <= x)
					&& ((*block_x_max_array)[block_index_1] >= x)
					&& ((*block_y_min_array)[block_index_1] <= y)
					&& ((*block_y_max_array)[block_index_1] >= y)
					&& ((*block_z_min_array)[block_index_1] <= z)
					&& ((*block_z_max_array)[block_index_1] >= z))
			{

				return (block_index_1);
			}
		}

		/*** if there are no amr levels defined, our block tree structure is useless - do an old fashioned linear search ***/
		/*
		 if (number_of_parents_at_amr_level == NULL) {

		 for (block_index_1 = 0; block_index_1 < number_of_blocks; block_index_1++) {
		 if ((block_x_min[block_index_1] <= x)
		 && (block_x_max[block_index_1] >= x)
		 && (block_y_min[block_index_1] <= y)
		 && (block_y_max[block_index_1] >= y)
		 && (block_z_min[block_index_1] <= z)
		 && (block_z_max[block_index_1] >= z)) {
		 return (block_index_1);
		 }
		 }

		 //**** no block was found ***

		 printf("ERROR:\tlinear block search returned no index!\n");

		 return (-1);

		 }
		 */
		/*printf("DEBUG\twhile %d < %d && %d == -1\n", root_index ,number_of_parents_at_amr_level[0] , block_index_2 );*/

		while ((root_index < (*block_at_amr_level_array)[0])
				&& (block_index_2 == -1))
		{
			//std::cout << "inside while loop. block_index_1 = "
					//<< variableData["block_at_amr_level"].size() << std::endl;
			block_index_1 = (*block_at_amr_level_array)[root_index];
//std::cout << "root_index: " << root_index << std::endl;
			/*

			 printf("\n\ntesting block_index_1\t%d\n", block_index_1 );

			 printf("\ntesting xmin %f <= %f\n", block_x_min[block_index_1], x );

			 printf("testing xmax %f >= %f\n", block_x_max[block_index_1], x );

			 printf("testing ymin %f <= %f\n", block_y_min[block_index_1], y );

			 printf("testing ymax %f >= %f\n", block_y_max[block_index_1], y );

			 printf("testing zmin %f <= %f\n", block_z_min[block_index_1], z );

			 printf("testing zmax %f >= %f\n", block_z_max[block_index_1], z );

			 */

			if (block_index_1 < (*block_x_min_array).size()
					&& ((*block_x_min_array)[block_index_1] <= x)
					&& ((*block_x_max_array)[block_index_1] >= x)
					&& ((*block_y_min_array)[block_index_1] <= y)
					&& ((*block_y_max_array)[block_index_1] >= y)
					&& ((*block_z_min_array)[block_index_1] <= z)
					&& ((*block_z_max_array)[block_index_1] >= z))
			{

				block_index_2 = climb_octree(block_index_1, x, y, z);
			} else
			{
				root_index++;
			}
		}
		//std::cout << "root_index: " << root_index << " variableData[\"block_at_amr_level\"][" << root_index << "]: " << variableDataInt["block_at_amr_level"][0] <<  " block_index_2: " << block_index_2 << std::endl;
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
	//std::cout << "root: " << root << std::endl;
	long recordStart = 0;
	long indices[1];

	int ix, iy, iz;

	int child_id, child_key;

	indices[0] = root;

	/*printf("\nclimbing block tree structure...now in block %d\n", root);*/

	/******** if main memory flag is NOT set, read all required data directly from cdf file **********/

	if ((*block_child_count_array)[root] == 0)
	{
		//std::cout << " returning the root value, since there are no children!" << std::endl;
		return (root);
	}

	ix = x > (*block_x_center_array)[root];
	iy = y > (*block_y_center_array)[root];
	iz = z > (*block_z_center_array)[root];

	/********** calculate & retrieve child ID **************/

	child_key = iz * 4 + 2* iy + ix;
//std::cout << "child_key: " << child_key << std::endl;
	/*printf("DEBUG\tsetting child id key = %d\n", child_key );*/

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

	/*printf("DEBUG\trecursive call to climb_octree with child_id = %d\n", child_id );    */

	return climb_octree(child_id, x, y, z);
}

/**
 * Destructor
 */
BATSRUSInterpolator::~BATSRUSInterpolator()
{
	// TODO Auto-generated destructor stub
}
