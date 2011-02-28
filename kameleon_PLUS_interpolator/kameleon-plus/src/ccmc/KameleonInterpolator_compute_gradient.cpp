/*
 * KameleonInterpolator_compute_gradient.cpp
 *
 *  Created on: Sep 28, 2009
 *      Author: David Berrios
 */

#include "KameleonInterpolator.h"
#include <vector>
#include <boost/algorithm/string.hpp>


namespace ccmc
{


	/**
	 * @param variable
	 * @param positionComponent1 The first component of the position
	 * @param positionComponent2 The second component of the position
	 * @param positionComponent3 The third component of the position
	 * @return
	 */
	float KameleonInterpolator::compute_gradient(const std::string& variable, const float& positionComponent1,
			const float& positionComponent2, const float& positionComponent3)
	{
		float dComponent1, dComponent2, dComponent3;
		return compute_gradient(variable, positionComponent1, positionComponent2, positionComponent3, dComponent1,
				dComponent2, dComponent3);
	}

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
	float KameleonInterpolator::compute_gradient(const std::string& variable, const float& c0,
			const float& c1, const float& c2, float& dc0, float& dc1,
			float& dc2)
	{
		//parse the string, and attempt to calculate the gradient of the parsed variable

		std::vector<std::string> tokens;
		boost::split(tokens, variable, boost::is_any_of("."), boost::token_compress_on);

		/*t = interpolate("t", positionComponent1, positionComponent2, positionComponent3, dComponent1, dComponent2,
				dComponent3);
		if (t == missingValue)
			return missingValue;
		n = interpolate("n", positionComponent1, positionComponent2, positionComponent3, dComponent1, dComponent2,
				dComponent3);
		if (n == missingValue)
			return missingValue;
		float s = t / pow(n, 2.0f / 3.0f);*/
		//cout << "gradient variable: '" << tokens[1] << "'" << endl;

		//super expensive to calculation this. Try to store the components of the gradient, and speed up the calculations
		//if the components are requested in sequential order
		float pointValue = interpolate(tokens[1],c0,c1,c2,dc0,dc1,dc2);
		float direction;
		float dc0_scaled = dc0*.5f;
		float dc1_scaled = dc1*.5f;
		float dc2_scaled = dc2*.5f;
		float positions_c0[8];
		float positions_c1[8];
		float positions_c2[8];

		positions_c0[0] = c0 + dc0_scaled;
		positions_c0[1] = c0 + dc0_scaled;
		positions_c0[2] = c0 - dc0_scaled;
		positions_c0[3] = c0 - dc0_scaled;
		positions_c0[4] = c0 + dc0_scaled;
		positions_c0[5] = c0 + dc0_scaled;
		positions_c0[6] = c0 - dc0_scaled;
		positions_c0[7] = c0 - dc0_scaled;


		positions_c1[0] = c1 - dc1_scaled;
		positions_c1[1] = c1 - dc1_scaled;
		positions_c1[2] = c1 - dc1_scaled;
		positions_c1[3] = c1 - dc1_scaled;
		positions_c1[4] = c1 + dc1_scaled;
		positions_c1[5] = c1 + dc1_scaled;
		positions_c1[6] = c1 + dc1_scaled;
		positions_c1[7] = c1 + dc1_scaled;

		positions_c2[0] = c2 - dc2_scaled;
		positions_c2[1] = c2 + dc2_scaled;
		positions_c2[2] = c2 + dc2_scaled;
		positions_c2[3] = c2 - dc2_scaled;
		positions_c2[4] = c2 - dc2_scaled;
		positions_c2[5] = c2 + dc2_scaled;
		positions_c2[6] = c2 + dc2_scaled;
		positions_c2[7] = c2 - dc2_scaled;

		//direction and magnitude of greatest change
		float p0 = c0;
		float p1 = c1;
		float p2 = c2;
		float current_max = -999999999999.f;
		for (int i = 0; i < 8; i++)
		{

			direction = interpolate(tokens[1],positions_c0[i],positions_c1[i],positions_c2[i]);
			//cout << "*i: " << i << endl;
			float current_diff = direction - pointValue;
			if (current_diff > current_max )
			{
				current_max = current_diff;
				p0 = positions_c0[i];
				p1 = positions_c1[i];
				p2 = positions_c2[i];
			}
		}

		return current_max;

	}
}
