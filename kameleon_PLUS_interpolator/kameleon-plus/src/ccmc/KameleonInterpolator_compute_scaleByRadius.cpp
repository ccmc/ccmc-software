#include "KameleonInterpolator.h"
#include <sstream>
#include <string>

namespace ccmc
{
	/**
	 * Calculates the variable times the radius to a power specified by the variable string.  For example, "n*r^2" represents the variable "n" scaled by
	 * the radius to the second power.
	 * @param variable Variable to interpolate and scale
	 * @param positionComponent1 The first component of the position
	 * @param positionComponent2 The second component of the position
	 * @param positionComponent3 The third component of the position
	 * @return The interpolated value prescaled by the radius to the specified power.
	 *
	 */
	float KameleonInterpolator::compute_scaleByRadius(const std::string& variable, const float& positionComponent1,
			const float& positionComponent2, const float& positionComponent3)
	{
		float dComponent1, dComponent2, dComponent3;
		return compute_scaleByRadius(variable, positionComponent1, positionComponent2, positionComponent3, dComponent1,
				dComponent2, dComponent3);

	}

	/**
	 * Calculates the variable times the radius to a power specified by the variable string.  For example, "n*r^2" represents the variable "n" scaled by
	 * the radius to the second power.
	 * @param variable Variable to interpolate and scale
	 * @param positionComponent1 The first component of the position
	 * @param positionComponent2 The second component of the position
	 * @param positionComponent3 The third component of the position
	 * @param dComponent1 The delta of the block size for component1 at the position specified.
	 * @param dComponent2 The delta of the block size for component2 at the position specified.
	 * @param dComponent3 The delta of the block size for component3 at the position specified.
	 * @return The interpolated value prescaled by the radius to the specified power.
	 *
	 */
	float KameleonInterpolator::compute_scaleByRadius(const std::string& variable, const float& positionComponent1,
			const float& positionComponent2, const float& positionComponent3, float& dComponent1, float& dComponent2,
			float& dComponent3)
	{
		float missingValue = this->modelReader->getMissingValue();

		//tokenize variable
		//interpolate base variable
		//multiply the value returned by the parsed power of the radius
		//variable must be in the form variable*r^power
		std::string variable_string = variable;
		int position = variable_string.find("*");
		std::string baseVariable = variable_string.substr(0, position);
		float interp_value = interpolate((std::string) baseVariable.c_str(), positionComponent1, positionComponent2,
				positionComponent3, dComponent1, dComponent2, dComponent3);
		if (interp_value == missingValue)
			return missingValue;

		position = variable_string.find("^");
		std::string powerString = variable_string.substr(position + 1, variable_string.length());
		int power;
		std::istringstream iss(powerString);
		iss >> power;

		for (int i = 0; i < power; i++)
		{
			interp_value *= positionComponent1;
		}

		return interp_value;
	}
}
