/*******************************************************************************
 *                                                                             *
 *    NAME:          get_ctip_cdf_info.c                                       *
 *                                                                             *
 *    VERSION:       1.0                                                       *
 *                                                                             *
 *    AUTHOR:        Marlo M. H. Maddox                                        *
 *                   NASA-GSFC-AETD-ISD-ADMA(587)-CCMC                         *
 *                   Marlo.Maddox@nasa.gov                                     *
 *                   (301) 286 - 5202                                          *
 *                                                                             *
 *    PURPOSE:       Extracts all relevant information for identifying         *
 *                   variables etc from the ctip cdf file.  This routine is    *
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
 *    08.08.2005     Maddox, Marlo                                             *
 *                   Initial development started                               *
 *                                                                             *
 *    07.20.2006     Maddox, Marlo                                             *
 *                   Allocate Defaults allocate_defaults_c() funtion added     *
 *                                                                             *
 *    07.25.2006     Maddox, Marlo                                             *
 *                   Adding a check cdf check CONFIRM zVAR_EXISTENCE for       *
 *                   all variables before getting numbers as well as CONFIRM   *
 *                   ATTR_EXISTENCE for corresponding N* values                *
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
#include "ctip_cdf_interface_variables.h"
#include "cdf.h"

void get_ctip_cdf_info(
      int main_memory_flag)
{

   void allocate_defaults_c(
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

   allocate_defaults_c();

   /** get all of the cdf attribute/variable numbers that will be needed ******/

   if (CDFlib( CONFIRM_, zVAR_EXISTENCE_, x_name, NULL_) == CDF_OK)
   {
      status = CDFlib( GET_, zVAR_NUMBER_, x_name, &x_cdf_varNum, NULL_);
      if (status != CDF_OK)
         StatusHandler(status);
   }
   if (CDFlib( CONFIRM_, zVAR_EXISTENCE_, y_name, NULL_) == CDF_OK)
   {
      status = CDFlib( GET_, zVAR_NUMBER_, y_name, &y_cdf_varNum, NULL_);
      if (status != CDF_OK)
         StatusHandler(status);
   }
   if (CDFlib( CONFIRM_, zVAR_EXISTENCE_, z_name, NULL_) == CDF_OK)
   {
      status = CDFlib( GET_, zVAR_NUMBER_, z_name, &z_cdf_varNum, NULL_);
      if (status != CDF_OK)
         StatusHandler(status);
   }
   if (CDFlib( CONFIRM_, zVAR_EXISTENCE_, H_name, NULL_) == CDF_OK)
   {
      status = CDFlib( GET_, zVAR_NUMBER_, H_name, &H_cdf_varNum, NULL_);
      if (status != CDF_OK)
         StatusHandler(status);
   }
   if (CDFlib( CONFIRM_, zVAR_EXISTENCE_, Vn_x_name, NULL_) == CDF_OK)
   {
      status = CDFlib( GET_, zVAR_NUMBER_, Vn_x_name, &Vn_x_cdf_varNum, NULL_);
      if (status != CDF_OK)
         StatusHandler(status);
   }
   if (CDFlib( CONFIRM_, zVAR_EXISTENCE_, Vn_y_name, NULL_) == CDF_OK)
   {
      status = CDFlib( GET_, zVAR_NUMBER_, Vn_y_name, &Vn_y_cdf_varNum, NULL_);
      if (status != CDF_OK)
         StatusHandler(status);
   }
   if (CDFlib( CONFIRM_, zVAR_EXISTENCE_, Vn_z_name, NULL_) == CDF_OK)
   {
      status = CDFlib( GET_, zVAR_NUMBER_, Vn_z_name, &Vn_z_cdf_varNum, NULL_);
      if (status != CDF_OK)
         StatusHandler(status);
   }
   if (CDFlib( CONFIRM_, zVAR_EXISTENCE_, Vi_x_name, NULL_) == CDF_OK)
   {
      status = CDFlib( GET_, zVAR_NUMBER_, Vi_x_name, &Vi_x_cdf_varNum, NULL_);
      if (status != CDF_OK)
         StatusHandler(status);
   }
   if (CDFlib( CONFIRM_, zVAR_EXISTENCE_, Vi_y_name, NULL_) == CDF_OK)
   {
      status = CDFlib( GET_, zVAR_NUMBER_, Vi_y_name, &Vi_y_cdf_varNum, NULL_);
      if (status != CDF_OK)
         StatusHandler(status);
   }
   if (CDFlib( CONFIRM_, zVAR_EXISTENCE_, N_e_name, NULL_) == CDF_OK)
   {
      status = CDFlib( GET_, zVAR_NUMBER_, N_e_name, &N_e_cdf_varNum, NULL_);
      if (status != CDF_OK)
         StatusHandler(status);
   }
   if (CDFlib( CONFIRM_, zVAR_EXISTENCE_, N_O_name, NULL_) == CDF_OK)
   {
      status = CDFlib( GET_, zVAR_NUMBER_, N_O_name, &N_O_cdf_varNum, NULL_);
      if (status != CDF_OK)
         StatusHandler(status);
   }
   if (CDFlib( CONFIRM_, zVAR_EXISTENCE_, N_O2_name, NULL_) == CDF_OK)
   {
      status = CDFlib( GET_, zVAR_NUMBER_, N_O2_name, &N_O2_cdf_varNum, NULL_);
      if (status != CDF_OK)
         StatusHandler(status);
   }
   if (CDFlib( CONFIRM_, zVAR_EXISTENCE_, N_N2_name, NULL_) == CDF_OK)
   {
      status = CDFlib( GET_, zVAR_NUMBER_, N_N2_name, &N_N2_cdf_varNum, NULL_);
      if (status != CDF_OK)
         StatusHandler(status);
   }
   if (CDFlib( CONFIRM_, zVAR_EXISTENCE_, Rmt_name, NULL_) == CDF_OK)
   {
      status = CDFlib( GET_, zVAR_NUMBER_, Rmt_name, &Rmt_cdf_varNum, NULL_);
      if (status != CDF_OK)
         StatusHandler(status);
   }
   if (CDFlib( CONFIRM_, zVAR_EXISTENCE_, Tn_name, NULL_) == CDF_OK)
   {
      status = CDFlib( GET_, zVAR_NUMBER_, Tn_name, &Tn_cdf_varNum, NULL_);
      if (status != CDF_OK)
         StatusHandler(status);
   }
   if (CDFlib( CONFIRM_, zVAR_EXISTENCE_, sigmaP_name, NULL_) == CDF_OK)
   {
      status
            = CDFlib( GET_, zVAR_NUMBER_, sigmaP_name, &sigmaP_cdf_varNum, NULL_);
      if (status != CDF_OK)
         StatusHandler(status);
   }
   if (CDFlib( CONFIRM_, zVAR_EXISTENCE_, sigmaH_name, NULL_) == CDF_OK)
   {
      status
            = CDFlib( GET_, zVAR_NUMBER_, sigmaH_name, &sigmaH_cdf_varNum, NULL_);
      if (status != CDF_OK)
         StatusHandler(status);
   }

   /***  NOTE THESE ARE ATTRIBUTES SO USE CONFRIM ATTR_EXISTENCE... ***/

   if (CDFlib( CONFIRM_, ATTR_EXISTENCE_, NX_name, NULL_) == CDF_OK)
   {
      status = CDFlib( GET_, ATTR_NUMBER_, NX_name, &NX_cdf_num, NULL_);
      if (status != CDF_OK)
         StatusHandler(status);
   }
   if (CDFlib( CONFIRM_, ATTR_EXISTENCE_, NY_name, NULL_) == CDF_OK)
   {
      status = CDFlib( GET_, ATTR_NUMBER_, NY_name, &NY_cdf_num, NULL_);
      if (status != CDF_OK)
         StatusHandler(status);
   }
   if (CDFlib( CONFIRM_, ATTR_EXISTENCE_, NZ_name, NULL_) == CDF_OK)
   {
      status = CDFlib( GET_, ATTR_NUMBER_, NZ_name, &NZ_cdf_num, NULL_);
      if (status != CDF_OK)
         StatusHandler(status);
   }
   if (CDFlib( CONFIRM_, ATTR_EXISTENCE_, NH_name, NULL_) == CDF_OK)
   {
      status = CDFlib( GET_, ATTR_NUMBER_, NH_name, &NH_cdf_num, NULL_);
      if (status != CDF_OK)
         StatusHandler(status);
   }

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

   /********* we would normally get NH here but lets just compute it from NX * NY * NZ ****/
   NH = NX * NY * NZ;

   /*convert original float N* to int n* */
   /* bats NX was float but ucla & ctip is int no conversion needed fix latter*/

   nx = NX;
   ny = NY;
   nz = NZ;
   nh = NH;

   /*
    printf("DEBUG\tINFO from get_ctip+cdf_info\n");
    printf("DEBUG\tNX=%d & nx=%d\n", NX, nx );
    printf("DEBUG\tNY=%d & ny=%d\n", NY, ny );
    printf("DEBUG\tNZ=%d & nz=%d\n", NZ, nz );
    printf("DEBUG\tNH=%d & nh=%d\n", NH, nh );

    printf("DEBUG\t main_memory_flag = %d\n", main_memory_flag);
    */

   /************************ allocate space for unique x, y, & z positions ****/
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
      printf("\ncalloc failed for z_pos buffer!\n");
      exit( EXIT_FAILURE);
   }
   if ( (H_pos = ( float * ) calloc(
         nh,
         sizeof(float)) ) == NULL)
   {
      printf("\ncalloc failed for H_pos buffer!\n");
      exit( EXIT_FAILURE);
   }

   /*printf("DEBUG\tspace allocated for variables...\n");    */

   /***************************** get unique x,y,z,h values *******************/

   counts[0] = nx;
   indices[0] = 0;
   intervals[0] = 1;

   status = CDFlib(
   SELECT_, zVAR_, x_cdf_varNum,
   SELECT_, zVAR_RECNUMBER_, recordStart,
   SELECT_, zVAR_RECCOUNT_, recordCount,
   SELECT_, zVAR_RECINTERVAL_, recordInterval,
   SELECT_, zVAR_DIMINDICES_, indices,
   SELECT_, zVAR_DIMCOUNTS_, counts,
   SELECT_, zVAR_DIMINTERVALS_, intervals,
   GET_, zVAR_HYPERDATA_, x_pos,
   NULL_);
   if (status != CDF_OK)
      StatusHandler(status);

   counts[0] = ny;

   status = CDFlib(
   SELECT_, zVAR_, y_cdf_varNum,
   SELECT_, zVAR_RECNUMBER_, recordStart,
   SELECT_, zVAR_RECCOUNT_, recordCount,
   SELECT_, zVAR_RECINTERVAL_, recordInterval,
   SELECT_, zVAR_DIMINDICES_, indices,
   SELECT_, zVAR_DIMCOUNTS_, counts,
   SELECT_, zVAR_DIMINTERVALS_, intervals,
   GET_, zVAR_HYPERDATA_, y_pos,
   NULL_);
   if (status != CDF_OK)
      StatusHandler(status);

   counts[0] = nz;

   status = CDFlib(
   SELECT_, zVAR_, z_cdf_varNum,
   SELECT_, zVAR_RECNUMBER_, recordStart,
   SELECT_, zVAR_RECCOUNT_, recordCount,
   SELECT_, zVAR_RECINTERVAL_, recordInterval,
   SELECT_, zVAR_DIMINDICES_, indices,
   SELECT_, zVAR_DIMCOUNTS_, counts,
   SELECT_, zVAR_DIMINTERVALS_, intervals,
   GET_, zVAR_HYPERDATA_, z_pos,
   NULL_);
   if (status != CDF_OK)
      StatusHandler(status);

   counts[0] = nh;

   status = CDFlib(
   SELECT_, zVAR_, H_cdf_varNum,
   SELECT_, zVAR_RECNUMBER_, recordStart,
   SELECT_, zVAR_RECCOUNT_, recordCount,
   SELECT_, zVAR_RECINTERVAL_, recordInterval,
   SELECT_, zVAR_DIMINDICES_, indices,
   SELECT_, zVAR_DIMCOUNTS_, counts,
   SELECT_, zVAR_DIMINTERVALS_, intervals,
   GET_, zVAR_HYPERDATA_, H_pos,
   NULL_);
   if (status != CDF_OK)
      StatusHandler(status);

   /****** more debug repoting ******/
   /** print first few values for position variables **/
   /*
    for( i = 0; i < 30; i++ )
    {
    printf( "INDEX-------------- %d\nx_pos[%d] = %f\ny_pos[%d] = %f\nz_pos[%d] = %f\nH_pos[%d] = %f\n\n", i, i, x_pos[i], i, y_pos[i], i, z_pos[i], i, H_pos[i] );
    }
    */
}

