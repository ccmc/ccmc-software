#include "KameleonInterpolator.h"
#define _USE_MATH_DEFINES
#include <math.h>
#include <string>

namespace ccmc
{

	/**
	 * @param variable
	 * @param positionComponent1 The first component of the position
	 * @param positionComponent2 The second component of the position
	 * @param positionComponent3 The third component of the position
	 * @param missingValue The value that should be returned if the interpolation fails.
	 * @param dComponent1 The delta of the block size for component1 at the position specified.
	 * @param dComponent2 The delta of the block size for component2 at the position specified.
	 * @param dComponent3 The delta of the block size for component3 at the position specified.
	 * @return
	 */
	float KameleonInterpolator::compute_beta(const std::string& variable, const float& positionComponent1,
			const float& positionComponent2, const float& positionComponent3, float& dComponent1, float& dComponent2,
			float& dComponent3)
	{
		float interp_value = 0.0;
		float mu_0 = 4.e-7 * M_PI;

		float bComponent1;
		float bComponent2;
		float bComponent3;
		float p;
		float missingValue = this->modelReader->getMissingValue();
		if (modelName == "mas")
		{
			bComponent1 = interpolate("br", positionComponent1, positionComponent2, positionComponent3, dComponent1,
					dComponent2, dComponent3);
			if (bComponent1 == missingValue)
				return missingValue;

			bComponent3 = interpolate("btheta", positionComponent1, positionComponent2, positionComponent3,
					dComponent1, dComponent2, dComponent3);
			if (bComponent3 == missingValue)
				return missingValue;

			bComponent2 = interpolate("bphi", positionComponent1, positionComponent2, positionComponent3, dComponent1,
					dComponent2, dComponent3);
			if (bComponent2 == missingValue)
				return missingValue;

		} else
		{
			bComponent1 = interpolate("bx", positionComponent1, positionComponent2, positionComponent3, dComponent1,
					dComponent2, dComponent3);
			if (bComponent1 == missingValue)
				return missingValue;

			bComponent2 = interpolate("by", positionComponent1, positionComponent2, positionComponent3, dComponent1,
					dComponent2, dComponent3);
			if (bComponent2 == missingValue)
				return missingValue;

			bComponent3 = interpolate("bz", positionComponent1, positionComponent2, positionComponent3, dComponent1,
					dComponent2, dComponent3);
			if (bComponent3 == missingValue)
				return missingValue;

		}
		p = interpolate("p", positionComponent1, positionComponent2, positionComponent3, dComponent1, dComponent2,
				dComponent3);
		if (p == missingValue)
			return missingValue;

		float numerator;
		//if (modelName == "mas")
		//	numerator = (2.0 * 8.854e-12 * p);
		//else
		numerator = (2.0 * mu_0 * p);

		float denominator = (bComponent1 * bComponent1 + bComponent2 * bComponent2 + bComponent3 * bComponent3);

		if (denominator <= 0.0)
			denominator = 1.e-5;
		interp_value = numerator / denominator;
		//if (interp_value > 500)
		//{
		//	cout << "beta = " << interp_value << " at position " << positionComponent1 << " " << positionComponent2 << " " << positionComponent3 <<  " bx=" << bComponent1;
		//	cout << " by=" << bComponent2 << " bz=" << bComponent3 << " p=" << p << endl;
		//}

		return interp_value;

	}

	/**
	 * @param variable
	 * @param positionComponent1 The first component of the position
	 * @param positionComponent2 The second component of the position
	 * @param positionComponent3 The third component of the position
	 * @return
	 */
	float KameleonInterpolator::compute_beta(const std::string& variable, const float& positionComponent1,
			const float& positionComponent2, const float& positionComponent3)
	{
		float dComponent1, dComponent2, dComponent3;
		return compute_beta(variable, positionComponent1, positionComponent2, positionComponent3, dComponent1,
				dComponent2, dComponent3);
	}
}
