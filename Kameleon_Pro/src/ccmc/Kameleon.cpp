/**
 * @file derived.cpp
 * @brief Utility functions to extend the functionality of the CCMC
 * access and interpolation library.
 *
 * The purpose of this library is to provide easy access to derived variables that
 * can be calculated from the base variables provided by the native cdf files. This
 * includes interpolating derived variables, and units of derived variables.
 *
 * @author David Berrios
 **/
#include "Kameleon.h"
#include "FileReader.h"
#include "Kameleon_compatibility.h"
#include "KameleonInterpolator.h"
#include <boost/lexical_cast.hpp>
#include <boost/foreach.hpp>

#include <cdf.h>
#include <string>
#include <map>
#include <set>
#include <vector>
#include <sstream>
#include <math.h>
#include <iostream>

//#define DEBUG_DERIVED
//#define DEPRECATED_WARNING
using namespace std;

//const Derived * derived = new Derived();

//Kameleon derived;
namespace ccmc
{
	const float Kameleon::defaultMissingValue = 1.0995116278e12;

	/**
	 * Default constructor
	 */
	Kameleon::Kameleon()
	{

		model = NULL;
		modelName = "NA";

		missingValue = defaultMissingValue;
		//	cout << "Derived object created" << endl;


	}

	Kameleon::~Kameleon()
	{

		if (model != NULL)
		{
			delete model;
		}

		closeFile();
		//TODO: delete contents of the maps and clear them
		/*	BOOST_FOREACH(mapStringFloat::value_type i, variableData)
		 {
		 delete i.second;
		 }
		 */

		//the other maps have the same pointers, so they are already deleted.
		//	variableData.clear();

		//	cout << "Derived object destroyed" << endl;
	}
	/*
	 void Kameleon::initializeExtraInformation()
	 {
	 modelName = getGlobalAtributeString("model_name");
	 clearMaps();
	 initializeUnits();
	 initializeListOfRequiredVariablesForComponentsAndVectors();
	 }*/

	/**
	 * Wrapper for Kameleon's gattribute_get(attribute) function.
	 */
	/*
	 bool Kameleon::doesAttributeExist(std::string attribute)
	 {
	 bool exists = model->doesAttributeExist(attribute);
	 return exists;

	 }
	 */

	float Kameleon::getMissingValue()
	{
		return missingValue;
	}

	void Kameleon::setMissingValue(float missingValue)
	{
		this->missingValue = missingValue;
	}

	/**
	 * Clears all maps used in this class.
	 */
	void Kameleon::clearMaps()
	{

		variableNativeUnits.clear();
		variableSIUnits.clear();
		variableVisUnits.clear();
		listOfRequiredVariablesForComponents.clear();
		listOfRequiredVariablesForVectors.clear();
		conversionFactorsToSI.clear();
		conversionFactorsToVis.clear();

		variableAliases.clear();

		//variableData.clear(); //stores the original variable data, if loaded into memory.
		listOfRequiredVariablesForComponents.clear();
		listOfRequiredVariablesForVectors.clear();

	}

	/**
	 * Initializes the extra information required by this library.  Usually this is called whenever
	 * the open methods are called.
	 */
	void Kameleon::initializeExtraInformation()
	{

		//modelName = getGlobalAttributeString("model_name");
		clearMaps();
		initializeUnits();
		initializeListOfRequiredVariablesForComponentsAndVectors();
		//	initializeCalculationMethods();
		//	initializeConversionFactorsToSI();
		//	initializeConversionFactorsToVis();
		initializeVariableAliases();
		initializeConversionFactorsToSI();
		initializeConversionFactorsToVis();
	}

	/**
	 * Returns the units of the variable specified.  The units may differ from the units in the original data.
	 * @param variable
	 * @return String representation of the units
	 */
	std::string Kameleon::getNativeUnit(const std::string& variable)
	{
		std::string units;
		std::string variable_str = variable;
		boost::unordered_map<std::string, std::string>::iterator iter = variableAliases.find(variable_str);
		if (iter != variableAliases.end())
			variable_str = variableAliases[variable_str];
		cout << "variable_str: " << variable_str << endl;

		//units were never fetched before
		//first, attempt fetch from Kameleon's map for derived variables.
		//then fall through to model specific variable units
		iter = variableNativeUnits.find(variable_str);
		if (iter == variableNativeUnits.end())
		{
			units = (model->getVariableAttribute(variable_str, "units")).getAttributeString();
			//variableUnits[variable] = units;
		} else
		{
			//cout << "found in variableUnits" << endl;
			units = "";
		}
		return units;
	}

