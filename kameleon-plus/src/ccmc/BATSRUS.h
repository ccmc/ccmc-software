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
	 * @class BATSRUS
	 * @brief TODO: brief description of BATSRUS class
	 *
	 * TODO: full description of BATSRUS class
	 */
	class BATSRUS: public Model
	{

		public:
			BATSRUS();
			long open(const std::string& filename);

			//Cell3D<float, float> getCell(std::string variable, float c0, float c1, float c2);
			Interpolator * createNewInterpolator();

			virtual ~BATSRUS();

			template<typename T>
			friend int binary_search(const std::vector<T>& vec, unsigned int start, unsigned int end, const T& key);

		protected:
			void initializeConversionFactorsToSI();
			void initializeSIUnits();



			//variable strings

			//	bool previousWasValid;


	};
}

#endif /* BATSRUS_H_ */
