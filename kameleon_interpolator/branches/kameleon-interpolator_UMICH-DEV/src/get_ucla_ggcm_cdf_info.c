/*******************************************************************************
 *                                                                             *
 *       NAME:          get_ucla_ggcm_cdf_info.c                               *
 *                                                                             *
 *       VERSION:       1.0                                                    *
 *                                                                             *
 *       AUTHOR:        Marlo M. H. Maddox                                     *
 *                      NASA-GSFC-AETD-ISD-ADMA(587)-CCMC                      *
 *                      Marlo.Maddox@nasa.gov                                  *
 *                      (301) 286 - 5202                                       *
 *                                                                             *
 *       PURPOSE:       Extracts all relevant information for identifying      *
 *                      variables etc from the ucla_ggcm cdf file.  This       *
 *                      routine is called by the open_cdf call Variable numbers*
 *                      are extracted along with actual position values. If an *
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
 *       06.07.2006     Maddox, Marlo                                          *
 *                      added dynamic memory allocation for all variables      *
 *                      regardless of main mem is selected or not. If no       *
 *                      main mem is selected, space is reserved for only       *
 *                      one value for specified type.  This allows the         *
 *                      all-inclusive close_ucla_ggcm_cdf to work properly     *
 *                      calling free for all valid variables.                  *
 *                      This was done with the addition of the function        *
 *                      allocate_defaults() along with proper allocates for    *
 *                      other related variables                                *
 *                                                                             *
 *        10.04.2006    Maddox, Marlo                                          *
 *                      Changed function type for                              *
 *                      load_*_cdf_variable_into_main_.. from void to int      *
 *                                                                             *
 *                      also checking return status from *reserve_mem_and_set  *
 *                      function...returning wit error if var not found        *
 *                                                                             *
 *        07.29.2008    Maddox, Marlo                                          *
 *                      Adding eta variable for ucla_ggcm/open_ggcm            *
 ******************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <math.h>
#include <stdarg.h>
#include "cdf_interface_variables.h"
#include "ucla_ggcm_cdf_interface_variables.h"
#include "cdf.h"

void get_ucla_ggcm_cdf_info(
      int main_memory_flag)
{

   void allocate_defaults_b(
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

   counts[0] = 1; /* set values */
   intervals[0] = 1;

   allocate_defaults_b();

   /******************** get all of the cdf attributes/variable numbers that will be needed ******************/

   status = CDFlib( GET_, zVAR_NUMBER_, rho_name, &rho_cdfNum, NULL_);
   if (status != CDF_OK)
      StatusHandler(status);

   status = CDFlib( GET_, zVAR_NUMBER_, p_name, &p_cdfNum, NULL_);
   if (status != CDF_OK)
      StatusHandler(status);

   status = CDFlib( GET_, zVAR_NUMBER_, jx_name, &jx_cdfNum, NULL_);
   if (status != CDF_OK)
      StatusHandler(status);

   status = CDFlib( GET_, zVAR_NUMBER_, jy_name, &jy_cdfNum, NULL_);
   if (status != CDF_OK)
      StatusHandler(status);

   status = CDFlib( GET_, zVAR_NUMBER_, jz_name, &jz_cdfNum, NULL_);
   if (status != CDF_OK)
      StatusHandler(status);

   status = CDFlib( GET_, zVAR_NUMBER_, ux_name, &ux_cdfNum, NULL_);
   if (status != CDF_OK)
      StatusHandler(status);

   status = CDFlib( GET_, zVAR_NUMBER_, uy_name, &uy_cdfNum, NULL_);
   if (status != CDF_OK)
      StatusHandler(status);

   status = CDFlib( GET_, zVAR_NUMBER_, uz_name, &uz_cdfNum, NULL_);
   if (status != CDF_OK)
      StatusHandler(status);

   status = CDFlib( GET_, zVAR_NUMBER_, bx_name, &bx_cdfNum, NULL_);
   if (status != CDF_OK)
      StatusHandler(status);

   status = CDFlib( GET_, zVAR_NUMBER_, by_name, &by_cdfNum, NULL_);
   if (status != CDF_OK)
      StatusHandler(status);

   status = CDFlib( GET_, zVAR_NUMBER_, bz_name, &bz_cdfNum, NULL_);
   if (status != CDF_OK)
      StatusHandler(status);

   status = CDFlib( GET_, zVAR_NUMBER_, bx1_name, &bx1_cdfNum, NULL_);
   if (status != CDF_OK)
      StatusHandler(status);

   status = CDFlib( GET_, zVAR_NUMBER_, by1_name, &by1_cdfNum, NULL_);
   if (status != CDF_OK)
      StatusHandler(status);

   status = CDFlib( GET_, zVAR_NUMBER_, bz1_name, &bz1_cdfNum, NULL_);
   if (status != CDF_OK)
      StatusHandler(status);

   status = CDFlib( GET_, zVAR_NUMBER_, x_name, &x_cdf_varNum, NULL_);
   if (status != CDF_OK)
      StatusHandler(status);

   status = CDFlib( GET_, zVAR_NUMBER_, y_name, &y_cdf_varNum, NULL_);
   if (status != CDF_OK)
      StatusHandler(status);

   status = CDFlib( GET_, zVAR_NUMBER_, z_name, &z_cdf_varNum, NULL_);
   if (status != CDF_OK)
      StatusHandler(status);

   status = CDFlib( GET_, zVAR_NUMBER_, x_bx_name, &x_bx_cdf_varNum, NULL_);
   if (status != CDF_OK)
      StatusHandler(status);

   status = CDFlib( GET_, zVAR_NUMBER_, y_bx_name, &y_bx_cdf_varNum, NULL_);
   if (status != CDF_OK)
      StatusHandler(status);

   status = CDFlib( GET_, zVAR_NUMBER_, z_bx_name, &z_bx_cdf_varNum, NULL_);
   if (status != CDF_OK)
      StatusHandler(status);

   status = CDFlib( GET_, zVAR_NUMBER_, x_by_name, &x_by_cdf_varNum, NULL_);
   if (status != CDF_OK)
      StatusHandler(status);

   status = CDFlib( GET_, zVAR_NUMBER_, y_by_name, &y_by_cdf_varNum, NULL_);
   if (status != CDF_OK)
      StatusHandler(status);

   status = CDFlib( GET_, zVAR_NUMBER_, z_by_name, &z_by_cdf_varNum, NULL_);
   if (status != CDF_OK)
      StatusHandler(status);

   status = CDFlib( GET_, zVAR_NUMBER_, x_bz_name, &x_bz_cdf_varNum, NULL_);
   if (status != CDF_OK)
      StatusHandler(status);

   status = CDFlib( GET_, zVAR_NUMBER_, y_bz_name, &y_bz_cdf_varNum, NULL_);
   if (status != CDF_OK)
      StatusHandler(status);

   status = CDFlib( GET_, zVAR_NUMBER_, z_bz_name, &z_bz_cdf_varNum, NULL_);
   if (status != CDF_OK)
      StatusHandler(status);

   status = CDFlib( GET_, ATTR_NUMBER_, NX_name, &NX_cdf_num, NULL_);
   if (status != CDF_OK)
      StatusHandler(status);

   status = CDFlib( GET_, ATTR_NUMBER_, NY_name, &NY_cdf_num, NULL_);
   if (status != CDF_OK)
      StatusHandler(status);

   status = CDFlib( GET_, ATTR_NUMBER_, NZ_name, &NZ_cdf_num, NULL_);
   if (status != CDF_OK)
      StatusHandler(status);

   status = CDFlib( GET_, zVAR_NUMBER_, eta_name, &eta_cdfNum, NULL_);
   if (status != CDF_OK)
      StatusHandler(status);

   /*printf("DEBUG\t finished getting cdf nums\n");*/

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

   /********* get value for NZ **********/
   status = CDFlib(
   SELECT_, ATTR_, NZ_cdf_num,
   SELECT_, gENTRY_, 0,
   GET_, gENTRY_DATA_, &NZ,
   NULL_);
   if (status != CDF_OK)
      StatusHandler(status);

   /*convert original float N* to int n* */
   /* bats NX was float but ucla is int no conversion needed fix latter*/

   nx = NX;
   ny = NY;
   nz = NZ;

   /*printf("DEBUG\tNX[%d] -> nx[%d]\n", NX, nx );
    printf("DEBUG\tNY[%d] -> ny[%d]\n", NY, ny );
    printf("DEBUG\tNZ[%d] -> nz[%d]\n", NZ, nz );

    printf("DEBUG\t main_memory_flag = %d\n", main_memory_flag);*/

   /********************* allocate space for unique x, y, & z positions *******/
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
   if ( (z_pos = ( float * ) calloc(
         nz,
         sizeof(float)) ) == NULL)
   {
      printf("\ncalloc failed for x_pos buffer!\n");
      exit( EXIT_FAILURE);
   }

   /************ allocate space for unique x_bx, y_bx, & z_bx positions *******/
   if ( (x_bx_pos = ( float * ) calloc(
         nx,
         sizeof(float)) ) == NULL)
   {
      printf("\ncalloc failed for x_bx_pos buffer!\n");
      exit( EXIT_FAILURE);
   }
   if ( (y_bx_pos = ( float * ) calloc(
         ny,
         sizeof(float)) ) == NULL)
   {
      printf("\ncalloc failed for y_bx_pos buffer!\n");
      exit( EXIT_FAILURE);
   }
   if ( (z_bx_pos = ( float * ) calloc(
         nz,
         sizeof(float)) ) == NULL)
   {
      printf("\ncalloc failed for x_bx_pos buffer!\n");
      exit( EXIT_FAILURE);
   }

   /************ allocate space for unique x_by, y_by, & z_by positions *******/
   if ( (x_by_pos = ( float * ) calloc(
         nx,
         sizeof(float)) ) == NULL)
   {
      printf("\ncalloc failed for x_by_pos buffer!\n");
      exit( EXIT_FAILURE);
   }
   if ( (y_by_pos = ( float * ) calloc(
         ny,
         sizeof(float)) ) == NULL)
   {
      printf("\ncalloc failed for y_by_pos buffer!\n");
      exit( EXIT_FAILURE);
   }
   if ( (z_by_pos = ( float * ) calloc(
         nz,
         sizeof(float)) ) == NULL)
   {
      printf("\ncalloc failed for x_by_pos buffer!\n");
      exit( EXIT_FAILURE);
   }

   /************ allocate space for unique x_bz, y_bz, & z_bz positions *******/
   if ( (x_bz_pos = ( float * ) calloc(
         nx,
         sizeof(float)) ) == NULL)
   {
      printf("\ncalloc failed for x_bz_pos buffer!\n");
      exit( EXIT_FAILURE);
   }
   if ( (y_bz_pos = ( float * ) calloc(
         ny,
         sizeof(float)) ) == NULL)
   {
      printf("\ncalloc failed for y_bz_pos buffer!\n");
      exit( EXIT_FAILURE);
   }
   if ( (z_bz_pos = ( float * ) calloc(
         nz,
         sizeof(float)) ) == NULL)
   {
      printf("\ncalloc failed for x_bz_pos buffer!\n");
      exit( EXIT_FAILURE);
   }

   /*printf("DEBUG\tspace allocated for variables...\n");    */

   /******************** get unique x,y,z values ******************************/

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

   counts[0] = nz;

   status = CDFlib(
   SELECT_, zVAR_, z_cdf_varNum,
   SELECT_, zVAR_RECNUMBER_, recordStart,
   SELECT_, zVAR_RECCOUNT_, recordCount,
   SELECT_, zVAR_RECINTERVAL_, recordInterval,
   SELECT_, zVAR_DIMINDICES_, indices,
   SELECT_, zVAR_DIMCOUNTS_, counts,
   SELECT_, zVAR_DIMINTERVALS_, intervals,
   GET_, zVAR_HYPERDATA_, z_pos,
   NULL_);
   if (status != CDF_OK)
      StatusHandler(status);

   /*printf("DEBUG\tunique x,y,z values read in successfully\n");*/

   /********* get unique x_bx, y_bx, z_bx values ******************************/

   counts[0] = nx;

   status = CDFlib(
   SELECT_, zVAR_, x_bx_cdf_varNum,
   SELECT_, zVAR_RECNUMBER_, recordStart,
   SELECT_, zVAR_RECCOUNT_, recordCount,
   SELECT_, zVAR_RECINTERVAL_, recordInterval,
   SELECT_, zVAR_DIMINDICES_, indices,
   SELECT_, zVAR_DIMCOUNTS_, counts,
   SELECT_, zVAR_DIMINTERVALS_, intervals,
   GET_, zVAR_HYPERDATA_, x_bx_pos,
   NULL_);
   if (status != CDF_OK)
      StatusHandler(status);

   counts[0] = ny;

   status = CDFlib(
   SELECT_, zVAR_, y_bx_cdf_varNum,
   SELECT_, zVAR_RECNUMBER_, recordStart,
   SELECT_, zVAR_RECCOUNT_, recordCount,
   SELECT_, zVAR_RECINTERVAL_, recordInterval,
   SELECT_, zVAR_DIMINDICES_, indices,
   SELECT_, zVAR_DIMCOUNTS_, counts,
   SELECT_, zVAR_DIMINTERVALS_, intervals,
   GET_, zVAR_HYPERDATA_, y_bx_pos,
   NULL_);
   if (status != CDF_OK)
      StatusHandler(status);

   counts[0] = nz;

   status = CDFlib(
   SELECT_, zVAR_, z_bx_cdf_varNum,
   SELECT_, zVAR_RECNUMBER_, recordStart,
   SELECT_, zVAR_RECCOUNT_, recordCount,
   SELECT_, zVAR_RECINTERVAL_, recordInterval,
   SELECT_, zVAR_DIMINDICES_, indices,
   SELECT_, zVAR_DIMCOUNTS_, counts,
   SELECT_, zVAR_DIMINTERVALS_, intervals,
   GET_, zVAR_HYPERDATA_, z_bx_pos,
   NULL_);
   if (status != CDF_OK)
      StatusHandler(status);

   /************** get unique x_by, y_by, z_by values *************************/

   counts[0] = nx;

   status = CDFlib(
   SELECT_, zVAR_, x_by_cdf_varNum,
   SELECT_, zVAR_RECNUMBER_, recordStart,
   SELECT_, zVAR_RECCOUNT_, recordCount,
   SELECT_, zVAR_RECINTERVAL_, recordInterval,
   SELECT_, zVAR_DIMINDICES_, indices,
   SELECT_, zVAR_DIMCOUNTS_, counts,
   SELECT_, zVAR_DIMINTERVALS_, intervals,
   GET_, zVAR_HYPERDATA_, x_by_pos,
   NULL_);
   if (status != CDF_OK)
      StatusHandler(status);

   counts[0] = ny;

   status = CDFlib(
   SELECT_, zVAR_, y_by_cdf_varNum,
   SELECT_, zVAR_RECNUMBER_, recordStart,
   SELECT_, zVAR_RECCOUNT_, recordCount,
   SELECT_, zVAR_RECINTERVAL_, recordInterval,
   SELECT_, zVAR_DIMINDICES_, indices,
   SELECT_, zVAR_DIMCOUNTS_, counts,
   SELECT_, zVAR_DIMINTERVALS_, intervals,
   GET_, zVAR_HYPERDATA_, y_by_pos,
   NULL_);
   if (status != CDF_OK)
      StatusHandler(status);

   counts[0] = nz;

   status = CDFlib(
   SELECT_, zVAR_, z_by_cdf_varNum,
   SELECT_, zVAR_RECNUMBER_, recordStart,
   SELECT_, zVAR_RECCOUNT_, recordCount,
   SELECT_, zVAR_RECINTERVAL_, recordInterval,
   SELECT_, zVAR_DIMINDICES_, indices,
   SELECT_, zVAR_DIMCOUNTS_, counts,
   SELECT_, zVAR_DIMINTERVALS_, intervals,
   GET_, zVAR_HYPERDATA_, z_by_pos,
   NULL_);
   if (status != CDF_OK)
      StatusHandler(status);

   /************************* get unique x_bz, y_bz, z_bz values **************/

   counts[0] = nx;

   status = CDFlib(
   SELECT_, zVAR_, x_bz_cdf_varNum,
   SELECT_, zVAR_RECNUMBER_, recordStart,
   SELECT_, zVAR_RECCOUNT_, recordCount,
   SELECT_, zVAR_RECINTERVAL_, recordInterval,
   SELECT_, zVAR_DIMINDICES_, indices,
   SELECT_, zVAR_DIMCOUNTS_, counts,
   SELECT_, zVAR_DIMINTERVALS_, intervals,
   GET_, zVAR_HYPERDATA_, x_bz_pos,
   NULL_);
   if (status != CDF_OK)
      StatusHandler(status);

   counts[0] = ny;

   status = CDFlib(
   SELECT_, zVAR_, y_bz_cdf_varNum,
   SELECT_, zVAR_RECNUMBER_, recordStart,
   SELECT_, zVAR_RECCOUNT_, recordCount,
   SELECT_, zVAR_RECINTERVAL_, recordInterval,
   SELECT_, zVAR_DIMINDICES_, indices,
   SELECT_, zVAR_DIMCOUNTS_, counts,
   SELECT_, zVAR_DIMINTERVALS_, intervals,
   GET_, zVAR_HYPERDATA_, y_bz_pos,
   NULL_);
   if (status != CDF_OK)
      StatusHandler(status);

   counts[0] = nz;

   status = CDFlib(
   SELECT_, zVAR_, z_bz_cdf_varNum,
   SELECT_, zVAR_RECNUMBER_, recordStart,
   SELECT_, zVAR_RECCOUNT_, recordCount,
   SELECT_, zVAR_RECINTERVAL_, recordInterval,
   SELECT_, zVAR_DIMINDICES_, indices,
   SELECT_, zVAR_DIMCOUNTS_, counts,
   SELECT_, zVAR_DIMINTERVALS_, intervals,
   GET_, zVAR_HYPERDATA_, z_bz_pos,
   NULL_);
   if (status != CDF_OK)
      StatusHandler(status);

}