	std::string Kameleon::getSIUnit(const std::string& variable)
	{
		std::string units;
		std::string variable_str = variable;
		boost::unordered_map<std::string, std::string>::iterator iter = variableAliases.find(variable_str);
		if (iter != variableAliases.end())
			variable_str = variableAliases[variable_str];
		cout << "variable_str: " << variable_str << endl;

		//units were never fetched before
		//first, attempt fetch from Kameleon's map for derived variables.
		//then fall through to model specific variable units
		iter = variableSIUnits.find(variable_str);
		if (iter != variableSIUnits.end())
		{
			units = (*iter).second;
		} else
		{
			units = model->getSIUnit(variable);
			//variableUnits[variable] = units;
		}
		return units;
	}

	std::string Kameleon::getVisUnit(const std::string& variable)
	{
		std::string units;
		std::string variable_str = variable;
		boost::unordered_map<std::string, std::string>::iterator iter = variableAliases.find(variable_str);
		if (iter != variableAliases.end())
			variable_str = variableAliases[variable_str];
		cout << "variable_str: " << variable_str << endl;

		//units were never fetched before
		//first, attempt fetch from Kameleon's map for derived variables.
		//then fall through to model specific variable units
		iter = variableVisUnits.find(variable_str);
		if (iter != variableVisUnits.end())
		{
			units = (*iter).second;
			//variableUnits[variable] = units;
		} else
		{
			units = model->getNativeUnit(variable_str);
		}
		return units;
	}

	/**
	 * Calculates the current time based on the start time and the elapsed time.
	 * @return Formatted string of the current time.  This is currently different for
	 * different models.  If the current time cannot be calculated, "TIME_UNAVAILABLE"
	 * is returned.
	 */
	std::string Kameleon::getCurrentTime()
	{
		//selectCDF(current_cdf_id);
		std::string time_string;
		//string model_name = gattribute_char_get("model_name");

		std::string start_time_char = "";
		if (modelName != "enlil")
			start_time_char = (model->getGlobalAttribute("start_time")).getAttributeString();

		if (start_time_char == "")
		{
			time_string = "TIME UNAVAILABLE";
			if (modelName == "enlil")
			{
				// from some example in http://www.csgnetwork.com/julianmodifdateconv.html
				float mjd = (model->getGlobalAttribute("time_refdate_mjd")).getAttributeFloat();
				float time_physical_time = (model->getGlobalAttribute("time_physical_time")).getAttributeFloat();
				time_physical_time = time_physical_time / 60.0 / 60.0 / 24.0;
				mjd = mjd + time_physical_time;
				double J_d = mjd + 2400001.0;
				double J = (int) J_d;
				int l = (int) J + 68569;
				int n = (int) (4 * l / 146097);
				l = (int) (l - (146097 * n + 3) / 4);
				int year = (int) (4000 * (l + 1) / 1461001);
				l = (int) (l - 1461 * year / 4 + 31);
				int month = (int) (80 * l / 2447);
				int D = l - 2447 * month / 80;
				l = (int) (month / 11);
				int Y = 100 * (n - 49) + year + l;
				int M = month + 2 - 12 * l;
				///MessageBox.Show(D + "/" + M + "/" + Y);
				float remainder = J_d - J;
				if (remainder > 1.0)
				{
					D = D + 1;
					remainder = remainder - 1.0;

				}
				float hours = remainder * 24.0;
				float hours_int = (int) hours;
				float minutes = (hours - hours_int) * 60.0;
				float minutes_int = (int) minutes;
				float seconds = (minutes - minutes_int) * 60.0;
				//double seconds_int = (int)(seconds + .5);
				float seconds_int = (int) (seconds);

				string time_string_month = boost::lexical_cast<std::string>(M);
				string time_string_day = boost::lexical_cast<std::string>(D);
				string time_string_year = boost::lexical_cast<std::string>(Y);
				string time_string_hours = boost::lexical_cast<std::string>((int) hours);
				string time_string_minutes = boost::lexical_cast<std::string>((int) minutes);
				string time_string_seconds = boost::lexical_cast<std::string>((int) seconds_int);

				time_string = padString(time_string_year, 4) + "/" + padString(time_string_month, 2) + "/" + padString(
						time_string_day, 2) + " " + padString(time_string_hours, 2) + ":" + padString(
						time_string_minutes, 2) + ":" + padString(time_string_seconds, 2);
			}

		} else
		{
			string start_time = start_time_char;
			float elapsed_time = (model->getGlobalAttribute("elapsed_time_in_seconds")).getAttributeFloat();

			char buffer[50];
			char current_time[25];
			double cdf_epoch_time = parseEPOCH3((char *) start_time.c_str());
			encodeEPOCH3(cdf_epoch_time, buffer);
			//float temp = elapsed_time * 1000.0;
			cdf_epoch_time = (elapsed_time * 1000.0) + cdf_epoch_time;
			encodeEPOCH3(cdf_epoch_time, current_time);
			time_string = current_time;
		}
		return time_string;
	}

