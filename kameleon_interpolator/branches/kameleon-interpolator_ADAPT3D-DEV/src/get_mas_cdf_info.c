/*******************************************************************************
 *                                                                             *
 *       NAME:          get_mas_cdf_info.c                                     *
 *                                                                             *
 *       VERSION:       1.0                                                    *
 *                                                                             *
 *       AUTHOR:        Marlo M. H. Maddox                                     *
 *                      NASA-GSFC-AETD-ISD-ADMA(587)-CCMC                      *
 *                      Marlo.Maddox@nasa.gov                                  *
 *                      (301) 286 - 5202                                       *
 *                                                                             *
 *       PURPOSE:       Extracts all relevant information for identifying      *
 *                      variables etc from the mas cdf file.  This routine is  *
 *                      called by the open_cdf call Variable numbers are       *
 *                      extracted along with actual position values. If an     *
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
 *       02.23.2007     Maddox, Marlo                                          *
 *                      Initial development started                            *
 *                                                                             *
 *       03.26.2007     Maddox, Marlo                                          *
 *                      Fixed bug in reading & indexing unique position values *
 *                                                                             *
 *       04.09.2007     Maddox, Marlo                                          *
 *                      Adding ofset grid combination support                  *
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

#define DEBUG_FLAG 0

void get_mas_cdf_info(
      int main_memory_flag)
{

   void allocate_defaults_mas(
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

   int i;

   counts[0] = 1; /* set values */
   intervals[0] = 1;

   allocate_defaults_mas();

   /******* get all of the cdf attribute/variable numbers that will be needed */

   if (CDFlib( CONFIRM_, zVAR_EXISTENCE_, r_name, NULL_) == CDF_OK)
   {
      status = CDFlib( GET_, zVAR_NUMBER_, r_name, &r_cdf_varNum, NULL_);
      if (status != CDF_OK)
      {
         printf(
               "ERROR: %s\ncouldn't read %s\n",
               __FILE__, r_name ); StatusHandler (status);
            }
         }

         if( CDFlib( CONFIRM_, zVAR_EXISTENCE_,r1_name ,
NULL_         )== CDF_OK )
         {
            status = CDFlib ( GET_, zVAR_NUMBER_, r1_name, &r1_cdf_varNum, NULL_ );

            if(status != CDF_OK)
            {
               printf("ERROR: %s\ncouldn't read %s\n", __FILE__, r1_name );
               StatusHandler (status);
            }
         }

         if( CDFlib( CONFIRM_, zVAR_EXISTENCE_, phi_name, NULL_ ) == CDF_OK )
         {
            status = CDFlib ( GET_, zVAR_NUMBER_, phi_name, &phi_cdf_varNum, NULL_ );
            if(status != CDF_OK)
            {  printf("ERROR: %s\ncouldn't read %s\n", __FILE__, phi_name ); StatusHandler (status);}
         }

         if( CDFlib( CONFIRM_, zVAR_EXISTENCE_, theta_name, NULL_ ) == CDF_OK )
         {
            status = CDFlib ( GET_, zVAR_NUMBER_, theta_name, &theta_cdf_varNum, NULL_ );
            if(status != CDF_OK)
            {  printf("ERROR: %s\ncouldn't read %s\n", __FILE__, theta_name ); StatusHandler (status);}
         }

         if( CDFlib( CONFIRM_, zVAR_EXISTENCE_, theta1_name, NULL_ ) == CDF_OK )
         {
            status = CDFlib ( GET_, zVAR_NUMBER_, theta1_name, &theta1_cdf_varNum, NULL_ );
            if(status != CDF_OK)
            {  printf("ERROR: %s\ncouldn't read %s\n", __FILE__, theta1_name ); StatusHandler (status);}
         }

         if( CDFlib( CONFIRM_, zVAR_EXISTENCE_, T_name, NULL_ ) == CDF_OK )
         {
            status = CDFlib ( GET_, zVAR_NUMBER_, T_name, &T_cdf_varNum, NULL_ );
            if(status != CDF_OK)
            {  printf("ERROR: %s\ncouldn't read %s\n", __FILE__, T_name ); StatusHandler (status);}
         }

         if( CDFlib( CONFIRM_, zVAR_EXISTENCE_, br_name, NULL_ ) == CDF_OK )
         {
            status = CDFlib ( GET_, zVAR_NUMBER_, br_name, &br_cdf_varNum, NULL_ );
            if(status != CDF_OK)
            {  printf("ERROR: %s\ncouldn't read %s\n", __FILE__, br_name ); StatusHandler (status);}
         }

         if( CDFlib( CONFIRM_, zVAR_EXISTENCE_, bphi_name, NULL_ ) == CDF_OK )
         {
            status = CDFlib ( GET_, zVAR_NUMBER_, bphi_name, &bphi_cdf_varNum, NULL_ );
            if(status != CDF_OK)
            {  printf("ERROR: %s\ncouldn't read %s\n", __FILE__, bphi_name ); StatusHandler (status);}
         }

         if( CDFlib( CONFIRM_, zVAR_EXISTENCE_, btheta_name, NULL_ ) == CDF_OK )
         {
            status = CDFlib ( GET_, zVAR_NUMBER_, btheta_name, &btheta_cdf_varNum, NULL_ );
            if(status != CDF_OK)
            {  printf("ERROR: %s\ncouldn't read %s\n", __FILE__, btheta_name ); StatusHandler (status);}
         }

         if( CDFlib( CONFIRM_, zVAR_EXISTENCE_, jr_name, NULL_ ) == CDF_OK )
         {
            status = CDFlib ( GET_, zVAR_NUMBER_, jr_name, &jr_cdf_varNum, NULL_ );
            if(status != CDF_OK)
            {  printf("ERROR: %s\ncouldn't read %s\n", __FILE__, jr_name ); StatusHandler (status);}
         }

         if( CDFlib( CONFIRM_, zVAR_EXISTENCE_, jphi_name, NULL_ ) == CDF_OK )
         {
            status = CDFlib ( GET_, zVAR_NUMBER_, jphi_name, &jphi_cdf_varNum, NULL_ );
            if(status != CDF_OK)
            {  printf("ERROR: %s\ncouldn't read %s\n", __FILE__, jphi_name ); StatusHandler (status);}
         }

         if( CDFlib( CONFIRM_, zVAR_EXISTENCE_, jtheta_name, NULL_ ) == CDF_OK )
         {
            status = CDFlib ( GET_, zVAR_NUMBER_, jtheta_name, &jtheta_cdf_varNum, NULL_ );
            if(status != CDF_OK)
            {  printf("ERROR: %s\ncouldn't read %s\n", __FILE__, jtheta_name ); StatusHandler (status);}
         }

         if( CDFlib( CONFIRM_, zVAR_EXISTENCE_, ur_name, NULL_ ) == CDF_OK )
         {
            status = CDFlib ( GET_, zVAR_NUMBER_, ur_name, &ur_cdf_varNum, NULL_ );
            if(status != CDF_OK)
            {  printf("ERROR: %s\ncouldn't read %s\n", __FILE__, ur_name ); StatusHandler (status);}
         }

         if( CDFlib( CONFIRM_, zVAR_EXISTENCE_, uphi_name, NULL_ ) == CDF_OK )
         {
            status = CDFlib ( GET_, zVAR_NUMBER_, uphi_name, &uphi_cdf_varNum, NULL_ );
            if(status != CDF_OK)
            {  printf("ERROR: %s\ncouldn't read %s\n", __FILE__, uphi_name ); StatusHandler (status);}
         }

         if( CDFlib( CONFIRM_, zVAR_EXISTENCE_, utheta_name, NULL_ ) == CDF_OK )
         {
            status = CDFlib ( GET_, zVAR_NUMBER_, utheta_name, &utheta_cdf_varNum, NULL_ );
            if(status != CDF_OK)
            {  printf("ERROR: %s\ncouldn't read %s\n", __FILE__, utheta_name ); StatusHandler (status);}
         }

         if( CDFlib( CONFIRM_, zVAR_EXISTENCE_, rho_name, NULL_ ) == CDF_OK )
         {
            status = CDFlib ( GET_, zVAR_NUMBER_, rho_name, &rho_cdf_varNum, NULL_ );
            if(status != CDF_OK)
            {  printf("ERROR: %s\ncouldn't read %s\n", __FILE__, rho_name ); StatusHandler (status);}
         }

         if( CDFlib( CONFIRM_, zVAR_EXISTENCE_, p_name, NULL_ ) == CDF_OK )
         {
            status = CDFlib ( GET_, zVAR_NUMBER_, p_name, &p_cdf_varNum, NULL_ );
            if(status != CDF_OK)
            {  printf("ERROR: %s\ncouldn't read %s\n", __FILE__, p_name ); StatusHandler (status);}
         }

         /***  NOTE THESE ARE ATTRIBUTES SO USE CONFRIM ATTR_EXISTENCE... ***/

         if( CDFlib( CONFIRM_, ATTR_EXISTENCE_, NX_name, NULL_ ) == CDF_OK )
         {
            status = CDFlib ( GET_, ATTR_NUMBER_, NX_name, &NX_cdf_num, NULL_ );
            if(status != CDF_OK)
            {  printf("ERROR: %s\ncouldn't read %s\n", __FILE__, NX_name ); StatusHandler (status);}
         }

         if( CDFlib( CONFIRM_, ATTR_EXISTENCE_, NY_name, NULL_ ) == CDF_OK )
         {
            status = CDFlib ( GET_, ATTR_NUMBER_, NY_name, &NY_cdf_num, NULL_ );
            if(status != CDF_OK)
            {  printf("ERROR: %s\ncouldn't read %s\n", __FILE__, NY_name ); StatusHandler (status);}
         }

         if( CDFlib( CONFIRM_, ATTR_EXISTENCE_, NZ_name, NULL_ ) == CDF_OK )
         {
            status = CDFlib ( GET_, ATTR_NUMBER_, NZ_name, &NZ_cdf_num, NULL_ );
            if(status != CDF_OK)
            {  printf("ERROR: %s\ncouldn't read %s\n", __FILE__, NZ_name ); StatusHandler (status);}
         }

         if( CDFlib( CONFIRM_, ATTR_EXISTENCE_, NY_plus1_name, NULL_ ) == CDF_OK ) /*** theta+1 position array ***/
         {
            status = CDFlib ( GET_, ATTR_NUMBER_, NY_plus1_name, &NY_plus1_cdf_num, NULL_ );
            if(status != CDF_OK)
            {  printf("ERROR: %s\ncouldn't read %s\n", __FILE__, NY_plus1_name ); StatusHandler (status);}
         }

         if( CDFlib( CONFIRM_, ATTR_EXISTENCE_, NZ_plus1_name, NULL_ ) == CDF_OK ) /*** r+1 position array **/
         {
            status = CDFlib ( GET_, ATTR_NUMBER_, NZ_plus1_name, &NZ_plus1_cdf_num, NULL_ );
            if(status != CDF_OK)
            {  printf("ERROR: %s\ncouldn't read %s\n", __FILE__, NZ_plus1_name ); StatusHandler (status);}
         }

         if( DEBUG_FLAG ) printf("%s  DEBUG\t finished getting cdf nums\n", __FILE__ );

         /***************** use cdf_varNums to get actual scalar values ************************************************/

         /********* get value for NX **********/
         status = CDFlib (
         SELECT_, ATTR_, NX_cdf_num,
         SELECT_, gENTRY_, 0,
         GET_, gENTRY_DATA_, &NX,
         NULL_
         );
         if(status != CDF_OK) StatusHandler (status);

         /********* get value for NY **********/
         status = CDFlib (
         SELECT_, ATTR_, NY_cdf_num,
         SELECT_, gENTRY_, 0,
         GET_, gENTRY_DATA_, &NY,
         NULL_
         );
         if(status != CDF_OK) StatusHandler (status);

         /********* get value for NZ **********/
         status = CDFlib (
         SELECT_, ATTR_, NZ_cdf_num,
         SELECT_, gENTRY_, 0,
         GET_, gENTRY_DATA_, &NZ,
         NULL_
         );
         if(status != CDF_OK) StatusHandler (status);

         /********* get value for NY_plus1 **********/
         status = CDFlib (
         SELECT_, ATTR_, NY_plus1_cdf_num,
         SELECT_, gENTRY_, 0,
         GET_, gENTRY_DATA_, &NY_plus1,
         NULL_
         );
         if(status != CDF_OK) StatusHandler (status);

         /********* get value for NZ_plus1 **********/
         status = CDFlib (
         SELECT_, ATTR_, NZ_plus1_cdf_num,
         SELECT_, gENTRY_, 0,
         GET_, gENTRY_DATA_, &NZ_plus1,
         NULL_
         );
         if(status != CDF_OK) StatusHandler (status);

         /*convert original float N* to int n* */
         /* bats NX was float but ucla,ctip,mas are int no conversion needed fix latter*/

         /*** for mas lets use the unchanging phi, and r_N+1 and theta_N+1 grid ***/

         nx = NX;
         ny = NY;
         nz = NZ;

         nx_plus1 = NX_plus1;
         ny_plus1 = NY_plus1;
         nz_plus1 = NZ_plus1;

         if( DEBUG_FLAG )
         {
            printf("DEBUG\tINFO from get_mas_cdf_info\n");
            printf("DEBUG\tNX=%d & nx=%d\n", NX, nx );
            printf("DEBUG\tNY=%d & ny=%d\n", NY, ny );
            printf("DEBUG\tNZ=%d & nz=%d\n", NZ, nz );
            printf("DEBUG\tNY_plus1=%d & ny_plus1=%d\n", NY_plus1, ny_plus1 );
            printf("DEBUG\tNZ_plus1=%d & nz_plus1=%d\n", NZ_plus1, nz_plus1 );
            printf("DEBUG\t main_memory_flag = %d\n", main_memory_flag);
         }

         /************************ allocate space for unique r, r1, phi, theta, & theta1 positions *******/

         if( ( phi_pos = ( float * ) calloc( nx, sizeof( float ) ) ) == NULL )
         {
            printf("\ncalloc failed for phi_pos buffer!\n" );
            exit( EXIT_FAILURE );
         }

         if( ( theta_pos = ( float * ) calloc( ny, sizeof( float ) ) ) == NULL )
         {
            printf("\ncalloc failed for theta_pos buffer!\n" );
            exit( EXIT_FAILURE );
         }

         if( ( r_pos = ( float * ) calloc( nz, sizeof( float ) ) ) == NULL )
         {
            printf("\ncalloc failed for r_pos buffer!\n" );
            exit( EXIT_FAILURE );
         }

         if( ( theta1_pos = ( float * ) calloc( ny_plus1, sizeof( float ) ) ) == NULL )
         {
            printf("\ncalloc failed for theta1_pos buffer!\n" );
            exit( EXIT_FAILURE );
         }

         if( ( r1_pos = ( float * ) calloc( nz_plus1, sizeof( float ) ) ) == NULL )
         {
            printf("\ncalloc failed for r1_pos buffer!\n" );
            exit( EXIT_FAILURE );
         }

         if( DEBUG_FLAG ) printf("DEBUG\tspace allocated for variables...\n");

         /***************************** get unique r,phi,theta, r1, theta1 values ******************************/

         counts[0] = nx;
         indices[0] = 0;
         intervals[0] = 1;

         status = CDFlib (
         SELECT_, zVAR_, phi_cdf_varNum,
         SELECT_, zVAR_RECNUMBER_, recordStart,
         SELECT_, zVAR_RECCOUNT_, recordCount,
         SELECT_, zVAR_RECINTERVAL_, recordInterval,
         SELECT_, zVAR_DIMINDICES_, indices,
         SELECT_, zVAR_DIMCOUNTS_, counts,
         SELECT_, zVAR_DIMINTERVALS_, intervals,
         GET_, zVAR_HYPERDATA_, phi_pos,
         NULL_
         );
         if(status != CDF_OK) StatusHandler (status);

         counts[0] = ny;

         status = CDFlib (
         SELECT_, zVAR_, theta_cdf_varNum,
         SELECT_, zVAR_RECNUMBER_, recordStart,
         SELECT_, zVAR_RECCOUNT_, recordCount,
         SELECT_, zVAR_RECINTERVAL_, recordInterval,
         SELECT_, zVAR_DIMINDICES_, indices,
         SELECT_, zVAR_DIMCOUNTS_, counts,
         SELECT_, zVAR_DIMINTERVALS_, intervals,
         GET_, zVAR_HYPERDATA_, theta_pos,
         NULL_
         );
         if(status != CDF_OK) StatusHandler (status);

         counts[0] = nz;

         status = CDFlib (
         SELECT_, zVAR_, r_cdf_varNum,
         SELECT_, zVAR_RECNUMBER_, recordStart,
         SELECT_, zVAR_RECCOUNT_, recordCount,
         SELECT_, zVAR_RECINTERVAL_, recordInterval,
         SELECT_, zVAR_DIMINDICES_, indices,
         SELECT_, zVAR_DIMCOUNTS_, counts,
         SELECT_, zVAR_DIMINTERVALS_, intervals,
         GET_, zVAR_HYPERDATA_, r_pos,
         NULL_
         );
         if(status != CDF_OK) StatusHandler (status);

         counts[0] = ny_plus1;

         status = CDFlib (
         SELECT_, zVAR_, theta1_cdf_varNum,
         SELECT_, zVAR_RECNUMBER_, recordStart,
         SELECT_, zVAR_RECCOUNT_, recordCount,
         SELECT_, zVAR_RECINTERVAL_, recordInterval,
         SELECT_, zVAR_DIMINDICES_, indices,
         SELECT_, zVAR_DIMCOUNTS_, counts,
         SELECT_, zVAR_DIMINTERVALS_, intervals,
         GET_, zVAR_HYPERDATA_, theta1_pos,
         NULL_
         );
         if(status != CDF_OK) StatusHandler (status);

         counts[0] = nz_plus1;

         status = CDFlib (
         SELECT_, zVAR_, r1_cdf_varNum,
         SELECT_, zVAR_RECNUMBER_, recordStart,
         SELECT_, zVAR_RECCOUNT_, recordCount,
         SELECT_, zVAR_RECINTERVAL_, recordInterval,
         SELECT_, zVAR_DIMINDICES_, indices,
         SELECT_, zVAR_DIMCOUNTS_, counts,
         SELECT_, zVAR_DIMINTERVALS_, intervals,
         GET_, zVAR_HYPERDATA_, r1_pos,
         NULL_
         );
         if(status != CDF_OK) StatusHandler (status);

         return;

      }

