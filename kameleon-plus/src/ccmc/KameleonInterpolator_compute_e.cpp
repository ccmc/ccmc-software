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
	float KameleonInterpolator::compute_eComponent1(const std::string& variable, const float& positionComponent1,
			const float& positionComponent2, const float& positionComponent3, float& dComponent1, float& dComponent2,
			float& dComponent3)
	{

		float missingValue = this->modelReader->getMissingValue();
		float b_y, b_z, v_y, v_z, e_x;

		if (modelReader->doesVariableExist(ex_))
		{
//			std::cout<<"ex exists, calling interpolateSimple"<<std::endl;
			e_x = interpolateSimple(ex_, positionComponent1, positionComponent2, positionComponent3, dComponent1,
					dComponent2, dComponent3);
			return e_x;
		}

		if (modelName == mas_)
		{
			b_y = interpolate("btheta", positionComponent1, positionComponent2, positionComponent3, dComponent1,
					dComponent2, dComponent3);
			if (b_y == missingValue)
				return missingValue;
			b_z = interpolate("bphi", positionComponent1, positionComponent2, positionComponent3, dComponent1,
					dComponent2, dComponent3);
			if (b_z == missingValue)
				return missingValue;

			v_y = interpolate("utheta", positionComponent1, positionComponent2, positionComponent3, dComponent1,
					dComponent2, dComponent3);
			if (v_y == missingValue)
				return missingValue;
			v_z = interpolate("uphi", positionComponent1, positionComponent2, positionComponent3, dComponent1,
					dComponent2, dComponent3);
			if (v_z == missingValue)
				return missingValue;
			//e_x = v_y*b_z - v_z*b_y;
			//e_x = e_x * 1e-3;

		} else
		{
//			std::cout<<"using ex = vy*bz - vz*by" << std::endl;
			b_y = interpolate(by_, positionComponent1, positionComponent2, positionComponent3, dComponent1,
					dComponent2, dComponent3);
			if (b_y == missingValue)
				return missingValue;
			b_z = interpolate(bz_, positionComponent1, positionComponent2, positionComponent3, dComponent1,
					dComponent2, dComponent3);
			if (b_z == missingValue)
				return missingValue;
			v_y = interpolate(uy_, positionComponent1, positionComponent2, positionComponent3, dComponent1,
					dComponent2, dComponent3);
			if (v_y == missingValue)
				return missingValue;
			v_z = interpolate(uz_, positionComponent1, positionComponent2, positionComponent3, dComponent1,
					dComponent2, dComponent3);
			if (v_z == missingValue)
				return missingValue;
		}
		e_x = v_y * b_z - v_z * b_y;
		//e_x = e_x;
#ifdef DEBUG_DERIVED

		cerr << "return e_x" << e_x << endl;
#endif

		return e_x;

	}

	/**
	 * @param variable
	 * @param positionComponent1 The first component of the position
	 * @param positionComponent2 The second component of the position
	 * @param positionComponent3 The third component of the position
	 * @return
	 */
	float KameleonInterpolator::compute_eComponent1(const std::string& variable, const float& positionComponent1,
			const float& positionComponent2, const float& positionComponent3)
	{
		float dComponent1, dComponent2, dComponent3;
		return compute_eComponent1(variable, positionComponent1, positionComponent2, positionComponent3, dComponent1,
				dComponent2, dComponent3);
	}

	/**
	 * @param variable
	 * @param positionComponent1
	 * @param positionComponent2
	 * @param positionComponent3
	 * @param dComponent1
	 * @param dComponent2
	 * @param dComponent3
	 * @return
	 */
	float KameleonInterpolator::compute_ey(const std::string& variable, const float& positionComponent1,
			const float& positionComponent2, const float& positionComponent3, float& dComponent1, float& dComponent2,
			float& dComponent3)
	{
		float missingValue = this->modelReader->getMissingValue();

		float b_x, b_z, v_x, v_z, e_y;

		if (modelReader->doesVariableExist(ey_))
		{
			e_y = interpolateSimple(ey_, positionComponent1, positionComponent2, positionComponent3, dComponent1,
					dComponent2, dComponent3);
			return e_y;
		}


		if (modelName == mas_)
		{
			b_x = interpolate("br", positionComponent1, positionComponent2, positionComponent3, dComponent1,
					dComponent2, dComponent3);
			if (b_x == missingValue)
				return missingValue;
			b_z = interpolate("bphi", positionComponent1, positionComponent2, positionComponent3, dComponent1,
					dComponent2, dComponent3);
			if (b_z == missingValue)
				return missingValue;

			v_x = interpolate("ur", positionComponent1, positionComponent2, positionComponent3, dComponent1,
					dComponent2, dComponent3);
			if (v_x == missingValue)
				return missingValue;
			v_z = interpolate("uphi", positionComponent1, positionComponent2, positionComponent3, dComponent1,
					dComponent2, dComponent3);
			if (v_z == missingValue)
				return missingValue;

		} else
		{
			b_x = interpolate(bx_, positionComponent1, positionComponent2, positionComponent3, dComponent1,
					dComponent2, dComponent3);
			if (b_x == missingValue)
				return missingValue;
			b_z = interpolate(bz_, positionComponent1, positionComponent2, positionComponent3, dComponent1,
					dComponent2, dComponent3);
			if (b_z == missingValue)
				return missingValue;
			v_x = interpolate(ux_, positionComponent1, positionComponent2, positionComponent3, dComponent1,
					dComponent2, dComponent3);
			if (v_x == missingValue)
				return missingValue;
			v_z = interpolate(uz_, positionComponent1, positionComponent2, positionComponent3, dComponent1,
					dComponent2, dComponent3);
			if (v_z == missingValue)
				return missingValue;
		}

		e_y = v_z * b_x - v_x * b_z;
		//e_y = e_y;
#ifdef DEBUG_DERIVED

		cerr << "return e_y" << e_y << endl;
#endif

		return e_y;

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
	float KameleonInterpolator::compute_ez(const std::string& variable, const float& positionComponent1,
			const float& positionComponent2, const float& positionComponent3, float& dComponent1, float& dComponent2,
			float& dComponent3)
	{
		float missingValue = this->modelReader->getMissingValue();

		float b_x, b_y, v_x, v_y, e_z;

		if (modelReader->doesVariableExist(ez_))
		{
			e_z = interpolateSimple(ez_, positionComponent1, positionComponent2, positionComponent3, dComponent1,
					dComponent2, dComponent3);
			return e_z;
		}

		if (modelName == mas_)
		{
			b_x = interpolate("br", positionComponent1, positionComponent2, positionComponent3, dComponent1,
					dComponent2, dComponent3);
			if (b_x == missingValue)
				return missingValue;
			b_y = interpolate("btheta", positionComponent1, positionComponent2, positionComponent3, dComponent1,
					dComponent2, dComponent3);
			if (b_y == missingValue)
				return missingValue;
			v_x = interpolate("ur", positionComponent1, positionComponent2, positionComponent3, dComponent1,
					dComponent2, dComponent3);
			if (v_x == missingValue)
				return missingValue;
			v_y = interpolate("utheta", positionComponent1, positionComponent2, positionComponent3, dComponent1,
					dComponent2, dComponent3);
			if (v_y == missingValue)
				return missingValue;

		} else
		{
			b_x = interpolate(bx_, positionComponent1, positionComponent2, positionComponent3, dComponent1,
					dComponent2, dComponent3);
			if (b_x == missingValue)
				return missingValue;
			b_y = interpolate(by_, positionComponent1, positionComponent2, positionComponent3, dComponent1,
					dComponent2, dComponent3);
			if (b_y == missingValue)
				return missingValue;

			v_x = interpolate(ux_, positionComponent1, positionComponent2, positionComponent3, dComponent1,
					dComponent2, dComponent3);
			if (v_x == missingValue)
				return missingValue;
			v_y = interpolate(uy_, positionComponent1, positionComponent2, positionComponent3, dComponent1,
					dComponent2, dComponent3);
			if (v_y == missingValue)
				return missingValue;

		}
		e_z = v_x * b_y - v_y * b_x;
	//	e_z = e_z;

#ifdef DEBUG_DERIVED

		cerr << "return e_z" << e_z << endl;
#endif

		return e_z;

	}
	/**
	 * @param variable
	 * @param positionComponent1 The first component of the position
	 * @param positionComponent2 The second component of the position
	 * @param positionComponent3 The third component of the position
	 * @return
	 */
	float KameleonInterpolator::compute_ey(const std::string& variable, const float& positionComponent1,
			const float& positionComponent2, const float& positionComponent3)
	{
		float dComponent1, dComponent2, dComponent3;
		return compute_ey(variable, positionComponent1, positionComponent2, positionComponent3, dComponent1,
				dComponent2, dComponent3);
	}

	/**
	 * @param variable
	 * @param positionComponent1 The first component of the position
	 * @param positionComponent2 The second component of the position
	 * @param positionComponent3 The third component of the position
	 * @return
	 */
	float KameleonInterpolator::compute_ez(const std::string& variable, const float& positionComponent1,
			const float& positionComponent2, const float& positionComponent3)
	{
		float dComponent1, dComponent2, dComponent3;
		return compute_ez(variable, positionComponent1, positionComponent2, positionComponent3, dComponent1,
				dComponent2, dComponent3);
	}
}
