/*
 * Model.cpp
 *
 *  Created on: Jun 2, 2009
 *      Author: David Berrios
 */

#include "Model.h"
#include <iostream>
#include <algorithm>
#include <boost/foreach.hpp>
#include "Interpolator.h"
#include "Constants.h"

namespace ccmc
{
	/**
	 * Default constructor
	 */
	Model::Model()
	{
		missingValue = ccmc::defaults::missingValue;
		units_ = "units";
		variablesAdded = 0;
	}

	/**
	 * @brief Sets the model name to modelName.
	 *
	 * @param modelName
	 */
	void Model::setModelName(std::string modelName)
	{
		this->modelName = modelName;
	}

	/**
	 * @brief Returns the modelName of the currently selected file.
	 *
	 * @return
	 */
	std::string Model::getModelName()
	{
		return modelName;
	}

	/**
	 * @brief Closes the currently selected file
	 *
	 * @return
	 */
	long Model::close()
	{
		//TODO: clean up memory

		if (variableData.size() > 0)
		{
			boost::unordered_map<std::string, std::vector<float>*>::iterator iter;

			for ( iter=variableData.begin() ; iter != variableData.end(); iter++ )
			{
				delete iter->second;
			}
		}

		if (variableDataInt.size() > 0)
		{
			boost::unordered_map<std::string, std::vector<int>*>::iterator iter;

			for (iter=variableDataInt.begin(); iter != variableDataInt.end(); iter++)
			{
				delete iter->second;
			}
		}

		//the other maps have the same pointers, so they are already deleted.
		variableData.clear();
		variableDataInt.clear();
		variableDataByID.clear();
		variableDataIntByID.clear();

		return GeneralFileReader::close();
	}

	/**
	 * @brief Sets the missing value to use when no valid data exists.
	 *
	 * @param missingValue
	 */
	void Model::setMissingValue(float missingValue)
	{
		this->missingValue = missingValue;
	}

	/**
	 * @brief Returns the missing value that will be returned when no valid data exists.
	 *
	 * This value can be used to check whether an interpolated value is a missing value or not.
	 *
	 * @return Missing value
	 */
	float Model::getMissingValue()
	{
		return missingValue;
	}

	/**
	 * @brief Load a variable into memory.
	 *
	 * Use this method when the variable to load is of type float
	 *
	 * @param variable Variable to load into memory.
	 * @return status of the operation
	 */
	long Model::loadVariable(const std::string& variable)
	{


		//first, check to determine whether variable is already loaded
		if (variableData.find(variable) != variableData.end())
			return FileReader::OK;

		//first check if the variable exists in the file!!
		if (!this->doesVariableExist(variable))
		{
			std::cerr <<"Problem: "<< variable << " does not exist" << std::endl;
			return FileReader::VARIABLE_DOES_NOT_EXIST;
		}
		std::vector<float> * data = getVariable(variable);
		long id = getVariableID(variable);
//std::cout << BOOST_CURRENT_FUNCTION << " id: " << id << std::endl;
		if (data->size() > 0)
		{

			//std::cout << "adding " << variable << " to maps" << std::endl;
			variableData[variable] = data;
			variableDataByID[id] = data;
		} //else return false;
		else
		{
			//std::cout << "not adding " << variable << " to maps" << std::endl;
			delete data;
		}

		return FileReader::OK;
	}

	/**
	 * @brief Unload a variable from memory.
	 * This will return FileReader::OK if the variable is removed from memory, and FileReader::VARIABLE_NOT_IN_MEMORY
	 * if the variable was not already in memory.
	 *
	 * @param variable Variable to unload from memory.
	 * @return status of the operation
	 */
	long Model::unloadVariable(const std::string& variable)
	{
		//bool success = false;

		//first, check to determine whether variable is already loaded
		if (variableData.find(variable) != variableData.end())
		{
			std::vector<float> * data = variableData[variable];
			long id = getVariableID(variable);
			delete data;
			variableData.erase(variable);
			variableDataByID.erase(id);
			return FileReader::OK;
		}
		return FileReader::VARIABLE_NOT_IN_MEMORY;
	}

