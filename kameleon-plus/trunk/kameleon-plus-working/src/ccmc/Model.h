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
#include <boost/unordered_map.hpp>

#include "GeneralFileReader.h"
#include "Interpolator.h"

namespace ccmc
{
	class Interpolator;
	/**
	 * @class Model Model.h ccmc/Model.h
	 * @brief TODO: Brief description of Model class
	 *
	 * TODO: Full description of Model class
	 */
	class Model: public GeneralFileReader
	{

		public:
			Model();

			static const int BUSY=1;
			static const int OK=2;

			/**
			 * @brief Opens a file
			 *
			 * Opens a file and performs any necessary initialization required to
			 * work with the data.
			 * @param filename
			 */
			virtual long open(const std::string& filename) = 0; //the individual models need a different open method
			void setModelName(std::string modelName);
			std::string getModelName();

			long loadVariable(const std::string& variable);
			long unloadVariable(const std::string& variable);
			long loadVariableInt(const std::string& variable);

			std::vector<float>* getVariableFromMapRW(const std::string& variable);
			std::vector<int>* getIntVariableFromMapRW(const std::string& variable);
			const std::vector<float>* const getVariableFromMap(const std::string& variable);
			const std::vector<int>* const getIntVariableFromMap(const std::string& variable);

			void addFloatVariableToMap(const std::string& variable, std::vector<float>* variableData);

			const std::vector<float>* const getVariableFromMap(long variable_id);
			const std::vector<int>* const getIntVariableFromMap(long variable_id);

			std::vector<float>* getVariableFromMapRW(long variable_id);
			std::vector<int>* getIntVariableFromMapRW(long variable_id);

			virtual const std::vector<std::string> getLoadedVariables();

			void setMissingValue(float missingValue);
			float getMissingValue();
			float getConversionFactorToSI(const std::string& variable);
			std::string getNativeUnit(const std::string& variable);
			std::string getSIUnit(const std::string& variable);
			int getProgress();
			int getBusyStatus();


			long close();

			/**
			 * @brief Returns an Interpolator object for the currently opened file.
			 *
			 * This returns an Interpolator object that contains all the necessary local variables required to
			 * interpolate independent of any other Interpolator object.  The pointer must be deleted from the calling program.
			 * @return A pointer to an Interpolator object.
			 */
			virtual Interpolator* createNewInterpolator() = 0;

			//const FileReader& getFileReader();
			virtual ~Model();

		protected:
			//only need these for the BOOST_FOREACH macro to work
			typedef boost::unordered_map<std::string, std::vector<float>*> mapStringFloat;
			typedef boost::unordered_map<std::string, std::vector<int>*> mapStringInt;
			typedef boost::unordered_map<long, std::vector<float>*> mapLongFloat;
			typedef boost::unordered_map<long, std::vector<int>*> mapLongInt;
			std::string modelName;
			boost::unordered_map<std::string, float> conversionFactorsToSI; //fetch variable name first, if using variable id instead
			boost::unordered_map<std::string, std::string> variableSIUnits;
			boost::unordered_map<std::string, std::vector<float>*> variableData;
			boost::unordered_map<std::string, std::vector<int>*> variableDataInt;

			boost::unordered_map<long, std::vector<float>*> variableDataByID;
			boost::unordered_map<long, std::vector<int>*> variableDataIntByID;

			/**
			 * @brief Initializes the conversionFactorsToSI map.
			 *
			 * These factors are used to convert interpolated values to SI units.
			 */
			virtual void initializeConversionFactorsToSI() = 0;

			void setBusyStatus(int busyStatus);

			/**
			 * @brief Initializes the variableSIUnits map.
			 */
			virtual void initializeSIUnits() = 0;
			std::string units_;

			float missingValue;
			int busyStatus;
			int progress;
			int variablesAdded;

	};
}
#endif /* MODEL_H_ */
