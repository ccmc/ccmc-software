/*******************************************************************************
 *                                                                             *
 *     NAME:       read_enlil.h                                                *
 *                                                                             *
 *     VERSION:    1.0                                                         *
 *                                                                             *
 *     AUTHOR:     Marlo M. H. Maddox                                          *
 *                 NASA-GSFC-AETD-ISD-ADMA(587)/CCMC(612.3)                    *
 *                 Marlo.Maddox@nasa.gov                                       *
 *                 (301) 286 - 5202                                            *
 *                                                                             *
 *     PURPOSE:    identify ALL values that can be extracted from the model    *
 *                 output and create variables that will hold the values.      *
 *                 Define any additional variables that will be required to    *
 *                 extract the data ie ( variable id numbers )                 *
 *                                                                             *
 *     INPUTS:     *                                                           *
 *                                                                             *
 *     OUTPUTS:                                                                *
 *                                                                             *
 *     FUNCTIONS:  *                                                           *
 *                                                                             *
 *     VARIABLES:  *                                                           *
 *                                                                             *
 *                                                                             *
 *     MODIFICATION                                                            *
 *     HISTORY:                                                                *
 *                                                                             *
 *    02.02.2006   Maddox, Marlo                                               *
 *                 initial development started                                 *
 *                                                                             *
 *    10.18.2006   Maddox, Marlo                                               *
 *                 Fixing declarations for some of the time variables          *
 *                                                                             *
 ******************************************************************************/

/** prevent the contents of the header from being scanned multiple times and also solve the problem of multiple inclusion **/

#ifndef READ_ENLIL_H_
#define READ_ENLIL_H_

/*** VARIABLES for the tim.<rrrr>.nc files also include netCDFid variables needed
 * for netCDF interface ******/

/* dimensions */

size_t tim_n1;
size_t tim_n2;
size_t tim_n3;
size_t tim_n1h;
size_t tim_n2h;
size_t tim_n3h;
size_t tim_number_of_blocks_nblk;

static int tim_number_of_dimensions = 4;

/* dimension netCDFid variabels */

int tim_n1_netCDFid;
int tim_n2_netCDFid;
int tim_n3_netCDFid;
int tim_n1h_netCDFid;
int tim_n2h_netCDFid;
int tim_n3h_netCDFid;
int tim_number_of_blocks_nblk_netCDFid;

/* position variable arrays */

float *tim_x1_position_X1;
float *tim_x2_position_X2;
float *tim_x3_position_X3;
float *tim_x1h_position_X1H;
float *tim_x2h_position_X2H;
float *tim_x3h_position_X3H;

/* position variable attributes */

char *tim_x1_position_X1_long_name_vattr;
char *tim_x2_position_X2_long_name_vattr;
char *tim_x3_position_X3_long_name_vattr;
char *tim_x1h_position_X1H_long_name_vattr;
char *tim_x2h_position_X2H_long_name_vattr;
char *tim_x3h_position_X3H_long_name_vattr;

char *tim_x1_position_X1_units_vattr;
char *tim_x2_position_X2_units_vattr;
char *tim_x3_position_X3_units_vattr;
char *tim_x1h_position_X1H_units_vattr;
char *tim_x2h_position_X2H_units_vattr;
char *tim_x3h_position_X3H_units_vattr;

/* position netCDFid numbers */

int tim_x1_position_X1_netCDFid;
int tim_x2_position_X2_netCDFid;
int tim_x3_position_X3_netCDFid;
int tim_x1h_position_X1H_netCDFid;
int tim_x2h_position_X2H_netCDFid;
int tim_x3h_position_X3H_netCDFid;

/* scalar variables */

float *tim_physical_time_TIME;
float *tim_physical_time_step_DT;
int *tim_numerical_time_step_NSTEP;

/* scalar variable attributes */

char *tim_physical_time_TIME_long_name_vattr;
char *tim_physical_time_step_DT_long_name_vattr;
char *tim_numerical_time_step_NSTEP_long_name_vattr;

char *tim_physical_time_TIME_units_vattr;
char *tim_physical_time_step_DT_units_vattr;
char *tim_numerical_time_step_NSTEP_units_vattr;

