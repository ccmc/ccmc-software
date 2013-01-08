#include "Kameleon.h"

namespace ccmc
{
	/**
	 * Initializes the units for the derived variables.  Derived variables need to be converted
	 * to SI units to match these units.
	 */
	void Kameleon::initializeUnits()
	{

		initializeVisUnits();
		initializeSIUnits();

	}

	/**
	 *
	 */
	void Kameleon::initializeVisUnits()
	{
		//initialize visualization units
		variableVisUnits["t"] = "K";
		variableVisUnits["pram"] = "nPa";
		variableVisUnits["edotj"] = "nW/m^3";
		variableVisUnits["e"] = "mV/m";
		variableVisUnits["ex"] = "mV/m";
		variableVisUnits["ey"] = "mV/m";
		variableVisUnits["ez"] = "mV/m";
		variableVisUnits["er"] = "mV/m";
		variableVisUnits["etheta"] = "mV/m";
		variableVisUnits["ephi"] = "mV/m";
		variableVisUnits["b"] = "nT";
		variableVisUnits["br"] = "nT";
		variableVisUnits["b_r"] = "nT";
		variableVisUnits["b_lat"] = "nT";
		variableVisUnits["btheta"] = "nT";
		variableVisUnits["b_lon"] = "nT";
		variableVisUnits["bphi"] = "nT";
		variableVisUnits["j"] = "muA/m^2";
		variableVisUnits["jparx"] = "muA/m^2";
		variableVisUnits["jpary"] = "muA/m^2";
		variableVisUnits["jparz"] = "muA/m^2";
		variableVisUnits["jparr"] = "muA/m^2";
		variableVisUnits["jpartheta"] = "muA/m^2";
		variableVisUnits["jparphi"] = "muA/m^2";
		variableVisUnits["jpar"] = "muA/m^2";
		variableVisUnits["jxbx"] = "fN/m^3";
		variableVisUnits["jxby"] = "fN/m^3";
		variableVisUnits["jxbz"] = "fN/m^3";
		variableVisUnits["jxb"] = "fN/m^3";
		variableVisUnits["exbr"] = "muW/m^2";
		variableVisUnits["exbphi"] = "muW/m^2";
		variableVisUnits["exbtheta"] = "muW/m^2";
		variableVisUnits["exbx"] = "muW/m^2";
		variableVisUnits["exby"] = "muW/m^2";
		variableVisUnits["exbz"] = "muW/m^2";
		variableVisUnits["exb"] = "muW/m^2";
		variableVisUnits["s"] = "Km^2";
		variableVisUnits["en"] = "J/m^3";
		variableVisUnits["nv"] = "cm^-2s^-1";
		variableVisUnits["nvx"] = "cm^-2s^-1";
		variableVisUnits["nvy"] = "cm^-2s^-1";
		variableVisUnits["nvz"] = "cm^-2s^-1";
		variableVisUnits["u"] = "km/s";
		variableVisUnits["v"] = "km/s";
		variableVisUnits["v_r"] = "km/s";
		variableVisUnits["v_lat"] = "km/s";
		variableVisUnits["v_lon"] = "km/s";
		variableVisUnits["n"] = "amu/cm^3";
		variableVisUnits["n*r^2"] = "AU^2 cm^-3";
		variableVisUnits["p*r^2"] = "nPa AU^2";
		variableVisUnits["p"] = "nPa";
		variableVisUnits["polb"] = "";
		variableVisUnits["b1"] = "nT";
		variableVisUnits["eta*j"] = "mV/m";
		variableVisUnits["eta*jx"] = "mV/m";
		variableVisUnits["eta*jy"] = "mV/m";
		variableVisUnits["eta*jz"] = "mV/m";
		variableVisUnits["eta*j^2"] = "pW/m^3";

		variableVisUnits["beta"] = "";
		if (modelName == "enlil")
		{
			variableVisUnits["ur"] = "km/s";
			variableVisUnits["uphi"] = "km/s";
			variableVisUnits["utheta"] = "km/s";
			variableVisUnits["br"] = "nT";
			variableVisUnits["bphi"] = "nT";
			variableVisUnits["btheta"] = "nT";
			variableVisUnits["b1r"] = "nT";
			variableVisUnits["b1phi"] = "nT";
			variableVisUnits["b1theta"] = "nT";
		}

	}

