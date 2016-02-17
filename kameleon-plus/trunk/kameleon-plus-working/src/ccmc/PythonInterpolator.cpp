/*
 * PythonInterpolator.cpp
 *
 *  Created on: Aug 5, 2013
 *      Author: Asher Pembroke
 */
#include "PythonInterpolator.h"
#include "PythonModel.h"
#include "StringConstants.h"
#include <boost/python.hpp>
#include <sstream>
#include <string>


namespace ccmc
{
	namespace bp = boost::python;

	/**
	 * Constructor
	 * @param modelReader
	 */
	PythonInterpolator::PythonInterpolator(Model * modelReader)
	{
		this->modelReader = modelReader; //points to PythonModel
		
		// bp::exec("import pprint\npprint.pprint(locals())\n", this->modelReader->python_namespace);
		
		//These must be specified by python
		// conversionFactors["x"] = 1.0f;
		// conversionFactorsByID[modelReader->getVariableID("x")] = 1.0f;

	};


	/**
	 * @param variable_id
	 * @param c0
	 * @param c1
	 * @param c2
	 * @return
	 * @ ToDo: call the python interpolator stored in the python namespace
	 */
	float PythonInterpolator::interpolate(const long& variable_id, const float& c0, const float& c1, const float& c2)
	{
		this->modelReader->python_namespace["variable_id"] = variable_id;
		this->modelReader->python_namespace["c0"] = c0;
		this->modelReader->python_namespace["c1"] = c1;
		this->modelReader->python_namespace["c2"] = c2;

		std::stringstream run_string;
		run_string << "result = python_reader.interpolate(variable_id, c0,c1,c2)";
		
		try{
			bp::exec(run_string.str().c_str(), this->modelReader->python_namespace);
			return bp::extract<float>(this->modelReader->python_namespace["result"]);
		} catch (bp::error_already_set) {
			PyErr_Print();
			return this->modelReader->getMissingValue();
		}

	}

	/**
	 * @param variable
	 * @param c0
	 * @param c1
	 * @param c2
	 * @return
	 */
	float PythonInterpolator::interpolate(const std::string& variable, const float& c0, const float& c1,
			const float& c2)
	{
		this->modelReader->python_namespace["variable"] = variable;
		this->modelReader->python_namespace["c0"] = c0;
		this->modelReader->python_namespace["c1"] = c1;
		this->modelReader->python_namespace["c2"] = c2;
		
		std::stringstream run_string;
		run_string << "result = python_reader.interpolate(variable, c0,c1,c2)";
		try{
			bp::exec(run_string.str().c_str(), this->modelReader->python_namespace);
			return bp::extract<float>(this->modelReader->python_namespace["result"]);
		} catch (bp::error_already_set) {
			PyErr_Print();
			return this->modelReader->getMissingValue();
		}
		

	}

	/**
	 * @param variable
	 * @param c0
	 * @param c1
	 * @param c2
	 * @param dc0 Reference to a variable to store the delta for component 0
	 * @param dc1 Reference to a variable to store the delta for component 1
	 * @param dc2 Reference to a variable to store the delta for component 2
	 * @return The interpolated value at position (c0,c1,c2) with deltas (dc0,dc1,dc2)
	 */
	float PythonInterpolator::interpolate(const std::string& variable, const float& c0, const float& c1,
			const float& c2, float& dc0, float& dc1, float& dc2)
	{

		this->modelReader->python_namespace["variable"] = variable;
		this->modelReader->python_namespace["c0"] = c0;
		this->modelReader->python_namespace["c1"] = c1;
		this->modelReader->python_namespace["c2"] = c2;
		
		std::stringstream run_string;
		run_string << "result, dc0, dc1, dc2 = python_reader.interpolate_dc(variable, c0,c1,c2)";
		try{
			bp::exec(run_string.str().c_str(), this->modelReader->python_namespace);

			dc0 = bp::extract<float>(this->modelReader->python_namespace["dc0"]);
			dc1 = bp::extract<float>(this->modelReader->python_namespace["dc1"]);
			dc2 = bp::extract<float>(this->modelReader->python_namespace["dc2"]);

			return bp::extract<float>(this->modelReader->python_namespace["result"]);
			} catch (bp::error_already_set) {
			PyErr_Print();
			return this->modelReader->getMissingValue();
			}
		}

	/**
	 * @param variable_id
	 * @param c0
	 * @param c1
	 * @param c2
	 * @param dc0 Reference to a variable to store the delta for component 0
	 * @param dc1 Reference to a variable to store the delta for component 1
	 * @param dc2 Reference to a variable to store the delta for component 2
	 * @return The interpolated value at position (c0,c1,c2) with deltas (dc0,dc1,dc2)
	 */
	float PythonInterpolator::interpolate(const long& variable_id, const float& c0, const float& c1, const float& c2,
			float& dc0, float& dc1, float& dc2)
	{

		this->modelReader->python_namespace["variable_id"] = variable_id;
		this->modelReader->python_namespace["c0"] = c0;
		this->modelReader->python_namespace["c1"] = c1;
		this->modelReader->python_namespace["c2"] = c2;
		
		std::stringstream run_string;
		run_string << "result, dc0, dc1, dc2 = python_reader.interpolate_dc(variable_id, c0,c1,c2)";
		try{
			bp::exec(run_string.str().c_str(), this->modelReader->python_namespace);

			dc0 = bp::extract<float>(this->modelReader->python_namespace["dc0"]);
			dc1 = bp::extract<float>(this->modelReader->python_namespace["dc1"]);
			dc2 = bp::extract<float>(this->modelReader->python_namespace["dc2"]);

			return bp::extract<float>(this->modelReader->python_namespace["result"]);
		} catch (bp::error_already_set) {
			PyErr_Print();
			return this->modelReader->getMissingValue();

		}
	}

	/**
	 * @param variable
	 * @return
	 */
	float PythonInterpolator::getConversionFactor(const std::string& variable)
	{
		float conversionFactor = 1.0f;
		boost::unordered_map<std::string, float>::iterator cf = conversionFactors.find(variable);

		if (cf != conversionFactors.end())
			conversionFactor = (*cf).second;
		return conversionFactor;
	}

	/**
	 * @param variable_id
	 * @return
	 */
	float PythonInterpolator::getConversionFactor(const long& variable_id)
	{
		float conversionFactor = 1.0f;
		boost::unordered_map<long, float>::iterator cf = conversionFactorsByID.find(variable_id);

		if (cf != conversionFactorsByID.end())
			conversionFactor = (*cf).second;
		return conversionFactor;
	}



	/**
	 * Destructor
	 */
	PythonInterpolator::~PythonInterpolator()
	{
		// std::cout << "Deconstructing Python interpolator object" << std::endl;
	}


}
