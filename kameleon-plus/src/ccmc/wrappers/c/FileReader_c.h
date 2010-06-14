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

/** FileReader class **/
extern _C_ int FileReader_create(int id);
extern _C_ long FileReader_open(int id, const char * filename);
extern _C_ void FileReader_getVariable(int id, const char * variable, float * variableData);
extern _C_ void FileReader_getVariableByID(int id, long variableID, float * variableData);
extern _C_ void FileReader_getVariableSubRange(int id, const char * variable, long startIndex, long count, float * variableData);
extern _C_ void FileReader_getVariableByIDSubRange(int id, long variableID, long startIndex, long count, float * variableData);
extern _C_ long FileReader_getVariableID(int id, const char * variable);
extern _C_ void FileReader_getVariableInt(int id, const char * variable, int * variableData);
extern _C_ int FileReader_getNumberOfGlobalAttributes(int id);
extern _C_ int FileReader_getNumberOfVariables(int id);
extern _C_ int FileReader_getNumberOfVariableAttributes(int id);
extern _C_ long FileReader_getNumberOfRecords(int id, const char * variable);
extern _C_ long FileReader_getNumberOfRecordsByID(int id, long variable_id);
extern _C_ long FileReader_close(int id);
extern _C_ long FileReader_delete(int id);


#endif /* FILEREADER_C_H_ */
