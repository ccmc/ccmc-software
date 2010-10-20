/*******************************************************************************
 *                                                                             *
 *       NAME:          interpolate_ctip_cdf.c                                 *
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
 *       08.08.2005     Maddox, Marlo                                          *
 *                      Initial Development Started                            *
 *       07.20.2006     Maddox, Marlo                                          *
 *                      Added more verbose error message for variable names    *
 *                      that are not found - prints list of valid variables    *
 *                      names now                                              *
 *       10.05.2006     Maddox, Marlo                                          *
 *                      Adding a main_memory_flag check/overide control.       *
 *                      Function wil call is_var_in_memory() and set mem_flag  *
 *                      based on return value.                                 *
 ******************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <math.h>
#include <stdarg.h>
#include "cdf_interface_variables.h"
#include "ctip_cdf_interface_variables.h"
#include "cdf.h"

/*
 #define DEBUG = 0
 */

#define MISSING        -1.e31
#define MIN_RANGE    -1e9
#define MAX_RANGE    +1e9

#define min(a,b) ( a < b ? a : b )
#define max(a,b) ( a > b ? a : b )

float interpolate_ctip_cdf(
      char *variable,
      float LON,
      float LAT,
      float H,
      int main_memory_flag,
      int optional_argument_flag,
      ...)
