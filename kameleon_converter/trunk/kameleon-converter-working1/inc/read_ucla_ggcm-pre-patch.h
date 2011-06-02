/*******************************************************************************
*                                                                              *
*       NAME:           read_ucla_ggcm.h                                       *
*                                                                              *
*       VERSION:        1.0                                                    *
*                                                                              *
*       AUTHOR:         Marlo M. H. Maddox                                     *
*                       NASA-GSFC-AETD-ISD-ADMA(587)                           *
*                       Marlo.Maddox@nasa.gov                                  *
*                       (301) 286 - 5202                                       *
*                                                                              *
*       PURPOSE:                                                               *
*                                                                              *
*       INPUTS:                                                                *
*                                                                              *
*       OUTPUTS:                                                               *
*                                                                              *
*       FUNCTIONS:                                                             *
*                                                                              *
*       VARIABLES:                                                             *
*                                                                              *
*                                                                              *
*       MODIFICATION                                                           *
*       HISTORY:                                                               *
*                                                                              *
*       03.22.2004    Maddox, Marlo                                            *
*                     initial development started                              *
*                                                                              *
*       03.29.2004    Maddox, Marlo                                            *
*                     made array declarations with = to static types           *
*                     to fix multiplicity error                                *
*                                                                              *
*       07.25.2005    Maddox, Marlo                                            *
*                     Fixed typo that had wrong MAX_GRID_SIZE_*                *
*                     definitions for grid*_b*                                 *
*                                                                              *
*       04.09.2008    Maddox, Marlo                                            *
*                     Changed MAX_GRID_SIZE_X, MAX_GRID_SIZE_Y, MAX_GRID_SIZE_Z*
*                     from 500x300x300 to 1000x500x500                         *
*                                                                              *
*       06.10.2008   Maddox, Marlo                                             *
*                    Changed MAX_GRID_SIZE_X, MAX_GRID_SIZE_Y, MAX_GRID_SIZE_Z *
*                    1000x500x500 to 10000x5000x5000                           *
*                                                                              *
*                                                                              *
*******************************************************************************/


#define MAX_GRID_SIZE_X        10000
#define MAX_GRID_SIZE_Y        5000
#define MAX_GRID_SIZE_Z        5000

int     NX, NY, NZ, dimension_size;    /* dimension_size = NX*NY*NZ were N* is the number of elements in that particular dimmension */

float     gridx[MAX_GRID_SIZE_X];        /* we don't know the dimension sizes until we call getf11 for gridx, gridy, and gridz */
float     gridy[MAX_GRID_SIZE_Y];        /* hope this is large enough ... */
float     gridz[MAX_GRID_SIZE_Z];

float    gridx_bx[MAX_GRID_SIZE_X];    /* bx1, by1, bz1 are on stagered grids seperate from above...thus the following grids are needed*/
float    gridy_bx[MAX_GRID_SIZE_Y];    /* ie. bx1 is defined on gridx_bx, gridy_bx, gridz_bx */
float    gridz_bx[MAX_GRID_SIZE_Z];

float    gridx_by[MAX_GRID_SIZE_X];
float    gridy_by[MAX_GRID_SIZE_Y];
float    gridz_by[MAX_GRID_SIZE_Z];

float    gridx_bz[MAX_GRID_SIZE_X];
float    gridy_bz[MAX_GRID_SIZE_Y];
float    gridz_bz[MAX_GRID_SIZE_Z];

float    *density_rr;            /* 15 data array pointers */
float    *pressure_pp;
float    *velocity_vx;
float    *velocity_vy;
float    *velocity_vz;
float    *magnetic_field_bx;
float    *magnetic_field_by;
float    *magnetic_field_bz;
float    *magnetic_field_bx1;
float    *magnetic_field_by1;
float    *magnetic_field_bz1;
float    *current_density_xjx;
float    *current_density_xjy;
float    *current_density_xjz;
float    *resitivity_resis;

/****************** actual min/max variables ******************/

