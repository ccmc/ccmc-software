/*******************************************************************************
 *                                                                             *
 *      NAME:           read_enlil.c                                           *
 *                                                                             *
 *      VERSION:        0.1                                                    *
 *                                                                             *
 *      AUTHOR:         Marlo M. H. Maddox                                     *
 *                      NASA-GSFC-AETD-ISD-ADMA(587)/CCMC(612.3)               *
 *                      Marlo.Maddox@nasa.gov                                  *
 *                      (301) 286 - 5202                                       *
 *                                                                             *
 *      PURPOSE:        Custom read routine for ENLIL model output.            *
 *                                                                             *
 *      INPUTS:         char *input_file_name, char *aux_file,                 *
 *                      int verbose_flag, int minmax_flag                      *
 *                                                                             *
 *      OUTPUTS:        enlil variable values stored in main memory            *
 *                                                                             *
 *      FUNCTIONS:      a few                                                  *
 *      VARIABLES:      a few more                                             *
 *                                                                             *
 *      MODIFICATION                                                           *
 *      HISTORY:                                                               *
 *                                                                             *
 *      2006 February 2nd   Maddox, Marlo                                      *
 *                          Initial Development Started                        *
 *                                                                             *
 *      2006 October        a bunch of bug fixes...                            *
 *                                                                             *
 *                                                                             *
 *                          made the get_enlil_global_netcdf_attributes        *
 *                          function accept double pointers - pointers to the  *
 *                          pointers declared in the read_enlil.h header file  *
 *                                                                             *
 *                          added the DEBUG_FLAG macro                         *
 *                                                                             *
 *      2006 October 23rd   Maddox, Marlo                                      *
 *                          when reading global attribute values using         *
 *                          get_enlil_global_netcdf_attribute(), when don't    *
 *                          need to return EXIT_FAILURE if a single attribute  *
 *                          is not found. Instead, let the rouitne finish      *
 *                          running...                                         *
 *                                                                             *
 *      2010 December 8th  Maddox, Marlo                                       *
 *                         The new ENLIL files do NOT contain n1h, n2hm and n3h*
 *                         coordinate information so we do NOT need to EXIT    *
 *                         FAILURE when the values are not encountered...      *
 ******************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <unistd.h>
#include "read_enlil.h"

#define DEBUG_FLAG 0

#ifdef NETCDF
#include "netcdf.h"
#endif

void netCDF_StatusHandler(int);

int get_enlil_global_netcdf_attributes(
      int,
      char**,
      char **,
      char **,
      char **,
      char **,
      char **,
      char **,
      char **,
      char **,
      char **,
      char **,
      char **,
      double **,
      double **,
      double **,
      char **);
int get_enlil_variable_netcdf_attributes(int, int, char *, char *);

/*******************************************************************************
 *                     READ ENLIL MAIN                                         *
 ******************************************************************************/

int read_enlil(
      char *input_file_name,
      char *aux_file,
      int verbose_flag,
      int minmax_flag)
{

   extern int linear_minmax_search(float *, int, float *, float *);

   int read_enlil_time_file(char *, int);
   int read_enlil_evh_file(char *, int);
   int read_enlil_evl_file(char *, int);
   int read_enlil_evg_file(char *, int);
   int read_enlil_evp_file(char *, int);

   char *evh_file, *evl_file, *evg_file, *evp_file;

   char *evh_file_name = "evh.nc";
   char *evl_file_name = "evl.nc";
   char *evg_file_name = "evg.nc";
   char *evp_file_name = "evp.nc";

   char *time_file_prefix = "tim.";

   size_t first_4_characters = 4;

   int evh_file_exists = 1, evl_file_exists = 1, evg_file_exists = 1,
         evp_file_exists = 1;

   int status_code, aux_file_length, ev_full_path_length;

   /* If the Compiler macro NETCDF is not defined, that means the makefile
    * determined that libnetcdf.a was not available.  Instead of shutting down
    * the entire converter
    * Lets just disable any read routines thatt depend on the netCDF library */

#ifndef NETCDF
   printf("\n\n!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!"
         "!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!\n");
   printf("!! WARNING: from %s line [%d]. Conversion Software was not compiled "
         "and linked with netCDF libraries ( libnetcdf.a ).  No .nc netCDF files"
         " will be ingested... \n", __FILE__, __LINE__ );
   return EXIT_FAILURE;
#else

   if ( DEBUG_FLAG)
      printf("\n\n\nDEBUG\tEntering read_enlil()\n");

   /* libnetcdf.a must be available, ingest the ENLIL output files */

   /******** read the time file first ***********/

   /* lets go ahead and rely on the time file naming convention and only
    * proceed if the current file name begins with tim. ***/

   if (strstr(input_file_name, time_file_prefix) == NULL)
   {
      printf("*WARNING: %s\t Current input file --->%s<--- does not match the "
         "standard prefix --->%s<---...ignoring\n",
      __FILE__, input_file_name, time_file_prefix );
      return EXIT_FAILURE;
   }

   status_code = read_enlil_time_file( input_file_name, verbose_flag );

      if( verbose_flag )
      {  printf("searching for ev*.nc files in directory %s\n", aux_file );}

      /**** build full path and file name for all ev*.nc files ****/

      /* this is just the length of the specified directory that should contain
       * the ev files */
      aux_file_length = strlen( aux_file );

      /** we'll assume that all ev file names will remain unchanged and thus be
       * 6 characters in lenght + 1 for NULL terminator **/

      ev_full_path_length = aux_file_length + 7;

      evh_file = ( char * ) malloc( ev_full_path_length * sizeof(char) );
      evl_file = ( char * ) malloc( ev_full_path_length * sizeof(char) );
      evg_file = ( char * ) malloc( ev_full_path_length * sizeof(char) );
      evp_file = ( char * ) malloc( ev_full_path_length * sizeof(char) );

      /** copy specified ev file directory into newly alocated strings **/

      strcpy( evh_file, aux_file );
      strcpy( evl_file, aux_file );
      strcpy( evg_file, aux_file );
      strcpy( evp_file, aux_file );

      /** now append strings with specific file names **/

      strcat( evh_file, evh_file_name );
      strcat( evl_file, evl_file_name );
      strcat( evg_file, evg_file_name );
      strcat( evp_file, evp_file_name );

      /* see if we can read the files, access returns -1 if we can't read file*/

      if( access( evh_file, R_OK ) )
      {  evh_file_exists = 0; printf(" %s found in %s\n", evh_file_name, aux_file );}
      if( access( evl_file, R_OK ) )
      {  evl_file_exists = 0; printf(" %s found in %s\n", evl_file_name, aux_file );}
      if( access( evg_file, R_OK ) )
      {  evg_file_exists = 0; printf(" %s found in %s\n", evg_file_name, aux_file );}
      if( access( evp_file, R_OK ) )
      {  evp_file_exists = 0; printf(" %s found in %s\n", evp_file_name, aux_file );}

      if( DEBUG_FLAG ) printf("DEBUG\tcalling read_enlil_evh_file\n");

      if( evh_file_exists )
      {  status_code = read_enlil_evh_file( evh_file, verbose_flag );}

      if( DEBUG_FLAG ) printf("DEBUG\tcalling read_enlil_evl_file\n");

      if( evl_file_exists )
      {  status_code = read_enlil_evl_file( evl_file, verbose_flag );}

      if( DEBUG_FLAG ) printf("DEBUG\tcalling read_enlil_evg_file\n");

      if( evg_file_exists )
      {  status_code = read_enlil_evg_file( evg_file, verbose_flag );}

      if( DEBUG_FLAG ) printf("DEBUG\tcalling read_enlil_evp_file\n");

      if( evp_file_exists )
      {  status_code = read_enlil_evp_file( evp_file, verbose_flag );}

#endif

      return EXIT_SUCCESS;

   }

   /****************************************************************************
    *                     READ ENLIL TIME FILE                                 *
    ***************************************************************************/

