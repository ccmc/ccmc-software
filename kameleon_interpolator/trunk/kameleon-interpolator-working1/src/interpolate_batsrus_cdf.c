/*******************************************************************************
 *      NAME:        interpolate_batsrus_cdf.c                                 *
 *                                                                             *
 *      VERSION:     1.0                                                       *
 *                                                                             *
 *      AUTHOR:      Marlo M. H. Maddox                                        *
 *                   NASA-GSFC-AETD-ISD-ADMA(587)-CCMC                         *
 *                   Marlo.Maddox@nasa.gov                                     *
 *                   (301) 286 - 5202                                          *
 *                                                                             *
 *      PURPOSE:     Retrun an interpolated variable value for any given       *
 *                   position in the simulation box.  Requires a file to have  *
 *                   alreday been opened with open_cdf call                    *
 *                                                                             *
 *      INPUTS:      (1)  variable name (2-4) x,y,z posotion (5)mem flag (6)opt*
 *                   arg flag                                                  *
 *                                                                             *
 *      OUTPUTS:     unit varibale attribute value                             *
 *                                                                             *
 *      FUNCTIONS:                                                             *
 *                                                                             *
 *      VARIABLES:                                                             *
 *                                                                             *
 *      MODIFICATION                                                           *
 *      HISTORY:                                                               *
 *                                                                             *
 *      04.24.2006   Maddox, Marlo                                             *
 *                   Modofied logic block that determines whether the          *
 *                   interpolator has encoutered a new file/and or position.   *
 *                   THIS determines if a new block needs found                *
 *                                                                             *
 *      07.20.2006   Maddox, Marlo                                             *
 *                   added more verbose error message for variable names that  *
 *                   are not found - prints list of valid variables names now  *
 *                                                                             *
 *      09.xx.2006   Rastaetter, L.                                            *
 *                   Update.......                                             *
 *                                                                             *
 *      09.28.2006   Maddox, Marlo                                             *
 *                   Fixed bug that didn't set dx,dy,dz values if the function *
 *                   noticed the requested position hadn't changed...the if    *
 *                   !new_position logic block was updated tp set the supplied *
 *                   dx,dy,dz arguments to the calling program                 *
 *                                                                             *
 *      10.05.2006   Maddox, Marlo                                             *
 *                   Adding a main_memory_flag check/overide control.          *
 *                   Function wil call is_var_in_memory() and set mem_flag     *
 *                   based on return value.                                    *
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

#define max(a,b) ( (a)>(b) ? (a) : (b) )
#define min(a,b) ( (a)<(b) ? (a) : (b) )

#define DEBUG_FLAG 0

/*********************** interpolate_batsrus_cdf ************************/

