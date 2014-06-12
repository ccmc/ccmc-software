/*
 * LFM.h
 *
 *  Created on: Dec 4, 2012
 *      Author: Brian Curtis
 */

// #ifndef LFM_H_
// #define LFM_H_
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
			Interpolator * createNewInterpolator();
			void setResolution();
			void getResolution(int & nip1, int & njp1, int & nkp1);
			void loadPressure();
			void loadEfield();
			static int getIndex(const int i,const int j, const int k, const int ii, const int jj);
			float averageFace(const std::vector<float> * variable, int face_index,
					const int i, const int j, const int k, const int ii, const int jj);
			virtual ~LFM();
			std::vector<float> *getLFMVariable(const std::string& variable);


		protected:
			void initializeConversionFactorsToSI();
			void initializeSIUnits();

		private:
//			int nx;
//			int ny;
//			int nz;
			int nip1,njp1,nkp1,ni,nj,nk;

			std::string previousVariable;
			long previousVariableID;
			float previousConversionFactor;
			float previous_x;
			float previous_y;
			float previous_z;
			int previous_ix;
			int previous_iy;
			int previous_iz;

			std::vector<float> * x_array;
			std::vector<float> * y_array;
			std::vector<float> * z_array;

	};

}
// #endif /* LFM_H_ */
