/*******************************************************************************
 *                                                                             *
 *       NAME:          interpolate_kpvt_cdf.c                                 *
 *                                                                             *
 *       VERSION:       1.0                                                    *
 *                                                                             *
 *       AUTHOR:        Marlo M. H. Maddox                                     *
 *                      NASA-GSFC-AETD-ISD-ADMA(587)-CCMC                      *
 *                      Marlo.Maddox@nasa.gov                                  *
 *                      (301) 286 - 5202                                       *
 *                                                                             *
 *       PURPOSE:       Retrun an interpolated variable value for any given    *
 *                      position in the simulation grid.  Requires a file to   *
 *                      have alreday been opened with open_cdf call            *
 *                                                                             *
 *       INPUTS:        (1)  variable name (2-4) x,y,z posotion (5)mem flag    *
 *                      (6)opt arg flag                                        *
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
 *       11.30.2006     Maddox, Marlo    Inital Development Started            *
 *                                                                             *
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

#define MISSING      -1.e31
#define MIN_RANGE    -1e9
#define MAX_RANGE    +1e9

#define DEBUG_FLAG    0

float interpolate_kpvt_cdf(
      char *variable,
      float X,
      float Y,
      float Z,
      int main_memory_flag,
      int optional_argument_flag,
      ...)
{

   long counts[1] =
   { 0 };
   long intervals[1] =
   { 1 };
   long cdf_varNum;

   float interpolated_value;

   float *x_position_array, *y_position_array, *z_position_array; /* generic array pointers that will be set to cooresponding kpvt grid according to which variable is being interpolated */

   int index_x = -1, index_y = -1, index_z = -1; /* used in numerical recipes hunt function */

   float dx_dummy, dy_dummy, dz_dummy; /* dummy variables mem locs to set if no real mem locs are input as optional args */

   /* create variables to pass on to interpolate_in_block regardless if no optional arguments exist ( so interpolate_in_block can have a fixed arg list */
   float missing;
   float *dx, *dy, *dz;

   va_list argptr; /* create a pointer to variable list of arguments */

   float interpolate_in_block_kpvt(
         float,
         float,
         float,
         int,
         int,
         int,
         float *,
         float *,
         float *,
         char *,
         float *,
         float *,
         float *,
         int,
         int);
   extern void hunt(
         float *,
         int,
         float,
         int *);

   extern int is_var_in_memory(
         char *);

   /** lets see if requested variable is in memory **/

   main_memory_flag = is_var_in_memory(variable);

   /********************************************************************************
    ***** USERS are expecting to use GSE so we need to convert GSE_JR to GSE ********
    ***** by reversing the sign of x & y and all x & y vector components    *********
    ********************************************************************************/

   counts[0] = 0; /*reset values after once through */
   intervals[0] = 1;

   va_start( argptr, optional_argument_flag );
   /* make argptr point to first UNAMED arguments which should be the missing variable */

   /*printf("DEBUG:\tchecking for optional arguments using optional_argument_flag: %d\n", optional_argument_flag );*/

   if (optional_argument_flag)
   {

      /*printf("DEBUG:\toptional arguments are present\n");*/

      /* there are optional arguments 1 - 4, therofore set missing, dx, dy, dz */
      /*missing = va_arg( argptr, float );*/

      /*hack = va_arg( argptr, double );*/
      missing = va_arg( argptr, double );
      dx = va_arg( argptr, float * );
      dy = va_arg( argptr, float * );
      dz = va_arg( argptr, float * );

      /*printf("DEBUG:\toptional arguments are present setting missing value to -->%f<-- and also copying pointers...\n", missing);*/

   }
   else
   {
      /*printf("DEBUG:\tabout to set missing variable\n");*/
      missing = MISSING;
      dx = &dx_dummy;
      dy = &dy_dummy;
      dz = &dz_dummy;
      /*printf("DEBUG:\tNO optional arguments present, setting missing to -->%f<--\n", missing );*/

   }

   /* for field line tracing,etc..., select appropriate variable number ie. bx_cdfNum|by_cdfNum|bz_cdfNum based on *variable_string */
   /* also select appropriate position arrays x, y , z for bx1, by1,bz1 */

   /* set default grid arrays and change if neccessary */

   x_position_array = x_pos;
   y_position_array = y_pos;

   if ( !strcmp(
         variable,
         net_flux_name) )
   {
      cdf_varNum = net_flux_cdf_varNum;
   }
   else if ( !strcmp(
         variable,
         total_flux_name) )
   {
      cdf_varNum = total_flux_cdf_varNum;
   }
   else if ( !strcmp(
         variable,
         weights_name) )
   {
      cdf_varNum = weights_cdf_varNum;
   }
   else
   {
      printf(
            "ERROR:\tcould not find cdf variable number for %s\n",
            variable);
      printf(
            "Valid Variable Names for kpvt ( Kitt Peak Observation Data ):\n%s"
            "\n%s\n%s\n----------------------------------------------\n",
            net_flux_name,
            total_flux_name,
            weights_name);
      return 0;
   }

   if ( DEBUG_FLAG)
   {
      printf(
            "DEBUG:\tabout to call external hunt routine using indices -----> "
            "%d, %d, %d \n",
            index_x,
            index_y,
            index_z);
      printf(
            "DEBUG:\tx[0] = %f, nx = %d, X = %f\n",
            x_position_array[0],
            nx,
            X);
      printf(
            "DEBUG:\ty[0] = %f, ny = %d, Y = %f\n",
            y_position_array[0],
            ny,
            Y);
   }

   hunt(
         x_position_array,
         nx,
         X,
         &index_x);
   hunt(
         y_position_array,
         ny,
         Y,
         &index_y);

   if ( DEBUG_FLAG)
      printf("DEBUG\tback from hunt\n");

   /* index_* points to the cell to the left below the current position OR indicates that the position is out of range by returning index* = -1 or n* */

   if ( (index_x < 0 ) || (index_x >= nx-1 ) || (index_y < 0 ) || (index_y
         >= ny-1 ))
   {
      return (missing );
   }
   else
   {

      if ( DEBUG_FLAG)
         printf("DEBUG:\thunt calls completed successfully\n");

      if ( DEBUG_FLAG)
         printf(
               "DEBUG:\tabout to call interpolate_in_block for %s - using "
               "indices -----> %d, %d, %d \n",
               variable,
               index_x,
               index_y,
               index_z);

      interpolated_value = interpolate_in_block_kpvt(
            X,
            Y,
            Z,
            index_x,
            index_y,
            index_z,
            x_position_array,
            y_position_array,
            z_position_array,
            variable,
            dx,
            dy,
            dz,
            cdf_varNum,
            main_memory_flag);

      /*
       interpolated_value = interpolate_in_block_kpvt( Y, X, Z, index_y, index_z, index_z, y_position_array, x_position_array, z_position_array, variable, dy, dx, dz, cdf_varNum, main_memory_flag );
       */

      if ( DEBUG_FLAG)
         printf("DEBUG:\tcall to interpolate_in_block complete\n");

      /*return interpolated_value;*/

      if (interpolated_value < MIN_RANGE || interpolated_value > MAX_RANGE)
      {
         return missing;
      }
      else
      {
         return interpolated_value;
      }
   }

}

