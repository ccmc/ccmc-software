/*******************************************************************************
 *                                                                             *
 *    NAME:         read_batsrus.h                                             *
 *                                                                             *
 *    VERSION:      1.0                                                        *
 *                                                                             *
 *    AUTHOR:       Marlo M. H. Maddox                                         *
 *                  NASA-GSFC-AETD-ISD-ADMA(587)                               *
 *                  Marlo.Maddox@nasa.gov                                      *
 *                  (301) 286 - 5202                                           *
 *                                                                             *
 *    PURPOSE:                                                                 *
 *                                                                             *
 *    INPUTS:       *                                                          *
 *                                                                             *
 *    OUTPUTS:      *                                                          *
 *                                                                             *
 *    FUNCTIONS:    *                                                          *
 *                                                                             *
 *    VARIABLES:    *                                                          *
 *                                                                             *
 *                                                                             *
 *    MODIFICATION                                                             *
 *    HISTORY:                                                                 *
 *                                                                             *
 *    09.11.2003     Maddox, Marlo                                             *
 *                   initial development started                               *
 *    09.09.2004     Maddox, Marlo                                             *
 *                   adding actual min/max variables for each plot             *
 *                   /physical variable                                        *
 *    03.17.2005     Rastaetter, Lutz                                          *
 *                   defined strsplit, free_strarr routines,                   *
 *                   strarr structure, and flip_endian, is_double,             *
 *                   dtmp, *tmp_double variables for new endianess             *
 *                   and .out double precision read BATS functions             *
 *                                                                             *
 *    11.01.2006     Maddox, Marlo                                             *
 *                   batsrus component of SWMF has larger records              *
 *                   namely record 1 & 5.  When we changed the                 *
 *                   MAX_FIRST_RECORD macro from 100 -> 200 we should          *
 *                   have updated the MAX_UNITS_STRING macro here as           *
 *                   well, since the units_string[MAX_UNITS_STRING]            *
 *                   array will hold the first record which can now be > 100   *
 *                                                                             *
 *                   ...changing MAX_UNITS_STRING from 100 -> 200              *
 *                                                                             *
 *                   ... also changing MAX_VARIABLE_NAME_STRING                *
 *                   from 100 -> 200                                           *
 *                                                                             *
 *                   you know what, why are these hardcoded anyway?            *
 *                   changing unit_string & variable_name_string               *
 *                   variables from from char arrays to char pointers          *
 *                   lets dynamically allocate memory for them in              *
 *                   read_batsrus.c based on the values returned from          *
 *                   fread...                                                  *
 *                 							       *
 *    08.30.2011   Maddox, Marlo					       *
 *    		   Adding new "status" variable to batsrus                     *
 ******************************************************************************/

/***** we don't need these anymore - leave as a reminder
 #define MAX_VARIABLE_NAME_STRING    200
 #define MAX_UNITS_STRING        200
 ******/

FILE *input_filePtr;

int byte_value;
int byte_value_2;

int flip_endian, is_double;
double dtmp, *tmp_double;

/*********************** record 1 ****************/
/*char    *unit_string[MAX_UNITS_STRING];*/
char *unit_string;

/************************ record 2 ***************/
int current_iteration_step;
float elapsed_time_seconds;
int number_dimensions;
int number_special_parameters;
int number_plot_variables;

/********************** record 3 ****************/
int x_dimension_size;
int y_dimension_size;
int z_dimension_size;

/********************** record 4 ****************/
float special_parameter_g;
float special_parameter_c;
float special_parameter_th;
float special_parameter_P1;
float special_parameter_P2;
float special_parameter_P3;
float special_parameter_R;
float special_parameter_NX;
float special_parameter_NY;
float special_parameter_NZ;

/********************** record 5 *****************/
/*char     variable_name_string[MAX_VARIABLE_NAME_STRING];*/
char *variable_name_string;

int number_of_cells;

/********************** record 6 ***************/
float *x_position_arrayPtr;
float *y_position_arrayPtr;
float *z_position_arrayPtr;

/******************** record 7 ********************/
float *rho_arrayPtr;
float *ux_arrayPtr;
float *uy_arrayPtr;
float *uz_arrayPtr;
float *swrho_arrayPtr;
float *swux_arrayPtr;
float *swuy_arrayPtr;
float *swuz_arrayPtr;
float *ionorho_arrayPtr;
float *ionoux_arrayPtr;
float *ionouy_arrayPtr;
float *ionouz_arrayPtr;
float *bx_arrayPtr;
float *by_arrayPtr;
float *bz_arrayPtr;
float *b1x_arrayPtr;
float *b1y_arrayPtr;
float *b1z_arrayPtr;
float *p_arrayPtr;
float *swp_arrayPtr;
float *ionop_arrayPtr;
float *e_arrayPtr;
float *jx_arrayPtr;
float *jy_arrayPtr;
float *jz_arrayPtr;

float *status_arrayPtr;
int optional_status_variable_present;
int optional_swrho_variable_present;
int optional_swp_variable_present;
int optional_swux_variable_present;
int optional_swuy_variable_present;
int optional_swuz_variable_present;
int optional_ionop_variable_present;
int optional_ionorho_variable_present;
int optional_ionoux_variable_present;
int optional_ionouy_variable_present;
int optional_ionouz_variable_present;

/****************** actual min/max variables ******************/

float x_position_actual_min, x_position_actual_max;
float y_position_actual_min, y_position_actual_max;
float z_position_actual_min, z_position_actual_max;
float rho_actual_min, rho_actual_max;
float swrho_actual_min, swrho_actual_max;
float ionorho_actual_min, ionorho_actual_max;
float ux_actual_min, ux_actual_max;
float uy_actual_min, uy_actual_max;
float uz_actual_min, uz_actual_max;
float swux_actual_min, swux_actual_max;
float swuy_actual_min, swuy_actual_max;
float swuz_actual_min, swuz_actual_max;
float ionoux_actual_min, ionoux_actual_max;
float ionouy_actual_min, ionouy_actual_max;
float ionouz_actual_min, ionouz_actual_max;
float bx_actual_min, bx_actual_max;
float by_actual_min, by_actual_max;
float bz_actual_min, bz_actual_max;
float b1x_actual_min, b1x_actual_max;
float b1y_actual_min, b1y_actual_max;
float b1z_actual_min, b1z_actual_max;
float p_actual_min, p_actual_max;
float swp_actual_min, swp_actual_max;
float ionop_actual_min, ionop_actual_max;
float e_actual_min, e_actual_max;
float jx_actual_min, jx_actual_max;
float jy_actual_min, jy_actual_max;
float jz_actual_min, jz_actual_max;

float status_actual_min, status_actual_max;

/****************** Grid Dexription Variables *****************/
int number_of_blocks;

float smallest_cell_size;

float max_amr_level;

float global_x_min;
float global_x_max;
float global_y_min;
float global_y_max;
float global_z_min;
float global_z_max;

float *block_x_min;
float *block_x_max;
float *block_y_min;
float *block_y_max;
float *block_z_min;
float *block_z_max;

float *block_x_center;
float *block_y_center;
float *block_z_center;

float *block_amr_levels;

int *block_at_amr_level;
int *number_of_blocks_at_amr_level;
int *number_of_parent_blocks_at_amr_level;

int *parent_id;
int *child_count;

int *child_id_1;
int *child_id_2;
int *child_id_3;
int *child_id_4;
int *child_id_5;
int *child_id_6;
int *child_id_7;
int *child_id_8;

