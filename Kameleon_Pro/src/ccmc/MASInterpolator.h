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

#endif /* MASINTERPOLATOR_H_ */
