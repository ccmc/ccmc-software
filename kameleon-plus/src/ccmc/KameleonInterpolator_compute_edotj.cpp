#include "KameleonInterpolator.h"
#include "StringConstants.h"
#include <string>

namespace ccmc
{
	using namespace ccmc::strings::variables;
	using namespace ccmc::strings::models;
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
	float KameleonInterpolator::compute_edotj(const std::string& variable, const float& positionComponent1,
			const float& positionComponent2, const float& positionComponent3, float& dComponent1, float& dComponent2,
			float& dComponent3)
	{
		float missingValue = this->modelReader->getMissingValue();

		float j_x, j_y, j_z, e_x, e_y, e_z;
		if (modelName == mas_)
		{
			j_x = interpolate("jr", positionComponent1, positionComponent2, positionComponent3, dComponent1,
					dComponent2, dComponent3);
			if (j_x == missingValue)
				return missingValue;
			j_y = interpolate("jtheta", positionComponent1, positionComponent2, positionComponent3, dComponent1,
					dComponent2, dComponent3);
			if (j_y == missingValue)
				return missingValue;
			j_z = interpolate("jphi", positionComponent1, positionComponent2, positionComponent3, dComponent1,
					dComponent2, dComponent3);
			if (j_z == missingValue)
				return missingValue;
			e_x = interpolate("er", positionComponent1, positionComponent2, positionComponent3, dComponent1,
					dComponent2, dComponent3);
			if (e_x == missingValue)
				return missingValue;
			e_y = interpolate("etheta", positionComponent1, positionComponent2, positionComponent3, dComponent1,
					dComponent2, dComponent3);
			if (e_y == missingValue)
				return missingValue;
			e_z = interpolate("ephi", positionComponent1, positionComponent2, positionComponent3, dComponent1,
					dComponent2, dComponent3);
			if (e_z == missingValue)
				return missingValue;
		} else
		{
			j_x = interpolate(jx_, positionComponent1, positionComponent2, positionComponent3, dComponent1,
					dComponent2, dComponent3);
			if (j_x == missingValue)
				return missingValue;
			j_y = interpolate(jy_, positionComponent1, positionComponent2, positionComponent3, dComponent1,
					dComponent2, dComponent3);
			if (j_y == missingValue)
				return missingValue;
			j_z = interpolate(jz_, positionComponent1, positionComponent2, positionComponent3, dComponent1,
					dComponent2, dComponent3);
			if (j_z == missingValue)
				return missingValue;
			e_x = interpolate(ex_, positionComponent1, positionComponent2, positionComponent3, dComponent1,
					dComponent2, dComponent3);
			if (e_x == missingValue)
				return missingValue;
			e_y = interpolate(ey_, positionComponent1, positionComponent2, positionComponent3, dComponent1,
					dComponent2, dComponent3);
			if (e_y == missingValue)
				return missingValue;
			e_z = interpolate(ez_, positionComponent1, positionComponent2, positionComponent3, dComponent1,
					dComponent2, dComponent3);
			if (e_z == missingValue)
				return missingValue;
		}

		return e_x * j_x + e_y * j_y + e_z * j_z;

	}

	/**
	 * @param variable
	 * @param positionComponent1 The first component of the position
	 * @param positionComponent2 The second component of the position
	 * @param positionComponent3 The third component of the position
	 * @return
	 */
	float KameleonInterpolator::compute_edotj(const std::string& variable, const float& positionComponent1,
			const float& positionComponent2, const float& positionComponent3)
	{
		float dComponent1, dComponent2, dComponent3;
		return compute_edotj(variable, positionComponent1, positionComponent2, positionComponent3, dComponent1,
				dComponent2, dComponent3);
	}
}
