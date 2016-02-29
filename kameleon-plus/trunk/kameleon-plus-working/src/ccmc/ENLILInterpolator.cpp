/*
 * ENLILInterpolator.cpp
 *
 *  Created on: Jul 9, 2009
 *      Author: dberrios
 */

#include "MathHelper.h"
#include "Kameleon.h"
#include "ENLILInterpolator.h"
#include "ENLIL.h"
#include "Constants.h"
#include "StringConstants.h"
#include "Utils.h"
#include <iostream>
using namespace ccmc::constants;
namespace ccmc
{
	/**
	 * @param model
	 */
	ENLILInterpolator::ENLILInterpolator(Model * model)
	{

		this->modelReader = model;
		r_string = ccmc::strings::variables::r_;
		lat_string = ccmc::strings::variables::theta_;
		lon_string = ccmc::strings::variables::phi_;

		//TODO: fix the phi/theta issue to correspond to the actual
		//lat lon
		r_data = modelReader->getVariableFromMap(r_string);
		lat_data = modelReader->getVariableFromMap(lat_string);
		lon_data = modelReader->getVariableFromMap(lon_string);
		nr = r_data->size();
		nlat = lat_data->size();
		nlon = lon_data->size();

		previous_r = model->getMissingValue();
		previous_lon = model->getMissingValue();
		previous_lat = model->getMissingValue();
		previous_ir = 0;
		previous_ilon = 0;
		previous_ilat = 0;


	}

	void ENLILInterpolator::convertCoordinates(const std::string& source, const std::string& dest, const long time_et,
				const float& c0, const float& c1, const float& c2, 
				float& dc0, float& dc1, float& dc2)
	{
		Position preferred_p = {c0, c1, c2};
		Position target_p;
		Position spherical, heeq;


		if (source == "NATIVE"){ // use input position as output
			target_p = preferred_p; 
		} else if (dest == "UNKNOWN"){
			// do nothing		

		} else if (dest == "HNM"){ //Enlil
		
		// std::cout << "ENLIL interpolator converting input from " << source << " to cartesian HEEQ" << std::endl;
		// std::cout << "ENLIL interpolator input:" << preferred_p.c0 << " " << preferred_p.c1 << " " << preferred_p.c2 << std::endl;
		Kameleon::_cxform(source.c_str(), "HEEQ", time_et, &preferred_p, &heeq);		
		// std::cout << "ENLIL interpolator HEEQ:" << " " << heeq.c0 << " " << heeq.c1 << " " << heeq.c2 << std::endl;
		
		// std::cout << "ENLIL interpolator converting to spherical (r[km],theta[rad], phi[rad])" << std::endl;
		Math::convert_xyz_to_rthetaphi(heeq.c0,heeq.c1,heeq.c2, &spherical.c0, &spherical.c1, &spherical.c2);
		// std::cout << "ENLIL interpolator spherical:" << spherical.c0 << " " << spherical.c1 << " " << spherical.c2 << std::endl;
		
		// std::cout << "ENLIL interpolator convert to HNM (r0 [au],lat[deg],lon[deg]) " << std::endl;
		target_p.c0 = spherical.c0/AU_in_kilometers;
		target_p.c1 = spherical.c1*RadiansToDegrees - 90;
		target_p.c2 = spherical.c2*RadiansToDegrees;
		// std::cout << "ENLIL interpolator HNM:" << target_p.c0 << " " << target_p.c1 << " " << target_p.c2 << std::endl;
		// where's the 180 rotation?

		} else { 
			Kameleon::_cxform(source.c_str(), dest.c_str(), time_et, &preferred_p, &target_p);
		}

		dc0 = target_p.c0;
		dc1 = target_p.c1;
		dc2 = target_p.c2;	

	}



