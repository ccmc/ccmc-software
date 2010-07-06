/*
 * MASInterpolator.h
 *
 *  Created on: Jul 9, 2009
 *      Author: dberrios
 */

#ifndef MASINTERPOLATOR_H_
#define MASINTERPOLATOR_H_

#include "Interpolator.h"
#include "Model.h"

namespace ccmc
{
	/**
	 * @class MASInterpolator MASInterpolator.h ccmc/MASInterpolator.h
	 * @brief TODO: Brief description of MASInterpolator class
	 *
	 * TODO: Full description of MASInteprolator class
	 */
	class MASInterpolator: public Interpolator
	{
		public:
			MASInterpolator(Model * model);
			float interpolate(const std::string& variable, const float& r, const float& lat, const float& lon);
			float interpolate(const std::string& variable, const float& r, const float& lat, const float& lon, float& dr, float& dlat, float& dlon);
			float interpolate(long int variableID, const float& r, const float& lat, const float& lon);
			float interpolate(long int variableID, const float& r, const float& lat, const float& lon, float& dr, float& dlat, float& dlon);
			virtual ~MASInterpolator();

	};
}
#endif /* MASINTERPOLATOR_H_ */
