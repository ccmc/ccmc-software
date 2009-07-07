#include "Kameleon.h"
#include <math.h>
#include "Constants.h"

/**
 * Initializes the conversion factors required to change the units of the base
 * variables into those required for the visualization plots.  The conversionFactors
 * only need to be specified by those base variables requiring a conversion factor.
 * All variables that are not specified in the conversionFactorsToVis map are multiplied
 * by 1.0
 *
 * For some vector variables, the conversion factors need to be specified for the magnitude
 * calculations as well, since some of the magnitude calculations are independent of the vector
 * component calculations (to increase performance).
 */
void Kameleon::initializeConversionFactorsToVis()
{
	float avogadrosNumber = 6.0221415e23;
    float boltzmannConstant = 1.3806503e-23;
    float gauss2Tesla = 1.e-4;
    float mu_0 = 8.854e-12;
    float l_0 = 6.96e8;
    std::string bx_,by_,bz_,ux_,uy_,uz_,n_,rho_,ex_,ey_,ez_;
    bx_ = "bx";
    by_ = "by";
    bz_ = "bz";
    ux_ = "ux";
    uy_ = "uy";
    uz_ = "uz";
    n_ = "n";
    rho_ = "rho";
    ex_ = "ex";
    ey_ = "ey";
    ez_ = "ez";

/** need to make this work from SI units.  Simplify the maintenance of the conversions **/

	if (modelName == "enlil")
	{
		//conversionFactorsToVis["bx"] = 1.e9;
		//conversionFactorsToVis["by"] = 1.e9;
		//conversionFactorsToVis["bz"] = 1.e9;
		conversionFactorsToVis["b1x"] = 1.e9;
		conversionFactorsToVis["b1y"] = 1.e9;
		conversionFactorsToVis["b1z"] = 1.e9;
		conversionFactorsToVis["ux"] = 1.e-3;
		conversionFactorsToVis["uy"] = 1.e-3;
		conversionFactorsToVis["uz"] = 1.e-3;
		conversionFactorsToVis["n"] = 1.e-6 * Avogadro;
		conversionFactorsToVis["rho"] = 1.e3;
		conversionFactorsToVis["ex"] = 1.e-3;
		conversionFactorsToVis["ey"] = 1.e-3;
		conversionFactorsToVis["ez"] = 1.e-3;


		//blah
	}
	else if (modelName == "mas")
	{
		conversionFactorsToVis["n"] = 1.e8;
		conversionFactorsToVis["t"] = 2.84e7;
		conversionFactorsToVis["p"] = 0.387 * 1.e9 * .1; //Dyn/cm^2 * to_nano * .1
		conversionFactorsToVis["ur"] = 481.37;
		conversionFactorsToVis["uphi"] = 481.37;
		conversionFactorsToVis["utheta"] = 481.37;
		conversionFactorsToVis["jr"] = 1.e2*2.205/(mu_0*l_0);
		conversionFactorsToVis["jphi"] = 1.e2*2.205/(mu_0*l_0);
		conversionFactorsToVis["jtheta"] = 1.e2*2.205/(mu_0*l_0);
		conversionFactorsToVis["er"] = 1.e-3;
		conversionFactorsToVis["ephi"] = 1.e-3;
		conversionFactorsToVis["etheta"] = 1.e-3;
		conversionFactorsToVis["br"] = 2.205 * gauss2Tesla * 1.e9;
		conversionFactorsToVis["bphi"] = 2.205 * gauss2Tesla * 1.e9;
		conversionFactorsToVis["btheta"] = 2.205 * gauss2Tesla * 1.e9;



	}
	else if (modelName == "ucla_ggcm" || modelName == "open_ggcm")
	{
		conversionFactorsToVis["p"] = 1.e-3;
		conversionFactorsToVis["t"] = 1.e-15/Boltzmann;
		conversionFactorsToVis["s"] = 1.e-4;
		conversionFactorsToVis["pram"] = 1.627e-6;
		conversionFactorsToVis["nv"] = 1.e6;
		conversionFactorsToVis["nvx"] = 1.e6;
		conversionFactorsToVis["nvy"] = 1.e6;
		conversionFactorsToVis["nvz"] = 1.e6;
		conversionFactorsToVis["beta"] = 1.e9;





	}
	else //just BATSRUS for now
	{
		conversionFactorsToVis["p"] = 1.e-3;
		conversionFactorsToVis["t"] = 1.e-12/Boltzmann;
		conversionFactorsToVis["s"] = 1.e-4;
		conversionFactorsToVis["pram"] = 1.627e-6;
		conversionFactorsToVis["nv"] = 1.e6;
		conversionFactorsToVis["nvx"] = 1.e6;
		conversionFactorsToVis["nvy"] = 1.e6;
		conversionFactorsToVis["nvz"] = 1.e6;
		conversionFactorsToVis["beta"] = 1.e21;

	}


}