float interpolate_batsrus_cdf(
      char *variable,
      float X,
      float Y,
      float Z,
      int main_memory_flag,
      int optional_argument_flag,
      ...)
{

   CDFstatus status;

   extern int is_var_in_memory(
         char *);

   long recordStart = 0;
   long counts[1] =
   { 0 };
   long intervals[1] =
   { 1 };
   long indices[1];
   long cdf_varNum;

   int opt_arg_flag;

   int find_octree_block(
         float,
         float,
         float,
         int,
         int);
   void set_block_min_max(
         int);

   float dx_dummy, dy_dummy, dz_dummy; /* dummy variables mem locs to set if no real mem locs are input as optional args */

   /* create variables to pass on to interpolate_in_block regardless if no optional arguments exist ( so interpolate_in_block can have a fixed arg list */
   float missing;
   float *dx, *dy, *dz;

   va_list argptr; /* create a pointer to variable list of arguments */

   /*** tracking variables to automatically notice new vs old position ***/
   int new_position = 1;
   static int call_count = 0;
   static float previous_x, previous_y, previous_z;

   /********* interpolate_amr_data variables for new interpolation routine/upgrade *******/

   static long int ib = -1, ib_c[8], ix_c[8], iy_c[8], iz_c[8];
   static float xx_c[8], yy_c[8], zz_c[8];
   int ic, new_blk[8], valid;
   float ixx, iyy, izz, dx1, dy1, dz1, dx2, dy2, dz2, data_c[8], d_m1, d_m2,
         yy_c2[4], zz_c2[4];
   static int valid_c[8];
   float XMIN, XMAX, YMIN, YMAX, ZMIN, ZMAX;

   /*extern int is_var_in_memory( char * );*//*Previously Defined */

   /** lets see if requested variable is in memory **/

   main_memory_flag = is_var_in_memory(variable);

   /*************************************************************************************************

    * LOGIC BLOCK to determine if interpolator should search for new block or use previosuly stored

    * position and block

    *************************************************************************************************/

   if (call_count == 0) /*** first time this routine has been called ***/
   {
      previous_x = X;
      previous_y = Y;
      previous_z = Z;
   }
   else if (previous_x == X && previous_y == Y && previous_z == Z
         && !new_cdf_file) /*** not first time routine has been called, see if X, y, z has changed ***/
   { /** also check to see if this is a new file or not. **/
      new_position = 0;
   }
   else /** positions have changed or file has changed, store for next iteration **/
   {
      previous_x = X;
      previous_y = Y;
      previous_z = Z;
   }

   /** set new_cdf_file flag to false until open_cdf is called again **/

   new_cdf_file = 0;

   /*printf("%s \tDEBUG\tnew_position = %d\n", __FILE__, new_position );*/

   /**********************************************************************************************/

   counts[0] = 0; /*reset values after once through */
   intervals[0] = 1;

   opt_arg_flag = optional_argument_flag;

   va_start( argptr, optional_argument_flag );
   /* make argptr point to first UNAMED arguments which should be the missing variable */

   /*if( opt_arg_flag )*/
   if (optional_argument_flag)
   {

      /*printf("DEBUG:\toptional arguments are present\n");*/

      /* there are optional arguments 1 - 4, therofore set missing, dx, dy, dz */

      /*hack = va_arg( argptr, double );*/
      missing = va_arg( argptr, double );
      dx = va_arg( argptr, float * );
      dy = va_arg( argptr, float * );
      dz = va_arg( argptr, float * );

      /*

       printf("DEBUG:\toptional arguments are present setting missing value to -->%f<-- and also copying pointers...hack = %f\n", missing, hack );

       printf("DEBUG:\t******** value of copied pointers ****** dx = %p, dy = %p,dz = %p\n", dx, dy, dz );

       */

   }
   else
   {
      /*printf("DEBUG:\tabout to set missing variable\n");*/
      missing = -256.*256.*256.*256.*256.; /* -10000; */
      dx = &dx_dummy;
      dy = &dy_dummy;
      dz = &dz_dummy;
      /*printf("DEBUG:\tNO optional arguments present, setting missing to -->%f<--\n", missing );*/

   }

   /* for field line tracing,etc..., select appropriate variable number ie. bx_cdfNum|by_cdfNum|bz_cdfNum based on *variable_string */

   if ( !strcmp(
         variable,
         bx_name) )
   {
      cdf_varNum = bx_cdfNum;
   }
   else if ( !strcmp(
         variable,
         by_name) )
   {
      cdf_varNum = by_cdfNum;
   }
   else if ( !strcmp(
         variable,
         bz_name) )
   {
      cdf_varNum = bz_cdfNum;
   }
   else if ( !strcmp(
         variable,
         b1x_name) )
   {
      cdf_varNum = b1x_cdfNum;
   }
   else if ( !strcmp(
         variable,
         b1y_name) )
   {
      cdf_varNum = b1y_cdfNum;
   }
   else if ( !strcmp(
         variable,
         b1z_name) )
   {
      cdf_varNum = b1z_cdfNum;
   }
   else if ( !strcmp(
         variable,
         ux_name) )
   {
      cdf_varNum = ux_cdfNum;
   }
   else if ( !strcmp(
         variable,
         uy_name) )
   {
      cdf_varNum = uy_cdfNum;
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
   }
   else if ( !strcmp(
         variable,
         jy_name) )
   {
      cdf_varNum = jy_cdfNum;
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
         e_name) )
   {
      cdf_varNum = e_cdfNum;
   }
   else if ( !strcmp(
         variable,
         bats_status_name) )
   {
      cdf_varNum = bats_status_cdfNum;
   }
   else
   {
      printf(
            "ERROR:\tcould not find cdf variable number for %s\n",
            variable);
      printf(
            "Valid Variable Names for BATSRUS:\n%s\n%s\n%s\n%s\n%s\n%s\n%s\n%s"
            "\n%s\n%s\n%s\n%s\n%s\n%s\n%s\n%s\n-------------------------------------"
            "---------\n",
            bx_name,
            by_name,
            bz_name,
            b1x_name,
            b1y_name,
            b1z_name,
            ux_name,
            uy_name,
            uz_name,
            jx_name,
            jy_name,
            jz_name,
            rho_name,
            p_name,
            e_name,
            bats_status_name);
      return 0;
   }

   /************** NEW INTERPOLATION ROUTINE & MODIFICATION ***************/

   if (new_position)
   {
      ib = find_octree_block(
            X,
            Y,
            Z,
            -1,
            main_memory_flag);

      if (ib == -1)
      {
         return missing;
      }

      /*** set the BLOCK MIN/MAX values depending on main_memory flag ***/

      if (main_memory_flag)
      {
         XMIN = block_x_min_array[ ib ];
         XMAX = block_x_max_array[ ib ];
         YMIN = block_y_min_array[ ib ];
         YMAX = block_y_max_array[ ib ];
         ZMIN = block_z_min_array[ ib ];
         ZMAX = block_z_max_array[ ib ];

      }
      else
      {
         set_block_min_max(ib);
         XMIN = block_x_min;
         XMAX = block_x_max;
         YMIN = block_y_min;
         YMAX = block_y_max;
         ZMIN = block_z_min;
         ZMAX = block_z_max;
      }

      /********** DEBUG

       rintf("DEBUG:\tBLOCK X MIN = %f\n", XMIN );

       rintf("DEBUG:\tBLOCK X MAX = %f\n", XMAX );

       rintf("DEBUG:\tBLOCK Y MIN = %f\n", YMIN );

       rintf("DEBUG:\tBLOCK Y MAX = %f\n", YMAX );

       rintf("DEBUG:\tBLOCK Z MIN = %f\n", ZMIN );

       rintf("DEBUG:\tBLOCK Z MAX = %f\n", ZMAX );

       ****************/

      /*** set delta's and sample xyz positions ***/

      dx1 = (XMAX - XMIN ) / NX;
      dy1 = (YMAX - YMIN ) / NY;
      dz1 = (ZMAX - ZMIN ) / NZ;

      ixx = (X - XMIN )/ dx1 - 0.5;
      iyy = (Y - YMIN )/ dy1 - 0.5;
      izz = (Z - ZMIN )/ dz1 - 0.5;

      /* return delta values to calling program */
      *dx=dx1;
      *dy=dy1;
      *dz=dz1;

      /********** DEBUG

       rintf("nx = %d,ny = %d, nz = %d\n", nx,ny,nz );

       rintf("NX = %f,NY = %f, NZ = %f\n", NX,NY,NZ );

       rintf("dx1 = %f,dy1 = %f, dz1 = %f\n", dx1,dy1,dz1 );

       rintf("ixx = %f,iyy = %f, izz = %f\n", ixx,iyy,izz );

       ************/

      /*** indices of grid positions around sample locations ***/

      ix_c[0] = ix_c[2] = ix_c[4] = ix_c[6] = floor(ixx);
      ix_c[1] = ix_c[3] = ix_c[5] = ix_c[7] = floor(ixx + 1);
      iy_c[0] = iy_c[1] = iy_c[4] = iy_c[5] = floor(iyy);
      iy_c[2] = iy_c[3] = iy_c[6] = iy_c[7] = floor(iyy + 1);
      iz_c[0] = iz_c[1] = iz_c[2] = iz_c[3] = floor(izz);
      iz_c[4] = iz_c[5] = iz_c[6] = iz_c[7] = floor(izz + 1);

      for (ic = 0; ic < 8; ic++)
      {

         ib_c[ic]=ib;

         xx_c[ ic ] = XMIN + ( 0.5 + ix_c[ ic ] ) * dx1;
         yy_c[ ic ] = YMIN + ( 0.5 + iy_c[ ic ] ) * dy1;
         zz_c[ ic ] = ZMIN + ( 0.5 + iz_c[ ic ] ) * dz1;

         new_blk[ic]=0;

         /* validate positions in block and find new block as necessary */

         if (ix_c[ ic ] < 0.0)
         {
            new_blk[ic]=1;
            xx_c[ ic ] = XMIN - 0.5 * dx1;
         }
         else
         {
            if (ix_c[ ic ] > (NX - 0.5 ))
            {
               new_blk[ic]=1;
               xx_c[ ic ] = XMAX + 0.5 * dx1;
            }
            else
            {
               xx_c[ ic ] = XMIN + ( 0.5 + ix_c[ ic ]) * dx1;
            }

         }

         if (iy_c[ic] < 0)
         {
            new_blk[ic]=1;
            yy_c[ ic ] = YMIN - 0.5 * dy1;
         }
         else
         {
            if (iy_c[ ic ] > (NY - 1 ))
            {
               new_blk[ ic ] = 1;
               yy_c[ ic ] = YMAX + 0.5 * dy1;
            }
            else
            {
               yy_c[ ic ] = YMIN + ( 0.5 + iy_c[ ic ]) * dy1;
            }
         }

         if (iz_c[ic] < 0)
         {
            new_blk[ic]=1;
            zz_c[ ic ] = ZMIN - 0.5 * dz1;
         }
         else
         {
            if (iz_c[ ic ] > (NZ - 1 ))
            {
               new_blk[ ic ] = 1;
               zz_c[ ic ] = ZMAX + 0.5 * dz1;
            }
            else
            {
               zz_c[ ic ] = ZMIN + ( 0.5 + iz_c[ ic ]) * dz1;
            }
         }

      } /*** end for ( ic = 0; ic < 8; ic++ ) loop ***/

      for (ic = 0; ic < 8; ic++)
      {
         valid_c[ ic ]=1;
         if (new_blk[ ic ])
         {
            long ibc;
            ibc = find_octree_block(
                  xx_c[ ic ],
                  yy_c[ ic ],
                  zz_c[ ic ],
                  -1L,
                  main_memory_flag);

            /* ; now snap into position at new block cell */

            if (ibc != -1)
            {
               ib_c[ ic ] = ibc; /* change block number */

               /*** set the BLOCK MIN/MAX values depending on main_memory flag ***/

               if (main_memory_flag)
               {
                  XMIN = block_x_min_array[ ibc ];
                  XMAX = block_x_max_array[ ibc ];
                  YMIN = block_y_min_array[ ibc ];
                  YMAX = block_y_max_array[ ibc ];
                  ZMIN = block_z_min_array[ ibc ];
                  ZMAX = block_z_max_array[ ibc ];
               }
               else
               {
                  set_block_min_max(ibc);
                  XMIN = block_x_min;
                  XMAX = block_x_max;
                  YMIN = block_y_min;
                  YMAX = block_y_max;
                  ZMIN = block_z_min;
                  ZMAX = block_z_max;
               }

               dx2 = (XMAX - XMIN ) / NX;
               dy2 = (YMAX - YMIN ) / NY;
               dz2 = (ZMAX - ZMIN ) / NZ;

               xx_c[ ic ] = X + (ic % 2) * dx2; /* adjust stencil with */
               yy_c[ ic ] = Y + ( (ic % 4) / 2) * dy2; /* resolution change between */
               zz_c[ ic ] = Z + (ic / 4) * dz2; /* neighboring blocks */

               ix_c[ ic ] = min( nx-1, max( 0, floor( ( xx_c[ ic] - XMIN ) / dx2 - 0.5 ) ) );
               iy_c[ ic ] = min( ny-1, max( 0, floor( ( yy_c[ ic] - YMIN ) / dy2 - 0.5 ) ) );
               iz_c[ ic ] = min( nz-1, max( 0, floor( ( zz_c[ ic] - ZMIN ) / dz2 - 0.5 ) ) );
               xx_c[ ic ] = XMIN + dx2 * ( 0.5 + ix_c[ ic ] );
               yy_c[ ic ] = YMIN + dx2 * ( 0.5 + iy_c[ ic ] );
               zz_c[ ic ] = ZMIN + dx2 * ( 0.5 + iz_c[ ic ] );

            }
            else
            {
               valid_c[ic]=0;
               ib_c[ic]=ib;

               /* point to some grid point near the position (xx,yy,zz) */
               ix_c[ ic ] = nx / 2;
               iy_c[ ic ] = ny / 2;
               iz_c[ ic ] = nz / 2;

               /* throw out those points by moving them off */
               xx_c[ ic ] = fabs(missing); /*x_blk[ib*NX+ix_c[ic]]; */
               yy_c[ ic ] = fabs(missing); /*y_blk[ib*NY+iy_c[ic]]; */
               zz_c[ ic ] = fabs(missing); /*z_blk[ib*NZ+iz_c[ic]]; */
            }
         }
      }

   } /** end of if new position loop **/
   else
   {

      /*** if it is not a new position, still calculate the delta as necessary ***/

      /*** set the BLOCK MIN/MAX values depending on main_memory flag ***/

      if (main_memory_flag)
      {
         XMIN = block_x_min_array[ ib ];
         XMAX = block_x_max_array[ ib ];
         YMIN = block_y_min_array[ ib ];
         YMAX = block_y_max_array[ ib ];
         ZMIN = block_z_min_array[ ib ];
         ZMAX = block_z_max_array[ ib ];

      }
      else
      {
         set_block_min_max(ib);
         XMIN = block_x_min;
         XMAX = block_x_max;
         YMIN = block_y_min;
         YMAX = block_y_max;
         ZMIN = block_z_min;
         ZMAX = block_z_max;
      }

      /*** set delta's and sample xyz positions ***/

      dx1 = (XMAX - XMIN ) / NX;
      dy1 = (YMAX - YMIN ) / NY;
      dz1 = (ZMAX - ZMIN ) / NZ;

      /* set delta values from calling program */
      *dx=dx1;
      *dy=dy1;
      *dz=dz1;

      if (ib == -1L) /*** outside of range ***/
      {
         *dx=0.;
         *dy=0.;
         *dz=0.;
         return missing;
      }
   }

   /*********************************************************************

    *                            retreive the data                        **

    ********************************************************************/

   for (ic = 0; ic < 8; ic++)
   {

      float temp_space;

      if ( !main_memory_flag)
      {

         indices[0] = ix_c[ ic ] + nx * (iy_c[ ic ] + ny * (iz_c[ic] + nz
               * ib_c[ ic] ) );

         status = CDFlib(
         SELECT_, zVAR_, cdf_varNum,
         SELECT_, zVAR_RECNUMBER_, recordStart,
         SELECT_, zVAR_DIMINDICES_, indices,
         GET_, zVAR_DATA_, &temp_space,
         NULL_);
         if (status != CDF_OK)
            StatusHandler(status);

         data_c[ ic ] = temp_space;

      }
      else
      {

         /********* NOTE to SELF:  Put variable name in an array or something next time, but since this works.... ***********/

         if ( !strcmp(
               variable,
               bx_name) )
         {
            data_c[ ic ] = bx[ ix_c[ ic ] + nx * ( iy_c[ ic ] + ny * ( iz_c[ic] + nz * ib_c[ ic] ) ) ];
         }
         else if ( !strcmp(
               variable,
               by_name) )
         {
            data_c[ ic ] = by[ ix_c[ ic ] + nx * ( iy_c[ ic ] + ny * ( iz_c[ic] + nz * ib_c[ ic] ) ) ];
         }
         else if ( !strcmp(
               variable,
               bz_name) )
         {
            data_c[ ic ] = bz[ ix_c[ ic ] + nx * ( iy_c[ ic ] + ny * ( iz_c[ic] + nz * ib_c[ ic] ) ) ];
         }
         else if ( !strcmp(
               variable,
               b1x_name) )
         {
            data_c[ ic ] = b1x[ ix_c[ ic ] + nx * ( iy_c[ ic ] + ny * ( iz_c[ic] + nz * ib_c[ ic] ) ) ];
         }
         else if ( !strcmp(
               variable,
               b1y_name) )
         {
            data_c[ ic ] = b1y[ ix_c[ ic ] + nx * ( iy_c[ ic ] + ny * ( iz_c[ic] + nz * ib_c[ ic] ) ) ];
         }
         else if ( !strcmp(
               variable,
               b1z_name) )
         {
            data_c[ ic ] = b1z[ ix_c[ ic ] + nx * ( iy_c[ ic ] + ny * ( iz_c[ic] + nz * ib_c[ ic] ) ) ];
         }
         else if ( !strcmp(
               variable,
               ux_name) )
         {
            data_c[ ic ] = ux[ ix_c[ ic ] + nx * ( iy_c[ ic ] + ny * ( iz_c[ic] + nz * ib_c[ ic] ) ) ];
         }
         else if ( !strcmp(
               variable,
               uy_name) )
         {
            data_c[ ic ] = uy[ ix_c[ ic ] + nx * ( iy_c[ ic ] + ny * ( iz_c[ic] + nz * ib_c[ ic] ) ) ];
         }
         else if ( !strcmp(
               variable,
               uz_name) )
         {
            data_c[ ic ] = uz[ ix_c[ ic ] + nx * ( iy_c[ ic ] + ny * ( iz_c[ic] + nz * ib_c[ ic] ) ) ];
         }
         else if ( !strcmp(
               variable,
               jx_name) )
         {
            data_c[ ic ] = jx[ ix_c[ ic ] + nx * ( iy_c[ ic ] + ny * ( iz_c[ic] + nz * ib_c[ ic] ) ) ];
         }
         else if ( !strcmp(
               variable,
               jy_name) )
         {
            data_c[ ic ] = jy[ ix_c[ ic ] + nx * ( iy_c[ ic ] + ny * ( iz_c[ic] + nz * ib_c[ ic] ) ) ];
         }
         else if ( !strcmp(
               variable,
               jz_name) )
         {
            data_c[ ic ] = jz[ ix_c[ ic ] + nx * ( iy_c[ ic ] + ny * ( iz_c[ic] + nz * ib_c[ ic] ) ) ];
         }
         else if ( !strcmp(
               variable,
               rho_name) )
         {
            data_c[ ic ] = rho[ ix_c[ ic ] + nx * ( iy_c[ ic ] + ny * ( iz_c[ic] + nz * ib_c[ ic] ) ) ];
         }
         else if ( !strcmp(
               variable,
               p_name) )
         {
            data_c[ ic ] = p[ ix_c[ ic ] + nx * ( iy_c[ ic ] + ny * ( iz_c[ic] + nz * ib_c[ ic] ) ) ];
         }
         else if ( !strcmp(
               variable,
               e_name) )
         {
            data_c[ ic ] = e[ ix_c[ ic ] + nx * ( iy_c[ ic ] + ny * ( iz_c[ic] + nz * ib_c[ ic] ) ) ];
         }
         else
         {
            /* INSERT ERROR HANDLER HERE */
            printf("ERROR\tcouldn't find data variable...\n");
         }

      } /*  end of if/else main_memory flag  - */

   } /* end of for( ic = 0; ic < 8; ic++) loop */

   /************** per Lutz do pair-wise interpolations in x,y, & z directions *********/

   for (ic = 0; ic < 4; ic++)
   {
      int ic2, ic2_1;
      ic2 = 2 * ic;
      ic2_1 = ic2 + 1;
      d_m1 = (X - xx_c[ ic2 ] );
      d_m2 = (xx_c[ ic2_1 ] - X );

      if (valid_c[ic2] && valid_c[ic2_1] && (fabs(d_m1+d_m2) > (dx1/4.) ))
      /*      if( d_m1 * d_m2 > 0 ) */
      {

         data_c[ ic ] = (d_m2 * data_c[ ic2] + d_m1 * data_c[ ic2_1 ] ) / (d_m1
               + d_m2 );

         yy_c2[ ic ] = (d_m2 * yy_c[ ic2 ] + d_m1 * yy_c[ ic2_1 ] ) / (d_m1
               + d_m2 );

         zz_c2[ ic ] = (d_m2 * zz_c[ ic2 ] + d_m1 * zz_c[ ic2_1 ] ) / (d_m1
               + d_m2 );
         valid=1;
      }
      else
      {
         valid=0;
         /*            if( fabs( d_m1 ) > fabs( d_m2 ) ) */
         if (valid_c[ic2_1])
         {
            data_c[ ic ] = data_c[ ic2_1 ];

            yy_c2[ ic ] = yy_c[ ic2_1 ];

            zz_c2[ ic ] = zz_c[ ic2_1 ];
            valid=1;
         }
         /*            }

          else

          { */
         if (valid_c[ic2])
         {
            data_c[ ic ] = data_c[ ic2 ];

            yy_c2[ ic ] = yy_c[ ic2 ];

            zz_c2[ ic ] = zz_c[ ic2 ];
            valid=1;
         }
      }
      valid_c[ic]=valid;
   } /* end of for( ic = 0; ic < 4; ic++ ); loop*/

   for (ic = 0; ic < 2; ic++)
   {
      int ic2, ic2_1;
      ic2 = 2 * ic;
      ic2_1 = ic2 + 1;
      d_m1 = (Y - yy_c2[ ic2 ] );
      d_m2 = (yy_c2[ ic2_1 ] - Y );

      /*        if( d_m1 * d_m2 > 0 ) */
      if (valid_c[ic2] && valid_c[ic2_1] && (fabs(d_m1+d_m2) >= (dy1/4.) ))
      {
         zz_c2[ ic ] = (d_m2 * zz_c2[ ic2 ] + d_m1 * zz_c2[ ic2_1 ] ) / (d_m1
               + d_m2 );
         data_c[ ic ] = (d_m2 * data_c[ ic2 ] + d_m1 * data_c[ ic2_1 ] )
               / (d_m1 + d_m2 );
         valid=1;
      }
      else
      {
         valid=0;
         /*            if( fabs( d_m1 ) > fabs( d_m2 ) ) */
         if (valid_c[ic2_1])
         {
            data_c[ ic ] = data_c[ ic2_1 ];
            zz_c2[ ic ] = zz_c2[ ic2_1 ];
            valid=1;
         }
         /*            }

          else

          { */
         if (valid_c[ic2])
         {
            data_c[ ic ] = data_c [ ic2 ];
            zz_c2 [ ic ] = zz_c2[ ic2 ];
            valid=1;
         }
      }
      valid_c[ic]=valid;
   }

   d_m1 = (Z - zz_c2[ 0 ] );
   d_m2 = (zz_c2[ 1 ] - Z );

   /************************************************************************************

    ***************** return result of interpolation into data_c[0] ********************

    ***********************************************************************************/

   /*    if( d_m1 * d_m2 > 0 ) */
   if (valid_c[0] && valid_c[1] && (fabs(d_m1+d_m2) >= (dz1/4.) ))
   {
      data_c[ 0 ] = ( (d_m2 * data_c[ 0 ] + d_m1 * data_c[ 1 ] )
            / (d_m1 + d_m2 ) );
      valid=1;
   }
   else
   {
      /*        if( fabs( d_m1 ) > fabs( d_m2 ) ) */
      valid=0;
      if (valid_c[1])
      {
         data_c[ 0 ] = data_c[ 1 ];
         valid=1;
      }
      if (valid_c[0])
      {
         valid=1;
      }
   }

   call_count++; /* increment the call_count variable */

   if (valid)
   {
      return (data_c[0]);
   }
   else
   {
      return (missing);
   }
   /* return( data_c[ 0 ] ); */

}

