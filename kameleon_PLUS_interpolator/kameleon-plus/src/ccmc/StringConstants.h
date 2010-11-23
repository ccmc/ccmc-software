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
			//TODO: reorganize these
			//c++ strings
			static const std::string x_ = "x";
			static const std::string y_ = "y";
			static const std::string z_ = "z";

			static const std::string r_ = "r";
			static const std::string phi_ = "phi";
			static const std::string theta_ = "theta";

			static const std::string j_ = "j";
			static const std::string b_ = "b";

			static const std::string br_ = "br";
			static const std::string bphi_ = "bphi";
			static const std::string btheta_ = "btheta";

			static const std::string u_ = "u";
			static const std::string ur_ = "ur";
			static const std::string uphi_ = "uphi";
			static const std::string utheta_ = "utheta";

			static const std::string jr_ = "jr";
			static const std::string jphi_ = "jphi";
			static const std::string jtheta_ = "jtheta";

			static const std::string pram_ = "pram";
			static const std::string t_ = "t";
			static const std::string er_ = "er";
			static const std::string edotj_ = "edotj";
			static const std::string jparx_ = "jparx";
			static const std::string jpary_ = "jpary";
			static const std::string jparz_ = "jparz";
			static const std::string jpar_ = "jpar";
			static const std::string jxbx_ = "jxbx";
			static const std::string jxby_ = "jxby";
			static const std::string jxbz_ = "jxbz";
			static const std::string jxb_ = "jxb";
			static const std::string exbx_ = "exbx";
			static const std::string exby_ = "exby";
			static const std::string exbz_ = "exbz";
			static const std::string exb_ = "exb";
			static const std::string b1_ = "b1";
			static const std::string s_ = "s";
			static const std::string en_ = "en";
			static const std::string v_ = "v";
			static const std::string nv_ = "nv";
			static const std::string nvx_ = "nvx";
			static const std::string nvy_ = "nvy";
			static const std::string nvz_ = "nvz";
			static const std::string n_times_r_tothe_2_power_ = "n*r^2";
			static const std::string polb_ = "polb";
			static const std::string eta_ = "eta";
			static const std::string p_times_r_tothe_2_power_ = "p*r^2";
			static const std::string eta_times_j_ = "eta*j";
			static const std::string eta_times_jx_ = "eta*jx";
			static const std::string eta_times_jy_ = "eta*jy";
			static const std::string eta_times_jz_ = "eta*jz";
			static const std::string eta_times_jr_ = "eta*jr";
			static const std::string eta_times_jphi_ = "eta*jphi";
			static const std::string eta_times_jtheta_ = "eta*jtheta";
			static const std::string eta_times_j_tothe_2_power_ = "eta*j^2";
			static const std::string grad_n_ = "grad.n";
			static const std::string grad_b_ = "grad.b";
			static const std::string beta_ = "beta";
			static const std::string bp_ = "bp";
			static const std::string b1r_ = "b1r";
			static const std::string b1phi_ = "b1phi";
			static const std::string b1theta_ = "b1theta";
			static const std::string exbr_ = "exbr";
			static const std::string exbphi_ = "exbphi";
			static const std::string exbtheta_ = "exbtheta";
			static const std::string ephi_ = "ephi";
			static const std::string etheta_ = "etheta";

			static const std::string jparr_ = "jparr";
			static const std::string jparphi_ = "jparphi";
			static const std::string jpartheta_ = "jpartheta";
			static const std::string jxbr_ = "jxbr";
			static const std::string jxbphi_ = "jxbphi";
			static const std::string jxbtheta_ = "jxbtheta";
			static const std::string nvr_ = "nvr";
			static const std::string nvphi_ = "nvphi";
			static const std::string nvtheta_ = "nvtheta";










			//c strings
			//static const char * x_c_ = "x";
			//static const char * j_c_ = "j";

			//static const char * b_c_ = "b";

			static const std::string jx_ = "jx";
			//static const char * jx_c_ = "jx";

			static const std::string jy_ = "jy";
			//static const char * jy_c_ = "jy";

			static const std::string jz_ = "jz";
			//static const char * jz_c_ = "jz";

			static const std::string bx_ = "bx";
			//static const char * bx_c_ = "bx";

			static const std::string by_ = "by";
			//static const char * by_c_ = "by";

			static const std::string bz_ = "bz";
			//static const char * bz_c_ = "bz";


			static const std::string b1x_ = "b1x";
			static const std::string b1y_ = "b1y";
			static const std::string b1z_ = "b1z";
			//static const char * b1x_c_ = "b1x";
			//static const char * b1y_c_ = "b1y";
			//static const char * b1z_c_ = "b1z";




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

			//static const char * n_c_ = "n";
			//static const char * rho_c_ = "rho";
			//static const char * ux_c_ = "ux";
			//static const char * uy_c_ = "uy";
			//static const char * uz_c_ = "uz";
			//static const char * e_c_ = "e";
			//static const char * ex_c_ = "ex";
			//static const char * ey_c_ = "ey";
			//static const char * ez_c_ = "ez";
			//static const char * p_c_ = "p";

			//Adapt3D specific strings
			static const std::string coord_ = "coord";
			static const std::string unkno_ = "unkno";

			static const std::string temp_ = "temp";
			static const std::string intmat_ = "intmat";
			static const std::string bconi_ = "bconi";

			static const std::string grid_reg_no_ = "grid_reg_no";
			static const std::string npoin_ = "npoin";
			static const std::string nelem_ = "nelem";
			static const std::string ndimn_ = "ndimn";
			static const std::string nboun_ = "nboun";
			static const std::string nconi_ = "nconi";

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
			static const std::string adapt3d_ = "ADAPT3D";
		}

		namespace attributes
		{
			static const std::string actual_min_ = "actual_min";
			static const std::string actual_max_ = "actual_max";
			static const std::string global_x_min_ = "global_x_min";
			static const std::string global_y_min_ = "global_y_min";
			static const std::string global_z_min_ = "global_z_min";
			static const std::string global_x_max_ = "global_x_max";
			static const std::string global_y_max_ = "global_y_max";
			static const std::string global_z_max_ = "global_z_max";
		}
	}
}

#endif /* STRINGCONSTANTS_H_ */
