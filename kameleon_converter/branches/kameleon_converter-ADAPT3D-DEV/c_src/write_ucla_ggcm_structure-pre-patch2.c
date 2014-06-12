/*******************************************************************************
 *                                                                             *
 *      NAME:           write_ucla_ggcm_structure.c                            *
 *                                                                             *
 *      VERSION:        3.2                                                    *
 *                                                                             *
 *      AUTHOR:         Marlo M. H. Maddox                                     *
 *                      NASA-GSFC-AETD-ISD-ADMA(587)                           *
 *                      Marlo.Maddox@nasa.gov                                  *
 *                      (301) 286 - 5202                                       *
 *                                                                             *
 *      PURPOSE:        specialized write routine that writes ucla_ggcm data   *
 *                      from main mem to cdf format.                           *
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
 *      03.29.2004      Maddox, Marlo                                          *
 *                      initial development started                            *
 *                                                                             *
 *      04.19.2004      Maddox, Marlo                                          *
 *                      updated attributes etc..                               *
 *                                                                             *
 *      09.03.2004      Maddox, Marlo                                          *
 *                      added code to write DatabaseInfo values in CDF file as *
 *                      global attributes                                      *
 *                                                                             *
 *      09.13.2004      Maddox, Marlo                                          *
 *                      writting standardized cdfEPOCH3 dates for              *
 *                      start/end_time and generation/request_date             *
 *                                                                             *
 *      03.03.2005      Maddox, Marlo                                          *
 *                      changed local elapsed_time_in_seconds value from double*
 *                      to float                                               *
 *                                                                             *
 *      03.04.2005      Maddox, Marlo                                          *
 *                      Removed char **global/variable_metadata variable       *
 *                      declarations.  variables are set in main_write_driver.c*
 *                      and passed by reference to respective write_*_cdf.c    *
 *                      functions                                              *
 *                                                                             *
 *      03.24.2005      Maddox, Marlo                                          *
 *                      writing values for new gattributes:dipole_tilt,        *
 *                      dipole_tilt_y, run_type, dipole_update                 *
 *                                                                             *
 *      08.24.2005      Maddox, Marlo                                          *
 *                      trying to debug nan error on LINUX,                    *
 *                      elapsed_time_in_seconds calculation is the culprit,    *
 *                      modifying types and adding variable to repalce / 1000.0*
 *                      as * .001                                              *
 *                                                                             *
 *                      Forgot to update variable_structure_list with actual   *
 *                      min/max values                                         *
 *                                                                             *
 *      08.26.2005      Maddox, Marlo                                          *
 *                      changed converted_file_name[] from size                *
 *                      file_name_length to MAX_NEW_FILE_NAME_LENGTH           *
 *                                                                             *
 *     01.06.2006       Maddox, Marlo                                          *
 *                      Changed getwd to getcwd and associated variables...    *
 *                                                                             *
 *      10.31.2006      Maddox, Marlo                                          *
 *                      two extra variables where somehow present in the       *
 *                      function declaration for write_ucla_ggcm_structure()   *
 *                      function before the declaration of the                 *
 *                      write_database_info_flag variable. This caused the flag*
 *                      to loose its original value and thus the database info *
 *                      variables where not being written to cdf               *
 *                                                                             *
 *      02.06.2007      Maddox, Marlo                                          *
 *                      changing ccmc_metadata_structure.h to                  *
 *                      ccmc_structure_manager.h                               *
 *                                                                             *
 *      05.17.2007      Maddox, Marlo                                          *
 *                      adding update for kameleon_version gattribute          *
 *      07.16.2008      Rastaetter, Lutz                                       *
 *                      adding eta variable                                    *
 *                                                                             *
 ******************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <math.h>

#include "cdf.h"
#include "read_database.h"

#include "read_ucla_ggcm.h"

#include "ccmc_attributes.h"
#include "ccmc_variable_attributes.h"
#include "ccmc_registered_variables.h"

#include "ucla_ggcm_attributes.h"

#include "ccmc_structure_manager.h"

#include "kameleon_version_info.h"

#define MAX_NEW_FILE_NAME_LENGTH    512
#define NUMBER_GLOBAL_ATTR NUMBER_CCMC_ATTR + NUMBER_UCLA_GGCM_ATTR
#define NUMBER_VARIABLE_ATTR NUMBER_CCMC_VARIABLE_ATTR + NUMBER_UCLA_GGCM_VARIABLE_ATTR

int write_ucla_ggcm_structure(
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

   /* define static values for ccmc_attributes for ucla_ggcm_model */

   char *kameleon_version_num= KAMELEON_VERSION;

   char *readme_text =
         "Basic model concept: For the numerical details we refer to the "
            "literature cited below. Briefly, the magnetospheric part solves "
            "the MHD equations on a stretched Cartesian grid using second "
            "order explicit time integration with conservative and "
            "flux-limited spatial finite differences. A Yee grid is used to "
            "preserve the magnetic field divergence to round-off error. The "
            "code is parallelized using the message passing interface (MPI) "
            "and runs on a large variety of computers that support MPI. "
            "Currently the code is used on IBM-SP2, IBM-p690, SGI-Origin2000,"
            " HP/alpha based machines, and on a variety of Intel or AMD "
            "based Beowulf clusters. The code scales well up to 100s of "
            "processors on machines with high speed interconnect (IBM, "
            "SGI, Myrinet clusters), and up to several 10s of CPUs on 100 "
            "Mbit Ethernet based clusters. The numerical grid is adaptable "
            "and can have from a few times 10^5 to more than 10^7 cells. "
            "Correspondingly, the resolution in the MHD grid near the "
            "sub-solar magnetopause can be as small as 500 km or as large as "
            "0.5 RE. The code is robust and efficient. Real-time operation "
            "can be achieved with a moderately fine grid (~10^6 cells) using "
            "about 30 CPUs. At the ionosphere side, the code can either be "
            "run with uniform conductance, an empirical conductance model, "
            "or with CTIM. Model use and outputs:  The output of the model "
            "consists of snapshots of the full 3d grid of the primary "
            "variables (density, pressure, velocity, magnetic field, "
            "current density), snapshots on pre-defined 2d cuts "
            "perpendicular to any of the axes, snapshots of the ionospheric "
            "quantities (FAC, potential, electron precipitation fluxes and "
            "energies, ground magnetic perturbations), and if run with CTIM, "
            "also snapshots of the CTIM variables (neutral density, "
            "composition, winds, temperature, ion and electron densities and"
            " temperatures). Since the output can be virtually unlimited a "
            "judicious choice must be made before each run of which "
            "parameters are needed for analysis and at which cadence. It is "
            "easily possible to create 100GB or more with one run. Principal "
            "limitations and caveats:  The model does not include the "
            "energetic particle drift and ring current physics. Thus, "
            "results within about 5 RE from Earth are generally meaningless. "
            "Likewise, there is no plasmasphere. This should not affect the "
            "dynamics of the magnetosphere much, however, wave propagation "
            "through the inner magnetosphere is affected. Also, the model "
            "uses a `Boris correction' to keep time steps manageable. As a "
            "consequence wave propagation in the inner magnetosphere is "
            "artificially slowed. The developers do not provide any guarantee"
            " for the correctness of model results. There are many more "
            "caveats of various natures than listed here. Users of the model "
            "are therefore highly encouraged to contact the developers and "
            "discuss results with them prior to any publication.";

   char *terms_of_usage_text =
         "For tracking purposes for our government sponsors, we ask that "
            "you notify the CCMC whenever you use CCMC results in a "
            "scientific publications or presentations: ccmc@ccmc.gsfc.nasa."
            "gov ";

   char *model_type = "Global Magnetosphere";
   int grid_system_count = 4;
   char *output_type = "Global Magnetospheric";
   char *grid_system_1 = "[ x, y, z ]";
   char *grid_system_2 = "[ x_bx, y_bx, z_bx ]";
   char *grid_system_3 = "[ x_by, y_by, z _by]";
   char *grid_system_4 = "[ x_bz, y_bz, z_bz ]";
   char *standard_grid_target = "GSE";

   /**** since 03/2005 we would normally try to set the grid type based on
    * DatabaseInfo values but the GSE_JR is not represented correctly in the
    * file ***/

   char *default_grid_type = "GSE_JR";
   char *grid_1_type = "GSE_JR";
   char *grid_2_type = "GSE_JR";
   char *grid_3_type = "GSE_JR";
   char *grid_4_type = "GSE_JR";

   /* set not_applicable string for use when dipole time makes no sense */

   char *not_applicable = "N/A";

   /* I'm assuming there all 3 dimensional for now...add smarts later... */

   int grid_system_1_number_of_dimensions = 3;
   int grid_system_2_number_of_dimensions = 3;
   int grid_system_3_number_of_dimensions = 3;
   int grid_system_4_number_of_dimensions = 3;

   /* declare temp variable to hold extracted time values from each filename */

   float elapsed_time_in_seconds;
   float extracted_file_name_time_value;
   float start_time_delta_float;
   double start_time_delta;
   double exp1, exp2, operand1 = .001;
   char extracted_file_name_time_string[MAX_NEW_FILE_NAME_LENGTH];

   /*changed in vain i believe... char converted_file_name[file_name_length]; */
   char converted_file_name[MAX_NEW_FILE_NAME_LENGTH];
   char new_path_filename[MAX_NEW_FILE_NAME_LENGTH];

   char output_directory_copy[MAX_NEW_FILE_NAME_LENGTH];

   char base_file_name[MAX_NEW_FILE_NAME_LENGTH], *string_token, *string_token2;
   char *slash = "/";
   char *dot = ".";
   char *current_working_directoryPtr;
   char temp_current_working_directory[MAX_NEW_FILE_NAME_LENGTH];

   int buf_size= MAX_NEW_FILE_NAME_LENGTH;

   /* NX, NY, NZ are defined in read_ucla_ggcm.h */

   int number_of_cells, x_dimension_size = NX, y_dimension_size = NY,
         z_dimension_size = NZ;

   /********* input_file_name has strange behavior ***********/

   char input_file_name_local_copy[MAX_NEW_FILE_NAME_LENGTH];

   /* set dynamic values for ccmc_attributes for ucla_ggcm */

   /*  call/perform  necessary functions to set values for dynamic global
    * attributes */

   int grid_system_1_dimension_1_size = x_dimension_size;
   int grid_system_1_dimension_2_size = y_dimension_size;
   int grid_system_1_dimension_3_size = z_dimension_size;

   int grid_system_2_dimension_1_size = x_dimension_size;
   int grid_system_2_dimension_2_size = y_dimension_size;
   int grid_system_2_dimension_3_size = z_dimension_size;

   int grid_system_3_dimension_1_size = x_dimension_size;
   int grid_system_3_dimension_2_size = y_dimension_size;
   int grid_system_3_dimension_3_size = z_dimension_size;

   int grid_system_4_dimension_1_size = x_dimension_size;
   int grid_system_4_dimension_2_size = y_dimension_size;
   int grid_system_4_dimension_3_size = z_dimension_size;

   /* declare temp variable to hold extracted time values from each filename */

   /*
    double elapsed_time_in_seconds;
    double extracted_file_name_time_value;
    char extracted_file_name_time_string;
    char extracted_file_name_time_string[MAX_NEW_FILE_NAME_LENGTH];
    */

   strcpy(input_file_name_local_copy, input_file_name);
   input_file_name_local_copy[ file_name_length] = '\0';

   strcpy(output_directory_copy, output_directory);
   strcpy(converted_file_name, input_file_name_local_copy);

   /****************************************************************************
    ** UCLA_GGCM/OpenGGCM output files contain NO time step information aside  *
    ** from a field in the actual filename.  We need to extract this, resolve  *
    ** actual start time vs setup time, and store for use in standardized      *
    *  structure                                                               *
    ***************************************************************************/

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

   /* base_file_name should now contain base file name without path */

   if (verbose_flag)
   {
      printf("resulting base_file_name = %s\n", base_file_name);
   }

   /* Now extract current elapsed time in seconds from base_file_name */

   strcpy(extracted_file_name_time_string, base_file_name);

   extracted_file_name_time_string[ strlen( base_file_name) ] = '\0';

   if (verbose_flag)
   {
      printf(
            "extracting elapsed time is seconds from = %s\n",
            extracted_file_name_time_string);
   }

   string_token2 = strtok(extracted_file_name_time_string, dot);

   while (string_token2 != NULL)
   {
      strcpy(extracted_file_name_time_string, string_token2);
      if (verbose_flag)
      {
         printf(
               "extracting elapsed time is seconds from = %s\n",
               extracted_file_name_time_string);
      }
      string_token2 = strtok( NULL, dot);
   }

   if (verbose_flag)
   {
      printf(
            "extracted elapsed time in seconds string = %s\n",
            extracted_file_name_time_string);
   }

   extracted_file_name_time_value = atof(extracted_file_name_time_string);

   if (verbose_flag)
   {
      printf(
            "extracted elapsed time in seconds float value = %f\n",
            extracted_file_name_time_value);
   }

   /*************** calculate real elapsed_time_in_seconds ***********/

   /*
    printf("DEBUG\nsimulation init time = %s\n", open_ggcm_init_time_epoch3 );
    printf("DEBUG\nsimulation start time = %s\n", start_time_cdf_epoch3 );
    printf("DEBUG\nsimulation init time = %f\n", parseEPOCH3( open_ggcm_init_time_epoch3 ) );
    printf("DEBUG\nsimulation start time = %f\n", parseEPOCH3( start_time_cdf_epoch3 ) );
    */

   exp1 = parseEPOCH3(start_time_cdf_epoch3);
   exp2 = parseEPOCH3(open_ggcm_init_time_epoch3);

   /*
    printf("MMMDEBUG\texp1 = %f\n", exp1 );
    printf("MMMDEBUG\texp2 = %f\n", exp2 );
    */

   /* the below operation results in nan sometimes, lets separate the operations */
   /*start_time_delta = ( exp1 - exp2 ) / 1000.0;*/
   start_time_delta = (exp1 - exp2 );

   /*printf("MMMDEBUG\tstart_time_delta = ( %f - %f ) = %f\n", exp1, exp2, start_time_delta );*/

   start_time_delta_float = start_time_delta * operand1;

   /*printf("MMMDEBUG\tstart_time_delta_float = ( %f * %f ) = %f\n",  (float) start_time_delta, operand1, start_time_delta_float );*/

   elapsed_time_in_seconds = (extracted_file_name_time_value
         - start_time_delta_float );

   /*printf("MMMDEBUG\telapsed_time_in_seconds = %f - %f = %f\n", extracted_file_name_time_value, start_time_delta_float, elapsed_time_in_seconds );*/

   /*printf("DEBUG\textracted_file_name_time_value = %f\nstart_time_delta = %f\n", extracted_file_name_time_value, start_time_delta );*/

   if (verbose_flag)
   {
      printf(
            "calculated elapsed time in seconds float value = %f\n",
            elapsed_time_in_seconds);
   }

   /****************************************************************************

    WEIRD BUG HAS BEEN ISOLATED TO THIS LINE.  CAUSES PROGRAM TO DIE AFTER ONE
    ITERATION WHEN TRYING TO RETURN CONTROL TO main_conversion_driver().
    THE PROBLEM MAY BE ELSEWHERE BUT THIS WILL BE THE SOLUTION FOR NOW....

    elapsed_time_in_seconds = extracted_file_name_time_value -
    ( ( parseEPOCH3( start_time_cdf_epoch3 ) -
    parseEPOCH3( open_ggcm_init_time_epoch3 ) ) ) / 1000.0;

    ****************************************************************************/

    /*printf("DEBUG\t%f = %f - ( %f - %f )\n", elapsed_time_in_seconds,
    * extracted_file_name_time_value, parseEPOCH3( start_time_cdf_epoch3 ),
    * parseEPOCH3( open_ggcm_init_time_epoch3) );*/

   /*printf("DEBUG\t elapsed_time_is_seconds = %f\n", elapsed_time_in_seconds );*/

   /***************************************************************************/

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

   number_of_cells = x_dimension_size * y_dimension_size * z_dimension_size;

   /****************************************************************************
    *                                CREATE THE GLOBAL ATTRIBUTES              *
    ***************************************************************************/

   /*********************************** GLOBAL ATTRIBUTES *********************/

   /****************************************************************************
    *   initialize the baselined_ccmc_attribute_structure_list to hold n       *
    *   number of cccmc_attribute_structures where n = NUMBER_GLOBAL_ATTR =    *
    *   NUMBER_CCMC_ATTR + NUMBER_UCLA_GGCM_ATTR as defined in                 *
    *   ccmc_attributes.h & ucla_ggcm_attributes.h                             *
    ***************************************************************************/

   baselined_ccmc_attribute_structure_list
         = init_ccmc_attribute_structure( NUMBER_GLOBAL_ATTR);

   /* for each name-data type pair in the ccmc_attribute_name_type_list, call
    * put_ccmc_attribute with attr name & data type ***/

   /* since we are writing from the ccmc_attribute_name_type_list, the
    * attribute_type = global ***/

   for (i=0; i < NUMBER_CCMC_ATTR; i++)
   {
      put_ccmc_attribute(
            ccmc_attribute_name_type_list[i].ccmc_attr_name,
            "global",
            ccmc_attribute_name_type_list[i].ccmc_attr_data_type,
            "",
            baselined_ccmc_attribute_structure_list);
   }

   /* for each name-data type pair in the ucla_ggcm_attribute_name_type_list,
    * call put_ccmc_attribute with attr name & data type ***/

   /* since we are writing from the ucla_ggcm_attribute_name_type_list, the
    * attribute_type = model ***/

   for (i=0; i < NUMBER_UCLA_GGCM_ATTR; i++)
   {
      put_ccmc_attribute(
            ucla_ggcm_attribute_name_type_list[i].ucla_ggcm_attr_name,
            "model",
            ucla_ggcm_attribute_name_type_list[i].ucla_ggcm_attr_data_type,
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
         "grid_system_2_number_of_dimensions",
         &grid_system_2_number_of_dimensions,
         baselined_ccmc_attribute_structure_list);
   stat = update_ccmc_attribute_value(
         "grid_system_2_dimension_1_size",
         &grid_system_2_dimension_1_size,
         baselined_ccmc_attribute_structure_list);
   stat = update_ccmc_attribute_value(
         "grid_system_2_dimension_2_size",
         &grid_system_2_dimension_2_size,
         baselined_ccmc_attribute_structure_list);
   stat = update_ccmc_attribute_value(
         "grid_system_2_dimension_3_size",
         &grid_system_2_dimension_3_size,
         baselined_ccmc_attribute_structure_list);
   stat = update_ccmc_attribute_value(
         "grid_system_2",
         grid_system_2,
         baselined_ccmc_attribute_structure_list);
   stat = update_ccmc_attribute_value(
         "grid_2_type",
         grid_2_type,
         baselined_ccmc_attribute_structure_list);
   stat = update_ccmc_attribute_value(
         "grid_system_3_number_of_dimensions",
         &grid_system_3_number_of_dimensions,
         baselined_ccmc_attribute_structure_list);
   stat = update_ccmc_attribute_value(
         "grid_system_3_dimension_1_size",
         &grid_system_3_dimension_1_size,
         baselined_ccmc_attribute_structure_list);
   stat = update_ccmc_attribute_value(
         "grid_system_3_dimension_2_size",
         &grid_system_3_dimension_2_size,
         baselined_ccmc_attribute_structure_list);
   stat = update_ccmc_attribute_value(
         "grid_system_3_dimension_3_size",
         &grid_system_3_dimension_3_size,
         baselined_ccmc_attribute_structure_list);
   stat = update_ccmc_attribute_value(
         "grid_system_3",
         grid_system_3,
         baselined_ccmc_attribute_structure_list);
   stat = update_ccmc_attribute_value(
         "grid_3_type",
         grid_3_type,
         baselined_ccmc_attribute_structure_list);
   stat = update_ccmc_attribute_value(
         "grid_system_4_number_of_dimensions",
         &grid_system_4_number_of_dimensions,
         baselined_ccmc_attribute_structure_list);
   stat = update_ccmc_attribute_value(
         "grid_system_4_dimension_1_size",
         &grid_system_4_dimension_1_size,
         baselined_ccmc_attribute_structure_list);
   stat = update_ccmc_attribute_value(
         "grid_system_4_dimension_2_size",
         &grid_system_4_dimension_2_size,
         baselined_ccmc_attribute_structure_list);
   stat = update_ccmc_attribute_value(
         "grid_system_4_dimension_3_size",
         &grid_system_4_dimension_3_size,
         baselined_ccmc_attribute_structure_list);
   stat = update_ccmc_attribute_value(
         "grid_system_4",
         grid_system_4,
         baselined_ccmc_attribute_structure_list);
   stat = update_ccmc_attribute_value(
         "grid_4_type",
         grid_4_type,
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

   /****** individual ucla_ggcm global attributes set by name using read in
    * *grid &| *3df or values already in main memory *****/

   /****************** add attributes read in from DatabaseInfo file using
    * read_database.c code *****/

   /**** ONLY ADD if write_database_info_flag = 1 ***/

   if (write_database_info_flag)
   {

      if (verbose_flag)
         printf("updating specific DatabaseInfo values...\n");

      /*** check to see if each variable has been set ie. strlen( variable ) > 0
       *  , otherwise CDF exits with error ********/

      if (strlen(run_registration_number) )
      {
         stat = update_ccmc_attribute_value(
               "run_registration_number",
               run_registration_number,
               baselined_ccmc_attribute_structure_list);
      }
      if (strlen(request_date) )
      {
         stat = update_ccmc_attribute_value(
               "generation_date",
               request_date_cdf_epoch3,
               baselined_ccmc_attribute_structure_list);
      }
      if (strlen(generated_by) )
      {
         stat = update_ccmc_attribute_value(
               "generated_by",
               generated_by,
               baselined_ccmc_attribute_structure_list);
      }
      if (strlen(start_time) )
      {
         stat = update_ccmc_attribute_value(
               "start_time",
               start_time_cdf_epoch3,
               baselined_ccmc_attribute_structure_list);
      }
      if (strlen(end_time) )
      {
         stat = update_ccmc_attribute_value(
               "end_time",
               end_time_cdf_epoch3,
               baselined_ccmc_attribute_structure_list);
      }
      if (strlen(r_body) )
      {
         stat = update_ccmc_attribute_value(
               "r_body",
               r_body,
               baselined_ccmc_attribute_structure_list);
      }
      if (strlen(r_currents) )
      {
         stat = update_ccmc_attribute_value(
               "r_currents",
               r_currents,
               baselined_ccmc_attribute_structure_list);
      }
      if (strlen(open_ggcm_init_time) )
      {
         stat = update_ccmc_attribute_value(
               "elapsed_time_in_seconds",
               &elapsed_time_in_seconds,
               baselined_ccmc_attribute_structure_list);
      }
      if (strlen(run_type) )
      {
         stat = update_ccmc_attribute_value(
               "run_type",
               run_type,
               baselined_ccmc_attribute_structure_list);
      }
      if (strlen(dipole_update) )
      {
         stat = update_ccmc_attribute_value(
               "dipole_update",
               dipole_update,
               baselined_ccmc_attribute_structure_list);
      }

      /****** add dipole time only if dipole update flag from DatabaseInfo
       * file == yes ********/

      if ( !strcmp(dipole_update, "yes") || !strcmp(dipole_update, "Yes")
            || !strcmp(dipole_update, "YES") )
      {

         /*** since dipole update flag == yes, dipole time = start_time +
          * elapsed_time_in_seconds ****/

         cdf_epoch_start_time_value = parseEPOCH3(start_time_cdf_epoch3);

         cdf_epoch_dipole_time_value = cdf_epoch_start_time_value
               + (elapsed_time_in_seconds * 1000.0 );

         encodeEPOCH3(cdf_epoch_dipole_time_value, dipole_time_cdf_epoch3);

         if (strlen(dipole_time) )
         {
            stat = update_ccmc_attribute_value(
                  "dipole_time",
                  dipole_time_cdf_epoch3,
                  baselined_ccmc_attribute_structure_list);
         }

      }
      else /* dipole time is not applicable as no updates are available */
      {
         stat = update_ccmc_attribute_value(
               "dipole_time",
               not_applicable,
               baselined_ccmc_attribute_structure_list);
      }

      /******** add dipole_tilt and dipole_tilt_y values bases on cs_output flag
       *  from DatabaseInfo flag *****/

      if (strlen(cs_output) )
      {
         /*** for gsm coordinate system ***/

         if ( !strcmp(cs_output, "GSM") || !strcmp(cs_output, "Gsm")
               || !strcmp(cs_output, "gsm") )
         {
            float temp_dipole_tilt = atof(dipole_tilt);
            if (strlen(dipole_tilt) )
            {
               stat = update_ccmc_attribute_value(
                     "dipole_tilt",
                     &temp_dipole_tilt,
                     baselined_ccmc_attribute_structure_list);
            }
            stat = update_ccmc_attribute_value(
                  "dipole_tilt_y",
                  &zero,
                  baselined_ccmc_attribute_structure_list);
         }
         else if ( !strcmp(cs_output, "SM") || !strcmp(cs_output, "Sm")
               || !strcmp(cs_output, "sm") )
         {
            stat = update_ccmc_attribute_value(
                  "dipole_tilt",
                  &zero,
                  baselined_ccmc_attribute_structure_list);
            stat = update_ccmc_attribute_value(
                  "dipole_tilt_y",
                  &zero,
                  baselined_ccmc_attribute_structure_list);
         }
         else if ( !strcmp(cs_output, "GSE") || !strcmp(cs_output, "Gse")
               || !strcmp(cs_output, "gse") )
         {
            float temp_dipole_tilt = atof(dipole_tilt);
            float temp_dipole_tilt_y = atof(dipole_tilt_y);
            if (strlen(dipole_tilt) )
            {
               stat = update_ccmc_attribute_value(
                     "dipole_tilt",
                     &temp_dipole_tilt,
                     baselined_ccmc_attribute_structure_list);
            }
            if (strlen(dipole_tilt_y) )
            {
               stat = update_ccmc_attribute_value(
                     "dipole_tilt_y",
                     &temp_dipole_tilt_y,
                     baselined_ccmc_attribute_structure_list);
            }
         }
         else
         {
            float temp_dipole_tilt = atof(dipole_tilt);
            float temp_dipole_tilt_y = atof(dipole_tilt_y);
            printf(
                  "WARNING: unrecognized coordinate system: -->%s<-- .  Using "
                  "default dipole_tilt/tilt_y angles for %s\n",
                  cs_output,
                  default_grid_type);
            if (strlen(dipole_tilt) )
            {
               stat = update_ccmc_attribute_value(
                     "dipole_tilt",
                     &temp_dipole_tilt,
                     baselined_ccmc_attribute_structure_list);
            }
            if (strlen(dipole_tilt_y) )
            {
               stat = update_ccmc_attribute_value(
                     "dipole_tilt_y",
                     &temp_dipole_tilt_y,
                     baselined_ccmc_attribute_structure_list);
            }
         }

      }
      else
      {
         float temp_dipole_tilt = atof(dipole_tilt);
         float temp_dipole_tilt_y = atof(dipole_tilt_y);
         printf(
               "WARNING: coordinate system was not be identified through "
               "DatabaseInfo file.  Using default dipole_tilt/tilt_y angles for "
               "%s\n",
               default_grid_type);
         if (strlen(dipole_tilt) )
         {
            stat = update_ccmc_attribute_value(
                  "dipole_tilt",
                  &temp_dipole_tilt,
                  baselined_ccmc_attribute_structure_list);
         }
         if (strlen(dipole_tilt_y) )
         {
            stat = update_ccmc_attribute_value(
                  "dipole_tilt_y",
                  &temp_dipole_tilt_y,
                  baselined_ccmc_attribute_structure_list);
         }
      }

   }

   if (verbose_flag)
   {
      printf("global attributes values successfully added to "
            "baselined_ccmc_attribute_structure_list...\n");
   }

   /****************************************************************************
    *                        CREATE THE VARIABLES & VARIABLE ATTRIBUTES        *
    ***************************************************************************/

   /****************************************************************************
    *    initialize the baselined_ccmc_variable_structure_list to hold n number*
    *    of cccmc_variable_structures where n =                                *
    *    NUMBER_REGISTERED_UCLA_GGCM_VARIABLES defined in                      *
    *    ccmc_registered_variables.h                                           *
    ***************************************************************************/
   baselined_ccmc_variable_structure_list
         = init_ccmc_variable_structure( NUMBER_REGISTERED_UCLA_GGCM_VARIABLES);

   /**** Put default variable names and variable attribute values into the
    * ccmc_baselined_variable_structure_list ***/

   for (i = 0; i< NUMBER_REGISTERED_UCLA_GGCM_VARIABLES; i++)
   {
      put_ccmc_variable(
            registered_ucla_ggcm_var_list[i].ccmc_var_name,
            registered_ucla_ggcm_var_list[i].ccmc_var_data_type,
            0,
            registered_ucla_ggcm_var_list[i].ccmc_var_classification,
            "",
            registered_ucla_ggcm_var_list[i].valid_min,
            registered_ucla_ggcm_var_list[i].valid_max,
            registered_ucla_ggcm_var_list[i].units,
            registered_ucla_ggcm_var_list[i].grid_system,
            registered_ucla_ggcm_var_list[i].mask,
            registered_ucla_ggcm_var_list[i].description,
            registered_ucla_ggcm_var_list[i].is_vector_component,
            registered_ucla_ggcm_var_list[i].position_grid_system,
            registered_ucla_ggcm_var_list[i].data_grid_system,
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
    *   baselined_ccmc_variable_structure_list 08/25/05 - lets add the actual  *
    *   min/max values at the same time                                        *
    ***************************************************************************/

   /*** write the original model variable values ***/

   if (verbose_flag)
   {
      printf("\nwritting original model output variables values into "
            "baselined_ccmc_variable_structure_list...\n");
   }

   /*** update each variable size as the defualts are set to zero, this reallocs
    *  space for the actual values to be stored *****/

   /***************************** write position variables with three components
    *  (x*,y*,z*)  *********/

   stat = update_ccmc_variable_attribute_value(
         "x",
         "variable_size",
         &x_dimension_size,
         baselined_ccmc_variable_structure_list);
   stat = update_ccmc_variable_value(
         "x",
         x_dimension_size,
         gridx,
         baselined_ccmc_variable_structure_list);
   stat = update_ccmc_variable_attribute_value(
         "x",
         "actual_min",
         &gridx_actual_min,
         baselined_ccmc_variable_structure_list);
   stat = update_ccmc_variable_attribute_value(
         "x",
         "actual_max",
         &gridx_actual_max,
         baselined_ccmc_variable_structure_list);

   stat = update_ccmc_variable_attribute_value(
         "y",
         "variable_size",
         &y_dimension_size,
         baselined_ccmc_variable_structure_list);
   stat = update_ccmc_variable_value(
         "y",
         y_dimension_size,
         gridy,
         baselined_ccmc_variable_structure_list);
   stat = update_ccmc_variable_attribute_value(
         "y",
         "actual_min",
         &gridy_actual_min,
         baselined_ccmc_variable_structure_list);
   stat = update_ccmc_variable_attribute_value(
         "y",
         "actual_max",
         &gridy_actual_max,
         baselined_ccmc_variable_structure_list);

   stat = update_ccmc_variable_attribute_value(
         "z",
         "variable_size",
         &z_dimension_size,
         baselined_ccmc_variable_structure_list);
   stat = update_ccmc_variable_value(
         "z",
         z_dimension_size,
         gridz,
         baselined_ccmc_variable_structure_list);
   stat = update_ccmc_variable_attribute_value(
         "z",
         "actual_min",
         &gridz_actual_min,
         baselined_ccmc_variable_structure_list);
   stat = update_ccmc_variable_attribute_value(
         "z",
         "actual_max",
         &gridz_actual_max,
         baselined_ccmc_variable_structure_list);

   stat = update_ccmc_variable_attribute_value(
         "x_bx",
         "variable_size",
         &x_dimension_size,
         baselined_ccmc_variable_structure_list);
   stat = update_ccmc_variable_value(
         "x_bx",
         x_dimension_size,
         gridx_bx,
         baselined_ccmc_variable_structure_list);
   stat = update_ccmc_variable_attribute_value(
         "x_bx",
         "actual_min",
         &gridx_bx_actual_min,
         baselined_ccmc_variable_structure_list);
   stat = update_ccmc_variable_attribute_value(
         "x_bx",
         "actual_max",
         &gridx_bx_actual_max,
         baselined_ccmc_variable_structure_list);

   stat = update_ccmc_variable_attribute_value(
         "y_bx",
         "variable_size",
         &y_dimension_size,
         baselined_ccmc_variable_structure_list);
   stat = update_ccmc_variable_value(
         "y_bx",
         y_dimension_size,
         gridy_bx,
         baselined_ccmc_variable_structure_list);
   stat = update_ccmc_variable_attribute_value(
         "y_bx",
         "actual_min",
         &gridy_bx_actual_min,
         baselined_ccmc_variable_structure_list);
   stat = update_ccmc_variable_attribute_value(
         "y_bx",
         "actual_max",
         &gridy_bx_actual_max,
         baselined_ccmc_variable_structure_list);

   stat = update_ccmc_variable_attribute_value(
         "z_bx",
         "variable_size",
         &z_dimension_size,
         baselined_ccmc_variable_structure_list);
   stat = update_ccmc_variable_value(
         "z_bx",
         z_dimension_size,
         gridz_bx,
         baselined_ccmc_variable_structure_list);
   stat = update_ccmc_variable_attribute_value(
         "z_bx",
         "actual_min",
         &gridz_bx_actual_min,
         baselined_ccmc_variable_structure_list);
   stat = update_ccmc_variable_attribute_value(
         "z_bx",
         "actual_max",
         &gridz_bx_actual_max,
         baselined_ccmc_variable_structure_list);

   stat = update_ccmc_variable_attribute_value(
         "x_by",
         "variable_size",
         &x_dimension_size,
         baselined_ccmc_variable_structure_list);
   stat = update_ccmc_variable_value(
         "x_by",
         x_dimension_size,
         gridx_by,
         baselined_ccmc_variable_structure_list);
   stat = update_ccmc_variable_attribute_value(
         "x_by",
         "actual_min",
         &gridx_by_actual_min,
         baselined_ccmc_variable_structure_list);
   stat = update_ccmc_variable_attribute_value(
         "x_by",
         "actual_max",
         &gridx_by_actual_max,
         baselined_ccmc_variable_structure_list);

   stat = update_ccmc_variable_attribute_value(
         "y_by",
         "variable_size",
         &y_dimension_size,
         baselined_ccmc_variable_structure_list);
   stat = update_ccmc_variable_value(
         "y_by",
         y_dimension_size,
         gridy_by,
         baselined_ccmc_variable_structure_list);
   stat = update_ccmc_variable_attribute_value(
         "y_by",
         "actual_min",
         &gridy_by_actual_min,
         baselined_ccmc_variable_structure_list);
   stat = update_ccmc_variable_attribute_value(
         "y_by",
         "actual_max",
         &gridy_by_actual_max,
         baselined_ccmc_variable_structure_list);

   stat = update_ccmc_variable_attribute_value(
         "z_by",
         "variable_size",
         &z_dimension_size,
         baselined_ccmc_variable_structure_list);
   stat = update_ccmc_variable_value(
         "z_by",
         z_dimension_size,
         gridz_by,
         baselined_ccmc_variable_structure_list);
   stat = update_ccmc_variable_attribute_value(
         "z_by",
         "actual_min",
         &gridz_by_actual_min,
         baselined_ccmc_variable_structure_list);
   stat = update_ccmc_variable_attribute_value(
         "z_by",
         "actual_max",
         &gridz_by_actual_max,
         baselined_ccmc_variable_structure_list);

   stat = update_ccmc_variable_attribute_value(
         "x_bz",
         "variable_size",
         &x_dimension_size,
         baselined_ccmc_variable_structure_list);
   stat = update_ccmc_variable_value(
         "x_bz",
         x_dimension_size,
         gridx_bz,
         baselined_ccmc_variable_structure_list);
   stat = update_ccmc_variable_attribute_value(
         "x_bz",
         "actual_min",
         &gridx_bz_actual_min,
         baselined_ccmc_variable_structure_list);
   stat = update_ccmc_variable_attribute_value(
         "x_bz",
         "actual_max",
         &gridx_bz_actual_max,
         baselined_ccmc_variable_structure_list);

   stat = update_ccmc_variable_attribute_value(
         "y_bz",
         "variable_size",
         &y_dimension_size,
         baselined_ccmc_variable_structure_list);
   stat = update_ccmc_variable_value(
         "y_bz",
         y_dimension_size,
         gridy_bz,
         baselined_ccmc_variable_structure_list);
   stat = update_ccmc_variable_attribute_value(
         "y_bz",
         "actual_min",
         &gridy_bz_actual_min,
         baselined_ccmc_variable_structure_list);
   stat = update_ccmc_variable_attribute_value(
         "y_bz",
         "actual_max",
         &gridy_bz_actual_max,
         baselined_ccmc_variable_structure_list);

   stat = update_ccmc_variable_attribute_value(
         "z_bz",
         "variable_size",
         &z_dimension_size,
         baselined_ccmc_variable_structure_list);
   stat = update_ccmc_variable_value(
         "z_bz",
         z_dimension_size,
         gridz_bz,
         baselined_ccmc_variable_structure_list);
   stat = update_ccmc_variable_attribute_value(
         "z_bz",
         "actual_min",
         &gridz_bz_actual_min,
         baselined_ccmc_variable_structure_list);
   stat = update_ccmc_variable_attribute_value(
         "z_bz",
         "actual_max",
         &gridz_bz_actual_max,
         baselined_ccmc_variable_structure_list);

   stat = update_ccmc_variable_attribute_value(
         "bx",
         "variable_size",
         &number_of_cells,
         baselined_ccmc_variable_structure_list);
   stat = update_ccmc_variable_value(
         "bx",
         number_of_cells,
         magnetic_field_bx,
         baselined_ccmc_variable_structure_list);
   stat = update_ccmc_variable_attribute_value(
         "bx",
         "actual_min",
         &magnetic_field_bx_actual_min,
         baselined_ccmc_variable_structure_list);
   stat = update_ccmc_variable_attribute_value(
         "bx",
         "actual_max",
         &magnetic_field_bx_actual_max,
         baselined_ccmc_variable_structure_list);

   stat = update_ccmc_variable_attribute_value(
         "by",
         "variable_size",
         &number_of_cells,
         baselined_ccmc_variable_structure_list);
   stat = update_ccmc_variable_value(
         "by",
         number_of_cells,
         magnetic_field_by,
         baselined_ccmc_variable_structure_list);
   stat = update_ccmc_variable_attribute_value(
         "by",
         "actual_min",
         &magnetic_field_by_actual_min,
         baselined_ccmc_variable_structure_list);
   stat = update_ccmc_variable_attribute_value(
         "by",
         "actual_max",
         &magnetic_field_by_actual_max,
         baselined_ccmc_variable_structure_list);

   stat = update_ccmc_variable_attribute_value(
         "bz",
         "variable_size",
         &number_of_cells,
         baselined_ccmc_variable_structure_list);
   stat = update_ccmc_variable_value(
         "bz",
         number_of_cells,
         magnetic_field_bz,
         baselined_ccmc_variable_structure_list);
   stat = update_ccmc_variable_attribute_value(
         "bz",
         "actual_min",
         &magnetic_field_bz_actual_min,
         baselined_ccmc_variable_structure_list);
   stat = update_ccmc_variable_attribute_value(
         "bz",
         "actual_max",
         &magnetic_field_bz_actual_max,
         baselined_ccmc_variable_structure_list);

   stat = update_ccmc_variable_attribute_value(
         "bx1",
         "variable_size",
         &number_of_cells,
         baselined_ccmc_variable_structure_list);
   stat = update_ccmc_variable_value(
         "bx1",
         number_of_cells,
         magnetic_field_bx1,
         baselined_ccmc_variable_structure_list);
   stat = update_ccmc_variable_attribute_value(
         "bx1",
         "actual_min",
         &magnetic_field_bx1_actual_min,
         baselined_ccmc_variable_structure_list);
   stat = update_ccmc_variable_attribute_value(
         "bx1",
         "actual_max",
         &magnetic_field_bx1_actual_max,
         baselined_ccmc_variable_structure_list);

   stat = update_ccmc_variable_attribute_value(
         "by1",
         "variable_size",
         &number_of_cells,
         baselined_ccmc_variable_structure_list);
   stat = update_ccmc_variable_value(
         "by1",
         number_of_cells,
         magnetic_field_by1,
         baselined_ccmc_variable_structure_list);
   stat = update_ccmc_variable_attribute_value(
         "by1",
         "actual_min",
         &magnetic_field_by1_actual_min,
         baselined_ccmc_variable_structure_list);
   stat = update_ccmc_variable_attribute_value(
         "by1",
         "actual_max",
         &magnetic_field_by1_actual_max,
         baselined_ccmc_variable_structure_list);

   stat = update_ccmc_variable_attribute_value(
         "bz1",
         "variable_size",
         &number_of_cells,
         baselined_ccmc_variable_structure_list);
   stat = update_ccmc_variable_value(
         "bz1",
         number_of_cells,
         magnetic_field_bz1,
         baselined_ccmc_variable_structure_list);
   stat = update_ccmc_variable_attribute_value(
         "bz1",
         "actual_min",
         &magnetic_field_bz1_actual_min,
         baselined_ccmc_variable_structure_list);
   stat = update_ccmc_variable_attribute_value(
         "bz1",
         "actual_max",
         &magnetic_field_bz1_actual_max,
         baselined_ccmc_variable_structure_list);

   stat = update_ccmc_variable_attribute_value(
         "ux",
         "variable_size",
         &number_of_cells,
         baselined_ccmc_variable_structure_list);
   stat = update_ccmc_variable_value(
         "ux",
         number_of_cells,
         velocity_vx,
         baselined_ccmc_variable_structure_list);
   stat = update_ccmc_variable_attribute_value(
         "ux",
         "actual_min",
         &velocity_vx_actual_min,
         baselined_ccmc_variable_structure_list);
   stat = update_ccmc_variable_attribute_value(
         "ux",
         "actual_max",
         &velocity_vx_actual_max,
         baselined_ccmc_variable_structure_list);

   stat = update_ccmc_variable_attribute_value(
         "uy",
         "variable_size",
         &number_of_cells,
         baselined_ccmc_variable_structure_list);
   stat = update_ccmc_variable_value(
         "uy",
         number_of_cells,
         velocity_vy,
         baselined_ccmc_variable_structure_list);
   stat = update_ccmc_variable_attribute_value(
         "uy",
         "actual_min",
         &velocity_vy_actual_min,
         baselined_ccmc_variable_structure_list);
   stat = update_ccmc_variable_attribute_value(
         "uy",
         "actual_max",
         &velocity_vy_actual_max,
         baselined_ccmc_variable_structure_list);

   stat = update_ccmc_variable_attribute_value(
         "uz",
         "variable_size",
         &number_of_cells,
         baselined_ccmc_variable_structure_list);
   stat = update_ccmc_variable_value(
         "uz",
         number_of_cells,
         velocity_vz,
         baselined_ccmc_variable_structure_list);
   stat = update_ccmc_variable_attribute_value(
         "uz",
         "actual_min",
         &velocity_vz_actual_min,
         baselined_ccmc_variable_structure_list);
   stat = update_ccmc_variable_attribute_value(
         "uz",
         "actual_max",
         &velocity_vz_actual_max,
         baselined_ccmc_variable_structure_list);

   stat = update_ccmc_variable_attribute_value(
         "jx",
         "variable_size",
         &number_of_cells,
         baselined_ccmc_variable_structure_list);
   stat = update_ccmc_variable_value(
         "jx",
         number_of_cells,
         current_density_xjx,
         baselined_ccmc_variable_structure_list);
   stat = update_ccmc_variable_attribute_value(
         "jx",
         "actual_min",
         &current_density_xjx_actual_min,
         baselined_ccmc_variable_structure_list);
   stat = update_ccmc_variable_attribute_value(
         "jx",
         "actual_max",
         &current_density_xjx_actual_max,
         baselined_ccmc_variable_structure_list);

   stat = update_ccmc_variable_attribute_value(
         "jy",
         "variable_size",
         &number_of_cells,
         baselined_ccmc_variable_structure_list);
   stat = update_ccmc_variable_value(
         "jy",
         number_of_cells,
         current_density_xjy,
         baselined_ccmc_variable_structure_list);
   stat = update_ccmc_variable_attribute_value(
         "jy",
         "actual_min",
         &current_density_xjy_actual_min,
         baselined_ccmc_variable_structure_list);
   stat = update_ccmc_variable_attribute_value(
         "jy",
         "actual_max",
         &current_density_xjy_actual_max,
         baselined_ccmc_variable_structure_list);

   stat = update_ccmc_variable_attribute_value(
         "jz",
         "variable_size",
         &number_of_cells,
         baselined_ccmc_variable_structure_list);
   stat = update_ccmc_variable_value(
         "jz",
         number_of_cells,
         current_density_xjz,
         baselined_ccmc_variable_structure_list);
   stat = update_ccmc_variable_attribute_value(
         "jz",
         "actual_min",
         &current_density_xjz_actual_min,
         baselined_ccmc_variable_structure_list);
   stat = update_ccmc_variable_attribute_value(
         "jz",
         "actual_max",
         &current_density_xjz_actual_max,
         baselined_ccmc_variable_structure_list);

   stat = update_ccmc_variable_attribute_value(
         "rho",
         "variable_size",
         &number_of_cells,
         baselined_ccmc_variable_structure_list);
   stat = update_ccmc_variable_value(
         "rho",
         number_of_cells,
         density_rr,
         baselined_ccmc_variable_structure_list);
   stat = update_ccmc_variable_attribute_value(
         "rho",
         "actual_min",
         &density_rr_actual_min,
         baselined_ccmc_variable_structure_list);
   stat = update_ccmc_variable_attribute_value(
         "rho",
         "actual_max",
         &density_rr_actual_max,
         baselined_ccmc_variable_structure_list);

   stat = update_ccmc_variable_attribute_value(
         "p",
         "variable_size",
         &number_of_cells,
         baselined_ccmc_variable_structure_list);
   stat = update_ccmc_variable_value(
         "p",
         number_of_cells,
         pressure_pp,
         baselined_ccmc_variable_structure_list);
   stat = update_ccmc_variable_attribute_value(
         "p",
         "actual_min",
         &pressure_pp_actual_min,
         baselined_ccmc_variable_structure_list);
   stat = update_ccmc_variable_attribute_value(
         "p",
         "actual_max",
         &pressure_pp_actual_max,
         baselined_ccmc_variable_structure_list);

   stat = update_ccmc_variable_attribute_value(
         "eta",
         "variable_size",
         &number_of_cells,
         baselined_ccmc_variable_structure_list);
   stat = update_ccmc_variable_value(
         "eta",
         number_of_cells,
         resistivity_resis,
         baselined_ccmc_variable_structure_list);
   stat = update_ccmc_variable_attribute_value(
         "eta",
         "actual_min",
         &resistivity_resis_actual_min,
         baselined_ccmc_variable_structure_list);
   stat = update_ccmc_variable_attribute_value(
         "eta",
         "actual_max",
         &resistivity_resis_actual_max,
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

   free(density_rr);
   free(pressure_pp);
   free(velocity_vx);
   free(velocity_vy);
   free(velocity_vz);
   free(magnetic_field_bx);
   free(magnetic_field_by);
   free(magnetic_field_bz);
   free(magnetic_field_bx1);
   free(magnetic_field_by1);
   free(magnetic_field_bz1);
   free(current_density_xjx);
   free(current_density_xjy);
   free(current_density_xjz);

   return 0;

}