int load_ctip_cdf_variable_into_main_memory(
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

   int ctip_reserve_mem_and_set_cdf_num(
         char *);

   /*printf("DEBUG\tinside load\n");*/

   variable_cdf_number = ctip_reserve_mem_and_set_cdf_num(variable_to_read);

   if (variable_cdf_number == -1)
   {
      printf(
            "ERROR:\tcould not find cdf variable number for -->%s<--\n",
            variable_to_read);
      return EXIT_FAILURE; /*exit( EXIT_FAILURE );*/
   }

   /*(printf("DEBUG\tback from reserve mem with cdf_varNum %d for variable %s \n", variable_cdf_number, variable_to_read );*/

   counts[0] = nx * ny * nz;
   indices[0] = 0;
   intervals[0] = 1;

   if ( !strcmp(
         variable_to_read,
         Vn_x_name) )
   {
      status = CDFlib(
      SELECT_, zVAR_, variable_cdf_number,
      SELECT_, zVAR_RECNUMBER_, recordStart,
      SELECT_, zVAR_RECCOUNT_, recordCount,
      SELECT_, zVAR_RECINTERVAL_, recordInterval,
      SELECT_, zVAR_DIMINDICES_, indices,
      SELECT_, zVAR_DIMCOUNTS_, counts,
      SELECT_, zVAR_DIMINTERVALS_, intervals,
      GET_, zVAR_HYPERDATA_, Vn_x,
      NULL_);
      if (status != CDF_OK)
         StatusHandler(status);
   }
   else if ( !strcmp(
         variable_to_read,
         Vn_y_name) )
   {
      status = CDFlib(
      SELECT_, zVAR_, variable_cdf_number,
      SELECT_, zVAR_RECNUMBER_, recordStart,
      SELECT_, zVAR_RECCOUNT_, recordCount,
      SELECT_, zVAR_RECINTERVAL_, recordInterval,
      SELECT_, zVAR_DIMINDICES_, indices,
      SELECT_, zVAR_DIMCOUNTS_, counts,
      SELECT_, zVAR_DIMINTERVALS_, intervals,
      GET_, zVAR_HYPERDATA_, Vn_y,
      NULL_);
      if (status != CDF_OK)
         StatusHandler(status);
   }
   else if ( !strcmp(
         variable_to_read,
         Vn_z_name) )
   {
      status = CDFlib(
      SELECT_, zVAR_, variable_cdf_number,
      SELECT_, zVAR_RECNUMBER_, recordStart,
      SELECT_, zVAR_RECCOUNT_, recordCount,
      SELECT_, zVAR_RECINTERVAL_, recordInterval,
      SELECT_, zVAR_DIMINDICES_, indices,
      SELECT_, zVAR_DIMCOUNTS_, counts,
      SELECT_, zVAR_DIMINTERVALS_, intervals,
      GET_, zVAR_HYPERDATA_, Vn_z,
      NULL_);
      if (status != CDF_OK)
         StatusHandler(status);
   }
   else if ( !strcmp(
         variable_to_read,
         Vi_x_name) )
   {
      status = CDFlib(
      SELECT_, zVAR_, variable_cdf_number,
      SELECT_, zVAR_RECNUMBER_, recordStart,
      SELECT_, zVAR_RECCOUNT_, recordCount,
      SELECT_, zVAR_RECINTERVAL_, recordInterval,
      SELECT_, zVAR_DIMINDICES_, indices,
      SELECT_, zVAR_DIMCOUNTS_, counts,
      SELECT_, zVAR_DIMINTERVALS_, intervals,
      GET_, zVAR_HYPERDATA_, Vi_x,
      NULL_);
      if (status != CDF_OK)
         StatusHandler(status);
   }
   else if ( !strcmp(
         variable_to_read,
         Vi_y_name) )
   {
      status = CDFlib(
      SELECT_, zVAR_, variable_cdf_number,
      SELECT_, zVAR_RECNUMBER_, recordStart,
      SELECT_, zVAR_RECCOUNT_, recordCount,
      SELECT_, zVAR_RECINTERVAL_, recordInterval,
      SELECT_, zVAR_DIMINDICES_, indices,
      SELECT_, zVAR_DIMCOUNTS_, counts,
      SELECT_, zVAR_DIMINTERVALS_, intervals,
      GET_, zVAR_HYPERDATA_, Vi_y,
      NULL_);
      if (status != CDF_OK)
         StatusHandler(status);
   }
   else if ( !strcmp(
         variable_to_read,
         N_e_name) )
   {
      status = CDFlib(
      SELECT_, zVAR_, variable_cdf_number,
      SELECT_, zVAR_RECNUMBER_, recordStart,
      SELECT_, zVAR_RECCOUNT_, recordCount,
      SELECT_, zVAR_RECINTERVAL_, recordInterval,
      SELECT_, zVAR_DIMINDICES_, indices,
      SELECT_, zVAR_DIMCOUNTS_, counts,
      SELECT_, zVAR_DIMINTERVALS_, intervals,
      GET_, zVAR_HYPERDATA_, N_e,
      NULL_);
      if (status != CDF_OK)
         StatusHandler(status);
   }
   else if ( !strcmp(
         variable_to_read,
         N_O_name) )
   {
      status = CDFlib(
      SELECT_, zVAR_, variable_cdf_number,
      SELECT_, zVAR_RECNUMBER_, recordStart,
      SELECT_, zVAR_RECCOUNT_, recordCount,
      SELECT_, zVAR_RECINTERVAL_, recordInterval,
      SELECT_, zVAR_DIMINDICES_, indices,
      SELECT_, zVAR_DIMCOUNTS_, counts,
      SELECT_, zVAR_DIMINTERVALS_, intervals,
      GET_, zVAR_HYPERDATA_, N_O,
      NULL_);
      if (status != CDF_OK)
         StatusHandler(status);
   }
   else if ( !strcmp(
         variable_to_read,
         N_O2_name) )
   {
      status = CDFlib(
      SELECT_, zVAR_, variable_cdf_number,
      SELECT_, zVAR_RECNUMBER_, recordStart,
      SELECT_, zVAR_RECCOUNT_, recordCount,
      SELECT_, zVAR_RECINTERVAL_, recordInterval,
      SELECT_, zVAR_DIMINDICES_, indices,
      SELECT_, zVAR_DIMCOUNTS_, counts,
      SELECT_, zVAR_DIMINTERVALS_, intervals,
      GET_, zVAR_HYPERDATA_, N_O2,
      NULL_);
      if (status != CDF_OK)
         StatusHandler(status);
   }
   else if ( !strcmp(
         variable_to_read,
         N_N2_name) )
   {
      status = CDFlib(
      SELECT_, zVAR_, variable_cdf_number,
      SELECT_, zVAR_RECNUMBER_, recordStart,
      SELECT_, zVAR_RECCOUNT_, recordCount,
      SELECT_, zVAR_RECINTERVAL_, recordInterval,
      SELECT_, zVAR_DIMINDICES_, indices,
      SELECT_, zVAR_DIMCOUNTS_, counts,
      SELECT_, zVAR_DIMINTERVALS_, intervals,
      GET_, zVAR_HYPERDATA_, N_N2,
      NULL_);
      if (status != CDF_OK)
         StatusHandler(status);
   }
   else if ( !strcmp(
         variable_to_read,
         Rmt_name) )
   {
      status = CDFlib(
      SELECT_, zVAR_, variable_cdf_number,
      SELECT_, zVAR_RECNUMBER_, recordStart,
      SELECT_, zVAR_RECCOUNT_, recordCount,
      SELECT_, zVAR_RECINTERVAL_, recordInterval,
      SELECT_, zVAR_DIMINDICES_, indices,
      SELECT_, zVAR_DIMCOUNTS_, counts,
      SELECT_, zVAR_DIMINTERVALS_, intervals,
      GET_, zVAR_HYPERDATA_, Rmt,
      NULL_);
      if (status != CDF_OK)
         StatusHandler(status);
   }
   else if ( !strcmp(
         variable_to_read,
         Tn_name) )
   {
      status = CDFlib(
      SELECT_, zVAR_, variable_cdf_number,
      SELECT_, zVAR_RECNUMBER_, recordStart,
      SELECT_, zVAR_RECCOUNT_, recordCount,
      SELECT_, zVAR_RECINTERVAL_, recordInterval,
      SELECT_, zVAR_DIMINDICES_, indices,
      SELECT_, zVAR_DIMCOUNTS_, counts,
      SELECT_, zVAR_DIMINTERVALS_, intervals,
      GET_, zVAR_HYPERDATA_, Tn,
      NULL_);
      if (status != CDF_OK)
         StatusHandler(status);
   }
   else if ( !strcmp(
         variable_to_read,
         sigmaP_name) )
   {
      status = CDFlib(
      SELECT_, zVAR_, variable_cdf_number,
      SELECT_, zVAR_RECNUMBER_, recordStart,
      SELECT_, zVAR_RECCOUNT_, recordCount,
      SELECT_, zVAR_RECINTERVAL_, recordInterval,
      SELECT_, zVAR_DIMINDICES_, indices,
      SELECT_, zVAR_DIMCOUNTS_, counts,
      SELECT_, zVAR_DIMINTERVALS_, intervals,
      GET_, zVAR_HYPERDATA_, sigmaP,
      NULL_);
      if (status != CDF_OK)
         StatusHandler(status);
   }
   else if ( !strcmp(
         variable_to_read,
         sigmaH_name) )
   {
      status = CDFlib(
      SELECT_, zVAR_, variable_cdf_number,
      SELECT_, zVAR_RECNUMBER_, recordStart,
      SELECT_, zVAR_RECCOUNT_, recordCount,
      SELECT_, zVAR_RECINTERVAL_, recordInterval,
      SELECT_, zVAR_DIMINDICES_, indices,
      SELECT_, zVAR_DIMCOUNTS_, counts,
      SELECT_, zVAR_DIMINTERVALS_, intervals,
      GET_, zVAR_HYPERDATA_, sigmaH,
      NULL_);
      if (status != CDF_OK)
         StatusHandler(status);
   }
   else if ( !strcmp(
         variable_to_read,
         H_name) )
   {
      /*** don't need to load as H is already in memory ***/
   }
   else
   {
      /* INSERT ERROR HANDLER HERE */
      printf("couldn't find a variable to load...\n");
      return EXIT_FAILURE;
   }

   /*printf("DEBUG\tfinished hyper read returning\n");*/

   return EXIT_SUCCESS;

}

