/*
 * KameleonInterpolator.cpp
 *
 *  Created on: Jul 1, 2009
 *      Author: David Berrios
 */

#include "KameleonInterpolator.h"
#include "Constants.h"
#include <set>

namespace ccmc
{
	/**
	 * @param model
	 */
	KameleonInterpolator::KameleonInterpolator(Model * model)
	{

		this->modelReader = model;
		this->modelName = modelReader->getModelName();
		initializeCalculationMethods();
		initializeConversionFactorsToVis();
		initializeCoordinates();
		interpolator = modelReader->createNewInterpolator();
		std::string preferred_coordinates = "NATIVE";
		this->setPreferredCoordinates(preferred_coordinates);
		this->setModelCoordinates();
		setEphemTime();

	}

	KameleonInterpolator::KameleonInterpolator(Model * model, const std::string& preferred_coordinates)
	{

		this->modelReader = model;
		this->modelName = modelReader->getModelName();
		initializeCalculationMethods();
		initializeConversionFactorsToVis();
		initializeCoordinates();
		interpolator = modelReader->createNewInterpolator();
		this->setPreferredCoordinates(preferred_coordinates);
		this->setModelCoordinates();
		setEphemTime();
	}

	void KameleonInterpolator::initializeCoordinates(){
		std::string tmp[] = {	"NATIVE",
								"J2000",
								"GEI",
								"GEO",
								"MAG",
								"GSE",
								"GSM",
								"SM",
								"RTN",
								"GSEQ",
								"HEE",
								"HAE",
								"HEEQ"};
		std::set<std::string> coords(tmp, tmp + sizeof(tmp) / sizeof(tmp[0]));
		this->from_coords = coords;

		std::string tmp2[] = {	"J2000",
								"GEI",
								"GEO",
								"MAG",
								"GSE",
								"GSM",
								"SM",
								"RTN",
								"GSEQ",
								"HEE",
								"HAE",
								"HEEQ",
								"HNM" //ENLIL
							};
		std::set<std::string> coords2(tmp2, tmp2 + sizeof(tmp2) / sizeof(tmp2[0]));
		this->to_coords = coords2;

	}
	std::vector<std::string> KameleonInterpolator::get_preferred_coords_list(){
		std::vector<std::string> coords_list(this->from_coords.begin(), this->from_coords.end());
		return coords_list;
	}

	std::string KameleonInterpolator::get_model_coords(){
		return this->model_coordinates;
	}

	void KameleonInterpolator::setPreferredCoordinates(const std::string& preferred_coordinates){

		if (this->from_coords.count(preferred_coordinates) != 0){
			this->preferred_coordinates = preferred_coordinates;
		} else {
			std::cout << "Conversion from " << preferred_coordinates 
				<< " not implemented! Defaulting to Native" <<std::endl;
			this->preferred_coordinates = "NATIVE"; //assume native 
		}

	}

	void KameleonInterpolator::setModelCoordinates(){
		
		if (modelReader->doesAttributeExist("standard_grid_target")){
			ccmc::Attribute standard_gt_attr = modelReader->getGlobalAttribute("standard_grid_target");
			std::string standard_gt = standard_gt_attr.getAttributeString();
			if (this->to_coords.count(standard_gt) !=0){
				this->model_coordinates = standard_gt;
			} else{
				std::cout << "Warning: Conversion to " << standard_gt << " not implemented" << std::endl;
				std::cout << "available coordinates:\n";
				std::set<std::string>::iterator it;
				for (it = this->to_coords.begin(); it != this->to_coords.end(); it++){
					std::cout<< *it << std::endl;
				}
				this->model_coordinates = "UNKNOWN";
			}
		} else{
			std::cout << "Warning: No standard position information available. " 
						<< "Set standard_grid_target attribute" << std::endl;
			this->model_coordinates = "UNKNOWN";
		}
		
	}

	std::string KameleonInterpolator::getPreferredCoordinates(){
		return this->preferred_coordinates;
	}

