/*******************************************************************************
 *                                                                             *
 *      NAME:           write_structures_2_cdf.c                               *
 *                                                                             *
 *      VERSION:        4.1.1                                                  *
 *                                                                             *
 *      AUTHOR:         Marlo M. H. Maddox                                     *
 *                      NASA-GSFC-AETD-ISD-ADMA(587)                           *
 *                      Marlo.Maddox@nasa.gov                                  *
 *                      (301) 286 - 5202                                       *
 *                                                                             *
 *      PURPOSE:        specialized write routine that writes model data stored*
 *                      in the baselined_ccmc_attribute_structure_list &       *
 *                      baselined_ccmc_variable_structure_list structures      *
 *                      ( populated using the ccmc_structure_manager ) to cdf  *
 *                      format.                                                *
 *                                                                             *
 *      INPUTS:         *                                                      *
 *                                                                             *
 *      OUTPUTS:        *                                                      *
 *                                                                             *
 *      FUNCTIONS:      *                                                      *
 *                                                                             *
 *      VARIABLES:      *                                                      *
 *                                                                             *
 *                                                                             *
 *      MODIFICATION                                                           *
 *      HISTORY:                                                               *
 *                                                                             *
 *      04.21.2005      Maddox, Marlo                                          *
 *                      initial development started                            *
 *                                                                             *
 *      08.16.2005      Maddox, Marlo                                          *
 *                      added ccmc_registered_compression_algorithms.h and     *
 *                      functionality to dynamically set cdf compression       *
 *                      algorithm  based on command line arguments             *
 *                                                                             *
 *      01.04.2006      Maddox, Marlo                                          *
 *                      Changing all CDF aggument types form int to long.  They*
 *                      should have been long all along.  Problem is evident on*
 *                      64 bit mahines since the int & long sizes differ       *
 *                                                                             *
 *      01.06.2006      Maddox, Marlo                                          *
 *                      Changed getwd to getcwd and added int buf_size etc...  *
 *                                                                             *
 *      02.06.2007      Maddox, Marlo                                          *
 *                      changing ccmc_metadata_structure.h to                  *
 *                      ccmc_structure_manager.h                               *
 *                                                                             *
 *      02.16.2007      Maddox, Marlo                                          *
 *                      Adding a special file renaming case for mas data. Using*
 *                      the current_mas_step.h file, we will append the        *
 *                      current_mas_step value to the end of the generic string*
 *                      merged_mas_step_*                                      *
 *                                                                             *
 ******************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <math.h>

#include "ccmc_variable_attributes.h"
#include "ccmc_structure_manager.h"
#include "ccmc_registered_compression_algorithms.h"

#include "current_mas_step.h"

#include "cdf.h"

#define MAX_NEW_FILE_NAME_LENGTH    512
#define GZIP_LEVEL 7

/*** variables needed for dynamic cdf writes ***/
long current_entryNum, current_dataType, current_numElements, current_numDims =
      1, current_dimSizes[1] =
{ 1 }, current_dimVarys[1] =
{ VARY };
long current_counts[1] =
{ 0 };
void *current_Attribute_valuePtr, *current_Variable_valuePtr,
      *current_Variable_Attribute_valuePtr;