	/**
	 * @brief Loads a variable into memory.
	 *
	 * Use this method when the variable to load is of type int
	 *
	 * @param variable
	 * @return status of the operation
	 */
	long Model::loadVariableInt(const std::string& variable)
	{

		//first, check to determine whether variable is already loaded
		if (variableDataInt.find(variable) != variableDataInt.end())
			return true;

		//first check if the variable exists in the file!!
		if (!this->doesVariableExist(variable))
		{
			return FileReader::VARIABLE_DOES_NOT_EXIST;
		}
		std::vector<int> * data = getVariableInt(variable);
		long id = getVariableID(variable);
		if (data->size() > 0)
		{
			variableDataInt[variable] = data;
			variableDataIntByID[id] = data;
		} //else return false;

		return FileReader::OK;
	}

	/**
	 * @brief Returns a pointer to the entry in the variableData map containing the variable data.
	 *
	 * This pointer can be modified.
	 *
	 * @param variable Variable to fetch from memory.  This assumes the variable has already been loaded into memory.
	 * If the variable has not been loaded, the pointer will be NULL.
	 *
	 * @return std::vector<float>* of the requested variable.  Note that the pointer points to an entry in
	 * a map. The memory pointed to by the pointer will automatically be freed when
	 * the file is closed, or the Model object is deleted.
	 */
	std::vector<float>* Model::getVariableFromMapRW(const std::string& variable)
	{
		boost::unordered_map<std::string, std::vector<float>*>::iterator iter = variableData.find(variable);

		std::vector<float>* vData = NULL;
		if (iter != variableData.end())
			vData = (*iter).second;
		else
		{
			std::cerr << "variable: " << variable << " not found!!!" << std::endl;
		}
		return vData;
	}

	/**
	 * @brief Returns a const pointer to the entry in the variableData map containing the variable data.
	 *
	 * This pointer cannot be modified.
	 *
	 * @param variable Variable to fetch from memory.  This assumes the variable has already been loaded into memory.
	 * If the variable has not been loaded, the pointer will be NULL.
	 *
	 * @return std::vector<float>* of the requested variable.  Note that the pointer points to an entry in
	 * a map and should not and cannot be deleted. The memory pointed to by the pointer will automatically be freed when
	 * the file is closed, or the Model object is deleted.
	 */
	const std::vector<float>* const Model::getVariableFromMap(const std::string& variable)
	{
		return getVariableFromMapRW(variable);
	}

	/**
	 * @brief Returns the list of variables that have been loaded into memory, using the loadVariable or loadVectorVariable methods
	 *
	 *
	 */
	const std::vector<std::string> Model::getLoadedVariables()
	{
		boost::unordered_map<std::string, std::vector<float>*>::iterator iter;

		std::vector<std::string> variablesLoaded;
		for ( iter=variableData.begin() ; iter != variableData.end(); iter++ )
		{
			variablesLoaded.push_back((*iter).first);
		}
		sort (variablesLoaded.begin(), variablesLoaded.end());

		return variablesLoaded;

	}

	/**
	 * @brief Returns a pointer to the entry in the variableDataInt map containing the variable data.
	 *
	 * This pointer cannot be modified.
	 *
	 * @param variable Variable to fetch from memory.  This assumes the variable has already been loaded into memory.
	 * If the variable has not been loaded, the pointer will be NULL.
	 *
	 * @return std::vector<int>* of the requested variable.  Note that the pointer points to an entry in
	 * a map. The memory pointed to by the pointer will automatically be freed when
	 * the file is closed, or the Model object is deleted.
	 */
	std::vector<int>* Model::getIntVariableFromMapRW(const std::string& variable)
	{
		boost::unordered_map<std::string, std::vector<int>*>::iterator iter = variableDataInt.find(variable);

		std::vector<int>* vData = NULL;
		if (iter != variableDataInt.end())
			vData = (*iter).second;
		return vData;
	}

