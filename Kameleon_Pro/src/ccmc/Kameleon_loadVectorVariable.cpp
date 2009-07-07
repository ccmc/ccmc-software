#include "Kameleon.h"
#include <string>

/**
  * Loads a vector variable (including all of it's components) into memory.
  * A CDF file must be selected using the open(...) method first.
  *
  * @param variable Vector variable to load into memory
  * @return Returns true if the variable exists or false otherwise.
  */
/*
bool Kameleon::loadVectorVariable(std::string variable)
{

	bool success;

	std::string variable_copy = variable;

	//return the status of whether the variable was successfully loaded into main memory.
	//	This returns true or false, to determine whether the necessary variables
	//	existed in the file.

	//open_cdf((std::string)filename.c_str(), 0);

	bool found = false;
	if (variableAliases.find(variable_copy) != variableAliases.end())
		variable_copy = variableAliases[variable_copy];
	if (listOfRequiredVariablesForVectors.find(variable_copy) == listOfRequiredVariablesForVectors.end())
	{
		if (model->doesVariableExist(variable_copy))
		{
			model->loadVariable(variable_copy);
			found = true;
		}
	}
	else
	{
		//derived variable
		vector<string> listOfVariables = listOfRequiredVariablesForVectors[variable_copy];
		found = true;
		for (int i = 0; i < listOfVariables.size(); i++)
		{
			if (!(model->doesVariableExist(listOfVariables[i])))
				found = false;
			else
				model->loadVariable(listOfVariables[i]);
		}

	}
	return found;
}

*/