int write_structures_2_cdf(
      char *input_file_name,
      char *data_format,
      int file_name_length,
      int verbose_flag,
      char *output_directory,
      char *model_name,
      char *original_working_directory)
{

   int set_cdf_attr_arguments(int);
   int set_cdf_var_arguments(int);
   int set_cdf_var_attr_arguments(int, int);
   extern int StatusHandler(CDFstatus);

   /* declare required variables for use with cdf interface */

   CDFid id;
   CDFstatus status;

   long numDims = 1;
   long dimSizes[1] =
   { 1 }; /* set element 0 to number_of_cells later*/
   long majority = ROW_MAJOR;

   long dimVarys[1];
   long indices[1] =
   { 0 };
   long recNum;
   long attrNum;
   long cdf_varNum;

   /*variables needed for HyperPut*/
   long recordStart = 0;
   long recordCount = 1;
   long recordInterval = 1;
   long counts[1] =
   { 0 };

   /* this value will be dynamically set to variable_size structure element in
    * baselined_ccmc_variable_struct for each variable */

   long intervals[1] =
   { 1 };

   /* variables for compression algorithms */
   long gzip_level[] =
   { GZIP_LEVEL };
   long rle_level[] =
   { RLE_OF_ZEROs };
   long huff_level[] =
   { OPTIMAL_ENCODING_TREES };
   long ahuff_level[] =
   { OPTIMAL_ENCODING_TREES };

   int i, j, status2;

   char converted_file_name[file_name_length];
   char new_path_filename[MAX_NEW_FILE_NAME_LENGTH];
   char output_directory_copy[MAX_NEW_FILE_NAME_LENGTH];
   char base_file_name[MAX_NEW_FILE_NAME_LENGTH], *string_token;
   char *slash = "/";
   char current_working_directory[MAX_NEW_FILE_NAME_LENGTH],
         temp_current_working_directory[MAX_NEW_FILE_NAME_LENGTH];

   char *current_working_directoryPtr;
   int buf_size= MAX_NEW_FILE_NAME_LENGTH;

   /* before we do anything, make sure we have a
    * baselined_attribute_structure_list and a baselined_variable_structure_list
    */

   /*if we have user defined structures, they must be validated */

   strcpy(output_directory_copy, output_directory);
   strcpy(converted_file_name, input_file_name);

   /* extract base name from file name using subsequent call to strtok */
   string_token = strtok(converted_file_name, slash);

   while (string_token != NULL)
   {
      strcpy(base_file_name, string_token);
      if (verbose_flag)
      {
         printf(
               "extracting file name from path ...base_file_name = %s\n",
               base_file_name);
      }
      string_token = strtok( NULL, slash);
   }

   /***** special name formatting for mas data ****/

   if (strcmp(model_name, "mas") == 0)
   {
      /** since mas data is located in individual files per variable, we don't
       * want to name our cdf bp001.hdf.cdf for example.  Instead lets call them
       *  mas_merged_step_n **/

      sprintf(base_file_name, "mas_merged_step_%d", current_mas_step);
   }

   /* base_file_name should now contain base file name without path */

   if (verbose_flag)
   {
      printf("resulting base_file_name = %s\n", base_file_name);
   }

   current_working_directoryPtr = getcwd(
         temp_current_working_directory,
         buf_size);

   /* chdir to outdir and getcwd which returns directory path without appended
    * / ensures we add one each time */

   chdir(output_directory_copy);

   current_working_directoryPtr = getcwd(
         temp_current_working_directory,
         buf_size);

   /* change back to original working directory to when writing to
    * output_directory reference will still be the same */

   chdir(original_working_directory);

   strcpy(new_path_filename, temp_current_working_directory);

   strcat(new_path_filename, "/");

   strcat(new_path_filename, base_file_name);

   if (verbose_flag)
   {
      printf("new file name -----> %s\n", new_path_filename);
   }

   /****************************************************************************
    *                                   CREATE THE CDF                         *
    ***************************************************************************/

   status = CDFcreate(
         new_path_filename,
         numDims,
         dimSizes,
         NETWORK_ENCODING,
         majority,
         &id);
   if (status != CDF_OK)
   {
      StatusHandler(status);
   }

   if (verbose_flag)
   {
      printf("\nCDF file created...\n");
   }

   /********************* set cdf compression type *******************/

   if (verbose_flag)
      printf("\nSetting Compression -->%s<--...\n\n", current_compression);

   if ( !strcmp(current_compression, RLE) )
   {
      status
            = CDFlib(PUT_, CDF_COMPRESSION_, RLE_COMPRESSION, rle_level, NULL_);
   }
   else if ( !strcmp(current_compression, HUFF) )
   {
      status = CDFlib(
            PUT_,
            CDF_COMPRESSION_,
            HUFF_COMPRESSION,
            huff_level,
            NULL_);
   }
   else if ( !strcmp(current_compression, AHUFF) )
   {
      status = CDFlib(
            PUT_,
            CDF_COMPRESSION_,
            AHUFF_COMPRESSION,
            ahuff_level,
            NULL_);
   }
   else if ( !strcmp(current_compression, "gzip") )
   {

      status = CDFlib(
            PUT_,
            CDF_COMPRESSION_,
            GZIP_COMPRESSION,
            gzip_level,
            NULL_);
   }

   if (status != CDF_OK)
      StatusHandler(status);

   /*
    status = CDFlib (PUT_, CDF_COMPRESSION_, GZIP_COMPRESSION,gzip_level, NULL_);
    if(status != CDF_OK) StatusHandler (status);
    */

   /****************************************************************************
    *                    CREATE & WRITE CDF GLOBAL ATTRIBUTES                  *
    ***************************************************************************/

   /****************************************************************************
    * write data stored inside baselined_ccmc_attribute_structure_list to the  *
    * cdf file                                                                 *
    ***************************************************************************/

   if (verbose_flag)
   {
      printf("creating global attributes & writing values...\n");
   }

   for (i = 0; i < put_attribute_call_count; i++)
   {

      status2 = set_cdf_attr_arguments(i);

      if ( !status2) /** set_cdf_attr_arguments returned successfully **/
      {

         /** create the attribute **/
         status = CDFlib(
               CREATE_,
               ATTR_,
               baselined_ccmc_attribute_structure_list[i].attribute_name,
               GLOBAL_SCOPE,
               &attrNum,
               NULL_);
         if (status != CDF_OK)
            StatusHandler(status);

         /** write the attribute value **/
         status
               = CDFattrPut(
                     id,
                     CDFattrNum(
                           id,
                           baselined_ccmc_attribute_structure_list[i].attribute_name),
                     current_entryNum,
                     current_dataType,
                     current_numElements,
                     current_Attribute_valuePtr);
         if (status != CDF_OK)
            StatusHandler(status);

         if (verbose_flag)
         {
            printf(
                  "adding global attribute ( %d )\t%s\n",
                  i,
                  baselined_ccmc_attribute_structure_list[i].attribute_name);
         }

      }
      else
      {
         printf(
               "WARNING:\tUnable to write global attribute %s to CDF file...\n",
               baselined_ccmc_attribute_structure_list[i].attribute_name);
      }

   }

   if (verbose_flag)
   {
      printf("global attributes created successfully...\n");
   }

   /***************************************************************************/

   /****************************************************************************
    * create variable attributes stored inside                                 *
    * ccmc_variable_attribute_name_type_list                                   *
    ***************************************************************************/

   if (verbose_flag)
   {
      printf("creating variable attributes...\n");
   }

   for (i = 0; i < NUMBER_CCMC_VARIABLE_ATTR; i++)
   {
      status = CDFlib(
            CREATE_,
            ATTR_,
            ccmc_variable_attribute_name_type_list[i].ccmc_var_attr_name,
            VARIABLE_SCOPE,
            &attrNum,
            NULL_);
      if (status != CDF_OK)
         StatusHandler(status);

      if (verbose_flag)
      {
         printf(
               "creating variable attribute ( %d )\t%s\n",
               i,
               ccmc_variable_attribute_name_type_list[i].ccmc_var_attr_name);
      }
   }

   if (verbose_flag)
   {
      printf("variable attributes created successfully...\n");
   }

   /****************************************************************************
    *                                CREATE CDF VARIABLEs                      *
    ***************************************************************************/

   /****************************************************************************
    *           create variables stored inside                                 *
    *           baselined_ccmc_variable_structure_list                         *
    ***************************************************************************/

   if (verbose_flag)
   {
      printf("creating variables & writing variable attribute values...\n");
   }

   for (i = 0; i < put_variable_call_count; i++)
   {

      if (verbose_flag)
      {
         printf(
               "creating & writing values for variable ( %d )\t%s\t",
               i,
               baselined_ccmc_variable_structure_list[i].variable_name);
      }

      status2 = set_cdf_var_arguments(i);

      if ( !status2) /** set_cdf_var_arguments returned successfully **/
      {

         /* lets not even bother to create a variable if current_dimSizes ! >= 0 */

         /*
          printf("DEBUG\tcurrent_numElements = %d\ncurrent_numDims = "
          "%d\ncurrent_dimSizes = %d\nVARY = %d\ncurrent_dimVarys = %d\n",
          current_numElements, current_numDims, current_dimSizes[0], VARY,
          current_dimVarys[0]);
          */

         if (current_dimSizes[0] > 0)
         {
            status = CDFlib(
                  CREATE_,
                  zVAR_,
                  baselined_ccmc_variable_structure_list[i].variable_name,
                  current_dataType,
                  current_numElements,
                  current_numDims,
                  current_dimSizes,
                  VARY,
                  current_dimVarys,
                  &cdf_varNum,
                  NULL_);
            if (status != CDF_OK)
               StatusHandler(status);

            /*printf("\nDEBUG\tafter zvar create\n");*/

            status = CDFlib(
                  SELECT_,
                  zVAR_,
                  cdf_varNum,
                  SELECT_,
                  zVAR_RECNUMBER_,
                  recordStart,
                  SELECT_,
                  zVAR_RECCOUNT_,
                  recordCount,
                  SELECT_,
                  zVAR_RECINTERVAL_,
                  recordInterval,
                  SELECT_,
                  zVAR_DIMINDICES_,
                  indices,
                  SELECT_,
                  zVAR_DIMCOUNTS_,
                  current_counts,
                  SELECT_,
                  zVAR_DIMINTERVALS_,
                  intervals,
                  PUT_,
                  zVAR_HYPERDATA_,
                  current_Variable_valuePtr,
                  NULL_);
            if (status != CDF_OK)
               StatusHandler(status);

            if (verbose_flag)
            {
               printf("done\nwriting variable attribute values\t");
            }

            /*******************************************************************
             *  For each variable, write the variable attribute values stored  *
             *  inside the var struct                                          *
             ******************************************************************/

            for (j = 0; j < NUMBER_CCMC_VARIABLE_ATTR; j++)
            {

               /** send the current variable structute number and current ccmc
                * variable attribute structure number to set* routine **/

               status2 = set_cdf_var_attr_arguments(i, j);

               if ( !status2)
               {

                  status
                        = CDFlib(
                              SELECT_,
                              ATTR_NAME_,
                              ccmc_variable_attribute_name_type_list[j].ccmc_var_attr_name,
                              SELECT_,
                              zVAR_,
                              cdf_varNum,
                              SELECT_,
                              zENTRY_NAME_,
                              baselined_ccmc_variable_structure_list[i].variable_name,
                              PUT_,
                              zENTRY_DATA_,
                              current_dataType,
                              current_numElements,
                              current_Variable_Attribute_valuePtr,
                              NULL_);
               }
               else
               {
                  printf(
                        "WARNING:\tUnable to write variable attribute value for "
                           "%s to CDF file...\n",
                        ccmc_variable_attribute_name_type_list[j].ccmc_var_attr_name);
               }
            }
         } /* end of if current_dimSizes > 0 */
         else /** alert user that current variable was not created because it has no values **/
         {
            printf(
                  "WARNING:\t Variable %s was NOT created beacuase it size is 0 "
                     "or less\n",
                  baselined_ccmc_variable_structure_list[i].variable_name);
         }

         if (verbose_flag)
         {
            printf("done\n");
         }

      } /** end of if( status2 ) for set_cdf_var_arguments **/
      else /** set_cdf_var_arguments failed for current variable **/
      {
         printf(
               "WARNING:\tUnable to write variable %s to CDF file...\n",
               baselined_ccmc_variable_structure_list[i].variable_name);
      }

   } /** end of for each variable **/

   status = CDFclose(id);
   if (status != CDF_OK)
      StatusHandler(status);

   if (verbose_flag)
   {
      printf("variable attribute values added successfully...\n");
   }
   if (verbose_flag)
   {
      printf("cleaning attribute structure...");
   }

   status2 = free_ccmc_attribute_structure(
         baselined_ccmc_attribute_structure_list,
         put_attribute_call_count);
   if (verbose_flag)
   {
      printf("done\ncleaning variable structure...");
   }

   status2 = free_ccmc_variable_structure(
         baselined_ccmc_variable_structure_list,
         put_variable_call_count);
   if (verbose_flag)
   {
      printf("done\n");
   }

   return 0;

}

