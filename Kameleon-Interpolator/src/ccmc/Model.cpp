/*
 * Model.cpp
 *
 *  Created on: Jun 2, 2009
 *      Author: David Berrios
 */

#include "Model.h"
#include <iostream>
#include <boost/foreach.hpp>
#include "Interpolator.h"

namespace ccmc
{
	/**
	 * Default constructor
	 */
	Model::Model()
	{
		missingValue = -256. * -256. * -256. * -256. * -256.;
		units_ = "units";
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
		BOOST_FOREACH(mapStringFloat::value_type i, variableData)
		{
			delete i.second;
		}

		BOOST_FOREACH(mapStringInt::value_type i, variableDataInt)
		{
			delete i.second;
		}

		//the other maps have the same pointers, so they are already deleted.
		variableData.clear();
		variableDataInt.clear();
		variableDataByID.clear();
		variableDataIntByID.clear();

		return closeFile();
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
	 * @return true if successful, else false
	 */
	bool Model::loadVariable(const std::string& variable)
	{
		bool success = false;

		//first, check to determine whether variable is already loaded
		if (variableData.find(variable) != variableData.end())
			return true;

		vector<float> * data = getVariable(variable);
		long id = getVariableID(variable);
		if (data->size() > 0)
		{
			success = true;
			variableData[variable] = data;
			variableDataByID[id] = data;
		} //else return false;

		return success;
	}

	/**
	 * @brief Unload a variable from memory.
	 *
	 * @param variable Variable to unload from memory.
	 * @return true if successful, else false
	 */
	bool Model::unloadVariable(const std::string& variable)
	{
		bool success = false;

		//first, check to determine whether variable is already loaded
		if (variableData.find(variable) != variableData.end())
		{
			vector<float> * data = variableData[variable];
			long id = getVariableID(variable);
			delete data;
			variableData.erase(variable);
			variableDataByID.erase(id);
			return true;
		}
		return false;
	}

	/**
	 * @brief Loads a variable into memory.
	 *
	 * Use this method when the variable to load is of type int
	 *
	 * @param variable
	 * @return
	 */
	bool Model::loadVariableInt(const std::string& variable)
	{

		bool success = false;

		//first, check to determine whether variable is already loaded
		if (variableDataInt.find(variable) != variableDataInt.end())
			return true;

		vector<int> * data = getVariableInt(variable);
		long id = getVariableID(variable);
		if (data->size() > 0)
		{
			success = true;
			variableDataInt[variable] = data;
			variableDataIntByID[id] = data;
		} //else return false;

		return success;
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
	const std::vector<float>* Model::getVariableData(const std::string& variable)
	{
		boost::unordered_map<std::string, std::vector<float>*>::iterator iter = variableData.find(variable);

		std::vector<float>* vData = NULL;
		if (iter != variableData.end())
			vData = (*iter).second;
		else
			std::cout << "variable: " << variable << " not found!!!" << std::endl;
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
	const std::vector<int>* Model::getVariableDataInt(const std::string& variable)
	{
		boost::unordered_map<std::string, std::vector<int>*>::iterator iter = variableDataInt.find(variable);

		std::vector<int>* vData = NULL;
		if (iter != variableDataInt.end())
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
	const std::vector<float>* Model::getVariableDataByID(long variable_id)
	{
		boost::unordered_map<long, std::vector<float>*>::iterator iter = variableDataByID.find(variable_id);

		std::vector<float>* vData = NULL;
		if (iter != variableDataByID.end())
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
	const std::vector<int>* Model::getVariableDataIntByID(long variable_id)
	{
		boost::unordered_map<long, std::vector<int>*>::iterator iter = variableDataIntByID.find(variable_id);

		std::vector<int>* vData = NULL;
		if (iter != variableDataIntByID.end())
			vData = (*iter).second;
		return vData;
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
		BOOST_FOREACH(mapStringFloat::value_type i, variableData)
		{
			delete i.second;
		}

		BOOST_FOREACH(mapStringInt::value_type i, variableDataInt)
		{
			delete i.second;
		}

		//the other maps have the same pointers, so they are already deleted.
		variableData.clear();
		variableDataInt.clear();
		variableDataByID.clear();
		variableDataIntByID.clear();
		std::cout << "model: destructor!" << std::endl;

		/*boost::unordered_map<std::string, std::vector<int>* > variableDataInt;

		 boost::unordered_map<long, std::vector<float>* > variableDataByID;
		 boost::unordered_map<long, std::vector<int>* > variableDataIntByID;*/
	}
}
