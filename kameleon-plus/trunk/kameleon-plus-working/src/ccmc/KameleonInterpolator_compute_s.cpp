#include "KameleonInterpolator.h"
#include <math.h>
#include <string>

namespace ccmc
{
	/**
	 * @param variable
	 * @param positionComponent1 The first component of the position
	 * @param positionComponent2 The second component of the position
	 * @param positionComponent3 The third component of the position
	 * @return
	 */
	float KameleonInterpolator::compute_s(const std::string& variable, const float& positionComponent1,
			const float& positionComponent2, const float& positionComponent3)
	{
		float dComponent1, dComponent2, dComponent3;
		return compute_s(variable, positionComponent1, positionComponent2, positionComponent3, dComponent1,
				dComponent2, dComponent3);
	}

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
	float KameleonInterpolator::compute_s(const std::string& variable, const float& positionComponent1,
			const float& positionComponent2, const float& positionComponent3, float& dComponent1, float& dComponent2,
			float& dComponent3)
	{
		float t = 0.0;
		float n = 0.0;
		float missingValue = this->modelReader->getMissingValue();


		t = interpolate("t", positionComponent1, positionComponent2, positionComponent3, dComponent1, dComponent2,
				dComponent3);
		if (t == missingValue)
			return missingValue;
		n = interpolate("n", positionComponent1, positionComponent2, positionComponent3, dComponent1, dComponent2,
				dComponent3);
		if (n == missingValue)
			return missingValue;
		float s = t / pow(n, 2.0f / 3.0f);
		return s;

	}
}
