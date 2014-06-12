#include "KameleonInterpolator.h"
#include "Constants.h"
#include <iostream>
#include <string>
using namespace std;

namespace ccmc
{
	using namespace ccmc::constants;
	/**
	 * Interpolates the variable at the position specified.  Automatically selects the appropriate basic interpolator of the Kameleon
	 * library for the model currently opened.
	 * @param variable
	 * @param positionComponent1
	 * @param positionComponent2
	 * @param positionComponent3
	 * @return The interpolated value converted to the units appropriate for visualization.  Use the getUnits method to return the
	 * units of a particular variable, as returned by the Derived library.
	 */
	float KameleonInterpolator::interpolateSimple(const std::string& variable, const float& positionComponent1,
			const float& positionComponent2, const float& positionComponent3)
	{
		float dComponent1, dComponent2, dComponent3;
		return interpolateSimple(variable, positionComponent1, positionComponent2, positionComponent3, dComponent1,
				dComponent2, dComponent3);
	}

	/**
	 * Interpolates the variable at the position specified.  Automatically selects the appropriate basic interpolator of the Kameleon
	 * library for the model currently opened.
	 * @param variable
	 * @param positionComponent1
	 * @param positionComponent2
	 * @param positionComponent3
	 * @param missing
	 * @param dComponent1
	 * @param dComponent2
	 * @param dComponent3
	 * @return The interpolated value converted to the units appropriate for visualization.  Use the getUnits method to return the
	 * units of a particular variable, as returned by the Derived library.
	 */
	float KameleonInterpolator::interpolateSimple(const std::string& variable, const float& positionComponent1,
			const float& positionComponent2, const float& positionComponent3, float& dComponent1, float& dComponent2,
			float& dComponent3)
	{
		//selectCDF(current_cdf_id);
		float missingValue = this->modelReader->getMissingValue();

		float interp_value = missingValue;//model->getMissingValue();
		int flag1 = 0;
		int flag2 = 1;


		interp_value = interpolator->interpolate(variable, positionComponent1, positionComponent2, positionComponent3,
				dComponent1, dComponent2, dComponent3);
		return (interp_value);

	}

	/**
	 * Interpolates the variable at the position specified.  Automatically selects the appropriate basic interpolator of the Kameleon
	 * library for the model currently opened.
	 * @param variable_id
	 * @param positionComponent1
	 * @param positionComponent2
	 * @param positionComponent3
	 * @return The interpolated value converted to the units appropriate for visualization.  Use the getUnits method to return the
	 * units of a particular variable, as returned by the Derived library.
	 */
	float KameleonInterpolator::interpolateSimple(const long& variable_id, const float& positionComponent1,
			const float& positionComponent2, const float& positionComponent3)
	{
		float dComponent1, dComponent2, dComponent3;
		return interpolateSimple(variable_id, positionComponent1, positionComponent2, positionComponent3, dComponent1,
				dComponent2, dComponent3);
	}

	/**
	 * Interpolates the variable at the position specified.  Automatically selects the appropriate basic interpolator of the Kameleon
	 * library for the model currently opened.
	 * @param variable_id
	 * @param positionComponent1
	 * @param positionComponent2
	 * @param positionComponent3
	 * @param missing
	 * @param dComponent1
	 * @param dComponent2
	 * @param dComponent3
	 * @return The interpolated value converted to the units appropriate for visualization.  Use the getUnits method to return the
	 * units of a particular variable, as returned by the Derived library.
	 */
	float KameleonInterpolator::interpolateSimple(const long& variable_id, const float& positionComponent1,
			const float& positionComponent2, const float& positionComponent3, float& dComponent1, float& dComponent2,
			float& dComponent3)
	{
		//selectCDF(current_cdf_id);
		float missingValue = this->modelReader->getMissingValue();

		float interp_value = missingValue;//model->getMissingValue();
		int flag1 = 0;
		int flag2 = 1;



		interp_value = interpolator->interpolate(variable_id, positionComponent1, positionComponent2, positionComponent3,
				dComponent1, dComponent2, dComponent3);
		return interp_value;

	}
}
