/*
 * CTIP.h
 *
 *  Created on: Jul 24, 2009
 *      Author: dberrios
 */

#ifndef CTIP_H_
#define CTIP_H_

/*
 *
 */
#include "Model.h"

namespace ccmc
{

	/**
	 * @class CTIP CTIP.h ccmc/CTIP.h
	 * @brief TODO: Brief description of CTIP class
	 *
	 * TODO: Full description of CTIP class
	 */
	class CTIP: public Model
	{
		public:
			CTIP();
			long open(const std::string& filename);
			Interpolator* createNewInterpolator();

			virtual ~CTIP();

		protected:
			void initializeConversionFactorsToSI();
			void initializeSIUnits();
	};

}

#endif /* CTIP_H_ */