int load_mas_cdf_variable_into_main_memory(
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

   int mas_reserve_mem_and_set_cdf_num(
         char *);

   /*printf("%s  DEBUG\tinside load\n", __FILE__ );*/

   variable_cdf_number = mas_reserve_mem_and_set_cdf_num(variable_to_read);

   if ( DEBUG_FLAG)
   {
      printf(
            "DEBUG:\t%s\t\nvariable_to_read = %s\n",
__FILE__      , variable_to_read );
   }

   if( variable_cdf_number == -1 )
   {
      printf("ERROR:\tcould not find cdf variable number for -->%s<--\n", variable_to_read );
      return EXIT_FAILURE; /*exit( EXIT_FAILURE );*/
   }

   if( DEBUG_FLAG )
   {
      printf("DEBUG:\t%s\tvariable_to_read = %s\tvariable_number = %ld\n", __FILE__, variable_to_read, variable_cdf_number );
   }

   /*printf("DEBUG\tback from reserve mem with cdf_varNum %d for variable %s \n", variable_cdf_number, variable_to_read ); */

   counts[0] = nx * ny * nz;
   indices[0] = 0;
   intervals[0] = 1;

   if( DEBUG_FLAG )
   {
      printf("strcmp( %s, %s ) = %d\n", variable_to_read, br_name, strcmp( variable_to_read, br_name ) );
   }

   if ( ! strcmp( variable_to_read, br_name ) )
   {

      counts[0] = nx * ny * nz_plus1;

      if( DEBUG_FLAG )
      {
         printf("INSIDE br name test\tstrcmp( %s, %s ) = %d\n", variable_to_read, br_name, strcmp( variable_to_read, br_name ) );
      }

      status = CDFlib (
      SELECT_, zVAR_, variable_cdf_number,
      SELECT_, zVAR_RECNUMBER_, recordStart,
      SELECT_, zVAR_RECCOUNT_, recordCount,
      SELECT_, zVAR_RECINTERVAL_, recordInterval,
      SELECT_, zVAR_DIMINDICES_, indices,
      SELECT_, zVAR_DIMCOUNTS_, counts,
      SELECT_, zVAR_DIMINTERVALS_, intervals,
      GET_, zVAR_HYPERDATA_, br,
      NULL_
      );
      if(status != CDF_OK) StatusHandler (status);

   }
   else if ( ! strcmp( variable_to_read, bphi_name ) )
   {
      counts[0] = nx * ny * nz;

      if( DEBUG_FLAG ) printf("INSIDE bphi name test\n");

      status = CDFlib (
      SELECT_, zVAR_, variable_cdf_number,
      SELECT_, zVAR_RECNUMBER_, recordStart,
      SELECT_, zVAR_RECCOUNT_, recordCount,
      SELECT_, zVAR_RECINTERVAL_, recordInterval,
      SELECT_, zVAR_DIMINDICES_, indices,
      SELECT_, zVAR_DIMCOUNTS_, counts,
      SELECT_, zVAR_DIMINTERVALS_, intervals,
      GET_, zVAR_HYPERDATA_, bphi,
      NULL_
      );
      if(status != CDF_OK) StatusHandler (status);
   }
   else if ( ! strcmp( variable_to_read, btheta_name ) )
   {
      counts[0] = nx * ny_plus1 * nz;

      if( DEBUG_FLAG ) printf("INSIDE btheta name test\n");

      status = CDFlib (
      SELECT_, zVAR_, variable_cdf_number,
      SELECT_, zVAR_RECNUMBER_, recordStart,
      SELECT_, zVAR_RECCOUNT_, recordCount,
      SELECT_, zVAR_RECINTERVAL_, recordInterval,
      SELECT_, zVAR_DIMINDICES_, indices,
      SELECT_, zVAR_DIMCOUNTS_, counts,
      SELECT_, zVAR_DIMINTERVALS_, intervals,
      GET_, zVAR_HYPERDATA_, btheta,
      NULL_
      );
      if(status != CDF_OK) StatusHandler (status);
   }
   else if ( ! strcmp( variable_to_read, jr_name ) )
   {
      counts[0] = nx * ny_plus1 * nz;

      if( DEBUG_FLAG ) printf("INSIDE jr name test\n");

      status = CDFlib (
      SELECT_, zVAR_, variable_cdf_number,
      SELECT_, zVAR_RECNUMBER_, recordStart,
      SELECT_, zVAR_RECCOUNT_, recordCount,
      SELECT_, zVAR_RECINTERVAL_, recordInterval,
      SELECT_, zVAR_DIMINDICES_, indices,
      SELECT_, zVAR_DIMCOUNTS_, counts,
      SELECT_, zVAR_DIMINTERVALS_, intervals,
      GET_, zVAR_HYPERDATA_, jr,
      NULL_
      );
      if(status != CDF_OK) StatusHandler (status);
   }
   else if ( ! strcmp( variable_to_read, jphi_name ) )
   {
      counts[0] = nx * ny_plus1 * nz_plus1;

      if( DEBUG_FLAG ) printf("INSIDE jphi name test\n");

      status = CDFlib (
      SELECT_, zVAR_, variable_cdf_number,
      SELECT_, zVAR_RECNUMBER_, recordStart,
      SELECT_, zVAR_RECCOUNT_, recordCount,
      SELECT_, zVAR_RECINTERVAL_, recordInterval,
      SELECT_, zVAR_DIMINDICES_, indices,
      SELECT_, zVAR_DIMCOUNTS_, counts,
      SELECT_, zVAR_DIMINTERVALS_, intervals,
      GET_, zVAR_HYPERDATA_, jphi,
      NULL_
      );
      if(status != CDF_OK) StatusHandler (status);
   }
   else if ( ! strcmp( variable_to_read, jtheta_name ) )
   {
      counts[0] = nx * ny * nz_plus1;

      if( DEBUG_FLAG ) printf("INSIDE jtheta name test\n");

      status = CDFlib (
      SELECT_, zVAR_, variable_cdf_number,
      SELECT_, zVAR_RECNUMBER_, recordStart,
      SELECT_, zVAR_RECCOUNT_, recordCount,
      SELECT_, zVAR_RECINTERVAL_, recordInterval,
      SELECT_, zVAR_DIMINDICES_, indices,
      SELECT_, zVAR_DIMCOUNTS_, counts,
      SELECT_, zVAR_DIMINTERVALS_, intervals,
      GET_, zVAR_HYPERDATA_, jtheta,
      NULL_
      );
      if(status != CDF_OK) StatusHandler (status);
   }
   else if ( ! strcmp( variable_to_read, ur_name ) )
   {

      counts[0] = nx * ny_plus1 * nz;

      if( DEBUG_FLAG ) printf("INSIDE ur name test\n");

      status = CDFlib (
      SELECT_, zVAR_, variable_cdf_number,
      SELECT_, zVAR_RECNUMBER_, recordStart,
      SELECT_, zVAR_RECCOUNT_, recordCount,
      SELECT_, zVAR_RECINTERVAL_, recordInterval,
      SELECT_, zVAR_DIMINDICES_, indices,
      SELECT_, zVAR_DIMCOUNTS_, counts,
      SELECT_, zVAR_DIMINTERVALS_, intervals,
      GET_, zVAR_HYPERDATA_, ur,
      NULL_
      );
      if(status != CDF_OK) StatusHandler (status);
   }
   else if ( ! strcmp( variable_to_read, uphi_name ) )
   {
      counts[0] = nx * ny_plus1 * nz_plus1;

      if( DEBUG_FLAG ) printf("INSIDE uphi name test\n");

      status = CDFlib (
      SELECT_, zVAR_, variable_cdf_number,
      SELECT_, zVAR_RECNUMBER_, recordStart,
      SELECT_, zVAR_RECCOUNT_, recordCount,
      SELECT_, zVAR_RECINTERVAL_, recordInterval,
      SELECT_, zVAR_DIMINDICES_, indices,
      SELECT_, zVAR_DIMCOUNTS_, counts,
      SELECT_, zVAR_DIMINTERVALS_, intervals,
      GET_, zVAR_HYPERDATA_, uphi,
      NULL_
      );
      if(status != CDF_OK) StatusHandler (status);
   }
   else if ( ! strcmp( variable_to_read, utheta_name ) )
   {
      counts[0] = nx * ny * nz_plus1;

      if( DEBUG_FLAG ) printf("INSIDE utheta name test\n");

      status = CDFlib (
      SELECT_, zVAR_, variable_cdf_number,
      SELECT_, zVAR_RECNUMBER_, recordStart,
      SELECT_, zVAR_RECCOUNT_, recordCount,
      SELECT_, zVAR_RECINTERVAL_, recordInterval,
      SELECT_, zVAR_DIMINDICES_, indices,
      SELECT_, zVAR_DIMCOUNTS_, counts,
      SELECT_, zVAR_DIMINTERVALS_, intervals,
      GET_, zVAR_HYPERDATA_, utheta,
      NULL_
      );
      if(status != CDF_OK) StatusHandler (status);
   }
   else if ( ! strcmp( variable_to_read, rho_name ) )
   {
      counts[0] = nx * ny_plus1 * nz_plus1;

      if( DEBUG_FLAG ) printf("INSIDE rho name test\n");

      status = CDFlib (
      SELECT_, zVAR_, variable_cdf_number,
      SELECT_, zVAR_RECNUMBER_, recordStart,
      SELECT_, zVAR_RECCOUNT_, recordCount,
      SELECT_, zVAR_RECINTERVAL_, recordInterval,
      SELECT_, zVAR_DIMINDICES_, indices,
      SELECT_, zVAR_DIMCOUNTS_, counts,
      SELECT_, zVAR_DIMINTERVALS_, intervals,
      GET_, zVAR_HYPERDATA_, rho,
      NULL_
      );
      if(status != CDF_OK) StatusHandler (status);
   }
   else if ( ! strcmp( variable_to_read, T_name ) )
   {
      counts[0] = nx * ny_plus1 * nz_plus1;

      if( DEBUG_FLAG ) printf("INSIDE T name test\n");

      status = CDFlib (
      SELECT_, zVAR_, variable_cdf_number,
      SELECT_, zVAR_RECNUMBER_, recordStart,
      SELECT_, zVAR_RECCOUNT_, recordCount,
      SELECT_, zVAR_RECINTERVAL_, recordInterval,
      SELECT_, zVAR_DIMINDICES_, indices,
      SELECT_, zVAR_DIMCOUNTS_, counts,
      SELECT_, zVAR_DIMINTERVALS_, intervals,
      GET_, zVAR_HYPERDATA_, T,
      NULL_
      );
      if(status != CDF_OK) StatusHandler (status);
   }
   else if ( ! strcmp( variable_to_read, p_name ) )
   {
      counts[0] = nx * ny_plus1 * nz_plus1;

      if( DEBUG_FLAG ) printf("INSIDE p name test\n");

      status = CDFlib (
      SELECT_, zVAR_, variable_cdf_number,
      SELECT_, zVAR_RECNUMBER_, recordStart,
      SELECT_, zVAR_RECCOUNT_, recordCount,
      SELECT_, zVAR_RECINTERVAL_, recordInterval,
      SELECT_, zVAR_DIMINDICES_, indices,
      SELECT_, zVAR_DIMCOUNTS_, counts,
      SELECT_, zVAR_DIMINTERVALS_, intervals,
      GET_, zVAR_HYPERDATA_, p,
      NULL_
      );
      if(status != CDF_OK) StatusHandler (status);
   }
   else
   {
      /* INSERT ERROR HANDLER HERE */
      printf("couldn't identify %s variable to load...\n", variable_to_read );
      return EXIT_FAILURE;
   }

   /*printf("DEBUG\tfinished hyper read returning\n");*/

   return EXIT_SUCCESS;

}

