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
 * @mainpage Test Title
 * asdfasdfasdfadf
 * @section Purpose
 * @section Examples
 * @section License
 */

#include <map>
#include <set>
#include <string>
#include <vector>
#include <cdf.h>
#include <boost/unordered_map.hpp>
#include "Interpolator.h"
#include "Model.h"
#include "FileReader.h"
#include "Constants.h"

using namespace std;

class Kameleon : public FileReader
{
	public:

		Kameleon();
		long open(const std::string& filename); //the individual models need a different open method
		std::string getModelName();
		//bool loadVariable(std::string variable);
		bool loadVariableInt(const std::string& variable);
		//bool loadVariable(std::string variable);
		//bool loadVariableInt(std::string variable);
		//const std::vector<float>* getVariableData(std::string variable);
		//const std::vector<int>* getVariableDataInt(std::string variable);
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
		//typedef boost::unordered_map<std::string, std::vector<float>*> mapStringFloat;
		boost::unordered_map<std::string, std::string> variableNativeUnits ;
		boost::unordered_map<std::string, std::string> variableSIUnits;
		boost::unordered_map<std::string, std::string> variableVisUnits;

		//boost::unordered_map<std::string, std::vector <float> *> variableData; //stores the original variable data, if loaded into memory.

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


		//void initializeConversionFactorsToSI();
		//void initializeConversionFactorsToVis();
		void initializeVariableAliases();
		void initializeVariableUnits();

		vector<string>  createVectorFromList(int num, ...);
		std::string padString(const std::string& s, int minLength);
		std::string modelName;
		//std::string filename;
		std::string getCurrentTime();
		std::vector<std::string> getListOfRequiredVariablesForComponents(std::string variable);

		void initializeConversionFactorsToSI();
		void initializeConversionFactorsToVis();
		float missingValue;
};

//static boost::unordered_map
static boost::unordered_map<int, Kameleon *> kameleonObjects;
//static Interpolator * interpolatorObject = NULL;

#endif /** KAMELEON_H **/

