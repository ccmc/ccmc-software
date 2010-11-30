/*
 * Variable.cpp
 *
 *  Created on: Nov 24, 2010
 *      Author: David Berrios
 */

#include "Variable.h"

namespace ccmc
{
	Variable::Variable()
	{

	}

	void Variable::setName(std::string name)
	{
		this->name = name;
	}

	std::string Variable::getName()
	{
		return this->name;
	}

	void Variable::setVisUnits(std::string visUnits)
	{
		this->visUnits = visUnits;
	}

	void Variable::setSIUnits(std::string SIUnits)
	{
		this->SIUnits = SIUnits;
	}

	std::string Variable::getVisUnits()
	{
		return this->visUnits;
	}

	std::string Variable::getSIUnits()
	{
		return this->SIUnits;
	}

	void Variable::setRequiredVariables(std::vector<std::string> variables)
	{
		this->requiredVariables = variables;
	}

	void Variable::setRequiredVariablesForVectors(std::vector<std::string> variables)
	{
		this->requiredVariablesForVectors = variables;
	}

	void Variable::setEquation(std::string equation)
	{
		this->equation = equation;
	}

	Variable::~Variable()
	{

	}
}