int set_cdf_attr_arguments(int struct_number)
{

   /** set unique cdf required arguments based on current attribute data type **/
   if ( !strcmp(
         baselined_ccmc_attribute_structure_list[struct_number].attribute_data_type,
         "char") )
   {
      current_entryNum = 0;
      current_dataType = CDF_CHAR;
      current_numElements
            = strlen(baselined_ccmc_attribute_structure_list[struct_number].Attribute_value.string_value);
      current_Attribute_valuePtr
            = ( struct ccmc_attribute_structure * ) baselined_ccmc_attribute_structure_list[struct_number].Attribute_value.string_value;
      if (current_numElements > 0)
         return 0; /** if the stringlen <= 0 there is no need to try to write value to CDF as this will confuse the CDFattrPut call if numElements is zero **/
      else
         return 1;
   }
   else if ( !strcmp(
         baselined_ccmc_attribute_structure_list[struct_number].attribute_data_type,
         "int") )
   {
      current_entryNum = 0;
      current_dataType = CDF_INT4;
      current_numElements = 1;
      current_Attribute_valuePtr
            = ( struct ccmc_attribute_structure * ) &baselined_ccmc_attribute_structure_list[struct_number].Attribute_value.int_value;
      return 0;
   }
   else if ( !strcmp(
         baselined_ccmc_attribute_structure_list[struct_number].attribute_data_type,
         "float") )
   {
      current_entryNum = 0;
      current_dataType = CDF_FLOAT;
      current_numElements = 1;
      current_Attribute_valuePtr
            = ( struct ccmc_attribute_structure * ) &baselined_ccmc_attribute_structure_list[struct_number].Attribute_value.float_value;
      return 0;
   }
   else if ( !strcmp(
         baselined_ccmc_attribute_structure_list[struct_number].attribute_data_type,
         "double") )
   {
      current_entryNum = 0;
      current_dataType = CDF_DOUBLE;
      current_numElements = 1;
      current_Attribute_valuePtr
            = ( struct ccmc_attribute_structure * ) &baselined_ccmc_attribute_structure_list[struct_number].Attribute_value.double_value;
      return 0;
   }
   else
   {
      printf(
            "WARNING\tdata type %s not supported\n",
            baselined_ccmc_attribute_structure_list[struct_number].attribute_data_type);
      return 1;
   }

}

