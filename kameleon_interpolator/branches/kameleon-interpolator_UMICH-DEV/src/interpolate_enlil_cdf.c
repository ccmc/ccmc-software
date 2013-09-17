/*******************************************************************************
 *                                                                             *
 *    NAME:          interpolate_enlil_cdf.c                                   *
 *                                                                             *
 *    VERSION:       1.0                                                       *
 *                                                                             *
 *    AUTHOR:        Marlo M. H. Maddox                                        *
 *                   NASA-GSFC-AETD-ISD-ADMA(587)-CCMC                         *
 *                   Marlo.Maddox@nasa.gov                                     *
 *                   (301) 286 - 5202                                          *
 *                                                                             *
 *    PURPOSE:       Retrun an interpolated variable value for any given       *
 *                   position in the simulation box.  Requires a file to have  *
 *                   alreday been opened with open_cdf call                    *
 *                                                                             *
 *    INPUTS:        (1)  variable name (2-4) x,y,z posotion (5)mem flag       *
 *                   (6)opt arg flag                                           *
 *                                                                             *
 *    OUTPUTS:       unit varibale attribute value                             *
 *                                                                             *
 *    FUNCTIONS:     *                                                         *
 *                                                                             *
 *    VARIABLES:     *                                                         *
 *                                                                             *
 *                                                                             *
 *    MODIFICATION                                                             *
 *    HISTORY:                                                                 *
 *                                                                             *
 *    08.02.2006     Maddox, Marlo     Initial Development Started             *
 *                                                                             *
 *    10.05.2006     Rastaetter, Lutz  Periodic boundary conditions in "z"     *
 *                                     (longitude) valid data is returned for  *
 *                                     positions within one grid spacing beyond*
 *                                     longitude array                         *
 * *  10.05.2006    Maddox, Marlo      Adding a main_memory_flag check/overide *
 *                                     control. Function wil call              *
 *                                     is_var_in_memory() and set mem_flag     *
 *                                     based on return value.                  *
 *                                                                             *
 *    11.09.2006    Maddox, Marlo      Changed return value from double to     *
 *                                     float                                   *
 *                                                                             *
 *    02.22.2007    Maddox, Marlo      Added interpolate_enlil_cdf_2 that      *
 *                                     accepts float arguments. Not fully      *
 *                                     committed to this as default yet        *
 *                                                                             *
 *    02.05.2008  Maddox, Marlo        Changing optional arguments in          *
 *                                     interpolate_enlil_cdf_2 to floats...    *
 *                                                                             *
 *    02.06.2008  Maddox, Marlo        Changed the interpolate_in_block dx, dy *
 *                                     dz arguments from double to float and in*
 *                                     the interpolate_enlil_cdf() routine     *
 *                                     we make sure to copy our d*_float values*
 *                                     returned from interpolate in block into *
 *                                     the user defined double pointer space   *
 *                                                                             *
 *    08.04.2008  Maddox, Marlo        Changing interpolate_enlil_cdf() to use *
 *                                     floats just like the                    *
 *                                     interpolate_enlil_cdf()
 ******************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <math.h>
#include <stdarg.h>
#include "cdf_interface_variables.h"
#include "enlil_cdf_interface_variables.h"
#include "cdf.h"

#define MISSING        -256.*256.*256.*256.*256.
#define MIN_RANGE    -1e9
#define MAX_RANGE    +1e9
#define DEBUG_FLAG    0

float interpolate_enlil_cdf(
      char *variable,
      float X_float_input,
      float Y_float_input,
      float Z_float_input,
      int main_memory_flag,
      int optional_argument_flag,
      ...)
{

   double pi = 3.14159265;
   double au_in_meters = 149598.e6;
   double radian_in_degrees = 57.2957795;
   double degrees_in_radians = 0.0174532925;
   double local_x, local_y, local_z;

   long counts[1] =
   { 0 };
   long intervals[1] =
   { 1 };
   long cdf_varNum;
   int change_sign_flag = 0;

   double interpolated_value;
   double X = (double)X_float_input, Y= (double)Y_float_input, Z=
         (double)Z_float_input;

   float *x_position_array, *y_position_array, *z_position_array; /* generic array pointers that will be set to corresponding enlil grid according to which variable is being interpolated */

   int i, index_x = -1, index_y = -1, index_z = -1; /* used in numerical recipes hunt function */

   float dx_dummy_float, dy_dummy_float, dz_dummy_float; /* dummy variables mem locs to set if no real mem locs are input as optional args */
   double dx_dummy, dy_dummy, dz_dummy; /* dummy variables mem locs to set if no real mem locs are input as optional args */

   /* create variables to pass on to interpolate_in_block regardless if no optional arguments exist ( so interpolate_in_block can have a fixed arg list */
   float missing;
   double *dx, *dy, *dz;
   float *dx_float, *dy_float, *dz_float;

   va_list argptr; /* create a pointer to variable list of arguments */

   double interpolate_in_block_enlil(
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

   /****     since the cdf data is stored in r[meters], radians, radians or r, phi theta
    *         but we are accepting input as r[AU], lon, lat - we must do some coordiante transformations
    */

   local_x = X * au_in_meters; /** convert au to meters **/

   /* 1 *//*local_y = Z - ( pow( pi, 2 ) / 2 );*//** convert from latitude -60 to 60 to radians ...*/
   /* 2 *//*local_y = Z - ( pi / 2 );  *//** convert from latitude -60 to 60 to radians ...*/

   /*local_y = ( Z/radian_in_degrees ) + ( pi/2 );*//** convert from latitude -60...60 to 30...150 range in degress and then to radians...*/

   /*********** 3 ***********/

   Z = -1 * Z;

   /*local_y = ( -1 * ( Z / radian_in_degrees ) ) + ( pi/2); */

   local_y = (Z/radian_in_degrees ) + (pi/2 );

   /**********************************************************************************************************************************************/

   local_z = Y / radian_in_degrees; /** convert degrees ( 0 - 360 longitude ) to radiadns and while fliping y & z or theta -> phi **/

   /*
    #ifdef DEBUG_INTERFACE
    printf("DEBUG\tinput position = [r,lon,lat] = [%f,%f,%f]\n", X,Y,Z);
    printf("DEBUG\t\t\t\t\t\t\t\t\tconverted position = [r,phi,theta] = [%f,%f,%f]\n", local_x,local_y,local_z);
    printf("DEBUG\t\t\t\t\t\t\t\t\tconverted position = [x,y,z]       = [%f,%f,%f]\n", local_x,local_y,local_z);
    #endif
    */

   counts[0] = 0; /*reset values after once through */
   intervals[0] = 1;

   va_start( argptr, optional_argument_flag );
   /* make argptr point to first UNAMED arguments which should be the missing variable */

   /*printf("DEBUG:\tchecking for optional arguments using optional_argument_flag: %d\n", optional_argument_flag );*/

   if (optional_argument_flag)
   {

      /*printf("DEBUG:\toptional arguments are present\n");*/

      /* there are optional arguments 1 - 4, therofore set missing, dx, dy, dz */
      /* missing = va_arg( argptr, float );*/

      /*hack = va_arg( argptr, double );*/
      missing = va_arg( argptr, double );
      dx_float = va_arg( argptr, float * );
      dy_float = va_arg( argptr, float * );
      dz_float = va_arg( argptr, float * );

      /*printf("DEBUG:\toptional arguments are present setting missing value to -->%f<-- and also copying pointers...\n", missing);*/

   }
   else
   {
      /*printf("DEBUG:\tabout to set missing variable\n");*/
      missing = MISSING;
      dx_float = &dx_dummy_float;
      dy_float = &dy_dummy_float;
      dz_float = &dz_dummy_float;
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
         bx_name) || !strcmp(
         variable,
         br_name) )
   {
      cdf_varNum = bx_cdf_varNum;
   }
   else if ( !strcmp(
         variable,
         by_name) || !strcmp(
         variable,
         bphi_name) )
   {
      cdf_varNum = by_cdf_varNum;
      change_sign_flag = 1;
   }
   else if ( !strcmp(
         variable,
         bz_name) || !strcmp(
         variable,
         btheta_name))
   {
      cdf_varNum = bz_cdf_varNum;
   }
   else if ( !strcmp(
         variable,
         b1x_name) || !strcmp(
         variable,
         b1r_name) )
   {
      cdf_varNum = b1x_cdf_varNum;
   }
   else if ( !strcmp(
         variable,
         b1y_name) || !strcmp(
         variable,
         b1phi_name))
   {
      cdf_varNum = b1y_cdf_varNum;
      change_sign_flag = 1;
   }
   else if ( !strcmp(
         variable,
         b1z_name) || !strcmp(
         variable,
         b1theta_name))
   {
      cdf_varNum = b1z_cdf_varNum;
   }
   else if ( !strcmp(
         variable,
         ux_name) || !strcmp(
         variable,
         ur_name))
   {
      cdf_varNum = ux_cdf_varNum;
   }
   else if ( !strcmp(
         variable,
         uy_name) || !strcmp(
         variable,
         uphi_name))
   {
      cdf_varNum = uy_cdf_varNum;
      change_sign_flag = 1;
   }
   else if ( !strcmp(
         variable,
         uz_name) || !strcmp(
         variable,
         utheta_name))
   {
      cdf_varNum = uz_cdf_varNum;
   }
   else if ( !strcmp(
         variable,
         rho_name) )
   {
      cdf_varNum = rho_cdf_varNum;
   }
   else if ( !strcmp(
         variable,
         dp_name) )
   {
      cdf_varNum = dp_cdf_varNum;
   }
   else if ( !strcmp(
         variable,
         bp_name) )
   {
      cdf_varNum = bp_cdf_varNum;
   }
   else if ( !strcmp(
         variable,
         T_name) )
   {
      cdf_varNum = T_cdf_varNum;
   }
   else
   {
      printf(
            "ERROR:\tcould not find cdf variable number for %s\n",
            variable);
      printf(
            "Valid Variable Names for ENLIL :\n%s OR %s\n%s OR %s\n%s OR %s\n%s OR %s\n%s OR %s\n%s OR %s\n%s OR %s\n%s OR %s\n%s OR %s\n%s\n%s\n%s\n%s\n----------------------------------------------\n",
            bx_name,
            br_name,
            by_name,
            bphi_name,
            bz_name,
            btheta_name,
            b1x_name,
            b1r_name,
            b1y_name,
            b1phi_name,
            b1z_name,
            b1theta_name,
            ux_name,
            ur_name,
            uy_name,
            uphi_name,
            uz_name,
            utheta_name,
            rho_name,
            dp_name,
            bp_name,
            T_name);
      return 0;
   }

   /*
    printf("DEBUG:\tabout to call external hunt routine using indices -----> %d, %d, %d \n", index_x, index_y, index_z);

    printf("DEBUG:\tx[0] = %f, nx = %d, X = %f\n", x_position_array[0], nx, X );
    printf("DEBUG:\ty[0] = %f, ny = %d, Y = %f\n", y_position_array[0], ny, Y );
    printf("DEBUG:\tz[0] = %f, ny = %d, Z = %f\n", z_position_array[0], nz, Z );
    */

   /*
    hunt( x_position_array, nx, X, &index_x );
    hunt( y_position_array, ny, Y, &index_y );
    hunt( z_position_array, nz, Z, &index_z );
    */
   hunt(
         x_position_array,
         nx,
         local_x,
         &index_x);
   hunt(
         y_position_array,
         ny,
         local_y,
         &index_y);
   hunt(
         z_position_array,
         nz,
         local_z,
         &index_z);

   /** lutz reverses his y_blk array so lets invert the found index ***/

   /*printf("index_y before reverse = %d\n", index_y );*/

   /*index_y = ( ny - 1 ) - index_y;*/

   /*printf("index_y after reverse = %d\n", index_y );*/

   /*printf("DEBUG\tback from hunt\n");*/

   /* index_* point to the cell to the left below the current position OR indicates that the position is out of range by returning index* = -1 or n* */

   if ( (index_x < 0 ) || (index_x >= nx-1 ) || (index_y < 0 ) || (index_y
         >= ny-1 )
   /*    "z" is periodic
    || ( index_z < 0 ) || ( index_z >= nz-1 ) */
   )
   {
      return ( (float)missing );
   }
   else
   {

      /*printf("DEBUG:\thunt calls completed successfully\n");*/

      if ( DEBUG_FLAG)
         printf(
               "DEBUG:\tINTERPOLATE ENLIL 2\nabout to call interpolate_in_block using indices -----> "
                  "%d, %d, %d \nranges are %d, %d, %d",
               index_x,
               index_y,
               index_z,
               nx-1,
               ny-1,
               nz-1);
      /*
       interpolated_value = interpolate_in_block_enlil( X, Z, Y, index_x, index_z, index_y, x_position_array, z_position_array, y_position_array, variable, dx, dz, dy, cdf_varNum, main_memory_flag );
       */

      interpolated_value = interpolate_in_block_enlil(
            local_x,
            local_y,
            local_z,
            index_x,
            index_y,
            index_z,
            x_position_array,
            y_position_array,
            z_position_array,
            variable,
            dx_float,
            dy_float,
            dz_float,
            cdf_varNum,
            main_memory_flag);

      /****** we need to change the sign of any y vector component ... *********/

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

/****************************** this is a duplicate interpolate_enlil_cdf function that accepts float arguments vs double
 * **************************** the same could be do by just type casting the arguments but for convienience...
 */

float interpolate_enlil_cdf_2(
      char *variable,
      float X_float_input,
      float Y_float_input,
      float Z_float_input,
      int main_memory_flag,
      int optional_argument_flag,
      ...)
{

   double pi = 3.14159265;
   double au_in_meters = 149598.e6;
   double radian_in_degrees = 57.2957795;
   double degrees_in_radians = 0.0174532925;
   double local_x, local_y, local_z;

   long counts[1] =
   { 0 };
   long intervals[1] =
   { 1 };
   long cdf_varNum;
   int change_sign_flag = 0;

   double interpolated_value;
   double X = (double)X_float_input, Y= (double)Y_float_input, Z=
         (double)Z_float_input;

   float *x_position_array, *y_position_array, *z_position_array; /* generic array pointers that will be set to corresponding enlil grid according to which variable is being interpolated */

   int i, index_x = -1, index_y = -1, index_z = -1; /* used in numerical recipes hunt function */

   float dx_dummy_float, dy_dummy_float, dz_dummy_float; /* dummy variables mem locs to set if no real mem locs are input as optional args */
   double dx_dummy, dy_dummy, dz_dummy; /* dummy variables mem locs to set if no real mem locs are input as optional args */

   /* create variables to pass on to interpolate_in_block regardless if no optional arguments exist ( so interpolate_in_block can have a fixed arg list */
   float missing;
   double *dx, *dy, *dz;
   float *dx_float, *dy_float, *dz_float;

   va_list argptr; /* create a pointer to variable list of arguments */

   double interpolate_in_block_enlil(
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

   /****     since the cdf data is stored in r[meters], radians, radians or r, phi theta
    *         but we are accepting input as r[AU], lon, lat - we must do some coordiante transformations
    */

   local_x = X * au_in_meters; /** convert au to meters **/

   /* 1 *//*local_y = Z - ( pow( pi, 2 ) / 2 );*//** convert from latitude -60 to 60 to radians ...*/
   /* 2 *//*local_y = Z - ( pi / 2 );  *//** convert from latitude -60 to 60 to radians ...*/

   /*local_y = ( Z/radian_in_degrees ) + ( pi/2 );*//** convert from latitude -60...60 to 30...150 range in degress and then to radians...*/

   /*********** 3 ***********/

   Z = -1 * Z;

   /*local_y = ( -1 * ( Z / radian_in_degrees ) ) + ( pi/2); */

   local_y = (Z/radian_in_degrees ) + (pi/2 );

   /**********************************************************************************************************************************************/

   local_z = Y / radian_in_degrees; /** convert degrees ( 0 - 360 longitude ) to radiadns and while fliping y & z or theta -> phi **/

   /*
    #ifdef DEBUG_INTERFACE
    printf("DEBUG\tinput position = [r,lon,lat] = [%f,%f,%f]\n", X,Y,Z);
    printf("DEBUG\t\t\t\t\t\t\t\t\tconverted position = [r,phi,theta] = [%f,%f,%f]\n", local_x,local_y,local_z);
    printf("DEBUG\t\t\t\t\t\t\t\t\tconverted position = [x,y,z]       = [%f,%f,%f]\n", local_x,local_y,local_z);
    #endif
    */

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
      dx_float = va_arg( argptr, float * );
      dy_float = va_arg( argptr, float * );
      dz_float = va_arg( argptr, float * );

      /*printf("DEBUG:\toptional arguments are present setting missing value to -->%f<-- and also copying pointers...\n", missing);*/

   }
   else
   {
      /*printf("DEBUG:\tabout to set missing variable\n");*/
      missing = MISSING;
      dx_float = &dx_dummy_float;
      dy_float = &dy_dummy_float;
      dz_float = &dz_dummy_float;
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
         bx_name) || !strcmp(
         variable,
         br_name) )
   {
      cdf_varNum = bx_cdf_varNum;
   }
   else if ( !strcmp(
         variable,
         by_name) || !strcmp(
         variable,
         bphi_name) )
   {
      cdf_varNum = by_cdf_varNum;
      change_sign_flag = 1;
   }
   else if ( !strcmp(
         variable,
         bz_name) || !strcmp(
         variable,
         btheta_name))
   {
      cdf_varNum = bz_cdf_varNum;
   }
   else if ( !strcmp(
         variable,
         b1x_name) || !strcmp(
         variable,
         b1r_name) )
   {
      cdf_varNum = b1x_cdf_varNum;
   }
   else if ( !strcmp(
         variable,
         b1y_name) || !strcmp(
         variable,
         b1phi_name))
   {
      cdf_varNum = b1y_cdf_varNum;
      change_sign_flag = 1;
   }
   else if ( !strcmp(
         variable,
         b1z_name) || !strcmp(
         variable,
         b1theta_name))
   {
      cdf_varNum = b1z_cdf_varNum;
   }
   else if ( !strcmp(
         variable,
         ux_name) || !strcmp(
         variable,
         ur_name))
   {
      cdf_varNum = ux_cdf_varNum;
   }
   else if ( !strcmp(
         variable,
         uy_name) || !strcmp(
         variable,
         uphi_name))
   {
      cdf_varNum = uy_cdf_varNum;
      change_sign_flag = 1;
   }
   else if ( !strcmp(
         variable,
         uz_name) || !strcmp(
         variable,
         utheta_name))
   {
      cdf_varNum = uz_cdf_varNum;
   }
   else if ( !strcmp(
         variable,
         rho_name) )
   {
      cdf_varNum = rho_cdf_varNum;
   }
   else if ( !strcmp(
         variable,
         dp_name) )
   {
      cdf_varNum = dp_cdf_varNum;
   }
   else if ( !strcmp(
         variable,
         bp_name) )
   {
      cdf_varNum = bp_cdf_varNum;
   }
   else if ( !strcmp(
         variable,
         T_name) )
   {
      cdf_varNum = T_cdf_varNum;
   }
   else
   {
      printf(
            "ERROR:\tcould not find cdf variable number for %s\n",
            variable);
      printf(
            "Valid Variable Names for ENLIL :\n%s OR %s\n%s OR %s\n%s OR %s\n%s OR %s\n%s OR %s\n%s OR %s\n%s OR %s\n%s OR %s\n%s OR %s\n%s\n%s\n%s\n%s\n----------------------------------------------\n",
            bx_name,
            br_name,
            by_name,
            bphi_name,
            bz_name,
            btheta_name,
            b1x_name,
            b1r_name,
            b1y_name,
            b1phi_name,
            b1z_name,
            b1theta_name,
            ux_name,
            ur_name,
            uy_name,
            uphi_name,
            uz_name,
            utheta_name,
            rho_name,
            dp_name,
            bp_name,
            T_name);
      return 0;
   }

   /*
    printf("DEBUG:\tabout to call external hunt routine using indices -----> %d, %d, %d \n", index_x, index_y, index_z);

    printf("DEBUG:\tx[0] = %f, nx = %d, X = %f\n", x_position_array[0], nx, X );
    printf("DEBUG:\ty[0] = %f, ny = %d, Y = %f\n", y_position_array[0], ny, Y );
    printf("DEBUG:\tz[0] = %f, ny = %d, Z = %f\n", z_position_array[0], nz, Z );
    */

   /*
    hunt( x_position_array, nx, X, &index_x );
    hunt( y_position_array, ny, Y, &index_y );
    hunt( z_position_array, nz, Z, &index_z );
    */
   hunt(
         x_position_array,
         nx,
         local_x,
         &index_x);
   hunt(
         y_position_array,
         ny,
         local_y,
         &index_y);
   hunt(
         z_position_array,
         nz,
         local_z,
         &index_z);

   /** lutz reverses his y_blk array so lets invert the found index ***/

   /*printf("index_y before reverse = %d\n", index_y );*/

   /*index_y = ( ny - 1 ) - index_y;*/

   /*printf("index_y after reverse = %d\n", index_y );*/

   /*printf("DEBUG\tback from hunt\n");*/

   /* index_* point to the cell to the left below the current position OR indicates that the position is out of range by returning index* = -1 or n* */

   if ( (index_x < 0 ) || (index_x >= nx-1 ) || (index_y < 0 ) || (index_y
         >= ny-1 )
   /*    "z" is periodic
    || ( index_z < 0 ) || ( index_z >= nz-1 ) */
   )
   {
      return ( (float)missing );
   }
   else
   {

      /*printf("DEBUG:\thunt calls completed successfully\n");*/

      if ( DEBUG_FLAG)
         printf(
               "DEBUG:\tINTERPOLATE ENLIL 2\nabout to call interpolate_in_block using indices -----> "
                  "%d, %d, %d \nranges are %d, %d, %d",
               index_x,
               index_y,
               index_z,
               nx-1,
               ny-1,
               nz-1);
      /*
       interpolated_value = interpolate_in_block_enlil( X, Z, Y, index_x, index_z, index_y, x_position_array, z_position_array, y_position_array, variable, dx, dz, dy, cdf_varNum, main_memory_flag );
       */

      interpolated_value = interpolate_in_block_enlil(
            local_x,
            local_y,
            local_z,
            index_x,
            index_y,
            index_z,
            x_position_array,
            y_position_array,
            z_position_array,
            variable,
            dx_float,
            dy_float,
            dz_float,
            cdf_varNum,
            main_memory_flag);

      /****** we need to change the sign of any y vector component ... *********/

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

double interpolate_in_block_enlil(
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
   float dx_blk, dy_blk, dz_blk, m_x, m_y, m_z, two_pi=4*asin(1.);
   float data_1a, data_2a, data_3a, data_4a, data_5a, data_6a, data_7a, data_8a; /** values from actual data used for interploation equation these are floats for cdflib call because the actual variable are of type CDF_FLOAT or float... **/
   double data_1, data_2, data_3, data_4, data_5, data_6, data_7, data_8; /** values from actual data used for interploation equation **/

   /*int ix, iy, iz;*/
   int NV_blk = nx * ny, iz1=-1;

   /* int first_cell_in_block = found_block_index * nx * ny * nz; */

   /*printf("interpolating block %d\n", found_block_index );*/

   /*find_in_block( x, y, z, found_block_index, &ix, &iy, &iz, &data_ix, &data_iy, &data_iz );*/

   dx_blk = x_pos[ ix + 1 ] - x_pos[ ix ];
   dy_blk = y_pos[ iy + 1 ] - y_pos[ iy ];

   /* periodic boundary */
   if ( (iz == (nz-1) ) || (iz == -1))
   {
      iz=nz-1;
      iz1=0;
      dz_blk = (z_pos[ iz1 ] - z_pos[ iz ] + two_pi);
      m_z = (z - z_pos[ nz-1 ] + (z_pos[0] > z)*two_pi )/ dz_blk;
   }
   else
   {
      iz1=iz+1;
      dz_blk = z_pos[ iz + 1 ] - z_pos[ iz ];
      m_z = (z - z_pos[ iz ] ) / dz_blk;
   }

   if ( DEBUG_FLAG)
      printf(
            "\nDEBUG\tdx = %f, dy = %f, dz = %f\n",
            dx_blk,
            dy_blk,
            dz_blk);

   *dx = dx_blk; /* return values to caller */
   *dy = dy_blk;
   *dz = dz_blk;

   m_x = (x - x_pos[ ix ] ) / dx_blk;
   m_y = (y - y_pos[ iy ] ) / dy_blk;

#ifdef DEBUG
#define AU 1.49598e11
   printf("DEBUG\tx0 = %f\tx1 = %f\n\ty0 = %f\ty1= %f\nz0 = %f\tz1= %f\n", x_pos[ix]/AU,x_pos[ix+1]/AU,y_pos[0],y_pos[ny-1],z_pos[0],z_pos[nz-1] );
   printf("DEBUG\tX = %f\tY = %f\tZ = %f\n", x,y,z );
   printf("DEBUG\tdx = %f\n\tdy = %f\n\tdz = %f\nand ix = %d\tiy = %d\tiz = %d "
         "iz1 = %d\n\nand m_x = %f\tm_y = %f\tm_z = %f\n", dx_blk, dy_blk, dz_blk, ix, iy, iz,iz1,m_x,m_y,m_z );
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

      indices[0] = (ix + iy*nx + iz*NV_blk );
      status = CDFlib(
      SELECT_, zVAR_, cdf_varNum,
      SELECT_, zVAR_RECNUMBER_, recordStart,
      SELECT_, zVAR_DIMINDICES_, indices,
      GET_, zVAR_DATA_, &data_1a,
      NULL_);
      if (status != CDF_OK)
         StatusHandler(status);

      indices[0] = (ix + 1 + iy*nx + iz*NV_blk );
      status = CDFlib(
      SELECT_, zVAR_, cdf_varNum,
      SELECT_, zVAR_RECNUMBER_, recordStart,
      SELECT_, zVAR_DIMINDICES_, indices,
      GET_, zVAR_DATA_, &data_2a,
      NULL_);
      if (status != CDF_OK)
         StatusHandler(status);

      indices[0] = (ix + (iy + 1 )*nx + iz*NV_blk );
      status = CDFlib(
      SELECT_, zVAR_, cdf_varNum,
      SELECT_, zVAR_RECNUMBER_, recordStart,
      SELECT_, zVAR_DIMINDICES_, indices,
      GET_, zVAR_DATA_, &data_3a,
      NULL_);
      if (status != CDF_OK)
         StatusHandler(status);

      indices[0] = (ix + 1 + (iy + 1)*nx + iz*NV_blk );
      status = CDFlib(
      SELECT_, zVAR_, cdf_varNum,
      SELECT_, zVAR_RECNUMBER_, recordStart,
      SELECT_, zVAR_DIMINDICES_, indices,
      GET_, zVAR_DATA_, &data_4a,
      NULL_);
      if (status != CDF_OK)
         StatusHandler(status);

      indices[0] = (ix + iy*nx + (iz1 )*NV_blk );
      status = CDFlib(
      SELECT_, zVAR_, cdf_varNum,
      SELECT_, zVAR_RECNUMBER_, recordStart,
      SELECT_, zVAR_DIMINDICES_, indices,
      GET_, zVAR_DATA_, &data_5a,
      NULL_);
      if (status != CDF_OK)
         StatusHandler(status);

      indices[0] = (ix + 1 + iy*nx + (iz1 )*NV_blk );
      status = CDFlib(
      SELECT_, zVAR_, cdf_varNum,
      SELECT_, zVAR_RECNUMBER_, recordStart,
      SELECT_, zVAR_DIMINDICES_, indices,
      GET_, zVAR_DATA_, &data_6a,
      NULL_);
      if (status != CDF_OK)
         StatusHandler(status);

      indices[0] = (ix + (iy + 1 )*nx + (iz1 )*NV_blk );
      status = CDFlib(
      SELECT_, zVAR_, cdf_varNum,
      SELECT_, zVAR_RECNUMBER_, recordStart,
      SELECT_, zVAR_DIMINDICES_, indices,
      GET_, zVAR_DATA_, &data_7a,
      NULL_);
      if (status != CDF_OK)
         StatusHandler(status);

      indices[0] = (ix + 1 + (iy + 1)*nx + (iz1 )*NV_blk );
      status = CDFlib(
      SELECT_, zVAR_, cdf_varNum,
      SELECT_, zVAR_RECNUMBER_, recordStart,
      SELECT_, zVAR_DIMINDICES_, indices,
      GET_, zVAR_DATA_, &data_8a,
      NULL_);
      if (status != CDF_OK)
         StatusHandler(status);

      /********* now copy floats to doubles **********/

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
         data_5 = bx[ ( ix + iy*nx + ( iz1 )*NV_blk ) ];
         data_6 = bx[ ( ix + 1 + iy*nx + ( iz1 )*NV_blk ) ];
         data_7 = bx[ ( ix + ( iy + 1 )*nx + ( iz1 )*NV_blk ) ];
         data_8 = bx[ ( ix + 1 + ( iy + 1)*nx + ( iz1)*NV_blk ) ];
      }
      else if ( !strcmp(
            variable_name,
            by_name) )
      {
         data_1 = by[ ( ix + iy*nx + iz*NV_blk ) ];
         data_2 = by[ ( ix + 1 + iy*nx + iz*NV_blk ) ];
         data_3 = by[ ( ix + ( iy + 1 )*nx + iz*NV_blk ) ];
         data_4 = by[ ( ix + 1 + ( iy + 1)*nx + iz*NV_blk ) ];
         data_5 = by[ ( ix + iy*nx + ( iz1 )*NV_blk ) ];
         data_6 = by[ ( ix + 1 + iy*nx + ( iz1 )*NV_blk ) ];
         data_7 = by[ ( ix + ( iy + 1 )*nx + ( iz1 )*NV_blk ) ];
         data_8 = by[ ( ix + 1 + ( iy + 1)*nx + ( iz1)*NV_blk ) ];
      }
      else if ( !strcmp(
            variable_name,
            bz_name) )
      {
         data_1 = bz[ ( ix + iy*nx + iz*NV_blk ) ];
         data_2 = bz[ ( ix + 1 + iy*nx + iz*NV_blk ) ];
         data_3 = bz[ ( ix + ( iy + 1 )*nx + iz*NV_blk ) ];
         data_4 = bz[ ( ix + 1 + ( iy + 1)*nx + iz*NV_blk ) ];
         data_5 = bz[ ( ix + iy*nx + ( iz1 )*NV_blk ) ];
         data_6 = bz[ ( ix + 1 + iy*nx + ( iz1 )*NV_blk ) ];
         data_7 = bz[ ( ix + ( iy + 1 )*nx + ( iz1 )*NV_blk ) ];
         data_8 = bz[ ( ix + 1 + ( iy + 1)*nx + ( iz1)*NV_blk ) ];
      }
      else if ( !strcmp(
            variable_name,
            b1x_name) )
      {
         data_1 = b1x[ ( ix + iy*nx + iz*NV_blk ) ];
         data_2 = b1x[ ( ix + 1 + iy*nx + iz*NV_blk ) ];
         data_3 = b1x[ ( ix + ( iy + 1 )*nx + iz*NV_blk ) ];
         data_4 = b1x[ ( ix + 1 + ( iy + 1)*nx + iz*NV_blk ) ];
         data_5 = b1x[ ( ix + iy*nx + ( iz1 )*NV_blk ) ];
         data_6 = b1x[ ( ix + 1 + iy*nx + ( iz1 )*NV_blk ) ];
         data_7 = b1x[ ( ix + ( iy + 1 )*nx + ( iz1 )*NV_blk ) ];
         data_8 = b1x[ ( ix + 1 + ( iy + 1)*nx + ( iz1)*NV_blk ) ];
      }
      else if ( !strcmp(
            variable_name,
            b1y_name) )
      {
         data_1 = b1y[ ( ix + iy*nx + iz*NV_blk ) ];
         data_2 = b1y[ ( ix + 1 + iy*nx + iz*NV_blk ) ];
         data_3 = b1y[ ( ix + ( iy + 1 )*nx + iz*NV_blk ) ];
         data_4 = b1y[ ( ix + 1 + ( iy + 1)*nx + iz*NV_blk ) ];
         data_5 = b1y[ ( ix + iy*nx + ( iz1 )*NV_blk ) ];
         data_6 = b1y[ ( ix + 1 + iy*nx + ( iz1 )*NV_blk ) ];
         data_7 = b1y[ ( ix + ( iy + 1 )*nx + ( iz1 )*NV_blk ) ];
         data_8 = b1y[ ( ix + 1 + ( iy + 1)*nx + ( iz1)*NV_blk ) ];
      }
      else if ( !strcmp(
            variable_name,
            b1z_name) )
      {
         data_1 = b1z[ ( ix + iy*nx + iz*NV_blk ) ];
         data_2 = b1z[ ( ix + 1 + iy*nx + iz*NV_blk ) ];
         data_3 = b1z[ ( ix + ( iy + 1 )*nx + iz*NV_blk ) ];
         data_4 = b1z[ ( ix + 1 + ( iy + 1)*nx + iz*NV_blk ) ];
         data_5 = b1z[ ( ix + iy*nx + ( iz1 )*NV_blk ) ];
         data_6 = b1z[ ( ix + 1 + iy*nx + ( iz1 )*NV_blk ) ];
         data_7 = b1z[ ( ix + ( iy + 1 )*nx + ( iz1 )*NV_blk ) ];
         data_8 = b1z[ ( ix + 1 + ( iy + 1)*nx + ( iz1)*NV_blk ) ];
      }
      else if ( !strcmp(
            variable_name,
            ux_name) )
      {
         data_1 = ux[ ( ix + iy*nx + iz*NV_blk ) ];
         data_2 = ux[ ( ix + 1 + iy*nx + iz*NV_blk ) ];
         data_3 = ux[ ( ix + ( iy + 1 )*nx + iz*NV_blk ) ];
         data_4 = ux[ ( ix + 1 + ( iy + 1)*nx + iz*NV_blk ) ];
         data_5 = ux[ ( ix + iy*nx + ( iz1 )*NV_blk ) ];
         data_6 = ux[ ( ix + 1 + iy*nx + ( iz1 )*NV_blk ) ];
         data_7 = ux[ ( ix + ( iy + 1 )*nx + ( iz1 )*NV_blk ) ];
         data_8 = ux[ ( ix + 1 + ( iy + 1)*nx + ( iz1)*NV_blk ) ];
      }
      else if ( !strcmp(
            variable_name,
            uy_name) )
      {
         data_1 = uy[ ( ix + iy*nx + iz*NV_blk ) ];
         data_2 = uy[ ( ix + 1 + iy*nx + iz*NV_blk ) ];
         data_3 = uy[ ( ix + ( iy + 1 )*nx + iz*NV_blk ) ];
         data_4 = uy[ ( ix + 1 + ( iy + 1)*nx + iz*NV_blk ) ];
         data_5 = uy[ ( ix + iy*nx + ( iz1 )*NV_blk ) ];
         data_6 = uy[ ( ix + 1 + iy*nx + ( iz1 )*NV_blk ) ];
         data_7 = uy[ ( ix + ( iy + 1 )*nx + ( iz1 )*NV_blk ) ];
         data_8 = uy[ ( ix + 1 + ( iy + 1)*nx + ( iz1)*NV_blk ) ];
      }
      else if ( !strcmp(
            variable_name,
            uz_name) )
      {
         data_1 = uz[ ( ix + iy*nx + iz*NV_blk ) ];
         data_2 = uz[ ( ix + 1 + iy*nx + iz*NV_blk ) ];
         data_3 = uz[ ( ix + ( iy + 1 )*nx + iz*NV_blk ) ];
         data_4 = uz[ ( ix + 1 + ( iy + 1)*nx + iz*NV_blk ) ];
         data_5 = uz[ ( ix + iy*nx + ( iz1 )*NV_blk ) ];
         data_6 = uz[ ( ix + 1 + iy*nx + ( iz1 )*NV_blk ) ];
         data_7 = uz[ ( ix + ( iy + 1 )*nx + ( iz1 )*NV_blk ) ];
         data_8 = uz[ ( ix + 1 + ( iy + 1)*nx + ( iz1)*NV_blk ) ];
      }
      else if ( !strcmp(
            variable_name,
            rho_name) )
      {
         data_1 = rho[ ( ix + iy*nx + iz*NV_blk ) ];
         data_2 = rho[ ( ix + 1 + iy*nx + iz*NV_blk ) ];
         data_3 = rho[ ( ix + ( iy + 1 )*nx + iz*NV_blk ) ];
         data_4 = rho[ ( ix + 1 + ( iy + 1)*nx + iz*NV_blk ) ];
         data_5 = rho[ ( ix + iy*nx + ( iz1 )*NV_blk ) ];
         data_6 = rho[ ( ix + 1 + iy*nx + ( iz1 )*NV_blk ) ];
         data_7 = rho[ ( ix + ( iy + 1 )*nx + ( iz1 )*NV_blk ) ];
         data_8 = rho[ ( ix + 1 + ( iy + 1)*nx + ( iz1)*NV_blk ) ];
      }
      else if ( !strcmp(
            variable_name,
            dp_name) )
      {
         data_1 = dp[ ( ix + iy*nx + iz*NV_blk ) ];
         data_2 = dp[ ( ix + 1 + iy*nx + iz*NV_blk ) ];
         data_3 = dp[ ( ix + ( iy + 1 )*nx + iz*NV_blk ) ];
         data_4 = dp[ ( ix + 1 + ( iy + 1)*nx + iz*NV_blk ) ];
         data_5 = dp[ ( ix + iy*nx + ( iz1 )*NV_blk ) ];
         data_6 = dp[ ( ix + 1 + iy*nx + ( iz1 )*NV_blk ) ];
         data_7 = dp[ ( ix + ( iy + 1 )*nx + ( iz1 )*NV_blk ) ];
         data_8 = dp[ ( ix + 1 + ( iy + 1)*nx + ( iz1)*NV_blk ) ];
      }
      else if ( !strcmp(
            variable_name,
            bp_name) )
      {
         data_1 = bp[ ( ix + iy*nx + iz*NV_blk ) ];
         data_2 = bp[ ( ix + 1 + iy*nx + iz*NV_blk ) ];
         data_3 = bp[ ( ix + ( iy + 1 )*nx + iz*NV_blk ) ];
         data_4 = bp[ ( ix + 1 + ( iy + 1)*nx + iz*NV_blk ) ];
         data_5 = bp[ ( ix + iy*nx + ( iz1 )*NV_blk ) ];
         data_6 = bp[ ( ix + 1 + iy*nx + ( iz1 )*NV_blk ) ];
         data_7 = bp[ ( ix + ( iy + 1 )*nx + ( iz1 )*NV_blk ) ];
         data_8 = bp[ ( ix + 1 + ( iy + 1)*nx + ( iz1)*NV_blk ) ];
      }
      else if ( !strcmp(
            variable_name,
            T_name) )
      {

         /*printf("DEBUG\tgetting T data_1-8\n");*/

         data_1 = T[ ( ix + iy*nx + iz*NV_blk ) ];
         data_2 = T[ ( ix + 1 + iy*nx + iz*NV_blk ) ];
         data_3 = T[ ( ix + ( iy + 1 )*nx + iz*NV_blk ) ];
         data_4 = T[ ( ix + 1 + ( iy + 1)*nx + iz*NV_blk ) ];
         data_5 = T[ ( ix + iy*nx + ( iz1 )*NV_blk ) ];
         data_6 = T[ ( ix + 1 + iy*nx + ( iz1 )*NV_blk ) ];
         data_7 = T[ ( ix + ( iy + 1 )*nx + ( iz1 )*NV_blk ) ];
         data_8 = T[ ( ix + 1 + ( iy + 1)*nx + ( iz1)*NV_blk ) ];

      }
      else
      {
         /* INSERT ERROR HANDLER HERE */
         printf("ERROR\tcouldn't find data variable...\n");
      }

   }

#ifdef DEBUG
   printf("debug:\tfinished getting data values from cdf\n");
   printf("DEBUG:\t IX %d IY %d IZ %d Data: %g\n",ix ,iz ,iy+1,data_3);
   printf("DEBUG:\t IX %d IY %d IZ %d Data: %g\n",ix+1,iz ,iy+1,data_4);
   printf("DEBUG:\t IX %d IY %d IZ %d Data: %g\n",ix ,iz1,iy+1,data_7);
   printf("DEBUG:\t IX %d IY %d IZ %d Data: %g\n",ix+1,iz1,iy+1,data_8);
   printf("DEBUG:\t IX %d IY %d IZ %d Data: %g\n",ix ,iz ,iy ,data_1);
   printf("DEBUG:\t IX %d IY %d IZ %d Data: %g\n",ix+1,iz ,iy ,data_2);
   printf("DEBUG:\t IX %d IY %d IZ %d Data: %g\n",ix ,iz1,iy ,data_5);
   printf("DEBUG:\t IX %d IY %d IZ %d Data: %g\n",ix+1,iz1,iy ,data_6);
#endif
   /* printf("DEBUG\tdata 1..8 = %g %g %g %g %g %g %g this should be data_8-->%g<--\n", data_1, data_2, data_3, data_4, data_5, data_6, data_7, data_8 ); */

   value = (1-m_z)*( (1-m_y)*( (1-m_x)*data_1 + m_x *data_2 ) + m_y
         *( + (1-m_x)*data_3 + m_x *data_4 ) ) + m_z*( (1-m_y)*( +(1-m_x)
         *data_5 + m_x *data_6 ) + m_y*( +(1-m_x)*data_7 + m_x *data_8 ) );

   return (value );

}

