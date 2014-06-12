/*
 * GeneralFileReader_compatibility.h
 *
 *  Created on: Mar 16, 2010
 *      Author: dberrios
 */

#ifndef GENERALFILEREADER_C_H_
#define GENERALFILEREADER_C_H_

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
extern _C_ int GeneralFileReader_create();
extern _C_ long GeneralFileReader_open(int id, const char * filename);
extern _C_ void GeneralFileReader_getVariable(int id, const char * variable, float * variableData);
extern _C_ void GeneralFileReader_getVariableByID(int id, long variableID, float * variableData);
extern _C_ void GeneralFileReader_getVariableSubRange(int id, const char * variable, long startIndex, long count, float * variableData);
extern _C_ void GeneralFileReader_getVariableByIDSubRange(int id, long variableID, long startIndex, long count, float * variableData);
extern _C_ float GeneralFileReader_getVariableAtIndex(int id, const char * variable, long index);
extern _C_ float GeneralFileReader_getVariableAtIndexByID(long variable_id, long index);
extern _C_ void GeneralFileReader_getVariableInt(int id, const char * variable, int * variableData);
extern _C_ int GeneralFileReader_getVariableIntAtIndex(int id, const char * variable, long index);
extern _C_ int GeneralFileReader_getNumberOfGlobalAttributes(int id);
extern _C_ int GeneralFileReader_getNumberOfVariables(int id);
extern _C_ int GeneralFileReader_getNumberOfVariableAttributes(int id);
extern _C_ long GeneralFileReader_getNumberOfRecords (int id, const char * variable);
extern _C_ long GeneralFileReader_getNumberOfRecordsByID (int id, long variable_id);
extern _C_ long GeneralFileReader_getVariableID(int id, const char * variable);
extern _C_ void GeneralFileReader_getVariableName (int id, long variable_id, char * variableName);
/*extern _C_ struct Attribute GeneralFileReader_getGlobalAttributeByID(int id, long i);*/
extern _C_ void GeneralFileReader_getGlobalAttributeName(int id, long attribute_id, char * attributeName);
extern _C_ void GeneralFileReader_getVariableAttributeName(int id, long attribute_id, char * vAttributeName);
/*extern _C_ struct Attribute GeneralFileReader_getGlobalAttribute (int id, const char * attribute);*/
/*extern _C_ struct Attribute GeneralFileReader_getVariableAttribute (int id, const char * variable, const char * attribute);*/
extern _C_ bool GeneralFileReader_doesAttributeExist (int id,  const char * attribute);
extern _C_ bool GeneralFileReader_doesVariableExist (int id, const char * variable);
extern _C_ long GeneralFileReader_close (int id);
extern _C_ void GeneralFileReader_getCurrentFilename (int id, char * filename);
extern _C_ long GeneralFileReader_close(int id);
extern _C_ long GeneralFileReader_delete(int id);
/*CDFid 	getCurrentFileID ()*/

#endif /* GeneralFileReader_C_H_ */
