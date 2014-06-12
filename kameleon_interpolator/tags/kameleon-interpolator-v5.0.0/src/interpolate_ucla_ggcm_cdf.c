/*******************************************************************************
 *                                                                             *
 *       NAME:          interpolate_ucla_ggcm_cdf.c                            *
 *                                                                             *
 *       VERSION:       1.0                                                    *
 *                                                                             *
 *       AUTHOR:        Marlo M. H. Maddox                                     *
 *                      NASA-GSFC-AETD-ISD-ADMA(587)-CCMC                      *
 *                      Marlo.Maddox@nasa.gov                                  *
 *                      (301) 286 - 5202                                       *
 *                                                                             *
 *       PURPOSE:       Retrun an interpolated variable value for any given    *
 *                      position in the simulation box.  Requires a file to    *
 *                      have alreday been opened with open_cdf call            *
 *                                                                             *
 *       INPUTS:        (1)  variable name (2-4) x,y,z posotion (5)mem         *
 *                      flag (6)opt arg flag                                   *
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
 *       05.16.2005     Maddox, Marlo                                          *
 *                      Add conversion from GSE_JR to GSE by reversing x,y     *
 *                                                                             *
 *       07.28.2005     Maddox, Marlo                                          *
 *                      Fixed MISSING/missing return value and also changed    *
 *                      range limits from nx,ny,nz to nx-1,ny-1,nz-1           *
 *                                                                             *
 *       08.26.2005     Maddox, Marlo                                          *
 *                      Changed interpolated_value flag operation from 1 to 1.0*
 *                                                                             *
 *       07.20.2006     Maddox, Marlo                                          *
 *                      Added more verbose error message for variable names    *
 *                      that are not found - prints list of valid variables    *
 *                      names now                                              *
 *                                                                             *
 *       10.05.2006     Maddox, Marlo                                          *
 *                      Adding a main_memory_flag check/overide control.       *
 *                      Function wil call is_var_in_memory() and set mem_flag  *
 *                      based on return value.                                 *
 *                                                                             *
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

#define MISSING        -1.e31
#define MIN_RANGE    -1e9
#define MAX_RANGE    +1e9

float interpolate_ucla_ggcm_cdf(
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
   int change_sign_flag = 0;

   float interpolated_value;

   float *x_position_array, *y_position_array, *z_position_array; /* generic array pointers that will be set to cooresponding ucla grid according to which variable is being interpolated */

   int index_x = -1, index_y = -1, index_z = -1; /* used in numerical recipes hunt function */

   float dx_dummy, dy_dummy, dz_dummy; /* dummy variables mem locs to set if no real mem locs are input as optional args */

   /* create variables to pass on to interpolate_in_block regardless if no optional arguments exist ( so interpolate_in_block can have a fixed arg list */
   float missing;
   float *dx, *dy, *dz;

   va_list argptr; /* create a pointer to variable list of arguments */

   float interpolate_in_block_ucla_ggcm(
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

   /*** convert position form GSE to GSE_JR ***/

   X = X * ( -1.0 );
   Y = Y * ( -1.0 );

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
   z_position_array = z_pos;

   if ( !strcmp(
         variable,
         bx_name) )
   {
      cdf_varNum = bx_cdfNum;
      change_sign_flag = 1;
   }
   else if ( !strcmp(
         variable,
         by_name) )
   {
      cdf_varNum = by_cdfNum;
      change_sign_flag = 1;
   }
   else if ( !strcmp(
         variable,
         bz_name) )
   {
      cdf_varNum = bz_cdfNum;
   }
   else if ( !strcmp(
         variable,
         bx1_name) )
   {
      cdf_varNum = bx1_cdfNum;
      x_position_array = x_bx_pos;
      y_position_array = y_bx_pos;
      z_position_array = z_bx_pos;
      change_sign_flag = 1;
   }
   else if ( !strcmp(
         variable,
         by1_name) )
   {
      cdf_varNum = by1_cdfNum;
      x_position_array = x_by_pos;
      y_position_array = y_by_pos;
      z_position_array = z_by_pos;
      change_sign_flag = 1;
   }
   else if ( !strcmp(
         variable,
         bz1_name) )
   {
      cdf_varNum = bz1_cdfNum;
      x_position_array = x_bz_pos;
      y_position_array = y_bz_pos;
      z_position_array = z_bz_pos;
   }
   else if ( !strcmp(
         variable,
         ux_name) )
   {
      cdf_varNum = ux_cdfNum;
      change_sign_flag = 1;
   }
   else if ( !strcmp(
         variable,
         uy_name) )
   {
      cdf_varNum = uy_cdfNum;
      change_sign_flag = 1;
   }
   else if ( !strcmp(
         variable,
         uz_name) )
   {
      cdf_varNum = uz_cdfNum;
   }
   else if ( !strcmp(
         variable,
         jx_name) )
   {
      cdf_varNum = jx_cdfNum;
      change_sign_flag = 1;
   }
   else if ( !strcmp(
         variable,
         jy_name) )
   {
      cdf_varNum = jy_cdfNum;
      change_sign_flag = 1;
   }
   else if ( !strcmp(
         variable,
         jz_name) )
   {
      cdf_varNum = jz_cdfNum;
   }
   else if ( !strcmp(
         variable,
         rho_name) )
   {
      cdf_varNum = rho_cdfNum;
   }
   else if ( !strcmp(
         variable,
         p_name) )
   {
      cdf_varNum = p_cdfNum;
   }
   else if ( !strcmp(
         variable,
         eta_name) )
   {
      cdf_varNum = eta_cdfNum;
   }
   else
   {
      printf(
            "ERROR:\tcould not find cdf variable number for %s\n",
            variable);
      printf(
            "Valid Variable Names for OpenGGCM ( UCLA-GGCM ):"
               "\n%s\n%s\n%s\n%s\n%s\n%s\n%s\n%s\n%s\n%s\n%s\n%s\n%s\n%s\n%s"
               "\n----------------------------------------------\n",
            bx_name,
            by_name,
            bz_name,
            bx1_name,
            by1_name,
            bz1_name,
            ux_name,
            uy_name,
            uz_name,
            jx_name,
            jy_name,
            jz_name,
            rho_name,
            p_name,
            eta_name);
      return 0;
   }

   /*
    printf("DEBUG:\tabout to call external hunt routine using indices -----> %d, %d, %d \n", index_x, index_y, index_z);

    printf("DEBUG:\tx[0] = %f, nx = %d, X = %f\n", x_position_array[0], nx, X );
    printf("DEBUG:\ty[0] = %f, ny = %d, Y = %f\n", y_position_array[0], ny, Y );
    printf("DEBUG:\tz[0] = %f, ny = %d, Z = %f\n", z_position_array[0], nz, Z );

    */

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
   hunt(
         z_position_array,
         nz,
         Z,
         &index_z);

   /*printf("DEBUG\tback from hunt\n");*/

   /* index_* point to the cell to the left below the current position OR indicates that the position is out of range by returning index* = -1 or n* */

   if ( (index_x < 0 ) || (index_x >= nx-1 ) || (index_y < 0 ) || (index_y
         >= ny-1 ) || (index_z < 0 ) || (index_z >= nz-1 ))
   {
      return (missing );
   }
   else
   {

      /*printf("DEBUG:\thunt calls completed successfully\n");*/

      /*printf("DEBUG:\tabout to call interpolate_in_block using indices -----> %d, %d, %d \n", index_x, index_y, index_z );*/

      interpolated_value = interpolate_in_block_ucla_ggcm(
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

      /****** we need to change the sign of any x or y vector component to convert value from GSE_JR to GSE *********/

      if (change_sign_flag) /*** this flag is set when cdf_varNum is set above ***/
      {
         interpolated_value = interpolated_value * ( -1.0 );
      }

      /*printf("DEBUG:\tcall to interpolate_in_block complete\n");*/

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

float interpolate_in_block_ucla_ggcm(
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
   float dx_blk, dy_blk, dz_blk, m_x, m_y, m_z;
   float data_1, data_2, data_3, data_4, data_5, data_6, data_7, data_8; /** values from actual data used for interploation equation **/

   /*int ix, iy, iz;*/
   int NV_blk = nx * ny;

   /* int first_cell_in_block = found_block_index * nx * ny * nz; */

   /*printf("interpolating block %d\n", found_block_index );*/

   /*find_in_block( x, y, z, found_block_index, &ix, &iy, &iz, &data_ix, &data_iy, &data_iz );*/

   dx_blk = x_pos[ ix + 1 ] - x_pos[ ix ];
   dy_blk = y_pos[ iy + 1 ] - y_pos[ iy ];
   dz_blk = z_pos[ iz + 1 ] - z_pos[ iz ];

   /*printf("DEBUG\tdx = %f, dy = %f, dz = %f\n", dx_blk,dy_blk,dz_blk );*/

   *dx = dx_blk; /* return values to caller */
   *dy = dy_blk;
   *dz = dz_blk;

   /*printf("DEBUG\tdx = %f\n\tdy = %f\n\tdz = %f\nand ix = %d\niy = %d\niz = %d\n", dx_blk, dy_blk, dz_blk, ix, iy, iz );*/

   m_x = (x - x_pos[ ix ] ) / dx_blk;
   m_y = (y - y_pos[ iy ] ) / dy_blk;
   m_z = (z - z_pos[ iz ] ) / dz_blk;

   /*printf("DEBUG\tmx = %f\n\tmy = %f\n\tmz = %f\n", m_x, m_y, m_z );*/

   /*printf("debug:\tgetting data values from cdf\n");*/

   if ( !main_memory_flag)
   {

      indices[0] = (ix + iy*nx + iz*NV_blk );
      status = CDFlib(
      SELECT_, zVAR_, cdf_varNum,
      SELECT_, zVAR_RECNUMBER_, recordStart,
      SELECT_, zVAR_DIMINDICES_, indices,
      GET_, zVAR_DATA_, &data_1,
      NULL_);
      if (status != CDF_OK)
         StatusHandler(status);

      indices[0] = (ix + 1 + iy*nx + iz*NV_blk );
      status = CDFlib(
      SELECT_, zVAR_, cdf_varNum,
      SELECT_, zVAR_RECNUMBER_, recordStart,
      SELECT_, zVAR_DIMINDICES_, indices,
      GET_, zVAR_DATA_, &data_2,
      NULL_);
      if (status != CDF_OK)
         StatusHandler(status);

      indices[0] = (ix + (iy + 1 )*nx + iz*NV_blk );
      status = CDFlib(
      SELECT_, zVAR_, cdf_varNum,
      SELECT_, zVAR_RECNUMBER_, recordStart,
      SELECT_, zVAR_DIMINDICES_, indices,
      GET_, zVAR_DATA_, &data_3,
      NULL_);
      if (status != CDF_OK)
         StatusHandler(status);

      indices[0] = (ix + 1 + (iy + 1)*nx + iz*NV_blk );
      status = CDFlib(
      SELECT_, zVAR_, cdf_varNum,
      SELECT_, zVAR_RECNUMBER_, recordStart,
      SELECT_, zVAR_DIMINDICES_, indices,
      GET_, zVAR_DATA_, &data_4,
      NULL_);
      if (status != CDF_OK)
         StatusHandler(status);

      indices[0] = (ix + iy*nx + (iz + 1 )*NV_blk );
      status = CDFlib(
      SELECT_, zVAR_, cdf_varNum,
      SELECT_, zVAR_RECNUMBER_, recordStart,
      SELECT_, zVAR_DIMINDICES_, indices,
      GET_, zVAR_DATA_, &data_5,
      NULL_);
      if (status != CDF_OK)
         StatusHandler(status);

      indices[0] = (ix + 1 + iy*nx + (iz + 1 )*NV_blk );
      status = CDFlib(
      SELECT_, zVAR_, cdf_varNum,
      SELECT_, zVAR_RECNUMBER_, recordStart,
      SELECT_, zVAR_DIMINDICES_, indices,
      GET_, zVAR_DATA_, &data_6,
      NULL_);
      if (status != CDF_OK)
         StatusHandler(status);

      indices[0] = (ix + (iy + 1 )*nx + (iz + 1 )*NV_blk );
      status = CDFlib(
      SELECT_, zVAR_, cdf_varNum,
      SELECT_, zVAR_RECNUMBER_, recordStart,
      SELECT_, zVAR_DIMINDICES_, indices,
      GET_, zVAR_DATA_, &data_7,
      NULL_);
      if (status != CDF_OK)
         StatusHandler(status);

      indices[0] = (ix + 1 + (iy + 1)*nx + (iz + 1)*NV_blk );
      status = CDFlib(
      SELECT_, zVAR_, cdf_varNum,
      SELECT_, zVAR_RECNUMBER_, recordStart,
      SELECT_, zVAR_DIMINDICES_, indices,
      GET_, zVAR_DATA_, &data_8,
      NULL_);
      if (status != CDF_OK)
         StatusHandler(status);
   }
   else
   {

      /* reset ix, iy, & iz to conform to the general expressions used above and below were ix = ixt - 1 and now for mem_flag is */
      /* ( block_index_1 * nx ) + ixt - 1 */

      /*
       ix = ix - ( found_block_index * nx );
       iy = iy - ( found_block_index * ny );
       iz = iz - ( found_block_index * nz );
       */

      if ( !strcmp(
            variable_name,
            bx_name) )
      {
         data_1 = bx[ ( ix + iy*nx + iz*NV_blk ) ];
         data_2 = bx[ ( ix + 1 + iy*nx + iz*NV_blk ) ];
         data_3 = bx[ ( ix + ( iy + 1 )*nx + iz*NV_blk ) ];
         data_4 = bx[ ( ix + 1 + ( iy + 1)*nx + iz*NV_blk ) ];
         data_5 = bx[ ( ix + iy*nx + ( iz + 1 )*NV_blk ) ];
         data_6 = bx[ ( ix + 1 + iy*nx + ( iz + 1 )*NV_blk ) ];
         data_7 = bx[ ( ix + ( iy + 1 )*nx + ( iz + 1 )*NV_blk ) ];
         data_8 = bx[ ( ix + 1 + ( iy + 1)*nx + ( iz + 1)*NV_blk ) ];
      }
      else if ( !strcmp(
            variable_name,
            by_name) )
      {
         data_1 = by[ ( ix + iy*nx + iz*NV_blk ) ];
         data_2 = by[ ( ix + 1 + iy*nx + iz*NV_blk ) ];
         data_3 = by[ ( ix + ( iy + 1 )*nx + iz*NV_blk ) ];
         data_4 = by[ ( ix + 1 + ( iy + 1)*nx + iz*NV_blk ) ];
         data_5 = by[ ( ix + iy*nx + ( iz + 1 )*NV_blk ) ];
         data_6 = by[ ( ix + 1 + iy*nx + ( iz + 1 )*NV_blk ) ];
         data_7 = by[ ( ix + ( iy + 1 )*nx + ( iz + 1 )*NV_blk ) ];
         data_8 = by[ ( ix + 1 + ( iy + 1)*nx + ( iz + 1)*NV_blk ) ];
      }
      else if ( !strcmp(
            variable_name,
            bz_name) )
      {
         data_1 = bz[ ( ix + iy*nx + iz*NV_blk ) ];
         data_2 = bz[ ( ix + 1 + iy*nx + iz*NV_blk ) ];
         data_3 = bz[ ( ix + ( iy + 1 )*nx + iz*NV_blk ) ];
         data_4 = bz[ ( ix + 1 + ( iy + 1)*nx + iz*NV_blk ) ];
         data_5 = bz[ ( ix + iy*nx + ( iz + 1 )*NV_blk ) ];
         data_6 = bz[ ( ix + 1 + iy*nx + ( iz + 1 )*NV_blk ) ];
         data_7 = bz[ ( ix + ( iy + 1 )*nx + ( iz + 1 )*NV_blk ) ];
         data_8 = bz[ ( ix + 1 + ( iy + 1)*nx + ( iz + 1)*NV_blk ) ];
      }
      else if ( !strcmp(
            variable_name,
            bx1_name) )
      {
         data_1 = bx1[ ( ix + iy*nx + iz*NV_blk ) ];
         data_2 = bx1[ ( ix + 1 + iy*nx + iz*NV_blk ) ];
         data_3 = bx1[ ( ix + ( iy + 1 )*nx + iz*NV_blk ) ];
         data_4 = bx1[ ( ix + 1 + ( iy + 1)*nx + iz*NV_blk ) ];
         data_5 = bx1[ ( ix + iy*nx + ( iz + 1 )*NV_blk ) ];
         data_6 = bx1[ ( ix + 1 + iy*nx + ( iz + 1 )*NV_blk ) ];
         data_7 = bx1[ ( ix + ( iy + 1 )*nx + ( iz + 1 )*NV_blk ) ];
         data_8 = bx1[ ( ix + 1 + ( iy + 1)*nx + ( iz + 1)*NV_blk ) ];
      }
      else if ( !strcmp(
            variable_name,
            by1_name) )
      {
         data_1 = by1[ ( ix + iy*nx + iz*NV_blk ) ];
         data_2 = by1[ ( ix + 1 + iy*nx + iz*NV_blk ) ];
         data_3 = by1[ ( ix + ( iy + 1 )*nx + iz*NV_blk ) ];
         data_4 = by1[ ( ix + 1 + ( iy + 1)*nx + iz*NV_blk ) ];
         data_5 = by1[ ( ix + iy*nx + ( iz + 1 )*NV_blk ) ];
         data_6 = by1[ ( ix + 1 + iy*nx + ( iz + 1 )*NV_blk ) ];
         data_7 = by1[ ( ix + ( iy + 1 )*nx + ( iz + 1 )*NV_blk ) ];
         data_8 = by1[ ( ix + 1 + ( iy + 1)*nx + ( iz + 1)*NV_blk ) ];
      }
      else if ( !strcmp(
            variable_name,
            bz1_name) )
      {
         data_1 = bz1[ ( ix + iy*nx + iz*NV_blk ) ];
         data_2 = bz1[ ( ix + 1 + iy*nx + iz*NV_blk ) ];
         data_3 = bz1[ ( ix + ( iy + 1 )*nx + iz*NV_blk ) ];
         data_4 = bz1[ ( ix + 1 + ( iy + 1)*nx + iz*NV_blk ) ];
         data_5 = bz1[ ( ix + iy*nx + ( iz + 1 )*NV_blk ) ];
         data_6 = bz1[ ( ix + 1 + iy*nx + ( iz + 1 )*NV_blk ) ];
         data_7 = bz1[ ( ix + ( iy + 1 )*nx + ( iz + 1 )*NV_blk ) ];
         data_8 = bz1[ ( ix + 1 + ( iy + 1)*nx + ( iz + 1)*NV_blk ) ];
      }
      else if ( !strcmp(
            variable_name,
            ux_name) )
      {
         data_1 = ux[ ( ix + iy*nx + iz*NV_blk ) ];
         data_2 = ux[ ( ix + 1 + iy*nx + iz*NV_blk ) ];
         data_3 = ux[ ( ix + ( iy + 1 )*nx + iz*NV_blk ) ];
         data_4 = ux[ ( ix + 1 + ( iy + 1)*nx + iz*NV_blk ) ];
         data_5 = ux[ ( ix + iy*nx + ( iz + 1 )*NV_blk ) ];
         data_6 = ux[ ( ix + 1 + iy*nx + ( iz + 1 )*NV_blk ) ];
         data_7 = ux[ ( ix + ( iy + 1 )*nx + ( iz + 1 )*NV_blk ) ];
         data_8 = ux[ ( ix + 1 + ( iy + 1)*nx + ( iz + 1)*NV_blk ) ];
      }
      else if ( !strcmp(
            variable_name,
            uy_name) )
      {
         data_1 = uy[ ( ix + iy*nx + iz*NV_blk ) ];
         data_2 = uy[ ( ix + 1 + iy*nx + iz*NV_blk ) ];
         data_3 = uy[ ( ix + ( iy + 1 )*nx + iz*NV_blk ) ];
         data_4 = uy[ ( ix + 1 + ( iy + 1)*nx + iz*NV_blk ) ];
         data_5 = uy[ ( ix + iy*nx + ( iz + 1 )*NV_blk ) ];
         data_6 = uy[ ( ix + 1 + iy*nx + ( iz + 1 )*NV_blk ) ];
         data_7 = uy[ ( ix + ( iy + 1 )*nx + ( iz + 1 )*NV_blk ) ];
         data_8 = uy[ ( ix + 1 + ( iy + 1)*nx + ( iz + 1)*NV_blk ) ];
      }
      else if ( !strcmp(
            variable_name,
            uz_name) )
      {
         data_1 = uz[ ( ix + iy*nx + iz*NV_blk ) ];
         data_2 = uz[ ( ix + 1 + iy*nx + iz*NV_blk ) ];
         data_3 = uz[ ( ix + ( iy + 1 )*nx + iz*NV_blk ) ];
         data_4 = uz[ ( ix + 1 + ( iy + 1)*nx + iz*NV_blk ) ];
         data_5 = uz[ ( ix + iy*nx + ( iz + 1 )*NV_blk ) ];
         data_6 = uz[ ( ix + 1 + iy*nx + ( iz + 1 )*NV_blk ) ];
         data_7 = uz[ ( ix + ( iy + 1 )*nx + ( iz + 1 )*NV_blk ) ];
         data_8 = uz[ ( ix + 1 + ( iy + 1)*nx + ( iz + 1)*NV_blk ) ];
      }
      else if ( !strcmp(
            variable_name,
            jx_name) )
      {
         data_1 = jx[ ( ix + iy*nx + iz*NV_blk ) ];
         data_2 = jx[ ( ix + 1 + iy*nx + iz*NV_blk ) ];
         data_3 = jx[ ( ix + ( iy + 1 )*nx + iz*NV_blk ) ];
         data_4 = jx[ ( ix + 1 + ( iy + 1)*nx + iz*NV_blk ) ];
         data_5 = jx[ ( ix + iy*nx + ( iz + 1 )*NV_blk ) ];
         data_6 = jx[ ( ix + 1 + iy*nx + ( iz + 1 )*NV_blk ) ];
         data_7 = jx[ ( ix + ( iy + 1 )*nx + ( iz + 1 )*NV_blk ) ];
         data_8 = jx[ ( ix + 1 + ( iy + 1)*nx + ( iz + 1)*NV_blk ) ];
      }
      else if ( !strcmp(
            variable_name,
            jy_name) )
      {
         data_1 = jy[ ( ix + iy*nx + iz*NV_blk ) ];
         data_2 = jy[ ( ix + 1 + iy*nx + iz*NV_blk ) ];
         data_3 = jy[ ( ix + ( iy + 1 )*nx + iz*NV_blk ) ];
         data_4 = jy[ ( ix + 1 + ( iy + 1)*nx + iz*NV_blk ) ];
         data_5 = jy[ ( ix + iy*nx + ( iz + 1 )*NV_blk ) ];
         data_6 = jy[ ( ix + 1 + iy*nx + ( iz + 1 )*NV_blk ) ];
         data_7 = jy[ ( ix + ( iy + 1 )*nx + ( iz + 1 )*NV_blk ) ];
         data_8 = jy[ ( ix + 1 + ( iy + 1)*nx + ( iz + 1)*NV_blk ) ];
      }
      else if ( !strcmp(
            variable_name,
            jz_name) )
      {
         data_1 = jz[ ( ix + iy*nx + iz*NV_blk ) ];
         data_2 = jz[ ( ix + 1 + iy*nx + iz*NV_blk ) ];
         data_3 = jz[ ( ix + ( iy + 1 )*nx + iz*NV_blk ) ];
         data_4 = jz[ ( ix + 1 + ( iy + 1)*nx + iz*NV_blk ) ];
         data_5 = jz[ ( ix + iy*nx + ( iz + 1 )*NV_blk ) ];
         data_6 = jz[ ( ix + 1 + iy*nx + ( iz + 1 )*NV_blk ) ];
         data_7 = jz[ ( ix + ( iy + 1 )*nx + ( iz + 1 )*NV_blk ) ];
         data_8 = jz[ ( ix + 1 + ( iy + 1)*nx + ( iz + 1)*NV_blk ) ];
      }
      else if ( !strcmp(
            variable_name,
            rho_name) )
      {

         /*printf("DEBUG\tgetting rho data_1-8\n");*/

         data_1 = rho[ ( ix + iy*nx + iz*NV_blk ) ];
         data_2 = rho[ ( ix + 1 + iy*nx + iz*NV_blk ) ];
         data_3 = rho[ ( ix + ( iy + 1 )*nx + iz*NV_blk ) ];
         data_4 = rho[ ( ix + 1 + ( iy + 1)*nx + iz*NV_blk ) ];
         data_5 = rho[ ( ix + iy*nx + ( iz + 1 )*NV_blk ) ];
         data_6 = rho[ ( ix + 1 + iy*nx + ( iz + 1 )*NV_blk ) ];
         data_7 = rho[ ( ix + ( iy + 1 )*nx + ( iz + 1 )*NV_blk ) ];
         data_8 = rho[ ( ix + 1 + ( iy + 1)*nx + ( iz + 1)*NV_blk ) ];

         /*printf("DEBUG\tdata 1..8 = %f %f %f %f %f %f %f %f\n", data_1, data_2, data_3, data_4, data_5, data_6, data_7, data_8 );*/

      }
      else if ( !strcmp(
            variable_name,
            p_name) )
      {
         data_1 = p[ ( ix + iy*nx + iz*NV_blk ) ];
         data_2 = p[ ( ix + 1 + iy*nx + iz*NV_blk ) ];
         data_3 = p[ ( ix + ( iy + 1 )*nx + iz*NV_blk ) ];
         data_4 = p[ ( ix + 1 + ( iy + 1)*nx + iz*NV_blk ) ];
         data_5 = p[ ( ix + iy*nx + ( iz + 1 )*NV_blk ) ];
         data_6 = p[ ( ix + 1 + iy*nx + ( iz + 1 )*NV_blk ) ];
         data_7 = p[ ( ix + ( iy + 1 )*nx + ( iz + 1 )*NV_blk ) ];
         data_8 = p[ ( ix + 1 + ( iy + 1)*nx + ( iz + 1)*NV_blk ) ];
      }
      else if ( !strcmp(
            variable_name,
            eta_name) )
      {
         data_1 = eta[ ( ix + iy*nx + iz*NV_blk ) ];
         data_2 = eta[ ( ix + 1 + iy*nx + iz*NV_blk ) ];
         data_3 = eta[ ( ix + ( iy + 1 )*nx + iz*NV_blk ) ];
         data_4 = eta[ ( ix + 1 + ( iy + 1)*nx + iz*NV_blk ) ];
         data_5 = eta[ ( ix + iy*nx + ( iz + 1 )*NV_blk ) ];
         data_6 = eta[ ( ix + 1 + iy*nx + ( iz + 1 )*NV_blk ) ];
         data_7 = eta[ ( ix + ( iy + 1 )*nx + ( iz + 1 )*NV_blk ) ];
         data_8 = eta[ ( ix + 1 + ( iy + 1)*nx + ( iz + 1)*NV_blk ) ];
      }
      else
      {
         /* INSERT ERROR HANDLER HERE */
         printf("ERROR\tcouldn't find data variable...\n");
      }

   }

   /*printf("debug:\tfinished getting data values from cdf\n");*/
   /*printf("DEBUG\tdata 1..8 = %f %f %f %f %f %f %f this should be data_8-->%f<--\n", data_1, data_2, data_3, data_4, data_5, data_6, data_7, data_8 );*/

   value = (1-m_z)*( (1-m_y)*( (1-m_x)*data_1 + m_x *data_2 ) + m_y
         *( + (1-m_x)*data_3 + m_x *data_4 ) ) + m_z*( (1-m_y)*( +(1-m_x)
         *data_5 + m_x *data_6 ) + m_y*( +(1-m_x)*data_7 + m_x *data_8 ) );

   return (value );

}