	/**
	 * @brief Returns a const pointer to the entry in the variableDataInt map containing the variable data.
	 *
	 * This pointer cannot be modified.
	 *
	 * @param variable Variable to fetch from memory.  This assumes the variable has already been loaded into memory.
	 * If the variable has not been loaded, the pointer will be NULL.
	 *
	 * @return std::vector<int>* of the requested variable.  Note that the pointer points to an entry in
	 * a map and should not and cannot be deleted. The memory pointed to by the pointer will automatically be freed when
	 * the file is closed, or the Model object is deleted.
	 */
	const std::vector<int>* const Model::getIntVariableFromMap(const std::string& variable)
	{
		return getIntVariableFromMapRW(variable);
	}

	/**
	 * @brief Returns a pointer to the entry in the variableDataByID map containing the variable data.
	 *
	 * This pointer cannot be modified.
	 *
	 * @param variable_id Variable id of the variable to fetch from memory.  This assumes the variable has already been loaded into memory.
	 * If the variable has not been loaded, the pointer will be NULL.  Request the variable id by using FileReader::getVariableID(const std::string& variable)
	 *
	 * @return std::vector<float>* of the requested variable.  Note that the pointer points to an entry in
	 * a map. The memory pointed to by the pointer will automatically be freed when
	 * the file is closed, or the Model object is deleted.
	 */
	std::vector<float>* Model::getVariableFromMapRW(long variable_id)
	{
		boost::unordered_map<long, std::vector<float>*>::iterator iter = variableDataByID.find(variable_id);

		std::vector<float>* vData = NULL;
		if (iter != variableDataByID.end())
			vData = (*iter).second;
		return vData;
	}

	/**
	 * @brief Returns a const pointer to the entry in the variableDataByID map containing the variable data.
	 *
	 * This pointer cannot be modified.
	 *
	 * @param variable_id Variable id of the variable to fetch from memory.  This assumes the variable has already been loaded into memory.
	 * If the variable has not been loaded, the pointer will be NULL.  Request the variable id by using FileReader::getVariableID(const std::string& variable)
	 *
	 * @return std::vector<float>* of the requested variable.  Note that the pointer points to an entry in
	 * a map and should not and cannot be deleted. The memory pointed to by the pointer will automatically be freed when
	 * the file is closed, or the Model object is deleted.
	 */
	const std::vector<float>* const Model::getVariableFromMap(long variable_id)
	{

		return getVariableFromMapRW(variable_id);
	}

	/**
	 * @brief Returns a pointer to the entry in the variableDataIntByID map containing the variable data.
	 *
	 * This pointer cannot be modified.
	 *
	 * @param variable_id Variable id of the variable to fetch from memory.  This assumes the variable has already been loaded into memory.
	 * If the variable has not been loaded, the pointer will be NULL.  Request the variable id by using FileReader::getVariableID(const std::string& variable)
	 *
	 * @return std::vector<int>* of the requested variable.  Note that the pointer points to an entry in
	 * a map. The memory pointed to by the pointer will automatically be freed when
	 * the file is closed, or the Model object is deleted.
	 */
	std::vector<int>* Model::getIntVariableFromMapRW(long variable_id)
	{
		boost::unordered_map<long, std::vector<int>*>::iterator iter = variableDataIntByID.find(variable_id);

		std::vector<int>* vData = NULL;
		if (iter != variableDataIntByID.end())
			vData = (*iter).second;
		return vData;
	}

