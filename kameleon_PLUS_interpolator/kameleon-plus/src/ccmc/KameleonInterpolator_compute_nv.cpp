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
	 * @param positionComponent1 The first component of the position
	 * @param positionComponent2 The second component of the position
	 * @param positionComponent3 The third component of the position
	 * @param missingValue The value that should be returned if the interpolation fails.
	 * @param dComponent1 The delta of the block size for component1 at the position specified.
	 * @param dComponent2 The delta of the block size for component2 at the position specified.
	 * @param dComponent3 The delta of the block size for component3 at the position specified.
	 * @return
	 */
	float KameleonInterpolator::compute_nvComponent1(const std::string& variable, const float& positionComponent1,
			const float& positionComponent2, const float& positionComponent3, float& dComponent1, float& dComponent2,
			float& dComponent3)
	{
		float n, vx, nvx;
		std::string n_name, vx_name;
		float missingValue = this->modelReader->getMissingValue();

		if (modelName == enlil_ || modelName == mas_)
		{
			n_name = n_;
		} else
		{
			n_name = rho_;
		}

		if (modelName == mas_)
		{
			vx_name = "ur";

		} else
		{
			vx_name = ux_;
		}
		n = interpolate(n_name, positionComponent1, positionComponent2, positionComponent3, dComponent1, dComponent2,
				dComponent3);
		if (n == missingValue)
			return missingValue;
		vx = interpolate(vx_name, positionComponent1, positionComponent2, positionComponent3, dComponent1, dComponent2,
				dComponent3);
		if (vx == missingValue)
			return missingValue;
		nvx = n * vx;
		return nvx;

	}

	/**
	 * @param variable
	 * @param positionComponent1 The first component of the position
	 * @param positionComponent2 The second component of the position
	 * @param positionComponent3 The third component of the position
	 * @return
	 */
	float KameleonInterpolator::compute_nvComponent1(const std::string& variable, const float& positionComponent1,
			const float& positionComponent2, const float& positionComponent3)
	{
		float dComponent1, dComponent2, dComponent3;
		return compute_nvComponent1(variable, positionComponent1, positionComponent2, positionComponent3, dComponent1,
				dComponent2, dComponent3);
	}

	/**
	 * @param variable
	 * @param positionComponent1 The first component of the position
	 * @param positionComponent2 The second component of the position
	 * @param positionComponent3 The third component of the position
	 * @return
	 */
	float KameleonInterpolator::compute_nv(const std::string& variable, const float& positionComponent1,
			const float& positionComponent2, const float& positionComponent3)
	{
		float dComponent1, dComponent2, dComponent3;
		return compute_nv(variable, positionComponent1, positionComponent2, positionComponent3, dComponent1,
				dComponent2, dComponent3);
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
	float KameleonInterpolator::compute_nv(const std::string& variable, const float& positionComponent1,
			const float& positionComponent2, const float& positionComponent3, float& dComponent1, float& dComponent2,
			float& dComponent3)
	{
		float n, v, vx, vy, vz, nv;
		std::string n_name, vx_name, vy_name, vz_name;
		float missingValue = this->modelReader->getMissingValue();

		if (modelName == enlil_ || modelName == mas_)
		{
			n_name = n_;
		} else
		{
			n_name = rho_;
		}

		if (modelName == mas_)
		{

			vx_name = "ur";
			vy_name = "utheta";
			vz_name = "uphi";

		} else
		{

			vx_name = ux_;
			vy_name = uy_;
			vz_name = uz_;
		}

		n = interpolate(n_name, positionComponent1, positionComponent2, positionComponent3, dComponent1, dComponent2,
				dComponent3);
		if (n == missingValue)
			return missingValue;
		vx = interpolate(vx_name, positionComponent1, positionComponent2, positionComponent3, dComponent1, dComponent2,
				dComponent3);
		if (vx == missingValue)
			return missingValue;
		vy = interpolate(vy_name, positionComponent1, positionComponent2, positionComponent3, dComponent1, dComponent2,
				dComponent3);
		if (vy == missingValue)
			return missingValue;
		vz = interpolate(vz_name, positionComponent1, positionComponent2, positionComponent3, dComponent1, dComponent2,
				dComponent3);
		if (vz == missingValue)
			return missingValue;

		v = sqrt(vx * vx + vy * vy + vz * vz);
		nv = n * v;
		return nv;

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
	float KameleonInterpolator::compute_nvy(const std::string& variable, const float& positionComponent1,
			const float& positionComponent2, const float& positionComponent3, float& dComponent1, float& dComponent2,
			float& dComponent3)
	{
		float n, vy, nvy;
		std::string n_name, vy_name;
		float missingValue = this->modelReader->getMissingValue();

		if (modelName == enlil_ || modelName == mas_)
		{
			n_name = "n";
		} else
		{
			n_name = rho_;
		}

		if (modelName == mas_)
		{
			vy_name = "utheta";
		} else
		{
			vy_name = uy_;
		}

		n = interpolate(n_name, positionComponent1, positionComponent2, positionComponent3, dComponent1, dComponent2,
				dComponent3);
		if (n == missingValue)
			return missingValue;
		vy = interpolate(vy_name, positionComponent1, positionComponent2, positionComponent3, dComponent1, dComponent2,
				dComponent3);
		if (vy == missingValue)
			return missingValue;

		nvy = n * vy;
		return nvy;

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
	float KameleonInterpolator::compute_nvz(const std::string& variable, const float& positionComponent1,
			const float& positionComponent2, const float& positionComponent3, float& dComponent1, float& dComponent2,
			float& dComponent3)
	{
		float n, vz, nvz;
		std::string n_name, vz_name;
		float missingValue = this->modelReader->getMissingValue();

		if (modelName == enlil_ || modelName == mas_)
		{
			n_name = "n";
		} else
		{
			n_name = rho_;
		}
		if (modelName == mas_)
		{
			vz_name = "uphi";
		} else
		{
			vz_name = uz_;
		}

		n = interpolate(n_name, positionComponent1, positionComponent2, positionComponent3, dComponent1, dComponent2,
				dComponent3);
		if (n == missingValue)
			return missingValue;
		vz = interpolate(vz_name, positionComponent1, positionComponent2, positionComponent3, dComponent1, dComponent2,
				dComponent3);
		if (vz == missingValue)
			return missingValue;
		nvz = n * vz;
		return nvz;

	}

	/**
	 * @param variable
	 * @param positionComponent1 The first component of the position
	 * @param positionComponent2 The second component of the position
	 * @param positionComponent3 The third component of the position
	 * @return
	 */
	float KameleonInterpolator::compute_nvy(const std::string& variable, const float& positionComponent1,
			const float& positionComponent2, const float& positionComponent3)
	{
		float dComponent1, dComponent2, dComponent3;
		return compute_nvy(variable, positionComponent1, positionComponent2, positionComponent3, dComponent1,
				dComponent2, dComponent3);
	}

	/**
	 * @param variable
	 * @param positionComponent1 The first component of the position
	 * @param positionComponent2 The second component of the position
	 * @param positionComponent3 The third component of the position
	 * @return
	 */
	float KameleonInterpolator::compute_nvz(const std::string& variable, const float& positionComponent1,
			const float& positionComponent2, const float& positionComponent3)
	{
		float dComponent1, dComponent2, dComponent3;
		return compute_nvz(variable, positionComponent1, positionComponent2, positionComponent3, dComponent1,
				dComponent2, dComponent3);
	}
}