/* scalar variables netCDFid numbers */

int tim_physical_time_TIME_netCDFid;
int tim_physical_time_step_DT_netCDFid;
int tim_numerical_time_step_NSTEP_netCDFid;

/* physical quantity variable arrays */

float *tim_density_D;
float *tim_temperature_T;
float *tim_x1_velocity_V1;
float *tim_x2_velocity_V2;
float *tim_x3_velocity_V3;
float *tim_x1_magnetic_field_B1;
float *tim_x2_magnetic_field_B2;
float *tim_x3_magnetic_field_B3;
float *tim_cloud_mass_density_DP;
float *tim_polarity_of_magnetic_field_BP;

/* physical quantity variable attributes */

char *tim_density_D_long_name_vattr;
char *tim_temperature_T_long_name_vattr;
char *tim_x1_velocity_V1_long_name_vattr;
char *tim_x2_velocity_V2_long_name_vattr;
char *tim_x3_velocity_V3_long_name_vattr;
char *tim_x1_magnetic_field_B1_long_name_vattr;
char *tim_x2_magnetic_field_B2_long_name_vattr;
char *tim_x3_magnetic_field_B3_long_name_vattr;
char *tim_cloud_mass_density_DP_long_name_vattr;
char *tim_polarity_of_magnetic_field_BP_long_name_vattr;

char *tim_density_D_units_vattr;
char *tim_temperature_T_units_vattr;
char *tim_x1_velocity_V1_units_vattr;
char *tim_x2_velocity_V2_units_vattr;
char *tim_x3_velocity_V3_units_vattr;
char *tim_x1_magnetic_field_B1_units_vattr;
char *tim_x2_magnetic_field_B2_units_vattr;
char *tim_x3_magnetic_field_B3_units_vattr;
char *tim_cloud_mass_density_DP_units_vattr;
char *tim_polarity_of_magnetic_field_BP_units_vattr;

/* physical quantity variable netCDFid numbers */

int tim_density_D_netCDFid;
int tim_temperature_T_netCDFid;
int tim_x1_velocity_V1_netCDFid;
int tim_x2_velocity_V2_netCDFid;
int tim_x3_velocity_V3_netCDFid;
int tim_x1_magnetic_field_B1_netCDFid;
int tim_x2_magnetic_field_B2_netCDFid;
int tim_x3_magnetic_field_B3_netCDFid;
int tim_cloud_mass_density_DP_netCDFid;
int tim_polarity_of_magnetic_field_BP_netCDFid;

/********** global attributes for the time.<rrrr>.nc files ******/

char *time_gattribute_type;
char *time_gattribute_title;
char *time_gattribute_name;
char *time_gattribute_project;
char *time_gattribute_initial;
char *time_gattribute_resume;
char *time_gattribute_boundary;
char *time_gattribute_passage;
char *time_gattribute_grid;
char *time_gattribute_geometry;
char *time_gattribute_code;
char *time_gattribute_parameters;
double *time_gattribute_refdate_mjd;
double *time_gattribute_gamma;
double *time_gattribute_xalpha;
char *time_gattribute_history;

/***********************************************************
 *             VARIABLES for the evh.nc files                *
 ***********************************************************/

size_t evh_nhel;
size_t evh_nevo;

static int evh_number_of_dimensions = 2;

float *evh_x1_position_X1;
float *evh_x2_position_X2;
float *evh_x3_position_X3;

float *evh_physical_time_TIME;
float *evh_physical_time_step_DT;
int *evh_numerical_time_step_NSTEP;

float *evh_density_D;
float *evh_temperature_T;
float *evh_x1_velocity_V1;
float *evh_x2_velocity_V2;
float *evh_x3_velocity_V3;
float *evh_x1_magnetic_field_B1;
float *evh_x2_magnetic_field_B2;
float *evh_x3_magnetic_field_B3;
float *evh_cloud_mass_density_DP;
float *evh_polarity_of_magnetic_field_BP;

/* coresponding evh variable netCDFid numbers */

int evh_nhel_netCDFid;
int evh_nevo_netCDFid;

int evh_x1_position_X1_netCDFid;
int evh_x2_position_X2_netCDFid;
int evh_x3_position_X3_netCDFid;

