/*
 * ENLILInterpolator.h
 *
 *  Created on: Jul 9, 2009
 *      Author: dberrios
 */

#ifndef ENLILINTERPOLATOR_H_
#define ENLILINTERPOLATOR_H_

#include "Interpolator.h"
#include "Model.h"

namespace ccmc
{
	/**
	 * @class ENLILInterpolator ENLILInterpolator.h ccmc/ENLILInterpolator.h
	 * @brief TODO: Brief description of ENLILInterpolator class
	 *
	 * TODO: Full description of ENLILInteprolator class
	 */
	class ENLILInterpolator: public Interpolator
	{
		public:
			ENLILInterpolator(Model * model);
			float interpolate(const std::string&, const float&, const float&, const float&);
			float interpolate(const std::string&, const float&, const float&, const float&, float&, float&, float&);
			float interpolate(long int, const float&, const float&, const float&);
			float interpolate(long int, const float&, const float&, const float&, float&, float&, float&);

			virtual ~ENLILInterpolator();
	};
}

#endif /* ENLILINTERPOLATOR_H_ */
