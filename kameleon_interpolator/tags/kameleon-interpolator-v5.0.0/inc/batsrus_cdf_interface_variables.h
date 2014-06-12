/*******************************************************************************
 *                                                                             *
 *       NAME:           batsrus_cdf_interface_variables.h                     *
 *                                                                             *
 *       VERSION:        1.0                                                   *
 *                                                                             *
 *       AUTHOR:         Marlo M. H. Maddox                                    *
 *                       NASA-GSFC-AETD-ISD-ADMA(587)-CCMC                     *
 *                       Marlo.Maddox@nasa.gov                                 *
 *                       (301) 286 - 5202                                      *
 *                                                                             *
 *       PURPOSE:        Definitions all available batsrus cdf variables       *
 *                                                                             *
 *       INPUTS:                                                               *
 *                                                                             *
 *       OUTPUTS:        used by get_batsrus_cdf_info.c, interpolate_batsrus.c,*
 *                       close_batsrus.c                                       *
 *                                                                             *
 *       FUNCTIONS:                                                            *
 *                                                                             *
 *       VARIABLES:                                                            *
 *                                                                             *
 *                                                                             *
 *       MODIFICATION                                                          *
 *       HISTORY:                                                              *
 *                                                                             *
 ******************************************************************************/

/** prevent the contents of the header from being scanned multiple times and
 *  also solve the problem of multiple inclusion **/

#ifndef BATSRUS_CDF_INTERFACE_VARIABLES_
#define BATSRUS_CDF_INTERFACE_VARIABLES_

int number_of_blocks_plus_virtual;

/* value will be reset by counting number of elements in block_at_amr_level array */
/*static int     number_of_parents_at_amr_level[1] = {0}; */
int number_of_parents_at_amr_level[1];

float *x_pos, *y_pos, *z_pos; /* holds cdf variable values */
long x_cdf_varNum, y_cdf_varNum, z_cdf_varNum; /* holds cdf variable number */
static char *x_name = "x", *y_name = "y", *z_name = "z"; /* holds cdf variable name */

float *bx, *by, *bz;
long bx_cdfNum, by_cdfNum, bz_cdfNum;
static char *bx_name = "bx", *by_name = "by", *bz_name = "bz";

float *b1x, *b1y, *b1z;
long b1x_cdfNum, b1y_cdfNum, b1z_cdfNum;
static char *b1x_name = "b1x", *b1y_name = "b1y", *b1z_name = "b1z";

float *ux, *uy, *uz;
long ux_cdfNum, uy_cdfNum, uz_cdfNum;
static char *ux_name = "ux", *uy_name = "uy", *uz_name = "uz";

float *jx, *jy, *jz;
long jx_cdfNum, jy_cdfNum, jz_cdfNum;
static char *jx_name = "jx", *jy_name = "jy", *jz_name = "jz";

float *rho, *p, *e;
long rho_cdfNum, p_cdfNum, e_cdfNum;
static char *rho_name = "rho", *p_name = "p", *e_name ="e";

int nx, ny, nz;
float NX, NY, NZ;
long NX_cdf_num, NY_cdf_num, NZ_cdf_num;
static char *NX_name = "special_parameter_NX", *NY_name =
      "special_parameter_NX", *NZ_name = "special_parameter_NX";

float *block_x_min_array, *block_y_min_array, *block_z_min_array;
float block_x_min, block_y_min, block_z_min;
long block_x_min_cdfNum, block_y_min_cdfNum, block_z_min_cdfNum;
static char *block_x_min_name = "block_x_min", *block_y_min_name =
      "block_y_min", *block_z_min_name = "block_z_min";

float *block_x_max_array, *block_y_max_array, *block_z_max_array;
float block_x_max, block_y_max, block_z_max;
long block_x_max_cdfNum, block_y_max_cdfNum, block_z_max_cdfNum;
static char *block_x_max_name = "block_x_max", *block_y_max_name =
      "block_y_max", *block_z_max_name = "block_z_max";

float *block_x_center_array, *block_y_center_array, *block_z_center_array;
float block_x_center, block_y_center, block_z_center;
long block_x_center_cdfNum, block_y_center_cdfNum, block_z_center_cdfNum;
static char *block_x_center_name = "block_x_center", *block_y_center_name =
      "block_y_center", *block_z_center_name = "block_z_center";

int *block_at_amr_level;
long block_at_amr_level_cdfNum;
static char *block_at_amr_level_name = "block_at_amr_level";

int number_of_blocks;
long number_of_blocks_cdfNum;
static char *number_of_blocks_name = "number_of_blocks";

int *block_child_count_array;
int block_child_count;
long block_child_count_cdfNum;
static char *block_child_count_name = "block_child_count";

int *block_child_id_1_array, *block_child_id_2_array, *block_child_id_3_array,
      *block_child_id_4_array;
int block_child_id_1, block_child_id_2, block_child_id_3, block_child_id_4;
long block_child_id_1_cdfNum, block_child_id_2_cdfNum, block_child_id_3_cdfNum,
      block_child_id_4_cdfNum;
static char *block_child_id_1_name = "block_child_id_1",
      *block_child_id_2_name = "block_child_id_2", *block_child_id_3_name =
            "block_child_id_3", *block_child_id_4_name = "block_child_id_4";

int *block_child_id_5_array, *block_child_id_6_array, *block_child_id_7_array,
      *block_child_id_8_array;
int block_child_id_5, block_child_id_6, block_child_id_7, block_child_id_8;
long block_child_id_5_cdfNum, block_child_id_6_cdfNum, block_child_id_7_cdfNum,
      block_child_id_8_cdfNum;
static char *block_child_id_5_name = "block_child_id_5",
      *block_child_id_6_name = "block_child_id_6", *block_child_id_7_name =
            "block_child_id_7", *block_child_id_8_name = "block_child_id_8";

#endif /*BATSRUS_CDF_INTERFACE_VARIABLES_*/

