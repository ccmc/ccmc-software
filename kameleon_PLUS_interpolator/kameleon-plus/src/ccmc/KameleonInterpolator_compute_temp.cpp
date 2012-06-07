#include "KameleonInterpolator.h"
#include <string>

namespace ccmc
{
	/**
	 * @param variable
	 * @param positionComponent1 The first component of the position
	 * @param positionComponent2 The second component of the position
	 * @param positionComponent3 The third component of the position
	 * @param dComponent1 The delta of the block size for component1 at the position specified.
	 * @param dComponent2 The delta of the block size for component2 at the position specified.
	 * @param dComponent3 The delta of the block size for component3 at the position specified.
	 * @return
	 */
	float KameleonInterpolator::compute_temp(const std::string& variable, const float& positionComponent1,
			const float& positionComponent2, const float& positionComponent3, float& dComponent1, float& dComponent2,
			float& dComponent3)
	{

		float interp_value = 0.0;
		float missingValue = this->modelReader->getMissingValue();

		if (modelReader->doesVariableExist("T"))
		{
			interp_value = interpolateSimple("T", positionComponent1, positionComponent2, positionComponent3,
					dComponent1, dComponent2, dComponent3);
		} else
		{
			float p = interpolate("p", positionComponent1, positionComponent2, positionComponent3, dComponent1,
					dComponent2, dComponent3);
			if (p == missingValue)
				return missingValue;
			float rho = interpolate("rho", positionComponent1, positionComponent2, positionComponent3, dComponent1,
					dComponent2, dComponent3);
			if (rho == missingValue)
				return missingValue;
			interp_value = p / rho;
		}
		return interp_value;

	}

	/**
	 * @param variable
	 * @param positionComponent1 The first component of the position
	 * @param positionComponent2 The second component of the position
	 * @param positionComponent3 The third component of the position
	 * @return
	 */
	float KameleonInterpolator::compute_temp(const std::string& variable, const float& positionComponent1,
			const float& positionComponent2, const float& positionComponent3)
	{
		float dComponent1, dComponent2, dComponent3;
		return compute_temp(variable, positionComponent1, positionComponent2, positionComponent3, dComponent1,
				dComponent2, dComponent3);
	}

}
