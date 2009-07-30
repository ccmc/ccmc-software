/*
 * ENLIL.h
 *
 *  Created on: Jul 9, 2009
 *      Author: dberrios
 */

#ifndef ENLIL_H_
#define ENLIL_H_

#include "Model.h"
#include "Interpolator.h"

namespace ccmc
{

	/**
	 * @class ENLIL ENLIL.h ccmc/ENLIL.h
	 * @brief TODO: Brief description of ENLIL class
	 *
	 * TODO: Full description of ENLIL class
	 */
	class ENLIL: public Model
	{
		public:
			ENLIL();
			long open(const std::string&);
			Interpolator* createNewInterpolator();
			void initializeConversionFactorsToSI();
			void initializeSIUnits();

			virtual ~ENLIL();

		private:
			std::string r_string;
			std::string lat_string;
			std::string lon_string;
			std::string filename;
	};
}

#endif /* ENLIL_H_ */
