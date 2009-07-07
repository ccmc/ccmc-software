#include "KameleonInterpolator.h"
#include <algorithm>
#include <string>

/**
  * Selects appropriate interpolation function from the kameleon library based on the modelName. Requested
  * derived variables are calculated.
  * @param variable
  * @param positionComponent1 x for cartesian, r for spherical
  * @param positionComponent2 y for cartesian, phi for spherical
  * @param positionComponent3 z for cartesian, theta for spherical
  * @param dComponent1
  * @param dComponent2
  * @param dComponent3
  */
float KameleonInterpolator::interpolate(const std::string& variable, const float& positionComponent1, const float& positionComponent2, const float& positionComponent3,
		float& dComponent1, float& dComponent2, float& dComponent3)
{

#ifdef DEBUG_DERIVED
    cout << "entered interpolate with: " << variable << endl;
#endif
    //selectCDF(current_cdf_id);

    //std::transform(variable_string.begin(), variable_string.end(), variable_string.begin(), ::tolower);
	float interp_value;

	//first check if the variable is an included variable.  if so, send directly to interpolateSimple
	/*if (variableAliases.find(variable_string) != variableAliases.end())
	{
		variable_string = variableAliases[variable_string];
		interp_value = interpolate(variable_string, positionComponent1, positionComponent2, positionComponent3,
				dComponent1, dComponent2, dComponent3);
		if (interp_value == missingValue)
		{
			return missingValue;
			//cerr << "***Derived::interpolate: Interpolate returned the missing value for variable " << variable_string << endl;
		}
	} else*/
	if (calculationMethod.find(variable) != calculationMethod.end())
	{
		interp_value = (this->*(calculationMethod[variable]))(variable, positionComponent1, positionComponent2, positionComponent3,
			dComponent1, dComponent2, dComponent3);
		if (interp_value == missingValue)
		{
			return missingValue;
			//cerr << "***Derived::interpolate: Interpolate returned the missing value for variable " << variable_string << endl;
		}
	} else
	{
		// Variable: Wish me luck!
		// Derived: Good luck variable!
		interp_value = interpolateSimple(variable, positionComponent1, positionComponent2, positionComponent3,
				dComponent1, dComponent2, dComponent3);
		if (interp_value == missingValue)
		{
			return missingValue;
			//cerr << "***Derived::interpolate: Interpolate returned the missing value for variable " << variable_string << endl;
		}

	}

	float conversionFactor;

	if (conversionFactorsToVis.find(variable) != conversionFactorsToVis.end())
		conversionFactor = conversionFactorsToVis[variable];
	else
		conversionFactor = 1.0;

    return interp_value * conversionFactor;

}

/**
  * Selects appropriate interpolation function from the kameleon library based on the modelName. Requested
  * derived variables are calculated.
  * @param variable
  * @param positionComponent1
  * @param positionComponent2
  * @param positionComponent3
  */
float KameleonInterpolator::interpolate(const std::string& variable, const float& positionComponent1, const float& positionComponent2, const float& positionComponent3)
{
	float dComponent1, dComponent2, dComponent3;
	return interpolate(variable, positionComponent1, positionComponent2, positionComponent3,
			dComponent1, dComponent2, dComponent3);
}