	string Kameleon::padString(const string& s, int minLength)
	{
		string output = s;
		while ((int) (output.length()) < minLength)
		{
			output = "0" + output;
		}
		return output;
	}

	/**string Kameleon::convertToString(int value)
	 {
	 stringstream ss;
	 ss << value;
	 string return_value;
	 ss >> return_value;
	 return return_value;

	 }**/

	/**
	 string Kameleon::convertToString(double value)
	 {
	 stringstream ss;
	 ss << value;
	 string return_value;
	 ss >> return_value;
	 return return_value;

	 }
	 */
	/**
	 * Helper function to create an apporpriate vector containing the list of required variables.
	 * This is used in conjunction with openWithMultipleVariables
	 * @return a vector<string> containing the variable names
	 */
	vector<string> Kameleon::createVectorFromList(int num, ...)
	{
		vector<string> neededVariables;
		va_list list_of_variables;
		va_start(list_of_variables, num);
		for (int i = 0; i < num; i++)
		{
			neededVariables.push_back(va_arg(list_of_variables, char *));
		}
		return neededVariables;
	}

	/**
	 * Opens a CDF with multiple variables, in a more programmer friendly way.
	 */
	/*void Kameleon::openWithMultipleVariables(char * filename, vector<string> variables)
	 {
	 int num = variables.size();
	 while(variables.size() < 10)
	 {
	 variables.push_back("");
	 }
	 open_cdf(filename, num, (char *)variables[0].c_str(), (char *)variables[1].c_str(), (char *)variables[2].c_str(), (char *)variables[3].c_str(),
	 (char *)variables[4].c_str(), (char *)variables[5].c_str(), (char *)variables[6].c_str(), (char *)variables[7].c_str(),
	 (char *)variables[8].c_str(), (char *)variables[9].c_str());
	 current_cdf_id = get_current_CDFid();
	 //cout << "Opened multiple variables" << endl;

	 }*/

	/**
	 * Closes the currently opened file.
	 */
	long Kameleon::close()
	{
		model->close();
		delete model;
		model = NULL;
		closeFile();
		//TODO: delete contents of the maps and clear them


	}

	/*void Kameleon::selectCDF(CDFid cdf_id)
	 {
	 CDFstatus status = CDFlib(SELECT_, CDF_, cdf_id,
	 NULL_);
	 }*/

	/*
	 void ccmc_derived_close_cdf()
	 {
	 derived.close();
	 }
	 */

	/** Opens a cdf file and loads the variables requested into main memory. When appropriate,
	 * the required variables to calculate the derived variable are loaded.
	 * @param filename Filename of the cdf file to load
	 * @param variable Variable to load into main memory
	 **/
	/**
	 void ccmc_derived_open_cdf_main_mem(char * filename, char * variable)
	 {
	 derived.open(filename);
	 derived.loadVariable(variable);




	 }

	 void ccmc_derived_open_cdf_main_mem_vector(char * filename, char * var)
	 {

	 derived.open(filename);
	 derived.loadVectorVariable(var);



	 }
	 */

