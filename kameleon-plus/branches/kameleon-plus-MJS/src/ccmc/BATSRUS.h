/*
 * OpenGGCM.h
 *
 *  Created on: Jun 2, 2009
 *      Author: David Berrios
 */

#ifndef BATSRUS_H_
#define BATSRUS_H_
#include <string>
#include "Cell3D.h"
#include "Model.h"
#include "Constants.h"
#include <vector>

namespace ccmc
{
	/**
	 * @class BATSRUS BATSRUS.h ccmc/BATSRUS.h
	 * @brief TODO: brief description of BATSRUS class
	 *
	 * TODO: full description of BATSRUS class
	 */
	class BATSRUS: public Model
	{

		public:
			BATSRUS();
			long open(const std::string& filename);

			Interpolator * createNewInterpolator();
			const std::vector<std::string> getLoadedVariables();
			virtual ~BATSRUS();

			template<typename T>
			friend int binary_search(const std::vector<T>& vec, unsigned int start, unsigned int end, const T& key);

		protected:
			void initializeConversionFactorsToSI();
			void initializeSIUnits();

	};
}

#endif /* BATSRUS_H_ */
