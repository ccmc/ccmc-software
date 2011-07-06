/*
 * FileReader_compatibility.h
 *
 *  Created on: Mar 16, 2010
 *      Author: dberrios
 */

#ifndef FILEREADER_C_H_
#define FILEREADER_C_H_

#ifdef __cplusplus
#define _C_ "C"
#endif

#ifndef __cplusplus
#define _C_
#endif

/*struct Attribute
{
	char * attributeName;
	char * sValue;
	int iValue;
	float fValue;
	enum {FLOAT, INT, STRING} Type;
};*/
/** FileReader class **/
extern _C_ int FileReader_create();
extern _C_ long FileReader_open(int id, const char * filename);
extern _C_ void FileReader_getVariable(int id, const char * variable, float * variableData);
extern _C_ void FileReader_getVariableByID(int id, long variableID, float * variableData);
extern _C_ void FileReader_getVariableSubRange(int id, const char * variable, long startIndex, long count, float * variableData);
extern _C_ void FileReader_getVariableByIDSubRange(int id, long variableID, long startIndex, long count, float * variableData);
extern _C_ float FileReader_getVariableAtIndex(int id, const char * variable, long index);
extern _C_ float FileReader_getVariableAtIndexByID(long variable_id, long index);
extern _C_ void FileReader_getVariableInt(int id, const char * variable, int * variableData);
extern _C_ int FileReader_getVariableIntAtIndex(int id, const char * variable, long index);
extern _C_ int FileReader_getNumberOfGlobalAttributes(int id);
extern _C_ int FileReader_getNumberOfVariables(int id);
extern _C_ int FileReader_getNumberOfVariableAttributes(int id);
extern _C_ long FileReader_getNumberOfRecords (int id, const char * variable);
extern _C_ long FileReader_getNumberOfRecordsByID (int id, long variable_id);
extern _C_ long FileReader_getVariableID(int id, const char * variable);
extern _C_ void FileReader_getVariableName (int id, long variable_id, char * variableName);
/*extern _C_ struct Attribute FileReader_getGlobalAttributeByID(int id, long i);*/
extern _C_ void FileReader_getGlobalAttributeName(int id, long attribute_id, char * attributeName);
extern _C_ void FileReader_getVariableAttributeName(int id, long attribute_id, char * vAttributeName);
/*extern _C_ struct Attribute FileReader_getGlobalAttribute (int id, const char * attribute);*/
/*extern _C_ struct Attribute FileReader_getVariableAttribute (int id, const char * variable, const char * attribute);*/
extern _C_ bool FileReader_doesAttributeExist (int id,  const char * attribute);
extern _C_ bool FileReader_doesVariableExist (int id, const char * variable);
extern _C_ long FileReader_close (int id);
extern _C_ void FileReader_getCurrentFilename (int id, char * filename);
extern _C_ long FileReader_close(int id);
extern _C_ long FileReader_delete(int id);
/*CDFid 	getCurrentFileID ()*/

#endif /* FILEREADER_C_H_ */