int ctip_reserve_mem_and_set_cdf_num(
      char *variable_number_to_get)
{
   int buffer_size = nx * ny * nz;

   /*printf("DEBUG\tinside resrve mem\n");*/

   /* return cdf variable Number for given variable */

   if ( !strcmp(
         variable_number_to_get,
         Vn_x_name) )
   {
      if ( (Vn_x = ( float * ) calloc(
            buffer_size,
            sizeof(float)) ) == NULL)
      {
         printf(
               "\ncalloc failed for %s buffer!\n",
               Vn_x_name);
         exit( EXIT_FAILURE);
      }
      return Vn_x_cdf_varNum;
   }
   else if ( !strcmp(
         variable_number_to_get,
         Vn_y_name) )
   {
      if ( (Vn_y = ( float * ) calloc(
            buffer_size,
            sizeof(float)) ) == NULL)
      {
         printf(
               "\ncalloc failed for %s buffer!\n",
               Vn_y_name);
         exit( EXIT_FAILURE);
      }
      return Vn_y_cdf_varNum;
   }
   else if ( !strcmp(
         variable_number_to_get,
         Vn_z_name) )
   {
      if ( (Vn_z = ( float * ) calloc(
            buffer_size,
            sizeof(float)) ) == NULL)
      {
         printf(
               "\ncalloc failed for %s buffer!\n",
               Vn_z_name);
         exit( EXIT_FAILURE);
      }
      return Vn_z_cdf_varNum;
   }
   else if ( !strcmp(
         variable_number_to_get,
         Vi_x_name) )
   {
      if ( (Vi_x = ( float * ) calloc(
            buffer_size,
            sizeof(float)) ) == NULL)
      {
         printf(
               "\ncalloc failed for %s buffer!\n",
               Vi_x_name);
         exit( EXIT_FAILURE);
      }
      return Vi_x_cdf_varNum;
   }
   else if ( !strcmp(
         variable_number_to_get,
         Vi_y_name) )
   {
      if ( (Vi_y = ( float * ) calloc(
            buffer_size,
            sizeof(float)) ) == NULL)
      {
         printf(
               "\ncalloc failed for %s buffer!\n",
               Vi_y_name);
         exit( EXIT_FAILURE);
      }
      return Vi_y_cdf_varNum;
   }
   else if ( !strcmp(
         variable_number_to_get,
         N_e_name) )
   {
      if ( (N_e = ( float * ) calloc(
            buffer_size,
            sizeof(float)) ) == NULL)
      {
         printf(
               "\ncalloc failed for %s buffer!\n",
               N_e_name);
         exit( EXIT_FAILURE);
      }
      return N_e_cdf_varNum;
   }
   else if ( !strcmp(
         variable_number_to_get,
         N_O_name) )
   {
      if ( (N_O = ( float * ) calloc(
            buffer_size,
            sizeof(float)) ) == NULL)
      {
         printf(
               "\ncalloc failed for %s buffer!\n",
               N_O_name);
         exit( EXIT_FAILURE);
      }
      return N_O_cdf_varNum;
   }
   else if ( !strcmp(
         variable_number_to_get,
         N_O2_name) )
   {
      if ( (N_O2 = ( float * ) calloc(
            buffer_size,
            sizeof(float)) ) == NULL)
      {
         printf(
               "\ncalloc failed for %s buffer!\n",
               N_O2_name);
         exit( EXIT_FAILURE);
      }
      return N_O2_cdf_varNum;
   }
   else if ( !strcmp(
         variable_number_to_get,
         N_N2_name) )
   {
      if ( (N_N2 = ( float * ) calloc(
            buffer_size,
            sizeof(float)) ) == NULL)
      {
         printf(
               "\ncalloc failed for %s buffer!\n",
               N_N2_name);
         exit( EXIT_FAILURE);
      }
      return N_N2_cdf_varNum;
   }
   else if ( !strcmp(
         variable_number_to_get,
         Rmt_name) )
   {
      if ( (Rmt = ( float * ) calloc(
            buffer_size,
            sizeof(float)) ) == NULL)
      {
         printf(
               "\ncalloc failed for %s buffer!\n",
               Rmt_name);
         exit( EXIT_FAILURE);
      }
      return Rmt_cdf_varNum;
   }
   else if ( !strcmp(
         variable_number_to_get,
         Tn_name) )
   {
      if ( (Tn = ( float * ) calloc(
            buffer_size,
            sizeof(float)) ) == NULL)
      {
         printf(
               "\ncalloc failed for %s buffer!\n",
               Tn_name);
         exit( EXIT_FAILURE);
      }
      return Tn_cdf_varNum;
   }
   else if ( !strcmp(
         variable_number_to_get,
         sigmaP_name) )
   {
      if ( (sigmaP = ( float * ) calloc(
            buffer_size,
            sizeof(float)) ) == NULL)
      {
         printf(
               "\ncalloc failed for %s buffer!\n",
               sigmaP_name);
         exit( EXIT_FAILURE);
      }
      return sigmaP_cdf_varNum;
   }
   else if ( !strcmp(
         variable_number_to_get,
         sigmaH_name) )
   {
      if ( (sigmaH = ( float * ) calloc(
            buffer_size,
            sizeof(float)) ) == NULL)
      {
         printf(
               "\ncalloc failed for %s buffer!\n",
               sigmaH_name);
         exit( EXIT_FAILURE);
      }
      return sigmaH_cdf_varNum;
   }
   else if ( !strcmp(
         variable_number_to_get,
         H_name) )
   {
      /** Don't need to allocate memory as position variables are set automatically on open **/

      return H_cdf_varNum;
   }
   else
   {
      return -1;
   }
   /*else { printf("ERROR:\tcould not find cdf variable number for %s\n", variable ); return 0; }*/

}