	/**
	 *
	 */
	void Kameleon::initializeSIUnits()
	{
		//initialize SI units
		variableSIUnits["t"] = "K";
		variableSIUnits["pram"] = "nPa";
		variableSIUnits["edotj"] = "nW/m^3";
		variableSIUnits["e"] = "mV/m";
		variableSIUnits["ex"] = "mV/m";
		variableSIUnits["ey"] = "mV/m";
		variableSIUnits["ez"] = "mV/m";
		variableSIUnits["er"] = "mV/m";
		variableSIUnits["etheta"] = "mV/m";
		variableSIUnits["ephi"] = "mV/m";
		variableSIUnits["b"] = "nT";
		variableSIUnits["br"] = "nT";
		variableSIUnits["b_r"] = "nT";
		variableSIUnits["b_lat"] = "nT";
		variableSIUnits["btheta"] = "nT";
		variableSIUnits["b_lon"] = "nT";
		variableSIUnits["bphi"] = "nT";
		variableSIUnits["j"] = "muA/m^2";
		variableSIUnits["jparx"] = "muA/m^2";
		variableSIUnits["jpary"] = "muA/m^2";
		variableSIUnits["jparz"] = "muA/m^2";
		variableSIUnits["jparr"] = "muA/m^2";
		variableSIUnits["jpartheta"] = "muA/m^2";
		variableSIUnits["jparphi"] = "muA/m^2";
		variableSIUnits["jpar"] = "muA/m^2";
		variableSIUnits["jxbx"] = "fN/m^3";
		variableSIUnits["jxby"] = "fN/m^3";
		variableSIUnits["jxbz"] = "fN/m^3";
		variableSIUnits["jxb"] = "fN/m^3";
		variableSIUnits["exbr"] = "muW/m^2";
		variableSIUnits["exbphi"] = "muW/m^2";
		variableSIUnits["exbtheta"] = "muW/m^2";
		variableSIUnits["exbx"] = "muW/m^2";
		variableSIUnits["exby"] = "muW/m^2";
		variableSIUnits["exbz"] = "muW/m^2";
		variableSIUnits["exb"] = "muW/m^2";
		variableSIUnits["s"] = "Km^2";
		variableSIUnits["en"] = "J/m^3";
		variableSIUnits["nv"] = "cm^-2s^-1";
		variableSIUnits["nvx"] = "cm^-2s^-1";
		variableSIUnits["nvy"] = "cm^-2s^-1";
		variableSIUnits["nvz"] = "cm^-2s^-1";
		variableSIUnits["u"] = "km/s";
		variableSIUnits["v"] = "km/s";
		variableSIUnits["v_r"] = "km/s";
		variableSIUnits["v_lat"] = "km/s";
		variableSIUnits["v_lon"] = "km/s";
		variableSIUnits["n"] = "amu/cm^3";
		variableSIUnits["n*r^2"] = "AU^2 cm^-3";
		variableSIUnits["p*r^2"] = "nPa AU^2";
		variableSIUnits["p"] = "nPa";
		variableSIUnits["polb"] = "";
		variableSIUnits["b1"] = "nT";
		variableSIUnits["eta*j"] = "mV/m";
		variableSIUnits["eta*jx"] = "mV/m";
		variableSIUnits["eta*jy"] = "mV/m";
		variableSIUnits["eta*jz"] = "mV/m";
		variableSIUnits["eta*j^2"] = "pW/m^3";

		variableSIUnits["beta"] = "";
		if (modelName == "enlil")
		{
			variableSIUnits["ux"] = "km/s";
			variableSIUnits["uy"] = "km/s";
			variableSIUnits["uz"] = "km/s";
			variableSIUnits["bx"] = "nT";
			variableSIUnits["by"] = "nT";
			variableSIUnits["bz"] = "nT";
			variableSIUnits["b1x"] = "nT";
			variableSIUnits["b1y"] = "nT";
			variableSIUnits["b1z"] = "nT";
		}
	}

}

