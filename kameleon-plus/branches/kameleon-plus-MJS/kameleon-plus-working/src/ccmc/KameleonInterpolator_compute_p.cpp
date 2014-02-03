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
	float KameleonInterpolator::compute_p(const std::string& variable, const float& positionComponent1,
			const float& positionComponent2, const float& positionComponent3, float& dComponent1, float& dComponent2,
			float& dComponent3)
	{
		//float k_B = 1.3806503e-23;

		//vector<string> requiredVariables = listOfRequiredVariablesForComponents["p"];
		float p;

		if (modelName == lfm_)
		{
			float density = interpolateSimple("rho", positionComponent1, positionComponent2, positionComponent3, dComponent1,
					dComponent2, dComponent3); //[gm/cc]
			float soundSpeed = interpolateSimple("V_th", positionComponent1, positionComponent2, positionComponent3, dComponent1,
					dComponent2, dComponent3); //[cm/s]
			float gamma = 5.0/3; //adiabatic index
			// pressure[dyn/cm^2] = density[gm/cc]*(ionSoundSpeed[cm/s])^2/gamma
			// conversion factor of 1e8: cgs units of pressure are [dyn/cm^2] = .1 [Pa]
			p = .1*(density)*pow(soundSpeed,2)/gamma; // [Pa]
			return p;
		}
		else{
			if (modelReader->doesVariableExist(p_))
			{
				p = interpolateSimple(p_, positionComponent1, positionComponent2, positionComponent3, dComponent1,
						dComponent2, dComponent3);
			} else
			{
				float n = interpolate(n_, positionComponent1, positionComponent2, positionComponent3, dComponent1,
						dComponent2, dComponent3);
				float t = interpolate("T", positionComponent1, positionComponent2, positionComponent3, dComponent1,
						dComponent2, dComponent3);
				p = n * t;

			}
			return p;
		}

	}

	/**
	 * @param variable
	 * @param positionComponent1 The first component of the position
	 * @param positionComponent2 The second component of the position
	 * @param positionComponent3 The third component of the position
	 * @return
	 */
	float KameleonInterpolator::compute_p(const std::string& variable, const float& positionComponent1,
			const float& positionComponent2, const float& positionComponent3)
	{
		float dComponent1, dComponent2, dComponent3;
		return compute_p(variable, positionComponent1, positionComponent2, positionComponent3, dComponent1,
				dComponent2, dComponent3);
	}

}