int mas_reserve_mem_and_set_cdf_num(
      char *variable_number_to_get)
{

   int buffer_size; /** dynamically set buffer size on the fly based on the variable... **/

   /*printf("DEBUG\tinside resrve mem\n");*/

   /* return cdf variable Number for given variable */

   if ( !strcmp(
         variable_number_to_get,
         br_name) )
   {

      buffer_size = nx * ny * nz_plus1;

      if ( (br = ( float * ) calloc(
            buffer_size,
            sizeof(float)) ) == NULL)
      {
         printf(
               "\ncalloc failed for %s buffer!\n",
               br_name);
         exit( EXIT_FAILURE);
      }
      return br_cdf_varNum;
   }
   else if ( !strcmp(
         variable_number_to_get,
         bphi_name) )
   {
      buffer_size = nx * ny * nz;

      if ( (bphi = ( float * ) calloc(
            buffer_size,
            sizeof(float)) ) == NULL)
      {
         printf(
               "\ncalloc failed for %s buffer!\n",
               bphi_name);
         exit( EXIT_FAILURE);
      }
      return bphi_cdf_varNum;
   }
   else if ( !strcmp(
         variable_number_to_get,
         btheta_name) )
   {
      buffer_size = nx * ny_plus1 * nz;

      if ( (btheta = ( float * ) calloc(
            buffer_size,
            sizeof(float)) ) == NULL)
      {
         printf(
               "\ncalloc failed for %s buffer!\n",
               btheta_name);
         exit( EXIT_FAILURE);
      }
      return btheta_cdf_varNum;
   }
   if ( !strcmp(
         variable_number_to_get,
         jr_name) )
   {
      buffer_size = nx * ny_plus1 * nz;

      if ( (jr = ( float * ) calloc(
            buffer_size,
            sizeof(float)) ) == NULL)
      {
         printf(
               "\ncalloc failed for %s buffer!\n",
               jr_name);
         exit( EXIT_FAILURE);
      }
      return jr_cdf_varNum;
   }
   else if ( !strcmp(
         variable_number_to_get,
         jphi_name) )
   {
      buffer_size = nx * ny_plus1 * nz_plus1;

      if ( (jphi = ( float * ) calloc(
            buffer_size,
            sizeof(float)) ) == NULL)
      {
         printf(
               "\ncalloc failed for %s buffer!\n",
               jphi_name);
         exit( EXIT_FAILURE);
      }
      return jphi_cdf_varNum;
   }
   else if ( !strcmp(
         variable_number_to_get,
         jtheta_name) )
   {
      buffer_size = nx * ny * nz_plus1;

      if ( (jtheta = ( float * ) calloc(
            buffer_size,
            sizeof(float)) ) == NULL)
      {
         printf(
               "\ncalloc failed for %s buffer!\n",
               jtheta_name);
         exit( EXIT_FAILURE);
      }
      return jtheta_cdf_varNum;
   }
   else if ( !strcmp(
         variable_number_to_get,
         ur_name) )
   {
      buffer_size = nx * ny_plus1 * nz;

      if ( (ur = ( float * ) calloc(
            buffer_size,
            sizeof(float)) ) == NULL)
      {
         printf(
               "\ncalloc failed for %s buffer!\n",
               ur_name);
         exit( EXIT_FAILURE);
      }
      return ur_cdf_varNum;
   }
   else if ( !strcmp(
         variable_number_to_get,
         uphi_name) )
   {
      buffer_size = nx * ny_plus1 * nz_plus1;

      if ( (uphi = ( float * ) calloc(
            buffer_size,
            sizeof(float)) ) == NULL)
      {
         printf(
               "\ncalloc failed for %s buffer!\n",
               uphi_name);
         exit( EXIT_FAILURE);
      }
      return uphi_cdf_varNum;
   }
   else if ( !strcmp(
         variable_number_to_get,
         utheta_name) )
   {
      buffer_size = nx * ny * nz_plus1;

      if ( (utheta = ( float * ) calloc(
            buffer_size,
            sizeof(float)) ) == NULL)
      {
         printf(
               "\ncalloc failed for %s buffer!\n",
               utheta_name);
         exit( EXIT_FAILURE);
      }
      return utheta_cdf_varNum;
   }
   else if ( !strcmp(
         variable_number_to_get,
         rho_name) )
   {
      buffer_size = nx * ny_plus1 * nz_plus1;

      if ( (rho = ( float * ) calloc(
            buffer_size,
            sizeof(float)) ) == NULL)
      {
         printf(
               "\ncalloc failed for %s buffer!\n",
               rho_name);
         exit( EXIT_FAILURE);
      }
      return rho_cdf_varNum;
   }
   else if ( !strcmp(
         variable_number_to_get,
         p_name) )
   {
      buffer_size = nx * ny_plus1 * nz_plus1;

      if ( (p = ( float * ) calloc(
            buffer_size,
            sizeof(float)) ) == NULL)
      {
         printf(
               "\ncalloc failed for %s buffer!\n",
               p_name);
         exit( EXIT_FAILURE);
      }
      return p_cdf_varNum;
   }
   else if ( !strcmp(
         variable_number_to_get,
         T_name) )
   {
      buffer_size = nx * ny_plus1 * nz_plus1;

      if ( (T = ( float * ) calloc(
            buffer_size,
            sizeof(float)) ) == NULL)
      {
         printf(
               "\ncalloc failed for %s buffer!\n",
               T_name);
         exit( EXIT_FAILURE);
      }
      return T_cdf_varNum;
   }
   else
   {
      return -1;
   }
   /*else { printf("ERROR:\tcould not find cdf variable number for %s\n", variable ); return 0; }*/

}

