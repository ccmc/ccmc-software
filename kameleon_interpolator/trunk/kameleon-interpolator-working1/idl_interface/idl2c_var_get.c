/*******************************************************************************
 *                                                                             *
 *    NAME:       idl2c_var_get.c                                              *
 *                                                                             *
 *    VERSION:    1.0                                                          *
 *                                                                             *
 *    AUTHOR:     Marlo M. H. Maddox                                           *
 *                NASA-GSFC-AETD-ISD-ADMA(587)-CCMC                            *
 *                Marlo.Maddox@nasa.gov                                        *
 *                (301) 286 - 5202                                             *
 *                                                                             *
 *    PURPOSE:    This code is used tot create an IDL system routine that      *
 *                returns an array of variable values    This module is added  *
 *                to a shared library and linked to IDL with LINKIMAGE...      *
 *                ( DLM's are also possible )                                  *
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
 *                Initial development started                                  *
 *    10.10.2006  Maddox, Marlo                                                *
 *                Using export.h exclusivley                                   *
 *                                                                             *
 *    10.25.2006  Maddox, Marlo                                                *
 *                We chaged CDFid id to current_file_id in                     *
 *                the cdf_interface_variables.h file but                       *
 *                forgot to update the extern declaration                      *
 *                here                                                         *
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
#include "export.h"
/*
 #include "idl_export.h"
 */
#define MAX_MESSAGE_BUFFER_SIZE 500

