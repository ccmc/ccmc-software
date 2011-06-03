/*******************************************************************************
 *                                                                             *
 *       NAME:          f2c_interp_msfc_tvm_cdf.c                              *
 *                                                                             *
 *       VERSION:       1.0                                                    *
 *                                                                             *
 *       AUTHOR:        Marlo M. H. Maddox                                     *
 *                      NASA-GSFC-AETD-ISD-ADMA(587)-CCMC                      *
 *                      Marlo.Maddox@nasa.gov                                  *
 *                      (301) 286 - 5202                                       *
 *                                                                             *
 *       PURPOSE:       fortran wrapper to interpolate_msfc_tvm_cdf routine.   *
 *                                                                             *
 *       INPUTS:        ...                                                    *
 *                                                                             *
 *       OUTPUTS:       ...                                                    *
 *                                                                             *
 *       FUNCTIONS:     ...                                                    *
 *                                                                             *
 *       VARIABLES:     ...                                                    *
 *                                                                             *
 *                                                                             *
 *       MODIFICATION                                                          *
 *       HISTORY:                                                              *
 *                                                                             *
 *                                                                             *
 *       08.28.2007     Maddox, Marlo                                          *
 *                      added -fno-second-underscore to Makefile to avoid      *
 *                      having to implement pre-processor macros to choose     *
 *                      the correct function name.                             *
 ******************************************************************************/

#include <string.h>
#include <stdio.h>
#include <stdlib.h>

float f2c_interp_msfc_tvm_cdf_(
      double *x,
      double *y,
      double *z,
      double *var,
      char *var_name)
{

   extern float interpolate_msfc_tvm_cdf(
         char *,
         float,
         float,
         float,
         int,
         int,
         ...);
   extern char * f2c_string_convert(
         char *);

   char *var_name_copy;
   float interpolated_value, local_x, local_y, local_z;

   local_x = x[0];
   local_y = y[0];
   local_z = z[0];

   /*printf("%f %f %f\n", local_x, local_y, local_z );*/

   var_name_copy = f2c_string_convert(var_name);

   /*
    printf("var_name -->%s<--\n", var_name );
    printf("var_name_copy -->%s<--\n", var_name_copy );
    */

   interpolated_value = interpolate_msfc_tvm_cdf(
         var_name_copy,
         local_x,
         local_y,
         local_z,
         0,
         0);

   var[0] = interpolated_value;
   return 0;

}
