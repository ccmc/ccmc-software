/*
 * MAS.h
 *
 *  Created on: Jul 9, 2009
 *      Author: dberrios
 */

#ifndef MAS_H_
#define MAS_H_

#include "Model.h"

namespace ccmc
{
	/**
	 * @class MAS MAS.h ccmc/MAS.h
	 * @brief TODO: Brief description of MAS class
	 *
	 * TODO: Full description of MAS class
	 */
	class MAS: public Model
	{
		public:
			MAS();
			long open(const std::string&);
			Interpolator* createNewInterpolator();
			virtual ~MAS();

		protected:
			void initializeConversionFactorsToSI();
			void initializeSIUnits();

		private:
			std::string r_string;
			std::string r1_string;
			std::string lat_string;
			std::string lat1_string;
			std::string lon_string;
			std::string filename;
	};
}
#endif /* MAS_H_ */
