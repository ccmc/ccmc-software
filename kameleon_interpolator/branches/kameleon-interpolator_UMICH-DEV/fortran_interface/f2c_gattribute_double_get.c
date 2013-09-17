/*******************************************************************************
 *                                                                             *
 *    NAME:          f2c_gattribute_double_get.c                                *
 *                                                                             *
 *    VERSION:       1.0                                                       *
 *                                                                             *
 *    AUTHOR:        Marlo M. H. Maddox                                        *
 *                   NASA-GSFC-AETD-ISD-ADMA(587)-CCMC                         *
 *                   Marlo.Maddox@nasa.gov                                     *
 *                   (301) 286 - 5202                                          *
 *                                                                             *
 *    PURPOSE:       fortran wrapper to gattribute_get routine.  Extract Global*
 *                   Attribute values of type double from ccmc standardized cdf *
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
 *    05.03.2011     Maddox, Marlo                                             *
 *                   adapted from f2c_gattribute_float_get.c
 ******************************************************************************/

#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

long f2c_gattribute_double_get_(
      char *gattr_name,
      double *gattr_double_value)
{

   extern void * gattribute_get(
         char *);
   extern char * f2c_string_convert(
         char *);

   char *gattr_name_copy;

   double *gattr_double_valuePtr;

   gattr_name_copy = f2c_string_convert(gattr_name);

   gattr_double_valuePtr = gattribute_get(gattr_name_copy);

   if (gattr_double_valuePtr == NULL)
   {
      printf(
            "ERROR aquiring attribute value for --> %s <--. setting value "
            "pointer = NULL & returning 0\n",
            gattr_name_copy);
      gattr_double_value = NULL;
/*      return 0;      */
      return -10;     /* pmn modification 9/28/2010 */
   }

   gattr_double_value[0] = *gattr_double_valuePtr;

   /*
    printf("f2c_gattr_double_get.c\nDEBUG:\tgattr_double_value = ->%f<-\n", *gattr_double_value );
    printf("f2c_gattr_double_get.c\nDEBUG:\tgattr_double_valuePtr = ->%f<-\n", *gattr_double_valuePtr );
    */

   return 0;

}

