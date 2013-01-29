/*
 * LFM.h
 *
 *  Created on: Dec 4, 2012
 *      Author: Brian Curtis
 */

#ifndef LFM_H_
#define LFM_H_
#include <string>
#include "Cell3D.h"
#include "Model.h"
#include <vector>
#include <boost/unordered_map.hpp>

namespace ccmc
{
	/**
	 * @class LFM LFM.h ccmc/LFM.h
	 * @brief TODO: Brief description of LFM class
	 *
	 * TODO: Full description of LFM class
	 */
	class LFM: public Model
	{

		public:
			LFM();
			long open(const std::string& filename);

			//Cell3D<float, float> getCell(std::string& variable, float c0, float c1, float c2);
			Interpolator * createNewInterpolator();
			const std::vector<float>* const getXGrid(const std::string& variable);
			const std::vector<float>* const getXGrid(long variable);
			const std::vector<float>* const getYGrid(const std::string& variable);
			const std::vector<float>* const getYGrid(long variable);
			const std::vector<float>* const getZGrid(const std::string& variable);
			const std::vector<float>* const getZGrid(long variable);

			std::string getXGridName(const std::string& variable);
			std::string getXGridName(long variable_id);
			std::string getYGridName(const std::string& variable);
			std::string getYGridName(long variable_id);
			std::string getZGridName(const std::string& variable);
			std::string getZGridName(long variable_id);
			const std::vector<std::string> getLoadedVariables();

			//void loadVariable(std::string variable);
			virtual ~LFM();

		protected:
			void initializeConversionFactorsToSI();
			void initializeSIUnits();

		private:
//			int nx;
//			int ny;
//			int nz;
			std::vector<float> * x_array;
			std::vector<float> * y_array;
			std::vector<float> * z_array;
			std::string previousVariable;
			long previousVariableID;
			float previousConversionFactor;
			float previous_x;
			float previous_y;
			float previous_z;
			int previous_ix;
			int previous_iy;
			int previous_iz;

			void initializeMaps();


			boost::unordered_map<std::string, std::string> xGrid;
			boost::unordered_map<long, std::string> xGridByID;

			boost::unordered_map<std::string, std::string> yGrid;
			boost::unordered_map<long, std::string> yGridByID;

			boost::unordered_map<std::string, std::string> zGrid;
			boost::unordered_map<long, std::string> zGridByID;

	};

}
#endif /* LFM_H_ */
