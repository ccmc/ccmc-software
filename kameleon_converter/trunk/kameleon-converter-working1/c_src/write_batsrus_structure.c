/*******************************************************************************
 *                                                                             *
 *    NAME:          write_bartrus_structure.c                                 *
 *                                                                             *
 *    VERSION:       1.0.0                                                     *
 *                                                                             *
 *    AUTHOR:        Marlo M. H. Maddox                                        *
 *                   NASA-GSFC-AETD-ISD-ADMA(587)                              *
 *                   Marlo.Maddox@nasa.gov                                     *
 *                   (301) 286 - 5202                                          *
 *                                                                             *
 *    PURPOSE:       specialized write routine that writes enlil data from main*
 *                   memory into the baselined_ccmc_attribute_structure_list & *
 *                   baselinded_ccmc_variable_structure_list structures using  *
 *                   the ccmc_structure manager interface and static values    *
 *                   defined in .h files                                       *
 *                                                                             *
 *    INPUTS:        *                                                         *
 *                                                                             *
 *    OUTPUTS:       *                                                         *
 *                                                                             *
 *    FUNCTIONS:     *                                                         *
 *                                                                             *
 *    VARIABLES:     *                                                         *
 *                                                                             *
 *                                                                             *
 *    MODIFICATION                                                             *
 *    HISTORY:                                                                 *
 *                                                                             *
 *    09.11.2003     Maddox, Marlo                                             *
 *                   initial development started                               *
 *                                                                             *
 *    12.10.2003     Maddox, Marlo                                             *
 *                   changing everything to cdf z variables to account for new *
 *                   block/grid info with diff dimmension size                 *
 *                                                                             *
 *    03.26.2004     Maddox, Marlo                                             *
 *                   Decomposing main_write_driver to create meta_data package *
 *                   for current model then call external specialized          *
 *                   model-format write routine ie. write_batsrus_cdf or       *
 *                   write_ucla_cdf to do the actual write                     *
 *                                                                             *
 *    04.19.2004     Maddox, Marlo                                             *
 *                   updated attributes etc..                                  *
 *                                                                             *
 *    08.12.2004     Maddox, Marlo                                             *
 *                   added <math.h> for MAC OS X - needed for setting          *
 *                   dimSizes[0] = ( 8 * number_of_blocks ) / 7                *
 *                                                                             *
 *    09.??.2004     Maddox, Marlo                                             *
 *                   added code to write DatabaseInfo values in CDF file as    *
 *                   global attributes                                         *
 *                                                                             *
 *    09.13.2004     Maddox, Marlo                                             *
 *                   added code to write actual min & max variable values to   *
 *                   cdf after they have been defined and extracted in         *
 *                   read_*.h/.c                                               *
 *                                                                             *
 *    09.13.2004     Maddox, Marlo                                             *
 *                   writting standardized cdfEPOCH3 dates for start/end_time  *
 *                   and generation/request_date                               *
 *                                                                             *
 *    03.04.2005     Maddox, Marlo                                             *
 *                   Removed char **global/variable_metadata variable          *
 *                   declarations.  variables are set in main_write_driver.c   *
 *                   and passed by reference to respective write_*_cdf.c       *
 *                   functions                                                 *
 *                                                                             *
 *    03.24.2005     Maddox, Marlo                                             *
 *                   writing values for new gattributes:dipole_tilt,           *
 *                   dipole_tilt_y, run_type, dipole_update                    *
 *                                                                             *
 *    04.05.2005     Maddox, Marlo                                             *
 *                   write to baselined_ccmc_attribute_structure_list instead  *
 *                   of cdf                                                    *
 *                                                                             *
 *    08.24.2005     Maddox, Marlo                                             *
 *                   Fixing a few bugs: (1) rho, p, e were all being written   *
 *                   using &rho (2) MAX_NEW_FILE_NAME_LENGTH 512 vs 300        *
 *                                                                             *
 *                   Forgot to update variable_structure_list with actual      *
 *                   min/max values                                            *
 *                                                                             *
 *    02.06.2007     Maddox, Marlo                                             *
 *                   changing ccmc_metadata_structure.h to                     *
 *                   ccmc_structure_manager.h                                  *
 *                                                                             *
 *    02.16.2007     Maddox, Marlo                                             *
 *                   removed MAX_NEW_FILE_NAME_LENGTH macro, not used here as  *
 *                   naming is done in the write_structures_cdf() function     *
 *                                                                             *
 *    05.17.2007     Maddox, Marlo                                             *
 *                   adding update for kameleon_version gattribute             *
 *                                                                             *
 *                 							                                   *
 *    08.30.2011   	      Maddox, Marlo				                           *
 *    		   	      Adding new "status" variable to batsrus                  *
 ******************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <math.h>

#include "cdf.h"

#include "read_database.h"

#include "read_batsrus.h"

#include "ccmc_attributes.h"
#include "ccmc_variable_attributes.h"
#include "ccmc_registered_variables.h"

#include "batsrus_attributes.h"

#include "ccmc_structure_manager.h"

#include "kameleon_version_info.h"

#define NUMBER_GLOBAL_ATTR    NUMBER_CCMC_ATTR + NUMBER_BATSRUS_ATTR
#define NUMBER_VARIABLE_ATTR  NUMBER_CCMC_VARIABLE_ATTR + NUMBER_BATSRUS_VARIABLE_ATTR

int write_batsrus_structure(
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

   double cdf_epoch_start_time_value, cdf_epoch_dipole_time_value;

   /****************************************************************************
    *       Define Static ccmc attribute values for Batsrus Model              *
    ***************************************************************************/

   char *kameleon_version_num= KAMELEON_VERSION;

   char *readme_text =
         "Model Description:  BATS-R-US, the Block-Adaptive-Tree-Solarwind"
            "-Roe-Upwind-Scheme, was developed by the Computational "
            "Magnetohydrodynamics (MHD) Group at the University of Michigan,"
            "now Center for Space Environment Modeling (CSEM). It was "
            "designed using the Message Passing Interface (MPI) and the "
            "Fortran90 standard and executes on a massively parallel computer"
            " system.  The BATS-R-US code solves 3D MHD equations in finite "
            "volume form using numerical methods related to Roe's Approximate"
            " Riemann Solver. BATSRUS uses an adaptive grid composed of "
            "rectangular blocks arranged in varying degrees of spatial "
            "refinement levels. The magnetospheric MHD part is attached to "
            "an ionospheric potential solver that provides electric "
            "potentials and conductances in the ionosphere from "
            "magnetospheric field-aligned currents. Model Authors/Developers:"
            " Dr. Tamas Gombosi, University of Michigan,Center for Space "
            "Environment Modeling (CSEM) Model Input Parameters: Inputs to "
            "BATS-R_US are the solar wind plasma (density, velocity, V_x, "
            "V_y, V_z, temperature) and magnetic field (B_x, B_y, B_z) "
            "measurement, transformed into GSM coordinates and propagated "
            "from the solar wind monitoring satellite's position propagated "
            "to the sunward boundary of the simulation domain. The Earth's "
            "magnetic field is approximated by a dipole with updated axis "
            "orientation and co-rotating inner magnetospheric plasma or with "
            "a fixed orientation during the entire simulation run. The "
            "orientation angle is updated according to the time simulated or "
            "a fixed axis position can be specified independently from the "
            "time interval that is simulated. Model Outputs: Outputs include "
            "the magnetospheric plasma parameters (atomic mass unit density "
            "N, pressure P, velocity V_x, V_y, V_z, magnetic field B_x, B_y, "
            "B_z, electric currents, J_x, J_y, J_z) and ionospheric "
            "parameters (electric potential PHI, and Hall and Pedersen "
            "conductances Sigma_H, Sigma_P).";

   char *terms_of_usage_text =
         "For tracking purposes for our government sponsors, we ask that "
            "you notify the CCMC whenever you use CCMC results in a "
            "scientific publications or presentations: "
            "ccmc@ccmc.gsfc.nasa.gov ";

   char *model_type = "Global Magnetosphere";
   int grid_system_count = 1;
   char *output_type = "Global Magnetospheric";
   char *grid_system_1 = "[ x, y, z ]";
   char *standard_grid_target = "GSM";
   char *default_grid_type = "GSM";

   /* use DatabaseInfo field cs_output */

   char *grid_1_type;

   /* set not_applicable string for use when dipole time makes no sense */

   char *not_applicable = "N/A";

   /*************** set dynamic values for ccmc_attributes for batsrus ********/
   /*  call/perform  necessary functions to set values for dynamic global
    * attributes */

   int grid_system_1_number_of_dimensions = abs(number_dimensions);

   int grid_system_1_dimension_1_size = special_parameter_NX * number_of_blocks;
   int grid_system_1_dimension_2_size = special_parameter_NY * number_of_blocks;
   int grid_system_1_dimension_3_size = special_parameter_NZ * number_of_blocks;

   /* set grid_type based on database info if available */

   if (strlen(cs_output) > 1)
   {
      grid_1_type = ( char * ) malloc(strlen(cs_output) + 1);
      strcpy(grid_1_type, cs_output);
   }
   else
   {
      printf("WARNING: coordinate system could not be identified through "
         "DatabaseInfo file.  Using default: %s\n", default_grid_type);
      grid_1_type = ( char * ) malloc(strlen(default_grid_type) + 1);
      strcpy(grid_1_type, default_grid_type);
   }

   number_of_cells = x_dimension_size * y_dimension_size * z_dimension_size;

   /****************************************************************************
    *                                CREATE THE GLOBAL ATTRIBUTES              *
    ***************************************************************************/

   /*********************************** GLOBAL ATTRIBUTES *********************/

   /****************************************************************************
    *    initialize the baselined_ccmc_attribute_structure_list to hold n      *
    *    number of cccmc_attribute_structures where                            *
    *    n = NUMBER_GLOBAL_ATTR = NUMBER_CCMC_ATTR + NUMBER_BATSRUS_ATTR as    *
    *    defined in ccmc_attributes.h & batsrus_attributes.h                   *
    ***************************************************************************/

   baselined_ccmc_attribute_structure_list
         = init_ccmc_attribute_structure( NUMBER_GLOBAL_ATTR);

   /*** for each name-data type pair in the ccmc_attribute_name_type_list, call
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

   /* for each name-data type pair in the batsrus_attribute_name_type_list,
    * call put_ccmc_attribute with attr name & data type since we are writing
    * from the batsrus_attribute_name_type_list, the attribute_type = model ***/

   for (i=0; i < NUMBER_BATSRUS_ATTR; i++)
   {
      put_ccmc_attribute(
            batsrus_attribute_name_type_list[i].batsrus_attr_name,
            "model",
            batsrus_attribute_name_type_list[i].batsrus_attr_data_type,
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
         "output_type",
         output_type,
         baselined_ccmc_attribute_structure_list);
   stat = update_ccmc_attribute_value(
         "standard_grid_target",
         standard_grid_target,
         baselined_ccmc_attribute_structure_list);
   stat = update_ccmc_attribute_value(
         "grid_1_type",
         grid_1_type,
         baselined_ccmc_attribute_structure_list);
   stat = update_ccmc_attribute_value(
         "kameleon_version",
         kameleon_version_num,
         baselined_ccmc_attribute_structure_list);

   /****** individual batsrus global attributes set by name using BATSRUS .OUT
    * values already in main memory *****/

   stat = update_ccmc_attribute_value(
         "elapsed_time_in_seconds",
         &elapsed_time_seconds,
         baselined_ccmc_attribute_structure_list);
   stat = update_ccmc_attribute_value(
         "number_of_dimensions",
         &number_dimensions,
         baselined_ccmc_attribute_structure_list);
   stat = update_ccmc_attribute_value(
         "special_parameter_g",
         &special_parameter_g,
         baselined_ccmc_attribute_structure_list);
   stat = update_ccmc_attribute_value(
         "special_parameter_c",
         &special_parameter_c,
         baselined_ccmc_attribute_structure_list);
   stat = update_ccmc_attribute_value(
         "special_parameter_th",
         &special_parameter_th,
         baselined_ccmc_attribute_structure_list);
   stat = update_ccmc_attribute_value(
         "special_parameter_P1",
         &special_parameter_P1,
         baselined_ccmc_attribute_structure_list);
   stat = update_ccmc_attribute_value(
         "special_parameter_P2",
         &special_parameter_P2,
         baselined_ccmc_attribute_structure_list);
   stat = update_ccmc_attribute_value(
         "special_parameter_P3",
         &special_parameter_P3,
         baselined_ccmc_attribute_structure_list);
   stat = update_ccmc_attribute_value(
         "special_parameter_R",
         &special_parameter_R,
         baselined_ccmc_attribute_structure_list);
   stat = update_ccmc_attribute_value(
         "special_parameter_NX",
         &special_parameter_NX,
         baselined_ccmc_attribute_structure_list);
   stat = update_ccmc_attribute_value(
         "special_parameter_NY",
         &special_parameter_NY,
         baselined_ccmc_attribute_structure_list);
   stat = update_ccmc_attribute_value(
         "special_parameter_NZ",
         &special_parameter_NZ,
         baselined_ccmc_attribute_structure_list);
   stat = update_ccmc_attribute_value(
         "x_dimension_size",
         &x_dimension_size,
         baselined_ccmc_attribute_structure_list);
   stat = update_ccmc_attribute_value(
         "y_dimension_size",
         &y_dimension_size,
         baselined_ccmc_attribute_structure_list);
   stat = update_ccmc_attribute_value(
         "z_dimension_size",
         &z_dimension_size,
         baselined_ccmc_attribute_structure_list);
   stat = update_ccmc_attribute_value(
         "current_iteration_step",
         &current_iteration_step,
         baselined_ccmc_attribute_structure_list);
   stat = update_ccmc_attribute_value(
         "global_x_min",
         &global_x_min,
         baselined_ccmc_attribute_structure_list);
   stat = update_ccmc_attribute_value(
         "global_x_max",
         &global_x_max,
         baselined_ccmc_attribute_structure_list);
   stat = update_ccmc_attribute_value(
         "global_y_min",
         &global_y_min,
         baselined_ccmc_attribute_structure_list);
   stat = update_ccmc_attribute_value(
         "global_y_max",
         &global_y_max,
         baselined_ccmc_attribute_structure_list);
   stat = update_ccmc_attribute_value(
         "global_z_min",
         &global_z_min,
         baselined_ccmc_attribute_structure_list);
   stat = update_ccmc_attribute_value(
         "global_z_max",
         &global_z_max,
         baselined_ccmc_attribute_structure_list);
   stat = update_ccmc_attribute_value(
         "max_amr_level",
         &max_amr_level,
         baselined_ccmc_attribute_structure_list);
   stat = update_ccmc_attribute_value(
         "number_of_cells",
         &number_of_cells,
         baselined_ccmc_attribute_structure_list);
   stat = update_ccmc_attribute_value(
         "number_of_blocks",
         &number_of_blocks,
         baselined_ccmc_attribute_structure_list);
   stat = update_ccmc_attribute_value(
         "smallest_cell_size",
         &smallest_cell_size,
         baselined_ccmc_attribute_structure_list);

   /****************** add attributes read in from DatabaseInfo file using
    * read_database.c code *****/

   /****** ONLY add if write_database_info_flag = 1 ********/

   if (write_database_info_flag)
   {

      /*** check to see if each variable has been set ie. strlen( variable ) >
       * 0 , otherwise CDF exits with error ********/

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
      if (strlen(b_dipole) )
      {
         stat = update_ccmc_attribute_value(
               "b_dipole",
               b_dipole,
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

      /****** add dipole time only if dipole update flag from DatabaseInfo file
       * == yes ********/

      if ( !strcmp(dipole_update, "yes") || !strcmp(dipole_update, "Yes")
            || !strcmp(dipole_update, "YES") )
      {

         /*** since dipole update flag == yes, dipole time = start_time +
          * elapsed_time_in_seconds ****/

         cdf_epoch_start_time_value = parseEPOCH3(start_time_cdf_epoch3);

         /*** so eleapsed_time_seconds won't be iin milliseconds ***/

         cdf_epoch_dipole_time_value = cdf_epoch_start_time_value
               + (elapsed_time_seconds * 1000 );

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
            stat = update_ccmc_attribute_value(
                  "dipole_tilt",
                  &special_parameter_th,
                  baselined_ccmc_attribute_structure_list);
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
                  &special_parameter_th,
                  baselined_ccmc_attribute_structure_list);
            stat = update_ccmc_attribute_value(
                  "dipole_tilt_y",
                  &zero,
                  baselined_ccmc_attribute_structure_list);
         }
         else if ( !strcmp(cs_output, "GSE") || !strcmp(cs_output, "Gse")
               || !strcmp(cs_output, "gse") )
         {
            float temp_dipole_tilt_y = atof(dipole_tilt_y);
            stat = update_ccmc_attribute_value(
                  "dipole_tilt",
                  &special_parameter_th,
                  baselined_ccmc_attribute_structure_list);
            stat = update_ccmc_attribute_value(
                  "dipole_tilt_y",
                  &temp_dipole_tilt_y,
                  baselined_ccmc_attribute_structure_list);
         }
         else
         {
            float temp_dipole_tilt_y = atof(dipole_tilt_y);
            printf(
                  "WARNING: unrecognized coordinate system: -->%s<-- .  Using "
                     "default dipole_tilt/tilt_y angles for %s\n",
                  cs_output,
                  default_grid_type);
            stat = update_ccmc_attribute_value(
                  "dipole_tilt",
                  &special_parameter_th,
                  baselined_ccmc_attribute_structure_list);
            stat = update_ccmc_attribute_value(
                  "dipole_tilt_y",
                  &temp_dipole_tilt_y,
                  baselined_ccmc_attribute_structure_list);
         }

      }
      else
      {
         float temp_dipole_tilt_y = atof(dipole_tilt_y);
         printf("WARNING: coordinate system was not be identified through "
            "DatabaseInfo file.  Using default dipole_tilt/tilt_y angles for "
            "%s\n", default_grid_type);
         stat = update_ccmc_attribute_value(
               "dipole_tilt",
               &special_parameter_th,
               baselined_ccmc_attribute_structure_list);
         stat = update_ccmc_attribute_value(
               "dipole_tilt_y",
               &temp_dipole_tilt_y,
               baselined_ccmc_attribute_structure_list);
      }

   } /* end of if( write_database_flag ) block */

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
    *    NUMBER_REGISTERED_BATSRUS_VARIABLES defined in                        *
    *    ccmc_registered_variables.h                                           *
    ***************************************************************************/

   baselined_ccmc_variable_structure_list
         = init_ccmc_variable_structure( NUMBER_REGISTERED_BATSRUS_VARIABLES);

   /**** Put default variable names and variable attribute values into the
    * ccmc_baselined_variable_structure_list ***/

   for (i = 0; i< NUMBER_REGISTERED_BATSRUS_VARIABLES; i++)
   {
      put_ccmc_variable(
            registered_batsrus_var_list[i].ccmc_var_name,
            registered_batsrus_var_list[i].ccmc_var_data_type,
            0,
            registered_batsrus_var_list[i].ccmc_var_classification,
            "",
            registered_batsrus_var_list[i].valid_min,
            registered_batsrus_var_list[i].valid_max,
            registered_batsrus_var_list[i].units,
            registered_batsrus_var_list[i].grid_system,
            registered_batsrus_var_list[i].mask,
            registered_batsrus_var_list[i].description,
            registered_batsrus_var_list[i].is_vector_component,
            registered_batsrus_var_list[i].position_grid_system,
            registered_batsrus_var_list[i].data_grid_system,
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
    *    write the actual variable values into the                             *
    *    baselined_ccmc_variable_structure_list                                *
    *    08/25/05 - lets add the actual min/max values at the same time        *
    ***************************************************************************/

   /*** write the original model variable values ***/

   if (verbose_flag)
   {
      printf("\nwritting original model output variables values into "
         "baselined_ccmc_variable_structure_list...\n");
   }

   /*** update each variable size as the defualts are set to zero, this reallocs
    * space for the actual values to be stored *****/

   stat = update_ccmc_variable_attribute_value(
         "x",
         "variable_size",
         &number_of_cells,
         baselined_ccmc_variable_structure_list);

   stat = update_ccmc_variable_value(
         "x",
         number_of_cells,
         x_position_arrayPtr,
         baselined_ccmc_variable_structure_list);

   stat = update_ccmc_variable_attribute_value(
         "x",
         "actual_min",
         &x_position_actual_min,
         baselined_ccmc_variable_structure_list);

   stat = update_ccmc_variable_attribute_value(
         "x",
         "actual_max",
         &x_position_actual_max,
         baselined_ccmc_variable_structure_list);

   stat = update_ccmc_variable_attribute_value(
         "y",
         "variable_size",
         &number_of_cells,
         baselined_ccmc_variable_structure_list);

   stat = update_ccmc_variable_value(
         "y",
         number_of_cells,
         y_position_arrayPtr,
         baselined_ccmc_variable_structure_list);

   stat = update_ccmc_variable_attribute_value(
         "y",
         "actual_min",
         &y_position_actual_min,
         baselined_ccmc_variable_structure_list);

   stat = update_ccmc_variable_attribute_value(
         "y",
         "actual_max",
         &y_position_actual_max,
         baselined_ccmc_variable_structure_list);

   stat = update_ccmc_variable_attribute_value(
         "z",
         "variable_size",
         &number_of_cells,
         baselined_ccmc_variable_structure_list);

   stat = update_ccmc_variable_value(
         "z",
         number_of_cells,
         z_position_arrayPtr,
         baselined_ccmc_variable_structure_list);

   stat = update_ccmc_variable_attribute_value(
         "z",
         "actual_min",
         &z_position_actual_min,
         baselined_ccmc_variable_structure_list);

   stat = update_ccmc_variable_attribute_value(
         "z",
         "actual_max",
         &z_position_actual_max,
         baselined_ccmc_variable_structure_list);

   stat = update_ccmc_variable_attribute_value(
         "bx",
         "variable_size",
         &number_of_cells,
         baselined_ccmc_variable_structure_list);

   stat = update_ccmc_variable_value(
         "bx",
         number_of_cells,
         bx_arrayPtr,
         baselined_ccmc_variable_structure_list);

   stat = update_ccmc_variable_attribute_value(
         "bx",
         "actual_min",
         &bx_actual_min,
         baselined_ccmc_variable_structure_list);

   stat = update_ccmc_variable_attribute_value(
         "bx",
         "actual_max",
         &bx_actual_max,
         baselined_ccmc_variable_structure_list);

   stat = update_ccmc_variable_attribute_value(
         "by",
         "variable_size",
         &number_of_cells,
         baselined_ccmc_variable_structure_list);

   stat = update_ccmc_variable_value(
         "by",
         number_of_cells,
         by_arrayPtr,
         baselined_ccmc_variable_structure_list);

   stat = update_ccmc_variable_attribute_value(
         "by",
         "actual_min",
         &by_actual_min,
         baselined_ccmc_variable_structure_list);

   stat = update_ccmc_variable_attribute_value(
         "by",
         "actual_max",
         &by_actual_max,
         baselined_ccmc_variable_structure_list);

   stat = update_ccmc_variable_attribute_value(
         "bz",
         "variable_size",
         &number_of_cells,
         baselined_ccmc_variable_structure_list);

   stat = update_ccmc_variable_value(
         "bz",
         number_of_cells,
         bz_arrayPtr,
         baselined_ccmc_variable_structure_list);

   stat = update_ccmc_variable_attribute_value(
         "bz",
         "actual_min",
         &bz_actual_min,
         baselined_ccmc_variable_structure_list);

   stat = update_ccmc_variable_attribute_value(
         "bz",
         "actual_max",
         &bz_actual_max,
         baselined_ccmc_variable_structure_list);

   stat = update_ccmc_variable_attribute_value(
         "b1x",
         "variable_size",
         &number_of_cells,
         baselined_ccmc_variable_structure_list);

   stat = update_ccmc_variable_value(
         "b1x",
         number_of_cells,
         b1x_arrayPtr,
         baselined_ccmc_variable_structure_list);

   stat = update_ccmc_variable_attribute_value(
         "b1x",
         "actual_min",
         &b1x_actual_min,
         baselined_ccmc_variable_structure_list);

   stat = update_ccmc_variable_attribute_value(
         "b1x",
         "actual_max",
         &b1x_actual_max,
         baselined_ccmc_variable_structure_list);

   stat = update_ccmc_variable_attribute_value(
         "b1y",
         "variable_size",
         &number_of_cells,
         baselined_ccmc_variable_structure_list);

   stat = update_ccmc_variable_value(
         "b1y",
         number_of_cells,
         b1y_arrayPtr,
         baselined_ccmc_variable_structure_list);

   stat = update_ccmc_variable_attribute_value(
         "b1y",
         "actual_min",
         &b1y_actual_min,
         baselined_ccmc_variable_structure_list);

   stat = update_ccmc_variable_attribute_value(
         "b1y",
         "actual_max",
         &b1y_actual_max,
         baselined_ccmc_variable_structure_list);

   stat = update_ccmc_variable_attribute_value(
         "b1z",
         "variable_size",
         &number_of_cells,
         baselined_ccmc_variable_structure_list);

   stat = update_ccmc_variable_value(
         "b1z",
         number_of_cells,
         b1z_arrayPtr,
         baselined_ccmc_variable_structure_list);

   stat = update_ccmc_variable_attribute_value(
         "b1z",
         "actual_min",
         &b1z_actual_min,
         baselined_ccmc_variable_structure_list);

   stat = update_ccmc_variable_attribute_value(
         "b1z",
         "actual_max",
         &b1z_actual_max,
         baselined_ccmc_variable_structure_list);

   stat = update_ccmc_variable_attribute_value(
         "ux",
         "variable_size",
         &number_of_cells,
         baselined_ccmc_variable_structure_list);

   stat = update_ccmc_variable_value(
         "ux",
         number_of_cells,
         ux_arrayPtr,
         baselined_ccmc_variable_structure_list);

   stat = update_ccmc_variable_attribute_value(
         "ux",
         "actual_min",
         &ux_actual_min,
         baselined_ccmc_variable_structure_list);

   stat = update_ccmc_variable_attribute_value(
         "ux",
         "actual_max",
         &ux_actual_max,
         baselined_ccmc_variable_structure_list);

   stat = update_ccmc_variable_attribute_value(
         "uy",
         "variable_size",
         &number_of_cells,
         baselined_ccmc_variable_structure_list);

   stat = update_ccmc_variable_value(
         "uy",
         number_of_cells,
         uy_arrayPtr,
         baselined_ccmc_variable_structure_list);

   stat = update_ccmc_variable_attribute_value(
         "uy",
         "actual_min",
         &uy_actual_min,
         baselined_ccmc_variable_structure_list);

   stat = update_ccmc_variable_attribute_value(
         "uy",
         "actual_max",
         &uy_actual_max,
         baselined_ccmc_variable_structure_list);

   stat = update_ccmc_variable_attribute_value(
         "uz",
         "variable_size",
         &number_of_cells,
         baselined_ccmc_variable_structure_list);

   stat = update_ccmc_variable_value(
         "uz",
         number_of_cells,
         uz_arrayPtr,
         baselined_ccmc_variable_structure_list);

   stat = update_ccmc_variable_attribute_value(
         "uz",
         "actual_min",
         &uz_actual_min,
         baselined_ccmc_variable_structure_list);

   stat = update_ccmc_variable_attribute_value(
         "uz",
         "actual_max",
         &uz_actual_max,
         baselined_ccmc_variable_structure_list);

   stat = update_ccmc_variable_attribute_value(
         "jx",
         "variable_size",
         &number_of_cells,
         baselined_ccmc_variable_structure_list);

   stat = update_ccmc_variable_value(
         "jx",
         number_of_cells,
         jx_arrayPtr,
         baselined_ccmc_variable_structure_list);

   stat = update_ccmc_variable_attribute_value(
         "jx",
         "actual_min",
         &jx_actual_min,
         baselined_ccmc_variable_structure_list);

   stat = update_ccmc_variable_attribute_value(
         "jx",
         "actual_max",
         &jx_actual_max,
         baselined_ccmc_variable_structure_list);

   stat = update_ccmc_variable_attribute_value(
         "jy",
         "variable_size",
         &number_of_cells,
         baselined_ccmc_variable_structure_list);

   stat = update_ccmc_variable_value(
         "jy",
         number_of_cells,
         jy_arrayPtr,
         baselined_ccmc_variable_structure_list);

   stat = update_ccmc_variable_attribute_value(
         "jy",
         "actual_min",
         &jy_actual_min,
         baselined_ccmc_variable_structure_list);

   stat = update_ccmc_variable_attribute_value(
         "jy",
         "actual_max",
         &jy_actual_max,
         baselined_ccmc_variable_structure_list);

   stat = update_ccmc_variable_attribute_value(
         "jz",
         "variable_size",
         &number_of_cells,
         baselined_ccmc_variable_structure_list);

   stat = update_ccmc_variable_value(
         "jz",
         number_of_cells,
         jz_arrayPtr,
         baselined_ccmc_variable_structure_list);

   stat = update_ccmc_variable_attribute_value(
         "jz",
         "actual_min",
         &jz_actual_min,
         baselined_ccmc_variable_structure_list);

   stat = update_ccmc_variable_attribute_value(
         "jz",
         "actual_max",
         &jz_actual_max,
         baselined_ccmc_variable_structure_list);

   stat = update_ccmc_variable_attribute_value(
         "rho",
         "variable_size",
         &number_of_cells,
         baselined_ccmc_variable_structure_list);

   stat = update_ccmc_variable_value(
         "rho",
         number_of_cells,
         rho_arrayPtr,
         baselined_ccmc_variable_structure_list);

   stat = update_ccmc_variable_attribute_value(
         "rho",
         "actual_min",
         &rho_actual_min,
         baselined_ccmc_variable_structure_list);

   stat = update_ccmc_variable_attribute_value(
         "rho",
         "actual_max",
         &rho_actual_max,
         baselined_ccmc_variable_structure_list);

   stat = update_ccmc_variable_attribute_value(
         "p",
         "variable_size",
         &number_of_cells,
         baselined_ccmc_variable_structure_list);

   stat = update_ccmc_variable_value(
         "p",
         number_of_cells,
         p_arrayPtr,
         baselined_ccmc_variable_structure_list);

   stat = update_ccmc_variable_attribute_value(
         "p",
         "actual_min",
         &p_actual_min,
         baselined_ccmc_variable_structure_list);

   stat = update_ccmc_variable_attribute_value(
         "p",
         "actual_max",
         &p_actual_max,
         baselined_ccmc_variable_structure_list);

   stat = update_ccmc_variable_attribute_value(
         "e",
         "variable_size",
         &number_of_cells,
         baselined_ccmc_variable_structure_list);

   stat = update_ccmc_variable_value(
         "e",
         number_of_cells,
         e_arrayPtr,
         baselined_ccmc_variable_structure_list);

   stat = update_ccmc_variable_attribute_value(
         "e",
         "actual_min",
         &e_actual_min,
         baselined_ccmc_variable_structure_list);

   stat = update_ccmc_variable_attribute_value(
         "e",
         "actual_max",
         &e_actual_max,
         baselined_ccmc_variable_structure_list);
         
         
   /*** Adding status variable ***/
   if( optional_status_variable_present )
     {
       stat = update_ccmc_variable_attribute_value("status","variable_size",&number_of_cells,baselined_ccmc_variable_structure_list);
       stat = update_ccmc_variable_value("status",number_of_cells,status_arrayPtr,baselined_ccmc_variable_structure_list);
       stat = update_ccmc_variable_attribute_value("status","actual_min",&status_actual_min,baselined_ccmc_variable_structure_list);
       stat = update_ccmc_variable_attribute_value("status","actual_max",&status_actual_max,baselined_ccmc_variable_structure_list);  
     }
         
  /*** Adding solar wind fluid variables ***/
	 /* swux */
   if( optional_swux_variable_present )
     {
       stat = update_ccmc_variable_attribute_value("swux","variable_size",&number_of_cells,baselined_ccmc_variable_structure_list);
       stat = update_ccmc_variable_value("swux",number_of_cells,swux_arrayPtr,baselined_ccmc_variable_structure_list);
       stat = update_ccmc_variable_attribute_value("swux","actual_min",&swux_actual_min,baselined_ccmc_variable_structure_list);
       stat = update_ccmc_variable_attribute_value("swux","actual_max",&swux_actual_max,baselined_ccmc_variable_structure_list);
     }
	 /* swuy */
   if( optional_swuy_variable_present )
     {
       stat = update_ccmc_variable_attribute_value("swuy","variable_size",&number_of_cells,baselined_ccmc_variable_structure_list);
       stat = update_ccmc_variable_value("swuy",number_of_cells,swuy_arrayPtr,baselined_ccmc_variable_structure_list);
       stat = update_ccmc_variable_attribute_value("swuy","actual_min",&swuy_actual_min,baselined_ccmc_variable_structure_list);
       stat = update_ccmc_variable_attribute_value("swuy","actual_max",&swuy_actual_max,baselined_ccmc_variable_structure_list);
     }
	 /* swuz */
   if( optional_swuz_variable_present )
     {
       stat = update_ccmc_variable_attribute_value("swuz","variable_size",&number_of_cells,baselined_ccmc_variable_structure_list);
       stat = update_ccmc_variable_value("swuz",number_of_cells,swuz_arrayPtr,baselined_ccmc_variable_structure_list);
       stat = update_ccmc_variable_attribute_value("swuz","actual_min",&swuz_actual_min,baselined_ccmc_variable_structure_list);
       stat = update_ccmc_variable_attribute_value("swuz","actual_max",&swuz_actual_max,baselined_ccmc_variable_structure_list);
     }
	 /* swrho */
   if( optional_swrho_variable_present )
     {
       stat = update_ccmc_variable_attribute_value("swrho","variable_size",&number_of_cells,baselined_ccmc_variable_structure_list);
       stat = update_ccmc_variable_value("swrho",number_of_cells,swrho_arrayPtr,baselined_ccmc_variable_structure_list);
       stat = update_ccmc_variable_attribute_value("swrho","actual_min",&swrho_actual_min,baselined_ccmc_variable_structure_list);
       stat = update_ccmc_variable_attribute_value("swrho","actual_max",&swrho_actual_max,baselined_ccmc_variable_structure_list);
     }
	 /* swp */
   if( optional_swp_variable_present )
     {
       stat = update_ccmc_variable_attribute_value("swp","variable_size",&number_of_cells,baselined_ccmc_variable_structure_list);
       stat = update_ccmc_variable_value("swp",number_of_cells,swp_arrayPtr,baselined_ccmc_variable_structure_list);
       stat = update_ccmc_variable_attribute_value("swp","actual_min",&swp_actual_min,baselined_ccmc_variable_structure_list);
       stat = update_ccmc_variable_attribute_value("swp","actual_max",&swp_actual_max,baselined_ccmc_variable_structure_list);
     }
	 /* ionoux */
   if( optional_ionoux_variable_present )
     {
       stat = update_ccmc_variable_attribute_value("ionoux","variable_size",&number_of_cells,baselined_ccmc_variable_structure_list);
       stat = update_ccmc_variable_value("ionoux",number_of_cells,ionoux_arrayPtr,baselined_ccmc_variable_structure_list);
       stat = update_ccmc_variable_attribute_value("ionoux","actual_min",&ionoux_actual_min,baselined_ccmc_variable_structure_list);
       stat = update_ccmc_variable_attribute_value("ionoux","actual_max",&ionoux_actual_max,baselined_ccmc_variable_structure_list);
     }
	 /* ionouy */
   if( optional_ionouy_variable_present )
     {
       stat = update_ccmc_variable_attribute_value("ionouy","variable_size",&number_of_cells,baselined_ccmc_variable_structure_list);
       stat = update_ccmc_variable_value("ionouy",number_of_cells,ionouy_arrayPtr,baselined_ccmc_variable_structure_list);
       stat = update_ccmc_variable_attribute_value("ionouy","actual_min",&ionouy_actual_min,baselined_ccmc_variable_structure_list);
       stat = update_ccmc_variable_attribute_value("ionouy","actual_max",&ionouy_actual_max,baselined_ccmc_variable_structure_list);
     }
	 /* ionouy */
   if( optional_ionouy_variable_present )
     {
       stat = update_ccmc_variable_attribute_value("ionouy","variable_size",&number_of_cells,baselined_ccmc_variable_structure_list);
       stat = update_ccmc_variable_value("ionouy",number_of_cells,ionouy_arrayPtr,baselined_ccmc_variable_structure_list);
       stat = update_ccmc_variable_attribute_value("ionouy","actual_min",&ionouy_actual_min,baselined_ccmc_variable_structure_list);
       stat = update_ccmc_variable_attribute_value("ionouy","actual_max",&ionouy_actual_max,baselined_ccmc_variable_structure_list);
     }
	 /* ionouz */
   if( optional_ionouz_variable_present )
     {
       stat = update_ccmc_variable_attribute_value("ionouz","variable_size",&number_of_cells,baselined_ccmc_variable_structure_list);
       stat = update_ccmc_variable_value("ionouz",number_of_cells,ionouz_arrayPtr,baselined_ccmc_variable_structure_list);
       stat = update_ccmc_variable_attribute_value("ionouz","actual_min",&ionouz_actual_min,baselined_ccmc_variable_structure_list);
       stat = update_ccmc_variable_attribute_value("ionouz","actual_max",&ionouz_actual_max,baselined_ccmc_variable_structure_list);
     }
	 /* ionorho */
   if( optional_ionorho_variable_present )
     {
       stat = update_ccmc_variable_attribute_value("ionorho","variable_size",&number_of_cells,baselined_ccmc_variable_structure_list);
       stat = update_ccmc_variable_value("ionorho",number_of_cells,ionorho_arrayPtr,baselined_ccmc_variable_structure_list);
       stat = update_ccmc_variable_attribute_value("ionorho","actual_min",&ionorho_actual_min,baselined_ccmc_variable_structure_list);
       stat = update_ccmc_variable_attribute_value("ionorho","actual_max",&ionorho_actual_max,baselined_ccmc_variable_structure_list);
     }
	 /* ionop */
   if( optional_ionop_variable_present )
     {
       stat = update_ccmc_variable_attribute_value("ionop","variable_size",&number_of_cells,baselined_ccmc_variable_structure_list);
       stat = update_ccmc_variable_value("ionop",number_of_cells,ionop_arrayPtr,baselined_ccmc_variable_structure_list);
       stat = update_ccmc_variable_attribute_value("ionop","actual_min",&ionop_actual_min,baselined_ccmc_variable_structure_list);
       stat = update_ccmc_variable_attribute_value("ionop","actual_max",&ionop_actual_max,baselined_ccmc_variable_structure_list);
     }


   /**************** now write grid description variables *********************/

   if (verbose_flag)
   {
      printf("\nwritting grid description data values into "
         "baselined_ccmc_variable_structure_list...\n");
   }

   stat = update_ccmc_variable_attribute_value(
         "block_at_amr_level",
         "variable_size",
         &number_of_parent_blocks_at_amr_level[0],
         baselined_ccmc_variable_structure_list);
   stat = update_ccmc_variable_value(
         "block_at_amr_level",
         number_of_parent_blocks_at_amr_level[0],
         block_at_amr_level,
         baselined_ccmc_variable_structure_list);

   /* make room for all original blocks plus virtual blocks */

   local_size = ceil( ( 8 * number_of_blocks ) / 7);

   stat = update_ccmc_variable_attribute_value(
         "block_x_min",
         "variable_size",
         &local_size,
         baselined_ccmc_variable_structure_list);

   stat = update_ccmc_variable_value(
         "block_x_min",
         local_size,
         block_x_min,
         baselined_ccmc_variable_structure_list);

   stat = update_ccmc_variable_attribute_value(
         "block_x_max",
         "variable_size",
         &local_size,
         baselined_ccmc_variable_structure_list);

   stat = update_ccmc_variable_value(
         "block_x_max",
         local_size,
         block_x_max,
         baselined_ccmc_variable_structure_list);

   stat = update_ccmc_variable_attribute_value(
         "block_y_min",
         "variable_size",
         &local_size,
         baselined_ccmc_variable_structure_list);

   stat = update_ccmc_variable_value(
         "block_y_min",
         local_size,
         block_y_min,
         baselined_ccmc_variable_structure_list);

   stat = update_ccmc_variable_attribute_value(
         "block_y_max",
         "variable_size",
         &local_size,
         baselined_ccmc_variable_structure_list);

   stat = update_ccmc_variable_value(
         "block_y_max",
         local_size,
         block_y_max,
         baselined_ccmc_variable_structure_list);

   stat = update_ccmc_variable_attribute_value(
         "block_z_min",
         "variable_size",
         &local_size,
         baselined_ccmc_variable_structure_list);

   stat = update_ccmc_variable_value(
         "block_z_min",
         local_size,
         block_z_min,
         baselined_ccmc_variable_structure_list);

   stat = update_ccmc_variable_attribute_value(
         "block_z_max",
         "variable_size",
         &local_size,
         baselined_ccmc_variable_structure_list);

   stat = update_ccmc_variable_value(
         "block_z_max",
         local_size,
         block_z_max,
         baselined_ccmc_variable_structure_list);

   stat = update_ccmc_variable_attribute_value(
         "block_x_center",
         "variable_size",
         &local_size,
         baselined_ccmc_variable_structure_list);

   stat = update_ccmc_variable_value(
         "block_x_center",
         local_size,
         block_x_center,
         baselined_ccmc_variable_structure_list);

   stat = update_ccmc_variable_attribute_value(
         "block_y_center",
         "variable_size",
         &local_size,
         baselined_ccmc_variable_structure_list);

   stat = update_ccmc_variable_value(
         "block_y_center",
         local_size,
         block_y_center,
         baselined_ccmc_variable_structure_list);

   stat = update_ccmc_variable_attribute_value(
         "block_z_center",
         "variable_size",
         &local_size,
         baselined_ccmc_variable_structure_list);

   stat = update_ccmc_variable_value(
         "block_z_center",
         local_size,
         block_z_center,
         baselined_ccmc_variable_structure_list);

   stat = update_ccmc_variable_attribute_value(
         "block_amr_levels",
         "variable_size",
         &local_size,
         baselined_ccmc_variable_structure_list);

   stat = update_ccmc_variable_value(
         "block_amr_levels",
         local_size,
         block_amr_levels,
         baselined_ccmc_variable_structure_list);

   stat = update_ccmc_variable_attribute_value(
         "block_child_count",
         "variable_size",
         &local_size,
         baselined_ccmc_variable_structure_list);

   stat = update_ccmc_variable_value(
         "block_child_count",
         local_size,
         child_count,
         baselined_ccmc_variable_structure_list);

   stat = update_ccmc_variable_attribute_value(
         "block_parent_id",
         "variable_size",
         &local_size,
         baselined_ccmc_variable_structure_list);

   stat = update_ccmc_variable_value(
         "block_parent_id",
         local_size,
         parent_id,
         baselined_ccmc_variable_structure_list);

   stat = update_ccmc_variable_attribute_value(
         "block_child_id_1",
         "variable_size",
         &local_size,
         baselined_ccmc_variable_structure_list);

   stat = update_ccmc_variable_value(
         "block_child_id_1",
         local_size,
         child_id_1,
         baselined_ccmc_variable_structure_list);

   stat = update_ccmc_variable_attribute_value(
         "block_child_id_2",
         "variable_size",
         &local_size,
         baselined_ccmc_variable_structure_list);

   stat = update_ccmc_variable_value(
         "block_child_id_2",
         local_size,
         child_id_2,
         baselined_ccmc_variable_structure_list);

   stat = update_ccmc_variable_attribute_value(
         "block_child_id_3",
         "variable_size",
         &local_size,
         baselined_ccmc_variable_structure_list);

   stat = update_ccmc_variable_value(
         "block_child_id_3",
         local_size,
         child_id_3,
         baselined_ccmc_variable_structure_list);

   stat = update_ccmc_variable_attribute_value(
         "block_child_id_4",
         "variable_size",
         &local_size,
         baselined_ccmc_variable_structure_list);

   stat = update_ccmc_variable_value(
         "block_child_id_4",
         local_size,
         child_id_4,
         baselined_ccmc_variable_structure_list);

   stat = update_ccmc_variable_attribute_value(
         "block_child_id_5",
         "variable_size",
         &local_size,
         baselined_ccmc_variable_structure_list);

   stat = update_ccmc_variable_value(
         "block_child_id_5",
         local_size,
         child_id_5,
         baselined_ccmc_variable_structure_list);

   stat = update_ccmc_variable_attribute_value(
         "block_child_id_6",
         "variable_size",
         &local_size,
         baselined_ccmc_variable_structure_list);

   stat = update_ccmc_variable_value(
         "block_child_id_6",
         local_size,
         child_id_6,
         baselined_ccmc_variable_structure_list);

   stat = update_ccmc_variable_attribute_value(
         "block_child_id_7",
         "variable_size",
         &local_size,
         baselined_ccmc_variable_structure_list);

   stat = update_ccmc_variable_value(
         "block_child_id_7",
         local_size,
         child_id_7,
         baselined_ccmc_variable_structure_list);

   stat = update_ccmc_variable_attribute_value(
         "block_child_id_8",
         "variable_size",
         &local_size,
         baselined_ccmc_variable_structure_list);

   stat = update_ccmc_variable_value(
         "block_child_id_8",
         local_size,
         child_id_8,
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

   free(rho_arrayPtr);
   free(ux_arrayPtr);
   free(uy_arrayPtr);
   free(uz_arrayPtr);
   free(p_arrayPtr);

   free(swrho_arrayPtr);
   free(swux_arrayPtr);
   free(swuy_arrayPtr);
   free(swuz_arrayPtr);
   free(swp_arrayPtr);

   free(ionorho_arrayPtr);
   free(ionoux_arrayPtr);
   free(ionouy_arrayPtr);
   free(ionouz_arrayPtr);
   free(ionop_arrayPtr);

   free(bx_arrayPtr);
   free(by_arrayPtr);
   free(bz_arrayPtr);
   free(b1x_arrayPtr);
   free(b1y_arrayPtr);
   free(b1z_arrayPtr);
   free(e_arrayPtr);
   free(jx_arrayPtr);
   free(jy_arrayPtr);
   free(jz_arrayPtr);
   free(x_position_arrayPtr);
   free(y_position_arrayPtr);
   free(z_position_arrayPtr);
   free(block_x_min);
   free(block_x_max);
   free(block_y_min);
   free(block_y_max);
   free(block_z_min);
   free(block_z_max);
   free(block_x_center);
   free(block_y_center);
   free(block_z_center);
   free(block_amr_levels);
   free(block_at_amr_level);
   free(number_of_blocks_at_amr_level);
   free(number_of_parent_blocks_at_amr_level);
   free(parent_id);
   free(child_count);
   free(child_id_1);
   free(child_id_2);
   free(child_id_3);
   free(child_id_4);
   free(child_id_5);
   free(child_id_6);
   free(child_id_7);
   free(child_id_8);

   /*
   print_ccmc_attribute_structure(
         baselined_ccmc_attribute_structure_list,
         put_attribute_call_count);
   print_ccmc_variable_structure(
         baselined_ccmc_variable_structure_list,
         put_variable_call_count);
   */

   return 0;

}
