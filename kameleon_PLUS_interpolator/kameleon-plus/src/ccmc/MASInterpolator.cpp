/*
 * MASInterpolator.cpp
 *
 *  Created on: Jul 9, 2009
 *      Author: dberrios
 */

#include "MASInterpolator.h"
#include "StringConstants.h"

namespace ccmc
{
	/**
	 * @param model
	 */
	MASInterpolator::MASInterpolator(Model * model)
	{
		// TODO Auto-generated constructor stub
		// TODO Auto-generated constructor stub
		this->modelReader = model;
		this->setMissingValue(this->modelReader->getMissingValue());
		//the model open should have done the proper error checks, so we
		//just check the first component name to see which set to use
		//if (this->modelReader->doesVariableExist("r"))
		{
			r_string = strings::variables::r_;
			r1_string = strings::variables::r1_;
			lat_string = strings::variables::theta_;
			lon_string = strings::variables::phi_;
			lat1_string = strings::variables::theta1_;


		}

		//TODO: fix the phi/theta issue to correspond to the actual
		//lat lon
		r_data = modelReader->getVariableData(r_string);
		r1_data = modelReader->getVariableData(r1_string);
		lat_data = modelReader->getVariableData(lat_string);
		lon_data = modelReader->getVariableData(lon_string);
		lat1_data = modelReader->getVariableData(lat1_string);
		nr = (*r_data).size();
		nlat = (*lat_data).size();
		nlon = (*lon_data).size();

		this->nr_plus1 = (*r1_data).size();
		this->nlat_plus1 = (*lat1_data).size();
		this->nlon_plus1 = nlon;
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
		return 0.f;
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
		return 0.f;
	}

	/**
	 * @param variableID
	 * @param r
	 * @param lat
	 * @param lon
	 * @return
	 */
	float MASInterpolator::interpolate(long int variableID, const float& r, const float& lat, const float& lon)
	{
		return 0.f;
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
	float MASInterpolator::interpolate(long int variableID, const float& r, const float& lat, const float& lon, float& dr, float& dlat, float& dlon)
	{


	}

	/**
	 *
	 */
	MASInterpolator::~MASInterpolator()
	{
		// TODO Auto-generated destructor stub
	}
}
