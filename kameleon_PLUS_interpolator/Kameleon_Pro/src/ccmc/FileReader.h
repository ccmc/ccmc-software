/*
 * FileReader.h
 *
 *  Created on: Apr 22, 2009
 *      Author: David Berrios
 */

#ifndef FILEREADER_H_
#define FILEREADER_H_
#include <cdf.h>
#include <cdfdist.h>
#include <string>
#include <vector>
#include <iostream>
#include "Attribute.h"
#include <boost/unordered_map.hpp>

using namespace std;


class FileReader
{

	public:

		FileReader();
		virtual long open(const std::string& filename);


		std::vector<float>* getVariable(const std::string& variable);
		std::vector<int>* getVariableInt(const std::string& variable);
		int getNumberOfGlobalAttributes();
		long getVariableID(const std::string& variable);
		std::string getVariableName(long variable_id);
		Attribute getGlobalAttribute(int i);
		Attribute getGlobalAttribute(const std::string& attribute);
		Attribute getVariableAttribute(const std::string& variable, const std::string& attribute);
		bool doesAttributeExist(const std::string& attribute);
		bool doesVariableExist(const std::string& variable);




		virtual long close();
		CDFid getCurrentFileID();
		virtual ~FileReader();

	protected:
		CDFid current_file_id;
		long closeFile();
		long openFile(const std::string& filename);
		boost::unordered_map<std::string, long> variableIDs;
		boost::unordered_map<long, std::string> variableNames;

};

#endif /* FILEREADER_H_ */
