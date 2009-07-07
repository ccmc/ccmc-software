/*
 * Interpolator.h
 *
 *  Created on: Jun 30, 2009
 *      Author: dberrios
 */

#ifndef INTERPOLATOR_H_
#define INTERPOLATOR_H_
#include "Model.h"

class Model;
class Interpolator
{

	public:
		Interpolator();
		virtual float interpolate(const std::string& variable, const float& c0, const float& c1, const float& c2) = 0;
		virtual float interpolate(const std::string& variable, const float& c0, const float& c1, const float& c2, float& dc0,
				float& dc1, float& dc2) = 0;
		virtual float interpolate(long variable_id, const float & c0, const float& c1, const float& c2) = 0;
		virtual float interpolate(long variable_id, const float& c0, const float& c1, const float& c2, float& dc0,
				float& dc1, float& dc2) = 0;
		void setMissingValue(float missingValue);
		virtual ~Interpolator();

	protected:
		Model * modelReader;
		float missingValue;
};

#endif /* INTERPOLATOR_H_ */