void allocate_defaults_mas(
      void)
{
   /************ we need to allocate space for all varoiables even when no main mem option is set
    * this allows the all inclusing close to work properly as it tries to free all variables.  We only need
    * to hold a enough space for a single value ******/

   if ( (br = ( float * ) calloc(
         1,
         sizeof(float)) ) == NULL)
   {
      printf(
            "\nallocate_defaults_mas calloc failed for %s buffer!\n",
            br_name);
      exit( EXIT_FAILURE);
   }
   if ( (bphi = ( float * ) calloc(
         1,
         sizeof(float)) ) == NULL)
   {
      printf(
            "\nallocate_defaults_mas calloc failed for %s buffer!\n",
            bphi_name);
      exit( EXIT_FAILURE);
   }
   if ( (btheta = ( float * ) calloc(
         1,
         sizeof(float)) ) == NULL)
   {
      printf(
            "\nallocate_defaults_mas calloc failed for %s buffer!\n",
            btheta_name);
      exit( EXIT_FAILURE);
   }
   if ( (jr = ( float * ) calloc(
         1,
         sizeof(float)) ) == NULL)
   {
      printf(
            "\nallocate_defaults_mas calloc failed for %s buffer!\n",
            jr_name);
      exit( EXIT_FAILURE);
   }
   if ( (jphi = ( float * ) calloc(
         1,
         sizeof(float)) ) == NULL)
   {
      printf(
            "\nallocate_defaults_mas calloc failed for %s buffer!\n",
            jphi_name);
      exit( EXIT_FAILURE);
   }
   if ( (jtheta = ( float * ) calloc(
         1,
         sizeof(float)) ) == NULL)
   {
      printf(
            "\nallocate_defaults_mas calloc failed for %s buffer!\n",
            jtheta_name);
      exit( EXIT_FAILURE);
   }
   if ( (ur = ( float * ) calloc(
         1,
         sizeof(float)) ) == NULL)
   {
      printf(
            "\nallocate_defaults_mas calloc failed for %s buffer!\n",
            ur_name);
      exit( EXIT_FAILURE);
   }
   if ( (uphi = ( float * ) calloc(
         1,
         sizeof(float)) ) == NULL)
   {
      printf(
            "\nallocate_defaults_mas calloc failed for %s buffer!\n",
            uphi_name);
      exit( EXIT_FAILURE);
   }
   if ( (utheta = ( float * ) calloc(
         1,
         sizeof(float)) ) == NULL)
   {
      printf(
            "\nallocate_defaults_mas calloc failed for %s buffer!\n",
            utheta_name);
      exit( EXIT_FAILURE);
   }
   if ( (rho = ( float * ) calloc(
         1,
         sizeof(float)) ) == NULL)
   {
      printf(
            "\nallocate_defaults_mas calloc failed for %s buffer!\n",
            rho_name);
      exit( EXIT_FAILURE);
   }
   if ( (p = ( float * ) calloc(
         1,
         sizeof(float)) ) == NULL)
   {
      printf(
            "\nallocate_defaults_mas calloc failed for %s buffer!\n",
            p_name);
      exit( EXIT_FAILURE);
   }
   if ( (T = ( float * ) calloc(
         1,
         sizeof(float)) ) == NULL)
   {
      printf(
            "\nallocate_defaults_mas calloc failed for %s buffer!\n",
            T_name);
      exit( EXIT_FAILURE);
   }

   return;
}

