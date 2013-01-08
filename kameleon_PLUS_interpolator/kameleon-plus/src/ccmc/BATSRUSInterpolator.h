/*
 * BATSRUSInterpolator.h
 *
 *  Created on: Jun 30, 2009
 *      Author: dberrios
 */

#ifndef BATSRUSINTERPOLATOR_H_
#define BATSRUSINTERPOLATOR_H_

#include "Interpolator.h"

namespace ccmc
{
	/**
	 * @class BATSRUSInterpolator
	 * @brief TODO: brief description of BATSRUSInterpolator class
	 *
	 * TODO: full description of BATSRUSInterpolator class
	 */
	class BATSRUSInterpolator: public Interpolator
	{
		public:
			BATSRUSInterpolator(Model * modelReader);
			float interpolate(const std::string&, const float& c0, const float& c1, const float& c2);
			float interpolate(const std::string&, const float& c0, const float& c1, const float& c2, float& dc0,
					float& dc1, float& dc2);
			float interpolate(const long& variable_id, const float& c0, const float& c1, const float& c2);
			float interpolate(const long& variable_id, const float& c0, const float& c1, const float& c2, float& dc0, float& dc1,
					float& dc2);
			virtual ~BATSRUSInterpolator();

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



			float global_x_min;
			float global_x_max;
			float global_y_min;
			float global_y_max;
			float global_z_min;
			float global_z_max;
			bool previousWasValid;

	};
}

#endif /* BATSRUSINTERPOLATOR_H_ */
