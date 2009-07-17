/*
 * OpenGGCM.h
 *
 *  Created on: Jun 2, 2009
 *      Author: David Berrios
 */

#ifndef BATSRUS_H_
#define BATSRUS_H_
#include <string>
#include "Cell3D.h"
#include "Model.h"
#include "Constants.h"
#include <vector>

/**
 * @class BATSRUS
 * @brief TODO: brief description of BATSRUS class
 *
 * TODO: full description of BATSRUS class
 */
class BATSRUS: public Model
{

	public:
		BATSRUS();
		long open(const std::string& filename);


		//Cell3D<float, float> getCell(std::string variable, float c0, float c1, float c2);
		Interpolator * createNewInterpolator();


		virtual ~BATSRUS();

		template<typename T>
		friend int binary_search(const std::vector<T>& vec, unsigned int start, unsigned int end, const T& key);

	private:
		void initializeConversionFactorsToSI();
		void initializeSIUnits();
		void initializeVariableNames();
		//std::map<std::string, float> conversionFactors;
		//float getConversionFactor(std::string);
		/*int find_octree_block(float x, float y, float z, int old_block_number);
		void set_block_min_max(int block_index);
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
		int valid_c[8];*/

		//variable strings
		std::string bx_, by_, bz_, ux_, uy_, uz_, jx_, jy_, jz_, b1x_, b1y_, b1z_, rho_, p_, e_;
		std::string block_x_min;
		std::string block_y_min;
		std::string block_z_min;
		std::string block_x_max;
		std::string block_y_max;
		std::string block_z_max;
		/*std::vector<float> * block_x_min_array;
		std::vector<float> * block_y_min_array;
		std::vector<float> * block_z_min_array;
		std::vector<float> * block_x_max_array;
		std::vector<float> * block_y_max_array;
		std::vector<float> * block_z_max_array;


		std::vector<float> * block_x_center_array;
		std::vector<float> * block_y_center_array;
		std::vector<float> * block_z_center_array;
		std::vector<int> * block_child_id_1_array;
		std::vector<int> * block_child_id_2_array;
		std::vector<int> * block_child_id_3_array;
		std::vector<int> * block_child_id_4_array;
		std::vector<int> * block_child_id_5_array;
		std::vector<int> * block_child_id_6_array;
		std::vector<int> * block_child_id_7_array;
		std::vector<int> * block_child_id_8_array;

		std::vector<int> * block_child_count_array;
		std::vector<int> * block_at_amr_level_array;
*/

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
	//	bool previousWasValid;




};

#endif /* BATSRUS_H_ */