	/**
	 * @brief Returns a const pointer to the entry in the variableDataIntByID map containing the variable data.
	 *
	 * This pointer cannot be modified.
	 *
	 * @param variable_id Variable id of the variable to fetch from memory.  This assumes the variable has already been loaded into memory.
	 * If the variable has not been loaded, the pointer will be NULL.  Request the variable id by using FileReader::getVariableID(const std::string& variable)
	 *
	 * @return std::vector<int>* of the requested variable.  Note that the pointer points to an entry in
	 * a map and should not and cannot be deleted. The memory pointed to by the pointer will automatically be freed when
	 * the file is closed, or the Model object is deleted.
	 */
	const std::vector<int>* const Model::getIntVariableFromMap(long variable_id)
	{
		return getIntVariableFromMapRW(variable_id);
	}

	/**
	 * @brief Returns the conversion factor needed to convert the interpolated value to SI units.
	 * @param variable Variable to request the conversion factor for.
	 * @return Conversion factor to convert the specified variable to SI units
	 */
	float Model::getConversionFactorToSI(const std::string& variable)
	{
		boost::unordered_map<std::string, float>::iterator iter = conversionFactorsToSI.find(variable);

		if (iter != conversionFactorsToSI.end())
			return (*iter).second;
		else
			return 1.0f;

	}

	/**
	 * @brief Fetches the native units of the variable as stored in the file.
	 *
	 * @param variable Variable to request units for.
	 * @return The native units of the specified variable, as stored in the file.
	 */
	std::string Model::getNativeUnit(const std::string& variable)
	{
		std::string unit = (getVariableAttribute(variable, units_)).getAttributeString();
		return unit;
	}

	/**
	 * @brief Returns the SI units of the specified variable.
	 *
	 * @param variable
	 * @return
	 */
	std::string Model::getSIUnit(const std::string& variable)
	{
		std::string unit = "";

		boost::unordered_map<std::string, std::string>::iterator iter = variableSIUnits.find(variable);
		if (iter != variableSIUnits.end())
			unit = (*iter).second;
		return unit;
	}

	/**
	 * Destructor
	 */
	Model::~Model()
	{
		//TODO: delete contents of the maps and clear them
		if (variableData.size() > 0)
		{
			boost::unordered_map<std::string, std::vector<float>*>::iterator iter;

			for ( iter=variableData.begin() ; iter != variableData.end(); iter++ )
			{
				delete iter->second;
			}
		}

		if (variableDataInt.size() > 0)
		{
			boost::unordered_map<std::string, std::vector<int>*>::iterator iter;

			for (iter=variableDataInt.begin(); iter != variableDataInt.end(); iter++)
			{
				delete iter->second;
			}
		}

		//the other maps have the same pointers, so they are already deleted.
		variableData.clear();
		variableDataInt.clear();
		variableDataByID.clear();
		variableDataIntByID.clear();
//		std::cout << "model: destructor!" << std::endl;

		/*boost::unordered_map<std::string, std::vector<int>* > variableDataInt;

		 boost::unordered_map<long, std::vector<float>* > variableDataByID;
		 boost::unordered_map<long, std::vector<int>* > variableDataIntByID;*/
	}

	void Model::setBusyStatus(int busyStatus)
	{
		this->busyStatus = busyStatus;
		if (busyStatus == Model::OK)
			this->progress = 100;
		else //busyStatus == Model::BUSY
			this->progress = 0;
	}

	int Model::getBusyStatus()
	{
		return this->busyStatus;
	}

	int Model::getProgress()
	{
		return this->progress;
	}

	/*
	 * Add variable to map
	 */
	void Model::addFloatVariableToMap(const std::string& variable, std::vector<float>* varData){
		int numberOfVariables = getNumberOfVariables(); //number of variables in data file
		long newVariableId = numberOfVariables + variablesAdded; //ID#s start at 0, variablesAdded initially 0
		std::cout<<"adding variable "<<variable<<" to map with id "<< newVariableId << std::endl;
		variableData[variable]=varData;
		variableDataByID[newVariableId]=varData;
		fileReader->addVariableName(variable,newVariableId);
		variablesAdded ++;
		return;
	}

}