	/**
	 float ccmc_derived_interpolate( char * var, float x, float y, float z, int flag1, int flag2, ...)
	 {

	 #ifdef DEBUG_DERIVED
	 cout << "entered interpolate with: " << var << endl;
	 #endif
	 string variable = var;
	 va_list optional_arguments;
	 va_start(optional_arguments, flag2);
	 float missing, dx_blk, dy_blk, dz_blk, *dx, *dy, *dz;
	 float interp_value = -pow(256.f,5.f);
	 if (flag2 == 1)
	 {
	 #ifdef DEBUG_DERIVED
	 cerr << "flag2 = 1" << endl;
	 #endif
	 missing = va_arg(optional_arguments, double);
	 dx = va_arg(optional_arguments, float *);
	 dy = va_arg(optional_arguments, float *);
	 dz = va_arg(optional_arguments, float *);
	 interp_value = missing;
	 }
	 else
	 {
	 #ifdef DEBUG_DERIVED
	 cerr << "flag2 = 0" << endl;
	 #endif
	 missing = interp_value;
	 }

	 interp_value = derived.interpolate( variable, x, y, z, dx_blk, dy_blk, dz_blk);
	 //interp_value = interpolate_batsrus_cdf( var, x, y, z,1,0);// missing, dx_blk, dy_blk, dz_blk);
	 if (flag2 == 1)
	 {
	 *dx = dx_blk;
	 *dy = dy_blk;
	 *dz = dz_blk;
	 }
	 else
	 {
	 dx = &dx_blk;
	 dy = &dy_blk;
	 dz = &dz_blk;
	 }//*dx = dx_blk;
	 //*dy = dy_blk;
	 //*dz = dz_blk;
	 return interp_value;
	 }

	 */
	/**
	 * Returns the units of a variable, including derived variables.
	 * @param var The variable to get the units for
	 * @return The units of the variable var
	 **/
	/*
	 char * ccmc_derived_get_units(char * var)
	 {
	 return (char *)derived.getUnits(var).c_str();
	 }
	 */

	/**
	 * Returns the current time as a string
	 * @return String representation of the current time
	 */
	/*
	 char * ccmc_derived_getcurrenttime()
	 {
	 return (char *)(derived.getCurrentTime().c_str());
	 }
	 */
	/**
	 *
	 */
	/*
	 void open_cdf_derived_main_mem( char * filename, char * variable)
	 {
	 #ifdef DEPRECATED_WARNING
	 cerr << __FUNCTION__ << " is deprecated. Please use ccmc_derived_open_cdf_main_mem instead." << endl;
	 #endif
	 ccmc_derived_open_cdf_main_mem(filename, variable);
	 }

	 void open_cdf_derived_main_mem_vector( char * filename, char * variable)
	 {
	 #ifdef DEPRECATED_WARNING
	 cerr << __FUNCTION__ << " is deprecated. Please use ccmc_derived_main_mem_vector instead." << endl;
	 #endif
	 ccmc_derived_open_cdf_main_mem_vector(filename, variable);
	 }

	 float interpolate_derived( char * variable, float x, float y, float z, int flag1, int flag2, ...)
	 {
	 #ifdef DEPRECATED_WARNING
	 cerr << __FUNCTION__ << " is deprecated. Please use ccmc_derived_" << __FUNCTION__ << " instead." << endl;
	 #endif
	 va_list optional_arguments;

	 va_start(optional_arguments, flag2);
	 float missing,  *dx_blk,  *dy_blk,  *dz_blk;
	 if (flag2 == 1)
	 {
	 missing = va_arg(optional_arguments, double);
	 dx_blk = va_arg(optional_arguments, float *);
	 dy_blk = va_arg(optional_arguments, float *);
	 dz_blk = va_arg(optional_arguments, float *);
	 }
	 else
	 {
	 missing = 0.0;
	 }

	 return ccmc_derived_interpolate(variable, x, y, z, flag1, flag2, missing, dx_blk, dy_blk, dz_blk);
	 }

	 char * derived_get_units(char * variable)
	 {
	 #ifdef DEPRECATED_WARNING
	 cerr << __FUNCTION__ << " is deprecated. Please use ccmc_derived_" << __FUNCTION__ << " instead." << endl;
	 #endif
	 return ccmc_derived_get_units(variable);
	 }

	 char * getcurrenttime()
	 {
	 #ifdef DEPRECATED_WARNING
	 cerr << __FUNCTION__ << " is deprecated. Please use ccmc_derived_" << __FUNCTION__ << " instead." << endl;
	 #endif
	 return ccmc_derived_getcurrenttime();
	 }
	 */

