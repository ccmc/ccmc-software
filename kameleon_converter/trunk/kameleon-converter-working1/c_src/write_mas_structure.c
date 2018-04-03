/*******************************************************************************
 *                                                                             *
 *    NAME:          write_mas_structure.c                                     *
 *                                                                             *
 *    VERSION:       1.0                                                       *
 *                                                                             *
 *    AUTHOR:        Marlo M. H. Maddox                                        *
 *                   NASA-GSFC-AETD-ISD-ADMA(587)                              *
 *                   Marlo.Maddox@nasa.gov                                     *
 *                   (301) 286 - 5202                                          *
 *                                                                             *
 *    PURPOSE:       specialized write routine that writes mas data from main  *
 *                   mem to CCMC standardized data structures.                 *
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
 *    02.06.2007     Maddox, Marlo                                             *
 *                   initial development started                               *
 *                                                                             *
 *    03.19.2007     Maddox, Marlo                                             *
 *                   fixing reversed r, theta problem                          *
 *                                                                             *
 *    03.26.2007     Maddox, Marlo                                             *
 *                   Fixing all coordinates                                    *
 *                                                                             *
 *    04.06.2007     Maddox, Marlo                                             *
 *                   adding support for offset position arrays                 *
 *                                                                             *
 *    05.17.2007     Maddox, Marlo                                             *
 *                   adding update for kameleon_version gattribute             *
 *                   							       *
 *    06.07.2011     Maddox, Marlo                                             *
 *    		     Adding attributes for new grid combinations due to newly  *
 *    		     added phi+1 dimmension                                    *
 *    2018/03/30     Lutz Rastaetter                                           *
 *                   Model output file for p is optional                       *
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

#include "mas_attributes.h"
#include "read_mas.h"

#include "kameleon_version_info.h"

#define NUMBER_GLOBAL_ATTR    NUMBER_CCMC_ATTR + NUMBER_MAS_ATTR
#define NUMBER_VARIABLE_ATTR  NUMBER_CCMC_VARIABLE_ATTR + NUMBER_MAS_VARIABLE_ATTR

#define DEBUG_FLAG 1

int write_mas_structure(
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

   /** define static values for ccmc_attributes for mas_model **/

   char *kameleon_version_num= KAMELEON_VERSION;

   char
         *readme_text =
               "MHD Model of Solar Corona. Model domain: 1 - 30 solar radii. "
               "**INSTITUTION: Science Applications International Corporation "
               "(SAIC), San Diego, CA.  **AUTHORS: J. Linker, Z. Mikic, R. "
               "Lionello, P. Riley";

   char *model_type = "solar";

   char
         *terms_of_usage_text =
               "For tracking purposes for our government sponsors, we ask that "
               "you notify the CCMC whenever you use CCMC results in a "
               "scientific publications or presentations: "
               "ccmc@ccmc.gsfc.nasa.gov ";

   int grid_system_count = 7;

   char *grid_system_1 = "[ phi1, theta1, r1 ]";
   char *grid_system_2 = "[ phi1, theta1, r ]";
   char *grid_system_3 = "[ phi1, theta , r1 ]";
   char *grid_system_4 = "[ phi1 , theta, r ]";

   /* Addedgridds 5 - 8 below on 06.07.2011 to account for new phi+1 offset dimension */

   char *grid_system_5 = "[ phi, theta1 , r1 ]";
   char *grid_system_6 = "[ phi , theta1, r ]";
   char *grid_system_7 = "[ phi, theta , r1 ]";

   char *output_type = "solar";

   char *standard_grid_target = "[ r, lon, colat ]";

   char *grid_1_type = "spherical phi1, theta1, r1";
   char *grid_2_type = "spherical phi1, theta1, r";
   char *grid_3_type = "spherical phi1, theta , r1";
   char *grid_4_type = "spherical phi1, theta , r";
   char *grid_5_type = "spherical phi , theta1, r1";
   char *grid_6_type = "spherical phi , theta1, r";
   char *grid_7_type = "spherical phi , theta , r1";


   /** we also need to set the inits for r to R_S **/

   char *r_units = "R_s";

   /* set not_applicable string for use when dipole time makes no sense */

   char *not_applicable = "N/A";

   int grid_system_1_number_of_dimensions = 3; /* assuming 3 add smarts later */
   int grid_system_2_number_of_dimensions = 3; /* assuming 3 add smarts later */
   int grid_system_3_number_of_dimensions = 3; /* assuming 3 add smarts later */
   int grid_system_4_number_of_dimensions = 3; /* assuming 3 add smarts later */
   int grid_system_5_number_of_dimensions = 3; /* assuming 3 add smarts later */
   int grid_system_6_number_of_dimensions = 3; /* assuming 3 add smarts later */
   int grid_system_7_number_of_dimensions = 3; /* assuming 3 add smarts later */

   int i, stat;
   float zero = 0.0;

   /* naxis1, naxis2 defined in read_mas.h */

   int number_of_cells1, number_of_cells2, number_of_cells3, number_of_cells4,
   number_of_cells5, number_of_cells6, number_of_cells7, number_of_cells8,
         phi_dimension_size = fakedim0, theta_dimension_size = fakedim1,
         r_dimension_size = fakedim2;
   int phi_plus1_dimension_size = fakedim0_plus1,theta_plus1_dimension_size = fakedim1_plus1, r_plus1_dimension_size =
         fakedim2_plus1;

   /*************** set dynamic values for ccmc_attributes for mas ************/
   /*  call/perform  necessary functions to set values for dynamic global
    * attributes */

   int grid_system_1_dimension_1_size = phi_plus1_dimension_size;
   int grid_system_1_dimension_2_size = theta_plus1_dimension_size;
   int grid_system_1_dimension_3_size = r_plus1_dimension_size;

   int grid_system_2_dimension_1_size = phi_plus1_dimension_size;
   int grid_system_2_dimension_2_size = theta_plus1_dimension_size;
   int grid_system_2_dimension_3_size = r_dimension_size;

   int grid_system_3_dimension_1_size = phi_plus1_dimension_size;
   int grid_system_3_dimension_2_size = theta_dimension_size;
   int grid_system_3_dimension_3_size = r_plus1_dimension_size;

   int grid_system_4_dimension_1_size = phi_plus1_dimension_size;
   int grid_system_4_dimension_2_size = theta_dimension_size;
   int grid_system_4_dimension_3_size = r_dimension_size;

   int grid_system_5_dimension_1_size = phi_dimension_size;
   int grid_system_5_dimension_2_size = theta_plus1_dimension_size;
   int grid_system_5_dimension_3_size = r_plus1_dimension_size;

   int grid_system_6_dimension_1_size = phi_dimension_size;
   int grid_system_6_dimension_2_size = theta_plus1_dimension_size;
   int grid_system_6_dimension_3_size = r_dimension_size;

   int grid_system_7_dimension_1_size = phi_dimension_size;
   int grid_system_7_dimension_2_size = theta_dimension_size;
   int grid_system_7_dimension_3_size = r_plus1_dimension_size;


   /** calculate the number of expected cells for each grid combo **/

   number_of_cells1 = phi_plus1_dimension_size * theta_plus1_dimension_size
         * r_plus1_dimension_size; /** for variables rho, t, p **/

   number_of_cells2 = phi_plus1_dimension_size * theta_plus1_dimension_size
         * r_dimension_size; /** vr, jr  **/

   number_of_cells3 = phi_plus1_dimension_size * theta_dimension_size
         * r_plus1_dimension_size; /** v_theta, j_theta  **/

   number_of_cells4 = phi_plus1_dimension_size * theta_dimension_size
         * r_dimension_size; /** b_phi **/

   number_of_cells5 = phi_dimension_size * theta_plus1_dimension_size
         * r_plus1_dimension_size; /** v_phi, j_phi **/

   number_of_cells6 = phi_dimension_size * theta_plus1_dimension_size
         * r_dimension_size; /** b_theta  **/

   number_of_cells7 = phi_dimension_size * theta_dimension_size
         * r_plus1_dimension_size; /** b_r  **/


   /****************************************************************************
    *                                CREATE THE GLOBAL ATTRIBUTES              *
    ***************************************************************************/

   /*********************************** GLOBAL ATTRIBUTES *********************/

   /****************************************************************************
    *    initialize the baselined_ccmc_attribute_structure_list to hold n      *
    *    number of cccmc_attribute_structures where n = NUMBER_GLOBAL_ATTR =   *
    *    NUMBER_CCMC_ATTR + NUMBER_MAS_ATTR as defined in ccmc_attributes.h &  *
    *    mas_attributes.h                                                      *
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

   /* for each name-data type pair in the mas_attribute_name_type_list, call
    * put_ccmc_attribute with attr name & data type since we are writing from
    * the mas_attribute_name_type_list, the attribute_type = model ***/

   /** !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! NOTE !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
    * weird error when trying to execute following loop with an empty
    * mas_attribute_name_type_list, code must have evaluated 0 < 0 to even
    * enter that block
    ***/

   for (i=0; i < NUMBER_MAS_ATTR; i++)
   {
      put_ccmc_attribute(
            mas_attribute_name_type_list[i].mas_attr_name,
            "model",
            mas_attribute_name_type_list[i].mas_attr_data_type,
            "",
            baselined_ccmc_attribute_structure_list);

   }

   if (verbose_flag)
   {
      printf("global attribute names successfully added to "
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
            "grid_system_5_number_of_dimensions",
            &grid_system_5_number_of_dimensions,
            baselined_ccmc_attribute_structure_list);
      stat = update_ccmc_attribute_value(
            "grid_system_5_dimension_1_size",
            &grid_system_5_dimension_1_size,
            baselined_ccmc_attribute_structure_list);
      stat = update_ccmc_attribute_value(
            "grid_system_5_dimension_2_size",
            &grid_system_5_dimension_2_size,
            baselined_ccmc_attribute_structure_list);
      stat = update_ccmc_attribute_value(
            "grid_system_5_dimension_3_size",
            &grid_system_5_dimension_3_size,
            baselined_ccmc_attribute_structure_list);
      stat = update_ccmc_attribute_value(
            "grid_system_5",
            grid_system_5,
            baselined_ccmc_attribute_structure_list);
      stat = update_ccmc_attribute_value(
            "grid_5_type",
            grid_5_type,
            baselined_ccmc_attribute_structure_list);


      stat = update_ccmc_attribute_value(
               "grid_system_6_number_of_dimensions",
               &grid_system_6_number_of_dimensions,
               baselined_ccmc_attribute_structure_list);
         stat = update_ccmc_attribute_value(
               "grid_system_6_dimension_1_size",
               &grid_system_6_dimension_1_size,
               baselined_ccmc_attribute_structure_list);
         stat = update_ccmc_attribute_value(
               "grid_system_6_dimension_2_size",
               &grid_system_6_dimension_2_size,
               baselined_ccmc_attribute_structure_list);
         stat = update_ccmc_attribute_value(
               "grid_system_6_dimension_3_size",
               &grid_system_6_dimension_3_size,
               baselined_ccmc_attribute_structure_list);
         stat = update_ccmc_attribute_value(
               "grid_system_6",
               grid_system_6,
               baselined_ccmc_attribute_structure_list);
         stat = update_ccmc_attribute_value(
               "grid_6_type",
               grid_6_type,
               baselined_ccmc_attribute_structure_list);



         stat = update_ccmc_attribute_value(
                  "grid_system_7_number_of_dimensions",
                  &grid_system_7_number_of_dimensions,
                  baselined_ccmc_attribute_structure_list);
            stat = update_ccmc_attribute_value(
                  "grid_system_7_dimension_1_size",
                  &grid_system_7_dimension_1_size,
                  baselined_ccmc_attribute_structure_list);
            stat = update_ccmc_attribute_value(
                  "grid_system_7_dimension_2_size",
                  &grid_system_7_dimension_2_size,
                  baselined_ccmc_attribute_structure_list);
            stat = update_ccmc_attribute_value(
                  "grid_system_7_dimension_3_size",
                  &grid_system_7_dimension_3_size,
                  baselined_ccmc_attribute_structure_list);
            stat = update_ccmc_attribute_value(
                  "grid_system_7",
                  grid_system_7,
                  baselined_ccmc_attribute_structure_list);
            stat = update_ccmc_attribute_value(
                  "grid_7_type",
                  grid_7_type,
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
    *    initialize the baselined_ccmc_variable_structure_list to hold n number*
    *    of cccmc_variable_structures where n = NUMBER_REGISTERED_MAS_VARIABLES*
    *    defined in ccmc_registered_variables.h                                *
    ***************************************************************************/

   baselined_ccmc_variable_structure_list
         = init_ccmc_variable_structure( NUMBER_REGISTERED_MAS_VARIABLES);

   /**** Put default variable names and variable attribute values into the
    * ccmc_baselined_variable_structure_list ***/

   for (i = 0; i< NUMBER_REGISTERED_MAS_VARIABLES; i++)
   {
      put_ccmc_variable(
            registered_mas_var_list[i].ccmc_var_name,
            registered_mas_var_list[i].ccmc_var_data_type,
            0,
            registered_mas_var_list[i].ccmc_var_classification,
            "",
            registered_mas_var_list[i].valid_min,
            registered_mas_var_list[i].valid_max,
            registered_mas_var_list[i].units,
            registered_mas_var_list[i].grid_system,
            registered_mas_var_list[i].mask,
            registered_mas_var_list[i].description,
            registered_mas_var_list[i].is_vector_component,
            registered_mas_var_list[i].position_grid_system,
            registered_mas_var_list[i].data_grid_system,
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
    *    baselined_ccmc_variable_structure_list 08/25/05 - lets add the actual *
    *    min/max values at the same time                                       *
    ***************************************************************************/

   /*** write the original model variable values ***/

   if (verbose_flag)
   {
      printf("\nwritting original model output variables values into "
            "baselined_ccmc_variable_structure_list...\n");
   }

   /*** update each variable size as the defualts are set to zero, this reallocs
    * space for the actual values to be stored *****/

   /***************************** write position variables with two components
    * (carrington_longitiude,carrington_sine_latitude)  *********/

   stat = update_ccmc_variable_attribute_value(
         "phi",
         "variable_size",
         &phi_dimension_size,
         baselined_ccmc_variable_structure_list);
   stat = update_ccmc_variable_value(
         "phi",
         phi_dimension_size,
         dim0,
         baselined_ccmc_variable_structure_list);
   stat = update_ccmc_variable_attribute_value(
         "phi",
         "actual_min",
         &phi_actual_min,
         baselined_ccmc_variable_structure_list);
   stat = update_ccmc_variable_attribute_value(
         "phi",
         "actual_max",
         &phi_actual_max,
         baselined_ccmc_variable_structure_list);

   stat = update_ccmc_variable_attribute_value(
         "theta",
         "variable_size",
         &theta_dimension_size,
         baselined_ccmc_variable_structure_list);
   stat = update_ccmc_variable_value(
         "theta",
         theta_dimension_size,
         dim1,
         baselined_ccmc_variable_structure_list);
   stat = update_ccmc_variable_attribute_value(
         "theta",
         "actual_min",
         &theta_actual_min,
         baselined_ccmc_variable_structure_list);
   stat = update_ccmc_variable_attribute_value(
         "theta",
         "actual_max",
         &theta_actual_max,
         baselined_ccmc_variable_structure_list);

   stat = update_ccmc_variable_attribute_value(
         "r",
         "variable_size",
         &r_dimension_size,
         baselined_ccmc_variable_structure_list);
   stat = update_ccmc_variable_value(
         "r",
         r_dimension_size,
         dim2,
         baselined_ccmc_variable_structure_list);
   stat = update_ccmc_variable_attribute_value(
         "r",
         "actual_min",
         &r_actual_min,
         baselined_ccmc_variable_structure_list);
   stat = update_ccmc_variable_attribute_value(
         "r",
         "actual_max",
         &r_actual_max,
         baselined_ccmc_variable_structure_list);

   stat = update_ccmc_variable_attribute_value(
         "phi1",
         "variable_size",
         &phi_plus1_dimension_size,
         baselined_ccmc_variable_structure_list);
   stat = update_ccmc_variable_value(
         "phi1",
         phi_plus1_dimension_size,
         dim0_plus1,
         baselined_ccmc_variable_structure_list);
   stat = update_ccmc_variable_attribute_value(
         "phi1",
         "actual_min",
         &phi_plus1_actual_min,
         baselined_ccmc_variable_structure_list);
   stat = update_ccmc_variable_attribute_value(
         "phi1",
         "actual_max",
         &phi_plus1_actual_max,
         baselined_ccmc_variable_structure_list);



   stat = update_ccmc_variable_attribute_value(
         "theta1",
         "variable_size",
         &theta_plus1_dimension_size,
         baselined_ccmc_variable_structure_list);
   stat = update_ccmc_variable_value(
         "theta1",
         theta_plus1_dimension_size,
         dim1_plus1,
         baselined_ccmc_variable_structure_list);
   stat = update_ccmc_variable_attribute_value(
         "theta1",
         "actual_min",
         &theta_plus1_actual_min,
         baselined_ccmc_variable_structure_list);
   stat = update_ccmc_variable_attribute_value(
         "theta1",
         "actual_max",
         &theta_plus1_actual_max,
         baselined_ccmc_variable_structure_list);

   stat = update_ccmc_variable_attribute_value(
         "r1",
         "variable_size",
         &r_plus1_dimension_size,
         baselined_ccmc_variable_structure_list);
   stat = update_ccmc_variable_value(
         "r1",
         r_plus1_dimension_size,
         dim2_plus1,
         baselined_ccmc_variable_structure_list);
   stat = update_ccmc_variable_attribute_value(
         "r1",
         "actual_min",
         &r_plus1_actual_min,
         baselined_ccmc_variable_structure_list);
   stat = update_ccmc_variable_attribute_value(
         "r1",
         "actual_max",
         &r_plus1_actual_max,
         baselined_ccmc_variable_structure_list);

   stat = update_ccmc_variable_attribute_value(
         "T",
         "variable_size",
         &number_of_cells1,
         baselined_ccmc_variable_structure_list);
   stat = update_ccmc_variable_value(
         "T",
         number_of_cells1,
         t,
         baselined_ccmc_variable_structure_list);
   stat = update_ccmc_variable_attribute_value(
         "T",
         "actual_min",
         &t_actual_min,
         baselined_ccmc_variable_structure_list);
   stat = update_ccmc_variable_attribute_value(
         "T",
         "actual_max",
         &t_actual_max,
         baselined_ccmc_variable_structure_list);
   stat = update_ccmc_variable_attribute_value(
         "T",
         "grid_system",
         grid_system_4,
         baselined_ccmc_variable_structure_list);
   stat = update_ccmc_variable_attribute_value(
         "T",
         "data_grid_system",
         grid_system_4,
         baselined_ccmc_variable_structure_list);
   stat = update_ccmc_variable_attribute_value(
         "T",
         "position_grid_system",
         grid_system_4,
         baselined_ccmc_variable_structure_list);

   if (p != NULL){
     stat = update_ccmc_variable_attribute_value(
         "p",
         "variable_size",
         &number_of_cells1,
         baselined_ccmc_variable_structure_list);
     stat = update_ccmc_variable_value(
         "p",
         number_of_cells1,
         p,
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
         "p",
         "grid_system",
         grid_system_4,
         baselined_ccmc_variable_structure_list);
     stat = update_ccmc_variable_attribute_value(
         "p",
         "data_grid_system",
         grid_system_4,
         baselined_ccmc_variable_structure_list);
     stat = update_ccmc_variable_attribute_value(
         "p",
         "position_grid_system",
         grid_system_4,
         baselined_ccmc_variable_structure_list);
   }

   stat = update_ccmc_variable_attribute_value(
         "rho",
         "variable_size",
         &number_of_cells1,
         baselined_ccmc_variable_structure_list);
   stat = update_ccmc_variable_value(
         "rho",
         number_of_cells1,
         rho,
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
         "rho",
         "grid_system",
         grid_system_1,
         baselined_ccmc_variable_structure_list);
   stat = update_ccmc_variable_attribute_value(
         "rho",
         "data_grid_system",
         grid_system_1,
         baselined_ccmc_variable_structure_list);
   stat = update_ccmc_variable_attribute_value(
         "rho",
         "position_grid_system",
         grid_system_1,
         baselined_ccmc_variable_structure_list);

   stat = update_ccmc_variable_attribute_value(
         "ur",
         "variable_size",
         &number_of_cells2,
         baselined_ccmc_variable_structure_list);
   
   stat = update_ccmc_variable_value(
         "ur",
         number_of_cells2,
         vr,
         baselined_ccmc_variable_structure_list);
   
   stat = update_ccmc_variable_attribute_value(
         "ur",
         "actual_min",
         &vr_actual_min,
         baselined_ccmc_variable_structure_list);
   
   stat = update_ccmc_variable_attribute_value(
         "ur",
         "actual_max",
         &vr_actual_max,
         baselined_ccmc_variable_structure_list);
   
   stat = update_ccmc_variable_attribute_value(
         "ur",
         "grid_system",
         grid_system_2,
         baselined_ccmc_variable_structure_list);
   
   stat = update_ccmc_variable_attribute_value(
         "ur",
         "data_grid_system",
         grid_system_2,
         baselined_ccmc_variable_structure_list);
   
   stat = update_ccmc_variable_attribute_value(
         "ur",
         "position_grid_system",
         grid_system_2,
         baselined_ccmc_variable_structure_list);

   stat = update_ccmc_variable_attribute_value(
         "uphi",
         "variable_size",
         &number_of_cells1,
         baselined_ccmc_variable_structure_list);
   
   stat = update_ccmc_variable_value(
         "uphi",
         number_of_cells1,
         vp,
         baselined_ccmc_variable_structure_list);
   
   stat = update_ccmc_variable_attribute_value(
         "uphi",
         "actual_min",
         &vp_actual_min,
         baselined_ccmc_variable_structure_list);
   
   stat = update_ccmc_variable_attribute_value(
         "uphi",
         "actual_max",
         &vp_actual_max,
         baselined_ccmc_variable_structure_list);
   
   stat = update_ccmc_variable_attribute_value(
         "uphi",
         "grid_system",
         grid_system_5,
         baselined_ccmc_variable_structure_list);
   stat = update_ccmc_variable_attribute_value(
         "uphi",
         "data_grid_system",
         grid_system_5,
         baselined_ccmc_variable_structure_list);
   stat = update_ccmc_variable_attribute_value(
         "uphi",
         "position_grid_system",
         grid_system_5,
         baselined_ccmc_variable_structure_list);

   stat = update_ccmc_variable_attribute_value(
         "utheta",
         "variable_size",
         &number_of_cells3,
         baselined_ccmc_variable_structure_list);
   stat = update_ccmc_variable_value(
         "utheta",
         number_of_cells3,
         vt,
         baselined_ccmc_variable_structure_list);
   stat = update_ccmc_variable_attribute_value(
         "utheta",
         "actual_min",
         &vt_actual_min,
         baselined_ccmc_variable_structure_list);
   stat = update_ccmc_variable_attribute_value(
         "utheta",
         "actual_max",
         &vt_actual_max,
         baselined_ccmc_variable_structure_list);
   stat = update_ccmc_variable_attribute_value(
         "utheta",
         "grid_system",
         grid_system_3,
         baselined_ccmc_variable_structure_list);
   stat = update_ccmc_variable_attribute_value(
         "utheta",
         "data_grid_system",
         grid_system_3,
         baselined_ccmc_variable_structure_list);
   stat = update_ccmc_variable_attribute_value(
         "utheta",
         "position_grid_system",
         grid_system_3,
         baselined_ccmc_variable_structure_list);

   stat = update_ccmc_variable_attribute_value(
         "br",
         "variable_size",
         &number_of_cells3,
         baselined_ccmc_variable_structure_list);
   stat = update_ccmc_variable_value(
         "br",
         number_of_cells3,
         br,
         baselined_ccmc_variable_structure_list);
   stat = update_ccmc_variable_attribute_value(
         "br",
         "actual_min",
         &br_actual_min,
         baselined_ccmc_variable_structure_list);
   stat = update_ccmc_variable_attribute_value(
         "br",
         "actual_max",
         &br_actual_max,
         baselined_ccmc_variable_structure_list);
   stat = update_ccmc_variable_attribute_value(
         "br",
         "grid_system",
         grid_system_7,
         baselined_ccmc_variable_structure_list);
   stat = update_ccmc_variable_attribute_value(
         "br",
         "data_grid_system",
         grid_system_7,
         baselined_ccmc_variable_structure_list);
   stat = update_ccmc_variable_attribute_value(
         "br",
         "position_grid_system",
         grid_system_7,
         baselined_ccmc_variable_structure_list);

   stat = update_ccmc_variable_attribute_value(
         "bphi",
         "variable_size",
         &number_of_cells4,
         baselined_ccmc_variable_structure_list);
   stat = update_ccmc_variable_value(
         "bphi",
         number_of_cells4,
         bp,
         baselined_ccmc_variable_structure_list);
   stat = update_ccmc_variable_attribute_value(
         "bphi",
         "actual_min",
         &bp_actual_min,
         baselined_ccmc_variable_structure_list);
   stat = update_ccmc_variable_attribute_value(
         "bphi",
         "actual_max",
         &bp_actual_max,
         baselined_ccmc_variable_structure_list);
   stat = update_ccmc_variable_attribute_value(
         "bphi",
         "grid_system",
         grid_system_4,
         baselined_ccmc_variable_structure_list);
   stat = update_ccmc_variable_attribute_value(
         "bphi",
         "data_grid_system",
         grid_system_4,
         baselined_ccmc_variable_structure_list);
   stat = update_ccmc_variable_attribute_value(
         "bphi",
         "position_grid_system",
         grid_system_4,
         baselined_ccmc_variable_structure_list);

   stat = update_ccmc_variable_attribute_value(
         "btheta",
         "variable_size",
         &number_of_cells2,
         baselined_ccmc_variable_structure_list);
   stat = update_ccmc_variable_value(
         "btheta",
         number_of_cells2,
         bt,
         baselined_ccmc_variable_structure_list);
   stat = update_ccmc_variable_attribute_value(
         "btheta",
         "actual_min",
         &bt_actual_min,
         baselined_ccmc_variable_structure_list);
   stat = update_ccmc_variable_attribute_value(
         "btheta",
         "actual_max",
         &bt_actual_max,
         baselined_ccmc_variable_structure_list);
   stat = update_ccmc_variable_attribute_value(
         "btheta",
         "grid_system",
         grid_system_6,
         baselined_ccmc_variable_structure_list);
   stat = update_ccmc_variable_attribute_value(
         "btheta",
         "data_grid_system",
         grid_system_6,
         baselined_ccmc_variable_structure_list);
   stat = update_ccmc_variable_attribute_value(
         "btheta",
         "position_grid_system",
         grid_system_6,
         baselined_ccmc_variable_structure_list);

   stat = update_ccmc_variable_attribute_value(
         "jr",
         "variable_size",
         &number_of_cells2,
         baselined_ccmc_variable_structure_list);
   stat = update_ccmc_variable_value(
         "jr",
         number_of_cells2,
         jr,
         baselined_ccmc_variable_structure_list);
   stat = update_ccmc_variable_attribute_value(
         "jr",
         "actual_min",
         &jr_actual_min,
         baselined_ccmc_variable_structure_list);
   stat = update_ccmc_variable_attribute_value(
         "jr",
         "actual_max",
         &jr_actual_max,
         baselined_ccmc_variable_structure_list);
   stat = update_ccmc_variable_attribute_value(
         "jr",
         "grid_system",
         grid_system_2,
         baselined_ccmc_variable_structure_list);
   stat = update_ccmc_variable_attribute_value(
         "jr",
         "data_grid_system",
         grid_system_2,
         baselined_ccmc_variable_structure_list);
   stat = update_ccmc_variable_attribute_value(
         "jr",
         "position_grid_system",
         grid_system_2,
         baselined_ccmc_variable_structure_list);

   stat = update_ccmc_variable_attribute_value(
         "jphi",
         "variable_size",
         &number_of_cells1,
         baselined_ccmc_variable_structure_list);
   stat = update_ccmc_variable_value(
         "jphi",
         number_of_cells1,
         jp,
         baselined_ccmc_variable_structure_list);
   stat = update_ccmc_variable_attribute_value(
         "jphi",
         "actual_min",
         &jp_actual_min,
         baselined_ccmc_variable_structure_list);
   stat = update_ccmc_variable_attribute_value(
         "jphi",
         "actual_max",
         &jp_actual_max,
         baselined_ccmc_variable_structure_list);
   stat = update_ccmc_variable_attribute_value(
         "jphi",
         "grid_system",
         grid_system_5,
         baselined_ccmc_variable_structure_list);
   stat = update_ccmc_variable_attribute_value(
         "jphi",
         "data_grid_system",
         grid_system_5,
         baselined_ccmc_variable_structure_list);
   stat = update_ccmc_variable_attribute_value(
         "jphi",
         "position_grid_system",
         grid_system_5,
         baselined_ccmc_variable_structure_list);

   stat = update_ccmc_variable_attribute_value(
         "jtheta",
         "variable_size",
         &number_of_cells3,
         baselined_ccmc_variable_structure_list);
   stat = update_ccmc_variable_value(
         "jtheta",
         number_of_cells3,
         jt,
         baselined_ccmc_variable_structure_list);
   stat = update_ccmc_variable_attribute_value(
         "jtheta",
         "actual_min",
         &jt_actual_min,
         baselined_ccmc_variable_structure_list);
   stat = update_ccmc_variable_attribute_value(
         "jtheta",
         "actual_max",
         &jt_actual_max,
         baselined_ccmc_variable_structure_list);
   stat = update_ccmc_variable_attribute_value(
         "jtheta",
         "grid_system",
         grid_system_3,
         baselined_ccmc_variable_structure_list);
   stat = update_ccmc_variable_attribute_value(
         "jtheta",
         "data_grid_system",
         grid_system_3,
         baselined_ccmc_variable_structure_list);
   stat = update_ccmc_variable_attribute_value(
         "jtheta",
         "position_grid_system",
         grid_system_3,
         baselined_ccmc_variable_structure_list);

   if (verbose_flag)
   {
      printf("cleaning memory allocated for initial read routine...\n");
   }

   free(dim2);
   free(dim1);
   free(dim0);
   free(dim2_plus1);
   free(dim1_plus1);
   free(dim0_plus1);
   free(t);
   free(p);
   free(rho);
   free(vr);
   free(vt);
   free(vp);
   free(jr);
   free(jp);
   free(jt);
   free(br);
   free(bp);
   free(bt);

   return 0;

}
