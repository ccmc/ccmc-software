/*******************************************************************************
 *                                                                             *
 *       NAME:          enlil_cdf_interface_variables.h                        *
 *                                                                             *
 *       VERSION:       1.0                                                    *
 *                                                                             *
 *       AUTHOR:        Marlo M. H. Maddox                                     *
 *                      NASA-GSFC-AETD-ISD-ADMA(587)-CCMC                      *
 *                      Marlo.Maddox@nasa.gov                                  *
 *                      (301) 286 - 5202                                       *
 *                                                                             *
 *       PURPOSE:       Definitions all available enlil cdf variables          *
 *                                                                             *
 *       INPUTS:                                                               *
 *                                                                             *
 *       OUTPUTS:       used by get_enlil_cdf_info.c, interpolate_enlil.c,     *
 *                      close_enlil.c                                          *
 *                                                                             *
 *       FUNCTIONS:                                                            *
 *                                                                             *
 *       VARIABLES:                                                            *
 *                                                                             *
 *                                                                             *
 *       MODIFICATION                                                          *
 *       HISTORY:                                                              *
 *       08.29.2006     Marlo Maddox                                           *
 *                      Adding b1x, b1y,b1z                                    *
 *                                                                             *
 *       08.01.2008     Marlo Maddox                                           *
 *                      Adding r. phi. theta based variables names             *
 *                                                                             *
 ******************************************************************************/

#ifndef ENLIL_CDF_INTERFACE_VARAIBLES_H_
#define ENLIL_CDF_INTERFACE_VARAIBLES_H_

float *x_pos, *y_pos, *z_pos; /* holds cdf variable values */
long x_cdf_varNum, y_cdf_varNum, z_cdf_varNum; /* holds cdf variable number */
static char *x_name = "x", *y_name = "y", *z_name = "z"; /* holds cdf variable name */

static char *r_name = "r", *phi_name = "phi", *theta_name = "theta"; /* holds cdf variable name */

float *bx, *by, *bz;
long bx_cdf_varNum, by_cdf_varNum, bz_cdf_varNum;
static char *bx_name = "bx", *by_name = "by", *bz_name = "bz";

static char *br_name = "br", *bphi_name = "bphi", *btheta_name = "btheta";

float *b1x, *b1y, *b1z;
long b1x_cdf_varNum, b1y_cdf_varNum, b1z_cdf_varNum;
static char *b1x_name = "b1x", *b1y_name = "b1y", *b1z_name = "b1z";

static char *b1r_name = "b1r", *b1phi_name = "b1phi", *b1theta_name = "b1theta";

float *ux, *uy, *uz;
long ux_cdf_varNum, uy_cdf_varNum, uz_cdf_varNum;
static char *ux_name = "ux", *uy_name = "uy", *uz_name = "uz";

static char *ur_name = "ur", *uphi_name = "uphi", *utheta_name = "utheta";

float *rho, *dp, *T, *bp;
long rho_cdf_varNum, dp_cdf_varNum, T_cdf_varNum, bp_cdf_varNum;
static char *rho_name = "rho", *dp_name = "dp", *T_name = "T", *bp_name ="bp";

int nx, ny, nz;
int NX, NY, NZ;
long NX_cdf_num, NY_cdf_num, NZ_cdf_num;

static char *NX_name = "grid_system_1_dimension_1_size", *NY_name =
      "grid_system_1_dimension_2_size", *NZ_name =
      "grid_system_1_dimension_3_size";

#endif /*ENLIL_CDF_INTERFACE_VARAIBLES_H_*/

