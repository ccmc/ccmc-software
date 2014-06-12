/*******************************************************************************
 *                                                                             *
 *       NAME:           read_database.h                                       *
 *                                                                             *
 *       VERSION:        3.2                                                   *
 *                                                                             *
 *       AUTHOR:         Marlo M. H. Maddox                                    *
 *                       NASA-GSFC-AETD-ISD-ADMA(587)                          *
 *                       Marlo.Maddox@nasa.gov                                 *
 *                       (301) 286 - 5202                                      *
 *                                                                             *
 *       PURPOSE:        *                                                     *
 *                                                                             *
 *       INPUTS:         *                                                     *
 *                                                                             *
 *       OUTPUTS:        *                                                     *
 *                                                                             *
 *       FUNCTIONS:      *                                                     *
 *                                                                             *
 *       VARIABLES:      *                                                     *
 *                                                                             *
 *                                                                             *
 *       MODIFICATION                                                          *
 *       HISTORY:                                                              *
 *                                                                             *
 *       09.01.2004     Maddox, Marlo                                          *
 *                      initial development started                            *
 *                                                                             *
 *       09.13.2004     Maddox, Marlo                                          *
 *                      added date/time_cdf_epoch3 starage for date/time       *
 *                      variables request_date & start/end_time                *
 *                                                                             *
 *       02.07.2005     Maddox, Marlo                                          *
 *                      added open_ggcm_init_start_time arrays                 *
 *                                                                             *
 *       03.23.2005     Maddox, Marlo                                          *
 *                      adding dipole_tilt, dipole_tilt_y, run_type, &         *
 *                      dipole_update DatabaseInfo value arrays                *
 *                                                                             *
 *      06.06.2006      Maddox, Marlo                                          *
 *                      Adding arrays for NX,NY,NZ special parameters for      *
 *                      batsrus model.  Latest version of SWMF-BATS .out files *
 *                      do not contain these important values                  *
 *                                                                             *
 *      06.08.2006      Maddox, Marlo                                          *
 *                      Adding arrays for P1,P2,P3 special parameters for      *
 *                      batsrus model.  Latest version of SWMF-BATS .out files *
 *                      do not contain these important values                  *
 *                                                                             *
 *      12.14.2006      Maddox, Marlo                                          *
 *                      Adding carrington_rotation_start and                   *
 *                      carrington_rotation_end                                *
 *                                                                             *
 *      07.18.2008      Rastaetter, Lutz                                       *
 *                      Added Batsrus Special Paramenter th                    *
 ******************************************************************************/

#ifndef READ_DATABASE_H_
#define READ_DATABASE_H_

#define MAX_STRING_LEN_LONG    100
#define MAX_STRING_LEN_SHORT    25

char run_registration_number[MAX_STRING_LEN_LONG];
char generated_by[MAX_STRING_LEN_SHORT]; /* used as last_name_entry */
char request_date[MAX_STRING_LEN_SHORT];
char start_time[MAX_STRING_LEN_SHORT];
char end_time[MAX_STRING_LEN_SHORT];
char r_body[MAX_STRING_LEN_SHORT];
char r_currents[MAX_STRING_LEN_SHORT];
char b_dipole[MAX_STRING_LEN_SHORT];
char dipole_update[MAX_STRING_LEN_SHORT];
char dipole_time[MAX_STRING_LEN_SHORT];
char open_ggcm_init_time[MAX_STRING_LEN_SHORT];
char dipole_tilt[MAX_STRING_LEN_SHORT];
char dipole_tilt_y[MAX_STRING_LEN_SHORT];
char run_type[MAX_STRING_LEN_SHORT];
char cs_output[MAX_STRING_LEN_SHORT]; /* use for grid_*_type attribute */
char carrington_rotation_start[MAX_STRING_LEN_SHORT];
char carrington_rotation_end[MAX_STRING_LEN_SHORT];

char batsrus_special_parameter_NX[MAX_STRING_LEN_SHORT];
char batsrus_special_parameter_NY[MAX_STRING_LEN_SHORT];
char batsrus_special_parameter_NZ[MAX_STRING_LEN_SHORT];
char batsrus_special_parameter_P1[MAX_STRING_LEN_SHORT];
char batsrus_special_parameter_P2[MAX_STRING_LEN_SHORT];
char batsrus_special_parameter_P3[MAX_STRING_LEN_SHORT];
char batsrus_special_parameter_th[MAX_STRING_LEN_SHORT];

/**** add flags to determine if the database info file contained the batsrus_special_parameter_N* ****/

int batsrus_special_parameter_NX_exists_in_databaseinfo;
int batsrus_special_parameter_NY_exists_in_databaseinfo;
int batsrus_special_parameter_NZ_exists_in_databaseinfo;
int batsrus_special_parameter_P1_exists_in_databaseinfo;
int batsrus_special_parameter_P2_exists_in_databaseinfo;
int batsrus_special_parameter_P3_exists_in_databaseinfo;
int batsrus_special_parameter_th_exists_in_databaseinfo;

/******  time will be converted to standard for easy manipulation and consistency ****/

char request_date_cdf_epoch3[MAX_STRING_LEN_SHORT];
char start_time_cdf_epoch3[MAX_STRING_LEN_SHORT];
char end_time_cdf_epoch3[MAX_STRING_LEN_SHORT];
char dipole_time_cdf_epoch3[MAX_STRING_LEN_SHORT];
char open_ggcm_init_time_epoch3[MAX_STRING_LEN_SHORT];

#endif

