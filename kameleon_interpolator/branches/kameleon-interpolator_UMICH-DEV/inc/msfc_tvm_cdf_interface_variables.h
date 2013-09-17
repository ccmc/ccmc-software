/*******************************************************************************
 *                                                                             *
 *    NAME:          msfc_tvm_cdf_interface_variables.h                        *
 *                                                                             *
 *    VERSION:       1.0                                                       *
 *                                                                             *
 *    AUTHOR:        Marlo M. H. Maddox                                        *
 *                   NASA-GSFC-AETD-ISD-ADMA(587)-CCMC                         *
 *                   Marlo.Maddox@nasa.gov                                     *
 *                   (301) 286 - 5202                                          *
 *                                                                             *
 *    PURPOSE:       Definitions all available msfc_tvm cdf variables          *
 *                                                                             *
 *    INPUTS:                                                                  *
 *                                                                             *
 *    OUTPUTS:       used by get_msfc_tvm_cdf_info.c, interpolate_msfc_tvm.c,  *
 *                   close_msfc_tvm.c                                          *
 *                                                                             *
 *    FUNCTIONS:                                                               *
 *                                                                             *
 *    VARIABLES:                                                               *
 *                                                                             *
 *                                                                             *
 *    MODIFICATION                                                             *
 *    HISTORY:                                                                 *
 *    11.30.2006     Marlo Maddox     Initial Development Started              *
 *                                                                             *
 ******************************************************************************/

#ifndef MSFC_TVM_CDF_INTERFACE_VARAIBLES_H_
#define MSFC_TVM_CDF_INTERFACE_VARAIBLES_H_

float *x_pos, *y_pos; /* holds cdf variable values */
long x_cdf_varNum, y_cdf_varNum; /* holds cdf variable number */
static char *x_name = "lat", *y_name = "lon"; /* holds cdf variable name */

float *b_l, *b_t, *raz;
long b_l_cdf_varNum, b_t_cdf_varNum, raz_cdf_varNum;
static char *b_l_name = "b_l", *b_t_name = "b_t", *raz_name = "raz";

int nx, ny, nz;
int NX, NY, NZ;
long NX_cdf_num, NY_cdf_num, NZ_cdf_num;

static char *NX_name = "grid_system_1_dimension_1_size", *NY_name =
      "grid_system_1_dimension_2_size", *NZ_name =
      "grid_system_1_dimension_3_size";

#endif /*MSFC_TVM_CDF_INTERFACE_VARAIBLES_H_*/

