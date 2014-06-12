/*******************************************************************************
 *                                                                             *
 *       NAME:         mas_cdf_interface_variables.h                           *
 *                                                                             *
 *       VERSION:      1.0                                                     *
 *                                                                             *
 *       AUTHOR:       Marlo M. H. Maddox                                      *
 *                     NASA-GSFC-AETD-ISD-ADMA(587)-CCMC                       *
 *                     Marlo.Maddox@nasa.gov                                   *
 *                     (301) 286 - 5202                                        *
 *                                                                             *
 *       PURPOSE:      Definitions all available mas cdf variables             *
 *                                                                             *
 *       INPUTS:                                                               *
 *                                                                             *
 *       OUTPUTS:      used by get_mas_cdf_info.c, interpolate_mas.c,          *
 *                     close_mas.c                                             *
 *                                                                             *
 *       FUNCTIONS:                                                            *
 *                                                                             *
 *       VARIABLES:                                                            *
 *                                                                             *
 * *                                                                           *
 *       MODIFICATION                                                          *
 *       HISTORY:                                                              *
 *                     Marlo Maddox 02.20.2007    Initial development started  *
 *                                                                             *
 *                     Marlo Maddox 04.09.2007    Adding offset position info  *
 *                     r1 and theta1                                           *
 *                                                                             *
 ******************************************************************************/

#ifndef MAS_CDF_INTERFACE_VARAIBLES_H_
#define MAS_CDF_INTERFACE_VARAIBLES_H_

float *r_pos, *phi_pos, *theta_pos; /* holds cdf variable values */
long r_cdf_varNum, phi_cdf_varNum, theta_cdf_varNum; /* holds cdf variable number */
static char *r_name = "r", *phi_name = "phi", *theta_name = "theta"; /* holds cdf variable name */

float *r1_pos, *theta1_pos;
long *r1_cdf_varNum, theta1_cdf_varNum;
static char *r1_name = "r1", *theta1_name = "theta1";

float *br, *bphi, *btheta;
long br_cdf_varNum, bphi_cdf_varNum, btheta_cdf_varNum;
static char *br_name = "br", *bphi_name = "bphi", *btheta_name = "btheta";

float *ur, *uphi, *utheta;
long ur_cdf_varNum, uphi_cdf_varNum, utheta_cdf_varNum;
static char *ur_name = "ur", *uphi_name = "uphi", *utheta_name = "utheta";

float *jr, *jphi, *jtheta;
long jr_cdf_varNum, jphi_cdf_varNum, jtheta_cdf_varNum;
static char *jr_name = "jr", *jphi_name = "jphi", *jtheta_name = "jtheta";

float *rho, *p, *T;
long rho_cdf_varNum, p_cdf_varNum, T_cdf_varNum;
static char *rho_name = "rho", *p_name = "p", *T_name = "T";

int nx, ny, nz;
int NX, NY, NZ;
long NX_cdf_num, NY_cdf_num, NZ_cdf_num;

int nx_plus1, ny_plus1, nz_plus1;
int NX_plus1, NY_plus1, NZ_plus1;
long NX_plus1_cdf_num, NY_plus1_cdf_num, NZ_plus1_cdf_num;

static char *NX_name = "grid_system_1_dimension_1_size", *NY_name =
      "grid_system_1_dimension_2_size", *NZ_name =
      "grid_system_1_dimension_3_size";

static char *NX_plus1_name = "grid_system_4_dimension_1_size", *NY_plus1_name =
      "grid_system_4_dimension_2_size", *NZ_plus1_name =
      "grid_system_4_dimension_3_size";

#endif /*MAS_CDF_INTERFACE_VARAIBLES_H_*/