/****************************** find_octree_block ******************************/

int find_octree_block(
      float x,
      float y,
      float z,
      int old_block_number,
      int main_memory_flag)
{

   int climb_octree(
         int,
         float,
         float,
         float,
         int);

   void set_block_min_max(
         int);

   int block_index_1, block_index_2, root_index;

   block_index_1 = old_block_number;

   block_index_2 = -1;
   root_index = 0;

   /****** when main memory flag is set, retreive all required data values from main memory

    data loaded with open_cdf routine ********/

   if (main_memory_flag)
   {

      /*** we may be in the same block used previously, if so check first to increase performance ***/

      if (old_block_number != -1)
      {
         if ( (block_x_min_array[block_index_1] <= x )
               && (block_x_max_array[block_index_1] >= x )
               && (block_y_min_array[block_index_1] <= y )
               && (block_y_max_array[block_index_1] >= y )
               && (block_z_min_array[block_index_1] <= z )
               && (block_z_max_array[block_index_1] >= z ))
         {
            return (block_index_1 );
         }
      }

      /*** if there are no amr levels defined, our block tree structure is useless - do an old fashioned linear search ***/

      if (number_of_parents_at_amr_level == NULL)
      {

         for (block_index_1 = 0; block_index_1 < number_of_blocks; block_index_1++)
         {
            if ( (block_x_min_array[block_index_1] <= x )
                  && (block_x_max_array[block_index_1] >= x )
                  && (block_y_min_array[block_index_1] <= y )
                  && (block_y_max_array[block_index_1] >= y )
                  && (block_z_min_array[block_index_1] <= z )
                  && (block_z_max_array[block_index_1] >= z ))
            {
               return (block_index_1 );
            }
         }

         /**** no block was found ****/

         printf("ERROR:\tlinear block search returned no index!\n");

         return ( -1 );

      }

      /*printf("DEBUG\twhile %d < %d && %d == -1\n", root_index ,number_of_parents_at_amr_level[0] , block_index_2 );*/

      while ( (root_index < number_of_parents_at_amr_level[0] )
            && (block_index_2 == -1 ))
      {
         block_index_1 = block_at_amr_level[ root_index ];

         /*

          printf("\n\ntesting block_index_1\t%d\n", block_index_1 );

          printf("\ntesting xmin %f <= %f\n", block_x_min_array[block_index_1], x );

          printf("testing xmax %f >= %f\n", block_x_max_array[block_index_1], x );

          printf("testing ymin %f <= %f\n", block_y_min_array[block_index_1], y );

          printf("testing ymax %f >= %f\n", block_y_max_array[block_index_1], y );

          printf("testing zmin %f <= %f\n", block_z_min_array[block_index_1], z );

          printf("testing zmax %f >= %f\n", block_z_max_array[block_index_1], z );

          */

         if ( (block_x_min_array[block_index_1] <= x )
               && (block_x_max_array[block_index_1] >= x )
               && (block_y_min_array[block_index_1] <= y )
               && (block_y_max_array[block_index_1] >= y )
               && (block_z_min_array[block_index_1] <= z )
               && (block_z_max_array[block_index_1] >= z ))
         {

            block_index_2 = climb_octree(
                  block_index_1,
                  x,
                  y,
                  z,
                  main_memory_flag);
         }
         else
         {
            root_index++;
         }
      }
   }
   else /* read each block min.max for x,y,z one at a time */
   {

      block_index_1 = block_at_amr_level[ root_index ];

      /*printf("searching parent number: %d out of %d located in block %d\n", root_index + 1, number_of_parents_at_amr_level[0], block_index_1 );*/

      set_block_min_max(block_index_1); /*** reads block min/max from cdf file ***/

      /*** we may be in the same block used previously, if so check first to increase performance ***/

      if (old_block_number != -1)
      {
         if ( (block_x_min <= x ) && (block_x_max >= x ) && (block_y_min <= y )
               && (block_y_max >= y ) && (block_z_min <= z ) && (block_z_max
               >= z ))
         {
            return (block_index_1 );
         }
      }

      block_index_2 = -1;
      root_index = 0;

      /*** if there are no amr levels defined, our block tree structure is useless - do an old fashioned linear search ***/

      if (number_of_parents_at_amr_level == NULL)
      {

         for (block_index_1 = 0; block_index_1 < number_of_blocks; block_index_1++)
         {

            set_block_min_max(block_index_1); /*** reads block min/max from cdf file ***/

            if ( (block_x_min <= x ) && (block_x_max >= x ) && (block_y_min
                  <= y ) && (block_y_max >= y ) && (block_z_min <= z )
                  && (block_z_max >= z ))
            {
               return (block_index_1 );
            }
         }

         /**** no block was found ****/

         printf("ERROR:\tlinear block search returned no index!\n");

         return ( -1 );

      }

      /*printf("DEBUG\twhile %d < %d && %d == -1\n", root_index ,number_of_parents_at_amr_level[0] , block_index_2 );*/

      while ( (root_index < number_of_parents_at_amr_level[0] )
            && (block_index_2 == -1 ))
      {

         block_index_1 = block_at_amr_level[ root_index ];

         /*printf("searching parent number: %d out of %d located in block %d\n", root_index + 1, number_of_parents_at_amr_level[0], block_index_1 );*/

         set_block_min_max(block_index_1);

         if ( (block_x_min <= x ) && (block_x_max >= x ) && (block_y_min <= y )
               && (block_y_max >= y ) && (block_z_min <= z ) && (block_z_max
               >= z ))
         {

            block_index_2 = climb_octree(
                  block_index_1,
                  x,
                  y,
                  z,
                  main_memory_flag);
         }
         else
         {
            root_index++;
         }
      }

   }

   return (block_index_2 );

}