	/**
	 * @param variable
	 * @param r
	 * @param lat
	 * @param lon
	 * @return
	 */
	float ENLILInterpolator::interpolate(const std::string& variable, const float& r, const float& lat,
			const float& lon)
	{
		long variable_id = modelReader->getVariableID(variable);
		float dc0, dc1, dc2;
		return interpolate(variable_id, r, lat, lon, dc0, dc1, dc2);

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
	float ENLILInterpolator::interpolate(const std::string& variable, const float& r, const float& lat,
			const float& lon, float& dr, float& dlat, float& dlon)
	{
		long variable_id = modelReader->getVariableID(variable);

		return interpolate(variable_id, r, lat, lon, dr, dlat, dlon);

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
	float ENLILInterpolator::interpolate(const long& variableID, const float& r, const float& lat,
			const float& lon, float& dr, float& dlat, float& dlon)
	{

		float missingValue = this->modelReader->getMissingValue();
//		int change_sign_flag = 0;


		//Convert radius to meters
		float r_converted = r * ccmc::constants::AU_in_meters;

		/* 1 *//*local_y = Z - ( pow( pi, 2 ) / 2 );*//** convert from latitude -60 to 60 to radians ...*/
		/* 2 *//*local_y = Z - ( pi / 2 );  *//** convert from latitude -60 to 60 to radians ...*/

		/*local_y = ( Z/radian_in_degrees ) + ( pi/2 );*//** convert from latitude -60...60 to 30...150 range in degress and then to radians...*/

		/*********** 3 ***********/

		float lat_converted = -lat/ccmc::constants::RadiansToDegrees + ccmc::constants::Pi/2.f;

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

		lon_converted = lon_converted / ccmc::constants::RadiansToDegrees;
		int ir, ilat, ilon;
		if (previous_r == r && previous_lon == lon && previous_lat == lat)
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

		float value = missingValue;
		if ((ir < 0) || (ir >= nr - 1) || (ilat < 0) || (ilat >= nlat - 1))
		{
			value = missingValue;
//			std::cerr << "returning missing value" << std::endl;
		} else
		{
//cout << "about to enter interpolate_in_block_enlil" << endl;
			value = interpolate_in_block_enlil(r_converted, lat_converted, lon_converted, ir, ilat, ilon,
					variableID, dr, dlat, dlon);

			/****** we need to change the sign of any y vector component ... *********/

			if (((ENLIL*)modelReader)->getChangeSignFlagByID(variableID)) /*** this flag is set when cdf_varNum is set above ***/
			{
				value = value * (-1.0);
			}

			dr /= ccmc::constants::AU_in_meters;

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
	 * @param variableID
	 * @param r
	 * @param lat
	 * @param lon
	 * @return
	 */
	float ENLILInterpolator::interpolate(const long& variableID, const float& r, const float& lat, const float& lon)
	{
		float dc0, dc1, dc2;
		return interpolate(variableID, r, lat, lon, dc0, dc1, dc2);

	}

	/**
	 *
	 */
	ENLILInterpolator::~ENLILInterpolator()
	{
		// TODO Auto-generated destructor stub
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
	float ENLILInterpolator::interpolate_in_block_enlil(float r, float lat, float lon, int ir, int ilat, int ilon,
			long variableID, float& dr, float& dlat, float& dlon)
	{

		float missingValue = this->modelReader->getMissingValue();
		bool main_memory_flag = true;
		const std::vector<float> * vData = modelReader->getVariableFromMap(variableID);
		if (vData == NULL)
			main_memory_flag = false;
		float value;
		float dr_blk, dlat_blk, dlon_blk, m_r, m_lat, m_lon, two_pi = 4 * asin(1.);

		int NV_blk = nr * nlat, ilon1 = -1;

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
				data[i] = modelReader->getVariableAtIndex(variableID, indices[i]);
			}

		} else
		{

			for (int i = 0; i < 8; i++)
			{
				if (indices[i] > vData->size()-1)
					return missingValue;
				data[i] = (*vData)[indices[i]];
			}

		}

		/* printf("DEBUG\tdata 1..8 = %g %g %g %g %g %g %g this should be data_8-->%g<--\n", data_1, data_2, data_3, data_4, data_5, data_6, data_7, data_8 ); */

		value = (1 - m_lon) * ((1 - m_lat) * ((1 - m_r) * data[0] + m_r * data[1]) + m_lat * (+(1 - m_r) * data[2] + m_r
				* data[3])) + m_lon * ((1 - m_lat) * (+(1 - m_r) * data[4] + m_r * data[5]) + m_lat * (+(1 - m_r) * data[6] + m_r
				* data[7]));

		//dr *= ccmc::constants::AU_in_meters;

		return (value);

	}
}
