/*******************************************************************************
 *                                                                             *
 *       NAME:           ucla_ggcm_cdf_interface_variables.h                   *
 *                                                                             *
 *       VERSION:        1.0                                                   *
 *                                                                             *
 *       AUTHOR:         Marlo M. H. Maddox                                    *
 *                       NASA-GSFC-AETD-ISD-ADMA(587)-CCMC                     *
 *                       Marlo.Maddox@nasa.gov                                 *
 *                       (301) 286 - 5202                                      *
 *                                                                             *
 *       PURPOSE:        Definitions all available ctip cdf variables          *
 *                                                                             *
 *       INPUTS:                                                               *
 *                                                                             *
 *       OUTPUTS:        used by get_ucla_ggcm_cdf_info.c,                     *
 *                       interpolate_ucla_ggcm.c, close_ucla_ggcm.c            *
 *                                                                             *
 *       FUNCTIONS:                                                            *
 *                                                                             *
 *       VARIABLES:                                                            *
 *                                                                             *
 *                                                                             *
 *       MODIFICATION                                                          *
 *       HISTORY:                                                              *
 *                                                                             *
 *       07.29.2008     Maddox, Marlo                                          *
 *                      Adding eta variable for ucla_ggcm/open_ggcm            *
 ******************************************************************************/

#ifndef OpenGGCM_CDF_INTERFACE_VARIABLES_
#define OpenGGCM_CDF_INTERFACE_VARIABLES_

float *x_pos, *y_pos, *z_pos; /* holds cdf variable values */
long x_cdf_varNum, y_cdf_varNum, z_cdf_varNum; /* holds cdf variable number */
static char *x_name = "x", *y_name = "y", *z_name = "z"; /* holds cdf variable name */

float *x_bx_pos, *y_bx_pos, *z_bx_pos;
long x_bx_cdf_varNum, y_bx_cdf_varNum, z_bx_cdf_varNum;
static char *x_bx_name = "x_bx", *y_bx_name = "y_bx", *z_bx_name = "z_bx";

float *x_by_pos, *y_by_pos, *z_by_pos;
long x_by_cdf_varNum, y_by_cdf_varNum, z_by_cdf_varNum;
static char *x_by_name = "x_by", *y_by_name = "y_by", *z_by_name = "z_by";

float *x_bz_pos, *y_bz_pos, *z_bz_pos;
long x_bz_cdf_varNum, y_bz_cdf_varNum, z_bz_cdf_varNum;
static char *x_bz_name = "x_bz", *y_bz_name = "y_bz", *z_bz_name = "z_bz";

float *bx, *by, *bz;
long bx_cdfNum, by_cdfNum, bz_cdfNum;
static char *bx_name = "bx", *by_name = "by", *bz_name = "bz";

float *bx1, *by1, *bz1;
long bx1_cdfNum, by1_cdfNum, bz1_cdfNum;
static char *bx1_name = "bx1", *by1_name = "by1", *bz1_name = "bz1";

float *ux, *uy, *uz;
long ux_cdfNum, uy_cdfNum, uz_cdfNum;
static char *ux_name = "ux", *uy_name = "uy", *uz_name = "uz";

float *jx, *jy, *jz;
long jx_cdfNum, jy_cdfNum, jz_cdfNum;
static char *jx_name = "jx", *jy_name = "jy", *jz_name = "jz";

float *rho, *p, *eta;
long rho_cdfNum, p_cdfNum, eta_cdfNum;
static char *rho_name = "rho", *p_name = "p", *eta_name = "eta";

int nx, ny, nz;
int NX, NY, NZ;
long NX_cdf_num, NY_cdf_num, NZ_cdf_num;
static char *NX_name = "grid_system_1_dimension_1_size", *NY_name =
      "grid_system_1_dimension_2_size", *NZ_name =
      "grid_system_1_dimension_3_size";

#endif /* OpenGGCM_CDF_INTERFACE_VARIABLES */