/******************** climb_octree ********************************************/

int climb_octree(
      int root,
      float x,
      float y,
      float z,
      int main_memory_flag)
{

   CDFstatus status;

   long recordStart = 0;
   long indices[1];

   int climb_octree(
         int,
         float,
         float,
         float,
         int);

   int ix, iy, iz;

   int child_id, child_key;

   indices[0] = root;

   /*printf("\nclimbing block tree structure...now in block %d\n", root);*/

   /******** if main memory flag is NOT set, read all required data directly from cdf file **********/

   if ( !main_memory_flag)
   {

      status = CDFlib(
      SELECT_, zVAR_, block_child_count_cdfNum,
      SELECT_, zVAR_RECNUMBER_, recordStart,
      SELECT_, zVAR_DIMINDICES_, indices,
      GET_, zVAR_DATA_, &block_child_count,
      NULL_);
      if (status != CDF_OK)
         StatusHandler(status);

      /*printf("block %d has %d children...\n", root, block_child_count );*/

      if (block_child_count != 8)
      {
         return (root );
      }

      /*printf("searching child...\t");*/

      status = CDFlib(
      SELECT_, zVAR_, block_x_center_cdfNum,
      SELECT_, zVAR_RECNUMBER_, recordStart,
      SELECT_, zVAR_DIMINDICES_, indices,
      GET_, zVAR_DATA_, &block_x_center,
      NULL_);
      if (status != CDF_OK)
         StatusHandler(status);

      status = CDFlib(
      SELECT_, zVAR_, block_y_center_cdfNum,
      SELECT_, zVAR_RECNUMBER_, recordStart,
      SELECT_, zVAR_DIMINDICES_, indices,
      GET_, zVAR_DATA_, &block_y_center,
      NULL_);
      if (status != CDF_OK)
         StatusHandler(status);

      status = CDFlib(
      SELECT_, zVAR_, block_z_center_cdfNum,
      SELECT_, zVAR_RECNUMBER_, recordStart,
      SELECT_, zVAR_DIMINDICES_, indices,
      GET_, zVAR_DATA_, &block_z_center,
      NULL_);
      if (status != CDF_OK)
         StatusHandler(status);

      ix = x > block_x_center;
      iy = y > block_y_center;
      iz = z > block_z_center;

   }
   else /******** if main memory flag is set, read all required data from main memory ( previously loaded with open cdf ) **********/
   {

      if (block_child_count_array[root] == 0)
      {
         return (root );
      }

      ix = x > block_x_center_array[root];
      iy = y > block_y_center_array[root];
      iz = z > block_z_center_array[root];
   }

   /********** calculate & retrieve child ID **************/

   child_key = iz*4+2*iy+ix;

   /*printf("DEBUG\tsetting child id key = %d\n", child_key );*/

   indices[0] = root;

   switch (child_key)
   {
      case 0:
         if ( !main_memory_flag)
         {
            status = CDFlib(
            SELECT_, zVAR_, block_child_id_1_cdfNum,
            SELECT_, zVAR_RECNUMBER_, recordStart,
            SELECT_, zVAR_DIMINDICES_, indices,
            GET_, zVAR_DATA_, &child_id,
            NULL_);
            if (status != CDF_OK)
               StatusHandler(status);
         }
         else
         {
            child_id = block_child_id_1_array[root];
         }
         break;
      case 1:
         if ( !main_memory_flag)
         {
            status = CDFlib(
            SELECT_, zVAR_, block_child_id_2_cdfNum,
            SELECT_, zVAR_RECNUMBER_, recordStart,
            SELECT_, zVAR_DIMINDICES_, indices,
            GET_, zVAR_DATA_, &child_id,
            NULL_);
            if (status != CDF_OK)
               StatusHandler(status);
         }
         else
         {
            child_id = block_child_id_2_array[root];
         }
         break;
      case 2:
         if ( !main_memory_flag)
         {
            status = CDFlib(
            SELECT_, zVAR_, block_child_id_3_cdfNum,
            SELECT_, zVAR_RECNUMBER_, recordStart,
            SELECT_, zVAR_DIMINDICES_, indices,
            GET_, zVAR_DATA_, &child_id,
            NULL_);
            if (status != CDF_OK)
               StatusHandler(status);
         }
         else
         {
            child_id = block_child_id_3_array[root];
         }
         break;
      case 3:
         if ( !main_memory_flag)
         {
            status = CDFlib(
            SELECT_, zVAR_, block_child_id_4_cdfNum,
            SELECT_, zVAR_RECNUMBER_, recordStart,
            SELECT_, zVAR_DIMINDICES_, indices,
            GET_, zVAR_DATA_, &child_id,
            NULL_);
            if (status != CDF_OK)
               StatusHandler(status);
         }
         else
         {
            child_id = block_child_id_4_array[root];
         }
         break;
      case 4:
         if ( !main_memory_flag)
         {
            status = CDFlib(
            SELECT_, zVAR_, block_child_id_5_cdfNum,
            SELECT_, zVAR_RECNUMBER_, recordStart,
            SELECT_, zVAR_DIMINDICES_, indices,
            GET_, zVAR_DATA_, &child_id,
            NULL_);
            if (status != CDF_OK)
               StatusHandler(status);
         }
         else
         {
            child_id = block_child_id_5_array[root];
         }
         break;
      case 5:
         if ( !main_memory_flag)
         {
            status = CDFlib(
            SELECT_, zVAR_, block_child_id_6_cdfNum,
            SELECT_, zVAR_RECNUMBER_, recordStart,
            SELECT_, zVAR_DIMINDICES_, indices,
            GET_, zVAR_DATA_, &child_id,
            NULL_);
            if (status != CDF_OK)
               StatusHandler(status);
         }
         else
         {
            child_id = block_child_id_6_array[root];
         }
         break;
      case 6:
         if ( !main_memory_flag)
         {
            status = CDFlib(
            SELECT_, zVAR_, block_child_id_7_cdfNum,
            SELECT_, zVAR_RECNUMBER_, recordStart,
            SELECT_, zVAR_DIMINDICES_, indices,
            GET_, zVAR_DATA_, &child_id,
            NULL_);
            if (status != CDF_OK)
               StatusHandler(status);
         }
         else
         {
            child_id = block_child_id_7_array[root];
         }
         break;
      case 7:
         if ( !main_memory_flag)
         {
            status = CDFlib(
            SELECT_, zVAR_, block_child_id_8_cdfNum,
            SELECT_, zVAR_RECNUMBER_, recordStart,
            SELECT_, zVAR_DIMINDICES_, indices,
            GET_, zVAR_DATA_, &child_id,
            NULL_);
            if (status != CDF_OK)
               StatusHandler(status);
         }
         else
         {
            child_id = block_child_id_8_array[root];
         }
         break;

      default:
         /* do something constructive */
         printf(
               "\n\nERROR cannot select appropriate child_id array for child id %d\n\n",
               child_id);
         return 0;
         break;

   }

   /*printf("DEBUG\trecursive call to climb_octree with child_id = %d\n", child_id );    */

   return (climb_octree(
         child_id,
         x,
         y,
         z,
         main_memory_flag) );

}

