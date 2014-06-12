/*******************************************************************************
 *                                                                             *
 *    NAME:       ctip_cdf_interface_variables.h                               *
 *                                                                             *
 *    VERSION:    1.0                                                          *
 *                                                                             *
 *    AUTHOR:     Marlo M. H. Maddox                                           *
 *                NASA-GSFC-AETD-ISD-ADMA(587)-CCMC                            *
 *                Marlo.Maddox@nasa.gov                                        *
 *                (301) 286 - 5202                                             *
 *                                                                             *
 *    PURPOSE:    Definitions all available ctip cdf variables                 *
 *                                                                             *
 *    INPUTS:                                                                  *
 *                                                                             *
 *    OUTPUTS:    used by get_ctip_cdf_info.c, interpolate_ctip.c, close_ctip.c*
 *                                                                             *
 *    FUNCTIONS:                                                               *
 *                                                                             *
 *    VARIABLES:                                                               *
 *                                                                             *
 *                                                                             *
 *    MODIFICATION                                                             *
 *    HISTORY:                                                                 *
 *                                                                             *
 ******************************************************************************/

#ifndef CTIP_CDF_INTERFACE_VARIABLES_
#define CTIP_CDF_INTERFACE_VARIABLES_

float *x_pos, *y_pos, *z_pos, *H_pos; /* holds cdf variable values */
long x_cdf_varNum, y_cdf_varNum, z_cdf_varNum, H_cdf_varNum; /* holds cdf variable number */
static char *x_name = "x", *y_name = "y", *z_name = "z", *H_name = "H"; /* holds cdf variable name */

float *Vn_x, *Vn_y, *Vn_z;
long Vn_x_cdf_varNum, Vn_y_cdf_varNum, Vn_z_cdf_varNum;
static char *Vn_x_name = "Vn_x", *Vn_y_name = "Vn_y", *Vn_z_name = "Vn_z";

float *Vi_x, *Vi_y;
long Vi_x_cdf_varNum, Vi_y_cdf_varNum;
static char *Vi_x_name = "Vi_x", *Vi_y_name = "Vi_y";

float *N_e, *N_O, *N_O2, *N_N2;
long N_e_cdf_varNum, N_O_cdf_varNum, N_O2_cdf_varNum, N_N2_cdf_varNum;
static char *N_e_name = "N_e", *N_O_name = "N_O", *N_O2_name = "N_O2",
      *N_N2_name = "N_N2";

float *Rmt, *Tn;
long Rmt_cdf_varNum, Tn_cdf_varNum;
static char *Rmt_name = "Rmt", *Tn_name = "Tn";

float *sigmaP, *sigmaH;
long sigmaP_cdf_varNum, sigmaH_cdf_varNum;
static char *sigmaP_name = "sigmaP", *sigmaH_name = "sigmaH";

int nx, ny, nz, nh;
int NX, NY, NZ, NH;
long NX_cdf_num, NY_cdf_num, NZ_cdf_num, NH_cdf_num;
static char *NX_name = "grid_system_1_dimension_2_size", *NY_name =
      "grid_system_1_dimension_3_size", *NZ_name =
      "grid_system_1_dimension_1_size";
static char *NH_name = "grid_system_2_dimension_1_size";

#endif /* CTIP_CDF_INTERFACE_VARIABLES_ */

