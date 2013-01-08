#include "KameleonInterpolator.h"
#include "StringConstants.h"
#include <cmath>
#include <string>

namespace ccmc
{
	using namespace ccmc::strings::variables;
	using namespace ccmc::strings::models;

	/**
	 * Computes the magnitude of the vector specified.  The vector magnitude does not directly exists in the data,
	 * and must be calculated.  Most vector magnitudes of derived variables have specific methods to calculate
	 * the magnitudes of that vector, mainly to prevent duplicate interpolations.
	 *
	 * @param variable
	 * @param positionComponent1
	 * @param positionComponent2
	 * @param positionComponent3
	 * @param dComponent1
	 * @param dComponent2
	 * @param dComponent3
	 * @return The magnitude of the vector
	 */
	float KameleonInterpolator::compute_magnitude(const std::string& variable, const float& positionComponent1,
			const float& positionComponent2, const float& positionComponent3, float& dComponent1, float& dComponent2,
			float& dComponent3)
	{
		float component1, component2, component3, magnitude;
		float missingValue = this->modelReader->getMissingValue();

		//string variable_name = variable;
		std::string component1Name, component2Name, component3Name;
		if (modelName == mas_ || modelName == enlil_)
		{
			if (variable == j_)
			{
				component1Name = "jr";
				component2Name = "jtheta";
				component3Name = "jphi";
			} else if (variable == b_)
			{
				component1Name = "br";
				component2Name = "btheta";
				component3Name = "bphi";
			} else if (variable == "e")
			{
				component1Name = "er";
				component2Name = "etheta";
				component3Name = "ephi";
			} else if (variable == "u")
			{
				component1Name = "ur";
				component2Name = "utheta";
				component3Name = "uphi";
			} else if (variable == "b1")
			{
				component1Name = "b1r";
				component2Name = "b1theta";
				component3Name = "bphi";
			} else
				return missingValue;
		} else
		{
			if (variable == j_)
			{
				component1Name = "jx";
				component2Name = "jy";
				component3Name = "jz";
			} else if (variable == b_)
			{
				component1Name = bx_;
				component2Name = by_;
				component3Name = bz_;
			} else if (variable == "b1")
			{
				component1Name = "b1x";
				component2Name = "b1y";
				component3Name = "b1z";

			} else if (variable == "e")
			{
				component1Name = "ex";
				component2Name = "ey";
				component3Name = "ez";
			} else if (variable == "u")
			{
				component1Name = "ux";
				component2Name = "uy";
				component3Name = "uz";
			} else
				return missingValue;

		}

		component1 = interpolate(component1Name, positionComponent1, positionComponent2, positionComponent3,
				dComponent1, dComponent2, dComponent3);
		if (component1 == missingValue)
			return missingValue;
		component2 = interpolate(component2Name, positionComponent1, positionComponent2, positionComponent3,
				dComponent1, dComponent2, dComponent3);
		if (component2 == missingValue)
			return missingValue;
		component3 = interpolate(component3Name, positionComponent1, positionComponent2, positionComponent3,
				dComponent1, dComponent2, dComponent3);
		if (component3 == missingValue)
			return missingValue;
		magnitude = std::sqrt(component1 * component1 + component2 * component2 + component3 * component3);
		return magnitude;

	}

	/**
	 * @param variable
	 * @param positionComponent1 The first component of the position
	 * @param positionComponent2 The second component of the position
	 * @param positionComponent3 The third component of the position
	 * @return
	 */
	float KameleonInterpolator::compute_magnitude(const std::string& variable, const float& positionComponent1,
			const float& positionComponent2, const float& positionComponent3)
	{
		float dComponent1, dComponent2, dComponent3;
		return compute_magnitude(variable, positionComponent1, positionComponent2, positionComponent3, dComponent1,
				dComponent2, dComponent3);

	}
}