/**************** find_in_block **********************************************/

void find_in_block(
      float x,
      float y,
      float z,
      int block_index,
      int *ix,
      int *iy,
      int *iz,
      int *data_ix,
      int *data_iy,
      int *data_iz,
      int main_memory_flag)
{

   CDFstatus status;

   long recordStart = 0;
   long indices[1];
   long counts[1] =
   { 0 };
   long intervals[1] =
   { 1 };

   int i;
   float current_element;
   long indices2[1]=
   { 0 };

   int ixt, iyt, izt;
   int NX1, NY1, NZ1;

   int first_cell_in_block = block_index * nx * ny * nz;

   NX1 = nx - 1;
   NY1 = ny - 1;
   NZ1 = nz - 1;

   ixt = 1;
   iyt = 1;
   izt = 1;

   /*printf("DEBUG:inside find_in_block\tfound_block_index = %d\tnx = %d\tNY = %d\tNZ = %d\n", block_index, nx, ny, nz );*/

   /** read in all uniqiue x position values **/

   /*printf("debug:\tgetting unique x positions for block %d\n", block_index );*/

   /*printf("DEBUG\tindices2 = %d\nrecordStart = %d\n", indices2[0], recordStart );*/

   if ( !main_memory_flag)
   {

      indices[0] = first_cell_in_block; /** begining position for first value of current block **/
      counts[0] = nx;
      intervals[0] = 1;

      for (i = 0; i < nx; i++)
      {
         indices2[0]=indices[0]+i;
         status = CDFlib(
         SELECT_, zVAR_, x_cdf_varNum,
         SELECT_, zVAR_RECNUMBER_, recordStart,
         SELECT_, zVAR_DIMINDICES_, indices2,
         GET_, zVAR_DATA_, &current_element,
         NULL_);
         if (status != CDF_OK)
            StatusHandler(status);

         x_pos[i] = current_element;
      }

      /*printf("debug:\tfinished getting unique x positions for block %d\n", block_index );*/

      /** read in all uniqiue y position values **/

      counts[0] = ny;
      intervals[0] = nx;

      /*printf("debug:\tgetting unique y positions for block %d\n", block_index );*/

      for (i = 0; i < ny; i++)
      {
         indices2[0]=indices[0]+(i * nx );
         status = CDFlib(
         SELECT_, zVAR_, y_cdf_varNum,
         SELECT_, zVAR_RECNUMBER_, recordStart,
         SELECT_, zVAR_DIMINDICES_, indices2,
         GET_, zVAR_DATA_, &current_element,
         NULL_);
         if (status != CDF_OK)
            StatusHandler(status);

         y_pos[i] = current_element;
      }

      /*printf("debug:\tfinished getting unique y positions for block %d\n", block_index );*/

      /** read in all uniqiue z position values **/

      counts[0] = nz;
      intervals[0] = nx*ny;

      /*printf("debug:\tgetting unique z positions for block %d\n", block_index );*/

      for (i = 0; i < nz; i++)
      {
         indices2[0]=indices[0]+(i*nx*ny );
         status = CDFlib(
         SELECT_, zVAR_, z_cdf_varNum,
         SELECT_, zVAR_RECNUMBER_, recordStart,
         SELECT_, zVAR_DIMINDICES_, indices2,
         GET_, zVAR_DATA_, &current_element,
         NULL_);
         if (status != CDF_OK)
            StatusHandler(status);

         z_pos[i] = current_element;
      }

      /*printf("debug:\tfinished getting unique z positions for block %d\n", block_index );*/

      while ( (x_pos[ ixt ] < x ) && (ixt < NX1 ))
      {
         ixt++;
      }
      while ( (y_pos[ iyt ] < y ) && (iyt < NY1 ))
      {
         iyt++;
      }
      while ( (z_pos[ izt ] < z ) && (izt < NZ1 ))
      {
         izt++;
      }

      ix[0] = ixt - 1;
      iy[0] = iyt - 1;
      iz[0] = izt - 1;

      data_ix[0] = ixt - 1;
      data_iy[0] = (iyt - 1 ) * nx;
      data_iz[0] = (izt - 1 ) * nx * ny;
   }
   else /* use values that are already in main memory */
   {
      while ( (x_pos[ ( block_index * nx ) + ixt ] < x ) && (ixt < NX1 ))
      {
         ixt++;
      }
      while ( (y_pos[ ( block_index * ny ) + iyt ] < y ) && (iyt < NY1 ))
      {
         iyt++;
      }
      while ( (z_pos[ ( block_index * nz ) + izt ] < z ) && (izt < NZ1 ))
      {
         izt++;
      }

      /*

       printf("DEBUG\tnx = %d, ny = %d, nz = %d\n", nx, ny, nz );

       printf("DEBUG\tabout to set ix[0] = ( %d * %d ) + %d - 1 = %d\n", block_index, nx,ixt, ( block_index * nx ) + ixt - 1 );

       printf("DEBUG\tabout to set iy[0] = ( %d * %d ) + %d - 1 = %d\n", block_index, ny,iyt, ( block_index * ny ) + iyt - 1 );

       printf("DEBUG\tabout to set iz[0] = ( %d * %d ) + %d - 1 = %d\n", block_index, nz,izt, ( block_index * nz ) + izt - 1 );

       */

      ix[0] = (block_index * nx ) + ixt - 1;
      iy[0] = (block_index * ny ) + iyt - 1;
      iz[0] = (block_index * nz ) + izt - 1;

      /*

       ix[0] = ixt - 1;

       iy[0] = iyt - 1;

       iz[0] = izt - 1;

       */

      data_ix[0] = (block_index * nx ) + ixt - 1;
      data_iy[0] = ((block_index * ny ) + iyt - 1 ) * nx;
      data_iz[0] = ((block_index * nz ) + izt - 1 ) * nx * ny;
   }

}

