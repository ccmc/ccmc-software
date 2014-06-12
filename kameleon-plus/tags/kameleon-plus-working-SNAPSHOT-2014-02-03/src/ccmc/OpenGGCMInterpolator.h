/*
 * OpenGGCMInterpolator.h
 *
 *  Created on: Jun 30, 2009
 *      Author: dberrios
 */

#ifndef OPENGGCMINTERPOLATOR_H_
#define OPENGGCMINTERPOLATOR_H_

#include "Interpolator.h"
#include "Model.h"

namespace ccmc
{
	/**
	 * @class OpenGGCMInterpolator OpenGGCMInterpolator.h ccmc/OpenGGCMInterpolator.h
	 * @brief TODO: Brief description of OpenGGCMInterpolator class
	 *
	 * TODO: Full description of OpenGGCMInterpolator class
	 */
	class OpenGGCMInterpolator: public Interpolator
	{
		public:
			OpenGGCMInterpolator(Model * modelReader);
			float interpolate(const std::string& variable, const float& c0, const float& c1, const float& c2);
			float interpolate(const std::string& variable, const float& c0, const float& c1, const float& c2,
					float& dc0, float& dc1, float& dc2);
			float interpolate(const long& variable_id, const float & c0, const float& c1, const float& c2);
			float interpolate(const long& variable_id, const float& c0, const float& c1, const float& c2, float& dc0,
					float& dc1, float& dc2);
			float getConversionFactor(const std::string&);
			float getConversionFactor(const long& variable_id);
			virtual ~OpenGGCMInterpolator();

		private:
			Model * modelReader;
			boost::unordered_map<std::string, float> conversionFactors;
			boost::unordered_map<long, float> conversionFactorsByID;


//			int nx;
//			int ny;
//			int nz;
			const std::vector<float> * x_array;
			const std::vector<float> * y_array;
			const std::vector<float> * z_array;
			std::string previousVariable;
			long previousVariableID;
			float previousConversionFactor;
			float previous_x;
			float previous_y;
			float previous_z;
			int previous_ix;
			int previous_iy;
			int previous_iz;
			float previousValue;
	};
}
#endif /* OPENGGCMINTERPOLATOR_H_ */
