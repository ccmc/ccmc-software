/*
 * MASInterpolator.cpp
 *
 *  Created on: Jul 9, 2009
 *      Author: dberrios
 */

#include "MASInterpolator.h"
#include "MAS.h"
#include "StringConstants.h"
#include "Constants.h"
#include "Utils.h"
//#define DEBUG_MAS_INTERPOLATOR

namespace ccmc
{
	/**
	 * @param model
	 */
	MASInterpolator::MASInterpolator(Model * model)
	{
		//std::cout << "Creating MAS Interpolator" << std::endl;
		// TODO Auto-generated constructor stub
		// TODO Auto-generated constructor stub
		this->modelReader = model;
		float missingValue = this->modelReader->getMissingValue();

		previous_r = missingValue;
		previous_lon = missingValue;
		previous_lat = missingValue;

	}

	/**
	 * @param variable
	 * @param r
	 * @param lat
	 * @param lon
	 * @return
	 */
	float MASInterpolator::interpolate(const std::string& variable, const float& r, const float& lat, const float& lon)
	{
#ifdef DEBUG_MAS_INTERPOLATOR
		std::cout << "calling MASInterpolator::interpolate(const std::string& variable, const float& r, const float& lat, const float& lon)" << std::endl;
#endif
		float dr, dlat, dlon;
		long variable_id = modelReader->getVariableID(variable);

		return interpolate(variable_id, r, lat, lon, dr, dlat, dlon);

	}

	/**
	 * @param variable
	 * @param r
	 * @param lat
	 * @param lon
	 * @param dr
	 * @param dlat
	 * @param dlon
	 * @return
	 */
	float MASInterpolator::interpolate(const std::string& variable, const float& r, const float& lat, const float& lon, float& dr, float& dlat,
			float& dlon)
	{
		long variable_id = modelReader->getVariableID(variable);
		//std::cerr << "inside interpolate. varaible_id=" << variable_id << " for variable=" << variable << std::endl;
		return interpolate(variable_id, r, lat, lon, dr, dlat, dlon);
	}

	/**
	 * @param variableID
	 * @param r
	 * @param lat
	 * @param lon
	 * @return
	 */
	float MASInterpolator::interpolate(const long& variableID, const float& r, const float& lat, const float& lon)
	{
		float dr, dlat, dlon;
		return this->interpolate(variableID, r, lat, lon, dr, dlat, dlon);
	}