int read_enlil_time_file(char *input_file_name, int verbose_flag)
{

#ifdef NETCDF

   int i, grid_size;
   int status;

   /*** create var_exists variable to determine if a variable needs to be
    * allocated and read or not ***/

   int time_exists = 1, dt_exists = 1, nstep_exists = 1, d_exists = 1,
         t_exists = 1, v1_exists = 1, v2_exists = 1, v3_exists = 1, b1_exists =
               1, b2_exists = 1, b3_exists = 1, dp_exists = 1, bp_exists = 1;

   /******* variable for netCDF specific calls *********/

   int netCDFid;

   int file_open_mode= NC_NOWRITE;

   if (verbose_flag)
   {
      printf("...reading %s\n", input_file_name);
   }

   /******** open time.<rrrr>.nc netCDF file *****/

   status = nc_open(input_file_name, file_open_mode, &netCDFid);

   /*** if netCDF file was not opened properly, return contron to calling
    * program with EXIT_FAILUE **/

   if (status != NC_NOERR)
   {
      netCDF_StatusHandler(status);
      return EXIT_FAILURE;
   }

   /*call get_enlil_global_attributes() function */

   if ( DEBUG_FLAG)
   {
      printf(
            "DEBUG\t%s\tbefore get_enlil_global_getcdf_attributes() call\nglobal "
               "attr time_gattribute_type = ->%s<-\taddress = %p\n",
            __FILE__, time_gattribute_type, time_gattribute_type );

         }

         if( DEBUG_FLAG ) printf("DEBUG\t%s\tbefore "
               "get_enlil_global_getcdf_attributes() call\nglobal attr "
               "time_gattribute_refdate_mjd = -><-\taddress = %p\n"
               "", __FILE__, time_gattribute_refdate_mjd );

      status = get_enlil_global_netcdf_attributes( netCDFid,
      &time_gattribute_type, &time_gattribute_title, &time_gattribute_name,
      &time_gattribute_project, &time_gattribute_initial,
      &time_gattribute_resume, &time_gattribute_boundary,
      &time_gattribute_passage, &time_gattribute_grid,
      &time_gattribute_geometry, &time_gattribute_code,
      &time_gattribute_parameters, &time_gattribute_refdate_mjd,
      &time_gattribute_gamma, &time_gattribute_xalpha,
      &time_gattribute_history );

      if( DEBUG_FLAG ) printf("DEBUG\t%s\tafter "
      "get_enlil_global_getcdf_attributes() call\nglobal attr "
      "time_gattribute_type = ->%s<-address = %p\n", __FILE__,
      time_gattribute_type, time_gattribute_type );

      if( DEBUG_FLAG ) printf("DEBUG\t%s\tafter "
      "get_enlil_global_betcdf_attributes() call\nglobal attr "
      "time_gattribute_refdate_mjd = ->%g<-\taddress = %p\n",
      __FILE__, *time_gattribute_refdate_mjd,
      time_gattribute_refdate_mjd );

      if( DEBUG_FLAG ) printf("getting id's\n");

      /* get dimension id's and sizes from time.<rrr>.nc file */

      if( ( status = nc_inq_dimid( netCDFid, "n1", &tim_n1_netCDFid ) ) != NC_NOERR )
      {  netCDF_StatusHandler( status ); return EXIT_FAILURE;}
      if( ( status = nc_inq_dimid( netCDFid, "n2", &tim_n2_netCDFid ) ) != NC_NOERR )
      {  netCDF_StatusHandler( status ); return EXIT_FAILURE;}
      if( ( status = nc_inq_dimid( netCDFid, "n3", &tim_n3_netCDFid ) ) != NC_NOERR )
      {  netCDF_StatusHandler( status ); return EXIT_FAILURE;}
      if( ( status = nc_inq_dimid( netCDFid, "n1h", &tim_n1h_netCDFid ) ) != NC_NOERR )
      {  netCDF_StatusHandler( status ); /*return EXIT_FAILURE;*/  printf("WARNING: n1h dimension not present in this file.  Skipping...\n");}
      if( ( status = nc_inq_dimid( netCDFid, "n2h", &tim_n2h_netCDFid ) ) != NC_NOERR )
      {  netCDF_StatusHandler( status ); /*return EXIT_FAILURE;*/ printf("WARNING: n2h dimension not present in this file.  Skipping...\n");}
      if( ( status = nc_inq_dimid( netCDFid, "n3h", &tim_n3h_netCDFid ) )!= NC_NOERR )
      {  netCDF_StatusHandler( status ); /*return EXIT_FAILURE;*/ printf("WARNING: n3h dimension not present in this file.  Skipping...\n");}
      if( ( status = nc_inq_dimid( netCDFid, "nblk", &tim_number_of_blocks_nblk_netCDFid ) ) != NC_NOERR )
      {  netCDF_StatusHandler( status ); return EXIT_FAILURE;}

      if( ( status = nc_inq_dimlen( netCDFid, tim_n1_netCDFid, &tim_n1 ) ) != NC_NOERR )
      {  netCDF_StatusHandler( status ); return EXIT_FAILURE;}
      if( ( status = nc_inq_dimlen( netCDFid, tim_n2_netCDFid, &tim_n2 ) ) != NC_NOERR )
      {  netCDF_StatusHandler( status ); return EXIT_FAILURE;}
      if( ( status = nc_inq_dimlen( netCDFid, tim_n3_netCDFid, &tim_n3 ) ) != NC_NOERR )
      {  netCDF_StatusHandler( status ); return EXIT_FAILURE;}
      if( ( status = nc_inq_dimlen( netCDFid, tim_n1h_netCDFid, &tim_n1h ) ) != NC_NOERR )
      {  netCDF_StatusHandler( status ); /*return EXIT_FAILURE;*/ printf("WARNING: n1h dimension not present in this file.  Skipping...\n");}
      if( ( status = nc_inq_dimlen( netCDFid, tim_n2h_netCDFid, &tim_n2h ) ) != NC_NOERR )
      {  netCDF_StatusHandler( status ); /*return EXIT_FAILURE;*/ printf("WARNING: n2h dimension not present in this file.  Skipping...\n");}
      if( ( status = nc_inq_dimlen( netCDFid, tim_n3h_netCDFid, &tim_n3h ) ) != NC_NOERR )
      {  netCDF_StatusHandler( status ); /*return EXIT_FAILURE;*/ printf("WARNING: n3h dimension not present in this file.  Skipping...\n");}
      if( ( status = nc_inq_dimlen( netCDFid, tim_number_of_blocks_nblk_netCDFid, &tim_number_of_blocks_nblk ) ) != NC_NOERR )
      {  netCDF_StatusHandler( status ); return EXIT_FAILURE;}

      /*get all of the variable id numbers from the time.<rrrr>.nc file ******/

      if( ( status = nc_inq_varid( netCDFid, "X1", &tim_x1_position_X1_netCDFid ) ) != NC_NOERR )
      {  netCDF_StatusHandler( status ); return EXIT_FAILURE;}
      if( ( status = nc_inq_varid( netCDFid, "X2", &tim_x2_position_X2_netCDFid ) ) != NC_NOERR )
      {  netCDF_StatusHandler( status ); return EXIT_FAILURE;}
      if( ( status = nc_inq_varid( netCDFid, "X3", &tim_x3_position_X3_netCDFid ) ) != NC_NOERR )
      {  netCDF_StatusHandler( status ); return EXIT_FAILURE;}
      if( ( status = nc_inq_varid( netCDFid, "X1H", &tim_x1h_position_X1H_netCDFid ) ) != NC_NOERR )
      {  netCDF_StatusHandler( status ); /*return EXIT_FAILURE;*/  printf("WARNING: X1H dimension not present in this file.  Skipping...\n");}
      if( ( status = nc_inq_varid( netCDFid, "X2H", &tim_x2h_position_X2H_netCDFid ) ) != NC_NOERR )
      {  netCDF_StatusHandler( status ); /*return EXIT_FAILURE;*/  printf("WARNING: X2H dimension not present in this file.  Skipping...\n");}
      if( ( status = nc_inq_varid( netCDFid, "X3H", &tim_x3h_position_X3H_netCDFid ) ) != NC_NOERR )
      {  netCDF_StatusHandler( status ); /*return EXIT_FAILURE;*/  printf("WARNING: X3H dimension not present in this file.  Skipping...\n");}

      /*** we don't want to die if a regular variable is not found, so don't
       * return control doing this primarily because my development test data
       * does not contain dp cloud mass density variable but we should not
       * abandon program beacause of this - plus it's a good idea anyway - just
       * print a warning **/

      if( ( status = nc_inq_varid( netCDFid, "TIME", &tim_physical_time_TIME_netCDFid ) ) != NC_NOERR )
      {
         time_exists = 0; fprintf( stderr, "\n%s !!!!! WARNING: Problem aquiring "
         "netCDFid for variable TIME. NETCDF ERROR:", __FILE__ );
         netCDF_StatusHandler( status );
      }

      if( ( status = nc_inq_varid( netCDFid, "DT", &tim_physical_time_step_DT_netCDFid ) ) != NC_NOERR )
      {
         dt_exists = 0; fprintf( stderr, "\n%s !!!!! WARNING: Problem aquiring "
         "netCDFid for variable DT. NETCDF ERROR:", __FILE__ );
         netCDF_StatusHandler( status );
      }

      if( ( status = nc_inq_varid( netCDFid, "NSTEP", &tim_numerical_time_step_NSTEP_netCDFid ) ) != NC_NOERR )
      {
         nstep_exists = 0; fprintf( stderr, "\n%s !!!!! WARNING: Problem "
         "aquiring netCDFid for variable NSTEP. NETCDF ERROR:", __FILE__ );
         netCDF_StatusHandler( status );
      }

      if( ( status = nc_inq_varid( netCDFid, "D", &tim_density_D_netCDFid ) ) != NC_NOERR )
      {
         d_exists = 0; fprintf( stderr, "\n%s !!!!! WARNING: Problem aquiring "
         "netCDFid for variable D. NETCDF ERROR:", __FILE__ );
         netCDF_StatusHandler( status );
      }

      if( ( status = nc_inq_varid( netCDFid, "T", &tim_temperature_T_netCDFid ) ) != NC_NOERR )
      {
         t_exists = 0; fprintf( stderr,"\n%s !!!!! WARNING: Problem aquiring "
         "netCDFid for variable T. NETCDF ERROR:", __FILE__ );
         netCDF_StatusHandler( status );
      }

      if( ( status = nc_inq_varid( netCDFid, "V1", &tim_x1_velocity_V1_netCDFid ) ) != NC_NOERR )
      {
         v1_exists = 0; fprintf( stderr,"\n%s !!!!! WARNING: Problem aquiring "
         "netCDFid for variable V1. NETCDF ERROR:", __FILE__ );
         netCDF_StatusHandler( status );
      }

      if( ( status = nc_inq_varid( netCDFid, "V2", &tim_x2_velocity_V2_netCDFid ) ) != NC_NOERR )
      {
         v2_exists = 0; fprintf( stderr,"\n%s !!!!! WARNING: Problem aquiring "
         "netCDFid for variable V2. NETCDF ERROR:", __FILE__ );
         netCDF_StatusHandler( status );
      }

      if( ( status = nc_inq_varid( netCDFid, "V3", &tim_x3_velocity_V3_netCDFid ) ) != NC_NOERR )
      {
         v3_exists = 0; fprintf( stderr,"\n%s !!!!! WARNING: Problem aquiring "
         "netCDFid for variable V3. NETCDF ERROR:", __FILE__ );
         netCDF_StatusHandler( status );
      }

      if( ( status = nc_inq_varid( netCDFid, "B1", &tim_x1_magnetic_field_B1_netCDFid ) ) != NC_NOERR )
      {
         b1_exists = 0; fprintf( stderr,"\n%s !!!!! WARNING: Problem aquiring "
         "netCDFid for variable B1. NETCDF ERROR:", __FILE__ );
         netCDF_StatusHandler( status );
      }

      if( ( status = nc_inq_varid( netCDFid, "B2", &tim_x2_magnetic_field_B2_netCDFid ) ) != NC_NOERR )
      {
         b2_exists = 0; fprintf( stderr,"\n%s !!!!! WARNING: Problem aquiring "
         "netCDFid for variable B2. NETCDF ERROR:", __FILE__ );
         netCDF_StatusHandler( status );
      }

      if( ( status = nc_inq_varid( netCDFid, "B3", &tim_x3_magnetic_field_B3_netCDFid )) != NC_NOERR )
      {
         b3_exists = 0; fprintf( stderr,"\n%s !!!!! WARNING: Problem aquiring "
         "netCDFid for variable B3. NETCDF ERROR:", __FILE__ );
         netCDF_StatusHandler( status );
      }

      if( ( status = nc_inq_varid( netCDFid, "DP", &tim_cloud_mass_density_DP_netCDFid ) ) != NC_NOERR )
      {
         dp_exists = 0; fprintf( stderr,"\n%s !!!!! WARNING: Problem aquiring "
         "netCDFid for variable DP. NETCDF ERROR:", __FILE__ );
         netCDF_StatusHandler( status );
      }

      if( ( status = nc_inq_varid( netCDFid, "BP", &tim_polarity_of_magnetic_field_BP_netCDFid ) ) != NC_NOERR )
      {
         bp_exists = 0; fprintf( stderr,"\n%s !!!!! WARNING: Problem aquiring "
         "netCDFid for variable BP. NETCDF ERROR:", __FILE__ );
         netCDF_StatusHandler( status );
      }

      /* call get_enlil_variable_attributes() function
       * for each variable if id is valid ********************/

      if( DEBUG_FLAG )
      {
         printf("DEBUG\tcalling variable attr get function for "
         "block of time variables \n");
      }

      /* printf("\nNC_NOERR = %dtim_x1_position_X1_netCDFid =
       * %d\ntim_cloud_mass_density_DP_netCDFid = %d\n", NC_NOERR,
       * tim_x1_position_X1_netCDFid,tim_cloud_mass_density_DP_netCDFid );*/

      if( 1 )
      {
         status = get_enlil_variable_netcdf_attributes( netCDFid,
         tim_x1_position_X1_netCDFid, tim_x1_position_X1_long_name_vattr,
         tim_x1_position_X1_units_vattr );
      }
      if( 1 )
      {
         status = get_enlil_variable_netcdf_attributes( netCDFid,
         tim_x2_position_X2_netCDFid, tim_x2_position_X2_long_name_vattr,
         tim_x2_position_X2_units_vattr );
      }
      if( 1 )
      {
         status = get_enlil_variable_netcdf_attributes( netCDFid,
         tim_x3_position_X3_netCDFid, tim_x3_position_X3_long_name_vattr,
         tim_x3_position_X3_units_vattr );
      }
      if( 1 )
      {
         status = get_enlil_variable_netcdf_attributes( netCDFid,
         tim_x1h_position_X1H_netCDFid, tim_x1h_position_X1H_long_name_vattr,
         tim_x1h_position_X1H_units_vattr );
      }
      if( 1 )
      {
         status = get_enlil_variable_netcdf_attributes( netCDFid,
         tim_x2h_position_X2H_netCDFid, tim_x2h_position_X2H_long_name_vattr,
         tim_x2h_position_X2H_units_vattr );
      }
      if( 1 )
      {
         status = get_enlil_variable_netcdf_attributes( netCDFid,
         tim_x3h_position_X3H_netCDFid,
         tim_x3h_position_X3H_long_name_vattr,
         tim_x3h_position_X3H_units_vattr );
      }
      if( time_exists )
      {
         status = get_enlil_variable_netcdf_attributes( netCDFid,
         tim_physical_time_TIME_netCDFid,
         tim_physical_time_TIME_long_name_vattr,
         tim_physical_time_TIME_units_vattr );
      }
      if( dt_exists )
      {
         status = get_enlil_variable_netcdf_attributes( netCDFid,
         tim_physical_time_step_DT_netCDFid,
         tim_physical_time_step_DT_long_name_vattr,
         tim_physical_time_step_DT_units_vattr );
      }
      if( nstep_exists )
      {
         status = get_enlil_variable_netcdf_attributes( netCDFid,
         tim_numerical_time_step_NSTEP_netCDFid,
         tim_numerical_time_step_NSTEP_long_name_vattr,
         tim_numerical_time_step_NSTEP_units_vattr );
      }
      if( d_exists )
      {
         status = get_enlil_variable_netcdf_attributes( netCDFid,
         tim_density_D_netCDFid, tim_density_D_long_name_vattr,
         tim_density_D_units_vattr );
      }
      if( t_exists )
      {
         status = get_enlil_variable_netcdf_attributes( netCDFid,
         tim_temperature_T_netCDFid,
         tim_temperature_T_long_name_vattr,
         tim_temperature_T_units_vattr );
      }
      if( v1_exists )
      {
         status = get_enlil_variable_netcdf_attributes( netCDFid,
         tim_x1_velocity_V1_netCDFid,
         tim_x1_velocity_V1_long_name_vattr,
         tim_x1_velocity_V1_units_vattr );
      }
      if( v2_exists )
      {
         status = get_enlil_variable_netcdf_attributes( netCDFid,
         tim_x2_velocity_V2_netCDFid,
         tim_x2_velocity_V2_long_name_vattr,
         tim_x2_velocity_V2_units_vattr );
      }
      if( v3_exists )
      {
         status = get_enlil_variable_netcdf_attributes( netCDFid,
         tim_x3_velocity_V3_netCDFid,
         tim_x3_velocity_V3_long_name_vattr,
         tim_x3_velocity_V3_units_vattr );
      }
      if( b1_exists )
      {
         status = get_enlil_variable_netcdf_attributes( netCDFid,
         tim_x1_magnetic_field_B1_netCDFid,
         tim_x1_magnetic_field_B1_long_name_vattr,
         tim_x1_magnetic_field_B1_units_vattr );
      }
      if( b2_exists )
      {
         status = get_enlil_variable_netcdf_attributes( netCDFid,
         tim_x2_magnetic_field_B2_netCDFid,
         tim_x2_magnetic_field_B2_long_name_vattr,
         tim_x2_magnetic_field_B2_units_vattr );
      }
      if( b3_exists )
      {
         status = get_enlil_variable_netcdf_attributes( netCDFid,
         tim_x3_magnetic_field_B3_netCDFid,
         tim_x3_magnetic_field_B3_long_name_vattr,
         tim_x3_magnetic_field_B3_units_vattr );
      }
      if( dp_exists )
      {
         status = get_enlil_variable_netcdf_attributes( netCDFid,
         tim_cloud_mass_density_DP_netCDFid,
         tim_cloud_mass_density_DP_long_name_vattr,
         tim_cloud_mass_density_DP_units_vattr );
      }
      if( bp_exists )
      {
         status = get_enlil_variable_netcdf_attributes( netCDFid,
         tim_polarity_of_magnetic_field_BP_netCDFid,
         tim_polarity_of_magnetic_field_BP_long_name_vattr,
         tim_polarity_of_magnetic_field_BP_units_vattr );
      }

      if( DEBUG_FLAG ) printf("DEBUG\tback from calling variable attr get "
      "function for block of time variables \n");

      /*
       * dynamically allocate memory and then get the actual variable values
       *
       */

      if( DEBUG_FLAG ) printf("allocating mem\n");

      /**** allocate memory for position variables X1,X2,X3,X1H,X2H,X3H *****/

      /************ POSITION VARIABLES ************/

      if ( ( tim_x1_position_X1 = calloc( tim_n1 * tim_number_of_blocks_nblk, sizeof(float) ) ) == NULL )
      {
         printf("MALLOC for tim_x1_position_X1 array FAILED\n");
         exit(EXIT_FAILURE);
      }
      if ( ( tim_x2_position_X2 = calloc( tim_n2 * tim_number_of_blocks_nblk, sizeof(float) ) ) == NULL )
      {
         printf("MALLOC for tim_x2_position_X2 array FAILED\n");
         exit(EXIT_FAILURE);
      }
      if ( ( tim_x3_position_X3 = calloc( tim_n3 * tim_number_of_blocks_nblk, sizeof(float) ) ) == NULL )
      {
         printf("MALLOC for tim_x3h_position_X3 array FAILED\n");
         exit(EXIT_FAILURE);
      }
      if ( ( tim_x1h_position_X1H = calloc( tim_n1h * tim_number_of_blocks_nblk, sizeof(float) ) ) == NULL )
      {
         printf("MALLOC for tim_x1h_position_X1H array FAILED\n");
         exit(EXIT_FAILURE);
      }
      if ( ( tim_x2h_position_X2H = calloc( tim_n2h * tim_number_of_blocks_nblk, sizeof(float) ) ) == NULL )
      {
         printf("MALLOC for tim_x2h_position_X2H array FAILED\n");
         exit(EXIT_FAILURE);
      }
      if ( ( tim_x3h_position_X3H = calloc( tim_n3h * tim_number_of_blocks_nblk, sizeof(float) ) ) == NULL )
      {
         printf("MALLOC for tim_x3h_position_X3H array FAILED\n");
         exit(EXIT_FAILURE);
      }

      /***** scalar variables time, dt & nstep *****/

      if ( ( tim_physical_time_TIME = calloc( 1 , sizeof(float) ) ) == NULL )
      {
         printf("MALLOC for tim_physical_time_TIME FAILED\n");
         exit(EXIT_FAILURE);
      }
      if ( ( tim_physical_time_step_DT = calloc( 1 , sizeof(float) ) ) == NULL )
      {
         printf("MALLOC for tim_physical_time_step_DT FAILED\n");
         exit(EXIT_FAILURE);
      }
      if ( ( tim_numerical_time_step_NSTEP = calloc( 1 , sizeof(int) ) ) == NULL )
      {
         printf("MALLOC for tim_numerical_time_step_NSTEP FAILED\n");
         exit(EXIT_FAILURE);
      }

      /************ DATA VARIABLES ************/

      grid_size = tim_n1*tim_n2*tim_n3*tim_number_of_blocks_nblk;

      /** only allocate memory if variable exists **/

      if( d_exists )
      {
         if ( ( tim_density_D = calloc( grid_size, sizeof(float) ) ) == NULL )
         {
            printf("MALLOC for tim_density_D array FAILED\n");
            exit(EXIT_FAILURE);
         }
      }
      if( t_exists )
      {
         if ( ( tim_temperature_T = calloc( grid_size, sizeof(float) ) ) == NULL )
         {
            printf("MALLOC for tim_temperature_T array FAILED\n");
            exit(EXIT_FAILURE);
         }
      }
      if( v1_exists )
      {
         if ( ( tim_x1_velocity_V1 = calloc( grid_size, sizeof(float) ) ) == NULL )
         {
            printf("MALLOC for tim_x1_velocity_V1 array FAILED\n");
            exit(EXIT_FAILURE);
         }
      }
      if( v2_exists )
      {
         if ( ( tim_x2_velocity_V2 = calloc( grid_size, sizeof(float) ) ) == NULL )
         {
            printf("MALLOC for tim_x2_velocity_V2 array FAILED\n");
            exit(EXIT_FAILURE);
         }
      }
      if( v3_exists )
      {
         if ( ( tim_x3_velocity_V3 = calloc( grid_size, sizeof(float) ) ) == NULL )
         {
            printf("MALLOC for tim_x3_velocity_V3 array FAILED\n");
            exit(EXIT_FAILURE);
         }
      }
      if( b1_exists )
      {
         if ( ( tim_x1_magnetic_field_B1 = calloc( grid_size, sizeof(float) ) ) == NULL )
         {
            printf("MALLOC for tim_x1_velocity_B1 array FAILED\n");
            exit(EXIT_FAILURE);
         }
      }
      if( b2_exists )
      {
         if ( ( tim_x2_magnetic_field_B2 = calloc( grid_size, sizeof(float) ) ) == NULL )
         {
            printf("MALLOC for tim_x2_magnetic_field_B2 array FAILED\n");
            exit(EXIT_FAILURE);
         }
      }
      if( b3_exists )
      {
         if ( ( tim_x3_magnetic_field_B3 = calloc( grid_size, sizeof(float) ) ) == NULL )
         {
            printf("MALLOC for tim_x3_magnetic_field_B3 array FAILED\n");
            exit(EXIT_FAILURE);
         }
      }
      if( dp_exists )
      {
         if ( ( tim_cloud_mass_density_DP = calloc( grid_size, sizeof(float) ) ) == NULL )
         {
            printf("MALLOC for tim_cloud_mass_density_DP array FAILED\n");
            exit(EXIT_FAILURE);
         }
      }
      if( bp_exists )
      {
         if ( ( tim_polarity_of_magnetic_field_BP = calloc( grid_size, sizeof(float) ) ) == NULL )
         {
            printf("MALLOC for tim_polarity_of_magnetic_field_BP array FAILED\n");
            exit(EXIT_FAILURE);
         }
      }

      if( DEBUG_FLAG ) printf("reading files\n");

      /******** now get actual position values from netCDF file *************/

      if( nc_get_var_float( netCDFid, tim_x1_position_X1_netCDFid, tim_x1_position_X1 ) != NC_NOERR )
      {
         netCDF_StatusHandler( status ); return EXIT_FAILURE;
      }
      if( nc_get_var_float( netCDFid, tim_x2_position_X2_netCDFid, tim_x2_position_X2 ) != NC_NOERR )
      {
         netCDF_StatusHandler( status ); return EXIT_FAILURE;
      }
      if( nc_get_var_float( netCDFid, tim_x3_position_X3_netCDFid, tim_x3_position_X3 ) != NC_NOERR )
      {
         netCDF_StatusHandler( status ); return EXIT_FAILURE;
      }
      if( nc_get_var_float( netCDFid, tim_x1h_position_X1H_netCDFid, tim_x1h_position_X1H ) != NC_NOERR )
      {
         netCDF_StatusHandler( status ); return EXIT_FAILURE;
      }
      if( nc_get_var_float( netCDFid, tim_x2h_position_X2H_netCDFid, tim_x2h_position_X2H ) != NC_NOERR )
      {
         netCDF_StatusHandler( status ); return EXIT_FAILURE;
      }
      if( nc_get_var_float( netCDFid, tim_x3h_position_X3H_netCDFid, tim_x3h_position_X3H ) != NC_NOERR )
      {
         netCDF_StatusHandler( status ); return EXIT_FAILURE;
      }

      if( DEBUG_FLAG )
      {
         printf("DEBUG\tmain grid size = [%d,%d,%d]\n",
         (int ) tim_n1, (int ) tim_n2,
         (int ) tim_n3 );
      }

      /*
       for( i = 0; i < tim_n2 ; i++ )
       {
       printf(" cell_%d_position[%f,%f,%f]\n", i, tim_x1_position_X1[i], tim_x2_position_X2[i], tim_x3_position_X3[i] );
       }
       */

      /*********** DATA VALUES ********/

      if( d_exists )
      {
         if( nc_get_var_float( netCDFid, tim_density_D_netCDFid, tim_density_D ) != NC_NOERR )
         {
            netCDF_StatusHandler( status ); return EXIT_FAILURE;
         }
      }
      if( t_exists )
      {
         if( nc_get_var_float( netCDFid, tim_temperature_T_netCDFid, tim_temperature_T ) != NC_NOERR )
         {
            netCDF_StatusHandler( status ); return EXIT_FAILURE;
         }
      }
      if( v1_exists )
      {
         if( nc_get_var_float( netCDFid, tim_x1_velocity_V1_netCDFid, tim_x1_velocity_V1 ) != NC_NOERR )
         {
            netCDF_StatusHandler( status ); return EXIT_FAILURE;
         }
      }
      if( v2_exists )
      {
         if( nc_get_var_float( netCDFid, tim_x2_velocity_V2_netCDFid, tim_x2_velocity_V2 ) != NC_NOERR )
         {
            netCDF_StatusHandler( status ); return EXIT_FAILURE;
         }
      }
      if( v3_exists )
      {
         if( nc_get_var_float( netCDFid, tim_x3_velocity_V3_netCDFid, tim_x3_velocity_V3 ) != NC_NOERR )
         {
            netCDF_StatusHandler( status ); return EXIT_FAILURE;
         }
      }
      if( b1_exists )
      {
         if( nc_get_var_float( netCDFid, tim_x1_magnetic_field_B1_netCDFid, tim_x1_magnetic_field_B1 ) != NC_NOERR )
         {
            netCDF_StatusHandler( status ); return EXIT_FAILURE;
         }
      }
      if( b2_exists )
      {
         if( nc_get_var_float( netCDFid, tim_x2_magnetic_field_B2_netCDFid, tim_x2_magnetic_field_B2 ) != NC_NOERR )
         {
            netCDF_StatusHandler( status );
            return EXIT_FAILURE;
         }
      }
      if( b3_exists )
      {
         if( nc_get_var_float( netCDFid, tim_x3_magnetic_field_B3_netCDFid, tim_x3_magnetic_field_B3 ) != NC_NOERR )
         {
            netCDF_StatusHandler( status );
            return EXIT_FAILURE;
         }
      }
      if( dp_exists )
      {
         if( nc_get_var_float( netCDFid, tim_cloud_mass_density_DP_netCDFid, tim_cloud_mass_density_DP ) != NC_NOERR )
         {
            netCDF_StatusHandler( status );
            return EXIT_FAILURE;
         }
      }
      if( bp_exists )
      {
         if( nc_get_var_float( netCDFid, tim_polarity_of_magnetic_field_BP_netCDFid, tim_polarity_of_magnetic_field_BP ) != NC_NOERR )
         {
            netCDF_StatusHandler( status );
            return EXIT_FAILURE;
         }
      }

      /*
       for( i = 0; i < grid_size ; i+=30000 )
       {
       printf(" tim_polarity_of_magnetic_field_BP[%d] = %f\n",i, tim_polarity_of_magnetic_field_BP[i] );
       }
       */
      /******* Now get the three scalar variables for time, dt, nstep **************/

      if( nc_get_var_float( netCDFid, tim_physical_time_TIME_netCDFid, tim_physical_time_TIME ) != NC_NOERR )
      {
         netCDF_StatusHandler( status );
         return EXIT_FAILURE;
      }

      if( nc_get_var_float( netCDFid, tim_physical_time_step_DT_netCDFid, tim_physical_time_step_DT ) != NC_NOERR )
      {
         netCDF_StatusHandler( status );
         return EXIT_FAILURE;
      }

      if( nc_get_var_int( netCDFid, tim_numerical_time_step_NSTEP_netCDFid, tim_numerical_time_step_NSTEP ) != NC_NOERR )
      {
         netCDF_StatusHandler( status );
         return EXIT_FAILURE;
      }

      /********* close the currently open netCDF file **************/

      nc_close( netCDFid );

#endif

      return EXIT_SUCCESS;

   }

   /***************************************
    *             READ ENLIL EVH          *
    ***************************************/

