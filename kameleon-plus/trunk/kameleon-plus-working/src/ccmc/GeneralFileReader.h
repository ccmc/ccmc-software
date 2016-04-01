/*
 * FileReader.h
 *
 *  Created on: Apr 22, 2009
 *      Author: David Berrios
 */

#ifndef GENERALFILEREADER_H_
#define GENERALFILEREADER_H_
#include <string>
#include <vector>
#include <iostream>
#include "Attribute.h"
#include "FileReader.h"
#include <boost/unordered_map.hpp>
#include <boost/filesystem.hpp>
#include "cdf.h"
#include <boost/python.hpp>


namespace ccmc
{


	/**
	 * @class GeneralFileReader GeneralFileReader.h ccmc/GeneralFileReader.h
	 * @brief The GeneralFileReader class is a factory class for creating file readers in a model-agnostic fashion.
	 * 
	 * GeneralFileReader is not a subclass of FileReader, but instead creates an instance of FileReader (CDFFileReader,HDF5FileReader, or other) and calls its functions. 
	 * Warning: FileReader's protected maps are repeated here, so it is possible to fill the maps variableIDs, variableNames, 
	 * gAttributes, gAttributeByID, and vAttributes of GeneralFileReader independently from those of the FileReader class. 
	 * Try to work with the FileReader maps wherever possible to avoid confusion. 
	 */
	class GeneralFileReader
	{

		public:

			GeneralFileReader();
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
			~GeneralFileReader();
			boost::python::object python_namespace; //can execute python commands on this object


		protected:
			std::string current_filename; 

			boost::unordered_map<std::string, long> variableIDs; /*!< variable srtring to id map */
			boost::unordered_map<long, std::string> variableNames; /*!< variableID to string name */
			boost::unordered_map<std::string, Attribute> gAttributes; /*!< global attributes by name */
			boost::unordered_map<long, Attribute> gAttributeByID; /*!< global attributes by ID */
			boost::unordered_map<std::string, boost::unordered_map<std::string, Attribute> > vAttributes; /*!< variable attributes map */
			void initializeGlobalAttributes(); //no defintion, see FileReader
			void initializeVariableAttributes(); //no definition, see FileReader
			void initializeVariableIDs(); 
			void initializeVariableNames(); 

			FileReader * fileReader; /*!< polymorphic pointer to FileReader. It will point to either a CDFFileReader or HDF5FileReader instance. */

	};
}

#endif /* GENERALFILEREADER_H_ */
