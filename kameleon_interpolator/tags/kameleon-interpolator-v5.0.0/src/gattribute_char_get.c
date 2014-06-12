/*******************************************************************************
 *                                                                             *
 *    NAME:          gattribute_char_get.c                                     *
 *                                                                             *
 *    VERSION:       1.0                                                       *
 *                                                                             *
 *    AUTHOR:        Marlo M. H. Maddox                                        *
 *                   NASA-GSFC-AETD-ISD-ADMA(587)-CCMC                         *
 *                   Marlo.Maddox@nasa.gov                                     *
 *                   (301) 286 - 5202                                          *
 *                                                                             *
 *    PURPOSE:       Extract a char type( string ) global attribute value from *
 *                   the currently open cdf file. File must have been open     *
 *                   with the kameleon lib open_cdf routine                    *
 *                                                                             *
 *    INPUTS:        (1)  global attribute name                                *
 *                                                                             *
 *    OUTPUTS:       global attribute value                                    *
 *                                                                             *
 *    FUNCTIONS:                                                               *
 *                                                                             *
 *    VARIABLES:                                                               *
 *                                                                             *
 *                                                                             *
 *    MODIFICATION                                                             *
 *    HISTORY:                                                                 *
 *                                                                             *
 *    11.22.2005     Marlo Maddox                                              *
 *                   Changed cdf arguments from int -> long                    *
 *    07.18.2006     Marlo Maddox                                              *
 *                   If the attribute number could not be found, print         *
 *                   warning and return NULL                                   *
 *                                                                             *
 ******************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <math.h>
#include <stdarg.h>
#include "cdf_interface_variables.h"
#include "cdf.h"

char * gattribute_char_get(
      char *attribute_name)
{

   CDFstatus status;

   long attrNum, data_type, numEntries, numElements;
   static char *attribute_value;
   /*
    printf("DEBUG\tgetting number for %s global attribute\n", attribute_name );
    */

   /* get the CDF attribute number for attribute_name */

   status = CDFlib( GET_, ATTR_NUMBER_, attribute_name, &attrNum, NULL_);

   /** if we couldn't find the attribute number, return NULL to calling program **/

   if (status != CDF_OK)
   {
      StatusHandler(status);
      return NULL;
   }

   /*
    printf("DEBUG\tattribute number for %s = %d\n", attribute_name, attrNum );
    */
   /* get the number og gENTRIES for the current attribute */

   status = CDFlib(
   SELECT_, ATTR_, attrNum,
   GET_, ATTR_NUMgENTRIES_, &numEntries,
   GET_, gENTRY_DATATYPE_, &data_type,
   GET_, gENTRY_NUMELEMS_, &numElements,
   NULL_);
   if (status != CDF_OK)
      StatusHandler(status);

   /*attribute_value = ( char * ) malloc( numElements + 1 );*/
   attribute_value = ( char * ) malloc(numElements + 1);

   /* get the data value for attribute_name */

   status = CDFlib( GET_, gENTRY_DATA_, attribute_value, NULL_);
   if (status != CDF_OK)
      StatusHandler(status);

   /*** why did I comment this? - uncomented 03.21.2005 ***/
   attribute_value[numElements] = '\0';

   return attribute_value;

}

