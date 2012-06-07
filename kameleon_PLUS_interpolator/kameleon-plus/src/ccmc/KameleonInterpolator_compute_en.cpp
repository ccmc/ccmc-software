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
	float KameleonInterpolator::compute_en(const std::string& variable, const float& positionComponent1,
			const float& positionComponent2, const float& positionComponent3, float& dComponent1, float& dComponent2,
			float& dComponent3)
	{
		float interp_value = 0.0;
		float missingValue = this->modelReader->getMissingValue();


		//std::string modelName = gattribute_char_get("model_name");
		if (modelName == batsrus_)
		{
			interp_value = interpolateSimple("e", positionComponent1, positionComponent2, positionComponent3,
					dComponent1, dComponent2, dComponent3);
		} else //default to ucla_ggcm
		{
			interp_value = missingValue;
		}
		return interp_value;

	}

	/**
	 * @param variable
	 * @param positionComponent1 The first component of the position
	 * @param positionComponent2 The second component of the position
	 * @param positionComponent3 The third component of the position
	 * @return
	 */
	float KameleonInterpolator::compute_en(const std::string& variable, const float& positionComponent1,
			const float& positionComponent2, const float& positionComponent3)
	{
		float dComponent1, dComponent2, dComponent3;
		return compute_en(variable, positionComponent1, positionComponent2, positionComponent3, dComponent1,
				dComponent2, dComponent3);
	}
}
