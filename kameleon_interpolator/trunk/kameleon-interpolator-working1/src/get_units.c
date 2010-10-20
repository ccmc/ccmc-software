/*******************************************************************************
 *                                                                             *
 *       NAME:          get_units.c                                            *
 *                                                                             *
 *       VERSION:       1.0                                                    *
 *                                                                             *
 *       AUTHOR:        Marlo M. H. Maddox                                     *
 *                      NASA-GSFC-AETD-ISD-ADMA(587)-CCMC                      *
 *                      Marlo.Maddox@nasa.gov                                  *
 *                      (301) 286 - 5202                                       *
 *                                                                             *
 *       PURPOSE:       Extract a char type ( string ) variable unit attribute *
 *                      value from the currently open cdf file and specified   *
 *                      variable. File must have been open wiith the kameleon  *
 *                      lib open_cdf routine                                   *
 *                                                                             *
 *       INPUTS:        (1)  variable name                                     *
 *                                                                             *
 *       OUTPUTS:       unit varibale attribute value                          *
 *                                                                             *
 *       FUNCTIONS:                                                            *
 *                                                                             *
 *       VARIABLES:                                                            *
 *                                                                             *
 *                                                                             *
 *       MODIFICATION                                                          *
 *       HISTORY:                                                              *
 *                                                                             *
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

char * get_units(
      char *variable_name)
{

   CDFstatus status;

   long var_attrNum, varNum, data_type, num_elements;
   char *units;

   /*printf("DEBUG inside get_units\n");

    printf("CDFattrNum call\n");*/

   /*var_attrNum = CDFattrNum( id, "units" );*/

   status = CDFlib( GET_, ATTR_NUMBER_, "units", &var_attrNum, NULL_);

   /** if we couldn't find the attribute number, return NULL to calling program **/

   if (status != CDF_OK)
   {
      StatusHandler(status);
      return NULL;
   }

   /*printf("var_attrNum = %d\n", var_attrNum );

    printf("CDFvarNum call for %s \n", variable_name );*/

   /*varNum = CDFvarNum( id, variable_name );*/

   if (CDFlib( CONFIRM_, zVAR_EXISTENCE_, variable_name, NULL_) == CDF_OK)
   {
      status = CDFlib( GET_, zVAR_NUMBER_, variable_name, &varNum, NULL_);
      if (status != CDF_OK)
         StatusHandler(status);
   }
   else
   {
      printf(
            "%s - ERROR:\t Variable %s does NOT exist in this CDF file...Returning NULL value for units...\n",
            __FILE__, variable_name );
            return NULL;
         }

         /*printf("varNum = %d\n", varNum );

         printf("CDFattr data type call\n");*/

         status = CDFlib (
               SELECT_, ATTR_, var_attrNum,
               SELECT_, zENTRY_NAME_, variable_name,
               GET_, zENTRY_DATATYPE_, &data_type,
               NULL_
         );
         if(status != CDF_OK) StatusHandler (status);

         /*printf("CDFattr num elements call\n");*/

         status = CDFlib (
               SELECT_, ATTR_, var_attrNum,
               SELECT_, zENTRY_NAME_, variable_name,
               GET_, zENTRY_NUMELEMS_, &num_elements,
               NULL_
         );
         if(status != CDF_OK) StatusHandler (status);

         /*printf("DEBUG\tnum_elements = %d\n", num_elements );*/

         units = ( char * ) malloc( num_elements + 1 );

         status = CDFlib (
               SELECT_, ATTR_, var_attrNum,
               SELECT_, zENTRY_NAME_, variable_name,
               GET_, zENTRY_DATA_, units,
               NULL_
         );
         if(status != CDF_OK) StatusHandler (status);

         /*printf("DEBUT\tunits should be loaded\n");*/

         units[num_elements] = '\0';

         /*printf("DEBUG\t %s has units %s\n", variable_name, units );*/

         return units;

      }

