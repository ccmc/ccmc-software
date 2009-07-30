/***
 David Berrios


 Header file for derived variable functionality.  Just include this file
 and link to the interoplation library.  Include derived_compatibility.h if
 you want to use the C interface instead.
 **/

#ifndef KAMELEON_H
#define KAMELEON_H
#include <stdarg.h>

/**
 * @mainpage Kameleon Interpolator
 * The Kameleon-Interpolator library is a C++ library that provides classes to access and interpolate from data files
 * produced by multiple space weather products.  It currently supports the output from the following products:
 * - OpenGGCM
 * - BATSRUS (Global MHD) and the BATSRUS component of SWMF
 * - ENLIL
 * - MAS
 * - CTIP
 *
 * A C and Fortran compatible interface is provided through the use of the compatibility header (ccmc/kameleon_compatibility.h).
 *
 * @section Purpose
 * The purpose of this library is to provide easy access and interpolation methods
 * for multiple space weather simulation models, and to provide access to derived variables.  Derived
 * variables are those that can be calculated from the base variables provided by the native data files.
 *
 * @section Examples Example code
 *
 * @section License License blurb
 */


#include <map>
#include <set>
#include <string>
#include <vector>
#include <boost/unordered_map.hpp>
#include "Interpolator.h"
#include "Model.h"
#include "FileReader.h"
#include "Constants.h"

using namespace std;
using namespace ccmc;

/**
 * @namespace ccmc
 * @brief Namespace containing CCMC created and maintained code
 */
namespace ccmc
{
	/**
	 * @class Kameleon Kameleon.h ccmc/Kameleon.h
	 * @brief TODO: Brief description of Kameleon class
	 *
	 * TODO: Full description of Kameleon class
	 */
	class Kameleon: public FileReader
	{
		public:

			Kameleon();
			long open(const std::string& filename); //the individual models need a different open method
			const std::string& getModelName();
			void setMissingValue(float missingValue);
			float getMissingValue();

			Interpolator* createNewInterpolator();
			bool loadVariable(const std::string& variable);
			bool loadVectorVariable(const std::string& variable);
			std::string getNativeUnit(const std::string& variable);
			std::string getSIUnit(const std::string& variable);
			std::string getVisUnit(const std::string& variable);
			float getConversionFactorToSI(const std::string& variable);
			float getConversionFactorToVis(const std::string& variable);
			static const float defaultMissingValue;//  = -1.0995116278e12; //-256.*-256.*-256.*-256.
			long close();
			virtual ~Kameleon();

		private:
			boost::unordered_map<std::string, std::string> variableNativeUnits;
			boost::unordered_map<std::string, std::string> variableSIUnits;
			boost::unordered_map<std::string, std::string> variableVisUnits;

			boost::unordered_map<std::string, float> conversionFactorsToSI;
			boost::unordered_map<std::string, float> conversionFactorsToVis;

			boost::unordered_map<std::string, std::string> variableAliases;
			boost::unordered_map<std::string, std::vector<std::string> > listOfRequiredVariablesForComponents;
			boost::unordered_map<std::string, std::vector<std::string> > listOfRequiredVariablesForVectors;

			Model * model;
			void clearMaps();
			void initializeUnits();
			void initializeSIUnits();
			void initializeVisUnits();

			void initializeListOfRequiredVariablesForComponentsAndVectors();
			void initializeExtraInformation();

			void initializeVariableAliases();
			void initializeVariableUnits();

			vector<string> createVectorFromList(int num, ...);
			std::string padString(const std::string& s, int minLength);
			std::string modelName;
			std::string getCurrentTime();
			std::vector<std::string> getListOfRequiredVariablesForComponents(std::string variable);
			std::vector<std::string> getListOfRequiredVariablesForVectors(std::string variable);

			void initializeConversionFactorsToSI();
			void initializeConversionFactorsToVis();
			float missingValue;
	};
}

//declared static variables for the C/Fortran interface functions
static boost::unordered_map<int, Kameleon *> kameleonObjects;
static boost::unordered_map<int, Interpolator *> interpolatorObjects;

#endif /** KAMELEON_H **/

