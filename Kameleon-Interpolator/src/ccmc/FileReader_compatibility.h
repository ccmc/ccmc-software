/*
 * FileReader_compatibility.h
 *
 *  Created on: Mar 16, 2010
 *      Author: dberrios
 */

#ifndef FILEREADER_COMPATIBILITY_H_
#define FILEREADER_COMPATIBILITY_H_

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
extern _C_ long FileReader_close(int id);
extern _C_ long FileReader_delete(int id);

/** FileReader fortran wrappers **/
extern _C_ void f_filereader_create_(int * id);
extern _C_ void f_filereader_open_(int * id, const char * filename, long * status);
extern _C_ void f_filereader_getvariable_(int * id, const char * variable, float * variableData);
extern _C_ void f_filereader_getvariablebyid_(int * id, long * variableID, float * variableData);
extern _C_ void f_filereader_getvariablesubrange_(int * id, const char * variable, long * startIndex, long * count, float * variableData);
extern _C_ void f_filereader_getvariablebyidsubrange_(int * id, long * variableID, long * startIndex, long * count, float * variableData);
extern _C_ long f_filereader_getvariableid_(int * id, const char * variable);
extern _C_ long f_filereader_close_(int * id, long * status);
extern _C_ long f_filereader_delete_(int * id, long * status);




#endif /* FILEREADER_COMPATIBILITY_H_ */
