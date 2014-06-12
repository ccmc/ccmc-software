/*******************************************************************************
 *                                                                             *
 *       NAME:          gattribute_get.c                                       *
 *                                                                             *
 *       VERSION:       1.0                                                    *
 *                                                                             *
 *       AUTHOR:        Marlo M. H. Maddox                                     *
 *                      NASA-GSFC-AETD-ISD-ADMA(587)-CCMC                      *
 *                      Marlo.Maddox@nasa.gov                                  *
 *                      (301) 286 - 5202                                       *
 *                                                                             *
 *       PURPOSE:       Extract a global attribute value from the currently    *
 *                      open cdf file. File must have been open wiith the      *
 *                      kameleon lib open_cdf routine                          *
 *                                                                             *
 *       INPUTS:        (1)  global attribute name                             *
 *                                                                             *
 *       OUTPUTS:       global attribute value                                 *
 *                                                                             *
 *       FUNCTIONS:                                                            *
 *                                                                             *
 *       VARIABLES:                                                            *
 *                                                                             *
 * *                                                                           *
 *       MODIFICATION                                                          *
 *       HISTORY:                                                              *
 *                                                                             *
 *       11.22.2005     Marlo Maddox                                           *
 *                      Changed cdf arguments from int -> long                 *
 *                                                                             *
 *       07.18.2006     Marlo Maddox                                           *
 *                      If the attribute number could not be found, print      *
 *                      warning and return NULL                                *
 *                                                                             *
 *       11.09.2006     Marlo Maddox                                           *
 *                      If the attribute data type could not be found, print   *
 *                      warning and return NULL                                *
 *                                                                             *
 *       01.31.2007     Marlo Maddox                                           *
 *                      Adding double support                                  *
 *                                                                             *
 ******************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <math.h>
#include <stdarg.h>
#include "cdf_interface_variables.h"
#include "cdf.h"

void * gattribute_get(
      char *attribute_name)
{

   CDFstatus status;

   long attrNum, data_type, numEntries, numElements;
   static char *tmp_char_value;
   static float tmp_float_value;
   static int tmp_int_value;
   static double tmp_double_value;

   status = CDFlib( GET_, ATTR_NUMBER_, attribute_name, &attrNum, NULL_);

   /** if we couldn't find the attribute number, return NULL to calling program **/

   if (status != CDF_OK)
   {
      StatusHandler(status);
      return NULL;
   }

   status = CDFlib( SELECT_, ATTR_, attrNum,
   GET_, ATTR_NUMgENTRIES_, &numEntries,
   GET_, gENTRY_DATATYPE_, &data_type,
   GET_, gENTRY_NUMELEMS_, &numElements,
   NULL_);
   if (status != CDF_OK)
      StatusHandler(status);

   if (data_type == CDF_CHAR)
   {

      tmp_char_value = ( char * ) malloc(numElements + 1);

      status = CDFlib( GET_, gENTRY_DATA_, tmp_char_value, NULL_);
      if (status != CDF_OK)
         StatusHandler(status);

      tmp_char_value[numElements] = '\0';

      return tmp_char_value;

   }
   else if (data_type == CDF_INT4)
   {

      status = CDFlib( GET_, gENTRY_DATA_, &tmp_int_value, NULL_);
      if (status != CDF_OK)
         StatusHandler(status);

      return &tmp_int_value;

   }
   else if (data_type == CDF_FLOAT)
   {

      status = CDFlib( GET_, gENTRY_DATA_, &tmp_float_value, NULL_);
      if (status != CDF_OK)
         StatusHandler(status);

      return &tmp_float_value;

   }
   else if (data_type == CDF_DOUBLE)
   {

      status = CDFlib( GET_, gENTRY_DATA_, &tmp_double_value, NULL_);
      if (status != CDF_OK)
         StatusHandler(status);

      return &tmp_double_value;

   }
   else
   {

      /*** code will NOT get here unitl we capture CDF's error of NO_SUCH_ATTR:..., and return control instead of dying ***/

      printf(
            "ERROR:\tCouldn't determine data type for %s\n",
            attribute_name);
      return NULL;
   }

   /*** program should never reach here ***/

}

