/*******************************************************************************
 *                                                                             *
 *    NAME:       interpolate_mas_cdf.c                                        *
 *                                                                             *
 *    VERSION:    1.0                                                          *
 *                                                                             *
 *    AUTHOR:     Marlo M. H. Maddox                                           *
 *                NASA-GSFC-AETD-ISD-ADMA(587)-CCMC                            *
 *                Marlo.Maddox@nasa.gov                                        *
 *                (301) 286 - 5202                                             *
 *                                                                             *
 *    PURPOSE:    Retrun an interpolated variable value for alocal_ny given    *
 *                position in the simulation box.  Requires a file to have     *
 *                alreday been opened with open_cdf call                       *
 *                                                                             *
 *    INPUTS:     (1)variable name (2-4) r,phi,theta posotion (5)mem flag      *
 *                (6)opt arg flag                                              *
 *                                                                             *
 *    OUTPUTS:    unit varibale attribute value                                *
 *                                                                             *
 *    FUNCTIONS:  *                                                            *
 *                                                                             *
 *    VARIABLES:  *                                                            *
 *                                                                             *
 *                                                                             *
 *    MODIFICATION                                                             *
 *    HISTORY:                                                                 *
 *                                                                             *
 *    02.23.2007  Maddox, Marlo  Initial Development Started                   *
 *                                                                             *
 *    03.19.2007  Maddox, Marlo                                                *
 *                Removed legacy change_sign_flag values to all 0. This was a  *
 *                carry over from enlil. There is also no need to reverse the  *
 *                sign of position angle phi                                   *
 *                                                                             *
 *    03.23.2007  Maddox, Marlo                                                *
 *                Debugging float->float...                                    *
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
#include "mas_cdf_interface_variables.h"
#include "cdf.h"

#define MISSING        -256.*256.*256.*256.*256.
#define MIN_RANGE    -1e9
#define MAX_RANGE    +1e9
#define DEBUG_FLAG    0

int local_nx, local_ny, local_nz;

float interpolate_mas_cdf(
      char *variable,
      float r,
      float phi,
      float theta,
      int main_memory_flag,
      int optional_argument_flag,
      ...)
{

   float pi = 3.14159265;
   float au_in_meters = 149598.e6;
   float radian_in_degrees = 57.2957795;
   float degrees_in_radians = 0.0174532925;

   float local_r, local_phi, local_theta;

   long counts[1] =
   { 0 };
   long intervals[1] =
   { 1 };
   long cdf_varNum;
   int change_sign_flag = 0;

   float interpolated_value;

   float *r_position_array, *phi_position_array, *theta_position_array; /* generic array pointers that will be set to corresponding mas grid according to which variable is being interpolated */

   int i, index_r = -1, index_phi = -1, index_theta = -1; /* used in numerical recipes hunt function */

   float dphi_dummy, dtheta_dummy, dr_dummy; /* dummy variables mem locs to set if no real mem locs are input as optional args */

   /* create variables to pass on to interpolate_in_block regardless if no optional arguments exist ( so interpolate_in_block can have a fixed arg list */
   float missing;
   float *dphi, *dtheta, *dr;

   va_list argptr; /* create a pointer to variable list of arguments */

   float interpolate_in_block_mas(
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
   float interpolate_in_block_mas2(
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

   /****     since the cdf data is stored in r[solar radii], radians, radians or r, phi theta
    *         but we are accepting input as r[solar radii], lon, lat - we must do some coordiante transformations
    */

   local_phi = phi / radian_in_degrees; /** convert degrees ( 0 - 360 longitude ) to radiadns 0 = 2 pi **/

   theta = -1 * theta; /** convert from -90 - 90 to radians 0 - pi **/

   local_theta = (theta/radian_in_degrees ) + (pi/2 );

   local_r = r;

   /****************************************************************************/

   if ( DEBUG_FLAG)
   {
      printf(
            "DEBUG\tinput position = [r,lon,lat] ( r, phi, theta ) = [%f,%f,%f]\n",
            r,
            phi,
            theta);
      printf(
            "DEBUG\tconverted position = [r,phi,theta] = [%f,%f,%f]\n",
            local_r,
            local_phi,
            local_theta);
      printf(
            "DEBUG\tconverted position = [x,y,z]       = [%f,%f,%f]\n",
            local_r,
            local_phi,
            local_theta);
      printf(
            "\nDEBUG\tdata is stored as [phi, theta, r ] = [ %f, %f, %f ]\n",
            local_phi,
            local_theta,
            local_r);
   }

   counts[0] = 0; /*reset values after once through */
   intervals[0] = 1;

   va_start( argptr, optional_argument_flag );
   /* make argptr point to first UNAMED arguments which should be the missing variable */

   /*printf("DEBUG:\tchecking for optional arguments using optional_argument_flag: %d\n", optional_argument_flag );*/

   if (optional_argument_flag)
   {

      /*printf("DEBUG:\toptional arguments are present\n");*/

      /* there are optional arguments 1 - 4, therofore set missing, dphi, dtheta, dr */
      /*missing = va_arg( argptr, float );*/

      /*hack = va_arg( argptr, float );*/
      missing = va_arg( argptr, double );
      dphi = va_arg( argptr, float * );
      dtheta = va_arg( argptr, float * );
      dr = va_arg( argptr, float * );

      /*printf("DEBUG:\toptional arguments are present setting missing value to -->%f<-- and also copying pointers...\n", missing);*/

   }
   else
   {
      /*printf("DEBUG:\tabout to set missing variable\n");*/
      missing = MISSING;
      dphi = &dphi_dummy;
      dtheta = &dtheta_dummy;
      dr = &dr_dummy;
      /*printf("DEBUG:\tNO optional arguments present, setting missing to -->%f<--\n", missing );*/

   }

   /* for field line tracing,etc..., select appropriate variable number ie. br_cdfNum|bphi_cdfNum|btheta_cdfNum based on *variable_string */

   /* set default grid arrays and sizes and change if neccessary */

   phi_position_array = phi_pos;
   theta_position_array = theta_pos;
   r_position_array = r_pos;

   if ( DEBUG_FLAG)
      printf(
            "stored dimension sizes\nnx = %d\nny = %d\nnz = %d\nny_plus1 = %d\nnz_plus1 = %d\n",
            nx,
            ny,
            nz,
            ny_plus1,
            nz_plus1);

   local_nx = nx;
   local_ny = ny;
   local_nz = nz;

   if ( !strcmp(
         variable,
         br_name) )
   {
      cdf_varNum = br_cdf_varNum;
      r_position_array = r1_pos;
      local_nz = nz_plus1;
   }
   else if ( !strcmp(
         variable,
         bphi_name) )
   {
      cdf_varNum = bphi_cdf_varNum;
      change_sign_flag = 0;
   }
   else if ( !strcmp(
         variable,
         btheta_name) )
   {
      cdf_varNum = btheta_cdf_varNum;
      change_sign_flag = 1;
      theta_position_array = theta1_pos;
      local_ny = ny_plus1;
   }
   else if ( !strcmp(
         variable,
         jr_name) )
   {
      cdf_varNum = jr_cdf_varNum;
      theta_position_array = theta1_pos;
      local_ny = ny_plus1;
   }
   else if ( !strcmp(
         variable,
         jphi_name) )
   {
      cdf_varNum = jphi_cdf_varNum;
      change_sign_flag = 0;
      r_position_array = r1_pos;
      theta_position_array = theta1_pos;
      local_nz = nz_plus1;
      local_ny = ny_plus1;
   }
   else if ( !strcmp(
         variable,
         jtheta_name) )
   {
      cdf_varNum = jtheta_cdf_varNum;
      change_sign_flag = 1;
      r_position_array = r1_pos;
      local_nz = nz_plus1;
   }
   else if ( !strcmp(
         variable,
         ur_name) )
   {
      cdf_varNum = ur_cdf_varNum;
      theta_position_array = theta1_pos;
      local_ny = ny_plus1;
   }
   else if ( !strcmp(
         variable,
         uphi_name) )
   {
      cdf_varNum = uphi_cdf_varNum;
      change_sign_flag = 0;
      r_position_array = r1_pos;
      theta_position_array = theta1_pos;
      local_nz = nz_plus1;
      local_ny = ny_plus1;
   }
   else if ( !strcmp(
         variable,
         utheta_name) )
   {
      cdf_varNum = utheta_cdf_varNum;
      change_sign_flag = 1;
      r_position_array = r1_pos;
      local_nz = nz_plus1;
   }
   else if ( !strcmp(
         variable,
         rho_name) )
   {
      cdf_varNum = rho_cdf_varNum;
      r_position_array = r1_pos;
      theta_position_array = theta1_pos;
      local_nz = nz_plus1;
      local_ny = ny_plus1;
   }
   else if ( !strcmp(
         variable,
         p_name) )
   {
      cdf_varNum = p_cdf_varNum;
      r_position_array = r1_pos;
      theta_position_array = theta1_pos;
      local_nz = nz_plus1;
      local_ny = ny_plus1;
   }
   else if ( !strcmp(
         variable,
         T_name) )
   {
      cdf_varNum = T_cdf_varNum;
      r_position_array = r1_pos;
      theta_position_array = theta1_pos;
      local_nz = nz_plus1;
      local_ny = ny_plus1;
   }
   else
   {
      printf(
            "ERROR:\tcould not find cdf variable number for %s\n",
            variable);
      printf(
            "Valid Variable Names for ENLIL :\n%s\n%s\n%s\n%s\n%s\n%s\n%s\n%s\n"
            "%s\n%s\n%s\n%s\n----------------------------------------------\n",
            br_name,
            bphi_name,
            btheta_name,
            jr_name,
            jphi_name,
            jtheta_name,
            ur_name,
            uphi_name,
            utheta_name,
            rho_name,
            p_name,
            T_name);
      return 0;
   }

   /*if(DEBUG_FLAG)
    {
    for( i = 0; i < local_ny-1; i++ )
    {
    printf("\ntheta_position_array[%d] = %f\ntheta_pos[%d] = %f\ntheta_pos1[%d] = %f\n", i, theta_position_array[i]    ,i, theta_pos[i], i, theta1_pos[i] );
    }
    }
    */

   if ( DEBUG_FLAG)
   {
      printf(
            "DEBUG:\tabout to call external hunt routine using indices -----> "
            "index_phi = %d, index_theta = %d, index_r = %d \n",
            index_phi,
            index_theta,
            index_r);

      printf(
            "DEBUG:\tphi[0] = %f\t\tlocal_nx = %d\t\tphi = %f\n",
            phi_position_array[0],
            local_nx,
            phi);
      printf(
            "DEBUG:\ttheta[0] = %f\t\tlocal_ny = %d\t\ttheta = %f\n",
            theta_position_array[0],
            local_ny,
            theta);
      printf(
            "DEBUG:\tr[0] = %f\t\tlocal_nz = %d\t\tr = %f\n",
            r_position_array[0],
            local_nz,
            r);

   }

   hunt(
         phi_position_array,
         local_nx,
         local_phi,
         &index_phi);
   hunt(
         theta_position_array,
         local_ny,
         local_theta,
         &index_theta);
   hunt(
         r_position_array,
         local_nz,
         local_r,
         &index_r);

   /** lutz reverses his y_blk array so lets invert the found index ***/

   /*printf("index_phi before reverse = %d\n", index_phi );*/

   /*index_phi = ( local_ny - 1 ) - index_phi;*/

   /*printf("index_phi after reverse = %d\n", index_phi );*/

   /*printf("DEBUG\tback from hunt\n");*/

   /* index_* point to the cell to the left below the current position OR indicates that the position is out of range by returning index* = -1 or n* */

   /*** Marlo Maddox - Modifying Periodic Boundary Conditions ***/

   if ( (index_r < 0 ) || (index_r >= local_nz-1 ) ||
   /*      (index_phi < 0 ) || (index_phi >= local_nx-1 ) */
   /* phi is periodic */
   (index_theta < 0 ) || (index_theta >= local_ny-1 ))
   {
      return ( (float)missing );
   }
   else
   {

      /*printf("DEBUG:\thunt calls completed successfully\n");*/

      if ( DEBUG_FLAG)
         printf(
               "DEBUG:\tabout to call interpolate_in_block using indices -----> "
               "index_phi = %d, index_theta = %d, index_r = %d\nranges are phi "
               "range = 0 - %d, theta range 0 - %d, r range 0 - %d\n",
               index_phi,
               index_theta,
               index_r,
               local_nx,
               local_ny,
               local_nz);
      /*
       interpolated_value = interpolate_in_block_mas( r, theta, phi, index_r, index_theta, index_phi, r_position_array, theta_position_array, phi_position_array, variable, dphi, dr, dtheta, cdf_varNum, main_memory_flag );
       */

      /********* original
       **
       interpolated_value = interpolate_in_block_mas( local_r, local_phi, local_theta, index_r, index_phi, index_theta, r_position_array, phi_position_array, theta_position_array, variable, dphi, dtheta, dr, cdf_varNum, main_memory_flag );
       **

       */
      /*
       ************ changed 3.23.07 **/

      interpolated_value = interpolate_in_block_mas2(
            local_phi,
            local_theta,
            local_r,
            index_phi,
            index_theta,
            index_r,
            phi_position_array,
            theta_position_array,
            r_position_array,
            variable,
            dphi,
            dtheta,
            dr,
            cdf_varNum,
            main_memory_flag);

      /****** we need to change the sign of alocal_ny y vector component ... *********/

      if (change_sign_flag) /*** this flag is set when cdf_varNum is set above ***/
      {
         interpolated_value = interpolated_value * ( -1.0 );
      }

      /*printf("DEBUG:\tcall to interpolate_in_block complete\n");*/

      /*return interpolated_value;*/

      if (interpolated_value < MIN_RANGE || interpolated_value > MAX_RANGE)
      {
         return (float)missing;
      }
      else
      {
         return (float)interpolated_value;
      }
   }

}

/**************** interpolate_in_block *****************************************/

float interpolate_in_block_mas(
      float x,
      float y,
      float z,
      int ix,
      int iy,
      int iz,
      float *r_pos,
      float *phi_pos,
      float *theta_pos,
      char *variable_name,
      float *dphi,
      float *dtheta,
      float *dr,
      int cdf_varNum,
      int main_memory_flag)
{

   CDFstatus status;

   long recordStart = 0;
   long indices[1];

   float value;
   float dphi_blk, dtheta_blk, dr_blk, m_x, m_y, m_z, two_pi=4*asin(1.);
   float data_1a, data_2a, data_3a, data_4a, data_5a, data_6a, data_7a, data_8a; /** values from actual data used for interploation equation these are floats for cdflib call because the actual variable are of type CDF_FLOAT or float... **/
   float data_1, data_2, data_3, data_4, data_5, data_6, data_7, data_8; /** values from actual data used for interploation equation **/

   /*int ix, iy, iz;*/
   int NV_blk = local_nx * local_ny, iz1=-1;

   /* int first_cell_in_block = found_block_index * local_nx * local_ny * local_nz; */

   /*printf("interpolating block %d\n", found_block_index );*/

   /*find_in_block( x, y, z, found_block_index, &ix, &iy, &iz, &data_ix, &data_iy, &data_iz );*/

   if ( DEBUG_FLAG)
      printf(
            "position  %f,%f,%f\nindices are %d, %d, %d\n",
            x,
            y,
            z,
            ix,
            iy,
            iz);

   dphi_blk = r_pos[ ix + 1 ] - r_pos[ ix ];
   dtheta_blk = phi_pos[ iy + 1 ] - phi_pos[ iy ];

   /* periodic boundary */
   /**** 03.23.07
    if ( (iz == (local_nz-1) ) || (iz == -1) ) {
    iz=local_nz-1;
    iz1=0;
    dr_blk = ( theta_pos[ iz1 ] - theta_pos[ iz ] + two_pi);
    m_z = (z - theta_pos[ local_nz-1 ] + (theta_pos[0] > z)*two_pi )/ dr_blk;
    } else {
    iz1=iz+1;
    dr_blk = theta_pos[ iz + 1 ] - theta_pos[ iz ];
    m_z = ( z - theta_pos[ iz ] ) / dr_blk;
    }
    */

   if ( DEBUG_FLAG)
      printf(
            "\nDEBUG\tdphi = %f, dtheta = %f, dr = %f\n",
            dphi_blk,
            dtheta_blk,
            dr_blk);

   *dphi = dphi_blk; /* return values to caller */
   *dtheta = dtheta_blk;
   *dr = dr_blk;

   m_x = (x - r_pos[ ix ] ) / dphi_blk;
   m_y = (y - phi_pos[ iy ] ) / dtheta_blk;

   /*
    #ifdef DEBUG
    #define AU 1.49598e11
    printf("DEBUG\tx0 = %f\tx1 = %f\n\ty0 = %f\ty1= %f\local_nz0 = %f\tz1= %f\n", r_pos[ix]/AU,r_pos[ix+1]/AU,phi_pos[0],phi_pos[local_ny-1],theta_pos[0],theta_pos[local_nz-1] );
    printf("DEBUG\tr = %f\tphi = %f\ttheta = %f\n", x,y,z );
    printf("DEBUG\tdphi = %f\n\tdtheta = %f\n\tdr = %f\nand ix = %d\tiy = %d\tiz = %d iz1 = %d\n\nand m_x = %f\tm_y = %f\tm_z = %f\n", dphi_blk, dtheta_blk, dr_blk, ix, iy, iz,iz1,m_x,m_y,m_z );
    #endif
    */

   if ( DEBUG_FLAG)
      printf(
            "DEBUG\tmx = %f\n\tmy = %f\n\tmz = %f\n",
            m_x,
            m_y,
            m_z);

   if ( DEBUG_FLAG)
      printf("debug:\tgetting data values from cdf\n");

   if ( !main_memory_flag)
   {

      indices[0] = (ix + iy*local_nx + iz*NV_blk );
      status = CDFlib(
      SELECT_, zVAR_, cdf_varNum,
      SELECT_, zVAR_RECNUMBER_, recordStart,
      SELECT_, zVAR_DIMINDICES_, indices,
      GET_, zVAR_DATA_, &data_1a,
      NULL_);
      if (status != CDF_OK)
         StatusHandler(status);

      indices[0] = (ix + 1 + iy*local_nx + iz*NV_blk );
      status = CDFlib(
      SELECT_, zVAR_, cdf_varNum,
      SELECT_, zVAR_RECNUMBER_, recordStart,
      SELECT_, zVAR_DIMINDICES_, indices,
      GET_, zVAR_DATA_, &data_2a,
      NULL_);
      if (status != CDF_OK)
         StatusHandler(status);

      indices[0] = (ix + (iy + 1 )*local_nx + iz*NV_blk );
      status = CDFlib(
      SELECT_, zVAR_, cdf_varNum,
      SELECT_, zVAR_RECNUMBER_, recordStart,
      SELECT_, zVAR_DIMINDICES_, indices,
      GET_, zVAR_DATA_, &data_3a,
      NULL_);
      if (status != CDF_OK)
         StatusHandler(status);

      indices[0] = (ix + 1 + (iy + 1)*local_nx + iz*NV_blk );
      status = CDFlib(
      SELECT_, zVAR_, cdf_varNum,
      SELECT_, zVAR_RECNUMBER_, recordStart,
      SELECT_, zVAR_DIMINDICES_, indices,
      GET_, zVAR_DATA_, &data_4a,
      NULL_);
      if (status != CDF_OK)
         StatusHandler(status);

      indices[0] = (ix + iy*local_nx + (iz1 )*NV_blk );
      status = CDFlib(
      SELECT_, zVAR_, cdf_varNum,
      SELECT_, zVAR_RECNUMBER_, recordStart,
      SELECT_, zVAR_DIMINDICES_, indices,
      GET_, zVAR_DATA_, &data_5a,
      NULL_);
      if (status != CDF_OK)
         StatusHandler(status);

      indices[0] = (ix + 1 + iy*local_nx + (iz1 )*NV_blk );
      status = CDFlib(
      SELECT_, zVAR_, cdf_varNum,
      SELECT_, zVAR_RECNUMBER_, recordStart,
      SELECT_, zVAR_DIMINDICES_, indices,
      GET_, zVAR_DATA_, &data_6a,
      NULL_);
      if (status != CDF_OK)
         StatusHandler(status);

      indices[0] = (ix + (iy + 1 )*local_nx + (iz1 )*NV_blk );
      status = CDFlib(
      SELECT_, zVAR_, cdf_varNum,
      SELECT_, zVAR_RECNUMBER_, recordStart,
      SELECT_, zVAR_DIMINDICES_, indices,
      GET_, zVAR_DATA_, &data_7a,
      NULL_);
      if (status != CDF_OK)
         StatusHandler(status);

      indices[0] = (ix + 1 + (iy + 1)*local_nx + (iz1 )*NV_blk );
      status = CDFlib(
      SELECT_, zVAR_, cdf_varNum,
      SELECT_, zVAR_RECNUMBER_, recordStart,
      SELECT_, zVAR_DIMINDICES_, indices,
      GET_, zVAR_DATA_, &data_8a,
      NULL_);
      if (status != CDF_OK)
         StatusHandler(status);

      /********* now copy floats to floats **********/

      data_1 = data_1a;
      data_2 = data_2a;
      data_3 = data_3a;
      data_4 = data_4a;
      data_5 = data_5a;
      data_6 = data_6a;
      data_7 = data_7a;
      data_8 = data_8a;

   }
   else
   {

      /* reset ix, iy, & iz to conform to the general expressions used above and below were ix = ixt - 1 and now for mem_flag is */
      /* ( block_index_1 * local_nx ) + ixt - 1 */

      /*
       ix = ix - ( found_block_index * local_nx );
       iy = iy - ( found_block_index * local_ny );
       iz = iz - ( found_block_index * local_nz );
       */

      if ( !strcmp(
            variable_name,
            br_name) )
      {
         data_1 = br[ ( ix + iy*local_nx + iz*NV_blk ) ];
         data_2 = br[ ( ix + 1 + iy*local_nx + iz*NV_blk ) ];
         data_3 = br[ ( ix + ( iy + 1 )*local_nx + iz*NV_blk ) ];
         data_4 = br[ ( ix + 1 + ( iy + 1)*local_nx + iz*NV_blk ) ];
         data_5 = br[ ( ix + iy*local_nx + ( iz1 )*NV_blk ) ];
         data_6 = br[ ( ix + 1 + iy*local_nx + ( iz1 )*NV_blk ) ];
         data_7 = br[ ( ix + ( iy + 1 )*local_nx + ( iz1 )*NV_blk ) ];
         data_8 = br[ ( ix + 1 + ( iy + 1)*local_nx + ( iz1)*NV_blk ) ];
      }
      else if ( !strcmp(
            variable_name,
            bphi_name) )
      {
         data_1 = bphi[ ( ix + iy*local_nx + iz*NV_blk ) ];
         data_2 = bphi[ ( ix + 1 + iy*local_nx + iz*NV_blk ) ];
         data_3 = bphi[ ( ix + ( iy + 1 )*local_nx + iz*NV_blk ) ];
         data_4 = bphi[ ( ix + 1 + ( iy + 1)*local_nx + iz*NV_blk ) ];
         data_5 = bphi[ ( ix + iy*local_nx + ( iz1 )*NV_blk ) ];
         data_6 = bphi[ ( ix + 1 + iy*local_nx + ( iz1 )*NV_blk ) ];
         data_7 = bphi[ ( ix + ( iy + 1 )*local_nx + ( iz1 )*NV_blk ) ];
         data_8 = bphi[ ( ix + 1 + ( iy + 1)*local_nx + ( iz1)*NV_blk ) ];
      }
      else if ( !strcmp(
            variable_name,
            btheta_name) )
      {
         data_1 = btheta[ ( ix + iy*local_nx + iz*NV_blk ) ];
         data_2 = btheta[ ( ix + 1 + iy*local_nx + iz*NV_blk ) ];
         data_3 = btheta[ ( ix + ( iy + 1 )*local_nx + iz*NV_blk ) ];
         data_4 = btheta[ ( ix + 1 + ( iy + 1)*local_nx + iz*NV_blk ) ];
         data_5 = btheta[ ( ix + iy*local_nx + ( iz1 )*NV_blk ) ];
         data_6 = btheta[ ( ix + 1 + iy*local_nx + ( iz1 )*NV_blk ) ];
         data_7 = btheta[ ( ix + ( iy + 1 )*local_nx + ( iz1 )*NV_blk ) ];
         data_8 = btheta[ ( ix + 1 + ( iy + 1)*local_nx + ( iz1)*NV_blk ) ];
      }
      else if ( !strcmp(
            variable_name,
            jr_name) )
      {
         data_1 = jr[ ( ix + iy*local_nx + iz*NV_blk ) ];
         data_2 = jr[ ( ix + 1 + iy*local_nx + iz*NV_blk ) ];
         data_3 = jr[ ( ix + ( iy + 1 )*local_nx + iz*NV_blk ) ];
         data_4 = jr[ ( ix + 1 + ( iy + 1)*local_nx + iz*NV_blk ) ];
         data_5 = jr[ ( ix + iy*local_nx + ( iz1 )*NV_blk ) ];
         data_6 = jr[ ( ix + 1 + iy*local_nx + ( iz1 )*NV_blk ) ];
         data_7 = jr[ ( ix + ( iy + 1 )*local_nx + ( iz1 )*NV_blk ) ];
         data_8 = jr[ ( ix + 1 + ( iy + 1)*local_nx + ( iz1)*NV_blk ) ];
      }
      else if ( !strcmp(
            variable_name,
            jphi_name) )
      {
         data_1 = jphi[ ( ix + iy*local_nx + iz*NV_blk ) ];
         data_2 = jphi[ ( ix + 1 + iy*local_nx + iz*NV_blk ) ];
         data_3 = jphi[ ( ix + ( iy + 1 )*local_nx + iz*NV_blk ) ];
         data_4 = jphi[ ( ix + 1 + ( iy + 1)*local_nx + iz*NV_blk ) ];
         data_5 = jphi[ ( ix + iy*local_nx + ( iz1 )*NV_blk ) ];
         data_6 = jphi[ ( ix + 1 + iy*local_nx + ( iz1 )*NV_blk ) ];
         data_7 = jphi[ ( ix + ( iy + 1 )*local_nx + ( iz1 )*NV_blk ) ];
         data_8 = jphi[ ( ix + 1 + ( iy + 1)*local_nx + ( iz1)*NV_blk ) ];
      }
      else if ( !strcmp(
            variable_name,
            jtheta_name) )
      {
         data_1 = jtheta[ ( ix + iy*local_nx + iz*NV_blk ) ];
         data_2 = jtheta[ ( ix + 1 + iy*local_nx + iz*NV_blk ) ];
         data_3 = jtheta[ ( ix + ( iy + 1 )*local_nx + iz*NV_blk ) ];
         data_4 = jtheta[ ( ix + 1 + ( iy + 1)*local_nx + iz*NV_blk ) ];
         data_5 = jtheta[ ( ix + iy*local_nx + ( iz1 )*NV_blk ) ];
         data_6 = jtheta[ ( ix + 1 + iy*local_nx + ( iz1 )*NV_blk ) ];
         data_7 = jtheta[ ( ix + ( iy + 1 )*local_nx + ( iz1 )*NV_blk ) ];
         data_8 = jtheta[ ( ix + 1 + ( iy + 1)*local_nx + ( iz1)*NV_blk ) ];
      }
      else if ( !strcmp(
            variable_name,
            ur_name) )
      {
         data_1 = ur[ ( ix + iy*local_nx + iz*NV_blk ) ];
         data_2 = ur[ ( ix + 1 + iy*local_nx + iz*NV_blk ) ];
         data_3 = ur[ ( ix + ( iy + 1 )*local_nx + iz*NV_blk ) ];
         data_4 = ur[ ( ix + 1 + ( iy + 1)*local_nx + iz*NV_blk ) ];
         data_5 = ur[ ( ix + iy*local_nx + ( iz1 )*NV_blk ) ];
         data_6 = ur[ ( ix + 1 + iy*local_nx + ( iz1 )*NV_blk ) ];
         data_7 = ur[ ( ix + ( iy + 1 )*local_nx + ( iz1 )*NV_blk ) ];
         data_8 = ur[ ( ix + 1 + ( iy + 1)*local_nx + ( iz1)*NV_blk ) ];
      }
      else if ( !strcmp(
            variable_name,
            uphi_name) )
      {
         data_1 = uphi[ ( ix + iy*local_nx + iz*NV_blk ) ];
         data_2 = uphi[ ( ix + 1 + iy*local_nx + iz*NV_blk ) ];
         data_3 = uphi[ ( ix + ( iy + 1 )*local_nx + iz*NV_blk ) ];
         data_4 = uphi[ ( ix + 1 + ( iy + 1)*local_nx + iz*NV_blk ) ];
         data_5 = uphi[ ( ix + iy*local_nx + ( iz1 )*NV_blk ) ];
         data_6 = uphi[ ( ix + 1 + iy*local_nx + ( iz1 )*NV_blk ) ];
         data_7 = uphi[ ( ix + ( iy + 1 )*local_nx + ( iz1 )*NV_blk ) ];
         data_8 = uphi[ ( ix + 1 + ( iy + 1)*local_nx + ( iz1)*NV_blk ) ];
      }
      else if ( !strcmp(
            variable_name,
            utheta_name) )
      {
         data_1 = utheta[ ( ix + iy*local_nx + iz*NV_blk ) ];
         data_2 = utheta[ ( ix + 1 + iy*local_nx + iz*NV_blk ) ];
         data_3 = utheta[ ( ix + ( iy + 1 )*local_nx + iz*NV_blk ) ];
         data_4 = utheta[ ( ix + 1 + ( iy + 1)*local_nx + iz*NV_blk ) ];
         data_5 = utheta[ ( ix + iy*local_nx + ( iz1 )*NV_blk ) ];
         data_6 = utheta[ ( ix + 1 + iy*local_nx + ( iz1 )*NV_blk ) ];
         data_7 = utheta[ ( ix + ( iy + 1 )*local_nx + ( iz1 )*NV_blk ) ];
         data_8 = utheta[ ( ix + 1 + ( iy + 1)*local_nx + ( iz1)*NV_blk ) ];
      }
      else if ( !strcmp(
            variable_name,
            rho_name) )
      {
         data_1 = rho[ ( ix + iy*local_nx + iz*NV_blk ) ];
         data_2 = rho[ ( ix + 1 + iy*local_nx + iz*NV_blk ) ];
         data_3 = rho[ ( ix + ( iy + 1 )*local_nx + iz*NV_blk ) ];
         data_4 = rho[ ( ix + 1 + ( iy + 1)*local_nx + iz*NV_blk ) ];
         data_5 = rho[ ( ix + iy*local_nx + ( iz1 )*NV_blk ) ];
         data_6 = rho[ ( ix + 1 + iy*local_nx + ( iz1 )*NV_blk ) ];
         data_7 = rho[ ( ix + ( iy + 1 )*local_nx + ( iz1 )*NV_blk ) ];
         data_8 = rho[ ( ix + 1 + ( iy + 1)*local_nx + ( iz1)*NV_blk ) ];
      }
      else if ( !strcmp(
            variable_name,
            p_name) )
      {
         data_1 = p[ ( ix + iy*local_nx + iz*NV_blk ) ];
         data_2 = p[ ( ix + 1 + iy*local_nx + iz*NV_blk ) ];
         data_3 = p[ ( ix + ( iy + 1 )*local_nx + iz*NV_blk ) ];
         data_4 = p[ ( ix + 1 + ( iy + 1)*local_nx + iz*NV_blk ) ];
         data_5 = p[ ( ix + iy*local_nx + ( iz1 )*NV_blk ) ];
         data_6 = p[ ( ix + 1 + iy*local_nx + ( iz1 )*NV_blk ) ];
         data_7 = p[ ( ix + ( iy + 1 )*local_nx + ( iz1 )*NV_blk ) ];
         data_8 = p[ ( ix + 1 + ( iy + 1)*local_nx + ( iz1)*NV_blk ) ];
      }
      else if ( !strcmp(
            variable_name,
            T_name) )
      {

         /*printf("DEBUG\tgetting T data_1-8\n");*/

         data_1 = T[ ( ix + iy*local_nx + iz*NV_blk ) ];
         data_2 = T[ ( ix + 1 + iy*local_nx + iz*NV_blk ) ];
         data_3 = T[ ( ix + ( iy + 1 )*local_nx + iz*NV_blk ) ];
         data_4 = T[ ( ix + 1 + ( iy + 1)*local_nx + iz*NV_blk ) ];
         data_5 = T[ ( ix + iy*local_nx + ( iz1 )*NV_blk ) ];
         data_6 = T[ ( ix + 1 + iy*local_nx + ( iz1 )*NV_blk ) ];
         data_7 = T[ ( ix + ( iy + 1 )*local_nx + ( iz1 )*NV_blk ) ];
         data_8 = T[ ( ix + 1 + ( iy + 1)*local_nx + ( iz1)*NV_blk ) ];

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
            "DEBUG:\t Ir %d Iphi %d Itheta %d Data: %g\n",
            ix,
            iz,
            iy+1,
            data_3);
      printf(
            "DEBUG:\t Ir %d Iphi %d Itheta %d Data: %g\n",
            ix+1,
            iz,
            iy+1,
            data_4);
      printf(
            "DEBUG:\t Ir %d Iphi %d Itheta %d Data: %g\n",
            ix,
            iz1,
            iy+1,
            data_7);
      printf(
            "DEBUG:\t Ir %d Iphi %d Itheta %d Data: %g\n",
            ix+1,
            iz1,
            iy+1,
            data_8);
      printf(
            "DEBUG:\t Ir %d Iphi %d Itheta %d Data: %g\n",
            ix,
            iz,
            iy,
            data_1);
      printf(
            "DEBUG:\t Ir %d Iphi %d Itheta %d Data: %g\n",
            ix+1,
            iz,
            iy,
            data_2);
      printf(
            "DEBUG:\t Ir %d Iphi %d Itheta %d Data: %g\n",
            ix,
            iz1,
            iy,
            data_5);
      printf(
            "DEBUG:\t Ir %d Iphi %d Itheta %d Data: %g\n",
            ix+1,
            iz1,
            iy,
            data_6);
   }

   /* printf("DEBUG\tdata 1..8 = %g %g %g %g %g %g %g this should be data_8-->%g<--\n", data_1, data_2, data_3, data_4, data_5, data_6, data_7, data_8 ); */

   value = (1-m_z)*( (1-m_y)*( (1-m_x)*data_1 + m_x *data_2 ) + m_y
         *( + (1-m_x)*data_3 + m_x *data_4 ) ) + m_z*( (1-m_y)*( +(1-m_x)
         *data_5 + m_x *data_6 ) + m_y*( +(1-m_x)*data_7 + m_x *data_8 ) );

   return (value );

}

/**************** interpolate_in_block refactored*****************************************/

float interpolate_in_block_mas2(
      float phi,
      float theta,
      float r,
      int index_phi,
      int index_theta,
      int index_r,
      float *phi_pos,
      float *theta_pos,
      float *r_pos,
      char *variable_name,
      float *dphi,
      float *dtheta,
      float *dr,
      int cdf_varNum,
      int main_memory_flag)
{

   CDFstatus status;

   long recordStart = 0;
   long indices[1];

   float value;
   float dphi_blk, dtheta_blk, dr_blk, m_x, m_y, m_z, two_pi=4*asin(1.);
   float data_1a, data_2a, data_3a, data_4a, data_5a, data_6a, data_7a, data_8a; /** values from actual data used for interploation equation these are floats for cdflib call because the actual variable are of type CDF_FLOAT or float... **/
   float data_1, data_2, data_3, data_4, data_5, data_6, data_7, data_8; /** values from actual data used for interploation equation **/

   int NV_blk = local_nx * local_ny;
   int index_phi1=-1;

   if ( DEBUG_FLAG)
      printf(
            "positions\t  %f, %f, %f\nindices are %d, %d, %d\n",
            phi,
            theta,
            r,
            index_phi,
            index_theta,
            index_r);

   if ( DEBUG_FLAG)
      printf(
            "twp_pi = 4*asin(1.) = %f\n",
            4*asin(1.) );

   /* periodic boundary */
   /**** 03.23.07 **/
   if ( (index_phi == (local_nx-1) ) || (index_phi == -1))
   {
      index_phi=local_nx-1;
      index_phi1=0;
      dphi_blk = (phi_pos[ index_phi1 ] - phi_pos[ index_phi ] + two_pi);
      m_x = (phi - phi_pos[ local_nx-1 ] + (phi_pos[0] > phi)*two_pi )
            / dphi_blk;
   }
   else
   {
      index_phi1=index_phi+1;
      dphi_blk = phi_pos[ index_phi + 1 ] - phi_pos[ index_phi ];
      m_x = (phi - phi_pos[ index_phi ] ) / dphi_blk;
   }

   if (DEBUG_FLAG)
   {
      printf(
            "\nWTF test theta_pos[0] = %f\ncalculating theta_pos[%d] %f - theta_pos[%d] %f\n",
            theta_pos[0],
            index_theta +1,
            theta_pos[ index_theta + 1 ],
            index_theta,
            theta_pos[ index_theta ]);
      printf(
            "\nWTF test r_pos[0] = %f\ncalculating r_pos[%d] %f - r_pos[%d] %f\n",
            r_pos[0],
            index_r +1,
            r_pos[ index_r + 1 ],
            index_r,
            r_pos[ index_r ]);
   }

   dtheta_blk = theta_pos[ index_theta + 1 ] - theta_pos[ index_theta ];
   dr_blk = r_pos[ index_r + 1 ] - r_pos[ index_r ];

   if ( DEBUG_FLAG)
      printf(
            "\nDEBUG\tdphi = %f, dtheta = %f, dr = %f\n",
            dphi_blk,
            dtheta_blk,
            dr_blk);

   *dphi = dphi_blk; /* return values to caller */
   *dtheta = dtheta_blk;
   *dr = dr_blk;

   m_y = (theta - theta_pos[ index_theta ] ) / dtheta_blk;
   m_z = (r - r_pos[ index_r ] ) / dr_blk;

#ifdef DEBUG
#define AU 1.49598e11
   printf("DEBUG\tx0 = %f\tx1 = %f\n\ty0 = %f\ty1= %f\local_nz0 = %f\tz1= %f\n", phi_pos[index_phi]/AU,phi_pos[index_phi+1]/AU,theta_pos[0],theta_pos[local_ny-1],r_pos[0],r_pos[local_nz-1] );
   printf("DEBUG\tr = %f\tphi = %f\ttheta = %f\n", x,y,z );
   printf("DEBUG\tdphi = %f\n\tdtheta = %f\n\tdr = %f\nand index_phi = %d\tindex_theta = %d\tindex_r = %d index_r1 = %d\n\nand m_x = %f\tm_y = %f\tm_z = %f\n", dphi_blk, dtheta_blk, dr_blk, index_phi, index_theta, index_r,index_r1,m_x,m_y,m_z );
#endif

   if ( DEBUG_FLAG)
      printf(
            "DEBUG\tmx = %f\n\tmy = %f\n\tmz = %f\n",
            m_x,
            m_y,
            m_z);

   if ( DEBUG_FLAG)
      printf("debug:\tgetting data values from cdf\n");

   if ( !main_memory_flag)
   {

      indices[0] = (index_phi + index_theta*local_nx + index_r*NV_blk );
      status = CDFlib(
      SELECT_, zVAR_, cdf_varNum,
      SELECT_, zVAR_RECNUMBER_, recordStart,
      SELECT_, zVAR_DIMINDICES_, indices,
      GET_, zVAR_DATA_, &data_1a,
      NULL_);
      if (status != CDF_OK)
         StatusHandler(status);

      indices[0] = (index_phi1 + index_theta*local_nx + index_r*NV_blk );
      status = CDFlib(
      SELECT_, zVAR_, cdf_varNum,
      SELECT_, zVAR_RECNUMBER_, recordStart,
      SELECT_, zVAR_DIMINDICES_, indices,
      GET_, zVAR_DATA_, &data_2a,
      NULL_);
      if (status != CDF_OK)
         StatusHandler(status);

      indices[0] = (index_phi + (index_theta + 1 )*local_nx + index_r*NV_blk );
      status = CDFlib(
      SELECT_, zVAR_, cdf_varNum,
      SELECT_, zVAR_RECNUMBER_, recordStart,
      SELECT_, zVAR_DIMINDICES_, indices,
      GET_, zVAR_DATA_, &data_3a,
      NULL_);
      if (status != CDF_OK)
         StatusHandler(status);

      indices[0] = (index_phi1 + (index_theta + 1)*local_nx + index_r*NV_blk );
      status = CDFlib(
      SELECT_, zVAR_, cdf_varNum,
      SELECT_, zVAR_RECNUMBER_, recordStart,
      SELECT_, zVAR_DIMINDICES_, indices,
      GET_, zVAR_DATA_, &data_4a,
      NULL_);
      if (status != CDF_OK)
         StatusHandler(status);

      /******** changing index_r1 below to index_r and index_phi to index_phi1 ********* 032607 */

      indices[0] = (index_phi + index_theta*local_nx + (index_r + 1)*NV_blk );
      status = CDFlib(
      SELECT_, zVAR_, cdf_varNum,
      SELECT_, zVAR_RECNUMBER_, recordStart,
      SELECT_, zVAR_DIMINDICES_, indices,
      GET_, zVAR_DATA_, &data_5a,
      NULL_);
      if (status != CDF_OK)
         StatusHandler(status);

      indices[0] = (index_phi1 + index_theta*local_nx + (index_r + 1)*NV_blk );
      status = CDFlib(
      SELECT_, zVAR_, cdf_varNum,
      SELECT_, zVAR_RECNUMBER_, recordStart,
      SELECT_, zVAR_DIMINDICES_, indices,
      GET_, zVAR_DATA_, &data_6a,
      NULL_);
      if (status != CDF_OK)
         StatusHandler(status);

      indices[0] = (index_phi + (index_theta + 1 )*local_nx + (index_r + 1 )
            * NV_blk );
      status = CDFlib(
      SELECT_, zVAR_, cdf_varNum,
      SELECT_, zVAR_RECNUMBER_, recordStart,
      SELECT_, zVAR_DIMINDICES_, indices,
      GET_, zVAR_DATA_, &data_7a,
      NULL_);
      if (status != CDF_OK)
         StatusHandler(status);

      indices[0] = (index_phi1 + (index_theta + 1)*local_nx + (index_r + 1 )
            *NV_blk );
      status = CDFlib(
      SELECT_, zVAR_, cdf_varNum,
      SELECT_, zVAR_RECNUMBER_, recordStart,
      SELECT_, zVAR_DIMINDICES_, indices,
      GET_, zVAR_DATA_, &data_8a,
      NULL_);
      if (status != CDF_OK)
         StatusHandler(status);

      /********* now copy floats to floats **********/

      data_1 = data_1a;
      data_2 = data_2a;
      data_3 = data_3a;
      data_4 = data_4a;
      data_5 = data_5a;
      data_6 = data_6a;
      data_7 = data_7a;
      data_8 = data_8a;

   }
   else
   {

      /* reset index_phi, index_theta, & index_r to conform to the general expressions used above and below were index_phi = index_phit - 1 and now for mem_flag is */
      /* ( block_index_1 * local_nx ) + index_phit - 1 */

      /*
       index_phi = index_phi - ( found_block_index * local_nx );
       index_theta = index_theta - ( found_block_index * local_ny );
       index_r = index_r - ( found_block_index * local_nz );
       */

      if ( !strcmp(
            variable_name,
            br_name) )
      {
         data_1 = br[ ( index_phi + index_theta*local_nx + index_r*NV_blk ) ];
         data_2 = br[ ( index_phi1 + index_theta*local_nx + index_r*NV_blk ) ];
         data_3 = br[ ( index_phi + ( index_theta + 1 )*local_nx + index_r*NV_blk ) ];
         data_4 = br[ ( index_phi1 + ( index_theta + 1)*local_nx + index_r*NV_blk ) ];
         data_5 = br[ ( index_phi + index_theta*local_nx + ( index_r + 1 )*NV_blk ) ];
         data_6 = br[ ( index_phi1 + index_theta*local_nx + ( index_r + 1 )*NV_blk ) ];
         data_7 = br[ ( index_phi + ( index_theta + 1 )*local_nx + ( index_r + 1 )*NV_blk ) ];
         data_8 = br[ ( index_phi1 + ( index_theta + 1)*local_nx + ( index_r + 1 )*NV_blk ) ];
      }
      else if ( !strcmp(
            variable_name,
            bphi_name) )
      {
         data_1 = bphi[ ( index_phi + index_theta*local_nx + index_r*NV_blk ) ];
         data_2 = bphi[ ( index_phi1 + index_theta*local_nx + index_r*NV_blk ) ];
         data_3 = bphi[ ( index_phi + ( index_theta + 1 )*local_nx + index_r*NV_blk ) ];
         data_4 = bphi[ ( index_phi1 + ( index_theta + 1)*local_nx + index_r*NV_blk ) ];
         data_5 = bphi[ ( index_phi + index_theta*local_nx + ( index_r + 1 )*NV_blk ) ];
         data_6 = bphi[ ( index_phi1 + index_theta*local_nx + ( index_r + 1 )*NV_blk ) ];
         data_7 = bphi[ ( index_phi + ( index_theta + 1 )*local_nx + ( index_r + 1 )*NV_blk ) ];
         data_8 = bphi[ ( index_phi1 + ( index_theta + 1)*local_nx + ( index_r + 1 )*NV_blk ) ];
      }
      else if ( !strcmp(
            variable_name,
            btheta_name) )
      {
         data_1 = btheta[ ( index_phi + index_theta*local_nx + index_r*NV_blk ) ];
         data_2 = btheta[ ( index_phi1 + index_theta*local_nx + index_r*NV_blk ) ];
         data_3 = btheta[ ( index_phi + ( index_theta + 1 )*local_nx + index_r*NV_blk ) ];
         data_4 = btheta[ ( index_phi1 + ( index_theta + 1)*local_nx + index_r*NV_blk ) ];
         data_5 = btheta[ ( index_phi + index_theta*local_nx + ( index_r + 1 )*NV_blk ) ];
         data_6 = btheta[ ( index_phi1 + index_theta*local_nx + ( index_r + 1 )*NV_blk ) ];
         data_7 = btheta[ ( index_phi + ( index_theta + 1 )*local_nx + ( index_r + 1 )*NV_blk ) ];
         data_8 = btheta[ ( index_phi1 + ( index_theta + 1)*local_nx + ( index_r + 1 )*NV_blk ) ];
      }
      else if ( !strcmp(
            variable_name,
            jr_name) )
      {
         data_1 = jr[ ( index_phi + index_theta*local_nx + index_r*NV_blk ) ];
         data_2 = jr[ ( index_phi1 + index_theta*local_nx + index_r*NV_blk ) ];
         data_3 = jr[ ( index_phi + ( index_theta + 1 )*local_nx + index_r*NV_blk ) ];
         data_4 = jr[ ( index_phi1 + ( index_theta + 1)*local_nx + index_r*NV_blk ) ];
         data_5 = jr[ ( index_phi + index_theta*local_nx + ( index_r + 1 )*NV_blk ) ];
         data_6 = jr[ ( index_phi1 + index_theta*local_nx + ( index_r + 1 )*NV_blk ) ];
         data_7 = jr[ ( index_phi + ( index_theta + 1 )*local_nx + ( index_r + 1 )*NV_blk ) ];
         data_8 = jr[ ( index_phi1 + ( index_theta + 1)*local_nx + ( index_r + 1 )*NV_blk ) ];
      }
      else if ( !strcmp(
            variable_name,
            jphi_name) )
      {
         data_1 = jphi[ ( index_phi + index_theta*local_nx + index_r*NV_blk ) ];
         data_2 = jphi[ ( index_phi1 + index_theta*local_nx + index_r*NV_blk ) ];
         data_3 = jphi[ ( index_phi + ( index_theta + 1 )*local_nx + index_r*NV_blk ) ];
         data_4 = jphi[ ( index_phi1 + ( index_theta + 1)*local_nx + index_r*NV_blk ) ];
         data_5 = jphi[ ( index_phi + index_theta*local_nx + ( index_r + 1 )*NV_blk ) ];
         data_6 = jphi[ ( index_phi1 + index_theta*local_nx + ( index_r + 1 )*NV_blk ) ];
         data_7 = jphi[ ( index_phi + ( index_theta + 1 )*local_nx + ( index_r + 1 )*NV_blk ) ];
         data_8 = jphi[ ( index_phi1 + ( index_theta + 1)*local_nx + ( index_r + 1 )*NV_blk ) ];
      }
      else if ( !strcmp(
            variable_name,
            jtheta_name) )
      {
         data_1 = jtheta[ ( index_phi + index_theta*local_nx + index_r*NV_blk ) ];
         data_2 = jtheta[ ( index_phi1 + index_theta*local_nx + index_r*NV_blk ) ];
         data_3 = jtheta[ ( index_phi + ( index_theta + 1 )*local_nx + index_r*NV_blk ) ];
         data_4 = jtheta[ ( index_phi1 + ( index_theta + 1)*local_nx + index_r*NV_blk ) ];
         data_5 = jtheta[ ( index_phi + index_theta*local_nx + ( index_r + 1 )*NV_blk ) ];
         data_6 = jtheta[ ( index_phi1 + index_theta*local_nx + ( index_r + 1 )*NV_blk ) ];
         data_7 = jtheta[ ( index_phi + ( index_theta + 1 )*local_nx + ( index_r + 1 )*NV_blk ) ];
         data_8 = jtheta[ ( index_phi1 + ( index_theta + 1)*local_nx + ( index_r + 1 )*NV_blk ) ];
      }
      else if ( !strcmp(
            variable_name,
            ur_name) )
      {
         data_1 = ur[ ( index_phi + index_theta*local_nx + index_r*NV_blk ) ];
         data_2 = ur[ ( index_phi1 + index_theta*local_nx + index_r*NV_blk ) ];
         data_3 = ur[ ( index_phi + ( index_theta + 1 )*local_nx + index_r*NV_blk ) ];
         data_4 = ur[ ( index_phi1 + ( index_theta + 1)*local_nx + index_r*NV_blk ) ];
         data_5 = ur[ ( index_phi + index_theta*local_nx + ( index_r + 1 )*NV_blk ) ];
         data_6 = ur[ ( index_phi1 + index_theta*local_nx + ( index_r + 1 )*NV_blk ) ];
         data_7 = ur[ ( index_phi + ( index_theta + 1 )*local_nx + ( index_r + 1 )*NV_blk ) ];
         data_8 = ur[ ( index_phi1 + ( index_theta + 1)*local_nx + ( index_r + 1 )*NV_blk ) ];
      }
      else if ( !strcmp(
            variable_name,
            uphi_name) )
      {
         data_1 = uphi[ ( index_phi + index_theta*local_nx + index_r*NV_blk ) ];
         data_2 = uphi[ ( index_phi1 + index_theta*local_nx + index_r*NV_blk ) ];
         data_3 = uphi[ ( index_phi + ( index_theta + 1 )*local_nx + index_r*NV_blk ) ];
         data_4 = uphi[ ( index_phi1 + ( index_theta + 1)*local_nx + index_r*NV_blk ) ];
         data_5 = uphi[ ( index_phi + index_theta*local_nx + ( index_r + 1 )*NV_blk ) ];
         data_6 = uphi[ ( index_phi1 + index_theta*local_nx + ( index_r + 1 )*NV_blk ) ];
         data_7 = uphi[ ( index_phi + ( index_theta + 1 )*local_nx + ( index_r + 1 )*NV_blk ) ];
         data_8 = uphi[ ( index_phi1 + ( index_theta + 1)*local_nx + ( index_r + 1 )*NV_blk ) ];
      }
      else if ( !strcmp(
            variable_name,
            utheta_name) )
      {
         data_1 = utheta[ ( index_phi + index_theta*local_nx + index_r*NV_blk ) ];
         data_2 = utheta[ ( index_phi1 + index_theta*local_nx + index_r*NV_blk ) ];
         data_3 = utheta[ ( index_phi + ( index_theta + 1 )*local_nx + index_r*NV_blk ) ];
         data_4 = utheta[ ( index_phi1 + ( index_theta + 1)*local_nx + index_r*NV_blk ) ];
         data_5 = utheta[ ( index_phi + index_theta*local_nx + ( index_r + 1 )*NV_blk ) ];
         data_6 = utheta[ ( index_phi1 + index_theta*local_nx + ( index_r + 1 )*NV_blk ) ];
         data_7 = utheta[ ( index_phi + ( index_theta + 1 )*local_nx + ( index_r + 1 )*NV_blk ) ];
         data_8 = utheta[ ( index_phi1 + ( index_theta + 1)*local_nx + ( index_r + 1 )*NV_blk ) ];
      }
      else if ( !strcmp(
            variable_name,
            rho_name) )
      {
         data_1 = rho[ ( index_phi + index_theta*local_nx + index_r*NV_blk ) ];
         data_2 = rho[ ( index_phi1 + index_theta*local_nx + index_r*NV_blk ) ];
         data_3 = rho[ ( index_phi + ( index_theta + 1 )*local_nx + index_r*NV_blk ) ];
         data_4 = rho[ ( index_phi1 + ( index_theta + 1)*local_nx + index_r*NV_blk ) ];
         data_5 = rho[ ( index_phi + index_theta*local_nx + ( index_r + 1 )*NV_blk ) ];
         data_6 = rho[ ( index_phi1 + index_theta*local_nx + ( index_r + 1 )*NV_blk ) ];
         data_7 = rho[ ( index_phi + ( index_theta + 1 )*local_nx + ( index_r + 1 )*NV_blk ) ];
         data_8 = rho[ ( index_phi1 + ( index_theta + 1)*local_nx + ( index_r + 1 )*NV_blk ) ];
      }
      else if ( !strcmp(
            variable_name,
            p_name) )
      {
         data_1 = p[ ( index_phi + index_theta*local_nx + index_r*NV_blk ) ];
         data_2 = p[ ( index_phi1 + index_theta*local_nx + index_r*NV_blk ) ];
         data_3 = p[ ( index_phi + ( index_theta + 1 )*local_nx + index_r*NV_blk ) ];
         data_4 = p[ ( index_phi1 + ( index_theta + 1)*local_nx + index_r*NV_blk ) ];
         data_5 = p[ ( index_phi + index_theta*local_nx + ( index_r + 1 )*NV_blk ) ];
         data_6 = p[ ( index_phi1 + index_theta*local_nx + ( index_r + 1 )*NV_blk ) ];
         data_7 = p[ ( index_phi + ( index_theta + 1 )*local_nx + ( index_r + 1 )*NV_blk ) ];
         data_8 = p[ ( index_phi1 + ( index_theta + 1)*local_nx + ( index_r + 1 )*NV_blk ) ];
      }
      else if ( !strcmp(
            variable_name,
            T_name) )
      {

         /*printf("DEBUG\tgetting T data_1-8\n");*/

         data_1 = T[ ( index_phi + index_theta*local_nx + index_r*NV_blk ) ];
         data_2 = T[ ( index_phi1 + index_theta*local_nx + index_r*NV_blk ) ];
         data_3 = T[ ( index_phi + ( index_theta + 1 )*local_nx + index_r*NV_blk ) ];
         data_4 = T[ ( index_phi1 + ( index_theta + 1)*local_nx + index_r*NV_blk ) ];
         data_5 = T[ ( index_phi + index_theta*local_nx + ( index_r + 1 )*NV_blk ) ];
         data_6 = T[ ( index_phi1 + index_theta*local_nx + ( index_r + 1 )*NV_blk ) ];
         data_7 = T[ ( index_phi + ( index_theta + 1 )*local_nx + ( index_r + 1 )*NV_blk ) ];
         data_8 = T[ ( index_phi1 + ( index_theta + 1)*local_nx + ( index_r + 1 )*NV_blk ) ];

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
            "DEBUG:\t Iphi %d Itheta %d Ir %d Data: %g\n",
            index_phi,
            index_theta+1,
            index_r,
            data_3);
      printf(
            "DEBUG:\t Iphi %d Itheta %d Ir %d Data: %g\n",
            index_phi1,
            index_theta+1,
            index_r,
            data_4);
      printf(
            "DEBUG:\t Iphi %d Itheta %d Ir %d Data: %g\n",
            index_phi,
            index_theta+1,
            index_r+1,
            data_7);
      printf(
            "DEBUG:\t Iphi %d Itheta %d Ir %d Data: %g\n",
            index_phi1,
            index_theta+1,
            index_r+1,
            data_8);
      printf(
            "DEBUG:\t Iphi %d Itheta %d Ir %d Data: %g\n",
            index_phi,
            index_theta,
            index_r,
            data_1);
      printf(
            "DEBUG:\t Iphi %d Itheta %d Ir %d Data: %g\n",
            index_phi1,
            index_theta,
            index_r,
            data_2);
      printf(
            "DEBUG:\t Iphi %d Itheta %d Ir %d Data: %g\n",
            index_phi,
            index_theta,
            index_r+1,
            data_5);
      printf(
            "DEBUG:\t Iphi %d Itheta %d Ir %d Data: %g\n",
            index_phi1,
            index_theta,
            index_r+1,
            data_6);
   }

   /* printf("DEBUG\tdata 1..8 = %g %g %g %g %g %g %g this should be data_8-->%g<--\n", data_1, data_2, data_3, data_4, data_5, data_6, data_7, data_8 ); */

   value = (1-m_z)*( (1-m_y)*( (1-m_x)*data_1 + m_x *data_2 ) + m_y
         *( + (1-m_x)*data_3 + m_x *data_4 ) ) + m_z*( (1-m_y)*( +(1-m_x)
         *data_5 + m_x *data_6 ) + m_y*( +(1-m_x)*data_7 + m_x *data_8 ) );

   return (value );

}

