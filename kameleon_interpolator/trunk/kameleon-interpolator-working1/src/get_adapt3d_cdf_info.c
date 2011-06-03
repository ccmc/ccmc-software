/*******************************************************************************
 *                                                                             *
 *       NAME:          get_adapt3d_cdf_info.c                                 *
 *                                                                             *
 *       VERSION:       1.0                                                    *
 *                                                                             *
 *       AUTHOR:        Peter MacNeice                                         *
 *                      plagiarized from original template of Marlo Maddox     *
 *                      NASA-GSFC-AETD-ISD-ADMA(587)-CCMC                      *
 *                      Marlo.Maddox@nasa.gov                                  *
 *                      (301) 286 - 5202                                       *
 *                                                                             *
 *       PURPOSE:       Extracts all relevant information for identifying      *
 *                      variables etc from the adapt3d cdf file. This routine  *
 *                      is called by the open_cdf call. Variable numbers are   *
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
 *       03.10.2010     Peter MacNeice                                         *
 ******************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <math.h>
#include <stdarg.h>
#include "cdf_interface_variables.h"
#include "adapt3d_cdf_interface_variables.h"
#include "cdf.h"

#define DEBUG_FLAG    0

void get_adapt3d_cdf_info(
      int main_memory_flag)
{

   void allocate_defaults_adapt3d(
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


   ndimn=NDIMN_ADAPT3D;
   NDIMN=NDIMN_ADAPT3D;
   nnode=NNODE_ADAPT3D;
   nvars=NVARS_ADAPT3D;
   nnode=NNODE_ADAPT3D;


   counts[0] = 1; /* set values */
   intervals[0] = 1;

   allocate_defaults_adapt3d();

   /* get all of the cdf attribute/variable numbers that will be needed *******/

   if (CDFlib( CONFIRM_, zVAR_EXISTENCE_, coord_name, NULL_) == CDF_OK)
   {
      status = CDFlib( GET_, zVAR_NUMBER_, coord_name, &coord_cdf_varNum, NULL_);
      if (status != CDF_OK)
         StatusHandler(status);
   }
   if (CDFlib( CONFIRM_, zVAR_EXISTENCE_, unkno_name, NULL_) == CDF_OK)
   {
      status = CDFlib( GET_, zVAR_NUMBER_, unkno_name, &unkno_cdf_varNum, NULL_);
      if (status != CDF_OK)
         StatusHandler(status);
   }
   if (CDFlib( CONFIRM_, zVAR_EXISTENCE_, rho_name, NULL_) == CDF_OK)
   {
      status = CDFlib( GET_, zVAR_NUMBER_, rho_name, &rho_cdf_varNum, NULL_);
      if (status != CDF_OK)
         StatusHandler(status);
   }
   if (CDFlib( CONFIRM_, zVAR_EXISTENCE_, bx_name, NULL_) == CDF_OK)
   {
      status = CDFlib( GET_, zVAR_NUMBER_, bx_name, &bx_cdf_varNum, NULL_);
      if (status != CDF_OK)
         StatusHandler(status);
   }
   if (CDFlib( CONFIRM_, zVAR_EXISTENCE_, by_name, NULL_) == CDF_OK)
   {
      status = CDFlib( GET_, zVAR_NUMBER_, by_name, &by_cdf_varNum, NULL_);
      if (status != CDF_OK)
         StatusHandler(status);
   }
   if (CDFlib( CONFIRM_, zVAR_EXISTENCE_, bz_name, NULL_) == CDF_OK)
   {
      status = CDFlib( GET_, zVAR_NUMBER_, bz_name, &bz_cdf_varNum, NULL_);
      if (status != CDF_OK)
         StatusHandler(status);
   }
   if (CDFlib( CONFIRM_, zVAR_EXISTENCE_, ux_name, NULL_) == CDF_OK)
   {
      status = CDFlib( GET_, zVAR_NUMBER_, ux_name, &ux_cdf_varNum, NULL_);
      if (status != CDF_OK)
         StatusHandler(status);
   }
   if (CDFlib( CONFIRM_, zVAR_EXISTENCE_, uy_name, NULL_) == CDF_OK)
   {
      status = CDFlib( GET_, zVAR_NUMBER_, uy_name, &uy_cdf_varNum, NULL_);
      if (status != CDF_OK)
         StatusHandler(status);
   }
   if (CDFlib( CONFIRM_, zVAR_EXISTENCE_, uz_name, NULL_) == CDF_OK)
   {
      status = CDFlib( GET_, zVAR_NUMBER_, uz_name, &uz_cdf_varNum, NULL_);
      if (status != CDF_OK)
         StatusHandler(status);
   }
   if (CDFlib( CONFIRM_, zVAR_EXISTENCE_, p_name, NULL_) == CDF_OK)
   {
      status = CDFlib( GET_, zVAR_NUMBER_, p_name, &p_cdf_varNum, NULL_);
      if (status != CDF_OK)
         StatusHandler(status);
   }
   if (CDFlib( CONFIRM_, zVAR_EXISTENCE_, temp_name, NULL_) == CDF_OK)
   {
      status = CDFlib( GET_, zVAR_NUMBER_, temp_name, &temp_cdf_varNum, NULL_);
      if (status != CDF_OK)
         StatusHandler(status);
   }
   if (CDFlib( CONFIRM_, zVAR_EXISTENCE_, intmat_name, NULL_) == CDF_OK)
   {
      status = CDFlib( GET_, zVAR_NUMBER_, intmat_name, &intmat_cdf_varNum, NULL_);
      if (status != CDF_OK)
         StatusHandler(status);
   }

   /***  NOTE THESE ARE ATTRIBUTES SO USE CONFRIM ATTR_EXISTENCE... ***/

   if (CDFlib( CONFIRM_, ATTR_EXISTENCE_, GRID_REG_NO_name, NULL_) == CDF_OK)
   {
      status = CDFlib( GET_, ATTR_NUMBER_, GRID_REG_NO_name, &GRID_REG_NO_cdf_num, NULL_);
      if (status != CDF_OK)
         StatusHandler(status);
   }
   if (CDFlib( CONFIRM_, ATTR_EXISTENCE_, NDIMN_name, NULL_) == CDF_OK)
   {
      status = CDFlib( GET_, ATTR_NUMBER_, NDIMN_name, &NDIMN_cdf_num, NULL_);
      if (status != CDF_OK)
         StatusHandler(status);
   }
   if (CDFlib( CONFIRM_, ATTR_EXISTENCE_, NPOIN_name, NULL_) == CDF_OK)
   {
      status = CDFlib( GET_, ATTR_NUMBER_, NPOIN_name, &NPOIN_cdf_num, NULL_);
      if (status != CDF_OK)
         StatusHandler(status);
   }
   if (CDFlib( CONFIRM_, ATTR_EXISTENCE_, NELEM_name, NULL_) == CDF_OK)
   {
      status = CDFlib( GET_, ATTR_NUMBER_, NELEM_name, &NELEM_cdf_num, NULL_);
      if (status != CDF_OK)
         StatusHandler(status);
   }
   if (CDFlib( CONFIRM_, ATTR_EXISTENCE_, NBOUN_name, NULL_) == CDF_OK)
   {
      status = CDFlib( GET_, ATTR_NUMBER_, NBOUN_name, &NBOUN_cdf_num, NULL_);
      if (status != CDF_OK)
         StatusHandler(status);
   }
   if (CDFlib( CONFIRM_, ATTR_EXISTENCE_, NCONI_name, NULL_) == CDF_OK)
   {
      status = CDFlib( GET_, ATTR_NUMBER_, NCONI_name, &NCONI_cdf_num, NULL_);
      if (status != CDF_OK)
         StatusHandler(status);
   }

   if ( DEBUG_FLAG)
      printf("DEBUG\t finished getting cdf nums\n");

   /***************** use cdf_varNums to get actual scalar values *************/

   /********* get value for GRID_REG_NO **********/
   status = CDFlib(
   SELECT_, ATTR_, GRID_REG_NO_cdf_num,
   SELECT_, gENTRY_, 0,
   GET_, gENTRY_DATA_, &GRID_REG_NO,
   NULL_);
   if (status != CDF_OK)
      StatusHandler(status);

   /********* get value for NDIMN **********/
   status = CDFlib(
   SELECT_, ATTR_, NDIMN_cdf_num,
   SELECT_, gENTRY_, 0,
   GET_, gENTRY_DATA_, &NDIMN,
   NULL_);
   if (status != CDF_OK)
      StatusHandler(status);

   /********* get value for NPOIN **********/
   status = CDFlib(
   SELECT_, ATTR_, NPOIN_cdf_num,
   SELECT_, gENTRY_, 0,
   GET_, gENTRY_DATA_, &NPOIN,
   NULL_);
   if (status != CDF_OK)
      StatusHandler(status);

   /********* get value for NELEM **********/
   status = CDFlib(
   SELECT_, ATTR_, NELEM_cdf_num,
   SELECT_, gENTRY_, 0,
   GET_, gENTRY_DATA_, &NELEM,
   NULL_);
   if (status != CDF_OK)
      StatusHandler(status);


   /********* get value for NBOUN **********/
   status = CDFlib(
   SELECT_, ATTR_, NBOUN_cdf_num,
   SELECT_, gENTRY_, 0,
   GET_, gENTRY_DATA_, &NBOUN,
   NULL_);
   if (status != CDF_OK)
      StatusHandler(status);

   /********* get value for NCONI **********/
   status = CDFlib(
   SELECT_, ATTR_, NCONI_cdf_num,
   SELECT_, gENTRY_, 0,
   GET_, gENTRY_DATA_, &NCONI,
   NULL_);
   if (status != CDF_OK)
      StatusHandler(status);

   /*convert original float N* to int n* if required */
   /* bats NX was float but ucla, ctip, enlil, kpvt... are interger - no conversion needed here*/

   grid_reg_no = GRID_REG_NO;
   npoin = NPOIN;
   nelem = NELEM;
   ndimn = NDIMN;
   nboun = NBOUN;
   nconi = NCONI;

   if ( DEBUG_FLAG)
   {
      printf("DEBUG\tINFO from get_adapt3d_cdf_info\n");
      printf(
            "DEBUG\tNPOIN=%d & npoin=%d\n",
            NPOIN,
            npoin);
      printf(
            "DEBUG\tNELEM=%d & nelem=%d\n",
            NELEM,
            nelem);
      printf(
            "DEBUG\t main_memory_flag = %d\n",
            main_memory_flag);
   }


   /************************ allocate space for variables ********/

   if ( (coord = ( float * ) calloc(
         npoin*ndimn,
         sizeof(float)) ) == NULL)
   {
      printf("\ncalloc failed for coord buffer!\n");
      exit( EXIT_FAILURE);
   }
   else {
      printf("\ncalloc succeeded for coord buffer with size %d !\n",npoin*ndimn);
   }
   if ( (unkno = ( float * ) calloc(
         npoin*NVARS_ADAPT3D,
         sizeof(float)) ) == NULL)
   {
      printf("\ncalloc failed for unkno buffer!\n");
      exit( EXIT_FAILURE);
   }
   if ( (rho = ( float * ) calloc(
         npoin,
         sizeof(float)) ) == NULL)
   {
      printf("\ncalloc failed for rho buffer!\n");
      exit( EXIT_FAILURE);
   }
   if ( (bx = ( float * ) calloc(
         npoin,
         sizeof(float)) ) == NULL)
   {
      printf("\ncalloc failed for bx buffer!\n");
      exit( EXIT_FAILURE);
   }
   if ( (by = ( float * ) calloc(
         npoin,
         sizeof(float)) ) == NULL)
   {
      printf("\ncalloc failed for by buffer!\n");
      exit( EXIT_FAILURE);
   }
   if ( (bz = ( float * ) calloc(
         npoin,
         sizeof(float)) ) == NULL)
   {
      printf("\ncalloc failed for bz buffer!\n");
      exit( EXIT_FAILURE);
   }
   if ( (ux = ( float * ) calloc(
         npoin,
         sizeof(float)) ) == NULL)
   {
      printf("\ncalloc failed for ux buffer!\n");
      exit( EXIT_FAILURE);
   }
   if ( (uy = ( float * ) calloc(
         npoin,
         sizeof(float)) ) == NULL)
   {
      printf("\ncalloc failed for uy buffer!\n");
      exit( EXIT_FAILURE);
   }
   if ( (uz = ( float * ) calloc(
         npoin,
         sizeof(float)) ) == NULL)
   {
      printf("\ncalloc failed for uz buffer!\n");
      exit( EXIT_FAILURE);
   }
   if ( (p = ( float * ) calloc(
         npoin,
         sizeof(float)) ) == NULL)
   {
      printf("\ncalloc failed for p buffer!\n");
      exit( EXIT_FAILURE);
   }
   if ( (temp = ( float * ) calloc(
         npoin,
         sizeof(float)) ) == NULL)
   {
      printf("\ncalloc failed for T buffer!\n");
      exit( EXIT_FAILURE);
   }
   if ( (intmat = ( int * ) calloc(
         nelem*4,
         sizeof(int)) ) == NULL)
   {
      printf("\ncalloc failed for intmat buffer!\n");
      exit( EXIT_FAILURE);
   }
   if ( (bconi = ( int * ) calloc(
         nconi*nboun,
         sizeof(int)) ) == NULL)
   {
      printf("\ncalloc failed for bconi buffer!\n");
      exit( EXIT_FAILURE);
   }
   if ( (indx = ( int * ) calloc(
         nelem,
         sizeof(int)) ) == NULL)
   {
      printf("\ncalloc failed for indx buffer!\n");
      exit( EXIT_FAILURE);
   }
   if ( (esup1 = ( int * ) calloc(
         nelem*4,
         sizeof(int)) ) == NULL)
   {
      printf("\ncalloc failed for esup1 buffer!\n");
      exit( EXIT_FAILURE);
   }
   if ( (esup2 = ( int * ) calloc(
         npoin+1,
         sizeof(int)) ) == NULL)
   {
      printf("\ncalloc failed for esup2 buffer!\n");
      exit( EXIT_FAILURE);
   }

   if ( DEBUG_FLAG)
      printf("DEBUG\tspace allocated for variables...\n");

   /***************************** get array values **********************/

   /* No need to pre-load variables for this application */

   /* set the flag that records whether the setup has been done for unstructured grid use */
    unstructured_grid_setup_done=-1;             /* not yet done */
   if ( DEBUG_FLAG)
    printf("DEBUG\tunstructured_grid_setup_done marked undone : %d\n",unstructured_grid_setup_done);

}

