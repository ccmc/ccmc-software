/*******************************************************************************
 *                                                                             *
 *    NAME:       idl2c_gattribute_get.c                                       *
 *                                                                             *
 *    VERSION:    1.0                                                          *
 *                                                                             *
 *    AUTHOR:     Marlo M. H. Maddox                                           *
 *                NASA-GSFC-AETD-ISD-ADMA(587)-CCMC                            *
 *                Marlo.Maddox@nasa.gov                                        *
 *                (301) 286 - 5202                                             *
 *                                                                             *
 *    PURPOSE:    This code is used to create an IDL system routine that       *
 *                returns a gloabal attribute for a specified file. It         *
 *                essentially provides the same functionality as the           *
 *                gattribute_get routine. This module is added to a shared     *
 *                library and linked to IDL with LINKIMAGE...                  *
 *                ( DLM's are also possible )                                  *
 *                                                                             *
 *    INPUTS:     (1) full path file name (2) global attribute name            *
 *                                                                             *
 *    OUTPUTS:    global attribute value                                       *
 *                                                                             *
 *    FUNCTIONS:                                                               *
 *                                                                             *
 *    VARIABLES:                                                               *
 *                                                                             *
 * *                                                                           *
 *    MODIFICATION                                                             *
 *    HISTORY:                                                                 *
 *                                                                             *
 *    09.xx.2006  Maddox, Marlo                                                *
 *                Initial development started                                  *
 *                                                                             *
 *    10.10.2006  Maddox, Marlo                                                *
 *                Using export.h exclusivley                                   *
 *                                                                             *
 *    10.11.2006  Maddox, Marlo                                                *
 *                Removed kameleon library cdf_open & cdf_close calls and      *
 *                replaced them with native CDFopen and CDFclose calls         *
 *                                                                             *
 *                Had to add SELECT_, gEntry to CDFLIB call during attribute   *
 *                inquire step                                                 *
 *                                                                             *
 *    10.25.2006  Maddox, Marlo                                                *
 *                we chaged CDFid id to current_file_id in the                 *
 *                cdf_interface_variables.h file but forgot to update the      *
 *                extern declaration here                                      *
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
#define MAX_MESSAGE_BUFFER_SIZE 7000

#define DEBUG_FLAG 1

IDL_VPTR idl2c_gattribute_get(
      int argc,
      IDL_VPTR argv[])
{

   /*extern CDFid current_file_id;*//* why is this extern */
   CDFid current_file_id;
   CDFstatus status;

   long attrNum, data_type, numEntries, numElements, gEntryNum;
   static char *tmp_char_value;
   static float tmp_float_value;
   static int tmp_int_value;

   /* declared local pointers to store returned vattribute value pointers ****/

   char *char_attribute_valuePtr;
   int *int_attribute_valuePtr;
   float *float_attribute_valuePtr;

   /****** idl2c interface variables ******/

   IDL_VPTR input_file_name, gattribute_name;

   IDL_VPTR idl_local_float_value, idl_local_int_value;
   IDL_VPTR idl_return_float_value, idl_return_int_value;

   /** possible store argv[0] & argv[1] using more descriptive names **/

   char *idl_input_file_name, *idl_gattribute_name;
   int idl_input_file_name_length, idl_gattribute_name_length;

   char message_buffer1[MAX_MESSAGE_BUFFER_SIZE];

   int tmp_ndims;

   IDL_LONG tmp_dims[IDL_MAX_ARRAY_DIM];

   IDL_VPTR no_value= NULL;

   /* check our argc count first */

   if (argc != 2)
   {
      IDL_Message(
            IDL_M_NAMED_GENERIC,
            IDL_MSG_LONGJMP,
            "ERROR:program requires 2 arguments: (1) full path and file name of "
               "data file (2) global attribute name");

      return no_value;
   }

   /** get a local copies of idl2c function arguments **/

   IDL_Message(
         IDL_M_NAMED_GENERIC,
         IDL_MSG_INFO,
         "Builing Arguments\n");

   input_file_name=argv[0];
   gattribute_name=argv[1];

   idl_input_file_name_length = strlen(IDL_VarGetString(input_file_name));
   idl_gattribute_name_length = strlen(IDL_VarGetString(gattribute_name));

   snprintf(
         message_buffer1,
         MAX_MESSAGE_BUFFER_SIZE,
         "DEBUG:\nidl_input_file_name_length -->%d<--\nidl_gattribute_name_length -->%d<--\n",
         idl_input_file_name_length,
         idl_gattribute_name_length);

   IDL_Message(
         IDL_M_NAMED_GENERIC,
         IDL_MSG_INFO,
         message_buffer1);

   idl_input_file_name = malloc(sizeof(char) * (idl_input_file_name_length +1));
   idl_gattribute_name = malloc(sizeof(char) * (idl_gattribute_name_length +1));

   strncpy(
         idl_input_file_name,
         IDL_VarGetString(input_file_name),
         idl_input_file_name_length+1);
   strncpy(
         idl_gattribute_name,
         IDL_VarGetString(gattribute_name),
         idl_gattribute_name_length+1);

   snprintf(
         message_buffer1,
         MAX_MESSAGE_BUFFER_SIZE,
         "DEBUG:\ninput_file_name -->%s<--\ngattribute_name -->%s<--\n\nidl_input_file_name -->%s<--\nidl_gattribute_name -->%s<--\nargv[0] -->%s<--\nargv[1] -->%s<--\n",
         IDL_VarGetString(input_file_name),
         IDL_VarGetString(gattribute_name),
         idl_input_file_name,
         idl_gattribute_name,
         IDL_VarGetString(argv[0]),
         IDL_VarGetString(argv[1]) );

   IDL_Message(
         IDL_M_NAMED_GENERIC,
         IDL_MSG_INFO,
         message_buffer1);

   /**************************************
    * OPEN CDF FILE AND AQUIRE CDFid        *
    **************************************/

   /**** lets see if the file even exists first ****/

   if ( !access(
         idl_input_file_name,
         R_OK) )
   {

      status = CDFopen( idl_input_file_name, &current_file_id );

      if (status != CDF_OK)
      {

         /** if we couldn't access file, return contron with message to IDL **/

         StatusHandler(status);
         snprintf(
               message_buffer1,
               MAX_MESSAGE_BUFFER_SIZE,
               "ERROR: cannot read \t%s\n",
               idl_input_file_name);
         IDL_Message(
               IDL_M_NAMED_GENERIC,
               IDL_MSG_LONGJMP,
               message_buffer1);

         return no_value;

      }

   }
   else
   {
      /** if we couldn't access file, return contron with message to IDL **/

      snprintf(
            message_buffer1,
            MAX_MESSAGE_BUFFER_SIZE,
            "ERROR: cannot read \t%s\n",
            idl_input_file_name);
      IDL_Message(
            IDL_M_NAMED_GENERIC,
            IDL_MSG_LONGJMP,
            message_buffer1);

      return no_value;

   }

   sprintf(
         message_buffer1,
         "Open %s complete...\n",
         idl_input_file_name);

   IDL_Message(
         IDL_M_NAMED_GENERIC,
         IDL_MSG_INFO,
         message_buffer1);

   /** now lets work attaining the specified attribute of interest **/

   status = CDFlib( GET_, ATTR_NUMBER_, idl_gattribute_name, &attrNum, NULL_);

   /** if we couldn't find the attribute number, return with message to calling idl program **/

   if (status != CDF_OK)
   {
      StatusHandler(status);
      sprintf(
            message_buffer1,
            "Couldn't find specified gattribute = %s\n",
            idl_gattribute_name);
      IDL_Message(
            IDL_M_NAMED_GENERIC,
            IDL_MSG_LONGJMP,
            message_buffer1);

      return no_value;

   }


   sprintf(
         message_buffer1,
         "%s ATTR_NUMBER_ = %d\n",
         idl_gattribute_name,
         attrNum);

   IDL_Message(
         IDL_M_NAMED_GENERIC,
         IDL_MSG_INFO,
         message_buffer1);



   /***********************************************
    * Get info about the attribute of interest    *
    ***********************************************/

    status = CDFlib( SELECT_, ATTR_, attrNum,
    SELECT_, gENTRY_, 0,
    GET_, ATTR_NUMgENTRIES_, &numEntries,
    GET_, gENTRY_DATATYPE_, &data_type,
    GET_, gENTRY_NUMELEMS_, &numElements,
    NULL_);

