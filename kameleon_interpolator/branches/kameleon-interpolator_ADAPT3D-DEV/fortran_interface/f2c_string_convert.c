/*******************************************************************************
 *                                                                             *
 *    NAME:          f2c_string_convert.c                                      *
 *                                                                             *
 *    VERSION:       1.0                                                       *
 *                                                                             *
 *    AUTHOR:        Marlo M. H. Maddox                                        *
 *                   NASA-GSFC-AETD-ISD-ADMA(587)-CCMC                         *
 *                   Marlo.Maddox@nasa.gov                                     *
 *                   (301) 286 - 5202                                          *
 *                                                                             *
 *    PURPOSE:       Convert "trailing blank" fortran string to usefull "null  *
 *                   terminated" trailing '\0' C string.                       *
 *                                                                             *
 *    INPUTS:        String Generated In Fortran                               *
 *                                                                             *
 *    OUTPUTS:       Pointer to C formated Fortran String                      *
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
 *                   Moved string stuff from all f2c_ routines to this         *
 *                   central location                                          *
 *                                                                             *
 ******************************************************************************/

#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#define VERBOSE_FLAG     0
#define MAX_STRING_LEN   5000

/*** NOTE TO SELF:  Malloc fortran_string_copy would be an option but would require proper cleanup from external program ***/

char * f2c_string_convert(
      char *fortran_string)
{

   static char fortran_string_copy[ MAX_STRING_LEN + 1 ];
   char *blank_pos, *char_ptr;
   int string_length;

   /** Lutz string conversion fortran -> c Modified by Marlo Maddox ***********/

   blank_pos=strstr(
         fortran_string,
         " ");

   if (blank_pos == NULL)
   {
      printf(
            "f2c_string_convert.c\tERROR:\tSupplied Fortran String does not "
            "contain a trailing blank character\nSupplied Fortran String ---->%s<----\n",
            fortran_string);
      return ( NULL );
   }

   string_length = blank_pos - fortran_string;

   if (string_length > MAX_STRING_LEN + 1)
   {
      printf(
            "f2c_string_convert.c\tERROR:\tSupplied Fortran String Length: %d, "
            "exceeds this functions available space of %d\n",
            string_length,
            MAX_STRING_LEN);
      printf("Modify MAX_STRING_LEN macro in f2c_string_convert.c and re-make "
            "interpolation library...\n");
      return ( NULL ); /* string too long as determined by C */
   }

   char_ptr = strncpy(
         fortran_string_copy,
         fortran_string,
         string_length);

   if (char_ptr == NULL)
   {
      printf(
            "ERROR:\tAn error occured during fortran to c conversion operation "
            "in f2c_string_convert.c.  Supplied Fortran String -->%s<-- was "
            "not converted properly\n",
            fortran_string);
      return ( NULL );
   }

   strcpy(
         fortran_string_copy + string_length,
         "\0");

   return fortran_string_copy;

}

