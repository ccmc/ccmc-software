/***
 David Berrios


 Header file for derived variable functionality.  Just include this file
 and link to the interplation library.  Include derived_compatibility.h if
 you want to use the C interface instead.
 **/

#ifndef KAMELEON_H
#define KAMELEON_H

/**
 * @mainpage Kameleon Interpolator
 * The Kameleon-Interpolator library is a C++ library that provides classes to access and interpolate from data files
 * produced by multiple space weather products.  It currently supports the output from the following products:
 * - OpenGGCM
 * - BATSRUS (Global MHD) and the BATSRUS component of SWMF
 * - Ionospheric component of SWMF
 * - ENLIL
 * - MAS
 * - CTIP
 * - ADAPT3D
 *
 * Java, C, and Fortran compatible interfaces are provided through the use of the wrapper headers and code (ccmc/wrappers/*).
 *
 * @section Purpose
 * The purpose of this library is to provide easy access and interpolation methods
 * for multiple space weather simulation model datasets, to provide access to derived variables, and
 * to calculate fieldlines using any of the model vector field variables.  Derived
 * variables are those that can be calculated from the base variables provided by the native data files.
 *
 * @section Examples Example code
 *
 * @section License License
 * @class Kameleon Kameleon.h ccmc/Kameleon.h
 */


#include <map>
#include <set>
#include <string>
#include <vector>
#include <boost/unordered_map.hpp>
#include <boost/array.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/format.hpp>
#include "Interpolator.h"
#include "Model.h"
#include "GeneralFileReader.h"
#include "TimeInterpolator.h"
#include "Constants.h"
#include "cxform.h"
#include <iostream>
#include "CCMCTime.h"
#include "Tracer.h"


//cdf constants
#define EPOCH3_STRING_LEN        24




/**
 * @namespace ccmc
 * @brief Namespace containing CCMC created and maintained code
 */
namespace ccmc
{

	extern "C" double parseEPOCH3(char epString[EPOCH3_STRING_LEN+1]);
	extern "C" void encodeEPOCH3(double epoch, char epString[EPOCH3_STRING_LEN+1]);
	extern "C" void EPOCHbreakdown(
			double epoch,
			long *year,
			long *month,
			long *day,
			long *hour,
			long *minute,
			long *second,
			long *msec);

	class Tracer;
	class TimeInterpolator;
	struct Position
	{
		float c0;
		float c1;
		float c2;
	};





	/**
	 * @class Kameleon Kameleon.h ccmc/Kameleon.h
	 * @brief TODO: Brief description of Kameleon class
	 *
	 * TODO: Full description of Kameleon class
	 */
	class Kameleon
	{
		public:
			Kameleon();
			virtual ~Kameleon();
			long close();
			Interpolator* createNewInterpolator();
			bool doesAttributeExist(const std::string& attribute);
			bool doesVariableExist(const std::string& variable);
			float getConversionFactorToSI(const std::string& variable);
			const std::string& getCurrentFilename();
			Time getCurrentTime();
			Attribute getGlobalAttribute(long i);
			Attribute getGlobalAttribute(const std::string& attribute);
			std::string getGlobalAttributeName(long attribute_id);
			std::vector<std::string> getLoadedVariables();
			float getMissingValue();
			const std::string& getModelName();
			std::string getNativeUnit(const std::string& variable);
			int getNumberOfGlobalAttributes();
			int getNumberOfVariableAttributes();
			int getNumberOfVariables();
			std::string getSIUnit(const std::string& variable);
			std::vector<float>* getVariable(const std::string& variable);
			Attribute getVariableAttribute(const std::string& variable, const std::string& attribute);
			std::string getVariableAttributeName(long attribute_id);
			const std::vector<float>* const getVariableFromMap(const std::string& variable);
			long getVariableID(const std::string& variable);
			std::vector<int>* getVariableInt(const std::string& variable);
			const std::vector<int>* const getVariableIntFromMap(const std::string& variable);
			std::string getVariableName(long variable_id);
			std::string getVisUnit(const std::string& variable);
			bool loadVariable(const std::string& variable);
			bool loadVectorVariable(const std::string& variable);
			long open(const std::string& filename); //the individual models need a different open method
			void setMissingValue(float missingValue);
			bool unloadVariable(const std::string& variable);
			bool unloadVectorVariable(const std::string& variable);
			int getProgress();
			static int _cxform(const char *from,const char *to,const double et,Position* v_in,Position* v_out);
			static long _cxRound(double doub);
			static long _date2es(int yyyy, int mm, int dd, int hh, int mm2, int ss);
			static double _gregorian_calendar_to_jd(int y, int m, int d, int h, int mi, int s);

			Model * model;

		private:

			boost::unordered_map<std::string, std::string> variableNativeUnits;
			boost::unordered_map<std::string, std::string> variableSIUnits;
			boost::unordered_map<std::string, std::string> variableVisUnits;

			boost::unordered_map<std::string, float> conversionFactorsToSI;


			boost::unordered_map<std::string, std::string> variableAliases;
			boost::unordered_map<std::string, std::vector<std::string> > listOfRequiredVariablesForComponents;
			boost::unordered_map<std::string, std::vector<long> > listOfRequiredVariablesForComponentsByID;
			boost::unordered_map<std::string, std::vector<std::string> > listOfRequiredVariablesForVectors;
			boost::unordered_map<std::string, std::vector<long> > listOfRequiredVariablesForVectorsByID;


			void clearMaps();
			void initializeUnits();
			void initializeSIUnits();
			void initializeVisUnits();

			void initializeListOfRequiredVariablesForComponentsAndVectors();
			void initializeExtraInformation();

			void initializeVariableAliases();
			void initializeVariableUnits();


			std::vector<std::string> createVectorOfStringFromList(int num, std::string * strings);
			std::vector<long> createVectorOfLongFromList(int num, std::string * strings);
			std::string padString(const std::string& s, int minLength);
			std::string modelName;

			std::vector<std::string> getListOfRequiredVariablesForComponents(std::string variable);
			std::vector<std::string> getListOfRequiredVariablesForVectors(std::string variable);

			void initializeConversionFactorsToSI();
			//void initializeConversionFactorsToVis();
			float missingValue;

			void addRequirementsForComponents(std::string variable, int num, std::string * variables);
			void addRequirementsForVectors(std::string variable, int num, std::string * variables);




	};
	static boost::unordered_map<int, ccmc::Kameleon *> kameleonObjects;
	static boost::unordered_map<int, ccmc::Tracer *> tracerObjects;
	static boost::unordered_map<int, ccmc::Interpolator *> interpolatorObjects;
	static boost::unordered_map<int, ccmc::GeneralFileReader *> generalFileReaderObjects;
	static boost::unordered_map<int, ccmc::TimeInterpolator *> timeInterpolatorObjects;

}

#endif/** KAMELEON_H **/

