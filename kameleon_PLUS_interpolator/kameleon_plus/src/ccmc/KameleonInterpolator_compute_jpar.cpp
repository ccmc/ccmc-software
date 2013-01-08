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
	 * @param dc0 The delta of the block size for component1 at the position specified.
	 * @param dc1 The delta of the block size for component2 at the position specified.
	 * @param dc2 The delta of the block size for component3 at the position specified.
	 * @return
	 */
	float KameleonInterpolator::compute_jparComponent1(const std::string& variable, const float& c0, const float& c1,
			const float& c2, float& dc0, float& dc1, float& dc2)
	{
		//B_x*(J_x*B-x+J-y*B_y+J_z*B_z)/(B*B)
		float b0, b1, b2, j0, j1, j2, b;
		std::string bComponent0, bComponent1, bComponent2;
		std::string jComponent0, jComponent1, jComponent2;
		float missingValue = this->modelReader->getMissingValue();


		if (modelName == mas_)
		{
			bComponent0 = "br";
			bComponent1 = "bphi";
			bComponent2 = "btheta";
			jComponent0 = "jr";
			jComponent1 = "jphi";
			jComponent2 = "jtheta";
		} else
		{
			bComponent0 = "bx";
			bComponent1 = "by";
			bComponent2 = "bz";
			jComponent0 = "jx";
			jComponent1 = "jy";
			jComponent2 = "jz";
		}
		b0 = interpolate((std::string) bComponent0.c_str(), c0, c1, c2, dc0, dc1, dc2);
		if (b0 == missingValue)
			return missingValue;
		b1 = interpolate((std::string) bComponent1.c_str(), c0, c1, c2, dc0, dc1, dc2);
		if (b1 == missingValue)
			return missingValue;
		b2 = interpolate((std::string) bComponent2.c_str(), c0, c1, c2, dc0, dc1, dc2);
		if (b2 == missingValue)
			return missingValue;
		j0 = interpolate((std::string) jComponent0.c_str(), c0, c1, c2, dc0, dc1, dc2);
		if (j0 == missingValue)
			return missingValue;
		j1 = interpolate((std::string) jComponent1.c_str(), c0, c1, c2, dc0, dc1, dc2);
		if (j1 == missingValue)
			return missingValue;
		j2 = interpolate((std::string) jComponent2.c_str(), c0, c1, c2, dc0, dc1, dc2);
		if (j2 == missingValue)
			return missingValue;
		b = sqrt(b0 * b0 + b1 * b1 + b2 * b2);

		float value = b0 * (j0 * b0 + j1 * b1 + j2 * b2) / (b * b);

		return value;

	}

	/**
	 * @param variable
	 * @param c0 The first component of the position
	 * @param c1 The second component of the position
	 * @param c2 The third component of the position
	 * @return
	 */
	float KameleonInterpolator::compute_jparComponent1(const std::string& variable, const float& c0, const float& c1,
			const float& c2)
	{
		float dc0, dc1, dc2;
		return compute_jparComponent1(variable, c0, c1, c2, dc0, dc1, dc2);
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
	float KameleonInterpolator::compute_jpar(const std::string& variable, const float& c0, const float& c1,
			const float& c2, float& dc0, float& dc1, float& dc2)
	{
		//B_x*(J_x*B-x+J-y*B_y+J_z*B_z)/(B*B)
		float b0, b1, b2, j0, j1, j2, b, value, value0, value1, value2;
		std::string bComponent0, bComponent1, bComponent2;
		std::string jComponent0, jComponent1, jComponent2;
		float missingValue = this->modelReader->getMissingValue();


		if (modelName == mas_)
		{
			bComponent0 = "br";
			bComponent1 = "bphi";
			bComponent2 = "btheta";
			jComponent0 = "jr";
			jComponent1 = "jphi";
			jComponent2 = "jtheta";
		} else
		{
			bComponent0 = "bx";
			bComponent1 = "by";
			bComponent2 = "bz";
			jComponent0 = "jx";
			jComponent1 = "jy";
			jComponent2 = "jz";
		}

		b0 = interpolate((std::string) bComponent0.c_str(), c0, c1, c2, dc0, dc1, dc2);
		if (b0 == missingValue)
			return missingValue;
		b1 = interpolate((std::string) bComponent1.c_str(), c0, c1, c2, dc0, dc1, dc2);
		if (b1 == missingValue)
			return missingValue;
		b2 = interpolate((std::string) bComponent2.c_str(), c0, c1, c2, dc0, dc1, dc2);
		if (b2 == missingValue)
			return missingValue;
		j0 = interpolate((std::string) jComponent0.c_str(), c0, c1, c2, dc0, dc1, dc2);
		if (j0 == missingValue)
			return missingValue;
		j1 = interpolate((std::string) jComponent1.c_str(), c0, c1, c2, dc0, dc1, dc2);
		if (j1 == missingValue)
			return missingValue;
		j2 = interpolate((std::string) jComponent2.c_str(), c0, c1, c2, dc0, dc1, dc2);
		if (j2 == missingValue)
			return missingValue;
		b = sqrt(b0 * b0 + b1 * b1 + b2 * b2);

		value0 = b0 * (j0 * b0 + j1 * b1 + j2 * b2) / (b * b);
		value1 = b1 * (j0 * b0 + j1 * b1 + j2 * b2) / (b * b);
		value2 = b2 * (j0 * b0 + j1 * b1 + j2 * b2) / (b * b);

		value = sqrt(value0 * value0 + value1 * value1 + value2 * value2);

		return value;

	}

	/**
	 * @param variable
	 * @param c0 The first component of the position
	 * @param c1 The second component of the position
	 * @param c2 The third component of the position
	 * @return
	 */
	float KameleonInterpolator::compute_jpar(const std::string& variable, const float& c0, const float& c1,
			const float& c2)
	{
		float dc0, dc1, dc2;
		return compute_jpar(variable, c0, c1, c2, dc0, dc1, dc2);
	}

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
	float KameleonInterpolator::compute_jpary(const std::string& variable, const float& c0, const float& c1,
			const float& c2, float& dc0, float& dc1, float& dc2)
	{
		//B_x*(J_x*B-x+J-y*B_y+J_z*B_z)/(B*B)
		float b0, b1, b2, j0, j1, j2, b;
		std::string bComponent0, bComponent1, bComponent2;
		std::string jComponent0, jComponent1, jComponent2;
		float missingValue = this->modelReader->getMissingValue();


		if (modelName == mas_)
		{
			bComponent0 = "br";
			bComponent1 = "bphi";
			bComponent2 = "btheta";
			jComponent0 = "jr";
			jComponent1 = "jphi";
			jComponent2 = "jtheta";
		} else
		{
			bComponent0 = "bx";
			bComponent1 = "by";
			bComponent2 = "bz";
			jComponent0 = "jx";
			jComponent1 = "jy";
			jComponent2 = "jz";
		}

		b0 = interpolate((std::string) bComponent0.c_str(), c0, c1, c2, dc0, dc1, dc2);
		if (b0 == missingValue)
			return missingValue;
		b1 = interpolate((std::string) bComponent1.c_str(), c0, c1, c2, dc0, dc1, dc2);
		if (b1 == missingValue)
			return missingValue;
		b2 = interpolate((std::string) bComponent2.c_str(), c0, c1, c2, dc0, dc1, dc2);
		if (b2 == missingValue)
			return missingValue;
		j0 = interpolate((std::string) jComponent0.c_str(), c0, c1, c2, dc0, dc1, dc2);
		if (j0 == missingValue)
			return missingValue;
		j1 = interpolate((std::string) jComponent1.c_str(), c0, c1, c2, dc0, dc1, dc2);
		if (j1 == missingValue)
			return missingValue;
		j2 = interpolate((std::string) jComponent2.c_str(), c0, c1, c2, dc0, dc1, dc2);
		if (j2 == missingValue)
			return missingValue;
		b = sqrt(b0 * b0 + b1 * b1 + b2 * b2);

		float value = b1 * (j0 * b0 + j1 * b1 + j2 * b2) / (b * b);

		return value;

	}

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
	float KameleonInterpolator::compute_jparz(const std::string& variable, const float& c0, const float& c1,
			const float& c2, float& dc0, float& dc1, float& dc2)
	{
		//B_x*(J_x*B-x+J-y*B_y+J_z*B_z)/(B*B)
		float b0, b1, b2, j0, j1, j2, b;
		std::string bComponent0, bComponent1, bComponent2;
		std::string jComponent0, jComponent1, jComponent2;
		float missingValue = this->modelReader->getMissingValue();


		if (modelName == mas_)
		{
			bComponent0 = "br";
			bComponent1 = "bphi";
			bComponent2 = "btheta";
			jComponent0 = "jr";
			jComponent1 = "jphi";
			jComponent2 = "jtheta";
		} else
		{
			bComponent0 = "bx";
			bComponent1 = "by";
			bComponent2 = "bz";
			jComponent0 = "jx";
			jComponent1 = "jy";
			jComponent2 = "jz";
		}

		b0 = interpolate((std::string) bComponent0.c_str(), c0, c1, c2, dc0, dc1, dc2);
		if (b0 == missingValue)
			return missingValue;
		b1 = interpolate((std::string) bComponent1.c_str(), c0, c1, c2, dc0, dc1, dc2);
		if (b1 == missingValue)
			return missingValue;
		b2 = interpolate((std::string) bComponent2.c_str(), c0, c1, c2, dc0, dc1, dc2);
		if (b2 == missingValue)
			return missingValue;
		j0 = interpolate((std::string) jComponent0.c_str(), c0, c1, c2, dc0, dc1, dc2);
		if (j0 == missingValue)
			return missingValue;
		j1 = interpolate((std::string) jComponent1.c_str(), c0, c1, c2, dc0, dc1, dc2);
		if (j1 == missingValue)
			return missingValue;
		j2 = interpolate((std::string) jComponent2.c_str(), c0, c1, c2, dc0, dc1, dc2);
		if (j2 == missingValue)
			return missingValue;
		b = sqrt(b0 * b0 + b1 * b1 + b2 * b2);

		float value = b2 * (j0 * b0 + j1 * b1 + j2 * b2) / (b * b);

		return value;
	}

	/**
	 * @param variable
	 * @param c0 The first component of the position
	 * @param c1 The second component of the position
	 * @param c2 The third component of the position
	 * @return
	 */
	float KameleonInterpolator::compute_jpary(const std::string& variable, const float& c0, const float& c1,
			const float& c2)
	{
		float dc0, dc1, dc2;
		return compute_jpary(variable, c0, c1, c2, dc0, dc1, dc2);
	}

	/**
	 * @param variable
	 * @param c0 The first component of the position
	 * @param c1 The second component of the position
	 * @param c2 The third component of the position
	 * @return
	 */
	float KameleonInterpolator::compute_jparz(const std::string& variable, const float& c0, const float& c1,
			const float& c2)
	{
		float dc0, dc1, dc2;
		return compute_jparz(variable, c0, c1, c2, dc0, dc1, dc2);
	}
}