void allocate_defaults_c(
      void)
{
   /************ we need to allocate space for all varoiables even when no main mem option is set
    * this allows the all inclusing close to work properly as it tries to free all variables.  We only need
    * to hold a enough space for a single value ******/

   if ( (Vn_x = ( float * ) calloc(
         1,
         sizeof(float)) ) == NULL)
   {
      printf(
            "\nallocate_defaults_c calloc failed for %s buffer!\n",
            Vn_x_name);
      exit( EXIT_FAILURE);
   }
   if ( (Vn_y = ( float * ) calloc(
         1,
         sizeof(float)) ) == NULL)
   {
      printf(
            "\nallocate_defaults_c calloc failed for %s buffer!\n",
            Vn_y_name);
      exit( EXIT_FAILURE);
   }
   if ( (Vn_z = ( float * ) calloc(
         1,
         sizeof(float)) ) == NULL)
   {
      printf(
            "\nallocate_defaults_c calloc failed for %s buffer!\n",
            Vn_z_name);
      exit( EXIT_FAILURE);
   }
   if ( (Vi_x = ( float * ) calloc(
         1,
         sizeof(float)) ) == NULL)
   {
      printf(
            "\nallocate_defaults_c calloc failed for %s buffer!\n",
            Vi_x_name);
      exit( EXIT_FAILURE);
   }
   if ( (Vi_y = ( float * ) calloc(
         1,
         sizeof(float)) ) == NULL)
   {
      printf(
            "\nallocate_defaults_c calloc failed for %s buffer!\n",
            Vi_y_name);
      exit( EXIT_FAILURE);
   }
   if ( (N_e = ( float * ) calloc(
         1,
         sizeof(float)) ) == NULL)
   {
      printf(
            "\nallocate_defaults_c calloc failed for %s buffer!\n",
            N_e_name);
      exit( EXIT_FAILURE);
   }
   if ( (N_O = ( float * ) calloc(
         1,
         sizeof(float)) ) == NULL)
   {
      printf(
            "\nallocate_defaults_c calloc failed for %s buffer!\n",
            N_O_name);
      exit( EXIT_FAILURE);
   }
   if ( (N_O2 = ( float * ) calloc(
         1,
         sizeof(float)) ) == NULL)
   {
      printf(
            "\nallocate_defaults_c calloc failed for %s buffer!\n",
            N_O2_name);
      exit( EXIT_FAILURE);
   }
   if ( (N_N2 = ( float * ) calloc(
         1,
         sizeof(float)) ) == NULL)
   {
      printf(
            "\nallocate_defaults_c calloc failed for %s buffer!\n",
            N_N2_name);
      exit( EXIT_FAILURE);
   }
   if ( (Rmt = ( float * ) calloc(
         1,
         sizeof(float)) ) == NULL)
   {
      printf(
            "\nallocate_defaults_c calloc failed for %s buffer!\n",
            Rmt_name);
      exit( EXIT_FAILURE);
   }
   if ( (Tn = ( float * ) calloc(
         1,
         sizeof(float)) ) == NULL)
   {
      printf(
            "\nallocate_defaults_c calloc failed for %s buffer!\n",
            Tn_name);
      exit( EXIT_FAILURE);
   }
   if ( (sigmaP = ( float * ) calloc(
         1,
         sizeof(float)) ) == NULL)
   {
      printf(
            "\nallocate_defaults_c calloc failed for %s buffer!\n",
            sigmaP_name);
      exit( EXIT_FAILURE);
   }
   if ( (sigmaH = ( float * ) calloc(
         1,
         sizeof(float)) ) == NULL)
   {
      printf(
            "\nallocate_defaults_c calloc failed for %s buffer!\n",
            sigmaH_name);
      exit( EXIT_FAILURE);
   }

   return;
}

