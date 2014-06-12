#include "KameleonInterpolator.h"
#include "StringConstants.h"
#include <string>

namespace ccmc
{
	using namespace ccmc::strings::variables;
	using namespace ccmc::strings::models;
	/**
	 * @param variable
	 * @param c0 The first component of the position
	 * @param c1 The second component of the position
	 * @param c2 The third component of the position
	 * @param dc0 The delta of the block size for component1 at the position specified.
	 * @param dc1 The delta of the block size for component2 at the position specified.
	 * @param dc2 The delta of the block size for component3 at the position specified.
	 * @return
	 */
	float KameleonInterpolator::compute_pram(const std::string& variable, const float& c0, const float& c1,
			const float& c2, float& dc0, float& dc1, float& dc2)
	{
		//	std::cout << "KameleonInterpolator::compute_pram" << std::endl;
		//float n_to_rho = 1.672e-27;
		float n, v0, v1, v2, p_factor, interp_value;
		n = interpolate(n_, c0, c1, c2, dc0, dc1, dc2);
		//std::cout << "after interpolating n" << std::endl;
		std::string component0, component1, component2;
		float missingValue = this->modelReader->getMissingValue();

		if (modelName == mas_)
		{
			v0 = interpolate("ur", c0, c1, c2, dc0, dc1, dc2);
			if (v0 == missingValue)
				return missingValue;

			v1 = interpolate("uphi", c0, c1, c2, dc0, dc1, dc2);
			if (v1 == missingValue)
				return missingValue;

			v2 = interpolate("utheta", c0, c1, c2, dc0, dc1, dc2);
			if (v2 == missingValue)
				return missingValue;
		} else
		{
			v0 = interpolate(ux_, c0, c1, c2, dc0, dc1, dc2);
			if (v0 == missingValue)
				return missingValue;

			v1 = interpolate(uy_, c0, c1, c2, dc0, dc1, dc2);
			if (v1 == missingValue)
				return missingValue;

			v2 = interpolate(uz_, c0, c1, c2, dc0, dc1, dc2);
			if (v2 == missingValue)
				return missingValue;

		}

		/*float p_factor = n_to_rho*1e21;
		 p_factor = 1.627e-6;*/
		interp_value = (n / 2.0 * (v0 * v0 + v1 * v1 + v2 * v2));

		return interp_value;
	}

	/**
	 * @param variable
	 * @param c0 The first component of the position
	 * @param c1 The second component of the position
	 * @param c2 The third component of the position
	 * @return
	 */
	float KameleonInterpolator::compute_pram(const std::string& variable, const float& c0, const float& c1,
			const float& c2)
	{
		float dc0, dc1, dc2;
		return compute_pram(variable, c0, c1, c2, dc0, dc1, dc2);
	}
}
