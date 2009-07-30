#include "Kameleon.h"

namespace ccmc
{
	/**
	 * Initializes the list of required variables for the each derived variable,
	 * both for the individual component, and for the vectors.
	 *
	 */
	void Kameleon::initializeListOfRequiredVariablesForComponentsAndVectors()
	{
		//base variables

		char * bx_ = "bx";
		char * by_ = "by";
		char * bz_ = "bz";
		char * ux_ = "ux";
		char * uy_ = "uy";
		char * uz_ = "uz";
		char * jx_ = "jx";
		char * jy_ = "jy";
		char * jz_ = "jz";
		char * rho_ = "rho";
		char * p_ = "p";
		/*std::string pram[] = {rho_, ux_, uy_, uz_};
		 std::string t[] = {p_, rho_};
		 std::string ex[] = {by_,bz_,uy_,uz_};
		 std::string e_r[] = {by_,bz_,uy_,uz_};
		 std::string ey[] = {bx_,bz_,ux_,uz_};
		 std::string e_lat[] = {bx_,bz_,ux_,uz_};
		 std::string ez[] = {bx_,by_,ux_,uy_};
		 std::string e_lon[] = {bx_,by_,ux_,uy_};
		 std::string edotj[] = {jx_,jy_,jz_,bx_,by_,bz_,ux_,uy_,uz_};
		 std::string jparx[] = {jx_,jy_,jz_,bx_,by_,bz_};
		 std::string jpary[] = {jx_,jy_,jz_,bx_,by_,bz_};
		 std::string jparz[] = {jx_,jy_,jz_,bx_,by_,bz_};
		 std::string jparr[] = {"jr","jphi","jtheta","br","bphi","btheta"};
		 std::string jparphi[] = {"jr","jphi","jtheta","br","bphi","btheta"};
		 std::string jpartheta[] = {"jr","jphi","jtheta","br","bphi","btheta"};
		 std::string jpar[] = {jx_,jy_,jz_,bx_,by_,bz_};
		 std::string jxbx[] = {by_,bz_,jy_,jz_};
		 std::string jxby[] = {bx_,bz_,jx_,jz_};
		 std::string jxbz[] = {bx_,by_,jx_,jy_};
		 std::string jxb[] = {bx_,by_,bz_,jx_,jy_,jz_};
		 std::string exbx[] = {bx_,by_,bz_,ux_,uy_,uz_};
		 std::string exby[] = {bx_,by_,bz_,ux_,uy_,uz_};
		 std::string exbz[] = {bx_,by_,bz_,ux_,uy_,uz_};
		 std::string exb[] = {bx_,by_,bz_,ux_,uy_,uz_};
		 std::string exb_r[] = {bx_,by_,bz_,ux_,uy_,uz_};
		 std::string exb_lat[] = {bx_,by_,bz_,ux_,uy_,uz_};
		 std::string exb_lon[] = {bx_,by_,bz_,ux_,uy_,uz_};
		 std::string j[] = {jx_,jy_,jz_};
		 std::string b[] = {bx_,by_,bz_};
		 std::string b1[] = {"b1x","b1y","b1z"};
		 std::string b_r[] = {bx_};
		 std::string b_lat[] = {by_};
		 std::string b_lon[] = {bz_};
		 std::string u[] = {ux_,uy_,uz_};
		 std::string v[] = {ux_,uy_,uz_};
		 std::string v_r[] = {ux_};
		 std::string v_lat[] = {uy_};
		 std::string v_lon[] = {uz_};
		 std::string s[] = {p_,rho_};
		 std::string en[] = {"e"};
		 std::string e[] = {bx_,by_,bz_,ux_,uy_,uz_};
		 std::string nv[] = {rho_,ux_,uy_,uz_};
		 std::string nvx[] = {rho_,ux_};
		 std::string nvy[] = {rho_,uy_};
		 std::string nvz[] = {rho_,uz_};
		 std::string n[] = {rho_};
		 std::string n_times_r_tothe_2_power[] = {rho_};
		 std::string polb[] = {"bp"};
		 std::string p_times_r_tothe_2_power[] = {p_};
		 std::string eta_times_jx[] = {"eta", jx_};
		 std::string eta_times_jy[] = {"eta", jy_};
		 std::string eta_times_jz[] = {"eta", jz_};
		 std::string eta_times_j_tothe_2"] = createVectorFromList(4, "eta", jx_, jy_, jz_);
		 */

		listOfRequiredVariablesForComponents["pram"] = createVectorFromList(4, rho_, ux_, uy_, uz_);
		listOfRequiredVariablesForComponents["t"] = createVectorFromList(2, p_, rho_);
		listOfRequiredVariablesForComponents["ex"] = createVectorFromList(4, by_, bz_, uy_, uz_);
		listOfRequiredVariablesForComponents["e_r"] = createVectorFromList(4, by_, bz_, uy_, uz_);
		listOfRequiredVariablesForComponents["ey"] = createVectorFromList(4, bx_, bz_, ux_, uz_);
		listOfRequiredVariablesForComponents["e_lat"] = createVectorFromList(4, bx_, bz_, ux_, uz_);
		listOfRequiredVariablesForComponents["ez"] = createVectorFromList(4, bx_, by_, ux_, uy_);
		listOfRequiredVariablesForComponents["e_lon"] = createVectorFromList(4, bx_, by_, ux_, uy_);
		listOfRequiredVariablesForComponents["edotj"] = createVectorFromList(9, jx_, jy_, jz_, bx_, by_, bz_, ux_, uy_,
				uz_);
		listOfRequiredVariablesForComponents["jparx"] = createVectorFromList(6, jx_, jy_, jz_, bx_, by_, bz_);
		listOfRequiredVariablesForComponents["jpary"] = createVectorFromList(6, jx_, jy_, jz_, bx_, by_, bz_);
		listOfRequiredVariablesForComponents["jparz"] = createVectorFromList(6, jx_, jy_, jz_, bx_, by_, bz_);
		listOfRequiredVariablesForComponents["jparr"] = createVectorFromList(6, "jr", "jphi", "jtheta", "br", "bphi",
				"btheta");
		listOfRequiredVariablesForComponents["jparphi"] = createVectorFromList(6, "jr", "jphi", "jtheta", "br", "bphi",
				"btheta");
		listOfRequiredVariablesForComponents["jpartheta"] = createVectorFromList(6, "jr", "jphi", "jtheta", "br",
				"bphi", "btheta");
		listOfRequiredVariablesForComponents["jpar"] = createVectorFromList(6, jx_, jy_, jz_, bx_, by_, bz_);
		listOfRequiredVariablesForComponents["jxbx"] = createVectorFromList(4, by_, bz_, jy_, jz_);
		listOfRequiredVariablesForComponents["jxby"] = createVectorFromList(4, bx_, bz_, jx_, jz_);
		listOfRequiredVariablesForComponents["jxbz"] = createVectorFromList(4, bx_, by_, jx_, jy_);
		listOfRequiredVariablesForComponents["jxb"] = createVectorFromList(6, bx_, by_, bz_, jx_, jy_, jz_);
		listOfRequiredVariablesForComponents["exbx"] = createVectorFromList(6, bx_, by_, bz_, ux_, uy_, uz_);
		listOfRequiredVariablesForComponents["exby"] = createVectorFromList(6, bx_, by_, bz_, ux_, uy_, uz_);
		listOfRequiredVariablesForComponents["exbz"] = createVectorFromList(6, bx_, by_, bz_, ux_, uy_, uz_);
		listOfRequiredVariablesForComponents["exb"] = createVectorFromList(6, bx_, by_, bz_, ux_, uy_, uz_);
		listOfRequiredVariablesForComponents["exb_r"] = createVectorFromList(6, bx_, by_, bz_, ux_, uy_, uz_);
		listOfRequiredVariablesForComponents["exb_lat"] = createVectorFromList(6, bx_, by_, bz_, ux_, uy_, uz_);
		listOfRequiredVariablesForComponents["exb_lon"] = createVectorFromList(6, bx_, by_, bz_, ux_, uy_, uz_);
		listOfRequiredVariablesForComponents["j"] = createVectorFromList(3, jx_, jy_, jz_);
		listOfRequiredVariablesForComponents["b"] = createVectorFromList(3, bx_, by_, bz_);
		listOfRequiredVariablesForComponents["b1"] = createVectorFromList(3, "b1x", "b1y", "b1z");
		listOfRequiredVariablesForComponents["b_r"] = createVectorFromList(1, bx_);
		listOfRequiredVariablesForComponents["b_lat"] = createVectorFromList(1, by_);
		listOfRequiredVariablesForComponents["b_lon"] = createVectorFromList(1, bz_);
		listOfRequiredVariablesForComponents["u"] = createVectorFromList(3, ux_, uy_, uz_);
		listOfRequiredVariablesForComponents["v"] = createVectorFromList(3, ux_, uy_, uz_);
		listOfRequiredVariablesForComponents["v_r"] = createVectorFromList(1, ux_);
		listOfRequiredVariablesForComponents["v_lat"] = createVectorFromList(1, uy_);
		listOfRequiredVariablesForComponents["v_lon"] = createVectorFromList(1, uz_);
		listOfRequiredVariablesForComponents["s"] = createVectorFromList(2, p_, rho_);
		listOfRequiredVariablesForComponents["en"] = createVectorFromList(1, "e");
		listOfRequiredVariablesForComponents["e"] = createVectorFromList(6, bx_, by_, bz_, ux_, uy_, uz_);
		listOfRequiredVariablesForComponents["nv"] = createVectorFromList(4, rho_, ux_, uy_, uz_);
		listOfRequiredVariablesForComponents["nvx"] = createVectorFromList(2, rho_, ux_);
		listOfRequiredVariablesForComponents["nvy"] = createVectorFromList(2, rho_, uy_);
		listOfRequiredVariablesForComponents["nvz"] = createVectorFromList(2, rho_, uz_);
		listOfRequiredVariablesForComponents["n"] = createVectorFromList(1, rho_);
		listOfRequiredVariablesForComponents["n*r^2"] = createVectorFromList(1, rho_);
		listOfRequiredVariablesForComponents["polb"] = createVectorFromList(1, "bp");
		listOfRequiredVariablesForComponents["p*r^2"] = createVectorFromList(1, p_);
		listOfRequiredVariablesForComponents["eta*jx"] = createVectorFromList(2, "eta", jx_);
		listOfRequiredVariablesForComponents["eta*jy"] = createVectorFromList(2, "eta", jy_);
		listOfRequiredVariablesForComponents["eta*jz"] = createVectorFromList(2, "eta", jz_);
		listOfRequiredVariablesForComponents["eta*j^2"] = createVectorFromList(4, "eta", jx_, jy_, jz_);

		if (model->doesVariableExist(p_))
		{

			listOfRequiredVariablesForComponents[p_] = createVectorFromList(1, p_);
			listOfRequiredVariablesForComponents["beta"] = createVectorFromList(4, bx_, by_, bz_, p_);
		} else
		{
			if (model->doesVariableExist("T"))
			{
				listOfRequiredVariablesForComponents[p_] = createVectorFromList(2, rho_, "T");
				listOfRequiredVariablesForComponents["beta"] = createVectorFromList(5, bx_, by_, bz_, rho_, "T");
			} else if (model->doesVariableExist("t"))
			{
				listOfRequiredVariablesForComponents[p_] = createVectorFromList(2, rho_, "t");
				listOfRequiredVariablesForComponents["beta"] = createVectorFromList(5, bx_, by_, bz_, rho_, "t");
			} else
			{
				//not sure whatelse to do.
				listOfRequiredVariablesForComponents[p_] = createVectorFromList(1, rho_);
				listOfRequiredVariablesForComponents["beta"] = createVectorFromList(4, bx_, by_, bz_, rho_);
			}
		}

		if (modelName == "enlil")
		{
			listOfRequiredVariablesForComponents["b"] = createVectorFromList(3, "b1x", "b1y", "b1z");
			listOfRequiredVariablesForComponents[bx_] = createVectorFromList(1, "b1x");
			listOfRequiredVariablesForComponents[by_] = createVectorFromList(1, "b1y");
			listOfRequiredVariablesForComponents[bz_] = createVectorFromList(1, "b1z");
			listOfRequiredVariablesForComponents["b1"] = createVectorFromList(3, "b1x", "b1y", "b1z");
			listOfRequiredVariablesForComponents["b1x"] = createVectorFromList(1, "b1x");
			listOfRequiredVariablesForComponents["b1y"] = createVectorFromList(1, "b1y");
			listOfRequiredVariablesForComponents["b1z"] = createVectorFromList(1, "b1z");
			listOfRequiredVariablesForComponents["br"] = createVectorFromList(1, "b1x");
			listOfRequiredVariablesForComponents["btheta"] = createVectorFromList(1, "b1y");
			listOfRequiredVariablesForComponents["bphi"] = createVectorFromList(1, "b1z");
			listOfRequiredVariablesForComponents["e"] = createVectorFromList(6, "b1x", "b1y", "b1z", ux_, uy_, uz_);
			listOfRequiredVariablesForComponents["ex"] = createVectorFromList(4, "b1y", "b1z", uy_, uz_);
			listOfRequiredVariablesForComponents["e_r"] = createVectorFromList(4, "b1y", "b1z", uy_, uz_);
			listOfRequiredVariablesForComponents["ey"] = createVectorFromList(4, "b1x", "b1z", ux_, uz_);
			listOfRequiredVariablesForComponents["e_lat"] = createVectorFromList(4, "b1x", "b1z", ux_, uz_);
			listOfRequiredVariablesForComponents["ez"] = createVectorFromList(4, "b1x", "b1y", ux_, uy_);
		}
		if (modelName == "mas")
		{
			listOfRequiredVariablesForComponents["pram"] = createVectorFromList(4, rho_, "ur", "uphi", "utheta");
			listOfRequiredVariablesForComponents["exb"] = createVectorFromList(6, "br", "bphi", "btheta", "ur", "uphi",
					"utheta");
			listOfRequiredVariablesForComponents["exbr"] = createVectorFromList(6, "br", "bphi", "btheta", "ur",
					"uphi", "utheta");
			listOfRequiredVariablesForComponents["exbtheta"] = createVectorFromList(6, "br", "bphi", "btheta", "ur",
					"uphi", "utheta");
			listOfRequiredVariablesForComponents["exbphi"] = createVectorFromList(6, "br", "bphi", "btheta", "ur",
					"uphi", "utheta");
			listOfRequiredVariablesForComponents["edotj"] = createVectorFromList(9, "jr", "jphi", "jtheta", "br",
					"bphi", "btheta", "ur", "uphi", "utheta");
			listOfRequiredVariablesForComponents["v"] = createVectorFromList(3, "ur", "uphi", "utheta");
			listOfRequiredVariablesForComponents["j"] = createVectorFromList(3, "jr", "jphi", "jtheta");
			listOfRequiredVariablesForComponents["b"] = createVectorFromList(3, "br", "bphi", "btheta");
			listOfRequiredVariablesForComponents["er"] = createVectorFromList(4, "bphi", "btheta", "utheta", "uphi");
			listOfRequiredVariablesForComponents["ephi"] = createVectorFromList(4, "br", "bphi", "ur", "uphi");
			listOfRequiredVariablesForComponents["etheta"] = createVectorFromList(4, "br", "btheta", "ur", "utheta");

		} else
		{

		}

		//add variables for vectors.  same as above, except, for vectors, we want to add all components
		listOfRequiredVariablesForVectors["pram"] = createVectorFromList(4, rho_, ux_, uy_, uz_);
		listOfRequiredVariablesForVectors["t"] = createVectorFromList(2, p_, rho_);
		listOfRequiredVariablesForVectors["ex"] = createVectorFromList(6, bx_, by_, bz_, ux_, uy_, uz_);
		listOfRequiredVariablesForVectors["e_r"] = createVectorFromList(6, bx_, by_, bz_, ux_, uy_, uz_);
		listOfRequiredVariablesForVectors["ey"] = createVectorFromList(6, bx_, by_, bz_, ux_, uy_, uz_);
		listOfRequiredVariablesForVectors["e_lat"] = createVectorFromList(6, bx_, by_, bz_, ux_, uy_, uz_);
		listOfRequiredVariablesForVectors["ez"] = createVectorFromList(6, bx_, by_, bz_, ux_, uy_, uz_);
		listOfRequiredVariablesForVectors["e_lon"] = createVectorFromList(6, bx_, by_, bz_, ux_, uy_, uz_);
		listOfRequiredVariablesForVectors["edotj"] = createVectorFromList(9, jx_, jy_, jz_, bx_, by_, bz_, ux_, uy_,
				uz_);
		listOfRequiredVariablesForVectors["jparx"] = createVectorFromList(6, jx_, jy_, jz_, bx_, by_, bz_);
		listOfRequiredVariablesForVectors["jpary"] = createVectorFromList(6, jx_, jy_, jz_, bx_, by_, bz_);
		listOfRequiredVariablesForVectors["jparz"] = createVectorFromList(6, jx_, jy_, jz_, bx_, by_, bz_);
		listOfRequiredVariablesForVectors["jpar"] = createVectorFromList(6, jx_, jy_, jz_, bx_, by_, bz_);
		listOfRequiredVariablesForVectors["jxbx"] = createVectorFromList(6, bx_, by_, bz_, jx_, jy_, jz_);
		listOfRequiredVariablesForVectors["jxby"] = createVectorFromList(6, bx_, by_, bz_, jx_, jy_, jz_);
		listOfRequiredVariablesForVectors["jxbz"] = createVectorFromList(6, bx_, by_, bz_, jx_, jy_, jz_);
		listOfRequiredVariablesForVectors["jxb"] = createVectorFromList(6, bx_, by_, bz_, jx_, jy_, jz_);
		listOfRequiredVariablesForVectors["exbx"] = createVectorFromList(6, bx_, by_, bz_, ux_, uy_, uz_);
		listOfRequiredVariablesForVectors["exby"] = createVectorFromList(6, bx_, by_, bz_, ux_, uy_, uz_);
		listOfRequiredVariablesForVectors["exbz"] = createVectorFromList(6, bx_, by_, bz_, ux_, uy_, uz_);
		listOfRequiredVariablesForVectors["exb"] = createVectorFromList(6, bx_, by_, bz_, ux_, uy_, uz_);
		listOfRequiredVariablesForVectors["exb_r"] = createVectorFromList(6, bx_, by_, bz_, ux_, uy_, uz_);
		listOfRequiredVariablesForVectors["exb_lat"] = createVectorFromList(6, bx_, by_, bz_, ux_, uy_, uz_);
		listOfRequiredVariablesForVectors["exb_lon"] = createVectorFromList(6, bx_, by_, bz_, ux_, uy_, uz_);
		listOfRequiredVariablesForVectors["j"] = createVectorFromList(3, jx_, jy_, jz_);
		listOfRequiredVariablesForVectors[jx_] = createVectorFromList(3, jx_, jy_, jz_);
		listOfRequiredVariablesForVectors[jy_] = createVectorFromList(3, jx_, jy_, jz_);
		listOfRequiredVariablesForVectors[jz_] = createVectorFromList(3, jx_, jy_, jz_);
		listOfRequiredVariablesForVectors["b"] = createVectorFromList(3, bx_, by_, bz_);
		listOfRequiredVariablesForVectors[bx_] = createVectorFromList(3, bx_, by_, bz_);
		listOfRequiredVariablesForVectors[by_] = createVectorFromList(3, bx_, by_, bz_);
		listOfRequiredVariablesForVectors[bz_] = createVectorFromList(3, bx_, by_, bz_);
		listOfRequiredVariablesForVectors["b1"] = createVectorFromList(3, "b1x", "b1y", "b1z");
		listOfRequiredVariablesForVectors["b1x"] = createVectorFromList(3, "b1x", "b1y", "b1z");
		listOfRequiredVariablesForVectors["b1y"] = createVectorFromList(3, "b1x", "b1y", "b1z");
		listOfRequiredVariablesForVectors["b1z"] = createVectorFromList(3, "b1x", "b1y", "b1z");
		listOfRequiredVariablesForVectors["b_r"] = createVectorFromList(3, bx_, by_, bz_);
		listOfRequiredVariablesForVectors["b_lat"] = createVectorFromList(3, bx_, by_, bz_);
		listOfRequiredVariablesForVectors["b_lon"] = createVectorFromList(3, bx_, by_, bz_);

		listOfRequiredVariablesForVectors["u"] = createVectorFromList(3, ux_, uy_, uz_);
		listOfRequiredVariablesForVectors[ux_] = createVectorFromList(3, ux_, uy_, uz_);
		listOfRequiredVariablesForVectors[uy_] = createVectorFromList(3, ux_, uy_, uz_);
		listOfRequiredVariablesForVectors[uz_] = createVectorFromList(3, ux_, uy_, uz_);
		listOfRequiredVariablesForVectors["v_r"] = createVectorFromList(3, ux_, uy_, uz_);
		listOfRequiredVariablesForVectors["v_lat"] = createVectorFromList(3, ux_, uy_, uz_);
		listOfRequiredVariablesForVectors["v_lon"] = createVectorFromList(3, ux_, uy_, uz_);

		listOfRequiredVariablesForVectors["s"] = createVectorFromList(2, p_, rho_);
		listOfRequiredVariablesForVectors["en"] = createVectorFromList(1, "e");
		listOfRequiredVariablesForVectors["e"] = createVectorFromList(6, bx_, by_, bz_, ux_, uy_, uz_);
		listOfRequiredVariablesForVectors["nv"] = createVectorFromList(4, rho_, ux_, uy_, uz_);
		listOfRequiredVariablesForVectors["nvx"] = createVectorFromList(4, rho_, ux_, uy_, uz_);
		listOfRequiredVariablesForVectors["nvy"] = createVectorFromList(4, rho_, ux_, uy_, uz_);
		listOfRequiredVariablesForVectors["nvz"] = createVectorFromList(4, rho_, ux_, uy_, uz_);
		listOfRequiredVariablesForVectors["n"] = createVectorFromList(1, rho_);
		listOfRequiredVariablesForVectors["n*r^2"] = createVectorFromList(1, rho_);
		listOfRequiredVariablesForVectors["polb"] = createVectorFromList(1, "bp");
		listOfRequiredVariablesForVectors["p*r^2"] = createVectorFromList(1, p_);
		listOfRequiredVariablesForVectors["eta*j"] = createVectorFromList(4, "eta", jx_, jy_, jz_);
		listOfRequiredVariablesForVectors["eta*jx"] = createVectorFromList(4, "eta", jx_, jy_, jz_);
		listOfRequiredVariablesForVectors["eta*jy"] = createVectorFromList(4, "eta", jx_, jy_, jz_);
		listOfRequiredVariablesForVectors["eta*jz"] = createVectorFromList(4, "eta", jx_, jy_, jz_);
		listOfRequiredVariablesForVectors["eta*j^2"] = createVectorFromList(4, "eta", jx_, jy_, jz_);

		if (model->doesVariableExist(p_))
		{
			listOfRequiredVariablesForVectors[p_] = createVectorFromList(1, p_);
			listOfRequiredVariablesForVectors["beta"] = createVectorFromList(4, bx_, by_, bz_, p_);

		} else
		{
			if (model->doesVariableExist("T"))
			{
				listOfRequiredVariablesForVectors[p_] = createVectorFromList(2, rho_, "T");
				listOfRequiredVariablesForVectors["beta"] = createVectorFromList(5, bx_, by_, bz_, rho_, "T");
			} else if (model->doesVariableExist("t"))
			{
				listOfRequiredVariablesForVectors[p_] = createVectorFromList(2, rho_, "t");
				listOfRequiredVariablesForVectors["beta"] = createVectorFromList(5, bx_, by_, bz_, rho_, "t");
			} else
			{
				//not sure whatelse to do.
				listOfRequiredVariablesForVectors[p_] = createVectorFromList(1, rho_);
				listOfRequiredVariablesForVectors["beta"] = createVectorFromList(4, bx_, by_, bz_, rho_);
			}
		}

		if ("mas" == modelName)
		{
			listOfRequiredVariablesForVectors["b"] = createVectorFromList(3, "br", "bphi", "btheta");
			listOfRequiredVariablesForVectors["br"] = createVectorFromList(3, "br", "bphi", "btheta");
			listOfRequiredVariablesForVectors["btheta"] = createVectorFromList(3, "br", "bphi", "btheta");
			listOfRequiredVariablesForVectors["bphi"] = createVectorFromList(3, "br", "bphi", "btheta");
			listOfRequiredVariablesForVectors["u"] = createVectorFromList(3, "ur", "uphi", "utheta");
			listOfRequiredVariablesForVectors["ur"] = createVectorFromList(3, "ur", "uphi", "utheta");
			listOfRequiredVariablesForVectors["uphi"] = createVectorFromList(3, "ur", "uphi", "utheta");
			listOfRequiredVariablesForVectors["utheta"] = createVectorFromList(3, "ur", "uphi", "utheta");
			listOfRequiredVariablesForVectors["nv"] = createVectorFromList(4, rho_, "ur", "uphi", "utheta");
			listOfRequiredVariablesForVectors["nvr"] = createVectorFromList(4, rho_, "ur", "uphi", "utheta");
			listOfRequiredVariablesForVectors["nvtheta"] = createVectorFromList(4, rho_, "ur", "uphi", "utheta");
			listOfRequiredVariablesForVectors["nvphi"] = createVectorFromList(4, rho_, "ur", "uphi", "utheta");
			listOfRequiredVariablesForVectors["pram"] = createVectorFromList(4, rho_, "ur", "uphi", "utheta");
			listOfRequiredVariablesForVectors["exb"] = createVectorFromList(6, "br", "bphi", "btheta", "ur", "uphi",
					"utheta");
			listOfRequiredVariablesForVectors["exbr"] = createVectorFromList(6, "br", "bphi", "btheta", "ur", "uphi",
					"utheta");
			listOfRequiredVariablesForVectors["exbtheta"] = createVectorFromList(6, "br", "bphi", "btheta", "ur",
					"uphi", "utheta");
			listOfRequiredVariablesForVectors["exbphi"] = createVectorFromList(6, "br", "bphi", "btheta", "ur", "uphi",
					"utheta");
			listOfRequiredVariablesForVectors["e"] = createVectorFromList(6, "br", "bphi", "btheta", "ur", "uphi",
					"utheta");
			listOfRequiredVariablesForVectors["er"] = createVectorFromList(6, "br", "bphi", "btheta", "ur", "uphi",
					"utheta");
			listOfRequiredVariablesForVectors["ephi"] = createVectorFromList(6, "br", "bphi", "btheta", "ur", "uphi",
					"utheta");
			listOfRequiredVariablesForVectors["etheta"] = createVectorFromList(6, "br", "bphi", "btheta", "ur", "uphi",
					"utheta");

		}

		if ("enlil" == modelName)
		{

			listOfRequiredVariablesForVectors["b"] = createVectorFromList(3, "b1x", "b1y", "b1z");
			listOfRequiredVariablesForVectors[bx_] = createVectorFromList(3, "b1x", "b1y", "b1z");
			listOfRequiredVariablesForVectors[by_] = createVectorFromList(3, "b1x", "b1y", "b1z");
			listOfRequiredVariablesForVectors[bz_] = createVectorFromList(3, "b1x", "b1y", "b1z");
			listOfRequiredVariablesForVectors["b1"] = createVectorFromList(3, "b1x", "b1y", "b1z");
			listOfRequiredVariablesForVectors["b1x"] = createVectorFromList(3, "b1x", "b1y", "b1z");
			listOfRequiredVariablesForVectors["b1y"] = createVectorFromList(3, "b1x", "b1y", "b1z");
			listOfRequiredVariablesForVectors["b1z"] = createVectorFromList(3, "b1x", "b1y", "b1z");
			listOfRequiredVariablesForVectors["br"] = createVectorFromList(3, "b1x", "b1y", "b1z");
			listOfRequiredVariablesForVectors["btheta"] = createVectorFromList(3, "b1x", "b1y", "b1z");
			listOfRequiredVariablesForVectors["bphi"] = createVectorFromList(3, "b1x", "b1y", "b1z");
			listOfRequiredVariablesForVectors["e"] = createVectorFromList(6, "b1x", "b1y", "b1z", ux_, uy_, uz_);
			listOfRequiredVariablesForVectors["ex"] = createVectorFromList(6, "b1x", "b1y", "b1z", ux_, uy_, uz_);
			listOfRequiredVariablesForVectors["ey"] = createVectorFromList(6, "b1x", "b1y", "b1z", ux_, uy_, uz_);
			listOfRequiredVariablesForVectors["ez"] = createVectorFromList(6, "b1x", "b1y", "b1z", ux_, uy_, uz_);

		}

	}
}
