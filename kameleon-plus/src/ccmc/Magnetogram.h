/*
 * Magnetogram.h
 *
 *  Created on: Sep 23, 2009
 *      Author: dberrios
 */

#ifndef MAGNETOGRAM_H_
#define MAGNETOGRAM_H_

#include "Model.h"
#include "Interpolator.h"

namespace ccmc
{

	/**
	 * @class Magnetogram Magnetogram.h ccmc/Magnetogram.h
	 * @brief TODO: Brief description of Magnetogram class
	 *
	 * TODO: Full description of Magnetogram class
	 */
	class Magnetogram: public Model
	{
		public:
			Magnetogram();
			long open(const std::string& filename);
			Interpolator* createNewInterpolator();


			virtual ~Magnetogram();

		protected:
			void initializeConversionFactorsToSI();
			void initializeSIUnits();

		private:
			//std::string r_string;
			std::string lat_string;
			std::string lon_string;
			std::string filename;
	};
}

#endif /* MAGNETOGRAM_H_ */
