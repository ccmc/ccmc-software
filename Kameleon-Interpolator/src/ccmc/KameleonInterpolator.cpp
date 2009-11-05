/*
 * KameleonInterpolator.cpp
 *
 *  Created on: Jul 1, 2009
 *      Author: David Berrios
 */

#include "KameleonInterpolator.h"
#include "Constants.h"

namespace ccmc
{
	KameleonInterpolator::KameleonInterpolator(Model * modelReader)
	{

		this->modelReader = modelReader;
		initializeCalculationMethods();
		initializeConversionFactorsToVis();
		interpolator = modelReader->createNewInterpolator();
		j_ = "j";
		b_ = "b";
		jx_ = "jx";
		jy_ = "jy";
		jz_ = "jz";
		bx_ = "bx";
		by_ = "by";
		bz_ = "bz";
		n_ = "n";
		rho_ = "rho";
		ux_ = "ux";
		uy_ = "uy";
		uz_ = "uz";
		ex_ = "ex";
		ey_ = "ey";
		ez_ = "ez";
		p_ = "p";
		batsrus_ = "batsrus";
		open_ggcm_ = "open_ggcm";
		ucla_ggcm_ = "ucla_ggcm";
		mas_ = "mas";
		enlil_ = "enlil";

	}

	float KameleonInterpolator::interpolate(const std::string& variable, const float& c0, const float& c1,
			const float& c2)
	{
		float interp_value;
		float dc0, dc1, dc2;

		return interpolate(variable, c0, c1, c2, dc0, dc1, dc2);
	}

	float KameleonInterpolator::interpolate(const std::string& variable, const float& c0, const float& c1,
			const float& c2, float& dc0, float& dc1, float& dc2)
	{
		float interp_value;
		//	std::cout << "variable: '" << variable << "'" << std::endl;
		if (calculationMethod.find(variable) != calculationMethod.end())
		{
			//		std::cout << "found something: " << std:: endl;
			interp_value = (this->*(calculationMethod[variable]))(variable, c0, c1, c2, dc0, dc1, dc2);
			if (interp_value == missingValue)
			{
				return missingValue;
				//cerr << "***Derived::interpolate: Interpolate returned the missing value for variable " << variable_string << endl;
			}
			//interp_value = interp_value*this->getConversionFactorToVis(variable);
		} else
		{
			// Variable: Wish me luck!
			// Derived: Good luck variable!
			interp_value = interpolateSimple(variable, c0, c1, c2, dc0, dc1, dc2);
			if (interp_value == missingValue)
			{
				return missingValue;
				//cerr << "***Derived::interpolate: Interpolate returned the missing value for variable " << variable_string << endl;
			}

		}


		return interp_value;
	}

	/**
	 * Does nothing.  A variable_id won't work well, since derived variables can be requested, which do not exist in the data.
	 */
	float KameleonInterpolator::interpolate(long variable_id, const float& c0, const float& c1, const float& c2)
	{
		// Variable: Wish me luck!
		// Derived: Good luck variable!
		float interp_value = interpolateSimple(variable_id, c0, c1, c2);
		return interp_value;
	}

	/**
	 * Does nothing.  A variable_id won't work well, since derived variables can be requested, which do not exist in the data.
	 */
	float KameleonInterpolator::interpolate(long variable_id, const float& c0, const float& c1, const float& c2,
			float& dc0, float& dc1, float& dc2)
	{
		// Variable: Wish me luck!
		// Derived: Good luck variable!
		float interp_value = interpolateSimple(variable_id, c0, c1, c2, dc0, dc1, dc2);
		return interp_value;
	}