int read_enlil_evh_file(char *input_file_name, int verbose_flag)
{

#ifdef NETCDF

   int i, grid_size;
   int status;

   /*** create var_exists variable to determine if a variable needs to be allocated and read or not ***/

   int time_exists = 1, dt_exists = 1, nstep_exists = 1, d_exists = 1,
         t_exists = 1, v1_exists = 1, v2_exists = 1, v3_exists = 1, b1_exists =
               1, b2_exists = 1, b3_exists = 1, dp_exists = 1, bp_exists = 1;

   /******* variable for netCDF specific calls *********/

   int netCDFid;

   int file_open_mode= NC_NOWRITE;

   if (verbose_flag)
   {
      printf("...reading %s\n", input_file_name);
   }

   /******** open evh.nc netCDF file *****/

   if ( DEBUG_FLAG)
   {
      printf("calling nc_open\n");
   }

   status = nc_open(input_file_name, file_open_mode, &netCDFid);

   if ( DEBUG_FLAG)
   {
      printf("netCDFid = %d\n", netCDFid);
   }

   /* if netCDF file was not opened properly, return contron to calling
    * program with EXIT_FAILUE **/

   if (status != NC_NOERR)
   {
      netCDF_StatusHandler(status);
      return EXIT_FAILURE;
   }

   /************** call get_enlil_global_attribute() function ***************/

   status = get_enlil_global_netcdf_attributes(
         netCDFid,
         &evh_gattribute_type,
         &evh_gattribute_title,
         &evh_gattribute_name,
         &evh_gattribute_project,
         &evh_gattribute_initial,
         &evh_gattribute_resume,
         &evh_gattribute_boundary,
         &evh_gattribute_passage,
         &evh_gattribute_grid,
         &evh_gattribute_geometry,
         &evh_gattribute_code,
         &evh_gattribute_parameters,
         &evh_gattribute_refdate_mjd,
         &evh_gattribute_gamma,
         &evh_gattribute_xalpha,
         &evh_gattribute_history);
   if ( DEBUG_FLAG)
      printf(
            "DEBUG\t%s\treturn status from get_enlil_global_attributes = %d\n",
            __FILE__, status );

            if( DEBUG_FLAG ) printf("getting id's\n");

            /* get dimensionid'sand sizes from evh.nc file *******************/

      if( ( status = nc_inq_dimid( netCDFid, "nevo", &evh_nevo_netCDFid ) ) != NC_NOERR )
      {
         netCDF_StatusHandler( status );
         return EXIT_FAILURE;
      }

      if( ( status = nc_inq_dimid( netCDFid, "nhel", &evh_nhel_netCDFid ) ) != NC_NOERR )
      {
         netCDF_StatusHandler( status );
         return EXIT_FAILURE;
      }

      if( ( status = nc_inq_dimlen( netCDFid, evh_nevo_netCDFid, &evh_nevo ) ) != NC_NOERR )
      {
         netCDF_StatusHandler( status );
         return EXIT_FAILURE;
      }

      if( ( status = nc_inq_dimlen( netCDFid, evh_nhel_netCDFid, &evh_nhel ) ) != NC_NOERR )
      {
         netCDF_StatusHandler( status );
         return EXIT_FAILURE;
      }

      /******* get all of the variable id numbers from the evh.nc file ********/

      if( ( status = nc_inq_varid( netCDFid, "X1", &evh_x1_position_X1_netCDFid ) ) != NC_NOERR )
      {
         netCDF_StatusHandler( status );
         return EXIT_FAILURE;
      }

      if( ( status = nc_inq_varid( netCDFid, "X2", &evh_x2_position_X2_netCDFid ) ) != NC_NOERR )
      {
         netCDF_StatusHandler( status );
         return EXIT_FAILURE;
      }

      if( ( status = nc_inq_varid( netCDFid, "X3", &evh_x3_position_X3_netCDFid ) ) != NC_NOERR )
      {
         netCDF_StatusHandler( status );
         return EXIT_FAILURE;
      }

      /*
       * we don't want to die if a regular variable is not found, so don't
       * return control
       *
       */

      /* doing this primarily because my development test data does not
       * contain dp cloud mass density variable
       * but we should not abandon program beacause of this - plus it's a good
       * idea anyway - just print a warning **/

      if( ( status = nc_inq_varid( netCDFid, "TIME", &evh_physical_time_TIME_netCDFid ) ) != NC_NOERR )
      {
         time_exists = 0; fprintf( stderr, "\n%s !!!!! WARNING: Problem aquiring"
               " netCDFid for variable TIME. NETCDF ERROR:", __FILE__ );
         netCDF_StatusHandler( status );
      }
      if( ( status = nc_inq_varid( netCDFid, "DT", &evh_physical_time_step_DT_netCDFid ) ) != NC_NOERR )
      {
         dt_exists = 0; fprintf( stderr, "\n%s !!!!! WARNING: Problem aquiring "
               "netCDFid for variable DT. NETCDF ERROR:", __FILE__ );
         netCDF_StatusHandler( status );
      }
      if( ( status = nc_inq_varid( netCDFid, "NSTEP", &evh_numerical_time_step_NSTEP_netCDFid ) ) != NC_NOERR )
      {
         nstep_exists = 0; fprintf( stderr, "\n%s !!!!! WARNING: Problem aquiring "
               "netCDFid for variable NSTEP. NETCDF ERROR:", __FILE__ );
         netCDF_StatusHandler( status );
      }

      if( ( status = nc_inq_varid( netCDFid, "D", &evh_density_D_netCDFid ) ) != NC_NOERR )
      {
         d_exists = 0; fprintf( stderr, "\n%s !!!!! WARNING: Problem aquiring "
               "netCDFid for variable D. NETCDF ERROR:", __FILE__ );
         netCDF_StatusHandler( status );
      }
      if( ( status = nc_inq_varid( netCDFid, "T", &evh_temperature_T_netCDFid ) ) != NC_NOERR )
      {
         t_exists = 0; fprintf( stderr,"\n%s !!!!! WARNING: Problem aquiring"
               " netCDFid for variable T. NETCDF ERROR:", __FILE__ );
         netCDF_StatusHandler( status );
      }
      if( ( status = nc_inq_varid( netCDFid, "V1", &evh_x1_velocity_V1_netCDFid ) ) != NC_NOERR )
      {
         v1_exists = 0; fprintf( stderr,"\n%s !!!!! WARNING: Problem aquiring "
               "netCDFid for variable V1. NETCDF ERROR:", __FILE__ );
         netCDF_StatusHandler( status );
      }
      if( ( status = nc_inq_varid( netCDFid, "V2", &evh_x2_velocity_V2_netCDFid ) ) != NC_NOERR )
      {
         v2_exists = 0; fprintf( stderr,"\n%s !!!!! WARNING: Problem aquiring "
               "netCDFid for variable V2. NETCDF ERROR:", __FILE__ );
         netCDF_StatusHandler( status );
      }
      if( ( status = nc_inq_varid( netCDFid, "V3", &evh_x3_velocity_V3_netCDFid ) ) != NC_NOERR )
      {
         v3_exists = 0; fprintf( stderr,"\n%s !!!!! WARNING: Problem aquiring "
               "netCDFid for variable V3. NETCDF ERROR:", __FILE__ );
         netCDF_StatusHandler( status );
      }
      if( ( status = nc_inq_varid( netCDFid, "B1", &evh_x1_magnetic_field_B1_netCDFid ) ) != NC_NOERR )
      {  b1_exists = 0; fprintf( stderr,"\n%s !!!!! WARNING: Problem aquiring "
               "netCDFid for variable B1. NETCDF ERROR:", __FILE__ );
         netCDF_StatusHandler( status );
      }
      if( ( status = nc_inq_varid( netCDFid, "B2", &evh_x2_magnetic_field_B2_netCDFid ) ) != NC_NOERR )
      {  b2_exists = 0; fprintf( stderr,"\n%s !!!!! WARNING: Problem aquiring "
               "netCDFid for variable B2. NETCDF ERROR:", __FILE__ );
         netCDF_StatusHandler( status );
      }
      if( ( status = nc_inq_varid( netCDFid, "B3", &evh_x3_magnetic_field_B3_netCDFid )) != NC_NOERR )
      {  b3_exists = 0; fprintf( stderr,"\n%s !!!!! WARNING: Problem aquiring "
               "netCDFid for variable B3. NETCDF ERROR:", __FILE__ );
         netCDF_StatusHandler( status );
      }
      if( ( status = nc_inq_varid( netCDFid, "DP", &evh_cloud_mass_density_DP_netCDFid ) ) != NC_NOERR )
      {  dp_exists = 0; fprintf( stderr,"\n%s !!!!! WARNING: Problem aquiring "
               "netCDFid for variable DP. NETCDF ERROR:", __FILE__ );
         netCDF_StatusHandler( status );
      }
      if( ( status = nc_inq_varid( netCDFid, "BP", &evh_polarity_of_magnetic_field_BP_netCDFid ) ) != NC_NOERR )
      {  bp_exists = 0; fprintf( stderr,"\n%s !!!!! WARNING: Problem aquiring "
               "netCDFid for variable BP. NETCDF ERROR:", __FILE__ );
         netCDF_StatusHandler( status );
      }

      /* call get_enlil_variable_attributes() function for each variable if id
       * is valid ********************/

      if( 1 )
      {
         status = get_enlil_variable_netcdf_attributes( netCDFid,
               evh_x1_position_X1_netCDFid, evh_x1_position_X1_long_name_vattr,
               evh_x1_position_X1_units_vattr );
      }
      if( 1 )
      {
         status = get_enlil_variable_netcdf_attributes( netCDFid,
               evh_x2_position_X2_netCDFid, evh_x2_position_X2_long_name_vattr,
               evh_x2_position_X2_units_vattr );
      }
      if( 1 )
      {
         status = get_enlil_variable_netcdf_attributes( netCDFid,
               evh_x3_position_X3_netCDFid, evh_x3_position_X3_long_name_vattr,
               evh_x3_position_X3_units_vattr );
      }
      if( time_exists )
      {
         status = get_enlil_variable_netcdf_attributes( netCDFid,
               evh_physical_time_TIME_netCDFid,
               evh_physical_time_TIME_long_name_vattr,
               evh_physical_time_TIME_units_vattr );
      }
      if( dt_exists )
      {
         status = get_enlil_variable_netcdf_attributes( netCDFid,
               evh_physical_time_step_DT_netCDFid,
               evh_physical_time_step_DT_long_name_vattr,
               evh_physical_time_step_DT_units_vattr );
      }
      if( nstep_exists )
      {
         status = get_enlil_variable_netcdf_attributes( netCDFid,
               evh_numerical_time_step_NSTEP_netCDFid,
               evh_numerical_time_step_NSTEP_long_name_vattr,
               evh_numerical_time_step_NSTEP_units_vattr );
      }
      if( d_exists )
      {
         status = get_enlil_variable_netcdf_attributes( netCDFid,
               evh_density_D_netCDFid, evh_density_D_long_name_vattr,
               evh_density_D_units_vattr );
      }
      if( t_exists )
      {
         status = get_enlil_variable_netcdf_attributes( netCDFid,
               evh_temperature_T_netCDFid,evh_temperature_T_long_name_vattr,
               evh_temperature_T_units_vattr );
      }
      if( v1_exists )
      {
         status = get_enlil_variable_netcdf_attributes( netCDFid,
               evh_x1_velocity_V1_netCDFid,evh_x1_velocity_V1_long_name_vattr,
               evh_x1_velocity_V1_units_vattr );
      }
      if( v2_exists )
      {
         status = get_enlil_variable_netcdf_attributes( netCDFid,
               evh_x2_velocity_V2_netCDFid,evh_x2_velocity_V2_long_name_vattr,
               evh_x2_velocity_V2_units_vattr );
      }
      if( v3_exists )
      {
         status = get_enlil_variable_netcdf_attributes( netCDFid,
               evh_x3_velocity_V3_netCDFid,evh_x3_velocity_V3_long_name_vattr,
               evh_x3_velocity_V3_units_vattr );
      }
      if( b1_exists )
      {
         status = get_enlil_variable_netcdf_attributes( netCDFid,
               evh_x1_magnetic_field_B1_netCDFid,
               evh_x1_magnetic_field_B1_long_name_vattr,
               evh_x1_magnetic_field_B1_units_vattr );
      }
      if( b2_exists )
      {
         status = get_enlil_variable_netcdf_attributes( netCDFid,
               evh_x2_magnetic_field_B2_netCDFid,
               evh_x2_magnetic_field_B2_long_name_vattr,
               evh_x2_magnetic_field_B2_units_vattr );
      }
      if( b3_exists )
      {
         status = get_enlil_variable_netcdf_attributes( netCDFid,
               evh_x3_magnetic_field_B3_netCDFid,
               evh_x3_magnetic_field_B3_long_name_vattr,
               evh_x3_magnetic_field_B3_units_vattr );
      }
      if( dp_exists )
      {
         status = get_enlil_variable_netcdf_attributes( netCDFid,
               evh_cloud_mass_density_DP_netCDFid,
               evh_cloud_mass_density_DP_long_name_vattr,
               evh_cloud_mass_density_DP_units_vattr );
      }
      if( bp_exists )
      {
         status = get_enlil_variable_netcdf_attributes( netCDFid,
               evh_polarity_of_magnetic_field_BP_netCDFid,
               evh_polarity_of_magnetic_field_BP_long_name_vattr,
               evh_polarity_of_magnetic_field_BP_units_vattr );
      }

      /* dynamically allocate memory and then get the actual variable values */

      if( DEBUG_FLAG ) printf("allocating mem\n");

      /****** allocate memory for position variables X1,X2,X3 *******/

      /**** NOTE: the size of x1,x2,x3 for evh = nhel ****/

      /************ POSITION VARIABLES ************/

      if ( ( evh_x1_position_X1 = calloc( evh_nhel, sizeof(float) ) ) == NULL )
      {
         printf("MALLOC for evh_x1_position_X1 array FAILED\n");
         exit(EXIT_FAILURE);
      }
      if ( ( evh_x2_position_X2 = calloc( evh_nhel, sizeof(float) ) ) == NULL )
      {
         printf("MALLOC for evh_x2_position_X2 array FAILED\n");
         exit(EXIT_FAILURE);
      }
      if ( ( evh_x3_position_X3 = calloc( evh_nhel, sizeof(float) ) ) == NULL )
      {
         printf("MALLOC for evh_x3h_position_X3 array FAILED\n");
         exit(EXIT_FAILURE);
      }

      /***** variables time, dt & nstep *****/

      if ( ( evh_physical_time_TIME = calloc( evh_nevo , sizeof(float) ) ) == NULL )
      {
         printf("MALLOC for evh_physical_time_TIME FAILED\n");
         exit(EXIT_FAILURE);
      }
      if ( ( evh_physical_time_step_DT = calloc( evh_nevo , sizeof(float) ) ) == NULL )
      {
         printf("MALLOC for evh_physical_time_step_DT FAILED\n");
         exit(EXIT_FAILURE);
      }
      if ( ( evh_numerical_time_step_NSTEP = calloc( evh_nevo , sizeof(int) ) ) == NULL )
      {
         printf("MALLOC for evh_numerical_time_step_NSTEP FAILED\n");
         exit(EXIT_FAILURE);
      }

      /************ DATA VARIABLES ************/

      grid_size = evh_nhel * evh_nevo;

      /** only allocate memory if variable exists **/

      if( d_exists )
      {
         if ( ( evh_density_D = calloc( grid_size, sizeof(float) ) ) == NULL )
         {
            printf("MALLOC for evh_density_D array FAILED\n");
            exit(EXIT_FAILURE);
         }
      }
      if( t_exists )
      {
         if ( ( evh_temperature_T = calloc( grid_size, sizeof(float) ) ) == NULL )
         {
            printf("MALLOC for evh_temperature_T array FAILED\n");
            exit(EXIT_FAILURE);
         }
      }
      if( v1_exists )
      {
         if ( ( evh_x1_velocity_V1 = calloc( grid_size, sizeof(float) ) ) == NULL )
         {
            printf("MALLOC for evh_x1_velocity_V1 array FAILED\n");
            exit(EXIT_FAILURE);
         }
      }
      if( v2_exists )
      {
         if ( ( evh_x2_velocity_V2 = calloc( grid_size, sizeof(float) ) ) == NULL )
         {
            printf("MALLOC for evh_x2_velocity_V2 array FAILED\n");
            exit(EXIT_FAILURE);
         }
      }
      if( v3_exists )
      {
         if ( ( evh_x3_velocity_V3 = calloc( grid_size, sizeof(float) ) ) == NULL )
         {
            printf("MALLOC for evh_x3_velocity_V3 array FAILED\n");
            exit(EXIT_FAILURE);
         }
      }
      if( b1_exists )
      {
         if ( ( evh_x1_magnetic_field_B1 = calloc( grid_size, sizeof(float) ) ) == NULL )
         {
            printf("MALLOC for evh_x1_velocity_B1 array FAILED\n");
            exit(EXIT_FAILURE);
         }
      }
      if( b2_exists )
      {
         if ( ( evh_x2_magnetic_field_B2 = calloc( grid_size, sizeof(float) ) ) == NULL )
         {
            printf("MALLOC for evh_x2_magnetic_field_B2 array FAILED\n");
            exit(EXIT_FAILURE);
         }
      }
      if( b3_exists )
      {
         if ( ( evh_x3_magnetic_field_B3 = calloc( grid_size, sizeof(float) ) ) == NULL )
         {
            printf("MALLOC for evh_x3_magnetic_field_B3 array FAILED\n");
            exit(EXIT_FAILURE);
         }
      }
      if( dp_exists )
      {
         if ( ( evh_cloud_mass_density_DP = calloc( grid_size, sizeof(float) ) ) == NULL )
         {
            printf("MALLOC for evh_cloud_mass_density_DP array FAILED\n");
            exit(EXIT_FAILURE);
         }
      }
      if( bp_exists )
      {
         if ( ( evh_polarity_of_magnetic_field_BP = calloc( grid_size, sizeof(float) ) ) == NULL )
         {
            printf("MALLOC for evh_polarity_of_magnetic_field_BP array FAILED\n");
            exit(EXIT_FAILURE);
         }
      }

      if( DEBUG_FLAG ) printf("reading files\n");

      /******** now get actual position values from netCDF file *************/

      if( nc_get_var_float( netCDFid, evh_x1_position_X1_netCDFid, evh_x1_position_X1 ) != NC_NOERR )
      {
         netCDF_StatusHandler( status ); return EXIT_FAILURE;
      }
      if( nc_get_var_float( netCDFid, evh_x2_position_X2_netCDFid, evh_x2_position_X2 ) != NC_NOERR )
      {
         netCDF_StatusHandler( status ); return EXIT_FAILURE;
      }
      if( nc_get_var_float( netCDFid, evh_x3_position_X3_netCDFid, evh_x3_position_X3 ) != NC_NOERR )
      {
         netCDF_StatusHandler( status ); return EXIT_FAILURE;
      }

      /*
       printf("DEBUG\tmain grid size = [%d,%d]\n", (int ) evh_nevo, (int ) evh_nhel );

       for( i = 0; i < evh_nhel ; i++ )
       {
       printf(" cell_%d_position[%f,%f,%f]\n", i, evh_x1_position_X1[i], evh_x2_position_X2[i], evh_x3_position_X3[i] );
       }
       */

      /*********** DATA VALUES ********/

      if( d_exists )
      {
         if( nc_get_var_float( netCDFid, evh_density_D_netCDFid, evh_density_D ) != NC_NOERR )
         {
            netCDF_StatusHandler( status ); return EXIT_FAILURE;
         }
      }
      if( t_exists )
      {
         if( nc_get_var_float( netCDFid, evh_temperature_T_netCDFid, evh_temperature_T ) != NC_NOERR )
         {
            netCDF_StatusHandler( status ); return EXIT_FAILURE;
         }
      }
      if( v1_exists )
      {
         if( nc_get_var_float( netCDFid, evh_x1_velocity_V1_netCDFid, evh_x1_velocity_V1 ) != NC_NOERR )
         {
            netCDF_StatusHandler( status ); return EXIT_FAILURE;
         }
      }
      if( v2_exists )
      {
         if( nc_get_var_float( netCDFid, evh_x2_velocity_V2_netCDFid, evh_x2_velocity_V2 ) != NC_NOERR )
         {
            netCDF_StatusHandler( status ); return EXIT_FAILURE;
         }
      }
      if( v3_exists )
      {
         if( nc_get_var_float( netCDFid, evh_x3_velocity_V3_netCDFid, evh_x3_velocity_V3 ) != NC_NOERR )
         {
            netCDF_StatusHandler( status ); return EXIT_FAILURE;
         }
      }
      if( b1_exists )
      {
         if( nc_get_var_float( netCDFid, evh_x1_magnetic_field_B1_netCDFid, evh_x1_magnetic_field_B1 ) != NC_NOERR )
         {
            netCDF_StatusHandler( status ); return EXIT_FAILURE;
         }
      }
      if( b2_exists )
      {
         if( nc_get_var_float( netCDFid, evh_x2_magnetic_field_B2_netCDFid, evh_x2_magnetic_field_B2 ) != NC_NOERR )
         {
            netCDF_StatusHandler( status ); return EXIT_FAILURE;
         }
      }
      if( b3_exists )
      {
         if( nc_get_var_float( netCDFid, evh_x3_magnetic_field_B3_netCDFid, evh_x3_magnetic_field_B3 ) != NC_NOERR )
         {
            netCDF_StatusHandler( status ); return EXIT_FAILURE;
         }
      }
      if( dp_exists )
      {
         if( nc_get_var_float( netCDFid, evh_cloud_mass_density_DP_netCDFid, evh_cloud_mass_density_DP ) != NC_NOERR )
         {
            netCDF_StatusHandler( status ); return EXIT_FAILURE;
         }
      }
      if( bp_exists )
      {
         if( nc_get_var_float( netCDFid, evh_polarity_of_magnetic_field_BP_netCDFid, evh_polarity_of_magnetic_field_BP ) != NC_NOERR )
         {
            netCDF_StatusHandler( status ); return EXIT_FAILURE;
         }
      }

      /*
       for( i = 0; i < grid_size ; i+=1635 )
       {
       printf(" evh_polarity_of_magnetic_field_BP[%d] = %f\n",i, evh_polarity_of_magnetic_field_BP[i] );
       }
       */

      /******* Now get the three time variables for time, dt, nstep **************/

      if( nc_get_var_float( netCDFid, evh_physical_time_TIME_netCDFid, evh_physical_time_TIME ) != NC_NOERR )
      {
         netCDF_StatusHandler( status ); return EXIT_FAILURE;

      }
      if( nc_get_var_float( netCDFid, evh_physical_time_step_DT_netCDFid, evh_physical_time_step_DT ) != NC_NOERR )
      {
         netCDF_StatusHandler( status ); return EXIT_FAILURE;
      }
      if( nc_get_var_int( netCDFid, evh_numerical_time_step_NSTEP_netCDFid, evh_numerical_time_step_NSTEP ) != NC_NOERR )
      {
         netCDF_StatusHandler( status ); return EXIT_FAILURE;
      }

      /********* close the currently open netCDF file **************/

      nc_close( netCDFid );

#endif

      return EXIT_SUCCESS;
   }

   /**************************************
    *             READ ENLIL EVL            *
    **************************************/

