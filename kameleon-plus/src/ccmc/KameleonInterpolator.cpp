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
	KameleonInterpolator::KameleonInterpolator(Model * model)
	{

		this->modelReader = model;
		this->modelName = modelReader->getModelName();
		initializeCalculationMethods();
		initializeConversionFactorsToVis();
		interpolator = modelReader->createNewInterpolator();

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
			interp_value = interp_value*this->getConversionFactorToVis(variable);
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
			interp_value *= this->getConversionFactorToVis(variable);

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
	 * A variable_id won't work well, since derived variables can be requested, which do not exist in the data.
	 *
	 *
	 * OpenGGCM and BATSRUS: c0,c1,c2 corresponds to x,y,z, respectively.  ENLIL and MAS: c0,c1,c2 corresponds
	 * to r,phi(latitude), theta(longitude), respectively
	 * @param variable
	 * @param c0
	 * @param c1
	 * @param c2
	 * @param dc0
	 * @param dc1
	 * @param dc2
	 */
	float KameleonInterpolator::interpolate(long variable_id, const float& c0, const float& c1, const float& c2,
			float& dc0, float& dc1, float& dc2)
	{
		// Variable: Wish me luck!
		// Derived: Good luck variable!
		float interp_value = interpolateSimple(variable_id, c0, c1, c2, dc0, dc1, dc2);
		interp_value *= this->getConversionFactorToVis(this->modelReader->getVariableName(variable_id));
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
			conversionFactorsToVis["br"] = 1.e9;
			conversionFactorsToVis["bphi"] = 1.e9;
			conversionFactorsToVis["btheta"] = 1.e9;
			conversionFactorsToVis["b1r"] = 1.e9;
			conversionFactorsToVis["b1phi"] = 1.e9;
			conversionFactorsToVis["b1theta"] = 1.e9;
			conversionFactorsToVis["ur"] = 1.e-3;
			conversionFactorsToVis["uphi"] = 1.e-3;
			conversionFactorsToVis["utheta"] = 1.e-3;
			conversionFactorsToVis["n"] = 1.e-6 * ccmc::constants::Avogadro;
			conversionFactorsToVis["rho"] = 1.e3;
			conversionFactorsToVis["er"] = 1.e-3;
			conversionFactorsToVis["ephi"] = 1.e-3;
			conversionFactorsToVis["etheta"] = 1.e-3;

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
			conversionFactorsToVis["t"] = 1.e-15 / ccmc::constants::Boltzmann;
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
			conversionFactorsToVis["t"] = 1.e-12 / ccmc::constants::Boltzmann;
			conversionFactorsToVis["s"] = 1.e-4;
			conversionFactorsToVis["pram"] = 1.627e-6;
			conversionFactorsToVis["nv"] = 1.e6;
			conversionFactorsToVis["nvx"] = 1.e6;
			conversionFactorsToVis["nvy"] = 1.e6;
			conversionFactorsToVis["nvz"] = 1.e6;
			conversionFactorsToVis["beta"] = 1.e21;

		}

	}

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

	std::vector<float>* KameleonInterpolator::interpolateSimple(std::vector<std::string>& variables, const float& c0, const float& c1, const float& c2)
	{
		std::vector<float>* interpolatedData = new std::vector<float>();
		for (int i = 0; i < variables.size(); i++)
		{
			interpolatedData->push_back(this->interpolateSimple(variables[i], c0, c1, c2));
		}
		return interpolatedData;
	}

	std::vector<float>* KameleonInterpolator::interpolateSimple(std::vector<long>& variables, const float& c0, const float& c1, const float& c2)
	{
		std::vector<float>* interpolatedData = new std::vector<float>();
		for (int i = 0; i < variables.size(); i++)
		{
			interpolatedData->push_back(this->interpolateSimple(variables[i], c0, c1, c2));
		}
		return interpolatedData;
	}

	KameleonInterpolator::~KameleonInterpolator()
	{
		// TODO Auto-generated destructor stub
		delete interpolator;
	}
}
