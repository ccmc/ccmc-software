/*
 * FileReader_compatibility.h
 *
 *  Created on: Mar 16, 2010
 *      Author: dberrios
 */

#ifndef FILEREADER_F_H_
#define FILEREADER_F_H_

#ifdef __cplusplus
#define _C_ "C"
#endif

#ifndef __cplusplus
#define _C_
#endif

/** FileReader fortran wrappers **/
extern _C_ void f_filereader_create_(int * id);
extern _C_ void f_filereader_open_(int * id, const char * filename, long * status);
extern _C_ void f_filereader_getvariable_(int * id, const char * variable, float * variableData);
extern _C_ void f_filereader_getvariablebyid_(int * id, long * variableID, float * variableData);
extern _C_ void f_filereader_getvariablesubrange_(int * id, const char * variable, long * startIndex, long * count, float * variableData);
extern _C_ void f_filereader_getvariablebyidsubrange_(int * id, long * variableID, long * startIndex, long * count, float * variableData);
extern _C_ void f_filereader_getvariableid_(int * id, const char * variable);
extern _C_ void f_filereader_getvariableint(int * id, const char * variable, int * variableData);
extern _C_ void f_filereader_getnumberofglobalattributes(int * id, long * num);
extern _C_ void f_filereader_getnumberofvariables(int * id, long * num);
extern _C_ void f_filereader_getnumberofvariableattributes(int * id, long * num);
extern _C_ void f_filereader_getnumberofrecords(int id, const char * variable, long * num);
extern _C_ void f_filereader_getnumberofrecordsbyid(int id, long variable_id, long * num);
extern _C_ void f_filereader_close_(int * id, long * status);
extern _C_ void f_filereader_delete_(int * id, long * status);




#endif /* FILEREADER_F_H_ */