	Interpolator * Kameleon::createNewInterpolator()
	{
		return new KameleonInterpolator(model);
	}

	bool Kameleon::loadVariable(const std::string& variable)
	{

		std::vector<std::string> requiredVariables = this->getListOfRequiredVariablesForComponents(variable);
		bool success = true;
		for (int i = 0; i < requiredVariables.size(); i++)
		{
			std::cout << "loading " << requiredVariables[i] << std::endl;
			if (!model->loadVariable(requiredVariables[i]))
				success = false;
		}
		return success;
	}

	bool Kameleon::loadVectorVariable(const std::string& variable)
	{

		std::vector<std::string> requiredVariables = this->getListOfRequiredVariablesForVectors(variable);
		bool success = true;
		for (int i = 0; i < requiredVariables.size(); i++)
		{
			std::cout << "loading " << requiredVariables[i] << std::endl;
			if (!model->loadVariable(requiredVariables[i]))
				success = false;
		}
		return success;
	}

	std::vector<std::string> Kameleon::getListOfRequiredVariablesForComponents(std::string variable)
	{
		boost::unordered_map<std::string, std::vector<std::string> >::iterator iter =
				this->listOfRequiredVariablesForComponents.find(variable);
		if (iter != listOfRequiredVariablesForComponents.end())
		{
			return (*iter).second;
		} else
		{
			if (model->doesVariableExist(variable))
			{
				std::vector<std::string> required;
				required.push_back(variable);
				return required;
			} else
			{
				std::vector<std::string> required;
				return required;
			}
		}
	}

	std::vector<std::string> Kameleon::getListOfRequiredVariablesForVectors(std::string variable)
	{
		boost::unordered_map<std::string, std::vector<std::string> >::iterator iter =
				this->listOfRequiredVariablesForVectors.find(variable);
		if (iter != listOfRequiredVariablesForVectors.end())
		{
			return (*iter).second;
		} else
		{
			if (model->doesVariableExist(variable))
			{
				std::vector<std::string> required;
				required.push_back(variable);
				return required;
			} else
			{
				std::vector<std::string> required;
				return required;
			}
		}
	}

	/*
	 bool Kameleon::loadVariableInt(const std::string& variable)
	 {
	 return model->loadVariableInt(variable);
	 }*/

	/*bool Kameleon::loadVectorVariable(std::string variable)
	 {
	 return false;
	 }*/

	float Kameleon::getConversionFactorToSI(const std::string& variable)
	{
		boost::unordered_map<std::string, float>::iterator iter = conversionFactorsToSI.find(variable);

		if (iter != conversionFactorsToSI.end())
			return (*iter).second;
		else
		{
			std::cout << "Kameleon::getConversionFactorToSI(): " << std::endl;
			return model->getConversionFactorToSI(variable);
		}
	}

	float Kameleon::getConversionFactorToVis(const std::string& variable)
	{
		boost::unordered_map<std::string, float>::iterator iter = conversionFactorsToVis.find(variable);

		if (iter != conversionFactorsToVis.end())
			return (*iter).second;
		else
		{
			return 1.0f;
		}
	}

