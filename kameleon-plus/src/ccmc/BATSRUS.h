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

namespace ccmc
{
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

		protected:
			void initializeConversionFactorsToSI();
			void initializeSIUnits();

		private:

			void initializeVariableNames();


			//variable strings
			std::string bx_, by_, bz_, ux_, uy_, uz_, jx_, jy_, jz_, b1x_, b1y_, b1z_, rho_, p_, e_;
			std::string block_x_min;
			std::string block_y_min;
			std::string block_z_min;
			std::string block_x_max;
			std::string block_y_max;
			std::string block_z_max;

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
}

#endif /* BATSRUS_H_ */