IDL_VPTR idl2c_var_get(
      int argc,
      IDL_VPTR argv[])
{

   extern CDFid current_file_id;
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

   void *memory_buffer;
   double *double_memory_buffer;
   float *float_memory_buffer;
   int *int_memory_buffer;

   /****** idl2c interface variables ******/

   IDL_VPTR input_file_name, variable_name;
   IDL_VPTR idl_variable_value;

   int *local_int_variable_valuePtr;
   float *local_float_variable_valuePtr;
   double *local_double_variable_valuePtr;

   char message_buffer1[MAX_MESSAGE_BUFFER_SIZE];

   int tmp_ndims;
   IDL_LONG tmp_dims[IDL_MAX_ARRAY_DIM];

   /* check our argc count first */

   if (argc != 2)
   {
      IDL_Message(
            IDL_M_NAMED_GENERIC,
            IDL_MSG_LONGJMP,
            "ERROR:program requires 2 arguments: (1) full path and file name of data file (2) variable name");
   }

   /** get a local copies of idl2c function arguments **/

   input_file_name=argv[0];
   variable_name=argv[1];

   /**************************************
    * OPEN CDF FILE AND AQUIRE CDFid        *
    **************************************/

   /**** lets see if the file even exists first ****/

   if ( !access(
         IDL_VarGetString(input_file_name),
         R_OK) )
   {

      status = CDFopen( IDL_VarGetString(input_file_name), &current_file_id );

      if (status != CDF_OK)
      {

         /** if we couldn't access file, return contron with message to IDL **/

         StatusHandler(status);
         snprintf(
               message_buffer1,
               MAX_MESSAGE_BUFFER_SIZE,
               "ERROR: cannot read \t%s\n",
               IDL_VarGetString(input_file_name) );
         IDL_Message(
               IDL_M_NAMED_GENERIC,
               IDL_MSG_LONGJMP,
               message_buffer1);

      }

   }
   else
   {
      /** if we couldn't access file, return contron with message to IDL **/

      snprintf(
            message_buffer1,
            MAX_MESSAGE_BUFFER_SIZE,
            "ERROR: cannot read \t%s\n",
            IDL_VarGetString(input_file_name) );
      IDL_Message(
            IDL_M_NAMED_GENERIC,
            IDL_MSG_LONGJMP,
            message_buffer1);

   }

   /**************************************
    * NOW LETS locate the variable id    *
    **************************************/

   status = CDFlib( GET_, zVAR_NUMBER_, IDL_VarGetString( variable_name ), &variable_cdfNum, NULL_);

   if (status != CDF_OK)
   {
      StatusHandler(status);
      snprintf(
            message_buffer1,
            MAX_MESSAGE_BUFFER_SIZE,
            "ERROR: couldn't find variable %s\n",
            IDL_VarGetString(variable_name) );
      IDL_Message(
            IDL_M_NAMED_GENERIC,
            IDL_MSG_LONGJMP,
            message_buffer1);
   }

   /******************************************
    * Get info about the variable of interest    *
    *******************************************/

   status = CDFlib(
   SELECT_, zVAR_, variable_cdfNum,
   GET_, zVAR_DATATYPE_, &variable_dataType,
   GET_, zVAR_NUMDIMS_, &numDims,
   GET_, zVAR_DIMSIZES_, &dimSizes,
   NULL_);

   /****************************************************************************
    * Allocate space for variable depending on data type and read in values     *
    ****************************************************************************/

   /** calculate total varoable size **/

   for (i=0; i<numDims; i++)
   {
      variable_size = variable_size + dimSizes[i];
      tmp_dims[i] = dimSizes[i];
   }

   counts[0] = variable_size;
   indices[0] = 0;
   intervals[0] = 1;

   tmp_ndims = numDims;

   switch (variable_dataType)
   {
      case CDF_INT4:

         if ( (int_memory_buffer = ( int * ) calloc(
               variable_size,
               sizeof(int)) ) == NULL)
         {
            snprintf(
                  message_buffer1,
                  MAX_MESSAGE_BUFFER_SIZE,
                  "%s\tERROR: couldn't allocate space for %s\n",
__FILE__                  , IDL_VarGetString( variable_name ) );
                  IDL_Message(IDL_M_NAMED_GENERIC, IDL_MSG_LONGJMP, message_buffer1 );
               }

               /** Create a temporary IDL variable and have our local pointer point to it **/

               local_int_variable_valuePtr = (int *) IDL_MakeTempArray( IDL_TYP_LONG, tmp_ndims, tmp_dims, IDL_ARR_INI_ZERO, &idl_variable_value );

               /** read in the variable vallues to the memory_buffer **/

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
                  snprintf( message_buffer1, MAX_MESSAGE_BUFFER_SIZE, "ERROR: Problem reading variable data to memory_buffer for variable: %s\n", IDL_VarGetString( variable_name ) );
                  IDL_Message(IDL_M_NAMED_GENERIC, IDL_MSG_LONGJMP, message_buffer1 );
               }

               /** using our local pointer, modify the contents of the IDL Variable **/

               for( i=0; i<variable_size;i++ )
               {  local_int_variable_valuePtr[i] = int_memory_buffer[i];}

               break;

               case CDF_FLOAT:

               if( ( float_memory_buffer = ( float * ) calloc( variable_size, sizeof( float ) ) ) == NULL )
               {
                  snprintf( message_buffer1, MAX_MESSAGE_BUFFER_SIZE, "%s\tERROR: couldn't allocate space for %s\n", __FILE__, IDL_VarGetString( variable_name ) );
                  IDL_Message(IDL_M_NAMED_GENERIC, IDL_MSG_LONGJMP, message_buffer1 );
               }

               /** Create a temporary IDL variable and have our local pointer point to it **/

               local_float_variable_valuePtr = (float *) IDL_MakeTempArray( IDL_TYP_FLOAT, tmp_ndims, tmp_dims, IDL_ARR_INI_ZERO, &idl_variable_value );

               /** read in the variable vallues to the memory_buffer **/

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
                  snprintf( message_buffer1, MAX_MESSAGE_BUFFER_SIZE, "ERROR: Problem reading variable data to memory_buffer for variable: %s\n", IDL_VarGetString( variable_name ) );
                  IDL_Message(IDL_M_NAMED_GENERIC, IDL_MSG_LONGJMP, message_buffer1 );
               }

               /** using our local pointer, modify the contents of the IDL Variable **/

               for( i=0; i<variable_size;i++ )
               {  local_float_variable_valuePtr[i] = float_memory_buffer[i];}

               break;

               case CDF_DOUBLE:

               if( ( double_memory_buffer = ( double * ) calloc( variable_size, sizeof( double ) ) ) == NULL )
               {
                  snprintf( message_buffer1, MAX_MESSAGE_BUFFER_SIZE, "%s\tERROR: couldn't allocate space for %s\n", __FILE__, IDL_VarGetString( variable_name ) );
                  IDL_Message(IDL_M_NAMED_GENERIC, IDL_MSG_LONGJMP, message_buffer1 );
               }

               /** Create a temporary IDL variable and have our local pointer point to it **/

               local_double_variable_valuePtr = (double *) IDL_MakeTempArray( IDL_TYP_DOUBLE, tmp_ndims, tmp_dims, IDL_ARR_INI_ZERO, &idl_variable_value );

               /** read in the variable vallues to the memory_buffer **/

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
                  snprintf( message_buffer1, MAX_MESSAGE_BUFFER_SIZE, "ERROR: Problem reading variable data to memory_buffer for variable: %s\n", IDL_VarGetString( variable_name ) );
                  IDL_Message(IDL_M_NAMED_GENERIC, IDL_MSG_LONGJMP, message_buffer1 );
               }

               /** using our local pointer, modify the contents of the IDL Variable **/

               for( i=0; i<variable_size;i++ )
               {  local_double_variable_valuePtr[i] = double_memory_buffer[i];}

               break;

               default:
               snprintf( message_buffer1, MAX_MESSAGE_BUFFER_SIZE, "ERROR: Variable Type for %s isn't currently supported \n", IDL_VarGetString( variable_name ) );
               IDL_Message(IDL_M_NAMED_GENERIC, IDL_MSG_LONGJMP, message_buffer1 );
               break;

            }

            status = CDFclose ( current_file_id );
            if(status != CDF_OK)
            {  StatusHandler (status);}

            return idl_variable_value;

         }

