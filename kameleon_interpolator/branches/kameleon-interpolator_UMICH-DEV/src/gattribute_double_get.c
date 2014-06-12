/*******************************************************************************
 *                                                                             *
 *    NAME:          gattribute_double_get.c                                    *
 *                                                                             *
 *    VERSION:       1.0                                                       *
 *                                                                             *
 *    AUTHOR:        Marlo M. H. Maddox                                        *
 *                   NASA-GSFC-AETD-ISD-ADMA(587)-CCMC                         *
 *                   Marlo.Maddox@nasa.gov                                     *
 *                   (301) 286 - 5202                                          *
 *                                                                             *
 *    PURPOSE:       Extract a double  type global attribute value from        *
 *                   the currently open cdf file.    File must have been open  *
 *                   wiith the kameleon lib open_cdf routine                   *
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
 *    05.03.2011     Marlo Maddox                                              *
 *                   Adapting from gattribute_float_get.c                      *
 ******************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <math.h>
#include <stdarg.h>
#include "cdf_interface_variables.h"
#include "cdf.h"

double gattribute_double_get(
      char *attribute_name)
{

   CDFstatus status;

   long attrNum, data_type, numEntries, numElements;
   double attr_value;

   /*
    printf("DEBUG\tgetting number for %s global attribute\n", attribute_name );
    */

   /* get the CDF attribute number for attribute_name */

   status = CDFlib( GET_, ATTR_NUMBER_, attribute_name, &attrNum, NULL_);

   /** if we couldn't find the attribute number, return NULL to calling program **/

   if (status != CDF_OK)
   {
      StatusHandler(status);
      return cdf_missing_value;
   }

   /*printf("DEBUG\tattribute number for %s = %d\n", attribute_name, attrNum );*/

   /* get the number of gENTRIES for the current attribute */

   status = CDFlib( SELECT_, ATTR_, attrNum,
   GET_, ATTR_NUMgENTRIES_, &numEntries,
   GET_, gENTRY_DATATYPE_, &data_type,
   GET_, gENTRY_NUMELEMS_, &numElements,
   NULL_);
   if (status != CDF_OK)
      StatusHandler(status);

   /* get the data value for attribute_name */

   status = CDFlib( GET_, gENTRY_DATA_, &attr_value, NULL_);
   if (status != CDF_OK)
      StatusHandler(status);

   /*printf("DEBUG\tbefore returning, value = %f\n", attr_value );*/

   return attr_value;

}

