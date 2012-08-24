/*
 * FileReader.h
 *
 *  Created on: Apr 22, 2009
 *      Author: David Berrios
 */

#ifndef CDFFILEREADER_H_
#define CDFFILEREADER_H_
#include <string>
#include <vector>
#include <iostream>
#include "Attribute.h"
#include <boost/unordered_map.hpp>
#include "cdf.h"
#include "FileReader.h"





namespace ccmc
{


	/**
	 * @class FileReader FileReader.h ccmc/FileReader.h
	 * @brief TODO: Brief description of FileReader class
	 *
	 * TODO: Full description of FileReader class
	 */

	class CDFFileReader : public FileReader
	{

		private:
			std::string current_filename;
			CDFid current_file_id;

		public:

			CDFFileReader();
			std::vector<float>* getVariable(const std::string& variable);
			std::vector<float>* getVariable(long variableID);
			std::vector<float>* getVariable(const std::string& variable, long startIndex, long count);
			std::vector<float>* getVariable(long variableID, long startIndex, long count);
			float getVariableAtIndex(const std::string& variable, long index);
			float getVariableAtIndex(long variable_id, long index);
			std::vector<int>* getVariableInt(const std::string& variable);
			int getVariableIntAtIndex(const std::string& variable, long index);
			int getNumberOfGlobalAttributes();
			int getNumberOfVariables();
			int getNumberOfVariableAttributes();
			long getNumberOfRecords(const std::string& variable);
			long getNumberOfRecords(long variable_id);
			long getVariableID(const std::string& variable);
			std::string getVariableName(long variable_id);
			Attribute getGlobalAttribute(long i);
			std::string getGlobalAttributeName(long attribute_id);
			std::string getVariableAttributeName(long attribute_id);
			Attribute getGlobalAttribute(const std::string& attribute);
			long getGlobalAttributeID(const std::string& attribute);
			Attribute getVariableAttribute(const std::string& variable, const std::string& attribute);
			std::vector<std::string> getVariableAttributeNames();
			bool doesAttributeExist(const std::string& attribute);
			bool doesVariableExist(const std::string& variable);
			bool doesAttributeExist(long attribute);
			bool doesVariableExist(long variable);
			const std::string& getCurrentFilename();
			virtual ~CDFFileReader();


		protected:


			long closeFile();
			long openFile(const std::string& filename, bool readonly = true);
			void initializeGlobalAttributes();
			void initializeVariableAttributes();
			void initializeVariableIDs();
			void initializeVariableNames();

	};
}

#endif /* CDFFILEREADER_H_ */
