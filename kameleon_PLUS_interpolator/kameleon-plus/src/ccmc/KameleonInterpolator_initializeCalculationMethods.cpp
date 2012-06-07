#include "KameleonInterpolator.h"

namespace ccmc
{
	/**
	 * Initializes the calculationMethod map.  calculationMethod is a map of function pointers.
	 */
	void KameleonInterpolator::initializeCalculationMethods()
	{
		calculationMethod["beta"] = &KameleonInterpolator::compute_beta;
		calculationMethod["t"] = &KameleonInterpolator::compute_temp;
		calculationMethod["pram"] = &KameleonInterpolator::compute_pram;
		calculationMethod["pram*r^2"] = &KameleonInterpolator::compute_scaleByRadius;
		calculationMethod["edotj"] = &KameleonInterpolator::compute_edotj;
		calculationMethod["ex"] = &KameleonInterpolator::compute_eComponent1;
		calculationMethod["e_r"] = &KameleonInterpolator::compute_eComponent1;
		calculationMethod["er"] = &KameleonInterpolator::compute_eComponent1;
		calculationMethod["ey"] = &KameleonInterpolator::compute_ey;
		calculationMethod["e_lat"] = &KameleonInterpolator::compute_ey;
		calculationMethod["etheta"] = &KameleonInterpolator::compute_ey;
		calculationMethod["ez"] = &KameleonInterpolator::compute_ez;
		calculationMethod["e_lon"] = &KameleonInterpolator::compute_ez;
		calculationMethod["ephi"] = &KameleonInterpolator::compute_ez;
		calculationMethod["e"] = &KameleonInterpolator::compute_magnitude;
		calculationMethod["en"] = &KameleonInterpolator::compute_en;
		calculationMethod["j"] = &KameleonInterpolator::compute_magnitude;
		calculationMethod["b"] = &KameleonInterpolator::compute_magnitude;
		calculationMethod["u"] = &KameleonInterpolator::compute_magnitude;
		calculationMethod["v"] = &KameleonInterpolator::compute_magnitude;
		calculationMethod["jparx"] = &KameleonInterpolator::compute_jparComponent1;
		calculationMethod["jparr"] = &KameleonInterpolator::compute_jparComponent1;
		calculationMethod["jpary"] = &KameleonInterpolator::compute_jpary;
		calculationMethod["jpartheta"] = &KameleonInterpolator::compute_jpary;
		calculationMethod["jparz"] = &KameleonInterpolator::compute_jparz;
		calculationMethod["jparphi"] = &KameleonInterpolator::compute_jparz;
		calculationMethod["jpar"] = &KameleonInterpolator::compute_jpar;
		calculationMethod["jxbx"] = &KameleonInterpolator::compute_jxbComponent1;
		calculationMethod["jxby"] = &KameleonInterpolator::compute_jxby;
		calculationMethod["jxbz"] = &KameleonInterpolator::compute_jxbz;
		calculationMethod["jxb"] = &KameleonInterpolator::compute_jxb;
		calculationMethod["exbx"] = &KameleonInterpolator::compute_exbComponent1;
		calculationMethod["exb_r"] = &KameleonInterpolator::compute_exbComponent1;
		calculationMethod["exbr"] = &KameleonInterpolator::compute_exbComponent1;
		calculationMethod["exb_r*r^2"] = &KameleonInterpolator::compute_scaleByRadius;
		calculationMethod["exby"] = &KameleonInterpolator::compute_exby;
		calculationMethod["exb_lat"] = &KameleonInterpolator::compute_exby;
		calculationMethod["exbtheta"] = &KameleonInterpolator::compute_exby;
		calculationMethod["exb_lat*r^2"] = &KameleonInterpolator::compute_scaleByRadius;
		calculationMethod["exbz"] = &KameleonInterpolator::compute_exbz;
		calculationMethod["exb_lon"] = &KameleonInterpolator::compute_exbz;
		calculationMethod["exbphi"] = &KameleonInterpolator::compute_exbz;
		calculationMethod["exb_lon*r^2"] = &KameleonInterpolator::compute_scaleByRadius;
		calculationMethod["exb"] = &KameleonInterpolator::compute_exb;
		calculationMethod["exb*r^2"] = &KameleonInterpolator::compute_scaleByRadius;
		calculationMethod["eta*j"] = &KameleonInterpolator::compute_etaj;
		calculationMethod["eta*jx"] = &KameleonInterpolator::compute_etajComponent1;
		calculationMethod["eta*jy"] = &KameleonInterpolator::compute_etajComponent2;
		calculationMethod["eta*jz"] = &KameleonInterpolator::compute_etajComponent3;
		calculationMethod["eta*j^2"] = &KameleonInterpolator::compute_scaleByRadius;
		calculationMethod["s"] = &KameleonInterpolator::compute_s;
		calculationMethod["n"] = &KameleonInterpolator::compute_n;
		calculationMethod["n*r^2"] = &KameleonInterpolator::compute_scaleByRadius;
		calculationMethod["rho*r^2"] = &KameleonInterpolator::compute_scaleByRadius;
		calculationMethod["nv"] = &KameleonInterpolator::compute_nv;
		calculationMethod["nvx"] = &KameleonInterpolator::compute_nvComponent1;
		calculationMethod["nv_r"] = &KameleonInterpolator::compute_nvComponent1;
		calculationMethod["nvr"] = &KameleonInterpolator::compute_nvComponent1;
		calculationMethod["nv_r*r^2"] = &KameleonInterpolator::compute_scaleByRadius;
		calculationMethod["nvy"] = &KameleonInterpolator::compute_nvy;
		calculationMethod["nv_lat"] = &KameleonInterpolator::compute_nvy;
		calculationMethod["nvtheta"] = &KameleonInterpolator::compute_nvy;
		calculationMethod["nv_lat*r^2"] = &KameleonInterpolator::compute_scaleByRadius;
		calculationMethod["nvz"] = &KameleonInterpolator::compute_nvz;
		calculationMethod["nv_lon"] = &KameleonInterpolator::compute_nvz;
		calculationMethod["nvphi"] = &KameleonInterpolator::compute_nvz;
		calculationMethod["nv_lon*r^2"] = &KameleonInterpolator::compute_scaleByRadius;
		calculationMethod["p*r^2"] = &KameleonInterpolator::compute_scaleByRadius;
		calculationMethod["p"] = &KameleonInterpolator::compute_p;
		calculationMethod["polb"] = &KameleonInterpolator::compute_polb;
		calculationMethod["b_r*r^2"] = &KameleonInterpolator::compute_polb;
		calculationMethod["b1"] = &KameleonInterpolator::compute_magnitude;
		calculationMethod["grad.n"] = &KameleonInterpolator::compute_gradient;
		calculationMethod["grad.b"] = &KameleonInterpolator::compute_gradient;

		/*else if (variable_string == "b_r*r^2")
		 {
		 interp_value = interpolate("bx",positionComponent1,positionComponent2,positionComponent3,
		 missing, dComponent1, dComponent2, dComponent3);
		 interp_value *= positionComponent1 * positionComponent1;
		 }
		 else if (variable_string == "b_lat")
		 {
		 interp_value = interpolate("by",positionComponent1,positionComponent2,positionComponent3,
		 missing, dComponent1, dComponent2, dComponent3);
		 //interp_value = interp_value * 1.0e9;
		 }
		 else if (variable_string == "b_lat*r^2")
		 {
		 interp_value = interpolate("by",positionComponent1,positionComponent2,positionComponent3,
		 missing, dComponent1, dComponent2, dComponent3);
		 interp_value *= positionComponent1 * positionComponent1;
		 }
		 else if (variable_string == "b_lon")
		 {
		 interp_value = interpolate("bz",positionComponent1,positionComponent2,positionComponent3,
		 missing, dComponent1, dComponent2, dComponent3);
		 //interp_value = interp_value * 1.0e9;
		 }
		 else if (variable_string == "b_lon*r^2")
		 {
		 interp_value = interpolate("bz",positionComponent1,positionComponent2,positionComponent3,
		 missing, dComponent1, dComponent2, dComponent3);
		 interp_value *= positionComponent1 * positionComponent1;
		 }
		 else
		 {
		 interp_value = interpolateSimple(variable, positionComponent1, positionComponent2, positionComponent3,
		 missing, dComponent1, dComponent2, dComponent3);
		 }

		 else if (variable_string == "j")
		 {
		 interp_value = compute_magnitude("j",positionComponent1,positionComponent2,positionComponent3,
		 missing, dComponent1, dComponent2, dComponent3);
		 }
		 else if (variable_string == "b")
		 {
		 interp_value = compute_magnitude("b",positionComponent1,positionComponent2,positionComponent3,
		 missing, dComponent1, dComponent2, dComponent3);
		 }
		 else if (variable_string == "b*r")
		 {
		 interp_value = compute_magnitude("b",positionComponent1, positionComponent2, positionComponent3,
		 missing, dComponent1, dComponent2, dComponent3);
		 interp_value *= positionComponent1;
		 }
		 else if (variable_string == "u" || variable_string == "v")
		 {
		 interp_value = compute_magnitude("u",positionComponent1,positionComponent2,positionComponent3,
		 missing, dComponent1, dComponent2, dComponent3);
		 }
		 else if (variable_string == "v_r")
		 {
		 interp_value = interpolate("ux",positionComponent1,positionComponent2,positionComponent3,
		 missing, dComponent1, dComponent2, dComponent3);
		 }
		 else if (variable_string == "v_lon")
		 {
		 interp_value = interpolate("uz",positionComponent1,positionComponent2,positionComponent3,
		 missing, dComponent1, dComponent2, dComponent3);
		 }
		 else if (variable_string == "v_lat")
		 {
		 interp_value = interpolate("uy",positionComponent1,positionComponent2,positionComponent3,
		 missing, dComponent1, dComponent2, dComponent3);
		 }
		 else if (variable_string == "jparx" || variable_string == "jparr")
		 {
		 interp_value = compute_jparx(positionComponent1,positionComponent2,positionComponent3,
		 missing, dComponent1, dComponent2, dComponent3);
		 }
		 else if (variable_string == "jpary" || variable_string == "jpartheta")
		 {
		 interp_value = compute_jpary(positionComponent1,positionComponent2,positionComponent3,
		 missing, dComponent1, dComponent2, dComponent3);
		 }
		 else if (variable_string == "jparz" || variable_string == "jparphi")
		 {
		 interp_value = compute_jparz(positionComponent1,positionComponent2,positionComponent3,
		 missing, dComponent1, dComponent2, dComponent3);
		 }
		 else if (variable_string == "jpar")
		 {
		 interp_value = compute_jpar(positionComponent1,positionComponent2,positionComponent3,
		 missing, dComponent1, dComponent2, dComponent3);
		 }
		 else if (variable_string == "jxbx")
		 {
		 interp_value = compute_jxbx(positionComponent1,positionComponent2,positionComponent3,
		 missing, dComponent1, dComponent2, dComponent3);
		 }
		 else if (variable_string == "jxby")
		 {
		 interp_value = compute_jxby(positionComponent1,positionComponent2,positionComponent3,
		 missing, dComponent1, dComponent2, dComponent3);
		 }
		 else if (variable_string == "jxbz")
		 {
		 interp_value = compute_jxbz(positionComponent1,positionComponent2,positionComponent3,
		 missing, dComponent1, dComponent2, dComponent3);
		 }
		 else if (variable_string == "jxb")
		 {
		 interp_value = compute_jxb(positionComponent1,positionComponent2,positionComponent3,
		 missing, dComponent1, dComponent2, dComponent3);
		 }
		 else if (variable_string == "exbx" || variable_string == "exb_r" | variable_string == "exbr")
		 {
		 interp_value = compute_exbx(positionComponent1,positionComponent2,positionComponent3,
		 missing, dComponent1, dComponent2, dComponent3);
		 }
		 else if (variable_string == "exb_r*r^2")
		 {
		 interp_value = compute_exbx(positionComponent1,positionComponent2,positionComponent3,
		 missing, dComponent1, dComponent2, dComponent3);
		 interp_value *= positionComponent1 * positionComponent1;
		 }
		 else if (variable_string == "exby" || variable_string == "exb_lat" || variable_string == "exbtheta")
		 {
		 interp_value = compute_exby(positionComponent1,positionComponent2,positionComponent3,
		 missing, dComponent1, dComponent2, dComponent3);
		 }
		 else if (variable_string == "exb_lat*r^2")
		 {
		 interp_value = compute_exby(positionComponent1,positionComponent2,positionComponent3,
		 missing, dComponent1, dComponent2, dComponent3);
		 interp_value *= positionComponent1 * positionComponent1;
		 }
		 else if (variable_string == "exbz" || variable_string == "exb_lon" || variable_string == "exbphi")
		 {
		 interp_value = compute_exbz(positionComponent1,positionComponent2,positionComponent3,
		 missing, dComponent1, dComponent2, dComponent3);
		 }
		 else if (variable_string == "exb_lon*r^2")
		 {
		 interp_value = compute_exbz(positionComponent1,positionComponent2,positionComponent3,
		 missing, dComponent1, dComponent2, dComponent3);
		 interp_value *= positionComponent1 * positionComponent1;
		 }
		 else if (variable_string == "exb")
		 {
		 interp_value = compute_exb(positionComponent1,positionComponent2,positionComponent3,
		 missing, dComponent1, dComponent2, dComponent3);
		 }
		 else if (variable_string == "exb*r^2")
		 {
		 interp_value = compute_exb(positionComponent1,positionComponent2,positionComponent3,
		 missing, dComponent1, dComponent2, dComponent3);
		 interp_value *= positionComponent1;
		 }
		 else if (variable_string == "s")
		 {
		 interp_value = compute_s(positionComponent1,positionComponent2,positionComponent3,
		 missing, dComponent1, dComponent2, dComponent3);
		 }
		 else if (variable_string == "n")
		 {
		 interp_value = compute_n(positionComponent1,positionComponent2,positionComponent3,
		 missing, dComponent1, dComponent2, dComponent3);
		 }
		 else if (variable_string == "nv")
		 {
		 interp_value = compute_nv(positionComponent1,positionComponent2,positionComponent3,
		 missing, dComponent1, dComponent2, dComponent3);
		 }
		 else if (variable_string == "nvx" || variable_string == "nv_r" || variable_string == "nvr")
		 {
		 interp_value = compute_nvx(positionComponent1,positionComponent2,positionComponent3,
		 missing, dComponent1, dComponent2, dComponent3);
		 }
		 else if (variable_string == "nv_r*r^2")
		 {
		 interp_value = compute_nvx(positionComponent1,positionComponent2,positionComponent3,
		 missing, dComponent1, dComponent2, dComponent3);
		 interp_value *= positionComponent1 * positionComponent1;
		 }
		 else if (variable_string == "nvy" || variable_string == "nv_lat" || variable_string == "nvtheta")
		 {
		 interp_value = compute_nvy(positionComponent1,positionComponent2,positionComponent3,
		 missing, dComponent1, dComponent2, dComponent3);
		 }
		 else if (variable_string == "nv_lat*r^2")
		 {
		 interp_value = compute_nvy(positionComponent1,positionComponent2,positionComponent3,
		 missing, dComponent1, dComponent2, dComponent3);
		 interp_value *= positionComponent1 * positionComponent1;
		 }
		 else if (variable_string == "nvz" || variable_string == "nv_lon" || variable_string == "nvphi")
		 {
		 interp_value = compute_nvz(positionComponent1,positionComponent2,positionComponent3,
		 missing, dComponent1, dComponent2, dComponent3);
		 }
		 else if (variable_string == "nv_lon*r^2")
		 {
		 interp_value = compute_nvz(positionComponent1,positionComponent2,positionComponent3,
		 missing, dComponent1, dComponent2, dComponent3);
		 interp_value *= positionComponent1 * positionComponent1;
		 }
		 else if (variable_string == "n*r^2")
		 {
		 interp_value = compute_n(positionComponent1,positionComponent2,positionComponent3,
		 missing,dComponent1, dComponent2, dComponent3);
		 interp_value = interp_value * pow(positionComponent1,2.0);
		 }
		 else if (variable_string == "p*r^2")
		 {
		 interp_value = interpolate("p",positionComponent1,positionComponent2,positionComponent3,
		 missing, dComponent1, dComponent2, dComponent3);
		 interp_value = interp_value * pow(positionComponent1,2.0);
		 }
		 else if (variable_string == "p")
		 {
		 interp_value = compute_p(positionComponent1,positionComponent2,positionComponent3,
		 missing, dComponent1, dComponent2, dComponent3);
		 }
		 else if (variable_string == "polb")
		 {
		 interp_value = interpolate("bp",positionComponent1,positionComponent2,positionComponent3,
		 missing, dComponent1, dComponent2, dComponent3);
		 if (interp_value < 0.0)
		 {
		 interp_value = 1.0;
		 }
		 else if (interp_value > 0.0)
		 {
		 interp_value = -1.0;
		 }
		 else
		 interp_value = 0.0;
		 }
		 else if (variable_string == "b_r")
		 {
		 interp_value = interpolate("bx",positionComponent1,positionComponent2,positionComponent3,
		 missing, dComponent1, dComponent2, dComponent3);
		 //interp_value = interp_value * 1.0e9;
		 }
		 else if (variable_string == "b_r*r^2")
		 {
		 interp_value = interpolate("bx",positionComponent1,positionComponent2,positionComponent3,
		 missing, dComponent1, dComponent2, dComponent3);
		 interp_value *= positionComponent1 * positionComponent1;
		 }
		 else if (variable_string == "b_lat")
		 {
		 interp_value = interpolate("by",positionComponent1,positionComponent2,positionComponent3,
		 missing, dComponent1, dComponent2, dComponent3);
		 //interp_value = interp_value * 1.0e9;
		 }
		 else if (variable_string == "b_lat*r^2")
		 {
		 interp_value = interpolate("by",positionComponent1,positionComponent2,positionComponent3,
		 missing, dComponent1, dComponent2, dComponent3);
		 interp_value *= positionComponent1 * positionComponent1;
		 }
		 else if (variable_string == "b_lon")
		 {
		 interp_value = interpolate("bz",positionComponent1,positionComponent2,positionComponent3,
		 missing, dComponent1, dComponent2, dComponent3);
		 //interp_value = interp_value * 1.0e9;
		 }
		 else if (variable_string == "b_lon*r^2")
		 {
		 interp_value = interpolate("bz",positionComponent1,positionComponent2,positionComponent3,
		 missing, dComponent1, dComponent2, dComponent3);
		 interp_value *= positionComponent1 * positionComponent1;
		 }
		 else
		 {
		 interp_value = interpolateSimple(variable, positionComponent1, positionComponent2, positionComponent3,
		 missing, dComponent1, dComponent2, dComponent3);
		 }*/

	}

}
