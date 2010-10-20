/*******************************************************************************
 *                                                                             *
 *    NAME:           f2c_vattribute_float_get.c                               *
 *                                                                             *
 *    VERSION:        1.0                                                      *
 *                                                                             *
 *    AUTHOR:         Marlo M. H. Maddox                                       *
 *                    NASA-GSFC-AETD-ISD-ADMA(587)-CCMC                        *
 *                    Marlo.Maddox@nasa.gov                                    *
 *                    (301) 286 - 5202                                         *
 *                                                                             *
 *    PURPOSE:       Fortran wrapper to vattribute_get routine.  Extract       *
 *                   specific variable Attribute values of type float from     *
 *                   ccmc standardized cdf file.                               *
 *                                                                             *
 *    INPUTS:        vattribute_name, string pointer to hold value             *
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
 *                                                                             *
 *    07.19.2005     Maddox, Marlo                                             *
 *                   Moved String Conversion Functionality to external         *
 *                   libinterp routine f2c_string_convert.c                    *
 *                                                                             *
 *   09.11.2006      Maddox, Marlo                                             *
 *                   Added check and funtionality to see if attribute          *
 *                   exists, if not set value = NULL and return 0              *
 *                                                                             *
 *   08.28.2007      Maddox, Marlo                                             *
 *                   added -fno-second-underscore to Makefile to avoid         *
 *                   having to implement pre-processor macros to choose        *
 *                   the correct function name.                                *
 *                                                                             *
 ******************************************************************************/

#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

long f2c_vattribute_float_get_(
      char *variable_name,
      char *vattr_name,
      float *vattr_float_value)
{

   extern void * vattribute_get(
         char *,
         char *);
   extern char * f2c_string_convert(
         char *);

   char *variable_name_copy;
   char *variable_name_copy_local_storage;
   char *vattr_name_copy;

   float *vattr_float_valuePtr;

   /*** format variable_name first ***/

   variable_name_copy = f2c_string_convert(variable_name);

   /** since we need to make another call to f2c_string_convert, we have to store the contents
    of variable_name_copy locally in this function.  If we didn't the next call would overwite
    the memory location that the first pointer pointed too ...
    **/

   variable_name_copy_local_storage
         = ( char * ) malloc(strlen(variable_name_copy) + 1);

   if (variable_name_copy_local_storage == NULL)
   {
      printf("ERROR:\tCouldn't allocate memory for string proper fortran to C "
            "string conversion in f2c_vattribute_char_get\n");
      return (-5L);
   }

   strcpy(
         variable_name_copy_local_storage,
         variable_name_copy);

   /*** format vattr_name second ***/

   vattr_name_copy = f2c_string_convert(vattr_name);

   vattr_float_valuePtr = vattribute_get(
         variable_name_copy_local_storage,
         vattr_name_copy);

   /*** lets immediatley clean up our malloced local memory ***/

   free(variable_name_copy_local_storage);

   if (vattr_float_valuePtr == NULL)
   {
      printf(
            "ERROR aquiring attribute value for --> %s <--. setting value "
            "pointer = NULL & returning 0\n",
            vattr_name_copy);
      vattr_float_value = NULL;
      return 0;
   }

   vattr_float_value[0] = *vattr_float_valuePtr;

   /*
    printf("f2c_vattr_float_get.c\nDEBUG:\tvattr_float_value = ->%f<-\n", *vattr_float_value );
    printf("f2c_vattr_float_get.c\nDEBUG:\tvattr_float_valuePtr = ->%f<-\n", *vattr_float_valuePtr );
    */

   return 0;

}