int load_ucla_ggcm_cdf_variable_into_main_memory(
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

   long ucla_ggcm_reserve_mem_and_set_cdf_num(
         char *);

   /*printf("DEBUG\tinside load\n");*/

   variable_cdf_number
         = ucla_ggcm_reserve_mem_and_set_cdf_num(variable_to_read);

   /* instead of EXITING PROGRAM with exit failure - print warning and return control to calling function */

   if (variable_cdf_number == -1)
   {
      printf(
            "ERROR:\tcould not find cdf variable number for -->%s<--\n",
            variable_to_read);
      return EXIT_FAILURE; /*exit( EXIT_FAILURE );*/
   }

   /*(printf("DEBUG\tback from reserve mem with cdfnum %d for variable %s \n", variable_cdf_number, variable_to_read );*/

   counts[0] = nx * ny * nz;
   indices[0] = 0;
   intervals[0] = 1;

   if ( !strcmp(
         variable_to_read,
         bx_name) )
   {
      status = CDFlib(
      SELECT_, zVAR_, variable_cdf_number,
      SELECT_, zVAR_RECNUMBER_, recordStart,
      SELECT_, zVAR_RECCOUNT_, recordCount,
      SELECT_, zVAR_RECINTERVAL_, recordInterval,
      SELECT_, zVAR_DIMINDICES_, indices,
      SELECT_, zVAR_DIMCOUNTS_, counts,
      SELECT_, zVAR_DIMINTERVALS_, intervals,
      GET_, zVAR_HYPERDATA_, bx,
      NULL_);
      if (status != CDF_OK)
         StatusHandler(status);
   }
   else if ( !strcmp(
         variable_to_read,
         by_name) )
   {
      status = CDFlib(
      SELECT_, zVAR_, variable_cdf_number,
      SELECT_, zVAR_RECNUMBER_, recordStart,
      SELECT_, zVAR_RECCOUNT_, recordCount,
      SELECT_, zVAR_RECINTERVAL_, recordInterval,
      SELECT_, zVAR_DIMINDICES_, indices,
      SELECT_, zVAR_DIMCOUNTS_, counts,
      SELECT_, zVAR_DIMINTERVALS_, intervals,
      GET_, zVAR_HYPERDATA_, by,
      NULL_);
      if (status != CDF_OK)
         StatusHandler(status);
   }
   else if ( !strcmp(
         variable_to_read,
         bz_name) )
   {
      status = CDFlib(
      SELECT_, zVAR_, variable_cdf_number,
      SELECT_, zVAR_RECNUMBER_, recordStart,
      SELECT_, zVAR_RECCOUNT_, recordCount,
      SELECT_, zVAR_RECINTERVAL_, recordInterval,
      SELECT_, zVAR_DIMINDICES_, indices,
      SELECT_, zVAR_DIMCOUNTS_, counts,
      SELECT_, zVAR_DIMINTERVALS_, intervals,
      GET_, zVAR_HYPERDATA_, bz,
      NULL_);
      if (status != CDF_OK)
         StatusHandler(status);
   }
   else if ( !strcmp(
         variable_to_read,
         bx1_name) )
   {
      status = CDFlib(
      SELECT_, zVAR_, variable_cdf_number,
      SELECT_, zVAR_RECNUMBER_, recordStart,
      SELECT_, zVAR_RECCOUNT_, recordCount,
      SELECT_, zVAR_RECINTERVAL_, recordInterval,
      SELECT_, zVAR_DIMINDICES_, indices,
      SELECT_, zVAR_DIMCOUNTS_, counts,
      SELECT_, zVAR_DIMINTERVALS_, intervals,
      GET_, zVAR_HYPERDATA_, bx1,
      NULL_);
      if (status != CDF_OK)
         StatusHandler(status);
   }
   else if ( !strcmp(
         variable_to_read,
         by1_name) )
   {
      status = CDFlib(
      SELECT_, zVAR_, variable_cdf_number,
      SELECT_, zVAR_RECNUMBER_, recordStart,
      SELECT_, zVAR_RECCOUNT_, recordCount,
      SELECT_, zVAR_RECINTERVAL_, recordInterval,
      SELECT_, zVAR_DIMINDICES_, indices,
      SELECT_, zVAR_DIMCOUNTS_, counts,
      SELECT_, zVAR_DIMINTERVALS_, intervals,
      GET_, zVAR_HYPERDATA_, by1,
      NULL_);
      if (status != CDF_OK)
         StatusHandler(status);
   }
   else if ( !strcmp(
         variable_to_read,
         bz1_name) )
   {
      status = CDFlib(
      SELECT_, zVAR_, variable_cdf_number,
      SELECT_, zVAR_RECNUMBER_, recordStart,
      SELECT_, zVAR_RECCOUNT_, recordCount,
      SELECT_, zVAR_RECINTERVAL_, recordInterval,
      SELECT_, zVAR_DIMINDICES_, indices,
      SELECT_, zVAR_DIMCOUNTS_, counts,
      SELECT_, zVAR_DIMINTERVALS_, intervals,
      GET_, zVAR_HYPERDATA_, bz1,
      NULL_);
      if (status != CDF_OK)
         StatusHandler(status);
   }
   else if ( !strcmp(
         variable_to_read,
         ux_name) )
   {
      status = CDFlib(
      SELECT_, zVAR_, variable_cdf_number,
      SELECT_, zVAR_RECNUMBER_, recordStart,
      SELECT_, zVAR_RECCOUNT_, recordCount,
      SELECT_, zVAR_RECINTERVAL_, recordInterval,
      SELECT_, zVAR_DIMINDICES_, indices,
      SELECT_, zVAR_DIMCOUNTS_, counts,
      SELECT_, zVAR_DIMINTERVALS_, intervals,
      GET_, zVAR_HYPERDATA_, ux,
      NULL_);
      if (status != CDF_OK)
         StatusHandler(status);
   }
   else if ( !strcmp(
         variable_to_read,
         uy_name) )
   {
      status = CDFlib(
      SELECT_, zVAR_, variable_cdf_number,
      SELECT_, zVAR_RECNUMBER_, recordStart,
      SELECT_, zVAR_RECCOUNT_, recordCount,
      SELECT_, zVAR_RECINTERVAL_, recordInterval,
      SELECT_, zVAR_DIMINDICES_, indices,
      SELECT_, zVAR_DIMCOUNTS_, counts,
      SELECT_, zVAR_DIMINTERVALS_, intervals,
      GET_, zVAR_HYPERDATA_, uy,
      NULL_);
      if (status != CDF_OK)
         StatusHandler(status);
   }
   else if ( !strcmp(
         variable_to_read,
         uz_name) )
   {
      status = CDFlib(
      SELECT_, zVAR_, variable_cdf_number,
      SELECT_, zVAR_RECNUMBER_, recordStart,
      SELECT_, zVAR_RECCOUNT_, recordCount,
      SELECT_, zVAR_RECINTERVAL_, recordInterval,
      SELECT_, zVAR_DIMINDICES_, indices,
      SELECT_, zVAR_DIMCOUNTS_, counts,
      SELECT_, zVAR_DIMINTERVALS_, intervals,
      GET_, zVAR_HYPERDATA_, uz,
      NULL_);
      if (status != CDF_OK)
         StatusHandler(status);
   }
   else if ( !strcmp(
         variable_to_read,
         jx_name) )
   {
      status = CDFlib(
      SELECT_, zVAR_, variable_cdf_number,
      SELECT_, zVAR_RECNUMBER_, recordStart,
      SELECT_, zVAR_RECCOUNT_, recordCount,
      SELECT_, zVAR_RECINTERVAL_, recordInterval,
      SELECT_, zVAR_DIMINDICES_, indices,
      SELECT_, zVAR_DIMCOUNTS_, counts,
      SELECT_, zVAR_DIMINTERVALS_, intervals,
      GET_, zVAR_HYPERDATA_, jx,
      NULL_);
      if (status != CDF_OK)
         StatusHandler(status);
   }
   else if ( !strcmp(
         variable_to_read,
         jy_name) )
   {
      status = CDFlib(
      SELECT_, zVAR_, variable_cdf_number,
      SELECT_, zVAR_RECNUMBER_, recordStart,
      SELECT_, zVAR_RECCOUNT_, recordCount,
      SELECT_, zVAR_RECINTERVAL_, recordInterval,
      SELECT_, zVAR_DIMINDICES_, indices,
      SELECT_, zVAR_DIMCOUNTS_, counts,
      SELECT_, zVAR_DIMINTERVALS_, intervals,
      GET_, zVAR_HYPERDATA_, jy,
      NULL_);
      if (status != CDF_OK)
         StatusHandler(status);
   }
   else if ( !strcmp(
         variable_to_read,
         jz_name) )
   {
      status = CDFlib(
      SELECT_, zVAR_, variable_cdf_number,
      SELECT_, zVAR_RECNUMBER_, recordStart,
      SELECT_, zVAR_RECCOUNT_, recordCount,
      SELECT_, zVAR_RECINTERVAL_, recordInterval,
      SELECT_, zVAR_DIMINDICES_, indices,
      SELECT_, zVAR_DIMCOUNTS_, counts,
      SELECT_, zVAR_DIMINTERVALS_, intervals,
      GET_, zVAR_HYPERDATA_, jz,
      NULL_);
      if (status != CDF_OK)
         StatusHandler(status);
   }
   else if ( !strcmp(
         variable_to_read,
         rho_name) )
   {
      status = CDFlib(
      SELECT_, zVAR_, variable_cdf_number,
      SELECT_, zVAR_RECNUMBER_, recordStart,
      SELECT_, zVAR_RECCOUNT_, recordCount,
      SELECT_, zVAR_RECINTERVAL_, recordInterval,
      SELECT_, zVAR_DIMINDICES_, indices,
      SELECT_, zVAR_DIMCOUNTS_, counts,
      SELECT_, zVAR_DIMINTERVALS_, intervals,
      GET_, zVAR_HYPERDATA_, rho,
      NULL_);
      if (status != CDF_OK)
         StatusHandler(status);
   }
   else if ( !strcmp(
         variable_to_read,
         p_name) )
   {
      status = CDFlib(
      SELECT_, zVAR_, variable_cdf_number,
      SELECT_, zVAR_RECNUMBER_, recordStart,
      SELECT_, zVAR_RECCOUNT_, recordCount,
      SELECT_, zVAR_RECINTERVAL_, recordInterval,
      SELECT_, zVAR_DIMINDICES_, indices,
      SELECT_, zVAR_DIMCOUNTS_, counts,
      SELECT_, zVAR_DIMINTERVALS_, intervals,
      GET_, zVAR_HYPERDATA_, p,
      NULL_);
      if (status != CDF_OK)
         StatusHandler(status);
   }
   else if ( !strcmp(
            variable_to_read,
            eta_name) )
      {
         status = CDFlib(
         SELECT_, zVAR_, variable_cdf_number,
         SELECT_, zVAR_RECNUMBER_, recordStart,
         SELECT_, zVAR_RECCOUNT_, recordCount,
         SELECT_, zVAR_RECINTERVAL_, recordInterval,
         SELECT_, zVAR_DIMINDICES_, indices,
         SELECT_, zVAR_DIMCOUNTS_, counts,
         SELECT_, zVAR_DIMINTERVALS_, intervals,
         GET_, zVAR_HYPERDATA_, eta,
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

long ucla_ggcm_reserve_mem_and_set_cdf_num(
      char *variable_number_to_get)
{

   int buffer_size = nx * ny * nz;

   /*printf("DEBUG\tinside resrve mem\n");*/

   /* return cdf variable Number for given variable */

   if ( !strcmp(
         variable_number_to_get,
         bx_name) )
   {
      if ( (bx = ( float * ) calloc(
            buffer_size,
            sizeof(float)) ) == NULL)
      {
         printf(
               "\ncalloc failed for %s buffer!\n",
               bx_name);
         exit( EXIT_FAILURE);
      }
      return bx_cdfNum;
   }
   else if ( !strcmp(
         variable_number_to_get,
         by_name) )
   {
      if ( (by = ( float * ) calloc(
            buffer_size,
            sizeof(float)) ) == NULL)
      {
         printf(
               "\ncalloc failed for %s buffer!\n",
               by_name);
         exit( EXIT_FAILURE);
      }
      return by_cdfNum;
   }
   else if ( !strcmp(
         variable_number_to_get,
         bz_name) )
   {
      if ( (bz = ( float * ) calloc(
            buffer_size,
            sizeof(float)) ) == NULL)
      {
         printf(
               "\ncalloc failed for %s buffer!\n",
               bz_name);
         exit( EXIT_FAILURE);
      }
      return bz_cdfNum;
   }
   else if ( !strcmp(
         variable_number_to_get,
         bx1_name) )
   {
      if ( (bx1 = ( float * ) calloc(
            buffer_size,
            sizeof(float)) ) == NULL)
      {
         printf(
               "\ncalloc failed for %s buffer!\n",
               bx1_name);
         exit( EXIT_FAILURE);
      }
      return bx1_cdfNum;
   }
   else if ( !strcmp(
         variable_number_to_get,
         by1_name) )
   {
      if ( (by1 = ( float * ) calloc(
            buffer_size,
            sizeof(float)) ) == NULL)
      {
         printf(
               "\ncalloc failed for %s buffer!\n",
               by1_name);
         exit( EXIT_FAILURE);
      }
      return by1_cdfNum;
   }
   else if ( !strcmp(
         variable_number_to_get,
         bz1_name) )
   {
      if ( (bz1 = ( float * ) calloc(
            buffer_size,
            sizeof(float)) ) == NULL)
      {
         printf(
               "\ncalloc failed for %s buffer!\n",
               bz1_name);
         exit( EXIT_FAILURE);
      }
      return bz1_cdfNum;
   }
   else if ( !strcmp(
         variable_number_to_get,
         ux_name) )
   {
      if ( (ux = ( float * ) calloc(
            buffer_size,
            sizeof(float)) ) == NULL)
      {
         printf(
               "\ncalloc failed for %s buffer!\n",
               ux_name);
         exit( EXIT_FAILURE);
      }
      return ux_cdfNum;
   }
   else if ( !strcmp(
         variable_number_to_get,
         uy_name) )
   {
      if ( (uy = ( float * ) calloc(
            buffer_size,
            sizeof(float)) ) == NULL)
      {
         printf(
               "\ncalloc failed for %s buffer!\n",
               uy_name);
         exit( EXIT_FAILURE);
      }
      return uy_cdfNum;
   }
   else if ( !strcmp(
         variable_number_to_get,
         uz_name) )
   {
      if ( (uz = ( float * ) calloc(
            buffer_size,
            sizeof(float)) ) == NULL)
      {
         printf(
               "\ncalloc failed for %s buffer!\n",
               uz_name);
         exit( EXIT_FAILURE);
      }
      return uz_cdfNum;
   }
   else if ( !strcmp(
         variable_number_to_get,
         jx_name) )
   {
      if ( (jx = ( float * ) calloc(
            buffer_size,
            sizeof(float)) ) == NULL)
      {
         printf(
               "\ncalloc failed for %s buffer!\n",
               jx_name);
         exit( EXIT_FAILURE);
      }
      return jx_cdfNum;
   }
   else if ( !strcmp(
         variable_number_to_get,
         jy_name) )
   {
      if ( (jy = ( float * ) calloc(
            buffer_size,
            sizeof(float)) ) == NULL)
      {
         printf(
               "\ncalloc failed for %s buffer!\n",
               jy_name);
         exit( EXIT_FAILURE);
      }
      return jy_cdfNum;
   }
   else if ( !strcmp(
         variable_number_to_get,
         jz_name) )
   {
      if ( (jz = ( float * ) calloc(
            buffer_size,
            sizeof(float)) ) == NULL)
      {
         printf(
               "\ncalloc failed for %s buffer!\n",
               jz_name);
         exit( EXIT_FAILURE);
      }
      return jz_cdfNum;
   }
   else if ( !strcmp(
         variable_number_to_get,
         rho_name) )
   {
      if ( (rho = ( float * ) calloc(
            buffer_size,
            sizeof(float)) ) == NULL)
      {
         printf(
               "\ncalloc failed for %s buffer!\n",
               rho_name);
         exit( EXIT_FAILURE);
      }
      return rho_cdfNum;
   }
   else if ( !strcmp(
         variable_number_to_get,
         p_name) )
   {
      if ( (p = ( float * ) calloc(
            buffer_size,
            sizeof(float)) ) == NULL)
      {
         printf(
               "\ncalloc failed for %s buffer!\n",
               p_name);
         exit( EXIT_FAILURE);
      }
      return p_cdfNum;
   }
   else if ( !strcmp(
            variable_number_to_get,
            eta_name) )
      {
         if ( (eta = ( float * ) calloc(
               buffer_size,
               sizeof(float)) ) == NULL)
         {
            printf(
                  "\ncalloc failed for %s buffer!\n",
                  eta_name);
            exit( EXIT_FAILURE);
         }
         return eta_cdfNum;
      }
   else
   {
      return -1;
   }
   /*else { printf("ERROR:\tcould not find cdf variable number for %s\n", variable ); return 0; }*/

}

void allocate_defaults_b(
      void)
{
   /************ we need to allocate space for all varoiables even when no main mem option is set
    * this allows the all inclusive close to work properly as it tries to free all variables.  We only need
    * to hold a enough space for a single value ******/

   if ( (bx = ( float * ) calloc(
         1,
         sizeof(float)) ) == NULL)
   {
      printf(
            "\ncalloc failed for %s buffer!\n",
            bx_name);
      exit( EXIT_FAILURE);
   }
   if ( (by = ( float * ) calloc(
         1,
         sizeof(float)) ) == NULL)
   {
      printf(
            "\ncalloc failed for %s buffer!\n",
            by_name);
      exit( EXIT_FAILURE);
   }
   if ( (bz = ( float * ) calloc(
         1,
         sizeof(float)) ) == NULL)
   {
      printf(
            "\ncalloc failed for %s buffer!\n",
            bz_name);
      exit( EXIT_FAILURE);
   }
   if ( (bx1 = ( float * ) calloc(
         1,
         sizeof(float)) ) == NULL)
   {
      printf(
            "\ncalloc failed for %s buffer!\n",
            bx1_name);
      exit( EXIT_FAILURE);
   }
   if ( (by1 = ( float * ) calloc(
         1,
         sizeof(float)) ) == NULL)
   {
      printf(
            "\ncalloc failed for %s buffer!\n",
            by1_name);
      exit( EXIT_FAILURE);
   }
   if ( (bz1 = ( float * ) calloc(
         1,
         sizeof(float)) ) == NULL)
   {
      printf(
            "\ncalloc failed for %s buffer!\n",
            bz1_name);
      exit( EXIT_FAILURE);
   }
   if ( (ux = ( float * ) calloc(
         1,
         sizeof(float)) ) == NULL)
   {
      printf(
            "\ncalloc failed for %s buffer!\n",
            ux_name);
      exit( EXIT_FAILURE);
   }
   if ( (uy = ( float * ) calloc(
         1,
         sizeof(float)) ) == NULL)
   {
      printf(
            "\ncalloc failed for %s buffer!\n",
            uy_name);
      exit( EXIT_FAILURE);
   }
   if ( (uz = ( float * ) calloc(
         1,
         sizeof(float)) ) == NULL)
   {
      printf(
            "\ncalloc failed for %s buffer!\n",
            uz_name);
      exit( EXIT_FAILURE);
   }
   if ( (jx = ( float * ) calloc(
         1,
         sizeof(float)) ) == NULL)
   {
      printf(
            "\ncalloc failed for %s buffer!\n",
            jx_name);
      exit( EXIT_FAILURE);
   }
   if ( (jy = ( float * ) calloc(
         1,
         sizeof(float)) ) == NULL)
   {
      printf(
            "\ncalloc failed for %s buffer!\n",
            jy_name);
      exit( EXIT_FAILURE);
   }
   if ( (jz = ( float * ) calloc(
         1,
         sizeof(float)) ) == NULL)
   {
      printf(
            "\ncalloc failed for %s buffer!\n",
            jz_name);
      exit( EXIT_FAILURE);
   }
   if ( (rho = ( float * ) calloc(
         1,
         sizeof(float)) ) == NULL)
   {
      printf(
            "\ncalloc failed for %s buffer!\n",
            rho_name);
      exit( EXIT_FAILURE);
   }
   if ( (p = ( float * ) calloc(
         1,
         sizeof(float)) ) == NULL)
   {
      printf(
            "\ncalloc failed for %s buffer!\n",
            p_name);
      exit( EXIT_FAILURE);
   }
   if ( (eta = ( float * ) calloc(
            1,
            sizeof(float)) ) == NULL)
      {
         printf(
               "\ncalloc failed for %s buffer!\n",
               eta_name);
         exit( EXIT_FAILURE);
      }

   return;
}

