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
	float KameleonInterpolator::compute_n(const std::string& variable, const float& positionComponent1,
			const float& positionComponent2, const float& positionComponent3, float& dComponent1, float& dComponent2,
			float& dComponent3)
	{
		/*double n;

		 if (modelName == "enlil")
		 {
		 n = (double) interpolate("rho", positionComponent1, positionComponent2, positionComponent3,
		 missing, dComponent1, dComponent2, dComponent3);
		 n = n / 1e6;
		 n = n * 6.0221415e26;
		 } else if (modelName == "mas")
		 {
		 float rho_conversion = 1.e8;
		 n = (double) interpolate("rho", positionComponent1, positionComponent2, positionComponent3,
		 missing, dComponent1, dComponent2, dComponent3);
		 n = n * rho_conversion;

		 }
		 else
		 n = (double) interpolate("rho", positionComponent1, positionComponent2, positionComponent3,
		 missing, dComponent1, dComponent2, dComponent3);
		 */
		//std::cout << "KameleonInterpolator::compute_n." << std::endl;
		float n = interpolate(rho_, positionComponent1, positionComponent2, positionComponent3, dComponent1,
				dComponent2, dComponent3);
		return n;

	}

	/**
	 * @param variable
	 * @param positionComponent1 The first component of the position
	 * @param positionComponent2 The second component of the position
	 * @param positionComponent3 The third component of the position
	 * @return
	 */
	float KameleonInterpolator::compute_n(const std::string& variable, const float& positionComponent1,
			const float& positionComponent2, const float& positionComponent3)
	{
		float dComponent1, dComponent2, dComponent3;
		return compute_n(variable, positionComponent1, positionComponent2, positionComponent3, dComponent1,
				dComponent2, dComponent3);
	}
}
