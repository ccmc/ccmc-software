/*
 * ENLILInterpolator.cpp
 *
 *  Created on: Jul 9, 2009
 *      Author: dberrios
 */

#include "ENLILInterpolator.h"
#include "Constants.h"
#include "Utils.h"
#include <iostream>

namespace ccmc
{
	ENLILInterpolator::ENLILInterpolator(Model * model)
	{
		// TODO Auto-generated constructor stub
		this->modelReader = model;
		this->setMissingValue(this->modelReader->getMissingValue());
		r_string = "r";
		lat_string = "phi";
		lon_string = "theta";
		r_data = modelReader->getVariableData(r_string);
		std::cout << "r_data" << std::endl;
		lat_data = modelReader->getVariableData(lat_string);
		lon_data = modelReader->getVariableData(lon_string);
		nr = r_data->size();
		nlat = lat_data->size();
		nlon = lon_data->size();

	}

	float ENLILInterpolator::interpolate(const std::string& variable, const float& r, const float& lon,
			const float& lat)
	{
		long variable_id = modelReader->getVariableID(variable);
		float dc0, dc1, dc2;
		return interpolate(variable_id, r, lon, lat, dc0, dc1, dc2);

	}

	float ENLILInterpolator::interpolate(const std::string& variable, const float& r, const float& lon,
			const float& lat, float& dr, float& dlon, float& dlat)
	{
		long variable_id = modelReader->getVariableID(variable);

		return interpolate(variable_id, r, lon, lat, dr, dlon, dlat);

	}

	float ENLILInterpolator::interpolate(long variableID, const float& r, const float& lon,
			const float& lat, float& dr, float& dlon, float& dlat)
	{

		int change_sign_flag = 0;


		//Convert radius to meters
		float r_converted = r * ccmc::constants::AU_in_meters;

		/* 1 *//*local_y = Z - ( pow( pi, 2 ) / 2 );*//** convert from latitude -60 to 60 to radians ...*/
		/* 2 *//*local_y = Z - ( pi / 2 );  *//** convert from latitude -60 to 60 to radians ...*/

		/*local_y = ( Z/radian_in_degrees ) + ( pi/2 );*//** convert from latitude -60...60 to 30...150 range in degress and then to radians...*/

		/*********** 3 ***********/

		float lat_converted = -lat/ccmc::constants::Radian_in_degrees + ccmc::constants::Pi/2.f;

		/** convert degrees ( 0 - 360 longitude ) to radiadns **/
		//first check to see if the degrees are between 0 and 360

		/** correct for longitude angles less than 0 or having a magnitude greater
		 * than 360.f
		 */
		float lon_converted = lon;

		if (lon_converted < 0.f)
		{
			lon_converted = -lon_converted;
			while(lon_converted > 360.f)
				lon_converted = lon_converted - 360.f;
			lon_converted = 360.f - lon_converted;

		} else if (lon_converted > 360.f)
		{
			while(lon_converted > 360.f)
				lon_converted = lon_converted - 360.f;

		}

		lon_converted = lon_converted / ccmc::constants::Radian_in_degrees;
		/*
		 #ifdef DEBUG_INTERFACE
		 printf("DEBUG\tinput position = [r,lon,lat] = [%f,%f,%f]\n", X,Y,Z);
		 printf("DEBUG\t\t\t\t\t\t\t\t\tconverted position = [r,phi,theta] = [%f,%f,%f]\n", local_x,local_y,local_z);
		 printf("DEBUG\t\t\t\t\t\t\t\t\tconverted position = [x,y,z]       = [%f,%f,%f]\n", local_x,local_y,local_z);
		 #endif
		 */

		int ir = Utils<float>::binary_search(*r_data, 0, (*r_data).size() - 1, r_converted);
		int ilat = Utils<float>::binary_search(*lat_data, 0, (*lat_data).size() - 1, lat_converted);
		int ilon = Utils<float>::binary_search(*lon_data, 0, (*lon_data).size() - 1, lon_converted);
//		cout << "ir: " << ir << " ilon: " << ilon << " ilat: " << ilat << endl;
		float value;
		if ((ir < 0) || (ir >= nr - 1) || (ilat < 0) || (ilat >= nlat - 1))
		{
			value = this->missingValue;
		} else
		{
//cout << "about to enter interpolate_in_block_enlil" << endl;
			value = interpolate_in_block_enlil(r_converted, lon_converted, lat_converted, ir, ilon, ilat,
					variableID, dr, dlon, dlat);

			/****** we need to change the sign of any y vector component ... *********/

			if (change_sign_flag) /*** this flag is set when cdf_varNum is set above ***/
			{
				value = value * (-1.0);
			}

			/*printf("DEBUG:\tcall to interpolate_in_block complete\n");*/

			/*return interpolated_value;*/



		}

		return value;
	}