int read_enlil_evl_file(char *input_file_name, int verbose_flag)
{
#ifdef NETCDF

   int i, grid_size;
   int status;

   /*** create var_exists variable to determine if a variable needs to be allocated and read or not ***/

   int time_exists = 1, dt_exists = 1, nstep_exists = 1, d_exists = 1,
         t_exists = 1, v1_exists = 1, v2_exists = 1, v3_exists = 1, b1_exists =
               1, b2_exists = 1, b3_exists = 1, dp_exists = 1, bp_exists = 1;

   /******* variable for netCDF specific calls *********/

   int netCDFid;

   int file_open_mode= NC_NOWRITE;

   if (verbose_flag)
   {
      printf("...reading %s\n", input_file_name);
   }

   /******** open evl.nc netCDF file *****/

   if ( DEBUG_FLAG)
      printf("calling nc_open\n");

   status = nc_open(input_file_name, file_open_mode, &netCDFid);

   if ( DEBUG_FLAG)
      printf("netCDFid = %d\n", netCDFid);

   /*** if netCDF file was not opened properly, return contron to calling program with EXIT_FAILUE **/

   if (status != NC_NOERR)
   {
      netCDF_StatusHandler(status);
      return EXIT_FAILURE;
   }

   /************************ call get_enlil_global_attribute() function ********************/

   status = get_enlil_global_netcdf_attributes(
         netCDFid,
         &evl_gattribute_type,
         &evl_gattribute_title,
         &evl_gattribute_name,
         &evl_gattribute_project,
         &evl_gattribute_initial,
         &evl_gattribute_resume,
         &evl_gattribute_boundary,
         &evl_gattribute_passage,
         &evl_gattribute_grid,
         &evl_gattribute_geometry,
         &evl_gattribute_code,
         &evl_gattribute_parameters,
         &evl_gattribute_refdate_mjd,
         &evl_gattribute_gamma,
         &evl_gattribute_xalpha,
         &evl_gattribute_history);
   if ( DEBUG_FLAG)
      printf(
            "DEBUG\t%s\treturn status from get_enlil_global_attributes = %d\n",
            __FILE__, status );

            if( DEBUG_FLAG ) printf("getting id's\n");

            /********************* get dimension id'sand sizes from evl.nc file **********************************/

      if( ( status = nc_inq_dimid( netCDFid, "nevo", &evl_nevo_netCDFid ) ) != NC_NOERR )
      {
         netCDF_StatusHandler( status );
         return EXIT_FAILURE;
      }

      if( ( status = nc_inq_dimid( netCDFid, "nsel", &evl_nsel_netCDFid ) ) != NC_NOERR )
      {
         netCDF_StatusHandler( status );
         return EXIT_FAILURE;
      }

      if( ( status = nc_inq_dimlen( netCDFid, evl_nevo_netCDFid, &evl_nevo ) ) != NC_NOERR )
      {
         netCDF_StatusHandler( status );
         return EXIT_FAILURE;
      }

      if( ( status = nc_inq_dimlen( netCDFid, evl_nsel_netCDFid, &evl_nsel ) ) != NC_NOERR )
      {
         netCDF_StatusHandler( status );
         return EXIT_FAILURE;
      }

      /******* get all of the variable id numbers from the evl.nc file ********/

      if( ( status = nc_inq_varid( netCDFid, "X1", &evl_x1_position_X1_netCDFid ) ) != NC_NOERR )
      {
         netCDF_StatusHandler( status );
         return EXIT_FAILURE;
      }

      if( ( status = nc_inq_varid( netCDFid, "X2", &evl_x2_position_X2_netCDFid ) ) != NC_NOERR )
      {
         netCDF_StatusHandler( status );
         return EXIT_FAILURE;
      }

      if( ( status = nc_inq_varid( netCDFid, "X3", &evl_x3_position_X3_netCDFid ) ) != NC_NOERR )
      {
         netCDF_StatusHandler( status );
         return EXIT_FAILURE;
      }

      /*** we don't want to die if a regular variable is not found, so don't return control ***/
      /*** doing this primarily because my development test data does not contain dp cloud mass density variable
       * but we should not abandon program beacause of this - plus it's a good idea anyway - just print a warning **/

      if( ( status = nc_inq_varid( netCDFid, "TIME", &evl_physical_time_TIME_netCDFid ) ) != NC_NOERR )
      {
         time_exists = 0; fprintf( stderr, "\n%s !!!!! WARNING: Problem aquiring"
               " netCDFid for variable TIME. NETCDF ERROR:", __FILE__ );
         netCDF_StatusHandler( status );
      }

      if( ( status = nc_inq_varid( netCDFid, "DT", &evl_physical_time_step_DT_netCDFid ) ) != NC_NOERR )
      {
         dt_exists = 0; fprintf( stderr, "\n%s !!!!! WARNING: Problem aquiring "
               "netCDFid for variable DT. NETCDF ERROR:", __FILE__ );
         netCDF_StatusHandler( status );
      }

      if( ( status = nc_inq_varid( netCDFid, "NSTEP", &evl_numerical_time_step_NSTEP_netCDFid ) ) != NC_NOERR )
      {
         nstep_exists = 0; fprintf( stderr, "\n%s !!!!! WARNING: Problem "
               "aquiring netCDFid for variable NSTEP. NETCDF ERROR:", __FILE__ );
         netCDF_StatusHandler( status );
      }

      if( ( status = nc_inq_varid( netCDFid, "D", &evl_density_D_netCDFid ) ) != NC_NOERR )
      {
         d_exists = 0; fprintf( stderr, "\n%s !!!!! WARNING: Problem aquiring "
               "netCDFid for variable D. NETCDF ERROR:", __FILE__ );
         netCDF_StatusHandler( status );
      }

      if( ( status = nc_inq_varid( netCDFid, "T", &evl_temperature_T_netCDFid ) ) != NC_NOERR )
      {
         t_exists = 0; fprintf( stderr,"\n%s !!!!! WARNING: Problem aquiring "
               "netCDFid for variable T. NETCDF ERROR:", __FILE__ ); netCDF_StatusHandler( status );
      }

      if( ( status = nc_inq_varid( netCDFid, "V1", &evl_x1_velocity_V1_netCDFid ) ) != NC_NOERR )
      {  v1_exists = 0; fprintf( stderr,"\n%s !!!!! WARNING: Problem aquiring "
               "netCDFid for variable V1. NETCDF ERROR:", __FILE__ );
         netCDF_StatusHandler( status );
      }

      if( ( status = nc_inq_varid( netCDFid, "V2", &evl_x2_velocity_V2_netCDFid ) ) != NC_NOERR )
      {  v2_exists = 0; fprintf( stderr,"\n%s !!!!! WARNING: Problem aquiring "
               "netCDFid for variable V2. NETCDF ERROR:", __FILE__ );
         netCDF_StatusHandler( status );
      }

      if( ( status = nc_inq_varid( netCDFid, "V3", &evl_x3_velocity_V3_netCDFid ) ) != NC_NOERR )
      {  v3_exists = 0; fprintf( stderr,"\n%s !!!!! WARNING: Problem aquiring "
               "netCDFid for variable V3. NETCDF ERROR:", __FILE__ );
         netCDF_StatusHandler( status );
      }

      if( ( status = nc_inq_varid( netCDFid, "B1", &evl_x1_magnetic_field_B1_netCDFid ) ) != NC_NOERR )
      {  b1_exists = 0; fprintf( stderr,"\n%s !!!!! WARNING: Problem aquiring "
               "netCDFid for variable B1. NETCDF ERROR:", __FILE__ );
         netCDF_StatusHandler( status );
      }

      if( ( status = nc_inq_varid( netCDFid, "B2", &evl_x2_magnetic_field_B2_netCDFid ) ) != NC_NOERR )
      {  b2_exists = 0; fprintf( stderr,"\n%s !!!!! WARNING: Problem aquiring "
               "netCDFid for variable B2. NETCDF ERROR:", __FILE__ );
         netCDF_StatusHandler( status );
      }

      if( ( status = nc_inq_varid( netCDFid, "B3", &evl_x3_magnetic_field_B3_netCDFid )) != NC_NOERR )
      {  b3_exists = 0; fprintf( stderr,"\n%s !!!!! WARNING: Problem aquiring "
               "netCDFid for variable B3. NETCDF ERROR:", __FILE__ );
         netCDF_StatusHandler( status );
      }

      if( ( status = nc_inq_varid( netCDFid, "DP", &evl_cloud_mass_density_DP_netCDFid ) ) != NC_NOERR )
      {  dp_exists = 0; fprintf( stderr,"\n%s !!!!! WARNING: Problem aquiring "
               "netCDFid for variable DP. NETCDF ERROR:", __FILE__ );
         netCDF_StatusHandler( status );
      }

      if( ( status = nc_inq_varid( netCDFid, "BP", &evl_polarity_of_magnetic_field_BP_netCDFid ) ) != NC_NOERR )
      {
         bp_exists = 0; fprintf( stderr,"\n%s !!!!! WARNING: Problem aquiring "
               "netCDFid for variable BP. NETCDF ERROR:", __FILE__ );
         netCDF_StatusHandler( status );
      }

      /************************ call get_enlil_variable_attributes() function
       * for each variable if id is valid ********************/

      if( 1 )
      {
         status = get_enlil_variable_netcdf_attributes( netCDFid,
               evl_x1_position_X1_netCDFid, evl_x1_position_X1_long_name_vattr,
               evl_x1_position_X1_units_vattr );
      }

      if( 1 )
      {
         status = get_enlil_variable_netcdf_attributes( netCDFid,
               evl_x2_position_X2_netCDFid, evl_x2_position_X2_long_name_vattr,
               evl_x2_position_X2_units_vattr );
      }
      if( 1 )
      {
         status = get_enlil_variable_netcdf_attributes( netCDFid,
               evl_x3_position_X3_netCDFid, evl_x3_position_X3_long_name_vattr,
               evl_x3_position_X3_units_vattr );
      }

      if( time_exists )
      {
         status = get_enlil_variable_netcdf_attributes( netCDFid,
               evl_physical_time_TIME_netCDFid,
               evl_physical_time_TIME_long_name_vattr,
               evl_physical_time_TIME_units_vattr );
      }

      if( dt_exists )
      {  status = get_enlil_variable_netcdf_attributes( netCDFid,
               evl_physical_time_step_DT_netCDFid,
               evl_physical_time_step_DT_long_name_vattr,
               evl_physical_time_step_DT_units_vattr );
      }

      if( nstep_exists )
      {
         status = get_enlil_variable_netcdf_attributes( netCDFid,
               evl_numerical_time_step_NSTEP_netCDFid,
               evl_numerical_time_step_NSTEP_long_name_vattr,
               evl_numerical_time_step_NSTEP_units_vattr );
      }

      if( d_exists )
      {
         status = get_enlil_variable_netcdf_attributes( netCDFid,
               evl_density_D_netCDFid, evl_density_D_long_name_vattr,
               evl_density_D_units_vattr );
      }

      if( t_exists )
      {  status = get_enlil_variable_netcdf_attributes( netCDFid,
               evl_temperature_T_netCDFid,evl_temperature_T_long_name_vattr,
               evl_temperature_T_units_vattr );
      }

      if( v1_exists )
      {
         status = get_enlil_variable_netcdf_attributes( netCDFid,
               evl_x1_velocity_V1_netCDFid,evl_x1_velocity_V1_long_name_vattr,
               evl_x1_velocity_V1_units_vattr );
      }

      if( v2_exists )
      {
         status = get_enlil_variable_netcdf_attributes( netCDFid,
               evl_x2_velocity_V2_netCDFid,evl_x2_velocity_V2_long_name_vattr,
               evl_x2_velocity_V2_units_vattr );
      }

      if( v3_exists )
      {  status = get_enlil_variable_netcdf_attributes( netCDFid,
               evl_x3_velocity_V3_netCDFid,evl_x3_velocity_V3_long_name_vattr,
               evl_x3_velocity_V3_units_vattr );
      }

      if( b1_exists )
      {  status = get_enlil_variable_netcdf_attributes( netCDFid,
               evl_x1_magnetic_field_B1_netCDFid,
               evl_x1_magnetic_field_B1_long_name_vattr,
               evl_x1_magnetic_field_B1_units_vattr );
      }

      if( b2_exists )
      {  status = get_enlil_variable_netcdf_attributes( netCDFid,
               evl_x2_magnetic_field_B2_netCDFid,
               evl_x2_magnetic_field_B2_long_name_vattr,
               evl_x2_magnetic_field_B2_units_vattr );
      }

      if( b3_exists )
      {
         status = get_enlil_variable_netcdf_attributes( netCDFid,
               evl_x3_magnetic_field_B3_netCDFid,
               evl_x3_magnetic_field_B3_long_name_vattr,
               evl_x3_magnetic_field_B3_units_vattr );
      }

      if( dp_exists )
      {
         status = get_enlil_variable_netcdf_attributes( netCDFid,
               evl_cloud_mass_density_DP_netCDFid,
               evl_cloud_mass_density_DP_long_name_vattr,
               evl_cloud_mass_density_DP_units_vattr );
      }

      if( bp_exists )
      {
         status = get_enlil_variable_netcdf_attributes( netCDFid,
               evl_polarity_of_magnetic_field_BP_netCDFid,
               evl_polarity_of_magnetic_field_BP_long_name_vattr,
               evl_polarity_of_magnetic_field_BP_units_vattr );
      }

      /* dynamically allocate memory and then get the actual variable values */

      if( DEBUG_FLAG ) printf("allocating mem\n");

      /****** allocate memory for position variables X1,X2,X3 *******/

      /**** NOTE: the size of x1,x2,x3 for evl = nevo * nsel ****/

      grid_size = evl_nsel * evl_nevo;

      /************ POSITION VARIABLES ************/

      if ( ( evl_x1_position_X1 = calloc( grid_size, sizeof(float) ) ) == NULL )
      {
         printf("MALLOC for evl_x1_position_X1 array FAILED\n");
         exit(EXIT_FAILURE);
      }
      if ( ( evl_x2_position_X2 = calloc( grid_size, sizeof(float) ) ) == NULL )
      {
         printf("MALLOC for evl_x2_position_X2 array FAILED\n");
         exit(EXIT_FAILURE);
      }
      if ( ( evl_x3_position_X3 = calloc( grid_size, sizeof(float) ) ) == NULL )
      {
         printf("MALLOC for evl_x3h_position_X3 array FAILED\n");
         exit(EXIT_FAILURE);
      }

      /***** variables time, dt & nstep *****/

      if ( ( evl_physical_time_TIME = calloc( evl_nevo , sizeof(float) ) ) == NULL )
      {
         printf("MALLOC for evl_physical_time_TIME FAILED\n");
         exit(EXIT_FAILURE);
      }
      if ( ( evl_physical_time_step_DT = calloc( evl_nevo , sizeof(float) ) ) == NULL )
      {
         printf("MALLOC for evl_physical_time_step_DT FAILED\n");
         exit(EXIT_FAILURE);
      }
      if ( ( evl_numerical_time_step_NSTEP = calloc( evl_nevo , sizeof(int) ) ) == NULL )
      {
         printf("MALLOC for evl_numerical_time_step_NSTEP FAILED\n");
         exit(EXIT_FAILURE);
      }

      /************ DATA VARIABLES ************/

      /** only allocate memory if variable exists **/

      if( d_exists )
      {
         if ( ( evl_density_D = calloc( grid_size, sizeof(float) ) ) == NULL )
         {
            printf("MALLOC for evl_density_D array FAILED\n");
            exit(EXIT_FAILURE);
         }
      }
      if( t_exists )
      {
         if ( ( evl_temperature_T = calloc( grid_size, sizeof(float) ) ) == NULL )
         {
            printf("MALLOC for evl_temperature_T array FAILED\n");
            exit(EXIT_FAILURE);
         }
      }
      if( v1_exists )
      {
         if ( ( evl_x1_velocity_V1 = calloc( grid_size, sizeof(float) ) ) == NULL )
         {
            printf("MALLOC for evl_x1_velocity_V1 array FAILED\n");
            exit(EXIT_FAILURE);
         }
      }
      if( v2_exists )
      {
         if ( ( evl_x2_velocity_V2 = calloc( grid_size, sizeof(float) ) ) == NULL )
         {
            printf("MALLOC for evl_x2_velocity_V2 array FAILED\n");
            exit(EXIT_FAILURE);
         }
      }
      if( v3_exists )
      {
         if ( ( evl_x3_velocity_V3 = calloc( grid_size, sizeof(float) ) ) == NULL )
         {
            printf("MALLOC for evl_x3_velocity_V3 array FAILED\n");
            exit(EXIT_FAILURE);
         }
      }
      if( b1_exists )
      {
         if ( ( evl_x1_magnetic_field_B1 = calloc( grid_size, sizeof(float) ) ) == NULL )
         {
            printf("MALLOC for evl_x1_velocity_B1 array FAILED\n");
            exit(EXIT_FAILURE);
         }
      }
      if( b2_exists )
      {
         if ( ( evl_x2_magnetic_field_B2 = calloc( grid_size, sizeof(float) ) ) == NULL )
         {
            printf("MALLOC for evl_x2_magnetic_field_B2 array FAILED\n");
            exit(EXIT_FAILURE);
         }
      }
      if( b3_exists )
      {
         if ( ( evl_x3_magnetic_field_B3 = calloc( grid_size, sizeof(float) ) ) == NULL )
         {
            printf("MALLOC for evl_x3_magnetic_field_B3 array FAILED\n");
            exit(EXIT_FAILURE);
         }
      }
      if( dp_exists )
      {
         if ( ( evl_cloud_mass_density_DP = calloc( grid_size, sizeof(float) ) ) == NULL )
         {
            printf("MALLOC for evl_cloud_mass_density_DP array FAILED\n");
            exit(EXIT_FAILURE);
         }
      }
      if( bp_exists )
      {
         if ( ( evl_polarity_of_magnetic_field_BP = calloc( grid_size, sizeof(float) ) ) == NULL )
         {
            printf("MALLOC for evl_polarity_of_magnetic_field_BP array FAILED\n");
            exit(EXIT_FAILURE);
         }
      }

      if( DEBUG_FLAG ) printf("reading files\n");

      /******** now get actual position values from netCDF file *************/

      if( nc_get_var_float( netCDFid, evl_x1_position_X1_netCDFid, evl_x1_position_X1 ) != NC_NOERR )
      {
         netCDF_StatusHandler( status );
         return EXIT_FAILURE;
      }

      if( nc_get_var_float( netCDFid, evl_x2_position_X2_netCDFid, evl_x2_position_X2 ) != NC_NOERR )
      {
         netCDF_StatusHandler( status ); return EXIT_FAILURE;
      }

      if( nc_get_var_float( netCDFid, evl_x3_position_X3_netCDFid, evl_x3_position_X3 ) != NC_NOERR )
      {
         netCDF_StatusHandler( status );
         return EXIT_FAILURE;
      }

      if( DEBUG_FLAG ) printf("DEBUG\tmain grid size = [%d,%d]\n", (int ) evl_nevo, (int ) evl_nsel );

      /*
       for( i = 0; i < evl_nsel ; i++ )
       {
       printf(" cell_%d_position[%f,%f,%f]\n", i, evl_x1_position_X1[i], evl_x2_position_X2[i], evl_x3_position_X3[i] );
       }
       */
      /*********** DATA VALUES ********/

      if( d_exists )
      {
         if( nc_get_var_float( netCDFid, evl_density_D_netCDFid, evl_density_D ) != NC_NOERR )
         {
            netCDF_StatusHandler( status );
            return EXIT_FAILURE;
         }
      }
      if( t_exists )
      {
         if( nc_get_var_float( netCDFid, evl_temperature_T_netCDFid, evl_temperature_T ) != NC_NOERR )
         {
            netCDF_StatusHandler( status );
            return EXIT_FAILURE;
         }
      }
      if( v1_exists )
      {
         if( nc_get_var_float( netCDFid, evl_x1_velocity_V1_netCDFid, evl_x1_velocity_V1 ) != NC_NOERR )
         {
            netCDF_StatusHandler( status );
            return EXIT_FAILURE;
         }
      }
      if( v2_exists )
      {
         if( nc_get_var_float( netCDFid, evl_x2_velocity_V2_netCDFid, evl_x2_velocity_V2 ) != NC_NOERR )
         {
            netCDF_StatusHandler( status );
            return EXIT_FAILURE;
         }
      }
      if( v3_exists )
      {
         if( nc_get_var_float( netCDFid, evl_x3_velocity_V3_netCDFid, evl_x3_velocity_V3 ) != NC_NOERR )
         {
            netCDF_StatusHandler( status );
            return EXIT_FAILURE;
         }
      }
      if( b1_exists )
      {
         if( nc_get_var_float( netCDFid, evl_x1_magnetic_field_B1_netCDFid, evl_x1_magnetic_field_B1 ) != NC_NOERR )
         {
            netCDF_StatusHandler( status );
            return EXIT_FAILURE;
         }
      }
      if( b2_exists )
      {
         if( nc_get_var_float( netCDFid, evl_x2_magnetic_field_B2_netCDFid, evl_x2_magnetic_field_B2 ) != NC_NOERR )
         {
            netCDF_StatusHandler( status );
            return EXIT_FAILURE;
         }
      }
      if( b3_exists )
      {
         if( nc_get_var_float( netCDFid, evl_x3_magnetic_field_B3_netCDFid, evl_x3_magnetic_field_B3 ) != NC_NOERR )
         {
            netCDF_StatusHandler( status );
            return EXIT_FAILURE;
         }
      }
      if( dp_exists )
      {
         if( nc_get_var_float( netCDFid, evl_cloud_mass_density_DP_netCDFid, evl_cloud_mass_density_DP ) != NC_NOERR )
         {
            netCDF_StatusHandler( status );
            return EXIT_FAILURE;
         }
      }
      if( bp_exists )
      {
         if( nc_get_var_float( netCDFid, evl_polarity_of_magnetic_field_BP_netCDFid, evl_polarity_of_magnetic_field_BP ) != NC_NOERR )
         {
            netCDF_StatusHandler( status );
            return EXIT_FAILURE;
         }
      }
      /*
       for( i = 0; i < grid_size ; i+=1635 )
       {
       printf(" evl_polarity_of_magnetic_field_BP[%d] = %f\n",i, evl_polarity_of_magnetic_field_BP[i] );
       }
       */
      if( DEBUG_FLAG ) printf("DEBUG aquiring three scalar variables for time, dt, nstep...\n");

      /******* Now get the three time variables for time, dt, nstep **************/

      if( nc_get_var_float( netCDFid, evl_physical_time_TIME_netCDFid, evl_physical_time_TIME ) != NC_NOERR )
      {
         netCDF_StatusHandler( status );
         return EXIT_FAILURE;
      }

      if( nc_get_var_float( netCDFid, evl_physical_time_step_DT_netCDFid, evl_physical_time_step_DT ) != NC_NOERR )
      {
         netCDF_StatusHandler( status );
         return EXIT_FAILURE;
      }

      if( nc_get_var_int( netCDFid, evl_numerical_time_step_NSTEP_netCDFid, evl_numerical_time_step_NSTEP ) != NC_NOERR )
      {
         netCDF_StatusHandler( status );
         return EXIT_FAILURE;
      }

      if( DEBUG_FLAG ) printf("DEBUG finished aquiring three scalar variables "
            "for time, dt, nstep...\n");

      /********* close the currently open netCDF file **************/

      if( DEBUG_FLAG ) printf("DEBUG\tclosing currently open netCDF file with "
            "id = %d\n", netCDFid);

      nc_close( netCDFid );

      if( DEBUG_FLAG ) printf("DEBUG\tfinished closing currently open netCDF "
            "file with id = %d\n", netCDFid);

#endif

      return EXIT_SUCCESS;
   }

   /**************************************
    *             READ ENLIL EVG            *
    **************************************/

