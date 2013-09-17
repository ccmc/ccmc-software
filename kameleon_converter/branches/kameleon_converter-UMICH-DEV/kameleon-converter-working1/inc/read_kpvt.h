/*******************************************************************************
 *                                                                             *
 *       NAME:          read_kpvt.h                                            *
 *                                                                             *
 *       VERSION:       1.0                                                    *
 *                                                                             *
 *       AUTHOR:        Marlo Maddox                                           *
 *                      NASA/GSFC/AETD/ADMA/CCMC                               *
 *                      Marlo.Maddox@nasa.gov                                  *
 *                      (301) 286 - 5202                                       *
 *                                                                             *
 *       PURPOSE:       Kitt Peak Vacuum Telescope Magnetogram Data Ingestion  *
 *                                                                             *
 *       INPUTS:                                                               *
 *                                                                             *
 *       OUTPUTS:                                                              *
 *                                                                             *
 *       FUNCTIONS:                                                            *
 *                                                                             *
 *       VARIABLES:                                                            *
 *                                                                             *
 *                                                                             *
 *       MODIFICATION                                                          *
 *       HISTORY:                                                              *
 *                                                                             *
 *       11.27.2006     Maddox, Marlo                                          *
 *                      initial development started                            *
 *                                                                             *
 ******************************************************************************/

/* some global attribute values */
#define KPTV_MODEL_DESCRIPTION  "********** PROTOTYPE ************ Kitt Peak Vacuum Telescope Observation"

#define KPVT_COORDINATE_DISCRIPTION "********** PROTOTYPE ************ Carrington Longitude x Carrington sine Latitude"

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

/*** data storage ***/

float *carrington_longitiude, *carrington_sine_latitude;
float **net_flux_2d, **total_flux_2d, **weights_2d; /** 2 d arrays for fits lib **/
float *net_flux, *total_flux, *weights; /** flat arrays for conversion **/

/** actual min/max values **/

float carrington_longitiude_actual_min, carrington_longitiude_actual_max;
float carrington_sine_latitude_actual_min, carrington_sine_latitude_actual_max;
float net_flux_actual_min, net_flux_actual_max;
float total_flux_actual_min, total_flux_actual_max;
float weights_actual_min, weights_actual_max;

