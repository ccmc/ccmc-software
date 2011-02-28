/*
 * Adapt3D.h
 *
 *  Created on: Oct 29, 2010
 *      Author: David Berrios
 */

#ifndef ADAPT3D_H_
#define ADAPT3D_H_
#include <string>
#include "Cell3D.h"
#include "Model.h"
#include "Constants.h"
#include <vector>

#define NNODE_ADAPT3D 4
#define NVARS_ADAPT3D 12
#define NDIMN_ADAPT3D 3
#define INNER_RADIUS 1.0
#define OUTER_RADIUS 30.0

#define nx_sg 10
#define ny_sg 10
#define nz_sg 10

namespace ccmc
{
	struct SmartGridSearchValues
	{
		float xl_sg,xr_sg,yl_sg,yr_sg,zl_sg,zr_sg;
		float dx_sg,dy_sg,dz_sg;
		int *indx;
		int *esup1;
		int *esup2;
		int start_index[nz_sg][ny_sg][nx_sg];
		int end_index[nz_sg][ny_sg][nx_sg];
	};
	/**
	 * @class Adapt3D
	 * @brief TODO: brief description of Adapt3D class
	 *
	 * TODO: full description of Adapt3D class
	 */
	class Adapt3D: public Model
	{

		public:
			Adapt3D();
			long open(const std::string& filename);

			Interpolator * createNewInterpolator();
			const SmartGridSearchValues * getSmartGridSearchValues();

			virtual ~Adapt3D();

			template<typename T>
			friend int binary_search(const std::vector<T>& vec, unsigned int start, unsigned int end, const T& key);

		protected:
			void initializeConversionFactorsToSI();
			void initializeSIUnits();

		private:
			int ndimn;
			int nnode;
			int nvars;
			int grid_reg_no, npoin, nelem, nboun, nconi;
			long GRID_REG_NO_cdf_num, NPOIN_cdf_num, NELEM_cdf_num, NDIMN_cdf_num;
			long NBOUN_cdf_num, NCONI_cdf_num;
			void smartSearchSetup();
			bool setupUnstructuredGridSearch();
			SmartGridSearchValues smartSearchValues;
			const std::vector<float> * coord;
			const std::vector<int> * intmat;
			int index_2d_to_1d( int i1, int i2, int n1, int n2);





	};
}

#endif /* ADAPT3D_H_ */
