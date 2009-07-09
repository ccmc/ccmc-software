/*
 * MAS.h
 *
 *  Created on: Jul 9, 2009
 *      Author: dberrios
 */

#ifndef MAS_H_
#define MAS_H_

#include "Model.h"

class MAS: public Model
{
	public:
		MAS();
		long open(const std::string&);
		Interpolator* createNewInterpolator();
		void initializeConversionFactorsToSI();
		void initializeSIUnits();
		virtual ~MAS();

	private:
		std::string r_string;
		std::string lat_string;
		std::string lon_string;
		std::string filename;
};

#endif /* MAS_H_ */
