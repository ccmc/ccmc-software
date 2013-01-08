#include "KameleonInterpolator.h"
#include "StringConstants.h"
#include <math.h>
#include <string>

namespace ccmc
{
	using namespace ccmc::strings::variables;
	/**
	 * @param variable
	 * @param c0 The first component of the position
	 * @param c1 The second component of the position
	 * @param c2 The third component of the position
	 * @param missingValue The value that should be returned if the interpolation fails.
	 * @param dc0 The delta of the block size for component1 at the position specified.
	 * @param dc1 The delta of the block size for component2 at the position specified.
	 * @param dc2 The delta of the block size for component3 at the position specified.
	 * @return
	 */
	float KameleonInterpolator::compute_jxbComponent1(const std::string& variable, const float& c0, const float& c1,
			const float& c2, float& dc0, float& dc1, float& dc2)
	{
		//J_y*B_z-J_z*B_y
		float value;
		float b_y, b_z, j_y, j_z;
		float missingValue = this->modelReader->getMissingValue();

		b_y = interpolate(by_, c0, c1, c2, dc0, dc1, dc2);
		if (b_y == missingValue)
			return missingValue;
		b_z = interpolate(bz_, c0, c1, c2, dc0, dc1, dc2);
		if (b_z == missingValue)
			return missingValue;
		j_y = interpolate("jy", c0, c1, c2, dc0, dc1, dc2);
		if (j_y == missingValue)
			return missingValue;
		j_z = interpolate("jz", c0, c1, c2, dc0, dc1, dc2);
		if (j_z == missingValue)
			return missingValue;

		value = j_y * b_z - j_z * b_y;
		return value;

	}

	/**
	 * @param variable
	 * @param c0 The first component of the position
	 * @param c1 The second component of the position
	 * @param c2 The third component of the position
	 * @return
	 */
	float KameleonInterpolator::compute_jxbComponent1(const std::string& variable, const float& c0, const float& c1,
			const float& c2)
	{
		float dc0, dc1, dc2;
		return compute_jxbComponent1(variable, c0, c1, c2, dc0, dc1, dc2);
	}

	/**
	 * @param variable
	 * @param c0 The first component of the position
	 * @param c1 The second component of the position
	 * @param c2 The third component of the position
	 * @param missingValue The value that should be returned if the interpolation fails.
	 * @param dc0 The delta of the block size for component1 at the position specified.
	 * @param dc1 The delta of the block size for component2 at the position specified.
	 * @param dc2 The delta of the block size for component3 at the position specified.
	 * @return
	 */
	float KameleonInterpolator::compute_jxb(const std::string& variable, const float& c0, const float& c1,
			const float& c2, float& dc0, float& dc1, float& dc2)
	{

		float value, value_x, value_y, value_z;
		float b_x, b_y, b_z, j_x, j_y, j_z;
		float missingValue = this->modelReader->getMissingValue();

		b_x = interpolate(bx_, c0, c1, c2, dc0, dc1, dc2);
		if (b_x == missingValue)
			return missingValue;
		b_y = interpolate(by_, c0, c1, c2, dc0, dc1, dc2);
		if (b_y == missingValue)
			return missingValue;
		b_z = interpolate(bz_, c0, c1, c2, dc0, dc1, dc2);
		if (b_z == missingValue)
			return missingValue;
		j_x = interpolate("jx", c0, c1, c2, dc0, dc1, dc2);
		if (j_x == missingValue)
			return missingValue;
		j_y = interpolate("jy", c0, c1, c2, dc0, dc1, dc2);
		if (j_y == missingValue)
			return missingValue;
		j_z = interpolate("jz", c0, c1, c2, dc0, dc1, dc2);
		if (j_z == missingValue)
			return missingValue;

		value_x = j_y * b_z - j_z * b_y;
		value_y = j_z * b_x - j_x * b_z;
		value_z = j_x * b_y - j_y * b_x;
		value = sqrt(value_x * value_x + value_y * value_y + value_z * value_z);
		return value;

	}

