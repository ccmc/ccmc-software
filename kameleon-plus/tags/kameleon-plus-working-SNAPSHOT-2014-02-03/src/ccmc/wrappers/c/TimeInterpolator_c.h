#ifndef TIMEINTERPOLATOR_C_H
#define TIMEINTERPOLATOR_C_H


#ifdef __cplusplus
#define _C_ "C"
#endif

#ifndef __cplusplus
#define _C_
#endif

extern _C_ int TimeInterp_create();
extern _C_ int TimeInterp_addTimestep(int tid, const char * filename);
extern _C_ void TimeInterp_manageMem(int tid, double epoch, const char * variables[], int numVars);
extern _C_ float TimeInterp_interpolate(int tid, double epoch, const char * variable, float c0, float c1, float c2);
extern _C_ double TimeInterp_TimeToEpoch(int year, int month, int day, int hour, int minute, int second, int millisecond);
extern _C_ double TimeInterp_TimeStrToEpoch(const char * time_str);
extern _C_ void TimeInterp_closeAndCleanupMemory(int tid);
#endif