void set_block_min_max(
      int block_index)
{

   CDFstatus status;

   long recordStart = 0;
   long indices[1];

   indices[0] = block_index;

   /*printf("searching parent number: %d out of %d located in block %d\n", root_index + 1, number_of_parents_at_amr_level[0], block_index_1 );*/

   status = CDFlib(
   SELECT_, zVAR_, block_x_min_cdfNum,
   SELECT_, zVAR_RECNUMBER_, recordStart,
   SELECT_, zVAR_DIMINDICES_, indices,
   GET_, zVAR_DATA_, &block_x_min,
   NULL_);
   if (status != CDF_OK)
      StatusHandler(status);

   /*printf("DEBUG 4\n");*/

   status = CDFlib(
   SELECT_, zVAR_, block_x_max_cdfNum,
   SELECT_, zVAR_RECNUMBER_, recordStart,
   SELECT_, zVAR_DIMINDICES_, indices,
   GET_, zVAR_DATA_, &block_x_max,
   NULL_);
   if (status != CDF_OK)
      StatusHandler(status);

   status = CDFlib(
   SELECT_, zVAR_, block_y_min_cdfNum,
   SELECT_, zVAR_RECNUMBER_, recordStart,
   SELECT_, zVAR_DIMINDICES_, indices,
   GET_, zVAR_DATA_, &block_y_min,
   NULL_);
   if (status != CDF_OK)
      StatusHandler(status);

   status = CDFlib(
   SELECT_, zVAR_, block_y_max_cdfNum,
   SELECT_, zVAR_RECNUMBER_, recordStart,
   SELECT_, zVAR_DIMINDICES_, indices,
   GET_, zVAR_DATA_, &block_y_max,
   NULL_);
   if (status != CDF_OK)
      StatusHandler(status);

   status = CDFlib(
   SELECT_, zVAR_, block_z_min_cdfNum,
   SELECT_, zVAR_RECNUMBER_, recordStart,
   SELECT_, zVAR_DIMINDICES_, indices,
   GET_, zVAR_DATA_, &block_z_min,
   NULL_);
   if (status != CDF_OK)
      StatusHandler(status);

   status = CDFlib(
   SELECT_, zVAR_, block_z_max_cdfNum,
   SELECT_, zVAR_RECNUMBER_, recordStart,
   SELECT_, zVAR_DIMINDICES_, indices,
   GET_, zVAR_DATA_, &block_z_max,
   NULL_);
   if (status != CDF_OK)
      StatusHandler(status);

   return;

}

