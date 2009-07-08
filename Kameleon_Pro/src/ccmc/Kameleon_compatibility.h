#ifndef KAMELEON_COMPATIBILITY_H
#define KAMELEON_COMPATIBILITY_H

#ifdef __cplusplus
extern "C"
{
	int Kameleon_create(int id);
    int Kameleon_open(int id, const char * filename);
    int Kameleon_close(int id);
    int Kameleon_delete(int id);
}
#endif

#ifndef __cplusplus
extern int Kameleon_create(int id);
extern int Kameleon_open(int id, const char * filename);
extern int Kameleon_close(int id);
extern int Kameleon_delete(int id);
#endif

#endif//end KAMELEON_COMPATIBILITY_H
