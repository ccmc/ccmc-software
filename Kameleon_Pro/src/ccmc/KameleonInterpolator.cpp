/*
 * KameleonInterpolator.cpp
 *
 *  Created on: Jul 1, 2009
 *      Author: David Berrios
 */

#include "KameleonInterpolator.h"

namespace ccmc
{
	KameleonInterpolator::KameleonInterpolator(Model * modelReader)
	{

		this->modelReader = modelReader;
		initializeCalculationMethods();
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
		return 0.f;
	}

	/**
	 * Does nothing.  A variable_id won't work well, since derived variables can be requested, which do not exist in the data.
	 */
	float KameleonInterpolator::interpolate(long variable_id, const float& c0, const float& c1, const float& c2,
			float& dc0, float& dc1, float& dc2)
	{
		return 0.f;
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

	KameleonInterpolator::~KameleonInterpolator()
	{
		// TODO Auto-generated destructor stub
		delete interpolator;
	}
}
