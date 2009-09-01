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

using namespace std;

//cdf constants and types
typedef void* CDFid;
typedef long CDFstatus;
#define CDF_OK                          ((CDFstatus) 0)
#define NO_SUCH_ENTRY                   ((CDFstatus) (-2018))
#define CDF_CHAR                51L
#define CDF_INT4                4L


namespace ccmc
{
	//cdf externs
	extern "C" long CDFgetVarNum( CDFid id, char * varName);
	extern "C" CDFstatus CDFgetzVarNumElements( CDFid id, long varNum, long * numElems);
	extern "C" CDFstatus CDFgetzVarName(CDFid id, long varNum, char * varName);
	extern "C" CDFstatus CDFconfirmAttrExistence(CDFid id, char * attrName);
	extern "C" CDFstatus CDFgetAttrzEntry(CDFid id, long attrNum, long entryNum, void * value);
	extern "C" CDFstatus CDFgetAttrzEntryNumElements(CDFid id, long attrNum, long entryNum, long * numElems);
	extern "C" CDFstatus CDFgetAttrzEntryDataType(CDFid id, long attrNum, long entryNum, long * dataType);
	extern "C" CDFstatus CDFgetAttrNum(CDFid id, char * attrName);
	extern "C" CDFstatus CDFgetAttrName(CDFid id, long attrNum, char * attrName);
	extern "C" CDFstatus CDFgetAttrgEntry(CDFid id, long attrNum, long entryNum, void * value);
	extern "C" CDFstatus CDFinquireAttrgEntry(CDFid id, long attrNum, long entryNum, long * dataType, long * numElements);
	extern "C" CDFstatus CDFhyperGetzVarData(CDFid id, long varNum, long recStart, long recCount, long recInterval, long indices[],
			long counts[], long intervals[], void * buffer);
	extern "C" CDFstatus CDFgetzVarDimSizes(CDFid id, long varNum, long dimSizes[]);
	extern "C" CDFstatus CDFcloseCDF(CDFid id);
	extern "C" CDFstatus CDFopenCDF(char * CDFname, CDFid * id);

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
}

#endif /* FILEREADER_H_ */
