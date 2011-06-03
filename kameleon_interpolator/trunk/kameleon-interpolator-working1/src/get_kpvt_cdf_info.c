/*******************************************************************************
 *                                                                             *
 *       NAME:          get_kpvt_cdf_info.c                                    *
 *                                                                             *
 *       VERSION:       1.0                                                    *
 *                                                                             *
 *       AUTHOR:        Marlo M. H. Maddox                                     *
 *                      NASA-GSFC-AETD-ISD-ADMA(587)-CCMC                      *
 *                      Marlo.Maddox@nasa.gov                                  *
 *                      (301) 286 - 5202                                       *
 *                                                                             *
 *       PURPOSE:       Extracts all relevant information for identifying      *
 *                      variables etc from the kpvt cdf file.  This routine is *
 *                      called by the open_cdf call.  Variable numbers are     *
 *                      extracted along with actual position values. If an     *
 *                      interpolate call is issued, the code will already have *
 *                      information in memory to make data access faster.      *
 *                                                                             *
 *       INPUTS:                                                               *
 *                                                                             *
 *       OUTPUTS:                                                              *
 *                                                                             *
 *       FUNCTIONS:                                                            *
 *                                                                             *
 *       VARIABLES:                                                            *
 *                                                                             *
 *                                                                             *
 *       MODIFICATION                                                          *
 *       HISTORY:                                                              *
 *                                                                             *
 *       11.30.2006     Maddox, Marlo                                          *
 *                      Initial development started                            *
 ******************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <math.h>
#include <stdarg.h>
#include "cdf_interface_variables.h"
#include "kpvt_cdf_interface_variables.h"
#include "cdf.h"

#define DEBUG_FLAG    0

void get_kpvt_cdf_info(
      int main_memory_flag)
{

   void allocate_defaults_kpvt(
         void);

   CDFstatus status;

   long recordStart = 0;
   long recordCount = 1;
   long recordInterval = 1;
   long counts[1] =
   { 0 };
   long intervals[1] =
   { 1 };
   long indices[1];

   int i;

   counts[0] = 1; /* set values */
   intervals[0] = 1;

   allocate_defaults_kpvt();

   /* get all of the cdf attribute/variable numbers that will be needed *******/

   if (CDFlib( CONFIRM_, zVAR_EXISTENCE_, x_name, NULL_) == CDF_OK)
   {
      status = CDFlib( GET_, zVAR_NUMBER_, x_name, &x_cdf_varNum, NULL_);
      if (status != CDF_OK)
         StatusHandler(status);
   }
   if (CDFlib( CONFIRM_, zVAR_EXISTENCE_, y_name, NULL_) == CDF_OK)
   {
      status = CDFlib( GET_, zVAR_NUMBER_, y_name, &y_cdf_varNum, NULL_);
      if (status != CDF_OK)
         StatusHandler(status);
   }
   if (CDFlib( CONFIRM_, zVAR_EXISTENCE_, net_flux_name, NULL_) == CDF_OK)
   {
      status
            = CDFlib( GET_, zVAR_NUMBER_, net_flux_name, &net_flux_cdf_varNum, NULL_);
      if (status != CDF_OK)
         StatusHandler(status);
   }
   if (CDFlib( CONFIRM_, zVAR_EXISTENCE_, total_flux_name, NULL_) == CDF_OK)
   {
      status
            = CDFlib( GET_, zVAR_NUMBER_, total_flux_name, &total_flux_cdf_varNum, NULL_);
      if (status != CDF_OK)
         StatusHandler(status);
   }
   if (CDFlib( CONFIRM_, zVAR_EXISTENCE_, weights_name, NULL_) == CDF_OK)
   {
      status
            = CDFlib( GET_, zVAR_NUMBER_, weights_name, &weights_cdf_varNum, NULL_);
      if (status != CDF_OK)
         StatusHandler(status);
   }

   /***  NOTE THESE ARE ATTRIBUTES SO USE CONFRIM ATTR_EXISTENCE... ***/

   if (CDFlib( CONFIRM_, ATTR_EXISTENCE_, NX_name, NULL_) == CDF_OK)
   {
      status = CDFlib( GET_, ATTR_NUMBER_, NX_name, &NX_cdf_num, NULL_);
      if (status != CDF_OK)
         StatusHandler(status);
   }
   if (CDFlib( CONFIRM_, ATTR_EXISTENCE_, NY_name, NULL_) == CDF_OK)
   {
      status = CDFlib( GET_, ATTR_NUMBER_, NY_name, &NY_cdf_num, NULL_);
      if (status != CDF_OK)
         StatusHandler(status);
   }

   if ( DEBUG_FLAG)
      printf("DEBUG\t finished getting cdf nums\n");

   /***************** use cdf_varNums to get actual scalar values *************/

   /********* get value for NX **********/
   status = CDFlib(
   SELECT_, ATTR_, NX_cdf_num,
   SELECT_, gENTRY_, 0,
   GET_, gENTRY_DATA_, &NX,
   NULL_);
   if (status != CDF_OK)
      StatusHandler(status);

   /********* get value for NY **********/
   status = CDFlib(
   SELECT_, ATTR_, NY_cdf_num,
   SELECT_, gENTRY_, 0,
   GET_, gENTRY_DATA_, &NY,
   NULL_);
   if (status != CDF_OK)
      StatusHandler(status);

   /*convert original float N* to int n* if required */
   /* bats NX was float but ucla, ctip, enlil, kpvt... are interger - no conversion needed here*/

   nx = NX;
   ny = NY;

   if ( DEBUG_FLAG)
   {
      printf("DEBUG\tINFO from get_kpvt_cdf_info\n");
      printf(
            "DEBUG\tNX=%d & nx=%d\n",
            NX,
            nx);
      printf(
            "DEBUG\tNY=%d & ny=%d\n",
            NY,
            ny);
      printf(
            "DEBUG\t main_memory_flag = %d\n",
            main_memory_flag);
   }

   /************************ allocate space for unique x & y positions ********/

   if ( (x_pos = ( float * ) calloc(
         nx,
         sizeof(float)) ) == NULL)
   {
      printf("\ncalloc failed for x_pos buffer!\n");
      exit( EXIT_FAILURE);
   }
   if ( (y_pos = ( float * ) calloc(
         ny,
         sizeof(float)) ) == NULL)
   {
      printf("\ncalloc failed for y_pos buffer!\n");
      exit( EXIT_FAILURE);
   }

   if ( DEBUG_FLAG)
      printf("DEBUG\tspace allocated for variables...\n");

   /***************************** get unique x,y  values **********************/

   counts[0] = nx;
   indices[0] = 0;
   intervals[0] = 1;

   status = CDFlib(
   SELECT_, zVAR_, x_cdf_varNum,
   SELECT_, zVAR_RECNUMBER_, recordStart,
   SELECT_, zVAR_RECCOUNT_, recordCount,
   SELECT_, zVAR_RECINTERVAL_, recordInterval,
   SELECT_, zVAR_DIMINDICES_, indices,
   SELECT_, zVAR_DIMCOUNTS_, counts,
   SELECT_, zVAR_DIMINTERVALS_, intervals,
   GET_, zVAR_HYPERDATA_, x_pos,
   NULL_);
   if (status != CDF_OK)
      StatusHandler(status);

   counts[0] = ny;

   status = CDFlib(
   SELECT_, zVAR_, y_cdf_varNum,
   SELECT_, zVAR_RECNUMBER_, recordStart,
   SELECT_, zVAR_RECCOUNT_, recordCount,
   SELECT_, zVAR_RECINTERVAL_, recordInterval,
   SELECT_, zVAR_DIMINDICES_, indices,
   SELECT_, zVAR_DIMCOUNTS_, counts,
   SELECT_, zVAR_DIMINTERVALS_, intervals,
   GET_, zVAR_HYPERDATA_, y_pos,
   NULL_);
   if (status != CDF_OK)
      StatusHandler(status);

   /****** more debug repoting ******/
   /** print first few values for position variables **/

   if ( DEBUG_FLAG)
   {
      for (i = 0; i < 30; i++)
      {
         printf(
               "INDEX-------------- %d\nx_pos[%d] = %f\ny_pos[%d] = %f\n\n",
               i,
               i,
               x_pos[i],
               i,
               y_pos[i]);
      }
   }
}