int read_enlil_evg_file(char *input_file_name, int verbose_flag)
{
#ifdef NETCDF

   int i, grid_size;
   int status;

   /*** create var_exists variable to determine if a variable needs to be allocated and read or not ***/

   int time_exists = 1, dt_exists = 1, nstep_exists = 1, d_exists = 1,
         t_exists = 1, v1_exists = 1, v2_exists = 1, v3_exists = 1, b1_exists =
               1, b2_exists = 1, b3_exists = 1, dp_exists = 1, bp_exists = 1;

   /******* variable for netCDF specific calls *********/

   int netCDFid;

   int file_open_mode= NC_NOWRITE;

   if (verbose_flag)
   {
      printf("...reading %s\n", input_file_name);
   }

   /******** open evg.nc netCDF file *****/

   if ( DEBUG_FLAG)
      printf("calling nc_open\n");
   if ( DEBUG_FLAG)
      printf("input_file_name = %s\n", input_file_name);
   if ( DEBUG_FLAG)
      printf("file_open_mode = %d\n", file_open_mode);
   if ( DEBUG_FLAG)
      printf("netCDFid = %d\n", netCDFid);
   status = nc_open(input_file_name, file_open_mode, &netCDFid);

   if ( DEBUG_FLAG)
      printf("netCDFid = %d\n", netCDFid);

   if ( DEBUG_FLAG)
      printf("back from nc_open\n");

   /*** if netCDF file was not opened properly, return contron to calling
    * program with EXIT_FAILUE **/

   if (status != NC_NOERR)
   {
      netCDF_StatusHandler(status);
      return EXIT_FAILURE;
   }

   /* call get_enlil_global_attribute() function */

   if ( DEBUG_FLAG)
      printf("calling get_enlil_global_netcdf_attributes for evg file\n");

   status = get_enlil_global_netcdf_attributes(
         netCDFid,
         &evg_gattribute_type,
         &evg_gattribute_title,
         &evg_gattribute_name,
         &evg_gattribute_project,
         &evg_gattribute_initial,
         &evg_gattribute_resume,
         &evg_gattribute_boundary,
         &evg_gattribute_passage,
         &evg_gattribute_grid,
         &evg_gattribute_geometry,
         &evg_gattribute_code,
         &evg_gattribute_parameters,
         &evg_gattribute_refdate_mjd,
         &evg_gattribute_gamma,
         &evg_gattribute_xalpha,
         &evg_gattribute_history);

   if ( DEBUG_FLAG)
      printf(
            "DEBUG\t%s\treturn status from get_enlil_global_attributes = %d\n",
            __FILE__, status );

            if( DEBUG_FLAG ) printf("getting id's\n");

            /* get dimensionid'sand sizes from evg.nc file */

      if( ( status = nc_inq_dimid( netCDFid, "nevo", &evg_nevo_netCDFid ) ) != NC_NOERR )
      {
         netCDF_StatusHandler( status );
         return EXIT_FAILURE;
      }

      if( ( status = nc_inq_dimid( netCDFid, "ngeo", &evg_ngeo_netCDFid ) ) != NC_NOERR )
      {
         netCDF_StatusHandler( status );
         return EXIT_FAILURE;
      }

      if( ( status = nc_inq_dimlen( netCDFid, evg_nevo_netCDFid, &evg_nevo ) ) != NC_NOERR )
      {
         netCDF_StatusHandler( status );
         return EXIT_FAILURE;
      }
      if( ( status = nc_inq_dimlen( netCDFid, evg_ngeo_netCDFid, &evg_ngeo ) ) != NC_NOERR )
      {
         netCDF_StatusHandler( status );
         return EXIT_FAILURE;
      }

      /******* get all of the variable id numbers from the evg.nc file ********/

      if( ( status = nc_inq_varid( netCDFid, "X1", &evg_x1_position_X1_netCDFid ) ) != NC_NOERR )
      {
         netCDF_StatusHandler( status );
         return EXIT_FAILURE;
      }
      if( ( status = nc_inq_varid( netCDFid, "X2", &evg_x2_position_X2_netCDFid ) ) != NC_NOERR )
      {
         netCDF_StatusHandler( status );
         return EXIT_FAILURE;
      }
      if( ( status = nc_inq_varid( netCDFid, "X3", &evg_x3_position_X3_netCDFid ) ) != NC_NOERR )
      {
         netCDF_StatusHandler( status );
         return EXIT_FAILURE;
      }

      /*** we don't want to die if a regular variable is not found, so don't return control ***/
      /*** doing this primarily because my development test data does not contain dp cloud mass density variable
       * but we should not abandon program beacause of this - plus it's a good idea anyway - just print a warning **/

      if( ( status = nc_inq_varid( netCDFid, "TIME", &evg_physical_time_TIME_netCDFid ) ) != NC_NOERR )
      {
         time_exists = 0; fprintf( stderr, "\n%s !!!!! WARNING: Problem aquiring "
               "netCDFid for variable TIME. NETCDF ERROR:", __FILE__ );
         netCDF_StatusHandler( status );
      }

      if( ( status = nc_inq_varid( netCDFid, "DT", &evg_physical_time_step_DT_netCDFid ) ) != NC_NOERR )
      {
         dt_exists = 0; fprintf( stderr, "\n%s !!!!! WARNING: Problem aquiring "
               "netCDFid for variable DT. NETCDF ERROR:", __FILE__ );
         netCDF_StatusHandler( status );
      }

      if( ( status = nc_inq_varid( netCDFid, "NSTEP", &evg_numerical_time_step_NSTEP_netCDFid ) ) != NC_NOERR )
      {
         nstep_exists = 0; fprintf( stderr, "\n%s !!!!! WARNING: Problem "
               "aquiring netCDFid for variable NSTEP. NETCDF ERROR:", __FILE__ );
         netCDF_StatusHandler( status );
      }

      if( ( status = nc_inq_varid( netCDFid, "D", &evg_density_D_netCDFid ) ) != NC_NOERR )
      {
         d_exists = 0; fprintf( stderr, "\n%s !!!!! WARNING: Problem aquiring "
               "netCDFid for variable D. NETCDF ERROR:", __FILE__ );
         netCDF_StatusHandler( status );
      }

      if( ( status = nc_inq_varid( netCDFid, "T", &evg_temperature_T_netCDFid ) ) != NC_NOERR )
      {  t_exists = 0; fprintf( stderr,"\n%s !!!!! WARNING: Problem aquiring "
               "netCDFid for variable T. NETCDF ERROR:", __FILE__ );
         netCDF_StatusHandler( status );
      }

      if( ( status = nc_inq_varid( netCDFid, "V1", &evg_x1_velocity_V1_netCDFid ) ) != NC_NOERR )
      {
         v1_exists = 0; fprintf( stderr,"\n%s !!!!! WARNING: Problem aquiring "
               "netCDFid for variable V1. NETCDF ERROR:", __FILE__ );
         netCDF_StatusHandler( status );
      }

      if( ( status = nc_inq_varid( netCDFid, "V2", &evg_x2_velocity_V2_netCDFid ) ) != NC_NOERR )
      {
         v2_exists = 0; fprintf( stderr,"\n%s !!!!! WARNING: Problem aquiring "
               "netCDFid for variable V2. NETCDF ERROR:", __FILE__ );
         netCDF_StatusHandler( status );
      }

      if( ( status = nc_inq_varid( netCDFid, "V3", &evg_x3_velocity_V3_netCDFid ) ) != NC_NOERR )
      {
         v3_exists = 0; fprintf( stderr,"\n%s !!!!! WARNING: Problem aquiring "
               "netCDFid for variable V3. NETCDF ERROR:", __FILE__ );
         netCDF_StatusHandler( status );
      }

      if( ( status = nc_inq_varid( netCDFid, "B1", &evg_x1_magnetic_field_B1_netCDFid ) ) != NC_NOERR )
      {  b1_exists = 0; fprintf( stderr,"\n%s !!!!! WARNING: Problem aquiring "
               "netCDFid for variable B1. NETCDF ERROR:", __FILE__ );
         netCDF_StatusHandler( status );
      }

      if( ( status = nc_inq_varid( netCDFid, "B2", &evg_x2_magnetic_field_B2_netCDFid ) ) != NC_NOERR )
      {
         b2_exists = 0; fprintf( stderr,"\n%s !!!!! WARNING: Problem aquiring "
               "netCDFid for variable B2. NETCDF ERROR:", __FILE__ );
         netCDF_StatusHandler( status );
      }

      if( ( status = nc_inq_varid( netCDFid, "B3", &evg_x3_magnetic_field_B3_netCDFid )) != NC_NOERR )
      {  b3_exists = 0; fprintf( stderr,"\n%s !!!!! WARNING: Problem aquiring "
               "netCDFid for variable B3. NETCDF ERROR:", __FILE__ );
         netCDF_StatusHandler( status );
      }

      if( ( status = nc_inq_varid( netCDFid, "DP", &evg_cloud_mass_density_DP_netCDFid ) ) != NC_NOERR )
      {
         dp_exists = 0; fprintf( stderr,"\n%s !!!!! WARNING: Problem aquiring "
               "netCDFid for variable DP. NETCDF ERROR:", __FILE__ );
         netCDF_StatusHandler( status );
      }

      if( ( status = nc_inq_varid( netCDFid, "BP", &evg_polarity_of_magnetic_field_BP_netCDFid ) ) != NC_NOERR )
      {
         bp_exists = 0; fprintf( stderr,"\n%s !!!!! WARNING: Problem aquiring "
               "netCDFid for variable BP. NETCDF ERROR:", __FILE__ );
         netCDF_StatusHandler( status );
      }

      /************************ call get_enlil_variable_attributes() function for each variable if id is valid ********************/

      if( 1 )
      {
         status = get_enlil_variable_netcdf_attributes( netCDFid,
               evg_x1_position_X1_netCDFid, evg_x1_position_X1_long_name_vattr,
               evg_x1_position_X1_units_vattr );
      }

      if( 1 )
      {
         status = get_enlil_variable_netcdf_attributes( netCDFid,
               evg_x2_position_X2_netCDFid, evg_x2_position_X2_long_name_vattr,
               evg_x2_position_X2_units_vattr );
      }

      if( 1 )
      {
         status = get_enlil_variable_netcdf_attributes( netCDFid,
               evg_x3_position_X3_netCDFid, evg_x3_position_X3_long_name_vattr,
               evg_x3_position_X3_units_vattr );
      }

      if( time_exists )
      {
         status = get_enlil_variable_netcdf_attributes( netCDFid,
               evg_physical_time_TIME_netCDFid,
               evg_physical_time_TIME_long_name_vattr,
               evg_physical_time_TIME_units_vattr );
      }

      if( dt_exists )
      {
         status = get_enlil_variable_netcdf_attributes( netCDFid,
               evg_physical_time_step_DT_netCDFid,
               evg_physical_time_step_DT_long_name_vattr,
               evg_physical_time_step_DT_units_vattr );
      }

      if( nstep_exists )
      {
         status = get_enlil_variable_netcdf_attributes( netCDFid,
               evg_numerical_time_step_NSTEP_netCDFid,
               evg_numerical_time_step_NSTEP_long_name_vattr,
               evg_numerical_time_step_NSTEP_units_vattr );
      }

      if( d_exists )
      {  status = get_enlil_variable_netcdf_attributes( netCDFid,
               evg_density_D_netCDFid, evg_density_D_long_name_vattr,
               evg_density_D_units_vattr );
      }

      if( t_exists )
      {  status = get_enlil_variable_netcdf_attributes( netCDFid,
               evg_temperature_T_netCDFid,evg_temperature_T_long_name_vattr,
               evg_temperature_T_units_vattr );
      }

      if( v1_exists )
      {  status = get_enlil_variable_netcdf_attributes( netCDFid,
               evg_x1_velocity_V1_netCDFid,evg_x1_velocity_V1_long_name_vattr,
               evg_x1_velocity_V1_units_vattr );
      }

      if( v2_exists )
      {  status = get_enlil_variable_netcdf_attributes( netCDFid,
               evg_x2_velocity_V2_netCDFid,evg_x2_velocity_V2_long_name_vattr,
               evg_x2_velocity_V2_units_vattr );
      }

      if( v3_exists )
      {  status = get_enlil_variable_netcdf_attributes( netCDFid,
               evg_x3_velocity_V3_netCDFid,evg_x3_velocity_V3_long_name_vattr,
               evg_x3_velocity_V3_units_vattr );
      }

      if( b1_exists )
      {
         status = get_enlil_variable_netcdf_attributes( netCDFid,
               evg_x1_magnetic_field_B1_netCDFid,
               evg_x1_magnetic_field_B1_long_name_vattr,
               evg_x1_magnetic_field_B1_units_vattr );
      }

      if( b2_exists )
      {  status = get_enlil_variable_netcdf_attributes( netCDFid,
               evg_x2_magnetic_field_B2_netCDFid,
               evg_x2_magnetic_field_B2_long_name_vattr,
               evg_x2_magnetic_field_B2_units_vattr );
      }

      if( b3_exists )
      {
         status = get_enlil_variable_netcdf_attributes( netCDFid,
               evg_x3_magnetic_field_B3_netCDFid,
               evg_x3_magnetic_field_B3_long_name_vattr,
               evg_x3_magnetic_field_B3_units_vattr );
      }

      if( dp_exists )
      {
         status = get_enlil_variable_netcdf_attributes( netCDFid,
               evg_cloud_mass_density_DP_netCDFid,
               evg_cloud_mass_density_DP_long_name_vattr,
               evg_cloud_mass_density_DP_units_vattr );
      }

      if( bp_exists )
      {
         status = get_enlil_variable_netcdf_attributes( netCDFid,
               evg_polarity_of_magnetic_field_BP_netCDFid,
               evg_polarity_of_magnetic_field_BP_long_name_vattr,
               evg_polarity_of_magnetic_field_BP_units_vattr );
      }

      /* dynamically allocate memory and then get the actual variable values */

      if( DEBUG_FLAG ) printf("allocating mem\n");

      /****** allocate memory for position variables X1,X2,X3 *******/

      /**** NOTE: the size of x1,x2,x3 for evg = nevo * ngeo ******/

      grid_size = evg_ngeo * evg_nevo;

      /************ POSITION VARIABLES ************/

      if ( ( evg_x1_position_X1 = calloc( grid_size, sizeof(float) ) ) == NULL )
      {
         printf("MALLOC for evg_x1_position_X1 array FAILED\n");
         exit(EXIT_FAILURE);
      }
      if ( ( evg_x2_position_X2 = calloc( grid_size, sizeof(float) ) ) == NULL )
      {
         printf("MALLOC for evg_x2_position_X2 array FAILED\n");
         exit(EXIT_FAILURE);
      }
      if ( ( evg_x3_position_X3 = calloc( grid_size, sizeof(float) ) ) == NULL )
      {
         printf("MALLOC for evg_x3h_position_X3 array FAILED\n");
         exit(EXIT_FAILURE);
      }

      /***** variables time, dt & nstep *****/

      if ( ( evg_physical_time_TIME = calloc( evg_nevo , sizeof(float) ) ) == NULL )
      {
         printf("MALLOC for evg_physical_time_TIME FAILED\n");
         exit(EXIT_FAILURE);
      }
      if ( ( evg_physical_time_step_DT = calloc( evg_nevo , sizeof(float) ) ) == NULL )
      {
         printf("MALLOC for evg_physical_time_step_DT FAILED\n");
         exit(EXIT_FAILURE);
      }
      if ( ( evg_numerical_time_step_NSTEP = calloc( evg_nevo , sizeof(int) ) ) == NULL )
      {
         printf("MALLOC for evg_numerical_time_step_NSTEP FAILED\n");
         exit(EXIT_FAILURE);
      }

      /************ DATA VARIABLES ************/

      /** only allocate memory if variable exists **/

      if( d_exists )
      {
         if ( ( evg_density_D = calloc( grid_size, sizeof(float) ) ) == NULL )
         {
            printf("MALLOC for evg_density_D array FAILED\n");
            exit(EXIT_FAILURE);
         }
      }
      if( t_exists )
      {
         if ( ( evg_temperature_T = calloc( grid_size, sizeof(float) ) ) == NULL )
         {
            printf("MALLOC for evg_temperature_T array FAILED\n");
            exit(EXIT_FAILURE);
         }
      }
      if( v1_exists )
      {
         if ( ( evg_x1_velocity_V1 = calloc( grid_size, sizeof(float) ) ) == NULL )
         {
            printf("MALLOC for evg_x1_velocity_V1 array FAILED\n");
            exit(EXIT_FAILURE);
         }
      }
      if( v2_exists )
      {
         if ( ( evg_x2_velocity_V2 = calloc( grid_size, sizeof(float) ) ) == NULL )
         {
            printf("MALLOC for evg_x2_velocity_V2 array FAILED\n");
            exit(EXIT_FAILURE);
         }
      }
      if( v3_exists )
      {
         if ( ( evg_x3_velocity_V3 = calloc( grid_size, sizeof(float) ) ) == NULL )
         {
            printf("MALLOC for evg_x3_velocity_V3 array FAILED\n");
            exit(EXIT_FAILURE);
         }
      }
      if( b1_exists )
      {
         if ( ( evg_x1_magnetic_field_B1 = calloc( grid_size, sizeof(float) ) ) == NULL )
         {
            printf("MALLOC for evg_x1_velocity_B1 array FAILED\n");
            exit(EXIT_FAILURE);
         }
      }
      if( b2_exists )
      {
         if ( ( evg_x2_magnetic_field_B2 = calloc( grid_size, sizeof(float) ) ) == NULL )
         {
            printf("MALLOC for evg_x2_magnetic_field_B2 array FAILED\n");
            exit(EXIT_FAILURE);
         }
      }
      if( b3_exists )
      {
         if ( ( evg_x3_magnetic_field_B3 = calloc( grid_size, sizeof(float) ) ) == NULL )
         {
            printf("MALLOC for evg_x3_magnetic_field_B3 array FAILED\n");
            exit(EXIT_FAILURE);
         }
      }
      if( dp_exists )
      {
         if ( ( evg_cloud_mass_density_DP = calloc( grid_size, sizeof(float) ) ) == NULL )
         {
            printf("MALLOC for evg_cloud_mass_density_DP array FAILED\n");
            exit(EXIT_FAILURE);
         }
      }
      if( bp_exists )
      {
         if ( ( evg_polarity_of_magnetic_field_BP = calloc( grid_size, sizeof(float) ) ) == NULL )
         {
            printf("MALLOC for evg_polarity_of_magnetic_field_BP array FAILED\n");
            exit(EXIT_FAILURE);
         }
      }

      if( DEBUG_FLAG ) printf("reading files\n");

      /******** now get actual position values from netCDF file *************/

      if( nc_get_var_float( netCDFid, evg_x1_position_X1_netCDFid, evg_x1_position_X1 ) != NC_NOERR )
      {  netCDF_StatusHandler( status ); return EXIT_FAILURE;}
      if( nc_get_var_float( netCDFid, evg_x2_position_X2_netCDFid, evg_x2_position_X2 ) != NC_NOERR )
      {  netCDF_StatusHandler( status ); return EXIT_FAILURE;}
      if( nc_get_var_float( netCDFid, evg_x3_position_X3_netCDFid, evg_x3_position_X3 ) != NC_NOERR )
      {  netCDF_StatusHandler( status ); return EXIT_FAILURE;}

      if( DEBUG_FLAG ) printf("DEBUG\tmain grid size = [%d,%d]\n", (int ) evg_nevo, (int ) evg_ngeo );

      /*
       for( i = 0; i < evg_ngeo ; i++ )
       {
       printf(" cell_%d_position[%f,%f,%f]\n", i, evg_x1_position_X1[i], evg_x2_position_X2[i], evg_x3_position_X3[i] );
       }
       */

      /*********** DATA VALUES ********/

      if( d_exists )
      {
         if( nc_get_var_float( netCDFid, evg_density_D_netCDFid, evg_density_D ) != NC_NOERR )
         {  netCDF_StatusHandler( status ); return EXIT_FAILURE;}
      }
      if( t_exists )
      {
         if( nc_get_var_float( netCDFid, evg_temperature_T_netCDFid, evg_temperature_T ) != NC_NOERR )
         {  netCDF_StatusHandler( status ); return EXIT_FAILURE;}
      }
      if( v1_exists )
      {
         if( nc_get_var_float( netCDFid, evg_x1_velocity_V1_netCDFid, evg_x1_velocity_V1 ) != NC_NOERR )
         {  netCDF_StatusHandler( status ); return EXIT_FAILURE;}
      }
      if( v2_exists )
      {
         if( nc_get_var_float( netCDFid, evg_x2_velocity_V2_netCDFid, evg_x2_velocity_V2 ) != NC_NOERR )
         {  netCDF_StatusHandler( status ); return EXIT_FAILURE;}
      }
      if( v3_exists )
      {
         if( nc_get_var_float( netCDFid, evg_x3_velocity_V3_netCDFid, evg_x3_velocity_V3 ) != NC_NOERR )
         {  netCDF_StatusHandler( status ); return EXIT_FAILURE;}
      }
      if( b1_exists )
      {
         if( nc_get_var_float( netCDFid, evg_x1_magnetic_field_B1_netCDFid, evg_x1_magnetic_field_B1 ) != NC_NOERR )
         {  netCDF_StatusHandler( status ); return EXIT_FAILURE;}
      }
      if( b2_exists )
      {
         if( nc_get_var_float( netCDFid, evg_x2_magnetic_field_B2_netCDFid, evg_x2_magnetic_field_B2 ) != NC_NOERR )
         {  netCDF_StatusHandler( status ); return EXIT_FAILURE;}
      }
      if( b3_exists )
      {
         if( nc_get_var_float( netCDFid, evg_x3_magnetic_field_B3_netCDFid, evg_x3_magnetic_field_B3 ) != NC_NOERR )
         {  netCDF_StatusHandler( status ); return EXIT_FAILURE;}
      }
      if( dp_exists )
      {
         if( nc_get_var_float( netCDFid, evg_cloud_mass_density_DP_netCDFid, evg_cloud_mass_density_DP ) != NC_NOERR )
         {  netCDF_StatusHandler( status ); return EXIT_FAILURE;}
      }
      if( bp_exists )
      {
         if( nc_get_var_float( netCDFid, evg_polarity_of_magnetic_field_BP_netCDFid, evg_polarity_of_magnetic_field_BP ) != NC_NOERR )
         {  netCDF_StatusHandler( status ); return EXIT_FAILURE;}
      }

      /*
       for( i = 0; i < grid_size ; i+=1635 )
       {
       printf(" evg_polarity_of_magnetic_field_BP[%d] = %f\n",i, evg_polarity_of_magnetic_field_BP[i] );
       }
       */

      /******* Now get the three time variables for time, dt, nstep **************/

      if( nc_get_var_float( netCDFid, evg_physical_time_TIME_netCDFid, evg_physical_time_TIME ) != NC_NOERR )
      {  netCDF_StatusHandler( status ); return EXIT_FAILURE;}
      if( nc_get_var_float( netCDFid, evg_physical_time_step_DT_netCDFid, evg_physical_time_step_DT ) != NC_NOERR )
      {  netCDF_StatusHandler( status ); return EXIT_FAILURE;}
      if( nc_get_var_int( netCDFid, evg_numerical_time_step_NSTEP_netCDFid, evg_numerical_time_step_NSTEP ) != NC_NOERR )
      {  netCDF_StatusHandler( status ); return EXIT_FAILURE;}

      /********* close the currently open netCDF file **************/

      nc_close( netCDFid );

#endif

      return EXIT_SUCCESS;
   }

   /**************************************
    *             READ ENLIL EVP            *
    **************************************/

