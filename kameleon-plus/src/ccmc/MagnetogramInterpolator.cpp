/*
 * MagnetogramInterpolator.cpp
 *
 *  Created on: Sep 23, 2009
 *      Author: dberrios
 */

#include "MagnetogramInterpolator.h"

namespace ccmc
{
	MagnetogramInterpolator::MagnetogramInterpolator(Model * model)
	{
		// TODO Auto-generated constructor stub

	}

	float MagnetogramInterpolator::interpolate(const std::string&, const float&, const float&, const float&)
	{
		return 0.f;
	}

	float MagnetogramInterpolator::interpolate(const std::string&, const float&, const float&, const float&, float&, float&,
			float&)
	{
		return 0.f;
	}

	float MagnetogramInterpolator::interpolate(long int, const float&, const float&, const float&)
	{
		return 0.f;
	}

	float MagnetogramInterpolator::interpolate(long int, const float&, const float&, const float&, float&, float&, float&)
	{
		return 0.f;
	}

	MagnetogramInterpolator::~MagnetogramInterpolator()
	{
		// TODO Auto-generated destructor stub
	}
}