	long KameleonInterpolator::getEphemTime()
	{
		return this->time_et;
	}

	void KameleonInterpolator::setEphemTime()
	{
		if (modelReader->doesAttributeExist("start_time")){
			ccmc::Attribute tattr = modelReader->getGlobalAttribute("start_time");
			std::string time_string = tattr.getAttributeString();
			if (time_string.length() == 19){
				time_string += ".000Z";
			}
			ccmc::Time time = TimeInterpolator::parseTime(time_string);
			long start_ephem = Kameleon::_date2es(time.getYear(), time.getMonth(), time.getDay(), 
				time.getHour(), time.getMinute(),time.getSeconds());
			if (modelReader->doesAttributeExist("elapsed_time_in_seconds")){
				ccmc::Attribute elapsed_attr = modelReader->getGlobalAttribute("elapsed_time_in_seconds");
				long elapsed_ephem = long(elapsed_attr.getAttributeFloat());
				this->time_et = start_ephem + elapsed_ephem;
			} else {
				this->time_et = start_ephem;
			}
		} else if (modelReader->doesAttributeExist("tim_crstart_cal")){ //for some versions of enlil
			ccmc::Attribute tattr = modelReader->getGlobalAttribute("tim_crstart_cal");
			std::string time_string = tattr.getAttributeString();
			if (time_string.length() == 19){
				time_string += ".000Z";
			}
			ccmc::Time time = TimeInterpolator::parseTime(time_string);
			long start_ephem = Kameleon::_date2es(time.getYear(), time.getMonth(), time.getDay(), 
				time.getHour(), time.getMinute(),time.getSeconds());
			if (modelReader->doesAttributeExist("time_physical_time")){
				ccmc::Attribute elapsed_attr = modelReader->getGlobalAttribute("time_physical_time");
				long elapsed_ephem = long(elapsed_attr.getAttributeFloat());
				this->time_et = start_ephem + elapsed_ephem;
			} else {
				this->time_et = start_ephem;
			}

		} else {
			this->time_et = 0;
		}
	}


	/**
	 * @param time - seconds past 12h January 2000
	 */
	void KameleonInterpolator::setEphemTime(long time)
	{
		this->time_et = time;
	}
			
	

	// Use preferred, model coordinates and stored time
	void KameleonInterpolator::convertCoordinates(Position* v_in,Position* v_out)
	{
		// std::cout << "KameleonInterpolator::convertCoordinates calling convertCoordinates for " 
		// 			<< modelName << " interpolator" << std::endl; 
		this->interpolator->convertCoordinates(this->preferred_coordinates, this->model_coordinates, 
							this->time_et, 
							v_in->c0,v_in->c1,v_in->c2,
							v_out->c0, v_out->c1, v_out->c2);
	}

	/**
	 * @param variable
	 * @param c0
	 * @param c1
	 * @param c2
	 * @return
	 */
	float KameleonInterpolator::interpolate(const std::string& variable, const float& c0, const float& c1,
			const float& c2)
	{
		float dc0, dc1, dc2;
		return interpolate(variable, c0, c1, c2, dc0, dc1, dc2);
		
	}

	/**
	 * @param variable
	 * @param c0
	 * @param c1
	 * @param c2
	 * @param dc0
	 * @param dc1
	 * @param dc2
	 * @param return
	 */
	float KameleonInterpolator::interpolate(const std::string& variable, const float& c0, const float& c1,
			const float& c2, float& dc0, float& dc1, float& dc2)
	{

		Position preferred_p = {c0, c1, c2};
		Position target_p;

		convertCoordinates(&preferred_p, &target_p);

		float interp_value;
		float missingValue = this->modelReader->getMissingValue();

		if (calculationMethod.find(variable) != calculationMethod.end())
		{
			interp_value = (this->*(calculationMethod[variable]))(variable, target_p.c0, target_p.c1, target_p.c2, dc0, dc1, dc2);
			if (interp_value == missingValue)
			{
				return missingValue;
			}
			interp_value = interp_value*this->getConversionFactorToVis(variable);
		} else
		{
			interp_value = interpolateSimple(variable, target_p.c0, target_p.c1, target_p.c2, dc0, dc1, dc2);
			if (interp_value == missingValue)
			{
				return missingValue;
			}
			interp_value *= this->getConversionFactorToVis(variable);

		}


		return interp_value;
	}

