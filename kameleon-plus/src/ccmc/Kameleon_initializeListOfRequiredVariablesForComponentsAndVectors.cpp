#include "Kameleon.h"
#include "StringConstants.h";

namespace ccmc
{
	using namespace ccmc::strings::variables;
	/**
	 * Initializes the list of required variables for the each derived variable,
	 * both for the individual component, and for the vectors.
	 *
	 * This requires major cleanup! This is so inefficient and hard to maintain
	 *
	 */
	void Kameleon::initializeListOfRequiredVariablesForComponentsAndVectors()
	{
		//base variables


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

		listOfRequiredVariablesForComponents["pram"] = createVectorOfStringFromList(4, rho_c_, ux_c_, uy_c_, uz_c_);
		listOfRequiredVariablesForComponentsByID["pram"] = createVectorOfLongFromList(4, rho_c_, ux_c_, uy_c_, uz_c_);

		listOfRequiredVariablesForComponents["t"] = createVectorOfStringFromList(2, p_c_, rho_c_);
		listOfRequiredVariablesForComponentsByID["t"] = createVectorOfLongFromList(2, p_c_, rho_c_);

		listOfRequiredVariablesForComponents["ex"] = createVectorOfStringFromList(4, by_c_, bz_c_, uy_c_, uz_c_);
		listOfRequiredVariablesForComponentsByID["ex"] = createVectorOfLongFromList(4, by_c_, bz_c_, uy_c_, uz_c_);

		listOfRequiredVariablesForComponents["e_r"] = createVectorOfStringFromList(4, by_c_, bz_c_, uy_c_, uz_c_);
		listOfRequiredVariablesForComponentsByID["e_r"] = createVectorOfLongFromList(4, by_c_, bz_c_, uy_c_, uz_c_);

		listOfRequiredVariablesForComponents["ey"] = createVectorOfStringFromList(4, bx_c_, bz_c_, ux_c_, uz_c_);
		listOfRequiredVariablesForComponentsByID["ey"] = createVectorOfLongFromList(4, bx_c_, bz_c_, ux_c_, uz_c_);

		listOfRequiredVariablesForComponents["e_lat"] = createVectorOfStringFromList(4, bx_c_, bz_c_, ux_c_, uz_c_);
		listOfRequiredVariablesForComponentsByID["e_lat"] = createVectorOfLongFromList(4, bx_c_, bz_c_, ux_c_, uz_c_);

		listOfRequiredVariablesForComponents["ez"] = createVectorOfStringFromList(4, bx_c_, by_c_, ux_c_, uy_c_);
		listOfRequiredVariablesForComponentsByID["ez"] = createVectorOfLongFromList(4, bx_c_, by_c_, ux_c_, uy_c_);

		listOfRequiredVariablesForComponents["e_lon"] = createVectorOfStringFromList(4, bx_c_, by_c_, ux_c_, uy_c_);
		listOfRequiredVariablesForComponentsByID["e_lon"] = createVectorOfLongFromList(4, bx_c_, by_c_, ux_c_, uy_c_);

		listOfRequiredVariablesForComponents["edotj"] = createVectorOfStringFromList(9, jx_c_, jy_c_, jz_c_, bx_c_, by_c_, bz_c_, ux_c_, uy_c_,
				uz_c_);
		listOfRequiredVariablesForComponentsByID["edotj"] = createVectorOfLongFromList(9, jx_c_, jy_c_, jz_c_, bx_c_, by_c_, bz_c_, ux_c_, uy_c_,
				uz_c_);

		listOfRequiredVariablesForComponents["jparx"] = createVectorOfStringFromList(6, jx_c_, jy_c_, jz_c_, bx_c_, by_c_, bz_c_);
		listOfRequiredVariablesForComponentsByID["jparx"] = createVectorOfLongFromList(6, jx_c_, jy_c_, jz_c_, bx_c_, by_c_, bz_c_);

		listOfRequiredVariablesForComponents["jpary"] = createVectorOfStringFromList(6, jx_c_, jy_c_, jz_c_, bx_c_, by_c_, bz_c_);
		listOfRequiredVariablesForComponentsByID["jpary"] = createVectorOfLongFromList(6, jx_c_, jy_c_, jz_c_, bx_c_, by_c_, bz_c_);

		listOfRequiredVariablesForComponents["jparz"] = createVectorOfStringFromList(6, jx_c_, jy_c_, jz_c_, bx_c_, by_c_, bz_c_);
		listOfRequiredVariablesForComponentsByID["jparz"] = createVectorOfLongFromList(6, jx_c_, jy_c_, jz_c_, bx_c_, by_c_, bz_c_);

		listOfRequiredVariablesForComponents["jparr"] = createVectorOfStringFromList(6, "jr", "jphi", "jtheta", "br", "bphi",
				"btheta");
		listOfRequiredVariablesForComponentsByID["jparr"] = createVectorOfLongFromList(6, "jr", "jphi", "jtheta", "br", "bphi",
				"btheta");

		listOfRequiredVariablesForComponents["jparphi"] = createVectorOfStringFromList(6, "jr", "jphi", "jtheta", "br", "bphi",
				"btheta");
		listOfRequiredVariablesForComponentsByID["jparphi"] = createVectorOfLongFromList(6, "jr", "jphi", "jtheta", "br", "bphi",
				"btheta");

		listOfRequiredVariablesForComponents["jpartheta"] = createVectorOfStringFromList(6, "jr", "jphi", "jtheta", "br",
				"bphi", "btheta");
		listOfRequiredVariablesForComponentsByID["jpartheta"] = createVectorOfLongFromList(6, "jr", "jphi", "jtheta", "br",
				"bphi", "btheta");

		listOfRequiredVariablesForComponents["jpar"] = createVectorOfStringFromList(6, jx_c_, jy_c_, jz_c_, bx_c_, by_c_, bz_c_);
		listOfRequiredVariablesForComponentsByID["jpar"] = createVectorOfLongFromList(6, jx_c_, jy_c_, jz_c_, bx_c_, by_c_, bz_c_);

		listOfRequiredVariablesForComponents["jxbx"] = createVectorOfStringFromList(4, by_c_, bz_c_, jy_c_, jz_c_);
		listOfRequiredVariablesForComponentsByID["jxbx"] = createVectorOfLongFromList(4, by_c_, bz_c_, jy_c_, jz_c_);

		listOfRequiredVariablesForComponents["jxby"] = createVectorOfStringFromList(4, bx_c_, bz_c_, jx_c_, jz_c_);
		listOfRequiredVariablesForComponentsByID["jxby"] = createVectorOfLongFromList(4, bx_c_, bz_c_, jx_c_, jz_c_);

		listOfRequiredVariablesForComponents["jxbz"] = createVectorOfStringFromList(4, bx_c_, by_c_, jx_c_, jy_c_);
		listOfRequiredVariablesForComponentsByID["jxbz"] = createVectorOfLongFromList(4, bx_c_, by_c_, jx_c_, jy_c_);

		listOfRequiredVariablesForComponents["jxb"] = createVectorOfStringFromList(6, bx_c_, by_c_, bz_c_, jx_c_, jy_c_, jz_c_);
		listOfRequiredVariablesForComponentsByID["jxb"] = createVectorOfLongFromList(6, bx_c_, by_c_, bz_c_, jx_c_, jy_c_, jz_c_);

		listOfRequiredVariablesForComponents["exbx"] = createVectorOfStringFromList(6, bx_c_, by_c_, bz_c_, ux_c_, uy_c_, uz_c_);
		listOfRequiredVariablesForComponentsByID["exbx"] = createVectorOfLongFromList(6, bx_c_, by_c_, bz_c_, ux_c_, uy_c_, uz_c_);

		listOfRequiredVariablesForComponents["exby"] = createVectorOfStringFromList(6, bx_c_, by_c_, bz_c_, ux_c_, uy_c_, uz_c_);
		listOfRequiredVariablesForComponentsByID["exby"] = createVectorOfLongFromList(6, bx_c_, by_c_, bz_c_, ux_c_, uy_c_, uz_c_);

		listOfRequiredVariablesForComponents["exbz"] = createVectorOfStringFromList(6, bx_c_, by_c_, bz_c_, ux_c_, uy_c_, uz_c_);
		listOfRequiredVariablesForComponentsByID["exbz"] = createVectorOfLongFromList(6, bx_c_, by_c_, bz_c_, ux_c_, uy_c_, uz_c_);

		listOfRequiredVariablesForComponents["exb"] = createVectorOfStringFromList(6, bx_c_, by_c_, bz_c_, ux_c_, uy_c_, uz_c_);
		listOfRequiredVariablesForComponentsByID["exb"] = createVectorOfLongFromList(6, bx_c_, by_c_, bz_c_, ux_c_, uy_c_, uz_c_);

		listOfRequiredVariablesForComponents["exb_r"] = createVectorOfStringFromList(6, bx_c_, by_c_, bz_c_, ux_c_, uy_c_, uz_c_);
		listOfRequiredVariablesForComponentsByID["exb_r"] = createVectorOfLongFromList(6, bx_c_, by_c_, bz_c_, ux_c_, uy_c_, uz_c_);

		listOfRequiredVariablesForComponents["exb_lat"] = createVectorOfStringFromList(6, bx_c_, by_c_, bz_c_, ux_c_, uy_c_, uz_c_);
		listOfRequiredVariablesForComponentsByID["exb_lat"] = createVectorOfLongFromList(6, bx_c_, by_c_, bz_c_, ux_c_, uy_c_, uz_c_);

		listOfRequiredVariablesForComponents["exb_lon"] = createVectorOfStringFromList(6, bx_c_, by_c_, bz_c_, ux_c_, uy_c_, uz_c_);
		listOfRequiredVariablesForComponentsByID["exb_lon"] = createVectorOfLongFromList(6, bx_c_, by_c_, bz_c_, ux_c_, uy_c_, uz_c_);

		listOfRequiredVariablesForComponents["j"] = createVectorOfStringFromList(3, jx_c_, jy_c_, jz_c_);
		listOfRequiredVariablesForComponentsByID["j"] = createVectorOfLongFromList(3, jx_c_, jy_c_, jz_c_);

		listOfRequiredVariablesForComponents["b"] = createVectorOfStringFromList(3, bx_c_, by_c_, bz_c_);
		listOfRequiredVariablesForComponentsByID["b"] = createVectorOfLongFromList(3, bx_c_, by_c_, bz_c_);

		listOfRequiredVariablesForComponents["b1"] = createVectorOfStringFromList(3, "b1x", "b1y", "b1z");
		listOfRequiredVariablesForComponentsByID["b1"] = createVectorOfLongFromList(3, "b1x", "b1y", "b1z");

		listOfRequiredVariablesForComponents["b_r"] = createVectorOfStringFromList(1, bx_c_);
		listOfRequiredVariablesForComponentsByID["b_r"] = createVectorOfLongFromList(1, bx_c_);

		listOfRequiredVariablesForComponents["b_lat"] = createVectorOfStringFromList(1, by_c_);
		listOfRequiredVariablesForComponentsByID["b_lat"] = createVectorOfLongFromList(1, by_c_);

		listOfRequiredVariablesForComponents["b_lon"] = createVectorOfStringFromList(1, bz_c_);
		listOfRequiredVariablesForComponentsByID["b_lon"] = createVectorOfLongFromList(1, bz_c_);

		listOfRequiredVariablesForComponents["u"] = createVectorOfStringFromList(3, ux_c_, uy_c_, uz_c_);
		listOfRequiredVariablesForComponentsByID["u"] = createVectorOfLongFromList(3, ux_c_, uy_c_, uz_c_);

		listOfRequiredVariablesForComponents["v"] = createVectorOfStringFromList(3, ux_c_, uy_c_, uz_c_);
		listOfRequiredVariablesForComponentsByID["v"] = createVectorOfLongFromList(3, ux_c_, uy_c_, uz_c_);

		listOfRequiredVariablesForComponents["v_r"] = createVectorOfStringFromList(1, ux_c_);
		listOfRequiredVariablesForComponentsByID["v_r"] = createVectorOfLongFromList(1, ux_c_);

		listOfRequiredVariablesForComponents["v_lat"] = createVectorOfStringFromList(1, uy_c_);
		listOfRequiredVariablesForComponentsByID["v_lat"] = createVectorOfLongFromList(1, uy_c_);

		listOfRequiredVariablesForComponents["v_lon"] = createVectorOfStringFromList(1, uz_c_);
		listOfRequiredVariablesForComponentsByID["v_lon"] = createVectorOfLongFromList(1, uz_c_);

		listOfRequiredVariablesForComponents["s"] = createVectorOfStringFromList(2, p_c_, rho_c_);
		listOfRequiredVariablesForComponentsByID["s"] = createVectorOfLongFromList(2, p_c_, rho_c_);

		listOfRequiredVariablesForComponents["en"] = createVectorOfStringFromList(1, "e");
		listOfRequiredVariablesForComponentsByID["en"] = createVectorOfLongFromList(1, "e");

		listOfRequiredVariablesForComponents["e"] = createVectorOfStringFromList(6, bx_c_, by_c_, bz_c_, ux_c_, uy_c_, uz_c_);
		listOfRequiredVariablesForComponentsByID["e"] = createVectorOfLongFromList(6, bx_c_, by_c_, bz_c_, ux_c_, uy_c_, uz_c_);

		listOfRequiredVariablesForComponents["nv"] = createVectorOfStringFromList(4, rho_c_, ux_c_, uy_c_, uz_c_);
		listOfRequiredVariablesForComponentsByID["nv"] = createVectorOfLongFromList(4, rho_c_, ux_c_, uy_c_, uz_c_);

		listOfRequiredVariablesForComponents["nvx"] = createVectorOfStringFromList(2, rho_c_, ux_c_);
		listOfRequiredVariablesForComponentsByID["nvx"] = createVectorOfLongFromList(2, rho_c_, ux_c_);

		listOfRequiredVariablesForComponents["nvy"] = createVectorOfStringFromList(2, rho_c_, uy_c_);
		listOfRequiredVariablesForComponentsByID["nvy"] = createVectorOfLongFromList(2, rho_c_, uy_c_);

		listOfRequiredVariablesForComponents["nvz"] = createVectorOfStringFromList(2, rho_c_, uz_c_);
		listOfRequiredVariablesForComponentsByID["nvz"] = createVectorOfLongFromList(2, rho_c_, uz_c_);

		listOfRequiredVariablesForComponents["n"] = createVectorOfStringFromList(1, rho_c_);
		listOfRequiredVariablesForComponentsByID["n"] = createVectorOfLongFromList(1, rho_c_);

		listOfRequiredVariablesForComponents["n*r^2"] = createVectorOfStringFromList(1, rho_c_);
		listOfRequiredVariablesForComponentsByID["n*r^2"] = createVectorOfLongFromList(1, rho_c_);

		listOfRequiredVariablesForComponents["polb"] = createVectorOfStringFromList(1, "bp");
		listOfRequiredVariablesForComponentsByID["polb"] = createVectorOfLongFromList(1, "bp");

		listOfRequiredVariablesForComponents["p*r^2"] = createVectorOfStringFromList(1, p_c_);
		listOfRequiredVariablesForComponentsByID["p*r^2"] = createVectorOfLongFromList(1, p_c_);

		listOfRequiredVariablesForComponents["eta*jx"] = createVectorOfStringFromList(2, "eta", jx_c_);
		listOfRequiredVariablesForComponentsByID["eta*jx"] = createVectorOfLongFromList(2, "eta", jx_c_);

		listOfRequiredVariablesForComponents["eta*jy"] = createVectorOfStringFromList(2, "eta", jy_c_);
		listOfRequiredVariablesForComponentsByID["eta*jy"] = createVectorOfLongFromList(2, "eta", jy_c_);

		listOfRequiredVariablesForComponents["eta*jz"] = createVectorOfStringFromList(2, "eta", jz_c_);
		listOfRequiredVariablesForComponentsByID["eta*jz"] = createVectorOfLongFromList(2, "eta", jz_c_);

		listOfRequiredVariablesForComponents["eta*j^2"] = createVectorOfStringFromList(4, "eta", jx_c_, jy_c_, jz_c_);
		listOfRequiredVariablesForComponentsByID["eta*j^2"] = createVectorOfLongFromList(4, "eta", jx_c_, jy_c_, jz_c_);

		listOfRequiredVariablesForComponents["grad.n"] = createVectorOfStringFromList(1, "rho");
		listOfRequiredVariablesForComponentsByID["grad.n"] = createVectorOfLongFromList(1, "rho");

		listOfRequiredVariablesForComponents["grad.b"] = createVectorOfStringFromList(1, bx_c_, by_c_, bz_c_);
		listOfRequiredVariablesForComponentsByID["grad.b"] = createVectorOfLongFromList(1, bx_c_, by_c_, bz_c_);


		if (model->doesVariableExist(p_c_))
		{

			listOfRequiredVariablesForComponents[p_] = createVectorOfStringFromList(1, p_c_);
			listOfRequiredVariablesForComponentsByID[p_] = createVectorOfLongFromList(1, p_c_);

			listOfRequiredVariablesForComponents["beta"] = createVectorOfStringFromList(4, bx_c_, by_c_, bz_c_, p_c_);
			listOfRequiredVariablesForComponentsByID["beta"] = createVectorOfLongFromList(4, bx_c_, by_c_, bz_c_, p_c_);

		} else
		{
			if (model->doesVariableExist("T"))
			{
				listOfRequiredVariablesForComponents[p_] = createVectorOfStringFromList(2, rho_c_, "T");
				listOfRequiredVariablesForComponentsByID[p_] = createVectorOfLongFromList(2, rho_c_, "T");

				listOfRequiredVariablesForComponents["beta"] = createVectorOfStringFromList(5, bx_c_, by_c_, bz_c_, rho_c_, "T");
				listOfRequiredVariablesForComponentsByID["beta"] = createVectorOfLongFromList(5, bx_c_, by_c_, bz_c_, rho_c_, "T");

			} else if (model->doesVariableExist("t"))
			{
				listOfRequiredVariablesForComponents[p_] = createVectorOfStringFromList(2, rho_c_, "t");
				listOfRequiredVariablesForComponentsByID[p_] = createVectorOfLongFromList(2, rho_c_, "t");

				listOfRequiredVariablesForComponents["beta"] = createVectorOfStringFromList(5, bx_c_, by_c_, bz_c_, rho_c_, "t");
				listOfRequiredVariablesForComponentsByID["beta"] = createVectorOfLongFromList(5, bx_c_, by_c_, bz_c_, rho_c_, "t");

			} else
			{
				//not sure whatelse to do.
				listOfRequiredVariablesForComponents[p_] = createVectorOfStringFromList(1, rho_c_);
				listOfRequiredVariablesForComponentsByID[p_] = createVectorOfLongFromList(1, rho_c_);

				listOfRequiredVariablesForComponents["beta"] = createVectorOfStringFromList(4, bx_c_, by_c_, bz_c_, rho_c_);
				listOfRequiredVariablesForComponentsByID["beta"] = createVectorOfLongFromList(4, bx_c_, by_c_, bz_c_, rho_c_);

			}
		}

		if (modelName == "enlil")
		{
			listOfRequiredVariablesForComponents["b"] = createVectorOfStringFromList(3, "br", "bphi", "btheta");
			listOfRequiredVariablesForComponentsByID["b"] = createVectorOfLongFromList(3, "br", "bphi", "btheta");

			listOfRequiredVariablesForComponents["br"] = createVectorOfStringFromList(1, "br");
			listOfRequiredVariablesForComponentsByID["br"] = createVectorOfLongFromList(1, "br");

			listOfRequiredVariablesForComponents["bphi"] = createVectorOfStringFromList(1, "bphi");
			listOfRequiredVariablesForComponentsByID["bphi"] = createVectorOfLongFromList(1, "bphi");

			listOfRequiredVariablesForComponents["btheta"] = createVectorOfStringFromList(1, "btheta");
			listOfRequiredVariablesForComponentsByID["btheta"] = createVectorOfLongFromList(1, "btheta");

			listOfRequiredVariablesForComponents["b1"] = createVectorOfStringFromList(3, "b1r", "b1phi", "b1theta");
			listOfRequiredVariablesForComponentsByID["b1"] = createVectorOfLongFromList(3, "b1r", "b1phi", "b1theta");

			listOfRequiredVariablesForComponents["b1r"] = createVectorOfStringFromList(1, "b1r");
			listOfRequiredVariablesForComponentsByID["b1r"] = createVectorOfLongFromList(1, "b1r");

			listOfRequiredVariablesForComponents["b1phi"] = createVectorOfStringFromList(1, "b1phi");
			listOfRequiredVariablesForComponentsByID["b1phi"] = createVectorOfLongFromList(1, "b1phi");

			listOfRequiredVariablesForComponents["b1theta"] = createVectorOfStringFromList(1, "b1theta");
			listOfRequiredVariablesForComponentsByID["b1theta"] = createVectorOfLongFromList(1, "b1theta");

			listOfRequiredVariablesForComponents["e"] = createVectorOfStringFromList(6, "b1r", "b1phi", "b1theta", "ur", "uphi", "utheta");
			listOfRequiredVariablesForComponentsByID["e"] = createVectorOfLongFromList(6, "b1r", "b1phi", "b1theta", "ur", "uphi", "utheta");

			listOfRequiredVariablesForComponents["er"] = createVectorOfStringFromList(4, "b1phi", "b1theta", "uphi", "utheta");
			listOfRequiredVariablesForComponentsByID["er"] = createVectorOfLongFromList(4, "b1phi", "b1theta", "uphi", "utheta");

			listOfRequiredVariablesForComponents["e_r"] = createVectorOfStringFromList(4, "b1phi", "b1theta", "uphi", "utheta");
			listOfRequiredVariablesForComponentsByID["e_r"] = createVectorOfLongFromList(4, "b1phi", "b1theta", "uphi", "utheta");

			listOfRequiredVariablesForComponents["ephi"] = createVectorOfStringFromList(4, "b1r", "b1theta", "ur", "utheta");
			listOfRequiredVariablesForComponentsByID["ephi"] = createVectorOfLongFromList(4, "b1r", "b1theta", "ur", "utheta");

			listOfRequiredVariablesForComponents["e_phi"] = createVectorOfStringFromList(4, "b1r", "b1theta", "ur", "utheta");
			listOfRequiredVariablesForComponentsByID["e_phi"] = createVectorOfLongFromList(4, "b1r", "b1theta", "ur", "utheta");

			listOfRequiredVariablesForComponents["etheta"] = createVectorOfStringFromList(4, "b1r", "b1phi", "ur", "uphi");
			listOfRequiredVariablesForComponentsByID["etheta"] = createVectorOfLongFromList(4, "b1r", "b1phi", "ur", "uphi");

			listOfRequiredVariablesForComponents["e_theta"] = createVectorOfStringFromList(4, "b1r", "b1phi", "ur", "uphi");
			listOfRequiredVariablesForComponentsByID["e_theta"] = createVectorOfLongFromList(4, "b1r", "b1phi", "ur", "uphi");

		}
		if (modelName == "mas")
		{
			listOfRequiredVariablesForComponents["pram"] = createVectorOfStringFromList(4, rho_c_, "ur", "uphi", "utheta");
			listOfRequiredVariablesForComponentsByID["pram"] = createVectorOfLongFromList(4, rho_c_, "ur", "uphi", "utheta");

			listOfRequiredVariablesForComponents["exb"] = createVectorOfStringFromList(6, "br", "bphi", "btheta", "ur", "uphi",
					"utheta");
			listOfRequiredVariablesForComponentsByID["exb"] = createVectorOfLongFromList(6, "br", "bphi", "btheta", "ur", "uphi",
					"utheta");

			listOfRequiredVariablesForComponents["exbr"] = createVectorOfStringFromList(6, "br", "bphi", "btheta", "ur",
					"uphi", "utheta");
			listOfRequiredVariablesForComponentsByID["exbr"] = createVectorOfLongFromList(6, "br", "bphi", "btheta", "ur",
					"uphi", "utheta");

			listOfRequiredVariablesForComponents["exbtheta"] = createVectorOfStringFromList(6, "br", "bphi", "btheta", "ur",
					"uphi", "utheta");
			listOfRequiredVariablesForComponentsByID["exbtheta"] = createVectorOfLongFromList(6, "br", "bphi", "btheta", "ur",
					"uphi", "utheta");

			listOfRequiredVariablesForComponents["exbphi"] = createVectorOfStringFromList(6, "br", "bphi", "btheta", "ur",
					"uphi", "utheta");
			listOfRequiredVariablesForComponentsByID["exbphi"] = createVectorOfLongFromList(6, "br", "bphi", "btheta", "ur",
					"uphi", "utheta");

			listOfRequiredVariablesForComponents["edotj"] = createVectorOfStringFromList(9, "jr", "jphi", "jtheta", "br",
					"bphi", "btheta", "ur", "uphi", "utheta");
			listOfRequiredVariablesForComponentsByID["edotj"] = createVectorOfLongFromList(9, "jr", "jphi", "jtheta", "br",
					"bphi", "btheta", "ur", "uphi", "utheta");

			listOfRequiredVariablesForComponents["v"] = createVectorOfStringFromList(3, "ur", "uphi", "utheta");
			listOfRequiredVariablesForComponentsByID["v"] = createVectorOfLongFromList(3, "ur", "uphi", "utheta");

			listOfRequiredVariablesForComponents["j"] = createVectorOfStringFromList(3, "jr", "jphi", "jtheta");
			listOfRequiredVariablesForComponentsByID["j"] = createVectorOfLongFromList(3, "jr", "jphi", "jtheta");

			listOfRequiredVariablesForComponents["b"] = createVectorOfStringFromList(3, "br", "bphi", "btheta");
			listOfRequiredVariablesForComponentsByID["b"] = createVectorOfLongFromList(3, "br", "bphi", "btheta");

			listOfRequiredVariablesForComponents["er"] = createVectorOfStringFromList(4, "bphi", "btheta", "utheta", "uphi");
			listOfRequiredVariablesForComponentsByID["er"] = createVectorOfLongFromList(4, "bphi", "btheta", "utheta", "uphi");

			listOfRequiredVariablesForComponents["ephi"] = createVectorOfStringFromList(4, "br", "bphi", "ur", "uphi");
			listOfRequiredVariablesForComponentsByID["ephi"] = createVectorOfLongFromList(4, "br", "bphi", "ur", "uphi");

			listOfRequiredVariablesForComponents["etheta"] = createVectorOfStringFromList(4, "br", "btheta", "ur", "utheta");
			listOfRequiredVariablesForComponentsByID["etheta"] = createVectorOfLongFromList(4, "br", "btheta", "ur", "utheta");

		} else
		{

		}

		//add variables for vectors.  same as above, except, for vectors, we want to add all components
		listOfRequiredVariablesForVectors["pram"] = createVectorOfStringFromList(4, rho_c_, ux_c_, uy_c_, uz_c_);
		listOfRequiredVariablesForVectorsByID["pram"] = createVectorOfLongFromList(4, rho_c_, ux_c_, uy_c_, uz_c_);

		listOfRequiredVariablesForVectors["t"] = createVectorOfStringFromList(2, p_c_, rho_c_);
		listOfRequiredVariablesForVectorsByID["t"] = createVectorOfLongFromList(2, p_c_, rho_c_);

		listOfRequiredVariablesForVectors["ex"] = createVectorOfStringFromList(6, bx_c_, by_c_, bz_c_, ux_c_, uy_c_, uz_c_);
		listOfRequiredVariablesForVectorsByID["ex"] = createVectorOfLongFromList(6, bx_c_, by_c_, bz_c_, ux_c_, uy_c_, uz_c_);

		listOfRequiredVariablesForVectors["e_r"] = createVectorOfStringFromList(6, bx_c_, by_c_, bz_c_, ux_c_, uy_c_, uz_c_);
		listOfRequiredVariablesForVectorsByID["e_r"] = createVectorOfLongFromList(6, bx_c_, by_c_, bz_c_, ux_c_, uy_c_, uz_c_);

		listOfRequiredVariablesForVectors["ey"] = createVectorOfStringFromList(6, bx_c_, by_c_, bz_c_, ux_c_, uy_c_, uz_c_);
		listOfRequiredVariablesForVectorsByID["ey"] = createVectorOfLongFromList(6, bx_c_, by_c_, bz_c_, ux_c_, uy_c_, uz_c_);

		listOfRequiredVariablesForVectors["e_lat"] = createVectorOfStringFromList(6, bx_c_, by_c_, bz_c_, ux_c_, uy_c_, uz_c_);
		listOfRequiredVariablesForVectorsByID["e_lat"] = createVectorOfLongFromList(6, bx_c_, by_c_, bz_c_, ux_c_, uy_c_, uz_c_);

		listOfRequiredVariablesForVectors["ez"] = createVectorOfStringFromList(6, bx_c_, by_c_, bz_c_, ux_c_, uy_c_, uz_c_);
		listOfRequiredVariablesForVectorsByID["ez"] = createVectorOfLongFromList(6, bx_c_, by_c_, bz_c_, ux_c_, uy_c_, uz_c_);

		listOfRequiredVariablesForVectors["e_lon"] = createVectorOfStringFromList(6, bx_c_, by_c_, bz_c_, ux_c_, uy_c_, uz_c_);
		listOfRequiredVariablesForVectorsByID["e_lon"] = createVectorOfLongFromList(6, bx_c_, by_c_, bz_c_, ux_c_, uy_c_, uz_c_);

		listOfRequiredVariablesForVectors["edotj"] = createVectorOfStringFromList(9, jx_c_, jy_c_, jz_c_, bx_c_, by_c_, bz_c_, ux_c_, uy_c_,
				uz_c_);
		listOfRequiredVariablesForVectorsByID["edotj"] = createVectorOfLongFromList(9, jx_c_, jy_c_, jz_c_, bx_c_, by_c_, bz_c_, ux_c_, uy_c_,
				uz_c_);

		listOfRequiredVariablesForVectors["jparx"] = createVectorOfStringFromList(6, jx_c_, jy_c_, jz_c_, bx_c_, by_c_, bz_c_);
		listOfRequiredVariablesForVectorsByID["jparx"] = createVectorOfLongFromList(6, jx_c_, jy_c_, jz_c_, bx_c_, by_c_, bz_c_);

		listOfRequiredVariablesForVectors["jpary"] = createVectorOfStringFromList(6, jx_c_, jy_c_, jz_c_, bx_c_, by_c_, bz_c_);
		listOfRequiredVariablesForVectorsByID["jpary"] = createVectorOfLongFromList(6, jx_c_, jy_c_, jz_c_, bx_c_, by_c_, bz_c_);

		listOfRequiredVariablesForVectors["jparz"] = createVectorOfStringFromList(6, jx_c_, jy_c_, jz_c_, bx_c_, by_c_, bz_c_);
		listOfRequiredVariablesForVectorsByID["jparz"] = createVectorOfLongFromList(6, jx_c_, jy_c_, jz_c_, bx_c_, by_c_, bz_c_);

		listOfRequiredVariablesForVectors["jpar"] = createVectorOfStringFromList(6, jx_c_, jy_c_, jz_c_, bx_c_, by_c_, bz_c_);
		listOfRequiredVariablesForVectorsByID["jpar"] = createVectorOfLongFromList(6, jx_c_, jy_c_, jz_c_, bx_c_, by_c_, bz_c_);

		listOfRequiredVariablesForVectors["jxbx"] = createVectorOfStringFromList(6, bx_c_, by_c_, bz_c_, jx_c_, jy_c_, jz_c_);
		listOfRequiredVariablesForVectorsByID["jxbx"] = createVectorOfLongFromList(6, bx_c_, by_c_, bz_c_, jx_c_, jy_c_, jz_c_);

		listOfRequiredVariablesForVectors["jxby"] = createVectorOfStringFromList(6, bx_c_, by_c_, bz_c_, jx_c_, jy_c_, jz_c_);
		listOfRequiredVariablesForVectorsByID["jxby"] = createVectorOfLongFromList(6, bx_c_, by_c_, bz_c_, jx_c_, jy_c_, jz_c_);

		listOfRequiredVariablesForVectors["jxbz"] = createVectorOfStringFromList(6, bx_c_, by_c_, bz_c_, jx_c_, jy_c_, jz_c_);
		listOfRequiredVariablesForVectorsByID["jxbz"] = createVectorOfLongFromList(6, bx_c_, by_c_, bz_c_, jx_c_, jy_c_, jz_c_);

		listOfRequiredVariablesForVectors["jxb"] = createVectorOfStringFromList(6, bx_c_, by_c_, bz_c_, jx_c_, jy_c_, jz_c_);
		listOfRequiredVariablesForVectorsByID["jxb"] = createVectorOfLongFromList(6, bx_c_, by_c_, bz_c_, jx_c_, jy_c_, jz_c_);

		listOfRequiredVariablesForVectors["exbx"] = createVectorOfStringFromList(6, bx_c_, by_c_, bz_c_, ux_c_, uy_c_, uz_c_);
		listOfRequiredVariablesForVectorsByID["exbx"] = createVectorOfLongFromList(6, bx_c_, by_c_, bz_c_, ux_c_, uy_c_, uz_c_);

		listOfRequiredVariablesForVectors["exby"] = createVectorOfStringFromList(6, bx_c_, by_c_, bz_c_, ux_c_, uy_c_, uz_c_);
		listOfRequiredVariablesForVectorsByID["exby"] = createVectorOfLongFromList(6, bx_c_, by_c_, bz_c_, ux_c_, uy_c_, uz_c_);

		listOfRequiredVariablesForVectors["exbz"] = createVectorOfStringFromList(6, bx_c_, by_c_, bz_c_, ux_c_, uy_c_, uz_c_);
		listOfRequiredVariablesForVectorsByID["exbz"] = createVectorOfLongFromList(6, bx_c_, by_c_, bz_c_, ux_c_, uy_c_, uz_c_);

		listOfRequiredVariablesForVectors["exb"] = createVectorOfStringFromList(6, bx_c_, by_c_, bz_c_, ux_c_, uy_c_, uz_c_);
		listOfRequiredVariablesForVectorsByID["exb"] = createVectorOfLongFromList(6, bx_c_, by_c_, bz_c_, ux_c_, uy_c_, uz_c_);

		listOfRequiredVariablesForVectors["exb_r"] = createVectorOfStringFromList(6, bx_c_, by_c_, bz_c_, ux_c_, uy_c_, uz_c_);
		listOfRequiredVariablesForVectorsByID["exb_r"] = createVectorOfLongFromList(6, bx_c_, by_c_, bz_c_, ux_c_, uy_c_, uz_c_);

		listOfRequiredVariablesForVectors["exb_lat"] = createVectorOfStringFromList(6, bx_c_, by_c_, bz_c_, ux_c_, uy_c_, uz_c_);
		listOfRequiredVariablesForVectorsByID["exb_lat"] = createVectorOfLongFromList(6, bx_c_, by_c_, bz_c_, ux_c_, uy_c_, uz_c_);

		listOfRequiredVariablesForVectors["exb_lon"] = createVectorOfStringFromList(6, bx_c_, by_c_, bz_c_, ux_c_, uy_c_, uz_c_);
		listOfRequiredVariablesForVectorsByID["exb_lon"] = createVectorOfLongFromList(6, bx_c_, by_c_, bz_c_, ux_c_, uy_c_, uz_c_);

		listOfRequiredVariablesForVectors["j"] = createVectorOfStringFromList(3, jx_c_, jy_c_, jz_c_);
		listOfRequiredVariablesForVectorsByID["j"] = createVectorOfLongFromList(3, jx_c_, jy_c_, jz_c_);

		listOfRequiredVariablesForVectors[jx_] = createVectorOfStringFromList(3, jx_c_, jy_c_, jz_c_);
		listOfRequiredVariablesForVectorsByID[jx_] = createVectorOfLongFromList(3, jx_c_, jy_c_, jz_c_);

		listOfRequiredVariablesForVectors[jy_] = createVectorOfStringFromList(3, jx_c_, jy_c_, jz_c_);
		listOfRequiredVariablesForVectorsByID[jy_] = createVectorOfLongFromList(3, jx_c_, jy_c_, jz_c_);

		listOfRequiredVariablesForVectors[jz_] = createVectorOfStringFromList(3, jx_c_, jy_c_, jz_c_);
		listOfRequiredVariablesForVectorsByID[jz_] = createVectorOfLongFromList(3, jx_c_, jy_c_, jz_c_);

		listOfRequiredVariablesForVectors["b"] = createVectorOfStringFromList(3, bx_c_, by_c_, bz_c_);
		listOfRequiredVariablesForVectorsByID["b"] = createVectorOfLongFromList(3, bx_c_, by_c_, bz_c_);

		listOfRequiredVariablesForVectors[bx_] = createVectorOfStringFromList(3, bx_c_, by_c_, bz_c_);
		listOfRequiredVariablesForVectorsByID[bx_] = createVectorOfLongFromList(3, bx_c_, by_c_, bz_c_);

		listOfRequiredVariablesForVectors[by_] = createVectorOfStringFromList(3, bx_c_, by_c_, bz_c_);
		listOfRequiredVariablesForVectorsByID[by_] = createVectorOfLongFromList(3, bx_c_, by_c_, bz_c_);

		listOfRequiredVariablesForVectors[bz_] = createVectorOfStringFromList(3, bx_c_, by_c_, bz_c_);
		listOfRequiredVariablesForVectorsByID[bz_] = createVectorOfLongFromList(3, bx_c_, by_c_, bz_c_);

		listOfRequiredVariablesForVectors["b1"] = createVectorOfStringFromList(3, "b1x", "b1y", "b1z");
		listOfRequiredVariablesForVectorsByID["b1"] = createVectorOfLongFromList(3, "b1x", "b1y", "b1z");

		listOfRequiredVariablesForVectors["b1x"] = createVectorOfStringFromList(3, "b1x", "b1y", "b1z");
		listOfRequiredVariablesForVectorsByID["b1x"] = createVectorOfLongFromList(3, "b1x", "b1y", "b1z");

		listOfRequiredVariablesForVectors["b1y"] = createVectorOfStringFromList(3, "b1x", "b1y", "b1z");
		listOfRequiredVariablesForVectorsByID["b1y"] = createVectorOfLongFromList(3, "b1x", "b1y", "b1z");

		listOfRequiredVariablesForVectors["b1z"] = createVectorOfStringFromList(3, "b1x", "b1y", "b1z");
		listOfRequiredVariablesForVectorsByID["b1z"] = createVectorOfLongFromList(3, "b1x", "b1y", "b1z");


		listOfRequiredVariablesForVectors["b_r"] = createVectorOfStringFromList(3, bx_c_, by_c_, bz_c_);
		listOfRequiredVariablesForVectorsByID["b_r"] = createVectorOfLongFromList(3, bx_c_, by_c_, bz_c_);

		listOfRequiredVariablesForVectors["b_lat"] = createVectorOfStringFromList(3, bx_c_, by_c_, bz_c_);
		listOfRequiredVariablesForVectorsByID["b_lat"] = createVectorOfLongFromList(3, bx_c_, by_c_, bz_c_);

		listOfRequiredVariablesForVectors["b_lon"] = createVectorOfStringFromList(3, bx_c_, by_c_, bz_c_);
		listOfRequiredVariablesForVectorsByID["b_lon"] = createVectorOfLongFromList(3, bx_c_, by_c_, bz_c_);

		listOfRequiredVariablesForVectors["u"] = createVectorOfStringFromList(3, ux_c_, uy_c_, uz_c_);
		listOfRequiredVariablesForVectorsByID["u"] = createVectorOfLongFromList(3, ux_c_, uy_c_, uz_c_);

		listOfRequiredVariablesForVectors[ux_] = createVectorOfStringFromList(3, ux_c_, uy_c_, uz_c_);
		listOfRequiredVariablesForVectorsByID[ux_] = createVectorOfLongFromList(3, ux_c_, uy_c_, uz_c_);

		listOfRequiredVariablesForVectors[uy_] = createVectorOfStringFromList(3, ux_c_, uy_c_, uz_c_);
		listOfRequiredVariablesForVectorsByID[uy_] = createVectorOfLongFromList(3, ux_c_, uy_c_, uz_c_);

		listOfRequiredVariablesForVectors[uz_] = createVectorOfStringFromList(3, ux_c_, uy_c_, uz_c_);
		listOfRequiredVariablesForVectorsByID[uz_] = createVectorOfLongFromList(3, ux_c_, uy_c_, uz_c_);

		listOfRequiredVariablesForVectors["v_r"] = createVectorOfStringFromList(3, ux_c_, uy_c_, uz_c_);
		listOfRequiredVariablesForVectorsByID["v_r"] = createVectorOfLongFromList(3, ux_c_, uy_c_, uz_c_);

		listOfRequiredVariablesForVectors["v_lat"] = createVectorOfStringFromList(3, ux_c_, uy_c_, uz_c_);
		listOfRequiredVariablesForVectorsByID["v_lat"] = createVectorOfLongFromList(3, ux_c_, uy_c_, uz_c_);

		listOfRequiredVariablesForVectors["v_lon"] = createVectorOfStringFromList(3, ux_c_, uy_c_, uz_c_);
		listOfRequiredVariablesForVectorsByID["v_lon"] = createVectorOfLongFromList(3, ux_c_, uy_c_, uz_c_);

		listOfRequiredVariablesForVectors["s"] = createVectorOfStringFromList(2, p_c_, rho_c_);
		listOfRequiredVariablesForVectorsByID["s"] = createVectorOfLongFromList(2, p_c_, rho_c_);

		listOfRequiredVariablesForVectors["en"] = createVectorOfStringFromList(1, "e");
		listOfRequiredVariablesForVectorsByID["en"] = createVectorOfLongFromList(1, "e");

		listOfRequiredVariablesForVectors["e"] = createVectorOfStringFromList(6, bx_c_, by_c_, bz_c_, ux_c_, uy_c_, uz_c_);
		listOfRequiredVariablesForVectorsByID["e"] = createVectorOfLongFromList(6, bx_c_, by_c_, bz_c_, ux_c_, uy_c_, uz_c_);

		listOfRequiredVariablesForVectors["nv"] = createVectorOfStringFromList(4, rho_c_, ux_c_, uy_c_, uz_c_);
		listOfRequiredVariablesForVectorsByID["nv"] = createVectorOfLongFromList(4, rho_c_, ux_c_, uy_c_, uz_c_);

		listOfRequiredVariablesForVectors["nvx"] = createVectorOfStringFromList(4, rho_c_, ux_c_, uy_c_, uz_c_);
		listOfRequiredVariablesForVectorsByID["nvx"] = createVectorOfLongFromList(4, rho_c_, ux_c_, uy_c_, uz_c_);

		listOfRequiredVariablesForVectors["nvy"] = createVectorOfStringFromList(4, rho_c_, ux_c_, uy_c_, uz_c_);
		listOfRequiredVariablesForVectorsByID["nvy"] = createVectorOfLongFromList(4, rho_c_, ux_c_, uy_c_, uz_c_);

		listOfRequiredVariablesForVectors["nvz"] = createVectorOfStringFromList(4, rho_c_, ux_c_, uy_c_, uz_c_);
		listOfRequiredVariablesForVectorsByID["nvz"] = createVectorOfLongFromList(4, rho_c_, ux_c_, uy_c_, uz_c_);

		listOfRequiredVariablesForVectors["n"] = createVectorOfStringFromList(1, rho_c_);
		listOfRequiredVariablesForVectorsByID["n"] = createVectorOfLongFromList(1, rho_c_);

		listOfRequiredVariablesForVectors["n*r^2"] = createVectorOfStringFromList(1, rho_c_);
		listOfRequiredVariablesForVectorsByID["n*r^2"] = createVectorOfLongFromList(1, rho_c_);

		listOfRequiredVariablesForVectors["polb"] = createVectorOfStringFromList(1, "bp");
		listOfRequiredVariablesForVectorsByID["polb"] = createVectorOfLongFromList(1, "bp");

		listOfRequiredVariablesForVectors["p*r^2"] = createVectorOfStringFromList(1, p_c_);
		listOfRequiredVariablesForVectorsByID["p*r^2"] = createVectorOfLongFromList(1, p_c_);

		listOfRequiredVariablesForVectors["eta*j"] = createVectorOfStringFromList(4, "eta", jx_c_, jy_c_, jz_c_);
		listOfRequiredVariablesForVectorsByID["eta*j"] = createVectorOfLongFromList(4, "eta", jx_c_, jy_c_, jz_c_);

		listOfRequiredVariablesForVectors["eta*jx"] = createVectorOfStringFromList(4, "eta", jx_c_, jy_c_, jz_c_);
		listOfRequiredVariablesForVectorsByID["eta*jx"] = createVectorOfLongFromList(4, "eta", jx_c_, jy_c_, jz_c_);

		listOfRequiredVariablesForVectors["eta*jy"] = createVectorOfStringFromList(4, "eta", jx_c_, jy_c_, jz_c_);
		listOfRequiredVariablesForVectorsByID["eta*jy"] = createVectorOfLongFromList(4, "eta", jx_c_, jy_c_, jz_c_);

		listOfRequiredVariablesForVectors["eta*jz"] = createVectorOfStringFromList(4, "eta", jx_c_, jy_c_, jz_c_);
		listOfRequiredVariablesForVectorsByID["eta*jz"] = createVectorOfLongFromList(4, "eta", jx_c_, jy_c_, jz_c_);

		listOfRequiredVariablesForVectors["eta*j^2"] = createVectorOfStringFromList(4, "eta", jx_c_, jy_c_, jz_c_);
		listOfRequiredVariablesForVectorsByID["eta*j^2"] = createVectorOfLongFromList(4, "eta", jx_c_, jy_c_, jz_c_);

		if (model->doesVariableExist(p_c_))
		{
			listOfRequiredVariablesForVectors[p_] = createVectorOfStringFromList(1, p_c_);
			listOfRequiredVariablesForVectorsByID[p_] = createVectorOfLongFromList(1, p_c_);

			listOfRequiredVariablesForVectors["beta"] = createVectorOfStringFromList(4, bx_c_, by_c_, bz_c_, p_c_);
			listOfRequiredVariablesForVectorsByID["beta"] = createVectorOfLongFromList(4, bx_c_, by_c_, bz_c_, p_c_);

		} else
		{
			if (model->doesVariableExist("T"))
			{
				listOfRequiredVariablesForVectors[p_] = createVectorOfStringFromList(2, rho_c_, "T");
				listOfRequiredVariablesForVectorsByID[p_] = createVectorOfLongFromList(2, rho_c_, "T");

				listOfRequiredVariablesForVectors["beta"] = createVectorOfStringFromList(5, bx_c_, by_c_, bz_c_, rho_c_, "T");
				listOfRequiredVariablesForVectorsByID["beta"] = createVectorOfLongFromList(5, bx_c_, by_c_, bz_c_, rho_c_, "T");

			} else if (model->doesVariableExist("t"))
			{
				listOfRequiredVariablesForVectors[p_] = createVectorOfStringFromList(2, rho_c_, "t");
				listOfRequiredVariablesForVectorsByID[p_] = createVectorOfLongFromList(2, rho_c_, "t");

				listOfRequiredVariablesForVectors["beta"] = createVectorOfStringFromList(5, bx_c_, by_c_, bz_c_, rho_c_, "t");
				listOfRequiredVariablesForVectorsByID["beta"] = createVectorOfLongFromList(5, bx_c_, by_c_, bz_c_, rho_c_, "t");

			} else
			{
				//not sure whatelse to do.
				listOfRequiredVariablesForVectors[p_] = createVectorOfStringFromList(1, rho_c_);
				listOfRequiredVariablesForVectorsByID[p_] = createVectorOfLongFromList(1, rho_c_);

				listOfRequiredVariablesForVectors["beta"] = createVectorOfStringFromList(4, bx_c_, by_c_, bz_c_, rho_c_);
				listOfRequiredVariablesForVectorsByID["beta"] = createVectorOfLongFromList(4, bx_c_, by_c_, bz_c_, rho_c_);

			}
		}

		if ("mas" == modelName)
		{
			listOfRequiredVariablesForVectors["b"] = createVectorOfStringFromList(3, "br", "bphi", "btheta");
			listOfRequiredVariablesForVectorsByID["b"] = createVectorOfLongFromList(3, "br", "bphi", "btheta");

			listOfRequiredVariablesForVectors["br"] = createVectorOfStringFromList(3, "br", "bphi", "btheta");
			listOfRequiredVariablesForVectorsByID["br"] = createVectorOfLongFromList(3, "br", "bphi", "btheta");

			listOfRequiredVariablesForVectors["btheta"] = createVectorOfStringFromList(3, "br", "bphi", "btheta");
			listOfRequiredVariablesForVectorsByID["btheta"] = createVectorOfLongFromList(3, "br", "bphi", "btheta");

			listOfRequiredVariablesForVectors["bphi"] = createVectorOfStringFromList(3, "br", "bphi", "btheta");
			listOfRequiredVariablesForVectorsByID["bphi"] = createVectorOfLongFromList(3, "br", "bphi", "btheta");

			listOfRequiredVariablesForVectors["u"] = createVectorOfStringFromList(3, "ur", "uphi", "utheta");
			listOfRequiredVariablesForVectorsByID["u"] = createVectorOfLongFromList(3, "ur", "uphi", "utheta");

			listOfRequiredVariablesForVectors["ur"] = createVectorOfStringFromList(3, "ur", "uphi", "utheta");
			listOfRequiredVariablesForVectorsByID["ur"] = createVectorOfLongFromList(3, "ur", "uphi", "utheta");

			listOfRequiredVariablesForVectors["uphi"] = createVectorOfStringFromList(3, "ur", "uphi", "utheta");
			listOfRequiredVariablesForVectorsByID["uphi"] = createVectorOfLongFromList(3, "ur", "uphi", "utheta");

			listOfRequiredVariablesForVectors["utheta"] = createVectorOfStringFromList(3, "ur", "uphi", "utheta");
			listOfRequiredVariablesForVectorsByID["utheta"] = createVectorOfLongFromList(3, "ur", "uphi", "utheta");

			listOfRequiredVariablesForVectors["nv"] = createVectorOfStringFromList(4, rho_c_, "ur", "uphi", "utheta");
			listOfRequiredVariablesForVectorsByID["nv"] = createVectorOfLongFromList(4, rho_c_, "ur", "uphi", "utheta");

			listOfRequiredVariablesForVectors["nvr"] = createVectorOfStringFromList(4, rho_c_, "ur", "uphi", "utheta");
			listOfRequiredVariablesForVectorsByID["nvr"] = createVectorOfLongFromList(4, rho_c_, "ur", "uphi", "utheta");

			listOfRequiredVariablesForVectors["nvtheta"] = createVectorOfStringFromList(4, rho_c_, "ur", "uphi", "utheta");
			listOfRequiredVariablesForVectorsByID["nvtheta"] = createVectorOfLongFromList(4, rho_c_, "ur", "uphi", "utheta");

			listOfRequiredVariablesForVectors["nvphi"] = createVectorOfStringFromList(4, rho_c_, "ur", "uphi", "utheta");
			listOfRequiredVariablesForVectorsByID["nvphi"] = createVectorOfLongFromList(4, rho_c_, "ur", "uphi", "utheta");

			listOfRequiredVariablesForVectors["pram"] = createVectorOfStringFromList(4, rho_c_, "ur", "uphi", "utheta");
			listOfRequiredVariablesForVectorsByID["pram"] = createVectorOfLongFromList(4, rho_c_, "ur", "uphi", "utheta");

			listOfRequiredVariablesForVectors["exb"] = createVectorOfStringFromList(6, "br", "bphi", "btheta", "ur", "uphi",
					"utheta");
			listOfRequiredVariablesForVectorsByID["exb"] = createVectorOfLongFromList(6, "br", "bphi", "btheta", "ur", "uphi",
					"utheta");

			listOfRequiredVariablesForVectors["exbr"] = createVectorOfStringFromList(6, "br", "bphi", "btheta", "ur", "uphi",
					"utheta");
			listOfRequiredVariablesForVectorsByID["exbr"] = createVectorOfLongFromList(6, "br", "bphi", "btheta", "ur", "uphi",
					"utheta");

			listOfRequiredVariablesForVectors["exbtheta"] = createVectorOfStringFromList(6, "br", "bphi", "btheta", "ur",
					"uphi", "utheta");
			listOfRequiredVariablesForVectorsByID["exbtheta"] = createVectorOfLongFromList(6, "br", "bphi", "btheta", "ur",
					"uphi", "utheta");

			listOfRequiredVariablesForVectors["exbphi"] = createVectorOfStringFromList(6, "br", "bphi", "btheta", "ur", "uphi",
					"utheta");
			listOfRequiredVariablesForVectorsByID["exbphi"] = createVectorOfLongFromList(6, "br", "bphi", "btheta", "ur", "uphi",
					"utheta");

			listOfRequiredVariablesForVectors["e"] = createVectorOfStringFromList(6, "br", "bphi", "btheta", "ur", "uphi",
					"utheta");
			listOfRequiredVariablesForVectorsByID["e"] = createVectorOfLongFromList(6, "br", "bphi", "btheta", "ur", "uphi",
					"utheta");

			listOfRequiredVariablesForVectors["er"] = createVectorOfStringFromList(6, "br", "bphi", "btheta", "ur", "uphi",
					"utheta");
			listOfRequiredVariablesForVectorsByID["er"] = createVectorOfLongFromList(6, "br", "bphi", "btheta", "ur", "uphi",
					"utheta");

			listOfRequiredVariablesForVectors["ephi"] = createVectorOfStringFromList(6, "br", "bphi", "btheta", "ur", "uphi",
					"utheta");
			listOfRequiredVariablesForVectorsByID["ephi"] = createVectorOfLongFromList(6, "br", "bphi", "btheta", "ur", "uphi",
					"utheta");

			listOfRequiredVariablesForVectors["etheta"] = createVectorOfStringFromList(6, "br", "bphi", "btheta", "ur", "uphi",
					"utheta");
			listOfRequiredVariablesForVectorsByID["etheta"] = createVectorOfLongFromList(6, "br", "bphi", "btheta", "ur", "uphi",
					"utheta");

		}

		if ("enlil" == modelName)
		{

			listOfRequiredVariablesForVectors["b"] = createVectorOfStringFromList(3, "br", "bphi", "btheta");
			listOfRequiredVariablesForVectorsByID["b"] = createVectorOfLongFromList(3, "br", "bphi", "btheta");

			listOfRequiredVariablesForVectors["b1"] = createVectorOfStringFromList(3, "b1r", "b1phi", "b1theta");
			listOfRequiredVariablesForVectorsByID["b1"] = createVectorOfLongFromList(3, "b1r", "b1phi", "b1theta");

			listOfRequiredVariablesForVectors["b1r"] = createVectorOfStringFromList(3, "b1r", "b1phi", "b1theta");
			listOfRequiredVariablesForVectorsByID["b1r"] = createVectorOfLongFromList(3, "b1r", "b1phi", "b1theta");

			listOfRequiredVariablesForVectors["b1phi"] = createVectorOfStringFromList(3, "b1r", "b1phi", "b1theta");
			listOfRequiredVariablesForVectorsByID["b1phi"] = createVectorOfLongFromList(3, "b1r", "b1phi", "b1theta");

			listOfRequiredVariablesForVectors["b1theta"] = createVectorOfStringFromList(3, "b1r", "b1phi", "b1theta");
			listOfRequiredVariablesForVectorsByID["b1theta"] = createVectorOfLongFromList(3, "b1r", "b1phi", "b1theta");


			listOfRequiredVariablesForVectors["br"] = createVectorOfStringFromList(3, "br", "bphi", "btheta");
			listOfRequiredVariablesForVectorsByID["br"] = createVectorOfLongFromList(3, "br", "bphi", "btheta");

			listOfRequiredVariablesForVectors["bphi"] = createVectorOfStringFromList(3, "br", "bphi", "btheta");
			listOfRequiredVariablesForVectorsByID["bphi"] = createVectorOfLongFromList(3, "br", "bphi", "btheta");

			listOfRequiredVariablesForVectors["btheta"] = createVectorOfStringFromList(3, "br", "bphi", "btheta");
			listOfRequiredVariablesForVectorsByID["btheta"] = createVectorOfLongFromList(3, "br", "bphi", "btheta");

			listOfRequiredVariablesForVectors["e"] = createVectorOfStringFromList(6, "br", "bphi", "btheta", "ur", "uphi", "utheta");
			listOfRequiredVariablesForVectorsByID["e"] = createVectorOfLongFromList(6, "br", "bphi", "btheta", "ur", "uphi", "utheta");

			listOfRequiredVariablesForVectors["er"] = createVectorOfStringFromList(6, "br", "bphi", "btheta", "ur", "uphi", "utheta");
			listOfRequiredVariablesForVectorsByID["er"] = createVectorOfLongFromList(6, "br", "bphi", "btheta", "ur", "uphi", "utheta");

			listOfRequiredVariablesForVectors["ephi"] = createVectorOfStringFromList(6, "br", "bphi", "btheta", "ur", "uphi", "utheta");
			listOfRequiredVariablesForVectorsByID["ephi"] = createVectorOfLongFromList(6, "br", "bphi", "btheta", "ur", "uphi", "utheta");

			listOfRequiredVariablesForVectors["etheta"] = createVectorOfStringFromList(6, "br", "bphi", "btheta", "ur", "uphi", "utheta");
			listOfRequiredVariablesForVectorsByID["etheta"] = createVectorOfLongFromList(6, "br", "bphi", "btheta", "ur", "uphi", "utheta");

		}

	}
}