int evh_physical_time_TIME_netCDFid;
int evh_physical_time_step_DT_netCDFid;
int evh_numerical_time_step_NSTEP_netCDFid;

int evh_density_D_netCDFid;
int evh_temperature_T_netCDFid;
int evh_x1_velocity_V1_netCDFid;
int evh_x2_velocity_V2_netCDFid;
int evh_x3_velocity_V3_netCDFid;
int evh_x1_magnetic_field_B1_netCDFid;
int evh_x2_magnetic_field_B2_netCDFid;
int evh_x3_magnetic_field_B3_netCDFid;
int evh_cloud_mass_density_DP_netCDFid;
int evh_polarity_of_magnetic_field_BP_netCDFid;

/* position variable attributes */

char *evh_x1_position_X1_long_name_vattr;
char *evh_x2_position_X2_long_name_vattr;
char *evh_x3_position_X3_long_name_vattr;

char *evh_x1_position_X1_units_vattr;
char *evh_x2_position_X2_units_vattr;
char *evh_x3_position_X3_units_vattr;

/* scalar variable attributes */

char *evh_physical_time_TIME_long_name_vattr;
char *evh_physical_time_step_DT_long_name_vattr;
char *evh_numerical_time_step_NSTEP_long_name_vattr;

char *evh_physical_time_TIME_units_vattr;
char *evh_physical_time_step_DT_units_vattr;
char *evh_numerical_time_step_NSTEP_units_vattr;

/* physical quantity variable attributes */

char *evh_density_D_long_name_vattr;
char *evh_temperature_T_long_name_vattr;
char *evh_x1_velocity_V1_long_name_vattr;
char *evh_x2_velocity_V2_long_name_vattr;
char *evh_x3_velocity_V3_long_name_vattr;
char *evh_x1_magnetic_field_B1_long_name_vattr;
char *evh_x2_magnetic_field_B2_long_name_vattr;
char *evh_x3_magnetic_field_B3_long_name_vattr;
char *evh_cloud_mass_density_DP_long_name_vattr;
char *evh_polarity_of_magnetic_field_BP_long_name_vattr;

char *evh_density_D_units_vattr;
char *evh_temperature_T_units_vattr;
char *evh_x1_velocity_V1_units_vattr;
char *evh_x2_velocity_V2_units_vattr;
char *evh_x3_velocity_V3_units_vattr;
char *evh_x1_magnetic_field_B1_units_vattr;
char *evh_x2_magnetic_field_B2_units_vattr;
char *evh_x3_magnetic_field_B3_units_vattr;
char *evh_cloud_mass_density_DP_units_vattr;
char *evh_polarity_of_magnetic_field_BP_units_vattr;

/********** global attributes for the evh.nc files ******/

char *evh_gattribute_type;
char *evh_gattribute_title;
char *evh_gattribute_name;
char *evh_gattribute_project;
char *evh_gattribute_initial;
char *evh_gattribute_resume;
char *evh_gattribute_boundary;
char *evh_gattribute_passage;
char *evh_gattribute_grid;
char *evh_gattribute_geometry;
char *evh_gattribute_code;
char *evh_gattribute_parameters;
double *evh_gattribute_refdate_mjd;
double *evh_gattribute_gamma;
double *evh_gattribute_xalpha;
char *evh_gattribute_history;

/***********************************************************
 *             VARIABLES for the evl.nc files                *
 ***********************************************************/

size_t evl_nsel;
size_t evl_nevo;

static int evl_number_of_dimensions = 2;

float *evl_x1_position_X1;
float *evl_x2_position_X2;
float *evl_x3_position_X3;

float *evl_physical_time_TIME;
float *evl_physical_time_step_DT;

int *evl_numerical_time_step_NSTEP;

float *evl_density_D;
float *evl_temperature_T;
float *evl_x1_velocity_V1;
float *evl_x2_velocity_V2;
float *evl_x3_velocity_V3;
float *evl_x1_magnetic_field_B1;
float *evl_x2_magnetic_field_B2;
float *evl_x3_magnetic_field_B3;
float *evl_cloud_mass_density_DP;
float *evl_polarity_of_magnetic_field_BP;

