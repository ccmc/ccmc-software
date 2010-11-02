/*
 * Adapt3DInterpolator.h
 *
 *  Created on: Oct 29, 2010
 *      Author: dberrios
 */

#ifndef ADAPT3DINTERPOLATOR_H_
#define ADAPT3DINTERPOLATOR_H_

#include "Interpolator.h"

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
			int find_octree_block(float x, float y, float z, int old_block_number);
			int climb_octree(int root, float x, float y, float z);
			int nx;
			int ny;
			int nz;
			int old_block;
			int callCount;
			float old_x;
			float old_y;
			float old_z;
			long int ib, ib_c[8], ix_c[8], iy_c[8], iz_c[8];
			float xx_c[8], yy_c[8], zz_c[8];
			int valid_c[8];
			std::string block_x_min;
			std::string block_y_min;
			std::string block_z_min;
			std::string block_x_max;
			std::string block_y_max;
			std::string block_z_max;
			const std::vector<float> * block_x_min_array;
			const std::vector<float> * block_y_min_array;
			const std::vector<float> * block_z_min_array;
			const std::vector<float> * block_x_max_array;
			const std::vector<float> * block_y_max_array;
			const std::vector<float> * block_z_max_array;

			const std::vector<float> * block_x_center_array;
			const std::vector<float> * block_y_center_array;
			const std::vector<float> * block_z_center_array;
			const std::vector<int> * block_child_id_1_array;
			const std::vector<int> * block_child_id_2_array;
			const std::vector<int> * block_child_id_3_array;
			const std::vector<int> * block_child_id_4_array;
			const std::vector<int> * block_child_id_5_array;
			const std::vector<int> * block_child_id_6_array;
			const std::vector<int> * block_child_id_7_array;
			const std::vector<int> * block_child_id_8_array;

			const std::vector<int> * block_child_count_array;
			const std::vector<int> * block_at_amr_level_array;

			std::string block_child_count;
			std::string block_x_center;
			std::string block_y_center;
			std::string block_z_center;
			std::string block_child_id_1;
			std::string block_child_id_2;
			std::string block_child_id_3;
			std::string block_child_id_4;
			std::string block_child_id_5;
			std::string block_child_id_6;
			std::string block_child_id_7;
			std::string block_child_id_8;
			std::string block_at_amr_level;

			float global_x_min;
			float global_x_max;
			float global_y_min;
			float global_y_max;
			float global_z_min;
			float global_z_max;
			bool previousWasValid;

#define NNODE_ADAPT3D 4
#define NVARS_ADAPT3D 9
#define NDIMN_ADAPT3D 3

#define nx_sg 10
#define ny_sg 10
#define nz_sg 10


			int nnode;
			int nvars;
			/* variables needed for searching unstructured grids */
			double            xl_sg,xr_sg,yl_sg,yr_sg,zl_sg,zr_sg;
			double            dx_sg,dy_sg,dz_sg;
			int               start_index[nz_sg][ny_sg][nx_sg];
			int               end_index[nz_sg][ny_sg][nx_sg];
			int               *indx;
			int               *esup1;
			int               *esup2;
			int               unstructured_grid_setup_done;
			int               last_element_found;

			/* support routines */
			int setup_search_unstructured_grid();
			int find_element(double cintp[NDIMN_ADAPT3D], int clear_cache);
			int chkineln(double cintp[NDIMN_ADAPT3D] ,int ielem , double shapex[NNODE_ADAPT3D]);
			void smart_search_setup();
			int smart_search(double search_point_coords[NDIMN_ADAPT3D]);
			int point_within_grid( double scoord[NDIMN_ADAPT3D] );
			int index_2d_to_1d( int i1, int i2, int n1, int n2);

	};
}

#endif /* ADAPT3DINTERPOLATOR_H_ */
