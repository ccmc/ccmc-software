/*
 * Adapt3DInterpolator.h
 *
 *  Created on: Oct 29, 2010
 *      Author: dberrios
 */

#ifndef ADAPT3DINTERPOLATOR_H_
#define ADAPT3DINTERPOLATOR_H_

#include "Interpolator.h"
#include "Adapt3D.h"

#define NNODE_ADAPT3D 4
#define NVARS_ADAPT3D 9
#define NDIMN_ADAPT3D 3

#define nx_sg 10
#define ny_sg 10
#define nz_sg 10

namespace ccmc
{
	/**
	 * @class Adapt3DInterpolator
	 * @brief TODO: brief description of BATSRUSInterpolator class
	 *
	 * TODO: full description of BATSRUSInterpolator class
	 */
	class Adapt3DInterpolator: public Interpolator
	{
		public:
			Adapt3DInterpolator(Model * modelReader);
			float interpolate(const std::string&, const float& c0, const float& c1, const float& c2);
			float interpolate(const std::string&, const float& c0, const float& c1, const float& c2, float& dc0,
					float& dc1, float& dc2);
			float interpolate(long, const float& c0, const float& c1, const float& c2);
			float interpolate(long, const float& c0, const float& c1, const float& c2, float& dc0, float& dc1,
					float& dc2);
			virtual ~Adapt3DInterpolator();

		private:
			int nnode;
			int nvars;
			/* variables needed for searching unstructured grids */


			const SmartGridSearchValues *smartSearchValues;
			//int               unstructured_grid_setup_done;

			//bool setupUnstructuredGridSearchFinished;
			//bool setupUnstructuredGridSearch();
			int last_element_found;


			int ndimn, grid_reg_no, npoin, nelem;

			const vector<float> * coord;
			const vector<int> * intmat;
			const vector<float> * unkno;

			/* support routines */

			int findElement(double *cintp, int clear_cache);
			int chkineln(double * cintp ,int ielem , double * shapex);

			int smartSearch(double *search_point_coords);
			int point_within_grid( double * scoord );
			int index_2d_to_1d( int i1, int i2, int n1, int n2);
			void interpolate_adapt3d_solution(double *coord1,int ielem, double *unkno_local);

	};
}

#endif /* ADAPT3DINTERPOLATOR_H_ */
