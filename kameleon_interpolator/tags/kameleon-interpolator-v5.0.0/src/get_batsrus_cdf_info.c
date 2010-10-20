/*******************************************************************************
 *                                                                             *
 *    NAME:          get_batsrus_cdf_info.c                                    *
 *                                                                             *
 *    VERSION:       1.0                                                       *
 *                                                                             *
 *    AUTHOR:        Marlo M. H. Maddox                                        *
 *                   NASA-GSFC-AETD-ISD-ADMA(587)-CCMC                         *
 *                   Marlo.Maddox@nasa.gov                                     *
 *                   (301) 286 - 5202                                          *
 *                                                                             *
 *    PURPOSE:       Extracts all relevant information for identifying         *
 *                   variables etc from the batsrus cdf file.  This routine is *
 *                   called by the open_cdf call. Variable numbers are         *
 *                   extracted along with actual position values. If an        *
 *                   interpolate call is issued, the code will already have    *
 *                   information in memory to make data access faster.         *
 *                                                                             *
 *    INPUTS:                                                                  *
 *                                                                             *
 *    OUTPUTS:                                                                 *
 *                                                                             *
 *    FUNCTIONS:                                                               *
 *                                                                             *
 *    VARIABLES:                                                               *
 *                                                                             *
 *                                                                             *
 *    MODIFICATION                                                             *
 *    HISTORY:                                                                 *
 *                                                                             *
 *    02.22.2006     Maddox, Marlo                                             *
 *                   Added dynamic memory allocation for all variables         *
 *                   regardless of main mem is selected or not. If no          *
 *                   main mem is selected, space is reserved for only          *
 *                   one value for specified type.  This allows the            *
 *                   all-inclusive close_batsrus_cdf to work properly          *
 *                   calling free for all valid variables.                     *
 *                   This was done with the addition of the function           *
 *                   allocate_defaults() along with proper allocates for       *
 *                   AMR related variables                                     *
 *                                                                             *
 *    10.04.2006     Maddox, Marlo                                             *
 *                   Changed function type for load_*_cdf_variable_into_main_..*
 *                   from void to int                                          *
 *                                                                             *
 ******************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <math.h>
#include <stdarg.h>
#include "cdf_interface_variables.h"
#include "batsrus_cdf_interface_variables.h"
#include "cdf.h"

#define DEBUG_FLAG 1

void get_batsrus_cdf_info(
      int main_memory_flag)
{

   void allocate_defaults(
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

   int i, ix, iy, iz;
   float current_position;

   counts[0] = 1; /* set values */
   intervals[0] = 1;

   if ( DEBUG_FLAG)
   {
      printf(
            "allocate_deafults_done = %d\n",
            allocate_deafults_done);
      printf(
            "allocate_deafults_done variable address = %x\n",
            &allocate_deafults_done);
   }

   if (allocate_deafults_done <= 0)
   {
      allocate_defaults();
   }

   /***** get all of the cdf attributes/variable numbers that will be needed **/

   status = CDFlib( GET_, zVAR_NUMBER_, rho_name, &rho_cdfNum, NULL_);
   if (status != CDF_OK)
      StatusHandler(status);

   status = CDFlib( GET_, zVAR_NUMBER_, p_name, &p_cdfNum, NULL_);
   if (status != CDF_OK)
      StatusHandler(status);

   status = CDFlib( GET_, zVAR_NUMBER_, e_name, &e_cdfNum, NULL_);
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

   status = CDFlib( GET_, zVAR_NUMBER_, b1x_name, &b1x_cdfNum, NULL_);
   if (status != CDF_OK)
      StatusHandler(status);

   status = CDFlib( GET_, zVAR_NUMBER_, b1y_name, &b1y_cdfNum, NULL_);
   if (status != CDF_OK)
      StatusHandler(status);

   status = CDFlib( GET_, zVAR_NUMBER_, b1z_name, &b1z_cdfNum, NULL_);
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

   status = CDFlib( GET_, ATTR_NUMBER_, NX_name, &NX_cdf_num, NULL_);
   if (status != CDF_OK)
      StatusHandler(status);

   status = CDFlib( GET_, ATTR_NUMBER_, NY_name, &NY_cdf_num, NULL_);
   if (status != CDF_OK)
      StatusHandler(status);

   status = CDFlib( GET_, ATTR_NUMBER_, NZ_name, &NZ_cdf_num, NULL_);
   if (status != CDF_OK)
      StatusHandler(status);

   status
         = CDFlib( GET_, ATTR_NUMBER_, number_of_blocks_name, &number_of_blocks_cdfNum, NULL_);
   if (status != CDF_OK)
      StatusHandler(status);

   status
         = CDFlib( GET_, zVAR_NUMBER_, block_x_min_name, &block_x_min_cdfNum, NULL_);
   if (status != CDF_OK)
      StatusHandler(status);

   status
         = CDFlib( GET_, zVAR_NUMBER_, block_y_min_name, &block_y_min_cdfNum, NULL_);
   if (status != CDF_OK)
      StatusHandler(status);

   status
         = CDFlib( GET_, zVAR_NUMBER_, block_z_min_name, &block_z_min_cdfNum, NULL_);
   if (status != CDF_OK)
      StatusHandler(status);

   status
         = CDFlib( GET_, zVAR_NUMBER_, block_x_max_name, &block_x_max_cdfNum, NULL_);
   if (status != CDF_OK)
      StatusHandler(status);

   status
         = CDFlib( GET_, zVAR_NUMBER_, block_y_max_name, &block_y_max_cdfNum, NULL_);
   if (status != CDF_OK)
      StatusHandler(status);

   status
         = CDFlib( GET_, zVAR_NUMBER_, block_z_max_name, &block_z_max_cdfNum, NULL_);
   if (status != CDF_OK)
      StatusHandler(status);

   status
         = CDFlib( GET_, zVAR_NUMBER_, block_x_center_name, &block_x_center_cdfNum, NULL_);
   if (status != CDF_OK)
      StatusHandler(status);

   status
         = CDFlib( GET_, zVAR_NUMBER_, block_y_center_name, &block_y_center_cdfNum, NULL_);
   if (status != CDF_OK)
      StatusHandler(status);

   status
         = CDFlib( GET_, zVAR_NUMBER_, block_z_center_name, &block_z_center_cdfNum, NULL_);
   if (status != CDF_OK)
      StatusHandler(status);

   status
         = CDFlib( GET_, zVAR_NUMBER_, block_at_amr_level_name,&block_at_amr_level_cdfNum, NULL_);
   if (status != CDF_OK)
      StatusHandler(status);

   status
         = CDFlib( GET_, zVAR_NUMBER_, block_child_count_name, &block_child_count_cdfNum, NULL_);
   if (status != CDF_OK)
      StatusHandler(status);

   status
         = CDFlib( GET_, zVAR_NUMBER_, block_child_id_1_name, &block_child_id_1_cdfNum, NULL_);
   if (status != CDF_OK)
      StatusHandler(status);

   status
         = CDFlib( GET_, zVAR_NUMBER_, block_child_id_2_name, &block_child_id_2_cdfNum, NULL_);
   if (status != CDF_OK)
      StatusHandler(status);

   status
         = CDFlib( GET_, zVAR_NUMBER_, block_child_id_3_name, &block_child_id_3_cdfNum, NULL_);
   if (status != CDF_OK)
      StatusHandler(status);

   status
         = CDFlib( GET_, zVAR_NUMBER_, block_child_id_4_name, &block_child_id_4_cdfNum, NULL_);
   if (status != CDF_OK)
      StatusHandler(status);

   status
         = CDFlib( GET_, zVAR_NUMBER_, block_child_id_5_name, &block_child_id_5_cdfNum, NULL_);
   if (status != CDF_OK)
      StatusHandler(status);

   status
         = CDFlib( GET_, zVAR_NUMBER_, block_child_id_6_name, &block_child_id_6_cdfNum, NULL_);
   if (status != CDF_OK)
      StatusHandler(status);

   status
         = CDFlib( GET_, zVAR_NUMBER_, block_child_id_7_name, &block_child_id_7_cdfNum, NULL_);
   if (status != CDF_OK)
      StatusHandler(status);

   status
         = CDFlib( GET_, zVAR_NUMBER_, block_child_id_8_name, &block_child_id_8_cdfNum, NULL_);
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

   /********* get value for number_of_blocks **********/
   status = CDFlib(
   SELECT_, ATTR_, number_of_blocks_cdfNum,
   SELECT_, gENTRY_, 0,
   GET_, gENTRY_DATA_, &number_of_blocks,
   NULL_);
   if (status != CDF_OK)
      StatusHandler(status);

   /******** find size of block_at_amr_level array *****/
   status = CDFlib(
   SELECT_, zVAR_, block_at_amr_level_cdfNum,
   CONFIRM_, zVAR_DIMCOUNTS_, number_of_parents_at_amr_level,
   NULL_);

   /*convert original float N* to int n* */
   nx = NX;
   ny = NY;
   nz = NZ;

   /*printf("DEBUG\t main_memory_flag = %d\n", main_memory_flag);*/

   if ( !main_memory_flag)
   {

      /* allocate space for unique x, y,& z positions for a particular block */
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

   }
   else /* allocate space and read in unique x, y,& z positions for every block */
   {
      if ( (x_pos = ( float * ) calloc(
            nx * number_of_blocks,
            sizeof(float)) ) == NULL)
      {
         printf("\ncalloc failed for x_pos buffer!\n");
         exit( EXIT_FAILURE);
      }
      if ( (y_pos = ( float * ) calloc(
            ny * number_of_blocks,
            sizeof(float)) ) == NULL)
      {
         printf("\ncalloc failed for y_pos buffer!\n");
         exit( EXIT_FAILURE);
      }
      if ( (z_pos = ( float * ) calloc(
            nz * number_of_blocks,
            sizeof(float)) ) == NULL)
      {
         printf("\ncalloc failed for z_pos buffer!\n");
         exit( EXIT_FAILURE);
      }

      /********** get unique x,y,z values for every block *********************/

      for (i = 0; i < number_of_blocks; i++)
      {
         for (ix = 0; ix < nx; ix++)
         {
            indices[0] = (i*nx*ny*nz ) + ix;
            status = CDFlib(
            SELECT_, zVAR_, x_cdf_varNum,
            SELECT_, zVAR_RECNUMBER_, recordStart,
            SELECT_, zVAR_DIMINDICES_, indices,
            GET_, zVAR_DATA_, &current_position,
            NULL_);
            if (status != CDF_OK)
               StatusHandler(status);

            x_pos[ix + nx*i] = current_position;
         }
         for (iy = 0; iy < ny; iy++)
         {
            indices[0] = (i*nx*ny*nz ) + iy*nx;
            status = CDFlib(
            SELECT_, zVAR_, y_cdf_varNum,
            SELECT_, zVAR_RECNUMBER_, recordStart,
            SELECT_, zVAR_DIMINDICES_, indices,
            GET_, zVAR_DATA_, &current_position,
            NULL_);
            if (status != CDF_OK)
               StatusHandler(status);

            y_pos[iy + ny*i] = current_position;
         }
         for (iz = 0; iz < nz; iz++)
         {
            indices[0] = (i*nx*ny*nz ) + iz*nx*ny;
            status = CDFlib(
            SELECT_, zVAR_, z_cdf_varNum,
            SELECT_, zVAR_RECNUMBER_, recordStart,
            SELECT_, zVAR_DIMINDICES_, indices,
            GET_, zVAR_DATA_, &current_position,
            NULL_);
            if (status != CDF_OK)
               StatusHandler(status);

            z_pos[iz + nz*i] = current_position;
         }
      }

   }

   number_of_blocks_plus_virtual = ( 8 * number_of_blocks ) / 7; /*  add ceil or floor later for consistency */

   if (main_memory_flag)
   {

      /*********************** allocate space for block_*_min/max *************/
      if ( (block_x_min_array = ( float * ) calloc(
            number_of_blocks_plus_virtual,
            sizeof(float)) ) == NULL)
      {
         printf("\ncalloc failed for block_x_min_array buffer!\n");
         exit( EXIT_FAILURE);
      }
      if ( (block_x_max_array = ( float * ) calloc(
            number_of_blocks_plus_virtual,
            sizeof(float)) ) == NULL)
      {
         printf("\ncalloc failed for block_x_max_array buffer!\n");
         exit( EXIT_FAILURE);
      }
      if ( (block_y_min_array = ( float * ) calloc(
            number_of_blocks_plus_virtual,
            sizeof(float)) ) == NULL)
      {
         printf("\ncalloc failed for block_y_min_array buffer!\n");
         exit( EXIT_FAILURE);
      }
      if ( (block_y_max_array = ( float * ) calloc(
            number_of_blocks_plus_virtual,
            sizeof(float)) ) == NULL)
      {
         printf("\ncalloc failed for block_y_max_array buffer!\n");
         exit( EXIT_FAILURE);
      }
      if ( (block_z_min_array = ( float * ) calloc(
            number_of_blocks_plus_virtual,
            sizeof(float)) ) == NULL)
      {
         printf("\ncalloc failed for block_z_min_array buffer!\n");
         exit( EXIT_FAILURE);
      }
      if ( (block_z_max_array = ( float * ) calloc(
            number_of_blocks_plus_virtual,
            sizeof(float)) ) == NULL)
      {
         printf("\ncalloc failed for block_z_max_array buffer!\n");
         exit( EXIT_FAILURE);
      }

      /**** allocate space for block_child_count_array, block_*_center_array, & block_child_id_*_array ****/
      if ( (block_child_count_array = ( int * ) calloc(
            number_of_blocks_plus_virtual,
            sizeof(int)) ) == NULL)
      {
         printf("\ncalloc failed for block_child_count_array buffer!\n");
         exit( EXIT_FAILURE);
      }
      if ( (block_x_center_array = ( float * ) calloc(
            number_of_blocks_plus_virtual,
            sizeof(float)) ) == NULL)
      {
         printf("\ncalloc failed for block_x_center_array buffer!\n");
         exit( EXIT_FAILURE);
      }
      if ( (block_y_center_array = ( float * ) calloc(
            number_of_blocks_plus_virtual,
            sizeof(float)) ) == NULL)
      {
         printf("\ncalloc failed for block_y_center_array buffer!\n");
         exit( EXIT_FAILURE);
      }
      if ( (block_z_center_array = ( float * ) calloc(
            number_of_blocks_plus_virtual,
            sizeof(float)) ) == NULL)
      {
         printf("\ncalloc failed for block_z_center_array buffer!\n");
         exit( EXIT_FAILURE);
      }
      if ( (block_child_id_1_array = ( int * ) calloc(
            number_of_blocks_plus_virtual,
            sizeof(int)) ) == NULL)
      {
         printf("\ncalloc failed forblock_child_id_1_array buffer!\n");
         exit( EXIT_FAILURE);
      }
      if ( (block_child_id_2_array = ( int * ) calloc(
            number_of_blocks_plus_virtual,
            sizeof(int)) ) == NULL)
      {
         printf("\ncalloc failed forblock_child_id_2_array buffer!\n");
         exit( EXIT_FAILURE);
      }
      if ( (block_child_id_3_array = ( int * ) calloc(
            number_of_blocks_plus_virtual,
            sizeof(int)) ) == NULL)
      {
         printf("\ncalloc failed forblock_child_id_3_array buffer!\n");
         exit( EXIT_FAILURE);
      }
      if ( (block_child_id_4_array = ( int * ) calloc(
            number_of_blocks_plus_virtual,
            sizeof(int)) ) == NULL)
      {
         printf("\ncalloc failed forblock_child_id_4_array buffer!\n");
         exit( EXIT_FAILURE);
      }
      if ( (block_child_id_5_array = ( int * ) calloc(
            number_of_blocks_plus_virtual,
            sizeof(int)) ) == NULL)
      {
         printf("\ncalloc failed forblock_child_id_5_array buffer!\n");
         exit( EXIT_FAILURE);
      }
      if ( (block_child_id_6_array = (int * ) calloc(
            number_of_blocks_plus_virtual,
            sizeof(int)) ) == NULL)
      {
         printf("\ncalloc failed forblock_child_id_6_array buffer!\n");
         exit( EXIT_FAILURE);
      }
      if ( (block_child_id_7_array = ( int * ) calloc(
            number_of_blocks_plus_virtual,
            sizeof(int)) ) == NULL)
      {
         printf("\ncalloc failed forblock_child_id_7_array buffer!\n");
         exit( EXIT_FAILURE);
      }
      if ( (block_child_id_8_array = ( int * ) calloc(
            number_of_blocks_plus_virtual,
            sizeof(int)) ) == NULL)
      {
         printf("\ncalloc failed forblock_child_id_8_array buffer!\n");
         exit( EXIT_FAILURE);
      }

      /********** get block_*_min/max values ************/
      counts[0] = number_of_blocks_plus_virtual;
      indices[0] = 0;
      intervals[0] = 1;

      status = CDFlib(
      SELECT_, zVAR_, block_x_min_cdfNum,
      SELECT_, zVAR_RECNUMBER_, recordStart,
      SELECT_, zVAR_RECCOUNT_, recordCount,
      SELECT_, zVAR_RECINTERVAL_, recordInterval,
      SELECT_, zVAR_DIMINDICES_, indices,
      SELECT_, zVAR_DIMCOUNTS_, counts,
      SELECT_, zVAR_DIMINTERVALS_, intervals,
      GET_, zVAR_HYPERDATA_, block_x_min_array,
      NULL_);
      if (status != CDF_OK)
         StatusHandler(status);

      status = CDFlib(
      SELECT_, zVAR_, block_y_min_cdfNum,
      SELECT_, zVAR_RECNUMBER_, recordStart,
      SELECT_, zVAR_RECCOUNT_, recordCount,
      SELECT_, zVAR_RECINTERVAL_, recordInterval,
      SELECT_, zVAR_DIMINDICES_, indices,
      SELECT_, zVAR_DIMCOUNTS_, counts,
      SELECT_, zVAR_DIMINTERVALS_, intervals,
      GET_, zVAR_HYPERDATA_, block_y_min_array,
      NULL_);
      if (status != CDF_OK)
         StatusHandler(status);

      status = CDFlib(
      SELECT_, zVAR_, block_z_min_cdfNum,
      SELECT_, zVAR_RECNUMBER_, recordStart,
      SELECT_, zVAR_RECCOUNT_, recordCount,
      SELECT_, zVAR_RECINTERVAL_, recordInterval,
      SELECT_, zVAR_DIMINDICES_, indices,
      SELECT_, zVAR_DIMCOUNTS_, counts,
      SELECT_, zVAR_DIMINTERVALS_, intervals,
      GET_, zVAR_HYPERDATA_, block_z_min_array,
      NULL_);
      if (status != CDF_OK)
         StatusHandler(status);

      status = CDFlib(
      SELECT_, zVAR_, block_x_max_cdfNum,
      SELECT_, zVAR_RECNUMBER_, recordStart,
      SELECT_, zVAR_RECCOUNT_, recordCount,
      SELECT_, zVAR_RECINTERVAL_, recordInterval,
      SELECT_, zVAR_DIMINDICES_, indices,
      SELECT_, zVAR_DIMCOUNTS_, counts,
      SELECT_, zVAR_DIMINTERVALS_, intervals,
      GET_, zVAR_HYPERDATA_, block_x_max_array,
      NULL_);
      if (status != CDF_OK)
         StatusHandler(status);

      status = CDFlib(
      SELECT_, zVAR_, block_y_max_cdfNum,
      SELECT_, zVAR_RECNUMBER_, recordStart,
      SELECT_, zVAR_RECCOUNT_, recordCount,
      SELECT_, zVAR_RECINTERVAL_, recordInterval,
      SELECT_, zVAR_DIMINDICES_, indices,
      SELECT_, zVAR_DIMCOUNTS_, counts,
      SELECT_, zVAR_DIMINTERVALS_, intervals,
      GET_, zVAR_HYPERDATA_, block_y_max_array,
      NULL_);
      if (status != CDF_OK)
         StatusHandler(status);

      status = CDFlib(
      SELECT_, zVAR_, block_z_max_cdfNum,
      SELECT_, zVAR_RECNUMBER_, recordStart,
      SELECT_, zVAR_RECCOUNT_, recordCount,
      SELECT_, zVAR_RECINTERVAL_, recordInterval,
      SELECT_, zVAR_DIMINDICES_, indices,
      SELECT_, zVAR_DIMCOUNTS_, counts,
      SELECT_, zVAR_DIMINTERVALS_, intervals,
      GET_, zVAR_HYPERDATA_, block_z_max_array,
      NULL_);
      if (status != CDF_OK)
         StatusHandler(status);

      /********** get block_child_count_array, block_*_center_array, & block_child_id_*_array values ************/

      status = CDFlib(
      SELECT_, zVAR_, block_child_count_cdfNum,
      SELECT_, zVAR_RECNUMBER_, recordStart,
      SELECT_, zVAR_RECCOUNT_, recordCount,
      SELECT_, zVAR_RECINTERVAL_, recordInterval,
      SELECT_, zVAR_DIMINDICES_, indices,
      SELECT_, zVAR_DIMCOUNTS_, counts,
      SELECT_, zVAR_DIMINTERVALS_, intervals,
      GET_, zVAR_HYPERDATA_, block_child_count_array,
      NULL_);
      if (status != CDF_OK)
         StatusHandler(status);

      status = CDFlib(
      SELECT_, zVAR_, block_x_center_cdfNum,
      SELECT_, zVAR_RECNUMBER_, recordStart,
      SELECT_, zVAR_RECCOUNT_, recordCount,
      SELECT_, zVAR_RECINTERVAL_, recordInterval,
      SELECT_, zVAR_DIMINDICES_, indices,
      SELECT_, zVAR_DIMCOUNTS_, counts,
      SELECT_, zVAR_DIMINTERVALS_, intervals,
      GET_, zVAR_HYPERDATA_, block_x_center_array,
      NULL_);
      if (status != CDF_OK)
         StatusHandler(status);

      status = CDFlib(
      SELECT_, zVAR_, block_y_center_cdfNum,
      SELECT_, zVAR_RECNUMBER_, recordStart,
      SELECT_, zVAR_RECCOUNT_, recordCount,
      SELECT_, zVAR_RECINTERVAL_, recordInterval,
      SELECT_, zVAR_DIMINDICES_, indices,
      SELECT_, zVAR_DIMCOUNTS_, counts,
      SELECT_, zVAR_DIMINTERVALS_, intervals,
      GET_, zVAR_HYPERDATA_, block_y_center_array,
      NULL_);
      if (status != CDF_OK)
         StatusHandler(status);

      status = CDFlib(
      SELECT_, zVAR_, block_z_center_cdfNum,
      SELECT_, zVAR_RECNUMBER_, recordStart,
      SELECT_, zVAR_RECCOUNT_, recordCount,
      SELECT_, zVAR_RECINTERVAL_, recordInterval,
      SELECT_, zVAR_DIMINDICES_, indices,
      SELECT_, zVAR_DIMCOUNTS_, counts,
      SELECT_, zVAR_DIMINTERVALS_, intervals,
      GET_, zVAR_HYPERDATA_, block_z_center_array,
      NULL_);
      if (status != CDF_OK)
         StatusHandler(status);

      status = CDFlib(
      SELECT_, zVAR_, block_child_id_1_cdfNum,
      SELECT_, zVAR_RECNUMBER_, recordStart,
      SELECT_, zVAR_RECCOUNT_, recordCount,
      SELECT_, zVAR_RECINTERVAL_, recordInterval,
      SELECT_, zVAR_DIMINDICES_, indices,
      SELECT_, zVAR_DIMCOUNTS_, counts,
      SELECT_, zVAR_DIMINTERVALS_, intervals,
      GET_, zVAR_HYPERDATA_, block_child_id_1_array,
      NULL_);
      if (status != CDF_OK)
         StatusHandler(status);

      status = CDFlib(
      SELECT_, zVAR_, block_child_id_2_cdfNum,
      SELECT_, zVAR_RECNUMBER_, recordStart,
      SELECT_, zVAR_RECCOUNT_, recordCount,
      SELECT_, zVAR_RECINTERVAL_, recordInterval,
      SELECT_, zVAR_DIMINDICES_, indices,
      SELECT_, zVAR_DIMCOUNTS_, counts,
      SELECT_, zVAR_DIMINTERVALS_, intervals,
      GET_, zVAR_HYPERDATA_, block_child_id_2_array,
      NULL_);
      if (status != CDF_OK)
         StatusHandler(status);

      status = CDFlib(
      SELECT_, zVAR_, block_child_id_3_cdfNum,
      SELECT_, zVAR_RECNUMBER_, recordStart,
      SELECT_, zVAR_RECCOUNT_, recordCount,
      SELECT_, zVAR_RECINTERVAL_, recordInterval,
      SELECT_, zVAR_DIMINDICES_, indices,
      SELECT_, zVAR_DIMCOUNTS_, counts,
      SELECT_, zVAR_DIMINTERVALS_, intervals,
      GET_, zVAR_HYPERDATA_, block_child_id_3_array,
      NULL_);
      if (status != CDF_OK)
         StatusHandler(status);

      status = CDFlib(
      SELECT_, zVAR_, block_child_id_4_cdfNum,
      SELECT_, zVAR_RECNUMBER_, recordStart,
      SELECT_, zVAR_RECCOUNT_, recordCount,
      SELECT_, zVAR_RECINTERVAL_, recordInterval,
      SELECT_, zVAR_DIMINDICES_, indices,
      SELECT_, zVAR_DIMCOUNTS_, counts,
      SELECT_, zVAR_DIMINTERVALS_, intervals,
      GET_, zVAR_HYPERDATA_, block_child_id_4_array,
      NULL_);
      if (status != CDF_OK)
         StatusHandler(status);

      status = CDFlib(
      SELECT_, zVAR_, block_child_id_5_cdfNum,
      SELECT_, zVAR_RECNUMBER_, recordStart,
      SELECT_, zVAR_RECCOUNT_, recordCount,
      SELECT_, zVAR_RECINTERVAL_, recordInterval,
      SELECT_, zVAR_DIMINDICES_, indices,
      SELECT_, zVAR_DIMCOUNTS_, counts,
      SELECT_, zVAR_DIMINTERVALS_, intervals,
      GET_, zVAR_HYPERDATA_, block_child_id_5_array,
      NULL_);
      if (status != CDF_OK)
         StatusHandler(status);

      status = CDFlib(
      SELECT_, zVAR_, block_child_id_6_cdfNum,
      SELECT_, zVAR_RECNUMBER_, recordStart,
      SELECT_, zVAR_RECCOUNT_, recordCount,
      SELECT_, zVAR_RECINTERVAL_, recordInterval,
      SELECT_, zVAR_DIMINDICES_, indices,
      SELECT_, zVAR_DIMCOUNTS_, counts,
      SELECT_, zVAR_DIMINTERVALS_, intervals,
      GET_, zVAR_HYPERDATA_, block_child_id_6_array,
      NULL_);
      if (status != CDF_OK)
         StatusHandler(status);

      status = CDFlib(
      SELECT_, zVAR_, block_child_id_7_cdfNum,
      SELECT_, zVAR_RECNUMBER_, recordStart,
      SELECT_, zVAR_RECCOUNT_, recordCount,
      SELECT_, zVAR_RECINTERVAL_, recordInterval,
      SELECT_, zVAR_DIMINDICES_, indices,
      SELECT_, zVAR_DIMCOUNTS_, counts,
      SELECT_, zVAR_DIMINTERVALS_, intervals,
      GET_, zVAR_HYPERDATA_, block_child_id_7_array,
      NULL_);
      if (status != CDF_OK)
         StatusHandler(status);

      status = CDFlib(
      SELECT_, zVAR_, block_child_id_8_cdfNum,
      SELECT_, zVAR_RECNUMBER_, recordStart,
      SELECT_, zVAR_RECCOUNT_, recordCount,
      SELECT_, zVAR_RECINTERVAL_, recordInterval,
      SELECT_, zVAR_DIMINDICES_, indices,
      SELECT_, zVAR_DIMCOUNTS_, counts,
      SELECT_, zVAR_DIMINTERVALS_, intervals,
      GET_, zVAR_HYPERDATA_, block_child_id_8_array,
      NULL_);
      if (status != CDF_OK)
         StatusHandler(status);

   }
   else /** we still need to set something for all the pionters so the all inclusive close_batsrus_cdf free's will work **/
   {
      /*********************** allocate space for block_*_min/max *****************************************/
      if ( (block_x_min_array = ( float * ) calloc(
            1,
            sizeof(float)) ) == NULL)
      {
         printf("\ncalloc failed for block_x_min_array buffer!\n");
         exit( EXIT_FAILURE);
      }
      if ( (block_x_max_array = ( float * ) calloc(
            1,
            sizeof(float)) ) == NULL)
      {
         printf("\ncalloc failed for block_x_max_array buffer!\n");
         exit( EXIT_FAILURE);
      }
      if ( (block_y_min_array = ( float * ) calloc(
            1,
            sizeof(float)) ) == NULL)
      {
         printf("\ncalloc failed for block_y_min_array buffer!\n");
         exit( EXIT_FAILURE);
      }
      if ( (block_y_max_array = ( float * ) calloc(
            1,
            sizeof(float)) ) == NULL)
      {
         printf("\ncalloc failed for block_y_max_array buffer!\n");
         exit( EXIT_FAILURE);
      }
      if ( (block_z_min_array = ( float * ) calloc(
            1,
            sizeof(float)) ) == NULL)
      {
         printf("\ncalloc failed for block_z_min_array buffer!\n");
         exit( EXIT_FAILURE);
      }
      if ( (block_z_max_array = ( float * ) calloc(
            1,
            sizeof(float)) ) == NULL)
      {
         printf("\ncalloc failed for block_z_max_array buffer!\n");
         exit( EXIT_FAILURE);
      }

      /**** allocate space for block_child_count_array, block_*_center_array, & block_child_id_*_array ****/
      if ( (block_child_count_array = ( int * ) calloc(
            1,
            sizeof(int)) ) == NULL)
      {
         printf("\ncalloc failed for block_child_count_array buffer!\n");
         exit( EXIT_FAILURE);
      }
      if ( (block_x_center_array = ( float * ) calloc(
            1,
            sizeof(float)) ) == NULL)
      {
         printf("\ncalloc failed for block_x_center_array buffer!\n");
         exit( EXIT_FAILURE);
      }
      if ( (block_y_center_array = ( float * ) calloc(
            1,
            sizeof(float)) ) == NULL)
      {
         printf("\ncalloc failed for block_y_center_array buffer!\n");
         exit( EXIT_FAILURE);
      }
      if ( (block_z_center_array = ( float * ) calloc(
            1,
            sizeof(float)) ) == NULL)
      {
         printf("\ncalloc failed for block_z_center_array buffer!\n");
         exit( EXIT_FAILURE);
      }
      if ( (block_child_id_1_array = ( int * ) calloc(
            1,
            sizeof(int)) ) == NULL)
      {
         printf("\ncalloc failed forblock_child_id_1_array buffer!\n");
         exit( EXIT_FAILURE);
      }
      if ( (block_child_id_2_array = ( int * ) calloc(
            1,
            sizeof(int)) ) == NULL)
      {
         printf("\ncalloc failed forblock_child_id_2_array buffer!\n");
         exit( EXIT_FAILURE);
      }
      if ( (block_child_id_3_array = ( int * ) calloc(
            1,
            sizeof(int)) ) == NULL)
      {
         printf("\ncalloc failed forblock_child_id_3_array buffer!\n");
         exit( EXIT_FAILURE);
      }
      if ( (block_child_id_4_array = ( int * ) calloc(
            1,
            sizeof(int)) ) == NULL)
      {
         printf("\ncalloc failed forblock_child_id_4_array buffer!\n");
         exit( EXIT_FAILURE);
      }
      if ( (block_child_id_5_array = ( int * ) calloc(
            1,
            sizeof(int)) ) == NULL)
      {
         printf("\ncalloc failed forblock_child_id_5_array buffer!\n");
         exit( EXIT_FAILURE);
      }
      if ( (block_child_id_6_array = (int * ) calloc(
            1,
            sizeof(int)) ) == NULL)
      {
         printf("\ncalloc failed forblock_child_id_6_array buffer!\n");
         exit( EXIT_FAILURE);
      }
      if ( (block_child_id_7_array = ( int * ) calloc(
            1,
            sizeof(int)) ) == NULL)
      {
         printf("\ncalloc failed forblock_child_id_7_array buffer!\n");
         exit( EXIT_FAILURE);
      }
      if ( (block_child_id_8_array = ( int * ) calloc(
            1,
            sizeof(int)) ) == NULL)
      {
         printf("\ncalloc failed forblock_child_id_8_array buffer!\n");
         exit( EXIT_FAILURE);
      }

   }

   counts[0] = number_of_parents_at_amr_level[0];
   indices[0] = 0;

   /******** get values for block_at_amr_level *******/

   if ( (block_at_amr_level = ( int * ) calloc(
         counts[0],
         sizeof(int)) ) == NULL)
   {
      printf("\ncalloc failed for buffer!\n");
      exit( EXIT_FAILURE);
   }

   status = CDFlib(
   SELECT_, zVAR_, block_at_amr_level_cdfNum,
   SELECT_, zVAR_RECNUMBER_, recordStart,
   SELECT_, zVAR_RECCOUNT_, recordCount,
   SELECT_, zVAR_RECINTERVAL_, recordInterval,
   SELECT_, zVAR_DIMINDICES_, indices,
   SELECT_, zVAR_DIMCOUNTS_, counts,
   SELECT_, zVAR_DIMINTERVALS_, intervals,
   GET_, zVAR_HYPERDATA_, block_at_amr_level,
   NULL_);
   if (status != CDF_OK)
      StatusHandler(status);

   return;

}

