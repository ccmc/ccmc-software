/*
 * FileReader.h
 *
 *  Created on: Apr 22, 2009
 *      Author: David Berrios
 */

#ifndef FILEREADER_H_
#define FILEREADER_H_
#include <string>
#include <vector>
#include <iostream>
#include "Attribute.h"
#include <boost/unordered_map.hpp>
#include "cdf.h"

using namespace std;



namespace ccmc
{


	/**
	 * @class FileReader FileReader.h ccmc/FileReader.h
	 * @brief TODO: Brief description of FileReader class
	 *
	 * TODO: Full description of FileReader class
	 */
	class FileReader
	{

		public:

			FileReader();
			virtual long open(const std::string& filename);

			std::vector<float>* getVariable(const std::string& variable);
			std::vector<int>* getVariableInt(const std::string& variable);
			int getNumberOfGlobalAttributes();
			int getNumberOfVariables();
			int getNumberOfVariableAttributes();
			long getVariableID(const std::string& variable);
			std::string getVariableName(long variable_id);
			Attribute getGlobalAttribute(long i);
			std::string getGlobalAttributeName(long attribute_id);
			std::string getVariableAttributeName(long attribute_id);
			Attribute getGlobalAttribute(const std::string& attribute);
			Attribute getVariableAttribute(const std::string& variable, const std::string& attribute);
			bool doesAttributeExist(const std::string& attribute);
			bool doesVariableExist(const std::string& variable);

			virtual long close();
			CDFid getCurrentFileID();
			const std::string& getCurrentFilename();
			virtual ~FileReader();

		protected:
			CDFid current_file_id;
			std::string current_filename;
			long closeFile();
			long openFile(const std::string& filename);
			boost::unordered_map<std::string, long> variableIDs;
			boost::unordered_map<long, std::string> variableNames;
			boost::unordered_map<std::string, Attribute> gAttributes;
			boost::unordered_map<long, Attribute> gAttributeByID;
			boost::unordered_map<std::string, Attribute> vAttributes;
			void initializeGlobalAttributes();
			void initializeVariableIDs();
			void initializeVariableNames();

	};
}

#endif /* FILEREADER_H_ */
