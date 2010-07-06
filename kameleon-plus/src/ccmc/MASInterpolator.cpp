/*
 * MASInterpolator.cpp
 *
 *  Created on: Jul 9, 2009
 *      Author: dberrios
 */

#include "MASInterpolator.h"

namespace ccmc
{
	/**
	 * @param model
	 */
	MASInterpolator::MASInterpolator(Model * model)
	{
		// TODO Auto-generated constructor stub

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
		return 0.f;
	}

	/**
	 *
	 */
	MASInterpolator::~MASInterpolator()
	{
		// TODO Auto-generated destructor stub
	}
}