	/**
	 * Interpolates using the base model interpolators. Does not perform derived variable calculations.
	 *
	 * @param variable_id
	 * @param c0
	 * @param c1
	 * @param c2
	 * @return
	 */
	float KameleonInterpolator::interpolate(const long& variable_id, const float& c0, const float& c1, const float& c2)
	{
		Position preferred_p = {c0, c1, c2};
		Position target_p;

		convertCoordinates(&preferred_p, &target_p);

		float interp_value = interpolateSimple(variable_id, target_p.c0, target_p.c1, target_p.c2);
		float missingValue = this->modelReader->getMissingValue();

		if (interp_value == missingValue)
		{
			return missingValue;
			//cerr << "***Derived::interpolate: Interpolate returned the missing value for variable " << variable_string << endl;
		}
		interp_value *= this->getConversionFactorToVis(this->modelReader->getVariableName(variable_id));
		return interp_value;
	}

	/**
	 * A variable_id won't work well, since derived variables can be requested, which do not exist in the data.
	 *
	 *
	 * OpenGGCM and BATSRUS: c0,c1,c2 maps to x,y,z, respectively.  ENLIL and MAS: c0,c1,c2 maps
	 * to r,theta(latitude), phi(longitude), respectively
	 * @param variable
	 * @param c0
	 * @param c1
	 * @param c2
	 * @param dc0
	 * @param dc1
	 * @param dc2
	 */
	float KameleonInterpolator::interpolate(const long& variable_id, const float& c0, const float& c1, const float& c2,
			float& dc0, float& dc1, float& dc2)
	{
		Position preferred_p = {c0, c1, c2};
		Position target_p;
		convertCoordinates(&preferred_p, &target_p);

		float interp_value = interpolateSimple(variable_id, target_p.c0, target_p.c1, target_p.c2, dc0, dc1, dc2);
		float missingValue = this->modelReader->getMissingValue();

		if (interp_value == missingValue)
		{
			return missingValue;

		}
		interp_value *= this->getConversionFactorToVis(this->modelReader->getVariableName(variable_id));
		return interp_value;
	}

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
	void KameleonInterpolator::initializeConversionFactorsToVis()
	{
//		float avogadrosNumber = 6.0221415e23;
//		float boltzmannConstant = 1.3806503e-23;
		float gauss2Tesla = 1.e-4;
		float mu_0 = 8.854e-12;
		float l_0 = 6.96e8;
		std::string bx_, by_, bz_, ux_, uy_, uz_, n_, rho_, ex_, ey_, ez_;
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
			conversionFactorsToVis["br"] = 1.e9;
			conversionFactorsToVis["bphi"] = 1.e9;
			conversionFactorsToVis["btheta"] = 1.e9;
			conversionFactorsToVis["b1r"] = 1.e9;
			conversionFactorsToVis["b1phi"] = 1.e9;
			conversionFactorsToVis["b1theta"] = 1.e9;
			conversionFactorsToVis["ur"] = 1.e-3;
			conversionFactorsToVis["uphi"] = 1.e-3;
			conversionFactorsToVis["utheta"] = 1.e-3;
			conversionFactorsToVis["n"] = 1.e-6 * ccmc::constants::Avogadro;
			conversionFactorsToVis["rho"] = 1.e3;
			conversionFactorsToVis["er"] = 1.e-3;
			conversionFactorsToVis["ephi"] = 1.e-3;
			conversionFactorsToVis["etheta"] = 1.e-3;

 			conversionFactorsToVis["br-back"] = 1.e9;
 			conversionFactorsToVis["bphi-back"] = 1.e9;
 			conversionFactorsToVis["btheta-back"] = 1.e9;
 			conversionFactorsToVis["b1r-back"] = 1.e9;
 			conversionFactorsToVis["b1phi-back"] = 1.e9;
 			conversionFactorsToVis["b1theta-back"] = 1.e9;
 			conversionFactorsToVis["ur-back"] = 1.e-3;
 			conversionFactorsToVis["uphi-back"] = 1.e-3;
 			conversionFactorsToVis["utheta-back"] = 1.e-3;
			conversionFactorsToVis["nback"] = 1.e-6 * ccmc::constants::Avogadro;
 			conversionFactorsToVis["rho-back"] = 1.e3;
 			conversionFactorsToVis["er-back"] = 1.e-3;
 			conversionFactorsToVis["ephi-back"] = 1.e-3;
 			conversionFactorsToVis["etheta-back"] = 1.e-3;
			//blah
		} else if (modelName == "mas")
		{
			conversionFactorsToVis["n"] = 1.e8;
			conversionFactorsToVis["t"] = 2.84e7;
			conversionFactorsToVis["p"] = 0.387 * 1.e9 * .1; //Dyn/cm^2 * to_nano * .1
			conversionFactorsToVis["ur"] = 481.37;
			conversionFactorsToVis["uphi"] = 481.37;
			conversionFactorsToVis["utheta"] = 481.37;
			conversionFactorsToVis["jr"] = 1.e2 * 2.205 / (mu_0 * l_0);
			conversionFactorsToVis["jphi"] = 1.e2 * 2.205 / (mu_0 * l_0);
			conversionFactorsToVis["jtheta"] = 1.e2 * 2.205 / (mu_0 * l_0);
			conversionFactorsToVis["er"] = 1.e-3;
			conversionFactorsToVis["ephi"] = 1.e-3;
			conversionFactorsToVis["etheta"] = 1.e-3;
			conversionFactorsToVis["br"] = 2.205 * gauss2Tesla * 1.e9;
			conversionFactorsToVis["bphi"] = 2.205 * gauss2Tesla * 1.e9;
			conversionFactorsToVis["btheta"] = 2.205 * gauss2Tesla * 1.e9;

		} else if (modelName == "ucla_ggcm" || modelName == "open_ggcm")
		{
			conversionFactorsToVis["p"] = 1.e-3;
			conversionFactorsToVis["t"] = 1.e-15 / ccmc::constants::Boltzmann;
			conversionFactorsToVis["s"] = 1.e-4;
			conversionFactorsToVis["pram"] = 1.627e-6;
			conversionFactorsToVis["nv"] = 1.e6;
			conversionFactorsToVis["nvx"] = 1.e6;
			conversionFactorsToVis["nvy"] = 1.e6;
			conversionFactorsToVis["nvz"] = 1.e6;
			conversionFactorsToVis["beta"] = 1.e9;


		} else if (modelName == "ADAPT3D")//just BATSRUS for now
		{
			conversionFactorsToVis["b"] = 2.205 * gauss2Tesla * 1.e9;
			conversionFactorsToVis["bx"] = 2.205 * gauss2Tesla * 1.e9;
			conversionFactorsToVis["by"] = 2.205 * gauss2Tesla * 1.e9;
			conversionFactorsToVis["bz"] = 2.205 * gauss2Tesla * 1.e9;

			conversionFactorsToVis["b"] = 2.205 * gauss2Tesla * 1.e9;
			conversionFactorsToVis["bx"] = 2.205 * gauss2Tesla * 1.e9;
			conversionFactorsToVis["by"] = 2.205 * gauss2Tesla * 1.e9;
			conversionFactorsToVis["bz"] = 2.205 * gauss2Tesla * 1.e9;

		} else if (modelName == "LFM")
		{
			/* See LFMInterpolator for native units
			 */
			conversionFactorsToVis["p"] = 1e9; //[Pa] to [nPa]
			conversionFactorsToVis["rho"] = 1.0/(1.672622e-24);//[grams/cc] to [amu/cc]
			conversionFactorsToVis["e"] = 1000; // [V/m] to [mV/m]
			conversionFactorsToVis["ex"] = 1000;
			conversionFactorsToVis["ey"] = 1000;
			conversionFactorsToVis["ez"] = 1000;
			conversionFactorsToVis["b"] = 1e9; //[T] to [nT]
			conversionFactorsToVis["bx"] = 1e9;
			conversionFactorsToVis["by"] = 1e9;
			conversionFactorsToVis["bz"] = 1e9;
			conversionFactorsToVis["u"] = 1.0f/(1.0e3); //[m/s] to [km/s]
			conversionFactorsToVis["ux"] = 1.0f/(1.0e3);
			conversionFactorsToVis["uy"] = 1.0f/(1.0e3);
			conversionFactorsToVis["uz"] = 1.0f/(1.0e3);

		} else if (modelName == "batsrus")
		{
			conversionFactorsToVis["p"] = 1.0;
			conversionFactorsToVis["t"] = 1.e-12 / ccmc::constants::Boltzmann;
			conversionFactorsToVis["s"] = 1.e-4;
			conversionFactorsToVis["pram"] = 1.627e-6;
			conversionFactorsToVis["nv"] = 1.e6;
			conversionFactorsToVis["nvx"] = 1.e6;
			conversionFactorsToVis["nvy"] = 1.e6;
			conversionFactorsToVis["nvz"] = 1.e6;
			conversionFactorsToVis["beta"] = 1.e12;

		}else {
			conversionFactorsToVis["p"] = 1.e-3;
			conversionFactorsToVis["t"] = 1.e-12 / ccmc::constants::Boltzmann;
			conversionFactorsToVis["s"] = 1.e-4;
			conversionFactorsToVis["pram"] = 1.627e-6;
			conversionFactorsToVis["nv"] = 1.e6;
			conversionFactorsToVis["nvx"] = 1.e6;
			conversionFactorsToVis["nvy"] = 1.e6;
			conversionFactorsToVis["nvz"] = 1.e6;
			conversionFactorsToVis["beta"] = 1.e21;

		}

	}

	/**
	 * Returns the stored conversion factor to convert the variable to the units
	 * used for visualization. These units may differ from SI units and the units
	 * stored in the file.
	 * @param variable
	 * @return
	 */
	float KameleonInterpolator::getConversionFactorToVis(const std::string& variable)
	{
		boost::unordered_map<std::string, float>::iterator iter = conversionFactorsToVis.find(variable);

		if (iter != conversionFactorsToVis.end())
			return (*iter).second;
		else
		{
			return 1.0f;
		}
	}

	/**
	 * @param variable
	 * @param c0
	 * @param c1
	 * @param c2
	 * @return
	 */
	std::vector<float>* KameleonInterpolator::interpolateSimple(std::vector<std::string>& variables, const float& c0, const float& c1, const float& c2)
	{
		std::vector<float>* interpolatedData = new std::vector<float>();
		for (int i = 0; i < variables.size(); i++)
		{
			interpolatedData->push_back(this->interpolateSimple(variables[i], c0, c1, c2));
		}
		return interpolatedData;
	}

	/**
	 * @param variables
	 * @param c0
	 * @param c1
	 * @param c2
	 * @return
	 */
	std::vector<float>* KameleonInterpolator::interpolateSimple(std::vector<long>& variables, const float& c0, const float& c1, const float& c2)
	{
		std::vector<float>* interpolatedData = new std::vector<float>();
		for (int i = 0; i < variables.size(); i++)
		{
			interpolatedData->push_back(this->interpolateSimple(variables[i], c0, c1, c2));
		}
		return interpolatedData;
	}

	/**
	 *
	 */
	KameleonInterpolator::~KameleonInterpolator()
	{
		// TODO Auto-generated destructor stub
		delete interpolator;
	}
}
