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
			long open(const std::string& filename, bool readonly = true);

			virtual std::vector<float>* getVariable(const std::string& variable) = 0;
			virtual std::vector<float>* getVariable(long variableID) = 0;
			virtual std::vector<float>* getVariable(const std::string& variable, long startIndex, long count) = 0;
			virtual std::vector<float>* getVariable(long variableID, long startIndex, long count) = 0;
			virtual float getVariableAtIndex(const std::string& variable, long index) = 0;
			virtual float getVariableAtIndex(long variable_id, long index) = 0;
			virtual std::vector<int>* getVariableInt(const std::string& variable) = 0;
			virtual int getVariableIntAtIndex(const std::string& variable, long index) = 0;
			virtual int getNumberOfGlobalAttributes() = 0;
			virtual int getNumberOfVariables() = 0;
			virtual int getNumberOfVariableAttributes() = 0;
			virtual long getNumberOfRecords(const std::string& variable) = 0;
			virtual long getNumberOfRecords(long variable_id) = 0;
			virtual long getVariableID(const std::string& variable) = 0;
			virtual std::string getVariableName(long variable_id) = 0;
			virtual Attribute getGlobalAttribute(long i) = 0;
			virtual std::string getGlobalAttributeName(long attribute_id) = 0;
			virtual std::string getVariableAttributeName(long attribute_id) = 0;
			virtual Attribute getGlobalAttribute(const std::string& attribute) = 0;
			virtual long getGlobalAttributeID(const std::string& attribute) = 0;
			virtual Attribute getVariableAttribute(const std::string& variable, const std::string& attribute) = 0;
			virtual std::vector<std::string> getVariableAttributeNames() = 0;
			virtual bool doesAttributeExist(const std::string& attribute) = 0;
			virtual bool doesVariableExist(const std::string& variable) = 0;
			void addVariableName(const std::string& variable, long id); //addVariableAttributes
			long close();
			virtual const std::string& getCurrentFilename() = 0;
			virtual void initializeVariableIDs() = 0;
			virtual void initializeVariableNames() = 0;
			virtual long closeFile() = 0;
			virtual long openFile(const std::string& filename, bool readonly) = 0;
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
			std::string current_filename;
			int numGAttributes;
			int numVAttributes;


			boost::unordered_map<std::string, long> variableIDs;
			boost::unordered_map<long, std::string> variableNames;
			boost::unordered_map<std::string, Attribute> gAttributes;
			boost::unordered_map<long, Attribute> gAttributeByID;
			boost::unordered_map<std::string, boost::unordered_map<std::string, Attribute> > vAttributes;
			void initializeGlobalAttributes();
			void initializeVariableAttributes();


	};
}

#endif /* FILEREADER_H_ */
