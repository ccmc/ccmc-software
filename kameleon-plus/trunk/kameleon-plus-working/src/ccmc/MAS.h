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

			const std::vector<float>* const getRPosGrid(const std::string& variable);
			const std::vector<float>* const getRPosGrid(long variable_id);
			std::string getRPosGridName(const std::string& variable);
			std::string getRPosGridName(long variable);

			const std::vector<float>* const getLatPosGrid(const std::string& variable);
			const std::vector<float>* const getLatPosGrid(long variable_id);
			std::string getLatPosGridName(const std::string& variable);
			std::string getLatPosGridName(long variable_id);

			const std::vector<float>* const getLonPosGrid(long variable_id);
			const std::vector<float>* const getlonPosGrid(const std::string& variable);
			std::string getLonPosGridName(const std::string& variable);
			std::string getLonPosGridName(long variable_id);

			bool getChangeSignFlag(std::string variable);
			bool getChangeSignFlag(long variable_id);
			const std::vector<std::string> getLoadedVariables();
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