int read_enlil_evp_file(char *input_file_name, int verbose_flag)
{
#ifdef NETCDF

   int i, grid_size;
   int status;

   /*** create var_exists variable to determine if a variable needs to be allocated and read or not ***/

   int time_exists = 1, dt_exists = 1, nstep_exists = 1, d_exists = 1,
         t_exists = 1, v1_exists = 1, v2_exists = 1, v3_exists = 1, b1_exists =
               1, b2_exists = 1, b3_exists = 1, dp_exists = 1, bp_exists = 1;

   /******* variable for netCDF specific calls *********/

   int netCDFid;

   int file_open_mode= NC_NOWRITE;

   if (verbose_flag)
   {
      printf("...reading %s\n", input_file_name);
   }

   /******** open evp.nc netCDF file *****/

   if ( DEBUG_FLAG)
      printf("calling nc_open\n");

   status = nc_open(input_file_name, file_open_mode, &netCDFid);

   if ( DEBUG_FLAG)
      printf("netCDFid = %d\n", netCDFid);

   /*** if netCDF file was not opened properly, return contron to calling program with EXIT_FAILUE **/

   if (status != NC_NOERR)
   {
      netCDF_StatusHandler(status);
      return EXIT_FAILURE;
   }

   status = get_enlil_global_netcdf_attributes(
         netCDFid,
         &evp_gattribute_type,
         &evp_gattribute_title,
         &evp_gattribute_name,
         &evp_gattribute_project,
         &evp_gattribute_initial,
         &evp_gattribute_resume,
         &evp_gattribute_boundary,
         &evp_gattribute_passage,
         &evp_gattribute_grid,
         &evp_gattribute_geometry,
         &evp_gattribute_code,
         &evp_gattribute_parameters,
         &evp_gattribute_refdate_mjd,
         &evp_gattribute_gamma,
         &evp_gattribute_xalpha,
         &evp_gattribute_history);
   if ( DEBUG_FLAG)
      printf(
            "DEBUG\t%s\treturn status from get_enlil_global_attributes = %d\n",
            __FILE__, status );

            if( DEBUG_FLAG ) printf("getting id's\n");

            /* get dimensionid's and sizes from evp.nc file */

      if( ( status = nc_inq_dimid( netCDFid, "nevo", &evp_nevo_netCDFid ) ) != NC_NOERR )
      {  netCDF_StatusHandler( status ); return EXIT_FAILURE;}
      if( ( status = nc_inq_dimid( netCDFid, "npla", &evp_npla_netCDFid ) ) != NC_NOERR )
      {  netCDF_StatusHandler( status ); return EXIT_FAILURE;}

      if( ( status = nc_inq_dimlen( netCDFid, evp_nevo_netCDFid, &evp_nevo ) ) != NC_NOERR )
      {  netCDF_StatusHandler( status ); return EXIT_FAILURE;}
      if( ( status = nc_inq_dimlen( netCDFid, evp_npla_netCDFid, &evp_npla ) ) != NC_NOERR )
      {  netCDF_StatusHandler( status ); return EXIT_FAILURE;}

      /******* get all of the variable id numbers from the evp.nc file ********/

      if( ( status = nc_inq_varid( netCDFid, "X1", &evp_x1_position_X1_netCDFid ) ) != NC_NOERR )
      {  netCDF_StatusHandler( status ); return EXIT_FAILURE;}
      if( ( status = nc_inq_varid( netCDFid, "X2", &evp_x2_position_X2_netCDFid ) ) != NC_NOERR )
      {  netCDF_StatusHandler( status ); return EXIT_FAILURE;}
      if( ( status = nc_inq_varid( netCDFid, "X3", &evp_x3_position_X3_netCDFid ) ) != NC_NOERR )
      {  netCDF_StatusHandler( status ); return EXIT_FAILURE;}

      /*** we don't want to die if a regular variable is not found, so don't return control ***/
      /*** doing this primarily because my development test data does not contain dp cloud mass density variable
       * but we should not abandon program beacause of this - plus it's a good idea anyway - just print a warning **/

      if( ( status = nc_inq_varid( netCDFid, "TIME", &evp_physical_time_TIME_netCDFid ) ) != NC_NOERR )
      {
         time_exists = 0; fprintf( stderr, "\n%s !!!!! WARNING: Problem aquiring "
               "netCDFid for variable TIME. NETCDF ERROR:", __FILE__ );
         netCDF_StatusHandler( status );
      }

      if( ( status = nc_inq_varid( netCDFid, "DT", &evp_physical_time_step_DT_netCDFid ) ) != NC_NOERR )
      {  dt_exists = 0; fprintf( stderr, "\n%s !!!!! WARNING: Problem aquiring "
               "netCDFid for variable DT. NETCDF ERROR:", __FILE__ );
         netCDF_StatusHandler( status );
      }

      if( ( status = nc_inq_varid( netCDFid, "NSTEP", &evp_numerical_time_step_NSTEP_netCDFid ) ) != NC_NOERR )
      {
         nstep_exists = 0; fprintf( stderr, "\n%s !!!!! WARNING: Problem "
               "aquiring netCDFid for variable NSTEP. NETCDF ERROR:", __FILE__ );
         netCDF_StatusHandler( status );
      }

      if( ( status = nc_inq_varid( netCDFid, "D", &evp_density_D_netCDFid ) ) != NC_NOERR )
      {
         d_exists = 0; fprintf( stderr, "\n%s !!!!! WARNING: Problem aquiring "
               "netCDFid for variable D. NETCDF ERROR:", __FILE__ );
         netCDF_StatusHandler( status );
      }

      if( ( status = nc_inq_varid( netCDFid, "T", &evp_temperature_T_netCDFid ) ) != NC_NOERR )
      {
         t_exists = 0; fprintf( stderr,"\n%s !!!!! WARNING: Problem aquiring "
               "netCDFid for variable T. NETCDF ERROR:", __FILE__ );
         netCDF_StatusHandler( status );
      }

      if( ( status = nc_inq_varid( netCDFid, "V1", &evp_x1_velocity_V1_netCDFid ) ) != NC_NOERR )
      {
         v1_exists = 0; fprintf( stderr,"\n%s !!!!! WARNING: Problem aquiring "
               "netCDFid for variable V1. NETCDF ERROR:", __FILE__ );
         netCDF_StatusHandler( status );
      }

      if( ( status = nc_inq_varid( netCDFid, "V2", &evp_x2_velocity_V2_netCDFid ) ) != NC_NOERR )
      {
         v2_exists = 0; fprintf( stderr,"\n%s !!!!! WARNING: Problem aquiring "
               "netCDFid for variable V2. NETCDF ERROR:", __FILE__ );
         netCDF_StatusHandler( status );
      }

      if( ( status = nc_inq_varid( netCDFid, "V3", &evp_x3_velocity_V3_netCDFid ) ) != NC_NOERR )
      {
         v3_exists = 0; fprintf( stderr,"\n%s !!!!! WARNING: Problem aquiring "
               "netCDFid for variable V3. NETCDF ERROR:", __FILE__ );
         netCDF_StatusHandler( status );
      }

      if( ( status = nc_inq_varid( netCDFid, "B1", &evp_x1_magnetic_field_B1_netCDFid ) ) != NC_NOERR )
      {
         b1_exists = 0; fprintf( stderr,"\n%s !!!!! WARNING: Problem aquiring "
               "netCDFid for variable B1. NETCDF ERROR:", __FILE__ );
         netCDF_StatusHandler( status );
      }

      if( ( status = nc_inq_varid( netCDFid, "B2", &evp_x2_magnetic_field_B2_netCDFid ) ) != NC_NOERR )
      {  b2_exists = 0; fprintf( stderr,"\n%s !!!!! WARNING: Problem aquiring "
               "netCDFid for variable B2. NETCDF ERROR:", __FILE__ );
         netCDF_StatusHandler( status );
      }

      if( ( status = nc_inq_varid( netCDFid, "B3", &evp_x3_magnetic_field_B3_netCDFid )) != NC_NOERR )
      {  b3_exists = 0; fprintf( stderr,"\n%s !!!!! WARNING: Problem aquiring "
               "netCDFid for variable B3. NETCDF ERROR:", __FILE__ );
         netCDF_StatusHandler( status );
      }

      if( ( status = nc_inq_varid( netCDFid, "DP", &evp_cloud_mass_density_DP_netCDFid ) ) != NC_NOERR )
      {
         dp_exists = 0; fprintf( stderr,"\n%s !!!!! WARNING: Problem aquiring "
               "netCDFid for variable DP. NETCDF ERROR:", __FILE__ );
         netCDF_StatusHandler( status );
      }

      if( ( status = nc_inq_varid( netCDFid, "BP", &evp_polarity_of_magnetic_field_BP_netCDFid ) ) != NC_NOERR )
      {
         bp_exists = 0; fprintf( stderr,"\n%s !!!!! WARNING: Problem aquiring "
               "netCDFid for variable BP. NETCDF ERROR:", __FILE__ );
         netCDF_StatusHandler( status );
      }

      /************************ call get_enlil_variable_attributes() function for each variable if id is valid ********************/

      if( 1 )
      {
         status = get_enlil_variable_netcdf_attributes( netCDFid,
               evp_x1_position_X1_netCDFid, evp_x1_position_X1_long_name_vattr,
               evp_x1_position_X1_units_vattr );
      }
      if( 1 )
      {
         status = get_enlil_variable_netcdf_attributes( netCDFid,
               evp_x2_position_X2_netCDFid, evp_x2_position_X2_long_name_vattr,
               evp_x2_position_X2_units_vattr );
      }
      if( 1 )
      {
         status = get_enlil_variable_netcdf_attributes( netCDFid,
               evp_x3_position_X3_netCDFid, evp_x3_position_X3_long_name_vattr,
               evp_x3_position_X3_units_vattr );
      }
      if( time_exists )
      {
         status = get_enlil_variable_netcdf_attributes( netCDFid,
               evp_physical_time_TIME_netCDFid,
               evp_physical_time_TIME_long_name_vattr,
               evp_physical_time_TIME_units_vattr );
      }
      if( dt_exists )
      {
         status = get_enlil_variable_netcdf_attributes( netCDFid,
               evp_physical_time_step_DT_netCDFid,
               evp_physical_time_step_DT_long_name_vattr,
               evp_physical_time_step_DT_units_vattr );
      }
      if( nstep_exists )
      {
         status = get_enlil_variable_netcdf_attributes( netCDFid,
               evp_numerical_time_step_NSTEP_netCDFid,
               evp_numerical_time_step_NSTEP_long_name_vattr,
               evp_numerical_time_step_NSTEP_units_vattr );
      }
      if( d_exists )
      {
         status = get_enlil_variable_netcdf_attributes( netCDFid,
               evp_density_D_netCDFid,
               evp_density_D_long_name_vattr,
               evp_density_D_units_vattr );
      }
      if( t_exists )
      {
         status = get_enlil_variable_netcdf_attributes( netCDFid,
               evp_temperature_T_netCDFid,
               evp_temperature_T_long_name_vattr,
               evp_temperature_T_units_vattr );
      }
      if( v1_exists )
      {
         status = get_enlil_variable_netcdf_attributes( netCDFid,
               evp_x1_velocity_V1_netCDFid,
               evp_x1_velocity_V1_long_name_vattr,
               evp_x1_velocity_V1_units_vattr );
      }
      if( v2_exists )
      {
         status = get_enlil_variable_netcdf_attributes( netCDFid,
               evp_x2_velocity_V2_netCDFid,
               evp_x2_velocity_V2_long_name_vattr,
               evp_x2_velocity_V2_units_vattr );
      }
      if( v3_exists )
      {
         status = get_enlil_variable_netcdf_attributes( netCDFid,
               evp_x3_velocity_V3_netCDFid,
               evp_x3_velocity_V3_long_name_vattr,
               evp_x3_velocity_V3_units_vattr );
      }
      if( b1_exists )
      {
         status = get_enlil_variable_netcdf_attributes( netCDFid,
               evp_x1_magnetic_field_B1_netCDFid,
               evp_x1_magnetic_field_B1_long_name_vattr,
               evp_x1_magnetic_field_B1_units_vattr );
      }
      if( b2_exists )
      {
         status = get_enlil_variable_netcdf_attributes( netCDFid,
               evp_x2_magnetic_field_B2_netCDFid,
               evp_x2_magnetic_field_B2_long_name_vattr,
               evp_x2_magnetic_field_B2_units_vattr );
      }
      if( b3_exists )
      {
         status = get_enlil_variable_netcdf_attributes( netCDFid,
               evp_x3_magnetic_field_B3_netCDFid,
               evp_x3_magnetic_field_B3_long_name_vattr,
               evp_x3_magnetic_field_B3_units_vattr );
      }
      if( dp_exists )
      {
         status = get_enlil_variable_netcdf_attributes( netCDFid,
               evp_cloud_mass_density_DP_netCDFid,
               evp_cloud_mass_density_DP_long_name_vattr,
               evp_cloud_mass_density_DP_units_vattr );
      }
      if( bp_exists )
      {
         status = get_enlil_variable_netcdf_attributes( netCDFid,
               evp_polarity_of_magnetic_field_BP_netCDFid,
               evp_polarity_of_magnetic_field_BP_long_name_vattr,
               evp_polarity_of_magnetic_field_BP_units_vattr );
      }

      /* dynamically allocate memory and then get the actual variable values */

      if( DEBUG_FLAG ) printf("allocating mem\n");

      grid_size = evp_npla * evp_nevo;

      /****** allocate memory for position variables X1,X2,X3,X1H,X2H,X3H *******/

      /************ POSITION VARIABLES ************/

      if ( ( evp_x1_position_X1 = calloc( grid_size, sizeof(float) ) ) == NULL )
      {
         printf("MALLOC for evp_x1_position_X1 array FAILED\n");
         exit(EXIT_FAILURE);
      }
      if ( ( evp_x2_position_X2 = calloc( grid_size, sizeof(float) ) ) == NULL )
      {
         printf("MALLOC for evp_x2_position_X2 array FAILED\n");
         exit(EXIT_FAILURE);
      }
      if ( ( evp_x3_position_X3 = calloc( grid_size, sizeof(float) ) ) == NULL )
      {
         printf("MALLOC for evp_x3h_position_X3 array FAILED\n");
         exit(EXIT_FAILURE);
      }

      /***** variables time, dt & nstep *****/

      if ( ( evp_physical_time_TIME = calloc( evp_nevo , sizeof(float) ) ) == NULL )
      {
         printf("MALLOC for evp_physical_time_TIME FAILED\n");
         exit(EXIT_FAILURE);
      }
      if ( ( evp_physical_time_step_DT = calloc( evp_nevo , sizeof(float) ) ) == NULL )
      {
         printf("MALLOC for evp_physical_time_step_DT FAILED\n");
         exit(EXIT_FAILURE);
      }
      if ( ( evp_numerical_time_step_NSTEP = calloc( evp_nevo , sizeof(int) ) ) == NULL )
      {
         printf("MALLOC for evp_numerical_time_step_NSTEP FAILED\n");
         exit(EXIT_FAILURE);
      }

      /************ DATA VARIABLES ************/

      /** only allocate memory if variable exists **/

      if( d_exists )
      {
         if ( ( evp_density_D = calloc( grid_size, sizeof(float) ) ) == NULL )
         {
            printf("MALLOC for evp_density_D array FAILED\n");
            exit(EXIT_FAILURE);
         }
      }
      if( t_exists )
      {
         if ( ( evp_temperature_T = calloc( grid_size, sizeof(float) ) ) == NULL )
         {
            printf("MALLOC for evp_temperature_T array FAILED\n");
            exit(EXIT_FAILURE);
         }
      }
      if( v1_exists )
      {
         if ( ( evp_x1_velocity_V1 = calloc( grid_size, sizeof(float) ) ) == NULL )
         {
            printf("MALLOC for evp_x1_velocity_V1 array FAILED\n");
            exit(EXIT_FAILURE);
         }
      }
      if( v2_exists )
      {
         if ( ( evp_x2_velocity_V2 = calloc( grid_size, sizeof(float) ) ) == NULL )
         {
            printf("MALLOC for evp_x2_velocity_V2 array FAILED\n");
            exit(EXIT_FAILURE);
         }
      }
      if( v3_exists )
      {
         if ( ( evp_x3_velocity_V3 = calloc( grid_size, sizeof(float) ) ) == NULL )
         {
            printf("MALLOC for evp_x3_velocity_V3 array FAILED\n");
            exit(EXIT_FAILURE);
         }
      }
      if( b1_exists )
      {
         if ( ( evp_x1_magnetic_field_B1 = calloc( grid_size, sizeof(float) ) ) == NULL )
         {
            printf("MALLOC for evp_x1_velocity_B1 array FAILED\n");
            exit(EXIT_FAILURE);
         }
      }
      if( b2_exists )
      {
         if ( ( evp_x2_magnetic_field_B2 = calloc( grid_size, sizeof(float) ) ) == NULL )
         {
            printf("MALLOC for evp_x2_magnetic_field_B2 array FAILED\n");
            exit(EXIT_FAILURE);
         }
      }
      if( b3_exists )
      {
         if ( ( evp_x3_magnetic_field_B3 = calloc( grid_size, sizeof(float) ) ) == NULL )
         {
            printf("MALLOC for evp_x3_magnetic_field_B3 array FAILED\n");
            exit(EXIT_FAILURE);
         }
      }
      if( dp_exists )
      {
         if ( ( evp_cloud_mass_density_DP = calloc( grid_size, sizeof(float) ) ) == NULL )
         {
            printf("MALLOC for evp_cloud_mass_density_DP array FAILED\n");
            exit(EXIT_FAILURE);
         }
      }
      if( bp_exists )
      {
         if ( ( evp_polarity_of_magnetic_field_BP = calloc( grid_size, sizeof(float) ) ) == NULL )
         {
            printf("MALLOC for evp_polarity_of_magnetic_field_BP array FAILED\n");
            exit(EXIT_FAILURE);
         }
      }

      if( DEBUG_FLAG ) printf("reading files\n");

      /******** now get actual position values from netCDF file *************/

      if( nc_get_var_float( netCDFid, evp_x1_position_X1_netCDFid, evp_x1_position_X1 ) != NC_NOERR )
      {  netCDF_StatusHandler( status ); return EXIT_FAILURE;}
      if( nc_get_var_float( netCDFid, evp_x2_position_X2_netCDFid, evp_x2_position_X2 ) != NC_NOERR )
      {  netCDF_StatusHandler( status ); return EXIT_FAILURE;}
      if( nc_get_var_float( netCDFid, evp_x3_position_X3_netCDFid, evp_x3_position_X3 ) != NC_NOERR )
      {  netCDF_StatusHandler( status ); return EXIT_FAILURE;}

      if( DEBUG_FLAG ) printf("DEBUG\tmain grid size = [%d,%d]\n", (int ) evp_nevo, (int ) evp_npla );

      /*********** DATA VALUES ********/

      if( d_exists )
      {
         if( nc_get_var_float( netCDFid, evp_density_D_netCDFid, evp_density_D ) != NC_NOERR )
         {  netCDF_StatusHandler( status ); return EXIT_FAILURE;}
      }
      if( t_exists )
      {
         if( nc_get_var_float( netCDFid, evp_temperature_T_netCDFid, evp_temperature_T ) != NC_NOERR )
         {  netCDF_StatusHandler( status ); return EXIT_FAILURE;}
      }
      if( v1_exists )
      {
         if( nc_get_var_float( netCDFid, evp_x1_velocity_V1_netCDFid, evp_x1_velocity_V1 ) != NC_NOERR )
         {  netCDF_StatusHandler( status ); return EXIT_FAILURE;}
      }
      if( v2_exists )
      {
         if( nc_get_var_float( netCDFid, evp_x2_velocity_V2_netCDFid, evp_x2_velocity_V2 ) != NC_NOERR )
         {  netCDF_StatusHandler( status ); return EXIT_FAILURE;}
      }
      if( v3_exists )
      {
         if( nc_get_var_float( netCDFid, evp_x3_velocity_V3_netCDFid, evp_x3_velocity_V3 ) != NC_NOERR )
         {  netCDF_StatusHandler( status ); return EXIT_FAILURE;}
      }
      if( b1_exists )
      {
         if( nc_get_var_float( netCDFid, evp_x1_magnetic_field_B1_netCDFid, evp_x1_magnetic_field_B1 ) != NC_NOERR )
         {  netCDF_StatusHandler( status ); return EXIT_FAILURE;}
      }
      if( b2_exists )
      {
         if( nc_get_var_float( netCDFid, evp_x2_magnetic_field_B2_netCDFid, evp_x2_magnetic_field_B2 ) != NC_NOERR )
         {  netCDF_StatusHandler( status ); return EXIT_FAILURE;}
      }
      if( b3_exists )
      {
         if( nc_get_var_float( netCDFid, evp_x3_magnetic_field_B3_netCDFid, evp_x3_magnetic_field_B3 ) != NC_NOERR )
         {  netCDF_StatusHandler( status ); return EXIT_FAILURE;}
      }
      if( dp_exists )
      {
         if( nc_get_var_float( netCDFid, evp_cloud_mass_density_DP_netCDFid, evp_cloud_mass_density_DP ) != NC_NOERR )
         {  netCDF_StatusHandler( status ); return EXIT_FAILURE;}
      }
      if( bp_exists )
      {
         if( nc_get_var_float( netCDFid, evp_polarity_of_magnetic_field_BP_netCDFid, evp_polarity_of_magnetic_field_BP ) != NC_NOERR )
         {  netCDF_StatusHandler( status ); return EXIT_FAILURE;}
      }

      /*
       for( i = 0; i < grid_size ; i+=1635 )
       {
       printf(" evp_polarity_of_magnetic_field_BP[%d] = %f\n",i, evp_polarity_of_magnetic_field_BP[i] );
       }
       */

      /******* Now get the three time variables for time, dt, nstep **************/

      if( nc_get_var_float( netCDFid, evp_physical_time_TIME_netCDFid, evp_physical_time_TIME ) != NC_NOERR )
      {  netCDF_StatusHandler( status ); return EXIT_FAILURE;}
      if( nc_get_var_float( netCDFid, evp_physical_time_step_DT_netCDFid, evp_physical_time_step_DT ) != NC_NOERR )
      {  netCDF_StatusHandler( status ); return EXIT_FAILURE;}
      if( nc_get_var_int( netCDFid, evp_numerical_time_step_NSTEP_netCDFid, evp_numerical_time_step_NSTEP ) != NC_NOERR )
      {  netCDF_StatusHandler( status ); return EXIT_FAILURE;}

      /********* close the currently open netCDF file **************/

      nc_close( netCDFid );

#endif

      return EXIT_SUCCESS;
   }

   /***********************************************************
    *             READ ENLIL VARIABLE NETCDF ATTRIBUTES            *
    **********************************************************/

