#ifndef DERIVED_COMPATIBILITY_H
#define DERIVED_COMPATIBILITY_H

#ifdef __cplusplus
extern "C"
{
    void ccmc_derived_close_cdf();
	void ccmc_derived_open_cdf_main_mem( char * filename, char * variable);
	//void ccmc_derived_open(char * filename);
	void ccmc_derived_open_cdf_main_mem_vector( char * filename, char * variable);
	float ccmc_derived_interpolate( char * variable, float x, float y, float z, int flag1, int flag2, ...);
	float ccmc_derived_gattribute_char_get_derived( char *);
	char * ccmc_derived_get_units(char * );
	char * ccmc_derived_getcurrenttime();
	float ccmc_derived_get_en(float x, float y, float z, int flag1, int flag2, ...);



	void open_cdf_derived_main_mem( char * filename, char * variable);
	void open_cdf_derived_main_mem_vector( char * filename, char * variable);
	float interpolate_derived( char * variable, float x, float y, float z, int flag1, int flag2, ...);
	float gattribute_char_get_derived( char *);



}
#endif

#ifndef __cplusplus
extern void ccmc_derived_close_cdf();
extern void ccmc_derived_open_cdf_main_mem( char * filename, char * variable);
extern void ccmc_derived_open_cdf_main_mem_vector( char * filename, char * variable);
extern float ccmc_derived_interpolate( char * variable, float x, float y, float z, int flag1, int flag2, ...);
extern float ccmc_derived_compute_magnitude(char *, float x, float y, float z, int flag1, int flag2, ...);
extern float ccmc_derived_compute_temp(float x, float y, float z, int flag1, int flag2, ...);
extern float ccmc_derived_compute_pram(float x, float y, float z, int flag1, int flag2, ...);
extern float ccmc_derived_compute_edotj(float x, float y, float z, int flag1, int flag2, ...);
extern float ccmc_derived_compute_ex(float x, float y, float z, int flag1, int flag2, ...);
extern float ccmc_derived_compute_ey(float x, float y, float z, int flag1, int flag2, ...);
extern float ccmc_derived_compute_ez(float x, float y, float z, int flag1, int flag2, ...);
extern float ccmc_derived_compute_jparx(float x, float y, float z, int flag1, int flag2, ...);
extern float ccmc_derived_compute_jpary(float x, float y, float z, int flag1, int flag2, ...);
extern float ccmc_derived_compute_jparz(float x, float y, float z, int flag1, int flag2, ...);
extern float ccmc_derived_compute_jpar(float x, float y, float z, int flag1, int flag2, ...);
extern float ccmc_derived_compute_jxbx(float x, float y, float z, int flag1, int flag2, ...);
extern float ccmc_derived_compute_jxby(float x, float y, float z, int flag1, int flag2, ...);
extern float ccmc_derived_compute_jxbz(float x, float y, float z, int flag1, int flag2, ...);
extern float ccmc_derived_compute_jxb(float x, float y, float z, int flag1, int flag2, ...);
extern float ccmc_derived_compute_exbx(float x, float y, float z, int flag1, int flag2, ...);
extern float ccmc_derived_compute_exby(float x, float y, float z, int flag1, int flag2, ...);
extern float ccmc_derived_compute_exbz(float x, float y, float z, int flag1, int flag2, ...);
extern float ccmc_derived_compute_exb(float x, float y, float z, int flag1, int flag2, ...);
extern float ccmc_derived_compute_s(float x, float y, float z, int flag1, int flag2, ...);
extern float ccmc_derived_compute_n(float x, float y, float z, int flag1, int flag2, ...);
extern float ccmc_derived_compute_nv(float x, float y, float z, int flag1, int flag2, ...);
extern float ccmc_derived_compute_nvx(float x, float y, float z, int flag1, int flag2, ...);
extern float ccmc_derived_compute_nvy(float x, float y, float z, int flag1, int flag2, ...);
extern float ccmc_derived_compute_nvz(float x, float y, float z, int flag1, int flag2, ...);
extern float ccmc_derived_compute_p(float x, float y, float z, int flag1, int flag2, ...);
extern float ccmc_derived_compute_beta(float x, float y, float z, int flag1, int flag2, ...);
extern char * ccmc_derived_get_units(char * );
extern char * ccmc_derived_getcurrenttime();
extern float ccmc_derived_get_en(float x, float y, float z, int flag1, int flag2, ...);
extern double parseEPOCH3(char *);
extern void encodeEPOCH3(double, char *);
extern void ccmc_derived_initializeExtraInformation();


extern void open_cdf_derived_main_mem( char * filename, char * variable);
extern void open_cdf_derived_main_mem_vector( char * filename, char * variable);
extern float interpolate_derived( char * variable, float x, float y, float z, int flag1, int flag2, ...);
extern float compute_magnitude(char *, float x, float y, float z, int flag1, int flag2, ...);
extern float compute_temp(float x, float y, float z, int flag1, int flag2, ...);
extern float compute_pram(float x, float y, float z, int flag1, int flag2, ...);
extern float compute_edotj(float x, float y, float z, int flag1, int flag2, ...);
extern float compute_eComponent1(float x, float y, float z, int flag1, int flag2, ...);
extern float compute_ey(float x, float y, float z, int flag1, int flag2, ...);
extern float compute_ez(float x, float y, float z, int flag1, int flag2, ...);
extern float compute_jparComponent1(float x, float y, float z, int flag1, int flag2, ...);
extern float compute_jpary(float x, float y, float z, int flag1, int flag2, ...);
extern float compute_jparz(float x, float y, float z, int flag1, int flag2, ...);
extern float compute_jpar(float x, float y, float z, int flag1, int flag2, ...);
extern float compute_jxbComponent1(float x, float y, float z, int flag1, int flag2, ...);
extern float compute_jxby(float x, float y, float z, int flag1, int flag2, ...);
extern float compute_jxbz(float x, float y, float z, int flag1, int flag2, ...);
extern float compute_jxb(float x, float y, float z, int flag1, int flag2, ...);
extern float compute_exbComponent1(float x, float y, float z, int flag1, int flag2, ...);
extern float compute_exby(float x, float y, float z, int flag1, int flag2, ...);
extern float compute_exbz(float x, float y, float z, int flag1, int flag2, ...);
extern float compute_exb(float x, float y, float z, int flag1, int flag2, ...);
extern float compute_s(float x, float y, float z, int flag1, int flag2, ...);
extern float compute_n(float x, float y, float z, int flag1, int flag2, ...);
extern float compute_nv(float x, float y, float z, int flag1, int flag2, ...);
extern float compute_nvComponent1(float x, float y, float z, int flag1, int flag2, ...);
extern float compute_nvy(float x, float y, float z, int flag1, int flag2, ...);
extern float compute_nvz(float x, float y, float z, int flag1, int flag2, ...);
extern float compute_p(float x, float y, float z, int flag1, int flag2, ...);
extern char * derived_get_units(char * );
extern char * getcurrenttime();
extern float get_en(float x, float y, float z, int flag1, int flag2, ...);
void initializeExtraInformation();


#endif

#endif//end compatibility