	/**
	 * @param variable
	 * @param c0 The first component of the position
	 * @param c1 The second component of the position
	 * @param c2 The third component of the position
	 * @return
	 */
	float KameleonInterpolator::compute_jxb(const std::string& variable, const float& c0, const float& c1,
			const float& c2)
	{
		float dc0, dc1, dc2;
		return compute_jxb(variable, c0, c1, c2, dc0, dc1, dc2);
	}
	/**
	 * @param variable
	 * @param c0 The first component of the position
	 * @param c1 The second component of the position
	 * @param c2 The third component of the position
	 * @param missingValue The value that should be returned if the interpolation fails.
	 * @param dc0 The delta of the block size for component1 at the position specified.
	 * @param dc1 The delta of the block size for component2 at the position specified.
	 * @param dc2 The delta of the block size for component3 at the position specified.
	 * @return
	 */
	float KameleonInterpolator::compute_jxby(const std::string& variable, const float& c0, const float& c1,
			const float& c2, float& dc0, float& dc1, float& dc2)
	{
		//J_z*B_x-J_x*B_z
		float value, b_x, b_z, j_x, j_z;
		float missingValue = this->modelReader->getMissingValue();

		b_x = interpolate(bx_, c0, c1, c2, dc0, dc1, dc2);
		if (b_x == missingValue)
			return missingValue;
		b_z = interpolate(bz_, c0, c1, c2, dc0, dc1, dc2);
		if (b_z == missingValue)
			return missingValue;
		j_x = interpolate("jx", c0, c1, c2, dc0, dc1, dc2);
		if (j_x == missingValue)
			return missingValue;
		j_z = interpolate("jz", c0, c1, c2, dc0, dc1, dc2);
		if (j_z == missingValue)
			return missingValue;

		value = j_z * b_x - j_x * b_z;
		return value;

	}

	/**
	 * @param variable
	 * @param c0 The first component of the position
	 * @param c1 The second component of the position
	 * @param c2 The third component of the position
	 * @param missingValue The value that should be returned if the interpolation fails.
	 * @param dc0 The delta of the block size for component1 at the position specified.
	 * @param dc1 The delta of the block size for component2 at the position specified.
	 * @param dc2 The delta of the block size for component3 at the position specified.
	 * @return
	 */
	float KameleonInterpolator::compute_jxbz(const std::string& variable, const float& c0, const float& c1,
			const float& c2, float& dc0, float& dc1, float& dc2)
	{
		//J_x*B_y-J_y*B_x
		float value, b_x, b_y, j_x, j_y;
		float missingValue = this->modelReader->getMissingValue();

		b_x = interpolate(bx_, c0, c1, c2, dc0, dc1, dc2);
		if (b_x == missingValue)
			return missingValue;
		b_y = interpolate(by_, c0, c1, c2, dc0, dc1, dc2);
		if (b_y == missingValue)
			return missingValue;
		j_x = interpolate("jx", c0, c1, c2, dc0, dc1, dc2);
		if (j_x == missingValue)
			return missingValue;
		j_y = interpolate("jy", c0, c1, c2, dc0, dc1, dc2);
		if (j_y == missingValue)
			return missingValue;

		value = j_x * b_y - j_y * b_x;
		return value;

	}

	/**
	 * @param variable
	 * @param c0 The first component of the position
	 * @param c1 The second component of the position
	 * @param c2 The third component of the position
	 * @return
	 */
	float KameleonInterpolator::compute_jxby(const std::string& variable, const float& c0, const float& c1,
			const float& c2)
	{
		float dc0, dc1, dc2;
		return compute_jxby(variable, c0, c1, c2, dc0, dc1, dc2);
	}

	/**
	 * @param variable
	 * @param c0 The first component of the position
	 * @param c1 The second component of the position
	 * @param c2 The third component of the position
	 * @return
	 */
	float KameleonInterpolator::compute_jxbz(const std::string& variable, const float& c0, const float& c1,
			const float& c2)
	{
		float dc0, dc1, dc2;
		return compute_jxbz(variable, c0, c1, c2, dc0, dc1, dc2);
	}
}