	float ENLILInterpolator::interpolate(long variableID, const float& r, const float& lon, const float& lat)
	{
		float dc0, dc1, dc2;
		return interpolate(variableID, r, lon, lat, dc0, dc1, dc2);

	}

	ENLILInterpolator::~ENLILInterpolator()
	{
		// TODO Auto-generated destructor stub
	}

	float ENLILInterpolator::interpolate_in_block_enlil(float r, float lon, float lat, int ir, int ilon, int ilat,
			long variableID, float& dr, float& dlon, float& dlat)
	{
		cout << "ir: " << ir << " ilon: " << ilon << " ilat: " << ilat << endl;

		//x y z = r lat lon = r phi theta
		bool main_memory_flag = true;
		const std::vector<float> * vData = modelReader->getVariableDataByID(variableID);
		if (vData == NULL)
			main_memory_flag = false;

		float value;
		float dr_blk, dlat_blk, dlon_blk, m_r, m_lat, m_lon, two_pi = 4 * asin(1.);

		/*int ix, iy, iz;*/
		int NV_blk = nr * nlat, ilon1 = -1;

		/* int first_cell_in_block = found_block_index * nx * ny * nz; */

		/*printf("interpolating block %d\n", found_block_index );*/

		/*find_in_block( x, y, z, found_block_index, &ix, &iy, &iz, &data_ix, &data_iy, &data_iz );*/

		dr_blk = (*r_data)[ir + 1] - (*r_data)[ir];
		dlat_blk = (*lat_data)[ilat + 1] - (*lat_data)[ilat];

		/* periodic boundary */
		if ((ilon == (nlon - 1)) || (ilon == -1))
		{
			ilon = nlon - 1;
			ilon1 = 0;
			dlon_blk = ((*lon_data)[ilon1] - (*lon_data)[ilon] + two_pi);
			m_lon = (lon - (*lon_data)[nlon - 1] + ((*lon_data)[0] > lon) * two_pi) / dlon_blk;
		} else
		{
			ilon1 = ilon + 1;
			dlon_blk = (*lon_data)[ilon + 1] - (*lon_data)[ilon];
			m_lon = (lon - (*lon_data)[ilon]) / dlon_blk;
		}

		dr = dr_blk; /* return values to caller */
		dlat = dlat_blk;
		dlon = dlon_blk;

		m_r = (r - (*r_data)[ir]) / dr_blk;
		m_lat = (lat - (*lat_data)[ilat]) / dlat_blk;

		double data[8];
		int indices[8];
		indices[0] = (ir + ilat * nr + ilon * NV_blk);
		indices[1] = (ir + 1 + ilat * nr + ilon * NV_blk);
		indices[2] = (ir + (ilat + 1) * nr + ilon * NV_blk);
		indices[3] = (ir + 1 + (ilat + 1) * nr + ilon * NV_blk);
		indices[4] = (ir + ilat * nr + (ilon1) * NV_blk);
		indices[5] = (ir + 1 + ilat * nr + (ilon1) * NV_blk);
		indices[6] = (ir + (ilat + 1) * nr + (ilon1) * NV_blk);
		indices[7] = (ir + 1 + (ilat + 1) * nr + (ilon1) * NV_blk);
		if (!main_memory_flag)
		{

			for (int i = 0; i < 8; i++)
			{
				data[i] = modelReader->getVariableAtIndexByID(variableID, indices[i]);
			}

		} else
		{

			for (int i = 0; i < 8; i++)
			{
				data[i] = (*vData)[indices[i]];
			}

		}

		/* printf("DEBUG\tdata 1..8 = %g %g %g %g %g %g %g this should be data_8-->%g<--\n", data_1, data_2, data_3, data_4, data_5, data_6, data_7, data_8 ); */

		value = (1 - m_lon) * ((1 - m_lat) * ((1 - m_r) * data[0] + m_r * data[1]) + m_lat * (+(1 - m_r) * data[2] + m_r
				* data[3])) + m_lon * ((1 - m_lat) * (+(1 - m_r) * data[4] + m_r * data[5]) + m_lat * (+(1 - m_r) * data[6] + m_r
				* data[7]));

		return (value);

	}
}
