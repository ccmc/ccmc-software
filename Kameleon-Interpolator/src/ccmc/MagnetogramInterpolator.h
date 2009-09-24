/*
 * MagnetogramInterpolator.h
 *
 *  Created on: Sep 23, 2009
 *      Author: dberrios
 */

#ifndef MAGNETOGRAMINTERPOLATOR_H_
#define MAGNETOGRAMINTERPOLATOR_H_

#include "Interpolator.h"
#include "Model.h"

namespace ccmc
{
	/**
	 * @class MagnetogramInterpolator MagnetogramInterpolator.h ccmc/MagnetogramInterpolator.h
	 * @brief TODO: Brief description of MagnetogramInterpolator class
	 *
	 * TODO: Full description of MagnetogramInteprolator class
	 */
	class MagnetogramInterpolator: public Interpolator
	{
		public:
			MagnetogramInterpolator(Model * model);
			float interpolate(const std::string&, const float&, const float&, const float&);
			float interpolate(const std::string&, const float&, const float&, const float&, float&, float&, float&);
			float interpolate(long int, const float&, const float&, const float&);
			float interpolate(long int, const float&, const float&, const float&, float&, float&, float&);

			virtual ~MagnetogramInterpolator();
	};
}

#endif /* MAGNETOGRAMINTERPOLATOR_H_ */