int load_kpvt_cdf_variable_into_main_memory(
      char *variable_to_read)
{

   CDFstatus status;

   long recordStart = 0;
   long recordCount = 1;
   long recordInterval = 1;
   long counts[1] =
   { 0 };
   long intervals[1] =
   { 1 };
   long indices[1];

   long variable_cdf_number;

   int kpvt_reserve_mem_and_set_cdf_num(
         char *);

   /*printf("DEBUG\tinside load\n");*/

   variable_cdf_number = kpvt_reserve_mem_and_set_cdf_num(variable_to_read);

   if (variable_cdf_number == -1)
   {
      printf(
            "ERROR:\tcould not find cdf variable number for -->%s<--\n",
            variable_to_read);
      return EXIT_FAILURE; /*exit( EXIT_FAILURE );*/
   }

   /*(printf("DEBUG\tback from reserve mem with cdf_varNum %d for variable %s \n", variable_cdf_number, variable_to_read );*/

   counts[0] = nx * ny;
   indices[0] = 0;
   intervals[0] = 1;

   if ( !strcmp(
         variable_to_read,
         net_flux_name) )
   {
      status = CDFlib(
      SELECT_, zVAR_, variable_cdf_number,
      SELECT_, zVAR_RECNUMBER_, recordStart,
      SELECT_, zVAR_RECCOUNT_, recordCount,
      SELECT_, zVAR_RECINTERVAL_, recordInterval,
      SELECT_, zVAR_DIMINDICES_, indices,
      SELECT_, zVAR_DIMCOUNTS_, counts,
      SELECT_, zVAR_DIMINTERVALS_, intervals,
      GET_, zVAR_HYPERDATA_, net_flux,
      NULL_);
      if (status != CDF_OK)
         StatusHandler(status);
   }
   else if ( !strcmp(
         variable_to_read,
         total_flux_name) )
   {
      status = CDFlib(
      SELECT_, zVAR_, variable_cdf_number,
      SELECT_, zVAR_RECNUMBER_, recordStart,
      SELECT_, zVAR_RECCOUNT_, recordCount,
      SELECT_, zVAR_RECINTERVAL_, recordInterval,
      SELECT_, zVAR_DIMINDICES_, indices,
      SELECT_, zVAR_DIMCOUNTS_, counts,
      SELECT_, zVAR_DIMINTERVALS_, intervals,
      GET_, zVAR_HYPERDATA_, total_flux,
      NULL_);
      if (status != CDF_OK)
         StatusHandler(status);
   }
   else if ( !strcmp(
         variable_to_read,
         weights_name) )
   {
      status = CDFlib(
      SELECT_, zVAR_, variable_cdf_number,
      SELECT_, zVAR_RECNUMBER_, recordStart,
      SELECT_, zVAR_RECCOUNT_, recordCount,
      SELECT_, zVAR_RECINTERVAL_, recordInterval,
      SELECT_, zVAR_DIMINDICES_, indices,
      SELECT_, zVAR_DIMCOUNTS_, counts,
      SELECT_, zVAR_DIMINTERVALS_, intervals,
      GET_, zVAR_HYPERDATA_, weights,
      NULL_);
      if (status != CDF_OK)
         StatusHandler(status);
   }
   else
   {
      /* INSERT ERROR HANDLER HERE */
      printf("couldn't find a variable to load...\n");
      return EXIT_FAILURE;
   }

   /*printf("DEBUG\tfinished hyper read returning\n");*/

   return EXIT_SUCCESS;

}

