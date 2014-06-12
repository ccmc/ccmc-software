/*******************************************************************************
 *                                                                             *
 *    NAME:          f2c_open_cdf.c                                            *
 *                                                                             *
 *    VERSION:       1.0                                                       *
 *                                                                             *
 *    AUTHOR:        Marlo M. H. Maddox                                        *
 *                   NASA-GSFC-AETD-ISD-ADMA(587)-CCMC                         *
 *                   Marlo.Maddox@nasa.gov                                     *
 *                   (301) 286 - 5202                                          *
 *                                                                             *
 *    PURPOSE:       Fortran wrapper to open_cdf routine                       *
 *                                                                             *
 *    INPUTS:        Path/Filename String Generated In Fortran                 *
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
 *    07.19.2005     Maddox, Marlo                                             *
 *                   Moved String Conversion Functionality to external         *
 *                   libinterp routine f2c_string_convert.c                    *
 *    08.28.2007     Maddox, Marlo                                             *
 *                   added -fno-second-underscore to Makefile to avoid         *
 *                   having to implement pre-processor macros to choose        *
 *                   the correct function name.                                *
 ******************************************************************************/

#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <dirent.h>
#include <sys/param.h>

#define VERBOSE_FLAG     1

long f2c_open_cdf_(
      char *path)
{

   extern void open_cdf(
         char *,
         int,
         ...);
   extern char * f2c_string_convert(
         char *);

   int verbose_flag= VERBOSE_FLAG;

   char *path_copy;

   path_copy = f2c_string_convert(path);

   if (access(
         path_copy,
         R_OK) )
   {
      printf(
            "\nERROR: will not be able to read output from -->%s<--\n",
            path_copy);
      exit(1);
   }
   else
   {
      if (verbose_flag)
      {
         printf(
               "\ninput file %s READ OK...\n",
               path_copy);
      }
   }

   open_cdf(
         path_copy,
         0);

   return 0;

}

