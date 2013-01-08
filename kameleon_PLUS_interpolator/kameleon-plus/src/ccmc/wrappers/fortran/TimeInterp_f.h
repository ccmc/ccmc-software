#ifndef TIMEINTERP_F_H
#define TIMEINTERP_F_H


#ifdef __cplusplus
#define _C_ "C"
#endif

#ifndef __cplusplus
#define _C_
#endif

extern _C_ void f_timeinterp_create_(int * tid);
extern _C_ void f_timeinterp_addtimestep_(int * tid, const char * filename);
extern _C_ void f_timeinterp_managemem_(int * tid, double * epoch, const char * variables);
extern _C_ void f_timeinterp_interpolate_(int * tid, double * epoch, const char * variable, float * c0, float * c1, float * c2, float * result);
extern _C_ void f_timeinterp_timetoepoch_(int * year, int * month, int * day, int * hour, int * minute, int * second, int * millisecond, double * epoch);
extern _C_ void f_timeinterp_timestrtoepoch_(const char * time_str, double * epoch);
extern _C_ void f_timeinterp_closeandcleanupmemory_(int * tid);
#endif
