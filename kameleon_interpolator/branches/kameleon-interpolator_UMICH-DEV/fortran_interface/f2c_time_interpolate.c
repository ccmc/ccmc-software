/*******************************************************************************
 *                                                                             *
 *     NAME:         f2c_time_interpolate.c                                    *
 *                                                                             *
 *     VERSION:      1.0                                                       *
 *                                                                             *
 *     AUTHOR:       Marlo M. H. Maddox                                        *
 *                   NASA-GSFC-AETD-ISD-ADMA(587)-CCMC                         *
 *                   Marlo.Maddox@nasa.gov                                     *
 *                   (301) 286 - 5202                                          *
 *                                                                             *
 *     PURPOSE:      interpolation wrapper to interpolate data at a given      *
 *                   point for a given time. Uses tree data structure to find  *
 *                   files whose times are <= & >= given target time.  Values  *
 *                   for known times are extracted from data and target time   *
 *                   values are interpolated using knwon values.               *
 *                                                                             *
 *     INPUTS:       target_time, x, y, z, & values defined in calling fortran *
 *                   program ux,uy,uz,bx,by,bz                                 *
 *                                                                             *
 *     OUTPUTS:       ...                                                      *
 *                                                                             *
 *     FUNCTIONS:     ...                                                      *
 *                                                                             *
 *     VARIABLES:     ...                                                      *
 *                                                                             *
 *                                                                             *
 *     MODIFICATION                                                            *
 *     HISTORY:                                                                *
 *                                                                             *
 *     11.08.2004    Maddox, Marlo                                             *
 *                   initial release                                           *
 *                                                                             *
 *     04.25.2005    Maddox, Marlo                                             *
 *                   since the latest version of the ccmc_interpolater         *
 *                   contains the time_interpolation functionality             *
 *                   this file will just be a fortran interface to those       *
 *                   functions                                                 *
 *                                                                             *
 *     06.28.2005    Maddox, Marlo                                             *
 *                   Polishing code for bundle with regular interp lib         *
 *                                                                             *
 *     07.19.2005    Maddox, Marlo                                             *
 *                   oved String Conversion Functionality to external          *
 *                   libinterp routine f2c_string_convert.c                    *
 *                                                                             *
 *     08.28.2007    Maddox, Marlo                                             *
 *                   added -fno-second-underscore to Makefile to avoid         *
 *                   having to implement pre-processor macros to choose        *
 *                   the correct function name.                                *
 ******************************************************************************/

#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

long f2c_time_interpolate_(
      double *time,
      double *x,
      double *y,
      double *z,
      char *var_name,
      double *var_value)
{

   extern float time_interpolate(
         char *,
         double,
         float,
         float,
         float);
   extern char * f2c_string_convert(
         char *);

   char *var_name_copy;

   var_name_copy = f2c_string_convert(var_name);

   var_value[0] = time_interpolate(
         var_name_copy,
         time[0],
         x[0],
         y[0],
         z[0]);

   return (0);

}