/*float interpolate_ctip_cdf( char *variable, float Z, float Y, float H, int main_memory_flag, int optional_argument_flag, ... ) */
{

   /** LON[0 - 360] LAT[-90 - 90] H[~80 - ~550]  **/

   float interpolate_in_block_ctip(
         float,
         float,
         float,
         int,
         int,
         int,
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
         int,
         float,
         float,
         float,
         float,
         float,
         float,
         float,
         float);

   extern void hunt(
         float *,
         int,
         float,
         int *);

   long counts[1] =
   { 0 };
   long intervals[1] =
   { 1 };
   long cdf_varNum;

   float X, Y, Z; /* Height or IP, COLAT = 90 - LAT,  and LON */

   /*** variables needed for interpolation ***/

   int i, j;
   int NX_blk, NY_blk, NZ_blk, NV_blk;

   float dx_blk, dy_blk, dz_blk, m_x0, m_y0, m_x1, m_y1, m_z0, m_z1;

   float m000, m001, m010, m011, m100, m101, m110, m111;

   float interpolated_value;
   float *x_position_array, *y_position_array, *z_position_array,
         *H_position_array; /* generic array pointers that will be set to corresponding ctip grid */

   float *height_column; /* store interpolated height values here */

   int index_x = -1, index_y = -1, index_z = -1; /* used in numerical recipes hunt function */
   int index_x1= -1, index_y1= -1, index_z1= -1;

   float dx_dummy, dy_dummy, dz_dummy; /* dummy variables mem locs to set if no real mem locs are input as optional args */

   /* create variables to pass on to interpolate_in_block regardless if no optional arguments exist ( so interpolate_in_block can have a fixed arg list */
   float missing;
   float *dx_return_value, *dy_return_value, *dz_return_value;

   extern int is_var_in_memory(
         char *);

   /** lets see if requested variable is in memory **/

   main_memory_flag = is_var_in_memory(variable);

   /************ END OF DECLARATION SECTION ***********************************/

   /* Lutz: convert position input from interface */
   X = H;
   Y = 90.+LAT;
   Z = LON;
   /* import grid dimension sizes */
   NX_blk = nz;
   NY_blk = nx;
   NZ_blk = ny;
   NV_blk = NX_blk*NY_blk;
   /* NV_blk = nx * nz; */

   va_list argptr; /* create a pointer to variable list of arguments */

   counts[0] = 0; /*reset values after once through */
   intervals[0] = 1;

   va_start( argptr, optional_argument_flag );
   /* make argptr point to first UNAMED arguments which should be the missing variable */

   /*#ifdef DEBUG
    printf("DEBUG:\tchecking for optional arguments using optional_argument_flag: %d\n", optional_argument_flag );
    #endif */

   if (optional_argument_flag)
   {

      /*printf("DEBUG:\toptional arguments are present\n");*/

      /* there are optional arguments 1 - 4, therofore set missing, dx, dy, dz */
      /*missing = va_arg( argptr, float );*/

      /*hack = va_arg( argptr, double );*/
      missing = va_arg( argptr, double );
      /* Lutz: this order reflects input: Lat., Long., Height/IP */
      dz_return_value = va_arg( argptr, float * );
      dy_return_value = va_arg( argptr, float * );
      dx_return_value = va_arg( argptr, float * );

      /*
       #ifdef DEBUG
       printf("DEBUG:\toptional arguments are present setting missing value to -->%f<-- and also copying pointers...\n", missing);
       #endif */
   }
   else
   {
      /*printf("DEBUG:\tabout to set missing variable\n");*/
      missing = MISSING;
      dx_return_value = &dx_dummy;
      dy_return_value = &dy_dummy;
      dz_return_value = &dz_dummy;
      /*printf("DEBUG:\tNO optional arguments present, setting missing to -->%f<--\n", missing );*/

   }

   /* for field line tracing,etc..., select appropriate variable number ie. bx_cdfNum|by_cdfNum|bz_cdfNum based on *variable_string */

   /* set default grid arrays and change if neccessary, right operand of assignment stament set during open_cdf() call */

   /* Lutz: this order reflects input: Lat., Long., Height/IP and coordinate definitions in CDF file */
   y_position_array = x_pos;
   z_position_array = y_pos;
   H_position_array = H_pos;

   if ( !strcmp(
         variable,
         Vn_x_name) )
   {
      cdf_varNum = Vn_x_cdf_varNum;
   }
   else if ( !strcmp(
         variable,
         Vn_y_name) )
   {
      cdf_varNum = Vn_y_cdf_varNum;
   }
   else if ( !strcmp(
         variable,
         Vn_z_name) )
   {
      cdf_varNum = Vn_z_cdf_varNum;
   }
   else if ( !strcmp(
         variable,
         Vi_x_name) )
   {
      cdf_varNum = Vi_x_cdf_varNum;
   }
   else if ( !strcmp(
         variable,
         Vi_y_name) )
   {
      cdf_varNum = Vi_y_cdf_varNum;
   }
   else if ( !strcmp(
         variable,
         N_e_name) )
   {
      cdf_varNum = N_e_cdf_varNum;
   }
   else if ( !strcmp(
         variable,
         N_O_name) )
   {
      cdf_varNum = N_O_cdf_varNum;
   }
   else if ( !strcmp(
         variable,
         N_O2_name) )
   {
      cdf_varNum = N_O2_cdf_varNum;
   }
   else if ( !strcmp(
         variable,
         N_N2_name) )
   {
      cdf_varNum = N_N2_cdf_varNum;
   }
   else if ( !strcmp(
         variable,
         Rmt_name) )
   {
      cdf_varNum = Rmt_cdf_varNum;
   }
   else if ( !strcmp(
         variable,
         Tn_name) )
   {
      cdf_varNum = Tn_cdf_varNum;
   }
   else if ( !strcmp(
         variable,
         sigmaP_name) )
   {
      cdf_varNum = sigmaP_cdf_varNum;
   }
   else if ( !strcmp(
         variable,
         sigmaH_name) )
   {
      cdf_varNum = sigmaH_cdf_varNum;
   }
   else if ( !strcmp(
         variable,
         H_name) )
   {
      cdf_varNum = H_cdf_varNum;
   }
   else
   {
      printf(
            "ERROR:\tcould not find cdf variable number for %s\n",
            variable);
      printf(
            "Valid Variable Names for CTIP:\n%s\n%s\n%s\n%s\n%s\n%s\n%s\n%s\n%s\n%s\n%s\n%s\n%s\n%s\n----------------------------------------------\n",
            Vn_x_name,
            Vn_y_name,
            Vn_z_name,
            Vi_x_name,
            Vi_y_name,
            N_e_name,
            N_O_name,
            N_O2_name,
            N_N2_name,
            Rmt_name,
            Tn_name,
            sigmaP_name,
            sigmaH_name,
            H_name);
      return 0;
   }

#ifdef DEBUG
   printf("DEBUG:\tx[0] = %f, x[%d] = %f\n", y_position_array[0], NY_blk-1, y_position_array[NY_blk-1] );
   printf("DEBUG:\tz[0] = %f, z[%d] = %f\n", z_position_array[0], NZ_blk-1, z_position_array[NZ_blk-1] );
   printf("DEBUG:\th[0] = %f, h[%d] = %f\n", H_position_array[0], NX_blk-1, H_position_array[NX_blk-1] );

   printf("user input Y = %f\n", Y );
   printf("user input Z = %f\n", Z );
   printf("user input H = %f\n", H );
#endif

   /******* if Z >= actual Z max we need to wrap around and interpolate between Z[NZ_blk-1] & Z[0]  *********/
   /*** for now lets just intercept and throw an error if Z > Z actual min ******/
   /*if( Z > 342 )
    {
    printf("ERROR: Functionality needs to be added for longitude values > 342\n");
    exit(0);
    }
    */

   /*printf("\nDEBUG:\tabout to call external hunt routine using indices -----> %d, %d, %d \n", index_x, index_y, index_z);*/

   /*** Find the array index numbers for Y, Z, & H ***/

   hunt(
         y_position_array,
         NY_blk,
         Y,
         &index_y);
   hunt(
         z_position_array,
         NZ_blk,
         Z,
         &index_z);

#ifdef DEBUG
   printf("DEBUG\tback from hunt routine where x[%d] = %f was found to be to the"
         " left of %f \n", index_y, y_position_array[index_y], Y);
   printf("DEBUG\tback from hunt routine where y[%d] = %f was found to be to the"
         " left of %f \n\n", index_z, z_position_array[index_z], Z);
#endif

   /* Lutz: limit indices and implement periodic boundary in y (longitude) */
   index_y = min( index_y , NY_blk - 2 );
   index_z = min( index_z , NZ_blk - 1 );
   index_y1 = index_y + 1;
   index_z1 = (index_z + 1) % NZ_blk;

   /** calculate dy & dz **/

   /** 1/9/06 changed if from NY_blk/NZ_blk - 1 to NY_blk/NZ_blk - 2 **/

   if (index_y <= (NY_blk - 2))
   {
      dy_blk = y_position_array[ index_y1 ] - y_position_array[ index_y ];
   }
   else
   {
      dy_blk = y_position_array[ index_y ] - y_position_array[ index_y - 1 ];
   }

   if (index_z <= (NZ_blk - 2))
   {
      dz_blk = z_position_array[ index_z1 ] - z_position_array[ index_z ];
   }
   else
   {
      dz_blk = z_position_array[ index_z ] - z_position_array[ index_z - 1 ];
   }

#ifdef DEBUG
   printf("DEBUG\tdy = %f\n", dy_blk );
   printf("DEBUG\tdz = %f\n", dz_blk );
#endif

   /**** Integrate Lutz's Implementation ****/

#ifdef DEBUG
   printf("DEBUG\tdy_blk = %f\n", dy_blk );
   printf("DEBUG\tdz_blk = %f\n", dz_blk );
#endif

   if (NY_blk >= 2)
   {
      /*    index_y = min( NY_blk - 2, index_y ); */
      m_y1 = 1.0 * (Y - y_position_array[ index_y ] ) / dy_blk;
   }
   else
   {
      index_y = 0;
      m_y1 = 0;
   }

   m_y0 = 1.0 - m_y1;

   if (NZ_blk >= 2)
   {
      /*    index_z = min( NZ_blk - 2, index_z ); */
      m_z1 = 1.0 * (Z - z_position_array[ index_z ] ) / dz_blk;
   }
   else
   {
      index_z = 0;
      m_z1 = 0;
   }

   m_z0 = 1.0 - m_z1;

#ifdef DEBUG
   printf("DEBUG\tm_y1 = %f\n\tm_y0 = %f\n", m_y1, m_y0 );
   printf("DEBUG\tm_z1 = %f\n\tm_z0 = %f\n", m_z1, m_z0 );

   printf("DEBUG\tindex_y = %d\n", index_y );
   printf("DEBUG\tindex_z = %d\n", index_z );
   printf("DEBUG\tindex_y1 = %d\n", index_y1 );
   printf("DEBUG\tindex_z1 = %d\n", index_z1 );
#endif

   /****** we can't just hunt for index_x using the original H_position array.  We need to create a local interpolated height array  ***/
   height_column = (float*)malloc(NX_blk*sizeof(float));

   if (H <= NX_blk)
   {
      /* Lutz: implement interpolation in pressure levels */
      for (i =0; i < NX_blk; i++)
      {
         height_column[i]=i+1;
      }
   }
   else
   {
      for (i =0; i < NX_blk; i++)
      {

         /* NEED TO REFORM INTERPOLATION SINCE position is [z,x,y] as in  x,y,z -> z, x, y */

         height_column[i]
               = (( H_position_array[i + index_y * NX_blk + index_z * NV_blk ] ) *m_y0
                     * m_z0) + (( H_position_array[i + index_y1 * NX_blk
                     + index_z * NV_blk ] ) *m_y1 * m_z0)
                     + (( H_position_array[i + index_y * NX_blk + index_z1
                           * NV_blk ] ) *m_y0 * m_z1) + (( H_position_array[i
                     + index_y1 * NX_blk + index_z1 * NV_blk ] ) *m_y1 * m_z1);
#ifdef DEBUG
         printf("DEBUG\tINTERPOLATED height_column[%d] = %f\n", i, height_column[i] );
#endif
      }
   }

#ifdef DEBUG
   printf("\nDEBUG\tmanually ( linear ) searching for index of H_position_array "
         "whose value is left of input\n");
#endif

   /* Lutz: how could we tell whether H<height_column[0] ?
    i=0;
    while( i < NX_blk )
    {
    if( H >= height_column[i]  )
    { */
   /* if current value of interploated height array is <= input height, set current index */
   /*
    index_x = i;

    }

    i++;

    }
    use hunt() instead */
   index_x=-1;
   hunt(
         height_column,
         NX_blk,
         H,
         &index_x);

   /****** WE NOW HAVE INDICES FOR H, Y, Z ************************************/

#ifdef DEBUG
   printf("DEBUG\tIndices for H, Y, Z aquired\nH = %d\nY = %d\nZ = %d\n", index_x, index_y, index_z );
#endif

   /* Lutz: this does never happen - need different logical condition here
    if( ( index_y < 0 ) || ( index_y >= NY_blk ) ||
    ( index_z < 0 ) || ( index_z >= NZ_blk ) ||
    ( index_x < 0 ) || ( index_x >= NX_blk )  )
    */

   if ((Y < 0. ) || (Y > 180. ) || (Z < 0. ) || (Z > 360. ) || (X < 0)
         || (index_x < 0 ) || (index_x >= NX_blk-1 ))
   {
      return (missing );
   }
   else
   {

      /**** 11.21.2005 adding further index_x processing ****/

      index_x = min( NX_blk-2, index_x );
      index_x1 = index_x + 1;

      /*  change from using original data to recreated height column array */
      /*dx_blk = H_position_array[ index_x1 ] - H_position_array[ index_x ];*/

      dx_blk = height_column[ index_x1 ] - height_column[ index_x ];

      m_x1 = (H - height_column[index_x] ) / dx_blk;
      m_x0 = 1.0 - m_x1;

#ifdef DEBUG
      printf("\nDEBUG:\nindex_x = %d\nindex_x1 = %d\ndx_blk = %f\nm_x1 = %f\nm_x0 = %f\n", index_x, index_x1, dx_blk, m_x1, m_x0 );
      printf("DEBUG\tNew processed indices [ %d, %d, %d ]\n", index_y1, index_z1, index_x1 );
#endif

      m000 = m_x0*m_y0*m_z0;
      m001 = m_x1*m_y0*m_z0;
      m010 = m_x0*m_y1*m_z0;
      m011 = m_x1*m_y1*m_z0;
      m100 = m_x0*m_y0*m_z1;
      m101 = m_x1*m_y0*m_z1;
      m110 = m_x0*m_y1*m_z1;
      m111 = m_x1*m_y1*m_z1;

   }

#ifdef DEBUG
   printf("DEBUG:\tabout to call interpolate_in_block_ctip using:\nH = %f\nY = "
         "%f\nZ = %f\nindex_x = %d\nindex_y = %d\nindex_z = %d\nindex_x1 = %d\n"
         "index_y1 = %d\nindex_z1 = %d\nm000 = %f\nm001 = %f\nm010 = %f\nm011 = "
         "%f\nm100 = %f\nm101 = %f\nm110 = %f\nm111 = %f\n,...\n"
         "", H, Y, Z, index_x, index_y, index_z, index_x1, index_y1, index_z1,
         m000, m001, m010, m011, m100, m101, m110, m111 );
#endif

   /* call with rearranged arguments to accomidate H, Y, Z coordinates vs normal Y, Z, H */

   interpolated_value = interpolate_in_block_ctip(
         H,
         Y,
         Z,
         index_x,
         index_y,
         index_z,
         index_x1,
         index_y1,
         index_z1,
         height_column,
         y_position_array,
         z_position_array,
         variable,
         dy_return_value,
         dz_return_value,
         dx_return_value,
         cdf_varNum,
         main_memory_flag,
         m000,
         m001,
         m010,
         m011,
         m100,
         m101,
         m110,
         m111);

   free(height_column);

   return (interpolated_value );

}