/* coresponding evl variable netCDFid numbers */

int evl_nsel_netCDFid;
int evl_nevo_netCDFid;
int evl_x1_position_X1_netCDFid;
int evl_x2_position_X2_netCDFid;
int evl_x3_position_X3_netCDFid;
int evl_physical_time_TIME_netCDFid;
int evl_physical_time_step_DT_netCDFid;
int evl_numerical_time_step_NSTEP_netCDFid;
int evl_density_D_netCDFid;
int evl_temperature_T_netCDFid;
int evl_x1_velocity_V1_netCDFid;
int evl_x2_velocity_V2_netCDFid;
int evl_x3_velocity_V3_netCDFid;
int evl_x1_magnetic_field_B1_netCDFid;
int evl_x2_magnetic_field_B2_netCDFid;
int evl_x3_magnetic_field_B3_netCDFid;
int evl_cloud_mass_density_DP_netCDFid;
int evl_polarity_of_magnetic_field_BP_netCDFid;

/* position variable attributes */

char *evl_x1_position_X1_long_name_vattr;
char *evl_x2_position_X2_long_name_vattr;
char *evl_x3_position_X3_long_name_vattr;

char *evl_x1_position_X1_units_vattr;
char *evl_x2_position_X2_units_vattr;
char *evl_x3_position_X3_units_vattr;

/* scalar variable attributes */

char *evl_physical_time_TIME_long_name_vattr;
char *evl_physical_time_step_DT_long_name_vattr;
char *evl_numerical_time_step_NSTEP_long_name_vattr;

char *evl_physical_time_TIME_units_vattr;
char *evl_physical_time_step_DT_units_vattr;
char *evl_numerical_time_step_NSTEP_units_vattr;

/* physical quantity variable attributes */

char *evl_density_D_long_name_vattr;
char *evl_temperature_T_long_name_vattr;
char *evl_x1_velocity_V1_long_name_vattr;
char *evl_x2_velocity_V2_long_name_vattr;
char *evl_x3_velocity_V3_long_name_vattr;
char *evl_x1_magnetic_field_B1_long_name_vattr;
char *evl_x2_magnetic_field_B2_long_name_vattr;
char *evl_x3_magnetic_field_B3_long_name_vattr;
char *evl_cloud_mass_density_DP_long_name_vattr;
char *evl_polarity_of_magnetic_field_BP_long_name_vattr;

char *evl_density_D_units_vattr;
char *evl_temperature_T_units_vattr;
char *evl_x1_velocity_V1_units_vattr;
char *evl_x2_velocity_V2_units_vattr;
char *evl_x3_velocity_V3_units_vattr;
char *evl_x1_magnetic_field_B1_units_vattr;
char *evl_x2_magnetic_field_B2_units_vattr;
char *evl_x3_magnetic_field_B3_units_vattr;
char *evl_cloud_mass_density_DP_units_vattr;
char *evl_polarity_of_magnetic_field_BP_units_vattr;

/********** global attributes for the evl.nc files ******/

char *evl_gattribute_type;
char *evl_gattribute_title;
char *evl_gattribute_name;
char *evl_gattribute_project;
char *evl_gattribute_initial;
char *evl_gattribute_resume;
char *evl_gattribute_boundary;
char *evl_gattribute_passage;
char *evl_gattribute_grid;
char *evl_gattribute_geometry;
char *evl_gattribute_code;
char *evl_gattribute_parameters;
double *evl_gattribute_refdate_mjd;
double *evl_gattribute_gamma;
double *evl_gattribute_xalpha;
char *evl_gattribute_history;

/***********************************************************
 *             VARIABLES for the evg.nc files                *
 ***********************************************************/

size_t evg_ngeo;
size_t evg_nevo;

static int evg_number_of_dimensions = 2;

float *evg_x1_position_X1;
float *evg_x2_position_X2;
float *evg_x3_position_X3;

float *evg_physical_time_TIME;
float *evg_physical_time_step_DT;

int *evg_numerical_time_step_NSTEP;

