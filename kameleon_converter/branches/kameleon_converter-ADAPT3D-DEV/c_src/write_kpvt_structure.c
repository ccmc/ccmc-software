/*******************************************************************************
 *                                                                             *
 *      NAME:                write_kpvt_structure.c                            *
 *                                                                             *
 *      VERSION:            1.0                                                *
 *                                                                             *
 *      AUTHOR:             Marlo M. H. Maddox                                 *
 *                          NASA-GSFC-AETD-ISD-ADMA(587)                       *
 *                          Marlo.Maddox@nasa.gov                              *
 *                          (301) 286 - 5202                                   *
 *                                                                             *
 *      PURPOSE:            specialized write routine that writes kpvt data    *
 *                          from main mem to CCMC standardized data structures.*
 *                                                                             *
 *      INPUTS:             *                                                  *
 *                                                                             *
 *      OUTPUTS:            *                                                  *
 *                                                                             *
 *      FUNCTIONS:          *                                                  *
 *                                                                             *
 *      VARIABLES:          *                                                  *
 *                                                                             *
 *                                                                             *
 *      MODIFICATION                                                           *
 *      HISTORY:                                                               *
 *                                                                             *
 *      11.29.2006          Maddox, Marlo                                      *
 *                          initial development started                        *
 *                                                                             *
 *      12.01.2006          Maddox, Marlo                                      *
 *                          Changed from 360 x 180 to 180 x 360 or             *
 *                          data[c_sine_lat, c_lon]                            *
 *                                                                             *
 *      02.06.2007          Maddox, Marlo                                      *
 *                          changing ccmc_metadata_structure.h to              *
 *                          ccmc_structure_manager.h                           *
 *                                                                             *
 *      05.17.2007          Maddox, Marlo                                      *
 *                          adding update for kameleon_version gattribute      *
 ******************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <math.h>

#include "cdf.h"
#include "read_database.h"
#include "ccmc_attributes.h"
#include "ccmc_variable_attributes.h"
#include "ccmc_registered_variables.h"
#include "ccmc_structure_manager.h"
#include "kpvt_attributes.h"

#include "read_kpvt.h"

#include "kameleon_version_info.h"

#define MAX_NEW_FILE_NAME_LENGTH    512
#define NUMBER_GLOBAL_ATTR NUMBER_CCMC_ATTR + NUMBER_KPVT_ATTR
#define NUMBER_VARIABLE_ATTR NUMBER_CCMC_VARIABLE_ATTR + NUMBER_KPVT_VARIABLE_ATTR

#define DEBUG_FLAG 1

int write_kpvt_structure(
      char *input_file_name,
      char *data_format,
      int file_name_length,
      int verbose_flag,
      char *output_directory,
      char *model_name,
      char *original_working_directory,
      int write_database_info_flag)
{

   extern int update_ccmc_variable_attribute_value(
         char *,
         char *,
         void *,
         struct ccmc_variable_structure *);

   int i, j, k, stat, local_size;

   float zero = 0.0;

   /**** values used to set dipole_time value if dipole_update = yes ***/

   double cdf_epoch_start_time_value, cdf_epoch_dipole_time_value;

   /* define static values for ccmc_attributes for kpvt_model */

   char *kameleon_version_num= KAMELEON_VERSION;

   char *readme_text = "********** PROTOTYPE ************";
   char *terms_of_usage_text =
         "For tracking purposes for our government sponsors, we ask "
            "that you notify the CCMC whenever you use CCMC results in "
            "a scientific publications or presentations: ccmc@ccmc.gsfc"
            ".nasa.gov ";

   char *model_type = "********** PROTOTYPE ************ Observation Data";
   int grid_system_count = 1;
   char *output_type = "********** PROTOTYPE ************ Magnetogram";
   char *grid_system_1 = "[ c_sine_lat, c_lon ]";
   char *standard_grid_target =
         "********** PROTOTYPE ************ [ c_sine_lat, c_lon ]";

   char *default_grid_type =
         "********** PROTOTYPE ************ [ c_sine_lat, c_lon ]";
   char *grid_1_type =
         "********** PROTOTYPE ************ [ c_sine_lat, c_lon ]";

   /* set not_applicable string for use when dipole time makes no sense */

   char *not_applicable = "N/A";

   /** assuming 2... add smarts later **/

   int grid_system_1_number_of_dimensions = 2;

   /*changed in vain i believe... char converted_file_name[file_name_length];*/
   char converted_file_name[MAX_NEW_FILE_NAME_LENGTH];
   char new_path_filename[MAX_NEW_FILE_NAME_LENGTH];

   char output_directory_copy[MAX_NEW_FILE_NAME_LENGTH];

   char base_file_name[MAX_NEW_FILE_NAME_LENGTH], *string_token, *string_token2;
   char *slash = "/";
   char *dot = ".";
   char *current_working_directoryPtr;
   char temp_current_working_directory[MAX_NEW_FILE_NAME_LENGTH];

   int buf_size= MAX_NEW_FILE_NAME_LENGTH;

   /* naxis1, naxis2 defined in read_kpvt.h */

   int number_of_cells, x_dimension_size = naxis2, y_dimension_size = naxis1,
         z_dimension_size = 0;

   /********* input_file_name has strange behavior ***********/

   char input_file_name_local_copy[MAX_NEW_FILE_NAME_LENGTH];

   /* set dynamic values for ccmc_attributes for kpvt */
   /*  call/perform  necessary functions to set values for dynamic global
    * attributes */

   int grid_system_1_dimension_1_size = x_dimension_size;
   int grid_system_1_dimension_2_size = y_dimension_size;
   int grid_system_1_dimension_3_size = z_dimension_size;

   strcpy(input_file_name_local_copy, input_file_name);
   input_file_name_local_copy[ file_name_length] = '\0';

   if ( DEBUG_FLAG)
      printf(
            "DEBUG\tINSIDE write routine\n\t\tinput_file_name argument = "
               "%s\nfile_name_length argument = %d\n",
            input_file_name,
            file_name_length);

   strcpy(output_directory_copy, output_directory);
   strcpy(converted_file_name, input_file_name_local_copy);

   /* extract base name from file name using subsequent calls to strtok */

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

   /*base_file_name should now contain base file name without path */

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

   /* new_path_filename is were we are creating/writing output two */

   number_of_cells = x_dimension_size * y_dimension_size;

   /****************************************************************************
    *                                CREATE THE GLOBAL ATTRIBUTES              *
    ***************************************************************************/

   /*********************************** GLOBAL ATTRIBUTES ********************/

   /****************************************************************************
    *   initialize the baselined_ccmc_attribute_structure_list to hold n       *
    *     number of cccmc_attribute_structures where n = NUMBER_GLOBAL_ATTR =  *
    *     NUMBER_CCMC_ATTR + NUMBER_KPVT_ATTR as defined in ccmc_attributes.h  *
    *     & kpvt_attributes.h                                                  *
    ***************************************************************************/

   baselined_ccmc_attribute_structure_list
         = init_ccmc_attribute_structure( NUMBER_GLOBAL_ATTR);

   /* for each name-data type pair in the ccmc_attribute_name_type_list, call
    * put_ccmc_attribute with attr name & data type since we are writing from
    * the ccmc_attribute_name_type_list, the attribute_type = global ***/

   for (i=0; i < NUMBER_CCMC_ATTR; i++)
   {
      put_ccmc_attribute(
            ccmc_attribute_name_type_list[i].ccmc_attr_name,
            "global",
            ccmc_attribute_name_type_list[i].ccmc_attr_data_type,
            "",
            baselined_ccmc_attribute_structure_list);
   }

   /* for each name-data type pair in the kpvt_attribute_name_type_list, call
    * put_ccmc_attribute with attr name & data type since we are writing from
    * the kpvt_attribute_name_type_list, the attribute_type = model ***/

   for (i=0; i < NUMBER_KPVT_ATTR; i++)
   {
      put_ccmc_attribute(
            kpvt_attribute_name_type_list[i].kpvt_attr_name,
            "model",
            kpvt_attribute_name_type_list[i].kpvt_attr_data_type,
            "",
            baselined_ccmc_attribute_structure_list);

   }

   if (verbose_flag)
   {
      printf("global attribute names succeffully added to "
            "baselined_ccmc_attribute_structure_list...\n");
   }

   /****************************************************************************
    *                            WRITE SPECIFIC GLOBAL ATTRIBUTE VALUES        *
    ***************************************************************************/

   /*************** WRITE SPECFIC GLOBAL ATTRIBUTE VALUES *********************/

   /*************** individual ccmc global attributes set by name *************/

   stat = update_ccmc_attribute_value(
         "README",
         readme_text,
         baselined_ccmc_attribute_structure_list);
   stat = update_ccmc_attribute_value(
         "model_name",
         model_name,
         baselined_ccmc_attribute_structure_list);
   stat = update_ccmc_attribute_value(
         "model_type",
         model_type,
         baselined_ccmc_attribute_structure_list);
   stat = update_ccmc_attribute_value(
         "original_output_file_name",
         input_file_name,
         baselined_ccmc_attribute_structure_list);
   stat = update_ccmc_attribute_value(
         "terms_of_usage",
         terms_of_usage_text,
         baselined_ccmc_attribute_structure_list);
   stat = update_ccmc_attribute_value(
         "grid_system_count",
         &grid_system_count,
         baselined_ccmc_attribute_structure_list);
   stat = update_ccmc_attribute_value(
         "grid_system_1_number_of_dimensions",
         &grid_system_1_number_of_dimensions,
         baselined_ccmc_attribute_structure_list);
   stat = update_ccmc_attribute_value(
         "grid_system_1_dimension_1_size",
         &grid_system_1_dimension_1_size,
         baselined_ccmc_attribute_structure_list);
   stat = update_ccmc_attribute_value(
         "grid_system_1_dimension_2_size",
         &grid_system_1_dimension_2_size,
         baselined_ccmc_attribute_structure_list);
   stat = update_ccmc_attribute_value(
         "grid_system_1_dimension_3_size",
         &grid_system_1_dimension_3_size,
         baselined_ccmc_attribute_structure_list);
   stat = update_ccmc_attribute_value(
         "grid_system_1",
         grid_system_1,
         baselined_ccmc_attribute_structure_list);
   stat = update_ccmc_attribute_value(
         "grid_1_type",
         grid_1_type,
         baselined_ccmc_attribute_structure_list);
   stat = update_ccmc_attribute_value(
         "output_type",
         output_type,
         baselined_ccmc_attribute_structure_list);
   stat = update_ccmc_attribute_value(
         "standard_grid_target",
         standard_grid_target,
         baselined_ccmc_attribute_structure_list);
   stat = update_ccmc_attribute_value(
         "kameleon_version",
         kameleon_version_num,
         baselined_ccmc_attribute_structure_list);

   /****************** add attributes read in from DatabaseInfo file using
    * read_database.c code *****/

   if (verbose_flag)
   {
      printf("global attributes values successfully added to "
            "baselined_ccmc_attribute_structure_list...\n");
   }

   /****************************************************************************
    *                        CREATE THE VARIABLES & VARIABLE ATTRIBUTES        *
    ***************************************************************************/

   /****************************************************************************
    *   initialize the baselined_ccmc_variable_structure_list to hold n number *
    *   of cccmc_variable_structures where n = NUMBER_REGISTERED_KPVT_VARIABLES*
    *   defined in ccmc_registered_variables.h                                 *
    ***************************************************************************/
   baselined_ccmc_variable_structure_list
         = init_ccmc_variable_structure( NUMBER_REGISTERED_KPVT_VARIABLES);

   /* Put default variable names and variable attribute values into the
    * ccmc_baselined_variable_structure_list ***/

   for (i = 0; i< NUMBER_REGISTERED_KPVT_VARIABLES; i++)
   {
      put_ccmc_variable(
            registered_kpvt_var_list[i].ccmc_var_name,
            registered_kpvt_var_list[i].ccmc_var_data_type,
            0,
            registered_kpvt_var_list[i].ccmc_var_classification,
            "",
            registered_kpvt_var_list[i].valid_min,
            registered_kpvt_var_list[i].valid_max,
            registered_kpvt_var_list[i].units,
            registered_kpvt_var_list[i].grid_system,
            registered_kpvt_var_list[i].mask,
            registered_kpvt_var_list[i].description,
            registered_kpvt_var_list[i].is_vector_component,
            registered_kpvt_var_list[i].position_grid_system,
            registered_kpvt_var_list[i].data_grid_system,
            zero,
            zero,
            baselined_ccmc_variable_structure_list);
   }

   if (verbose_flag)
   {
      printf("registered variable names and default variable attribute values "
            "succeffully added to baselined_ccmc_variable_structure_list...\n");
   }

   /****************************************************************************
    *   write the actual variable values into the                              *
    *   baselined_ccmc_variable_structure_list    08/25/05 - lets add the      *
    *   actual min/max values at the same time                                 *
    ***************************************************************************/

   /*** write the original model variable values ***/

   if (verbose_flag)
   {
      printf("\nwritting original model output variables values into "
            "baselined_ccmc_variable_structure_list...\n");
   }

   /*** update each variable size as the defualts are set to zero, this reallocs
    *  space for the actual values to be stored *****/

   /* write position variables with two components
    * (carrington_longitiude,carrington_sine_latitude)  *********/

   stat = update_ccmc_variable_attribute_value(
         "c_sine_lat",
         "variable_size",
         &x_dimension_size,
         baselined_ccmc_variable_structure_list);
   stat = update_ccmc_variable_value(
         "c_sine_lat",
         x_dimension_size,
         carrington_sine_latitude,
         baselined_ccmc_variable_structure_list);
   stat = update_ccmc_variable_attribute_value(
         "c_sine_lat",
         "actual_min",
         &carrington_sine_latitude_actual_min,
         baselined_ccmc_variable_structure_list);
   stat = update_ccmc_variable_attribute_value(
         "c_sine_lat",
         "actual_max",
         &carrington_sine_latitude_actual_max,
         baselined_ccmc_variable_structure_list);

   stat = update_ccmc_variable_attribute_value(
         "c_lon",
         "variable_size",
         &y_dimension_size,
         baselined_ccmc_variable_structure_list);
   stat = update_ccmc_variable_value(
         "c_lon",
         y_dimension_size,
         carrington_longitiude,
         baselined_ccmc_variable_structure_list);
   stat = update_ccmc_variable_attribute_value(
         "c_lon",
         "actual_min",
         &carrington_longitiude_actual_min,
         baselined_ccmc_variable_structure_list);
   stat = update_ccmc_variable_attribute_value(
         "c_lon",
         "actual_max",
         &carrington_longitiude_actual_max,
         baselined_ccmc_variable_structure_list);

   stat = update_ccmc_variable_attribute_value(
         "net_flux",
         "variable_size",
         &number_of_cells,
         baselined_ccmc_variable_structure_list);
   stat = update_ccmc_variable_value(
         "net_flux",
         number_of_cells,
         net_flux,
         baselined_ccmc_variable_structure_list);
   stat = update_ccmc_variable_attribute_value(
         "net_flux",
         "actual_min",
         &net_flux_actual_min,
         baselined_ccmc_variable_structure_list);
   stat = update_ccmc_variable_attribute_value(
         "net_flux",
         "actual_max",
         &net_flux_actual_max,
         baselined_ccmc_variable_structure_list);

   stat = update_ccmc_variable_attribute_value(
         "total_flux",
         "variable_size",
         &number_of_cells,
         baselined_ccmc_variable_structure_list);
   stat = update_ccmc_variable_value(
         "total_flux",
         number_of_cells,
         total_flux,
         baselined_ccmc_variable_structure_list);
   stat = update_ccmc_variable_attribute_value(
         "total_flux",
         "actual_min",
         &total_flux_actual_min,
         baselined_ccmc_variable_structure_list);
   stat = update_ccmc_variable_attribute_value(
         "total_flux",
         "actual_max",
         &total_flux_actual_max,
         baselined_ccmc_variable_structure_list);

   stat = update_ccmc_variable_attribute_value(
         "weights",
         "variable_size",
         &number_of_cells,
         baselined_ccmc_variable_structure_list);
   stat = update_ccmc_variable_value(
         "weights",
         number_of_cells,
         weights,
         baselined_ccmc_variable_structure_list);
   stat = update_ccmc_variable_attribute_value(
         "weights",
         "actual_min",
         &weights_actual_min,
         baselined_ccmc_variable_structure_list);
   stat = update_ccmc_variable_attribute_value(
         "weights",
         "actual_max",
         &weights_actual_max,
         baselined_ccmc_variable_structure_list);

   if (verbose_flag)
   {
      printf("grid description data successfully written to "
            "baselined_ccmc_variable_structure_list...\n");
   }
   if (verbose_flag)
   {
      printf("cleaning memory allocated for initial read routine...\n");
   }

   free(carrington_longitiude);
   free(carrington_sine_latitude);
   free(net_flux);
   free(total_flux);
   free(weights);

   return 0;

}