int set_cdf_var_arguments(int struct_number)
{

   /* we need strict checking of the variable_size structure elemet because
    * cdf is sensitive with regards to DIM_SIZE add restrction here but for know
    * lets check current_dimSizes[0] before use in write_structure_2_cdf */

   /** set unique cdf required arguments based on current variable data type and size**/

   if ( !strcmp(
         baselined_ccmc_variable_structure_list[struct_number].variable_data_type,
         "int") )
   {
      current_dataType = CDF_INT4;
      current_numElements = 1;
      /* we may expand structure to hold the number of dimension for future
       * multi dimensional variables, for now all are flat **/
      current_numDims = 1;
      current_dimSizes[0]
            = baselined_ccmc_variable_structure_list[struct_number].variable_size;
      current_dimVarys[0] = VARY;
      current_Variable_valuePtr
            = ( struct ccmc_variable_structure * ) baselined_ccmc_variable_structure_list[struct_number].Variable_values.int_value;

      /** if we add multi dimensional variables we could add current_counts[1] =
       *  current_dimSizes[1] ...**/

      current_counts[0] = current_dimSizes[0];
      return 0;
   }
   else if ( !strcmp(
         baselined_ccmc_variable_structure_list[struct_number].variable_data_type,
         "float") )
   {
      current_dataType = CDF_FLOAT;
      current_numElements = 1;
      current_numDims = 1; /** we may expand structure to hold the number of dimension for future multi dimensional variables, for now all are flat **/
      current_dimSizes[0]
            = baselined_ccmc_variable_structure_list[struct_number].variable_size;
      current_dimVarys[0] = VARY;
      current_Variable_valuePtr
            = ( struct ccmc_variable_structure * ) baselined_ccmc_variable_structure_list[struct_number].Variable_values.int_value;
      current_counts[0] = current_dimSizes[0]; /** if we add multi dimensional variables we could add current_counts[1] = current_dimSizes[1] ...**/
      return 0;
   }
   else if ( !strcmp(
         baselined_ccmc_variable_structure_list[struct_number].variable_data_type,
         "double") )
   {
      current_dataType = CDF_DOUBLE;
      current_numElements = 1;
      current_numDims = 1; /** we may expand structure to hold the number of dimension for future multi dimensional variables, for now all are flat **/
      current_dimSizes[0]
            = baselined_ccmc_variable_structure_list[struct_number].variable_size;
      current_dimVarys[0] = VARY;
      current_Variable_valuePtr
            = ( struct ccmc_variable_structure * ) baselined_ccmc_variable_structure_list[struct_number].Variable_values.int_value;
      current_counts[0] = current_dimSizes[0]; /** if we add multi dimensional variables we could add current_counts[1] = current_dimSizes[1] ...**/
      return 0;
   }
   else
   {
      printf(
            "WARNING\tdata type %s not supported\n",
            baselined_ccmc_variable_structure_list[struct_number].variable_data_type);
      return 1;
   }

}

