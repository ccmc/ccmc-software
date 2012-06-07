#include "KameleonInterpolator.h"
#include "StringConstants.h"
#include <math.h>
#include <string>

namespace ccmc
{
	using namespace ccmc::strings::variables;
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
	float KameleonInterpolator::compute_etajComponent1(const std::string& variable, const float& positionComponent1,
			const float& positionComponent2, const float& positionComponent3, float& dComponent1, float& dComponent2,
			float& dComponent3)
	{
		float missingValue = this->modelReader->getMissingValue();

		float eta, jx;
		eta = interpolate("eta", positionComponent1, positionComponent2, positionComponent3, dComponent1, dComponent2,
				dComponent3);
		if (eta == missingValue)
			return missingValue;

		jx = interpolate(jx_, positionComponent1, positionComponent2, positionComponent3, dComponent1, dComponent2,
				dComponent3);
		if (jx == missingValue)
			return missingValue;

		return eta * jx;

	}

	/**
	 * @param variable
	 * @param positionComponent1 The first component of the position
	 * @param positionComponent2 The second component of the position
	 * @param positionComponent3 The third component of the position
	 * @return
	 */
	float KameleonInterpolator::compute_etajComponent1(const std::string& variable, const float& positionComponent1,
			const float& positionComponent2, const float& positionComponent3)
	{
		float dComponent1, dComponent2, dComponent3;
		return compute_etajComponent1(variable, positionComponent1, positionComponent2, positionComponent3,
				dComponent1, dComponent2, dComponent3);
	}

	/**
	 * @param variable
	 * @param positionComponent1 The first component of the position
	 * @param positionComponent2 The second component of the position
	 * @param positionComponent3 The third component of the position
	 * @return
	 */
	float KameleonInterpolator::compute_etaj(const std::string& variable, const float& positionComponent1,
			const float& positionComponent2, const float& positionComponent3)
	{
		float dComponent1, dComponent2, dComponent3;
		return compute_etaj(variable, positionComponent1, positionComponent2, positionComponent3, dComponent1,
				dComponent2, dComponent3);
	}

	/**
	 * @param variable
	 * @param positionComponent1 The first component of the position
	 * @param positionComponent2 The second component of the position
	 * @param positionComponent3 The third component of the position
	 * @param missing The value that should be returned if the interpolation fails.
	 * @param dComponent1 The delta of the block size for component1 at the position specified.
	 * @param dComponent2 The delta of the block size for component2 at the position specified.
	 * @param dComponent3 The delta of the block size for component3 at the position specified.
	 * @return
	 */
	float KameleonInterpolator::compute_etaj(const std::string& variable, const float& positionComponent1,
			const float& positionComponent2, const float& positionComponent3, float& dComponent1, float& dComponent2,
			float& dComponent3)
	{
		float missingValue = this->modelReader->getMissingValue();

		float eta, jx, jy, jz;
		eta = interpolate("eta", positionComponent1, positionComponent2, positionComponent3, dComponent1, dComponent2,
				dComponent3);
		if (eta == missingValue)
			return missingValue;

		jx = interpolate(jx_, positionComponent1, positionComponent2, positionComponent3, dComponent1, dComponent2,
				dComponent3);
		if (jx == missingValue)
			return missingValue;

		jy = interpolate(jy_, positionComponent1, positionComponent2, positionComponent3, dComponent1, dComponent2,
				dComponent3);
		if (jy == missingValue)
			return missingValue;

		jz = interpolate(jz_, positionComponent1, positionComponent2, positionComponent3, dComponent1, dComponent2,
				dComponent3);
		if (jz == missingValue)
			return missingValue;

		float j = sqrt(jx * jx + jy * jy + jz * jz);
		return eta * j;

	}

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
	float KameleonInterpolator::compute_etajComponent2(const std::string& variable, const float& positionComponent1,
			const float& positionComponent2, const float& positionComponent3, float& dComponent1, float& dComponent2,
			float& dComponent3)
	{
		float missingValue = this->modelReader->getMissingValue();

		float eta, jy;
		eta = interpolate("eta", positionComponent1, positionComponent2, positionComponent3, dComponent1, dComponent2,
				dComponent3);
		if (eta == missingValue)
			return missingValue;

		jy = interpolate(jy_, positionComponent1, positionComponent2, positionComponent3, dComponent1, dComponent2,
				dComponent3);
		if (jy == missingValue)
			return missingValue;

		return eta * jy;
	}

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
	float KameleonInterpolator::compute_etajComponent3(const std::string& variable, const float& positionComponent1,
			const float& positionComponent2, const float& positionComponent3, float& dComponent1, float& dComponent2,
			float& dComponent3)
	{
		float missingValue = this->modelReader->getMissingValue();

		float eta, jz;
		eta = interpolate("eta", positionComponent1, positionComponent2, positionComponent3, dComponent1, dComponent2,
				dComponent3);
		if (eta == missingValue)
			return missingValue;

		jz = interpolate(jz_, positionComponent1, positionComponent2, positionComponent3, dComponent1, dComponent2,
				dComponent3);
		if (jz == missingValue)
			return missingValue;

		return eta * jz;
	}

	/**
	 * @param variable
	 * @param positionComponent1 The first component of the position
	 * @param positionComponent2 The second component of the position
	 * @param positionComponent3 The third component of the position
	 * @return
	 */
	float KameleonInterpolator::compute_etajComponent2(const std::string& variable, const float& positionComponent1,
			const float& positionComponent2, const float& positionComponent3)
	{
		float dComponent1, dComponent2, dComponent3;
		return compute_etajComponent2(variable, positionComponent1, positionComponent2, positionComponent3,
				dComponent1, dComponent2, dComponent3);
	}

	/**
	 * @param variable
	 * @param positionComponent1 The first component of the position
	 * @param positionComponent2 The second component of the position
	 * @param positionComponent3 The third component of the position
	 * @return
	 */
	float KameleonInterpolator::compute_etajComponent3(const std::string& variable, const float& positionComponent1,
			const float& positionComponent2, const float& positionComponent3)
	{
		float dComponent1, dComponent2, dComponent3;
		return compute_etajComponent3(variable, positionComponent1, positionComponent2, positionComponent3,
				dComponent1, dComponent2, dComponent3);
	}

}
