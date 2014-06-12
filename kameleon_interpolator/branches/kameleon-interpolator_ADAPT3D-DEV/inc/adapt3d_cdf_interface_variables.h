/*******************************************************************************
 *                                                                             *
 *    NAME:          adapt3d_cdf_interface_variables.h                         *
 *                                                                             *
 *    VERSION:       1.0                                                       *
 *                                                                             *
 *    AUTHOR:        Peter MacNeice                                            *
 *                   Based on  version by Marlo M. H. Maddox                   *
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
 *    03.10.2010     Peter MacNeice                                            *
 *                                                                             *
 ******************************************************************************/

#ifndef ADAPT3D_CDF_INTERFACE_VARAIBLES_H_

#define ADAPT3D_CDF_INTERFACE_VARAIBLES_H_

#define NNODE_ADAPT3D 4
#define NVARS_ADAPT3D 12
#define NDIMN_ADAPT3D 3

#define nx_sg 10
#define ny_sg 10
#define nz_sg 10

#define INNER_RADIUS 1.0
#define OUTER_RADIUS 30.0

int nnode;
int nvars;

float *coord;                                                   /* holds cdf variable values */
float *unkno;                                                   /* holds cdf variable values */
float *rho;                                                         /* holds cdf variable values */
float *bx;                                                          /* holds cdf variable values */
float *by;                                                          /* holds cdf variable values */
float *bz;                                                          /* holds cdf variable values */
float *ux;                                                          /* holds cdf variable values */
float *uy;                                                          /* holds cdf variable values */
float *uz;                                                          /* holds cdf variable values */
float *p;                                                           /* holds cdf variable values */
float *temp;                                                        /* holds cdf variable values */
int    *intmat;                                                      /* holds cdf variable values */
int    *bconi;                                                       /* holds cdf variable values */

long coord_cdf_varNum;                                               /* holds cdf variable number */
long unkno_cdf_varNum;                                               /* holds cdf variable number */
long rho_cdf_varNum;                                                 /* holds cdf variable number */
long bx_cdf_varNum;                                                  /* holds cdf variable number */
long by_cdf_varNum;                                                  /* holds cdf variable number */
long bz_cdf_varNum;                                                  /* holds cdf variable number */
long ux_cdf_varNum;                                                  /* holds cdf variable number */
long uy_cdf_varNum;                                                  /* holds cdf variable number */
long uz_cdf_varNum;                                                  /* holds cdf variable number */
long p_cdf_varNum;                                                   /* holds cdf variable number */
long temp_cdf_varNum;                                                /* holds cdf variable number */
long intmat_cdf_varNum;                                              /* holds cdf variable number */
long bconi_cdf_varNum;                                               /* holds cdf variable number */

static char *coord_name = "coord";                                   /* holds cdf variable name */
static char *unkno_name = "unkno";                                   /* holds cdf variable name */
static char *rho_name = "rho";                                       /* holds cdf variable name */
static char *bx_name = "bx";                                         /* holds cdf variable name */
static char *by_name = "by";                                         /* holds cdf variable name */
static char *bz_name = "bz";                                         /* holds cdf variable name */
static char *ux_name = "ux";                                         /* holds cdf variable name */
static char *uy_name = "uy";                                         /* holds cdf variable name */
static char *uz_name = "uz";                                         /* holds cdf variable name */
static char *p_name = "p";                                           /* holds cdf variable name */
static char *temp_name = "temp";                                     /* holds cdf variable name */
static char *intmat_name = "intmat";                                 /* holds cdf variable name */
static char *bconi_name = "bconi";                                   /* holds cdf variable name */

int ndimn;
int NDIMN;
int grid_reg_no, npoin, nelem, nboun, nconi;
int GRID_REG_NO, NPOIN, NELEM, NBOUN, NCONI;
long GRID_REG_NO_cdf_num, NPOIN_cdf_num, NELEM_cdf_num, NDIMN_cdf_num;
long NBOUN_cdf_num, NCONI_cdf_num;


static char *GRID_REG_NO_name = "grid_reg_no";
static char *NPOIN_name = "npoin";
static char *NELEM_name = "nelem";
static char *NDIMN_name = "ndimn";
static char *NBOUN_name = "nboun";
static char *NCONI_name = "nconi";

/* variables needed for searching unstructured grids */
float            xl_sg,xr_sg,yl_sg,yr_sg,zl_sg,zr_sg;
float            dx_sg,dy_sg,dz_sg;
int               start_index[nz_sg][ny_sg][nx_sg];
int               end_index[nz_sg][ny_sg][nx_sg];
int               *indx;
int               *esup1;
int               *esup2;
int               unstructured_grid_setup_done;
int               last_element_found;

/* support routines */
int setup_search_unstructured_grid();
int find_element(float cintp[NDIMN_ADAPT3D], int clear_cache);
int chkineln(float cintp[NDIMN_ADAPT3D] ,int ielem , float shapex[NNODE_ADAPT3D]);
void smart_search_setup();
int smart_search(float search_point_coords[NDIMN_ADAPT3D]);
int point_within_grid( float scoord[NDIMN_ADAPT3D] );
int index_2d_to_1d( int i1, int i2, int n1, int n2);

#endif /*ADAPT3D_CDF_INTERFACE_VARAIBLES_H_*/

