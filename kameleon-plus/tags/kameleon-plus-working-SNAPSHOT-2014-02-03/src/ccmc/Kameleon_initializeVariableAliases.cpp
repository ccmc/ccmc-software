#include "Kameleon.h"

namespace ccmc
{
	/**
	 * Specifies aliases for variables included in the CDF files, or used by the derived
	 * library.  Add variable names to make the library more convenient, as long as those
	 * variable names map to one of the known variables used by the library.  These should
	 * probably be read from an external file also, so the aliases could be updated
	 * without having to recompile the library.
	 *
	 * Ideally, we should probably create a map to an enumerated list of possible variables.
	 *
	 * I need to remove the variables that only differ by case.  I modified the interpolation to
	 * change the case of all characters of the input string to lower case.
	 */
	void Kameleon::initializeVariableAliases()
	{
		//create aliases used in the visualization programs here.
		if (modelName == "ucla_ggcm" || modelName == "open_ggcm")
		{
			//visualization variable names
			variableAliases["N"] = "n";
			variableAliases["T"] = "t";
			variableAliases["P"] = "p";
			variableAliases["B_x"] = "ux";
			variableAliases["B_y"] = "uy";
			variableAliases["B_z"] = "uz";
			variableAliases["J_x"] = "jx";
			variableAliases["J_y"] = "jy";
			variableAliases["J_z"] = "jz";
			variableAliases["E_x"] = "ex";
			variableAliases["E_y"] = "ey";
			variableAliases["E_z"] = "ez";
			variableAliases["E"] = "e";
			variableAliases["S"] = "s";
			variableAliases["B"] = "b";
			//varialbeAliases["NV"] = "nv";
			variableAliases["J"] = "j";
			variableAliases["P_ram"] = "pram";
			variableAliases["J_par_x"] = "jparx";
			variableAliases["J_par_y"] = "jpary";
			variableAliases["J_par_z"] = "jparz";
			variableAliases["JxB_x"] = "jxbx";
			variableAliases["JxB_y"] = "jxby";
			variableAliases["JxB_z"] = "jxbz";
			variableAliases["JxB"] = "jxb";
			variableAliases["ExB_x"] = "exbx";
			variableAliases["ExB_y"] = "exby";
			variableAliases["ExB_z"] = "exbz";
			variableAliases["ExB"] = "exb";
			variableAliases["E.J"] = "edotj";

			//u
			variableAliases["vx"] = "ux";
			variableAliases["vy"] = "uy";
			variableAliases["vz"] = "uz";

			variableAliases["V"] = "u";
			variableAliases["V_x"] = "ux";
			variableAliases["V_y"] = "uy";
			variableAliases["V_z"] = "uz";

		} else if (modelName == "batsrus")
		{
			//visualization variable names
			variableAliases["N"] = "n";
			//varialbeAliases["NV"] = "nv";
			variableAliases["T"] = "t";
			variableAliases["P"] = "p";
			variableAliases["B_x"] = "bx";
			variableAliases["B_y"] = "by";
			variableAliases["B_z"] = "bz";
			variableAliases["J_x"] = "jx";
			variableAliases["J_y"] = "jy";
			variableAliases["J_z"] = "jz";
			variableAliases["E_x"] = "ex";
			variableAliases["E_y"] = "ey";
			variableAliases["E_z"] = "ez";
			variableAliases["E"] = "e";
			variableAliases["S"] = "s";
			variableAliases["B"] = "b";
			variableAliases["J"] = "j";
			variableAliases["P_ram"] = "pram";
			variableAliases["J_par_x"] = "jparx";
			variableAliases["J_par_y"] = "jpary";
			variableAliases["J_par_z"] = "jparz";
			variableAliases["JxB_x"] = "jxbx";
			variableAliases["JxB_y"] = "jxby";
			variableAliases["JxB_z"] = "jxbz";
			variableAliases["JxB"] = "jxb";
			variableAliases["ExB_x"] = "exbx";
			variableAliases["ExB_y"] = "exby";
			variableAliases["ExB_z"] = "exbz";
			variableAliases["ExB"] = "exb";
			variableAliases["E.J"] = "edotj";

			//u
			variableAliases["vx"] = "ux";
			variableAliases["vy"] = "uy";
			variableAliases["vz"] = "uz";

			variableAliases["V"] = "u";
			variableAliases["V_x"] = "ux";
			variableAliases["V_y"] = "uy";
			variableAliases["V_z"] = "uz";

		} else if (modelName == "mas")

		{

			variableAliases["N"] = "n";
			variableAliases["T"] = "t";
			variableAliases["P"] = "p";

			variableAliases["vr"] = "ur";
			variableAliases["vphi"] = "uphi";
			variableAliases["vtheta"] = "utheta";

			variableAliases["V_r"] = "ur";
			variableAliases["V_lon"] = "uphi";
			variableAliases["V_lat"] = "utheta";

			variableAliases["B_r"] = "br";
			variableAliases["B_lon"] = "bphi";
			variableAliases["B_lat"] = "btheta";

			variableAliases["J_r"] = "jr";
			variableAliases["J_lon"] = "jphi";
			variableAliases["J_lat"] = "jtheta";

			variableAliases["B"] = "b";
			variableAliases["V"] = "u";
			variableAliases["J"] = "j";

			variableAliases["E_r"] = "er";
			variableAliases["E_lon"] = "ephi";
			variableAliases["E_lat"] = "etheta";

			variableAliases["E"] = "e";
			variableAliases["S"] = "s";
			variableAliases["P_ram"] = "pram";

			variableAliases["J_par_r"] = "jparr";
			variableAliases["J_par_lon"] = "jparphi";
			variableAliases["J_par_lat"] = "jpartheta";

			variableAliases["J_par"] = "jpar";
			variableAliases["JxB_r"] = "jxbr";
			variableAliases["JxB_lon"] = "jxbphi";
			variableAliases["JxB_lat"] = "jxbtheta";
			variableAliases["JxB"] = "jxb";
			variableAliases["ExB_r"] = "exbr";
			variableAliases["ExB_lon"] = "exbphi";
			variableAliases["ExB_lat"] = "exbtheta";
			variableAliases["ExB"] = "exb";
			variableAliases["E.J"] = "edotj";
			variableAliases["NV_r"] = "nvr";
			variableAliases["NV_lon"] = "nvphi";
			variableAliases["NV_lat"] = "nvtheta";
			variableAliases["NV_lat"] = "nv";

		} else if (modelName == "enlil")
		{

			variableAliases["N"] = "n";
			variableAliases["T"] = "t";
			variableAliases["P"] = "p";

			variableAliases["vr"] = "ur";
			variableAliases["vtheta"] = "utheta";
			variableAliases["vphi"] = "uphi";

			variableAliases["V_r"] = "ur";
			variableAliases["V_lon"] = "utheta";
			variableAliases["V_lat"] = "uphi";

			variableAliases["B_r"] = "br";
			variableAliases["B_lon"] = "btheta";
			variableAliases["B_lat"] = "bphi";

			variableAliases["N*r^2"] = "n*r^2";

		}

	}
}
