#include "KameleonInterpolator.h"
#include <iostream>
#include <string>
using namespace std;

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
float KameleonInterpolator::interpolateSimple(const std::string& variable, const float& positionComponent1, const float& positionComponent2, const float& positionComponent3)
{
	float dComponent1, dComponent2, dComponent3;
	return interpolateSimple(variable, positionComponent1, positionComponent2, positionComponent3,
			dComponent1, dComponent2, dComponent3);
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
float KameleonInterpolator::interpolateSimple(const std::string& variable, const float& positionComponent1, const float& positionComponent2, const float& positionComponent3,
				float& dComponent1, float& dComponent2, float& dComponent3)
{
	//selectCDF(current_cdf_id);
	float interp_value = missingValue;//model->getMissingValue();
	int flag1 = 0;
	int flag2 = 1;
	float mToAU = 6.68458134e-12;

/*
    if (modelName == "batsrus")
    {
        interp_value = interpolate_batsrus_cdf( variable, positionComponent1, positionComponent2, positionComponent3, flag1, flag2,
				missing, &dComponent1, &dComponent2, &dComponent3);

    }
    else if (modelName == "ucla_ggcm" || modelName == "open_ggcm")
    {
        interp_value = interpolate_ucla_ggcm_cdf(variable, positionComponent1, positionComponent2, positionComponent3, flag1, flag2,
				missing, &dComponent1, &dComponent2, &dComponent3);

    }
    else if (modelName == "enlil")
    {
        interp_value = interpolate_enlil_cdf_2(variable, positionComponent1, positionComponent2, positionComponent3, flag1, flag2,
				missing, &dComponent1, &dComponent2, &dComponent3);
        dComponent1 = dComponent1 * mToAU;


    }
    else if (modelName == "ctip")
    {
        interp_value = interpolate_ctip_cdf(variable, positionComponent1, positionComponent2, positionComponent3, flag1, flag2,
				missing, &dComponent1, &dComponent2, &dComponent3);
    } else if (modelName == "mas")
    {

    	interp_value = interpolate_mas_cdf(variable, positionComponent1, positionComponent2, positionComponent3, flag1, flag2,
    			missing, &dComponent1, &dComponent2, &dComponent3);


    }
*/
	//std::cout << "before grabbing appropriate interpolator" << std::endl;
	//Interpolator * interpolator = modelReader->createNewInterpolator();
	//std::cout << "after grabbing appropriate interpolator" << std::endl;

	//std::cout << "variable: " << variable << " c0: " << positionComponent1 << " c1: " << positionComponent2 << " c2: " << positionComponent3 << std::endl;
	interp_value = interpolator->interpolate(variable, positionComponent1, positionComponent2, positionComponent3, dComponent1, dComponent2, dComponent3);
	//delete interpolator;

    return interp_value;

}