float gridx_actual_min, gridx_actual_max;
float gridy_actual_min, gridy_actual_max;
float gridz_actual_min, gridz_actual_max;
float gridx_bx_actual_min, gridx_bx_actual_max;
float gridy_bx_actual_min, gridy_bx_actual_max;
float gridz_bx_actual_min, gridz_bx_actual_max;
float gridx_by_actual_min, gridx_by_actual_max;
float gridy_by_actual_min, gridy_by_actual_max;
float gridz_by_actual_min, gridz_by_actual_max;
float gridx_bz_actual_min, gridx_bz_actual_max;
float gridy_bz_actual_min, gridy_bz_actual_max;
float gridz_bz_actual_min, gridz_bz_actual_max;
float density_rr_actual_min, density_rr_actual_max;
float pressure_pp_actual_min, pressure_pp_actual_max;
float velocity_vx_actual_min, velocity_vx_actual_max;
float velocity_vy_actual_min, velocity_vy_actual_max;
float velocity_vz_actual_min, velocity_vz_actual_max;
float magnetic_field_bx_actual_min, magnetic_field_bx_actual_max;
float magnetic_field_by_actual_min, magnetic_field_by_actual_max;
float magnetic_field_bz_actual_min, magnetic_field_bz_actual_max;
float magnetic_field_bx1_actual_min, magnetic_field_bx1_actual_max;
float magnetic_field_by1_actual_min, magnetic_field_by1_actual_max;
float magnetic_field_bz1_actual_min, magnetic_field_bz1_actual_max;
float current_density_xjx_actual_min, current_density_xjx_actual_max;
float current_density_xjy_actual_min, current_density_xjy_actual_max;
float current_density_xjz_actual_min, current_density_xjz_actual_max;
float resitivity_resis_actual_min, resitivity_resis_actual_max;



static     char     gridx_name[80]     =     { 'g','r','i','d','x', '\0' };        /* field identifier tags used in fortran calls */
static     char     gridy_name[80]     =     { 'g','r','i','d','y', '\0' };
static     char     gridz_name[80]     =     { 'g','r','i','d','z', '\0' };

static    char    gridx_bx_name[80] =     { 'g','x','-','b','x', '\0' };
static     char    gridx_by_name[80] =     { 'g','x','-','b','y', '\0' };
static     char    gridx_bz_name[80] =     { 'g','x','-','b','z', '\0' };

static     char    gridy_bx_name[80] =     { 'g','y','-','b','x', '\0' };
static     char    gridy_by_name[80] =     { 'g','y','-','b','y', '\0' };
static     char    gridy_bz_name[80] =     { 'g','y','-','b','z', '\0' };

static     char    gridz_bx_name[80] =     { 'g','z','-','b','x', '\0' };
static     char    gridz_by_name[80] =     { 'g','z','-','b','y', '\0' };
static     char    gridz_bz_name[80] =     { 'g','z','-','b','z', '\0' };

static     char    density_rr_name[80]         = { 'r', 'r', '\0' };
static     char    pressure_pp_name[80]         = { 'p', 'p', '\0' };
static     char    velocity_vx_name[80]        = { 'v', 'x', '\0' };
static     char    velocity_vy_name[80]        = { 'v', 'y', '\0' };
static     char    velocity_vz_name[80]        = { 'v', 'z', '\0' };
static     char    magnetic_field_bx_name[80]    = { 'b', 'x', '\0' };
static     char    magnetic_field_by_name[80]    = { 'b', 'y', '\0' };
static     char    magnetic_field_bz_name[80]    = { 'b', 'z', '\0' };
static     char    magnetic_field_bx1_name[80]    = { 'b', 'x', '1', '\0' };
static     char    magnetic_field_by1_name[80]    = { 'b', 'y', '1', '\0' };
static     char    magnetic_field_bz1_name[80]    = { 'b', 'z', '1', '\0' };
static     char    current_density_xjx_name[80]    = { 'x', 'j', 'x', '\0' };
static     char    current_density_xjy_name[80]    = { 'x', 'j', 'y', '\0' };
static     char    current_density_xjz_name[80]    = { 'x', 'j', 'z', '\0' };

char    l2[80];        /* character tag for fortran mhdread (ucla) */
static     int     it = 0;        /* integer tag for fortran mhdread (ucla) */