/**************** interpolate_in_block *****************************************/

float interpolate_in_block_kpvt(
      float x,
      float y,
      float z,
      int ix,
      int iy,
      int iz,
      float *x_pos,
      float *y_pos,
      float *z_pos,
      char *variable_name,
      float *dx,
      float *dy,
      float *dz,
      int cdf_varNum,
      int main_memory_flag)
{

   CDFstatus status;

   long recordStart = 0;
   long indices[1];

   float value;
   float dx_blk, dy_blk, m_x, m_y;
   float data_1, data_2, data_3, data_4; /** values from actual data used for interploation equation **/

   dx_blk = x_pos[ ix + 1 ] - x_pos[ ix ];
   dy_blk = y_pos[ iy + 1 ] - y_pos[ iy ];

   if ( DEBUG_FLAG)
      printf(
            "DEBUG\tdx = %f, dy = %f\n",
            dx_blk,
            dy_blk);

   *dx = dx_blk;
   *dy = dy_blk;
   *dz = 0;

   if ( DEBUG_FLAG)
      printf(
            "DEBUG\tdx = %f\n\tdy = %f\nix = %d\niy = %d\n",
            dx_blk,
            dy_blk,
            ix,
            iy);

   m_x = (x - x_pos[ ix ] ) / dx_blk;
   m_y = (y - y_pos[ iy ] ) / dy_blk;

   if ( DEBUG_FLAG)
      printf(
            "DEBUG\tmx = %f\n\tmy = %f\n",
            m_x,
            m_y);

   if ( DEBUG_FLAG)
      printf("debug:\tgetting data values from cdf\n");

   if ( !main_memory_flag)
   {

      indices[0] = (ix * ny + iy );
      status = CDFlib(
      SELECT_, zVAR_, cdf_varNum,
      SELECT_, zVAR_RECNUMBER_, recordStart,
      SELECT_, zVAR_DIMINDICES_, indices,
      GET_, zVAR_DATA_, &data_1,
      NULL_);
      if (status != CDF_OK)
         StatusHandler(status);

      indices[0] = ( (ix+1) * ny + iy );
      status = CDFlib(
      SELECT_, zVAR_, cdf_varNum,
      SELECT_, zVAR_RECNUMBER_, recordStart,
      SELECT_, zVAR_DIMINDICES_, indices,
      GET_, zVAR_DATA_, &data_2,
      NULL_);
      if (status != CDF_OK)
         StatusHandler(status);

      indices[0] = ( (ix + 1) * ny + (iy + 1) );
      status = CDFlib(
      SELECT_, zVAR_, cdf_varNum,
      SELECT_, zVAR_RECNUMBER_, recordStart,
      SELECT_, zVAR_DIMINDICES_, indices,
      GET_, zVAR_DATA_, &data_3,
      NULL_);
      if (status != CDF_OK)
         StatusHandler(status);

      indices[0] = (ix * ny + (iy + 1) );
      status = CDFlib(
      SELECT_, zVAR_, cdf_varNum,
      SELECT_, zVAR_RECNUMBER_, recordStart,
      SELECT_, zVAR_DIMINDICES_, indices,
      GET_, zVAR_DATA_, &data_4,
      NULL_);
      if (status != CDF_OK)
         StatusHandler(status);

   }
   else
   {

      if ( !strcmp(
            variable_name,
            net_flux_name) )
      {
         data_1 = net_flux[ ( ix * nx + iy ) ];
         data_2 = net_flux[ ( (ix+1) * nx + iy ) ];
         data_3 = net_flux[ ( (ix + 1) * nx + (iy + 1) ) ];
         data_4 = net_flux[ ( ix * nx + (iy + 1) ) ];
      }
      else if ( !strcmp(
            variable_name,
            total_flux_name) )
      {
         data_1 = total_flux[ ( ix * nx + iy ) ];
         data_2 = total_flux[ ( (ix+1) * nx + iy ) ];
         data_3 = total_flux[ ( (ix + 1) * nx + (iy + 1) ) ];
         data_4 = total_flux[ ( ix * nx + (iy + 1) ) ];
      }
      else if ( !strcmp(
            variable_name,
            weights_name) )
      {
         data_1 = weights[ ( ix * nx + iy ) ];
         data_2 = weights[ ( (ix+1) * nx + iy ) ];
         data_3 = weights[ ( (ix + 1) * nx + (iy + 1) ) ];
         data_4 = weights[ ( ix * nx + (iy + 1) ) ];
      }
      else
      {
         /* INSERT ERROR HANDLER HERE */
         printf("ERROR\tcouldn't find data variable...\n");
      }

   }

   if ( DEBUG_FLAG)
   {
      printf("debug:\tfinished getting data values from cdf\n");
      printf(
            "DEBUG\tdata 1..4 = %f %f %f %f \n",
            data_1,
            data_2,
            data_3,
            data_4);
   }

   /** bi-linear interpolation **/

   value = ( ( 1 - m_x ) * ( 1 - m_y ) * data_1 )
         + (m_x * ( 1 - m_y ) * data_2 ) + (m_x * m_y * data_3 ) + ( ( 1 - m_x )
         * m_y * data_4 );

   return (value );

}

