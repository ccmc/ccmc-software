/*
 * PythonModel.h
 *
 *  Created on: Jun 1, 2015
 *      Author: Asher Pembroke
 */

#include <string>
#include "Model.h"
#include <vector>
#include <boost/unordered_map.hpp>
#include <boost/python.hpp>

namespace ccmc
{
	/**
	 * @class PythonModel PythonModel.h ccmc/PythonModel.h
	 * @brief Wrapps embedded python readers/interpolators
	 *
	 * Provides a c++ interface into models written in python and defined in ccmc/pyreaders/
	 */
	class PythonModel: public Model
	{
		public:
			PythonModel();
			long open(const std::string& filename);
			Interpolator * createNewInterpolator();
			virtual ~PythonModel();

		protected:
			void initializeConversionFactorsToSI();
			void initializeSIUnits();

	};
}
