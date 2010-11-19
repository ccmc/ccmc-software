#include "Kameleon.h"
#include "StringConstants.h"

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


		std::string pram[] = {rho_, ux_, uy_, uz_};
		std::string t[] = {p_, rho_};
		std::string ex[] = {by_,bz_,uy_,uz_};
		std::string er[] = {by_,bz_,uy_,uz_};
		std::string ey[] = {bx_,bz_,ux_,uz_};
		std::string e_lat[] = {bx_,bz_,ux_,uz_};
		std::string ez[] = {bx_,by_,ux_,uy_};
		std::string e_lon[] = {bx_,by_,ux_,uy_};
		std::string edotj[] = {jx_,jy_,jz_,bx_,by_,bz_,ux_,uy_,uz_};
		std::string jparx[] = {jx_,jy_,jz_,bx_,by_,bz_};
		std::string jpary[] = {jx_,jy_,jz_,bx_,by_,bz_};
		std::string jparz[] = {jx_,jy_,jz_,bx_,by_,bz_};
		std::string jparr[] = {jr_,jphi_,jtheta_,br_,bphi_,btheta_};
		std::string jparphi[] = {jr_,jphi_,jtheta_,br_,bphi_,btheta_};
		std::string jpartheta[] = {jr_,jphi_,jtheta_,br_,bphi_,btheta_};
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
		std::string b1[] = {b1x_,b1y_,b1z_};
		std::string b_r[] = {bx_};
		std::string b_lat[] = {by_};
		std::string b_lon[] = {bz_};
		std::string u[] = {ux_,uy_,uz_};
		std::string v[] = {ux_,uy_,uz_};
		std::string v_r[] = {ux_};
		std::string v_lat[] = {uy_};
		std::string v_lon[] = {uz_};
		std::string s[] = {p_,rho_};
		std::string en[] = {e_};
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
		std::string eta_times_j_tothe_2_power[] = {eta_, jx_, jy_, jz_};
		std::string grad_n[] = {rho_};
		std::string grad_b[] = {bx_,by_,bz_};
		std::string p[] = {p_};
		std::string p_without_p_T[] = {rho_, "T"};
		std::string p_without_p_t[] = {rho_, "t"};
		std::string beta[] = {bx_, by_, bz_, p_};
		std::string beta_without_p_T[] = {bx_, by_, bz_, rho_, "T"};
		std::string beta_without_p_t[] = {bx_, by_, bz_, rho_, "t"};

		addRequirementsForComponents(pram_, 4, pram);
		addRequirementsForComponents(t_, 2, t);
		addRequirementsForComponents(ex_, 4, ex);
		addRequirementsForComponents(ey_, 4, ey);
		addRequirementsForComponents(ez_, 4, ez);
		addRequirementsForComponents(edotj_, 9, edotj);
		addRequirementsForComponents(jparx_, 6, jparx);
		addRequirementsForComponents(jpary_, 6, jpary);
		addRequirementsForComponents(jparz_, 6, jparz);
		addRequirementsForComponents(jpar_, 6, jpar);
		addRequirementsForComponents(jxbx_, 4, jxbx);
		addRequirementsForComponents(jxby_, 4, jxby);
		addRequirementsForComponents(jxbz_, 4, jxbz);
		addRequirementsForComponents(jxb_, 6, jxb);
		addRequirementsForComponents(exbx_, 6, exbx);
		addRequirementsForComponents(exby_, 6, exby);
		addRequirementsForComponents(exbz_, 6, exbz);
		addRequirementsForComponents(exb_, 6, exbx);
		addRequirementsForComponents(j_, 3, j);
		addRequirementsForComponents(b_, 3, b);
		addRequirementsForComponents(b1_, 3, b1);
		addRequirementsForComponents(u_, 3, u);
		addRequirementsForComponents(s_, 2, s);
		addRequirementsForComponents(en_, 1, en);
		addRequirementsForComponents(e_, 6, e);
		addRequirementsForComponents(v_, 3, v);
		addRequirementsForComponents(nv_, 4, nv);
		addRequirementsForComponents(nvx_, 2, nvx);
		addRequirementsForComponents(nvy_, 2, nvy);
		addRequirementsForComponents(nvz_, 2, nvz);
		addRequirementsForComponents(n_, 1, n);
		addRequirementsForComponents(n_times_r_tothe_2_power_, 1, n_times_r_tothe_2_power);
		addRequirementsForComponents(polb_, 1, polb);
		addRequirementsForComponents(p_times_r_tothe_2_power_, 1, p_times_r_tothe_2_power);
		addRequirementsForComponents(eta_times_jx_, 2, eta_times_jx);
		addRequirementsForComponents(eta_times_jy_, 2, eta_times_jy);
		addRequirementsForComponents(eta_times_jz_, 2, eta_times_jz);
		addRequirementsForComponents(eta_times_j_tothe_2_power_, 4, eta_times_j_tothe_2_power);
		addRequirementsForComponents(grad_n_, 1, grad_n);
		addRequirementsForComponents(grad_b_, 3, grad_b);
		addRequirementsForComponents(p_, 1, p);
		addRequirementsForComponents(beta_, 4, beta);
		addRequirementsForComponents(p_, 2, p_without_p_T);
		addRequirementsForComponents(p_, 2, p_without_p_t);
		addRequirementsForComponents(beta_, 5, beta_without_p_T);
		addRequirementsForComponents(beta_, 5, beta_without_p_t);


		/*




		if (modelName == "enlil")
		{
			listOfRequiredVariablesForComponents["b1"] = createVectorOfStringFromList(3, b1r_, b1phi_, b1theta_);
			listOfRequiredVariablesForComponentsByID["b1"] = createVectorOfLongFromList(3, b1r_, b1phi_, b1theta_);

		}

		//TODO: do error checking to set the required variables!
		if (modelName == "mas" ||  modelName == "enlil")
		{
			//they probably use the newer naming convention for variables
			if (this->doesVariableExist(ccmc::strings::variables::ur_))
			{

					listOfRequiredVariablesForComponents["b_r"] = createVectorOfStringFromList(1, {bx_});
		listOfRequiredVariablesForComponentsByID["b_r"] = createVectorOfLongFromList(1, {bx_});

		listOfRequiredVariablesForComponents["b_lat"] = createVectorOfStringFromList(1, {by_});
		listOfRequiredVariablesForComponentsByID["b_lat"] = createVectorOfLongFromList(1, {by_});

		listOfRequiredVariablesForComponents["b_lon"] = createVectorOfStringFromList(1, {bz_});
		listOfRequiredVariablesForComponentsByID["b_lon"] = createVectorOfLongFromList(1, {bz_});

					listOfRequiredVariablesForComponents["exb_r"] = createVectorOfStringFromList(6, {bx_, by_, bz_, ux_, uy_, uz_});
		listOfRequiredVariablesForComponentsByID["exb_r"] = createVectorOfLongFromList(6, {bx_, by_, bz_, ux_, uy_, uz_});

					listOfRequiredVariablesForComponents["jparr"] = createVectorOfStringFromList(6, {jr_, jphi_, jtheta_, br_, bphi_,
				btheta_});
		listOfRequiredVariablesForComponentsByID["jparr"] = createVectorOfLongFromList(6, {jr_, jphi_, jtheta_, br_, bphi_,
				btheta_});

		listOfRequiredVariablesForComponents["jparphi"] = createVectorOfStringFromList(6, {jr_, jphi_, jtheta_, br_, bphi_,
				btheta_});
		listOfRequiredVariablesForComponentsByID["jparphi"] = createVectorOfLongFromList(6, {jr_, jphi_, jtheta_, br_, bphi_,
				btheta_});

		listOfRequiredVariablesForComponents["jpartheta"] = createVectorOfStringFromList(6, {jr_, jphi_, jtheta_, br_,
				bphi_, btheta_});
		listOfRequiredVariablesForComponentsByID["jpartheta"] = createVectorOfLongFromList(6, {jr_, jphi_, jtheta_, br_,
				bphi_, btheta_});
				listOfRequiredVariablesForComponents["pram"] = createVectorOfStringFromList(4, {rho_, ur_, uphi_, utheta_});
				listOfRequiredVariablesForComponentsByID["pram"] = createVectorOfLongFromList(4, {rho_, ur_, uphi_, utheta_});


				listOfRequiredVariablesForComponents["exb"] = createVectorOfStringFromList(6, {br_, bphi_, btheta_, ur_, uphi_,
						utheta_});
				listOfRequiredVariablesForComponentsByID["exb"] = createVectorOfLongFromList(6, {br_, bphi_, btheta_, ur_, uphi_,
						utheta_});

				listOfRequiredVariablesForComponents["exbr"] = createVectorOfStringFromList(6, {br_, bphi_, btheta_, ur_,
						uphi_, utheta_});
				listOfRequiredVariablesForComponentsByID["exbr"] = createVectorOfLongFromList(6, {br_, bphi_, btheta_, ur_,
						uphi_, utheta_});

				listOfRequiredVariablesForComponents["exbtheta"] = createVectorOfStringFromList(6, {br_, bphi_, btheta_, ur_,
						uphi_, utheta_});
				listOfRequiredVariablesForComponentsByID["exbtheta"] = createVectorOfLongFromList(6, {br_, bphi_, btheta_, ur_,
						uphi_, utheta_});

				listOfRequiredVariablesForComponents["exbphi"] = createVectorOfStringFromList(6, {br_, bphi_, btheta_, ur_,
						uphi_, utheta_});
				listOfRequiredVariablesForComponentsByID["exbphi"] = createVectorOfLongFromList(6, {br_, bphi_, btheta_, ur_,
						uphi_, utheta_});

				listOfRequiredVariablesForComponents["edotj"] = createVectorOfStringFromList(9, {jr_, jphi_, jtheta_, br_,
						bphi_, btheta_, ur_, uphi_, utheta_});
				listOfRequiredVariablesForComponentsByID["edotj"] = createVectorOfLongFromList(9, {jr_, jphi_, jtheta_, br_,
						bphi_, btheta_, ur_, uphi_, utheta_});

				listOfRequiredVariablesForComponents["v"] = createVectorOfStringFromList(3, {ur_, uphi_, utheta_});
				listOfRequiredVariablesForComponentsByID["v"] = createVectorOfLongFromList(3, {ur_, uphi_, utheta_});

				listOfRequiredVariablesForComponents["u"] = createVectorOfStringFromList(3, {ur_, uphi_, utheta_});
				listOfRequiredVariablesForComponentsByID["u"] = createVectorOfLongFromList(3, {ur_, uphi_, utheta_});

				listOfRequiredVariablesForComponents["j"] = createVectorOfStringFromList(3, {jr_, jphi_, jtheta_});
				listOfRequiredVariablesForComponentsByID["j"] = createVectorOfLongFromList(3, {jr_, jphi_, jtheta_});

				listOfRequiredVariablesForComponents["b"] = createVectorOfStringFromList(3, {br_, bphi_, btheta_});
				listOfRequiredVariablesForComponentsByID["b"] = createVectorOfLongFromList(3, {br_, bphi_, btheta_});

				listOfRequiredVariablesForComponents["er"] = createVectorOfStringFromList(4, {bphi_, btheta_, utheta_, uphi_});
				listOfRequiredVariablesForComponentsByID["er"] = createVectorOfLongFromList(4, {bphi_, btheta_, utheta_, uphi_});

				listOfRequiredVariablesForComponents["ephi"] = createVectorOfStringFromList(4, {br_, bphi_, ur_, uphi_});
				listOfRequiredVariablesForComponentsByID["ephi"] = createVectorOfLongFromList(4, {br_, bphi_, ur_, uphi_});

				listOfRequiredVariablesForComponents["etheta"] = createVectorOfStringFromList(4, {br_, btheta_, ur_, utheta_});
				listOfRequiredVariablesForComponentsByID["etheta"] = createVectorOfLongFromList(4, {br_, btheta_, ur_, utheta_});

				listOfRequiredVariablesForComponents[e_] = createVectorOfStringFromList(6, {br_, bphi_, btheta_, ur_, uphi_, utheta_});
				listOfRequiredVariablesForComponentsByID[e_] = createVectorOfLongFromList(6, {br_, bphi_, btheta_, ur_, uphi_, utheta_});

				listOfRequiredVariablesForComponents["jpar"] = createVectorOfStringFromList(6, {jr_, jphi_, jtheta_, br_, bphi_,
						btheta_});
				listOfRequiredVariablesForComponentsByID["jpar"] = createVectorOfLongFromList(6, {jr_, jphi_, jtheta_, br_, bphi_,
						btheta_});
			}

		} else
		{

		}

		//add variables for vectors.  same as above, except, for vectors, we want to add all components
		listOfRequiredVariablesForVectors["pram"] = createVectorOfStringFromList(4, {rho_, ux_, uy_, uz_});
		listOfRequiredVariablesForVectorsByID["pram"] = createVectorOfLongFromList(4, {rho_, ux_, uy_, uz_});

		listOfRequiredVariablesForVectors["t"] = createVectorOfStringFromList(2, {p_, rho_});
		listOfRequiredVariablesForVectorsByID["t"] = createVectorOfLongFromList(2, {p_, rho_});

		listOfRequiredVariablesForVectors["ex"] = createVectorOfStringFromList(6, {bx_, by_, bz_, ux_, uy_, uz_});
		listOfRequiredVariablesForVectorsByID["ex"] = createVectorOfLongFromList(6, {bx_, by_, bz_, ux_, uy_, uz_});

		listOfRequiredVariablesForVectors["e_r"] = createVectorOfStringFromList(6, {bx_, by_, bz_, ux_, uy_, uz_});
		listOfRequiredVariablesForVectorsByID["e_r"] = createVectorOfLongFromList(6, {bx_, by_, bz_, ux_, uy_, uz_});

		listOfRequiredVariablesForVectors["ey"] = createVectorOfStringFromList(6, {bx_, by_, bz_, ux_, uy_, uz_});
		listOfRequiredVariablesForVectorsByID["ey"] = createVectorOfLongFromList(6, {bx_, by_, bz_, ux_, uy_, uz_});

		listOfRequiredVariablesForVectors["e_lat"] = createVectorOfStringFromList(6, {bx_, by_, bz_, ux_, uy_, uz_});
		listOfRequiredVariablesForVectorsByID["e_lat"] = createVectorOfLongFromList(6, {bx_, by_, bz_, ux_, uy_, uz_});

		listOfRequiredVariablesForVectors["ez"] = createVectorOfStringFromList(6, {bx_, by_, bz_, ux_, uy_, uz_});
		listOfRequiredVariablesForVectorsByID["ez"] = createVectorOfLongFromList(6, {bx_, by_, bz_, ux_, uy_, uz_});

		listOfRequiredVariablesForVectors["e_lon"] = createVectorOfStringFromList(6, {bx_, by_, bz_, ux_, uy_, uz_});
		listOfRequiredVariablesForVectorsByID["e_lon"] = createVectorOfLongFromList(6, {bx_, by_, bz_, ux_, uy_, uz_});

		listOfRequiredVariablesForVectors["edotj"] = createVectorOfStringFromList(9, {jx_, jy_, jz_, bx_, by_, bz_, ux_, uy_,
				uz_});
		listOfRequiredVariablesForVectorsByID["edotj"] = createVectorOfLongFromList(9, {jx_, jy_, jz_, bx_, by_, bz_, ux_, uy_,
				uz_});

		listOfRequiredVariablesForVectors["jparx"] = createVectorOfStringFromList(6, {jx_, jy_, jz_, bx_, by_, bz_});
		listOfRequiredVariablesForVectorsByID["jparx"] = createVectorOfLongFromList(6, {jx_, jy_, jz_, bx_, by_, bz_});

		listOfRequiredVariablesForVectors["jpary"] = createVectorOfStringFromList(6, {jx_, jy_, jz_, bx_, by_, bz_});
		listOfRequiredVariablesForVectorsByID["jpary"] = createVectorOfLongFromList(6, {jx_, jy_, jz_, bx_, by_, bz_});

		listOfRequiredVariablesForVectors["jparz"] = createVectorOfStringFromList(6, {jx_, jy_, jz_, bx_, by_, bz_});
		listOfRequiredVariablesForVectorsByID["jparz"] = createVectorOfLongFromList(6, {jx_, jy_, jz_, bx_, by_, bz_});

		listOfRequiredVariablesForVectors["jpar"] = createVectorOfStringFromList(6, {jx_, jy_, jz_, bx_, by_, bz_});
		listOfRequiredVariablesForVectorsByID["jpar"] = createVectorOfLongFromList(6, {jx_, jy_, jz_, bx_, by_, bz_});

		listOfRequiredVariablesForVectors["jxbx"] = createVectorOfStringFromList(6, {bx_, by_, bz_, jx_, jy_, jz_});
		listOfRequiredVariablesForVectorsByID["jxbx"] = createVectorOfLongFromList(6, {bx_, by_, bz_, jx_, jy_, jz_});

		listOfRequiredVariablesForVectors["jxby"] = createVectorOfStringFromList(6, {bx_, by_, bz_, jx_, jy_, jz_});
		listOfRequiredVariablesForVectorsByID["jxby"] = createVectorOfLongFromList(6, {bx_, by_, bz_, jx_, jy_, jz_});

		listOfRequiredVariablesForVectors["jxbz"] = createVectorOfStringFromList(6, {bx_, by_, bz_, jx_, jy_, jz_});
		listOfRequiredVariablesForVectorsByID["jxbz"] = createVectorOfLongFromList(6, {bx_, by_, bz_, jx_, jy_, jz_});

		listOfRequiredVariablesForVectors["jxb"] = createVectorOfStringFromList(6, {bx_, by_, bz_, jx_, jy_, jz_});
		listOfRequiredVariablesForVectorsByID["jxb"] = createVectorOfLongFromList(6, {bx_, by_, bz_, jx_, jy_, jz_});

		listOfRequiredVariablesForVectors["exbx"] = createVectorOfStringFromList(6, {bx_, by_, bz_, ux_, uy_, uz_});
		listOfRequiredVariablesForVectorsByID["exbx"] = createVectorOfLongFromList(6, {bx_, by_, bz_, ux_, uy_, uz_});

		listOfRequiredVariablesForVectors["exby"] = createVectorOfStringFromList(6, {bx_, by_, bz_, ux_, uy_, uz_});
		listOfRequiredVariablesForVectorsByID["exby"] = createVectorOfLongFromList(6, {bx_, by_, bz_, ux_, uy_, uz_});

		listOfRequiredVariablesForVectors["exbz"] = createVectorOfStringFromList(6, {bx_, by_, bz_, ux_, uy_, uz_});
		listOfRequiredVariablesForVectorsByID["exbz"] = createVectorOfLongFromList(6, {bx_, by_, bz_, ux_, uy_, uz_});

		listOfRequiredVariablesForVectors["exb"] = createVectorOfStringFromList(6, {bx_, by_, bz_, ux_, uy_, uz_});
		listOfRequiredVariablesForVectorsByID["exb"] = createVectorOfLongFromList(6, {bx_, by_, bz_, ux_, uy_, uz_});

		listOfRequiredVariablesForVectors["exb_r"] = createVectorOfStringFromList(6, {bx_, by_, bz_, ux_, uy_, uz_});
		listOfRequiredVariablesForVectorsByID["exb_r"] = createVectorOfLongFromList(6, {bx_, by_, bz_, ux_, uy_, uz_});

		listOfRequiredVariablesForVectors["exb_lat"] = createVectorOfStringFromList(6, {bx_, by_, bz_, ux_, uy_, uz_});
		listOfRequiredVariablesForVectorsByID["exb_lat"] = createVectorOfLongFromList(6, {bx_, by_, bz_, ux_, uy_, uz_});

		listOfRequiredVariablesForVectors["exb_lon"] = createVectorOfStringFromList(6, {bx_, by_, bz_, ux_, uy_, uz_});
		listOfRequiredVariablesForVectorsByID["exb_lon"] = createVectorOfLongFromList(6, {bx_, by_, bz_, ux_, uy_, uz_});

		listOfRequiredVariablesForVectors["j"] = createVectorOfStringFromList(3, {jx_, jy_, jz_});
		listOfRequiredVariablesForVectorsByID["j"] = createVectorOfLongFromList(3, {jx_, jy_, jz_});

		listOfRequiredVariablesForVectors[jx_] = createVectorOfStringFromList(3, {jx_, jy_, jz_});
		listOfRequiredVariablesForVectorsByID[jx_] = createVectorOfLongFromList(3, {jx_, jy_, jz_});

		listOfRequiredVariablesForVectors[jy_] = createVectorOfStringFromList(3, {jx_, jy_, jz_});
		listOfRequiredVariablesForVectorsByID[jy_] = createVectorOfLongFromList(3, {jx_, jy_, jz_});

		listOfRequiredVariablesForVectors[jz_] = createVectorOfStringFromList(3, {jx_, jy_, jz_});
		listOfRequiredVariablesForVectorsByID[jz_] = createVectorOfLongFromList(3, {jx_, jy_, jz_});

		listOfRequiredVariablesForVectors["b"] = createVectorOfStringFromList(3, {bx_, by_, bz_});
		listOfRequiredVariablesForVectorsByID["b"] = createVectorOfLongFromList(3, {bx_, by_, bz_});

		listOfRequiredVariablesForVectors[bx_] = createVectorOfStringFromList(3, {bx_, by_, bz_});
		listOfRequiredVariablesForVectorsByID[bx_] = createVectorOfLongFromList(3, {bx_, by_, bz_});

		listOfRequiredVariablesForVectors[by_] = createVectorOfStringFromList(3, {bx_, by_, bz_});
		listOfRequiredVariablesForVectorsByID[by_] = createVectorOfLongFromList(3, {bx_, by_, bz_});

		listOfRequiredVariablesForVectors[bz_] = createVectorOfStringFromList(3, {bx_, by_, bz_});
		listOfRequiredVariablesForVectorsByID[bz_] = createVectorOfLongFromList(3, {bx_, by_, bz_});

		listOfRequiredVariablesForVectors["b1"] = createVectorOfStringFromList(3, {b1x_, b1y_, b1z_});
		listOfRequiredVariablesForVectorsByID["b1"] = createVectorOfLongFromList(3, {b1x_, b1y_, b1z_});

		listOfRequiredVariablesForVectors[b1x_] = createVectorOfStringFromList(3, {b1x_, b1y_, b1z_});
		listOfRequiredVariablesForVectorsByID[b1x_] = createVectorOfLongFromList(3, {b1x_, b1y_, b1z_});

		listOfRequiredVariablesForVectors[b1y_] = createVectorOfStringFromList(3, {b1x_, b1y_, b1z_});
		listOfRequiredVariablesForVectorsByID[b1y_] = createVectorOfLongFromList(3, {b1x_, b1y_, b1z_});

		listOfRequiredVariablesForVectors[b1z_] = createVectorOfStringFromList(3, {b1x_, b1y_, b1z_});
		listOfRequiredVariablesForVectorsByID[b1z_] = createVectorOfLongFromList(3, {b1x_, b1y_, b1z_});


		listOfRequiredVariablesForVectors["b_r"] = createVectorOfStringFromList(3, {bx_, by_, bz_});
		listOfRequiredVariablesForVectorsByID["b_r"] = createVectorOfLongFromList(3, {bx_, by_, bz_});

		listOfRequiredVariablesForVectors["b_lat"] = createVectorOfStringFromList(3, {bx_, by_, bz_});
		listOfRequiredVariablesForVectorsByID["b_lat"] = createVectorOfLongFromList(3, {bx_, by_, bz_});

		listOfRequiredVariablesForVectors["b_lon"] = createVectorOfStringFromList(3, {bx_, by_, bz_});
		listOfRequiredVariablesForVectorsByID["b_lon"] = createVectorOfLongFromList(3, {bx_, by_, bz_});

		listOfRequiredVariablesForVectors["u"] = createVectorOfStringFromList(3, {ux_, uy_, uz_});
		listOfRequiredVariablesForVectorsByID["u"] = createVectorOfLongFromList(3, {ux_, uy_, uz_});

		listOfRequiredVariablesForVectors[ux_] = createVectorOfStringFromList(3, {ux_, uy_, uz_});
		listOfRequiredVariablesForVectorsByID[ux_] = createVectorOfLongFromList(3, {ux_, uy_, uz_});

		listOfRequiredVariablesForVectors[uy_] = createVectorOfStringFromList(3, {ux_, uy_, uz_});
		listOfRequiredVariablesForVectorsByID[uy_] = createVectorOfLongFromList(3, {ux_, uy_, uz_});

		listOfRequiredVariablesForVectors[uz_] = createVectorOfStringFromList(3, {ux_, uy_, uz_});
		listOfRequiredVariablesForVectorsByID[uz_] = createVectorOfLongFromList(3, {ux_, uy_, uz_});

		listOfRequiredVariablesForVectors["v_r"] = createVectorOfStringFromList(3, {ux_, uy_, uz_});
		listOfRequiredVariablesForVectorsByID["v_r"] = createVectorOfLongFromList(3, {ux_, uy_, uz_});

		listOfRequiredVariablesForVectors["v_lat"] = createVectorOfStringFromList(3, {ux_, uy_, uz_});
		listOfRequiredVariablesForVectorsByID["v_lat"] = createVectorOfLongFromList(3, {ux_, uy_, uz_});

		listOfRequiredVariablesForVectors["v_lon"] = createVectorOfStringFromList(3, {ux_, uy_, uz_});
		listOfRequiredVariablesForVectorsByID["v_lon"] = createVectorOfLongFromList(3, {ux_, uy_, uz_});

		listOfRequiredVariablesForVectors["s"] = createVectorOfStringFromList(2, {p_, rho_});
		listOfRequiredVariablesForVectorsByID["s"] = createVectorOfLongFromList(2, {p_, rho_});

		listOfRequiredVariablesForVectors["en"] = createVectorOfStringFromList(1, {e_});
		listOfRequiredVariablesForVectorsByID["en"] = createVectorOfLongFromList(1, e_);

		listOfRequiredVariablesForVectors[e_] = createVectorOfStringFromList(6, {bx_, by_, bz_, ux_, uy_, uz_});
		listOfRequiredVariablesForVectorsByID[e_] = createVectorOfLongFromList(6, {bx_, by_, bz_, ux_, uy_, uz_});

		listOfRequiredVariablesForVectors["nv"] = createVectorOfStringFromList(4, {rho_, ux_, uy_, uz_});
		listOfRequiredVariablesForVectorsByID["nv"] = createVectorOfLongFromList(4, {rho_, ux_, uy_, uz_});

		listOfRequiredVariablesForVectors["nvx"] = createVectorOfStringFromList(4, {rho_, ux_, uy_, uz_});
		listOfRequiredVariablesForVectorsByID["nvx"] = createVectorOfLongFromList(4, {rho_, ux_, uy_, uz_});

		listOfRequiredVariablesForVectors["nvy"] = createVectorOfStringFromList(4, {rho_, ux_, uy_, uz_});
		listOfRequiredVariablesForVectorsByID["nvy"] = createVectorOfLongFromList(4, {rho_, ux_, uy_, uz_});

		listOfRequiredVariablesForVectors["nvz"] = createVectorOfStringFromList(4, {rho_, ux_, uy_, uz_});
		listOfRequiredVariablesForVectorsByID["nvz"] = createVectorOfLongFromList(4, {rho_, ux_, uy_, uz_});

		listOfRequiredVariablesForVectors["n"] = createVectorOfStringFromList(1, {rho_});
		listOfRequiredVariablesForVectorsByID["n"] = createVectorOfLongFromList(1, {rho_});

		listOfRequiredVariablesForVectors["n*r^2"] = createVectorOfStringFromList(1, {rho_});
		listOfRequiredVariablesForVectorsByID["n*r^2"] = createVectorOfLongFromList(1, {rho_});

		listOfRequiredVariablesForVectors["polb"] = createVectorOfStringFromList(1, {"bp"});
		listOfRequiredVariablesForVectorsByID["polb"] = createVectorOfLongFromList(1, {"bp"});

		listOfRequiredVariablesForVectors["p*r^2"] = createVectorOfStringFromList(1, {p_});
		listOfRequiredVariablesForVectorsByID["p*r^2"] = createVectorOfLongFromList(1, {p_});

		listOfRequiredVariablesForVectors["eta*j"] = createVectorOfStringFromList(4, {eta_, jx_, jy_, jz_});
		listOfRequiredVariablesForVectorsByID["eta*j"] = createVectorOfLongFromList(4, {eta_, jx_, jy_, jz_});

		listOfRequiredVariablesForVectors["eta*jx"] = createVectorOfStringFromList(4, {eta_, jx_, jy_, jz_});
		listOfRequiredVariablesForVectorsByID["eta*jx"] = createVectorOfLongFromList(4, {eta_, jx_, jy_, jz_});

		listOfRequiredVariablesForVectors["eta*jy"] = createVectorOfStringFromList(4, {eta_, jx_, jy_, jz_});
		listOfRequiredVariablesForVectorsByID["eta*jy"] = createVectorOfLongFromList(4, {eta_, jx_, jy_, jz_});

		listOfRequiredVariablesForVectors["eta*jz"] = createVectorOfStringFromList(4, {eta_, jx_, jy_, jz_});
		listOfRequiredVariablesForVectorsByID["eta*jz"] = createVectorOfLongFromList(4, {eta_, jx_, jy_, jz_});

		listOfRequiredVariablesForVectors["eta*j^2"] = createVectorOfStringFromList(4, {eta_, jx_, jy_, jz_});
		listOfRequiredVariablesForVectorsByID["eta*j^2"] = createVectorOfLongFromList(4, {eta_, jx_, jy_, jz_});

		if (model->doesVariableExist(p_))
		{
			listOfRequiredVariablesForVectors[p_] = createVectorOfStringFromList(1, {p_});
			listOfRequiredVariablesForVectorsByID[p_] = createVectorOfLongFromList(1, {p_});

			listOfRequiredVariablesForVectors["beta"] = createVectorOfStringFromList(4, {bx_, by_, bz_, p_});
			listOfRequiredVariablesForVectorsByID["beta"] = createVectorOfLongFromList(4, {bx_, by_, bz_, p_});

		} else
		{
			if (model->doesVariableExist("T"))
			{
				listOfRequiredVariablesForVectors[p_] = createVectorOfStringFromList(2, {rho_, "T"});
				listOfRequiredVariablesForVectorsByID[p_] = createVectorOfLongFromList(2, {rho_, "T"});

				listOfRequiredVariablesForVectors["beta"] = createVectorOfStringFromList(5, {bx_, by_, bz_, rho_, "T"});
				listOfRequiredVariablesForVectorsByID["beta"] = createVectorOfLongFromList(5, {bx_, by_, bz_, rho_, "T"});

			} else if (model->doesVariableExist("t"))
			{
				listOfRequiredVariablesForVectors[p_] = createVectorOfStringFromList(2, {rho_, "t"});
				listOfRequiredVariablesForVectorsByID[p_] = createVectorOfLongFromList(2, {rho_, "t"});

				listOfRequiredVariablesForVectors["beta"] = createVectorOfStringFromList(5, {bx_, by_, bz_, rho_, "t"});
				listOfRequiredVariablesForVectorsByID["beta"] = createVectorOfLongFromList(5, {bx_, by_, bz_, rho_, "t"});

			} else
			{
				//not sure whatelse to do.
				listOfRequiredVariablesForVectors[p_] = createVectorOfStringFromList(1, {rho_});
				listOfRequiredVariablesForVectorsByID[p_] = createVectorOfLongFromList(1, {rho_});

				listOfRequiredVariablesForVectors["beta"] = createVectorOfStringFromList(4, {bx_, by_, bz_, rho_});
				listOfRequiredVariablesForVectorsByID["beta"] = createVectorOfLongFromList(4, {bx_, by_, bz_, rho_});

			}
		}

		if ("mas" == modelName)
		{
			listOfRequiredVariablesForVectors["b"] = createVectorOfStringFromList(3, {br_, bphi_, btheta_});
			listOfRequiredVariablesForVectorsByID["b"] = createVectorOfLongFromList(3, {br_, bphi_, btheta_});

			listOfRequiredVariablesForVectors[br_] = createVectorOfStringFromList(3, {br_, bphi_, btheta_});
			listOfRequiredVariablesForVectorsByID[br_] = createVectorOfLongFromList(3, {br_, bphi_, btheta_});

			listOfRequiredVariablesForVectors[btheta_] = createVectorOfStringFromList(3, {br_, bphi_, btheta_});
			listOfRequiredVariablesForVectorsByID[btheta_] = createVectorOfLongFromList(3, {br_, bphi_, btheta_});

			listOfRequiredVariablesForVectors[bphi_] = createVectorOfStringFromList(3, {br_, bphi_, btheta_});
			listOfRequiredVariablesForVectorsByID[bphi_] = createVectorOfLongFromList(3, {br_, bphi_, btheta_});

			listOfRequiredVariablesForVectors["u"] = createVectorOfStringFromList(3, {ur_, uphi_, utheta_});
			listOfRequiredVariablesForVectorsByID["u"] = createVectorOfLongFromList(3, {ur_, uphi_, utheta_});

			listOfRequiredVariablesForVectors[ur_] = createVectorOfStringFromList(3, {ur_, uphi_, utheta_});
			listOfRequiredVariablesForVectorsByID[ur_] = createVectorOfLongFromList(3, {ur_, uphi_, utheta_});

			listOfRequiredVariablesForVectors[uphi_] = createVectorOfStringFromList(3, {ur_, uphi_, utheta_});
			listOfRequiredVariablesForVectorsByID[uphi_] = createVectorOfLongFromList(3, {ur_, uphi_, utheta_});

			listOfRequiredVariablesForVectors[utheta_] = createVectorOfStringFromList(3, {ur_, uphi_, utheta_});
			listOfRequiredVariablesForVectorsByID[utheta_] = createVectorOfLongFromList(3, {ur_, uphi_, utheta_});

			listOfRequiredVariablesForVectors["nv"] = createVectorOfStringFromList(4, {rho_, ur_, uphi_, utheta_});
			listOfRequiredVariablesForVectorsByID["nv"] = createVectorOfLongFromList(4, {rho_, ur_, uphi_, utheta_});

			listOfRequiredVariablesForVectors["nvr"] = createVectorOfStringFromList(4, {rho_, ur_, uphi_, utheta_});
			listOfRequiredVariablesForVectorsByID["nvr"] = createVectorOfLongFromList(4, {rho_, ur_, uphi_, utheta_});

			listOfRequiredVariablesForVectors["nvtheta"] = createVectorOfStringFromList(4, {rho_, ur_, uphi_, utheta_});
			listOfRequiredVariablesForVectorsByID["nvtheta"] = createVectorOfLongFromList(4, {rho_, ur_, uphi_, utheta_});

			listOfRequiredVariablesForVectors["nvphi"] = createVectorOfStringFromList(4, {rho_, ur_, uphi_, utheta_});
			listOfRequiredVariablesForVectorsByID["nvphi"] = createVectorOfLongFromList(4, {rho_, ur_, uphi_, utheta_});

			listOfRequiredVariablesForVectors["pram"] = createVectorOfStringFromList(4, {rho_, ur_, uphi_, utheta_});
			listOfRequiredVariablesForVectorsByID["pram"] = createVectorOfLongFromList(4, {rho_, ur_, uphi_, utheta_});

			listOfRequiredVariablesForVectors["exb"] = createVectorOfStringFromList(6, {br_, bphi_, btheta_, ur_, uphi_,
					utheta_});
			listOfRequiredVariablesForVectorsByID["exb"] = createVectorOfLongFromList(6, {br_, bphi_, btheta_, ur_, uphi_,
					utheta_});

			listOfRequiredVariablesForVectors["exbr"] = createVectorOfStringFromList(6, {br_, bphi_, btheta_, ur_, uphi_,
					utheta_});
			listOfRequiredVariablesForVectorsByID["exbr"] = createVectorOfLongFromList(6, {br_, bphi_, btheta_, ur_, uphi_,
					utheta_});

			listOfRequiredVariablesForVectors["exbtheta"] = createVectorOfStringFromList(6, {br_, bphi_, btheta_, ur_,
					uphi_, utheta_});
			listOfRequiredVariablesForVectorsByID["exbtheta"] = createVectorOfLongFromList(6, {br_, bphi_, btheta_, ur_,
					uphi_, utheta_});

			listOfRequiredVariablesForVectors["exbphi"] = createVectorOfStringFromList(6, {br_, bphi_, btheta_, ur_, uphi_,
					utheta_});
			listOfRequiredVariablesForVectorsByID["exbphi"] = createVectorOfLongFromList(6, {br_, bphi_, btheta_, ur_, uphi_,
					utheta_});

			listOfRequiredVariablesForVectors[e_] = createVectorOfStringFromList(6, {br_, bphi_, btheta_, ur_, uphi_,
					utheta_});
			listOfRequiredVariablesForVectorsByID[e_] = createVectorOfLongFromList(6, {br_, bphi_, btheta_, ur_, uphi_,
					utheta_});

			listOfRequiredVariablesForVectors["er"] = createVectorOfStringFromList(6, {br_, bphi_, btheta_, ur_, uphi_,
					utheta_});
			listOfRequiredVariablesForVectorsByID["er"] = createVectorOfLongFromList(6, {br_, bphi_, btheta_, ur_, uphi_,
					utheta_});

			listOfRequiredVariablesForVectors["ephi"] = createVectorOfStringFromList(6, {br_, bphi_, btheta_, ur_, uphi_,
					utheta_});
			listOfRequiredVariablesForVectorsByID["ephi"] = createVectorOfLongFromList(6, {br_, bphi_, btheta_, ur_, uphi_,
					utheta_});

			listOfRequiredVariablesForVectors["etheta"] = createVectorOfStringFromList(6, {br_, bphi_, btheta_, ur_, uphi_,
					utheta_});
			listOfRequiredVariablesForVectorsByID["etheta"] = createVectorOfLongFromList(6, {br_, bphi_, btheta_, ur_, uphi_,
					utheta_});

		}

		if ("enlil" == modelName)
		{

			if (model->doesVariableExist(ccmc::strings::variables::br_))
			{
				listOfRequiredVariablesForVectors["b"] = createVectorOfStringFromList(3, {br_, bphi_, btheta_});
				listOfRequiredVariablesForVectorsByID["b"] = createVectorOfLongFromList(3, {br_, bphi_, btheta_});

				listOfRequiredVariablesForVectors["b1"] = createVectorOfStringFromList(3, {b1r_, b1phi_, b1theta_});
				listOfRequiredVariablesForVectorsByID["b1"] = createVectorOfLongFromList(3, {b1r_, b1phi_, b1theta_});

				listOfRequiredVariablesForVectors[b1r_] = createVectorOfStringFromList(3, {b1r_, b1phi_, b1theta_});
				listOfRequiredVariablesForVectorsByID[b1r_] = createVectorOfLongFromList(3, {b1r_, b1phi_, b1theta_});

				listOfRequiredVariablesForVectors[b1phi_] = createVectorOfStringFromList(3, {b1r_, b1phi_, b1theta_});
				listOfRequiredVariablesForVectorsByID[b1phi_] = createVectorOfLongFromList(3, {b1r_, b1phi_, b1theta_});

				listOfRequiredVariablesForVectors[b1theta_] = createVectorOfStringFromList(3, {b1r_, b1phi_, b1theta_});
				listOfRequiredVariablesForVectorsByID[b1theta_] = createVectorOfLongFromList(3, {b1r_, b1phi_, b1theta_});


				listOfRequiredVariablesForVectors[br_] = createVectorOfStringFromList(3, {br_, bphi_, btheta_});
				listOfRequiredVariablesForVectorsByID[br_] = createVectorOfLongFromList(3, {br_, bphi_, btheta_});

				listOfRequiredVariablesForVectors[bphi_] = createVectorOfStringFromList(3, {br_, bphi_, btheta_});
				listOfRequiredVariablesForVectorsByID[bphi_] = createVectorOfLongFromList(3, {br_, bphi_, btheta_});

				listOfRequiredVariablesForVectors[btheta_] = createVectorOfStringFromList(3, {br_, bphi_, btheta_});
				listOfRequiredVariablesForVectorsByID[btheta_] = createVectorOfLongFromList(3, {br_, bphi_, btheta_});

				listOfRequiredVariablesForVectors[e_] = createVectorOfStringFromList(6, {br_, bphi_, btheta_, ur_, uphi_, utheta_});
				listOfRequiredVariablesForVectorsByID[e_] = createVectorOfLongFromList(6, {br_, bphi_, btheta_, ur_, uphi_, utheta_});

				listOfRequiredVariablesForVectors["er"] = createVectorOfStringFromList(6, {br_, bphi_, btheta_, ur_, uphi_, utheta_});
				listOfRequiredVariablesForVectorsByID["er"] = createVectorOfLongFromList(6, {br_, bphi_, btheta_, ur_, uphi_, utheta_});

				listOfRequiredVariablesForVectors["ephi"] = createVectorOfStringFromList(6, {br_, bphi_, btheta_, ur_, uphi_, utheta_});
				listOfRequiredVariablesForVectorsByID["ephi"] = createVectorOfLongFromList(6, {br_, bphi_, btheta_, ur_, uphi_, utheta_});

				listOfRequiredVariablesForVectors["etheta"] = createVectorOfStringFromList(6, {br_, bphi_, btheta_, ur_, uphi_, utheta_});
				listOfRequiredVariablesForVectorsByID["etheta"] = {br_, bphi_, btheta_, ur_, uphi_, utheta_};
			}
		}
		*/

	}

	void Kameleon::addRequirementsForComponents(std::string variable, int num, std::string * variables)
	{
		bool success = true;
		for (int i = 0; i < num; i++)
		{
			if (!model->doesVariableExist(variables[i]))
			{
				success = false;
			}
		}
		if (success)
		{
			listOfRequiredVariablesForComponents[variable] = createVectorOfStringFromList(num, variables);
			listOfRequiredVariablesForComponentsByID[variable] = createVectorOfLongFromList(num, variables);
		}
	}
}
