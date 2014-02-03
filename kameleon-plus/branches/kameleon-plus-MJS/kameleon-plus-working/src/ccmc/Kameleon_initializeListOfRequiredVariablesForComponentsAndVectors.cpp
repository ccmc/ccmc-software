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
		//TODO: Fix these

		std::string pram[] = {rho_, ux_, uy_, uz_};
		std::string pram_spherical[] = {rho_, ur_, uphi_, utheta_};
		std::string t[] = {p_, rho_};
		std::string ex[] = {by_,bz_,uy_,uz_};
		std::string ey[] = {bx_,bz_,ux_,uz_};
		std::string ez[] = {bx_,by_,ux_,uy_};
		std::string edotj[] = {jx_,jy_,jz_,bx_,by_,bz_,ux_,uy_,uz_};
		std::string edotj_spherical[] = {jr_,jphi_,jtheta_,br_,bphi_,btheta_,ur_,uphi_,utheta_};
		std::string jparx[] = {jx_,jy_,jz_,bx_,by_,bz_};
		std::string jpary[] = {jx_,jy_,jz_,bx_,by_,bz_};
		std::string jparz[] = {jx_,jy_,jz_,bx_,by_,bz_};
		std::string jparr[] = {jr_,jphi_,jtheta_,br_,bphi_,btheta_};
		std::string jparphi[] = {jr_,jphi_,jtheta_,br_,bphi_,btheta_};
		std::string jpartheta[] = {jr_,jphi_,jtheta_,br_,bphi_,btheta_};
		std::string jpar[] = {jx_,jy_,jz_,bx_,by_,bz_};
		std::string jpar_spherical[] = {jr_,jphi_,jtheta_,br_,bphi_,btheta_};
		std::string jxbx[] = {by_,bz_,jy_,jz_};
		std::string jxby[] = {bx_,bz_,jx_,jz_};
		std::string jxbz[] = {bx_,by_,jx_,jy_};
		std::string jxb[] = {bx_,by_,bz_,jx_,jy_,jz_};
		std::string jxb_spherical[] = {br_, bphi_, btheta_, jr_, jphi_, jtheta_};
		std::string exbx[] = {bx_,by_,bz_,ux_,uy_,uz_};
		std::string exby[] = {bx_,by_,bz_,ux_,uy_,uz_};
		std::string exbz[] = {bx_,by_,bz_,ux_,uy_,uz_};
		std::string exb[] = {bx_,by_,bz_,ux_,uy_,uz_};
		std::string exb_spherical[] = {br_, bphi_, btheta_, ur_, uphi_, utheta_};
		std::string j[] = {jx_,jy_,jz_};
		std::string j_spherical[] = {jr_, jphi_, jtheta_};
		std::string b[] = {bx_,by_,bz_};
		std::string b1[] = {b1x_,b1y_,b1z_};
		std::string b_1[] = {bx1_,by1_,bz1_,x_bx_, y_bx_, z_bx_,x_by_, y_by_, z_by_,x_bz_, y_bz_, z_bz_};
		std::string bx1[] = {bx1_,x_bx_, y_bx_, z_bx_};
		std::string by1[] = {by1_,x_by_, y_by_, z_by_};
		std::string bz1[] = {bz1_,x_bz_, y_bz_, z_bz_};
		std::string br[] = {br_};
		std::string bphi[] = {bphi_};
		std::string btheta[] = {btheta_};
		std::string u[] = {ux_,uy_,uz_};
		std::string u_spherical[] = {ur_,uphi_,utheta_};
		std::string v[] = {ux_,uy_,uz_};
		std::string v_r[] = {ux_};
		std::string v_lat[] = {uy_};
		std::string v_lon[] = {uz_};
		std::string s[] = {p_,rho_};
		std::string en[] = {e_};
		std::string e[] = {bx_,by_,bz_,ux_,uy_,uz_};
		std::string e_spherical[] = {br_, bphi_, btheta_, ur_, uphi_, utheta_};
		std::string nv[] = {rho_,ux_,uy_,uz_};
		std::string nvx[] = {rho_,ux_};
		std::string nvy[] = {rho_,uy_};
		std::string nvz[] = {rho_,uz_};
		std::string n[] = {rho_};
		std::string n_times_r_tothe_2_power[] = {rho_};
		std::string rho_times_r_tothe_2_power[] = {rho_};
		std::string polb[] = {bp_};
		std::string p_times_r_tothe_2_power[] = {p_};
		std::string eta_times_jx[] = {eta_, jx_};
		std::string eta_times_jy[] = {eta_, jy_};
		std::string eta_times_jz[] = {eta_, jz_};
		std::string eta_times_j[] = {eta_, jx_, jy_, jz_};
		std::string eta_times_j_spherical[] = {eta_, jr_, jphi_, jtheta_};
		std::string eta_times_j_tothe_2_power[] = {eta_, jx_, jy_, jz_};
		std::string grad_n[] = {rho_};
		std::string grad_b[] = {bx_,by_,bz_};
		std::string p[] = {p_};
		std::string p_without_p_T[] = {rho_, "T"};
		std::string p_without_p_t[] = {rho_, "t"};
		std::string beta[] = {bx_, by_, bz_, p_};
		std::string beta_without_p_T[] = {bx_, by_, bz_, rho_, "T"};
		std::string beta_without_p_t[] = {bx_, by_, bz_, rho_, "t"};
		std::string b1_spherical[] = {b1r_, bphi_, b1theta_, "bp"};

		std::string b_spherical[] = {br_, bphi_, btheta_};
		std::string mag_velocity_spherical[] = {br_,bphi_,btheta_,ur_,uphi_,utheta_};
		std::string er[] = {bphi_, btheta_, utheta_, uphi_};
		std::string ephi[] = {br_, bphi_, ur_, uphi_};
		std::string etheta[] ={br_, btheta_, ur_, utheta_};

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
		addRequirementsForComponents(eta_times_j_, 2, eta_times_j);
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
		addRequirementsForComponents(rho_times_r_tothe_2_power_, 1, rho_times_r_tothe_2_power);
		addRequirementsForComponents(bx1_, 4, bx1);
		addRequirementsForComponents(by1_, 4, by1);
		addRequirementsForComponents(bz1_, 4, bz1);
		addRequirementsForComponents(b_1_, 12, b_1);


		if (modelName == ccmc::strings::models::enlil_)
		{
			addRequirementsForComponents(b1_, 3, b1_spherical);
		}

		if (modelName == ccmc::strings::models::enlil_ ||
				modelName == ccmc::strings::models::mas_)
		{

			if (model->doesVariableExist(br_))
			{
				addRequirementsForComponents(b_, 3, b_spherical);
				addRequirementsForComponents(exb_, 6, exb_spherical);
				addRequirementsForComponents(exbr_, 6, exb_spherical);
				addRequirementsForComponents(exbphi_, 6, exb_spherical);
				addRequirementsForComponents(exbtheta_, 6, exb_spherical);
				addRequirementsForComponents(pram_, 4, pram_spherical);
				addRequirementsForComponents(edotj_, 9, edotj_spherical);
				addRequirementsForComponents(u_, 3, u_spherical);
				addRequirementsForComponents(j_, 3, j_spherical);
				addRequirementsForComponents(e_, 6, e_spherical);
				addRequirementsForComponents(er_, 4, er);
				addRequirementsForComponents(ephi_, 4, ephi);
				addRequirementsForComponents(etheta_, 4, etheta);
				addRequirementsForComponents(jpar_, 6, jpar_spherical);
				addRequirementsForComponents(jparr_, 6, jpar_spherical);
				addRequirementsForComponents(jparphi_, 6, jpar_spherical);
				addRequirementsForComponents(jpartheta_, 6, jpar_spherical);
				addRequirementsForComponents(jxbr_, 6, jxb_spherical);
				addRequirementsForComponents(jxbphi_, 6, jxb_spherical);
				addRequirementsForComponents(jxbtheta_, 6, jxb_spherical);

			}
		}

		//add variables for vectors.  same as above, except, for vectors, we want to add all components
		if (model->doesVariableExist(bx_))
		{
			addRequirementsForVectors(ex_, 6, e);
			addRequirementsForVectors(ey_, 6, e);
			addRequirementsForVectors(ez_, 6, e);
			addRequirementsForVectors(jxbx_, 6, jxb);
			addRequirementsForVectors(jxby_, 6, jxb);
			addRequirementsForVectors(jxbz_, 6, jxb);
			addRequirementsForVectors(jx_, 3, j);
			addRequirementsForVectors(jy_, 3, j);
			addRequirementsForVectors(jz_, 3, j);
			addRequirementsForVectors(bx_, 3, b);
			addRequirementsForVectors(by_, 3, b);
			addRequirementsForVectors(bz_, 3, b);
			addRequirementsForVectors(b1x_, 3, b1);
			addRequirementsForVectors(b1y_, 3, b1);
			addRequirementsForVectors(b1z_, 3, b1);
			addRequirementsForVectors(ux_, 3, u);
			addRequirementsForVectors(uy_, 3, u);
			addRequirementsForVectors(uz_, 3, u);
			addRequirementsForVectors(nvx_, 3, nv);
			addRequirementsForVectors(nvy_, 3, nv);
			addRequirementsForVectors(nvz_, 3, nv);
			addRequirementsForVectors(eta_times_jx_, 3, eta_times_j);
			addRequirementsForVectors(eta_times_jy_, 3, eta_times_j);
			addRequirementsForVectors(eta_times_jz_, 3, eta_times_j);
			addRequirementsForVectors(bx1_, 12, b_1);
			addRequirementsForVectors(by1_, 12, b_1);
			addRequirementsForVectors(bz1_, 12, b_1);
			addRequirementsForVectors(b_1_, 12, b_1);
		}
		else
		{
			addRequirementsForVectors(er_, 6, mag_velocity_spherical);
			addRequirementsForVectors(ephi_, 6, mag_velocity_spherical);
			addRequirementsForVectors(etheta_, 6, mag_velocity_spherical);
			addRequirementsForVectors(jr_, 3, j_spherical);
			addRequirementsForVectors(jphi_, 3, j_spherical);
			addRequirementsForVectors(jtheta_, 3, j_spherical);
			addRequirementsForVectors(br_, 3, b_spherical);
			addRequirementsForVectors(bphi_, 3, b_spherical);
			addRequirementsForVectors(btheta_, 3, b_spherical);
			addRequirementsForVectors(b1r_, 3, b1_spherical);
			addRequirementsForVectors(b1phi_, 3, b1_spherical);
			addRequirementsForVectors(b1theta_, 3, b1_spherical);
			addRequirementsForVectors(ur_, 3, u);
			addRequirementsForVectors(uphi_, 3, u);
			addRequirementsForVectors(utheta_, 3, u);
			addRequirementsForVectors(nvr_, 3, nv);
			addRequirementsForVectors(nvphi_, 3, nv);
			addRequirementsForVectors(nvtheta_, 3, nv);
			addRequirementsForVectors(eta_times_jr_, 3, eta_times_j_spherical);
			addRequirementsForVectors(eta_times_jphi_, 3, eta_times_j_spherical);
			addRequirementsForVectors(eta_times_jtheta_, 3, eta_times_j_spherical);
			addRequirementsForVectors(er_, 4, e_spherical);
			addRequirementsForVectors(ephi_, 4, e_spherical);
			addRequirementsForVectors(etheta_, 4, e_spherical);
		}

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

	void Kameleon::addRequirementsForVectors(std::string variable, int num, std::string * variables)
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
			listOfRequiredVariablesForVectors[variable] = createVectorOfStringFromList(num, variables);
			listOfRequiredVariablesForVectorsByID[variable] = createVectorOfLongFromList(num, variables);
		}
	}
}
