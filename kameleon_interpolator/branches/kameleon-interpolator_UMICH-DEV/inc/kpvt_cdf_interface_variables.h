/*******************************************************************************
 *                                                                             *
 *    NAME:          kpvt_cdf_interface_variables.h                            *
 *                                                                             *
 *    VERSION:       1.0                                                       *
 *                                                                             *
 *    AUTHOR:        Marlo M. H. Maddox                                        *
 *                   NASA-GSFC-AETD-ISD-ADMA(587)-CCMC                         *
 *                   Marlo.Maddox@nasa.gov                                     *
 *                   (301) 286 - 5202                                          *
 *                                                                             *
 *    PURPOSE:       Definitions all available kpvt cdf variables              *
 *                                                                             *
 *    INPUTS:                                                                  *
 *                                                                             *
 *    OUTPUTS:       Used by get_kpvt_cdf_info.c, interpolate_kpvt.c,          *
 *                   close_kpvt.c                                              *
 *                                                                             *
 *    FUNCTIONS:                                                               *
 *                                                                             *
 *    VARIABLES:                                                               *
 *                                                                             *
 *                                                                             *
 *    MODIFICATION                                                             *
 *    HISTORY:                                                                 *
 *    11.30.2006     Marlo Maddox                                              *
 *                   Initial Development Started                               *
 *                                                                             *
 ******************************************************************************/

#ifndef KPVT_CDF_INTERFACE_VARAIBLES_H_
#define KPVT_CDF_INTERFACE_VARAIBLES_H_

float *x_pos, *y_pos; /* holds cdf variable values */
long x_cdf_varNum, y_cdf_varNum; /* holds cdf variable number */
static char *x_name = "c_sine_lat", *y_name = "c_lon"; /* holds cdf variable name */

float *net_flux, *total_flux, *weights;
long net_flux_cdf_varNum, total_flux_cdf_varNum, weights_cdf_varNum;
static char *net_flux_name = "net_flux", *total_flux_name = "total_flux",
      *weights_name = "weights";

int nx, ny, nz;
int NX, NY, NZ;
long NX_cdf_num, NY_cdf_num, NZ_cdf_num;

static char *NX_name = "grid_system_1_dimension_1_size", *NY_name =
      "grid_system_1_dimension_2_size", *NZ_name =
      "grid_system_1_dimension_3_size";

#endif /*KPVT_CDF_INTERFACE_VARAIBLES_H_*/