/**************** interpolate_in_block *****************************************/

float interpolate_in_block_ctip(
      float x,
      float y,
      float z,
      int ix,
      int iy,
      int iz,
      int ix1,
      int iy1,
      int iz1,
      float *x_pos,
      float *y_pos,
      float *z_pos,
      char* variable_name,
      float *dx,
      float *dy,
      float *dz,
      int cdf_varNum,
      int main_memory_flag,
      float m000,
      float m001,
      float m010,
      float m011,
      float m100,
      float m101,
      float m110,
      float m111)
{

   CDFstatus status;

   long recordStart = 0;
   long indices[1];

   float mmm;

   float value, value2;
   float dx_blk, dy_blk, dz_blk, m_x, m_y, m_z;
   float data_1, data_2, data_3, data_4, data_5, data_6, data_7, data_8; /** values from actual data used for interploation equation **/

   /*int NV_blk = nx * ny;*//* it's not x,y,z were NV_blk is normally nx*ny--------x,y,z->z,x,y !!!! z,x,y so NV_blk = NX_blk*nx */
   int NX_blk, NY_blk, NZ_blk, NV_blk;

   NX_blk=nz;
   NY_blk=nx;
   NZ_blk=ny;
   NV_blk=NX_blk*NY_blk;

#ifdef DEBUG
   printf("DEBUG\tINSIDE interpolate_in_block_ctip\n");
#endif

   /*
    dx_blk = x_pos[ ix + 1 ] - x_pos[ ix ];
    dy_blk = y_pos[ iy + 1 ] - y_pos[ iy ];
    dz_blk = z_pos[ iz + 1 ] - z_pos[ iz ];
    */
   dx_blk = x_pos[ ix1 ] - x_pos[ ix ];
   dy_blk = y_pos[ iy1 ] - y_pos[ iy ];
   dz_blk = z_pos[ iz1 ] - z_pos[ iz ];

#ifdef DEBUG
   printf("DEBUG: ctip_interpolate_in_block: ix: %d ix1: %d iy: %d iy1: %d iz: %d iz1: %d \n",ix,ix1,iy,iy1,iz,iz1);
#endif

   /* don't know which one is periodic - I (Lutz) think it is "z"... */
   if (dx_blk <= 0)
   {
      dx_blk+=360.;
   }
   if (dy_blk <= 0)
   {
      dy_blk+=360.;
   }
   if (dz_blk <= 0)
   {
      dz_blk+=360.;
   }

#ifdef DEBUG
   printf("DEBUG\tdx_blk = %f, dy_blk = %f, dz_blk = %f\n", dx_blk,dy_blk,dz_blk );
#endif

   *dx = dx_blk; /* return values to caller */
   *dy = dy_blk;
   *dz = dz_blk;

#ifdef DEBUG
   printf("DEBUG\tdx = %f\n\tdy = %f\n\tdz = %f\nand ix = %d\niy = %d\niz = %d\n", dx_blk, dy_blk, dz_blk, ix, iy, iz );
#endif

   m_x = (x - x_pos[ ix ] ) / dx_blk;
   m_y = (y - y_pos[ iy ] ) / dy_blk;
   m_z = (z - z_pos[ iz ] ) / dz_blk;

#ifdef DEBUG
   printf("DEBUG\tmx = %f\n\tmy = %f\n\tmz = %f\n", m_x, m_y, m_z );
   printf("debug:\tgetting data values from cdf\n");
#endif

   /***** changing nx to nz ********/
   if ( !main_memory_flag)
   {

      indices[0] = (ix + iy*NX_blk + iz*NV_blk );

#ifdef DEBUG
      printf("indices[0] = ( %d + %d*%d + %d*%d ) = %ld\n", ix, iy,NX_blk,iz,NV_blk, indices[0] );
#endif

      status = CDFlib(
      SELECT_, zVAR_, cdf_varNum,
      SELECT_, zVAR_RECNUMBER_, recordStart,
      SELECT_, zVAR_DIMINDICES_, indices,
      GET_, zVAR_DATA_, &data_1,
      NULL_);
      if (status != CDF_OK)
         StatusHandler(status);

      indices[0] = (ix1 + iy*NX_blk + iz*NV_blk );

#ifdef DEBUG
      printf("indices[0] = ( %d + %d*%d + %d*%d ) = %ld\n", ix1, iy,NX_blk,iz,NV_blk, indices[0] );
#endif

      status = CDFlib(
      SELECT_, zVAR_, cdf_varNum,
      SELECT_, zVAR_RECNUMBER_, recordStart,
      SELECT_, zVAR_DIMINDICES_, indices,
      GET_, zVAR_DATA_, &data_2,
      NULL_);
      if (status != CDF_OK)
         StatusHandler(status);

      indices[0] = (ix + iy1*NX_blk + iz*NV_blk );

#ifdef DEBUG
      printf("indices[0] = ( %d + %d*%d + %d*%d ) = %ld\n", ix, iy1,NX_blk,iz,NV_blk, indices[0] );
#endif
      status = CDFlib(
      SELECT_, zVAR_, cdf_varNum,
      SELECT_, zVAR_RECNUMBER_, recordStart,
      SELECT_, zVAR_DIMINDICES_, indices,
      GET_, zVAR_DATA_, &data_3,
      NULL_);
      if (status != CDF_OK)
         StatusHandler(status);

      indices[0] = (ix1 + iy1*NX_blk + iz*NV_blk );

#ifdef DEBUG
      printf("indices[0] = ( %d + %d*%d + %d*%d ) = %ld\n", ix1, iy1,NX_blk,iz,NV_blk, indices[0] );
#endif
      status = CDFlib(
      SELECT_, zVAR_, cdf_varNum,
      SELECT_, zVAR_RECNUMBER_, recordStart,
      SELECT_, zVAR_DIMINDICES_, indices,
      GET_, zVAR_DATA_, &data_4,
      NULL_);
      if (status != CDF_OK)
         StatusHandler(status);

      indices[0] = (ix + iy*NX_blk + iz1*NV_blk );

#ifdef DEBUG
      printf("indices[0] = ( %d + %d*%d + %d*%d ) = %ld\n", ix, iy,NX_blk,iz1,NV_blk, indices[0] );
#endif
      status = CDFlib(
      SELECT_, zVAR_, cdf_varNum,
      SELECT_, zVAR_RECNUMBER_, recordStart,
      SELECT_, zVAR_DIMINDICES_, indices,
      GET_, zVAR_DATA_, &data_5,
      NULL_);
      if (status != CDF_OK)
         StatusHandler(status);

      indices[0] = (ix1 + iy*NX_blk + iz1*NV_blk );

#ifdef DEBUG
      printf("indices[0] = ( %d + %d*%d + %d*%d ) = %ld\n", ix1, iy,NX_blk,iz1,NV_blk, indices[0] );
#endif
      status = CDFlib(
      SELECT_, zVAR_, cdf_varNum,
      SELECT_, zVAR_RECNUMBER_, recordStart,
      SELECT_, zVAR_DIMINDICES_, indices,
      GET_, zVAR_DATA_, &data_6,
      NULL_);
      if (status != CDF_OK)
         StatusHandler(status);

      indices[0] = (ix + iy1*NX_blk + iz1*NV_blk );

#ifdef DEBUG
      printf("indices[0] = ( %d + %d*%d + %d*%d ) = %ld\n", ix, iy1,NX_blk,iz1,NV_blk, indices[0] );
#endif
      status = CDFlib(
      SELECT_, zVAR_, cdf_varNum,
      SELECT_, zVAR_RECNUMBER_, recordStart,
      SELECT_, zVAR_DIMINDICES_, indices,
      GET_, zVAR_DATA_, &data_7,
      NULL_);
      if (status != CDF_OK)
         StatusHandler(status);

      indices[0] = (ix1 + iy1*NX_blk + iz1*NV_blk );

#ifdef DEBUG
      printf("indices[0] = ( %d + %d*%d + %d*%d ) = %ld\n", ix1, iy1,NX_blk,iz1,NV_blk, indices[0] );
#endif
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
       iz = iz - ( found_block_index * NX_blk );
       */

      /** changing nx to NX_blk **/
      if ( !strcmp(
            variable_name,
            Rmt_name) )
      {
         data_1 = Rmt[ ( ix + iy*NX_blk + iz*NV_blk ) ];
         data_2 = Rmt[ ( ix1 + iy*NX_blk + iz*NV_blk ) ];
         data_3 = Rmt[ ( ix + ( iy1 )*NX_blk + iz*NV_blk ) ];
         data_4 = Rmt[ ( ix1 + ( iy1 )*NX_blk + iz*NV_blk ) ];
         data_5 = Rmt[ ( ix + iy*NX_blk + ( iz1 )*NV_blk ) ];
         data_6 = Rmt[ ( ix1 + iy*NX_blk + ( iz1 )*NV_blk ) ];
         data_7 = Rmt[ ( ix + ( iy1 )*NX_blk + ( iz1 )*NV_blk ) ];
         data_8 = Rmt[ ( ix1 + ( iy1 )*NX_blk + ( iz1 )*NV_blk ) ];
      }
      else if ( !strcmp(
            variable_name,
            Tn_name) )
      {
         data_1 = Tn[ ( ix + iy*NX_blk + iz*NV_blk ) ];
         data_2 = Tn[ ( ix1 + iy*NX_blk + iz*NV_blk ) ];
         data_3 = Tn[ ( ix + ( iy1 )*NX_blk + iz*NV_blk ) ];
         data_4 = Tn[ ( ix1 + ( iy1 )*NX_blk + iz*NV_blk ) ];
         data_5 = Tn[ ( ix + iy*NX_blk + ( iz1 )*NV_blk ) ];
         data_6 = Tn[ ( ix1 + iy*NX_blk + ( iz1 )*NV_blk ) ];
         data_7 = Tn[ ( ix + ( iy1 )*NX_blk + ( iz1 )*NV_blk ) ];
         data_8 = Tn[ ( ix1 + ( iy1 )*NX_blk + ( iz1 )*NV_blk ) ];
      }
      else if ( !strcmp(
            variable_name,
            sigmaP_name) )
      {
         data_1 = sigmaP[ ( ix + iy*NX_blk + iz*NV_blk ) ];
         data_2 = sigmaP[ ( ix1 + iy*NX_blk + iz*NV_blk ) ];
         data_3 = sigmaP[ ( ix + ( iy1 )*NX_blk + iz*NV_blk ) ];
         data_4 = sigmaP[ ( ix1 + ( iy1 )*NX_blk + iz*NV_blk ) ];
         data_5 = sigmaP[ ( ix + iy*NX_blk + ( iz1 )*NV_blk ) ];
         data_6 = sigmaP[ ( ix1 + iy*NX_blk + ( iz1 )*NV_blk ) ];
         data_7 = sigmaP[ ( ix + ( iy1 )*NX_blk + ( iz1 )*NV_blk ) ];
         data_8 = sigmaP[ ( ix1 + ( iy1 )*NX_blk + ( iz1 )*NV_blk ) ];
      }
      else if ( !strcmp(
            variable_name,
            sigmaH_name) )
      {
         data_1 = sigmaH[ ( ix + iy*NX_blk + iz*NV_blk ) ];
         data_2 = sigmaH[ ( ix1 + iy*NX_blk + iz*NV_blk ) ];
         data_3 = sigmaH[ ( ix + ( iy1 )*NX_blk + iz*NV_blk ) ];
         data_4 = sigmaH[ ( ix1 + ( iy1 )*NX_blk + iz*NV_blk ) ];
         data_5 = sigmaH[ ( ix + iy*NX_blk + ( iz1 )*NV_blk ) ];
         data_6 = sigmaH[ ( ix1 + iy*NX_blk + ( iz1 )*NV_blk ) ];
         data_7 = sigmaH[ ( ix + ( iy1 )*NX_blk + ( iz1 )*NV_blk ) ];
         data_8 = sigmaH[ ( ix1 + ( iy1 )*NX_blk + ( iz1 )*NV_blk ) ];
      }
      else if ( !strcmp(
            variable_name,
            N_e_name) )
      {
         data_1 = N_e[ ( ix + iy*NX_blk + iz*NV_blk ) ];
         data_2 = N_e[ ( ix1 + iy*NX_blk + iz*NV_blk ) ];
         data_3 = N_e[ ( ix + ( iy1 )*NX_blk + iz*NV_blk ) ];
         data_4 = N_e[ ( ix1 + ( iy1 )*NX_blk + iz*NV_blk ) ];
         data_5 = N_e[ ( ix + iy*NX_blk + ( iz1 )*NV_blk ) ];
         data_6 = N_e[ ( ix1 + iy*NX_blk + ( iz1 )*NV_blk ) ];
         data_7 = N_e[ ( ix + ( iy1 )*NX_blk + ( iz1 )*NV_blk ) ];
         data_8 = N_e[ ( ix1 + ( iy1 )*NX_blk + ( iz1 )*NV_blk ) ];
      }
      else if ( !strcmp(
            variable_name,
            N_O_name) )
      {
         data_1 = N_O[ ( ix + iy*NX_blk + iz*NV_blk ) ];
         data_2 = N_O[ ( ix1 + iy*NX_blk + iz*NV_blk ) ];
         data_3 = N_O[ ( ix + ( iy1 )*NX_blk + iz*NV_blk ) ];
         data_4 = N_O[ ( ix1 + ( iy1 )*NX_blk + iz*NV_blk ) ];
         data_5 = N_O[ ( ix + iy*NX_blk + ( iz1 )*NV_blk ) ];
         data_6 = N_O[ ( ix1 + iy*NX_blk + ( iz1 )*NV_blk ) ];
         data_7 = N_O[ ( ix + ( iy1 )*NX_blk + ( iz1 )*NV_blk ) ];
         data_8 = N_O[ ( ix1 + ( iy1 )*NX_blk + ( iz1 )*NV_blk ) ];
      }
      else if ( !strcmp(
            variable_name,
            N_O2_name) )
      {
         data_1 = N_O2[ ( ix + iy*NX_blk + iz*NV_blk ) ];
         data_2 = N_O2[ ( ix1 + iy*NX_blk + iz*NV_blk ) ];
         data_3 = N_O2[ ( ix + ( iy1 )*NX_blk + iz*NV_blk ) ];
         data_4 = N_O2[ ( ix1 + ( iy1 )*NX_blk + iz*NV_blk ) ];
         data_5 = N_O2[ ( ix + iy*NX_blk + ( iz1 )*NV_blk ) ];
         data_6 = N_O2[ ( ix1 + iy*NX_blk + ( iz1 )*NV_blk ) ];
         data_7 = N_O2[ ( ix + ( iy1 )*NX_blk + ( iz1 )*NV_blk ) ];
         data_8 = N_O2[ ( ix1 + ( iy1 )*NX_blk + ( iz1 )*NV_blk ) ];
      }
      else if ( !strcmp(
            variable_name,
            N_N2_name) )
      {
         data_1 = N_N2[ ( ix + iy*NX_blk + iz*NV_blk ) ];
         data_2 = N_N2[ ( ix1 + iy*NX_blk + iz*NV_blk ) ];
         data_3 = N_N2[ ( ix + ( iy1 )*NX_blk + iz*NV_blk ) ];
         data_4 = N_N2[ ( ix1 + ( iy1 )*NX_blk + iz*NV_blk ) ];
         data_5 = N_N2[ ( ix + iy*NX_blk + ( iz1 )*NV_blk ) ];
         data_6 = N_N2[ ( ix1 + iy*NX_blk + ( iz1 )*NV_blk ) ];
         data_7 = N_N2[ ( ix + ( iy1 )*NX_blk + ( iz1 )*NV_blk ) ];
         data_8 = N_N2[ ( ix1 + ( iy1 )*NX_blk + ( iz1 )*NV_blk ) ];
      }
      else if ( !strcmp(
            variable_name,
            Vn_x_name) )
      {
         data_1 = Vn_x[ ( ix + iy*NX_blk + iz*NV_blk ) ];
         data_2 = Vn_x[ ( ix1 + iy*NX_blk + iz*NV_blk ) ];
         data_3 = Vn_x[ ( ix + ( iy1 )*NX_blk + iz*NV_blk ) ];
         data_4 = Vn_x[ ( ix1 + ( iy1 )*NX_blk + iz*NV_blk ) ];
         data_5 = Vn_x[ ( ix + iy*NX_blk + ( iz1 )*NV_blk ) ];
         data_6 = Vn_x[ ( ix1 + iy*NX_blk + ( iz1 )*NV_blk ) ];
         data_7 = Vn_x[ ( ix + ( iy1 )*NX_blk + ( iz1 )*NV_blk ) ];
         data_8 = Vn_x[ ( ix1 + ( iy1 )*NX_blk + ( iz1 )*NV_blk ) ];
      }
      else if ( !strcmp(
            variable_name,
            Vn_y_name) )
      {
         data_1 = Vn_y[ ( ix + iy*NX_blk + iz*NV_blk ) ];
         data_2 = Vn_y[ ( ix1 + iy*NX_blk + iz*NV_blk ) ];
         data_3 = Vn_y[ ( ix + ( iy1 )*NX_blk + iz*NV_blk ) ];
         data_4 = Vn_y[ ( ix1 + ( iy1 )*NX_blk + iz*NV_blk ) ];
         data_5 = Vn_y[ ( ix + iy*NX_blk + ( iz1 )*NV_blk ) ];
         data_6 = Vn_y[ ( ix1 + iy*NX_blk + ( iz1 )*NV_blk ) ];
         data_7 = Vn_y[ ( ix + ( iy1 )*NX_blk + ( iz1 )*NV_blk ) ];
         data_8 = Vn_y[ ( ix1 + ( iy1 )*NX_blk + ( iz1 )*NV_blk ) ];
      }
      else if ( !strcmp(
            variable_name,
            Vn_z_name) )
      {
         data_1 = Vn_z[ ( ix + iy*NX_blk + iz*NV_blk ) ];
         data_2 = Vn_z[ ( ix1 + iy*NX_blk + iz*NV_blk ) ];
         data_3 = Vn_z[ ( ix + ( iy1 )*NX_blk + iz*NV_blk ) ];
         data_4 = Vn_z[ ( ix1 + ( iy1 )*NX_blk + iz*NV_blk ) ];
         data_5 = Vn_z[ ( ix + iy*NX_blk + ( iz1 )*NV_blk ) ];
         data_6 = Vn_z[ ( ix1 + iy*NX_blk + ( iz1 )*NV_blk ) ];
         data_7 = Vn_z[ ( ix + ( iy1 )*NX_blk + ( iz1 )*NV_blk ) ];
         data_8 = Vn_z[ ( ix1 + ( iy1 )*NX_blk + ( iz1 )*NV_blk ) ];
      }
      else if ( !strcmp(
            variable_name,
            Vi_x_name) )
      {
         data_1 = Vi_x[ ( ix + iy*NX_blk + iz*NV_blk ) ];
         data_2 = Vi_x[ ( ix1 + iy*NX_blk + iz*NV_blk ) ];
         data_3 = Vi_x[ ( ix + ( iy1 )*NX_blk + iz*NV_blk ) ];
         data_4 = Vi_x[ ( ix1 + ( iy1 )*NX_blk + iz*NV_blk ) ];
         data_5 = Vi_x[ ( ix + iy*NX_blk + ( iz1 )*NV_blk ) ];
         data_6 = Vi_x[ ( ix1 + iy*NX_blk + ( iz1 )*NV_blk ) ];
         data_7 = Vi_x[ ( ix + ( iy1 )*NX_blk + ( iz1 )*NV_blk ) ];
         data_8 = Vi_x[ ( ix1 + ( iy1 )*NX_blk + ( iz1 )*NV_blk ) ];
      }
      else if ( !strcmp(
            variable_name,
            Vi_y_name) )
      {
         data_1 = Vi_y[ ( ix + iy*NX_blk + iz*NV_blk ) ];
         data_2 = Vi_y[ ( ix1 + iy*NX_blk + iz*NV_blk ) ];
         data_3 = Vi_y[ ( ix + ( iy1 )*NX_blk + iz*NV_blk ) ];
         data_4 = Vi_y[ ( ix1 + ( iy1 )*NX_blk + iz*NV_blk ) ];
         data_5 = Vi_y[ ( ix + iy*NX_blk + ( iz1 )*NV_blk ) ];
         data_6 = Vi_y[ ( ix1 + iy*NX_blk + ( iz1 )*NV_blk ) ];
         data_7 = Vi_y[ ( ix + ( iy1 )*NX_blk + ( iz1 )*NV_blk ) ];
         data_8 = Vi_y[ ( ix1 + ( iy1 )*NX_blk + ( iz1 )*NV_blk ) ];
      }
      else if ( !strcmp(
            variable_name,
            H_name) )
      {
         data_1 = H_pos[ ( ix + iy*NX_blk + iz*NV_blk ) ];
         data_2 = H_pos[ ( ix1 + iy*NX_blk + iz*NV_blk ) ];
         data_3 = H_pos[ ( ix + ( iy1 )*NX_blk + iz*NV_blk ) ];
         data_4 = H_pos[ ( ix1 + ( iy1 )*NX_blk + iz*NV_blk ) ];
         data_5 = H_pos[ ( ix + iy*NX_blk + ( iz1 )*NV_blk ) ];
         data_6 = H_pos[ ( ix1 + iy*NX_blk + ( iz1 )*NV_blk ) ];
         data_7 = H_pos[ ( ix + ( iy1 )*NX_blk + ( iz1 )*NV_blk ) ];
         data_8 = H_pos[ ( ix1 + ( iy1 )*NX_blk + ( iz1 )*NV_blk ) ];
      }
      else
      {
         /* INSERT ERROR HANDLER HERE */
         printf("ERROR\tcouldn't find data variable...\n");
      }

   }

#ifdef DEBUG
   printf("debug:\tfinished getting data values from cdf\n");
   printf("DEBUG\tdata 1..8:\n1 = %f\n2 = %f\n3 = %f\n4 = %f\n5 = %f\n6 = %f\n7 = %f\n8 = %f\n", data_1, data_2, data_3, data_4, data_5, data_6, data_7, data_8 );
#endif

   value = (1-m_z) * ( (1-m_y) * ((1-m_x) * data_1 + m_x * data_2 ) + m_y
         * ( + (1-m_x)*data_3 + m_x *data_4) )

   + m_z * ( (1-m_y)*( + (1-m_x) * data_5 + m_x *data_6 ) + m_y*( + ( 1-m_x )
         * data_7 + m_x *data_8 ) );

   /*
    #ifdef DEBUG
    printf("DEBUG\tvalue before returning = %f\n", value );
    #endif
    */

   mmm = m000 + m001 + m010 + m011 + m100 + m101 + m110 + m111;

   value2 = (m000*data_1 + m001*data_2 + m010*data_3 + m011*data_4 + m100
         *data_5 + m101*data_6 + m110*data_7 + m111*data_8 )/mmm;

#ifdef DEBUG
   printf("\nDEBUG\tbefore returning from interpolate_in_block_ctip\n\nvalue\t= %f\nvalue2\t= %f\n\n", value, value2 );
#endif

   return (value );

}

