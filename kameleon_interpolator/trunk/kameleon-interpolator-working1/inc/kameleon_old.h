/*******************************************************************************
 *                                                                             *
 *    NAME:          kameleon.h                                                *
 *                                                                             *
 *    VERSION:       1.0                                                       *
 *                                                                             *
 *    AUTHOR:        Marlo M. H. Maddox                                        *
 *                   NASA-GSFC-AETD-ISD-ADMA(587)-CCMC                         *
 *                   Marlo.Maddox@nasa.gov                                     *
 *                   (301) 286 - 5202                                          *
 *                                                                             *
 *    PURPOSE:       Definitions for KAMELEON INTERPOLATION Library Routines   *
 *                                                                             *
 *    INPUTS:                                                                  *
 *                                                                             *
 *    OUTPUTS:                                                                 *
 *                                                                             *
 *    FUNCTIONS:                                                               *
 *                                                                             *
 *    VARIABLES:                                                               *
 *                                                                             *
 *                                                                             *
 *    MODIFICATION                                                             *
 *    HISTORY:                                                                 *
 *                                                                             *
 *    11.09.2006     Maddox, Marlo                                             *
 *                   Initial dev                                               *
 *                                                                             *
 *    04.09.2008     Maddox, Marlo                                             *
 *                   added definition for is_var_in_memory(), and              *
 *                   interpolate_mas...                                        *
 *                                                                             *
 ******************************************************************************/

#ifndef KAMELEON_H_
#define KAMELEON_H_

#ifndef __cplusplus /* for regular c compiler, use these function prototypes */

extern void open_cdf(
      char *,
      int number_of_arguments,
      ...);
extern void * gattribute_get(
      char *);
extern void * vattribute_get(
      char *,
      char *);
extern char * gattribute_char_get(
      char *);
extern float gattribute_float_get(
      char *);
extern char * get_units(
      char *);
extern void close_cdf(
      void);
extern int attribute_exists(
      char *);
extern int var_exists(
      char *);

extern float interpolate_batsrus_cdf(
      char *,
      float,
      float,
      float,
      int,
      int,
      ...);
extern float interpolate_ctip_cdf(
      char *,
      float,
      float,
      float,
      int,
      int,
      ...);
extern float interpolate_enlil_cdf(
      char *,
      float,
      float,
      float,
      int,
      int,
      ...);

extern float interpolate_enlil_cdf_2(
      char *,
      float,
      float,
      float,
      int,
      int,
      ...);

extern float interpolate_ucla_ggcm_cdf(
      char *,
      float,
      float,
      float,
      int,
      int,
      ...);

extern float interpolate_batsrus_cdf(
      char *,
      float,
      float,
      float,
      int,
      int,
      ...);

extern void * var_get(
      char *,
      char *,
      int *);

extern long init_time(
      char *,
      double *,
      double *);

extern float time_interpolate(
      char *,
      double,
      float,
      float,
      float);
extern float interpolate_kpvt_cdf(
      char *,
      float,
      float,
      float,
      int,
      int,
      ...);
extern float interpolate_msfc_tvm_cdf(
      char *,
      float,
      float,
      float,
      int,
      int,
      ...);

int is_var_in_memory(
      char *);

float interpolate_mas_cdf(
      char *,
      float,
      float,
      float,
      int,
      int,
      ...);

#endif

#ifdef __cplusplus /*** for c++ compiler use these function prototypes **/
extern "C"
{
   void open_cdf( char *, int number_of_arguments, ... );
   void * gattribute_get( char *);
   void * vattribute_get( char *, char *);
   char * gattribute_char_get( char * );
   float gattribute_float_get( char * );
   char * get_units(char * );
   void close_cdf( void );
   int attribute_exists( char * );
   int var_exists( char * );
   float interpolate_batsrus_cdf( char *, float, float, float, int, int, ... );
   float interpolate_ctip_cdf( char *, float, float, float, int, int, ... );
   float interpolate_enlil_cdf( char *, float, float, float, int, int, ... );
   float interpolate_enlil_cdf_2( char *, float, float, float, int, int, ... );
   float interpolate_ucla_ggcm_cdf( char *, float, float, float, int, int, ... );
   float interpolate_batsrus_cdf( char *, float, float, float, int, int, ... );
   void * var_get( char *, char *, int * );
   long init_time(char *, double *, double *);
   float time_interpolate(char *, double , float , float , float );
   float interpolate_kpvt_cdf( char *, float, float, float, int, int, ... );
   float interpolate_msfc_tvm_cdf( char *, float, float, float, int, int, ... );
   int is_var_in_memory(char *);
   float interpolate_mas_cdf(
         char *,
         float,
         float,
         float,
         int,
         int,
         ...);
   CDFid get_current_CDFid();
}
#endif

#endif /*KAMELEON_H_*/