	void Kameleon::initializeConversionFactorsToSI()
	{

		conversionFactorsToSI["b"] = 1e-9f;

		/*    conversionFactorsToSI["pram"] = 1.0f;
		 conversionFactorsToSI["edotj"] = 1.0f;
		 conversionFactorsToSI["e"] = "mV/m";
		 conversionFactorsToSI["ex"] = "mV/m";
		 conversionFactorsToSI["ey"] = "mV/m";
		 conversionFactorsToSI["ez"] = "mV/m";
		 conversionFactorsToSI["er"] = "mV/m";
		 conversionFactorsToSI["etheta"] = "mV/m";
		 conversionFactorsToSI["ephi"] = "mV/m";
		 conversionFactorsToSI["br"] = "nT";
		 conversionFactorsToSI["b_r"] = "nT";
		 conversionFactorsToSI["b_lat"] = "nT";
		 conversionFactorsToSI["btheta"] = "nT";
		 conversionFactorsToSI["b_lon"] = "nT";
		 conversionFactorsToSI["bphi"] = "nT";
		 conversionFactorsToSI["j"] = "muA/m^2";
		 conversionFactorsToSI["jparx"] = "muA/m^2";
		 conversionFactorsToSI["jpary"] = "muA/m^2";
		 conversionFactorsToSI["jparz"] = "muA/m^2";
		 conversionFactorsToSI["jparr"] = "muA/m^2";
		 conversionFactorsToSI["jpartheta"] = "muA/m^2";
		 conversionFactorsToSI["jparphi"] = "muA/m^2";
		 conversionFactorsToSI["jpar"] = "muA/m^2";
		 conversionFactorsToSI["jxbx"] = "fN/m^3";
		 conversionFactorsToSI["jxby"] = "fN/m^3";
		 conversionFactorsToSI["jxbz"] = "fN/m^3";
		 conversionFactorsToSI["jxb"] = "fN/m^3";
		 conversionFactorsToSI["exbr"] = "muW/m^2";
		 conversionFactorsToSI["exbphi"] = "muW/m^2";
		 conversionFactorsToSI["exbtheta"] = "muW/m^2";
		 conversionFactorsToSI["exbx"] = "muW/m^2";
		 conversionFactorsToSI["exby"] = "muW/m^2";
		 conversionFactorsToSI["exbz"] = "muW/m^2";
		 conversionFactorsToSI["exb"] = "muW/m^2";
		 conversionFactorsToSI["s"] = "Km^2";
		 conversionFactorsToSI["en"] = "J/m^3";
		 conversionFactorsToSI["nv"] = "cm^-2s^-1";
		 conversionFactorsToSI["nvx"] = "cm^-2s^-1";
		 conversionFactorsToSI["nvy"] = "cm^-2s^-1";
		 conversionFactorsToSI["nvz"] = "cm^-2s^-1";
		 conversionFactorsToSI["u"] = "km/s";
		 conversionFactorsToSI["v"] = "km/s";
		 conversionFactorsToSI["v_r"] = "km/s";
		 conversionFactorsToSI["v_lat"] = "km/s";
		 conversionFactorsToSI["v_lon"] = "km/s";
		 conversionFactorsToSI["n"] = "amu/cm^3";
		 conversionFactorsToSI["n*r^2"] = "AU^2 cm^-3";
		 conversionFactorsToSI["p*r^2"] = "nPa AU^2";
		 conversionFactorsToSI["p"] = "nPa";
		 conversionFactorsToSI["polb"] = "";
		 conversionFactorsToSI["b1"] = "nT";
		 conversionFactorsToSI["eta*j"] = "mV/m";
		 conversionFactorsToSI["eta*jx"] = "mV/m";
		 conversionFactorsToSI["eta*jy"] = "mV/m";
		 conversionFactorsToSI["eta*jz"] = "mV/m";
		 conversionFactorsToSI["eta*j^2"] = "pW/m^3";


		 conversionFactorsToSI["beta"] = "";
		 if (modelName == "enlil")
		 {
		 conversionFactorsToSI["ux"] = "km/s";
		 conversionFactorsToSI["uy"] = "km/s";
		 conversionFactorsToSI["uz"] = "km/s";
		 conversionFactorsToSI["bx"] = "nT";
		 conversionFactorsToSI["by"] = "nT";
		 conversionFactorsToSI["bz"] = "nT";
		 conversionFactorsToSI["b1x"] = "nT";
		 conversionFactorsToSI["b1y"] = "nT";
		 conversionFactorsToSI["b1z"] = "nT";
		 }

		 */

	}

	const std::string& Kameleon::getModelName()
	{
		return modelName;
	}
}
