/*******************************************************************************
 *                                                                             *
 *    NAME:       var_get.c                                                    *
 *                                                                             *
 *    VERSION:    1.0                                                          *
 *                                                                             *
 *    AUTHOR:     Marlo M. H. Maddox                                           *
 *                NASA-GSFC-AETD-ISD-ADMA(587)-CCMC                            *
 *                Marlo.Maddox@nasa.gov                                        *
 *                (301) 286 - 5202                                             *
 *                                                                             *
 *    PURPOSE:    This code returns an array of variable values.               *
 *                                                                             *
 *    INPUTS:     (1) full path file name (2) variable name                    *
 *                                                                             *
 *    OUTPUTS:    variable array of values                                     *
 *                                                                             *
 *    FUNCTIONS:                                                               *
 *                                                                             *
 *    VARIABLES:                                                               *
 *                                                                             *
 *                                                                             *
 *    MODIFICATION                                                             *
 *    HISTORY:                                                                 *
 *                                                                             *
 *    09.xx.2006  Maddox, Marlo                                                *
                  Initial development started                                  *
 *                                                                             *
 *    11.29.2006  Maddox, Marlo                                                *
 *                Try to check if a cdf is already open before trying to open  *
 *                                                                             *
 ******************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <math.h>
#include <stdarg.h>
#include "cdf_interface_variables.h"
#include "cdf.h"

#define DEBUG 0

void * var_get(
      char *input_file_name,
      char *variable_name,
      int *array_size)
{

   extern CDFid current_file_id;
   int cdf_already_open=0;
   CDFstatus status;

   /******* cdf interface variables *******/

   long recordStart = 0;
   long recordCount = 1;
   long recordInterval = 1;
   long counts[1] =
   { 0 };
   long intervals[1] =
   { 1 };
   long indices[1];

   long variable_cdfNum;
   long variable_dataType;
   long numDims;
   long dimSizes[CDF_MAX_DIMS];

   int i, variable_size=0;

   static double *double_memory_buffer;
   static float *float_memory_buffer;
   static int *int_memory_buffer;

   /**************************************
    * OPEN CDF FILE AND AQUIRE CDFid        *
    **************************************/

   /** before trying to open the file, lets see if a CDF is already open **/
   status = CDFlib( CONFIRM_, CDF_ACCESS_, NULL_);

   if (status != CDF_OK)
   {
      StatusHandler(status);
      fprintf( stderr, "INFO: cannot confirm cdf access\n");
      if ( DEBUG)
         printf(
               "%s INFO:\tNO currently open CDF found, will open %s\n",
__FILE__               , input_file_name );
               cdf_already_open = 0;
            }
            else
            {
               if( DEBUG ) printf("%s INFO:\tA CDF File is already open in memory,ignoring %s\n", __FILE__, input_file_name );
               cdf_already_open = 1;
            }

            /**** lets see if the file even exists first ****/

            if( ! cdf_already_open )
            {

               if( ! access( input_file_name, R_OK) )
               {

                  status = CDFopen( input_file_name, &current_file_id );

                  if(status != CDF_OK)
                  {

                     /** if we couldn't access file, return NULL **/

                     StatusHandler (status);
                     fprintf( stderr, "ERROR: cannot read \t%s\n", input_file_name );
                     return NULL;

                  }

               }
               else
               {
                  /** if we couldn't access file, return contron with message to IDL **/

                  fprintf( stderr, "ERROR: cannot read \t%s\n", input_file_name );
                  return NULL;

               }
            }

            /**************************************
             * NOW LETS locate the variable id    *
             **************************************/

            status = CDFlib ( GET_, zVAR_NUMBER_, variable_name, &variable_cdfNum, NULL_ );

            if(status != CDF_OK)
            {
               StatusHandler (status);
               fprintf( stderr, "ERROR: cannot locate variable:\t%s\n", variable_name );
               return NULL;
            }

            /******************************************
             * Get info about the variable of interest    *
             *******************************************/

            status = CDFlib (
            SELECT_, zVAR_, variable_cdfNum,
            GET_, zVAR_DATATYPE_, &variable_dataType,
            GET_, zVAR_NUMDIMS_, &numDims,
            GET_, zVAR_DIMSIZES_, &dimSizes,
            NULL_
            );

            /****************************************************************************
             * Allocate space for variable depending on data type and read in values     *
             ****************************************************************************/

            /** calculate total variable size **/

            for( i=0; i<numDims;i++ )
            {
               variable_size = variable_size + dimSizes[i];
            }

            counts[0] = variable_size;
            indices[0] = 0;
            intervals[0] = 1;

            /** lets set the array_size for calling program now **/

            array_size[0] = variable_size;

            switch ( variable_dataType )
            {
               case CDF_INT4:

               if( ( int_memory_buffer = ( int * ) calloc( variable_size, sizeof( int ) ) ) == NULL )
               {
                  fprintf( stderr, "ERROR: couldn't allocate space for %s\n", variable_name );
                  return NULL;
               }

               status = CDFlib (
               SELECT_, zVAR_, variable_cdfNum,
               SELECT_, zVAR_RECNUMBER_, recordStart,
               SELECT_, zVAR_RECCOUNT_, recordCount,
               SELECT_, zVAR_RECINTERVAL_, recordInterval,
               SELECT_, zVAR_DIMINDICES_, indices,
               SELECT_, zVAR_DIMCOUNTS_, counts,
               SELECT_, zVAR_DIMINTERVALS_, intervals,
               GET_, zVAR_HYPERDATA_, int_memory_buffer,
               NULL_
               );

               if(status != CDF_OK)
               {
                  StatusHandler (status);
                  fprintf( stderr, "ERROR: Problem reading variable data to memory_buffer for variable: %s\n", variable_name );
                  return NULL;
               }

               if( ! cdf_already_open )
               {
                  status = CDFclose ( current_file_id );
                  if(status != CDF_OK)
                  {  StatusHandler (status);}
               }

               return int_memory_buffer;

               break;

               case CDF_FLOAT:

               if( ( float_memory_buffer = ( float * ) calloc( variable_size, sizeof( float ) ) ) == NULL )
               {
                  fprintf( stderr, "ERROR: couldn't allocate space for %s\n", variable_name );
                  return NULL;
               }

               status = CDFlib (
               SELECT_, zVAR_, variable_cdfNum,
               SELECT_, zVAR_RECNUMBER_, recordStart,
               SELECT_, zVAR_RECCOUNT_, recordCount,
               SELECT_, zVAR_RECINTERVAL_, recordInterval,
               SELECT_, zVAR_DIMINDICES_, indices,
               SELECT_, zVAR_DIMCOUNTS_, counts,
               SELECT_, zVAR_DIMINTERVALS_, intervals,
               GET_, zVAR_HYPERDATA_, float_memory_buffer,
               NULL_
               );

               if(status != CDF_OK)
               {
                  StatusHandler (status);
                  fprintf( stderr, "ERROR: Problem reading variable data to memory_buffer for variable: %s\n", variable_name );
                  return NULL;
               }

               if( ! cdf_already_open )
               {
                  status = CDFclose ( current_file_id );
                  if(status != CDF_OK)
                  {  StatusHandler (status);}
               }

               return float_memory_buffer;

               break;

               case CDF_DOUBLE:

               if( ( double_memory_buffer = ( double * ) calloc( variable_size, sizeof( double ) ) ) == NULL )
               {
                  fprintf( stderr, "ERROR: couldn't allocate space for %s\n", variable_name );
                  return NULL;
               }

               status = CDFlib (
               SELECT_, zVAR_, variable_cdfNum,
               SELECT_, zVAR_RECNUMBER_, recordStart,
               SELECT_, zVAR_RECCOUNT_, recordCount,
               SELECT_, zVAR_RECINTERVAL_, recordInterval,
               SELECT_, zVAR_DIMINDICES_, indices,
               SELECT_, zVAR_DIMCOUNTS_, counts,
               SELECT_, zVAR_DIMINTERVALS_, intervals,
               GET_, zVAR_HYPERDATA_, double_memory_buffer,
               NULL_
               );

               if(status != CDF_OK)
               {
                  StatusHandler (status);
                  fprintf( stderr, "ERROR: Problem reading variable data to memory_buffer for variable: %s\n", variable_name );
                  return NULL;
               }

               if( ! cdf_already_open )
               {
                  status = CDFclose ( current_file_id );
                  if(status != CDF_OK)
                  {  StatusHandler (status);}
               }

               return double_memory_buffer;

               break;

               default:
               fprintf( stderr, "ERROR: Variable Type for %s isn't currently supported \n", variable_name );

               if( ! cdf_already_open )
               {
                  status = CDFclose ( current_file_id );
                  if(status != CDF_OK)
                  {  StatusHandler (status);}
               }
               return NULL;
               break;

            }

         }