	/**
	 * @param variableID
	 * @param r
	 * @param lat
	 * @param lon
	 * @param dr
	 * @param dlat
	 * @param dlon
	 * @return
	 */
	float MASInterpolator::interpolate(const long& variable_id, const float& r, const float& lat, const float& lon, float& dr, float& dlat, float& dlon)
	{








		float missingValue = this->modelReader->getMissingValue();


		if (variable_id < 0)
			return missingValue;

		//std::cout << "calling MASInterpolator::interpolate(const std::string& variable, const float& r, const float& lat, const float& lon, float& dr, float& dlat,	float& dlon)" << std::endl;

#ifdef DEBUG_MAS_INTERPOLATOR
		std::cout << "r: " << r << " theta (lat): " << lat << " phi (lon): " << lon << std::endl;
#endif
		//Convert radius to meters
		float r_converted = r;// * ccmc::constants::AU_in_meters;

		/* 1 *//*local_y = Z - ( pow( pi, 2 ) / 2 );*//** convert from latitude -60 to 60 to radians ...*/
		/* 2 *//*local_y = Z - ( pi / 2 );  *//** convert from latitude -60 to 60 to radians ...*/

		/*local_y = ( Z/radian_in_degrees ) + ( pi/2 );*//** convert from latitude -60...60 to 30...150 range in degress and then to radians...*/

		/*********** 3 ***********/

		//float lat_converted = -lat/ccmc::constants::RadiansToDegrees + ccmc::constants::Pi/2.f;
		float lat_converted = -lat;

		lat_converted = (lat_converted/ccmc::constants::RadiansToDegrees ) + (ccmc::constants::Pi/2. );

		/** convert degrees ( 0 - 360 longitude ) to radiadns **/
		//first check to see if the degrees are between 0 and 360

		/** correct for longitude angles less than 0 or having a magnitude greater
		 * than 360.f
		 */
		float lon_converted = lon;

		if (lon_converted < 0.f)
		{
			while(lon_converted < 0.f)
			{
				lon_converted += 360.f;
			}

		} else if (lon_converted > 360.f)
		{
			while(lon_converted > 360.f)
			{
				lon_converted -= 360.f;
			}

		}

		lon_converted = lon_converted / ccmc::constants::RadiansToDegrees;
		const std::vector<float> * const r_data = ((MAS*)modelReader)->getRPosGrid(variable_id);

#ifdef DEBUG_MAS_INTERPOLATOR
		std::cout << "fetched r_data" << std::endl;
#endif
		const std::vector<float> * const lat_data = ((MAS*)modelReader)->getLatPosGrid(variable_id);

		const std::vector<float> * const lon_data = ((MAS*)modelReader)->getLonPosGrid(variable_id);
		//std::cerr << "variable: " << this->modelReader->getVariableName(variable_id) << " r_data.size(): " << r_data->size() << " lat_data.size(): " << lat_data->size() << " lon_data.size(): " << lon_data->size() << std::endl;

#ifdef DEBUG_MAS_INTERPOLATOR
		std::cout << "fetched lat_data" << std::endl;
#endif

		int nr = (*r_data).size();
		int nlat = (*lat_data).size();
		int nlon = (*lon_data).size();

#ifdef DEBUG_MAS_INTERPOLATOR
		std::cout << "r_converted: " << r_converted << " theta_converted (lat): " << lat_converted << " phi_converted (lon): " << lon_converted << std::endl;
#endif

		std::string r_name = ((MAS*)modelReader)->getRPosGridName(variable_id);
		std::string lat_name = ((MAS*)modelReader)->getLatPosGridName(variable_id);
		std::string lon_name = ((MAS*)modelReader)->getLonPosGridName(variable_id);
		int ir, ilat, ilon;
		if (previous_r == r && previous_lon == lon && previous_lat == lat &&
			previous_r_grid_name == r_name && previous_lat_grid_name == lat_name && previous_lon_grid_name == lon_name)
		{
			ir = previous_ir;
			ilat = previous_ilat;
			ilon = previous_ilon;
		} else
		{
			//first, find the cell
			ir = Utils<float>::binary_search(*r_data, 0, (*r_data).size() - 1, r_converted);
			ilat = Utils<float>::binary_search(*lat_data, 0, (*lat_data).size() - 1, lat_converted);
			ilon = Utils<float>::binary_search(*lon_data, 0, (*lon_data).size() - 1, lon_converted);
		}

#ifdef DEBUG_MAS_INTERPOLATOR
		std::cerr << "ir: " << ir << " ilat: " << ilat << " ilon: " << ilon << std::endl;
#endif

		float value;
		if ((ir < 0) || (ir >= nr - 1) || (ilat < 0) || (ilat >= nlat - 1))
		{
			value = missingValue;
//			std::cerr << "returning missing value" << std::endl;
		} else
		{
//cout << "about to enter interpolate_in_block_enlil" << endl;
			value = interpolate_in_block_mas(r_converted, lat_converted, lon_converted, r_data, lat_data, lon_data,nr, nlat, nlon, ir, ilat, ilon,
					variable_id, dr, dlat, dlon);

			/****** we need to change the sign of any y vector component ... *********/

			if (((MAS*)modelReader)->getChangeSignFlag(variable_id)) /*** this flag is set when cdf_varNum is set above ***/
			{
				value = value * (-1.0);
			}

			/*printf("DEBUG:\tcall to interpolate_in_block complete\n");*/

			/*return interpolated_value;*/



		}

		previous_ir = ir;
		previous_ilon = ilon;
		previous_ilat = ilat;
		previous_r = r;
		previous_lon = lon;
		previous_lat = lat;
		return value;
	}

