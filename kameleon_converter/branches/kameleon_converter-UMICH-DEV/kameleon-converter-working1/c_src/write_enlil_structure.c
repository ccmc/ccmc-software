/*******************************************************************************
 *                                                                             *
 *    NAME:          write_enlil_structure.c                                   *
 *                                                                             *
 *    VERSION:       1.0.0                                                     *
 *                                                                             *
 *    AUTHOR:        Marlo M. H. Maddox                                        *
 *                   NASA-GSFC-AETD-ISD-ADMA(587)                              *
 *                   Marlo.Maddox@nasa.gov                                     *
 *                   (301) 286 - 5202                                          *
 *                                                                             *
 *                                                                             *
 *    PURPOSE:       specialized write routine that writes enlil data from     *
 *                   main memory into the                                      *
 *                   baselined_ccmc_attribute_structure_list &                 *
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
 *    MODIFICATION                                                             *
 *    HISTORY:                                                                 *
 *                                                                             *
 *    04.19.2006     Maddox, Marlo                                             *
 *                   Initial Development Started                               *
 *                                                                             *
 *                   Uncommented attribute update for                          *
 *                   grid_system_1_dimension_4 line                            *
 *                                                                             *
 *                   Fixing attribute typos...                                 *
 *                                                                             *
 *                   Setting Units...                                          *
 *                                                                             *
 *    08.29.2006     Maddox, Marlo - somehow this chabge was lost between      *
 *    10.17.2006     v4.3.0 and v4.3.1 ( and the working copy as of 10.17.2006 *
 *                   ...) Adding b1r, b1phi, b1theta to enlil_structure to     *
 *                   correct polarity of br,bphi,btheta using bp               *
 *                                                                             *
 *    10.19.2006     Maddox, Marlo                                             *
 *                   changed the ev* time values from attributes to variables  *
 *                   using the appropriate metadata_structure toolkit routines *
 *                                                                             *
 *                   added a linear_min_max_search_int to search the           *
 *                   numerical_step array                                      *
 *                                                                             *
 *                   added the DEBUG_FLAG macro                                *
 *                                                                             *
 *    10.23.2006     added some default enlil global attribute defineitions    *
 *                                                                             *
 *                   modified some of the default attribute values             *
 *                                                                             *
 *    02.06.2007     Maddox, Marlo                                             *
 *                   changing ccmc_metadata_structure.h to                     *
 *                   ccmc_structure_manager.h                                  *
 *                                                                             *
 *    02.16.2007     Maddox, Marlo                                             *
 *                   removed MAX_NEW_FILE_NAME_LENGTH macro, not used here as  *
 *                   naming is done in the write_structures_cdf() function     *
 *                                                                             *
 *    05.16.2007     Maddox, Marlo                                             *
 *                   Changing Variables to reflect new ENLIL variable          *
 *                   descriptions .ie x->r, y->phi, z->theta including vector  *
 *                   components b*,u*                                          *
 *                                                                             *
 *    05.17.2007     Maddox, Marlo                                             *
 *                   adding update for kameleon_version gattribute             *
 *                                                                             *
 *    05.05.2011     Maddox, Marlo                                             *
 *                   Modifying ENLIL Coordinate system order names             *
 ******************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <math.h>

#include "read_database.h"

#include "cdf.h"

#include "read_enlil.h"
#include "enlil_attributes.h"

#include "ccmc_attributes.h"
#include "ccmc_variable_attributes.h"
#include "ccmc_registered_variables.h"
#include "ccmc_structure_manager.h"

#include "kameleon_version_info.h"

#define NUMBER_GLOBAL_ATTR NUMBER_CCMC_ATTR + NUMBER_ENLIL_ATTR
#define NUMBER_VARIABLE_ATTR NUMBER_CCMC_VARIABLE_ATTR + NUMBER_ENLIL_VARIABLE_ATTR

#define DEBUG_FLAG 0

int write_enlil_structure(char *input_file_name, char *data_format,
        int file_name_length, int verbose_flag, char *output_directory,
        char *model_name, char *original_working_directory,
        int write_database_info_flag)
{

    extern int linear_minmax_search(float *, int, float *, float *);
    extern int linear_minmax_search_int(int *, int, int *, int *);
    extern int update_ccmc_variable_attribute_value(char *, char *, void *,
            struct ccmc_variable_structure *);

    int i, j, k, stat, local_size, number_of_cells;

    float actual_min, actual_max;
    int actual_min_int, actual_max_int;

    float zero = 0.0;

    float *temp_b_buffer;

    double cdf_epoch_start_time_value, cdf_epoch_dipole_time_value;

    /* Define Static ccmc attribute values for ENLIL Model */

    char *kameleon_version_num= KAMELEON_VERSION;

    char *readme_text =
            "ENLIL is a time-dependent 3D MHD model of the heliosphere. It "
                "solves for plasma mass, momentum and energy density, and "
                "magnetic field, using a Flux-Corrected-Transport (FCT) "
                "algorithm. Its inner radial boundary is located beyond the sonic"
                " point, typically at 21.5 or 30 solar radii. It can accept "
                "boundary condition information from either the WSA or MAS "
                "models. The outer radial boundary can be adjusted to include "
                "planets or spacecraft of interest (eg 2 AU to include both Earth"
                " and Mars, 5 AU to include Ulysses, 10 AU to include Cassini). "
                "It often covers 60 degrees north to 60 degrees south in latitude"
                " and 360 degrees in azimuth.********************************* "
                "NOTES:  There are two types of ENLIL output data that may be "
                "provided in this file - spatial distribution data and temporal "
                "evolution data.  The main data is the spatial distribution data "
                "which originally resides in a file of the name tim.<rrrr>.nc ( "
                "where rrrr is a 4 digit identification number of the time level."
                "  The other type of data, the temporal evolution data can exists"
                " for 4 different types of evolutions: (1) positions in the "
                "heliosphere - originally evh.nc files (2) positions on the "
                "Sun-Earth Line - originally evl.nc files (3) geospace positions "
                "- originally evg.nc files (4) planetary positions - originally "
                "evp.nc files.  ********************************* GLOBAL "
                "Attributes: In additional to the global attributes that apply to"
                " all data found in this file, specific sub-global attributes "
                "have the naming convention of a pre-pended string: time_, evh_, "
                "evl_, evg_, or evp_   depending on what attributes corresponds "
                "with what type of output.  For example, each of the originally "
                "separate data files would normally contain an attribute named "
                "parameters.  In this consolidated file, to access the parameters"
                " attribute for evh data you would look for the attribute named "
                "evh_parameters.*********************************The Grid & "
                "Variables:  tim files originally have two grids associated with "
                "them: x1,x2,x3 ( cell centers ) & x1h,x2h,x3h ( cell interfaces "
                ").  These two grids are now grid_system_1 -> r,theta,phi & "
                "grid_system_2 -> r1,theta1,phi1.  Variables for the spatial "
                "distribution type tim files do NOT have an underscore and number "
                "appended to them ie. rho, br,bphi,btheta,ur,uphi,utheta..."
                "******************for the temporal evolution data we have the "
                "following:  evh data is on grid_system_3 -> r2,theta2, phi2 with "
                "variables of the name *_1 ie. rho_1, br_1, btheta_1, bphi_1....."
                ".evl data is on grid_system_4 -> r3,theta3, phi3 with variables "
                "of the name *_2 ie. rho_2, br_2,btheta_2, bphi_2......evg "
                "data is on grid_system_5 -> r4,theta4, phi4 with variables of the"
                " name *_3 ie. rho_3, br_3,btheta_3, bphi_3......evp data is on "
                "grid_system_5 -> r4,theta4, phi4 with variables of the name *_4 "
                "ie. rho_4, br_4,btheta_4, bphi_4";

    char *model_type = "Heliosphere";
    char *terms_of_usage_text =
            "For tracking purposes for our government sponsors, we ask that "
                "you notify the CCMC whenever you use CCMC results in scientific "
                "publications or presentations: ccmc@ccmc.gsfc.nasa.gov ";

    char *output_type = "Heliosphere";
    char *standard_grid_target = "HNM";
    char *default_grid_type = "HNM";

    /* set not_applicable string for use when dipole time makes no sense */

    char *not_applicable = "N/A";
    float radius_m_valid_min = 13920000;
    float radius_m_valid_max = 1.4939e12;
    float radian_valid_min = 0;
    float radian_valid_max = 6.28318531;
    float mag_field_T_valid_min = -1;
    float mag_field_T_valid_max = 1;
    float polarity_valid_min = -1.001;
    float polarity_valid_max = 1.001;

    /* Define Static ccmc variable attribute values for ENLIL Model */

    char *b1r_description =
            "R magnetic field component generated from br and bp.  b1r "
                "corrects the polarity of br which may have been fliped due to "
                "the anti-reconenction feature of advection.  This value is "
                "essentailly calculated bphi multiplying the sign of bp * br.";

    char *b1theta_description =
                "THETA magnetic field component generated from btheta and bp.  "
                    "b1theta corrects the polarity of btheta which may have been "
                    "fliped due to the anti-reconenction feature of advection.  "
                    "This value is essentailly calculated by multiplying the sign of "
                    "bp * btheta.";

    char *b1phi_description =
            "PHI magnetic field component generated from bphi and bp.  "
                "b1phi corrects the polarity of bphi which may have been fliped "
                "due to the anti-reconenction feature of advection.  This value "
                "is essentailly calculated by multiplying the sign of bp * bphi.";


    int grid_system_count = 6;

    /** grid system one for time file x1,x2,x3 **/
    char *grid_system_1 = "[ r, theta, phi ]";

    /** grid system two for time file x1h,x2h,x3h **/
    char *grid_system_2 = "[ r1, theta1, phi1 ]";

    /** grid system three for evh files **/
    char *grid_system_3 = "[ r2, theta2, phi2 ]";

    /** grid system four for evl files **/
    char *grid_system_4 = "[ r3, theta3, phi3 ]";

    /** grid system five for evg files **/
    char *grid_system_5 = "[ r4, theta4, phi4 ]";

    /** grid system six for evp files **/
    char *grid_system_6 = "[ r5, theta5, phi5 ]";

    char *evh_grid_system = "[time||time_step||numerical_step, grid_system_3]";
    char *evl_grid_system = "[time||time_step||numerical_step, grid_system_4]";
    char *evg_grid_system = "[time|time_step||numerical_step, grid_system_5]";
    char *evp_grid_system = "[time||time_step||numerical_step, grid_system_6]";

    /* DETERMINE GRID TYPES - use DatabaseInfo field cs_output or default grid
    * target if database info isn't available*/

    char *grid_1_type;
    char *grid_2_type;
    char *grid_3_type;
    char *grid_4_type;
    char *grid_5_type;
    char *grid_6_type;

    /* ENLIL's Units are different from the default variable units - lets
    * define then here */

    char *meters = "m";
    char *radians = "rad";
    char *seconds = "s";
    char *mass_density = "kg/m^3";
    char *velocity = "m/s";
    char *magnetic_field = "T";

    /* set grid_type based on database info if available */

    if (strlen(cs_output) > 1)
    {
        grid_1_type = ( char * ) malloc(strlen(cs_output) + 1);
        grid_2_type = ( char * ) malloc(strlen(cs_output) + 1);
        grid_3_type = ( char * ) malloc(strlen(cs_output) + 1);
        grid_4_type = ( char * ) malloc(strlen(cs_output) + 1);
        grid_5_type = ( char * ) malloc(strlen(cs_output) + 1);
        grid_6_type = ( char * ) malloc(strlen(cs_output) + 1);
        strcpy(grid_1_type, cs_output);
        strcpy(grid_2_type, cs_output);
        strcpy(grid_3_type, cs_output);
        strcpy(grid_4_type, cs_output);
        strcpy(grid_5_type, cs_output);
        strcpy(grid_6_type, cs_output);
    }
    else
    {
        printf("\nWARNING:\tcoordinate system could not be identified through "
            "DatabaseInfo file.  Using default: %s\n", default_grid_type);
        grid_1_type = ( char * ) malloc(strlen(default_grid_type) + 1);
        grid_2_type = ( char * ) malloc(strlen(default_grid_type) + 1);
        grid_3_type = ( char * ) malloc(strlen(default_grid_type) + 1);
        grid_4_type = ( char * ) malloc(strlen(default_grid_type) + 1);
        grid_5_type = ( char * ) malloc(strlen(default_grid_type) + 1);
        grid_6_type = ( char * ) malloc(strlen(default_grid_type) + 1);

        strcpy(grid_1_type, default_grid_type);
        strcpy(grid_2_type, default_grid_type);
        strcpy(grid_3_type, default_grid_type);
        strcpy(grid_4_type, default_grid_type);
        strcpy(grid_5_type, default_grid_type);
        strcpy(grid_6_type, default_grid_type);
    }

    /****************************************************************************
    *                                CREATE THE GLOBAL ATTRIBUTES              *
    ***************************************************************************/

    /*********************************** GLOBAL ATTRIBUTES *********************/

    /****************************************************************************
    *    initialize the baselined_ccmc_attribute_structure_list to hold n      *
    *    number of cccmc_attribute_structures where n = NUMBER_GLOBAL_ATTR =   *
    *    NUMBER_CCMC_ATTR + NUMBER_ENLIL_ATTR as defined in ccmc_attributes.h  *
    *    & enlil_attributes.h                                                  *
    ***************************************************************************/

    baselined_ccmc_attribute_structure_list
            = init_ccmc_attribute_structure( NUMBER_GLOBAL_ATTR);

    /* for each name-data type pair in the ccmc_attribute_name_type_list, call
    * put_ccmc_attribute with attr name & data type since we are writing from
    * the ccmc_attribute_name_type_list, the attribute_type = global ***/

    for (i=0; i < NUMBER_CCMC_ATTR; i++)
    {
        put_ccmc_attribute(ccmc_attribute_name_type_list[i].ccmc_attr_name,
                "global", ccmc_attribute_name_type_list[i].ccmc_attr_data_type,
                "", baselined_ccmc_attribute_structure_list);
    }

    /* for each name-data type pair in the enlil_attribute_name_type_list, call
    * put_ccmc_attribute with attr name & data type since we are writing from
    * the enlil_attribute_name_type_list, the attribute_type = model
    */

    for (i=0; i < NUMBER_ENLIL_ATTR; i++)
    {
        put_ccmc_attribute(enlil_attribute_name_type_list[i].enlil_attr_name,
                "model",
                enlil_attribute_name_type_list[i].enlil_attr_data_type, "",
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

    stat = update_ccmc_attribute_value("README", readme_text,
            baselined_ccmc_attribute_structure_list);
    stat = update_ccmc_attribute_value("model_name", model_name,
            baselined_ccmc_attribute_structure_list);
    stat = update_ccmc_attribute_value("model_type", model_type,
            baselined_ccmc_attribute_structure_list);
    stat = update_ccmc_attribute_value("original_output_file_name",
            input_file_name, baselined_ccmc_attribute_structure_list);
    stat = update_ccmc_attribute_value("terms_of_usage", terms_of_usage_text,
            baselined_ccmc_attribute_structure_list);
    stat = update_ccmc_attribute_value("grid_system_count", &grid_system_count,
            baselined_ccmc_attribute_structure_list);
    stat = update_ccmc_attribute_value("grid_system_1_number_of_dimensions",
            &tim_number_of_dimensions, baselined_ccmc_attribute_structure_list);
    stat = update_ccmc_attribute_value("grid_system_1_dimension_1_size",
            &tim_n1, baselined_ccmc_attribute_structure_list);
    stat = update_ccmc_attribute_value("grid_system_1_dimension_2_size",
            &tim_n2, baselined_ccmc_attribute_structure_list);
    stat = update_ccmc_attribute_value("grid_system_1_dimension_3_size",
            &tim_n3, baselined_ccmc_attribute_structure_list);
    stat
            = update_ccmc_attribute_value("grid_system_1_dimension_4_size",
                    &tim_number_of_blocks_nblk,
                    baselined_ccmc_attribute_structure_list);
    stat = update_ccmc_attribute_value("grid_system_1", grid_system_1,
            baselined_ccmc_attribute_structure_list);
    stat = update_ccmc_attribute_value("output_type", output_type,
            baselined_ccmc_attribute_structure_list);
    stat = update_ccmc_attribute_value("standard_grid_target",
            standard_grid_target, baselined_ccmc_attribute_structure_list);
    stat = update_ccmc_attribute_value("grid_1_type", grid_1_type,
            baselined_ccmc_attribute_structure_list);
    stat = update_ccmc_attribute_value("kameleon_version",
            kameleon_version_num, baselined_ccmc_attribute_structure_list);

    /****** individual enlil global attributes set by name using ENLIL values
    * already in main memory *****/

    /******************************************
    *                 TIME FILE                *
    *****************************************/

    if ( DEBUG_FLAG)
        printf("DEBUG\twritting time file attributes\n");

    stat = update_ccmc_attribute_value("grid_system_2", grid_system_2,
            baselined_ccmc_attribute_structure_list);
    stat = update_ccmc_attribute_value("grid_2_type", grid_2_type,
            baselined_ccmc_attribute_structure_list);
    stat = update_ccmc_attribute_value("grid_system_2_number_of_dimensions",
            &tim_number_of_dimensions, baselined_ccmc_attribute_structure_list);
    stat = update_ccmc_attribute_value("grid_system_2_dimension_1_size",
            &tim_n1h, baselined_ccmc_attribute_structure_list);
    stat = update_ccmc_attribute_value("grid_system_2_dimension_2_size",
            &tim_n2h, baselined_ccmc_attribute_structure_list);
    stat = update_ccmc_attribute_value("grid_system_2_dimension_3_size",
            &tim_n3h, baselined_ccmc_attribute_structure_list);
    stat
            = update_ccmc_attribute_value("grid_system_2_dimension_4_size",
                    &tim_number_of_blocks_nblk,
                    baselined_ccmc_attribute_structure_list);

    stat = update_ccmc_attribute_value("time_physical_time",
            tim_physical_time_TIME, baselined_ccmc_attribute_structure_list);
    stat = update_ccmc_attribute_value("time_physical_time_step",
            tim_physical_time_step_DT, baselined_ccmc_attribute_structure_list);
    stat = update_ccmc_attribute_value("time_numerical_time_step",
            tim_numerical_time_step_NSTEP,
            baselined_ccmc_attribute_structure_list);

    if ( DEBUG_FLAG)
    {
        printf("\n\n\n%s\ttime_physical_time_step = %f\ntime_type = %s\n\n"
            "\n",__FILE__ , *tim_physical_time_step_DT, time_gattribute_type );
    }

    stat = update_ccmc_attribute_value( "time_type", time_gattribute_type,
    baselined_ccmc_attribute_structure_list);

    stat = update_ccmc_attribute_value( "time_title", time_gattribute_title,
    baselined_ccmc_attribute_structure_list);

    stat = update_ccmc_attribute_value( "time_name", time_gattribute_name,
    baselined_ccmc_attribute_structure_list);

    stat = update_ccmc_attribute_value( "time_project",time_gattribute_project,
    baselined_ccmc_attribute_structure_list);

    stat = update_ccmc_attribute_value( "time_initial", time_gattribute_initial,
   baselined_ccmc_attribute_structure_list);

   stat = update_ccmc_attribute_value( "time_resume", time_gattribute_resume,
   baselined_ccmc_attribute_structure_list);

   stat = update_ccmc_attribute_value( "time_boundary", time_gattribute_boundary,
      baselined_ccmc_attribute_structure_list);

   stat = update_ccmc_attribute_value( "time_passage", time_gattribute_passage,
      baselined_ccmc_attribute_structure_list);

   stat = update_ccmc_attribute_value( "time_grid", time_gattribute_grid,
      baselined_ccmc_attribute_structure_list);

 stat = update_ccmc_attribute_value( "time_geometry", time_gattribute_geometry,
      baselined_ccmc_attribute_structure_list);

 stat = update_ccmc_attribute_value( "time_code", time_gattribute_code,
      baselined_ccmc_attribute_structure_list);

stat = update_ccmc_attribute_value( "time_parameters",
      time_gattribute_parameters, baselined_ccmc_attribute_structure_list);

stat = update_ccmc_attribute_value( "time_refdate_mjd",
      time_gattribute_refdate_mjd, baselined_ccmc_attribute_structure_list);

stat = update_ccmc_attribute_value( "time_gamma",
      time_gattribute_gamma, baselined_ccmc_attribute_structure_list);

stat = update_ccmc_attribute_value( "time_xalpha",
      time_gattribute_xalpha, baselined_ccmc_attribute_structure_list);

stat = update_ccmc_attribute_value( "time_history",
      time_gattribute_history, baselined_ccmc_attribute_structure_list);

/******************************************
 *                 EVH FILE                *
 *****************************************/


if( DEBUG_FLAG ) printf("DEBUG\twritting evh file attributes\n");


stat = update_ccmc_attribute_value( "grid_system_3", grid_system_3,
      baselined_ccmc_attribute_structure_list );

stat = update_ccmc_attribute_value( "grid_3_type", grid_3_type,
      baselined_ccmc_attribute_structure_list );

stat = update_ccmc_attribute_value( "grid_system_3_number_of_dimensions",
      &evh_number_of_dimensions, baselined_ccmc_attribute_structure_list);

stat = update_ccmc_attribute_value( "grid_system_3_dimension_1_size",
      &evh_nhel, baselined_ccmc_attribute_structure_list);

stat = update_ccmc_attribute_value( "grid_system_3_dimension_2_size",
      &evh_nevo, baselined_ccmc_attribute_structure_list);

stat = update_ccmc_attribute_value( "evh_type", evh_gattribute_type,
      baselined_ccmc_attribute_structure_list);

stat = update_ccmc_attribute_value( "evh_title", evh_gattribute_title,
      baselined_ccmc_attribute_structure_list);

stat = update_ccmc_attribute_value( "evh_name", evh_gattribute_name,
      baselined_ccmc_attribute_structure_list);

stat = update_ccmc_attribute_value( "evh_project", evh_gattribute_project,
      baselined_ccmc_attribute_structure_list);

stat = update_ccmc_attribute_value( "evh_initial", evh_gattribute_initial,
      baselined_ccmc_attribute_structure_list);

stat = update_ccmc_attribute_value( "evh_resume", evh_gattribute_resume,
      baselined_ccmc_attribute_structure_list);

stat = update_ccmc_attribute_value( "evh_boundary", evh_gattribute_boundary,
      baselined_ccmc_attribute_structure_list);

stat = update_ccmc_attribute_value( "evh_passage", evh_gattribute_passage,
      baselined_ccmc_attribute_structure_list);

stat = update_ccmc_attribute_value( "evh_grid", evh_gattribute_grid,
      baselined_ccmc_attribute_structure_list);

stat = update_ccmc_attribute_value( "evh_geometry", evh_gattribute_geometry,
      baselined_ccmc_attribute_structure_list);

stat = update_ccmc_attribute_value( "evh_code", evh_gattribute_code,
      baselined_ccmc_attribute_structure_list);

stat = update_ccmc_attribute_value( "evh_parameters", evh_gattribute_parameters,
      baselined_ccmc_attribute_structure_list);

stat = update_ccmc_attribute_value( "evh_refdate_mjd",
      evh_gattribute_refdate_mjd, baselined_ccmc_attribute_structure_list);

stat = update_ccmc_attribute_value( "evh_gamma", evh_gattribute_gamma,
      baselined_ccmc_attribute_structure_list);

stat = update_ccmc_attribute_value( "evh_xalpha", evh_gattribute_xalpha,
      baselined_ccmc_attribute_structure_list);

stat = update_ccmc_attribute_value( "evh_history", evh_gattribute_history,
      baselined_ccmc_attribute_structure_list);

/******************************************
 *                 EVL FILE                *
 *****************************************/


if( DEBUG_FLAG ) printf("DEBUG\twritting evl file attributes\n");

stat = update_ccmc_attribute_value( "grid_system_4", grid_system_4,
      baselined_ccmc_attribute_structure_list );

stat = update_ccmc_attribute_value( "grid_4_type", grid_4_type,
      baselined_ccmc_attribute_structure_list );

stat = update_ccmc_attribute_value( "grid_system_4_number_of_dimensions",
      &evl_number_of_dimensions, baselined_ccmc_attribute_structure_list);

stat = update_ccmc_attribute_value( "grid_system_4_dimension_1_size",
      &evl_nsel, baselined_ccmc_attribute_structure_list);

stat = update_ccmc_attribute_value( "grid_system_4_dimension_2_size",
      &evl_nevo, baselined_ccmc_attribute_structure_list);

stat = update_ccmc_attribute_value( "evl_type", evl_gattribute_type,
      baselined_ccmc_attribute_structure_list);

stat = update_ccmc_attribute_value( "evl_title", evl_gattribute_title,
      baselined_ccmc_attribute_structure_list);

stat = update_ccmc_attribute_value( "evl_name", evl_gattribute_name,
      baselined_ccmc_attribute_structure_list);

stat = update_ccmc_attribute_value( "evl_project", evl_gattribute_project,
      baselined_ccmc_attribute_structure_list);

stat = update_ccmc_attribute_value( "evl_initial", evl_gattribute_initial,
      baselined_ccmc_attribute_structure_list);

stat = update_ccmc_attribute_value( "evl_resume", evl_gattribute_resume,
      baselined_ccmc_attribute_structure_list);

stat = update_ccmc_attribute_value( "evl_boundary", evl_gattribute_boundary,
      baselined_ccmc_attribute_structure_list);

stat = update_ccmc_attribute_value( "evl_passage", evl_gattribute_passage,
      baselined_ccmc_attribute_structure_list);

stat = update_ccmc_attribute_value( "evl_grid", evl_gattribute_grid,
      baselined_ccmc_attribute_structure_list);

stat = update_ccmc_attribute_value( "evl_geometry", evl_gattribute_geometry,
      baselined_ccmc_attribute_structure_list);

stat = update_ccmc_attribute_value( "evl_code", evl_gattribute_code,
      baselined_ccmc_attribute_structure_list);

stat = update_ccmc_attribute_value( "evl_parameters", evl_gattribute_parameters,
      baselined_ccmc_attribute_structure_list);

stat = update_ccmc_attribute_value( "evl_refdate_mjd",
      evl_gattribute_refdate_mjd, baselined_ccmc_attribute_structure_list);

stat = update_ccmc_attribute_value( "evl_gamma", evl_gattribute_gamma,
      baselined_ccmc_attribute_structure_list);

stat = update_ccmc_attribute_value( "evl_history", evl_gattribute_history,
      baselined_ccmc_attribute_structure_list);


/******************************************
 *                 EVG FILE                *
 *****************************************/


if( DEBUG_FLAG ) printf("DEBUG\twritting evg file attributes\n");


stat = update_ccmc_attribute_value( "grid_system_5", grid_system_5,
        baselined_ccmc_attribute_structure_list );
stat = update_ccmc_attribute_value( "grid_5_type", grid_5_type,
        baselined_ccmc_attribute_structure_list );
stat = update_ccmc_attribute_value( "grid_system_5_number_of_dimensions",
        &evg_number_of_dimensions, baselined_ccmc_attribute_structure_list);
stat = update_ccmc_attribute_value( "grid_system_5_dimension_1_size",
        &evg_ngeo, baselined_ccmc_attribute_structure_list);
stat = update_ccmc_attribute_value( "grid_system_5_dimension_2_size",
        &evg_nevo, baselined_ccmc_attribute_structure_list);



stat = update_ccmc_attribute_value( "evg_type", evg_gattribute_type,
        baselined_ccmc_attribute_structure_list);
stat = update_ccmc_attribute_value( "evg_title", evg_gattribute_title,
        baselined_ccmc_attribute_structure_list);
stat = update_ccmc_attribute_value( "evg_name", evg_gattribute_name,
        baselined_ccmc_attribute_structure_list);
stat = update_ccmc_attribute_value( "evg_project", evg_gattribute_project,
        baselined_ccmc_attribute_structure_list);
stat = update_ccmc_attribute_value( "evg_initial", evg_gattribute_initial,
        baselined_ccmc_attribute_structure_list);
stat = update_ccmc_attribute_value( "evg_resume", evg_gattribute_resume,
        baselined_ccmc_attribute_structure_list);
stat = update_ccmc_attribute_value( "evg_boundary", evg_gattribute_boundary,
        baselined_ccmc_attribute_structure_list);
stat = update_ccmc_attribute_value( "evg_passage", evg_gattribute_passage,
        baselined_ccmc_attribute_structure_list);
stat = update_ccmc_attribute_value( "evg_grid", evg_gattribute_grid,
        baselined_ccmc_attribute_structure_list);
stat = update_ccmc_attribute_value( "evg_geometry", evg_gattribute_geometry,
        baselined_ccmc_attribute_structure_list);
stat = update_ccmc_attribute_value( "evg_code", evg_gattribute_code,
        baselined_ccmc_attribute_structure_list);
stat = update_ccmc_attribute_value( "evg_parameters", evg_gattribute_parameters,
        baselined_ccmc_attribute_structure_list);
stat = update_ccmc_attribute_value( "evg_refdate_mjd",
        evg_gattribute_refdate_mjd, baselined_ccmc_attribute_structure_list);
stat = update_ccmc_attribute_value( "evg_gamma", evg_gattribute_gamma,
        baselined_ccmc_attribute_structure_list);
stat = update_ccmc_attribute_value( "evg_xalpha", evg_gattribute_xalpha,
        baselined_ccmc_attribute_structure_list);
stat = update_ccmc_attribute_value( "evg_history", evg_gattribute_history,
        baselined_ccmc_attribute_structure_list);

/******************************************
 *                 EVP FILE               *
 *****************************************/


if( DEBUG_FLAG ) printf("DEBUG\twritting evp file attributes\n");


stat = update_ccmc_attribute_value( "grid_system_6", grid_system_6,
        baselined_ccmc_attribute_structure_list );
stat = update_ccmc_attribute_value( "grid_6_type", grid_6_type,
        baselined_ccmc_attribute_structure_list );
stat = update_ccmc_attribute_value( "grid_system_6_number_of_dimensions",
        &evp_number_of_dimensions, baselined_ccmc_attribute_structure_list);
stat = update_ccmc_attribute_value( "grid_system_6_dimension_1_size",
        &evp_npla, baselined_ccmc_attribute_structure_list);
stat = update_ccmc_attribute_value( "grid_system_6_dimension_2_size",
        &evp_nevo, baselined_ccmc_attribute_structure_list);



stat = update_ccmc_attribute_value( "evp_type", evp_gattribute_type,
        baselined_ccmc_attribute_structure_list);
stat = update_ccmc_attribute_value( "evp_title", evp_gattribute_title,
        baselined_ccmc_attribute_structure_list);
stat = update_ccmc_attribute_value( "evp_name", evp_gattribute_name,
        baselined_ccmc_attribute_structure_list);
stat = update_ccmc_attribute_value( "evp_project", evp_gattribute_project,
        baselined_ccmc_attribute_structure_list);
stat = update_ccmc_attribute_value( "evp_initial", evp_gattribute_initial,
        baselined_ccmc_attribute_structure_list);
stat = update_ccmc_attribute_value( "evp_resume", evp_gattribute_resume,
        baselined_ccmc_attribute_structure_list);
stat = update_ccmc_attribute_value( "evp_boundary", evp_gattribute_boundary,
        baselined_ccmc_attribute_structure_list);
stat = update_ccmc_attribute_value( "evp_passage", evp_gattribute_passage,
        baselined_ccmc_attribute_structure_list);
stat = update_ccmc_attribute_value( "evp_grid", evp_gattribute_grid,
        baselined_ccmc_attribute_structure_list);
stat = update_ccmc_attribute_value( "evp_geometry", evp_gattribute_geometry,
        baselined_ccmc_attribute_structure_list);
stat = update_ccmc_attribute_value( "evp_code", evp_gattribute_code,
        baselined_ccmc_attribute_structure_list);
stat = update_ccmc_attribute_value( "evp_parameters", evp_gattribute_parameters,
        baselined_ccmc_attribute_structure_list);
stat = update_ccmc_attribute_value( "evp_refdate_mjd",
        evp_gattribute_refdate_mjd, baselined_ccmc_attribute_structure_list);
stat = update_ccmc_attribute_value( "evp_gamma", evp_gattribute_gamma,
        baselined_ccmc_attribute_structure_list);
stat = update_ccmc_attribute_value( "evp_xalpha", evp_gattribute_xalpha,
        baselined_ccmc_attribute_structure_list);
stat = update_ccmc_attribute_value( "evp_history", evp_gattribute_history,
        baselined_ccmc_attribute_structure_list);

/* add attributes read in from DatabaseInfo file using read_database.c code */



    /****** ONLY add if write_database_info_flag = 1 ********/

    if( write_database_info_flag )
    {

    /*** check to see if each variable has been set ie. strlen( variable ) > 0 ,
     *  otherwise CDF exits with error ********/

    if( strlen( run_registration_number ) )
    {
        stat = update_ccmc_attribute_value( "run_registration_number",
                run_registration_number, baselined_ccmc_attribute_structure_list );
    }
    if( strlen( request_date ) )
    {
        stat = update_ccmc_attribute_value( "generation_date",
                request_date_cdf_epoch3, baselined_ccmc_attribute_structure_list );
        }
    if( strlen( generated_by ) )
    {
        stat = update_ccmc_attribute_value( "generated_by", generated_by,
                baselined_ccmc_attribute_structure_list );
    }
    if( strlen( start_time ) )
    {
        stat = update_ccmc_attribute_value( "start_time", start_time_cdf_epoch3,
                baselined_ccmc_attribute_structure_list );
    }
    if( strlen( end_time ) )
    {
        stat = update_ccmc_attribute_value( "end_time", end_time_cdf_epoch3,
                baselined_ccmc_attribute_structure_list );
    }
    if( strlen( r_body ) )
    {
        stat = update_ccmc_attribute_value( "r_body", r_body,
                baselined_ccmc_attribute_structure_list );
    }
    if( strlen( r_currents ) )
    {
        stat = update_ccmc_attribute_value( "r_currents", r_currents,
                baselined_ccmc_attribute_structure_list );
    }
    if( strlen( b_dipole ) )
    {
        stat = update_ccmc_attribute_value( "b_dipole", b_dipole,
                baselined_ccmc_attribute_structure_list );
    }
    if( strlen( run_type ) )
    {
        stat = update_ccmc_attribute_value( "run_type", run_type,
                baselined_ccmc_attribute_structure_list );
    }
    if( strlen( dipole_update ) )
    {
        stat = update_ccmc_attribute_value( "dipole_update", dipole_update,
                baselined_ccmc_attribute_structure_list );
    }

        /****** add dipole time only if dipole update flag from DatabaseInfo
         * file == yes ********/

        if( ! strcmp( dipole_update, "yes" )
                || ! strcmp( dipole_update, "Yes" )
                || ! strcmp( dipole_update, "YES" ) )
        {


        }
        else /* dipole time is not applicable as no updates are available */
        {
        stat = update_ccmc_attribute_value( "dipole_time", not_applicable,
                baselined_ccmc_attribute_structure_list );
        }

        /*add dipole_tilt and dipole_tilt_y values bases on cs_output flag from
         * DatabaseInfo flag *****/

        if( strlen( cs_output ) )
        {

        }
        else
        {

        }

    } /* end of if( write_database_flag ) block */

if ( verbose_flag ) { printf("global attributes values successfully added to "
        "baselined_ccmc_attribute_structure_list...\n"); }

/*******************************************************************************
*                        CREATE THE VARIABLES & VARIABLE ATTRIBUTES            *
*******************************************************************************/

/*******************************************************************************
*    initialize the baselined_ccmc_variable_structure_list to hold n number of *
*    cccmc_variable_structures where n = NUMBER_REGISTERED_BATSRUS_VARIABLES   *
*    defined in ccmc_registered_variables.h                                    *
*******************************************************************************/

if( verbose_flag ) printf("initializing baselined_ccmc_variable_structure_list "
        "for %d variables...", NUMBER_REGISTERED_ENLIL_VARIABLES );

baselined_ccmc_variable_structure_list =
    init_ccmc_variable_structure( NUMBER_REGISTERED_ENLIL_VARIABLES );

if( verbose_flag ) printf("done\n");

/**** Put default variable names and variable attribute values into the
 * ccmc_baselined_variable_structure_list ***/

for( i = 0; i< NUMBER_REGISTERED_ENLIL_VARIABLES; i++ )
{

put_ccmc_variable(
        registered_enlil_var_list[i].ccmc_var_name,
        registered_enlil_var_list[i].ccmc_var_data_type,
        0,
        registered_enlil_var_list[i].ccmc_var_classification,
        "" ,
        registered_enlil_var_list[i].valid_min,
        registered_enlil_var_list[i].valid_max,
        registered_enlil_var_list[i].units,
        registered_enlil_var_list[i].grid_system,
        registered_enlil_var_list[i].mask,
        registered_enlil_var_list[i].description,
        registered_enlil_var_list[i].is_vector_component,
        registered_enlil_var_list[i].position_grid_system,
        registered_enlil_var_list[i].data_grid_system,
        zero, zero, baselined_ccmc_variable_structure_list );
}

if ( verbose_flag ) { printf("registered variable names and default variable "
        "attribute values succeffully added to "
        "baselined_ccmc_variable_structure_list...\n"); }

/*******************************************************************************
*    write the actual variable values into the                                 *
*    baselined_ccmc_variable_structure_list                                    *
*******************************************************************************/

/*** write the original model variable values ***/


/************************************************
 *                 TIME FILE                     *
 * *********************************************/


/** calculate the number of time file cells **/

number_of_cells = tim_n1 * tim_n2 *tim_n3 * tim_number_of_blocks_nblk;

if ( verbose_flag ) { printf("\nwritting original time file model output "
        "variables values into baselined_ccmc_variable_structure_list...\n"); }

/*** update each variable size as the defualts are set to zero, this reallocs
 * space for the actual values to be stored *****/

if( tim_x1_position_X1 != NULL )
{
stat = update_ccmc_variable_attribute_value( "r", "variable_size", &tim_n1,
        baselined_ccmc_variable_structure_list );
stat = update_ccmc_variable_value( "r", tim_n1, tim_x1_position_X1,
        baselined_ccmc_variable_structure_list );
linear_minmax_search( tim_x1_position_X1, tim_n1, &actual_min, &actual_max );
stat = update_ccmc_variable_attribute_value( "r", "actual_min", &actual_min,
        baselined_ccmc_variable_structure_list );
stat = update_ccmc_variable_attribute_value( "r", "actual_max", &actual_max,
        baselined_ccmc_variable_structure_list );
stat = update_ccmc_variable_attribute_value( "r", "units", meters,
        baselined_ccmc_variable_structure_list );
stat = update_ccmc_variable_attribute_value( "r", "valid_min",
        &radius_m_valid_min, baselined_ccmc_variable_structure_list );
stat = update_ccmc_variable_attribute_value( "r", "valid_max",
        &radius_m_valid_max, baselined_ccmc_variable_structure_list );
}

if( tim_x2_position_X2 != NULL )
{
stat = update_ccmc_variable_attribute_value( "theta", "variable_size", &tim_n2,
        baselined_ccmc_variable_structure_list );
stat = update_ccmc_variable_value( "theta", tim_n2, tim_x2_position_X2,
        baselined_ccmc_variable_structure_list );
linear_minmax_search( tim_x2_position_X2, tim_n2, &actual_min, &actual_max );
stat = update_ccmc_variable_attribute_value( "theta", "actual_min", &actual_min,
        baselined_ccmc_variable_structure_list );
stat = update_ccmc_variable_attribute_value( "theta", "actual_max", &actual_max,
        baselined_ccmc_variable_structure_list );
stat = update_ccmc_variable_attribute_value( "theta", "units", radians,
        baselined_ccmc_variable_structure_list );
stat = update_ccmc_variable_attribute_value( "theta", "valid_min",
        &radian_valid_min, baselined_ccmc_variable_structure_list );
stat = update_ccmc_variable_attribute_value( "theta", "valid_max",
        &radian_valid_max, baselined_ccmc_variable_structure_list );
}

if( tim_x3_position_X3 != NULL )
{
stat = update_ccmc_variable_attribute_value( "phi", "variable_size", &tim_n3,
        baselined_ccmc_variable_structure_list );
stat = update_ccmc_variable_value( "phi", tim_n3, tim_x3_position_X3,
        baselined_ccmc_variable_structure_list );
linear_minmax_search( tim_x3_position_X3, tim_n3, &actual_min, &actual_max );
stat = update_ccmc_variable_attribute_value( "phi", "actual_min", &actual_min,
        baselined_ccmc_variable_structure_list );
stat = update_ccmc_variable_attribute_value( "phi", "actual_max", &actual_max,
        baselined_ccmc_variable_structure_list );
stat = update_ccmc_variable_attribute_value( "phi", "units", radians,
        baselined_ccmc_variable_structure_list );
stat = update_ccmc_variable_attribute_value( "phi", "valid_min",
        &radian_valid_min, baselined_ccmc_variable_structure_list );
stat = update_ccmc_variable_attribute_value( "phi", "valid_max",
        &radian_valid_max, baselined_ccmc_variable_structure_list );
}

if( tim_x1h_position_X1H != NULL )
{
stat = update_ccmc_variable_attribute_value( "r1", "variable_size", &tim_n1h,
        baselined_ccmc_variable_structure_list );

stat = update_ccmc_variable_value( "r1", tim_n1h, tim_x1h_position_X1H,
        baselined_ccmc_variable_structure_list );

linear_minmax_search( tim_x1h_position_X1H, tim_n1h, &actual_min,
        &actual_max );

stat = update_ccmc_variable_attribute_value( "r1", "actual_min",
        &actual_min, baselined_ccmc_variable_structure_list );

stat = update_ccmc_variable_attribute_value( "r1", "actual_max",
        &actual_max, baselined_ccmc_variable_structure_list );

stat = update_ccmc_variable_attribute_value( "r1", "units", meters,
        baselined_ccmc_variable_structure_list );

stat = update_ccmc_variable_attribute_value( "r1", "valid_min",
        &radius_m_valid_min, baselined_ccmc_variable_structure_list );

stat = update_ccmc_variable_attribute_value( "r1", "valid_max",
        &radius_m_valid_max, baselined_ccmc_variable_structure_list );
}

if( tim_x2h_position_X2H != NULL )
{
stat = update_ccmc_variable_attribute_value( "theta1", "variable_size", &tim_n2h,
        baselined_ccmc_variable_structure_list );

stat = update_ccmc_variable_value( "theta1", tim_n2h, tim_x2h_position_X2H,
        baselined_ccmc_variable_structure_list );

linear_minmax_search( tim_x2h_position_X2H, tim_n2h, &actual_min, &actual_max );

stat = update_ccmc_variable_attribute_value( "theta1", "actual_min", &actual_min,
        baselined_ccmc_variable_structure_list );

stat = update_ccmc_variable_attribute_value( "theta1", "actual_max", &actual_max,
        baselined_ccmc_variable_structure_list );

stat = update_ccmc_variable_attribute_value( "theta1", "units", radians,
        baselined_ccmc_variable_structure_list );

stat = update_ccmc_variable_attribute_value( "theta1", "valid_min",
        &radian_valid_min, baselined_ccmc_variable_structure_list );

stat = update_ccmc_variable_attribute_value( "theta1", "valid_max",
        &radian_valid_max, baselined_ccmc_variable_structure_list );
}

if( tim_x3h_position_X3H != NULL )
{
stat = update_ccmc_variable_attribute_value( "phi1", "variable_size",
        &tim_n3h, baselined_ccmc_variable_structure_list );

stat = update_ccmc_variable_value( "phi1", tim_n3h, tim_x3h_position_X3H,
        baselined_ccmc_variable_structure_list );

linear_minmax_search( tim_x3h_position_X3H, tim_n3h, &actual_min, &actual_max );

stat = update_ccmc_variable_attribute_value( "phi1", "actual_min",
        &actual_min, baselined_ccmc_variable_structure_list );

stat = update_ccmc_variable_attribute_value( "phi1", "actual_max",
        &actual_max, baselined_ccmc_variable_structure_list );

stat = update_ccmc_variable_attribute_value( "phi1", "units", radians,
        baselined_ccmc_variable_structure_list );

stat = update_ccmc_variable_attribute_value( "phi1", "valid_min",
        &radian_valid_min, baselined_ccmc_variable_structure_list );

stat = update_ccmc_variable_attribute_value( "phi1", "valid_max",
        &radian_valid_max, baselined_ccmc_variable_structure_list );
}

if( tim_density_D != NULL )
{
stat = update_ccmc_variable_attribute_value( "rho", "variable_size",
        &number_of_cells, baselined_ccmc_variable_structure_list );

stat = update_ccmc_variable_value( "rho", number_of_cells, tim_density_D,
        baselined_ccmc_variable_structure_list );
linear_minmax_search( tim_density_D, number_of_cells, &actual_min, &actual_max );
stat = update_ccmc_variable_attribute_value( "rho", "actual_min", &actual_min,
        baselined_ccmc_variable_structure_list );
stat = update_ccmc_variable_attribute_value( "rho", "actual_max", &actual_max,
        baselined_ccmc_variable_structure_list );
stat = update_ccmc_variable_attribute_value( "rho", "units", mass_density,
        baselined_ccmc_variable_structure_list );
}

if( tim_temperature_T != NULL )
{
stat = update_ccmc_variable_attribute_value( "T", "variable_size",
        &number_of_cells, baselined_ccmc_variable_structure_list );
stat = update_ccmc_variable_value( "T", number_of_cells, tim_temperature_T,
        baselined_ccmc_variable_structure_list );
linear_minmax_search( tim_temperature_T, number_of_cells, &actual_min,
        &actual_max );
stat = update_ccmc_variable_attribute_value( "T", "actual_min", &actual_min,
        baselined_ccmc_variable_structure_list );
stat = update_ccmc_variable_attribute_value( "T", "actual_max", &actual_max,
        baselined_ccmc_variable_structure_list );
}

if( tim_x1_velocity_V1 != NULL )
{
stat = update_ccmc_variable_attribute_value( "ur", "variable_size",
        &number_of_cells, baselined_ccmc_variable_structure_list );
stat = update_ccmc_variable_value( "ur", number_of_cells, tim_x1_velocity_V1,
        baselined_ccmc_variable_structure_list );
linear_minmax_search( tim_x1_velocity_V1, number_of_cells, &actual_min,
        &actual_max );
stat = update_ccmc_variable_attribute_value( "ur", "actual_min", &actual_min,
        baselined_ccmc_variable_structure_list );
stat = update_ccmc_variable_attribute_value( "ur", "actual_max", &actual_max,
        baselined_ccmc_variable_structure_list );
stat = update_ccmc_variable_attribute_value( "ur", "units", velocity,
        baselined_ccmc_variable_structure_list );
}

if( tim_x2_velocity_V2 != NULL )
{
stat = update_ccmc_variable_attribute_value( "utheta", "variable_size",
        &number_of_cells, baselined_ccmc_variable_structure_list );
stat = update_ccmc_variable_value( "utheta", number_of_cells, tim_x2_velocity_V2,
        baselined_ccmc_variable_structure_list );
linear_minmax_search( tim_x2_velocity_V2, number_of_cells, &actual_min,
        &actual_max );
stat = update_ccmc_variable_attribute_value( "utheta", "actual_min", &actual_min,
        baselined_ccmc_variable_structure_list );
stat = update_ccmc_variable_attribute_value( "utheta", "actual_max", &actual_max,
        baselined_ccmc_variable_structure_list );
stat = update_ccmc_variable_attribute_value( "utheta", "units", velocity,
        baselined_ccmc_variable_structure_list );
}

if( tim_x3_velocity_V3 != NULL )
{
stat = update_ccmc_variable_attribute_value( "uphi", "variable_size",
        &number_of_cells, baselined_ccmc_variable_structure_list );
stat = update_ccmc_variable_value( "uphi", number_of_cells,
        tim_x3_velocity_V3, baselined_ccmc_variable_structure_list );
linear_minmax_search( tim_x3_velocity_V3, number_of_cells, &actual_min,
        &actual_max );
stat = update_ccmc_variable_attribute_value( "uphi", "actual_min",
        &actual_min, baselined_ccmc_variable_structure_list );
stat = update_ccmc_variable_attribute_value( "uphi", "actual_max",
        &actual_max, baselined_ccmc_variable_structure_list );
stat = update_ccmc_variable_attribute_value( "uphi", "units", velocity,
        baselined_ccmc_variable_structure_list );
}

if( tim_x1_magnetic_field_B1 != NULL )
{
stat = update_ccmc_variable_attribute_value( "br", "variable_size",
        &number_of_cells, baselined_ccmc_variable_structure_list );

stat = update_ccmc_variable_value( "br", number_of_cells,
        tim_x1_magnetic_field_B1, baselined_ccmc_variable_structure_list );

linear_minmax_search( tim_x1_magnetic_field_B1, number_of_cells,
        &actual_min, &actual_max );

stat = update_ccmc_variable_attribute_value( "br", "actual_min",
&actual_min, baselined_ccmc_variable_structure_list );

stat = update_ccmc_variable_attribute_value( "br", "actual_max",
        &actual_max, baselined_ccmc_variable_structure_list );
stat = update_ccmc_variable_attribute_value( "br", "units", magnetic_field,
        baselined_ccmc_variable_structure_list );
stat = update_ccmc_variable_attribute_value( "br", "valid_min",
        &mag_field_T_valid_min, baselined_ccmc_variable_structure_list );
stat = update_ccmc_variable_attribute_value( "br", "valid_max",
        &mag_field_T_valid_max, baselined_ccmc_variable_structure_list );
}

if( tim_x2_magnetic_field_B2 != NULL )
{
stat =
    update_ccmc_variable_attribute_value( "btheta", "variable_size", &number_of_cells, baselined_ccmc_variable_structure_list );
stat = update_ccmc_variable_value( "btheta", number_of_cells, tim_x2_magnetic_field_B2, baselined_ccmc_variable_structure_list );
linear_minmax_search( tim_x2_magnetic_field_B2, number_of_cells, &actual_min, &actual_max );
stat = update_ccmc_variable_attribute_value( "btheta", "actual_min", &actual_min, baselined_ccmc_variable_structure_list );
stat = update_ccmc_variable_attribute_value( "btheta", "actual_max", &actual_max, baselined_ccmc_variable_structure_list );
stat = update_ccmc_variable_attribute_value( "btheta", "units", magnetic_field, baselined_ccmc_variable_structure_list );
stat = update_ccmc_variable_attribute_value( "btheta", "valid_min", &mag_field_T_valid_min, baselined_ccmc_variable_structure_list );
stat = update_ccmc_variable_attribute_value( "btheta", "valid_max", &mag_field_T_valid_max, baselined_ccmc_variable_structure_list );
}

if( tim_x3_magnetic_field_B3 != NULL )
{
stat = update_ccmc_variable_attribute_value( "bphi", "variable_size", &number_of_cells, baselined_ccmc_variable_structure_list );
stat = update_ccmc_variable_value( "bphi", number_of_cells, tim_x3_magnetic_field_B3, baselined_ccmc_variable_structure_list );
linear_minmax_search( tim_x3_magnetic_field_B3, number_of_cells, &actual_min, &actual_max );
stat = update_ccmc_variable_attribute_value( "bphi", "actual_min", &actual_min, baselined_ccmc_variable_structure_list );
stat = update_ccmc_variable_attribute_value( "bphi", "actual_max", &actual_max, baselined_ccmc_variable_structure_list );
stat = update_ccmc_variable_attribute_value( "bphi", "units", magnetic_field, baselined_ccmc_variable_structure_list );
stat = update_ccmc_variable_attribute_value( "bphi", "valid_min", &mag_field_T_valid_min, baselined_ccmc_variable_structure_list );
stat = update_ccmc_variable_attribute_value( "bphi", "valid_max", &mag_field_T_valid_max, baselined_ccmc_variable_structure_list );
}

if( tim_cloud_mass_density_DP != NULL )
{
stat = update_ccmc_variable_attribute_value( "dp", "variable_size", &number_of_cells, baselined_ccmc_variable_structure_list );
stat = update_ccmc_variable_value( "dp", number_of_cells, tim_cloud_mass_density_DP, baselined_ccmc_variable_structure_list );
linear_minmax_search( tim_cloud_mass_density_DP, number_of_cells, &actual_min, &actual_max );
stat = update_ccmc_variable_attribute_value( "dp", "actual_min", &actual_min, baselined_ccmc_variable_structure_list );
stat = update_ccmc_variable_attribute_value( "dp", "actual_max", &actual_max, baselined_ccmc_variable_structure_list );
stat = update_ccmc_variable_attribute_value( "dp", "units", mass_density, baselined_ccmc_variable_structure_list );
}

if( tim_polarity_of_magnetic_field_BP != NULL )
{
stat = update_ccmc_variable_attribute_value( "bp", "variable_size", &number_of_cells, baselined_ccmc_variable_structure_list );
stat = update_ccmc_variable_value( "bp", number_of_cells, tim_polarity_of_magnetic_field_BP, baselined_ccmc_variable_structure_list );
linear_minmax_search( tim_polarity_of_magnetic_field_BP, number_of_cells, &actual_min, &actual_max );
stat = update_ccmc_variable_attribute_value( "bp", "actual_min", &actual_min, baselined_ccmc_variable_structure_list );
stat = update_ccmc_variable_attribute_value( "bp", "actual_max", &actual_max, baselined_ccmc_variable_structure_list );
stat = update_ccmc_variable_attribute_value( "bp", "valid_min", &polarity_valid_min, baselined_ccmc_variable_structure_list );
stat = update_ccmc_variable_attribute_value( "bp", "valid_max", &polarity_valid_max, baselined_ccmc_variable_structure_list );
}

/************** now create and update b1r, b1theta, and b1phi  on the fly using br, btheta, bphi & bp *******************/

if ( ( temp_b_buffer = calloc( number_of_cells, sizeof(float) ) ) == NULL )
{
printf("MALLOC for temp_b_buffer array FAILED\n");
}

if( tim_x1_magnetic_field_B1 != NULL && tim_polarity_of_magnetic_field_BP != NULL && temp_b_buffer != NULL )
{

if( verbose_flag ) { printf("Creating b1r variable from br and bp with correct polarity to undo anti-reconection feature of advection..."); }

    for( i=0; i<number_of_cells;i++)
    {
        if( tim_polarity_of_magnetic_field_BP[i] < 0 )
        {
        temp_b_buffer[i] = tim_x1_magnetic_field_B1[i] * -1;
        }
        else
        {
        temp_b_buffer[i] = tim_x1_magnetic_field_B1[i];
        }
    }

stat = update_ccmc_variable_attribute_value( "b1r", "variable_size", &number_of_cells, baselined_ccmc_variable_structure_list );
stat = update_ccmc_variable_attribute_value( "b1r", "description", b1r_description, baselined_ccmc_variable_structure_list );
stat = update_ccmc_variable_value( "b1r", number_of_cells, temp_b_buffer, baselined_ccmc_variable_structure_list );
linear_minmax_search( temp_b_buffer, number_of_cells, &actual_min, &actual_max );
stat = update_ccmc_variable_attribute_value( "b1r", "actual_min", &actual_min, baselined_ccmc_variable_structure_list );
stat = update_ccmc_variable_attribute_value( "b1r", "actual_max", &actual_max, baselined_ccmc_variable_structure_list );
stat = update_ccmc_variable_attribute_value( "b1r", "units", magnetic_field, baselined_ccmc_variable_structure_list );
stat = update_ccmc_variable_attribute_value( "b1r", "valid_min", &mag_field_T_valid_min, baselined_ccmc_variable_structure_list );
stat = update_ccmc_variable_attribute_value( "b1r", "valid_max", &mag_field_T_valid_max, baselined_ccmc_variable_structure_list );

if( verbose_flag ) { printf("Complete.\n"); }

}

if( tim_x2_magnetic_field_B2 != NULL && tim_polarity_of_magnetic_field_BP != NULL && temp_b_buffer != NULL )
{

if( verbose_flag ) { printf("Creating b1theta variable from btheta and bp with correct polarity to undo anti-reconection feature of advection..."); }

    for( i=0; i<number_of_cells;i++)
    {
        if( tim_polarity_of_magnetic_field_BP[i] < 0 )
        {
        temp_b_buffer[i] = tim_x2_magnetic_field_B2[i] * -1;
        }
        else
        {
        temp_b_buffer[i] = tim_x2_magnetic_field_B2[i];
        }
    }

stat = update_ccmc_variable_attribute_value( "b1theta", "variable_size", &number_of_cells, baselined_ccmc_variable_structure_list );
stat = update_ccmc_variable_attribute_value( "b1theta", "description", b1theta_description, baselined_ccmc_variable_structure_list );
stat = update_ccmc_variable_value( "b1theta", number_of_cells, temp_b_buffer, baselined_ccmc_variable_structure_list );
linear_minmax_search( temp_b_buffer, number_of_cells, &actual_min, &actual_max );
stat = update_ccmc_variable_attribute_value( "b1theta", "actual_min", &actual_min, baselined_ccmc_variable_structure_list );
stat = update_ccmc_variable_attribute_value( "b1theta", "actual_max", &actual_max, baselined_ccmc_variable_structure_list );
stat = update_ccmc_variable_attribute_value( "b1theta", "units", magnetic_field, baselined_ccmc_variable_structure_list );
stat = update_ccmc_variable_attribute_value( "b1theta", "valid_min", &mag_field_T_valid_min, baselined_ccmc_variable_structure_list );
stat = update_ccmc_variable_attribute_value( "b1theta", "valid_max", &mag_field_T_valid_max, baselined_ccmc_variable_structure_list );

if( verbose_flag ) { printf("Complete.\n"); }

}

if( tim_x3_magnetic_field_B3 != NULL && tim_polarity_of_magnetic_field_BP != NULL && temp_b_buffer != NULL )
{

if( verbose_flag ) { printf("Creating b1phi variable from bphi and bp with correct polarity to undo anti-reconection feature of advection..."); }

    for( i=0; i<number_of_cells;i++)
    {
        if( tim_polarity_of_magnetic_field_BP[i] < 0 )
        {
        temp_b_buffer[i] = tim_x3_magnetic_field_B3[i] * -1;
        }
        else
        {
        temp_b_buffer[i] = tim_x3_magnetic_field_B3[i];
        }
    }

stat = update_ccmc_variable_attribute_value( "b1phi", "variable_size", &number_of_cells, baselined_ccmc_variable_structure_list );
stat = update_ccmc_variable_attribute_value( "b1phi", "description", b1phi_description, baselined_ccmc_variable_structure_list );
stat = update_ccmc_variable_value( "b1phi", number_of_cells, temp_b_buffer, baselined_ccmc_variable_structure_list );
linear_minmax_search( temp_b_buffer, number_of_cells, &actual_min, &actual_max );
stat = update_ccmc_variable_attribute_value( "b1phi", "actual_min", &actual_min, baselined_ccmc_variable_structure_list );
stat = update_ccmc_variable_attribute_value( "b1phi", "actual_max", &actual_max, baselined_ccmc_variable_structure_list );
stat = update_ccmc_variable_attribute_value( "b1phi", "units", magnetic_field, baselined_ccmc_variable_structure_list );
stat = update_ccmc_variable_attribute_value( "b1phi", "valid_min", &mag_field_T_valid_min, baselined_ccmc_variable_structure_list );
stat = update_ccmc_variable_attribute_value( "b1phi", "valid_max", &mag_field_T_valid_max, baselined_ccmc_variable_structure_list );

if( verbose_flag ) { printf("Complete.\n"); }

}

/*** lets free the temp_b_buffer now ***/

free( temp_b_buffer );


/************************************************
 *                 EVH FILE                     *
 * *********************************************/


/** calculate the number of evh file cells **/

number_of_cells = evh_nhel * evh_nevo;

if ( verbose_flag ) { printf("\nwritting original evh file model output variables values into baselined_ccmc_variable_structure_list...\n"); }

/*** update each variable size as the defualts are set to zero, this reallocs space for the actual values to be stored *****/

if( evh_x1_position_X1 != NULL )
{
stat = update_ccmc_variable_attribute_value( "r2", "variable_size", &evh_nhel, baselined_ccmc_variable_structure_list );
stat = update_ccmc_variable_value( "r2", evh_nhel, evh_x1_position_X1, baselined_ccmc_variable_structure_list );
linear_minmax_search( evh_x1_position_X1, evh_nhel, &actual_min, &actual_max );
stat = update_ccmc_variable_attribute_value( "r2", "actual_min", &actual_min, baselined_ccmc_variable_structure_list );
stat = update_ccmc_variable_attribute_value( "r2", "actual_max", &actual_max, baselined_ccmc_variable_structure_list );
stat = update_ccmc_variable_attribute_value( "r2", "units", meters, baselined_ccmc_variable_structure_list );
stat = update_ccmc_variable_attribute_value( "r2", "valid_min", &radius_m_valid_min, baselined_ccmc_variable_structure_list );
stat = update_ccmc_variable_attribute_value( "r2", "valid_max", &radius_m_valid_max, baselined_ccmc_variable_structure_list );
}

if( evh_x2_position_X2 != NULL )
{
stat = update_ccmc_variable_attribute_value( "theta2", "variable_size", &evh_nhel, baselined_ccmc_variable_structure_list );
stat = update_ccmc_variable_value( "theta2", evh_nhel, evh_x2_position_X2, baselined_ccmc_variable_structure_list );
linear_minmax_search( evh_x2_position_X2, evh_nhel, &actual_min, &actual_max );
stat = update_ccmc_variable_attribute_value( "theta2", "actual_min", &actual_min, baselined_ccmc_variable_structure_list );
stat = update_ccmc_variable_attribute_value( "theta2", "actual_max", &actual_max, baselined_ccmc_variable_structure_list );
stat = update_ccmc_variable_attribute_value( "theta2", "units", radians, baselined_ccmc_variable_structure_list );
stat = update_ccmc_variable_attribute_value( "theta2", "valid_min", &radian_valid_min, baselined_ccmc_variable_structure_list );
stat = update_ccmc_variable_attribute_value( "theta2", "valid_max", &radian_valid_max, baselined_ccmc_variable_structure_list );
}

if( evh_x3_position_X3 != NULL )
{
stat = update_ccmc_variable_attribute_value( "phi2", "variable_size", &evh_nhel, baselined_ccmc_variable_structure_list );
stat = update_ccmc_variable_value( "phi2", evh_nhel, evh_x3_position_X3, baselined_ccmc_variable_structure_list );
linear_minmax_search( evh_x3_position_X3, evh_nhel, &actual_min, &actual_max );
stat = update_ccmc_variable_attribute_value( "phi2", "actual_min", &actual_min, baselined_ccmc_variable_structure_list );
stat = update_ccmc_variable_attribute_value( "phi2", "actual_max", &actual_max, baselined_ccmc_variable_structure_list );
stat = update_ccmc_variable_attribute_value( "phi2", "units", radians, baselined_ccmc_variable_structure_list );
stat = update_ccmc_variable_attribute_value( "phi2", "valid_min", &radian_valid_min, baselined_ccmc_variable_structure_list );
stat = update_ccmc_variable_attribute_value( "phi2", "valid_max", &radian_valid_max, baselined_ccmc_variable_structure_list );
}

if( evh_density_D != NULL )
{
stat = update_ccmc_variable_attribute_value( "rho_1", "variable_size", &number_of_cells, baselined_ccmc_variable_structure_list );
stat = update_ccmc_variable_value( "rho_1", number_of_cells, evh_density_D, baselined_ccmc_variable_structure_list );
linear_minmax_search( evh_density_D, number_of_cells, &actual_min, &actual_max );
stat = update_ccmc_variable_attribute_value( "rho_1", "actual_min", &actual_min, baselined_ccmc_variable_structure_list );
stat = update_ccmc_variable_attribute_value( "rho_1", "actual_max", &actual_max, baselined_ccmc_variable_structure_list );
stat = update_ccmc_variable_attribute_value( "rho_1", "units", mass_density, baselined_ccmc_variable_structure_list );
stat = update_ccmc_variable_attribute_value( "rho_1", "grid_system", grid_system_3, baselined_ccmc_variable_structure_list );
stat = update_ccmc_variable_attribute_value( "rho_1", "position_grid_system", grid_system_3, baselined_ccmc_variable_structure_list );
stat = update_ccmc_variable_attribute_value( "rho_1", "data_grid_system", evh_grid_system, baselined_ccmc_variable_structure_list );
}

if( evh_temperature_T != NULL )
{
stat = update_ccmc_variable_attribute_value( "T_1", "variable_size", &number_of_cells, baselined_ccmc_variable_structure_list );
stat = update_ccmc_variable_value( "T_1", number_of_cells, evh_temperature_T, baselined_ccmc_variable_structure_list );
linear_minmax_search( evh_temperature_T, number_of_cells, &actual_min, &actual_max );
stat = update_ccmc_variable_attribute_value( "T_1", "actual_min", &actual_min, baselined_ccmc_variable_structure_list );
stat = update_ccmc_variable_attribute_value( "T_1", "actual_max", &actual_max, baselined_ccmc_variable_structure_list );
stat = update_ccmc_variable_attribute_value( "T_1", "grid_system", grid_system_3, baselined_ccmc_variable_structure_list );
stat = update_ccmc_variable_attribute_value( "T_1", "position_grid_system", grid_system_3, baselined_ccmc_variable_structure_list );
stat = update_ccmc_variable_attribute_value( "T_1", "data_grid_system", evh_grid_system, baselined_ccmc_variable_structure_list );
}

if( evh_x1_velocity_V1 != NULL )
{
stat = update_ccmc_variable_attribute_value( "ur_1", "variable_size", &number_of_cells, baselined_ccmc_variable_structure_list );
stat = update_ccmc_variable_value( "ur_1", number_of_cells, evh_x1_velocity_V1, baselined_ccmc_variable_structure_list );
linear_minmax_search( evh_x1_velocity_V1, number_of_cells, &actual_min, &actual_max );
stat = update_ccmc_variable_attribute_value( "ur_1", "actual_min", &actual_min, baselined_ccmc_variable_structure_list );
stat = update_ccmc_variable_attribute_value( "ur_1", "actual_max", &actual_max, baselined_ccmc_variable_structure_list );
stat = update_ccmc_variable_attribute_value( "ur_1", "units", velocity, baselined_ccmc_variable_structure_list );
stat = update_ccmc_variable_attribute_value( "ur_1", "grid_system", grid_system_3, baselined_ccmc_variable_structure_list );
stat = update_ccmc_variable_attribute_value( "ur_1", "position_grid_system", grid_system_3, baselined_ccmc_variable_structure_list );
stat = update_ccmc_variable_attribute_value( "ur_1", "data_grid_system", evh_grid_system, baselined_ccmc_variable_structure_list );
}

if( evh_x2_velocity_V2 != NULL )
{
stat = update_ccmc_variable_attribute_value( "utheta_1", "variable_size", &number_of_cells, baselined_ccmc_variable_structure_list );
stat = update_ccmc_variable_value( "utheta_1", number_of_cells, evh_x2_velocity_V2, baselined_ccmc_variable_structure_list );
linear_minmax_search( evh_x2_velocity_V2, number_of_cells, &actual_min, &actual_max );
stat = update_ccmc_variable_attribute_value( "utheta_1", "actual_min", &actual_min, baselined_ccmc_variable_structure_list );
stat = update_ccmc_variable_attribute_value( "utheta_1", "actual_max", &actual_max, baselined_ccmc_variable_structure_list );
stat = update_ccmc_variable_attribute_value( "utheta_1", "units", velocity, baselined_ccmc_variable_structure_list );
stat = update_ccmc_variable_attribute_value( "utheta_1", "grid_system", grid_system_3, baselined_ccmc_variable_structure_list );
stat = update_ccmc_variable_attribute_value( "utheta_1", "position_grid_system", grid_system_3, baselined_ccmc_variable_structure_list );
stat = update_ccmc_variable_attribute_value( "utheta_1", "data_grid_system", evh_grid_system, baselined_ccmc_variable_structure_list );
}

if( evh_x3_velocity_V3 != NULL )
{
stat = update_ccmc_variable_attribute_value( "uphi_1", "variable_size", &number_of_cells, baselined_ccmc_variable_structure_list );
stat = update_ccmc_variable_value( "uphi_1", number_of_cells, evh_x3_velocity_V3, baselined_ccmc_variable_structure_list );
linear_minmax_search( evh_x3_velocity_V3, number_of_cells, &actual_min, &actual_max );
stat = update_ccmc_variable_attribute_value( "uphi_1", "actual_min", &actual_min, baselined_ccmc_variable_structure_list );
stat = update_ccmc_variable_attribute_value( "uphi_1", "actual_max", &actual_max, baselined_ccmc_variable_structure_list );
stat = update_ccmc_variable_attribute_value( "uphi_1", "units", velocity, baselined_ccmc_variable_structure_list );
stat = update_ccmc_variable_attribute_value( "uphi_1", "grid_system", grid_system_3, baselined_ccmc_variable_structure_list );
stat = update_ccmc_variable_attribute_value( "uphi_1", "position_grid_system", grid_system_3, baselined_ccmc_variable_structure_list );
stat = update_ccmc_variable_attribute_value( "uphi_1", "data_grid_system", evh_grid_system, baselined_ccmc_variable_structure_list );
}

if( evh_x1_magnetic_field_B1 != NULL )
{
stat = update_ccmc_variable_attribute_value( "br_1", "variable_size", &number_of_cells, baselined_ccmc_variable_structure_list );
stat = update_ccmc_variable_value( "br_1", number_of_cells, evh_x1_magnetic_field_B1, baselined_ccmc_variable_structure_list );
linear_minmax_search( evh_x1_magnetic_field_B1, number_of_cells, &actual_min, &actual_max );
stat = update_ccmc_variable_attribute_value( "br_1", "actual_min", &actual_min, baselined_ccmc_variable_structure_list );
stat = update_ccmc_variable_attribute_value( "br_1", "actual_max", &actual_max, baselined_ccmc_variable_structure_list );
stat = update_ccmc_variable_attribute_value( "br_1", "units", magnetic_field, baselined_ccmc_variable_structure_list );
stat = update_ccmc_variable_attribute_value( "br_1", "grid_system", grid_system_3, baselined_ccmc_variable_structure_list );
stat = update_ccmc_variable_attribute_value( "br_1", "position_grid_system", grid_system_3, baselined_ccmc_variable_structure_list );
stat = update_ccmc_variable_attribute_value( "br_1", "data_grid_system", evh_grid_system, baselined_ccmc_variable_structure_list );
stat = update_ccmc_variable_attribute_value( "br_1", "valid_min", &mag_field_T_valid_min, baselined_ccmc_variable_structure_list );
stat = update_ccmc_variable_attribute_value( "br_1", "valid_max", &mag_field_T_valid_max, baselined_ccmc_variable_structure_list );
}

if( evh_x2_magnetic_field_B2 != NULL )
{
stat = update_ccmc_variable_attribute_value( "btheta_1", "variable_size", &number_of_cells, baselined_ccmc_variable_structure_list );
stat = update_ccmc_variable_value( "btheta_1", number_of_cells, evh_x2_magnetic_field_B2, baselined_ccmc_variable_structure_list );
linear_minmax_search( evh_x2_magnetic_field_B2, number_of_cells, &actual_min, &actual_max );
stat = update_ccmc_variable_attribute_value( "btheta_1", "actual_min", &actual_min, baselined_ccmc_variable_structure_list );
stat = update_ccmc_variable_attribute_value( "btheta_1", "actual_max", &actual_max, baselined_ccmc_variable_structure_list );
stat = update_ccmc_variable_attribute_value( "btheta_1", "units", magnetic_field, baselined_ccmc_variable_structure_list );
stat = update_ccmc_variable_attribute_value( "btheta_1", "grid_system", grid_system_3, baselined_ccmc_variable_structure_list );
stat = update_ccmc_variable_attribute_value( "btheta_1", "position_grid_system", grid_system_3, baselined_ccmc_variable_structure_list );
stat = update_ccmc_variable_attribute_value( "btheta_1", "data_grid_system", evh_grid_system, baselined_ccmc_variable_structure_list );
stat = update_ccmc_variable_attribute_value( "btheta_1", "valid_min", &mag_field_T_valid_min, baselined_ccmc_variable_structure_list );
stat = update_ccmc_variable_attribute_value( "btheta_1", "valid_max", &mag_field_T_valid_max, baselined_ccmc_variable_structure_list );
}

if( evh_x3_magnetic_field_B3 != NULL )
{
stat = update_ccmc_variable_attribute_value( "bphi_1", "variable_size", &number_of_cells, baselined_ccmc_variable_structure_list );
stat = update_ccmc_variable_value( "bphi_1", number_of_cells, evh_x3_magnetic_field_B3, baselined_ccmc_variable_structure_list );
linear_minmax_search( evh_x3_magnetic_field_B3, number_of_cells, &actual_min, &actual_max );
stat = update_ccmc_variable_attribute_value( "bphi_1", "actual_min", &actual_min, baselined_ccmc_variable_structure_list );
stat = update_ccmc_variable_attribute_value( "bphi_1", "actual_max", &actual_max, baselined_ccmc_variable_structure_list );
stat = update_ccmc_variable_attribute_value( "bphi_1", "units", magnetic_field, baselined_ccmc_variable_structure_list );
stat = update_ccmc_variable_attribute_value( "bphi_1", "grid_system", grid_system_3, baselined_ccmc_variable_structure_list );
stat = update_ccmc_variable_attribute_value( "bphi_1", "position_grid_system", grid_system_3, baselined_ccmc_variable_structure_list );
stat = update_ccmc_variable_attribute_value( "bphi_1", "data_grid_system", evh_grid_system, baselined_ccmc_variable_structure_list );
stat = update_ccmc_variable_attribute_value( "bphi_1", "valid_min", &mag_field_T_valid_min, baselined_ccmc_variable_structure_list );
stat = update_ccmc_variable_attribute_value( "bphi_1", "valid_max", &mag_field_T_valid_max, baselined_ccmc_variable_structure_list );
}

if( evh_cloud_mass_density_DP != NULL )
{
stat = update_ccmc_variable_attribute_value( "dp_1", "variable_size", &number_of_cells, baselined_ccmc_variable_structure_list );
stat = update_ccmc_variable_value( "dp_1", number_of_cells, evh_cloud_mass_density_DP, baselined_ccmc_variable_structure_list );
linear_minmax_search( evh_cloud_mass_density_DP, number_of_cells, &actual_min, &actual_max );
stat = update_ccmc_variable_attribute_value( "dp_1", "actual_min", &actual_min, baselined_ccmc_variable_structure_list );
stat = update_ccmc_variable_attribute_value( "dp_1", "actual_max", &actual_max, baselined_ccmc_variable_structure_list );
stat = update_ccmc_variable_attribute_value( "dp_1", "units", mass_density, baselined_ccmc_variable_structure_list );
stat = update_ccmc_variable_attribute_value( "dp_1", "grid_system", grid_system_3, baselined_ccmc_variable_structure_list );
stat = update_ccmc_variable_attribute_value( "dp_1", "position_grid_system", grid_system_3, baselined_ccmc_variable_structure_list );
stat = update_ccmc_variable_attribute_value( "dp_1", "data_grid_system", evh_grid_system, baselined_ccmc_variable_structure_list );
}

if( evh_polarity_of_magnetic_field_BP != NULL )
{
stat = update_ccmc_variable_attribute_value( "bp_1", "variable_size", &number_of_cells, baselined_ccmc_variable_structure_list );
stat = update_ccmc_variable_value( "bp_1", number_of_cells, evh_polarity_of_magnetic_field_BP, baselined_ccmc_variable_structure_list );
linear_minmax_search( evh_polarity_of_magnetic_field_BP, number_of_cells, &actual_min, &actual_max );
stat = update_ccmc_variable_attribute_value( "bp_1", "actual_min", &actual_min, baselined_ccmc_variable_structure_list );
stat = update_ccmc_variable_attribute_value( "bp_1", "actual_max", &actual_max, baselined_ccmc_variable_structure_list );
stat = update_ccmc_variable_attribute_value( "bp_1", "grid_system", grid_system_3, baselined_ccmc_variable_structure_list );
stat = update_ccmc_variable_attribute_value( "bp_1", "position_grid_system", grid_system_3, baselined_ccmc_variable_structure_list );
stat = update_ccmc_variable_attribute_value( "bp_1", "data_grid_system", evh_grid_system, baselined_ccmc_variable_structure_list );
stat = update_ccmc_variable_attribute_value( "bp_1", "valid_min", &polarity_valid_min, baselined_ccmc_variable_structure_list );
stat = update_ccmc_variable_attribute_value( "bp_1", "valid_max", &polarity_valid_max, baselined_ccmc_variable_structure_list );
}

if( evh_physical_time_TIME != NULL )
{
stat = update_ccmc_variable_attribute_value( "time_1", "variable_size", &evh_nevo, baselined_ccmc_variable_structure_list );
stat = update_ccmc_variable_value( "time_1", evh_nevo, evh_physical_time_TIME, baselined_ccmc_variable_structure_list );
linear_minmax_search( evh_physical_time_TIME, evh_nevo, &actual_min, &actual_max );
stat = update_ccmc_variable_attribute_value( "time_1", "actual_min", &actual_min, baselined_ccmc_variable_structure_list );
stat = update_ccmc_variable_attribute_value( "time_1", "actual_max", &actual_max, baselined_ccmc_variable_structure_list );
stat = update_ccmc_variable_attribute_value( "time_1", "grid_system", grid_system_3, baselined_ccmc_variable_structure_list );
stat = update_ccmc_variable_attribute_value( "time_1", "position_grid_system", grid_system_3, baselined_ccmc_variable_structure_list );
stat = update_ccmc_variable_attribute_value( "time_1", "data_grid_system", grid_system_3, baselined_ccmc_variable_structure_list );

}

if( evh_physical_time_step_DT != NULL )
{
stat = update_ccmc_variable_attribute_value( "time_step_1", "variable_size", &evh_nevo, baselined_ccmc_variable_structure_list );
stat = update_ccmc_variable_value( "time_step_1", evh_nevo, evh_physical_time_step_DT, baselined_ccmc_variable_structure_list );
linear_minmax_search( evh_physical_time_step_DT, evh_nevo, &actual_min, &actual_max );
stat = update_ccmc_variable_attribute_value( "time_step_1", "actual_min", &actual_min, baselined_ccmc_variable_structure_list );
stat = update_ccmc_variable_attribute_value( "time_step_1", "actual_max", &actual_max, baselined_ccmc_variable_structure_list );
stat = update_ccmc_variable_attribute_value( "time_step_1", "grid_system", grid_system_3, baselined_ccmc_variable_structure_list );
stat = update_ccmc_variable_attribute_value( "time_step_1", "position_grid_system", grid_system_3, baselined_ccmc_variable_structure_list );
stat = update_ccmc_variable_attribute_value( "time_step_1", "data_grid_system", grid_system_3, baselined_ccmc_variable_structure_list );
}

if( evh_numerical_time_step_NSTEP != NULL )
{
stat = update_ccmc_variable_attribute_value( "numerical_step_1", "variable_size", &evh_nevo, baselined_ccmc_variable_structure_list );
stat = update_ccmc_variable_value( "numerical_step_1", evh_nevo, evh_numerical_time_step_NSTEP, baselined_ccmc_variable_structure_list );
linear_minmax_search_int( evh_numerical_time_step_NSTEP, evh_nevo, &actual_min_int, &actual_max_int );

/** copy and type cast actual_m*_int into actual_m* **/
actual_min = (float) actual_min_int;
actual_max = (float) actual_max_int;

stat = update_ccmc_variable_attribute_value( "numerical_step_1", "actual_min", &actual_min, baselined_ccmc_variable_structure_list );
stat = update_ccmc_variable_attribute_value( "numerical_step_1", "actual_max", &actual_max, baselined_ccmc_variable_structure_list );
}

/************************************************
 *                 EVL FILE                     *
 * *********************************************/


/** calculate the number of evl file cells **/

number_of_cells = evl_nsel * evl_nevo;

if ( verbose_flag ) { printf("\nwritting original evl file model output variables values into baselined_ccmc_variable_structure_list...\n"); }

/*** update each variable size as the defualts are set to zero, this reallocs space for the actual values to be stored *****/

if( evl_x1_position_X1 != NULL )
{
stat = update_ccmc_variable_attribute_value( "r3", "variable_size", &number_of_cells, baselined_ccmc_variable_structure_list );
stat = update_ccmc_variable_value( "r3", number_of_cells, evl_x1_position_X1, baselined_ccmc_variable_structure_list );
linear_minmax_search( evl_x1_position_X1, number_of_cells, &actual_min, &actual_max );
stat = update_ccmc_variable_attribute_value( "r3", "actual_min", &actual_min, baselined_ccmc_variable_structure_list );
stat = update_ccmc_variable_attribute_value( "r3", "actual_max", &actual_max, baselined_ccmc_variable_structure_list );
stat = update_ccmc_variable_attribute_value( "r3", "units", meters, baselined_ccmc_variable_structure_list );
stat = update_ccmc_variable_attribute_value( "r3", "valid_min", &radius_m_valid_min, baselined_ccmc_variable_structure_list );
stat = update_ccmc_variable_attribute_value( "r3", "valid_max", &radius_m_valid_max, baselined_ccmc_variable_structure_list );
}


if( evl_x2_position_X2 != NULL )
{
stat = update_ccmc_variable_attribute_value( "theta3", "variable_size", &number_of_cells, baselined_ccmc_variable_structure_list );
stat = update_ccmc_variable_value( "theta3", number_of_cells, evl_x2_position_X2, baselined_ccmc_variable_structure_list );
linear_minmax_search( evl_x2_position_X2, number_of_cells, &actual_min, &actual_max );
stat = update_ccmc_variable_attribute_value( "theta3", "actual_min", &actual_min, baselined_ccmc_variable_structure_list );
stat = update_ccmc_variable_attribute_value( "theta3", "actual_max", &actual_max, baselined_ccmc_variable_structure_list );
stat = update_ccmc_variable_attribute_value( "theta3", "units", radians, baselined_ccmc_variable_structure_list );
stat = update_ccmc_variable_attribute_value( "theta3", "valid_min", &radian_valid_min, baselined_ccmc_variable_structure_list );
stat = update_ccmc_variable_attribute_value( "theta3", "valid_max", &radian_valid_max, baselined_ccmc_variable_structure_list );
}

if( evl_x3_position_X3 != NULL )
{
stat = update_ccmc_variable_attribute_value( "phi3", "variable_size", &number_of_cells, baselined_ccmc_variable_structure_list );
stat = update_ccmc_variable_value( "phi3", number_of_cells, evl_x3_position_X3, baselined_ccmc_variable_structure_list );
linear_minmax_search( evl_x3_position_X3, number_of_cells, &actual_min, &actual_max );
stat = update_ccmc_variable_attribute_value( "phi3", "actual_min", &actual_min, baselined_ccmc_variable_structure_list );
stat = update_ccmc_variable_attribute_value( "phi3", "actual_max", &actual_max, baselined_ccmc_variable_structure_list );
stat = update_ccmc_variable_attribute_value( "phi3", "units", radians, baselined_ccmc_variable_structure_list );
stat = update_ccmc_variable_attribute_value( "phi3", "valid_min", &radian_valid_min, baselined_ccmc_variable_structure_list );
stat = update_ccmc_variable_attribute_value( "phi3", "valid_max", &radian_valid_max, baselined_ccmc_variable_structure_list );
}



if( evl_density_D != NULL )
{
stat = update_ccmc_variable_attribute_value( "rho_2", "variable_size", &number_of_cells, baselined_ccmc_variable_structure_list );
stat = update_ccmc_variable_value( "rho_2", number_of_cells, evl_density_D, baselined_ccmc_variable_structure_list );
linear_minmax_search( evl_density_D, number_of_cells, &actual_min, &actual_max );
stat = update_ccmc_variable_attribute_value( "rho_2", "actual_min", &actual_min, baselined_ccmc_variable_structure_list );
stat = update_ccmc_variable_attribute_value( "rho_2", "actual_max", &actual_max, baselined_ccmc_variable_structure_list );
stat = update_ccmc_variable_attribute_value( "rho_2", "units", mass_density, baselined_ccmc_variable_structure_list );
}

if( evl_temperature_T != NULL )
{
stat = update_ccmc_variable_attribute_value( "T_2", "variable_size", &number_of_cells, baselined_ccmc_variable_structure_list );
stat = update_ccmc_variable_value( "T_2", number_of_cells, evl_temperature_T, baselined_ccmc_variable_structure_list );
linear_minmax_search( evl_temperature_T, number_of_cells, &actual_min, &actual_max );
stat = update_ccmc_variable_attribute_value( "T_2", "actual_min", &actual_min, baselined_ccmc_variable_structure_list );
stat = update_ccmc_variable_attribute_value( "T_2", "actual_max", &actual_max, baselined_ccmc_variable_structure_list );
}

if( evl_x1_velocity_V1 != NULL )
{
stat = update_ccmc_variable_attribute_value( "ur_2", "variable_size", &number_of_cells, baselined_ccmc_variable_structure_list );
stat = update_ccmc_variable_value( "ur_2", number_of_cells, evl_x1_velocity_V1, baselined_ccmc_variable_structure_list );
linear_minmax_search( evl_x1_velocity_V1, number_of_cells, &actual_min, &actual_max );
stat = update_ccmc_variable_attribute_value( "ur_2", "actual_min", &actual_min, baselined_ccmc_variable_structure_list );
stat = update_ccmc_variable_attribute_value( "ur_2", "actual_max", &actual_max, baselined_ccmc_variable_structure_list );
stat = update_ccmc_variable_attribute_value( "ur_2", "units", velocity, baselined_ccmc_variable_structure_list );
}

if( evl_x2_velocity_V2 != NULL )
{
stat = update_ccmc_variable_attribute_value( "utheta_2", "variable_size", &number_of_cells, baselined_ccmc_variable_structure_list );
stat = update_ccmc_variable_value( "utheta_2", number_of_cells, evl_x2_velocity_V2, baselined_ccmc_variable_structure_list );
linear_minmax_search( evl_x2_velocity_V2 , number_of_cells, &actual_min, &actual_max );
stat = update_ccmc_variable_attribute_value( "utheta_2", "actual_min", &actual_min, baselined_ccmc_variable_structure_list );
stat = update_ccmc_variable_attribute_value( "utheta_2", "actual_max", &actual_max, baselined_ccmc_variable_structure_list );
stat = update_ccmc_variable_attribute_value( "utheta_2", "units", velocity, baselined_ccmc_variable_structure_list );
}


if( evl_x3_velocity_V3  != NULL )
{
stat = update_ccmc_variable_attribute_value( "uphi_2", "variable_size", &number_of_cells, baselined_ccmc_variable_structure_list );
stat = update_ccmc_variable_value( "uphi_2", number_of_cells, evl_x3_velocity_V3 , baselined_ccmc_variable_structure_list );
linear_minmax_search( evl_x3_velocity_V3, number_of_cells, &actual_min, &actual_max );
stat = update_ccmc_variable_attribute_value( "uphi_2", "actual_min", &actual_min, baselined_ccmc_variable_structure_list );
stat = update_ccmc_variable_attribute_value( "uphi_2", "actual_max", &actual_max, baselined_ccmc_variable_structure_list );
stat = update_ccmc_variable_attribute_value( "uphi_2", "units", velocity, baselined_ccmc_variable_structure_list );
}



if( evl_x1_magnetic_field_B1 != NULL )
{
stat = update_ccmc_variable_attribute_value( "br_2", "variable_size", &number_of_cells, baselined_ccmc_variable_structure_list );
stat = update_ccmc_variable_value( "br_2", number_of_cells, evl_x1_magnetic_field_B1, baselined_ccmc_variable_structure_list );
linear_minmax_search( evl_x1_magnetic_field_B1, number_of_cells, &actual_min, &actual_max );
stat = update_ccmc_variable_attribute_value( "br_2", "actual_min", &actual_min, baselined_ccmc_variable_structure_list );
stat = update_ccmc_variable_attribute_value( "br_2", "actual_max", &actual_max, baselined_ccmc_variable_structure_list );
stat = update_ccmc_variable_attribute_value( "br_2", "units", magnetic_field, baselined_ccmc_variable_structure_list );
stat = update_ccmc_variable_attribute_value( "br_2", "valid_min", &mag_field_T_valid_min, baselined_ccmc_variable_structure_list );
stat = update_ccmc_variable_attribute_value( "br_2", "valid_max", &mag_field_T_valid_max, baselined_ccmc_variable_structure_list );
}


if( evl_x2_magnetic_field_B2 != NULL )
{
stat = update_ccmc_variable_attribute_value( "btheta_2", "variable_size", &number_of_cells, baselined_ccmc_variable_structure_list );
stat = update_ccmc_variable_value( "btheta_2", number_of_cells, evl_x2_magnetic_field_B2, baselined_ccmc_variable_structure_list );
linear_minmax_search( evl_x2_magnetic_field_B2, number_of_cells, &actual_min, &actual_max );
stat = update_ccmc_variable_attribute_value( "btheta_2", "actual_min", &actual_min, baselined_ccmc_variable_structure_list );
stat = update_ccmc_variable_attribute_value( "btheta_2", "actual_max", &actual_max, baselined_ccmc_variable_structure_list );
stat = update_ccmc_variable_attribute_value( "btheta_2", "units", magnetic_field, baselined_ccmc_variable_structure_list );
stat = update_ccmc_variable_attribute_value( "btheta_2", "valid_min", &mag_field_T_valid_min, baselined_ccmc_variable_structure_list );
stat = update_ccmc_variable_attribute_value( "btheta_2", "valid_max", &mag_field_T_valid_max, baselined_ccmc_variable_structure_list );
}

if( evl_x3_magnetic_field_B3 != NULL )
{
stat = update_ccmc_variable_attribute_value( "bphi_2", "variable_size", &number_of_cells, baselined_ccmc_variable_structure_list );
stat = update_ccmc_variable_value( "bphi_2", number_of_cells, evl_x3_magnetic_field_B3, baselined_ccmc_variable_structure_list );
linear_minmax_search( evl_x3_magnetic_field_B3, number_of_cells, &actual_min, &actual_max );
stat = update_ccmc_variable_attribute_value( "bphi_2", "actual_min", &actual_min, baselined_ccmc_variable_structure_list );
stat = update_ccmc_variable_attribute_value( "bphi_2", "actual_max", &actual_max, baselined_ccmc_variable_structure_list );
stat = update_ccmc_variable_attribute_value( "bphi_2", "units", magnetic_field, baselined_ccmc_variable_structure_list );
stat = update_ccmc_variable_attribute_value( "bphi_2", "valid_min", &mag_field_T_valid_min, baselined_ccmc_variable_structure_list );
stat = update_ccmc_variable_attribute_value( "bphi_2", "valid_max", &mag_field_T_valid_max, baselined_ccmc_variable_structure_list );
}



if( evl_cloud_mass_density_DP != NULL )
{
stat = update_ccmc_variable_attribute_value( "dp_2", "variable_size", &number_of_cells, baselined_ccmc_variable_structure_list );
stat = update_ccmc_variable_value( "dp_2", number_of_cells, evl_cloud_mass_density_DP, baselined_ccmc_variable_structure_list );
linear_minmax_search( evl_cloud_mass_density_DP, number_of_cells, &actual_min, &actual_max );
stat = update_ccmc_variable_attribute_value( "dp_2", "actual_min", &actual_min, baselined_ccmc_variable_structure_list );
stat = update_ccmc_variable_attribute_value( "dp_2", "actual_max", &actual_max, baselined_ccmc_variable_structure_list );
stat = update_ccmc_variable_attribute_value( "dp_2", "units", mass_density, baselined_ccmc_variable_structure_list );
}

if( evl_polarity_of_magnetic_field_BP != NULL )
{
stat = update_ccmc_variable_attribute_value( "bp_2", "variable_size", &number_of_cells, baselined_ccmc_variable_structure_list );
stat = update_ccmc_variable_value( "bp_2", number_of_cells, evl_polarity_of_magnetic_field_BP, baselined_ccmc_variable_structure_list );
linear_minmax_search( evl_polarity_of_magnetic_field_BP, number_of_cells, &actual_min, &actual_max );
stat = update_ccmc_variable_attribute_value( "bp_2", "actual_min", &actual_min, baselined_ccmc_variable_structure_list );
stat = update_ccmc_variable_attribute_value( "bp_2", "actual_max", &actual_max, baselined_ccmc_variable_structure_list );
stat = update_ccmc_variable_attribute_value( "bp_2", "valid_min", &polarity_valid_min, baselined_ccmc_variable_structure_list );
stat = update_ccmc_variable_attribute_value( "bp_2", "valid_max", &polarity_valid_max, baselined_ccmc_variable_structure_list );
}

if( evl_physical_time_TIME != NULL )
{
stat = update_ccmc_variable_attribute_value( "time_2", "variable_size", &evl_nevo, baselined_ccmc_variable_structure_list );
stat = update_ccmc_variable_value( "time_2", evl_nevo, evl_physical_time_TIME, baselined_ccmc_variable_structure_list );
linear_minmax_search( evl_physical_time_TIME, evl_nevo, &actual_min, &actual_max );
stat = update_ccmc_variable_attribute_value( "time_2", "actual_min", &actual_min, baselined_ccmc_variable_structure_list );
stat = update_ccmc_variable_attribute_value( "time_2", "actual_max", &actual_max, baselined_ccmc_variable_structure_list );
}

if( evl_physical_time_step_DT != NULL )
{
stat = update_ccmc_variable_attribute_value( "time_step_2", "variable_size", &evl_nevo, baselined_ccmc_variable_structure_list );
stat = update_ccmc_variable_value( "time_step_2", evl_nevo, evl_physical_time_step_DT, baselined_ccmc_variable_structure_list );
linear_minmax_search( evl_physical_time_step_DT, evl_nevo, &actual_min, &actual_max );
stat = update_ccmc_variable_attribute_value( "time_step_2", "actual_min", &actual_min, baselined_ccmc_variable_structure_list );
stat = update_ccmc_variable_attribute_value( "time_step_2", "actual_max", &actual_max, baselined_ccmc_variable_structure_list );
}

if( evl_numerical_time_step_NSTEP != NULL )
{
stat = update_ccmc_variable_attribute_value( "numerical_step_2", "variable_size", &evl_nevo, baselined_ccmc_variable_structure_list );
stat = update_ccmc_variable_value( "numerical_step_2", evl_nevo, evl_numerical_time_step_NSTEP, baselined_ccmc_variable_structure_list );
linear_minmax_search_int( evl_numerical_time_step_NSTEP, evl_nevo, &actual_min_int, &actual_max_int );

/** copy and type cast actual_m*_int into actual_m* **/
actual_min = (float) actual_min_int;
actual_max = (float) actual_max_int;

stat = update_ccmc_variable_attribute_value( "numerical_step_2", "actual_min", &actual_min, baselined_ccmc_variable_structure_list );
stat = update_ccmc_variable_attribute_value( "numerical_step_2", "actual_max", &actual_max, baselined_ccmc_variable_structure_list );
}


/************************************************
 *                 EVG FILE                     *
 * *********************************************/


/** calculate the number of evg file cells **/

number_of_cells = evg_ngeo * evg_nevo;

if ( verbose_flag ) { printf("\nwritting original evg file model output variables values into baselined_ccmc_variable_structure_list...\n"); }

/*** update each variable size as the defualts are set to zero, this reallocs space for the actual values to be stored *****/

if( evg_x1_position_X1 != NULL )
{
stat = update_ccmc_variable_attribute_value( "r4", "variable_size", &number_of_cells, baselined_ccmc_variable_structure_list );
stat = update_ccmc_variable_value( "r4", number_of_cells, evg_x1_position_X1, baselined_ccmc_variable_structure_list );
linear_minmax_search( evg_x1_position_X1, number_of_cells, &actual_min, &actual_max );
stat = update_ccmc_variable_attribute_value( "r4", "actual_min", &actual_min, baselined_ccmc_variable_structure_list );
stat = update_ccmc_variable_attribute_value( "r4", "actual_max", &actual_max, baselined_ccmc_variable_structure_list );
stat = update_ccmc_variable_attribute_value( "r4", "units", meters, baselined_ccmc_variable_structure_list );
stat = update_ccmc_variable_attribute_value( "r4", "valid_min", &radius_m_valid_min, baselined_ccmc_variable_structure_list );
stat = update_ccmc_variable_attribute_value( "r4", "valid_max", &radius_m_valid_max, baselined_ccmc_variable_structure_list );
}

if( evg_x2_position_X2 != NULL )
{
stat = update_ccmc_variable_attribute_value( "theta4", "variable_size", &number_of_cells, baselined_ccmc_variable_structure_list );
stat = update_ccmc_variable_value( "theta4", number_of_cells, evg_x2_position_X2, baselined_ccmc_variable_structure_list );
linear_minmax_search( evg_x2_position_X2, number_of_cells, &actual_min, &actual_max );
stat = update_ccmc_variable_attribute_value( "theta4", "actual_min", &actual_min, baselined_ccmc_variable_structure_list );
stat = update_ccmc_variable_attribute_value( "theta4", "actual_max", &actual_max, baselined_ccmc_variable_structure_list );
stat = update_ccmc_variable_attribute_value( "theta4", "units", radians, baselined_ccmc_variable_structure_list );
stat = update_ccmc_variable_attribute_value( "theta4", "valid_min", &radian_valid_min, baselined_ccmc_variable_structure_list );
stat = update_ccmc_variable_attribute_value( "theta4", "valid_max", &radian_valid_max, baselined_ccmc_variable_structure_list );
}

if( evg_x3_position_X3 != NULL )
{
stat = update_ccmc_variable_attribute_value( "phi4", "variable_size", &number_of_cells, baselined_ccmc_variable_structure_list );
stat = update_ccmc_variable_value( "phi4", number_of_cells, evg_x3_position_X3, baselined_ccmc_variable_structure_list );
linear_minmax_search( evg_x3_position_X3, number_of_cells, &actual_min, &actual_max );
stat = update_ccmc_variable_attribute_value( "phi4", "actual_min", &actual_min, baselined_ccmc_variable_structure_list );
stat = update_ccmc_variable_attribute_value( "phi4", "actual_max", &actual_max, baselined_ccmc_variable_structure_list );
stat = update_ccmc_variable_attribute_value( "phi4", "units", radians, baselined_ccmc_variable_structure_list );
stat = update_ccmc_variable_attribute_value( "phi4", "valid_min", &radian_valid_min, baselined_ccmc_variable_structure_list );
stat = update_ccmc_variable_attribute_value( "phi4", "valid_max", &radian_valid_max, baselined_ccmc_variable_structure_list );
}

if( evg_density_D != NULL )
{
stat = update_ccmc_variable_attribute_value( "rho_3", "variable_size", &number_of_cells, baselined_ccmc_variable_structure_list );
stat = update_ccmc_variable_value( "rho_3", number_of_cells, evg_density_D, baselined_ccmc_variable_structure_list );
linear_minmax_search( evg_density_D, number_of_cells, &actual_min, &actual_max );
stat = update_ccmc_variable_attribute_value( "rho_3", "actual_min", &actual_min, baselined_ccmc_variable_structure_list );
stat = update_ccmc_variable_attribute_value( "rho_3", "actual_max", &actual_max, baselined_ccmc_variable_structure_list );
stat = update_ccmc_variable_attribute_value( "rho_3", "units", mass_density, baselined_ccmc_variable_structure_list );
}

if( evg_temperature_T != NULL )
{
stat = update_ccmc_variable_attribute_value( "T_3", "variable_size", &number_of_cells, baselined_ccmc_variable_structure_list );
stat = update_ccmc_variable_value( "T_3", number_of_cells, evg_temperature_T, baselined_ccmc_variable_structure_list );
linear_minmax_search( evg_temperature_T, number_of_cells, &actual_min, &actual_max );
stat = update_ccmc_variable_attribute_value( "T_3", "actual_min", &actual_min, baselined_ccmc_variable_structure_list );
stat = update_ccmc_variable_attribute_value( "T_3", "actual_max", &actual_max, baselined_ccmc_variable_structure_list );
}

if( evg_x1_velocity_V1 != NULL )
{
stat = update_ccmc_variable_attribute_value( "ur_3", "variable_size", &number_of_cells, baselined_ccmc_variable_structure_list );
stat = update_ccmc_variable_value( "ur_3", number_of_cells, evg_x1_velocity_V1, baselined_ccmc_variable_structure_list );
linear_minmax_search( evg_x1_velocity_V1, number_of_cells, &actual_min, &actual_max );
stat = update_ccmc_variable_attribute_value( "ur_3", "actual_min", &actual_min, baselined_ccmc_variable_structure_list );
stat = update_ccmc_variable_attribute_value( "ur_3", "actual_max", &actual_max, baselined_ccmc_variable_structure_list );
stat = update_ccmc_variable_attribute_value( "ur_3", "units", velocity, baselined_ccmc_variable_structure_list );
}

if( evg_x2_velocity_V2 != NULL )
{
stat = update_ccmc_variable_attribute_value( "utheta_3", "variable_size",
      &number_of_cells, baselined_ccmc_variable_structure_list );
stat = update_ccmc_variable_value( "utheta_3", number_of_cells,
      evg_x2_velocity_V2, baselined_ccmc_variable_structure_list );
linear_minmax_search( evg_x2_velocity_V2, number_of_cells, &actual_min,
      &actual_max );
stat = update_ccmc_variable_attribute_value( "utheta_3", "actual_min",
      &actual_min, baselined_ccmc_variable_structure_list );
stat = update_ccmc_variable_attribute_value( "utheta_3", "actual_max",
      &actual_max, baselined_ccmc_variable_structure_list );
stat = update_ccmc_variable_attribute_value( "utheta_3", "units", velocity,
      baselined_ccmc_variable_structure_list );
}

if( evg_x3_velocity_V3 != NULL )
{
stat = update_ccmc_variable_attribute_value( "uphi_3", "variable_size",
      &number_of_cells, baselined_ccmc_variable_structure_list );
stat = update_ccmc_variable_value( "uphi_3", number_of_cells,
      evg_x3_velocity_V3, baselined_ccmc_variable_structure_list );
linear_minmax_search( evg_x3_velocity_V3, number_of_cells, &actual_min,
      &actual_max );
stat = update_ccmc_variable_attribute_value( "uphi_3", "actual_min",
      &actual_min, baselined_ccmc_variable_structure_list );
stat = update_ccmc_variable_attribute_value( "uphi_3", "actual_max",
      &actual_max, baselined_ccmc_variable_structure_list );
stat = update_ccmc_variable_attribute_value( "uphi_3", "units", velocity,
      baselined_ccmc_variable_structure_list );
}



if( evg_x1_magnetic_field_B1 != NULL )
{
stat = update_ccmc_variable_attribute_value( "br_3", "variable_size",
      &number_of_cells, baselined_ccmc_variable_structure_list );
stat = update_ccmc_variable_value( "br_3", number_of_cells,
      evg_x1_magnetic_field_B1, baselined_ccmc_variable_structure_list );
linear_minmax_search( evg_x1_magnetic_field_B1, number_of_cells, &actual_min,
      &actual_max );
stat = update_ccmc_variable_attribute_value( "br_3", "actual_min", &actual_min,
      baselined_ccmc_variable_structure_list );
stat = update_ccmc_variable_attribute_value( "br_3", "actual_max", &actual_max,
      baselined_ccmc_variable_structure_list );
stat = update_ccmc_variable_attribute_value( "br_3", "units", magnetic_field,
      baselined_ccmc_variable_structure_list );
stat = update_ccmc_variable_attribute_value( "br_3", "valid_min",
      &mag_field_T_valid_min, baselined_ccmc_variable_structure_list );
stat = update_ccmc_variable_attribute_value( "br_3", "valid_max",
      &mag_field_T_valid_max, baselined_ccmc_variable_structure_list );
}

if( evg_x2_magnetic_field_B2 != NULL )
{
stat = update_ccmc_variable_attribute_value( "btheta_3", "variable_size",
      &number_of_cells, baselined_ccmc_variable_structure_list );
stat = update_ccmc_variable_value( "btheta_3", number_of_cells,
      evg_x2_magnetic_field_B2, baselined_ccmc_variable_structure_list );
linear_minmax_search( evg_x2_magnetic_field_B2, number_of_cells, &actual_min,
      &actual_max );
stat = update_ccmc_variable_attribute_value( "btheta_3", "actual_min",
      &actual_min, baselined_ccmc_variable_structure_list );
stat = update_ccmc_variable_attribute_value( "btheta_3", "actual_max",
      &actual_max, baselined_ccmc_variable_structure_list );
stat = update_ccmc_variable_attribute_value( "btheta_3", "units",
      magnetic_field, baselined_ccmc_variable_structure_list );
stat = update_ccmc_variable_attribute_value( "btheta_3", "valid_min",
      &mag_field_T_valid_min, baselined_ccmc_variable_structure_list );
stat = update_ccmc_variable_attribute_value( "btheta_3", "valid_max",
      &mag_field_T_valid_max, baselined_ccmc_variable_structure_list );
}

if( evg_x3_magnetic_field_B3 != NULL )
{
stat = update_ccmc_variable_attribute_value( "bphi_3", "variable_size",
      &number_of_cells, baselined_ccmc_variable_structure_list );
stat = update_ccmc_variable_value( "bphi_3", number_of_cells,
      evg_x3_magnetic_field_B3, baselined_ccmc_variable_structure_list );
linear_minmax_search( evg_x3_magnetic_field_B3, number_of_cells, &actual_min,
      &actual_max );
stat = update_ccmc_variable_attribute_value( "bphi_3", "actual_min",
      &actual_min, baselined_ccmc_variable_structure_list );
stat = update_ccmc_variable_attribute_value( "bphi_3", "actual_max",
      &actual_max, baselined_ccmc_variable_structure_list );
stat = update_ccmc_variable_attribute_value( "bphi_3", "units", magnetic_field,
      baselined_ccmc_variable_structure_list );
stat = update_ccmc_variable_attribute_value( "bphi_3", "valid_min",
      &mag_field_T_valid_min, baselined_ccmc_variable_structure_list );
stat = update_ccmc_variable_attribute_value( "bphi_3", "valid_max",
      &mag_field_T_valid_max, baselined_ccmc_variable_structure_list );
}



if( evg_cloud_mass_density_DP != NULL )
{
stat = update_ccmc_variable_attribute_value( "dp_3", "variable_size",
      &number_of_cells, baselined_ccmc_variable_structure_list );
stat = update_ccmc_variable_value( "dp_3", number_of_cells,
      evg_cloud_mass_density_DP, baselined_ccmc_variable_structure_list );
linear_minmax_search( evg_cloud_mass_density_DP, number_of_cells, &actual_min,
      &actual_max );
stat = update_ccmc_variable_attribute_value( "dp_3", "actual_min", &actual_min,
      baselined_ccmc_variable_structure_list );
stat = update_ccmc_variable_attribute_value( "dp_3", "actual_max", &actual_max,
      baselined_ccmc_variable_structure_list );
stat = update_ccmc_variable_attribute_value( "rho_3", "units", mass_density,
      baselined_ccmc_variable_structure_list );
}

if( evg_polarity_of_magnetic_field_BP != NULL )
{
stat = update_ccmc_variable_attribute_value( "bp_3", "variable_size",
      &number_of_cells, baselined_ccmc_variable_structure_list );
stat = update_ccmc_variable_value( "bp_3", number_of_cells,
      evg_polarity_of_magnetic_field_BP, baselined_ccmc_variable_structure_list );
linear_minmax_search( evg_polarity_of_magnetic_field_BP, number_of_cells,
      &actual_min, &actual_max );
stat = update_ccmc_variable_attribute_value( "bp_3", "actual_min",
      &actual_min, baselined_ccmc_variable_structure_list );
stat = update_ccmc_variable_attribute_value( "bp_3", "actual_max",
      &actual_max, baselined_ccmc_variable_structure_list );
stat = update_ccmc_variable_attribute_value( "bp_3", "valid_min",
      &polarity_valid_min, baselined_ccmc_variable_structure_list );
stat = update_ccmc_variable_attribute_value( "bp_3", "valid_max",
      &polarity_valid_max, baselined_ccmc_variable_structure_list );
}

if( evg_physical_time_TIME != NULL )
{
stat = update_ccmc_variable_attribute_value( "time_3", "variable_size",
      &evg_nevo, baselined_ccmc_variable_structure_list );
stat = update_ccmc_variable_value( "time_3", evg_nevo, evg_physical_time_TIME,
      baselined_ccmc_variable_structure_list );
linear_minmax_search( evg_physical_time_TIME, evg_nevo, &actual_min,
      &actual_max );
stat = update_ccmc_variable_attribute_value( "time_3", "actual_min",
      &actual_min, baselined_ccmc_variable_structure_list );
stat = update_ccmc_variable_attribute_value( "time_3", "actual_max",
      &actual_max, baselined_ccmc_variable_structure_list );
}

if( evg_physical_time_step_DT != NULL )
{
stat = update_ccmc_variable_attribute_value( "time_step_3",
      "variable_size", &evg_nevo, baselined_ccmc_variable_structure_list );
stat = update_ccmc_variable_value( "time_step_3", evg_nevo,
      evg_physical_time_step_DT, baselined_ccmc_variable_structure_list );
linear_minmax_search( evg_physical_time_step_DT, evg_nevo, &actual_min,
      &actual_max );
stat = update_ccmc_variable_attribute_value( "time_step_3", "actual_min",
      &actual_min, baselined_ccmc_variable_structure_list );
stat = update_ccmc_variable_attribute_value( "time_step_3", "actual_max",
      &actual_max, baselined_ccmc_variable_structure_list );
}

if( evg_numerical_time_step_NSTEP != NULL )
{
stat = update_ccmc_variable_attribute_value( "numerical_step_3",
      "variable_size", &evg_nevo, baselined_ccmc_variable_structure_list );
stat = update_ccmc_variable_value( "numerical_step_3", evg_nevo,
      evg_numerical_time_step_NSTEP, baselined_ccmc_variable_structure_list );
linear_minmax_search_int( evg_numerical_time_step_NSTEP, evg_nevo,
      &actual_min_int, &actual_max_int );

/** copy and type cast actual_m*_int into actual_m* **/
actual_min = (float) actual_min_int;
actual_max = (float) actual_max_int;

stat = update_ccmc_variable_attribute_value( "numerical_step_3", "actual_min",
      &actual_min, baselined_ccmc_variable_structure_list );
stat = update_ccmc_variable_attribute_value( "numerical_step_3", "actual_max",
      &actual_max, baselined_ccmc_variable_structure_list );
}

/************************************************
 *                 EVP FILE                     *
 * *********************************************/


/** calculate the number of evp file cells **/

number_of_cells = evp_npla * evp_nevo;

if ( verbose_flag ) { printf("\nwritting original evp file model output "
      "variables values into baselined_ccmc_variable_structure_list...\n"); }

/*** update each variable size as the defualts are set to zero, this reallocs
 * space for the actual values to be stored *****/

if( evp_x1_position_X1 != NULL )
{
stat = update_ccmc_variable_attribute_value( "r5", "variable_size",
      &number_of_cells, baselined_ccmc_variable_structure_list );
stat = update_ccmc_variable_value( "r5", number_of_cells, evp_x1_position_X1,
      baselined_ccmc_variable_structure_list );
linear_minmax_search( evp_x1_position_X1, number_of_cells, &actual_min,
      &actual_max );
stat = update_ccmc_variable_attribute_value( "r5", "actual_min",
      &actual_min, baselined_ccmc_variable_structure_list );
stat = update_ccmc_variable_attribute_value( "r5", "actual_max",
      &actual_max, baselined_ccmc_variable_structure_list );
stat = update_ccmc_variable_attribute_value( "r5", "units", meters,
      baselined_ccmc_variable_structure_list );
stat = update_ccmc_variable_attribute_value( "r5", "valid_min",
      &radius_m_valid_min, baselined_ccmc_variable_structure_list );
stat = update_ccmc_variable_attribute_value( "r5", "valid_max",
      &radius_m_valid_max, baselined_ccmc_variable_structure_list );
}

if( evp_x2_position_X2 != NULL )
{
stat = update_ccmc_variable_attribute_value( "theta5", "variable_size",
      &number_of_cells, baselined_ccmc_variable_structure_list );
stat = update_ccmc_variable_value( "theta5", number_of_cells, evp_x2_position_X2,
      baselined_ccmc_variable_structure_list );
linear_minmax_search( evp_x2_position_X2, number_of_cells, &actual_min,
      &actual_max );
stat = update_ccmc_variable_attribute_value( "theta5", "actual_min",
      &actual_min, baselined_ccmc_variable_structure_list );
stat = update_ccmc_variable_attribute_value( "theta5", "actual_max",
      &actual_max, baselined_ccmc_variable_structure_list );
stat = update_ccmc_variable_attribute_value( "theta5", "units", radians,
      baselined_ccmc_variable_structure_list );
}

if( evp_x3_position_X3 != NULL )
{
stat = update_ccmc_variable_attribute_value( "phi5", "variable_size",
      &number_of_cells, baselined_ccmc_variable_structure_list );
stat = update_ccmc_variable_value( "phi5", number_of_cells, evp_x3_position_X3,
      baselined_ccmc_variable_structure_list );
linear_minmax_search( evp_x3_position_X3, number_of_cells, &actual_min,
      &actual_max );
stat = update_ccmc_variable_attribute_value( "phi5", "actual_min", &actual_min,
      baselined_ccmc_variable_structure_list );
stat = update_ccmc_variable_attribute_value( "phi5", "actual_max", &actual_max,
      baselined_ccmc_variable_structure_list );
stat = update_ccmc_variable_attribute_value( "phi5", "units", radians,
      baselined_ccmc_variable_structure_list );
}



if( evp_density_D != NULL )
{
stat = update_ccmc_variable_attribute_value( "rho_4", "variable_size",
      &number_of_cells, baselined_ccmc_variable_structure_list );
stat = update_ccmc_variable_value( "rho_4", number_of_cells, evp_density_D,
      baselined_ccmc_variable_structure_list );
linear_minmax_search( evp_density_D, number_of_cells, &actual_min, &actual_max );
stat = update_ccmc_variable_attribute_value( "rho_4", "actual_min", &actual_min,
      baselined_ccmc_variable_structure_list );
stat = update_ccmc_variable_attribute_value( "rho_4", "actual_max", &actual_max,
      baselined_ccmc_variable_structure_list );
stat = update_ccmc_variable_attribute_value( "rho_4", "units", mass_density,
      baselined_ccmc_variable_structure_list );
}

if( evp_temperature_T != NULL )
{
stat = update_ccmc_variable_attribute_value( "T_4", "variable_size",
      &number_of_cells, baselined_ccmc_variable_structure_list );
stat = update_ccmc_variable_value( "T_4", number_of_cells, evp_temperature_T,
      baselined_ccmc_variable_structure_list );
linear_minmax_search( evp_temperature_T, number_of_cells, &actual_min,
      &actual_max );
stat = update_ccmc_variable_attribute_value( "T_4", "actual_min", &actual_min,
      baselined_ccmc_variable_structure_list );
stat = update_ccmc_variable_attribute_value( "T_4", "actual_max", &actual_max,
      baselined_ccmc_variable_structure_list );
}

if( evp_x1_velocity_V1 != NULL )
{
stat = update_ccmc_variable_attribute_value( "ur_4", "variable_size",
        &number_of_cells, baselined_ccmc_variable_structure_list );
stat = update_ccmc_variable_value( "ur_4", number_of_cells, evp_x1_velocity_V1,
        baselined_ccmc_variable_structure_list );
linear_minmax_search( evp_x1_velocity_V1, number_of_cells, &actual_min,
        &actual_max );
stat = update_ccmc_variable_attribute_value( "ur_4", "actual_min", &actual_min,
        baselined_ccmc_variable_structure_list );
stat = update_ccmc_variable_attribute_value( "ur_4", "actual_max", &actual_max,
        baselined_ccmc_variable_structure_list );
stat = update_ccmc_variable_attribute_value( "ur_4", "units", velocity,
        baselined_ccmc_variable_structure_list );
}

if( evp_x2_velocity_V2 != NULL )
{
stat = update_ccmc_variable_attribute_value( "utheta_4", "variable_size",
        &number_of_cells, baselined_ccmc_variable_structure_list );

stat = update_ccmc_variable_value( "utheta_4", number_of_cells,
      evp_x2_velocity_V2, baselined_ccmc_variable_structure_list );

linear_minmax_search( evp_x2_velocity_V2, number_of_cells, &actual_min,
        &actual_max );

stat = update_ccmc_variable_attribute_value( "utheta_4", "actual_min",
        &actual_min, baselined_ccmc_variable_structure_list );

stat = update_ccmc_variable_attribute_value( "utheta_4", "actual_max",
        &actual_max, baselined_ccmc_variable_structure_list );

stat = update_ccmc_variable_attribute_value( "utheta_4", "units", velocity,
        baselined_ccmc_variable_structure_list );
}

if( evp_x3_velocity_V3 != NULL )
{
stat = update_ccmc_variable_attribute_value( "uphi_4", "variable_size",
        &number_of_cells, baselined_ccmc_variable_structure_list );
stat = update_ccmc_variable_value( "uphi_4", number_of_cells,
      evp_x3_velocity_V3, baselined_ccmc_variable_structure_list );
linear_minmax_search( evp_x3_velocity_V3, number_of_cells, &actual_min,
        &actual_max );
stat = update_ccmc_variable_attribute_value( "uphi_4", "actual_min",
        &actual_min, baselined_ccmc_variable_structure_list );
stat = update_ccmc_variable_attribute_value( "uphi_4", "actual_max",
        &actual_max, baselined_ccmc_variable_structure_list );
stat = update_ccmc_variable_attribute_value( "uphi_4", "units", velocity,
        baselined_ccmc_variable_structure_list );
}



if( evp_x1_magnetic_field_B1 != NULL )
{
stat = update_ccmc_variable_attribute_value( "br_4", "variable_size",
        &number_of_cells, baselined_ccmc_variable_structure_list );
stat = update_ccmc_variable_value( "br_4", number_of_cells,
        evp_x1_magnetic_field_B1, baselined_ccmc_variable_structure_list );
linear_minmax_search( evp_x1_magnetic_field_B1, number_of_cells,
        &actual_min, &actual_max );
stat = update_ccmc_variable_attribute_value( "br_4", "actual_min",
        &actual_min, baselined_ccmc_variable_structure_list );
stat = update_ccmc_variable_attribute_value( "br_4", "actual_max",
        &actual_max, baselined_ccmc_variable_structure_list );
stat = update_ccmc_variable_attribute_value( "br_4", "units",
        magnetic_field, baselined_ccmc_variable_structure_list );
}

if( evp_x2_magnetic_field_B2 != NULL )
{

stat = update_ccmc_variable_attribute_value( "btheta_4", "variable_size",
        &number_of_cells, baselined_ccmc_variable_structure_list );

stat = update_ccmc_variable_value( "btheta_4", number_of_cells,
      evp_x2_magnetic_field_B2, baselined_ccmc_variable_structure_list );

linear_minmax_search( evp_x2_magnetic_field_B2, number_of_cells,
                &actual_min, &actual_max );

stat = update_ccmc_variable_attribute_value( "btheta_4", "actual_min",
        &actual_min, baselined_ccmc_variable_structure_list );

stat = update_ccmc_variable_attribute_value( "btheta_4", "actual_max",
        &actual_max, baselined_ccmc_variable_structure_list );

stat = update_ccmc_variable_attribute_value( "btheta_4", "units",
        magnetic_field, baselined_ccmc_variable_structure_list );
}

if(  evp_x3_magnetic_field_B3 != NULL )
{
stat = update_ccmc_variable_attribute_value( "bphi_4", "variable_size",
        &number_of_cells, baselined_ccmc_variable_structure_list );

stat = update_ccmc_variable_value( "bphi_4", number_of_cells,
      evp_x3_magnetic_field_B3, baselined_ccmc_variable_structure_list );

linear_minmax_search( evp_x3_magnetic_field_B3, number_of_cells,
        &actual_min, &actual_max );
stat = update_ccmc_variable_attribute_value( "bphi_4", "actual_min",
        &actual_min, baselined_ccmc_variable_structure_list );
stat = update_ccmc_variable_attribute_value( "bphi_4", "actual_max",
        &actual_max, baselined_ccmc_variable_structure_list );
stat = update_ccmc_variable_attribute_value( "bphi_4", "units", magnetic_field,
        baselined_ccmc_variable_structure_list );
}



if( evp_cloud_mass_density_DP != NULL )
{
stat = update_ccmc_variable_attribute_value( "dp_4", "variable_size",
        &number_of_cells, baselined_ccmc_variable_structure_list );
stat = update_ccmc_variable_value( "dp_4", number_of_cells,
        evp_cloud_mass_density_DP, baselined_ccmc_variable_structure_list );
linear_minmax_search( evp_cloud_mass_density_DP, number_of_cells,
        &actual_min, &actual_max );
stat = update_ccmc_variable_attribute_value( "dp_4", "actual_min",
        &actual_min, baselined_ccmc_variable_structure_list );
stat = update_ccmc_variable_attribute_value( "dp_4", "actual_max",
        &actual_max, baselined_ccmc_variable_structure_list );
stat = update_ccmc_variable_attribute_value( "dp_4", "units", mass_density,
        baselined_ccmc_variable_structure_list );
}

if( evp_polarity_of_magnetic_field_BP != NULL )
{
stat = update_ccmc_variable_attribute_value( "bp_4", "variable_size",
        &number_of_cells, baselined_ccmc_variable_structure_list );
stat = update_ccmc_variable_value( "bp_4", number_of_cells,
        evp_polarity_of_magnetic_field_BP, baselined_ccmc_variable_structure_list );
linear_minmax_search( evp_polarity_of_magnetic_field_BP, number_of_cells,
        &actual_min, &actual_max );
stat = update_ccmc_variable_attribute_value( "bp_4", "actual_min",
        &actual_min, baselined_ccmc_variable_structure_list );
stat = update_ccmc_variable_attribute_value( "bp_4", "actual_max",
        &actual_max, baselined_ccmc_variable_structure_list );
stat = update_ccmc_variable_attribute_value( "bp_4", "valid_min",
        &polarity_valid_min, baselined_ccmc_variable_structure_list );
stat = update_ccmc_variable_attribute_value( "bp_4", "valid_max",
        &polarity_valid_max, baselined_ccmc_variable_structure_list );
}

if( evp_physical_time_TIME != NULL )
{
stat = update_ccmc_variable_attribute_value( "time_4", "variable_size",
      &evp_nevo, baselined_ccmc_variable_structure_list );

stat = update_ccmc_variable_value( "time_4", evp_nevo, evp_physical_time_TIME,
      baselined_ccmc_variable_structure_list );

linear_minmax_search( evp_physical_time_TIME, evp_nevo, &actual_min,
      &actual_max );

stat = update_ccmc_variable_attribute_value( "time_4", "actual_min",
      &actual_min, baselined_ccmc_variable_structure_list );

stat = update_ccmc_variable_attribute_value( "time_4", "actual_max",
      &actual_max, baselined_ccmc_variable_structure_list );
}

if( evp_physical_time_step_DT != NULL )
{
stat = update_ccmc_variable_attribute_value( "time_step_4", "variable_size",
      &evp_nevo, baselined_ccmc_variable_structure_list );

stat = update_ccmc_variable_value( "time_step_4", evp_nevo,
      evp_physical_time_step_DT, baselined_ccmc_variable_structure_list );

linear_minmax_search( evp_physical_time_step_DT, evp_nevo, &actual_min,
      &actual_max );

stat = update_ccmc_variable_attribute_value( "time_step_4", "actual_min",
      &actual_min, baselined_ccmc_variable_structure_list );

stat = update_ccmc_variable_attribute_value( "time_step_4", "actual_max",
      &actual_max, baselined_ccmc_variable_structure_list );
}

if( evp_numerical_time_step_NSTEP != NULL )
{
stat = update_ccmc_variable_attribute_value( "numerical_step_4", "variable_size"
      , &evp_nevo, baselined_ccmc_variable_structure_list );

stat = update_ccmc_variable_value( "numerical_step_4", evp_nevo,
      evp_numerical_time_step_NSTEP, baselined_ccmc_variable_structure_list );

linear_minmax_search_int( evp_numerical_time_step_NSTEP, evp_nevo,
      &actual_min_int, &actual_max_int );

/** copy and type cast actual_m*_int into actual_m* **/
actual_min = (float) actual_min_int;
actual_max = (float) actual_max_int;


stat = update_ccmc_variable_attribute_value( "numerical_step_4",
      "actual_min", &actual_min, baselined_ccmc_variable_structure_list );

stat = update_ccmc_variable_attribute_value( "numerical_step_4",
      "actual_max", &actual_max, baselined_ccmc_variable_structure_list );
}

if ( verbose_flag ) { printf("cleaning memory allocated for initial read "
      "routine...\n"); }

/********* free all pointers *******/

free( tim_x1_position_X1);
free( tim_x2_position_X2);
free( tim_x3_position_X3);
free( tim_x1h_position_X1H);
free( tim_x2h_position_X2H);
free( tim_x3h_position_X3H);
free( tim_x1_position_X1_long_name_vattr);
free( tim_x2_position_X2_long_name_vattr);
free( tim_x3_position_X3_long_name_vattr);
free( tim_x1h_position_X1H_long_name_vattr);
free( tim_x2h_position_X2H_long_name_vattr);
free( tim_x3h_position_X3H_long_name_vattr);
free( tim_x1_position_X1_units_vattr);
free( tim_x2_position_X2_units_vattr);
free( tim_x3_position_X3_units_vattr);
free( tim_x1h_position_X1H_units_vattr);
free( tim_x2h_position_X2H_units_vattr);
free( tim_x3h_position_X3H_units_vattr);
free( tim_physical_time_TIME);
free( tim_physical_time_step_DT);
free( tim_numerical_time_step_NSTEP);
free( tim_physical_time_TIME_long_name_vattr);
free( tim_physical_time_step_DT_long_name_vattr);
free( tim_numerical_time_step_NSTEP_long_name_vattr);
free( tim_physical_time_TIME_units_vattr);
free( tim_physical_time_step_DT_units_vattr);
free( tim_numerical_time_step_NSTEP_units_vattr);
free( tim_density_D);
free( tim_temperature_T);
free( tim_x1_velocity_V1);
free( tim_x2_velocity_V2);
free( tim_x3_velocity_V3);
free( tim_x1_magnetic_field_B1);
free( tim_x2_magnetic_field_B2);
free( tim_x3_magnetic_field_B3);
free( tim_cloud_mass_density_DP);
free( tim_polarity_of_magnetic_field_BP);
free( tim_density_D_long_name_vattr);
free( tim_temperature_T_long_name_vattr);
free( tim_x1_velocity_V1_long_name_vattr);
free( tim_x2_velocity_V2_long_name_vattr);
free( tim_x3_velocity_V3_long_name_vattr);
free( tim_x1_magnetic_field_B1_long_name_vattr);
free( tim_x2_magnetic_field_B2_long_name_vattr);
free( tim_x3_magnetic_field_B3_long_name_vattr);
free( tim_cloud_mass_density_DP_long_name_vattr);
free( tim_polarity_of_magnetic_field_BP_long_name_vattr);
free( tim_density_D_units_vattr);
free( tim_temperature_T_units_vattr);
free( tim_x1_velocity_V1_units_vattr);
free( tim_x2_velocity_V2_units_vattr);
free( tim_x3_velocity_V3_units_vattr);
free( tim_x1_magnetic_field_B1_units_vattr);
free( tim_x2_magnetic_field_B2_units_vattr);
free( tim_x3_magnetic_field_B3_units_vattr);
free( tim_cloud_mass_density_DP_units_vattr);
free( tim_polarity_of_magnetic_field_BP_units_vattr);
free( time_gattribute_type);
free( time_gattribute_title);
free( time_gattribute_name);
free( time_gattribute_project);
free( time_gattribute_initial);
free( time_gattribute_resume);
free( time_gattribute_boundary);
free( time_gattribute_passage);
free( time_gattribute_grid);
free( time_gattribute_geometry);
free( time_gattribute_code);
free( time_gattribute_parameters);
free( time_gattribute_refdate_mjd);
free( time_gattribute_gamma);
free( time_gattribute_xalpha);
free( time_gattribute_history);
free( evh_x1_position_X1);
free( evh_x2_position_X2);
free( evh_x3_position_X3);
free( evh_physical_time_TIME);
free( evh_physical_time_step_DT);
free( evh_numerical_time_step_NSTEP);
free( evh_density_D);
free( evh_temperature_T);
free( evh_x1_velocity_V1);
free( evh_x2_velocity_V2);
free( evh_x3_velocity_V3);
free( evh_x1_magnetic_field_B1);
free( evh_x2_magnetic_field_B2);
free( evh_x3_magnetic_field_B3);
free( evh_cloud_mass_density_DP);
free( evh_polarity_of_magnetic_field_BP);
free( evh_x1_position_X1_long_name_vattr);
free( evh_x2_position_X2_long_name_vattr);
free( evh_x3_position_X3_long_name_vattr);
free( evh_x1_position_X1_units_vattr);
free( evh_x2_position_X2_units_vattr);
free( evh_x3_position_X3_units_vattr);
free( evh_physical_time_TIME_long_name_vattr);
free( evh_physical_time_step_DT_long_name_vattr);
free( evh_numerical_time_step_NSTEP_long_name_vattr);
free( evh_physical_time_TIME_units_vattr);
free( evh_physical_time_step_DT_units_vattr);
free( evh_numerical_time_step_NSTEP_units_vattr);
free( evh_density_D_long_name_vattr);
free( evh_temperature_T_long_name_vattr);
free( evh_x1_velocity_V1_long_name_vattr);
free( evh_x2_velocity_V2_long_name_vattr);
free( evh_x3_velocity_V3_long_name_vattr);
free( evh_x1_magnetic_field_B1_long_name_vattr);
free( evh_x2_magnetic_field_B2_long_name_vattr);
free( evh_x3_magnetic_field_B3_long_name_vattr);
free( evh_cloud_mass_density_DP_long_name_vattr);
free( evh_polarity_of_magnetic_field_BP_long_name_vattr);
free( evh_density_D_units_vattr);
free( evh_temperature_T_units_vattr);
free( evh_x1_velocity_V1_units_vattr);
free( evh_x2_velocity_V2_units_vattr);
free( evh_x3_velocity_V3_units_vattr);
free( evh_x1_magnetic_field_B1_units_vattr);
free( evh_x2_magnetic_field_B2_units_vattr);
free( evh_x3_magnetic_field_B3_units_vattr);
free( evh_cloud_mass_density_DP_units_vattr);
free( evh_polarity_of_magnetic_field_BP_units_vattr);
free( evh_gattribute_type);
free( evh_gattribute_title);
free( evh_gattribute_name);
free( evh_gattribute_project);
free( evh_gattribute_initial);
free( evh_gattribute_resume);
free( evh_gattribute_boundary);
free( evh_gattribute_passage);
free( evh_gattribute_grid);
free( evh_gattribute_geometry);
free( evh_gattribute_code);
free( evh_gattribute_parameters);
free( evh_gattribute_refdate_mjd);
free( evh_gattribute_gamma);
free( evh_gattribute_xalpha);
free( evh_gattribute_history);
free( evl_x1_position_X1);
free( evl_x2_position_X2);
free( evl_x3_position_X3);
free( evl_physical_time_TIME);
free( evl_physical_time_step_DT);
free( evl_numerical_time_step_NSTEP);
free( evl_density_D);
free( evl_temperature_T);
free( evl_x1_velocity_V1);
free( evl_x2_velocity_V2);
free( evl_x3_velocity_V3);
free( evl_x1_magnetic_field_B1);
free( evl_x2_magnetic_field_B2);
free( evl_x3_magnetic_field_B3);
free( evl_cloud_mass_density_DP);
free( evl_polarity_of_magnetic_field_BP);
free( evl_x1_position_X1_long_name_vattr);
free( evl_x2_position_X2_long_name_vattr);
free( evl_x3_position_X3_long_name_vattr);
free( evl_x1_position_X1_units_vattr);
free( evl_x2_position_X2_units_vattr);
free( evl_x3_position_X3_units_vattr);
free( evl_physical_time_TIME_long_name_vattr);
free( evl_physical_time_step_DT_long_name_vattr);
free( evl_numerical_time_step_NSTEP_long_name_vattr);
free( evl_physical_time_TIME_units_vattr);
free( evl_physical_time_step_DT_units_vattr);
free( evl_numerical_time_step_NSTEP_units_vattr);
free( evl_density_D_long_name_vattr);
free( evl_temperature_T_long_name_vattr);
free( evl_x1_velocity_V1_long_name_vattr);
free( evl_x2_velocity_V2_long_name_vattr);
free( evl_x3_velocity_V3_long_name_vattr);
free( evl_x1_magnetic_field_B1_long_name_vattr);
free( evl_x2_magnetic_field_B2_long_name_vattr);
free( evl_x3_magnetic_field_B3_long_name_vattr);
free( evl_cloud_mass_density_DP_long_name_vattr);
free( evl_polarity_of_magnetic_field_BP_long_name_vattr);
free( evl_density_D_units_vattr);
free( evl_temperature_T_units_vattr);
free( evl_x1_velocity_V1_units_vattr);
free( evl_x2_velocity_V2_units_vattr);
free( evl_x3_velocity_V3_units_vattr);
free( evl_x1_magnetic_field_B1_units_vattr);
free( evl_x2_magnetic_field_B2_units_vattr);
free( evl_x3_magnetic_field_B3_units_vattr);
free( evl_cloud_mass_density_DP_units_vattr);
free( evl_polarity_of_magnetic_field_BP_units_vattr);
free( evl_gattribute_type);
free( evl_gattribute_title);
free( evl_gattribute_name);
free( evl_gattribute_project);
free( evl_gattribute_initial);
free( evl_gattribute_resume);
free( evl_gattribute_boundary);
free( evl_gattribute_passage);
free( evl_gattribute_grid);
free( evl_gattribute_geometry);
free( evl_gattribute_code);
free( evl_gattribute_parameters);
free( evl_gattribute_refdate_mjd);
free( evl_gattribute_gamma);
free( evl_gattribute_xalpha);
free( evl_gattribute_history);
free( evg_x1_position_X1);
free( evg_x2_position_X2);
free( evg_x3_position_X3);
free( evg_physical_time_TIME);
free( evg_physical_time_step_DT);
free( evg_numerical_time_step_NSTEP);
free( evg_density_D);
free( evg_temperature_T);
free( evg_x1_velocity_V1);
free( evg_x2_velocity_V2);
free( evg_x3_velocity_V3);
free( evg_x1_magnetic_field_B1);
free( evg_x2_magnetic_field_B2);
free( evg_x3_magnetic_field_B3);
free( evg_cloud_mass_density_DP);
free( evg_polarity_of_magnetic_field_BP);
free( evg_x1_position_X1_long_name_vattr);
free( evg_x2_position_X2_long_name_vattr);
free( evg_x3_position_X3_long_name_vattr);
free( evg_x1_position_X1_units_vattr);
free( evg_x2_position_X2_units_vattr);
free( evg_x3_position_X3_units_vattr);
free( evg_physical_time_TIME_long_name_vattr);
free( evg_physical_time_step_DT_long_name_vattr);
free( evg_numerical_time_step_NSTEP_long_name_vattr);
free( evg_physical_time_TIME_units_vattr);
free( evg_physical_time_step_DT_units_vattr);
free( evg_numerical_time_step_NSTEP_units_vattr);
free( evg_density_D_long_name_vattr);
free( evg_temperature_T_long_name_vattr);
free( evg_x1_velocity_V1_long_name_vattr);
free( evg_x2_velocity_V2_long_name_vattr);
free( evg_x3_velocity_V3_long_name_vattr);
free( evg_x1_magnetic_field_B1_long_name_vattr);
free( evg_x2_magnetic_field_B2_long_name_vattr);
free( evg_x3_magnetic_field_B3_long_name_vattr);
free( evg_cloud_mass_density_DP_long_name_vattr);
free( evg_polarity_of_magnetic_field_BP_long_name_vattr);
free( evg_density_D_units_vattr);
free( evg_temperature_T_units_vattr);
free( evg_x1_velocity_V1_units_vattr);
free( evg_x2_velocity_V2_units_vattr);
free( evg_x3_velocity_V3_units_vattr);
free( evg_x1_magnetic_field_B1_units_vattr);
free( evg_x2_magnetic_field_B2_units_vattr);
free( evg_x3_magnetic_field_B3_units_vattr);
free( evg_cloud_mass_density_DP_units_vattr);
free( evg_polarity_of_magnetic_field_BP_units_vattr);
free( evg_gattribute_type);
free( evg_gattribute_title);
free( evg_gattribute_name);
free( evg_gattribute_project);
free( evg_gattribute_initial);
free( evg_gattribute_resume);
free( evg_gattribute_boundary);
free( evg_gattribute_passage);
free( evg_gattribute_grid);
free( evg_gattribute_geometry);
free( evg_gattribute_code);
free( evg_gattribute_parameters);
free( evg_gattribute_refdate_mjd);
free( evg_gattribute_gamma);
free( evg_gattribute_xalpha);
free( evg_gattribute_history);
free( evp_x1_position_X1);
free( evp_x2_position_X2);
free( evp_x3_position_X3);
free( evp_physical_time_TIME);
free( evp_physical_time_step_DT);
free( evp_numerical_time_step_NSTEP);
free( evp_density_D);
free( evp_temperature_T);
free( evp_x1_velocity_V1);
free( evp_x2_velocity_V2);
free( evp_x3_velocity_V3);
free( evp_x1_magnetic_field_B1);
free( evp_x2_magnetic_field_B2);
free( evp_x3_magnetic_field_B3);
free( evp_cloud_mass_density_DP);
free( evp_polarity_of_magnetic_field_BP);
free( evp_x1_position_X1_long_name_vattr);
free( evp_x2_position_X2_long_name_vattr);
free( evp_x3_position_X3_long_name_vattr);
free( evp_x1_position_X1_units_vattr);
free( evp_x2_position_X2_units_vattr);
free( evp_x3_position_X3_units_vattr);
free( evp_physical_time_TIME_long_name_vattr);
free( evp_physical_time_step_DT_long_name_vattr);
free( evp_numerical_time_step_NSTEP_long_name_vattr);
free( evp_physical_time_TIME_units_vattr);
free( evp_physical_time_step_DT_units_vattr);
free( evp_numerical_time_step_NSTEP_units_vattr);
free( evp_density_D_long_name_vattr);
free( evp_temperature_T_long_name_vattr);
free( evp_x1_velocity_V1_long_name_vattr);
free( evp_x2_velocity_V2_long_name_vattr);
free( evp_x3_velocity_V3_long_name_vattr);
free( evp_x1_magnetic_field_B1_long_name_vattr);
free( evp_x2_magnetic_field_B2_long_name_vattr);
free( evp_x3_magnetic_field_B3_long_name_vattr);
free( evp_cloud_mass_density_DP_long_name_vattr);
free( evp_polarity_of_magnetic_field_BP_long_name_vattr);
free( evp_density_D_units_vattr);
free( evp_temperature_T_units_vattr);
free( evp_x1_velocity_V1_units_vattr);
free( evp_x2_velocity_V2_units_vattr);
free( evp_x3_velocity_V3_units_vattr);
free( evp_x1_magnetic_field_B1_units_vattr);
free( evp_x2_magnetic_field_B2_units_vattr);
free( evp_x3_magnetic_field_B3_units_vattr);
free( evp_cloud_mass_density_DP_units_vattr);
free( evp_polarity_of_magnetic_field_BP_units_vattr);
free( evp_gattribute_type);
free( evp_gattribute_title);
free( evp_gattribute_name);
free( evp_gattribute_project);
free( evp_gattribute_initial);
free( evp_gattribute_resume);
free( evp_gattribute_boundary);
free( evp_gattribute_passage);
free( evp_gattribute_grid);
free( evp_gattribute_geometry);
free( evp_gattribute_code);
free( evp_gattribute_parameters);
free( evp_gattribute_refdate_mjd);
free( evp_gattribute_gamma);
free( evp_gattribute_xalpha);
free( evp_gattribute_history);

return 0;

}
