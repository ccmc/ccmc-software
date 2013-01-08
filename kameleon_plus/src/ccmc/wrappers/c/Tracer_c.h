#ifndef TRACER_C_H
#define TRACER_C_H

#ifdef __cplusplus
#define _C_ "C"
#endif

#ifndef __cplusplus
#define _C_
#endif

extern _C_ int Tracer_create(int kid);
extern _C_ int Tracer_setInnerBoundary(int tid, float innerBoundary);
//extern _C_ void Tracer_open(int id, const char * filename);
//extern _C_ void Tracer_loadVariable(int id, const char * variable);
extern _C_ void Tracer_bidirectionalTrace(int id, const char * variable, const float * startComponent1, const float * startComponent2,
		const float * startComponent3, const int* step_max, const float* dn, int * actual_steps, float * x_array, float * y_array, float * z_array);
extern _C_ void Tracer_unidirectionalTrace(int id, const char * variable, const float* startComponent1, const float* startComponent2,
		const float* startComponent3, const int* step_max, const float* dn, int * actual_steps, float * x_array, float * y_array, float * z_array);
extern _C_ int Tracer_delete(int id);

#endif/*end TRACER_C_H*/
