/*
 * PyFileReader.h
 *
 *  Created on: March 5, 2015
 *      Author: Asher Pembroke
 */
#ifdef PYFILEREADER_
#include <boost/python.hpp>
#include <FileReader.h>
#include <vector>
#include <string>
#include "Attribute.h"


namespace ccmc
{

	/**
	 * @class PyFileReader
	 * @brief Wraps the embedded python reader functions
	 *
	 * TODO: Full description of FileReader class
	 */
	class PyFileReader
	{

		public:

			PyFileReader();
			long open(const std::string& filename);

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
			long close();
			//long closeFile();
			//long openFile(const std::string& filename);
			const std::string& getCurrentFilename();
			~PyFileReader();

		protected:
			std::string current_filename;

			boost::unordered_map<std::string, long> variableIDs;
			boost::unordered_map<long, std::string> variableNames;
			boost::unordered_map<std::string, Attribute> gAttributes;
			boost::unordered_map<long, Attribute> gAttributeByID;
			boost::unordered_map<std::string, boost::unordered_map<std::string, Attribute> > vAttributes;
			void initializeGlobalAttributes();
			void initializeVariableAttributes();
			void initializeVariableIDs();
			void initializeVariableNames();

			FileReader * fileReader;

	};
}
#endif