int load_batsrus_cdf_variable_into_main_memory(
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

   int batsrus_reserve_mem_and_set_cdf_num(
         char *);

   /*printf("DEBUG\tinside load\n");*/

   variable_cdf_number = batsrus_reserve_mem_and_set_cdf_num(variable_to_read);

   /* instead of EXITING PROGRAM with exit failure - print warning and return control to calling function */

   if (variable_cdf_number == -1)
   {
      printf(
            "ERROR:\tcould not find cdf variable number for -->%s<--\n",
            variable_to_read);
      return EXIT_FAILURE; /*exit( EXIT_FAILURE );*/
   }

   /*printf("DEBUG\tback from reserve mem with cdfnum %d for variable %s \n", variable_cdf_number, variable_to_read );*/

   counts[0] = number_of_blocks * nx * ny * nz;
   indices[0] = 0;
   intervals[0] = 1;

   /*printf("\tDEBUGabout to  strcmp ( %s, %s ) \n", variable_to_read, bx_name );*/

   if ( !strcmp(
         variable_to_read,
         bx_name) )
   {
      /*printf("DEBUG\tabout to  hyper read bx \n");*/
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
         b1x_name) )
   {
      status = CDFlib(
      SELECT_, zVAR_, variable_cdf_number,
      SELECT_, zVAR_RECNUMBER_, recordStart,
      SELECT_, zVAR_RECCOUNT_, recordCount,
      SELECT_, zVAR_RECINTERVAL_, recordInterval,
      SELECT_, zVAR_DIMINDICES_, indices,
      SELECT_, zVAR_DIMCOUNTS_, counts,
      SELECT_, zVAR_DIMINTERVALS_, intervals,
      GET_, zVAR_HYPERDATA_, b1x,
      NULL_);
      if (status != CDF_OK)
         StatusHandler(status);
   }
   else if ( !strcmp(
         variable_to_read,
         b1y_name) )
   {
      status = CDFlib(
      SELECT_, zVAR_, variable_cdf_number,
      SELECT_, zVAR_RECNUMBER_, recordStart,
      SELECT_, zVAR_RECCOUNT_, recordCount,
      SELECT_, zVAR_RECINTERVAL_, recordInterval,
      SELECT_, zVAR_DIMINDICES_, indices,
      SELECT_, zVAR_DIMCOUNTS_, counts,
      SELECT_, zVAR_DIMINTERVALS_, intervals,
      GET_, zVAR_HYPERDATA_, b1y,
      NULL_);
      if (status != CDF_OK)
         StatusHandler(status);
   }
   else if ( !strcmp(
         variable_to_read,
         b1z_name) )
   {
      status = CDFlib(
      SELECT_, zVAR_, variable_cdf_number,
      SELECT_, zVAR_RECNUMBER_, recordStart,
      SELECT_, zVAR_RECCOUNT_, recordCount,
      SELECT_, zVAR_RECINTERVAL_, recordInterval,
      SELECT_, zVAR_DIMINDICES_, indices,
      SELECT_, zVAR_DIMCOUNTS_, counts,
      SELECT_, zVAR_DIMINTERVALS_, intervals,
      GET_, zVAR_HYPERDATA_, b1z,
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
         e_name) )
   {
      status = CDFlib(
      SELECT_, zVAR_, variable_cdf_number,
      SELECT_, zVAR_RECNUMBER_, recordStart,
      SELECT_, zVAR_RECCOUNT_, recordCount,
      SELECT_, zVAR_RECINTERVAL_, recordInterval,
      SELECT_, zVAR_DIMINDICES_, indices,
      SELECT_, zVAR_DIMCOUNTS_, counts,
      SELECT_, zVAR_DIMINTERVALS_, intervals,
      GET_, zVAR_HYPERDATA_, e,
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
   else
   {
      /* INSERT ERROR HANDLER HERE */
      printf("couldn't find a variable to load...\n");
      return EXIT_FAILURE;
   }

   return EXIT_SUCCESS;

}

int batsrus_reserve_mem_and_set_cdf_num(
      char *variable_number_to_get)
{

   int buffer_size = number_of_blocks * nx * ny * nz;

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
         b1x_name) )
   {
      if ( (b1x = ( float * ) calloc(
            buffer_size,
            sizeof(float)) ) == NULL)
      {
         printf(
               "\ncalloc failed for %s buffer!\n",
               b1x_name);
         exit( EXIT_FAILURE);
      }
      return b1x_cdfNum;
   }
   else if ( !strcmp(
         variable_number_to_get,
         b1y_name) )
   {
      if ( (b1y = ( float * ) calloc(
            buffer_size,
            sizeof(float)) ) == NULL)
      {
         printf(
               "\ncalloc failed for %s buffer!\n",
               b1y_name);
         exit( EXIT_FAILURE);
      }
      return b1y_cdfNum;
   }
   else if ( !strcmp(
         variable_number_to_get,
         b1z_name) )
   {
      if ( (b1z = ( float * ) calloc(
            buffer_size,
            sizeof(float)) ) == NULL)
      {
         printf(
               "\ncalloc failed for %s buffer!\n",
               b1z_name);
         exit( EXIT_FAILURE);
      }
      return b1z_cdfNum;
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
         e_name) )
   {
      if ( (e = ( float * ) calloc(
            buffer_size,
            sizeof(float)) ) == NULL)
      {
         printf(
               "\ncalloc failed for %s buffer!\n",
               e_name);
         exit( EXIT_FAILURE);
      }
      return e_cdfNum;
   }
   else
   {
      return -1;
   }

}

