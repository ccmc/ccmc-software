#include "KameleonInterpolator.h"
#include <string>

namespace ccmc
{
	/**
	 * Calculates and returns the polarity of B
	 * @param variable this is not used.  variable is only a parameter to maintain consistency with the other methods for use in calculationMethods
	 * @param positionComponent1 The first component of the position
	 * @param positionComponent2 The second component of the position
	 * @param positionComponent3 The third component of the position
	 * @return The polarity of B
	 */
	float KameleonInterpolator::compute_polb(const std::string& variable, const float& positionComponent1,
			const float& positionComponent2, const float& positionComponent3)
	{
		float dComponent1, dComponent2, dComponent3;
		return compute_polb(variable, positionComponent1, positionComponent2, positionComponent3, dComponent1,
				dComponent2, dComponent3);

	}

	/**
	 * Calculates the polarity of the magnetic field
	 * @param variable this is not used.  variable is only a parameter to maintain consistency with the other methods for use in calculationMethods
	 * @param positionComponent1 The first component of the position
	 * @param positionComponent2 The second component of the position
	 * @param positionComponent3 The third component of the position
	 * @param missingValue  The value that should be returned if the interpolation fails.
	 * @param dComponent1
	 * @param dComponent2
	 * @param dComponent3
	 * @return The polarity of B
	 *
	 */
	float KameleonInterpolator::compute_polb(const std::string& variable, const float& positionComponent1,
			const float& positionComponent2, const float& positionComponent3, float& dComponent1, float& dComponent2,
			float& dComponent3)
	{
		std::string bp_ = "bp";
		float interp_value = interpolate(bp_, positionComponent1, positionComponent2, positionComponent3, dComponent1,
				dComponent2, dComponent3);
		if (interp_value < 0.0)
		{
			interp_value = 1.0;
		} else if (interp_value > 0.0)
		{
			interp_value = -1.0;
		} else
			interp_value = 0.0;
		return interp_value;
	}
}
