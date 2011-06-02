/*******************************************************************************
 *                                                                             *
 *       NAME:        read_kpvt.h                                              *
 *                                                                             *
 *       VERSION:     1.0                                                      *
 *                                                                             *
 *       AUTHOR:      Marlo Maddox                                             *
 *                    NASA/GSFC/AETD/ADMA/CCMC                                 *
 *                    Marlo.Maddox@nasa.gov                                    *
 *                    (301) 286 - 5202                                         *
 *                                                                             *
 *       PURPOSE:     Marshall Space Flight Center Tower Vector Magnetograph   *
 *                    Telescope Magnetogram Data Ingestion                     *
 *                                                                             *
 *       INPUTS:      *                                                        *
 *                                                                             *
 *       OUTPUTS:     *                                                        *
 *                                                                             *
 *       FUNCTIONS:   *                                                        *
 *                                                                             *
 *       VARIABLES:   *                                                        *
 *                                                                             *
 *                                                                             *
 *       MODIFICATION                                                          *
 *       HISTORY:                                                              *
 *                                                                             *
 *       12.04.2006   Maddox, Marlo                                            *
 *                    initial development started                              *
 *                                                                             *
 ******************************************************************************/

/* some global attribute values */
#define MSFC_TVM_MODEL_DESCRIPTION "********** PROTOTYPE ************ Marshall Space Flight Center Tower Vector Magnetograph Telescope Observation"

#define KPVT_COORDINATE_DISCRIPTION "********** PROTOTYPE ************ Longitude x Latitude"

#define KPVT_DISCIPLINE "********** PROTOTYPE ************ Solar Observation"

#define CCMC_USE_POLICY "For tracking purposes for our government sponsors, we ask that you notify the CCMC whenever you use CCMC results in a scientific publications or presentations: ccmc@ccmc.gsfc.nasa.gov "

/*** FITS HEADER Metadata ***/

/** grid size variables **/
int naxis;
int naxis1;
int naxis2;
int naxis3;
int *naxis_size;

long number_of_elements;

char *date;
char *telescope;
char *instrument;
char *observation_site;

float carrington_rotation;
float sun_angle;

/*** data storage ***/

float *longitude, *latitude;
float *theta, *phi;
float *theta_rad, *phi_rad;
float *theta_deg, *phi_deg;
float *b_l, *b_t, *raz; /** flat arrays for conversion **/

/** actual min/max values **/

float longitude_actual_min, longitude_actual_max;
float latitude_actual_min, latitude_actual_max;
float b_l_actual_min, b_l_actual_max;
float b_t_actual_min, b_t_actual_max;
float raz_actual_min, raz_actual_max;