int load_adapt3d_cdf_variable_into_main_memory(
      char *variable_to_read)
{

   CDFstatus status;
   int       istatus;

   long recordStart = 0;
   long recordCount = 1;
   long recordInterval = 1;
   long counts[1] = { 0 };
   long indices[1];
   long intervals[1] = { 1 };
   long counts1[1] = { 0 };
   long counts2[1] = { 0 };
   long counts3[1] = { 0 };
   long counts4[1] = { 0 };

   long variable_cdf_number;

   int adapt3d_reserve_mem_and_set_cdf_num(
         char *);


   variable_cdf_number = adapt3d_reserve_mem_and_set_cdf_num(variable_to_read);

   if (variable_cdf_number == -1)
   {
      printf(
            "ERROR:\tcould not find cdf variable number for -->%s<--\n",
            variable_to_read);
      return EXIT_FAILURE; /*exit( EXIT_FAILURE );*/
   }

   /*(printf("DEBUG\tback from reserve mem with cdf_varNum %d for variable %s \n", variable_cdf_number, variable_to_read );*/

   counts[0] = 3*npoin;
   counts1[0] = npoin;
   counts2[0] = NVARS_ADAPT3D*npoin;
   counts3[0] = 4*nelem;
   counts4[0] = nconi;
   indices[0] = 0;
   intervals[0] = 1;

   if ( !strcmp(
         variable_to_read,
         coord_name) )
   {
      printf("calling CDFlib for variable_cdf_number %d\n",variable_cdf_number);
      status = CDFlib(
      SELECT_, zVAR_, variable_cdf_number,
      SELECT_, zVAR_RECNUMBER_, recordStart,
      SELECT_, zVAR_RECCOUNT_, recordCount,
      SELECT_, zVAR_RECINTERVAL_, recordInterval,
      SELECT_, zVAR_DIMINDICES_, indices,
      SELECT_, zVAR_DIMCOUNTS_, counts,
      SELECT_, zVAR_DIMINTERVALS_, intervals,
      GET_, zVAR_HYPERDATA_, coord,
      NULL_);
      printf("left CDFlib \n");
      if (status != CDF_OK)
         StatusHandler(status);
   }
   else if ( !strcmp(
         variable_to_read,
         unkno_name) )
   {
      printf("calling CDFlib for variable_cdf_number %d\n",variable_cdf_number);
      status = CDFlib(
      SELECT_, zVAR_, variable_cdf_number,
      SELECT_, zVAR_RECNUMBER_, recordStart,
      SELECT_, zVAR_RECCOUNT_, recordCount,
      SELECT_, zVAR_RECINTERVAL_, recordInterval,
      SELECT_, zVAR_DIMINDICES_, indices,
      SELECT_, zVAR_DIMCOUNTS_, counts2,
      SELECT_, zVAR_DIMINTERVALS_, intervals,
      GET_, zVAR_HYPERDATA_, unkno,
      NULL_);
      if (status != CDF_OK)
         StatusHandler(status);
   }
   else if ( !strcmp(
         variable_to_read,
         rho_name) )
   {
      printf("calling CDFlib for variable_cdf_number %d\n",variable_cdf_number);
      status = CDFlib(
      SELECT_, zVAR_, variable_cdf_number,
      SELECT_, zVAR_RECNUMBER_, recordStart,
      SELECT_, zVAR_RECCOUNT_, recordCount,
      SELECT_, zVAR_RECINTERVAL_, recordInterval,
      SELECT_, zVAR_DIMINDICES_, indices,
      SELECT_, zVAR_DIMCOUNTS_, counts1,
      SELECT_, zVAR_DIMINTERVALS_, intervals,
      GET_, zVAR_HYPERDATA_, rho,
      NULL_);
      if (status != CDF_OK)
         StatusHandler(status);
   }
   else if ( !strcmp(
         variable_to_read,
         bx_name) )
   {
      status = CDFlib(
      SELECT_, zVAR_, variable_cdf_number,
      SELECT_, zVAR_RECNUMBER_, recordStart,
      SELECT_, zVAR_RECCOUNT_, recordCount,
      SELECT_, zVAR_RECINTERVAL_, recordInterval,
      SELECT_, zVAR_DIMINDICES_, indices,
      SELECT_, zVAR_DIMCOUNTS_, counts1,
      SELECT_, zVAR_DIMINTERVALS_, intervals,
      GET_, zVAR_HYPERDATA_, bx,
      NULL_);
      if (status != CDF_OK)
         StatusHandler(status);
   }
   else if ( !strcmp(
         variable_to_read,
         by_name) )
   {
      status = CDFlib(
      SELECT_, zVAR_, variable_cdf_number,
      SELECT_, zVAR_RECNUMBER_, recordStart,
      SELECT_, zVAR_RECCOUNT_, recordCount,
      SELECT_, zVAR_RECINTERVAL_, recordInterval,
      SELECT_, zVAR_DIMINDICES_, indices,
      SELECT_, zVAR_DIMCOUNTS_, counts1,
      SELECT_, zVAR_DIMINTERVALS_, intervals,
      GET_, zVAR_HYPERDATA_, by,
      NULL_);
      if (status != CDF_OK)
         StatusHandler(status);
   }
   else if ( !strcmp(
         variable_to_read,
         bz_name) )
   {
      status = CDFlib(
      SELECT_, zVAR_, variable_cdf_number,
      SELECT_, zVAR_RECNUMBER_, recordStart,
      SELECT_, zVAR_RECCOUNT_, recordCount,
      SELECT_, zVAR_RECINTERVAL_, recordInterval,
      SELECT_, zVAR_DIMINDICES_, indices,
      SELECT_, zVAR_DIMCOUNTS_, counts1,
      SELECT_, zVAR_DIMINTERVALS_, intervals,
      GET_, zVAR_HYPERDATA_, bz,
      NULL_);
      if (status != CDF_OK)
         StatusHandler(status);
   }
   else if ( !strcmp(
         variable_to_read,
         ux_name) )
   {
      status = CDFlib(
      SELECT_, zVAR_, variable_cdf_number,
      SELECT_, zVAR_RECNUMBER_, recordStart,
      SELECT_, zVAR_RECCOUNT_, recordCount,
      SELECT_, zVAR_RECINTERVAL_, recordInterval,
      SELECT_, zVAR_DIMINDICES_, indices,
      SELECT_, zVAR_DIMCOUNTS_, counts1,
      SELECT_, zVAR_DIMINTERVALS_, intervals,
      GET_, zVAR_HYPERDATA_, ux,
      NULL_);
      if (status != CDF_OK)
         StatusHandler(status);
   }
   else if ( !strcmp(
         variable_to_read,
         uy_name) )
   {
      status = CDFlib(
      SELECT_, zVAR_, variable_cdf_number,
      SELECT_, zVAR_RECNUMBER_, recordStart,
      SELECT_, zVAR_RECCOUNT_, recordCount,
      SELECT_, zVAR_RECINTERVAL_, recordInterval,
      SELECT_, zVAR_DIMINDICES_, indices,
      SELECT_, zVAR_DIMCOUNTS_, counts1,
      SELECT_, zVAR_DIMINTERVALS_, intervals,
      GET_, zVAR_HYPERDATA_, uy,
      NULL_);
      if (status != CDF_OK)
         StatusHandler(status);
   }
   else if ( !strcmp(
         variable_to_read,
         uz_name) )
   {
      status = CDFlib(
      SELECT_, zVAR_, variable_cdf_number,
      SELECT_, zVAR_RECNUMBER_, recordStart,
      SELECT_, zVAR_RECCOUNT_, recordCount,
      SELECT_, zVAR_RECINTERVAL_, recordInterval,
      SELECT_, zVAR_DIMINDICES_, indices,
      SELECT_, zVAR_DIMCOUNTS_, counts1,
      SELECT_, zVAR_DIMINTERVALS_, intervals,
      GET_, zVAR_HYPERDATA_, uz,
      NULL_);
      if (status != CDF_OK)
         StatusHandler(status);
   }
   else if ( !strcmp(
         variable_to_read,
         p_name) )
   {
      status = CDFlib(
      SELECT_, zVAR_, variable_cdf_number,
      SELECT_, zVAR_RECNUMBER_, recordStart,
      SELECT_, zVAR_RECCOUNT_, recordCount,
      SELECT_, zVAR_RECINTERVAL_, recordInterval,
      SELECT_, zVAR_DIMINDICES_, indices,
      SELECT_, zVAR_DIMCOUNTS_, counts1,
      SELECT_, zVAR_DIMINTERVALS_, intervals,
      GET_, zVAR_HYPERDATA_, p,
      NULL_);
      if (status != CDF_OK)
         StatusHandler(status);
   }
   else if ( !strcmp(
         variable_to_read,
         temp_name) )
   {
      status = CDFlib(
      SELECT_, zVAR_, variable_cdf_number,
      SELECT_, zVAR_RECNUMBER_, recordStart,
      SELECT_, zVAR_RECCOUNT_, recordCount,
      SELECT_, zVAR_RECINTERVAL_, recordInterval,
      SELECT_, zVAR_DIMINDICES_, indices,
      SELECT_, zVAR_DIMCOUNTS_, counts1,
      SELECT_, zVAR_DIMINTERVALS_, intervals,
      GET_, zVAR_HYPERDATA_, temp,
      NULL_);
      if (status != CDF_OK)
         StatusHandler(status);
   }
   else if ( !strcmp(
         variable_to_read,
         intmat_name) )
   {
      status = CDFlib(
      SELECT_, zVAR_, variable_cdf_number,
      SELECT_, zVAR_RECNUMBER_, recordStart,
      SELECT_, zVAR_RECCOUNT_, recordCount,
      SELECT_, zVAR_RECINTERVAL_, recordInterval,
      SELECT_, zVAR_DIMINDICES_, indices,
      SELECT_, zVAR_DIMCOUNTS_, counts3,
      SELECT_, zVAR_DIMINTERVALS_, intervals,
      GET_, zVAR_HYPERDATA_, intmat,
      NULL_);
      if (status != CDF_OK)
         StatusHandler(status);
   }
   else if ( !strcmp(
         variable_to_read,
         bconi_name) )
   {
      status = CDFlib(
      SELECT_, zVAR_, variable_cdf_number,
      SELECT_, zVAR_RECNUMBER_, recordStart,
      SELECT_, zVAR_RECCOUNT_, recordCount,
      SELECT_, zVAR_RECINTERVAL_, recordInterval,
      SELECT_, zVAR_DIMINDICES_, indices,
      SELECT_, zVAR_DIMCOUNTS_, counts4,
      SELECT_, zVAR_DIMINTERVALS_, intervals,
      GET_, zVAR_HYPERDATA_, bconi,
      NULL_);
      if (status != CDF_OK)
         StatusHandler(status);
   }
   else
   {
      /* INSERT ERROR HANDLER HERE */
      printf("couldn't find a variable to load...\n");
      return EXIT_FAILURE;
   }

   /*printf("DEBUG\tfinished hyper read returning\n");*/

   /* if necessary run setup for efficient searching of unstructured grids */
   if ( !unstructured_grid_setup_done ) {


   if ( DEBUG_FLAG)
       printf("Calling setup_search_unstructured_grid \n");
/*       istatus=setup_search_unstructured_grid(npoin,nelem,intmat,coord,indx); */
       istatus=setup_search_unstructured_grid();
   if ( DEBUG_FLAG)
       printf("Exited setup_search_unstructured_grid: istatus=%d \n",istatus);


   if ( DEBUG_FLAG)
       printf("\nCalling  smart_search_setup \n");
/*       smart_search_setup(nelem,npoin,intmat); */
       smart_search_setup();
   if ( DEBUG_FLAG)
       printf("Exited smart_search_setup \n");

       last_element_found = -1;
       unstructured_grid_setup_done=1;
   }


   return EXIT_SUCCESS;

}

