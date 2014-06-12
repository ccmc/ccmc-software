/*******************************************************************************
 *                                                                             *
 *    NAME:       vattribute_get.c                                             *
 *                                                                             *
 *    VERSION:    1.0                                                          *
 *                                                                             *
 *    AUTHOR:     Marlo M. H. Maddox                                           *
 *                NASA-GSFC-AETD-ISD-ADMA(587)-CCMC                            *
 *                Marlo.Maddox@nasa.gov                                        *
 *                (301) 286 - 5202                                             *
 *                                                                             *
 *    PURPOSE:    Extract a variable attribute value from the currently open   *
 *                cdf file. File must have been open wiith the kameleon lib    *
 *                open_cdf routine.                                            *
 *                                                                             *
 *    INPUTS:     (1)  variable name (2) variable attribute name               *
 *                                                                             *
 *    OUTPUTS:    variable attribute value                                     *
 *                                                                             *
 *    FUNCTIONS:                                                               *
 *                                                                             *
 *    VARIABLES:                                                               *
 *                                                                             *
 *                                                                             *
 *    MODIFICATION                                                             *
 *    HISTORY:                                                                 *
 *                                                                             *
 *    11.22.2005  Marlo Maddox                                                 *
 *                Changed cdf arguments from int -> long                       *
 *                                                                             *
 *    07.18.2006  Marlo Maddox                                                 *
 *                If the attribute number could not be found, print            *
 *                warning and return NULL                                      *
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

void * vattribute_get(
      char *variable_name,
      char *variable_attr_name)
{

   CDFstatus status;

   long var_attrNum, varNum, data_type, num_elements;
   static void *value;
   static float tmp_float_value;
   static int tmp_int_value;

   status
         = CDFlib( GET_, ATTR_NUMBER_, variable_attr_name, &var_attrNum, NULL_);

   /** if we couldn't find the attribute number, return NULL to calling program **/

   if (status != CDF_OK)
   {
      StatusHandler(status);
      return NULL;
   }

   status = CDFlib( GET_, zVAR_NUMBER_, variable_name, &varNum, NULL_);

   /** if we couldn't find the variable number, return NULL to calling program **/

   if (status != CDF_OK)
   {
      StatusHandler(status);
      return NULL;
   }

   status = CDFlib( SELECT_, ATTR_, var_attrNum,
   SELECT_, zENTRY_NAME_, variable_name,
   GET_, zENTRY_DATATYPE_, &data_type,
   NULL_);

   if (status != CDF_OK)
      StatusHandler(status);

   status = CDFlib(
   SELECT_, ATTR_, var_attrNum,
   SELECT_, zENTRY_NAME_, variable_name,
   GET_, zENTRY_NUMELEMS_, &num_elements,
   NULL_);

   if (status != CDF_OK)
      StatusHandler(status);

   if (data_type == CDF_CHAR)
   {

      value = ( char * ) malloc(num_elements + 1);

      status = CDFlib(
      SELECT_, ATTR_, var_attrNum,
      SELECT_, zENTRY_NAME_, variable_name,
      GET_, zENTRY_DATA_, value,
      NULL_);
      if (status != CDF_OK)
         StatusHandler(status);

      ((char *) value)[num_elements] = '\0';

      return value;

   }
   else if (data_type == CDF_INT4)
   {

      status = CDFlib(
      SELECT_, ATTR_, var_attrNum,
      SELECT_, zENTRY_NAME_, variable_name,
      GET_, zENTRY_DATA_, &tmp_int_value,
      NULL_);

      if (status != CDF_OK)
         StatusHandler(status);

      return &tmp_int_value;

   }
   else if (data_type == CDF_FLOAT)
   {

      status = CDFlib(
      SELECT_, ATTR_, var_attrNum,
      SELECT_, zENTRY_NAME_, variable_name,
      GET_, zENTRY_DATA_, &tmp_float_value,
      NULL_);

      if (status != CDF_OK)
         StatusHandler(status);

      return &tmp_float_value;

   }
   else
   {

      /*** code will NOT get here unitl we capture CDF's error of NO_SUCH_ATTR:..., and return control instead of dying ***/

      printf(
            "ERROR:\tCouldn't determine data type for %s\n",
            variable_name);
      return " ";
   }

   /*** program should never reach here ***/

}

