/*******************************************************************************
 *                                                                             *
 *       NAME:          f2c_gattribute_char_get.c                              *
 *                                                                             *
 *       VERSION:       1.0                                                    *
 *                                                                             *
 *       AUTHOR:        Marlo M. H. Maddox                                     *
 *                      NASA-GSFC-AETD-ISD-ADMA(587)-CCMC                      *
 *                      Marlo.Maddox@nasa.gov                                  *
 *                      (301) 286 - 5202                                       *
 *                                                                             *
 *       PURPOSE:       fortran wrapper to gattribute_get routine.  Extract    *
 *                      Global Attribute values from ccmc standardized cdf     *
 *                      file.                                                  *
 *                                                                             *
 *       INPUTS:        gattribute_name, string pointer to hold value          *
 *                                                                             *
 *       OUTPUTS:       string_length is returned and sting value is set inside*
 *                      function                                               *
 *                                                                             *
 *       FUNCTIONS:     ...                                                    *
 *                                                                             *
 *       VARIABLES:     ...                                                    *
 *                                                                             *
 *                                                                             *
 *       MODIFICATION                                                          *
 *       HISTORY:                                                              *
 *                                                                             *
 *       07.05.2005     Maddox, Marlo                                          *
 *                      initial development started                            *
 *       07.19.2005     Maddox, Marlo                                          *
 *                      Moved String Conversion Functionality to external      *
 *                      libinterp routine f2c_string_convert.c                 *
 *       09.11.2006     Maddox, Marlo                                          *
 *                      Added check and funtionality to see if attribute       *
 *                      exists, if not set value = NULL and return 0           *
 *                                                                             *
 *       08.28.2007     Maddox, Marlo                                          *
 *                      added -fno-second-underscore to Makefile to avoid      *
 *                      having to implement pre-processor macros to choose     *
 *                      the correct function name.                             *
 ******************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>

long f2c_gattribute_char_get_(
      char *gattr_name,
      char *gattr_value)
{

   extern void * gattribute_get(
         char *);
   extern char * f2c_string_convert(
         char *);

   long string_length;

   char *gattr_name_copy;

   char *char_attribute_valuePtr;

   gattr_name_copy = f2c_string_convert(gattr_name);

   char_attribute_valuePtr = gattribute_get(gattr_name_copy);

   if (char_attribute_valuePtr == NULL)
   {
      printf(
            "ERROR aquiring attribute value for --> %s <--... Returning 0\n",
            gattr_name_copy);
      gattr_value = NULL;
      return 0;
   }

   string_length = strlen(char_attribute_valuePtr);

   strcpy(
         gattr_value,
         char_attribute_valuePtr);

   /*** put blank at end of value string **/

   gattr_value[string_length] = ' ';

   /*
    printf("f2c_gattr_char_get.c\nDEBUG:\tgattr_value = ->%s<-\n", gattr_value );
    printf("f2c_gattr_char_get.c\nDEBUG:\tchar_attribute_valuePtr = ->%s<-\n", char_attribute_valuePtr );
    printf("f2c_gattr_char_get.c\nDEBUG:\treturning string_length = %d\n", string_length );
    */

   return (string_length );

}

