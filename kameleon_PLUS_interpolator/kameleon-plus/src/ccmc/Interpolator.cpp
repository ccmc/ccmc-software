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
	 * Sets the missing value;
	 */
	void Interpolator::setMissingValue(float missingValue)
	{
		this->missingValue = missingValue;
	}


	float Interpolator::getConversionFactor(const std::string&)
	{
		//cout << "Inside Interpolator::getConversionFactor" << endl;
		return 1.0f;
	}

	float Interpolator::getConversionFactor(const long& variable_id)
	{
		return 1.0f;
	}


	/**
	 * Destructor
	 */
	Interpolator::~Interpolator()
	{
		// TODO Auto-generated destructor stub
	}
}
