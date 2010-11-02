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
#define NVARS_ADAPT3D 9
#define NDIMN_ADAPT3D 3

namespace ccmc
{
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

			int ndimn;
			int grid_reg_no, npoin, nelem, nboun, nconi;
			long GRID_REG_NO_cdf_num, NPOIN_cdf_num, NELEM_cdf_num, NDIMN_cdf_num;
			long NBOUN_cdf_num, NCONI_cdf_num;

			bool unstructuredGridSetup;




	};
}

#endif /* ADAPT3D_H_ */
