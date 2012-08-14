#ifndef KAMELEON_C_H
#define KAMELEON_C_H

#ifdef __cplusplus
#define _C_ "C"
#endif

#ifndef __cplusplus
#define _C_
#endif

extern _C_ int Kameleon_create();
extern _C_ int Interpolator_create(int kid);
extern _C_ int Interpolator_delete(int id);
extern _C_ int Kameleon_open(int id, const char * filename);
extern _C_ void Kameleon_get_model_name(int id, char * model_name);
extern _C_ float Kameleon_interpolate(int id, const char * variable, const float * c0,
		const float * c1, const float *c2, float * dc0, float * dc1, float * dc2);
extern _C_ float Kameleon_interpolate_by_id(int id, int variable, const float * c0,
		const float * c1, const float *c2, float * dc0, float * dc1, float * dc2);
extern _C_ int Kameleon_load_vector_variable(int id, const char * variable);
extern _C_ int Kameleon_load_variable(int id, char * variable);
extern _C_ int Kameleon_unload_variable(int id, char * variable);
extern _C_ void Kameleon_get_global_attribute_string(int id, const char * gAttribute, char * destbuffer);
extern _C_ int Kameleon_close(int id);
extern _C_ int Kameleon_delete(int id);
extern _C_ float Kameleon_get_missing_value(int id);
extern _C_ void Kameleon_get_native_unit(int id, const char * variable, char * destbuffer);
extern _C_ void Kameleon_get_vis_unit(int id, const char * variable, char * destbuffer);
extern _C_ void Kameleon_create_c_string(const char * string, char * destbuffer);
extern _C_ int Kameleon_get_variable_id(int id, const char * variable);

#endif/*end KAMELEON_C_H*/
