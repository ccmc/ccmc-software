/*
 * Interpolator.h
 *
 *  Created on: Jun 30, 2009
 *      Author: dberrios
 */

#ifndef INTERPOLATOR_H_
#define INTERPOLATOR_H_

#include "Model.h"
namespace ccmc
{
	class Model;

	/**
	 * @class Interpolator Interpolator.h ccmc/Interpolator.h
	 * @brief TODO: Brief description of Interpolator class
	 *
	 * TODO: Full description of Inteprolator class
	 */
	class Interpolator
	{

		public:
			Interpolator();

			/**
			 * @brief Interpolates the variable specified at position (c0,c1,c2).
			 *
			 * TODO: add detailed description
			 */
			virtual float
					interpolate(const std::string& variable, const float& c0, const float& c1, const float& c2) = 0;
			virtual float interpolate(const std::string& variable, const float& c0, const float& c1, const float& c2,
					float& dc0, float& dc1, float& dc2) = 0;
			virtual float interpolate(const long& variable_id, const float & c0, const float& c1, const float& c2) = 0;
			virtual float interpolate(const long& variable_id, const float& c0, const float& c1, const float& c2, float& dc0,
					float& dc1, float& dc2) = 0;
			virtual float getConversionFactor(const std::string&);
			virtual float getConversionFactor(const long& variable_id);
			std::string getModelName();
			virtual ~Interpolator();
			void setModelReader(Model * reader);

		protected:
			Model * modelReader;
			std::string modelName;
			//float missingValue;
	};

}
#endif /* INTERPOLATOR_H_ */
