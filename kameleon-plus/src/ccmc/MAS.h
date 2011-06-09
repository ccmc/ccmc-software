/*
 * MAS.h
 *
 *  Created on: Jul 9, 2009
 *      Author: dberrios
 */

#ifndef MAS_H_
#define MAS_H_

#include "Model.h"
#include <boost/unordered_map.hpp>

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
			long open(const std::string& filename);
			Interpolator* createNewInterpolator();
			const std::vector<float>* getRPosGrid(std::string variable);
			const std::vector<float>* getLatPosGrid(std::string variable);
			const std::vector<float>* getRPosGridByID(long variable);
			const std::vector<float>* getLatPosGridByID(long variable);
			const std::vector<float>* getLonPosGridByID(long variable_id);
			const std::vector<float>* getlonPosGrid(std::string variable);
			bool getChangeSignFlag(std::string variable);
			bool getChangeSignFlagByID(long variable_id);
			virtual ~MAS();

		protected:
			void initializeConversionFactorsToSI();
			void initializeSIUnits();

		private:

			void initializeMaps();

			boost::unordered_map<std::string, std::string> rPosGrid;
			boost::unordered_map<long, std::string> rPosGridByID;

			boost::unordered_map<std::string, std::string> latPosGrid;
			boost::unordered_map<long, std::string> latPosGridByID;

			boost::unordered_map<std::string, std::string> lonPosGrid;
			boost::unordered_map<long, std::string> lonPosGridByID;

			boost::unordered_map<std::string, bool> changeSignFlag;
			boost::unordered_map<long, bool> changeSignFlagByID;

	};
}
#endif /* MAS_H_ */
