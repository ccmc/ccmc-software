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
			Attribute getVariableAttribute(const std::string& variable, const std::string& attribute);
			std::vector<std::string> getVariableAttributeNames();
			bool doesAttributeExist(const std::string& attribute);
			bool doesVariableExist(const std::string& variable);
			virtual long close();
			long * getCurrentFileID();
			const std::string& getCurrentFilename();
			virtual ~FileReader();

			static const long OK = 0L;
			static const long OPEN_ERROR = -1L;
			static const long FILE_DOES_NOT_EXIST = -2L;
			static const long VARIABLE_DOES_NOT_EXIST = -3L;
			static const long ATTRIBUTE_DOES_NOT_EXIST = -4L;
			static const long LOAD_FAILED = -5L;
			static const long UNABLE_TO_ALLOCATE_MEMORY = -6L;
			static const long VARIABLE_NOT_IN_MEMORY = -7L;
			static const long MODEL_NOT_SUPPORTED = -8L;
			static const long NOT_A_VALID_KAMELEON_FILE = -9L;

		protected:
			long * current_file_id;
			std::string current_filename;
			long closeFile();
			long openFile(const std::string& filename);
			boost::unordered_map<std::string, long> variableIDs;
			boost::unordered_map<long, std::string> variableNames;
			boost::unordered_map<std::string, Attribute> gAttributes;
			boost::unordered_map<long, Attribute> gAttributeByID;
			boost::unordered_map<std::string, boost::unordered_map<std::string, Attribute> > vAttributes;
			void initializeGlobalAttributes();
			void initializeVariableAttributes();
			void initializeVariableIDs();
			void initializeVariableNames();

	};
}

#endif /* FILEREADER_H_ */
