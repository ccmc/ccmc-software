/*
 * UserDefinedVariable.h
 *
 *  Created on: Nov 30, 2010
 *      Author: dberrios
 */

#ifndef USERDEFINEDVARIABLE_H_
#define USERDEFINEDVARIABLE_H_
#include <vector>
#include "Interpolator.h"
/*
 *
 */
#include "Variable.h"

namespace ccmc
{
	class UserDefinedVariable: public Variable
	{
		public:
			UserDefinedVariable();

			float calculate(Interpolator * interpolator,
					const float& c0, const float& c1, const float& c2);
			float calculate(Interpolator * interpolator,
					const float& c0, const float& c1, const float& c2, float& d0, float& d1, float& d2);
			virtual ~UserDefinedVariable();
	};
}
#endif /* USERDEFINEDVARIABLE_H_ */
