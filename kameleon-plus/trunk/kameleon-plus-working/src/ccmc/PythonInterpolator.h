/*
 * PythonInterpolator.h
 *
 *  Created on: Jun 2, 2015
 *      Author: Asher Pembroke
 */

#ifndef PythonINTERPOLATOR_H_
#define PythonINTERPOLATOR_H_

#include "Interpolator.h"
#include "Model.h"
#include "boost/ptr_container/ptr_vector.hpp"
#include "boost/unordered_map.hpp"

namespace ccmc
{
	/**
	 * @class PythonInterpolator PythonInterpolator.h ccmc/PythonInterpolator.h
	 * @brief TODO: Brief description of PythonInterpolator class
	 *
	 * An interface into a python interpolator as defined by python Model reader
	 */

	class PythonInterpolator: public Interpolator
	{
		public:
			PythonInterpolator(Model * modelReader); //points to PythonModel
			float interpolate(const std::string& variable, const float& c0, const float& c1, const float& c2);
			float interpolate(const std::string& variable, const float& c0, const float& c1, const float& c2,
					float& dc0, float& dc1, float& dc2);
			float interpolate(const long& variable_id, const float & c0, const float& c1, const float& c2);
			float interpolate(const long& variable_id, const float& c0, const float& c1, const float& c2, float& dc0,
					float& dc1, float& dc2);
			float getConversionFactor(const std::string&);
			float getConversionFactor(const long& variable_id);
			virtual ~PythonInterpolator();
			// void initializeConversionFactors();


		private:
			Model * modelReader;
			boost::unordered_map<std::string, float> conversionFactors; //what are these for?
			boost::unordered_map<long, float> conversionFactorsByID;

	};

}


#endif /* PythonINTERPOLATOR_H_ */