int get_enlil_variable_netcdf_attributes(
      int netCDFid,
      int variable_id,
      char *long_name_var_attr,
      char *units_var_attr)
{
#ifdef NETCDF

   int status;
   size_t attr_size;
   nc_type attr_type;

   /*** for each variable attribute, get size, allocate memory and get value ***/
   /** if attribute number is not found, set attr_size = 1 and value == NULL... ***/

   if ( (status = nc_inq_att(
         netCDFid,
         variable_id,
         "long_name",
         &attr_type,
         &attr_size) ) != NC_NOERR)
   {
      netCDF_StatusHandler(status);
      attr_size = 1;
      long_name_var_attr = NULL;
   }
   if ( (long_name_var_attr = calloc(attr_size + 1, sizeof(char)) ) == NULL)
   {
      printf(
            "MALLOC for long_name variable attribute variable FAILED for variable_id %d\n",
            variable_id);
   }
   if ( (status = nc_get_att_text(
         netCDFid,
         variable_id,
         "long_name",
         long_name_var_attr) ) != NC_NOERR)
   {
      netCDF_StatusHandler(status);
      printf("%s\terror getting variable attribute value for long_name\n",__FILE__ );}

   /*printf("DEBUG\t%s\tvariable attr long_name has type %d and len = %d\t\t%s\n", __FILE__, attr_type, (int ) attr_size, long_name_var_attr );*/

   if( ( status = nc_inq_att( netCDFid, variable_id, "units", &attr_type, &attr_size ) ) != NC_NOERR )
   {  netCDF_StatusHandler( status ); attr_size = 1; units_var_attr = NULL;}
   if( ( units_var_attr = calloc( attr_size + 1, sizeof(char) ) ) == NULL )
   {  printf("MALLOC for units variable attribute variable FAILED for variable_id %d\n", variable_id);}
   if( ( status = nc_get_att_text( netCDFid, variable_id, "units", units_var_attr ) ) != NC_NOERR )
   {  netCDF_StatusHandler( status ); printf("%s\terror getting variable attribute value for units\n",__FILE__);}

   /*printf("DEBUG\t%s\tvariable attr units has type %d and len = %d\t\t%s\n", __FILE__, attr_type, (int ) attr_size, units_var_attr );*/

#endif
   return EXIT_SUCCESS;
}

