/*
 * Variable.h
 *
 *  Created on: Nov 24, 2010
 *      Author: David Berrios
 */

#ifndef VARIABLE_H_
#define VARIABLE_H_
#include <vector>
#include <string>
#include "Interpolator.h"

namespace ccmc
{

	/*
	 *
	 */
	class Variable
	{
		protected:
			std::string name;
			std::string visUnits;
			std::string SIUnits;
			std::vector<std::string> requiredVariables;
			std::vector<std::string> requiredVariablesForVectors;
			std::string equation;


		public:
			Variable();
			void setName(std::string name);
			std::string getName();
			void setVisUnits(std::string visUnits);
			void setSIUnits(std::string SIUnits);
			std::string getVisUnits();
			std::string getSIUnits();
			virtual float calculate(Interpolator * interpolator,
					const float& c0, const float& c1, const float& c2) = 0;
			virtual float calculate(Interpolator * interpolator,
					const float& c0, const float& c1, const float& c2, float& d0, float& d1, float& d2) = 0;
			void setRequiredVariables(std::vector<std::string> variables);
			void setRequiredVariablesForVectors(std::vector<std::string> variables);
			void setEquation(std::string equation);
			virtual ~Variable();
	};
}

#endif /* VARIABLE_H_ */
