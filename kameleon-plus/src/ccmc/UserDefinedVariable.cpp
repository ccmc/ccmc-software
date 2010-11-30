/*
 * UserDefinedVariable.cpp
 *
 *  Created on: Nov 30, 2010
 *      Author: dberrios
 */

#include "UserDefinedVariable.h"

namespace ccmc
{
UserDefinedVariable::UserDefinedVariable()
{
	// TODO Auto-generated constructor stub

}

float UserDefinedVariable::calculate(Interpolator * interpolator,
		const float& c0, const float& c1, const float& c2)
{
	return 0.;
}
float UserDefinedVariable::calculate(Interpolator * interpolator,
		const float& c0, const float& c1, const float& c2, float& d0, float& d1, float& d2)
{
	return 0.;
}

UserDefinedVariable::~UserDefinedVariable()
{
	// TODO Auto-generated destructor stub
}
}