float *evg_density_D;
float *evg_temperature_T;
float *evg_x1_velocity_V1;
float *evg_x2_velocity_V2;
float *evg_x3_velocity_V3;
float *evg_x1_magnetic_field_B1;
float *evg_x2_magnetic_field_B2;
float *evg_x3_magnetic_field_B3;
float *evg_cloud_mass_density_DP;
float *evg_polarity_of_magnetic_field_BP;

/* coresponding evg variable netCDFid numbers */

int evg_ngeo_netCDFid;
int evg_nevo_netCDFid;
int evg_x1_position_X1_netCDFid;
int evg_x2_position_X2_netCDFid;
int evg_x3_position_X3_netCDFid;
int evg_physical_time_TIME_netCDFid;
int evg_physical_time_step_DT_netCDFid;
int evg_numerical_time_step_NSTEP_netCDFid;
int evg_density_D_netCDFid;
int evg_temperature_T_netCDFid;
int evg_x1_velocity_V1_netCDFid;
int evg_x2_velocity_V2_netCDFid;
int evg_x3_velocity_V3_netCDFid;
int evg_x1_magnetic_field_B1_netCDFid;
int evg_x2_magnetic_field_B2_netCDFid;
int evg_x3_magnetic_field_B3_netCDFid;
int evg_cloud_mass_density_DP_netCDFid;
int evg_polarity_of_magnetic_field_BP_netCDFid;

/* position variable attributes */

char *evg_x1_position_X1_long_name_vattr;
char *evg_x2_position_X2_long_name_vattr;
char *evg_x3_position_X3_long_name_vattr;

char *evg_x1_position_X1_units_vattr;
char *evg_x2_position_X2_units_vattr;
char *evg_x3_position_X3_units_vattr;

/* scalar variable attributes */

char *evg_physical_time_TIME_long_name_vattr;
char *evg_physical_time_step_DT_long_name_vattr;
char *evg_numerical_time_step_NSTEP_long_name_vattr;

char *evg_physical_time_TIME_units_vattr;
char *evg_physical_time_step_DT_units_vattr;
char *evg_numerical_time_step_NSTEP_units_vattr;

/* physical quantity variable attributes */

char *evg_density_D_long_name_vattr;
char *evg_temperature_T_long_name_vattr;
char *evg_x1_velocity_V1_long_name_vattr;
char *evg_x2_velocity_V2_long_name_vattr;
char *evg_x3_velocity_V3_long_name_vattr;
char *evg_x1_magnetic_field_B1_long_name_vattr;
char *evg_x2_magnetic_field_B2_long_name_vattr;
char *evg_x3_magnetic_field_B3_long_name_vattr;
char *evg_cloud_mass_density_DP_long_name_vattr;
char *evg_polarity_of_magnetic_field_BP_long_name_vattr;

char *evg_density_D_units_vattr;
char *evg_temperature_T_units_vattr;
char *evg_x1_velocity_V1_units_vattr;
char *evg_x2_velocity_V2_units_vattr;
char *evg_x3_velocity_V3_units_vattr;
char *evg_x1_magnetic_field_B1_units_vattr;
char *evg_x2_magnetic_field_B2_units_vattr;
char *evg_x3_magnetic_field_B3_units_vattr;
char *evg_cloud_mass_density_DP_units_vattr;
char *evg_polarity_of_magnetic_field_BP_units_vattr;

/********** global attributes for the evg.nc files ******/

char *evg_gattribute_type;
char *evg_gattribute_title;
char *evg_gattribute_name;
char *evg_gattribute_project;
char *evg_gattribute_initial;
char *evg_gattribute_resume;
char *evg_gattribute_boundary;
char *evg_gattribute_passage;
char *evg_gattribute_grid;
char *evg_gattribute_geometry;
char *evg_gattribute_code;
char *evg_gattribute_parameters;
double *evg_gattribute_refdate_mjd;
double *evg_gattribute_gamma;
double *evg_gattribute_xalpha;
char *evg_gattribute_history;

/***********************************************************
 *             VARIABLES for the evp.nc files                *
 ***********************************************************/

size_t evp_npla;
size_t evp_nevo;

static int evp_number_of_dimensions = 2;

float *evp_x1_position_X1;
float *evp_x2_position_X2;
float *evp_x3_position_X3;