int adapt3d_reserve_mem_and_set_cdf_num(
      char *variable_number_to_get)
{
   int buffer_size = npoin*3;
   int buffer_size1 = npoin;
   int buffer_size2 = nelem*4;
   int buffer_size3 = nconi*nboun;
   int buffer_size4 = npoin*NVARS_ADAPT3D;

   /*printf("DEBUG\tinside resrve mem\n");*/

   /* return cdf variable Number for given variable */

   if ( !strcmp(
         variable_number_to_get,
         coord_name) )
   {
      if ( (coord = ( float * ) calloc(
            buffer_size,
            sizeof(float)) ) == NULL)
      {
         printf(
               "\ncalloc failed for %s buffer!\n",
               coord_name);
         exit( EXIT_FAILURE);
      }
      else {
         printf(
               "\ncalloc done for %s buffer with size %d !\n",
               coord_name,buffer_size);
      }
      return coord_cdf_varNum;
   }
   else if ( !strcmp(
         variable_number_to_get,
         unkno_name) )
   {
      if ( (unkno = ( float * ) calloc(
            buffer_size4,
            sizeof(float)) ) == NULL)
      {
         printf(
               "\ncalloc failed for %s buffer!\n",
               unkno_name);
         exit( EXIT_FAILURE);
      }
      return unkno_cdf_varNum;
   }
   else if ( !strcmp(
         variable_number_to_get,
         rho_name) )
   {
      if ( (rho = ( float * ) calloc(
            buffer_size1,
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
         bx_name) )
   {
      if ( (bx = ( float * ) calloc(
            buffer_size1,
            sizeof(float)) ) == NULL)
      {
         printf(
               "\ncalloc failed for %s buffer!\n",
               bx_name);
         exit( EXIT_FAILURE);
      }
      return bx_cdf_varNum;
   }
   else if ( !strcmp(
         variable_number_to_get,
         by_name) )
   {
      if ( (by = ( float * ) calloc(
            buffer_size1,
            sizeof(float)) ) == NULL)
      {
         printf(
               "\ncalloc failed for %s buffer!\n",
               by_name);
         exit( EXIT_FAILURE);
      }
      return by_cdf_varNum;
   }
   else if ( !strcmp(
         variable_number_to_get,
         bz_name) )
   {
      if ( (bz = ( float * ) calloc(
            buffer_size1,
            sizeof(float)) ) == NULL)
      {
         printf(
               "\ncalloc failed for %s buffer!\n",
               bz_name);
         exit( EXIT_FAILURE);
      }
      return bz_cdf_varNum;
   }
   else if ( !strcmp(
         variable_number_to_get,
         ux_name) )
   {
      if ( (ux = ( float * ) calloc(
            buffer_size1,
            sizeof(float)) ) == NULL)
      {
         printf(
               "\ncalloc failed for %s buffer!\n",
               ux_name);
         exit( EXIT_FAILURE);
      }
      return ux_cdf_varNum;
   }
   else if ( !strcmp(
         variable_number_to_get,
         uy_name) )
   {
      if ( (uy = ( float * ) calloc(
            buffer_size1,
            sizeof(float)) ) == NULL)
      {
         printf(
               "\ncalloc failed for %s buffer!\n",
               uy_name);
         exit( EXIT_FAILURE);
      }
      return uy_cdf_varNum;
   }
   else if ( !strcmp(
         variable_number_to_get,
         uz_name) )
   {
      if ( (uz = ( float * ) calloc(
            buffer_size1,
            sizeof(float)) ) == NULL)
      {
         printf(
               "\ncalloc failed for %s buffer!\n",
               uz_name);
         exit( EXIT_FAILURE);
      }
      return uz_cdf_varNum;
   }
   else if ( !strcmp(
         variable_number_to_get,
         p_name) )
   {
      if ( (p = ( float * ) calloc(
            buffer_size1,
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
         temp_name) )
   {
      if ( (temp = ( float * ) calloc(
            buffer_size1,
            sizeof(float)) ) == NULL)
      {
         printf(
               "\ncalloc failed for %s buffer!\n",
               temp_name);
         exit( EXIT_FAILURE);
      }
      return temp_cdf_varNum;
   }
   else if ( !strcmp(
         variable_number_to_get,
         intmat_name) )
   {
      if ( (intmat = ( int * ) calloc(
            buffer_size2,
            sizeof(int)) ) == NULL)
      {
         printf(
               "\ncalloc failed for %s buffer!\n",
               intmat_name);
         exit( EXIT_FAILURE);
      }
      return intmat_cdf_varNum;
   }
   else if ( !strcmp(
         variable_number_to_get,
         bconi_name) )
   {
      if ( (bconi = ( int * ) calloc(
            buffer_size3,
            sizeof(int)) ) == NULL)
      {
         printf(
               "\ncalloc failed for %s buffer!\n",
               bconi_name);
         exit( EXIT_FAILURE);
      }
      return bconi_cdf_varNum;
   }
   else
   {
      return -1;
   }
   /*else { printf("ERROR:\tcould not find cdf variable number for %s\n", variable ); return 0; }*/

}


void allocate_defaults_adapt3d(
      void)
{
   /************ we need to allocate space for all variables even when no main mem option is set
    * this allows the all inclusing close to work properly as it tries to free all variables.  We only need
    * to hold a enough space for a single value ******/

   if ( (coord = ( float * ) calloc(
         1,
         sizeof(float)) ) == NULL)
   {
      printf(
            "\nallocate_defaults_adapt3d calloc failed for %s buffer!\n",
            coord_name);
      exit( EXIT_FAILURE);
   }
   if ( (unkno = ( float * ) calloc(
         1,
         sizeof(float)) ) == NULL)
   {
      printf(
            "\nallocate_defaults_adapt3d calloc failed for %s buffer!\n",
            unkno_name);
      exit( EXIT_FAILURE);
   }
   if ( (rho = ( float * ) calloc(
         1,
         sizeof(float)) ) == NULL)
   {
      printf(
            "\nallocate_defaults_adapt3d calloc failed for %s buffer!\n",
            rho_name);
      exit( EXIT_FAILURE);
   }
   if ( (bx = ( float * ) calloc(
         1,
         sizeof(float)) ) == NULL)
   {
      printf(
            "\nallocate_defaults_adapt3d calloc failed for %s buffer!\n",
            bx_name);
      exit( EXIT_FAILURE);
   }
   if ( (by = ( float * ) calloc(
         1,
         sizeof(float)) ) == NULL)
   {
      printf(
            "\nallocate_defaults_adapt3d calloc failed for %s buffer!\n",
            by_name);
      exit( EXIT_FAILURE);
   }
   if ( (bz = ( float * ) calloc(
         1,
         sizeof(float)) ) == NULL)
   {
      printf(
            "\nallocate_defaults_adapt3d calloc failed for %s buffer!\n",
            bz_name);
      exit( EXIT_FAILURE);
   }
   if ( (ux = ( float * ) calloc(
         1,
         sizeof(float)) ) == NULL)
   {
      printf(
            "\nallocate_defaults_adapt3d calloc failed for %s buffer!\n",
            ux_name);
      exit( EXIT_FAILURE);
   }
   if ( (uy = ( float * ) calloc(
         1,
         sizeof(float)) ) == NULL)
   {
      printf(
            "\nallocate_defaults_adapt3d calloc failed for %s buffer!\n",
            uy_name);
      exit( EXIT_FAILURE);
   }
   if ( (uz = ( float * ) calloc(
         1,
         sizeof(float)) ) == NULL)
   {
      printf(
            "\nallocate_defaults_adapt3d calloc failed for %s buffer!\n",
            uz_name);
      exit( EXIT_FAILURE);
   }
   if ( (p = ( float * ) calloc(
         1,
         sizeof(float)) ) == NULL)
   {
      printf(
            "\nallocate_defaults_adapt3d calloc failed for %s buffer!\n",
            p_name);
      exit( EXIT_FAILURE);
   }
   if ( (temp = ( float * ) calloc(
         1,
         sizeof(float)) ) == NULL)
   {
      printf(
            "\nallocate_defaults_adapt3d calloc failed for %s buffer!\n",
            temp_name);
      exit( EXIT_FAILURE);
   }
   if ( (intmat = ( int * ) calloc(
         1,
         sizeof(int)) ) == NULL)
   {
      printf(
            "\nallocate_defaults_adapt3d calloc failed for %s buffer!\n",
            intmat_name);
      exit( EXIT_FAILURE);
   }
   if ( (bconi = ( int * ) calloc(
         1,
         sizeof(int)) ) == NULL)
   {
      printf(
            "\nallocate_defaults_adapt3d calloc failed for %s buffer!\n",
            bconi_name);
      exit( EXIT_FAILURE);
   }
   if ( (indx = ( int * ) calloc(
         1,
         sizeof(int)) ) == NULL)
   {
      printf(
            "\nallocate_defaults_adapt3d calloc failed for indx buffer!\n");
      exit( EXIT_FAILURE);
   }

   return;
}