/***********************************************************
 *             READ ENLIL GLOBAL NETCDF ATTRIBUTES            *
 **********************************************************/

int get_enlil_global_netcdf_attributes(
      int netCDFid,
      char **type,
      char **title,
      char **name,
      char **project,
      char **initial,
      char **resume,
      char **boundary,
      char **passage,
      char **grid,
      char **geometry,
      char **code,
      char **parameters,
      double **refdate_mjd,
      double **gamma,
      double **xalpha,
      char **history)
{
#ifdef NETCDF

   int status;
   size_t attr_size;
   nc_type attr_type;

   /*** for each global attribute, get size, allocate memory and get value ***/
   /** if attribute number is not found, set attr_size = 1 and value == NULL... ***/

   if ( DEBUG_FLAG)
      printf("DEBUG\t%s\tbefore malloc...time_gattribute_type address = %p\n",
      __FILE__, time_gattribute_type );
      if( DEBUG_FLAG ) printf("DEBUG\t%s\tbefore malloc...type address= %p\n", __FILE__, *type );

      if( ( status = nc_inq_att( netCDFid, NC_GLOBAL, "type", &attr_type, &attr_size ) ) != NC_NOERR )
      {
         netCDF_StatusHandler( status ); attr_size = 1; *type=NULL;
      }
      if ( ( *type = calloc( attr_size + 1, sizeof(char) ) ) == NULL )
      {
         printf("MALLOC for <type> global attribute variable FAILED\n");
      }
      if( ( status = nc_get_att_text( netCDFid, NC_GLOBAL, "type", *type ) ) != NC_NOERR )
      {
         netCDF_StatusHandler( status );
         printf("%s\terror getting attribute value for type\n",__FILE__);
      }

      if( DEBUG_FLAG ) printf("DEBUG\t%s\tglobal attr type has type %d and len "
            "= %d\t\t->%s<-\n", __FILE__, attr_type, (int ) attr_size, *type );

      if( DEBUG_FLAG ) printf("DEBUG\t%s\tafter malloc...global attr type has "
            "type %d and len = %d\t\t->%s<-\n", __FILE__, attr_type, (int ) attr_size, *type );

      if( DEBUG_FLAG ) printf("DEBUG\t%s\tafter malloc...time_gattribute_type "
            "address = %p\n", __FILE__, time_gattribute_type );

      if( DEBUG_FLAG ) printf("DEBUG\t%s\tafter malloc...type address = "
            "%p\n", __FILE__, *type );

      if( ( status = nc_inq_att( netCDFid, NC_GLOBAL, "title", &attr_type, &attr_size ) ) != NC_NOERR )
      {
         netCDF_StatusHandler( status ); attr_size = 1;
         *title=NULL;
      }

      if ( ( *title = calloc( attr_size + 1, sizeof(char) ) ) == NULL )
      {
         printf("MALLOC for <title> global attribute variable FAILED\n");
      }

      if( ( status = nc_get_att_text( netCDFid, NC_GLOBAL, "title", *title ) ) != NC_NOERR )
      {
         netCDF_StatusHandler( status ); printf("%s\terror getting attribute "
               "value for title\n",__FILE__);
      }

      if( DEBUG_FLAG )
      {
         printf("DEBUG\t%s\tglobal attr title has type %d and len = %d\t\t->%s"
               "<-\n", __FILE__, attr_type, (int ) attr_size, *title );
      }

      if( ( status = nc_inq_att( netCDFid, NC_GLOBAL, "name", &attr_type, &attr_size ) ) != NC_NOERR )
      {
         netCDF_StatusHandler( status ); attr_size = 1;
         *name=NULL;
      }

      if ( ( *name = calloc( attr_size + 1, sizeof(char) ) ) == NULL )
      {
         printf("MALLOC for <name> global attribute variable FAILED\n");
      }

      if( ( status = nc_get_att_text( netCDFid, NC_GLOBAL, "name", *name ) ) != NC_NOERR )
      {
         netCDF_StatusHandler( status );
         printf("%s\terror getting attribute value for name\n",__FILE__);
      }

      if( DEBUG_FLAG )
      {
         printf("DEBUG\t%s\tglobal attr name has type %d and len = %d\t\t->%s<-"
               "\n", __FILE__, attr_type, (int ) attr_size, *name );
      }

      if( ( status = nc_inq_att( netCDFid, NC_GLOBAL, "project", &attr_type, &attr_size ) ) != NC_NOERR )
      {
         netCDF_StatusHandler( status ); attr_size = 1; *project=NULL;
      }

      if ( ( *project = calloc( attr_size + 1, sizeof(char) ) ) == NULL )
      {
         printf("MALLOC for <project> global attribute variable FAILED\n");
      }

      if( ( status = nc_get_att_text( netCDFid, NC_GLOBAL, "project", *project ) ) != NC_NOERR )
      {
         netCDF_StatusHandler( status ); printf("%s\terror getting attribute "
               "value for project\n",__FILE__);
      }

      if( DEBUG_FLAG )
      {
         printf("DEBUG\t%s\tglobal attr project has type %d and len = %d\t\t->%s"
               "<-\n", __FILE__, attr_type, (int ) attr_size, *project );
      }

      if( ( status = nc_inq_att( netCDFid, NC_GLOBAL, "initial", &attr_type, &attr_size ) ) != NC_NOERR )
      {
         netCDF_StatusHandler( status ); attr_size = 1; *initial=NULL;
      }

      if ( ( *initial = calloc( attr_size + 1, sizeof(char) ) ) == NULL )
      {
         printf("MALLOC for <initial> global attribute variable FAILED\n");

      }

      if( ( status = nc_get_att_text( netCDFid, NC_GLOBAL, "initial", *initial ) ) != NC_NOERR )
      {
         netCDF_StatusHandler( status ); printf("%s\terror getting attribute "
               "value for initial\n",__FILE__);
      }

      if( DEBUG_FLAG )
      {
         printf("DEBUG\t%s\tglobal attr initial has type %d and len = %d\t\t->%s"
               "<-\n", __FILE__, attr_type, (int ) attr_size, *initial );
      }

      if( ( status = nc_inq_att( netCDFid, NC_GLOBAL, "resume", &attr_type, &attr_size ) ) != NC_NOERR )
      {
         netCDF_StatusHandler( status ); attr_size = 1; *resume=NULL;
      }

      if ( ( *resume = calloc( attr_size + 1, sizeof(char) ) ) == NULL )
      {
         printf("MALLOC for <resume> global attribute variable FAILED\n");
      }

      if( ( status = nc_get_att_text( netCDFid, NC_GLOBAL, "resume", *resume ) ) != NC_NOERR )
      {
         netCDF_StatusHandler( status ); printf("%s\terror getting attribute "
               "value for resume\n",__FILE__);
      }

      if( DEBUG_FLAG )
      {
         printf("DEBUG\t%s\tglobal attr resume has type %d and len= %d\t\t->%s<-"
               "\n", __FILE__, attr_type, (int ) attr_size, *resume );
      }

      if( ( status = nc_inq_att( netCDFid, NC_GLOBAL, "boundary", &attr_type, &attr_size ) ) != NC_NOERR )
      {
         netCDF_StatusHandler( status ); attr_size = 1;
         *boundary=NULL;
      }

      if ( ( *boundary = calloc( attr_size + 1, sizeof(char) ) ) == NULL )
      {
         printf("MALLOC for <boundary> global attribute variable FAILED\n");
      }

      if( ( status = nc_get_att_text( netCDFid, NC_GLOBAL, "boundary", *boundary ) ) != NC_NOERR )
      {
         netCDF_StatusHandler( status );
         printf("%s\terror getting attribute value for boundary\n",__FILE__);
      }

      if( DEBUG_FLAG )
      {
         printf("DEBUG\t%s\tglobal attr boundary has type %d and len = %d\t\t->"
               "%s<-\n", __FILE__, attr_type, (int ) attr_size, *boundary );
      }

      if( ( status = nc_inq_att( netCDFid, NC_GLOBAL, "passage", &attr_type, &attr_size ) ) != NC_NOERR )
      {
         netCDF_StatusHandler( status ); attr_size = 1;
         *passage=NULL;
      }

      if ( ( *passage = calloc( attr_size + 1, sizeof(char) ) ) == NULL )
      {
         printf("MALLOC for <passage> global attribute variable FAILED\n");
      }

      if( ( status = nc_get_att_text( netCDFid, NC_GLOBAL, "passage", *passage ) ) != NC_NOERR )
      {
         netCDF_StatusHandler( status );
         printf("%s\terror getting attribute value for passage\n",__FILE__);
      }

      if( DEBUG_FLAG )
      {
         printf("DEBUG\t%s\tglobal attr passage has type %d and len = %d\t\t->%s"
               "<-\n", __FILE__, attr_type, (int ) attr_size, *passage );
      }

      if( ( status = nc_inq_att( netCDFid, NC_GLOBAL, "grid", &attr_type, &attr_size ) ) != NC_NOERR )
      {
         netCDF_StatusHandler( status ); attr_size = 1;
         *grid=NULL;
      }

      if ( ( *grid = calloc( attr_size + 1, sizeof(char) ) ) == NULL )
      {
         printf("MALLOC for <grid> global attribute variable FAILED\n");
      }

      if( ( status = nc_get_att_text( netCDFid, NC_GLOBAL, "grid", *grid ) ) != NC_NOERR )
      {
         netCDF_StatusHandler( status ); printf("%s\terror getting attribute "
               "value for grid\n",__FILE__);
      }

      if( DEBUG_FLAG )
      {
         printf("DEBUG\t%s\tglobal attr grid has type %d and len = %d\t\t->%s<-"
               "\n", __FILE__, attr_type, (int ) attr_size, *grid );
      }

      if( ( status = nc_inq_att( netCDFid, NC_GLOBAL, "geometry", &attr_type, &attr_size ) ) != NC_NOERR )
      {
         netCDF_StatusHandler( status );attr_size = 1;
         *geometry=NULL;
      }

      if ( ( *geometry = calloc( attr_size + 1, sizeof(char) ) ) == NULL )
      {
         printf("MALLOC for <geometry> global attribute variable FAILED\n");
      }

      if( ( status = nc_get_att_text( netCDFid, NC_GLOBAL, "geometry", *geometry ) ) != NC_NOERR )
      {
         netCDF_StatusHandler( status );
         printf("%s\terror getting attribute value for geometry\n",__FILE__);
      }

      if( DEBUG_FLAG )
      {
         printf("DEBUG\t%s\tglobal attr geometry has type %d and len = %d\t\t->"
               "%s<-\n", __FILE__, attr_type, (int ) attr_size, *geometry );
      }

      if( ( status = nc_inq_att( netCDFid, NC_GLOBAL, "code", &attr_type, &attr_size ) ) != NC_NOERR )
      {
         netCDF_StatusHandler( status ); attr_size = 1; *code=NULL;
      }

      if ( ( *code = calloc( attr_size + 1, sizeof(char) ) ) == NULL )
      {
         printf("MALLOC for <code> global attribute variable FAILED\n");
      }

      if( ( status = nc_get_att_text( netCDFid, NC_GLOBAL, "code", *code ) ) != NC_NOERR )
      {
         netCDF_StatusHandler( status );
         printf("%s\terror getting attribute value for code\n",__FILE__);
      }

      if( DEBUG_FLAG )
      {
         printf("DEBUG\t%s\tglobal attr code has type %d and len = %d\t\t->%s<-"
               "\n", __FILE__, attr_type, (int ) attr_size, *code );
      }

      if( ( status = nc_inq_att( netCDFid, NC_GLOBAL, "parameters", &attr_type, &attr_size ) ) != NC_NOERR )
      {
         netCDF_StatusHandler( status );
         attr_size = 1;
         *parameters=NULL;
      }

      if ( ( *parameters = calloc( attr_size + 1, sizeof(char) ) ) == NULL )
      {
         printf("MALLOC for <parameters> global attribute variable FAILED\n");
      }

      if( ( status = nc_get_att_text( netCDFid, NC_GLOBAL, "parameters", *parameters ) ) != NC_NOERR )
      {
         netCDF_StatusHandler( status );
         printf("%s\terror getting attribute value for parameters\n",__FILE__);
      }

      if( DEBUG_FLAG )
      {
         printf("DEBUG\t%s\tglobal attr parameters has type %d and len = %d\t\t"
               "%s\n", __FILE__, attr_type, (int ) attr_size, *parameters );
      }

      if( DEBUG_FLAG )
      {
         printf("DEBUG\t%s\tbefore malloc...time_gattribute_refdate_mjd address "
               "= %p\n", __FILE__, time_gattribute_refdate_mjd );
      }

      if( DEBUG_FLAG )
      {
         printf("DEBUG\t%s\tbefore malloc...refdate_mjd address = %p"
               "\n", __FILE__, *refdate_mjd );
      }

      if( ( status = nc_inq_att( netCDFid, NC_GLOBAL, "refdate_mjd", &attr_type, &attr_size ) ) != NC_NOERR )
      {
         netCDF_StatusHandler( status );
         attr_size = 1;
         *refdate_mjd=NULL;
      }

      if ( ( *refdate_mjd = calloc( attr_size, sizeof(double) ) ) == NULL )
      {
         printf("MALLOC for <refdate_mjd> global attribute variable FAILED\n");
      }

      if( ( status = nc_get_att_double( netCDFid, NC_GLOBAL, "refdate_mjd", *refdate_mjd ) ) != NC_NOERR )
      {
         netCDF_StatusHandler( status );
         printf("%s\terror getting attribute value for refdate_mjd\n",__FILE__);
      }

      if( DEBUG_FLAG )
      {
         printf("DEBUG\t%s\after malloc...time_gattribute_refdate_mjd address = "
               "%p\n", __FILE__, time_gattribute_refdate_mjd );
      }

      if( DEBUG_FLAG )
      {
         printf("DEBUG\t%s\tafter malloc...refdate_mjd address = %p\n"
               "", __FILE__, *refdate_mjd );
      }

      if( DEBUG_FLAG )
      {
         printf("DEBUG\t%s\tglobal attr refdate_mjd has type %d and len = %d\t\t"
               "->%g<-\n", __FILE__, attr_type, (int ) attr_size, **refdate_mjd );
      }

      if( ( status = nc_inq_att( netCDFid, NC_GLOBAL, "gamma", &attr_type, &attr_size ) ) != NC_NOERR )
      {
         netCDF_StatusHandler( status ); attr_size = 1;
         *gamma=NULL;
      }

      if ( ( *gamma = calloc( attr_size, sizeof(double) ) ) == NULL )
      {
         printf("MALLOC for <gamma> global attribute variable FAILED\n");
      }

      if( ( status = nc_get_att_double( netCDFid, NC_GLOBAL, "gamma", *gamma ) ) != NC_NOERR )
      {
         netCDF_StatusHandler( status );
         printf("%s\terror getting attribute value for gamma\n",__FILE__);
      }

      if( ( status = nc_inq_att( netCDFid, NC_GLOBAL, "xalpha", &attr_type, &attr_size ) ) != NC_NOERR )
      {
         netCDF_StatusHandler( status );
         attr_size = 1;
         *xalpha=NULL;
      }

      if ( ( *xalpha = calloc( attr_size, sizeof(double) ) ) == NULL )
      {
         printf("MALLOC for <xalpha> global attribute variable FAILED\n");
      }

      if( ( status = nc_get_att_double( netCDFid, NC_GLOBAL, "xalpha", *xalpha ) ) != NC_NOERR )
      {
         netCDF_StatusHandler( status );
         printf("%s\terror getting attribute value for xalpha\n",__FILE__);
      }

      if( DEBUG_FLAG )
      {
         printf("DEBUG\t%s\tglobal attr xalpha has type %d and len = %d\t\t->%f"
               "<-\n", __FILE__, attr_type, (int ) attr_size, **xalpha );
      }

      if( ( status = nc_inq_att( netCDFid, NC_GLOBAL, "history", &attr_type, &attr_size ) ) != NC_NOERR )
      {
         netCDF_StatusHandler( status );
         attr_size = 1;
         *history=NULL;
      }

      if ( ( *history = calloc( attr_size + 1, sizeof(char) ) ) == NULL )
      {
         printf("MALLOC for <history> global attribute variable FAILED\n");
      }

      if( ( status = nc_get_att_text( netCDFid, NC_GLOBAL, "history", *history ) ) != NC_NOERR )
      {
         netCDF_StatusHandler( status ); printf("%s\terror getting attribute "
               "value for history\n",__FILE__);
      }

      if( DEBUG_FLAG )
      {
         printf("DEBUG\t%s\tglobal attr history has type %d and len = %d\t\t->%s"
               "<-\n", __FILE__, attr_type, (int ) attr_size, *history );
      }

#endif
      return EXIT_SUCCESS;
   }

   /****** Lets Create an in-house NETCDF Status Handler **********/

void netCDF_StatusHandler(int status)
{

#ifdef NETCDF

   fprintf( stderr, "\t%s\n\n", nc_strerror( status ));

#endif

   return;
}
