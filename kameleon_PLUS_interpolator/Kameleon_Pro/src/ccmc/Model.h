/*
 * Model.h
 *
 *  Created on: Jun 2, 2009
 *      Author: David Berrios
 */

#ifndef MODEL_H_
#define MODEL_H_
#include <string>
#include <map>
#include <vector>
#include <cdf.h>
#include <boost/unordered_map.hpp>

#include "FileReader.h"
#include "Interpolator.h"

class Interpolator;
/*
 *
 */
class Model: public FileReader
{

	public:
		Model();
		virtual long open(const std::string& filename) = 0; //the individual models need a different open method
		void setModelName(std::string);
		std::string getModelName();


		bool loadVariable(const std::string& variable);
		bool loadVariableInt(const std::string& variable);

		const std::vector<float>* getVariableData(const std::string& variable);
		const std::vector<int>* getVariableDataInt(const std::string& variable);

		const std::vector<float>* getVariableDataByID(long variable_id);
		const std::vector<int>* getVariableDataIntByID(long variable_id);

		void setMissingValue(float missingValue);
		float getMissingValue();
		float getConversionFactorToSI(const std::string& variable);
		std::string getNativeUnit(const std::string& variable);
		std::string getSIUnit(const std::string& variable);



		/**
		 * Closes the currently selected file.
		 */
		long close();

		virtual Interpolator* createNewInterpolator() = 0;


		//const FileReader& getFileReader();
		virtual ~Model();

	protected:
		//only need these for the BOOST_FOREACH macro to work
		typedef boost::unordered_map<std::string, std::vector<float>* > mapStringFloat;
		typedef boost::unordered_map<std::string, std::vector<int>* > mapStringInt;
		typedef boost::unordered_map<long, std::vector<float>* > mapLongFloat;
		typedef boost::unordered_map<long, std::vector<int>* > mapLongInt;
		std::string modelName;
		boost::unordered_map<std::string, float> conversionFactorsToSI; //fetch variable name first, if using variable id instead
		boost::unordered_map<std::string, std::string> variableSIUnits;
		boost::unordered_map<std::string, std::vector<float>* > variableData;
		boost::unordered_map<std::string, std::vector<int>* > variableDataInt;

		boost::unordered_map<long, std::vector<float>* > variableDataByID;
		boost::unordered_map<long, std::vector<int>* > variableDataIntByID;
		virtual void initializeConversionFactorsToSI() = 0;
		virtual void initializeSIUnits() = 0;
		std::string units_;

		float missingValue;


};

#endif /* MODEL_H_ */
