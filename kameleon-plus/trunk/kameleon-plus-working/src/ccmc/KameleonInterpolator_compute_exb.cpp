#include "KameleonInterpolator.h"
#include "StringConstants.h"
#include <math.h>
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
	 * @param dComponent1 The delta of the block size for component1 at the position specified.
	 * @param dComponent2 The delta of the block size for component2 at the position specified.
	 * @param dComponent3 The delta of the block size for component3 at the position specified.
	 * @return
	 */
	float KameleonInterpolator::compute_exbComponent1(const std::string& variable, const float& c0, const float& c1,
			const float& c2, float& dComponent1, float& dComponent2, float& dComponent3)
	{
		float missingValue = this->modelReader->getMissingValue();

		//J_x*B_y-J_y*B_x
		float value;
		float mu_0 = 4e-7 * 3.141592;
		float exb_factor = 1.e-6 / mu_0;

		float e_y, e_z, b_y, b_z;
		if (modelName == mas_)
		{
			e_y = interpolate("etheta", c0, c1, c2, dComponent1, dComponent2, dComponent3);
			if (e_y == missingValue)
				return missingValue;
			e_z = interpolate("ephi", c0, c1, c2, dComponent1, dComponent2, dComponent3);
			if (e_z == missingValue)
				return missingValue;
			b_y = interpolate("btheta", c0, c1, c2, dComponent1, dComponent2, dComponent3);
			if (b_y == missingValue)
				return missingValue;
			b_z = interpolate("bphi", c0, c1, c2, dComponent1, dComponent2, dComponent3);
			if (b_z == missingValue)
				return missingValue;
		} else
		{
			e_y = interpolate(ey_, c0, c1, c2, dComponent1, dComponent2, dComponent3);
			if (e_y == missingValue)
				return missingValue;
			e_z = interpolate(ez_, c0, c1, c2, dComponent1, dComponent2, dComponent3);
			if (e_z == missingValue)
				return missingValue;
			b_y = interpolate(by_, c0, c1, c2, dComponent1, dComponent2, dComponent3);
			if (b_y == missingValue)
				return missingValue;
			b_z = interpolate(bz_, c0, c1, c2, dComponent1, dComponent2, dComponent3);
			if (b_z == missingValue)
				return missingValue;
		}
		value = e_y * b_z - e_z * b_y;
		value *= exb_factor;
		return value;

	}

	/**
	 * @param variable
	 * @param c0 The first component of the position
	 * @param c1 The second component of the position
	 * @param c2 The third component of the position
	 * @return
	 */
	float KameleonInterpolator::compute_exbComponent1(const std::string& variable, const float& c0, const float& c1,
			const float& c2)
	{
		float dComponent1, dComponent2, dComponent3;
		return compute_exbComponent1(variable, c0, c1, c2, dComponent1, dComponent2, dComponent3);
	}

	/**
	 * @param variable
	 * @param c0 The first component of the position
	 * @param c1 The second component of the position
	 * @param c2 The third component of the position
	 * @param missingValue The value that should be returned if the interpolation fails.
	 * @param dComponent1 The delta of the block size for component1 at the position specified.
	 * @param dComponent2 The delta of the block size for component2 at the position specified.
	 * @param dComponent3 The delta of the block size for component3 at the position specified.
	 * @return
	 */
	float KameleonInterpolator::compute_exby(const std::string& variable, const float& c0, const float& c1,
			const float& c2, float& dComponent1, float& dComponent2, float& dComponent3)
	{
		//J_x*B_y-J_y*B_x
		float value;
		float mu_0 = 4e-7 * 3.141592;
		float exb_factor = 1.e-6 / mu_0;
		float e_x, e_z, b_x, b_z;
		float missingValue = this->modelReader->getMissingValue();


		if (modelName == mas_)
		{
			e_x = interpolate("er", c0, c1, c2, dComponent1, dComponent2, dComponent3);
			if (e_x == missingValue)
				return missingValue;
			e_z = interpolate("ephi", c0, c1, c2, dComponent1, dComponent2, dComponent3);
			if (e_z == missingValue)
				return missingValue;
			b_x = interpolate("br", c0, c1, c2, dComponent1, dComponent2, dComponent3);
			if (b_x == missingValue)
				return missingValue;
			b_z = interpolate("bphi", c0, c1, c2, dComponent1, dComponent2, dComponent3);
			if (b_z == missingValue)
				return missingValue;
		} else
		{
			e_x = interpolate(ex_, c0, c1, c2, dComponent1, dComponent2, dComponent3);
			if (e_x == missingValue)
				return missingValue;
			e_z = interpolate(ez_, c0, c1, c2, dComponent1, dComponent2, dComponent3);
			if (e_z == missingValue)
				return missingValue;
			b_x = interpolate(bx_, c0, c1, c2, dComponent1, dComponent2, dComponent3);
			if (b_x == missingValue)
				return missingValue;
			b_z = interpolate(bz_, c0, c1, c2, dComponent1, dComponent2, dComponent3);
			if (b_z == missingValue)
				return missingValue;
		}
		value = e_z * b_x - e_x * b_z;
		value *= exb_factor;
		return value;

	}

	/**
	 * @param variable
	 * @param c0 The first component of the position
	 * @param c1 The second component of the position
	 * @param c2 The third component of the position
	 * @param missingValue The value that should be returned if the interpolation fails.
	 * @param dComponent1 The delta of the block size for component1 at the position specified.
	 * @param dComponent2 The delta of the block size for component2 at the position specified.
	 * @param dComponent3 The delta of the block size for component3 at the position specified.
	 * @return
	 */
	float KameleonInterpolator::compute_exbz(const std::string& variable, const float& c0, const float& c1,
			const float& c2, float& dComponent1, float& dComponent2, float& dComponent3)
	{
		//J_x*B_y-J_y*B_x
		float value;
		float mu_0 = 4e-7 * 3.141592;
		float exb_factor = 1.e-6 / mu_0;
		float e_x, e_y, b_x, b_y;
		float missingValue = this->modelReader->getMissingValue();


		if (modelName == mas_)
		{
			b_x = interpolate("br", c0, c1, c2, dComponent1, dComponent2, dComponent3);
			if (b_x == missingValue)
				return missingValue;
			b_y = interpolate("btheta", c0, c1, c2, dComponent1, dComponent2, dComponent3);
			if (b_y == missingValue)
				return missingValue;
			e_x = interpolate("er", c0, c1, c2, dComponent1, dComponent2, dComponent3);
			if (e_x == missingValue)
				return missingValue;
			e_y = interpolate("etheta", c0, c1, c2, dComponent1, dComponent2, dComponent3);
			if (e_y == missingValue)
				return missingValue;
		} else
		{
			b_x = interpolate(bx_, c0, c1, c2, dComponent1, dComponent2, dComponent3);
			if (b_x == missingValue)
				return missingValue;
			b_y = interpolate(by_, c0, c1, c2, dComponent1, dComponent2, dComponent3);
			if (b_y == missingValue)
				return missingValue;
			e_x = interpolate(ex_, c0, c1, c2, dComponent1, dComponent2, dComponent3);
			if (e_x == missingValue)
				return missingValue;
			e_y = interpolate(ey_, c0, c1, c2, dComponent1, dComponent2, dComponent3);
			if (e_y == missingValue)
				return missingValue;
		}
		value = e_x * b_y - e_y * b_x;
		value *= exb_factor;
		return value;

	}

	/**
	 * @param variable
	 * @param c0 The first component of the position
	 * @param c1 The second component of the position
	 * @param c2 The third component of the position
	 * @param missingValue The value that should be returned if the interpolation fails.
	 * @param dComponent1 The delta of the block size for component1 at the position specified.
	 * @param dComponent2 The delta of the block size for component2 at the position specified.
	 * @param dComponent3 The delta of the block size for component3 at the position specified.
	 * @return
	 */
	float KameleonInterpolator::compute_exb(const std::string& variable, const float& c0, const float& c1,
			const float& c2, float& dComponent1, float& dComponent2, float& dComponent3)
	{
		//J_x*B_y-J_y*B_x
		float value, value_x, value_y, value_z;
		float mu_0 = 4e-7 * 3.141592;
		float exb_factor = 1.e-6 / mu_0;
		float missingValue = this->modelReader->getMissingValue();


		/** need to reduce number of duplicate interpolates to increase speed **/

		float e_x, e_y, e_z, b_x, b_y, b_z, v_x, v_y, v_z;
		if (modelName == mas_)
		{
			v_x = -1.0 * interpolate("ur", c0, c1, c2, dComponent1, dComponent2, dComponent3);
			if (v_x == missingValue)
				return missingValue;
			v_y = -1.0 * interpolate("utheta", c0, c1, c2, dComponent1, dComponent2, dComponent3);
			if (v_y == missingValue)
				return missingValue;
			v_z = -1.0 * interpolate("uphi", c0, c1, c2, dComponent1, dComponent2, dComponent3);
			if (v_z == missingValue)
				return missingValue;
			b_x = interpolate("br", c0, c1, c2, dComponent1, dComponent2, dComponent3);
			if (b_x == missingValue)
				return missingValue;
			b_y = interpolate("btheta", c0, c1, c2, dComponent1, dComponent2, dComponent3);
			if (b_y == missingValue)
				return missingValue;
			b_z = interpolate("bphi", c0, c1, c2, dComponent1, dComponent2, dComponent3);
			if (b_z == missingValue)
				return missingValue;
		} else
		{
			v_x = -1.0 * interpolate(ux_, c0, c1, c2, dComponent1, dComponent2, dComponent3);
			if (v_x == missingValue)
				return missingValue;
			v_y = -1.0 * interpolate(uy_, c0, c1, c2, dComponent1, dComponent2, dComponent3);
			if (v_y == missingValue)
				return missingValue;
			v_z = -1.0 * interpolate(uz_, c0, c1, c2, dComponent1, dComponent2, dComponent3);
			if (v_z == missingValue)
				return missingValue;
			b_x = interpolate(bx_, c0, c1, c2, dComponent1, dComponent2, dComponent3);
			if (b_x == missingValue)
				return missingValue;
			b_y = interpolate(by_, c0, c1, c2, dComponent1, dComponent2, dComponent3);
			if (b_y == missingValue)
				return missingValue;
			b_z = interpolate(bz_, c0, c1, c2, dComponent1, dComponent2, dComponent3);
			if (b_z == missingValue)
				return missingValue;
		}

		/** compute e_x **/
		e_x = v_y * b_z - v_z * b_y;
		e_x = e_x * 1e-3;

		/** compute e_y **/
		e_y = v_z * b_x - v_x * b_z;
		e_y = e_y * 1e-3;

		/** compute e_z **/
		e_z = v_x * b_y - v_y * b_x;
		e_z = e_z * 1e-3;

		/** compute exbx **/
		value_x = e_y * b_z - e_z * b_y;
		value_x *= exb_factor;

		/** compute exby **/
		value_y = e_z * b_x - e_x * b_z;
		value_y *= exb_factor;

		/** compute exbz **/
		value_z = e_x * b_y - e_y * b_x;
		value_z *= exb_factor;

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
	float KameleonInterpolator::compute_exby(const std::string& variable, const float& c0, const float& c1,
			const float& c2)
	{
		float dComponent1, dComponent2, dComponent3;
		return compute_exby(variable, c0, c1, c2, dComponent1, dComponent2, dComponent3);
	}

	/**
	 * @param variable
	 * @param c0 The first component of the position
	 * @param c1 The second component of the position
	 * @param c2 The third component of the position
	 * @return
	 */
	float KameleonInterpolator::compute_exbz(const std::string& variable, const float& c0, const float& c1,
			const float& c2)
	{
		float dComponent1, dComponent2, dComponent3;
		return compute_exbz(variable, c0, c1, c2, dComponent1, dComponent2, dComponent3);
	}

	/**
	 * @param variable
	 * @param c0 The first component of the position
	 * @param c1 The second component of the position
	 * @param c2 The third component of the position
	 * @return
	 */
	float KameleonInterpolator::compute_exb(const std::string& variable, const float& c0, const float& c1,
			const float& c2)
	{
		float dComponent1, dComponent2, dComponent3;
		return compute_exb(variable, c0, c1, c2, dComponent1, dComponent2, dComponent3);
	}
}
