/*
 * StringConstants.h
 *
 *  Created on: Mar 18, 2010
 *      Author: dberrios
 */

#ifndef STRINGCONSTANTS_H_
#define STRINGCONSTANTS_H_
#include <string>
namespace ccmc
{
	namespace strings
	{
		namespace variables
		{
			static const std::string j_ = "j";
			static const char * j_c_ = "j";

			static const std::string b_ = "b";
			static const char * b_c_ = "b";

			static const std::string jx_ = "jx";
			static const char * jx_c_ = "jx";

			static const std::string jy_ = "jy";
			static const char * jy_c_ = "jy";

			static const std::string jz_ = "jz";
			static const char * jz_c_ = "jz";

			static const std::string bx_ = "bx";
			static const char * bx_c_ = "bx";

			static const std::string by_ = "by";
			static const char * by_c_ = "by";

			static const std::string bz_ = "bz";
			static const char * bz_c_ = "bz";


			static const std::string b1x_ = "b1x";
			static const std::string b1y_ = "b1y";
			static const std::string b1z_ = "b1z";
			static const char * b1x_c_ = "b1x";
			static const char * b1y_c_ = "b1y";
			static const char * b1z_c_ = "b1z";




			static const std::string n_ = "n";
			static const std::string rho_ = "rho";
			static const std::string ux_ = "ux";
			static const std::string uy_ = "uy";
			static const std::string uz_ = "uz";
			static const std::string e_ = "e";
			static const std::string ex_ = "ex";
			static const std::string ey_ = "ey";
			static const std::string ez_ = "ez";
			static const std::string p_ = "p";

			static const char * n_c_ = "n";
			static const char * rho_c_ = "rho";
			static const char * ux_c_ = "ux";
			static const char * uy_c_ = "uy";
			static const char * uz_c_ = "uz";
			static const char * e_c_ = "e";
			static const char * ex_c_ = "ex";
			static const char * ey_c_ = "ey";
			static const char * ez_c_ = "ez";
			static const char * p_c_ = "p";

			//Adapt3D specific strings
			static const char * coord_ = "coord";
			static const char * unkno_ = "unkno";

			static const char *temp_name_ = "temp";
			static const char *intmat_name_ = "intmat";
			static const char *bconi_name_ = "bconi";

			static const char *grid_reg_no_ = "grid_reg_no";
			static const char *npoin_ = "npoin";
			static const char *nelem_ = "nelem";
			static const char *ndimn_ = "ndimn";
			static const char *nboun_ = "nboun";
			static const char *nconi_ = "nconi";

			static const std::string block_x_min_ = "block_x_min";
			static const std::string block_y_min_ = "block_y_min";
			static const std::string block_z_min_ = "block_z_min";
			static const std::string block_x_max_ = "block_x_max";
			static const std::string block_y_max_ = "block_y_max";
			static const std::string block_z_max_ = "block_z_max";
			static const std::string block_child_count_ = "block_child_count";
			static const std::string block_x_center_ = "block_x_center";
			static const std::string block_y_center_ = "block_y_center";
			static const std::string block_z_center_ = "block_z_center";
			static const std::string block_child_id_1_ = "block_child_id_1";
			static const std::string block_child_id_2_ = "block_child_id_2";
			static const std::string block_child_id_3_ = "block_child_id_3";
			static const std::string block_child_id_4_ = "block_child_id_4";
			static const std::string block_child_id_5_ = "block_child_id_5";
			static const std::string block_child_id_6_ = "block_child_id_6";
			static const std::string block_child_id_7_ = "block_child_id_7";
			static const std::string block_child_id_8_ = "block_child_id_8";
			static const std::string block_at_amr_level_ = "block_at_amr_level";

			static const char * block_x_min_c_ = "block_x_min";
			static const char * block_y_min_c_ = "block_y_min";
			static const char * block_z_min_c_ = "block_z_min";
			static const char * block_x_max_c_ = "block_x_max";
			static const char * block_y_max_c_ = "block_y_max";
			static const char * block_z_max_c_ = "block_z_max";
			static const char * block_child_count_c_ = "block_child_count";
			static const char * block_x_center_c_ = "block_x_center";
			static const char * block_y_center_c_ = "block_y_center";
			static const char * block_z_center_c_ = "block_z_center";
			static const char * block_child_id_1_c_ = "block_child_id_1";
			static const char * block_child_id_2_c_ = "block_child_id_2";
			static const char * block_child_id_3_c_ = "block_child_id_3";
			static const char * block_child_id_4_c_ = "block_child_id_4";
			static const char * block_child_id_5_c_ = "block_child_id_5";
			static const char * block_child_id_6_c_ = "block_child_id_6";
			static const char * block_child_id_7_c_ = "block_child_id_7";
			static const char * block_child_id_8_c_ = "block_child_id_8";
			static const char * block_at_amr_level_c_ = "block_at_amr_level";


		}

		namespace models
		{
			static const std::string batsrus_ = "batsrus";
			static const std::string open_ggcm_ = "open_ggcm";
			static const std::string ucla_ggcm_ = "ucla_ggcm";
			static const std::string mas_ = "mas";
			static const std::string enlil_ = "enlil";
		}
	}
}

#endif /* STRINGCONSTANTS_H_ */
