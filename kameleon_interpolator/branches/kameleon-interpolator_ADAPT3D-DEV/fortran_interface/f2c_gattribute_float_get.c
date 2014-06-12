/*******************************************************************************
 *                                                                             *
 *    NAME:          f2c_gattribute_float_get.c                                *
 *                                                                             *
 *    VERSION:       1.0                                                       *
 *                                                                             *
 *    AUTHOR:        Marlo M. H. Maddox                                        *
 *                   NASA-GSFC-AETD-ISD-ADMA(587)-CCMC                         *
 *                   Marlo.Maddox@nasa.gov                                     *
 *                   (301) 286 - 5202                                          *
 *                                                                             *
 *    PURPOSE:       fortran wrapper to gattribute_get routine.  Extract Global*
 *                   Attribute values of type float from ccmc standardized cdf *
 *                   file.                                                     *
 *                                                                             *
 *    INPUTS:        gattribute_name, string pointer to hold value             *
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
 *    07.14.2005     Maddox, Marlo                                             *
 *                   initial development started                               *
 *    07.19.2005     Maddox, Marlo                                             *
 *                   Moved String Conversion Functionality to external         *
 *                   libinterp routine f2c_string_convert.c                    *
 *    09.11.2006     Maddox, Marlo                                             *
 *                   Added check and funtionality to see if attribute          *
 *                   exists, if not set value = NULL and return 0              *
 *                                                                             *
 *    08.28.2007     Maddox, Marlo                                             *
 *                   added -fno-second-underscore to Makefile to avoid         *
 *                   having to implement pre-processor macros to choose        *
 *                   the correct function name.                                *
 ******************************************************************************/

#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

long f2c_gattribute_float_get_(
      char *gattr_name,
      float *gattr_float_value)
{

   extern void * gattribute_get(
         char *);
   extern char * f2c_string_convert(
         char *);

   char *gattr_name_copy;

   float *gattr_float_valuePtr;

   gattr_name_copy = f2c_string_convert(gattr_name);

   gattr_float_valuePtr = gattribute_get(gattr_name_copy);

   if (gattr_float_valuePtr == NULL)
   {
      printf(
            "ERROR aquiring attribute value for --> %s <--. setting value "
            "pointer = NULL & returning 0\n",
            gattr_name_copy);
      gattr_float_value = NULL;
/*      return 0;      */
      return -10;     /* pmn modification 9/28/2010 */
   }

   gattr_float_value[0] = *gattr_float_valuePtr;

   /*
    printf("f2c_gattr_float_get.c\nDEBUG:\tgattr_float_value = ->%f<-\n", *gattr_float_value );
    printf("f2c_gattr_float_get.c\nDEBUG:\tgattr_float_valuePtr = ->%f<-\n", *gattr_float_valuePtr );
    */

   return 0;

}

