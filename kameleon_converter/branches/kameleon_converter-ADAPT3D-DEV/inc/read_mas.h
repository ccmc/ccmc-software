/*******************************************************************************
 *                                                                             *
 *       NAME:          read_mas.h                                             *
 *                                                                             *
 *       VERSION:       1.0                                                    *
 *                                                                             *
 *       AUTHOR:        Marlo Maddox                                           *
 *                      NASA/GSFC/AETD/ADMA/CCMC                               *
 *                      Marlo.Maddox@nasa.gov                                  *
 *                      (301) 286 - 5202                                       *
 *                                                                             *
 *       PURPOSE:       MAS Model Output Ingestion                             *
 *                                                                             *
 *       INPUTS:        *                                                      *
 *                                                                             *
 *       OUTPUTS:       *                                                      *
 *                                                                             *
 *       FUNCTIONS:     *                                                      *
 *                                                                             *
 *       VARIABLES:     *                                                      *
 *                                                                             *
 *                                                                             *
 *       MODIFICATION                                                          *
 *       HISTORY:                                                              *
 *                                                                             *
 *       11.27.2006     Maddox, Marlo                                          *
 *                      initial development started                            *
 *                                                                             *
 *       04.06.2007     Maddox, Marlo                                          *
 *                      adding support for offset position arrays              *
 *                                                                             *
 ******************************************************************************/

/* some global attribute values */
#define MAS_MODEL_DESCRIPTION "MHD Model of Solar Corona. Model domain: 1 - 30 solar radii. - J. Linker,Z. Mikic,R. Lionello, P. Riley - Science Applications International Corporation (SAIC)"

#define MAS_COORDINATE_DISCRIPTION "Heliographic - r,p,t"

#define MAS_DISCIPLINE "Solar"

#define CCMC_USE_POLICY "For tracking purposes for our government sponsors, we ask that you notify the CCMC whenever you use CCMC results in a scientific publications or presentations: ccmc@ccmc.gsfc.nasa.gov "

int current_mas_step;

/** grid size variables **/
int number_of_dimensions;
int fakedim0;
int fakedim1;
int fakedim2;
int fakedim0_plus1;
int fakedim1_plus1;
int fakedim2_plus1;

/** mas hdf4 grid coordinate names **/

static char *dim0_name = "fakeDim0";
static char *dim1_name = "fakeDim1";
static char *dim2_name = "fakeDim2";

/*** data storage ***/

float *dim0, *dim1, *dim2;
float *dim0_plus1, *dim1_plus1, *dim2_plus1; /** extra locations to store offset positions **/

float *t;
float *p;
float *rho;
float *vr, *vp, *vt;
float *jr, *jp, *jt;
float *br, *bp, *bt;

/** actual min/max values **/

float r_plus1_actual_min, r_plus1_actual_max;
float phi_plus1_actual_min, phi_plus1_actual_max;
float theta_plus1_actual_min, theta_plus1_actual_max;
float r_actual_min, r_actual_max;
float phi_actual_min, phi_actual_max;
float theta_actual_min, theta_actual_max;
float t_actual_min, t_actual_max;
float p_actual_min, p_actual_max;
float rho_actual_min, rho_actual_max;
float vr_actual_min, vp_actual_min, vt_actual_min;
float jr_actual_min, jp_actual_min, jt_actual_min;
float br_actual_min, bp_actual_min, bt_actual_min;
float vr_actual_max, vp_actual_max, vt_actual_max;
float jr_actual_max, jp_actual_max, jt_actual_max;
float br_actual_max, bp_actual_max, bt_actual_max;

