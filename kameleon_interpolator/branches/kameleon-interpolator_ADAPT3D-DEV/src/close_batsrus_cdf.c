/*******************************************************************************
 *                                                                             *
 *       NAME:          close_batsrus_cdf.c                                    *
 *                                                                             *
 *       VERSION:       1.0                                                    *
 *                                                                             *
 *       AUTHOR:        Marlo M. H. Maddox                                     *
 *                      NASA-GSFC-AETD-ISD-ADMA(587)-CCMC                      *
 *                      Marlo.Maddox@nasa.gov                                  *
 *                      (301) 286 - 5202                                       *
 *                                                                             *
 *       PURPOSE:       Performs necessary clean up from opening a batsrus file*
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
 *       04.14.2008     Maddox, Marlo                                          *
 *                      Added full_bats_grid_in_memory reseting functionality  *
 *                      which is used to determine if the full bats amr grid is*
 *                                                                             *
 *                                                                             *
 *                                                                             *
 ******************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <math.h>
#include <stdarg.h>
#include "cdf.h"
#include "cdf_interface_variables.h"
#include "batsrus_cdf_interface_variables.h"

#define DEBUG_FLAG 0

void close_batsrus_cdf(
      void)
{

   CDFstatus status;
   extern CDFid current_file_id;

   status = CDFclose ( current_file_id );
   if (status != CDF_OK)
   {
      StatusHandler(status);
   }

   if ( DEBUG_FLAG)
   {
      printf("Calling free on position arrays...\n");
   }

   free(x_pos);
   free(y_pos);
   free(z_pos);

   if ( DEBUG_FLAG)
   {
      printf("Calling free on varaiable arrays...\n");
   }


   free(bx);
   free(by);
   free(bz);
   free(b1x);
   free(b1y);
   free(b1z);
   free(ux);
   free(uy);
   free(uz);
   free(jx);
   free(jy);
   free(jz);
   free(rho);
   free(p);
   free(e);

   if ( DEBUG_FLAG)
   {
      printf("Calling free on amr grid arrays...\n");
   }

   free(block_x_min_array);
   free(block_x_max_array);
   free(block_y_min_array);
   free(block_y_max_array);
   free(block_z_min_array);
   free(block_z_max_array);
   free(block_at_amr_level);
   free(block_child_count_array);
   free(block_x_center_array);
   free(block_y_center_array);
   free(block_z_center_array);
   free(block_child_id_1_array);
   free(block_child_id_2_array);
   free(block_child_id_3_array);
   free(block_child_id_4_array);
   free(block_child_id_5_array);
   free(block_child_id_6_array);
   free(block_child_id_7_array);
   free(block_child_id_8_array);

   full_bats_grid_in_memory = 0;

   return;

}

