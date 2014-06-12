/*
 * Interpolator.cpp
 *
 *  Created on: Jun 30, 2009
 *      Author: dberrios
 */

#include "Interpolator.h"
#include <iostream>
using namespace std;
namespace ccmc
{
	/**
	 * Default constructor
	 */
	Interpolator::Interpolator()
	{
		// TODO Auto-generated constructor stub

	}

	/**
	 * @param variable
	 * @return
	 */
	float Interpolator::getConversionFactor(const std::string& variable)
	{
		//cout << "Inside Interpolator::getConversionFactor" << endl;
		return 1.0f;
	}

	/**
	 * @param variable_id
	 * @return
	 */
	float Interpolator::getConversionFactor(const long& variable_id)
	{
		return 1.0f;
	}

	std::string Interpolator::getModelName()
	{
		return this->modelReader->getModelName();
	}

	/*
	 * Sets the model reader for the interpolator. This allows the user to use the same
	 * interpolator on different input, since interpolator initialization can be expensive
	 */
	void Interpolator::setModelReader(Model * reader)
	{
		this->modelReader = reader;
	}
	/**
	 * Destructor
	 */
	Interpolator::~Interpolator()
	{
		// TODO Auto-generated destructor stub
	}
}
