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
#define NDIMN_ADAPT3D 3
#define DELAUNEY_SEARCH

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
			float interpolate(const long& variable_id, const float& c0, const float& c1, const float& c2);
			float interpolate(const long& variable_id, const float& c0, const float& c1, const float& c2, float& dc0, float& dc1,
					float& dc2);
			static void calculation1(const float& a, const float& b, const float& c, const float& d, const float& e, float& result);
			virtual ~Adapt3DInterpolator();

		private:
			int nnode;

			/* variables needed for searching unstructured grids */

//			int numThreads;
			SmartGridSearchValues *smartSearchValues;
			//int               unstructured_grid_setup_done;

			//bool setupUnstructuredGridSearchFinished;
			//bool setupUnstructuredGridSearch();



			int ndimn, grid_reg_no, npoin, nelem;

			const std::vector<float> * coord;
			const std::vector<int> * intmat;
			float previous_c0;
			float previous_c1;
			float previous_c2;
			int last_node_found;
			int clear_cache;
			//const std::vector<float> * unkno;

			/* support routines */

			int findElement(const float& c0, const float& c1, const float& c2, int clear_cache);
			int chkineln(const float& c0, const float& c1, const float& c2, int ielem , float * shapex);

			int smartSearch(const float& c0, const float& c1, const float& c2);
			int point_within_grid( const float * scoord );
			int point_within_grid( const float& c0, const float& c1, const float& c2);
			int index_2d_to_1d( int i1, int i2, int n2);
			float interpolate_adapt3d_solution(const float& c0, const float& c1, const float& c2,int ielem, const std::string& variable);

	};
}

#endif /* ADAPT3DINTERPOLATOR_H_ */
