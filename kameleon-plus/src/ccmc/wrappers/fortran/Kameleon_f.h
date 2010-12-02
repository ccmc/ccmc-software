#ifndef KAMELEON_F_H
#define KAMELEON_F_H

#ifdef __cplusplus
#define _C_ "C"
#endif

#ifndef __cplusplus
#define _C_
#endif

extern _C_ void f_kameleon_create_(int * id);
extern _C_ void f_kameleon_open_(int * id, const char * filename, long * status);
extern _C_ void f_interpolator_create(int * kid, int * id);
//extern _C_ void f_kameleon_get_model_name(int id, char * model_name);
extern _C_ void f_kameleon_interpolate(int * id, const char * variable, const float * c0,
		float * c1, float *c2, float * dc0, float * dc1, float * dc2, float * returnValue);
//extern _C_ void f_kameleon_load_variable(int *id, const char * variable);
extern _C_ void f_kameleon_load_vector_variable_(int *id, const char * variable);
//extern _C_ void Kameleon_get_global_attribute_string(int id, const char * gAttribute, char * destbuffer);
extern _C_ void f_kameleon_close_(int * id);
extern _C_ void f_kameleon_delete_(int * id, long * status);
//extern _C_ float Kameleon_get_missing_value(int id);
//extern _C_ void Kameleon_get_native_unit(int id, const char * variable, char * destbuffer);
extern _C_ void f_kameleon_create_c_string_(const char * t_string, char * destbuffer);
#endif/*end KAMELEON_F_H*/