	/**
	 * Initializes the conversion factors required to change the units of the base
	 * variables into those required for the visualization plots.  The conversionFactors
	 * only need to be specified by those base variables requiring a conversion factor.
	 * All variables that are not specified in the conversionFactorsToVis map are multiplied
	 * by 1.0
	 *
	 * For some vector variables, the conversion factors need to be specified for the magnitude
	 * calculations as well, since some of the magnitude calculations are independent of the vector
	 * component calculations (to increase performance).
	 */
	void KameleonInterpolator::initializeConversionFactorsToVis()
	{
		float avogadrosNumber = 6.0221415e23;
		float boltzmannConstant = 1.3806503e-23;
		float gauss2Tesla = 1.e-4;
		float mu_0 = 8.854e-12;
		float l_0 = 6.96e8;
		std::string bx_, by_, bz_, ux_, uy_, uz_, n_, rho_, ex_, ey_, ez_;
		bx_ = "bx";
		by_ = "by";
		bz_ = "bz";
		ux_ = "ux";
		uy_ = "uy";
		uz_ = "uz";
		n_ = "n";
		rho_ = "rho";
		ex_ = "ex";
		ey_ = "ey";
		ez_ = "ez";

		/** need to make this work from SI units.  Simplify the maintenance of the conversions **/

		if (modelName == "enlil")
		{
			//conversionFactorsToVis["bx"] = 1.e9;
			//conversionFactorsToVis["by"] = 1.e9;
			//conversionFactorsToVis["bz"] = 1.e9;
			conversionFactorsToVis["b1x"] = 1.e9;
			conversionFactorsToVis["b1y"] = 1.e9;
			conversionFactorsToVis["b1z"] = 1.e9;
			conversionFactorsToVis["ux"] = 1.e-3;
			conversionFactorsToVis["uy"] = 1.e-3;
			conversionFactorsToVis["uz"] = 1.e-3;
			conversionFactorsToVis["n"] = 1.e-6 * Avogadro;
			conversionFactorsToVis["rho"] = 1.e3;
			conversionFactorsToVis["ex"] = 1.e-3;
			conversionFactorsToVis["ey"] = 1.e-3;
			conversionFactorsToVis["ez"] = 1.e-3;

			//blah
		} else if (modelName == "mas")
		{
			conversionFactorsToVis["n"] = 1.e8;
			conversionFactorsToVis["t"] = 2.84e7;
			conversionFactorsToVis["p"] = 0.387 * 1.e9 * .1; //Dyn/cm^2 * to_nano * .1
			conversionFactorsToVis["ur"] = 481.37;
			conversionFactorsToVis["uphi"] = 481.37;
			conversionFactorsToVis["utheta"] = 481.37;
			conversionFactorsToVis["jr"] = 1.e2 * 2.205 / (mu_0 * l_0);
			conversionFactorsToVis["jphi"] = 1.e2 * 2.205 / (mu_0 * l_0);
			conversionFactorsToVis["jtheta"] = 1.e2 * 2.205 / (mu_0 * l_0);
			conversionFactorsToVis["er"] = 1.e-3;
			conversionFactorsToVis["ephi"] = 1.e-3;
			conversionFactorsToVis["etheta"] = 1.e-3;
			conversionFactorsToVis["br"] = 2.205 * gauss2Tesla * 1.e9;
			conversionFactorsToVis["bphi"] = 2.205 * gauss2Tesla * 1.e9;
			conversionFactorsToVis["btheta"] = 2.205 * gauss2Tesla * 1.e9;

		} else if (modelName == "ucla_ggcm" || modelName == "open_ggcm")
		{
			conversionFactorsToVis["p"] = 1.e-3;
			conversionFactorsToVis["t"] = 1.e-15 / Boltzmann;
			conversionFactorsToVis["s"] = 1.e-4;
			conversionFactorsToVis["pram"] = 1.627e-6;
			conversionFactorsToVis["nv"] = 1.e6;
			conversionFactorsToVis["nvx"] = 1.e6;
			conversionFactorsToVis["nvy"] = 1.e6;
			conversionFactorsToVis["nvz"] = 1.e6;
			conversionFactorsToVis["beta"] = 1.e9;


		} else //just BATSRUS for now
		{
			conversionFactorsToVis["p"] = 1.e-3;
			conversionFactorsToVis["t"] = 1.e-12 / Boltzmann;
			conversionFactorsToVis["s"] = 1.e-4;
			conversionFactorsToVis["pram"] = 1.627e-6;
			conversionFactorsToVis["nv"] = 1.e6;
			conversionFactorsToVis["nvx"] = 1.e6;
			conversionFactorsToVis["nvy"] = 1.e6;
			conversionFactorsToVis["nvz"] = 1.e6;
			conversionFactorsToVis["beta"] = 1.e21;

		}

	}

	/*std::vector<std::string> KameleonInterpolator::getListOfRequiredVariablesForComponents(std::string variable)
	 {
	 boost::unordered_map<std::string, std::vector<std::string> >::iterator iter = this->listOfRequiredVariablesForComponents.find(variable);
	 if (iter != listOfRequiredVariablesForComponents.end())
	 {
	 return (*iter).second;
	 } else
	 {
	 if (modelReader->doesVariableExist(variable))
	 {
	 std::vector<std::string> required;
	 required.push_back(variable);
	 return  required;
	 }
	 else
	 {
	 std::vector<std::string> required;
	 return required;
	 }
	 }
	 }
	 */

	/**
	 * Returns the stored conversion factor to convert the variable to the units
	 * used for visualization. These units may differ from SI units and the units
	 * stored in the file.
	 * @param variable
	 * @return
	 */
	float KameleonInterpolator::getConversionFactorToVis(const std::string& variable)
	{
		boost::unordered_map<std::string, float>::iterator iter = conversionFactorsToVis.find(variable);

		if (iter != conversionFactorsToVis.end())
			return (*iter).second;
		else
		{
			return 1.0f;
		}
	}

	KameleonInterpolator::~KameleonInterpolator()
	{
		// TODO Auto-generated destructor stub
		delete interpolator;
	}
}