void allocate_defaults(
      void)
{

   /************ we need to allocate space for all varoiables even when no main mem option is set
    * this allows the all inclusing close to work properly as it tries to free all variables.  We only need
    * to hold a enough space for a single value ******/

   if ( DEBUG_FLAG)
   {
      printf("Allocating Defaults...\n");
   }

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
   if ( (b1x = ( float * ) calloc(
         1,
         sizeof(float)) ) == NULL)
   {
      printf(
            "\ncalloc failed for %s buffer!\n",
            b1x_name);
      exit( EXIT_FAILURE);
   }
   if ( (b1y = ( float * ) calloc(
         1,
         sizeof(float)) ) == NULL)
   {
      printf(
            "\ncalloc failed for %s buffer!\n",
            b1y_name);
      exit( EXIT_FAILURE);
   }
   if ( (b1z = ( float * ) calloc(
         1,
         sizeof(float)) ) == NULL)
   {
      printf(
            "\ncalloc failed for %s buffer!\n",
            b1z_name);
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
   if ( (e = ( float * ) calloc(
         1,
         sizeof(float)) ) == NULL)
   {
      printf(
            "\ncalloc failed for %s buffer!\n",
            e_name);
      exit( EXIT_FAILURE);
   }

   allocate_deafults_done = 1;

   return;
}

