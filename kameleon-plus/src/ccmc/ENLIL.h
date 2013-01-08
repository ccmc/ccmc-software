/*
 * ENLIL.h
 *
 *  Created on: Jul 9, 2009
 *      Author: dberrios
 */

#ifndef ENLIL_H_
#define ENLIL_H_

#include <string>
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
			long open(const std::string& filename);
			Interpolator* createNewInterpolator();
			bool getChangeSignFlag(std::string variable);
			bool getChangeSignFlagByID(long variable_id);
			const std::vector<std::string> getLoadedVariables();

			virtual ~ENLIL();

		protected:
			void initializeConversionFactorsToSI();
			void initializeSIUnits();

		private:
			std::string r_string;
			std::string lat_string;
			std::string lon_string;
			std::string filename;

			void initializeMaps();
			boost::unordered_map<std::string, bool> changeSignFlag;
			boost::unordered_map<long, bool> changeSignFlagByID;
	};
}

#endif /* ENLIL_H_ */
