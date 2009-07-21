/*
 * Model.cpp
 *
 *  Created on: Jun 2, 2009
 *      Author: David Berrios
 */

#include "Model.h"
#include <cdf.h>
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
	 * @param name
	 */
	void Model::setModelName(std::string name)
	{
		modelName = name;
	}

	/**
	 * @return
	 */
	std::string Model::getModelName()
	{
		return modelName;
	}

	/**
	 * Closes the file
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
	 * @param missingValue
	 */
	void Model::setMissingValue(float missingValue)
	{
		this->missingValue = missingValue;
	}

	float Model::getMissingValue()
	{
		return missingValue;
	}

	/**
	 * @param variable
	 * @return
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
	 * @param variable
	 * @return
	 */
	bool Model::loadVariableInt(const std::string& variable)
	{
		std::cout << "**************** inside loadVariableInt" << std::endl;
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
	 * @param variable
	 * @return std::vector<float>* of the requested variable.  Note that the pointer points to an entry in
	 * a map and should not be deleted. The memory pointed to by the pointer will automatically be freed when
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
	 * @param variable
	 * @return
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
	 * @param variable_id
	 * @return
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
	 * @param variable_id
	 * @return
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
	 * @param variable
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
	 * @param variable
	 * @return The native units of the specified variable, as stored in the file.
	 */
	std::string Model::getNativeUnit(const std::string& variable)
	{
		std::string unit = (getVariableAttribute(variable, units_)).getAttributeString();
		return unit;
	}

	/**
	 *
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
	 *
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