int set_cdf_var_attr_arguments(int struct_number, int current_var_attr)
{

   /** set unique cdf required arguments based on current attribute name & data type **/

   /** get the data type of the current variable attribute being processed **/

   if ( !strcmp(
         ccmc_variable_attribute_name_type_list[current_var_attr].ccmc_var_attr_data_type,
         "char") )
   {

      current_dataType = CDF_CHAR;

      /*** based on the current variable attribute name we are processing, set the char attribute value accordingly ***/

      if ( !strcmp(
            ccmc_variable_attribute_name_type_list[current_var_attr].ccmc_var_attr_name,
            "units") )
      {
         current_numElements
               = strlen(baselined_ccmc_variable_structure_list[struct_number].units);
         current_Variable_Attribute_valuePtr
               = ( struct ccmc_variable_structure * ) baselined_ccmc_variable_structure_list[struct_number].units;
      }
      else if ( !strcmp(
            ccmc_variable_attribute_name_type_list[current_var_attr].ccmc_var_attr_name,
            "grid_system") )
      {
         current_numElements
               = strlen(baselined_ccmc_variable_structure_list[struct_number].grid_system);
         current_Variable_Attribute_valuePtr
               = ( struct ccmc_variable_structure * ) baselined_ccmc_variable_structure_list[struct_number].grid_system;
      }
      else if ( !strcmp(
            ccmc_variable_attribute_name_type_list[current_var_attr].ccmc_var_attr_name,
            "description") )
      {
         current_numElements
               = strlen(baselined_ccmc_variable_structure_list[struct_number].description);
         current_Variable_Attribute_valuePtr
               = ( struct ccmc_variable_structure * ) baselined_ccmc_variable_structure_list[struct_number].description;
      }
      else if ( !strcmp(
            ccmc_variable_attribute_name_type_list[current_var_attr].ccmc_var_attr_name,
            "position_grid_system") )
      {
         current_numElements
               = strlen(baselined_ccmc_variable_structure_list[struct_number].position_grid_system);
         current_Variable_Attribute_valuePtr
               = ( struct ccmc_variable_structure * ) baselined_ccmc_variable_structure_list[struct_number].position_grid_system;
      }
      else if ( !strcmp(
            ccmc_variable_attribute_name_type_list[current_var_attr].ccmc_var_attr_name,
            "data_grid_system") )
      {
         current_numElements
               = strlen(baselined_ccmc_variable_structure_list[struct_number].data_grid_system);
         current_Variable_Attribute_valuePtr
               = ( struct ccmc_variable_structure * ) baselined_ccmc_variable_structure_list[struct_number].data_grid_system;
      }
      else
      {
         printf(
               "WARNING\tUnregistered Variable Attribute name %s...NOT adding value to CDF\n",
               ccmc_variable_attribute_name_type_list[current_var_attr].ccmc_var_attr_name);
         return 1;
      }

      if (current_numElements)
      {
         return 0;
      } /** if the stringlen == 0 there is no need to try to write value to CDF as this will confuse the CDFattrPut call if numElements is zero **/
      return 1;
   }
   else if ( !strcmp(
         ccmc_variable_attribute_name_type_list[current_var_attr].ccmc_var_attr_data_type,
         "int") )
   {
      current_dataType = CDF_INT4;
      current_numElements = 1;

      /*** based on the current variable attribute name we are processing, set the int attribute value accordingly ***/

      if ( !strcmp(
            ccmc_variable_attribute_name_type_list[current_var_attr].ccmc_var_attr_name,
            "is_vector_component") )
      {
         current_Variable_Attribute_valuePtr
               = ( struct ccmc_variable_structure * ) &baselined_ccmc_variable_structure_list[struct_number].is_vector_component;
      }
      else
      {
         printf(
               "WARNING\tUnregistered Variable Attribute name %s...NOT adding value to CDF\n",
               ccmc_variable_attribute_name_type_list[current_var_attr].ccmc_var_attr_name);
         return 1;
      }

      return 0;
   }
   else if ( !strcmp(
         ccmc_variable_attribute_name_type_list[current_var_attr].ccmc_var_attr_data_type,
         "float") )
   {
      current_dataType = CDF_FLOAT;
      current_numElements = 1;

      /*** based on the current variable attribute name we are processing, set the float attribute value accordingly ***/

      if ( !strcmp(
            ccmc_variable_attribute_name_type_list[current_var_attr].ccmc_var_attr_name,
            "valid_min") )
      {
         current_Variable_Attribute_valuePtr
               = ( struct ccmc_variable_structure * ) &baselined_ccmc_variable_structure_list[struct_number].valid_min;
      }
      else if ( !strcmp(
            ccmc_variable_attribute_name_type_list[current_var_attr].ccmc_var_attr_name,
            "valid_max") )
      {
         current_Variable_Attribute_valuePtr
               = ( struct ccmc_variable_structure * ) &baselined_ccmc_variable_structure_list[struct_number].valid_max;
      }
      else if ( !strcmp(
            ccmc_variable_attribute_name_type_list[current_var_attr].ccmc_var_attr_name,
            "mask") )
      {
         current_Variable_Attribute_valuePtr
               = ( struct ccmc_variable_structure * ) &baselined_ccmc_variable_structure_list[struct_number].mask;
      }
      else if ( !strcmp(
            ccmc_variable_attribute_name_type_list[current_var_attr].ccmc_var_attr_name,
            "actual_min") )
      {
         current_Variable_Attribute_valuePtr
               = ( struct ccmc_variable_structure * ) &baselined_ccmc_variable_structure_list[struct_number].actual_min;
      }
      else if ( !strcmp(
            ccmc_variable_attribute_name_type_list[current_var_attr].ccmc_var_attr_name,
            "actual_max") )
      {
         current_Variable_Attribute_valuePtr
               = ( struct ccmc_variable_structure * ) &baselined_ccmc_variable_structure_list[struct_number].actual_max;
      }
      else
      {
         printf(
               "WARNING\tUnregistered Variable Attribute name %s...NOT adding value to CDF\n",
               ccmc_variable_attribute_name_type_list[current_var_attr].ccmc_var_attr_name);
         return 1;
      }

      return 0;
   }
   else if ( !strcmp(
         ccmc_variable_attribute_name_type_list[current_var_attr].ccmc_var_attr_data_type,
         "double") )
   {

      /******* There are currently NO Variable Attributes that have double data type *******/

      current_dataType = CDF_DOUBLE;
      current_numElements = 1;
      return 1;
   }
   else
   {
      printf(
            "WARNING\tVariable Attribute data type %s not supported\n",
            ccmc_variable_attribute_name_type_list[current_var_attr].ccmc_var_attr_data_type);
      return 1;
   }

}

