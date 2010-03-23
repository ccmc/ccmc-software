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
			float interpolate(const std::string&, const float&, const float&, const float&);
			float interpolate(const std::string&, const float&, const float&, const float&, float&, float&, float&);
			float interpolate(long int, const float&, const float&, const float&);
			float interpolate(long int, const float&, const float&, const float&, float&, float&, float&);
			virtual ~MASInterpolator();

	};
}
#endif /* MASINTERPOLATOR_H_ */