int kpvt_reserve_mem_and_set_cdf_num(
      char *variable_number_to_get)
{
   int buffer_size = nx * ny;

   /*printf("DEBUG\tinside resrve mem\n");*/

   /* return cdf variable Number for given variable */

   if ( !strcmp(
         variable_number_to_get,
         net_flux_name) )
   {
      if ( (net_flux = ( float * ) calloc(
            buffer_size,
            sizeof(float)) ) == NULL)
      {
         printf(
               "\ncalloc failed for %s buffer!\n",
               net_flux_name);
         exit( EXIT_FAILURE);
      }
      return net_flux_cdf_varNum;
   }
   else if ( !strcmp(
         variable_number_to_get,
         total_flux_name) )
   {
      if ( (total_flux = ( float * ) calloc(
            buffer_size,
            sizeof(float)) ) == NULL)
      {
         printf(
               "\ncalloc failed for %s buffer!\n",
               total_flux_name);
         exit( EXIT_FAILURE);
      }
      return total_flux_cdf_varNum;
   }
   else if ( !strcmp(
         variable_number_to_get,
         weights_name) )
   {
      if ( (weights = ( float * ) calloc(
            buffer_size,
            sizeof(float)) ) == NULL)
      {
         printf(
               "\ncalloc failed for %s buffer!\n",
               weights_name);
         exit( EXIT_FAILURE);
      }
      return weights_cdf_varNum;
   }
   else
   {
      return -1;
   }
   /*else { printf("ERROR:\tcould not find cdf variable number for %s\n", variable ); return 0; }*/

}

void allocate_defaults_kpvt(
      void)
{
   /************ we need to allocate space for all varoiables even when no main mem option is set
    * this allows the all inclusing close to work properly as it tries to free all variables.  We only need
    * to hold a enough space for a single value ******/

   if ( (net_flux = ( float * ) calloc(
         1,
         sizeof(float)) ) == NULL)
   {
      printf(
            "\nallocate_defaults_kpvt calloc failed for %s buffer!\n",
            net_flux_name);
      exit( EXIT_FAILURE);
   }
   if ( (total_flux = ( float * ) calloc(
         1,
         sizeof(float)) ) == NULL)
   {
      printf(
            "\nallocate_defaults_kpvt calloc failed for %s buffer!\n",
            total_flux_name);
      exit( EXIT_FAILURE);
   }
   if ( (weights = ( float * ) calloc(
         1,
         sizeof(float)) ) == NULL)
   {
      printf(
            "\nallocate_defaults_kpvt calloc failed for %s buffer!\n",
            weights_name);
      exit( EXIT_FAILURE);
   }

   return;
}