/*
   status = CDFlib(
   SELECT_, ATTR_, attrNum,
   GET_, ATTR_NUMgENTRIES_, &numEntries,
   GET_, gENTRY_DATATYPE_, &data_type,
   GET_, gENTRY_NUMELEMS_, &numElements,
   NULL_);
*/
   if (status != CDF_OK)
      StatusHandler(status);

   snprintf(
         message_buffer1,
         MAX_MESSAGE_BUFFER_SIZE,
         "DEBUG:\tattrNum = %d\nnumEntries = %d\ndata_type = %d\nnumElements = %d\n",
         attrNum,
         numEntries,
         data_type,
         numElements);

   IDL_Message(
         IDL_M_NAMED_GENERIC,
         IDL_MSG_INFO,
         message_buffer1);

   snprintf(
         message_buffer1,
         MAX_MESSAGE_BUFFER_SIZE,
         "data_type = %d ... CDF_CHAR   %d OR CDF_UCHAR %d\n",
         data_type,
         CDF_CHAR,
         CDF_UCHAR);

   IDL_Message(
         IDL_M_NAMED_GENERIC,
         IDL_MSG_INFO,
         message_buffer1);

   if (data_type == CDF_CHAR || data_type == CDF_UCHAR)
   {

      snprintf(
            message_buffer1,
            MAX_MESSAGE_BUFFER_SIZE,
            "INSIDE: CDF_CHAR|UCHAR\tdata_type = %d ... CDF_CHAR   %d OR CDF_UCHAR %d\n",
            data_type,
            CDF_CHAR,
            CDF_UCHAR);

      IDL_Message(
            IDL_M_NAMED_GENERIC,
            IDL_MSG_INFO,
            message_buffer1);

      tmp_char_value = ( char * ) malloc(numElements + 1);

      status = CDFlib( GET_, gENTRY_DATA_, tmp_char_value, NULL_);
      if (status != CDF_OK)
         StatusHandler(status);

      tmp_char_value[numElements] = '\0';

      char_attribute_valuePtr = tmp_char_value;

      /*
       if( strlen( char_attribute_valuePtr ) >= MAX_MESSAGE_BUFFER_SIZE)
       {
       fprintf( stderr, "%s\tWARNING:\tmessage buffer isn't large enough to hold returned value...displaying text < %d characters\n", __FILE__, MAX_MESSAGE_BUFFER_SIZE );
       }

       snprintf( message_buffer1, MAX_MESSAGE_BUFFER_SIZE, "extracted gaatribute = %s\n", char_attribute_valuePtr );

       if( char_attribute_valuePtr != NULL ) { IDL_Message(IDL_M_NAMED_GENERIC, IDL_MSG_INFO, message_buffer1 ); }
       */

      status = CDFclose ( current_file_id );
      if (status != CDF_OK)
      {
         StatusHandler(status);
      }

      return IDL_StrToSTRING(char_attribute_valuePtr);

   }
   else if (data_type == CDF_INT4)
   {

      tmp_ndims = 1;
      tmp_dims[0] = 1;

      status = CDFlib( GET_, gENTRY_DATA_, &tmp_int_value, NULL_);
      if (status != CDF_OK)
         StatusHandler(status);

      /** Create a temporary IDL variable and have our local pointer point to it **/

      int_attribute_valuePtr = (int *) IDL_MakeTempArray(
            IDL_TYP_LONG,
            tmp_ndims,
            tmp_dims,
            IDL_ARR_INI_ZERO,
            &idl_local_int_value);

      /** using our local pointer, modify the contents of the IDL Variable **/

      int_attribute_valuePtr[0] = tmp_int_value;

      /*fprintf(stderr, "WTF4.25 *int_attribute_valuePtr = %d\n", *int_attribute_valuePtr );*/

      status = CDFclose ( current_file_id );
      if (status != CDF_OK)
      {
         StatusHandler(status);
      }

      return idl_local_int_value;

   }
   else if (data_type == CDF_FLOAT)
   {

      tmp_ndims = 1;
      tmp_dims[0] = 1;

      status = CDFlib( GET_, gENTRY_DATA_, &tmp_float_value, NULL_);
      if (status != CDF_OK)
         StatusHandler(status);

      /** Create a temporary IDL variable and have our local pointer point to it **/

      float_attribute_valuePtr = (float *) IDL_MakeTempArray(
            IDL_TYP_FLOAT,
            tmp_ndims,
            tmp_dims,
            IDL_ARR_INI_NOP,
            &idl_local_float_value);

      /** using our local pointer, modify the contents of the IDL Variable **/

      float_attribute_valuePtr[0] = tmp_float_value;

      status = CDFclose ( current_file_id );
      if (status != CDF_OK)
      {
         StatusHandler(status);
      }

      return idl_local_float_value;
   }
   else
   {

      /*IDL_VPTR no_value= NULL;*/

      /*** code will NOT get here until we capture CDF's error of NO_SUCH_ATTR:..., and return control instead of dying ***/
      /*
       printf("ERROR:\tCouldn't determine data type for %s\n", attribute_name );
       return " ";

       */

      status = CDFclose ( current_file_id );
      if (status != CDF_OK)
      {
         StatusHandler(status);
      }

      snprintf(
            message_buffer1,
            MAX_MESSAGE_BUFFER_SIZE,
            "ERROR:\tData type for -->%s<-- Gattribute NOT Currently supported by Kameleon\n",
            IDL_VarGetString(gattribute_name) );

      IDL_Message(
            IDL_M_NAMED_GENERIC,
            IDL_MSG_INFO,
            message_buffer1);

      return no_value;

   }

   /*** program should never reach here ***/

}

