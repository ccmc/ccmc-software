/*******************************************************************************
 *                                                                             *
 *    NAME:          f2c_init_time.c                                           *
 *                                                                             *
 *    VERSION:       1.0                                                       *
 *                                                                             *
 *    AUTHOR:        Marlo M. H. Maddox                                        *
 *                   NASA-GSFC-AETD-ISD-ADMA(587)-CCMC                         *
 *                   Marlo.Maddox@nasa.gov                                     *
 *                   (301) 286 - 5202                                          *
 *                                                                             *
 *    PURPOSE:       Initializes tree structure used by time_interpolate       *
 *                   routine.  A given output durectory is specified           *
 *                   containingCCMC statndardized CDF files converted using    *
 *                   the ccmc_converter tool.  Time and name information for   *
 *                   each CDF file is inserted into a tree structure that is   *
 *                   created in this program.  When subsequent calls to the    *
 *                   interpolate routine are made for a particular time,  the  *
 *                   data within the tree structure is used to find the two    *
 *                   files whose time steps are <= & >= the given target time  *
 *                   and ultimatley used in the time interpolation.            *
 *                                                                             *
 *    INPUTS:        char cdf_data_path - path of directory containing CDF data*
 *                   double start_time  - variable defined in calling fortran  *
 *                                        progran. Value will be set to        *
 *                                        earliest time in data range          *
 *                   double end_time    - variable defined in calling fortran  *
 *                                        progran.  Value will be set to latest*
 *                                        time in data range                   *
 *                                                                             *
 *    OUTPUTS:       ...                                                       *
 *                                                                             *
 *    FUNCTIONS:     ...                                                       *
 *                                                                             *
 *    VARIABLES:     ...                                                       *
 *                                                                             *
 *                                                                             *
 *    MODIFICATION                                                             *
 *    HISTORY:                                                                 *
 *                                                                             *
 *    11.08.2004     Maddox, Marlo                                             *
 *                   initial release                                           *
 *                                                                             *
 *    04.26.2005     Maddox, Marlo                                             *
 *                   since the latest version of the ccmc_interpolater         *
 *                   contains the time_interpolation functionality             *
 *                   this file will just be a fortran interface to those       *
 *                   functions                                                 *
 *                                                                             *
 *    06.20.2005     Maddox, Marlo                                             *
 *                   Generalizing for use in fortran libinterp interface       *
 *                                                                             *
 *    06.19.2005     Maddox, Marlo                                             *
 *                   Moved String Conversion Functionality to external         *
 *                   libinterp routine f2c_string_convert.c                    *
 *                                                                             *
 *    08.28.2007     Maddox, Marlo                                             *
 *                   added -fno-second-underscore to Makefile to avoid         *
 *                   having to implement pre-processor macros to choose        *
 *                   the correct function name.                                *
 *                                                                             *
 ******************************************************************************/

#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#define VERBOSE_FLAG     0

int f2c_init_time_(
      char *path,
      double *start_time,
      double *end_time)
{

   extern long init_time(
         char *,
         double *,
         double *);
   extern char * f2c_string_convert(
         char *);

   int return_status = 0;

   char *path_copy;

   path_copy = f2c_string_convert(path);

   /* call init_time library function to actually start reading contents of the directory */

   if ( VERBOSE_FLAG)
   {
      printf(
            "path = %s\npath_copy = %s\n",
            path,
            path_copy);
   }

   return_status = init_time(
         path_copy,
         start_time,
         end_time);

   return return_status;

}