	/**
	 * @param r
	 * @param lat
	 * @param lon
	 * @param ir
	 * @param ilat
	 * @param ilon
	 * @param variableID
	 * @param dr
	 * @param dlat
	 * @param dlon
	 * @return
	 */
	float MASInterpolator::interpolate_in_block_mas(float r, float lat, float lon,
			const std::vector<float>* r_data, const std::vector<float>* lat_data, const std::vector<float>* lon_data,
			int nr, int nlat, int nlon, int ir, int ilat, int ilon,
			const long& variable_id, float& dr, float& dlat, float& dlon)
	{
//		std::cout << "ir: " << ir << " ilon: " << ilon << " ilat: " << ilat << std::endl;

		//x y z = r lat lon = r phi theta
		bool main_memory_flag = true;
		const std::vector<float> * vData = modelReader->getVariableFromMap(variable_id);
		if (vData == NULL)
			main_memory_flag = false;
//std::cout << "variable_id: " << variableID << " main_memory_flag: " << main_memory_flag << std::endl;
		float value;
		float dr_blk, dlat_blk, dlon_blk, m_r, m_lat, m_lon, two_pi = 4 * asin(1.);

		/*int ix, iy, iz;*/
		//int NV_blk = local_nx * local_ny, iz1=-1;
		int NV_blk = nlon * nlat, ilon1 = -1;

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

		/**      data_1 = uphi[ ( ix + iy*local_nx + iz*NV_blk ) ];
		         data_2 = uphi[ ( ix + 1 + iy*local_nx + iz*NV_blk ) ];
		         data_3 = uphi[ ( ix + ( iy + 1 )*local_nx + iz*NV_blk ) ];
		         data_4 = uphi[ ( ix + 1 + ( iy + 1)*local_nx + iz*NV_blk ) ];
		         data_5 = uphi[ ( ix + iy*local_nx + ( iz1 )*NV_blk ) ];
		         data_6 = uphi[ ( ix + 1 + iy*local_nx + ( iz1 )*NV_blk ) ];
		         data_7 = uphi[ ( ix + ( iy + 1 )*local_nx + ( iz1 )*NV_blk ) ];
		         data_8 = uphi[ ( ix + 1 + ( iy + 1)*local_nx + ( iz1)*NV_blk ) ];
		         */

		//( index_phi + index_theta*local_nx + index_r*NV_blk )
		indices[0] = index(ir, ilat, ilon, nlat, nlon);
		indices[1] = index(ir+1, ilat, ilon, nlat, nlon);
		indices[2] = index(ir, ilat+1, ilon, nlat, nlon);
		indices[3] = index(ir+1, ilat+1, ilon, nlat, nlon);
		indices[4] = index(ir, ilat, ilon+1, nlat, nlon);
		indices[5] = index(ir+1, ilat, ilon+1, nlat, nlon);
		indices[6] = index(ir, ilat+1, ilon+1, nlat, nlon);
		indices[7] = index(ir+1, ilat+1, ilon+1, nlat, nlon);
		if (!main_memory_flag)
		{

			for (int i = 0; i < 8; i++)
			{

				data[i] = modelReader->getVariableAtIndex(variable_id, indices[i]);
			}

		} else
		{

			for (int i = 0; i < 8; i++)
			{
				data[i] = (*vData)[indices[i]];
			}

		}

		/* printf("DEBUG\tdata 1..8 = %g %g %g %g %g %g %g this should be data_8-->%g<--\n", data_1, data_2, data_3, data_4, data_5, data_6, data_7, data_8 ); */

		/**
		 *  value = (1-m_lon)*( (1-m_lat)*( (1-m_r)*data[0] + m_r *data[1] ) + m_lat
         *( + (1-m_r)*data[2] + m_r *data[3] ) ) + m_lon*( (1-m_lat)*( +(1-m_r)
         *data[4] + m_r *data[5] ) + m_lat*( +(1-m_r)*data[6] + m_r *data[7] ) );
		 */
		value = (1 - m_lon) * ((1 - m_lat) * ((1 - m_r) * data[0] + m_r * data[1]) + m_lat * (+(1 - m_r) * data[2] + m_r
				* data[3])) + m_lon * ((1 - m_lat) * (+(1 - m_r) * data[4] + m_r * data[5]) + m_lat * (+(1 - m_r) * data[6] + m_r
				* data[7]));

		return (value);

	}


	/**
	 *
	 */
	int MASInterpolator::index(const int& ir, const int& ilat, const int& ilon, const int& nlat, const int& nlon)
	{
		return ilon + ( ilat * nlon ) + ( ir * nlon * nlat );
	}

	/**
	 *
	 */
	MASInterpolator::~MASInterpolator()
	{
		// TODO Auto-generated destructor stub
	}
}
