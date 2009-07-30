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

	class CTIP: public Model
	{
		public:
			CTIP();
			long open(const std::string&);
			Interpolator* createNewInterpolator();

			virtual ~CTIP();

		protected:
			void initializeConversionFactorsToSI();
			void initializeSIUnits();
	};

}

#endif /* CTIP_H_ */