float *evp_physical_time_TIME;
float *evp_physical_time_step_DT;

int *evp_numerical_time_step_NSTEP;

float *evp_density_D;
float *evp_temperature_T;
float *evp_x1_velocity_V1;
float *evp_x2_velocity_V2;
float *evp_x3_velocity_V3;
float *evp_x1_magnetic_field_B1;
float *evp_x2_magnetic_field_B2;
float *evp_x3_magnetic_field_B3;
float *evp_cloud_mass_density_DP;
float *evp_polarity_of_magnetic_field_BP;

/* coresponding evp variable netCDFid numbers */

int evp_npla_netCDFid;
int evp_nevo_netCDFid;
int evp_x1_position_X1_netCDFid;
int evp_x2_position_X2_netCDFid;
int evp_x3_position_X3_netCDFid;
int evp_physical_time_TIME_netCDFid;
int evp_physical_time_step_DT_netCDFid;
int evp_numerical_time_step_NSTEP_netCDFid;
int evp_density_D_netCDFid;
int evp_temperature_T_netCDFid;
int evp_x1_velocity_V1_netCDFid;
int evp_x2_velocity_V2_netCDFid;
int evp_x3_velocity_V3_netCDFid;
int evp_x1_magnetic_field_B1_netCDFid;
int evp_x2_magnetic_field_B2_netCDFid;
int evp_x3_magnetic_field_B3_netCDFid;
int evp_cloud_mass_density_DP_netCDFid;
int evp_polarity_of_magnetic_field_BP_netCDFid;

/* position variable attributes */

char *evp_x1_position_X1_long_name_vattr;
char *evp_x2_position_X2_long_name_vattr;
char *evp_x3_position_X3_long_name_vattr;

char *evp_x1_position_X1_units_vattr;
char *evp_x2_position_X2_units_vattr;
char *evp_x3_position_X3_units_vattr;

/* scalar variable attributes */

char *evp_physical_time_TIME_long_name_vattr;
char *evp_physical_time_step_DT_long_name_vattr;
char *evp_numerical_time_step_NSTEP_long_name_vattr;

char *evp_physical_time_TIME_units_vattr;
char *evp_physical_time_step_DT_units_vattr;
char *evp_numerical_time_step_NSTEP_units_vattr;

/* physical quantity variable attributes */

char *evp_density_D_long_name_vattr;
char *evp_temperature_T_long_name_vattr;
char *evp_x1_velocity_V1_long_name_vattr;
char *evp_x2_velocity_V2_long_name_vattr;
char *evp_x3_velocity_V3_long_name_vattr;
char *evp_x1_magnetic_field_B1_long_name_vattr;
char *evp_x2_magnetic_field_B2_long_name_vattr;
char *evp_x3_magnetic_field_B3_long_name_vattr;
char *evp_cloud_mass_density_DP_long_name_vattr;
char *evp_polarity_of_magnetic_field_BP_long_name_vattr;

char *evp_density_D_units_vattr;
char *evp_temperature_T_units_vattr;
char *evp_x1_velocity_V1_units_vattr;
char *evp_x2_velocity_V2_units_vattr;
char *evp_x3_velocity_V3_units_vattr;
char *evp_x1_magnetic_field_B1_units_vattr;
char *evp_x2_magnetic_field_B2_units_vattr;
char *evp_x3_magnetic_field_B3_units_vattr;
char *evp_cloud_mass_density_DP_units_vattr;
char *evp_polarity_of_magnetic_field_BP_units_vattr;

/********** global attributes for the evh.nc files ******/

char *evp_gattribute_type;
char *evp_gattribute_title;
char *evp_gattribute_name;
char *evp_gattribute_project;
char *evp_gattribute_initial;
char *evp_gattribute_resume;
char *evp_gattribute_boundary;
char *evp_gattribute_passage;
char *evp_gattribute_grid;
char *evp_gattribute_geometry;
char *evp_gattribute_code;
char *evp_gattribute_parameters;
double *evp_gattribute_refdate_mjd;
double *evp_gattribute_gamma;
double *evp_gattribute_xalpha;
char *evp_gattribute_history;

#endif /*READ_ENLIL_H_*/

