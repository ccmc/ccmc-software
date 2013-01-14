/*******************************************************************************
 *                                                                             *
 *    NAME:        ccmc_registered_variables_.h                                *
 *                                                                             *
 *    VERSION:     2.0                                                         *
 *                                                                             *
 *    AUTHOR:      Marlo M. H. Maddox                                          *
 *                 NASA-GSFC-AETD-ISD-ADMA(587)                                *
 *                 Marlo.Maddox@nasa.gov                                       *
 *                 (301) 286 - 5202                                            *
 *                                                                             *
 *    PURPOSE:     centralized location for registerd CCMC                     *
 *                 variable names. To be used for data format                  *
 *                 standarization through conversion software                  *
 *                 ALSO build model specific arrays to be exported to general  *
 *                 variable array used by all write routines                   *
 *                                                                             *
 *    INPUTS:      NONE                                                        *
 *                                                                             *
 *    OUTPUTS:     NONE                                                        *
 *                                                                             *
 *    FUNCTIONS:   NONE                                                        *
 *                                                                             *
 *    VARIABLES:   - MACRO variable names registered within the CCMC           *
 *                                                                             *
 *    MODIFICATION                                                             *
 *    HISTORY:                                                                 *
 *                                                                             *
 *    09.11.2003   Maddox, Marlo                                               *
 *                 initial development started                                 *
 *                                                                             *
 *    03.26.2004   Maddox, Marlo                                               *
 *                 made array declarations static                              *
 *                                                                             *
 *    05.09.2005   Maddox, Marlo                                               *
 *                 registering additional variables used by ctip               *
 *                                                                             *
 *    03.29.2006   Maddox, Marlo                                               *
 *                 registering additional variables used by ENLIL.             *
 *                 Adding duplicates for some existing values since ENLIL      *
 *                 will have 6 grids all with virtually the same variables     *
 *                                                                             *
 *    10.16.2006   Maddox, Marlo                                               *
 *                 Cleaning up some typos...                                   *
 *                 Atron->Astron                                               *
 *                                                                             *
 *    10.17.2006   Maddox, Marlo                                               *
 *                 For some reason changes made to 4.3.0 were lost in 4.3.1    *
 *                 had to re-add b1x,b1y,b1z for enlil                         *
 *    10.19.2006   Maddox, Marlo                                               *
 *                 adding variables:                                           *
 *                                     (1)time_[1-4]                           *
 *                                     (2)time_step_[1-4]                      *
 *                                     (3)numerical_step_[1-4]                 *
 *                                                                             *
 *    11.29.2006   Maddox, Marlo                                               *
 *                 Working On KPTV - adding variables:                         *
 *                                     (1)c_sine_lat                           *
 *                                     (2)c_lon                                *
 *                                     (3)wt                                   *
 *                                     (4)net_flux                             *
 *                                     (5)total_flux                           *
 *                                                                             *
 *    12.04.2006   Maddox, Marlo                                               *
 *                 Working On MSFC_TVM - adding variables:                     *
 *                                     (1)lat                                  *
 *                                     (2)lon                                  *
 *                                     (3)b_l                                  *
 *                                     (4)b_t                                  *
 *                                     (5)raz                                  *
 *                                                                             *
 *    02.09.2007   Maddox, Marlo                                               *
 *                 Working on MAS, we need to add r, phi, theta                *
 *                 then rename all of the coordinates that use x,y,z that      *
 *                 should really be r, phi, theta                              *
 *                 ...also adde br,bphi,btheta, ur,uphi,utheta,jr,jphi,jtheta  *
 *                                                                             *
 *    03.15.2007   Maddox, Marlo                                               *
 *                 Finally fixing typos excedded -> exceeded                   *
 *                 and changing mask values from 123... to FLOAT|INT_MASK      *
 *                                                                             *
 *    05.15.2007   Maddox, Marlo                                               *
 *                 Adding Variables to modify ENLIL variable descriptions      *
 *                 ie x->r, y->phi, z->theta include vector components b*, u*  *
 *                                                                             *
 *    07.20.2008   Rastaetter, Lutz                                            *
 *                 Added include file for Missing values                       *
 *                 							       *
 *    08.30.2011   Maddox, Marlo					       *
 *    		   Adding new "status" variable to batsrus                     *
 ******************************************************************************/
#include "ccmc_missing.h"

/** prevent the contents of the header from being scanned multiple times and also solve the problem of multiple inclusion **/

#ifndef CCMC_REGISTERED_VARIABLES_H_
#define CCMC_REGISTERED_VARIABLES_H_

/** define registered ccmc variable names, default variable attribute values, & extra display properties **/

/******************************************************************************************************
 *                                                                                                 *
 *                                         POSITION VARIABLES                                        *
 *                                                                                                 *
 *******************************************************************************************************/

/*******************************************************************************************************/
/**                                        polar                                                    **/
/*******************************************************************************************************/

#define    CCMC_VARIABLE_NAME_R                            "r"
#define    CCMC_VARIABLE_NAME_R_DATA_TYPE                "float"
#define    CCMC_VARIABLE_NAME_R_CLASSIFICATION            "position"
#define    CCMC_VARIABLE_NAME_R_VALID_MIN                0.0
#define    CCMC_VARIABLE_NAME_R_VALID_MAX                10000.0
#define    CCMC_VARIABLE_NAME_R_UNITS                    "R"
#define    CCMC_VARIABLE_NAME_R_GRID_SYSTEM                "grid_system_1"
#define     CCMC_VARIABLE_NAME_R_MASK                        FLOAT_MASK
#define    CCMC_VARIABLE_NAME_R_DESCRIPTION                "radius r in spherical coordinates, R = R_E or R_S or AU, 1 R_E = 6371 km ( Earth ), 1 R_S = 696000 km ( Sun ), 1 AU = 1.5e8 km ( Astron - Unit for Heliosphere )"
#define    CCMC_VARIABLE_NAME_R_IS_VECTOR                0
#define    CCMC_VARIABLE_NAME_R_POSITION_GRID_SYSTEM        "grid_system_1"
#define    CCMC_VARIABLE_NAME_R_DATA_GRID_SYSTEM            "grid_system_1"

#define    CCMC_VARIABLE_NAME_R1                            "r1"
#define    CCMC_VARIABLE_NAME_R1_DATA_TYPE                "float"
#define    CCMC_VARIABLE_NAME_R1_CLASSIFICATION            "position"
#define    CCMC_VARIABLE_NAME_R1_VALID_MIN                0.0
#define    CCMC_VARIABLE_NAME_R1_VALID_MAX                10000.0
#define    CCMC_VARIABLE_NAME_R1_UNITS                    "R"
#define    CCMC_VARIABLE_NAME_R1_GRID_SYSTEM                "grid_system_1"
#define    CCMC_VARIABLE_NAME_R1_MASK                    FLOAT_MASK
#define    CCMC_VARIABLE_NAME_R1_DESCRIPTION                "radius r in spherical coordinates, R = R_E or R_S or AU, 1 R_E = 6371 km ( Earth ), 1 R_S = 696000 km ( Sun ), 1 AU = 1.5e8 km ( Astron - Unit for Heliosphere )"
#define    CCMC_VARIABLE_NAME_R1_IS_VECTOR                0
#define    CCMC_VARIABLE_NAME_R1_POSITION_GRID_SYSTEM        "grid_system_1"
#define    CCMC_VARIABLE_NAME_R1_DATA_GRID_SYSTEM            "grid_system_1"

#define    CCMC_VARIABLE_NAME_R2                            "r2"
#define    CCMC_VARIABLE_NAME_R2_DATA_TYPE                "float"
#define    CCMC_VARIABLE_NAME_R2_CLASSIFICATION            "position"
#define    CCMC_VARIABLE_NAME_R2_VALID_MIN                0.0
#define    CCMC_VARIABLE_NAME_R2_VALID_MAX                10000.0
#define    CCMC_VARIABLE_NAME_R2_UNITS                    "R"
#define    CCMC_VARIABLE_NAME_R2_GRID_SYSTEM                "grid_system_3"
#define    CCMC_VARIABLE_NAME_R2_MASK                    FLOAT_MASK
#define    CCMC_VARIABLE_NAME_R2_DESCRIPTION                "radius r in spherical coordinates, R = R_E or R_S or AU, 1 R_E = 6371 km ( Earth ), 1 R_S = 696000 km ( Sun ), 1 AU = 1.5e8 km ( Astron - Unit for Heliosphere )"
#define    CCMC_VARIABLE_NAME_R2_IS_VECTOR                0
#define    CCMC_VARIABLE_NAME_R2_POSITION_GRID_SYSTEM        "grid_system_3"
#define    CCMC_VARIABLE_NAME_R2_DATA_GRID_SYSTEM            "grid_system_3"

#define    CCMC_VARIABLE_NAME_R3                            "r3"
#define    CCMC_VARIABLE_NAME_R3_DATA_TYPE                "float"
#define    CCMC_VARIABLE_NAME_R3_CLASSIFICATION            "position"
#define    CCMC_VARIABLE_NAME_R3_VALID_MIN                0.0
#define    CCMC_VARIABLE_NAME_R3_VALID_MAX                10000.0
#define    CCMC_VARIABLE_NAME_R3_UNITS                    "R"
#define    CCMC_VARIABLE_NAME_R3_GRID_SYSTEM                "grid_system_4"
#define    CCMC_VARIABLE_NAME_R3_MASK                    FLOAT_MASK
#define    CCMC_VARIABLE_NAME_R3_DESCRIPTION                "radius r in spherical coordinates, R = R_E or R_S or AU, 1 R_E = 6371 km ( Earth ), 1 R_S = 696000 km ( Sun ), 1 AU = 1.5e8 km ( Astron - Unit for Heliosphere )"
#define    CCMC_VARIABLE_NAME_R3_IS_VECTOR                0
#define    CCMC_VARIABLE_NAME_R3_POSITION_GRID_SYSTEM        "grid_system_4"
#define    CCMC_VARIABLE_NAME_R3_DATA_GRID_SYSTEM            "grid_system_4"

#define    CCMC_VARIABLE_NAME_R4                            "r4"
#define    CCMC_VARIABLE_NAME_R4_DATA_TYPE                "float"
#define    CCMC_VARIABLE_NAME_R4_CLASSIFICATION            "position"
#define    CCMC_VARIABLE_NAME_R4_VALID_MIN                0.0
#define    CCMC_VARIABLE_NAME_R4_VALID_MAX                10000.0
#define    CCMC_VARIABLE_NAME_R4_UNITS                    "R"
#define    CCMC_VARIABLE_NAME_R4_GRID_SYSTEM                "grid_system_5"
#define    CCMC_VARIABLE_NAME_R4_MASK                        FLOAT_MASK
#define    CCMC_VARIABLE_NAME_R4_DESCRIPTION                "radius r in spherical coordinates, R = R_E or R_S or AU, 1 R_E = 6371 km ( Earth ), 1 R_S = 696000 km ( Sun ), 1 AU = 1.5e8 km ( Astron - Unit for Heliosphere )"
#define    CCMC_VARIABLE_NAME_R4_IS_VECTOR                0
#define    CCMC_VARIABLE_NAME_R4_POSITION_GRID_SYSTEM        "grid_system_5"
#define    CCMC_VARIABLE_NAME_R4_DATA_GRID_SYSTEM            "grid_system_5"

#define    CCMC_VARIABLE_NAME_R5                            "r5"
#define    CCMC_VARIABLE_NAME_R5_DATA_TYPE                "float"
#define    CCMC_VARIABLE_NAME_R5_CLASSIFICATION            "position"
#define    CCMC_VARIABLE_NAME_R5_VALID_MIN                0.0
#define    CCMC_VARIABLE_NAME_R5_VALID_MAX                10000.0
#define    CCMC_VARIABLE_NAME_R5_UNITS                    "R"
#define    CCMC_VARIABLE_NAME_R5_GRID_SYSTEM                "grid_system_6"
#define    CCMC_VARIABLE_NAME_R5_MASK                        FLOAT_MASK
#define    CCMC_VARIABLE_NAME_R5_DESCRIPTION                "radius r in spherical coordinates, R = R_E or R_S or AU, 1 R_E = 6371 km ( Earth ), 1 R_S = 696000 km ( Sun ), 1 AU = 1.5e8 km ( Astron - Unit for Heliosphere )"
#define    CCMC_VARIABLE_NAME_R5_IS_VECTOR                0
#define    CCMC_VARIABLE_NAME_R5_POSITION_GRID_SYSTEM        "grid_system_6"
#define    CCMC_VARIABLE_NAME_R5_DATA_GRID_SYSTEM            "grid_system_6"

#define    CCMC_VARIABLE_NAME_PHI                        "phi"
#define    CCMC_VARIABLE_NAME_PHI_DATA_TYPE                "float"
#define    CCMC_VARIABLE_NAME_PHI_CLASSIFICATION            "position"
#define    CCMC_VARIABLE_NAME_PHI_VALID_MIN                0.0
#define    CCMC_VARIABLE_NAME_PHI_VALID_MAX                360.0
#define    CCMC_VARIABLE_NAME_PHI_UNITS                    "degrees"
#define    CCMC_VARIABLE_NAME_PHI_GRID_SYSTEM            "grid_system_1"
#define CCMC_VARIABLE_NAME_PHI_MASK                    FLOAT_MASK
#define    CCMC_VARIABLE_NAME_PHI_DESCRIPTION            "angle phi in spherical coordinates"
#define    CCMC_VARIABLE_NAME_PHI_IS_VECTOR                0
#define    CCMC_VARIABLE_NAME_PHI_POSITION_GRID_SYSTEM    "grid_system_1"
#define    CCMC_VARIABLE_NAME_PHI_DATA_GRID_SYSTEM        "grid_system_1"

#define    CCMC_VARIABLE_NAME_PHI1                        "phi1"
#define    CCMC_VARIABLE_NAME_PHI1_DATA_TYPE                "float"
#define    CCMC_VARIABLE_NAME_PHI1_CLASSIFICATION            "position"
#define    CCMC_VARIABLE_NAME_PHI1_VALID_MIN                0.0
#define    CCMC_VARIABLE_NAME_PHI1_VALID_MAX                360.0
#define    CCMC_VARIABLE_NAME_PHI1_UNITS                    "degrees"
#define    CCMC_VARIABLE_NAME_PHI1_GRID_SYSTEM            "grid_system_2"
#define CCMC_VARIABLE_NAME_PHI1_MASK                    FLOAT_MASK
#define    CCMC_VARIABLE_NAME_PHI1_DESCRIPTION            "angle phi in spherical coordinates"
#define    CCMC_VARIABLE_NAME_PHI1_IS_VECTOR                0
#define    CCMC_VARIABLE_NAME_PHI1_POSITION_GRID_SYSTEM    "grid_system_2"
#define    CCMC_VARIABLE_NAME_PHI1_DATA_GRID_SYSTEM        "grid_system_2"

#define    CCMC_VARIABLE_NAME_PHI2                        "phi2"
#define    CCMC_VARIABLE_NAME_PHI2_DATA_TYPE                "float"
#define    CCMC_VARIABLE_NAME_PHI2_CLASSIFICATION            "position"
#define    CCMC_VARIABLE_NAME_PHI2_VALID_MIN                0.0
#define    CCMC_VARIABLE_NAME_PHI2_VALID_MAX                10000.0
#define    CCMC_VARIABLE_NAME_PHI2_UNITS                    "R"
#define    CCMC_VARIABLE_NAME_PHI2_GRID_SYSTEM            "grid_system_3"
#define    CCMC_VARIABLE_NAME_PHI2_MASK                    FLOAT_MASK
#define    CCMC_VARIABLE_NAME_PHI2_DESCRIPTION            "angle phi in spherical coordinates"
#define    CCMC_VARIABLE_NAME_PHI2_IS_VECTOR                0
#define    CCMC_VARIABLE_NAME_PHI2_POSITION_GRID_SYSTEM    "grid_system_3"
#define    CCMC_VARIABLE_NAME_PHI2_DATA_GRID_SYSTEM        "grid_system_3"

#define    CCMC_VARIABLE_NAME_PHI3                        "phi3"
#define    CCMC_VARIABLE_NAME_PHI3_DATA_TYPE                "float"
#define    CCMC_VARIABLE_NAME_PHI3_CLASSIFICATION            "position"
#define    CCMC_VARIABLE_NAME_PHI3_VALID_MIN                0.0
#define    CCMC_VARIABLE_NAME_PHI3_VALID_MAX                10000.0
#define    CCMC_VARIABLE_NAME_PHI3_UNITS                    "R"
#define    CCMC_VARIABLE_NAME_PHI3_GRID_SYSTEM            "grid_system_4"
#define    CCMC_VARIABLE_NAME_PHI3_MASK                    FLOAT_MASK
#define    CCMC_VARIABLE_NAME_PHI3_DESCRIPTION            "angle phi in spherical coordinates"
#define    CCMC_VARIABLE_NAME_PHI3_IS_VECTOR                0
#define    CCMC_VARIABLE_NAME_PHI3_POSITION_GRID_SYSTEM    "grid_system_4"
#define    CCMC_VARIABLE_NAME_PHI3_DATA_GRID_SYSTEM        "grid_system_4"

#define    CCMC_VARIABLE_NAME_PHI4                        "phi4"
#define    CCMC_VARIABLE_NAME_PHI4_DATA_TYPE                "float"
#define    CCMC_VARIABLE_NAME_PHI4_CLASSIFICATION            "position"
#define    CCMC_VARIABLE_NAME_PHI4_VALID_MIN                0.0
#define    CCMC_VARIABLE_NAME_PHI4_VALID_MAX                10000.0
#define    CCMC_VARIABLE_NAME_PHI4_UNITS                    "R"
#define    CCMC_VARIABLE_NAME_PHI4_GRID_SYSTEM            "grid_system_5"
#define    CCMC_VARIABLE_NAME_PHI4_MASK                    FLOAT_MASK
#define    CCMC_VARIABLE_NAME_PHI4_DESCRIPTION            "angle phi in spherical coordinates"
#define    CCMC_VARIABLE_NAME_PHI4_IS_VECTOR                0
#define    CCMC_VARIABLE_NAME_PHI4_POSITION_GRID_SYSTEM    "grid_system_5"
#define    CCMC_VARIABLE_NAME_PHI4_DATA_GRID_SYSTEM        "grid_system_5"

#define    CCMC_VARIABLE_NAME_PHI5                        "phi5"
#define    CCMC_VARIABLE_NAME_PHI5_DATA_TYPE                "float"
#define    CCMC_VARIABLE_NAME_PHI5_CLASSIFICATION            "position"
#define    CCMC_VARIABLE_NAME_PHI5_VALID_MIN                0.0
#define    CCMC_VARIABLE_NAME_PHI5_VALID_MAX                10000.0
#define    CCMC_VARIABLE_NAME_PHI5_UNITS                    "R"
#define    CCMC_VARIABLE_NAME_PHI5_GRID_SYSTEM            "grid_system_6"
#define    CCMC_VARIABLE_NAME_PHI5_MASK                    FLOAT_MASK
#define    CCMC_VARIABLE_NAME_PHI5_DESCRIPTION            "angle phi in spherical coordinates"
#define    CCMC_VARIABLE_NAME_PHI5_IS_VECTOR                0
#define    CCMC_VARIABLE_NAME_PHI5_POSITION_GRID_SYSTEM    "grid_system_6"
#define    CCMC_VARIABLE_NAME_PHI5_DATA_GRID_SYSTEM        "grid_system_6"

#define    CCMC_VARIABLE_NAME_THETA                        "theta"
#define    CCMC_VARIABLE_NAME_THETA_DATA_TYPE            "float"
#define    CCMC_VARIABLE_NAME_THETA_CLASSIFICATION        "position"
#define    CCMC_VARIABLE_NAME_THETA_VALID_MIN            0.0
#define    CCMC_VARIABLE_NAME_THETA_VALID_MAX            360.0
#define    CCMC_VARIABLE_NAME_THETA_UNITS                "degrees"
#define    CCMC_VARIABLE_NAME_THETA_GRID_SYSTEM            "grid_system_1"
#define CCMC_VARIABLE_NAME_THETA_MASK                    FLOAT_MASK
#define    CCMC_VARIABLE_NAME_THETA_DESCRIPTION            "angle theta in spherical coordinates"
#define    CCMC_VARIABLE_NAME_THETA_IS_VECTOR            0
#define    CCMC_VARIABLE_NAME_THETA_POSITION_GRID_SYSTEM    "grid_system_1"
#define    CCMC_VARIABLE_NAME_THETA_DATA_GRID_SYSTEM        "grid_system_1"

#define    CCMC_VARIABLE_NAME_THETA1                        "theta1"
#define    CCMC_VARIABLE_NAME_THETA1_DATA_TYPE            "float"
#define    CCMC_VARIABLE_NAME_THETA1_CLASSIFICATION        "position"
#define    CCMC_VARIABLE_NAME_THETA1_VALID_MIN            0.0
#define    CCMC_VARIABLE_NAME_THETA1_VALID_MAX            360.0
#define    CCMC_VARIABLE_NAME_THETA1_UNITS                "degrees"
#define    CCMC_VARIABLE_NAME_THETA1_GRID_SYSTEM            "grid_system_2"
#define CCMC_VARIABLE_NAME_THETA1_MASK                    FLOAT_MASK
#define    CCMC_VARIABLE_NAME_THETA1_DESCRIPTION            "angle theta in spherical coordinates"
#define    CCMC_VARIABLE_NAME_THETA1_IS_VECTOR            0
#define    CCMC_VARIABLE_NAME_THETA1_POSITION_GRID_SYSTEM    "grid_system_2"
#define    CCMC_VARIABLE_NAME_THETA1_DATA_GRID_SYSTEM        "grid_system_2"

#define    CCMC_VARIABLE_NAME_THETA2                        "theta2"
#define    CCMC_VARIABLE_NAME_THETA2_DATA_TYPE            "float"
#define    CCMC_VARIABLE_NAME_THETA2_CLASSIFICATION        "position"
#define    CCMC_VARIABLE_NAME_THETA2_VALID_MIN            0.0
#define    CCMC_VARIABLE_NAME_THETA2_VALID_MAX            360.0
#define    CCMC_VARIABLE_NAME_THETA2_UNITS                "degrees"
#define    CCMC_VARIABLE_NAME_THETA2_GRID_SYSTEM            "grid_system_3"
#define CCMC_VARIABLE_NAME_THETA2_MASK                    FLOAT_MASK
#define    CCMC_VARIABLE_NAME_THETA2_DESCRIPTION            "angle theta in spherical coordinates"
#define    CCMC_VARIABLE_NAME_THETA2_IS_VECTOR            0
#define    CCMC_VARIABLE_NAME_THETA2_POSITION_GRID_SYSTEM    "grid_system_3"
#define    CCMC_VARIABLE_NAME_THETA2_DATA_GRID_SYSTEM        "grid_system_3"

#define    CCMC_VARIABLE_NAME_THETA3                        "theta3"
#define    CCMC_VARIABLE_NAME_THETA3_DATA_TYPE            "float"
#define    CCMC_VARIABLE_NAME_THETA3_CLASSIFICATION        "position"
#define    CCMC_VARIABLE_NAME_THETA3_VALID_MIN            0.0
#define    CCMC_VARIABLE_NAME_THETA3_VALID_MAX            360.0
#define    CCMC_VARIABLE_NAME_THETA3_UNITS                "degrees"
#define    CCMC_VARIABLE_NAME_THETA3_GRID_SYSTEM            "grid_system_4"
#define CCMC_VARIABLE_NAME_THETA3_MASK                    FLOAT_MASK
#define    CCMC_VARIABLE_NAME_THETA3_DESCRIPTION            "angle theta in spherical coordinates"
#define    CCMC_VARIABLE_NAME_THETA3_IS_VECTOR            0
#define    CCMC_VARIABLE_NAME_THETA3_POSITION_GRID_SYSTEM    "grid_system_4"
#define    CCMC_VARIABLE_NAME_THETA3_DATA_GRID_SYSTEM        "grid_system_4"

#define    CCMC_VARIABLE_NAME_THETA4                        "theta4"
#define    CCMC_VARIABLE_NAME_THETA4_DATA_TYPE            "float"
#define    CCMC_VARIABLE_NAME_THETA4_CLASSIFICATION        "position"
#define    CCMC_VARIABLE_NAME_THETA4_VALID_MIN            0.0
#define    CCMC_VARIABLE_NAME_THETA4_VALID_MAX            360.0
#define    CCMC_VARIABLE_NAME_THETA4_UNITS                "degrees"
#define    CCMC_VARIABLE_NAME_THETA4_GRID_SYSTEM            "grid_system_5"
#define CCMC_VARIABLE_NAME_THETA4_MASK                    FLOAT_MASK
#define    CCMC_VARIABLE_NAME_THETA4_DESCRIPTION            "angle theta in spherical coordinates"
#define    CCMC_VARIABLE_NAME_THETA4_IS_VECTOR            0
#define    CCMC_VARIABLE_NAME_THETA4_POSITION_GRID_SYSTEM    "grid_system_5"
#define    CCMC_VARIABLE_NAME_THETA4_DATA_GRID_SYSTEM        "grid_system_5"

#define    CCMC_VARIABLE_NAME_THETA5                        "theta5"
#define    CCMC_VARIABLE_NAME_THETA5_DATA_TYPE            "float"
#define    CCMC_VARIABLE_NAME_THETA5_CLASSIFICATION        "position"
#define    CCMC_VARIABLE_NAME_THETA5_VALID_MIN            0.0
#define    CCMC_VARIABLE_NAME_THETA5_VALID_MAX            360.0
#define    CCMC_VARIABLE_NAME_THETA5_UNITS                "degrees"
#define    CCMC_VARIABLE_NAME_THETA5_GRID_SYSTEM            "grid_system_6"
#define CCMC_VARIABLE_NAME_THETA5_MASK                    FLOAT_MASK
#define    CCMC_VARIABLE_NAME_THETA5_DESCRIPTION            "angle theta in spherical coordinates"
#define    CCMC_VARIABLE_NAME_THETA5_IS_VECTOR            0
#define    CCMC_VARIABLE_NAME_THETA5_POSITION_GRID_SYSTEM    "grid_system_6"
#define    CCMC_VARIABLE_NAME_THETA5_DATA_GRID_SYSTEM        "grid_system_6"

/*******************************************************************************************************/
/**                                        cartesian                                                **/
/*******************************************************************************************************/

#define    CCMC_VARIABLE_NAME_X                            "x"
#define    CCMC_VARIABLE_NAME_X_DATA_TYPE                "float"
#define    CCMC_VARIABLE_NAME_X_CLASSIFICATION            "position"
#define    CCMC_VARIABLE_NAME_X_VALID_MIN                -10000.0
#define    CCMC_VARIABLE_NAME_X_VALID_MAX                10000.0
#define    CCMC_VARIABLE_NAME_X_UNITS                    "R"
#define    CCMC_VARIABLE_NAME_X_GRID_SYSTEM                "grid_system_1"
#define CCMC_VARIABLE_NAME_X_MASK                        FLOAT_MASK
#define    CCMC_VARIABLE_NAME_X_DESCRIPTION                "X Coordinate, R = R_E or R_S or AU, 1 R_E = 6371 km ( Earth ), 1 R_S = 696000 km ( Sun ), 1 AU = 1.5e8 km ( Astron - Unit for Heliosphere )"
#define    CCMC_VARIABLE_NAME_X_IS_VECTOR                0
#define    CCMC_VARIABLE_NAME_X_POSITION_GRID_SYSTEM        "grid_system_1"
#define    CCMC_VARIABLE_NAME_X_DATA_GRID_SYSTEM            "grid_system_1"

#define    CCMC_VARIABLE_NAME_X1                            "x1"
#define    CCMC_VARIABLE_NAME_X1_DATA_TYPE                "float"
#define    CCMC_VARIABLE_NAME_X1_CLASSIFICATION            "position"
#define    CCMC_VARIABLE_NAME_X1_VALID_MIN                -10000.0
#define    CCMC_VARIABLE_NAME_X1_VALID_MAX                10000.0
#define    CCMC_VARIABLE_NAME_X1_UNITS                    "R"
#define    CCMC_VARIABLE_NAME_X1_GRID_SYSTEM                "grid_system_2"
#define    CCMC_VARIABLE_NAME_X1_MASK                    FLOAT_MASK
#define    CCMC_VARIABLE_NAME_X1_DESCRIPTION                "X Coordinate, R = R_E or R_S or AU, 1 R_E = 6371 km ( Earth ), 1 R_S = 696000 km ( Sun ), 1 AU = 1.5e8 km ( Astron - Unit for Heliosphere )"

#define    CCMC_VARIABLE_NAME_X1_IS_VECTOR                0
#define    CCMC_VARIABLE_NAME_X1_POSITION_GRID_SYSTEM        "grid_system_2"
#define    CCMC_VARIABLE_NAME_X1_DATA_GRID_SYSTEM            "grid_system_2"

#define    CCMC_VARIABLE_NAME_X2                            "x2"
#define    CCMC_VARIABLE_NAME_X2_DATA_TYPE                "float"
#define    CCMC_VARIABLE_NAME_X2_CLASSIFICATION            "position"
#define    CCMC_VARIABLE_NAME_X2_VALID_MIN                -10000.0
#define    CCMC_VARIABLE_NAME_X2_VALID_MAX                10000.0
#define    CCMC_VARIABLE_NAME_X2_UNITS                    "R"
#define    CCMC_VARIABLE_NAME_X2_GRID_SYSTEM                "grid_system_3"
#define    CCMC_VARIABLE_NAME_X2_MASK                    FLOAT_MASK
#define    CCMC_VARIABLE_NAME_X2_DESCRIPTION                "X Coordinate, R = R_E or R_S or AU, 1 R_E = 6371 km ( Earth ), 1 R_S = 696000 km ( Sun ), 1 AU = 1.5e8 km ( Astron - Unit for Heliosphere )"
#define    CCMC_VARIABLE_NAME_X2_IS_VECTOR                0
#define    CCMC_VARIABLE_NAME_X2_POSITION_GRID_SYSTEM        "grid_system_3"
#define    CCMC_VARIABLE_NAME_X2_DATA_GRID_SYSTEM            "grid_system_3"

#define    CCMC_VARIABLE_NAME_X3                            "x3"
#define    CCMC_VARIABLE_NAME_X3_DATA_TYPE                "float"
#define    CCMC_VARIABLE_NAME_X3_CLASSIFICATION            "position"
#define    CCMC_VARIABLE_NAME_X3_VALID_MIN                -10000.0
#define    CCMC_VARIABLE_NAME_X3_VALID_MAX                10000.0
#define    CCMC_VARIABLE_NAME_X3_UNITS                    "R"
#define    CCMC_VARIABLE_NAME_X3_GRID_SYSTEM                "grid_system_4"
#define    CCMC_VARIABLE_NAME_X3_MASK                    FLOAT_MASK
#define    CCMC_VARIABLE_NAME_X3_DESCRIPTION                "X Coordinate, R = R_E or R_S or AU, 1 R_E = 6371 km ( Earth ), 1 R_S = 696000 km ( Sun ), 1 AU = 1.5e8 km ( Astron - Unit for Heliosphere )"
#define    CCMC_VARIABLE_NAME_X3_IS_VECTOR                0
#define    CCMC_VARIABLE_NAME_X3_POSITION_GRID_SYSTEM        "grid_system_4"
#define    CCMC_VARIABLE_NAME_X3_DATA_GRID_SYSTEM            "grid_system_4"

#define    CCMC_VARIABLE_NAME_X4                            "x4"
#define    CCMC_VARIABLE_NAME_X4_DATA_TYPE                "float"
#define    CCMC_VARIABLE_NAME_X4_CLASSIFICATION            "position"
#define    CCMC_VARIABLE_NAME_X4_VALID_MIN                -10000.0
#define    CCMC_VARIABLE_NAME_X4_VALID_MAX                10000.0
#define    CCMC_VARIABLE_NAME_X4_UNITS                    "R"
#define    CCMC_VARIABLE_NAME_X4_GRID_SYSTEM                "grid_system_5"
#define    CCMC_VARIABLE_NAME_X4_MASK                    FLOAT_MASK
#define    CCMC_VARIABLE_NAME_X4_DESCRIPTION                "X Coordinate, R = R_E or R_S or AU, 1 R_E = 6371 km ( Earth ), 1 R_S = 696000 km ( Sun ), 1 AU = 1.5e8 km ( Astron - Unit for Heliosphere )"
#define    CCMC_VARIABLE_NAME_X4_IS_VECTOR                0
#define    CCMC_VARIABLE_NAME_X4_POSITION_GRID_SYSTEM        "grid_system_5"
#define    CCMC_VARIABLE_NAME_X4_DATA_GRID_SYSTEM            "grid_system_5"

#define    CCMC_VARIABLE_NAME_X5                            "x5"
#define    CCMC_VARIABLE_NAME_X5_DATA_TYPE                "float"
#define    CCMC_VARIABLE_NAME_X5_CLASSIFICATION            "position"
#define    CCMC_VARIABLE_NAME_X5_VALID_MIN                -10000.0
#define    CCMC_VARIABLE_NAME_X5_VALID_MAX                10000.0
#define    CCMC_VARIABLE_NAME_X5_UNITS                    "R"
#define    CCMC_VARIABLE_NAME_X5_GRID_SYSTEM                "grid_system_6"
#define    CCMC_VARIABLE_NAME_X5_MASK                    FLOAT_MASK
#define    CCMC_VARIABLE_NAME_X5_DESCRIPTION                "X Coordinate, R = R_E or R_S or AU, 1 R_E = 6371 km ( Earth ), 1 R_S = 696000 km ( Sun ), 1 AU = 1.5e8 km ( Astron - Unit for Heliosphere )"
#define    CCMC_VARIABLE_NAME_X5_IS_VECTOR                0
#define    CCMC_VARIABLE_NAME_X5_POSITION_GRID_SYSTEM        "grid_system_6"
#define    CCMC_VARIABLE_NAME_X5_DATA_GRID_SYSTEM            "grid_system_6"

#define CCMC_VARIABLE_NAME_Y                            "y"
#define CCMC_VARIABLE_NAME_Y_DATA_TYPE                    "float"
#define    CCMC_VARIABLE_NAME_Y_CLASSIFICATION            "position"
#define    CCMC_VARIABLE_NAME_Y_VALID_MIN                -10000.0
#define    CCMC_VARIABLE_NAME_Y_VALID_MAX                10000.0
#define    CCMC_VARIABLE_NAME_Y_UNITS                    "R"
#define    CCMC_VARIABLE_NAME_Y_GRID_SYSTEM                "grid_system_1"
#define    CCMC_VARIABLE_NAME_Y_MASK                        FLOAT_MASK
#define    CCMC_VARIABLE_NAME_Y_DESCRIPTION                "Y Coordinate, R = R_E or R_S or AU, 1 R_E = 6371 km ( Earth ), 1 R_S = 696000 km ( Sun ), 1 AU = 1.5e8 km ( Astron - Unit for Heliosphere )"
#define    CCMC_VARIABLE_NAME_Y_IS_VECTOR                0
#define    CCMC_VARIABLE_NAME_Y_POSITION_GRID_SYSTEM        "grid_system_1"
#define    CCMC_VARIABLE_NAME_Y_DATA_GRID_SYSTEM            "grid_system_1"

#define    CCMC_VARIABLE_NAME_Y1                            "y1"
#define    CCMC_VARIABLE_NAME_Y1_DATA_TYPE                "float"
#define    CCMC_VARIABLE_NAME_Y1_CLASSIFICATION            "position"
#define    CCMC_VARIABLE_NAME_Y1_VALID_MIN                -10000.0
#define    CCMC_VARIABLE_NAME_Y1_VALID_MAX                10000.0
#define    CCMC_VARIABLE_NAME_Y1_UNITS                    "R"
#define    CCMC_VARIABLE_NAME_Y1_GRID_SYSTEM                "grid_system_2"
#define    CCMC_VARIABLE_NAME_Y1_MASK                    FLOAT_MASK
#define    CCMC_VARIABLE_NAME_Y1_DESCRIPTION                "Y Coordiante, R = R_E or R_S or AU, 1 R_E = 6371 km ( Earth ), 1 R_S = 696000 km ( Sun ), 1 AU = 1.5e8 km ( Astron - Unit for Heliosphere )"
#define    CCMC_VARIABLE_NAME_Y1_IS_VECTOR                0
#define    CCMC_VARIABLE_NAME_Y1_POSITION_GRID_SYSTEM        "grid_system_2"
#define    CCMC_VARIABLE_NAME_Y1_DATA_GRID_SYSTEM            "grid_system_2"

#define    CCMC_VARIABLE_NAME_Y2                            "y2"
#define    CCMC_VARIABLE_NAME_Y2_DATA_TYPE                "float"
#define    CCMC_VARIABLE_NAME_Y2_CLASSIFICATION            "position"
#define    CCMC_VARIABLE_NAME_Y2_VALID_MIN                -10000.0
#define    CCMC_VARIABLE_NAME_Y2_VALID_MAX                10000.0
#define    CCMC_VARIABLE_NAME_Y2_UNITS                    "R"
#define    CCMC_VARIABLE_NAME_Y2_GRID_SYSTEM                "grid_system_3"
#define    CCMC_VARIABLE_NAME_Y2_MASK                    FLOAT_MASK
#define    CCMC_VARIABLE_NAME_Y2_DESCRIPTION                "Y Coordiante, R = R_E or R_S or AU, 1 R_E = 6371 km ( Earth ), 1 R_S = 696000 km ( Sun ), 1 AU = 1.5e8 km ( Astron - Unit for Heliosphere )"
#define    CCMC_VARIABLE_NAME_Y2_IS_VECTOR                0
#define    CCMC_VARIABLE_NAME_Y2_POSITION_GRID_SYSTEM        "grid_system_3"
#define    CCMC_VARIABLE_NAME_Y2_DATA_GRID_SYSTEM            "grid_system_3"

#define    CCMC_VARIABLE_NAME_Y3                            "y3"
#define    CCMC_VARIABLE_NAME_Y3_DATA_TYPE                "float"
#define    CCMC_VARIABLE_NAME_Y3_CLASSIFICATION            "position"
#define    CCMC_VARIABLE_NAME_Y3_VALID_MIN                -10000.0
#define    CCMC_VARIABLE_NAME_Y3_VALID_MAX                10000.0
#define    CCMC_VARIABLE_NAME_Y3_UNITS                    "R"
#define    CCMC_VARIABLE_NAME_Y3_GRID_SYSTEM                "grid_system_4"
#define    CCMC_VARIABLE_NAME_Y3_MASK                    FLOAT_MASK
#define    CCMC_VARIABLE_NAME_Y3_DESCRIPTION                "Y Coordiante, R = R_E or R_S or AU, 1 R_E = 6371 km ( Earth ), 1 R_S = 696000 km ( Sun ), 1 AU = 1.5e8 km ( Astron - Unit for Heliosphere )"
#define    CCMC_VARIABLE_NAME_Y3_IS_VECTOR                0
#define    CCMC_VARIABLE_NAME_Y3_POSITION_GRID_SYSTEM        "grid_system_4"
#define    CCMC_VARIABLE_NAME_Y3_DATA_GRID_SYSTEM            "grid_system_4"

#define    CCMC_VARIABLE_NAME_Y4                            "y4"
#define    CCMC_VARIABLE_NAME_Y4_DATA_TYPE                "float"
#define    CCMC_VARIABLE_NAME_Y4_CLASSIFICATION            "position"
#define    CCMC_VARIABLE_NAME_Y4_VALID_MIN                -10000.0
#define    CCMC_VARIABLE_NAME_Y4_VALID_MAX                10000.0
#define    CCMC_VARIABLE_NAME_Y4_UNITS                    "R"
#define    CCMC_VARIABLE_NAME_Y4_GRID_SYSTEM                "grid_system_5"
#define    CCMC_VARIABLE_NAME_Y4_MASK                    FLOAT_MASK
#define    CCMC_VARIABLE_NAME_Y4_DESCRIPTION                "Y Coordiante, R = R_E or R_S or AU, 1 R_E = 6371 km ( Earth ), 1 R_S = 696000 km ( Sun ), 1 AU = 1.5e8 km ( Astron - Unit for Heliosphere )"
#define    CCMC_VARIABLE_NAME_Y4_IS_VECTOR                0
#define    CCMC_VARIABLE_NAME_Y4_POSITION_GRID_SYSTEM        "grid_system_5"
#define    CCMC_VARIABLE_NAME_Y4_DATA_GRID_SYSTEM            "grid_system_5"

#define    CCMC_VARIABLE_NAME_Y5                            "y5"
#define    CCMC_VARIABLE_NAME_Y5_DATA_TYPE                "float"
#define    CCMC_VARIABLE_NAME_Y5_CLASSIFICATION            "position"
#define    CCMC_VARIABLE_NAME_Y5_VALID_MIN                -10000.0
#define    CCMC_VARIABLE_NAME_Y5_VALID_MAX                10000.0
#define    CCMC_VARIABLE_NAME_Y5_UNITS                    "R"
#define    CCMC_VARIABLE_NAME_Y5_GRID_SYSTEM                "grid_system_6"
#define    CCMC_VARIABLE_NAME_Y5_MASK                    FLOAT_MASK
#define    CCMC_VARIABLE_NAME_Y5_DESCRIPTION                "Y Coordiante, R = R_E or R_S or AU, 1 R_E = 6371 km ( Earth ), 1 R_S = 696000 km ( Sun ), 1 AU = 1.5e8 km ( Astron - Unit for Heliosphere )"
#define    CCMC_VARIABLE_NAME_Y5_IS_VECTOR                0
#define    CCMC_VARIABLE_NAME_Y5_POSITION_GRID_SYSTEM        "grid_system_6"
#define    CCMC_VARIABLE_NAME_Y5_DATA_GRID_SYSTEM            "grid_system_6"

#define CCMC_VARIABLE_NAME_Z                            "z"
#define CCMC_VARIABLE_NAME_Z_DATA_TYPE                    "float"
#define    CCMC_VARIABLE_NAME_Z_CLASSIFICATION            "position"
#define    CCMC_VARIABLE_NAME_Z_VALID_MIN                -10000.0
#define    CCMC_VARIABLE_NAME_Z_VALID_MAX                10000.0
#define    CCMC_VARIABLE_NAME_Z_UNITS                    "R"
#define    CCMC_VARIABLE_NAME_Z_GRID_SYSTEM                "grid_system_1"
#define    CCMC_VARIABLE_NAME_Z_MASK                        FLOAT_MASK
#define    CCMC_VARIABLE_NAME_Z_DESCRIPTION                "Z Coordinate, R = R_E or R_S or AU, 1 R_E = 6371 km ( Earth ), 1 R_S = 696000 km ( Sun ), 1 AU = 1.5e8 km ( Astron - Unit for Heliosphere )"
#define    CCMC_VARIABLE_NAME_Z_IS_VECTOR                0
#define    CCMC_VARIABLE_NAME_Z_POSITION_GRID_SYSTEM        "grid_system_1"
#define    CCMC_VARIABLE_NAME_Z_DATA_GRID_SYSTEM            "grid_system_1"

#define    CCMC_VARIABLE_NAME_Z1                            "z1"
#define    CCMC_VARIABLE_NAME_Z1_DATA_TYPE                "float"
#define    CCMC_VARIABLE_NAME_Z1_CLASSIFICATION            "position"
#define    CCMC_VARIABLE_NAME_Z1_VALID_MIN                -10000.0
#define    CCMC_VARIABLE_NAME_Z1_VALID_MAX                10000.0
#define    CCMC_VARIABLE_NAME_Z1_UNITS                    "R"
#define    CCMC_VARIABLE_NAME_Z1_GRID_SYSTEM                "grid_system_2"
#define    CCMC_VARIABLE_NAME_Z1_MASK                    FLOAT_MASK
#define    CCMC_VARIABLE_NAME_Z1_DESCRIPTION                "Z Coordinate, R = R_E or R_S or AU, 1 R_E = 6371 km ( Earth ), 1 R_S = 696000 km ( Sun ), 1 AU = 1.5e8 km ( Astron - Unit for Heliosphere )"
#define    CCMC_VARIABLE_NAME_Z1_IS_VECTOR                0
#define    CCMC_VARIABLE_NAME_Z1_POSITION_GRID_SYSTEM        "grid_system_2"
#define    CCMC_VARIABLE_NAME_Z1_DATA_GRID_SYSTEM            "grid_system_2"

#define    CCMC_VARIABLE_NAME_Z2                            "z2"
#define    CCMC_VARIABLE_NAME_Z2_DATA_TYPE                "float"
#define    CCMC_VARIABLE_NAME_Z2_CLASSIFICATION            "position"
#define    CCMC_VARIABLE_NAME_Z2_VALID_MIN                -10000.0
#define    CCMC_VARIABLE_NAME_Z2_VALID_MAX                10000.0
#define    CCMC_VARIABLE_NAME_Z2_UNITS                    "R"
#define    CCMC_VARIABLE_NAME_Z2_GRID_SYSTEM                "grid_system_3"
#define    CCMC_VARIABLE_NAME_Z2_MASK                    FLOAT_MASK
#define    CCMC_VARIABLE_NAME_Z2_DESCRIPTION                "Z Coordinate, R = R_E or R_S or AU, 1 R_E = 6371 km ( Earth ), 1 R_S = 696000 km ( Sun ), 1 AU = 1.5e8 km ( Astron - Unit for Heliosphere )"
#define    CCMC_VARIABLE_NAME_Z2_IS_VECTOR                0
#define    CCMC_VARIABLE_NAME_Z2_POSITION_GRID_SYSTEM        "grid_system_3"
#define    CCMC_VARIABLE_NAME_Z2_DATA_GRID_SYSTEM            "grid_system_3"

#define    CCMC_VARIABLE_NAME_Z3                            "z3"
#define    CCMC_VARIABLE_NAME_Z3_DATA_TYPE                "float"
#define    CCMC_VARIABLE_NAME_Z3_CLASSIFICATION            "position"
#define    CCMC_VARIABLE_NAME_Z3_VALID_MIN                -10000.0
#define    CCMC_VARIABLE_NAME_Z3_VALID_MAX                10000.0
#define    CCMC_VARIABLE_NAME_Z3_UNITS                    "R"
#define    CCMC_VARIABLE_NAME_Z3_GRID_SYSTEM                "grid_system_4"
#define    CCMC_VARIABLE_NAME_Z3_MASK                    FLOAT_MASK
#define    CCMC_VARIABLE_NAME_Z3_DESCRIPTION                "Z Coordinate, R = R_E or R_S or AU, 1 R_E = 6371 km ( Earth ), 1 R_S = 696000 km ( Sun ), 1 AU = 1.5e8 km ( Astron - Unit for Heliosphere )"
#define    CCMC_VARIABLE_NAME_Z3_IS_VECTOR                0
#define    CCMC_VARIABLE_NAME_Z3_POSITION_GRID_SYSTEM        "grid_system_4"
#define    CCMC_VARIABLE_NAME_Z3_DATA_GRID_SYSTEM            "grid_system_4"

#define    CCMC_VARIABLE_NAME_Z4                            "z4"
#define    CCMC_VARIABLE_NAME_Z4_DATA_TYPE                "float"
#define    CCMC_VARIABLE_NAME_Z4_CLASSIFICATION            "position"
#define    CCMC_VARIABLE_NAME_Z4_VALID_MIN                -10000.0
#define    CCMC_VARIABLE_NAME_Z4_VALID_MAX                10000.0
#define    CCMC_VARIABLE_NAME_Z4_UNITS                    "R"
#define    CCMC_VARIABLE_NAME_Z4_GRID_SYSTEM                "grid_system_5"
#define    CCMC_VARIABLE_NAME_Z4_MASK                    FLOAT_MASK
#define    CCMC_VARIABLE_NAME_Z4_DESCRIPTION                "Z Coordinate, R = R_E or R_S or AU, 1 R_E = 6371 km ( Earth ), 1 R_S = 696000 km ( Sun ), 1 AU = 1.5e8 km ( Astron - Unit for Heliosphere )"
#define    CCMC_VARIABLE_NAME_Z4_IS_VECTOR                0
#define    CCMC_VARIABLE_NAME_Z4_POSITION_GRID_SYSTEM        "grid_system_5"
#define    CCMC_VARIABLE_NAME_Z4_DATA_GRID_SYSTEM            "grid_system_5"

#define    CCMC_VARIABLE_NAME_Z5                            "z5"
#define    CCMC_VARIABLE_NAME_Z5_DATA_TYPE                "float"
#define    CCMC_VARIABLE_NAME_Z5_CLASSIFICATION            "position"
#define    CCMC_VARIABLE_NAME_Z5_VALID_MIN                -10000.0
#define    CCMC_VARIABLE_NAME_Z5_VALID_MAX                10000.0
#define    CCMC_VARIABLE_NAME_Z5_UNITS                    "R"
#define    CCMC_VARIABLE_NAME_Z5_GRID_SYSTEM                "grid_system_6"
#define    CCMC_VARIABLE_NAME_Z5_MASK                    FLOAT_MASK
#define    CCMC_VARIABLE_NAME_Z5_DESCRIPTION                "Z Coordinate, R = R_E or R_S or AU, 1 R_E = 6371 km ( Earth ), 1 R_S = 696000 km ( Sun ), 1 AU = 1.5e8 km ( Astron - Unit for Heliosphere )"
#define    CCMC_VARIABLE_NAME_Z5_IS_VECTOR                0
#define    CCMC_VARIABLE_NAME_Z5_POSITION_GRID_SYSTEM        "grid_system_6"
#define    CCMC_VARIABLE_NAME_Z5_DATA_GRID_SYSTEM            "grid_system_6"

/*******************************************************************************************************/
/**                                        special    coordinate variables                                **/
/*******************************************************************************************************/

#define CCMC_VARIABLE_NAME_H                            "H"
#define CCMC_VARIABLE_NAME_H_DATA_TYPE                    "float"
#define    CCMC_VARIABLE_NAME_H_CLASSIFICATION            "position"
#define    CCMC_VARIABLE_NAME_H_VALID_MIN                0.0
#define    CCMC_VARIABLE_NAME_H_VALID_MAX                1000000.0
#define    CCMC_VARIABLE_NAME_H_UNITS                    "km"
#define    CCMC_VARIABLE_NAME_H_GRID_SYSTEM                "grid_system_1"
#define    CCMC_VARIABLE_NAME_H_MASK                        FLOAT_MASK
#define    CCMC_VARIABLE_NAME_H_DESCRIPTION                "Height above ground."
#define    CCMC_VARIABLE_NAME_H_IS_VECTOR                0
#define    CCMC_VARIABLE_NAME_H_POSITION_GRID_SYSTEM        "grid_system_1"
#define    CCMC_VARIABLE_NAME_H_DATA_GRID_SYSTEM            "grid_system_1"

#define    CCMC_VARIABLE_NAME_X_BX                        "x_bx"
#define    CCMC_VARIABLE_NAME_X_BX_DATA_TYPE                "float"
#define    CCMC_VARIABLE_NAME_X_BX_CLASSIFICATION            "position"
#define    CCMC_VARIABLE_NAME_X_BX_VALID_MIN                -10000.0
#define    CCMC_VARIABLE_NAME_X_BX_VALID_MAX                10000.0
#define    CCMC_VARIABLE_NAME_X_BX_UNITS                    "R"
#define    CCMC_VARIABLE_NAME_X_BX_GRID_SYSTEM            "grid_system_2"
#define    CCMC_VARIABLE_NAME_X_BX_GRID_TYPE                "cartesian"
#define    CCMC_VARIABLE_NAME_X_BX_MASK                    FLOAT_MASK
#define    CCMC_VARIABLE_NAME_X_BX_DESCRIPTION            "X Coordinate for BX1.  BX1 is defined on a staggered grid different from X,Y,Z for the other variables"
#define    CCMC_VARIABLE_NAME_X_BX_IS_VECTOR                0
#define    CCMC_VARIABLE_NAME_X_BX_POSITION_GRID_SYSTEM    "grid_system_2"
#define    CCMC_VARIABLE_NAME_X_BX_DATA_GRID_SYSTEM        "grid_system_2"

#define    CCMC_VARIABLE_NAME_X_BY                        "x_by"
#define    CCMC_VARIABLE_NAME_X_BY_DATA_TYPE                "float"
#define    CCMC_VARIABLE_NAME_X_BY_CLASSIFICATION            "position"
#define    CCMC_VARIABLE_NAME_X_BY_VALID_MIN                -10000.0
#define    CCMC_VARIABLE_NAME_X_BY_VALID_MAX                10000.0
#define    CCMC_VARIABLE_NAME_X_BY_UNITS                    "R"
#define    CCMC_VARIABLE_NAME_X_BY_GRID_SYSTEM            "grid_system_3"
#define    CCMC_VARIABLE_NAME_X_BY_MASK                    FLOAT_MASK
#define    CCMC_VARIABLE_NAME_X_BY_DESCRIPTION            "X Coordinate for BY1.  BY1 is defined on a staggered grid different from X,Y,Z for the other variables"
#define    CCMC_VARIABLE_NAME_X_BY_IS_VECTOR                0
#define    CCMC_VARIABLE_NAME_X_BY_POSITION_GRID_SYSTEM    "grid_system_3"
#define    CCMC_VARIABLE_NAME_X_BY_DATA_GRID_SYSTEM        "grid_system_3"

#define    CCMC_VARIABLE_NAME_X_BZ                        "x_bz"
#define    CCMC_VARIABLE_NAME_X_BZ_DATA_TYPE                "float"
#define    CCMC_VARIABLE_NAME_X_BZ_CLASSIFICATION            "position"
#define    CCMC_VARIABLE_NAME_X_BZ_VALID_MIN                -10000.0
#define    CCMC_VARIABLE_NAME_X_BZ_VALID_MAX                10000.0
#define    CCMC_VARIABLE_NAME_X_BZ_UNITS                    "R"
#define    CCMC_VARIABLE_NAME_X_BZ_GRID_SYSTEM            "grid_system_4"
#define    CCMC_VARIABLE_NAME_X_BZ_MASK                    FLOAT_MASK
#define    CCMC_VARIABLE_NAME_X_BZ_DESCRIPTION            "X Coordinate for BZ1.  BZ1 is defined on a staggered grid different from X,Y,Z for the other variables"
#define    CCMC_VARIABLE_NAME_X_BZ_IS_VECTOR                0
#define    CCMC_VARIABLE_NAME_X_BZ_POSITION_GRID_SYSTEM    "grid_system_4"
#define    CCMC_VARIABLE_NAME_X_BZ_DATA_GRID_SYSTEM        "grid_system_4"

#define CCMC_VARIABLE_NAME_Y_BX                        "y_bx"
#define CCMC_VARIABLE_NAME_Y_BX_DATA_TYPE                "float"
#define CCMC_VARIABLE_NAME_Y_BX_CLASSIFICATION            "position"
#define CCMC_VARIABLE_NAME_Y_BX_VALID_MIN                -10000.0
#define CCMC_VARIABLE_NAME_Y_BX_VALID_MAX                10000.0
#define CCMC_VARIABLE_NAME_Y_BX_UNITS                    "R"
#define CCMC_VARIABLE_NAME_Y_BX_GRID_SYSTEM            "grid_system_2"
#define CCMC_VARIABLE_NAME_Y_BX_MASK                    FLOAT_MASK
#define CCMC_VARIABLE_NAME_Y_BX_DESCRIPTION            "Y Coordinate for BX1.  BX1 is defined on a staggered grid different from X,Y,Z for the other variables"
#define CCMC_VARIABLE_NAME_Y_BX_IS_VECTOR                0
#define CCMC_VARIABLE_NAME_Y_BX_POSITION_GRID_SYSTEM    "grid_system_2"
#define CCMC_VARIABLE_NAME_Y_BX_DATA_GRID_SYSTEM        "grid_system_2"

#define CCMC_VARIABLE_NAME_Y_BY                        "y_by"
#define CCMC_VARIABLE_NAME_Y_BY_DATA_TYPE                "float"
#define CCMC_VARIABLE_NAME_Y_BY_CLASSIFICATION            "position"
#define CCMC_VARIABLE_NAME_Y_BY_VALID_MIN                -10000.0
#define CCMC_VARIABLE_NAME_Y_BY_VALID_MAX                10000.0
#define CCMC_VARIABLE_NAME_Y_BY_UNITS                    "R"
#define CCMC_VARIABLE_NAME_Y_BY_GRID_SYSTEM            "grid_system_3"
#define CCMC_VARIABLE_NAME_Y_BY_MASK                    FLOAT_MASK
#define CCMC_VARIABLE_NAME_Y_BY_DESCRIPTION            "Y Coordinate for BY1.  BY1 is defined on a staggered grid different from X,Y,Z for the other variables"
#define CCMC_VARIABLE_NAME_Y_BY_IS_VECTOR                0
#define CCMC_VARIABLE_NAME_Y_BY_POSITION_GRID_SYSTEM    "grid_system_3"
#define CCMC_VARIABLE_NAME_Y_BY_DATA_GRID_SYSTEM        "grid_system_3"

#define CCMC_VARIABLE_NAME_Y_BZ                        "y_bz"
#define CCMC_VARIABLE_NAME_Y_BZ_DATA_TYPE                "float"
#define CCMC_VARIABLE_NAME_Y_BZ_CLASSIFICATION            "position"
#define CCMC_VARIABLE_NAME_Y_BZ_VALID_MIN                -10000.0
#define CCMC_VARIABLE_NAME_Y_BZ_VALID_MAX                10000.0
#define CCMC_VARIABLE_NAME_Y_BZ_UNITS                    "R"
#define CCMC_VARIABLE_NAME_Y_BZ_GRID_SYSTEM            "grid_system_4"
#define CCMC_VARIABLE_NAME_Y_BZ_MASK                    FLOAT_MASK
#define CCMC_VARIABLE_NAME_Y_BZ_DESCRIPTION            "Y Coordinate for BZ1.  BZ1 is defined on a staggered grid different from X,Y,Z for the other variables"
#define CCMC_VARIABLE_NAME_Y_BZ_IS_VECTOR                0
#define CCMC_VARIABLE_NAME_Y_BZ_POSITION_GRID_SYSTEM    "grid_system_4"
#define CCMC_VARIABLE_NAME_Y_BZ_DATA_GRID_SYSTEM        "grid_system_4"

#define CCMC_VARIABLE_NAME_Z_BX                        "z_bx"
#define CCMC_VARIABLE_NAME_Z_BX_DATA_TYPE                "float"
#define CCMC_VARIABLE_NAME_Z_BX_CLASSIFICATION            "position"
#define CCMC_VARIABLE_NAME_Z_BX_VALID_MIN                -10000.0
#define CCMC_VARIABLE_NAME_Z_BX_VALID_MAX                10000.0
#define CCMC_VARIABLE_NAME_Z_BX_UNITS                    "R"
#define CCMC_VARIABLE_NAME_Z_BX_GRID_SYSTEM            "grid_system_2"
#define CCMC_VARIABLE_NAME_Z_BX_MASK                    FLOAT_MASK
#define CCMC_VARIABLE_NAME_Z_BX_DESCRIPTION            "Z Coordinate for BX1.  BX1 is defined on a staggered grid different from X,Y,Z for the other variables"
#define CCMC_VARIABLE_NAME_Z_BX_IS_VECTOR                0
#define CCMC_VARIABLE_NAME_Z_BX_POSITION_GRID_SYSTEM    "grid_system_2"
#define CCMC_VARIABLE_NAME_Z_BX_DATA_GRID_SYSTEM        "grid_system_2"

#define CCMC_VARIABLE_NAME_Z_BY                        "z_by"
#define CCMC_VARIABLE_NAME_Z_BY_DATA_TYPE                "float"
#define CCMC_VARIABLE_NAME_Z_BY_CLASSIFICATION            "position"
#define CCMC_VARIABLE_NAME_Z_BY_VALID_MIN                -10000.0
#define CCMC_VARIABLE_NAME_Z_BY_VALID_MAX                10000.0
#define CCMC_VARIABLE_NAME_Z_BY_UNITS                    "R"
#define CCMC_VARIABLE_NAME_Z_BY_GRID_SYSTEM            "grid_system_3"
#define CCMC_VARIABLE_NAME_Z_BY_MASK                    FLOAT_MASK
#define CCMC_VARIABLE_NAME_Z_BY_DESCRIPTION            "Z Coordinate for BY1.  BY1 is defined on a staggered grid different from X,Y,Z for the other variables"
#define CCMC_VARIABLE_NAME_Z_BY_IS_VECTOR                0
#define CCMC_VARIABLE_NAME_Z_BY_POSITION_GRID_SYSTEM    "grid_system_3"
#define CCMC_VARIABLE_NAME_Z_BY_DATA_GRID_SYSTEM        "grid_system_3"

#define CCMC_VARIABLE_NAME_Z_BZ                        "z_bz"
#define CCMC_VARIABLE_NAME_Z_BZ_DATA_TYPE                "float"
#define CCMC_VARIABLE_NAME_Z_BZ_CLASSIFICATION            "position"
#define CCMC_VARIABLE_NAME_Z_BZ_VALID_MIN                -10000.0
#define CCMC_VARIABLE_NAME_Z_BZ_VALID_MAX                10000.0
#define CCMC_VARIABLE_NAME_Z_BZ_UNITS                    "R"
#define CCMC_VARIABLE_NAME_Z_BZ_GRID_SYSTEM            "grid_system_4"
#define CCMC_VARIABLE_NAME_Z_BZ_MASK                    FLOAT_MASK
#define CCMC_VARIABLE_NAME_Z_BZ_DESCRIPTION            "Z Coordinate for BZ1.  BZ1 is defined on a staggered grid different from X,Y,Z for the other variables"
#define CCMC_VARIABLE_NAME_Z_BZ_IS_VECTOR                0
#define CCMC_VARIABLE_NAME_Z_BZ_POSITION_GRID_SYSTEM    "grid_system_4"
#define CCMC_VARIABLE_NAME_Z_BZ_DATA_GRID_SYSTEM        "grid_system_4"

/*******************************************************************************************************/
/**                                        geographic                                                **/
/*******************************************************************************************************/

#define CCMC_VARIABLE_NAME_C_LON                        "c_lon"
#define CCMC_VARIABLE_NAME_C_LON_DATA_TYPE                "float"
#define CCMC_VARIABLE_NAME_C_LON_CLASSIFICATION        "position"
#define CCMC_VARIABLE_NAME_C_LON_VALID_MIN                0
#define CCMC_VARIABLE_NAME_C_LON_VALID_MAX                360
#define CCMC_VARIABLE_NAME_C_LON_UNITS                    "deg"
#define CCMC_VARIABLE_NAME_C_LON_GRID_SYSTEM            "grid_system_1"
#define CCMC_VARIABLE_NAME_C_LON_MASK                    FLOAT_MASK
#define CCMC_VARIABLE_NAME_C_LON_DESCRIPTION            "Carrington Longitude"
#define CCMC_VARIABLE_NAME_C_LON_IS_VECTOR                0
#define CCMC_VARIABLE_NAME_C_LON_POSITION_GRID_SYSTEM    "grid_system_1"
#define CCMC_VARIABLE_NAME_C_LON_DATA_GRID_SYSTEM        "grid_system_1"

#define CCMC_VARIABLE_NAME_C_SINE_LAT                        "c_sine_lat"
#define CCMC_VARIABLE_NAME_C_SINE_LAT_DATA_TYPE            "float"
#define CCMC_VARIABLE_NAME_C_SINE_LAT_CLASSIFICATION        "position"
#define CCMC_VARIABLE_NAME_C_SINE_LAT_VALID_MIN            -1.0
#define CCMC_VARIABLE_NAME_C_SINE_LAT_VALID_MAX            1.0
#define CCMC_VARIABLE_NAME_C_SINE_LAT_UNITS                " "
#define CCMC_VARIABLE_NAME_C_SINE_LAT_GRID_SYSTEM            "grid_system_1"
#define CCMC_VARIABLE_NAME_C_SINE_LAT_MASK                    FLOAT_MASK
#define CCMC_VARIABLE_NAME_C_SINE_LAT_DESCRIPTION            "Carrington Sine Latitude"
#define CCMC_VARIABLE_NAME_C_SINE_LAT_IS_VECTOR            0
#define CCMC_VARIABLE_NAME_C_SINE_LAT_POSITION_GRID_SYSTEM    "grid_system_1"
#define CCMC_VARIABLE_NAME_C_SINE_LAT_DATA_GRID_SYSTEM        "grid_system_1"

#define CCMC_VARIABLE_NAME_LON                        "lon"
#define CCMC_VARIABLE_NAME_LON_DATA_TYPE                "float"
#define    CCMC_VARIABLE_NAME_LON_CLASSIFICATION            "data"
#define    CCMC_VARIABLE_NAME_LON_VALID_MIN                -100000000
#define    CCMC_VARIABLE_NAME_LON_VALID_MAX                100000000
#define    CCMC_VARIABLE_NAME_LON_UNITS                    "****** PROTOTYPE****** rad or deg "
#define    CCMC_VARIABLE_NAME_LON_GRID_SYSTEM            "grid_system_1"
#define CCMC_VARIABLE_NAME_LON_MASK                    FLOAT_MASK
#define    CCMC_VARIABLE_NAME_LON_DESCRIPTION            "****** PROTOTYPE****** Longitude"
#define    CCMC_VARIABLE_NAME_LON_IS_VECTOR                1
#define    CCMC_VARIABLE_NAME_LON_POSITION_GRID_SYSTEM    "grid_system_1"
#define    CCMC_VARIABLE_NAME_LON_DATA_GRID_SYSTEM        "grid_system_1"

#define CCMC_VARIABLE_NAME_LAT                        "lat"
#define CCMC_VARIABLE_NAME_LAT_DATA_TYPE                "float"
#define    CCMC_VARIABLE_NAME_LAT_CLASSIFICATION            "data"
#define    CCMC_VARIABLE_NAME_LAT_VALID_MIN                -100000000
#define    CCMC_VARIABLE_NAME_LAT_VALID_MAX                100000000
#define    CCMC_VARIABLE_NAME_LAT_UNITS                    "****** PROTOTYPE****** rad or deg "
#define    CCMC_VARIABLE_NAME_LAT_GRID_SYSTEM            "grid_system_1"
#define CCMC_VARIABLE_NAME_LAT_MASK                    FLOAT_MASK
#define    CCMC_VARIABLE_NAME_LAT_DESCRIPTION            "****** PROTOTYPE****** Latitude"
#define    CCMC_VARIABLE_NAME_LAT_IS_VECTOR                1
#define    CCMC_VARIABLE_NAME_LAT_POSITION_GRID_SYSTEM    "grid_system_1"
#define    CCMC_VARIABLE_NAME_LAT_DATA_GRID_SYSTEM        "grid_system_1"

/******************************************************************************************************
 *                                                                                                 *
 *                                         DATA VARIABLE NAMES                                        *
 *                                                                                                 *
 *******************************************************************************************************/

/*******************************************************************************************************/
/**                                        magnetic field                                             **/
/*******************************************************************************************************/

/*
 *
 * vector components on r,phi, theta
 *
 */

#define CCMC_VARIABLE_NAME_BR                            "br"
#define CCMC_VARIABLE_NAME_BR_DATA_TYPE                "float"
#define CCMC_VARIABLE_NAME_BR_CLASSIFICATION            "data"
#define CCMC_VARIABLE_NAME_BR_VALID_MIN                -1000000000.0
#define CCMC_VARIABLE_NAME_BR_VALID_MAX                1000000000.0
#define CCMC_VARIABLE_NAME_BR_UNITS                    "nT"
#define CCMC_VARIABLE_NAME_BR_GRID_SYSTEM                "grid_system_1"
#define CCMC_VARIABLE_NAME_BR_MASK                    FLOAT_MASK
#define CCMC_VARIABLE_NAME_BR_DESCRIPTION                "r Magnetic Field Component, Earth field up to 60000 nT, Solar field up to 3500 G in sunspots ( 3.5e8 nT ), 1 T is not exceeded in Solar/Heliospheric/Earth context"
#define CCMC_VARIABLE_NAME_BR_IS_VECTOR                1
#define CCMC_VARIABLE_NAME_BR_POSITION_GRID_SYSTEM        "grid_system_1"
#define CCMC_VARIABLE_NAME_BR_DATA_GRID_SYSTEM            "grid_system_1"

#define CCMC_VARIABLE_NAME_BR_1                        "br_1"
#define CCMC_VARIABLE_NAME_BR_1_DATA_TYPE                "float"
#define CCMC_VARIABLE_NAME_BR_1_CLASSIFICATION            "data"
#define CCMC_VARIABLE_NAME_BR_1_VALID_MIN                -1000000000.0
#define CCMC_VARIABLE_NAME_BR_1_VALID_MAX                1000000000.0
#define CCMC_VARIABLE_NAME_BR_1_UNITS                    "nT"
#define CCMC_VARIABLE_NAME_BR_1_GRID_SYSTEM            "grid_system_2"
#define CCMC_VARIABLE_NAME_BR_1_MASK                    FLOAT_MASK
#define CCMC_VARIABLE_NAME_BR_1_DESCRIPTION            "r Magnetic Field Component, Earth field up to 60000 nT, Solar field up to 3500 G in sunspots ( 3.5e8 nT ), 1 T is not exceeded in Solar/Heliospheric/Earth context"
#define CCMC_VARIABLE_NAME_BR_1_IS_VECTOR                1
#define CCMC_VARIABLE_NAME_BR_1_POSITION_GRID_SYSTEM    "grid_system_2"
#define CCMC_VARIABLE_NAME_BR_1_DATA_GRID_SYSTEM        "grid_system_2"

#define CCMC_VARIABLE_NAME_BR_2                        "br_2"
#define CCMC_VARIABLE_NAME_BR_2_DATA_TYPE                "float"
#define CCMC_VARIABLE_NAME_BR_2_CLASSIFICATION            "data"
#define CCMC_VARIABLE_NAME_BR_2_VALID_MIN                -1000000000.0
#define CCMC_VARIABLE_NAME_BR_2_VALID_MAX                1000000000.0
#define CCMC_VARIABLE_NAME_BR_2_UNITS                    "nT"
#define CCMC_VARIABLE_NAME_BR_2_GRID_SYSTEM            "grid_system_3"
#define CCMC_VARIABLE_NAME_BR_2_MASK                    FLOAT_MASK
#define CCMC_VARIABLE_NAME_BR_2_DESCRIPTION            "r Magnetic Field Component, Earth field up to 60000 nT, Solar field up to 3500 G in sunspots ( 3.5e8 nT ), 1 T is not exceeded in Solar/Heliospheric/Earth context"
#define CCMC_VARIABLE_NAME_BR_2_IS_VECTOR                1
#define CCMC_VARIABLE_NAME_BR_2_POSITION_GRID_SYSTEM    "grid_system_3"
#define CCMC_VARIABLE_NAME_BR_2_DATA_GRID_SYSTEM        "grid_system_3"

#define CCMC_VARIABLE_NAME_BR_3                        "br_3"
#define CCMC_VARIABLE_NAME_BR_3_DATA_TYPE                "float"
#define CCMC_VARIABLE_NAME_BR_3_CLASSIFICATION            "data"
#define CCMC_VARIABLE_NAME_BR_3_VALID_MIN                -1000000000.0
#define CCMC_VARIABLE_NAME_BR_3_VALID_MAX                1000000000.0
#define CCMC_VARIABLE_NAME_BR_3_UNITS                    "nT"
#define CCMC_VARIABLE_NAME_BR_3_GRID_SYSTEM            "grid_system_4"
#define CCMC_VARIABLE_NAME_BR_3_MASK                    FLOAT_MASK
#define CCMC_VARIABLE_NAME_BR_3_DESCRIPTION            "r Magnetic Field Component, Earth field up to 60000 nT, Solar field up to 3500 G in sunspots ( 3.5e8 nT ), 1 T is not exceeded in Solar/Heliospheric/Earth context"
#define CCMC_VARIABLE_NAME_BR_3_IS_VECTOR                1
#define CCMC_VARIABLE_NAME_BR_3_POSITION_GRID_SYSTEM    "grid_system_4"
#define CCMC_VARIABLE_NAME_BR_3_DATA_GRID_SYSTEM        "grid_system_4"

#define CCMC_VARIABLE_NAME_BR_4                        "br_4"
#define CCMC_VARIABLE_NAME_BR_4_DATA_TYPE                "float"
#define CCMC_VARIABLE_NAME_BR_4_CLASSIFICATION            "data"
#define CCMC_VARIABLE_NAME_BR_4_VALID_MIN                -1000000000.0
#define CCMC_VARIABLE_NAME_BR_4_VALID_MAX                1000000000.0
#define CCMC_VARIABLE_NAME_BR_4_UNITS                    "nT"
#define CCMC_VARIABLE_NAME_BR_4_GRID_SYSTEM            "grid_system_5"
#define CCMC_VARIABLE_NAME_BR_4_MASK                    FLOAT_MASK
#define CCMC_VARIABLE_NAME_BR_4_DESCRIPTION            "r Magnetic Field Component, Earth field up to 60000 nT, Solar field up to 3500 G in sunspots ( 3.5e8 nT ), 1 T is not exceeded in Solar/Heliospheric/Earth context"
#define CCMC_VARIABLE_NAME_BR_4_IS_VECTOR                1
#define CCMC_VARIABLE_NAME_BR_4_POSITION_GRID_SYSTEM    "grid_system_5"
#define CCMC_VARIABLE_NAME_BR_4_DATA_GRID_SYSTEM        "grid_system_5"

#define CCMC_VARIABLE_NAME_BPHI                        "bphi"
#define CCMC_VARIABLE_NAME_BPHI_DATA_TYPE                "float"
#define CCMC_VARIABLE_NAME_BPHI_CLASSIFICATION            "data"
#define CCMC_VARIABLE_NAME_BPHI_VALID_MIN                -1000000000.0
#define CCMC_VARIABLE_NAME_BPHI_VALID_MAX                1000000000.0
#define CCMC_VARIABLE_NAME_BPHI_UNITS                    "nT"
#define CCMC_VARIABLE_NAME_BPHI_GRID_SYSTEM            "grid_system_1"
#define CCMC_VARIABLE_NAME_BPHI_MASK                    FLOAT_MASK
#define CCMC_VARIABLE_NAME_BPHI_DESCRIPTION            "phi Magnetic Field Component, Earth field up to 60000 nT, Solar field up to 3500 G in sunspots ( 3.5e8 nT ), 1 T is not exceeded in Solar/Heliospheric/Earth context"
#define CCMC_VARIABLE_NAME_BPHI_IS_VECTOR                1
#define CCMC_VARIABLE_NAME_BPHI_POSITION_GRID_SYSTEM    "grid_system_1"
#define CCMC_VARIABLE_NAME_BPHI_DATA_GRID_SYSTEM        "grid_system_1"

#define CCMC_VARIABLE_NAME_BPHI_1                        "bphi_1"
#define CCMC_VARIABLE_NAME_BPHI_1_DATA_TYPE            "float"
#define CCMC_VARIABLE_NAME_BPHI_1_CLASSIFICATION        "data"
#define CCMC_VARIABLE_NAME_BPHI_1_VALID_MIN            -1000000000.0
#define CCMC_VARIABLE_NAME_BPHI_1_VALID_MAX            1000000000.0
#define CCMC_VARIABLE_NAME_BPHI_1_UNITS                "nT"
#define CCMC_VARIABLE_NAME_BPHI_1_GRID_SYSTEM            "grid_system_2"
#define CCMC_VARIABLE_NAME_BPHI_1_MASK                    FLOAT_MASK
#define CCMC_VARIABLE_NAME_BPHI_1_DESCRIPTION            "phi Magnetic Field Component, Earth field up to 60000 nT, Solar field up to 3500 G in sunspots ( 3.5e8 nT ), 1 T is not exceeded in Solar/Heliospheric/Earth context"
#define CCMC_VARIABLE_NAME_BPHI_1_IS_VECTOR            1
#define CCMC_VARIABLE_NAME_BPHI_1_POSITION_GRID_SYSTEM    "grid_system_2"
#define CCMC_VARIABLE_NAME_BPHI_1_DATA_GRID_SYSTEM        "grid_system_2"

#define CCMC_VARIABLE_NAME_BPHI_2                        "bphi_2"
#define CCMC_VARIABLE_NAME_BPHI_2_DATA_TYPE            "float"
#define CCMC_VARIABLE_NAME_BPHI_2_CLASSIFICATION        "data"
#define CCMC_VARIABLE_NAME_BPHI_2_VALID_MIN            -1000000000.0
#define CCMC_VARIABLE_NAME_BPHI_2_VALID_MAX            1000000000.0
#define CCMC_VARIABLE_NAME_BPHI_2_UNITS                "nT"
#define CCMC_VARIABLE_NAME_BPHI_2_GRID_SYSTEM            "grid_system_3"
#define CCMC_VARIABLE_NAME_BPHI_2_MASK                    FLOAT_MASK
#define CCMC_VARIABLE_NAME_BPHI_2_DESCRIPTION            "phi Magnetic Field Component, Earth field up to 60000 nT, Solar field up to 3500 G in sunspots ( 3.5e8 nT ), 1 T is not exceeded in Solar/Heliospheric/Earth context"
#define CCMC_VARIABLE_NAME_BPHI_2_IS_VECTOR            1
#define CCMC_VARIABLE_NAME_BPHI_2_POSITION_GRID_SYSTEM    "grid_system_3"
#define CCMC_VARIABLE_NAME_BPHI_2_DATA_GRID_SYSTEM        "grid_system_3"

#define CCMC_VARIABLE_NAME_BPHI_3                        "bphi_3"
#define CCMC_VARIABLE_NAME_BPHI_3_DATA_TYPE            "float"
#define CCMC_VARIABLE_NAME_BPHI_3_CLASSIFICATION        "data"
#define CCMC_VARIABLE_NAME_BPHI_3_VALID_MIN            -1000000000.0
#define CCMC_VARIABLE_NAME_BPHI_3_VALID_MAX            1000000000.0
#define CCMC_VARIABLE_NAME_BPHI_3_UNITS                "nT"
#define CCMC_VARIABLE_NAME_BPHI_3_GRID_SYSTEM            "grid_system_4"
#define CCMC_VARIABLE_NAME_BPHI_3_MASK                    FLOAT_MASK
#define CCMC_VARIABLE_NAME_BPHI_3_DESCRIPTION            "phi Magnetic Field Component, Earth field up to 60000 nT, Solar field up to 3500 G in sunspots ( 3.5e8 nT ), 1 T is not exceeded in Solar/Heliospheric/Earth context"
#define CCMC_VARIABLE_NAME_BPHI_3_IS_VECTOR            1
#define CCMC_VARIABLE_NAME_BPHI_3_POSITION_GRID_SYSTEM    "grid_system_4"
#define CCMC_VARIABLE_NAME_BPHI_3_DATA_GRID_SYSTEM        "grid_system_4"

#define CCMC_VARIABLE_NAME_BPHI_4                        "bphi_4"
#define CCMC_VARIABLE_NAME_BPHI_4_DATA_TYPE            "float"
#define CCMC_VARIABLE_NAME_BPHI_4_CLASSIFICATION        "data"
#define CCMC_VARIABLE_NAME_BPHI_4_VALID_MIN            -1000000000.0
#define CCMC_VARIABLE_NAME_BPHI_4_VALID_MAX            1000000000.0
#define CCMC_VARIABLE_NAME_BPHI_4_UNITS                "nT"
#define CCMC_VARIABLE_NAME_BPHI_4_GRID_SYSTEM            "grid_system_5"
#define CCMC_VARIABLE_NAME_BPHI_4_MASK                    FLOAT_MASK
#define CCMC_VARIABLE_NAME_BPHI_4_DESCRIPTION            "phi Magnetic Field Component, Earth field up to 60000 nT, Solar field up to 3500 G in sunspots ( 3.5e8 nT ), 1 T is not exceeded in Solar/Heliospheric/Earth context"
#define CCMC_VARIABLE_NAME_BPHI_4_IS_VECTOR            1
#define CCMC_VARIABLE_NAME_BPHI_4_POSITION_GRID_SYSTEM    "grid_system_5"
#define CCMC_VARIABLE_NAME_BPHI_4_DATA_GRID_SYSTEM        "grid_system_5"

#define CCMC_VARIABLE_NAME_BTHETA                        "btheta"
#define CCMC_VARIABLE_NAME_BTHETA_DATA_TYPE            "float"
#define CCMC_VARIABLE_NAME_BTHETA_CLASSIFICATION        "data"
#define CCMC_VARIABLE_NAME_BTHETA_VALID_MIN            -1000000000.0
#define CCMC_VARIABLE_NAME_BTHETA_VALID_MAX            1000000000.0
#define CCMC_VARIABLE_NAME_BTHETA_UNITS                "nT"
#define CCMC_VARIABLE_NAME_BTHETA_GRID_SYSTEM            "grid_system_1"
#define CCMC_VARIABLE_NAME_BTHETA_MASK                    FLOAT_MASK
#define CCMC_VARIABLE_NAME_BTHETA_DESCRIPTION            "theta Magnetic Field Component, Earth field up to 60000 nT, Solar field up to 3500 G in sunspots ( 3.5e8 nT ), 1 T is not exceeded in Solar/Heliospheric/Earth context"
#define CCMC_VARIABLE_NAME_BTHETA_IS_VECTOR            1
#define CCMC_VARIABLE_NAME_BTHETA_POSITION_GRID_SYSTEM    "grid_system_1"
#define CCMC_VARIABLE_NAME_BTHETA_DATA_GRID_SYSTEM        "grid_system_1"

#define CCMC_VARIABLE_NAME_BTHETA_1                        "btheta_1"
#define CCMC_VARIABLE_NAME_BTHETA_1_DATA_TYPE                "float"
#define CCMC_VARIABLE_NAME_BTHETA_1_CLASSIFICATION            "data"
#define CCMC_VARIABLE_NAME_BTHETA_1_VALID_MIN                -1000000000.0
#define CCMC_VARIABLE_NAME_BTHETA_1_VALID_MAX                1000000000.0
#define CCMC_VARIABLE_NAME_BTHETA_1_UNITS                    "nT"
#define CCMC_VARIABLE_NAME_BTHETA_1_GRID_SYSTEM            "grid_system_2"
#define CCMC_VARIABLE_NAME_BTHETA_1_MASK                    FLOAT_MASK
#define CCMC_VARIABLE_NAME_BTHETA_1_DESCRIPTION            "theta Magnetic Field Component, Earth field up to 60000 nT, Solar field up to 3500 G in sunspots ( 3.5e8 nT ), 1 T is not exceeded in Solar/Heliospheric/Earth context"
#define CCMC_VARIABLE_NAME_BTHETA_1_IS_VECTOR                1
#define CCMC_VARIABLE_NAME_BTHETA_1_POSITION_GRID_SYSTEM    "grid_system_2"
#define CCMC_VARIABLE_NAME_BTHETA_1_DATA_GRID_SYSTEM        "grid_system_2"

#define CCMC_VARIABLE_NAME_BTHETA_2                        "btheta_2"
#define CCMC_VARIABLE_NAME_BTHETA_2_DATA_TYPE                "float"
#define CCMC_VARIABLE_NAME_BTHETA_2_CLASSIFICATION            "data"
#define CCMC_VARIABLE_NAME_BTHETA_2_VALID_MIN                -1000000000.0
#define CCMC_VARIABLE_NAME_BTHETA_2_VALID_MAX                1000000000.0
#define CCMC_VARIABLE_NAME_BTHETA_2_UNITS                    "nT"
#define CCMC_VARIABLE_NAME_BTHETA_2_GRID_SYSTEM            "grid_system_3"
#define CCMC_VARIABLE_NAME_BTHETA_2_MASK                    FLOAT_MASK
#define CCMC_VARIABLE_NAME_BTHETA_2_DESCRIPTION            "theta Magnetic Field Component, Earth field up to 60000 nT, Solar field up to 3500 G in sunspots ( 3.5e8 nT ), 1 T is not exceeded in Solar/Heliospheric/Earth context"
#define CCMC_VARIABLE_NAME_BTHETA_2_IS_VECTOR                1
#define CCMC_VARIABLE_NAME_BTHETA_2_POSITION_GRID_SYSTEM    "grid_system_3"
#define CCMC_VARIABLE_NAME_BTHETA_2_DATA_GRID_SYSTEM        "grid_system_3"

#define CCMC_VARIABLE_NAME_BTHETA_3                        "btheta_3"
#define CCMC_VARIABLE_NAME_BTHETA_3_DATA_TYPE                "float"
#define CCMC_VARIABLE_NAME_BTHETA_3_CLASSIFICATION            "data"
#define CCMC_VARIABLE_NAME_BTHETA_3_VALID_MIN                -1000000000.0
#define CCMC_VARIABLE_NAME_BTHETA_3_VALID_MAX                1000000000.0
#define CCMC_VARIABLE_NAME_BTHETA_3_UNITS                    "nT"
#define CCMC_VARIABLE_NAME_BTHETA_3_GRID_SYSTEM            "grid_system_4"
#define CCMC_VARIABLE_NAME_BTHETA_3_MASK                    FLOAT_MASK
#define CCMC_VARIABLE_NAME_BTHETA_3_DESCRIPTION            "theta Magnetic Field Component, Earth field up to 60000 nT, Solar field up to 3500 G in sunspots ( 3.5e8 nT ), 1 T is not exceeded in Solar/Heliospheric/Earth context"
#define CCMC_VARIABLE_NAME_BTHETA_3_IS_VECTOR                1
#define CCMC_VARIABLE_NAME_BTHETA_3_POSITION_GRID_SYSTEM    "grid_system_4"
#define CCMC_VARIABLE_NAME_BTHETA_3_DATA_GRID_SYSTEM        "grid_system_4"

#define CCMC_VARIABLE_NAME_BTHETA_4                        "btheta_4"
#define CCMC_VARIABLE_NAME_BTHETA_4_DATA_TYPE                "float"
#define CCMC_VARIABLE_NAME_BTHETA_4_CLASSIFICATION            "data"
#define CCMC_VARIABLE_NAME_BTHETA_4_VALID_MIN                -1000000000.0
#define CCMC_VARIABLE_NAME_BTHETA_4_VALID_MAX                1000000000.0
#define CCMC_VARIABLE_NAME_BTHETA_4_UNITS                    "nT"
#define CCMC_VARIABLE_NAME_BTHETA_4_GRID_SYSTEM            "grid_system_5"
#define CCMC_VARIABLE_NAME_BTHETA_4_MASK                    FLOAT_MASK
#define CCMC_VARIABLE_NAME_BTHETA_4_DESCRIPTION            "phi Magnetic Field Component, Earth field up to 60000 nT, Solar field up to 3500 G in sunspots ( 3.5e8 nT ), 1 T is not exceeded in Solar/Heliospheric/Earth context"
#define CCMC_VARIABLE_NAME_BTHETA_4_IS_VECTOR                1
#define CCMC_VARIABLE_NAME_BTHETA_4_POSITION_GRID_SYSTEM    "grid_system_5"
#define CCMC_VARIABLE_NAME_BTHETA_4_DATA_GRID_SYSTEM        "grid_system_5"

#define CCMC_VARIABLE_NAME_B1R                        "b1r"
#define CCMC_VARIABLE_NAME_B1R_DATA_TYPE                "float"
#define    CCMC_VARIABLE_NAME_B1R_CLASSIFICATION            "data"
#define    CCMC_VARIABLE_NAME_B1R_VALID_MIN                -1000000000.0
#define    CCMC_VARIABLE_NAME_B1R_VALID_MAX                1000000000.0
#define    CCMC_VARIABLE_NAME_B1R_UNITS                    "nT"
#define    CCMC_VARIABLE_NAME_B1R_GRID_SYSTEM            "grid_system_1"
#define CCMC_VARIABLE_NAME_B1R_MASK                    FLOAT_MASK
#define    CCMC_VARIABLE_NAME_B1R_DESCRIPTION            "Deviative X Magnetic Field Component, Earth field up to 60000 nT, Solar field up to 3500 G in sunspots ( 3.5e8 nT ), 1 T is not exceeded in Solar/Heliospheric/Earth context"
#define    CCMC_VARIABLE_NAME_B1R_IS_VECTOR                1
#define    CCMC_VARIABLE_NAME_B1R_POSITION_GRID_SYSTEM    "grid_system_1"
#define    CCMC_VARIABLE_NAME_B1R_DATA_GRID_SYSTEM        "grid_system_1"

#define CCMC_VARIABLE_NAME_B1PHI                        "b1phi"
#define CCMC_VARIABLE_NAME_B1PHI_DATA_TYPE                "float"
#define    CCMC_VARIABLE_NAME_B1PHI_CLASSIFICATION        "data"
#define    CCMC_VARIABLE_NAME_B1PHI_VALID_MIN            -1000000000.0
#define    CCMC_VARIABLE_NAME_B1PHI_VALID_MAX            1000000000.0
#define    CCMC_VARIABLE_NAME_B1PHI_UNITS                "nT"
#define    CCMC_VARIABLE_NAME_B1PHI_GRID_SYSTEM            "grid_system_1"
#define CCMC_VARIABLE_NAME_B1PHI_MASK                    FLOAT_MASK
#define    CCMC_VARIABLE_NAME_B1PHI_DESCRIPTION            "Deviative Y Magnetic Field Component, Earth field up to 60000 nT, Solar field up to 3500 G in sunspots ( 3.5e8 nT ), 1 T is not exceeded in Solar/Heliospheric/Earth context"
#define    CCMC_VARIABLE_NAME_B1PHI_IS_VECTOR            1
#define    CCMC_VARIABLE_NAME_B1PHI_POSITION_GRID_SYSTEM    "grid_system_1"
#define    CCMC_VARIABLE_NAME_B1PHI_DATA_GRID_SYSTEM        "grid_system_1"

#define CCMC_VARIABLE_NAME_B1THETA                        "b1theta"
#define CCMC_VARIABLE_NAME_B1THETA_DATA_TYPE                "float"
#define    CCMC_VARIABLE_NAME_B1THETA_CLASSIFICATION            "data"
#define    CCMC_VARIABLE_NAME_B1THETA_VALID_MIN                -1000000000.0
#define    CCMC_VARIABLE_NAME_B1THETA_VALID_MAX                1000000000.0
#define    CCMC_VARIABLE_NAME_B1THETA_UNITS                    "nT"
#define    CCMC_VARIABLE_NAME_B1THETA_GRID_SYSTEM                "grid_system_1"
#define CCMC_VARIABLE_NAME_B1THETA_MASK                    FLOAT_MASK
#define    CCMC_VARIABLE_NAME_B1THETA_DESCRIPTION                "Deviative Z Magnetic Field Component, Earth field up to 60000 nT, Solar field up to 3500 G in sunspots ( 3.5e8 nT ), 1 T is not exceeded in Solar/Heliospheric/Earth context"
#define    CCMC_VARIABLE_NAME_B1THETA_IS_VECTOR                1
#define    CCMC_VARIABLE_NAME_B1THETA_POSITION_GRID_SYSTEM    "grid_system_1"
#define    CCMC_VARIABLE_NAME_B1THETA_DATA_GRID_SYSTEM        "grid_system_1"

/*
 *
 * vector components on x, y, z
 *
 */

#define CCMC_VARIABLE_NAME_BX                            "bx"
#define CCMC_VARIABLE_NAME_BX_DATA_TYPE                "float"
#define CCMC_VARIABLE_NAME_BX_CLASSIFICATION            "data"
#define CCMC_VARIABLE_NAME_BX_VALID_MIN                -1000000000.0
#define CCMC_VARIABLE_NAME_BX_VALID_MAX                1000000000.0
#define CCMC_VARIABLE_NAME_BX_UNITS                    "nT"
#define CCMC_VARIABLE_NAME_BX_GRID_SYSTEM                "grid_system_1"
#define CCMC_VARIABLE_NAME_BX_MASK                    FLOAT_MASK
#define CCMC_VARIABLE_NAME_BX_DESCRIPTION                "X Magnetic Field Component, Earth field up to 60000 nT, Solar field up to 3500 G in sunspots ( 3.5e8 nT ), 1 T is not exceeded in Solar/Heliospheric/Earth context"
#define CCMC_VARIABLE_NAME_BX_IS_VECTOR                1
#define CCMC_VARIABLE_NAME_BX_POSITION_GRID_SYSTEM        "grid_system_1"
#define CCMC_VARIABLE_NAME_BX_DATA_GRID_SYSTEM            "grid_system_1"

#define CCMC_VARIABLE_NAME_BX_1                        "bx_1"
#define CCMC_VARIABLE_NAME_BX_1_DATA_TYPE                "float"
#define CCMC_VARIABLE_NAME_BX_1_CLASSIFICATION            "data"
#define CCMC_VARIABLE_NAME_BX_1_VALID_MIN                -1000000000.0
#define CCMC_VARIABLE_NAME_BX_1_VALID_MAX                1000000000.0
#define CCMC_VARIABLE_NAME_BX_1_UNITS                    "nT"
#define CCMC_VARIABLE_NAME_BX_1_GRID_SYSTEM            "grid_system_2"
#define CCMC_VARIABLE_NAME_BX_1_MASK                    FLOAT_MASK
#define CCMC_VARIABLE_NAME_BX_1_DESCRIPTION            "X Magnetic Field Component, Earth field up to 60000 nT, Solar field up to 3500 G in sunspots ( 3.5e8 nT ), 1 T is not exceeded in Solar/Heliospheric/Earth context"
#define CCMC_VARIABLE_NAME_BX_1_IS_VECTOR                1
#define CCMC_VARIABLE_NAME_BX_1_POSITION_GRID_SYSTEM    "grid_system_2"
#define CCMC_VARIABLE_NAME_BX_1_DATA_GRID_SYSTEM        "grid_system_2"

#define CCMC_VARIABLE_NAME_BX_2                        "bx_2"
#define CCMC_VARIABLE_NAME_BX_2_DATA_TYPE                "float"
#define CCMC_VARIABLE_NAME_BX_2_CLASSIFICATION            "data"
#define CCMC_VARIABLE_NAME_BX_2_VALID_MIN                -1000000000.0
#define CCMC_VARIABLE_NAME_BX_2_VALID_MAX                1000000000.0
#define CCMC_VARIABLE_NAME_BX_2_UNITS                    "nT"
#define CCMC_VARIABLE_NAME_BX_2_GRID_SYSTEM            "grid_system_3"
#define CCMC_VARIABLE_NAME_BX_2_MASK                    FLOAT_MASK
#define CCMC_VARIABLE_NAME_BX_2_DESCRIPTION            "X Magnetic Field Component, Earth field up to 60000 nT, Solar field up to 3500 G in sunspots ( 3.5e8 nT ), 1 T is not exceeded in Solar/Heliospheric/Earth context"
#define CCMC_VARIABLE_NAME_BX_2_IS_VECTOR                1
#define CCMC_VARIABLE_NAME_BX_2_POSITION_GRID_SYSTEM    "grid_system_3"
#define CCMC_VARIABLE_NAME_BX_2_DATA_GRID_SYSTEM        "grid_system_3"

#define CCMC_VARIABLE_NAME_BX_3                        "bx_3"
#define CCMC_VARIABLE_NAME_BX_3_DATA_TYPE                "float"
#define CCMC_VARIABLE_NAME_BX_3_CLASSIFICATION            "data"
#define CCMC_VARIABLE_NAME_BX_3_VALID_MIN                -1000000000.0
#define CCMC_VARIABLE_NAME_BX_3_VALID_MAX                1000000000.0
#define CCMC_VARIABLE_NAME_BX_3_UNITS                    "nT"
#define CCMC_VARIABLE_NAME_BX_3_GRID_SYSTEM            "grid_system_4"
#define CCMC_VARIABLE_NAME_BX_3_MASK                    FLOAT_MASK
#define CCMC_VARIABLE_NAME_BX_3_DESCRIPTION            "X Magnetic Field Component, Earth field up to 60000 nT, Solar field up to 3500 G in sunspots ( 3.5e8 nT ), 1 T is not exceeded in Solar/Heliospheric/Earth context"
#define CCMC_VARIABLE_NAME_BX_3_IS_VECTOR                1
#define CCMC_VARIABLE_NAME_BX_3_POSITION_GRID_SYSTEM    "grid_system_4"
#define CCMC_VARIABLE_NAME_BX_3_DATA_GRID_SYSTEM        "grid_system_4"

#define CCMC_VARIABLE_NAME_BX_4                        "bx_4"
#define CCMC_VARIABLE_NAME_BX_4_DATA_TYPE                "float"
#define CCMC_VARIABLE_NAME_BX_4_CLASSIFICATION            "data"
#define CCMC_VARIABLE_NAME_BX_4_VALID_MIN                -1000000000.0
#define CCMC_VARIABLE_NAME_BX_4_VALID_MAX                1000000000.0
#define CCMC_VARIABLE_NAME_BX_4_UNITS                    "nT"
#define CCMC_VARIABLE_NAME_BX_4_GRID_SYSTEM            "grid_system_5"
#define CCMC_VARIABLE_NAME_BX_4_MASK                    FLOAT_MASK
#define CCMC_VARIABLE_NAME_BX_4_DESCRIPTION            "X Magnetic Field Component, Earth field up to 60000 nT, Solar field up to 3500 G in sunspots ( 3.5e8 nT ), 1 T is not exceeded in Solar/Heliospheric/Earth context"
#define CCMC_VARIABLE_NAME_BX_4_IS_VECTOR                1
#define CCMC_VARIABLE_NAME_BX_4_POSITION_GRID_SYSTEM    "grid_system_5"
#define CCMC_VARIABLE_NAME_BX_4_DATA_GRID_SYSTEM        "grid_system_5"

#define CCMC_VARIABLE_NAME_BY                            "by"
#define CCMC_VARIABLE_NAME_BY_DATA_TYPE                "float"
#define CCMC_VARIABLE_NAME_BY_CLASSIFICATION            "data"
#define    CCMC_VARIABLE_NAME_BY_VALID_MIN                -1000000000.0
#define    CCMC_VARIABLE_NAME_BY_VALID_MAX                1000000000.0
#define    CCMC_VARIABLE_NAME_BY_UNITS                    "nT"
#define    CCMC_VARIABLE_NAME_BY_GRID_SYSTEM                "grid_system_1"
#define CCMC_VARIABLE_NAME_BY_MASK                    FLOAT_MASK
#define    CCMC_VARIABLE_NAME_BY_DESCRIPTION                "Y Magnetic Field Component, Earth field up to 60000 nT, Solar field up to 3500 G in sunspots ( 3.5e8 nT ), 1 T is not exceeded in Solar/Heliospheric/Earth context"
#define    CCMC_VARIABLE_NAME_BY_IS_VECTOR                1
#define    CCMC_VARIABLE_NAME_BY_POSITION_GRID_SYSTEM        "grid_system_1"
#define    CCMC_VARIABLE_NAME_BY_DATA_GRID_SYSTEM            "grid_system_1"

#define CCMC_VARIABLE_NAME_BY_1                        "by_1"
#define CCMC_VARIABLE_NAME_BY_1_DATA_TYPE                "float"
#define CCMC_VARIABLE_NAME_BY_1_CLASSIFICATION            "data"
#define CCMC_VARIABLE_NAME_BY_1_VALID_MIN                -1000000000.0
#define CCMC_VARIABLE_NAME_BY_1_VALID_MAX                1000000000.0
#define CCMC_VARIABLE_NAME_BY_1_UNITS                    "nT"
#define CCMC_VARIABLE_NAME_BY_1_GRID_SYSTEM            "grid_system_2"
#define CCMC_VARIABLE_NAME_BY_1_MASK                    FLOAT_MASK
#define CCMC_VARIABLE_NAME_BY_1_DESCRIPTION            "Y Magnetic Field Component, Earth field up to 60000 nT, Solar field up to 3500 G in sunspots ( 3.5e8 nT ), 1 T is not exceeded in Solar/Heliospheric/Earth context"
#define CCMC_VARIABLE_NAME_BY_1_IS_VECTOR                1
#define CCMC_VARIABLE_NAME_BY_1_POSITION_GRID_SYSTEM    "grid_system_2"
#define CCMC_VARIABLE_NAME_BY_1_DATA_GRID_SYSTEM        "grid_system_2"

#define CCMC_VARIABLE_NAME_BY_2                        "by_2"
#define CCMC_VARIABLE_NAME_BY_2_DATA_TYPE                "float"
#define CCMC_VARIABLE_NAME_BY_2_CLASSIFICATION            "data"
#define CCMC_VARIABLE_NAME_BY_2_VALID_MIN                -1000000000.0
#define CCMC_VARIABLE_NAME_BY_2_VALID_MAX                1000000000.0
#define CCMC_VARIABLE_NAME_BY_2_UNITS                    "nT"
#define CCMC_VARIABLE_NAME_BY_2_GRID_SYSTEM            "grid_system_3"
#define CCMC_VARIABLE_NAME_BY_2_MASK                    FLOAT_MASK
#define CCMC_VARIABLE_NAME_BY_2_DESCRIPTION            "Y Magnetic Field Component, Earth field up to 60000 nT, Solar field up to 3500 G in sunspots ( 3.5e8 nT ), 1 T is not exceeded in Solar/Heliospheric/Earth context"
#define CCMC_VARIABLE_NAME_BY_2_IS_VECTOR                1
#define CCMC_VARIABLE_NAME_BY_2_POSITION_GRID_SYSTEM    "grid_system_3"
#define CCMC_VARIABLE_NAME_BY_2_DATA_GRID_SYSTEM        "grid_system_3"

#define CCMC_VARIABLE_NAME_BY_3                        "by_3"
#define CCMC_VARIABLE_NAME_BY_3_DATA_TYPE                "float"
#define CCMC_VARIABLE_NAME_BY_3_CLASSIFICATION            "data"
#define CCMC_VARIABLE_NAME_BY_3_VALID_MIN                -1000000000.0
#define CCMC_VARIABLE_NAME_BY_3_VALID_MAX                1000000000.0
#define CCMC_VARIABLE_NAME_BY_3_UNITS                    "nT"
#define CCMC_VARIABLE_NAME_BY_3_GRID_SYSTEM            "grid_system_4"
#define CCMC_VARIABLE_NAME_BY_3_MASK                    FLOAT_MASK
#define CCMC_VARIABLE_NAME_BY_3_DESCRIPTION            "Y Magnetic Field Component, Earth field up to 60000 nT, Solar field up to 3500 G in sunspots ( 3.5e8 nT ), 1 T is not exceeded in Solar/Heliospheric/Earth context"
#define CCMC_VARIABLE_NAME_BY_3_IS_VECTOR                1
#define CCMC_VARIABLE_NAME_BY_3_POSITION_GRID_SYSTEM    "grid_system_4"
#define CCMC_VARIABLE_NAME_BY_3_DATA_GRID_SYSTEM        "grid_system_4"

#define CCMC_VARIABLE_NAME_BY_4                        "by_4"
#define CCMC_VARIABLE_NAME_BY_4_DATA_TYPE                "float"
#define CCMC_VARIABLE_NAME_BY_4_CLASSIFICATION            "data"
#define CCMC_VARIABLE_NAME_BY_4_VALID_MIN                -1000000000.0
#define CCMC_VARIABLE_NAME_BY_4_VALID_MAX                1000000000.0
#define CCMC_VARIABLE_NAME_BY_4_UNITS                    "nT"
#define CCMC_VARIABLE_NAME_BY_4_GRID_SYSTEM            "grid_system_5"
#define CCMC_VARIABLE_NAME_BY_4_MASK                    FLOAT_MASK
#define CCMC_VARIABLE_NAME_BY_4_DESCRIPTION            "Y Magnetic Field Component, Earth field up to 60000 nT, Solar field up to 3500 G in sunspots ( 3.5e8 nT ), 1 T is not exceeded in Solar/Heliospheric/Earth context"
#define CCMC_VARIABLE_NAME_BY_4_IS_VECTOR                1
#define CCMC_VARIABLE_NAME_BY_4_POSITION_GRID_SYSTEM    "grid_system_5"
#define CCMC_VARIABLE_NAME_BY_4_DATA_GRID_SYSTEM        "grid_system_5"

#define CCMC_VARIABLE_NAME_BZ                            "bz"
#define CCMC_VARIABLE_NAME_BZ_DATA_TYPE                "float"
#define    CCMC_VARIABLE_NAME_BZ_CLASSIFICATION            "data"
#define    CCMC_VARIABLE_NAME_BZ_VALID_MIN                -1000000000.0
#define    CCMC_VARIABLE_NAME_BZ_VALID_MAX                1000000000.0
#define    CCMC_VARIABLE_NAME_BZ_UNITS                    "nT"
#define    CCMC_VARIABLE_NAME_BZ_GRID_SYSTEM                "grid_system_1"
#define CCMC_VARIABLE_NAME_BZ_MASK                    FLOAT_MASK
#define    CCMC_VARIABLE_NAME_BZ_DESCRIPTION                "Z Magnetic Field Component, Earth field up to 60000 nT, Solar field up to 3500 G in sunspots ( 3.5e8 nT ), 1 T is not exceeded in Solar/Heliospheric/Earth context"
#define    CCMC_VARIABLE_NAME_BZ_IS_VECTOR                1
#define    CCMC_VARIABLE_NAME_BZ_POSITION_GRID_SYSTEM        "grid_system_1"
#define    CCMC_VARIABLE_NAME_BZ_DATA_GRID_SYSTEM            "grid_system_1"

#define CCMC_VARIABLE_NAME_BZ_1                        "bz_1"
#define CCMC_VARIABLE_NAME_BZ_1_DATA_TYPE                "float"
#define CCMC_VARIABLE_NAME_BZ_1_CLASSIFICATION            "data"
#define CCMC_VARIABLE_NAME_BZ_1_VALID_MIN                -1000000000.0
#define CCMC_VARIABLE_NAME_BZ_1_VALID_MAX                1000000000.0
#define CCMC_VARIABLE_NAME_BZ_1_UNITS                    "nT"
#define CCMC_VARIABLE_NAME_BZ_1_GRID_SYSTEM            "grid_system_2"
#define CCMC_VARIABLE_NAME_BZ_1_MASK                    FLOAT_MASK
#define CCMC_VARIABLE_NAME_BZ_1_DESCRIPTION            "Z Magnetic Field Component, Earth field up to 60000 nT, Solar field up to 3500 G in sunspots ( 3.5e8 nT ), 1 T is not exceeded in Solar/Heliospheric/Earth context"
#define CCMC_VARIABLE_NAME_BZ_1_IS_VECTOR                1
#define CCMC_VARIABLE_NAME_BZ_1_POSITION_GRID_SYSTEM    "grid_system_2"
#define CCMC_VARIABLE_NAME_BZ_1_DATA_GRID_SYSTEM        "grid_system_2"

#define CCMC_VARIABLE_NAME_BZ_2                        "bz_2"
#define CCMC_VARIABLE_NAME_BZ_2_DATA_TYPE                "float"
#define CCMC_VARIABLE_NAME_BZ_2_CLASSIFICATION            "data"
#define CCMC_VARIABLE_NAME_BZ_2_VALID_MIN                -1000000000.0
#define CCMC_VARIABLE_NAME_BZ_2_VALID_MAX                1000000000.0
#define CCMC_VARIABLE_NAME_BZ_2_UNITS                    "nT"
#define CCMC_VARIABLE_NAME_BZ_2_GRID_SYSTEM            "grid_system_3"
#define CCMC_VARIABLE_NAME_BZ_2_MASK                    FLOAT_MASK
#define CCMC_VARIABLE_NAME_BZ_2_DESCRIPTION            "Z Magnetic Field Component, Earth field up to 60000 nT, Solar field up to 3500 G in sunspots ( 3.5e8 nT ), 1 T is not exceeded in Solar/Heliospheric/Earth context"
#define CCMC_VARIABLE_NAME_BZ_2_IS_VECTOR                1
#define CCMC_VARIABLE_NAME_BZ_2_POSITION_GRID_SYSTEM    "grid_system_3"
#define CCMC_VARIABLE_NAME_BZ_2_DATA_GRID_SYSTEM        "grid_system_3"

#define CCMC_VARIABLE_NAME_BZ_3                        "bz_3"
#define CCMC_VARIABLE_NAME_BZ_3_DATA_TYPE                "float"
#define CCMC_VARIABLE_NAME_BZ_3_CLASSIFICATION            "data"
#define CCMC_VARIABLE_NAME_BZ_3_VALID_MIN                -1000000000.0
#define CCMC_VARIABLE_NAME_BZ_3_VALID_MAX                1000000000.0
#define CCMC_VARIABLE_NAME_BZ_3_UNITS                    "nT"
#define CCMC_VARIABLE_NAME_BZ_3_GRID_SYSTEM            "grid_system_4"
#define CCMC_VARIABLE_NAME_BZ_3_MASK                    FLOAT_MASK
#define CCMC_VARIABLE_NAME_BZ_3_DESCRIPTION            "Z Magnetic Field Component, Earth field up to 60000 nT, Solar field up to 3500 G in sunspots ( 3.5e8 nT ), 1 T is not exceeded in Solar/Heliospheric/Earth context"
#define CCMC_VARIABLE_NAME_BZ_3_IS_VECTOR                1
#define CCMC_VARIABLE_NAME_BZ_3_POSITION_GRID_SYSTEM    "grid_system_4"
#define CCMC_VARIABLE_NAME_BZ_3_DATA_GRID_SYSTEM        "grid_system_4"

#define CCMC_VARIABLE_NAME_BZ_4                        "bz_4"
#define CCMC_VARIABLE_NAME_BZ_4_DATA_TYPE                "float"
#define CCMC_VARIABLE_NAME_BZ_4_CLASSIFICATION            "data"
#define CCMC_VARIABLE_NAME_BZ_4_VALID_MIN                -1000000000.0
#define CCMC_VARIABLE_NAME_BZ_4_VALID_MAX                1000000000.0
#define CCMC_VARIABLE_NAME_BZ_4_UNITS                    "nT"
#define CCMC_VARIABLE_NAME_BZ_4_GRID_SYSTEM            "grid_system_5"
#define CCMC_VARIABLE_NAME_BZ_4_MASK                    FLOAT_MASK
#define CCMC_VARIABLE_NAME_BZ_4_DESCRIPTION            "Z Magnetic Field Component, Earth field up to 60000 nT, Solar field up to 3500 G in sunspots ( 3.5e8 nT ), 1 T is not exceeded in Solar/Heliospheric/Earth context"
#define CCMC_VARIABLE_NAME_BZ_4_IS_VECTOR                1
#define CCMC_VARIABLE_NAME_BZ_4_POSITION_GRID_SYSTEM    "grid_system_5"
#define CCMC_VARIABLE_NAME_BZ_4_DATA_GRID_SYSTEM        "grid_system_5"

#define CCMC_VARIABLE_NAME_B1X                        "b1x"
#define CCMC_VARIABLE_NAME_B1X_DATA_TYPE                "float"
#define    CCMC_VARIABLE_NAME_B1X_CLASSIFICATION            "data"
#define    CCMC_VARIABLE_NAME_B1X_VALID_MIN                -1000000000.0
#define    CCMC_VARIABLE_NAME_B1X_VALID_MAX                1000000000.0
#define    CCMC_VARIABLE_NAME_B1X_UNITS                    "nT"
#define    CCMC_VARIABLE_NAME_B1X_GRID_SYSTEM            "grid_system_1"
#define CCMC_VARIABLE_NAME_B1X_MASK                    FLOAT_MASK
#define    CCMC_VARIABLE_NAME_B1X_DESCRIPTION            "Deviative X Magnetic Field Component, Earth field up to 60000 nT, Solar field up to 3500 G in sunspots ( 3.5e8 nT ), 1 T is not exceeded in Solar/Heliospheric/Earth context"
#define    CCMC_VARIABLE_NAME_B1X_IS_VECTOR                1
#define    CCMC_VARIABLE_NAME_B1X_POSITION_GRID_SYSTEM    "grid_system_1"
#define    CCMC_VARIABLE_NAME_B1X_DATA_GRID_SYSTEM        "grid_system_1"

#define CCMC_VARIABLE_NAME_B1Y                        "b1y"
#define CCMC_VARIABLE_NAME_B1Y_DATA_TYPE                "float"
#define    CCMC_VARIABLE_NAME_B1Y_CLASSIFICATION            "data"
#define    CCMC_VARIABLE_NAME_B1Y_VALID_MIN                -1000000000.0
#define    CCMC_VARIABLE_NAME_B1Y_VALID_MAX                1000000000.0
#define    CCMC_VARIABLE_NAME_B1Y_UNITS                    "nT"
#define    CCMC_VARIABLE_NAME_B1Y_GRID_SYSTEM            "grid_system_1"
#define CCMC_VARIABLE_NAME_B1Y_MASK                    FLOAT_MASK
#define    CCMC_VARIABLE_NAME_B1Y_DESCRIPTION            "Deviative Y Magnetic Field Component, Earth field up to 60000 nT, Solar field up to 3500 G in sunspots ( 3.5e8 nT ), 1 T is not exceeded in Solar/Heliospheric/Earth context"
#define    CCMC_VARIABLE_NAME_B1Y_IS_VECTOR                1
#define    CCMC_VARIABLE_NAME_B1Y_POSITION_GRID_SYSTEM    "grid_system_1"
#define    CCMC_VARIABLE_NAME_B1Y_DATA_GRID_SYSTEM        "grid_system_1"

#define CCMC_VARIABLE_NAME_B1Z                        "b1z"
#define CCMC_VARIABLE_NAME_B1Z_DATA_TYPE                "float"
#define    CCMC_VARIABLE_NAME_B1Z_CLASSIFICATION            "data"
#define    CCMC_VARIABLE_NAME_B1Z_VALID_MIN                -1000000000.0
#define    CCMC_VARIABLE_NAME_B1Z_VALID_MAX                1000000000.0
#define    CCMC_VARIABLE_NAME_B1Z_UNITS                    "nT"
#define    CCMC_VARIABLE_NAME_B1Z_GRID_SYSTEM            "grid_system_1"
#define CCMC_VARIABLE_NAME_B1Z_MASK                    FLOAT_MASK
#define    CCMC_VARIABLE_NAME_B1Z_DESCRIPTION            "Deviative Z Magnetic Field Component, Earth field up to 60000 nT, Solar field up to 3500 G in sunspots ( 3.5e8 nT ), 1 T is not exceeded in Solar/Heliospheric/Earth context"
#define    CCMC_VARIABLE_NAME_B1Z_IS_VECTOR                1
#define    CCMC_VARIABLE_NAME_B1Z_POSITION_GRID_SYSTEM    "grid_system_1"
#define    CCMC_VARIABLE_NAME_B1Z_DATA_GRID_SYSTEM        "grid_system_1"

#define CCMC_VARIABLE_NAME_BX1                        "bx1"
#define CCMC_VARIABLE_NAME_BX1_DATA_TYPE                "float"
#define    CCMC_VARIABLE_NAME_BX1_CLASSIFICATION            "data"
#define    CCMC_VARIABLE_NAME_BX1_VALID_MIN                -1000000000.00
#define    CCMC_VARIABLE_NAME_BX1_VALID_MAX                1000000000.00
#define    CCMC_VARIABLE_NAME_BX1_UNITS                    "nT"
#define    CCMC_VARIABLE_NAME_BX1_GRID_SYSTEM            "grid_system_2"
#define CCMC_VARIABLE_NAME_BX1_MASK                    FLOAT_MASK
#define    CCMC_VARIABLE_NAME_BX1_DESCRIPTION            "B Defined on staggered grid [ x_bx, y_bx, z_bx ]"
#define    CCMC_VARIABLE_NAME_BX1_IS_VECTOR                1
#define    CCMC_VARIABLE_NAME_BX1_POSITION_GRID_SYSTEM    "grid_system_2"
#define    CCMC_VARIABLE_NAME_BX1_DATA_GRID_SYSTEM        "grid_system_2"

#define CCMC_VARIABLE_NAME_BY1                        "by1"
#define CCMC_VARIABLE_NAME_BY1_DATA_TYPE                "float"
#define    CCMC_VARIABLE_NAME_BY1_CLASSIFICATION            "data"
#define    CCMC_VARIABLE_NAME_BY1_VALID_MIN                -1000000000.00
#define    CCMC_VARIABLE_NAME_BY1_VALID_MAX                1000000000.00
#define    CCMC_VARIABLE_NAME_BY1_UNITS                    "nT"
#define    CCMC_VARIABLE_NAME_BY1_GRID_SYSTEM            "grid_system_3"
#define CCMC_VARIABLE_NAME_BY1_MASK                    FLOAT_MASK
#define    CCMC_VARIABLE_NAME_BY1_DESCRIPTION            "B Defined on staggered grid [ x_by, y_by, z_by ]"
#define    CCMC_VARIABLE_NAME_BY1_IS_VECTOR                1
#define    CCMC_VARIABLE_NAME_BY1_POSITION_GRID_SYSTEM    "grid_system_3"
#define    CCMC_VARIABLE_NAME_BY1_DATA_GRID_SYSTEM        "grid_system_3"

#define CCMC_VARIABLE_NAME_BZ1                        "bz1"
#define CCMC_VARIABLE_NAME_BZ1_DATA_TYPE                "float"
#define    CCMC_VARIABLE_NAME_BZ1_CLASSIFICATION            "data"
#define    CCMC_VARIABLE_NAME_BZ1_VALID_MIN                -1000000000.00
#define    CCMC_VARIABLE_NAME_BZ1_VALID_MAX                1000000000.00
#define    CCMC_VARIABLE_NAME_BZ1_UNITS                    "nT"
#define    CCMC_VARIABLE_NAME_BZ1_GRID_SYSTEM            "grid_system_4"
#define CCMC_VARIABLE_NAME_BZ1_MASK                    FLOAT_MASK
#define    CCMC_VARIABLE_NAME_BZ1_DESCRIPTION            "B Defined on staggered grid [ x_bz, y_bz, z_bz ]"
#define    CCMC_VARIABLE_NAME_BZ1_IS_VECTOR                1
#define    CCMC_VARIABLE_NAME_BZ1_POSITION_GRID_SYSTEM    "grid_system_4"
#define    CCMC_VARIABLE_NAME_BZ1_DATA_GRID_SYSTEM        "grid_system_4"

/*
 *
 *
 * scalars
 *
 *
 *
 */

#define    CCMC_VARIABLE_NAME_BP                            "bp"
#define    CCMC_VARIABLE_NAME_BP_DATA_TYPE                "float"
#define    CCMC_VARIABLE_NAME_BP_CLASSIFICATION            "data"
#define    CCMC_VARIABLE_NAME_BP_VALID_MIN                0.0
#define    CCMC_VARIABLE_NAME_BP_VALID_MAX                1000000000000.0
#define    CCMC_VARIABLE_NAME_BP_UNITS                    " "
#define    CCMC_VARIABLE_NAME_BP_GRID_SYSTEM                "grid_system_1"
#define    CCMC_VARIABLE_NAME_BP_MASK                    FLOAT_MASK
#define    CCMC_VARIABLE_NAME_BP_DESCRIPTION                "Magnetic Field Polarity"
#define    CCMC_VARIABLE_NAME_BP_IS_VECTOR                1
#define    CCMC_VARIABLE_NAME_BP_POSITION_GRID_SYSTEM        "grid_system_1"
#define    CCMC_VARIABLE_NAME_BP_DATA_GRID_SYSTEM            "grid_system_1"

#define    CCMC_VARIABLE_NAME_BP_1                        "bp_1"
#define    CCMC_VARIABLE_NAME_BP_1_DATA_TYPE                "float"
#define    CCMC_VARIABLE_NAME_BP_1_CLASSIFICATION            "data"
#define    CCMC_VARIABLE_NAME_BP_1_VALID_MIN                0.0
#define    CCMC_VARIABLE_NAME_BP_1_VALID_MAX                1000000000000.0
#define    CCMC_VARIABLE_NAME_BP_1_UNITS                    " "
#define    CCMC_VARIABLE_NAME_BP_1_GRID_SYSTEM            "grid_system_2"
#define    CCMC_VARIABLE_NAME_BP_1_MASK                    FLOAT_MASK
#define    CCMC_VARIABLE_NAME_BP_1_DESCRIPTION            "Magnetic Field Polarity"
#define    CCMC_VARIABLE_NAME_BP_1_IS_VECTOR                1
#define    CCMC_VARIABLE_NAME_BP_1_POSITION_GRID_SYSTEM    "grid_system_2"
#define    CCMC_VARIABLE_NAME_BP_1_DATA_GRID_SYSTEM        "grid_system_2"

#define    CCMC_VARIABLE_NAME_BP_2                        "bp_2"
#define    CCMC_VARIABLE_NAME_BP_2_DATA_TYPE                "float"
#define    CCMC_VARIABLE_NAME_BP_2_CLASSIFICATION            "data"
#define    CCMC_VARIABLE_NAME_BP_2_VALID_MIN                0.0
#define    CCMC_VARIABLE_NAME_BP_2_VALID_MAX                1000000000000.0
#define    CCMC_VARIABLE_NAME_BP_2_UNITS                    " "
#define    CCMC_VARIABLE_NAME_BP_2_GRID_SYSTEM            "grid_system_3"
#define    CCMC_VARIABLE_NAME_BP_2_MASK                    FLOAT_MASK
#define    CCMC_VARIABLE_NAME_BP_2_DESCRIPTION            "Magnetic Field Polarity"
#define    CCMC_VARIABLE_NAME_BP_2_IS_VECTOR                1
#define    CCMC_VARIABLE_NAME_BP_2_POSITION_GRID_SYSTEM    "grid_system_3"
#define    CCMC_VARIABLE_NAME_BP_2_DATA_GRID_SYSTEM        "grid_system_3"

#define    CCMC_VARIABLE_NAME_BP_3                        "bp_3"
#define    CCMC_VARIABLE_NAME_BP_3_DATA_TYPE                "float"
#define    CCMC_VARIABLE_NAME_BP_3_CLASSIFICATION            "data"
#define    CCMC_VARIABLE_NAME_BP_3_VALID_MIN                0.0
#define    CCMC_VARIABLE_NAME_BP_3_VALID_MAX                1000000000000.0
#define    CCMC_VARIABLE_NAME_BP_3_UNITS                    " "
#define    CCMC_VARIABLE_NAME_BP_3_GRID_SYSTEM            "grid_system_4"
#define    CCMC_VARIABLE_NAME_BP_3_MASK                    FLOAT_MASK
#define    CCMC_VARIABLE_NAME_BP_3_DESCRIPTION            "Magnetic Field Polarity"
#define    CCMC_VARIABLE_NAME_BP_3_IS_VECTOR                1
#define    CCMC_VARIABLE_NAME_BP_3_POSITION_GRID_SYSTEM    "grid_system_4"
#define    CCMC_VARIABLE_NAME_BP_3_DATA_GRID_SYSTEM        "grid_system_4"

#define    CCMC_VARIABLE_NAME_BP_4                        "bp_4"
#define    CCMC_VARIABLE_NAME_BP_4_DATA_TYPE                "float"
#define    CCMC_VARIABLE_NAME_BP_4_CLASSIFICATION            "data"
#define    CCMC_VARIABLE_NAME_BP_4_VALID_MIN                0.0
#define    CCMC_VARIABLE_NAME_BP_4_VALID_MAX                1000000000000.0
#define    CCMC_VARIABLE_NAME_BP_4_UNITS                    " "
#define    CCMC_VARIABLE_NAME_BP_4_GRID_SYSTEM            "grid_system_5"
#define    CCMC_VARIABLE_NAME_BP_4_MASK                    FLOAT_MASK
#define    CCMC_VARIABLE_NAME_BP_4_DESCRIPTION            "Magnetic Field Polarity"
#define    CCMC_VARIABLE_NAME_BP_4_IS_VECTOR                1
#define    CCMC_VARIABLE_NAME_BP_4_POSITION_GRID_SYSTEM    "grid_system_5"
#define    CCMC_VARIABLE_NAME_BP_4_DATA_GRID_SYSTEM        "grid_system_5"

#define CCMC_VARIABLE_NAME_B_L                        "b_l"
#define CCMC_VARIABLE_NAME_B_L_DATA_TYPE                "float"
#define    CCMC_VARIABLE_NAME_B_L_CLASSIFICATION            "data"
#define    CCMC_VARIABLE_NAME_B_L_VALID_MIN                -100000000
#define    CCMC_VARIABLE_NAME_B_L_VALID_MAX                100000000
#define    CCMC_VARIABLE_NAME_B_L_UNITS                    " "
#define    CCMC_VARIABLE_NAME_B_L_GRID_SYSTEM            "grid_system_1"
#define CCMC_VARIABLE_NAME_B_L_MASK                    FLOAT_MASK
#define    CCMC_VARIABLE_NAME_B_L_DESCRIPTION            "****** PROTOTYPE****** Line Of Sight Magnetic Field..."
#define    CCMC_VARIABLE_NAME_B_L_IS_VECTOR                1
#define    CCMC_VARIABLE_NAME_B_L_POSITION_GRID_SYSTEM    "grid_system_1"
#define    CCMC_VARIABLE_NAME_B_L_DATA_GRID_SYSTEM        "grid_system_1"

#define CCMC_VARIABLE_NAME_B_T                        "b_t"
#define CCMC_VARIABLE_NAME_B_T_DATA_TYPE                "float"
#define    CCMC_VARIABLE_NAME_B_T_CLASSIFICATION            "data"
#define    CCMC_VARIABLE_NAME_B_T_VALID_MIN                -100000000
#define    CCMC_VARIABLE_NAME_B_T_VALID_MAX                100000000
#define    CCMC_VARIABLE_NAME_B_T_UNITS                    " "
#define    CCMC_VARIABLE_NAME_B_T_GRID_SYSTEM            "grid_system_1"
#define CCMC_VARIABLE_NAME_B_T_MASK                    FLOAT_MASK
#define    CCMC_VARIABLE_NAME_B_T_DESCRIPTION            "****** PROTOTYPE****** Transversal Magnetic Field..."
#define    CCMC_VARIABLE_NAME_B_T_IS_VECTOR                1
#define    CCMC_VARIABLE_NAME_B_T_POSITION_GRID_SYSTEM    "grid_system_1"
#define    CCMC_VARIABLE_NAME_B_T_DATA_GRID_SYSTEM        "grid_system_1"

#define CCMC_VARIABLE_NAME_RAZ                        "raz"
#define CCMC_VARIABLE_NAME_RAZ_DATA_TYPE                "float"
#define    CCMC_VARIABLE_NAME_RAZ_CLASSIFICATION            "data"
#define    CCMC_VARIABLE_NAME_RAZ_VALID_MIN                -100000000
#define    CCMC_VARIABLE_NAME_RAZ_VALID_MAX                100000000
#define    CCMC_VARIABLE_NAME_RAZ_UNITS                    " "
#define    CCMC_VARIABLE_NAME_RAZ_GRID_SYSTEM            "grid_system_1"
#define CCMC_VARIABLE_NAME_RAZ_MASK                    FLOAT_MASK
#define    CCMC_VARIABLE_NAME_RAZ_DESCRIPTION            "****** PROTOTYPE****** "
#define    CCMC_VARIABLE_NAME_RAZ_IS_VECTOR                1
#define    CCMC_VARIABLE_NAME_RAZ_POSITION_GRID_SYSTEM    "****** PROTOTYPE****** "
#define    CCMC_VARIABLE_NAME_RAZ_DATA_GRID_SYSTEM        "****** PROTOTYPE****** "

#define CCMC_VARIABLE_NAME_NET_FLUX                        "net_flux"
#define CCMC_VARIABLE_NAME_NET_FLUX_DATA_TYPE                "float"
#define CCMC_VARIABLE_NAME_NET_FLUX_CLASSIFICATION            "data"
#define CCMC_VARIABLE_NAME_NET_FLUX_VALID_MIN                -1000000000.0
#define CCMC_VARIABLE_NAME_NET_FLUX_VALID_MAX                1000000000.0
#define CCMC_VARIABLE_NAME_NET_FLUX_UNITS                    "Gs"
#define CCMC_VARIABLE_NAME_NET_FLUX_GRID_SYSTEM            "grid_system_1"
#define CCMC_VARIABLE_NAME_NET_FLUX_MASK                    FLOAT_MASK
#define CCMC_VARIABLE_NAME_NET_FLUX_DESCRIPTION            "Net Magnetic Flux"
#define CCMC_VARIABLE_NAME_NET_FLUX_IS_VECTOR                0
#define CCMC_VARIABLE_NAME_NET_FLUX_POSITION_GRID_SYSTEM    "grid_system_1"
#define CCMC_VARIABLE_NAME_NET_FLUX_DATA_GRID_SYSTEM        "grid_system_1"

#define CCMC_VARIABLE_NAME_TOTAL_FLUX                        "total_flux"
#define CCMC_VARIABLE_NAME_TOTAL_FLUX_DATA_TYPE            "float"
#define CCMC_VARIABLE_NAME_TOTAL_FLUX_CLASSIFICATION        "data"
#define CCMC_VARIABLE_NAME_TOTAL_FLUX_VALID_MIN            -1000000000.0
#define CCMC_VARIABLE_NAME_TOTAL_FLUX_VALID_MAX            1000000000.0
#define CCMC_VARIABLE_NAME_TOTAL_FLUX_UNITS                "Gs"
#define CCMC_VARIABLE_NAME_TOTAL_FLUX_GRID_SYSTEM            "grid_system_1"
#define CCMC_VARIABLE_NAME_TOTAL_FLUX_MASK                    FLOAT_MASK
#define CCMC_VARIABLE_NAME_TOTAL_FLUX_DESCRIPTION            "Total Magnetic Flux"
#define CCMC_VARIABLE_NAME_TOTAL_FLUX_IS_VECTOR            0
#define CCMC_VARIABLE_NAME_TOTAL_FLUX_POSITION_GRID_SYSTEM    "grid_system_1"
#define CCMC_VARIABLE_NAME_TOTAL_FLUX_DATA_GRID_SYSTEM        "grid_system_1"

/*******************************************************************************************************/
/**                                        velocity                                                 **/
/*******************************************************************************************************/

/*
 *
 * vector components on r,phi, theta
 *
 */

#define CCMC_VARIABLE_NAME_UR                            "ur"
#define CCMC_VARIABLE_NAME_UR_DATA_TYPE                "float"
#define CCMC_VARIABLE_NAME_UR_CLASSIFICATION            "data"
#define CCMC_VARIABLE_NAME_UR_VALID_MIN                -300000.0
#define CCMC_VARIABLE_NAME_UR_VALID_MAX                300000.0
#define CCMC_VARIABLE_NAME_UR_UNITS                    "km/s"
#define CCMC_VARIABLE_NAME_UR_GRID_SYSTEM                "grid_system_1"
#define CCMC_VARIABLE_NAME_UR_MASK                    FLOAT_MASK
#define CCMC_VARIABLE_NAME_UR_DESCRIPTION                "r velocity Component, Speed of Light is the Maximun"
#define CCMC_VARIABLE_NAME_UR_IS_VECTOR                1
#define CCMC_VARIABLE_NAME_UR_POSITION_GRID_SYSTEM        "grid_system_1"
#define CCMC_VARIABLE_NAME_UR_DATA_GRID_SYSTEM            "grid_system_1"

#define CCMC_VARIABLE_NAME_UR_1                        "ur_1"
#define CCMC_VARIABLE_NAME_UR_1_DATA_TYPE                "float"
#define CCMC_VARIABLE_NAME_UR_1_CLASSIFICATION            "data"
#define CCMC_VARIABLE_NAME_UR_1_VALID_MIN                -300000.0
#define CCMC_VARIABLE_NAME_UR_1_VALID_MAX                300000.0
#define CCMC_VARIABLE_NAME_UR_1_UNITS                    "km/s"
#define CCMC_VARIABLE_NAME_UR_1_GRID_SYSTEM            "grid_system_2"
#define CCMC_VARIABLE_NAME_UR_1_MASK                    FLOAT_MASK
#define CCMC_VARIABLE_NAME_UR_1_DESCRIPTION            "r velocity Component, Speed of Light is the Maximun"
#define CCMC_VARIABLE_NAME_UR_1_IS_VECTOR                1
#define CCMC_VARIABLE_NAME_UR_1_POSITION_GRID_SYSTEM    "grid_system_2"
#define CCMC_VARIABLE_NAME_UR_1_DATA_GRID_SYSTEM        "grid_system_2"

#define CCMC_VARIABLE_NAME_UR_2                        "ur_2"
#define CCMC_VARIABLE_NAME_UR_2_DATA_TYPE                "float"
#define CCMC_VARIABLE_NAME_UR_2_CLASSIFICATION            "data"
#define CCMC_VARIABLE_NAME_UR_2_VALID_MIN                -300000.0
#define CCMC_VARIABLE_NAME_UR_2_VALID_MAX                300000.0
#define CCMC_VARIABLE_NAME_UR_2_UNITS                    "km/s"
#define CCMC_VARIABLE_NAME_UR_2_GRID_SYSTEM            "grid_system_3"
#define CCMC_VARIABLE_NAME_UR_2_MASK                    FLOAT_MASK
#define CCMC_VARIABLE_NAME_UR_2_DESCRIPTION            "r velocity Component, Speed of Light is the Maximun"
#define CCMC_VARIABLE_NAME_UR_2_IS_VECTOR                1
#define CCMC_VARIABLE_NAME_UR_2_POSITION_GRID_SYSTEM    "grid_system_3"
#define CCMC_VARIABLE_NAME_UR_2_DATA_GRID_SYSTEM        "grid_system_3"

#define CCMC_VARIABLE_NAME_UR_3                        "ur_3"
#define CCMC_VARIABLE_NAME_UR_3_DATA_TYPE                "float"
#define CCMC_VARIABLE_NAME_UR_3_CLASSIFICATION            "data"
#define CCMC_VARIABLE_NAME_UR_3_VALID_MIN                -300000.0
#define CCMC_VARIABLE_NAME_UR_3_VALID_MAX                300000.0
#define CCMC_VARIABLE_NAME_UR_3_UNITS                    "km/s"
#define CCMC_VARIABLE_NAME_UR_3_GRID_SYSTEM            "grid_system_4"
#define CCMC_VARIABLE_NAME_UR_3_MASK                    FLOAT_MASK
#define CCMC_VARIABLE_NAME_UR_3_DESCRIPTION            "r velocity Component, Speed of Light is the Maximun"
#define CCMC_VARIABLE_NAME_UR_3_IS_VECTOR                1
#define CCMC_VARIABLE_NAME_UR_3_POSITION_GRID_SYSTEM    "grid_system_4"
#define CCMC_VARIABLE_NAME_UR_3_DATA_GRID_SYSTEM        "grid_system_4"

#define CCMC_VARIABLE_NAME_UR_4                        "ur_4"
#define CCMC_VARIABLE_NAME_UR_4_DATA_TYPE                "float"
#define CCMC_VARIABLE_NAME_UR_4_CLASSIFICATION            "data"
#define CCMC_VARIABLE_NAME_UR_4_VALID_MIN                -300000.0
#define CCMC_VARIABLE_NAME_UR_4_VALID_MAX                300000.0
#define CCMC_VARIABLE_NAME_UR_4_UNITS                    "km/s"
#define CCMC_VARIABLE_NAME_UR_4_GRID_SYSTEM            "grid_system_5"
#define CCMC_VARIABLE_NAME_UR_4_MASK                    FLOAT_MASK
#define CCMC_VARIABLE_NAME_UR_4_DESCRIPTION            "r velocity Component, Speed of Light is the Maximun"
#define CCMC_VARIABLE_NAME_UR_4_IS_VECTOR                1
#define CCMC_VARIABLE_NAME_UR_4_POSITION_GRID_SYSTEM    "grid_system_5"
#define CCMC_VARIABLE_NAME_UR_4_DATA_GRID_SYSTEM        "grid_system_5"

#define CCMC_VARIABLE_NAME_UPHI                        "uphi"
#define CCMC_VARIABLE_NAME_UPHI_DATA_TYPE                "float"
#define CCMC_VARIABLE_NAME_UPHI_CLASSIFICATION            "data"
#define CCMC_VARIABLE_NAME_UPHI_VALID_MIN                -300000.0
#define CCMC_VARIABLE_NAME_UPHI_VALID_MAX                300000.0
#define CCMC_VARIABLE_NAME_UPHI_UNITS                    "km/s"
#define CCMC_VARIABLE_NAME_UPHI_GRID_SYSTEM            "grid_system_1"
#define CCMC_VARIABLE_NAME_UPHI_MASK                    FLOAT_MASK
#define CCMC_VARIABLE_NAME_UPHI_DESCRIPTION            "phi velocity Component, Speed of Light is the Maximun"
#define CCMC_VARIABLE_NAME_UPHI_IS_VECTOR                1
#define CCMC_VARIABLE_NAME_UPHI_POSITION_GRID_SYSTEM    "grid_system_1"
#define CCMC_VARIABLE_NAME_UPHI_DATA_GRID_SYSTEM        "grid_system_1"

#define CCMC_VARIABLE_NAME_UPHI_1                        "uphi_1"
#define CCMC_VARIABLE_NAME_UPHI_1_DATA_TYPE            "float"
#define CCMC_VARIABLE_NAME_UPHI_1_CLASSIFICATION        "data"
#define CCMC_VARIABLE_NAME_UPHI_1_VALID_MIN            -300000.0
#define CCMC_VARIABLE_NAME_UPHI_1_VALID_MAX            300000.0
#define CCMC_VARIABLE_NAME_UPHI_1_UNITS                "km/s"
#define CCMC_VARIABLE_NAME_UPHI_1_GRID_SYSTEM            "grid_system_2"
#define CCMC_VARIABLE_NAME_UPHI_1_MASK                    FLOAT_MASK
#define CCMC_VARIABLE_NAME_UPHI_1_DESCRIPTION            "phi velocity Component, Speed of Light is the Maximun"
#define CCMC_VARIABLE_NAME_UPHI_1_IS_VECTOR            1
#define CCMC_VARIABLE_NAME_UPHI_1_POSITION_GRID_SYSTEM    "grid_system_2"
#define CCMC_VARIABLE_NAME_UPHI_1_DATA_GRID_SYSTEM        "grid_system_2"

#define CCMC_VARIABLE_NAME_UPHI_2                        "uphi_2"
#define CCMC_VARIABLE_NAME_UPHI_2_DATA_TYPE            "float"
#define CCMC_VARIABLE_NAME_UPHI_2_CLASSIFICATION        "data"
#define CCMC_VARIABLE_NAME_UPHI_2_VALID_MIN            -300000.0
#define CCMC_VARIABLE_NAME_UPHI_2_VALID_MAX            300000.0
#define CCMC_VARIABLE_NAME_UPHI_2_UNITS                "km/s"
#define CCMC_VARIABLE_NAME_UPHI_2_GRID_SYSTEM            "grid_system_3"
#define CCMC_VARIABLE_NAME_UPHI_2_MASK                    FLOAT_MASK
#define CCMC_VARIABLE_NAME_UPHI_2_DESCRIPTION            "phi velocity Component, Speed of Light is the Maximun"
#define CCMC_VARIABLE_NAME_UPHI_2_IS_VECTOR            1
#define CCMC_VARIABLE_NAME_UPHI_2_POSITION_GRID_SYSTEM    "grid_system_3"
#define CCMC_VARIABLE_NAME_UPHI_2_DATA_GRID_SYSTEM        "grid_system_3"

#define CCMC_VARIABLE_NAME_UPHI_3                        "uphi_3"
#define CCMC_VARIABLE_NAME_UPHI_3_DATA_TYPE            "float"
#define CCMC_VARIABLE_NAME_UPHI_3_CLASSIFICATION        "data"
#define CCMC_VARIABLE_NAME_UPHI_3_VALID_MIN            -300000.0
#define CCMC_VARIABLE_NAME_UPHI_3_VALID_MAX            300000.0
#define CCMC_VARIABLE_NAME_UPHI_3_UNITS                "km/s"
#define CCMC_VARIABLE_NAME_UPHI_3_GRID_SYSTEM            "grid_system_4"
#define CCMC_VARIABLE_NAME_UPHI_3_MASK                    FLOAT_MASK
#define CCMC_VARIABLE_NAME_UPHI_3_DESCRIPTION            "phi velocity Component, Speed of Light is the Maximun"
#define CCMC_VARIABLE_NAME_UPHI_3_IS_VECTOR            1
#define CCMC_VARIABLE_NAME_UPHI_3_POSITION_GRID_SYSTEM    "grid_system_4"
#define CCMC_VARIABLE_NAME_UPHI_3_DATA_GRID_SYSTEM        "grid_system_4"

#define CCMC_VARIABLE_NAME_UPHI_4                        "uphi_4"
#define CCMC_VARIABLE_NAME_UPHI_4_DATA_TYPE            "float"
#define CCMC_VARIABLE_NAME_UPHI_4_CLASSIFICATION        "data"
#define CCMC_VARIABLE_NAME_UPHI_4_VALID_MIN            -300000.0
#define CCMC_VARIABLE_NAME_UPHI_4_VALID_MAX            300000.0
#define CCMC_VARIABLE_NAME_UPHI_4_UNITS                "km/s"
#define CCMC_VARIABLE_NAME_UPHI_4_GRID_SYSTEM            "grid_system_5"
#define CCMC_VARIABLE_NAME_UPHI_4_MASK                    FLOAT_MASK
#define CCMC_VARIABLE_NAME_UPHI_4_DESCRIPTION            "phi velocity Component, Speed of Light is the Maximun"
#define CCMC_VARIABLE_NAME_UPHI_4_IS_VECTOR            1
#define CCMC_VARIABLE_NAME_UPHI_4_POSITION_GRID_SYSTEM    "grid_system_5"
#define CCMC_VARIABLE_NAME_UPHI_4_DATA_GRID_SYSTEM        "grid_system_5"

#define CCMC_VARIABLE_NAME_UTHETA                        "utheta"
#define CCMC_VARIABLE_NAME_UTHETA_DATA_TYPE            "float"
#define CCMC_VARIABLE_NAME_UTHETA_CLASSIFICATION        "data"
#define CCMC_VARIABLE_NAME_UTHETA_VALID_MIN            -300000.0
#define CCMC_VARIABLE_NAME_UTHETA_VALID_MAX            300000.0
#define CCMC_VARIABLE_NAME_UTHETA_UNITS                "km/s"
#define CCMC_VARIABLE_NAME_UTHETA_GRID_SYSTEM            "grid_system_1"
#define CCMC_VARIABLE_NAME_UTHETA_MASK                    FLOAT_MASK
#define CCMC_VARIABLE_NAME_UTHETA_DESCRIPTION            "theta velocity Component, Speed of Light is the Maximun"
#define CCMC_VARIABLE_NAME_UTHETA_IS_VECTOR            1
#define CCMC_VARIABLE_NAME_UTHETA_POSITION_GRID_SYSTEM    "grid_system_1"
#define CCMC_VARIABLE_NAME_UTHETA_DATA_GRID_SYSTEM        "grid_system_1"

#define CCMC_VARIABLE_NAME_UTHETA_1                        "utheta_1"
#define CCMC_VARIABLE_NAME_UTHETA_1_DATA_TYPE                "float"
#define CCMC_VARIABLE_NAME_UTHETA_1_CLASSIFICATION            "data"
#define CCMC_VARIABLE_NAME_UTHETA_1_VALID_MIN                -300000.0
#define CCMC_VARIABLE_NAME_UTHETA_1_VALID_MAX                300000.0
#define CCMC_VARIABLE_NAME_UTHETA_1_UNITS                    "km/s"
#define CCMC_VARIABLE_NAME_UTHETA_1_GRID_SYSTEM            "grid_system_2"
#define CCMC_VARIABLE_NAME_UTHETA_1_MASK                    FLOAT_MASK
#define CCMC_VARIABLE_NAME_UTHETA_1_DESCRIPTION            "theta velocity Component, Speed of Light is the Maximun"
#define CCMC_VARIABLE_NAME_UTHETA_1_IS_VECTOR                1
#define CCMC_VARIABLE_NAME_UTHETA_1_POSITION_GRID_SYSTEM    "grid_system_2"
#define CCMC_VARIABLE_NAME_UTHETA_1_DATA_GRID_SYSTEM        "grid_system_2"

#define CCMC_VARIABLE_NAME_UTHETA_2                        "utheta_2"
#define CCMC_VARIABLE_NAME_UTHETA_2_DATA_TYPE                "float"
#define CCMC_VARIABLE_NAME_UTHETA_2_CLASSIFICATION            "data"
#define CCMC_VARIABLE_NAME_UTHETA_2_VALID_MIN                -300000.0
#define CCMC_VARIABLE_NAME_UTHETA_2_VALID_MAX                300000.0
#define CCMC_VARIABLE_NAME_UTHETA_2_UNITS                    "km/s"
#define CCMC_VARIABLE_NAME_UTHETA_2_GRID_SYSTEM            "grid_system_3"
#define CCMC_VARIABLE_NAME_UTHETA_2_MASK                    FLOAT_MASK
#define CCMC_VARIABLE_NAME_UTHETA_2_DESCRIPTION            "theta velocity Component, Speed of Light is the Maximun"
#define CCMC_VARIABLE_NAME_UTHETA_2_IS_VECTOR                1
#define CCMC_VARIABLE_NAME_UTHETA_2_POSITION_GRID_SYSTEM    "grid_system_3"
#define CCMC_VARIABLE_NAME_UTHETA_2_DATA_GRID_SYSTEM        "grid_system_3"

#define CCMC_VARIABLE_NAME_UTHETA_3                        "utheta_3"
#define CCMC_VARIABLE_NAME_UTHETA_3_DATA_TYPE                "float"
#define CCMC_VARIABLE_NAME_UTHETA_3_CLASSIFICATION            "data"
#define CCMC_VARIABLE_NAME_UTHETA_3_VALID_MIN                -300000.0
#define CCMC_VARIABLE_NAME_UTHETA_3_VALID_MAX                300000.0
#define CCMC_VARIABLE_NAME_UTHETA_3_UNITS                    "km/s"
#define CCMC_VARIABLE_NAME_UTHETA_3_GRID_SYSTEM            "grid_system_4"
#define CCMC_VARIABLE_NAME_UTHETA_3_MASK                    FLOAT_MASK
#define CCMC_VARIABLE_NAME_UTHETA_3_DESCRIPTION            "theta velocity Component, Speed of Light is the Maximun"
#define CCMC_VARIABLE_NAME_UTHETA_3_IS_VECTOR                1
#define CCMC_VARIABLE_NAME_UTHETA_3_POSITION_GRID_SYSTEM    "grid_system_4"
#define CCMC_VARIABLE_NAME_UTHETA_3_DATA_GRID_SYSTEM        "grid_system_4"

#define CCMC_VARIABLE_NAME_UTHETA_4                        "utheta_4"
#define CCMC_VARIABLE_NAME_UTHETA_4_DATA_TYPE                "float"
#define CCMC_VARIABLE_NAME_UTHETA_4_CLASSIFICATION            "data"
#define CCMC_VARIABLE_NAME_UTHETA_4_VALID_MIN                -300000.0
#define CCMC_VARIABLE_NAME_UTHETA_4_VALID_MAX                300000.0
#define CCMC_VARIABLE_NAME_UTHETA_4_UNITS                    "km/s"
#define CCMC_VARIABLE_NAME_UTHETA_4_GRID_SYSTEM            "grid_system_5"
#define CCMC_VARIABLE_NAME_UTHETA_4_MASK                    FLOAT_MASK
#define CCMC_VARIABLE_NAME_UTHETA_4_DESCRIPTION            "theta velocity Component, Speed of Light is the Maximun"
#define CCMC_VARIABLE_NAME_UTHETA_4_IS_VECTOR                1
#define CCMC_VARIABLE_NAME_UTHETA_4_POSITION_GRID_SYSTEM    "grid_system_5"
#define CCMC_VARIABLE_NAME_UTHETA_4_DATA_GRID_SYSTEM        "grid_system_5"

/*
 *
 * vector components on x,y,z
 *
 */

#define CCMC_VARIABLE_NAME_UX                            "ux"
#define CCMC_VARIABLE_NAME_UX_DATA_TYPE                "float"
#define    CCMC_VARIABLE_NAME_UX_CLASSIFICATION            "data"
#define    CCMC_VARIABLE_NAME_UX_VALID_MIN                -300000.00
#define    CCMC_VARIABLE_NAME_UX_VALID_MAX                300000.00
#define    CCMC_VARIABLE_NAME_UX_UNITS                    "km/s"
#define    CCMC_VARIABLE_NAME_UX_GRID_SYSTEM                "grid_system_1"
#define CCMC_VARIABLE_NAME_UX_MASK                    FLOAT_MASK
#define    CCMC_VARIABLE_NAME_UX_DESCRIPTION                "X Velocity Component, Speed of Light is the Maximun"
#define    CCMC_VARIABLE_NAME_UX_IS_VECTOR                1
#define    CCMC_VARIABLE_NAME_UX_POSITION_GRID_SYSTEM        "grid_system_1"
#define    CCMC_VARIABLE_NAME_UX_DATA_GRID_SYSTEM            "grid_system_1"

#define CCMC_VARIABLE_NAME_UX_1                        "ux_1"
#define CCMC_VARIABLE_NAME_UX_1_DATA_TYPE                "float"
#define CCMC_VARIABLE_NAME_UX_1_CLASSIFICATION            "data"
#define CCMC_VARIABLE_NAME_UX_1_VALID_MIN                -300000.0
#define CCMC_VARIABLE_NAME_UX_1_VALID_MAX                300000.0
#define CCMC_VARIABLE_NAME_UX_1_UNITS                    "km/s"
#define CCMC_VARIABLE_NAME_UX_1_GRID_SYSTEM            "grid_system_2"
#define CCMC_VARIABLE_NAME_UX_1_MASK                    FLOAT_MASK
#define CCMC_VARIABLE_NAME_UX_1_DESCRIPTION            "X Velocity Component, Speed of Light is the Maximun"
#define CCMC_VARIABLE_NAME_UX_1_IS_VECTOR                1
#define CCMC_VARIABLE_NAME_UX_1_POSITION_GRID_SYSTEM    "grid_system_2"
#define CCMC_VARIABLE_NAME_UX_1_DATA_GRID_SYSTEM        "grid_system_2"

#define CCMC_VARIABLE_NAME_UX_2                        "ux_2"
#define CCMC_VARIABLE_NAME_UX_2_DATA_TYPE                "float"
#define CCMC_VARIABLE_NAME_UX_2_CLASSIFICATION            "data"
#define CCMC_VARIABLE_NAME_UX_2_VALID_MIN                -300000.0
#define CCMC_VARIABLE_NAME_UX_2_VALID_MAX                300000.0
#define CCMC_VARIABLE_NAME_UX_2_UNITS                    "km/s"
#define CCMC_VARIABLE_NAME_UX_2_GRID_SYSTEM            "grid_system_3"
#define CCMC_VARIABLE_NAME_UX_2_MASK                    FLOAT_MASK
#define CCMC_VARIABLE_NAME_UX_2_DESCRIPTION            "X Velocity Component, Speed of Light is the Maximun"
#define CCMC_VARIABLE_NAME_UX_2_IS_VECTOR                1
#define CCMC_VARIABLE_NAME_UX_2_POSITION_GRID_SYSTEM    "grid_system_3"
#define CCMC_VARIABLE_NAME_UX_2_DATA_GRID_SYSTEM        "grid_system_3"

#define CCMC_VARIABLE_NAME_UX_3                        "ux_3"
#define CCMC_VARIABLE_NAME_UX_3_DATA_TYPE                "float"
#define CCMC_VARIABLE_NAME_UX_3_CLASSIFICATION            "data"
#define CCMC_VARIABLE_NAME_UX_3_VALID_MIN                -300000.0
#define CCMC_VARIABLE_NAME_UX_3_VALID_MAX                300000.0
#define CCMC_VARIABLE_NAME_UX_3_UNITS                    "km/s"
#define CCMC_VARIABLE_NAME_UX_3_GRID_SYSTEM            "grid_system_4"
#define CCMC_VARIABLE_NAME_UX_3_MASK                    FLOAT_MASK
#define CCMC_VARIABLE_NAME_UX_3_DESCRIPTION            "X Velocity Component, Speed of Light is the Maximun"
#define CCMC_VARIABLE_NAME_UX_3_IS_VECTOR                1
#define CCMC_VARIABLE_NAME_UX_3_POSITION_GRID_SYSTEM    "grid_system_4"
#define CCMC_VARIABLE_NAME_UX_3_DATA_GRID_SYSTEM        "grid_system_4"

#define CCMC_VARIABLE_NAME_UX_4                        "ux_4"
#define CCMC_VARIABLE_NAME_UX_4_DATA_TYPE                "float"
#define CCMC_VARIABLE_NAME_UX_4_CLASSIFICATION            "data"
#define CCMC_VARIABLE_NAME_UX_4_VALID_MIN                -300000.0
#define CCMC_VARIABLE_NAME_UX_4_VALID_MAX                300000.0
#define CCMC_VARIABLE_NAME_UX_4_UNITS                    "km/s"
#define CCMC_VARIABLE_NAME_UX_4_GRID_SYSTEM            "grid_system_5"
#define CCMC_VARIABLE_NAME_UX_4_MASK                    FLOAT_MASK
#define CCMC_VARIABLE_NAME_UX_4_DESCRIPTION            "X Velocity Component, Speed of Light is the Maximun"
#define CCMC_VARIABLE_NAME_UX_4_IS_VECTOR                1
#define CCMC_VARIABLE_NAME_UX_4_POSITION_GRID_SYSTEM    "grid_system_5"
#define CCMC_VARIABLE_NAME_UX_4_DATA_GRID_SYSTEM        "grid_system_5"

#define CCMC_VARIABLE_NAME_UY                            "uy"
#define CCMC_VARIABLE_NAME_UY_DATA_TYPE                "float"
#define    CCMC_VARIABLE_NAME_UY_CLASSIFICATION            "data"
#define    CCMC_VARIABLE_NAME_UY_VALID_MIN                -300000.00
#define    CCMC_VARIABLE_NAME_UY_VALID_MAX                300000.00
#define    CCMC_VARIABLE_NAME_UY_UNITS                    "km/s"
#define    CCMC_VARIABLE_NAME_UY_GRID_SYSTEM                "grid_system_1"
#define CCMC_VARIABLE_NAME_UY_MASK                    FLOAT_MASK
#define    CCMC_VARIABLE_NAME_UY_DESCRIPTION                "Y Velocity Component, Speed of Light is the Maximun"
#define    CCMC_VARIABLE_NAME_UY_IS_VECTOR                1
#define    CCMC_VARIABLE_NAME_UY_POSITION_GRID_SYSTEM        "grid_system_1"
#define    CCMC_VARIABLE_NAME_UY_DATA_GRID_SYSTEM            "grid_system_1"

#define CCMC_VARIABLE_NAME_UY_1                        "uy_1"
#define CCMC_VARIABLE_NAME_UY_1_DATA_TYPE                "float"
#define CCMC_VARIABLE_NAME_UY_1_CLASSIFICATION            "data"
#define CCMC_VARIABLE_NAME_UY_1_VALID_MIN                -300000.0
#define CCMC_VARIABLE_NAME_UY_1_VALID_MAX                300000.0
#define CCMC_VARIABLE_NAME_UY_1_UNITS                    "km/s"
#define CCMC_VARIABLE_NAME_UY_1_GRID_SYSTEM            "grid_system_2"
#define CCMC_VARIABLE_NAME_UY_1_MASK                    FLOAT_MASK
#define CCMC_VARIABLE_NAME_UY_1_DESCRIPTION            "Y Velocity Component, Speed of Light is the Maximun"
#define CCMC_VARIABLE_NAME_UY_1_IS_VECTOR                1
#define CCMC_VARIABLE_NAME_UY_1_POSITION_GRID_SYSTEM    "grid_system_2"
#define CCMC_VARIABLE_NAME_UY_1_DATA_GRID_SYSTEM        "grid_system_2"

#define CCMC_VARIABLE_NAME_UY_2                        "uy_2"
#define CCMC_VARIABLE_NAME_UY_2_DATA_TYPE                "float"
#define CCMC_VARIABLE_NAME_UY_2_CLASSIFICATION            "data"
#define CCMC_VARIABLE_NAME_UY_2_VALID_MIN                -300000.0
#define CCMC_VARIABLE_NAME_UY_2_VALID_MAX                300000.0
#define CCMC_VARIABLE_NAME_UY_2_UNITS                    "km/s"
#define CCMC_VARIABLE_NAME_UY_2_GRID_SYSTEM            "grid_system_3"
#define CCMC_VARIABLE_NAME_UY_2_MASK                    FLOAT_MASK
#define CCMC_VARIABLE_NAME_UY_2_DESCRIPTION            "Y Velocity Component, Speed of Light is the Maximun"
#define CCMC_VARIABLE_NAME_UY_2_IS_VECTOR                1
#define CCMC_VARIABLE_NAME_UY_2_POSITION_GRID_SYSTEM    "grid_system_3"
#define CCMC_VARIABLE_NAME_UY_2_DATA_GRID_SYSTEM        "grid_system_3"

#define CCMC_VARIABLE_NAME_UY_3                        "uy_3"
#define CCMC_VARIABLE_NAME_UY_3_DATA_TYPE                "float"
#define CCMC_VARIABLE_NAME_UY_3_CLASSIFICATION            "data"
#define CCMC_VARIABLE_NAME_UY_3_VALID_MIN                -300000.0
#define CCMC_VARIABLE_NAME_UY_3_VALID_MAX                300000.0
#define CCMC_VARIABLE_NAME_UY_3_UNITS                    "km/s"
#define CCMC_VARIABLE_NAME_UY_3_GRID_SYSTEM            "grid_system_4"
#define CCMC_VARIABLE_NAME_UY_3_MASK                    FLOAT_MASK
#define CCMC_VARIABLE_NAME_UY_3_DESCRIPTION            "Y Velocity Component, Speed of Light is the Maximun"
#define CCMC_VARIABLE_NAME_UY_3_IS_VECTOR                1
#define CCMC_VARIABLE_NAME_UY_3_POSITION_GRID_SYSTEM    "grid_system_4"
#define CCMC_VARIABLE_NAME_UY_3_DATA_GRID_SYSTEM        "grid_system_4"

#define CCMC_VARIABLE_NAME_UY_4                        "uy_4"
#define CCMC_VARIABLE_NAME_UY_4_DATA_TYPE                "float"
#define CCMC_VARIABLE_NAME_UY_4_CLASSIFICATION            "data"
#define CCMC_VARIABLE_NAME_UY_4_VALID_MIN                -300000.0
#define CCMC_VARIABLE_NAME_UY_4_VALID_MAX                300000.0
#define CCMC_VARIABLE_NAME_UY_4_UNITS                    "km/s"
#define CCMC_VARIABLE_NAME_UY_4_GRID_SYSTEM            "grid_system_5"
#define CCMC_VARIABLE_NAME_UY_4_MASK                    FLOAT_MASK
#define CCMC_VARIABLE_NAME_UY_4_DESCRIPTION            "Y Velocity Component, Speed of Light is the Maximun"
#define CCMC_VARIABLE_NAME_UY_4_IS_VECTOR                1
#define CCMC_VARIABLE_NAME_UY_4_POSITION_GRID_SYSTEM    "grid_system_5"
#define CCMC_VARIABLE_NAME_UY_4_DATA_GRID_SYSTEM        "grid_system_5"

#define CCMC_VARIABLE_NAME_UZ                            "uz"
#define CCMC_VARIABLE_NAME_UZ_DATA_TYPE                "float"
#define    CCMC_VARIABLE_NAME_UZ_CLASSIFICATION            "data"
#define    CCMC_VARIABLE_NAME_UZ_VALID_MIN                -300000.00
#define    CCMC_VARIABLE_NAME_UZ_VALID_MAX                300000.00
#define    CCMC_VARIABLE_NAME_UZ_UNITS                    "km/s"
#define    CCMC_VARIABLE_NAME_UZ_GRID_SYSTEM                "grid_system_1"
#define CCMC_VARIABLE_NAME_UZ_MASK                    FLOAT_MASK
#define    CCMC_VARIABLE_NAME_UZ_DESCRIPTION                "Z Velocity Component, Speed of Light is the Maximun"
#define    CCMC_VARIABLE_NAME_UZ_IS_VECTOR                1
#define    CCMC_VARIABLE_NAME_UZ_POSITION_GRID_SYSTEM        "grid_system_1"
#define    CCMC_VARIABLE_NAME_UZ_DATA_GRID_SYSTEM            "grid_system_1"

#define CCMC_VARIABLE_NAME_UZ_1                        "uz_1"
#define CCMC_VARIABLE_NAME_UZ_1_DATA_TYPE                "float"
#define CCMC_VARIABLE_NAME_UZ_1_CLASSIFICATION            "data"
#define CCMC_VARIABLE_NAME_UZ_1_VALID_MIN                -300000.0
#define CCMC_VARIABLE_NAME_UZ_1_VALID_MAX                300000.0
#define CCMC_VARIABLE_NAME_UZ_1_UNITS                    "km/s"
#define CCMC_VARIABLE_NAME_UZ_1_GRID_SYSTEM            "grid_system_2"
#define CCMC_VARIABLE_NAME_UZ_1_MASK                    FLOAT_MASK
#define CCMC_VARIABLE_NAME_UZ_1_DESCRIPTION            "Z Velocity Component, Speed of Light is the Maximun"
#define CCMC_VARIABLE_NAME_UZ_1_IS_VECTOR                1
#define CCMC_VARIABLE_NAME_UZ_1_POSITION_GRID_SYSTEM    "grid_system_2"
#define CCMC_VARIABLE_NAME_UZ_1_DATA_GRID_SYSTEM        "grid_system_2"

#define CCMC_VARIABLE_NAME_UZ_2                        "uz_2"
#define CCMC_VARIABLE_NAME_UZ_2_DATA_TYPE                "float"
#define CCMC_VARIABLE_NAME_UZ_2_CLASSIFICATION            "data"
#define CCMC_VARIABLE_NAME_UZ_2_VALID_MIN                -300000.0
#define CCMC_VARIABLE_NAME_UZ_2_VALID_MAX                300000.0
#define CCMC_VARIABLE_NAME_UZ_2_UNITS                    "km/s"
#define CCMC_VARIABLE_NAME_UZ_2_GRID_SYSTEM            "grid_system_3"
#define CCMC_VARIABLE_NAME_UZ_2_MASK                    FLOAT_MASK
#define CCMC_VARIABLE_NAME_UZ_2_DESCRIPTION            "Z Velocity Component, Speed of Light is the Maximun"
#define CCMC_VARIABLE_NAME_UZ_2_IS_VECTOR                1
#define CCMC_VARIABLE_NAME_UZ_2_POSITION_GRID_SYSTEM    "grid_system_3"
#define CCMC_VARIABLE_NAME_UZ_2_DATA_GRID_SYSTEM        "grid_system_3"

#define CCMC_VARIABLE_NAME_UZ_3                        "uz_3"
#define CCMC_VARIABLE_NAME_UZ_3_DATA_TYPE                "float"
#define CCMC_VARIABLE_NAME_UZ_3_CLASSIFICATION            "data"
#define CCMC_VARIABLE_NAME_UZ_3_VALID_MIN                -300000.0
#define CCMC_VARIABLE_NAME_UZ_3_VALID_MAX                300000.0
#define CCMC_VARIABLE_NAME_UZ_3_UNITS                    "km/s"
#define CCMC_VARIABLE_NAME_UZ_3_GRID_SYSTEM            "grid_system_4"
#define CCMC_VARIABLE_NAME_UZ_3_MASK                    FLOAT_MASK
#define CCMC_VARIABLE_NAME_UZ_3_DESCRIPTION            "Z Velocity Component, Speed of Light is the Maximun"
#define CCMC_VARIABLE_NAME_UZ_3_IS_VECTOR                1
#define CCMC_VARIABLE_NAME_UZ_3_POSITION_GRID_SYSTEM    "grid_system_4"
#define CCMC_VARIABLE_NAME_UZ_3_DATA_GRID_SYSTEM        "grid_system_4"

#define CCMC_VARIABLE_NAME_UZ_4                        "uz_4"
#define CCMC_VARIABLE_NAME_UZ_4_DATA_TYPE                "float"
#define CCMC_VARIABLE_NAME_UZ_4_CLASSIFICATION            "data"
#define CCMC_VARIABLE_NAME_UZ_4_VALID_MIN                -300000.0
#define CCMC_VARIABLE_NAME_UZ_4_VALID_MAX                300000.0
#define CCMC_VARIABLE_NAME_UZ_4_UNITS                    "km/s"
#define CCMC_VARIABLE_NAME_UZ_4_GRID_SYSTEM            "grid_system_5"
#define CCMC_VARIABLE_NAME_UZ_4_MASK                    FLOAT_MASK
#define CCMC_VARIABLE_NAME_UZ_4_DESCRIPTION            "Z Velocity Component, Speed of Light is the Maximun"
#define CCMC_VARIABLE_NAME_UZ_4_IS_VECTOR                1
#define CCMC_VARIABLE_NAME_UZ_4_POSITION_GRID_SYSTEM    "grid_system_5"
#define CCMC_VARIABLE_NAME_UZ_4_DATA_GRID_SYSTEM        "grid_system_5"

#define    CCMC_VARIABLE_NAME_Vn_X                        "Vn_x"
#define    CCMC_VARIABLE_NAME_Vn_X_DATA_TYPE                "float"
#define    CCMC_VARIABLE_NAME_Vn_X_CLASSIFICATION            "data"
#define    CCMC_VARIABLE_NAME_Vn_X_VALID_MIN                -100000.0
#define    CCMC_VARIABLE_NAME_Vn_X_VALID_MAX                100000.0
#define    CCMC_VARIABLE_NAME_Vn_X_UNITS                    "m/s"
#define    CCMC_VARIABLE_NAME_Vn_X_GRID_SYSTEM            "grid_system_1"
#define CCMC_VARIABLE_NAME_Vn_X_MASK                    FLOAT_MASK
#define    CCMC_VARIABLE_NAME_Vn_X_DESCRIPTION            "Neutral Velocity - latitudinal / meridian component"
#define    CCMC_VARIABLE_NAME_Vn_X_IS_VECTOR                1
#define    CCMC_VARIABLE_NAME_Vn_X_POSITION_GRID_SYSTEM    "grid_system_1"
#define    CCMC_VARIABLE_NAME_Vn_X_DATA_GRID_SYSTEM        "grid_system_1"

#define CCMC_VARIABLE_NAME_Vn_Y                        "Vn_y"
#define CCMC_VARIABLE_NAME_Vn_Y_DATA_TYPE                "float"
#define    CCMC_VARIABLE_NAME_Vn_Y_CLASSIFICATION            "data"
#define    CCMC_VARIABLE_NAME_Vn_Y_VALID_MIN                -1000000.0
#define    CCMC_VARIABLE_NAME_Vn_Y_VALID_MAX                1000000.0
#define    CCMC_VARIABLE_NAME_Vn_Y_UNITS                    "m/s"
#define    CCMC_VARIABLE_NAME_Vn_Y_GRID_SYSTEM            "grid_system_1"
#define    CCMC_VARIABLE_NAME_Vn_Y_MASK                    FLOAT_MASK
#define    CCMC_VARIABLE_NAME_Vn_Y_DESCRIPTION            "Neutral Velocity - longitudinal / zonal component"
#define    CCMC_VARIABLE_NAME_Vn_Y_IS_VECTOR                1
#define    CCMC_VARIABLE_NAME_Vn_Y_POSITION_GRID_SYSTEM    "grid_system_1"
#define    CCMC_VARIABLE_NAME_Vn_Y_DATA_GRID_SYSTEM        "grid_system_1"

#define CCMC_VARIABLE_NAME_Vn_Z                        "Vn_z"
#define CCMC_VARIABLE_NAME_Vn_Z_DATA_TYPE                "float"
#define    CCMC_VARIABLE_NAME_Vn_Z_CLASSIFICATION            "data"
#define    CCMC_VARIABLE_NAME_Vn_Z_VALID_MIN                -1000000.0
#define    CCMC_VARIABLE_NAME_Vn_Z_VALID_MAX                1000000.0
#define    CCMC_VARIABLE_NAME_Vn_Z_UNITS                    "m/s"
#define    CCMC_VARIABLE_NAME_Vn_Z_GRID_SYSTEM            "grid_system_1"
#define    CCMC_VARIABLE_NAME_Vn_Z_MASK                    FLOAT_MASK
#define    CCMC_VARIABLE_NAME_Vn_Z_DESCRIPTION            "Neutral Velocity - vertical direction"
#define    CCMC_VARIABLE_NAME_Vn_Z_IS_VECTOR                1
#define    CCMC_VARIABLE_NAME_Vn_Z_POSITION_GRID_SYSTEM    "grid_system_1"
#define    CCMC_VARIABLE_NAME_Vn_Z_DATA_GRID_SYSTEM        "grid_system_1"

#define    CCMC_VARIABLE_NAME_Vi_X                        "Vi_x"
#define    CCMC_VARIABLE_NAME_Vi_X_DATA_TYPE                "float"
#define    CCMC_VARIABLE_NAME_Vi_X_CLASSIFICATION            "data"
#define    CCMC_VARIABLE_NAME_Vi_X_VALID_MIN                -100000.0
#define    CCMC_VARIABLE_NAME_Vi_X_VALID_MAX                100000.0
#define    CCMC_VARIABLE_NAME_Vi_X_UNITS                    "m/s"
#define    CCMC_VARIABLE_NAME_Vi_X_GRID_SYSTEM            "grid_system_1"
#define CCMC_VARIABLE_NAME_Vi_X_MASK                    FLOAT_MASK
#define    CCMC_VARIABLE_NAME_Vi_X_DESCRIPTION            "Plasma Velocity - latitudinal / meridian component"
#define    CCMC_VARIABLE_NAME_Vi_X_IS_VECTOR                1
#define    CCMC_VARIABLE_NAME_Vi_X_POSITION_GRID_SYSTEM    "grid_system_1"
#define    CCMC_VARIABLE_NAME_Vi_X_DATA_GRID_SYSTEM        "grid_system_1"

#define CCMC_VARIABLE_NAME_Vi_Y                        "Vi_y"
#define CCMC_VARIABLE_NAME_Vi_Y_DATA_TYPE                "float"
#define    CCMC_VARIABLE_NAME_Vi_Y_CLASSIFICATION            "data"
#define    CCMC_VARIABLE_NAME_Vi_Y_VALID_MIN                -1000000.0
#define    CCMC_VARIABLE_NAME_Vi_Y_VALID_MAX                1000000.0
#define    CCMC_VARIABLE_NAME_Vi_Y_UNITS                    "m/s"
#define    CCMC_VARIABLE_NAME_Vi_Y_GRID_SYSTEM            "grid_system_1"
#define    CCMC_VARIABLE_NAME_Vi_Y_MASK                    FLOAT_MASK
#define    CCMC_VARIABLE_NAME_Vi_Y_DESCRIPTION            "Plasma Velocity - longitudinal / zonal component"
#define    CCMC_VARIABLE_NAME_Vi_Y_IS_VECTOR                1
#define    CCMC_VARIABLE_NAME_Vi_Y_POSITION_GRID_SYSTEM    "grid_system_1"
#define    CCMC_VARIABLE_NAME_Vi_Y_DATA_GRID_SYSTEM        "grid_system_1"

/*******************************************************************************************************/
/**                                        current                                                     **/
/*******************************************************************************************************/

#define CCMC_VARIABLE_NAME_JR                            "jr"
#define CCMC_VARIABLE_NAME_JR_DATA_TYPE                "float"
#define CCMC_VARIABLE_NAME_JR_CLASSIFICATION            "data"
#define CCMC_VARIABLE_NAME_JR_VALID_MIN                -1000000000.0
#define CCMC_VARIABLE_NAME_JR_VALID_MAX                1000000000.0
#define CCMC_VARIABLE_NAME_JR_UNITS                    "muA/m^2"
#define CCMC_VARIABLE_NAME_JR_GRID_SYSTEM                "grid_system_1"
#define CCMC_VARIABLE_NAME_JR_MASK                    FLOAT_MASK
#define CCMC_VARIABLE_NAME_JR_DESCRIPTION                "r current Component, Speed of Light is the Maximun"
#define CCMC_VARIABLE_NAME_JR_IS_VECTOR                1
#define CCMC_VARIABLE_NAME_JR_POSITION_GRID_SYSTEM        "grid_system_1"
#define CCMC_VARIABLE_NAME_JR_DATA_GRID_SYSTEM            "grid_system_1"

#define CCMC_VARIABLE_NAME_JPHI                        "jphi"
#define CCMC_VARIABLE_NAME_JPHI_DATA_TYPE                "float"
#define CCMC_VARIABLE_NAME_JPHI_CLASSIFICATION            "data"
#define CCMC_VARIABLE_NAME_JPHI_VALID_MIN                -1000000000.0
#define CCMC_VARIABLE_NAME_JPHI_VALID_MAX                1000000000.0
#define CCMC_VARIABLE_NAME_JPHI_UNITS                    "muA/m^2"
#define CCMC_VARIABLE_NAME_JPHI_GRID_SYSTEM            "grid_system_1"
#define CCMC_VARIABLE_NAME_JPHI_MASK                    FLOAT_MASK
#define CCMC_VARIABLE_NAME_JPHI_DESCRIPTION            "phi current Component"
#define CCMC_VARIABLE_NAME_JPHI_IS_VECTOR                1
#define CCMC_VARIABLE_NAME_JPHI_POSITION_GRID_SYSTEM    "grid_system_1"
#define CCMC_VARIABLE_NAME_JPHI_DATA_GRID_SYSTEM        "grid_system_1"

#define CCMC_VARIABLE_NAME_JTHETA                        "jtheta"
#define CCMC_VARIABLE_NAME_JTHETA_DATA_TYPE            "float"
#define CCMC_VARIABLE_NAME_JTHETA_CLASSIFICATION        "data"
#define CCMC_VARIABLE_NAME_JTHETA_VALID_MIN            -1000000000.0
#define CCMC_VARIABLE_NAME_JTHETA_VALID_MAX            1000000000.0
#define CCMC_VARIABLE_NAME_JTHETA_UNITS                "muA/m^2"
#define CCMC_VARIABLE_NAME_JTHETA_GRID_SYSTEM            "grid_system_1"
#define CCMC_VARIABLE_NAME_JTHETA_MASK                    FLOAT_MASK
#define CCMC_VARIABLE_NAME_JTHETA_DESCRIPTION            "theta current Component"
#define CCMC_VARIABLE_NAME_JTHETA_IS_VECTOR            1
#define CCMC_VARIABLE_NAME_JTHETA_POSITION_GRID_SYSTEM    "grid_system_1"
#define CCMC_VARIABLE_NAME_JTHETA_DATA_GRID_SYSTEM        "grid_system_1"

#define CCMC_VARIABLE_NAME_JX                            "jx"
#define CCMC_VARIABLE_NAME_JX_DATA_TYPE                "float"
#define    CCMC_VARIABLE_NAME_JX_CLASSIFICATION            "data"
#define    CCMC_VARIABLE_NAME_JX_VALID_MIN                -1000000000.0
#define    CCMC_VARIABLE_NAME_JX_VALID_MAX                1000000000.0
#define    CCMC_VARIABLE_NAME_JX_UNITS                    "muA/m^2"
#define    CCMC_VARIABLE_NAME_JX_GRID_SYSTEM                "grid_system_1"
#define CCMC_VARIABLE_NAME_JX_MASK                    FLOAT_MASK
#define    CCMC_VARIABLE_NAME_JX_DESCRIPTION                "X Current Component"
#define    CCMC_VARIABLE_NAME_JX_IS_VECTOR                1
#define    CCMC_VARIABLE_NAME_JX_POSITION_GRID_SYSTEM        "grid_system_1"
#define    CCMC_VARIABLE_NAME_JX_DATA_GRID_SYSTEM            "grid_system_1"

#define CCMC_VARIABLE_NAME_JY                            "jy"
#define CCMC_VARIABLE_NAME_JY_DATA_TYPE                "float"
#define    CCMC_VARIABLE_NAME_JY_CLASSIFICATION            "data"
#define    CCMC_VARIABLE_NAME_JY_VALID_MIN                -1000000000.0
#define    CCMC_VARIABLE_NAME_JY_VALID_MAX                1000000000.0
#define    CCMC_VARIABLE_NAME_JY_UNITS                    "muA/m^2"
#define    CCMC_VARIABLE_NAME_JY_GRID_SYSTEM                "grid_system_1"
#define CCMC_VARIABLE_NAME_JY_MASK                    FLOAT_MASK
#define    CCMC_VARIABLE_NAME_JY_DESCRIPTION                "Y Current Component"
#define    CCMC_VARIABLE_NAME_JY_IS_VECTOR                1
#define    CCMC_VARIABLE_NAME_JY_POSITION_GRID_SYSTEM        "grid_system_1"
#define    CCMC_VARIABLE_NAME_JY_DATA_GRID_SYSTEM            "grid_system_1"

#define CCMC_VARIABLE_NAME_JZ                            "jz"
#define CCMC_VARIABLE_NAME_JZ_DATA_TYPE                "float"
#define    CCMC_VARIABLE_NAME_JZ_CLASSIFICATION            "data"
#define    CCMC_VARIABLE_NAME_JZ_VALID_MIN                -1000000000.0
#define    CCMC_VARIABLE_NAME_JZ_VALID_MAX                1000000000.0
#define    CCMC_VARIABLE_NAME_JZ_UNITS                    "muA/m^2"
#define    CCMC_VARIABLE_NAME_JZ_GRID_SYSTEM                "grid_system_1"
#define CCMC_VARIABLE_NAME_JZ_MASK                    FLOAT_MASK
#define    CCMC_VARIABLE_NAME_JZ_DESCRIPTION                "Z Current Component"
#define    CCMC_VARIABLE_NAME_JZ_IS_VECTOR                1
#define    CCMC_VARIABLE_NAME_JZ_POSITION_GRID_SYSTEM        "grid_system_1"
#define    CCMC_VARIABLE_NAME_JZ_DATA_GRID_SYSTEM            "grid_system_1"

/*******************************************************************************************************/
/**                                        resistivity                                                **/
/*******************************************************************************************************/

#define CCMC_VARIABLE_NAME_RESIS                        "eta"
#define CCMC_VARIABLE_NAME_RESIS_DATA_TYPE                "float"
#define    CCMC_VARIABLE_NAME_RESIS_CLASSIFICATION            "data"
#define    CCMC_VARIABLE_NAME_RESIS_VALID_MIN                0.0
#define    CCMC_VARIABLE_NAME_RESIS_VALID_MAX                1000000000000.0
#define    CCMC_VARIABLE_NAME_RESIS_UNITS                    "Vm/A"
#define    CCMC_VARIABLE_NAME_RESIS_GRID_SYSTEM            "grid_system_1"
#define CCMC_VARIABLE_NAME_RESIS_MASK                    FLOAT_MASK
#define    CCMC_VARIABLE_NAME_RESIS_DESCRIPTION            "resistivity"
#define    CCMC_VARIABLE_NAME_RESIS_IS_VECTOR                0
#define    CCMC_VARIABLE_NAME_RESIS_POSITION_GRID_SYSTEM    "grid_system_1"
#define    CCMC_VARIABLE_NAME_RESIS_DATA_GRID_SYSTEM        "grid_system_1"

/*******************************************************************************************************/
/**                                        density                                                     **/
/*******************************************************************************************************/

#define CCMC_VARIABLE_NAME_RHO                        "rho"
#define CCMC_VARIABLE_NAME_RHO_DATA_TYPE                "float"
#define    CCMC_VARIABLE_NAME_RHO_CLASSIFICATION            "data"
#define    CCMC_VARIABLE_NAME_RHO_VALID_MIN                0.0
#define    CCMC_VARIABLE_NAME_RHO_VALID_MAX                1000000000000.0
#define    CCMC_VARIABLE_NAME_RHO_UNITS                    "amu/cm^3"
#define    CCMC_VARIABLE_NAME_RHO_GRID_SYSTEM            "grid_system_1"
#define CCMC_VARIABLE_NAME_RHO_MASK                    FLOAT_MASK
#define    CCMC_VARIABLE_NAME_RHO_DESCRIPTION            "atomic mass density, limit may bee exceeded in dense atmosphere; solar corona 2e8"
#define    CCMC_VARIABLE_NAME_RHO_IS_VECTOR                0
#define    CCMC_VARIABLE_NAME_RHO_POSITION_GRID_SYSTEM    "grid_system_1"
#define    CCMC_VARIABLE_NAME_RHO_DATA_GRID_SYSTEM        "grid_system_1"

#define    CCMC_VARIABLE_NAME_RHO_1                        "rho_1"
#define    CCMC_VARIABLE_NAME_RHO_1_DATA_TYPE            "float"
#define    CCMC_VARIABLE_NAME_RHO_1_CLASSIFICATION        "data"
#define    CCMC_VARIABLE_NAME_RHO_1_VALID_MIN            0.0
#define    CCMC_VARIABLE_NAME_RHO_1_VALID_MAX            1000000000000.0
#define    CCMC_VARIABLE_NAME_RHO_1_UNITS                "amu/cm^3"
#define    CCMC_VARIABLE_NAME_RHO_1_GRID_SYSTEM            "grid_system_2"
#define    CCMC_VARIABLE_NAME_RHO_1_MASK                    FLOAT_MASK
#define    CCMC_VARIABLE_NAME_RHO_1_DESCRIPTION            "atomic mass density, limit may bee exceeded in dense atmosphere; solar corona 2e8"
#define    CCMC_VARIABLE_NAME_RHO_1_IS_VECTOR            0
#define    CCMC_VARIABLE_NAME_RHO_1_POSITION_GRID_SYSTEM    "grid_system_2"
#define    CCMC_VARIABLE_NAME_RHO_1_DATA_GRID_SYSTEM        "grid_system_2"

#define    CCMC_VARIABLE_NAME_RHO_2                        "rho_2"
#define    CCMC_VARIABLE_NAME_RHO_2_DATA_TYPE            "float"
#define    CCMC_VARIABLE_NAME_RHO_2_CLASSIFICATION        "data"
#define    CCMC_VARIABLE_NAME_RHO_2_VALID_MIN            0.0
#define    CCMC_VARIABLE_NAME_RHO_2_VALID_MAX            1000000000000.0
#define    CCMC_VARIABLE_NAME_RHO_2_UNITS                "amu/cm^3"
#define    CCMC_VARIABLE_NAME_RHO_2_GRID_SYSTEM            "grid_system_3"
#define    CCMC_VARIABLE_NAME_RHO_2_MASK                    FLOAT_MASK
#define    CCMC_VARIABLE_NAME_RHO_2_DESCRIPTION            "atomic mass density, limit may bee exceeded in dense atmosphere; solar corona 2e8"
#define    CCMC_VARIABLE_NAME_RHO_2_IS_VECTOR            0
#define    CCMC_VARIABLE_NAME_RHO_2_POSITION_GRID_SYSTEM    "grid_system_3"
#define    CCMC_VARIABLE_NAME_RHO_2_DATA_GRID_SYSTEM        "grid_system_3"

#define    CCMC_VARIABLE_NAME_RHO_3                        "rho_3"
#define    CCMC_VARIABLE_NAME_RHO_3_DATA_TYPE            "float"
#define    CCMC_VARIABLE_NAME_RHO_3_CLASSIFICATION        "data"
#define    CCMC_VARIABLE_NAME_RHO_3_VALID_MIN            0.0
#define    CCMC_VARIABLE_NAME_RHO_3_VALID_MAX            1000000000000.0
#define    CCMC_VARIABLE_NAME_RHO_3_UNITS                "amu/cm^3"
#define    CCMC_VARIABLE_NAME_RHO_3_GRID_SYSTEM            "grid_system_4"
#define    CCMC_VARIABLE_NAME_RHO_3_MASK                    FLOAT_MASK
#define    CCMC_VARIABLE_NAME_RHO_3_DESCRIPTION            "atomic mass density, limit may bee exceeded in dense atmosphere; solar corona 2e8"
#define    CCMC_VARIABLE_NAME_RHO_3_IS_VECTOR            0
#define    CCMC_VARIABLE_NAME_RHO_3_POSITION_GRID_SYSTEM    "grid_system_4"
#define    CCMC_VARIABLE_NAME_RHO_3_DATA_GRID_SYSTEM        "grid_system_4"

#define    CCMC_VARIABLE_NAME_RHO_4                        "rho_4"
#define    CCMC_VARIABLE_NAME_RHO_4_DATA_TYPE            "float"
#define    CCMC_VARIABLE_NAME_RHO_4_CLASSIFICATION        "data"
#define    CCMC_VARIABLE_NAME_RHO_4_VALID_MIN            0.0
#define    CCMC_VARIABLE_NAME_RHO_4_VALID_MAX            1000000000000.0
#define    CCMC_VARIABLE_NAME_RHO_4_UNITS                "amu/cm^3"
#define    CCMC_VARIABLE_NAME_RHO_4_GRID_SYSTEM            "grid_system_5"
#define    CCMC_VARIABLE_NAME_RHO_4_MASK                    FLOAT_MASK
#define    CCMC_VARIABLE_NAME_RHO_4_DESCRIPTION            "atomic mass density, limit may bee exceeded in dense atmosphere; solar corona 2e8"
#define    CCMC_VARIABLE_NAME_RHO_4_IS_VECTOR            0
#define    CCMC_VARIABLE_NAME_RHO_4_POSITION_GRID_SYSTEM    "grid_system_5"
#define    CCMC_VARIABLE_NAME_RHO_4_DATA_GRID_SYSTEM        "grid_system_5"

#define    CCMC_VARIABLE_NAME_DP                             "dp"
#define    CCMC_VARIABLE_NAME_DP_DATA_TYPE                "float"
#define    CCMC_VARIABLE_NAME_DP_CLASSIFICATION            "data"
#define    CCMC_VARIABLE_NAME_DP_VALID_MIN                0.0
#define    CCMC_VARIABLE_NAME_DP_VALID_MAX                1000000000000.0
#define    CCMC_VARIABLE_NAME_DP_UNITS                    "kg/m^3"
#define    CCMC_VARIABLE_NAME_DP_GRID_SYSTEM                "grid_system_1"
#define    CCMC_VARIABLE_NAME_DP_MASK                    FLOAT_MASK
#define    CCMC_VARIABLE_NAME_DP_DESCRIPTION                "Cloud Mass Density"
#define    CCMC_VARIABLE_NAME_DP_IS_VECTOR                1
#define    CCMC_VARIABLE_NAME_DP_POSITION_GRID_SYSTEM        "grid_system_1"
#define    CCMC_VARIABLE_NAME_DP_DATA_GRID_SYSTEM            "grid_system_1"

#define    CCMC_VARIABLE_NAME_DP_1                        "dp_1"
#define    CCMC_VARIABLE_NAME_DP_1_DATA_TYPE                "float"
#define    CCMC_VARIABLE_NAME_DP_1_CLASSIFICATION            "data"
#define    CCMC_VARIABLE_NAME_DP_1_VALID_MIN                0.0
#define    CCMC_VARIABLE_NAME_DP_1_VALID_MAX                1000000000000.0
#define    CCMC_VARIABLE_NAME_DP_1_UNITS                    "kg/m^3"
#define    CCMC_VARIABLE_NAME_DP_1_GRID_SYSTEM            "grid_system_2"
#define    CCMC_VARIABLE_NAME_DP_1_MASK                    FLOAT_MASK
#define    CCMC_VARIABLE_NAME_DP_1_DESCRIPTION            "Cloud Mass Density"
#define    CCMC_VARIABLE_NAME_DP_1_IS_VECTOR                1
#define    CCMC_VARIABLE_NAME_DP_1_POSITION_GRID_SYSTEM    "grid_system_2"
#define    CCMC_VARIABLE_NAME_DP_1_DATA_GRID_SYSTEM        "grid_system_2"

#define    CCMC_VARIABLE_NAME_DP_2                        "dp_2"
#define    CCMC_VARIABLE_NAME_DP_2_DATA_TYPE                "float"
#define    CCMC_VARIABLE_NAME_DP_2_CLASSIFICATION            "data"
#define    CCMC_VARIABLE_NAME_DP_2_VALID_MIN                0.0
#define    CCMC_VARIABLE_NAME_DP_2_VALID_MAX                1000000000000.0
#define    CCMC_VARIABLE_NAME_DP_2_UNITS                    "kg/m^3"
#define    CCMC_VARIABLE_NAME_DP_2_GRID_SYSTEM            "grid_system_3"
#define    CCMC_VARIABLE_NAME_DP_2_MASK                    FLOAT_MASK
#define    CCMC_VARIABLE_NAME_DP_2_DESCRIPTION            "Cloud Mass Density"
#define    CCMC_VARIABLE_NAME_DP_2_IS_VECTOR                1
#define    CCMC_VARIABLE_NAME_DP_2_POSITION_GRID_SYSTEM    "grid_system_3"
#define    CCMC_VARIABLE_NAME_DP_2_DATA_GRID_SYSTEM        "grid_system_3"

#define    CCMC_VARIABLE_NAME_DP_3                        "dp_3"
#define    CCMC_VARIABLE_NAME_DP_3_DATA_TYPE                "float"
#define    CCMC_VARIABLE_NAME_DP_3_CLASSIFICATION            "data"
#define    CCMC_VARIABLE_NAME_DP_3_VALID_MIN                0.0
#define    CCMC_VARIABLE_NAME_DP_3_VALID_MAX                1000000000000.0
#define    CCMC_VARIABLE_NAME_DP_3_UNITS                    "kg/m^3"
#define    CCMC_VARIABLE_NAME_DP_3_GRID_SYSTEM            "grid_system_4"
#define    CCMC_VARIABLE_NAME_DP_3_MASK                    FLOAT_MASK
#define    CCMC_VARIABLE_NAME_DP_3_DESCRIPTION            "Cloud Mass Density"
#define    CCMC_VARIABLE_NAME_DP_3_IS_VECTOR                1
#define    CCMC_VARIABLE_NAME_DP_3_POSITION_GRID_SYSTEM    "grid_system_4"
#define    CCMC_VARIABLE_NAME_DP_3_DATA_GRID_SYSTEM        "grid_system_4"

#define    CCMC_VARIABLE_NAME_DP_4                        "dp_4"
#define    CCMC_VARIABLE_NAME_DP_4_DATA_TYPE                "float"
#define    CCMC_VARIABLE_NAME_DP_4_CLASSIFICATION            "data"
#define    CCMC_VARIABLE_NAME_DP_4_VALID_MIN                0.0
#define    CCMC_VARIABLE_NAME_DP_4_VALID_MAX                1000000000000.0
#define    CCMC_VARIABLE_NAME_DP_4_UNITS                    "kg/m^3"
#define    CCMC_VARIABLE_NAME_DP_4_GRID_SYSTEM            "grid_system_5"
#define    CCMC_VARIABLE_NAME_DP_4_MASK                    FLOAT_MASK
#define    CCMC_VARIABLE_NAME_DP_4_DESCRIPTION            "Cloud Mass Density"
#define    CCMC_VARIABLE_NAME_DP_4_IS_VECTOR                1
#define    CCMC_VARIABLE_NAME_DP_4_POSITION_GRID_SYSTEM    "grid_system_5"
#define    CCMC_VARIABLE_NAME_DP_4_DATA_GRID_SYSTEM        "grid_system_5"

#define CCMC_VARIABLE_NAME_P                            "p"
#define CCMC_VARIABLE_NAME_P_DATA_TYPE                    "float"
#define    CCMC_VARIABLE_NAME_P_CLASSIFICATION            "data"
#define    CCMC_VARIABLE_NAME_P_VALID_MIN                0.0
#define    CCMC_VARIABLE_NAME_P_VALID_MAX                1000000000000.0
#define    CCMC_VARIABLE_NAME_P_UNITS                    "nPa"
#define    CCMC_VARIABLE_NAME_P_GRID_SYSTEM                "grid_system_1"
#define CCMC_VARIABLE_NAME_P_MASK                        FLOAT_MASK
#define    CCMC_VARIABLE_NAME_P_DESCRIPTION                "pressure, limit may be exceeded in dense atmosphere"
#define    CCMC_VARIABLE_NAME_P_IS_VECTOR                0
#define    CCMC_VARIABLE_NAME_P_POSITION_GRID_SYSTEM        "grid_system_1"
#define    CCMC_VARIABLE_NAME_P_DATA_GRID_SYSTEM            "grid_system_1"

#define CCMC_VARIABLE_NAME_E                            "e"
#define CCMC_VARIABLE_NAME_E_DATA_TYPE                    "float"
#define    CCMC_VARIABLE_NAME_E_CLASSIFICATION            "data"
#define    CCMC_VARIABLE_NAME_E_VALID_MIN                0.0
#define    CCMC_VARIABLE_NAME_E_VALID_MAX                1000000000000.0
#define    CCMC_VARIABLE_NAME_E_UNITS                    "J/m^3"
#define    CCMC_VARIABLE_NAME_E_GRID_SYSTEM                "grid_system_1"
#define CCMC_VARIABLE_NAME_E_MASK                        FLOAT_MASK
#define    CCMC_VARIABLE_NAME_E_DESCRIPTION                "energy, limit may be exceeded in dense atmosphere"
#define    CCMC_VARIABLE_NAME_E_IS_VECTOR                0
#define    CCMC_VARIABLE_NAME_E_POSITION_GRID_SYSTEM        "grid_system_1"
#define    CCMC_VARIABLE_NAME_E_DATA_GRID_SYSTEM            "grid_system_1"

#define CCMC_VARIABLE_NAME_N_E                        "N_e"
#define CCMC_VARIABLE_NAME_N_E_DATA_TYPE                "float"
#define    CCMC_VARIABLE_NAME_N_E_CLASSIFICATION            "data"
#define    CCMC_VARIABLE_NAME_N_E_VALID_MIN                0.0
#define    CCMC_VARIABLE_NAME_N_E_VALID_MAX                1000000000000000000000000000000.0
#define    CCMC_VARIABLE_NAME_N_E_UNITS                    "m/^-3"
#define    CCMC_VARIABLE_NAME_N_E_GRID_SYSTEM            "grid_system_1"
#define CCMC_VARIABLE_NAME_N_E_MASK                    FLOAT_MASK
#define    CCMC_VARIABLE_NAME_N_E_DESCRIPTION            "Number of Electrons"
#define    CCMC_VARIABLE_NAME_N_E_IS_VECTOR                0
#define    CCMC_VARIABLE_NAME_N_E_POSITION_GRID_SYSTEM    "grid_system_1"
#define    CCMC_VARIABLE_NAME_N_E_DATA_GRID_SYSTEM        "grid_system_1"

#define CCMC_VARIABLE_NAME_N_O                        "N_O"
#define CCMC_VARIABLE_NAME_N_O_DATA_TYPE                "float"
#define    CCMC_VARIABLE_NAME_N_O_CLASSIFICATION            "data"
#define    CCMC_VARIABLE_NAME_N_O_VALID_MIN                0.0
#define    CCMC_VARIABLE_NAME_N_O_VALID_MAX                1000000000000000000000000000000.0
#define    CCMC_VARIABLE_NAME_N_O_UNITS                    "m/^-3"
#define    CCMC_VARIABLE_NAME_N_O_GRID_SYSTEM            "grid_system_1"
#define CCMC_VARIABLE_NAME_N_O_MASK                    FLOAT_MASK
#define    CCMC_VARIABLE_NAME_N_O_DESCRIPTION            "Number Density of Oxygen Ions"
#define    CCMC_VARIABLE_NAME_N_O_IS_VECTOR                0
#define    CCMC_VARIABLE_NAME_N_O_POSITION_GRID_SYSTEM    "grid_system_1"
#define    CCMC_VARIABLE_NAME_N_O_DATA_GRID_SYSTEM        "grid_system_1"

#define CCMC_VARIABLE_NAME_N_O2                        "N_O2"
#define CCMC_VARIABLE_NAME_N_O2_DATA_TYPE                "float"
#define    CCMC_VARIABLE_NAME_N_O2_CLASSIFICATION            "data"
#define    CCMC_VARIABLE_NAME_N_O2_VALID_MIN                0.0
#define    CCMC_VARIABLE_NAME_N_O2_VALID_MAX                1000000000000000000000000000000.0
#define    CCMC_VARIABLE_NAME_N_O2_UNITS                    "m/s"
#define    CCMC_VARIABLE_NAME_N_O2_GRID_SYSTEM            "grid_system_1"
#define CCMC_VARIABLE_NAME_N_O2_MASK                    FLOAT_MASK
#define    CCMC_VARIABLE_NAME_N_O2_DESCRIPTION            "Number Density of Molecular Oxygen"
#define    CCMC_VARIABLE_NAME_N_O2_IS_VECTOR                0
#define    CCMC_VARIABLE_NAME_N_O2_POSITION_GRID_SYSTEM    "grid_system_1"
#define    CCMC_VARIABLE_NAME_N_O2_DATA_GRID_SYSTEM        "grid_system_1"

#define CCMC_VARIABLE_NAME_N_N2                        "N_N2"
#define CCMC_VARIABLE_NAME_N_N2_DATA_TYPE                "float"
#define    CCMC_VARIABLE_NAME_N_N2_CLASSIFICATION            "data"
#define    CCMC_VARIABLE_NAME_N_N2_VALID_MIN                0.0
#define    CCMC_VARIABLE_NAME_N_N2_VALID_MAX                1000000000000000000000000000000.0
#define    CCMC_VARIABLE_NAME_N_N2_UNITS                    "m/s"
#define    CCMC_VARIABLE_NAME_N_N2_GRID_SYSTEM            "grid_system_1"
#define CCMC_VARIABLE_NAME_N_N2_MASK                    FLOAT_MASK
#define    CCMC_VARIABLE_NAME_N_N2_DESCRIPTION            "Number Density of Molecular Nitrogen"
#define    CCMC_VARIABLE_NAME_N_N2_IS_VECTOR                0
#define    CCMC_VARIABLE_NAME_N_N2_POSITION_GRID_SYSTEM    "grid_system_1"
#define    CCMC_VARIABLE_NAME_N_N2_DATA_GRID_SYSTEM        "grid_system_1"

#define CCMC_VARIABLE_NAME_Tn                            "Tn"
#define CCMC_VARIABLE_NAME_Tn_DATA_TYPE                "float"
#define    CCMC_VARIABLE_NAME_Tn_CLASSIFICATION            "data"
#define    CCMC_VARIABLE_NAME_Tn_VALID_MIN                0.0
#define    CCMC_VARIABLE_NAME_Tn_VALID_MAX                1000000.0
#define    CCMC_VARIABLE_NAME_Tn_UNITS                    "K"
#define    CCMC_VARIABLE_NAME_Tn_GRID_SYSTEM                "grid_system_1"
#define CCMC_VARIABLE_NAME_Tn_MASK                    FLOAT_MASK
#define    CCMC_VARIABLE_NAME_Tn_DESCRIPTION                "Neutral Temperarture"
#define    CCMC_VARIABLE_NAME_Tn_IS_VECTOR                0
#define    CCMC_VARIABLE_NAME_Tn_POSITION_GRID_SYSTEM        "grid_system_1"
#define    CCMC_VARIABLE_NAME_Tn_DATA_GRID_SYSTEM            "grid_system_1"

#define    CCMC_VARIABLE_NAME_T                            "T"
#define    CCMC_VARIABLE_NAME_T_DATA_TYPE                "float"
#define    CCMC_VARIABLE_NAME_T_CLASSIFICATION            "data"
#define    CCMC_VARIABLE_NAME_T_VALID_MIN                0.0
#define    CCMC_VARIABLE_NAME_T_VALID_MAX                1000000.0
#define    CCMC_VARIABLE_NAME_T_UNITS                    "K"
#define    CCMC_VARIABLE_NAME_T_GRID_SYSTEM                "grid_system_1"
#define    CCMC_VARIABLE_NAME_T_MASK                        FLOAT_MASK
#define    CCMC_VARIABLE_NAME_T_DESCRIPTION                "Temperarture"
#define    CCMC_VARIABLE_NAME_T_IS_VECTOR                0
#define    CCMC_VARIABLE_NAME_T_POSITION_GRID_SYSTEM        "grid_system_1"
#define    CCMC_VARIABLE_NAME_T_DATA_GRID_SYSTEM            "grid_system_1"

#define    CCMC_VARIABLE_NAME_T_1                        "T_1"
#define    CCMC_VARIABLE_NAME_T_1_DATA_TYPE                "float"
#define    CCMC_VARIABLE_NAME_T_1_CLASSIFICATION            "data"
#define    CCMC_VARIABLE_NAME_T_1_VALID_MIN                0.0
#define    CCMC_VARIABLE_NAME_T_1_VALID_MAX                1000000.0
#define    CCMC_VARIABLE_NAME_T_1_UNITS                    "K"
#define    CCMC_VARIABLE_NAME_T_1_GRID_SYSTEM            "grid_system_2"
#define    CCMC_VARIABLE_NAME_T_1_MASK                    FLOAT_MASK
#define    CCMC_VARIABLE_NAME_T_1_DESCRIPTION            "Temperarture"
#define    CCMC_VARIABLE_NAME_T_1_IS_VECTOR                0
#define    CCMC_VARIABLE_NAME_T_1_POSITION_GRID_SYSTEM    "grid_system_2"
#define    CCMC_VARIABLE_NAME_T_1_DATA_GRID_SYSTEM        "grid_system_2"

#define    CCMC_VARIABLE_NAME_T_2                        "T_2"
#define    CCMC_VARIABLE_NAME_T_2_DATA_TYPE                "float"
#define    CCMC_VARIABLE_NAME_T_2_CLASSIFICATION            "data"
#define    CCMC_VARIABLE_NAME_T_2_VALID_MIN                0.0
#define    CCMC_VARIABLE_NAME_T_2_VALID_MAX                1000000.0
#define    CCMC_VARIABLE_NAME_T_2_UNITS                    "K"
#define    CCMC_VARIABLE_NAME_T_2_GRID_SYSTEM            "grid_system_3"
#define    CCMC_VARIABLE_NAME_T_2_MASK                    FLOAT_MASK
#define    CCMC_VARIABLE_NAME_T_2_DESCRIPTION            "Temperarture"
#define    CCMC_VARIABLE_NAME_T_2_IS_VECTOR                0
#define    CCMC_VARIABLE_NAME_T_2_POSITION_GRID_SYSTEM    "grid_system_3"
#define    CCMC_VARIABLE_NAME_T_2_DATA_GRID_SYSTEM        "grid_system_3"

#define    CCMC_VARIABLE_NAME_T_3                        "T_3"
#define    CCMC_VARIABLE_NAME_T_3_DATA_TYPE                "float"
#define    CCMC_VARIABLE_NAME_T_3_CLASSIFICATION            "data"
#define    CCMC_VARIABLE_NAME_T_3_VALID_MIN                0.0
#define    CCMC_VARIABLE_NAME_T_3_VALID_MAX                1000000.0
#define    CCMC_VARIABLE_NAME_T_3_UNITS                    "K"
#define    CCMC_VARIABLE_NAME_T_3_GRID_SYSTEM            "grid_system_4"
#define    CCMC_VARIABLE_NAME_T_3_MASK                    FLOAT_MASK
#define    CCMC_VARIABLE_NAME_T_3_DESCRIPTION            "Temperarture"
#define    CCMC_VARIABLE_NAME_T_3_IS_VECTOR                0
#define    CCMC_VARIABLE_NAME_T_3_POSITION_GRID_SYSTEM    "grid_system_4"
#define    CCMC_VARIABLE_NAME_T_3_DATA_GRID_SYSTEM        "grid_system_4"

#define    CCMC_VARIABLE_NAME_T_4                        "T_4"
#define    CCMC_VARIABLE_NAME_T_4_DATA_TYPE                "float"
#define    CCMC_VARIABLE_NAME_T_4_CLASSIFICATION            "data"
#define    CCMC_VARIABLE_NAME_T_4_VALID_MIN                0.0
#define    CCMC_VARIABLE_NAME_T_4_VALID_MAX                1000000.0
#define    CCMC_VARIABLE_NAME_T_4_UNITS                    "K"
#define    CCMC_VARIABLE_NAME_T_4_GRID_SYSTEM            "grid_system_5"
#define    CCMC_VARIABLE_NAME_T_4_MASK                    FLOAT_MASK
#define    CCMC_VARIABLE_NAME_T_4_DESCRIPTION            "Temperarture"
#define    CCMC_VARIABLE_NAME_T_4_IS_VECTOR                0
#define    CCMC_VARIABLE_NAME_T_4_POSITION_GRID_SYSTEM    "grid_system_5"
#define    CCMC_VARIABLE_NAME_T_4_DATA_GRID_SYSTEM        "grid_system_5"

#define CCMC_VARIABLE_NAME_RMT                        "Rmt"
#define CCMC_VARIABLE_NAME_RMT_DATA_TYPE                "float"
#define    CCMC_VARIABLE_NAME_RMT_CLASSIFICATION            "data"
#define    CCMC_VARIABLE_NAME_RMT_VALID_MIN                0.0
#define    CCMC_VARIABLE_NAME_RMT_VALID_MAX                1000000.0
#define    CCMC_VARIABLE_NAME_RMT_UNITS                    "amu"
#define    CCMC_VARIABLE_NAME_RMT_GRID_SYSTEM            "grid_system_1"
#define CCMC_VARIABLE_NAME_RMT_MASK                    FLOAT_MASK
#define    CCMC_VARIABLE_NAME_RMT_DESCRIPTION            "Mean Molecular Mass"
#define    CCMC_VARIABLE_NAME_RMT_IS_VECTOR                0
#define    CCMC_VARIABLE_NAME_RMT_POSITION_GRID_SYSTEM    "grid_system_1"
#define    CCMC_VARIABLE_NAME_RMT_DATA_GRID_SYSTEM        "grid_system_1"

#define CCMC_VARIABLE_NAME_SIGMA_P                    "sigmaP"
#define CCMC_VARIABLE_NAME_SIGMA_P_DATA_TYPE            "float"
#define    CCMC_VARIABLE_NAME_SIGMA_P_CLASSIFICATION        "data"
#define    CCMC_VARIABLE_NAME_SIGMA_P_VALID_MIN            0.0
#define    CCMC_VARIABLE_NAME_SIGMA_P_VALID_MAX            1000000.0
#define    CCMC_VARIABLE_NAME_SIGMA_P_UNITS                "mho/m"
#define    CCMC_VARIABLE_NAME_SIGMA_P_GRID_SYSTEM            "grid_system_1"
#define CCMC_VARIABLE_NAME_SIGMA_P_MASK                    FLOAT_MASK
#define    CCMC_VARIABLE_NAME_SIGMA_P_DESCRIPTION            "Pedersen Conductivity"
#define    CCMC_VARIABLE_NAME_SIGMA_P_IS_VECTOR            0
#define    CCMC_VARIABLE_NAME_SIGMA_P_POSITION_GRID_SYSTEM "grid_system_1"
#define    CCMC_VARIABLE_NAME_SIGMA_P_DATA_GRID_SYSTEM    "grid_system_1"

#define CCMC_VARIABLE_NAME_SIGMA_H                    "sigmaH"
#define CCMC_VARIABLE_NAME_SIGMA_H_DATA_TYPE            "float"
#define    CCMC_VARIABLE_NAME_SIGMA_H_CLASSIFICATION        "data"
#define    CCMC_VARIABLE_NAME_SIGMA_H_VALID_MIN            0.0
#define    CCMC_VARIABLE_NAME_SIGMA_H_VALID_MAX            1000000.0
#define    CCMC_VARIABLE_NAME_SIGMA_H_UNITS                "mho/m"
#define    CCMC_VARIABLE_NAME_SIGMA_H_GRID_SYSTEM            "grid_system_1"
#define CCMC_VARIABLE_NAME_SIGMA_H_MASK                FLOAT_MASK
#define    CCMC_VARIABLE_NAME_SIGMA_H_DESCRIPTION            "Hall Conductivity"
#define    CCMC_VARIABLE_NAME_SIGMA_H_IS_VECTOR            0
#define    CCMC_VARIABLE_NAME_SIGMA_H_POSITION_GRID_SYSTEM "grid_system_1"
#define    CCMC_VARIABLE_NAME_SIGMA_H_DATA_GRID_SYSTEM    "grid_system_1"

#define CCMC_VARIABLE_NAME_BLOCK_AMR_LEVEL                            "block_amr_levels"
#define CCMC_VARIABLE_NAME_BLOCK_AMR_LEVEL_DATA_TYPE                "float"
#define    CCMC_VARIABLE_NAME_BLOCK_AMR_LEVEL_CLASSIFICATION            "grid"
#define    CCMC_VARIABLE_NAME_BLOCK_AMR_LEVEL_VALID_MIN                0.0
#define    CCMC_VARIABLE_NAME_BLOCK_AMR_LEVEL_VALID_MAX                FLOAT_MASK
#define    CCMC_VARIABLE_NAME_BLOCK_AMR_LEVEL_UNITS                    "N/A"
#define    CCMC_VARIABLE_NAME_BLOCK_AMR_LEVEL_GRID_SYSTEM                "N/A"
#define CCMC_VARIABLE_NAME_BLOCK_AMR_LEVEL_MASK                        FLOAT_MASK
#define    CCMC_VARIABLE_NAME_BLOCK_AMR_LEVEL_DESCRIPTION                "BATSRUS Adaptive Mesh Refinement Level - the number of levels typically < 10 can be > 20"
#define    CCMC_VARIABLE_NAME_BLOCK_AMR_LEVEL_IS_VECTOR                0
#define    CCMC_VARIABLE_NAME_BLOCK_AMR_LEVEL_POSITION_GRID_SYSTEM        "N/A"
#define    CCMC_VARIABLE_NAME_BLOCK_AMR_LEVEL_DATA_GRID_SYSTEM            "N/A"

#define CCMC_VARIABLE_NAME_BLOCK_AT_AMR_LEVEL                        "block_at_amr_level"
#define CCMC_VARIABLE_NAME_BLOCK_AT_AMR_LEVEL_DATA_TYPE                "int"
#define    CCMC_VARIABLE_NAME_BLOCK_AT_AMR_LEVEL_CLASSIFICATION        "grid"
#define    CCMC_VARIABLE_NAME_BLOCK_AT_AMR_LEVEL_VALID_MIN                0
#define    CCMC_VARIABLE_NAME_BLOCK_AT_AMR_LEVEL_VALID_MAX                INT_MASK
#define    CCMC_VARIABLE_NAME_BLOCK_AT_AMR_LEVEL_UNITS                    "N/A"
#define    CCMC_VARIABLE_NAME_BLOCK_AT_AMR_LEVEL_GRID_SYSTEM            "N/A"
#define CCMC_VARIABLE_NAME_BLOCK_AT_AMR_LEVEL_MASK                    INT_MASK
#define    CCMC_VARIABLE_NAME_BLOCK_AT_AMR_LEVEL_DESCRIPTION            "Truncated look up table to get block id of root blocks.  Number of elements stored in CDF equal to number of parents at amr level 0"
#define    CCMC_VARIABLE_NAME_BLOCK_AT_AMR_LEVEL_IS_VECTOR                0
#define    CCMC_VARIABLE_NAME_BLOCK_AT_AMR_LEVEL_POSITION_GRID_SYSTEM    "N/A"
#define    CCMC_VARIABLE_NAME_BLOCK_AT_AMR_LEVEL_DATA_GRID_SYSTEM        "N/A"

#define CCMC_VARIABLE_NAME_BLOCK_X_MIN                                "block_x_min"
#define CCMC_VARIABLE_NAME_BLOCK_X_MIN_DATA_TYPE                    "float"
#define    CCMC_VARIABLE_NAME_BLOCK_X_MIN_CLASSIFICATION                "grid"
#define    CCMC_VARIABLE_NAME_BLOCK_X_MIN_VALID_MIN                    -10000.0
#define    CCMC_VARIABLE_NAME_BLOCK_X_MIN_VALID_MAX                    10000.0
#define    CCMC_VARIABLE_NAME_BLOCK_X_MIN_UNITS                        "R"
#define    CCMC_VARIABLE_NAME_BLOCK_X_MIN_GRID_SYSTEM                    "N/A"
#define CCMC_VARIABLE_NAME_BLOCK_X_MIN_MASK                            FLOAT_MASK
#define    CCMC_VARIABLE_NAME_BLOCK_X_MIN_DESCRIPTION                    "Smallest X position value included in current block, limits same as X,Y,Z"
#define    CCMC_VARIABLE_NAME_BLOCK_X_MIN_IS_VECTOR                    0
#define    CCMC_VARIABLE_NAME_BLOCK_X_MIN_POSITION_GRID_SYSTEM            "N/A"
#define    CCMC_VARIABLE_NAME_BLOCK_X_MIN_DATA_GRID_SYSTEM                "N/A"

#define CCMC_VARIABLE_NAME_BLOCK_X_MAX                                "block_x_max"
#define CCMC_VARIABLE_NAME_BLOCK_X_MAX_DATA_TYPE                    "float"
#define    CCMC_VARIABLE_NAME_BLOCK_X_MAX_CLASSIFICATION                "grid"
#define CCMC_VARIABLE_NAME_BLOCK_X_MAX_VALID_MIN                    -10000.0
#define    CCMC_VARIABLE_NAME_BLOCK_X_MAX_VALID_MAX                    10000.0
#define    CCMC_VARIABLE_NAME_BLOCK_X_MAX_UNITS                        "RE"
#define    CCMC_VARIABLE_NAME_BLOCK_X_MAX_GRID_SYSTEM                    "N/A"
#define CCMC_VARIABLE_NAME_BLOCK_X_MAX_MASK                            FLOAT_MASK
#define    CCMC_VARIABLE_NAME_BLOCK_X_MAX_DESCRIPTION                    "Largest X position value included in current block, limits same as X,Y,Z "
#define    CCMC_VARIABLE_NAME_BLOCK_X_MAX_IS_VECTOR                    0
#define    CCMC_VARIABLE_NAME_BLOCK_X_MAX_POSITION_GRID_SYSTEM            "N/A"
#define    CCMC_VARIABLE_NAME_BLOCK_X_MAX_DATA_GRID_SYSTEM                "N/A"

#define CCMC_VARIABLE_NAME_BLOCK_Y_MIN                                "block_y_min"
#define CCMC_VARIABLE_NAME_BLOCK_Y_MIN_DATA_TYPE                    "float"
#define    CCMC_VARIABLE_NAME_BLOCK_Y_MIN_CLASSIFICATION                "grid"
#define    CCMC_VARIABLE_NAME_BLOCK_Y_MIN_VALID_MIN                    -10000.0
#define    CCMC_VARIABLE_NAME_BLOCK_Y_MIN_VALID_MAX                    10000.0
#define    CCMC_VARIABLE_NAME_BLOCK_Y_MIN_UNITS                        "RE"
#define    CCMC_VARIABLE_NAME_BLOCK_Y_MIN_GRID_SYSTEM                    "N/A"
#define CCMC_VARIABLE_NAME_BLOCK_Y_MIN_MASK                            FLOAT_MASK
#define    CCMC_VARIABLE_NAME_BLOCK_Y_MIN_DESCRIPTION                    "Smallest Y position value included in current block, limits same as X,Y,Z "
#define    CCMC_VARIABLE_NAME_BLOCK_Y_MIN_IS_VECTOR                    0
#define    CCMC_VARIABLE_NAME_BLOCK_Y_MIN_POSITION_GRID_SYSTEM            "N/A"
#define    CCMC_VARIABLE_NAME_BLOCK_Y_MIN_DATA_GRID_SYSTEM                "N/A"

#define CCMC_VARIABLE_NAME_BLOCK_Y_MAX                                "block_y_max"
#define CCMC_VARIABLE_NAME_BLOCK_Y_MAX_DATA_TYPE                    "float"
#define    CCMC_VARIABLE_NAME_BLOCK_Y_MAX_CLASSIFICATION                "grid"
#define CCMC_VARIABLE_NAME_BLOCK_Y_MAX_VALID_MIN                    -10000.0
#define    CCMC_VARIABLE_NAME_BLOCK_Y_MAX_VALID_MAX                    10000.0
#define    CCMC_VARIABLE_NAME_BLOCK_Y_MAX_UNITS                        "RE"
#define    CCMC_VARIABLE_NAME_BLOCK_Y_MAX_GRID_SYSTEM                    "N/A"
#define CCMC_VARIABLE_NAME_BLOCK_Y_MAX_MASK                            FLOAT_MASK
#define    CCMC_VARIABLE_NAME_BLOCK_Y_MAX_DESCRIPTION                    "Largest Y position value included in current block, limits same as X,Y,Z "
#define    CCMC_VARIABLE_NAME_BLOCK_Y_MAX_IS_VECTOR                    0
#define    CCMC_VARIABLE_NAME_BLOCK_Y_MAX_POSITION_GRID_SYSTEM            "N/A"
#define    CCMC_VARIABLE_NAME_BLOCK_Y_MAX_DATA_GRID_SYSTEM                "N/A"

#define CCMC_VARIABLE_NAME_BLOCK_Z_MIN                                "block_z_min"
#define CCMC_VARIABLE_NAME_BLOCK_Z_MIN_DATA_TYPE                    "float"
#define    CCMC_VARIABLE_NAME_BLOCK_Z_MIN_CLASSIFICATION                "grid"
#define    CCMC_VARIABLE_NAME_BLOCK_Z_MIN_VALID_MIN                    -10000.0
#define    CCMC_VARIABLE_NAME_BLOCK_Z_MIN_VALID_MAX                    10000.0
#define    CCMC_VARIABLE_NAME_BLOCK_Z_MIN_UNITS                        "RE"
#define    CCMC_VARIABLE_NAME_BLOCK_Z_MIN_GRID_SYSTEM                    "N/A"
#define CCMC_VARIABLE_NAME_BLOCK_Z_MIN_MASK                            FLOAT_MASK
#define    CCMC_VARIABLE_NAME_BLOCK_Z_MIN_DESCRIPTION                    "Smallest Z position value included in current block, limits same as X,Y,Z "
#define    CCMC_VARIABLE_NAME_BLOCK_Z_MIN_IS_VECTOR                    0
#define    CCMC_VARIABLE_NAME_BLOCK_Z_MIN_POSITION_GRID_SYSTEM            "N/A"
#define    CCMC_VARIABLE_NAME_BLOCK_Z_MIN_DATA_GRID_SYSTEM                "N/A"

#define CCMC_VARIABLE_NAME_BLOCK_Z_MAX                                "block_z_max"
#define CCMC_VARIABLE_NAME_BLOCK_Z_MAX_DATA_TYPE                    "float"
#define    CCMC_VARIABLE_NAME_BLOCK_Z_MAX_CLASSIFICATION                "grid"
#define CCMC_VARIABLE_NAME_BLOCK_Z_MAX_VALID_MIN                    -10000.0
#define    CCMC_VARIABLE_NAME_BLOCK_Z_MAX_VALID_MAX                    10000.0
#define    CCMC_VARIABLE_NAME_BLOCK_Z_MAX_UNITS                        "RE"
#define    CCMC_VARIABLE_NAME_BLOCK_Z_MAX_GRID_SYSTEM                    "N/A"
#define CCMC_VARIABLE_NAME_BLOCK_Z_MAX_MASK                            FLOAT_MASK
#define    CCMC_VARIABLE_NAME_BLOCK_Z_MAX_DESCRIPTION                    "Largest Z position value included in current block, limits same as X,Y,Z "
#define    CCMC_VARIABLE_NAME_BLOCK_Z_MAX_IS_VECTOR                    0
#define    CCMC_VARIABLE_NAME_BLOCK_Z_MAX_POSITION_GRID_SYSTEM            "N/A"
#define    CCMC_VARIABLE_NAME_BLOCK_Z_MAX_DATA_GRID_SYSTEM                "N/A"

#define CCMC_VARIABLE_NAME_BLOCK_X_CENTER                            "block_x_center"
#define CCMC_VARIABLE_NAME_BLOCK_X_CENTER_DATA_TYPE                    "float"
#define    CCMC_VARIABLE_NAME_BLOCK_X_CENTER_CLASSIFICATION            "grid"
#define CCMC_VARIABLE_NAME_BLOCK_X_CENTER_VALID_MIN                    -10000.0
#define    CCMC_VARIABLE_NAME_BLOCK_X_CENTER_VALID_MAX                    10000.0
#define    CCMC_VARIABLE_NAME_BLOCK_X_CENTER_UNITS                        "RE"
#define    CCMC_VARIABLE_NAME_BLOCK_X_CENTER_GRID_SYSTEM                "N/A"
#define CCMC_VARIABLE_NAME_BLOCK_X_CENTER_MASK                        FLOAT_MASK
#define    CCMC_VARIABLE_NAME_BLOCK_X_CENTER_DESCRIPTION                "Center position value X for current block, limits same as X,Y,Z "
#define    CCMC_VARIABLE_NAME_BLOCK_X_CENTER_IS_VECTOR                    0
#define    CCMC_VARIABLE_NAME_BLOCK_X_CENTER_POSITION_GRID_SYSTEM        "N/A"
#define    CCMC_VARIABLE_NAME_BLOCK_X_CENTER_DATA_GRID_SYSTEM            "N/A"

#define CCMC_VARIABLE_NAME_BLOCK_Y_CENTER                            "block_y_center"
#define CCMC_VARIABLE_NAME_BLOCK_Y_CENTER_DATA_TYPE                    "float"
#define    CCMC_VARIABLE_NAME_BLOCK_Y_CENTER_CLASSIFICATION            "grid"
#define    CCMC_VARIABLE_NAME_BLOCK_Y_CENTER_VALID_MIN                    -10000.0
#define    CCMC_VARIABLE_NAME_BLOCK_Y_CENTER_VALID_MAX                    10000.0
#define    CCMC_VARIABLE_NAME_BLOCK_Y_CENTER_UNITS                        "RE"
#define    CCMC_VARIABLE_NAME_BLOCK_Y_CENTER_GRID_SYSTEM                "N/A"
#define CCMC_VARIABLE_NAME_BLOCK_Y_CENTER_MASK                        FLOAT_MASK
#define    CCMC_VARIABLE_NAME_BLOCK_Y_CENTER_DESCRIPTION                "Center position value Y for current block, limits same as X,Y,Z "
#define    CCMC_VARIABLE_NAME_BLOCK_Y_CENTER_IS_VECTOR                    0
#define    CCMC_VARIABLE_NAME_BLOCK_Y_CENTER_POSITION_GRID_SYSTEM        "N/A"
#define    CCMC_VARIABLE_NAME_BLOCK_Y_CENTER_DATA_GRID_SYSTEM            "N/A"

#define CCMC_VARIABLE_NAME_BLOCK_Z_CENTER                            "block_z_center"
#define CCMC_VARIABLE_NAME_BLOCK_Z_CENTER_DATA_TYPE                    "float"
#define    CCMC_VARIABLE_NAME_BLOCK_Z_CENTER_CLASSIFICATION            "grid"
#define CCMC_VARIABLE_NAME_BLOCK_Z_CENTER_VALID_MIN                    -10000.0
#define    CCMC_VARIABLE_NAME_BLOCK_Z_CENTER_VALID_MAX                    10000.0
#define    CCMC_VARIABLE_NAME_BLOCK_Z_CENTER_UNITS                        "RE"
#define    CCMC_VARIABLE_NAME_BLOCK_Z_CENTER_GRID_SYSTEM                "N/A"
#define CCMC_VARIABLE_NAME_BLOCK_Z_CENTER_MASK                        FLOAT_MASK
#define    CCMC_VARIABLE_NAME_BLOCK_Z_CENTER_DESCRIPTION                "Center position value Z for current block, limits same as X,Y,Z "
#define    CCMC_VARIABLE_NAME_BLOCK_Z_CENTER_IS_VECTOR                    0
#define    CCMC_VARIABLE_NAME_BLOCK_Z_CENTER_POSITION_GRID_SYSTEM        "N/A"
#define    CCMC_VARIABLE_NAME_BLOCK_Z_CENTER_DATA_GRID_SYSTEM            "N/A"

#define CCMC_VARIABLE_NAME_BLOCK_PARENT_ID                            "block_parent_id"
#define CCMC_VARIABLE_NAME_BLOCK_PARENT_ID_DATA_TYPE                "int"
#define    CCMC_VARIABLE_NAME_BLOCK_PARENT_ID_CLASSIFICATION            "grid"
#define    CCMC_VARIABLE_NAME_BLOCK_PARENT_ID_VALID_MIN                1
#define    CCMC_VARIABLE_NAME_BLOCK_PARENT_ID_VALID_MAX                1000000.0
#define    CCMC_VARIABLE_NAME_BLOCK_PARENT_ID_UNITS                    "N/A"
#define    CCMC_VARIABLE_NAME_BLOCK_PARENT_ID_GRID_SYSTEM                "N/A"
#define CCMC_VARIABLE_NAME_BLOCK_PARENT_ID_MASK                        INT_MASK
#define    CCMC_VARIABLE_NAME_BLOCK_PARENT_ID_DESCRIPTION                "Parent Block ID for current block, -1 would be invalid/empty - arrays prefilled with -1"
#define    CCMC_VARIABLE_NAME_BLOCK_PARENT_ID_IS_VECTOR                0
#define    CCMC_VARIABLE_NAME_BLOCK_PARENT_ID_POSITION_GRID_SYSTEM        "N/A"
#define    CCMC_VARIABLE_NAME_BLOCK_PARENT_ID_DATA_GRID_SYSTEM            "N/A"

#define CCMC_VARIABLE_NAME_BLOCK_CHILD_COUNT                        "block_child_count"
#define CCMC_VARIABLE_NAME_BLOCK_CHILD_COUNT_DATA_TYPE                "int"
#define    CCMC_VARIABLE_NAME_BLOCK_CHILD_COUNT_CLASSIFICATION            "grid"
#define CCMC_VARIABLE_NAME_BLOCK_CHILD_COUNT_VALID_MIN                0.0
#define    CCMC_VARIABLE_NAME_BLOCK_CHILD_COUNT_VALID_MAX                8
#define    CCMC_VARIABLE_NAME_BLOCK_CHILD_COUNT_UNITS                    "N/A"
#define    CCMC_VARIABLE_NAME_BLOCK_CHILD_COUNT_GRID_SYSTEM            "N/A"
#define CCMC_VARIABLE_NAME_BLOCK_CHILD_COUNT_MASK                    INT_MASK
#define    CCMC_VARIABLE_NAME_BLOCK_CHILD_COUNT_DESCRIPTION            "Number of children blocks created from current block, 0 or 8 children per block by design of BATSRUS grid"
#define    CCMC_VARIABLE_NAME_BLOCK_CHILD_COUNT_IS_VECTOR                0
#define    CCMC_VARIABLE_NAME_BLOCK_CHILD_COUNT_POSITION_GRID_SYSTEM    "N/A"
#define    CCMC_VARIABLE_NAME_BLOCK_CHILD_COUNT_DATA_GRID_SYSTEM        "N/A"

#define CCMC_VARIABLE_NAME_BLOCK_CHILD_ID_1                            "block_child_id_1"
#define CCMC_VARIABLE_NAME_BLOCK_CHILD_ID_1_DATA_TYPE                "int"
#define    CCMC_VARIABLE_NAME_BLOCK_CHILD_ID_1_CLASSIFICATION            "grid"
#define    CCMC_VARIABLE_NAME_BLOCK_CHILD_ID_1_VALID_MIN                0.0
#define    CCMC_VARIABLE_NAME_BLOCK_CHILD_ID_1_VALID_MAX                1000000.0
#define    CCMC_VARIABLE_NAME_BLOCK_CHILD_ID_1_UNITS                    "N/A"
#define    CCMC_VARIABLE_NAME_BLOCK_CHILD_ID_1_GRID_SYSTEM                "N/A"
#define CCMC_VARIABLE_NAME_BLOCK_CHILD_ID_1_MASK                    INT_MASK
#define    CCMC_VARIABLE_NAME_BLOCK_CHILD_ID_1_DESCRIPTION                "Child 1 Block ID for current block, -1 would be invalid/empty - arrays prefilled with -1"
#define    CCMC_VARIABLE_NAME_BLOCK_CHILD_ID_1_IS_VECTOR                0
#define    CCMC_VARIABLE_NAME_BLOCK_CHILD_ID_1_POSITION_GRID_SYSTEM    "N/A"
#define    CCMC_VARIABLE_NAME_BLOCK_CHILD_ID_1_DATA_GRID_SYSTEM        "N/A"

#define CCMC_VARIABLE_NAME_BLOCK_CHILD_ID_2                            "block_child_id_2"
#define CCMC_VARIABLE_NAME_BLOCK_CHILD_ID_2_DATA_TYPE                "int"
#define    CCMC_VARIABLE_NAME_BLOCK_CHILD_ID_2_CLASSIFICATION            "grid"
#define    CCMC_VARIABLE_NAME_BLOCK_CHILD_ID_2_VALID_MIN                0.0
#define    CCMC_VARIABLE_NAME_BLOCK_CHILD_ID_2_VALID_MAX                1000000.0
#define    CCMC_VARIABLE_NAME_BLOCK_CHILD_ID_2_UNITS                    "N/A"
#define    CCMC_VARIABLE_NAME_BLOCK_CHILD_ID_2_GRID_SYSTEM                "N/A"
#define CCMC_VARIABLE_NAME_BLOCK_CHILD_ID_2_MASK                    INT_MASK
#define    CCMC_VARIABLE_NAME_BLOCK_CHILD_ID_2_DESCRIPTION                "Child 2 Block ID for current block, -1 would be invalid/empty - arrays prefilled with -1"
#define    CCMC_VARIABLE_NAME_BLOCK_CHILD_ID_2_IS_VECTOR                0
#define    CCMC_VARIABLE_NAME_BLOCK_CHILD_ID_2_POSITION_GRID_SYSTEM    "N/A"
#define    CCMC_VARIABLE_NAME_BLOCK_CHILD_ID_2_DATA_GRID_SYSTEM        "N/A"

#define CCMC_VARIABLE_NAME_BLOCK_CHILD_ID_3                            "block_child_id_3"
#define CCMC_VARIABLE_NAME_BLOCK_CHILD_ID_3_DATA_TYPE                "int"
#define    CCMC_VARIABLE_NAME_BLOCK_CHILD_ID_3_CLASSIFICATION            "grid"
#define    CCMC_VARIABLE_NAME_BLOCK_CHILD_ID_3_VALID_MIN                0.0
#define    CCMC_VARIABLE_NAME_BLOCK_CHILD_ID_3_VALID_MAX                1000000.0
#define    CCMC_VARIABLE_NAME_BLOCK_CHILD_ID_3_UNITS                    "N/A"
#define    CCMC_VARIABLE_NAME_BLOCK_CHILD_ID_3_GRID_SYSTEM                "N/A"
#define CCMC_VARIABLE_NAME_BLOCK_CHILD_ID_3_MASK                    INT_MASK
#define    CCMC_VARIABLE_NAME_BLOCK_CHILD_ID_3_DESCRIPTION                "Child 3 Block ID for current block, -1 would be invalid/empty - arrays prefilled with -1"
#define    CCMC_VARIABLE_NAME_BLOCK_CHILD_ID_3_IS_VECTOR                0
#define    CCMC_VARIABLE_NAME_BLOCK_CHILD_ID_3_POSITION_GRID_SYSTEM    "N/A"
#define    CCMC_VARIABLE_NAME_BLOCK_CHILD_ID_3_DATA_GRID_SYSTEM        "N/A"

#define CCMC_VARIABLE_NAME_BLOCK_CHILD_ID_4                            "block_child_id_4"
#define CCMC_VARIABLE_NAME_BLOCK_CHILD_ID_4_DATA_TYPE                "int"
#define    CCMC_VARIABLE_NAME_BLOCK_CHILD_ID_4_CLASSIFICATION            "grid"
#define    CCMC_VARIABLE_NAME_BLOCK_CHILD_ID_4_VALID_MIN                0.0
#define    CCMC_VARIABLE_NAME_BLOCK_CHILD_ID_4_VALID_MAX                1000000.0
#define    CCMC_VARIABLE_NAME_BLOCK_CHILD_ID_4_UNITS                    "N/A"
#define    CCMC_VARIABLE_NAME_BLOCK_CHILD_ID_4_GRID_SYSTEM                "N/A"
#define CCMC_VARIABLE_NAME_BLOCK_CHILD_ID_4_MASK                    INT_MASK
#define    CCMC_VARIABLE_NAME_BLOCK_CHILD_ID_4_DESCRIPTION                "Child 4 Block ID for current block, -1 would be invalid/empty - arrays prefilled with -1"
#define    CCMC_VARIABLE_NAME_BLOCK_CHILD_ID_4_IS_VECTOR                0
#define    CCMC_VARIABLE_NAME_BLOCK_CHILD_ID_4_POSITION_GRID_SYSTEM    "N/A"
#define    CCMC_VARIABLE_NAME_BLOCK_CHILD_ID_4_DATA_GRID_SYSTEM        "N/A"

#define CCMC_VARIABLE_NAME_BLOCK_CHILD_ID_5                            "block_child_id_5"
#define CCMC_VARIABLE_NAME_BLOCK_CHILD_ID_5_DATA_TYPE                "int"
#define    CCMC_VARIABLE_NAME_BLOCK_CHILD_ID_5_CLASSIFICATION            "grid"
#define    CCMC_VARIABLE_NAME_BLOCK_CHILD_ID_5_VALID_MIN                0.0
#define    CCMC_VARIABLE_NAME_BLOCK_CHILD_ID_5_VALID_MAX                1000000.0
#define    CCMC_VARIABLE_NAME_BLOCK_CHILD_ID_5_UNITS                    "N/A"
#define    CCMC_VARIABLE_NAME_BLOCK_CHILD_ID_5_GRID_SYSTEM                "N/A"
#define CCMC_VARIABLE_NAME_BLOCK_CHILD_ID_5_MASK                    INT_MASK
#define    CCMC_VARIABLE_NAME_BLOCK_CHILD_ID_5_DESCRIPTION                "Child 5 Block ID for current block, -1 would be invalid/empty - arrays prefilled with -1"
#define    CCMC_VARIABLE_NAME_BLOCK_CHILD_ID_5_IS_VECTOR                0
#define    CCMC_VARIABLE_NAME_BLOCK_CHILD_ID_5_POSITION_GRID_SYSTEM    "N/A"
#define    CCMC_VARIABLE_NAME_BLOCK_CHILD_ID_5_DATA_GRID_SYSTEM        "N/A"

#define CCMC_VARIABLE_NAME_BLOCK_CHILD_ID_6                            "block_child_id_6"
#define CCMC_VARIABLE_NAME_BLOCK_CHILD_ID_6_DATA_TYPE                "int"
#define    CCMC_VARIABLE_NAME_BLOCK_CHILD_ID_6_CLASSIFICATION            "grid"
#define    CCMC_VARIABLE_NAME_BLOCK_CHILD_ID_6_VALID_MIN                0.0
#define    CCMC_VARIABLE_NAME_BLOCK_CHILD_ID_6_VALID_MAX                1000000.0
#define    CCMC_VARIABLE_NAME_BLOCK_CHILD_ID_6_UNITS                    "N/A"
#define    CCMC_VARIABLE_NAME_BLOCK_CHILD_ID_6_GRID_SYSTEM                "N/A"
#define CCMC_VARIABLE_NAME_BLOCK_CHILD_ID_6_MASK                    INT_MASK
#define    CCMC_VARIABLE_NAME_BLOCK_CHILD_ID_6_DESCRIPTION                "Child 6 Block ID for current block, -1 would be invalid/empty - arrays prefilled with -1"
#define    CCMC_VARIABLE_NAME_BLOCK_CHILD_ID_6_IS_VECTOR                0
#define    CCMC_VARIABLE_NAME_BLOCK_CHILD_ID_6_POSITION_GRID_SYSTEM    "N/A"
#define    CCMC_VARIABLE_NAME_BLOCK_CHILD_ID_6_DATA_GRID_SYSTEM        "N/A"

#define CCMC_VARIABLE_NAME_BLOCK_CHILD_ID_7                            "block_child_id_7"
#define CCMC_VARIABLE_NAME_BLOCK_CHILD_ID_7_DATA_TYPE                "int"
#define    CCMC_VARIABLE_NAME_BLOCK_CHILD_ID_7_CLASSIFICATION            "grid"
#define    CCMC_VARIABLE_NAME_BLOCK_CHILD_ID_7_VALID_MIN                0.0
#define    CCMC_VARIABLE_NAME_BLOCK_CHILD_ID_7_VALID_MAX                1000000.0
#define    CCMC_VARIABLE_NAME_BLOCK_CHILD_ID_7_UNITS                    "N/A"
#define    CCMC_VARIABLE_NAME_BLOCK_CHILD_ID_7_GRID_SYSTEM                "N/A"
#define CCMC_VARIABLE_NAME_BLOCK_CHILD_ID_7_MASK                    INT_MASK
#define    CCMC_VARIABLE_NAME_BLOCK_CHILD_ID_7_DESCRIPTION                "Child 7 Block ID for current block, -1 would be invalid/empty - arrays prefilled with -1"
#define    CCMC_VARIABLE_NAME_BLOCK_CHILD_ID_7_IS_VECTOR                0
#define    CCMC_VARIABLE_NAME_BLOCK_CHILD_ID_7_POSITION_GRID_SYSTEM    "N/A"
#define    CCMC_VARIABLE_NAME_BLOCK_CHILD_ID_7_DATA_GRID_SYSTEM        "N/A"

#define CCMC_VARIABLE_NAME_BLOCK_CHILD_ID_8                            "block_child_id_8"
#define CCMC_VARIABLE_NAME_BLOCK_CHILD_ID_8_DATA_TYPE                "int"
#define    CCMC_VARIABLE_NAME_BLOCK_CHILD_ID_8_CLASSIFICATION            "grid"
#define    CCMC_VARIABLE_NAME_BLOCK_CHILD_ID_8_VALID_MIN                0.0
#define    CCMC_VARIABLE_NAME_BLOCK_CHILD_ID_8_VALID_MAX                1000000.0
#define    CCMC_VARIABLE_NAME_BLOCK_CHILD_ID_8_UNITS                    "N/A"
#define    CCMC_VARIABLE_NAME_BLOCK_CHILD_ID_8_GRID_SYSTEM                "N/A"
#define CCMC_VARIABLE_NAME_BLOCK_CHILD_ID_8_MASK                    INT_MASK
#define    CCMC_VARIABLE_NAME_BLOCK_CHILD_ID_8_DESCRIPTION                "Child 8 Block ID for current block, -1 would be invalid/empty - arrays prefilled with -1"
#define    CCMC_VARIABLE_NAME_BLOCK_CHILD_ID_8_IS_VECTOR                0
#define    CCMC_VARIABLE_NAME_BLOCK_CHILD_ID_8_POSITION_GRID_SYSTEM    "N/A"
#define    CCMC_VARIABLE_NAME_BLOCK_CHILD_ID_8_DATA_GRID_SYSTEM        "N/A"

#define CCMC_VARIABLE_NAME_WEIGHTS                        "weights"
#define CCMC_VARIABLE_NAME_WEIGHTS_DATA_TYPE                "float"
#define CCMC_VARIABLE_NAME_WEIGHTS_CLASSIFICATION            "data"
#define CCMC_VARIABLE_NAME_WEIGHTS_VALID_MIN                -1000000000.0
#define CCMC_VARIABLE_NAME_WEIGHTS_VALID_MAX                1000000000.0
#define CCMC_VARIABLE_NAME_WEIGHTS_UNITS                    " "
#define CCMC_VARIABLE_NAME_WEIGHTS_GRID_SYSTEM                "grid_system_1"
#define CCMC_VARIABLE_NAME_WEIGHTS_MASK                    FLOAT_MASK
#define CCMC_VARIABLE_NAME_WEIGHTS_DESCRIPTION                "Weight Values"
#define CCMC_VARIABLE_NAME_WEIGHTS_IS_VECTOR                0
#define CCMC_VARIABLE_NAME_WEIGHTS_POSITION_GRID_SYSTEM        "grid_system_1"
#define CCMC_VARIABLE_NAME_WEIGHTS_DATA_GRID_SYSTEM        "grid_system_1"


#define CCMC_VARIABLE_NAME_STATUS                        "status"
#define CCMC_VARIABLE_NAME_STATUS_DATA_TYPE                "float"
#define CCMC_VARIABLE_NAME_STATUS_CLASSIFICATION            "data"
#define CCMC_VARIABLE_NAME_STATUS_VALID_MIN                -1000000000.0
#define CCMC_VARIABLE_NAME_STATUS_VALID_MAX                1000000000.0
#define CCMC_VARIABLE_NAME_STATUS_UNITS                    " "
#define CCMC_VARIABLE_NAME_STATUS_GRID_SYSTEM                "N/A"
#define CCMC_VARIABLE_NAME_STATUS_MASK                    FLOAT_MASK
#define CCMC_VARIABLE_NAME_STATUS_DESCRIPTION                "status"
#define CCMC_VARIABLE_NAME_STATUS_IS_VECTOR                0
#define CCMC_VARIABLE_NAME_STATUS_POSITION_GRID_SYSTEM        "N/A"
#define CCMC_VARIABLE_NAME_STATUS_DATA_GRID_SYSTEM        "N/A"




/************************************* TIME RELATED VARIABLE NAMES ***********************************************/

#define CCMC_VARIABLE_NAME_TIME_1                            "time_1"
#define CCMC_VARIABLE_NAME_TIME_1_DATA_TYPE                "float"
#define CCMC_VARIABLE_NAME_TIME_1_CLASSIFICATION            "data"
#define CCMC_VARIABLE_NAME_TIME_1_VALID_MIN                -1000000000.0
#define CCMC_VARIABLE_NAME_TIME_1_VALID_MAX                1000000000.0
#define CCMC_VARIABLE_NAME_TIME_1_UNITS                    "s"
#define CCMC_VARIABLE_NAME_TIME_1_GRID_SYSTEM                "grid_system_3"
#define CCMC_VARIABLE_NAME_TIME_1_MASK                        FLOAT_MASK
#define CCMC_VARIABLE_NAME_TIME_1_DESCRIPTION                "Physical Time Variable for time variant data"
#define CCMC_VARIABLE_NAME_TIME_1_IS_VECTOR                0
#define CCMC_VARIABLE_NAME_TIME_1_POSITION_GRID_SYSTEM        "grid_system_3"
#define CCMC_VARIABLE_NAME_TIME_1_DATA_GRID_SYSTEM            "grid_system_3"

#define CCMC_VARIABLE_NAME_TIME_2                            "time_2"
#define CCMC_VARIABLE_NAME_TIME_2_DATA_TYPE                "float"
#define CCMC_VARIABLE_NAME_TIME_2_CLASSIFICATION            "data"
#define CCMC_VARIABLE_NAME_TIME_2_VALID_MIN                -1000000000.0
#define CCMC_VARIABLE_NAME_TIME_2_VALID_MAX                1000000000.0
#define CCMC_VARIABLE_NAME_TIME_2_UNITS                    "s"
#define CCMC_VARIABLE_NAME_TIME_2_GRID_SYSTEM                "grid_system_4"
#define CCMC_VARIABLE_NAME_TIME_2_MASK                        FLOAT_MASK
#define CCMC_VARIABLE_NAME_TIME_2_DESCRIPTION                "Physical Time Variable for time variant data"
#define CCMC_VARIABLE_NAME_TIME_2_IS_VECTOR                0
#define CCMC_VARIABLE_NAME_TIME_2_POSITION_GRID_SYSTEM        "grid_system_4"
#define CCMC_VARIABLE_NAME_TIME_2_DATA_GRID_SYSTEM            "grid_system_4"

#define CCMC_VARIABLE_NAME_TIME_3                            "time_3"
#define CCMC_VARIABLE_NAME_TIME_3_DATA_TYPE                "float"
#define CCMC_VARIABLE_NAME_TIME_3_CLASSIFICATION            "data"
#define CCMC_VARIABLE_NAME_TIME_3_VALID_MIN                -1000000000.0
#define CCMC_VARIABLE_NAME_TIME_3_VALID_MAX                1000000000.0
#define CCMC_VARIABLE_NAME_TIME_3_UNITS                    "s"
#define CCMC_VARIABLE_NAME_TIME_3_GRID_SYSTEM                "grid_system_5"
#define CCMC_VARIABLE_NAME_TIME_3_MASK                        FLOAT_MASK
#define CCMC_VARIABLE_NAME_TIME_3_DESCRIPTION                "Physical Time Variable for time variant data"
#define CCMC_VARIABLE_NAME_TIME_3_IS_VECTOR                0
#define CCMC_VARIABLE_NAME_TIME_3_POSITION_GRID_SYSTEM        "grid_system_5"
#define CCMC_VARIABLE_NAME_TIME_3_DATA_GRID_SYSTEM            "grid_system_5"

#define CCMC_VARIABLE_NAME_TIME_4                            "time_4"
#define CCMC_VARIABLE_NAME_TIME_4_DATA_TYPE                "float"
#define CCMC_VARIABLE_NAME_TIME_4_CLASSIFICATION            "data"
#define CCMC_VARIABLE_NAME_TIME_4_VALID_MIN                -1000000000.0
#define CCMC_VARIABLE_NAME_TIME_4_VALID_MAX                1000000000.0
#define CCMC_VARIABLE_NAME_TIME_4_UNITS                    "s"
#define CCMC_VARIABLE_NAME_TIME_4_GRID_SYSTEM                "grid_system_6"
#define CCMC_VARIABLE_NAME_TIME_4_MASK                        FLOAT_MASK
#define CCMC_VARIABLE_NAME_TIME_4_DESCRIPTION                "Physical Time Variable for time variant data"
#define CCMC_VARIABLE_NAME_TIME_4_IS_VECTOR                0
#define CCMC_VARIABLE_NAME_TIME_4_POSITION_GRID_SYSTEM        "grid_system_6"
#define CCMC_VARIABLE_NAME_TIME_4_DATA_GRID_SYSTEM            "grid_system_6"

#define CCMC_VARIABLE_NAME_TIME_STEP_1                            "time_step_1"
#define CCMC_VARIABLE_NAME_TIME_STEP_1_DATA_TYPE                "float"
#define CCMC_VARIABLE_NAME_TIME_STEP_1_CLASSIFICATION            "data"
#define CCMC_VARIABLE_NAME_TIME_STEP_1_VALID_MIN                -1000000000.0
#define CCMC_VARIABLE_NAME_TIME_STEP_1_VALID_MAX                1000000000.0
#define CCMC_VARIABLE_NAME_TIME_STEP_1_UNITS                    "s"
#define CCMC_VARIABLE_NAME_TIME_STEP_1_GRID_SYSTEM                "grid_system_3"
#define CCMC_VARIABLE_NAME_TIME_STEP_1_MASK                    FLOAT_MASK
#define CCMC_VARIABLE_NAME_TIME_STEP_1_DESCRIPTION                "Time StepVariable for time variant data"
#define CCMC_VARIABLE_NAME_TIME_STEP_1_IS_VECTOR                0
#define CCMC_VARIABLE_NAME_TIME_STEP_1_POSITION_GRID_SYSTEM        "grid_system_3"
#define CCMC_VARIABLE_NAME_TIME_STEP_1_DATA_GRID_SYSTEM            "grid_system_3"

#define CCMC_VARIABLE_NAME_TIME_STEP_2                            "time_step_2"
#define CCMC_VARIABLE_NAME_TIME_STEP_2_DATA_TYPE                "float"
#define CCMC_VARIABLE_NAME_TIME_STEP_2_CLASSIFICATION            "data"
#define CCMC_VARIABLE_NAME_TIME_STEP_2_VALID_MIN                -1000000000.0
#define CCMC_VARIABLE_NAME_TIME_STEP_2_VALID_MAX                1000000000.0
#define CCMC_VARIABLE_NAME_TIME_STEP_2_UNITS                    "s"
#define CCMC_VARIABLE_NAME_TIME_STEP_2_GRID_SYSTEM                "grid_system_4"
#define CCMC_VARIABLE_NAME_TIME_STEP_2_MASK                    FLOAT_MASK
#define CCMC_VARIABLE_NAME_TIME_STEP_2_DESCRIPTION                "Time Step Variable for time variant data"
#define CCMC_VARIABLE_NAME_TIME_STEP_2_IS_VECTOR                0
#define CCMC_VARIABLE_NAME_TIME_STEP_2_POSITION_GRID_SYSTEM        "grid_system_4"
#define CCMC_VARIABLE_NAME_TIME_STEP_2_DATA_GRID_SYSTEM            "grid_system_4"

#define CCMC_VARIABLE_NAME_TIME_STEP_3                            "time_step_3"
#define CCMC_VARIABLE_NAME_TIME_STEP_3_DATA_TYPE                "float"
#define CCMC_VARIABLE_NAME_TIME_STEP_3_CLASSIFICATION            "data"
#define CCMC_VARIABLE_NAME_TIME_STEP_3_VALID_MIN                -1000000000.0
#define CCMC_VARIABLE_NAME_TIME_STEP_3_VALID_MAX                1000000000.0
#define CCMC_VARIABLE_NAME_TIME_STEP_3_UNITS                    "s"
#define CCMC_VARIABLE_NAME_TIME_STEP_3_GRID_SYSTEM                "grid_system_5"
#define CCMC_VARIABLE_NAME_TIME_STEP_3_MASK                    FLOAT_MASK
#define CCMC_VARIABLE_NAME_TIME_STEP_3_DESCRIPTION                "Time Step Variable for time variant data"
#define CCMC_VARIABLE_NAME_TIME_STEP_3_IS_VECTOR                0
#define CCMC_VARIABLE_NAME_TIME_STEP_3_POSITION_GRID_SYSTEM        "grid_system_5"
#define CCMC_VARIABLE_NAME_TIME_STEP_3_DATA_GRID_SYSTEM            "grid_system_5"

#define CCMC_VARIABLE_NAME_TIME_STEP_4                            "time_step_4"
#define CCMC_VARIABLE_NAME_TIME_STEP_4_DATA_TYPE                "float"
#define CCMC_VARIABLE_NAME_TIME_STEP_4_CLASSIFICATION            "data"
#define CCMC_VARIABLE_NAME_TIME_STEP_4_VALID_MIN                -1000000000.0
#define CCMC_VARIABLE_NAME_TIME_STEP_4_VALID_MAX                1000000000.0
#define CCMC_VARIABLE_NAME_TIME_STEP_4_UNITS                    "s"
#define CCMC_VARIABLE_NAME_TIME_STEP_4_GRID_SYSTEM                "grid_system_6"
#define CCMC_VARIABLE_NAME_TIME_STEP_4_MASK                    FLOAT_MASK
#define CCMC_VARIABLE_NAME_TIME_STEP_4_DESCRIPTION                "Time Step Variable for time variant data"
#define CCMC_VARIABLE_NAME_TIME_STEP_4_IS_VECTOR                0
#define CCMC_VARIABLE_NAME_TIME_STEP_4_POSITION_GRID_SYSTEM        "grid_system_6"
#define CCMC_VARIABLE_NAME_TIME_STEP_4_DATA_GRID_SYSTEM            "grid_system_6"

#define CCMC_VARIABLE_NAME_NUMERICAL_STEP_1                        "numerical_step_1"
#define CCMC_VARIABLE_NAME_NUMERICAL_STEP_1_DATA_TYPE                "int"
#define CCMC_VARIABLE_NAME_NUMERICAL_STEP_1_CLASSIFICATION            "data"
#define CCMC_VARIABLE_NAME_NUMERICAL_STEP_1_VALID_MIN                0
#define CCMC_VARIABLE_NAME_NUMERICAL_STEP_1_VALID_MAX                1000000000
#define CCMC_VARIABLE_NAME_NUMERICAL_STEP_1_UNITS                    " "
#define CCMC_VARIABLE_NAME_NUMERICAL_STEP_1_GRID_SYSTEM                "grid_system_3"
#define CCMC_VARIABLE_NAME_NUMERICAL_STEP_1_MASK                    INT_MASK
#define CCMC_VARIABLE_NAME_NUMERICAL_STEP_1_DESCRIPTION                "Numerical Step Number Variable for time variant data"
#define CCMC_VARIABLE_NAME_NUMERICAL_STEP_1_IS_VECTOR                0
#define CCMC_VARIABLE_NAME_NUMERICAL_STEP_1_POSITION_GRID_SYSTEM    "grid_system_3"
#define CCMC_VARIABLE_NAME_NUMERICAL_STEP_1_DATA_GRID_SYSTEM        "grid_system_3"

#define CCMC_VARIABLE_NAME_NUMERICAL_STEP_2                        "numerical_step_2"
#define CCMC_VARIABLE_NAME_NUMERICAL_STEP_2_DATA_TYPE                "int"
#define CCMC_VARIABLE_NAME_NUMERICAL_STEP_2_CLASSIFICATION            "data"
#define CCMC_VARIABLE_NAME_NUMERICAL_STEP_2_VALID_MIN                0
#define CCMC_VARIABLE_NAME_NUMERICAL_STEP_2_VALID_MAX                1000000000
#define CCMC_VARIABLE_NAME_NUMERICAL_STEP_2_UNITS                    " "
#define CCMC_VARIABLE_NAME_NUMERICAL_STEP_2_GRID_SYSTEM                "grid_system_4"
#define CCMC_VARIABLE_NAME_NUMERICAL_STEP_2_MASK                    INT_MASK
#define CCMC_VARIABLE_NAME_NUMERICAL_STEP_2_DESCRIPTION                "Numerical Step Number Variable for time variant data"
#define CCMC_VARIABLE_NAME_NUMERICAL_STEP_2_IS_VECTOR                0
#define CCMC_VARIABLE_NAME_NUMERICAL_STEP_2_POSITION_GRID_SYSTEM    "grid_system_4"
#define CCMC_VARIABLE_NAME_NUMERICAL_STEP_2_DATA_GRID_SYSTEM        "grid_system_4"

#define CCMC_VARIABLE_NAME_NUMERICAL_STEP_3                        "numerical_step_3"
#define CCMC_VARIABLE_NAME_NUMERICAL_STEP_3_DATA_TYPE                "int"
#define CCMC_VARIABLE_NAME_NUMERICAL_STEP_3_CLASSIFICATION            "data"
#define CCMC_VARIABLE_NAME_NUMERICAL_STEP_3_VALID_MIN                0
#define CCMC_VARIABLE_NAME_NUMERICAL_STEP_3_VALID_MAX                1000000000
#define CCMC_VARIABLE_NAME_NUMERICAL_STEP_3_UNITS                    " "
#define CCMC_VARIABLE_NAME_NUMERICAL_STEP_3_GRID_SYSTEM                "grid_system_5"
#define CCMC_VARIABLE_NAME_NUMERICAL_STEP_3_MASK                    INT_MASK
#define CCMC_VARIABLE_NAME_NUMERICAL_STEP_3_DESCRIPTION                "Numerical Step Number for time variant data"
#define CCMC_VARIABLE_NAME_NUMERICAL_STEP_3_IS_VECTOR                0
#define CCMC_VARIABLE_NAME_NUMERICAL_STEP_3_POSITION_GRID_SYSTEM    "grid_system_5"
#define CCMC_VARIABLE_NAME_NUMERICAL_STEP_3_DATA_GRID_SYSTEM        "grid_system_5"

#define CCMC_VARIABLE_NAME_NUMERICAL_STEP_4                        "numerical_step_4"
#define CCMC_VARIABLE_NAME_NUMERICAL_STEP_4_DATA_TYPE                "int"
#define CCMC_VARIABLE_NAME_NUMERICAL_STEP_4_CLASSIFICATION            "data"
#define CCMC_VARIABLE_NAME_NUMERICAL_STEP_4_VALID_MIN                0
#define CCMC_VARIABLE_NAME_NUMERICAL_STEP_4_VALID_MAX                1000000000
#define CCMC_VARIABLE_NAME_NUMERICAL_STEP_4_UNITS                    " "
#define CCMC_VARIABLE_NAME_NUMERICAL_STEP_4_GRID_SYSTEM                "grid_system_6"
#define CCMC_VARIABLE_NAME_NUMERICAL_STEP_4_MASK                    INT_MASK
#define CCMC_VARIABLE_NAME_NUMERICAL_STEP_4_DESCRIPTION                "Numerical Step Number for time variant data"
#define CCMC_VARIABLE_NAME_NUMERICAL_STEP_4_IS_VECTOR                0
#define CCMC_VARIABLE_NAME_NUMERICAL_STEP_4_POSITION_GRID_SYSTEM    "grid_system_6"
#define CCMC_VARIABLE_NAME_NUMERICAL_STEP_4_DATA_GRID_SYSTEM        "grid_system_6"

/******************************* BUILD Model Specific Arrays to export using defined ccmc registered variables *******************/

#define NUMBER_REGISTERED_BATSRUS_VARIABLES                    40
#define NUMBER_REGISTERED_ORIGINAL_FLOAT_BATSRUS_VARIABLES         18
#define NUMBER_REGISTERED_GRID_INT_BATSRUS_VARIABLES             10
#define NUMBER_REGISTERED_GRID_INT_BATSRUS_VARIABLES_2             1
#define NUMBER_REGISTERED_GRID_FLOAT_BATSRUS_VARIABLES             11

#define NUMBER_REGISTERED_UCLA_GGCM_VARIABLES                    27
#define NUMBER_REGISTERED_ORIGINAL_FLOAT_UCLA_GGCM_VARIABLES     15
#define NUMBER_REGISTERED_GRID_X_FLOAT_UCLA_GGCM_VARIABLES        4
#define NUMBER_REGISTERED_GRID_Y_FLOAT_UCLA_GGCM_VARIABLES        4
#define NUMBER_REGISTERED_GRID_Z_FLOAT_UCLA_GGCM_VARIABLES        4

#define NUMBER_REGISTERED_CTIP_VARIABLES                         17

/*    added three variables b1x,b1y,b1z to enlil count to insert
 *     corrected polarity of bx,by,bz based on bp directly into
 *     the CDF file - therofore it will not have to be calculated
 *     inside the interp lib
 */

/* 10.2006 added 12 EV* time values */

#define NUMBER_REGISTERED_ENLIL_VARIABLES                        83
#define NUMBER_REGISTERED_ENLIL_TIME_VARIABLES                    16
#define NUMBER_REGISTERED_ENLIL_EVH_VARIABLES                    13
#define NUMBER_REGISTERED_ENLIL_EVL_VARIABLES                    13
#define NUMBER_REGISTERED_ENLIL_EVG_VARIABLES                    13
#define NUMBER_REGISTERED_ENLIL_EVP_VARIABLES                    13
#define NUMBER_REGISTERED_ENLIL_CCMC_TIME_VARIABLES            3
#define NUMBER_REGISTERED_EV_TIME_VARIABLES                    16

#define NUMBER_REGISTERED_KPVT_VARIABLES                         5

#define NUMBER_REGISTERED_MSFC_TVM_VARIABLES                     5

#define NUMBER_REGISTERED_MAS_VARIABLES                         18

/**************************************************************** BATSRUS *******************************************************/

struct registered_batsrus_variables
{

   char *ccmc_var_name;
   char *ccmc_var_data_type;
   char *ccmc_var_classification;
   float valid_min;
   float valid_max;
   char *units;
   char *grid_system;
   float mask;
   char *description;
   int is_vector_component;
   char *position_grid_system;
   char *data_grid_system;
}static registered_batsrus_var_list[] =
{

      CCMC_VARIABLE_NAME_X,
      CCMC_VARIABLE_NAME_X_DATA_TYPE,
      CCMC_VARIABLE_NAME_X_CLASSIFICATION,
      CCMC_VARIABLE_NAME_X_VALID_MIN,
      CCMC_VARIABLE_NAME_X_VALID_MAX,
      CCMC_VARIABLE_NAME_X_UNITS,
      CCMC_VARIABLE_NAME_X_GRID_SYSTEM,
      CCMC_VARIABLE_NAME_X_MASK,
      CCMC_VARIABLE_NAME_X_DESCRIPTION,
      CCMC_VARIABLE_NAME_X_IS_VECTOR,
      CCMC_VARIABLE_NAME_X_POSITION_GRID_SYSTEM,
      CCMC_VARIABLE_NAME_X_DATA_GRID_SYSTEM,
      CCMC_VARIABLE_NAME_Y,
      CCMC_VARIABLE_NAME_Y_DATA_TYPE,
      CCMC_VARIABLE_NAME_Y_CLASSIFICATION,
      CCMC_VARIABLE_NAME_Y_VALID_MIN,
      CCMC_VARIABLE_NAME_Y_VALID_MAX,
      CCMC_VARIABLE_NAME_Y_UNITS,
      CCMC_VARIABLE_NAME_Y_GRID_SYSTEM,
      CCMC_VARIABLE_NAME_Y_MASK,
      CCMC_VARIABLE_NAME_Y_DESCRIPTION,
      CCMC_VARIABLE_NAME_Y_IS_VECTOR,
      CCMC_VARIABLE_NAME_Y_POSITION_GRID_SYSTEM,
      CCMC_VARIABLE_NAME_Y_DATA_GRID_SYSTEM,
      CCMC_VARIABLE_NAME_Z,
      CCMC_VARIABLE_NAME_Z_DATA_TYPE,
      CCMC_VARIABLE_NAME_Z_CLASSIFICATION,
      CCMC_VARIABLE_NAME_Z_VALID_MIN,
      CCMC_VARIABLE_NAME_Z_VALID_MAX,
      CCMC_VARIABLE_NAME_Z_UNITS,
      CCMC_VARIABLE_NAME_Z_GRID_SYSTEM,
      CCMC_VARIABLE_NAME_Z_MASK,
      CCMC_VARIABLE_NAME_Z_DESCRIPTION,
      CCMC_VARIABLE_NAME_Z_IS_VECTOR,
      CCMC_VARIABLE_NAME_Z_POSITION_GRID_SYSTEM,
      CCMC_VARIABLE_NAME_Z_DATA_GRID_SYSTEM,
      CCMC_VARIABLE_NAME_BX,
      CCMC_VARIABLE_NAME_BX_DATA_TYPE,
      CCMC_VARIABLE_NAME_BX_CLASSIFICATION,
      CCMC_VARIABLE_NAME_BX_VALID_MIN,
      CCMC_VARIABLE_NAME_BX_VALID_MAX,
      CCMC_VARIABLE_NAME_BX_UNITS,
      CCMC_VARIABLE_NAME_BX_GRID_SYSTEM,
      CCMC_VARIABLE_NAME_BX_MASK,
      CCMC_VARIABLE_NAME_BX_DESCRIPTION,
      CCMC_VARIABLE_NAME_BX_IS_VECTOR,
      CCMC_VARIABLE_NAME_BX_POSITION_GRID_SYSTEM,
      CCMC_VARIABLE_NAME_BX_DATA_GRID_SYSTEM,
      CCMC_VARIABLE_NAME_BY,
      CCMC_VARIABLE_NAME_BY_DATA_TYPE,
      CCMC_VARIABLE_NAME_BY_CLASSIFICATION,
      CCMC_VARIABLE_NAME_BY_VALID_MIN,
      CCMC_VARIABLE_NAME_BY_VALID_MAX,
      CCMC_VARIABLE_NAME_BY_UNITS,
      CCMC_VARIABLE_NAME_BY_GRID_SYSTEM,
      CCMC_VARIABLE_NAME_BY_MASK,
      CCMC_VARIABLE_NAME_BY_DESCRIPTION,
      CCMC_VARIABLE_NAME_BY_IS_VECTOR,
      CCMC_VARIABLE_NAME_BY_POSITION_GRID_SYSTEM,
      CCMC_VARIABLE_NAME_BY_DATA_GRID_SYSTEM,
      CCMC_VARIABLE_NAME_BZ,
      CCMC_VARIABLE_NAME_BZ_DATA_TYPE,
      CCMC_VARIABLE_NAME_BZ_CLASSIFICATION,
      CCMC_VARIABLE_NAME_BZ_VALID_MIN,
      CCMC_VARIABLE_NAME_BZ_VALID_MAX,
      CCMC_VARIABLE_NAME_BZ_UNITS,
      CCMC_VARIABLE_NAME_BZ_GRID_SYSTEM,
      CCMC_VARIABLE_NAME_BZ_MASK,
      CCMC_VARIABLE_NAME_BZ_DESCRIPTION,
      CCMC_VARIABLE_NAME_BZ_IS_VECTOR,
      CCMC_VARIABLE_NAME_BZ_POSITION_GRID_SYSTEM,
      CCMC_VARIABLE_NAME_BZ_DATA_GRID_SYSTEM,
      CCMC_VARIABLE_NAME_B1X,
      CCMC_VARIABLE_NAME_B1X_DATA_TYPE,
      CCMC_VARIABLE_NAME_B1X_CLASSIFICATION,
      CCMC_VARIABLE_NAME_B1X_VALID_MIN,
      CCMC_VARIABLE_NAME_B1X_VALID_MAX,
      CCMC_VARIABLE_NAME_B1X_UNITS,
      CCMC_VARIABLE_NAME_B1X_GRID_SYSTEM,
      CCMC_VARIABLE_NAME_B1X_MASK,
      CCMC_VARIABLE_NAME_B1X_DESCRIPTION,
      CCMC_VARIABLE_NAME_B1X_IS_VECTOR,
      CCMC_VARIABLE_NAME_B1X_POSITION_GRID_SYSTEM,
      CCMC_VARIABLE_NAME_B1X_DATA_GRID_SYSTEM,
      CCMC_VARIABLE_NAME_B1Y,
      CCMC_VARIABLE_NAME_B1Y_DATA_TYPE,
      CCMC_VARIABLE_NAME_B1Y_CLASSIFICATION,
      CCMC_VARIABLE_NAME_B1Y_VALID_MIN,
      CCMC_VARIABLE_NAME_B1Y_VALID_MAX,
      CCMC_VARIABLE_NAME_B1Y_UNITS,
      CCMC_VARIABLE_NAME_B1Y_GRID_SYSTEM,
      CCMC_VARIABLE_NAME_B1Y_MASK,
      CCMC_VARIABLE_NAME_B1Y_DESCRIPTION,
      CCMC_VARIABLE_NAME_B1Y_IS_VECTOR,
      CCMC_VARIABLE_NAME_B1Y_POSITION_GRID_SYSTEM,
      CCMC_VARIABLE_NAME_B1Y_DATA_GRID_SYSTEM,
      CCMC_VARIABLE_NAME_B1Z,
      CCMC_VARIABLE_NAME_B1Z_DATA_TYPE,
      CCMC_VARIABLE_NAME_B1Z_CLASSIFICATION,
      CCMC_VARIABLE_NAME_B1Z_VALID_MIN,
      CCMC_VARIABLE_NAME_B1Z_VALID_MAX,
      CCMC_VARIABLE_NAME_B1Z_UNITS,
      CCMC_VARIABLE_NAME_B1Z_GRID_SYSTEM,
      CCMC_VARIABLE_NAME_B1Z_MASK,
      CCMC_VARIABLE_NAME_B1Z_DESCRIPTION,
      CCMC_VARIABLE_NAME_B1Z_IS_VECTOR,
      CCMC_VARIABLE_NAME_B1Z_POSITION_GRID_SYSTEM,
      CCMC_VARIABLE_NAME_B1Z_DATA_GRID_SYSTEM,
      CCMC_VARIABLE_NAME_UX,
      CCMC_VARIABLE_NAME_UX_DATA_TYPE,
      CCMC_VARIABLE_NAME_UX_CLASSIFICATION,
      CCMC_VARIABLE_NAME_UX_VALID_MIN,
      CCMC_VARIABLE_NAME_UX_VALID_MAX,
      CCMC_VARIABLE_NAME_UX_UNITS,
      CCMC_VARIABLE_NAME_UX_GRID_SYSTEM,
      CCMC_VARIABLE_NAME_UX_MASK,
      CCMC_VARIABLE_NAME_UX_DESCRIPTION,
      CCMC_VARIABLE_NAME_UX_IS_VECTOR,
      CCMC_VARIABLE_NAME_UX_POSITION_GRID_SYSTEM,
      CCMC_VARIABLE_NAME_UX_DATA_GRID_SYSTEM,
      CCMC_VARIABLE_NAME_UY,
      CCMC_VARIABLE_NAME_UY_DATA_TYPE,
      CCMC_VARIABLE_NAME_UY_CLASSIFICATION,
      CCMC_VARIABLE_NAME_UY_VALID_MIN,
      CCMC_VARIABLE_NAME_UY_VALID_MAX,
      CCMC_VARIABLE_NAME_UY_UNITS,
      CCMC_VARIABLE_NAME_UY_GRID_SYSTEM,
      CCMC_VARIABLE_NAME_UY_MASK,
      CCMC_VARIABLE_NAME_UY_DESCRIPTION,
      CCMC_VARIABLE_NAME_UY_IS_VECTOR,
      CCMC_VARIABLE_NAME_UY_POSITION_GRID_SYSTEM,
      CCMC_VARIABLE_NAME_UY_DATA_GRID_SYSTEM,
      CCMC_VARIABLE_NAME_UZ,
      CCMC_VARIABLE_NAME_UZ_DATA_TYPE,
      CCMC_VARIABLE_NAME_UZ_CLASSIFICATION,
      CCMC_VARIABLE_NAME_UZ_VALID_MIN,
      CCMC_VARIABLE_NAME_UZ_VALID_MAX,
      CCMC_VARIABLE_NAME_UZ_UNITS,
      CCMC_VARIABLE_NAME_UZ_GRID_SYSTEM,
      CCMC_VARIABLE_NAME_UZ_MASK,
      CCMC_VARIABLE_NAME_UZ_DESCRIPTION,
      CCMC_VARIABLE_NAME_UZ_IS_VECTOR,
      CCMC_VARIABLE_NAME_UZ_POSITION_GRID_SYSTEM,
      CCMC_VARIABLE_NAME_UZ_DATA_GRID_SYSTEM,
      CCMC_VARIABLE_NAME_JX,
      CCMC_VARIABLE_NAME_JX_DATA_TYPE,
      CCMC_VARIABLE_NAME_JX_CLASSIFICATION,
      CCMC_VARIABLE_NAME_JX_VALID_MIN,
      CCMC_VARIABLE_NAME_JX_VALID_MAX,
      CCMC_VARIABLE_NAME_JX_UNITS,
      CCMC_VARIABLE_NAME_JX_GRID_SYSTEM,
      CCMC_VARIABLE_NAME_JX_MASK,
      CCMC_VARIABLE_NAME_JX_DESCRIPTION,
      CCMC_VARIABLE_NAME_JX_IS_VECTOR,
      CCMC_VARIABLE_NAME_JX_POSITION_GRID_SYSTEM,
      CCMC_VARIABLE_NAME_JZ_DATA_GRID_SYSTEM,
      CCMC_VARIABLE_NAME_JY,
      CCMC_VARIABLE_NAME_JY_DATA_TYPE,
      CCMC_VARIABLE_NAME_JY_CLASSIFICATION,
      CCMC_VARIABLE_NAME_JY_VALID_MIN,
      CCMC_VARIABLE_NAME_JY_VALID_MAX,
      CCMC_VARIABLE_NAME_JY_UNITS,
      CCMC_VARIABLE_NAME_JY_GRID_SYSTEM,
      CCMC_VARIABLE_NAME_JY_MASK,
      CCMC_VARIABLE_NAME_JY_DESCRIPTION,
      CCMC_VARIABLE_NAME_JY_IS_VECTOR,
      CCMC_VARIABLE_NAME_JY_POSITION_GRID_SYSTEM,
      CCMC_VARIABLE_NAME_JY_DATA_GRID_SYSTEM,
      CCMC_VARIABLE_NAME_JZ,
      CCMC_VARIABLE_NAME_JZ_DATA_TYPE,
      CCMC_VARIABLE_NAME_JZ_CLASSIFICATION,
      CCMC_VARIABLE_NAME_JZ_VALID_MIN,
      CCMC_VARIABLE_NAME_JZ_VALID_MAX,
      CCMC_VARIABLE_NAME_JZ_UNITS,
      CCMC_VARIABLE_NAME_JZ_GRID_SYSTEM,
      CCMC_VARIABLE_NAME_JZ_MASK,
      CCMC_VARIABLE_NAME_JZ_DESCRIPTION,
      CCMC_VARIABLE_NAME_JZ_IS_VECTOR,
      CCMC_VARIABLE_NAME_JZ_POSITION_GRID_SYSTEM,
      CCMC_VARIABLE_NAME_JZ_DATA_GRID_SYSTEM,
      CCMC_VARIABLE_NAME_RHO,
      CCMC_VARIABLE_NAME_RHO_DATA_TYPE,
      CCMC_VARIABLE_NAME_RHO_CLASSIFICATION,
      CCMC_VARIABLE_NAME_RHO_VALID_MIN,
      CCMC_VARIABLE_NAME_RHO_VALID_MAX,
      CCMC_VARIABLE_NAME_RHO_UNITS,
      CCMC_VARIABLE_NAME_RHO_GRID_SYSTEM,
      CCMC_VARIABLE_NAME_RHO_MASK,
      CCMC_VARIABLE_NAME_RHO_DESCRIPTION,
      CCMC_VARIABLE_NAME_RHO_IS_VECTOR,
      CCMC_VARIABLE_NAME_RHO_POSITION_GRID_SYSTEM,
      CCMC_VARIABLE_NAME_RHO_DATA_GRID_SYSTEM,
      CCMC_VARIABLE_NAME_P,
      CCMC_VARIABLE_NAME_P_DATA_TYPE,
      CCMC_VARIABLE_NAME_P_CLASSIFICATION,
      CCMC_VARIABLE_NAME_P_VALID_MIN,
      CCMC_VARIABLE_NAME_P_VALID_MAX,
      CCMC_VARIABLE_NAME_P_UNITS,
      CCMC_VARIABLE_NAME_P_GRID_SYSTEM,
      CCMC_VARIABLE_NAME_P_MASK,
      CCMC_VARIABLE_NAME_P_DESCRIPTION,
      CCMC_VARIABLE_NAME_P_IS_VECTOR,
      CCMC_VARIABLE_NAME_P_POSITION_GRID_SYSTEM,
      CCMC_VARIABLE_NAME_P_DATA_GRID_SYSTEM,
      CCMC_VARIABLE_NAME_E,
      CCMC_VARIABLE_NAME_E_DATA_TYPE,
      CCMC_VARIABLE_NAME_E_CLASSIFICATION,
      CCMC_VARIABLE_NAME_E_VALID_MIN,
      CCMC_VARIABLE_NAME_E_VALID_MAX,
      CCMC_VARIABLE_NAME_E_UNITS,
      CCMC_VARIABLE_NAME_E_GRID_SYSTEM,
      CCMC_VARIABLE_NAME_E_MASK,
      CCMC_VARIABLE_NAME_E_DESCRIPTION,
      CCMC_VARIABLE_NAME_E_IS_VECTOR,
      CCMC_VARIABLE_NAME_E_POSITION_GRID_SYSTEM,
      CCMC_VARIABLE_NAME_E_DATA_GRID_SYSTEM,
      CCMC_VARIABLE_NAME_BLOCK_AMR_LEVEL,
      CCMC_VARIABLE_NAME_BLOCK_AMR_LEVEL_DATA_TYPE,
      CCMC_VARIABLE_NAME_BLOCK_AMR_LEVEL_CLASSIFICATION,
      CCMC_VARIABLE_NAME_BLOCK_AMR_LEVEL_VALID_MIN,
      CCMC_VARIABLE_NAME_BLOCK_AMR_LEVEL_VALID_MAX,
      CCMC_VARIABLE_NAME_BLOCK_AMR_LEVEL_UNITS,
      CCMC_VARIABLE_NAME_BLOCK_AMR_LEVEL_GRID_SYSTEM,
      CCMC_VARIABLE_NAME_BLOCK_AMR_LEVEL_MASK,
      CCMC_VARIABLE_NAME_BLOCK_AMR_LEVEL_DESCRIPTION,
      CCMC_VARIABLE_NAME_BLOCK_AMR_LEVEL_IS_VECTOR,
      CCMC_VARIABLE_NAME_BLOCK_AMR_LEVEL_POSITION_GRID_SYSTEM,
      CCMC_VARIABLE_NAME_BLOCK_AMR_LEVEL_DATA_GRID_SYSTEM,
      CCMC_VARIABLE_NAME_BLOCK_X_MIN,
      CCMC_VARIABLE_NAME_BLOCK_X_MIN_DATA_TYPE,
      CCMC_VARIABLE_NAME_BLOCK_X_MIN_CLASSIFICATION,
      CCMC_VARIABLE_NAME_BLOCK_X_MIN_VALID_MIN,
      CCMC_VARIABLE_NAME_BLOCK_X_MIN_VALID_MAX,
      CCMC_VARIABLE_NAME_BLOCK_X_MIN_UNITS,
      CCMC_VARIABLE_NAME_BLOCK_X_MIN_GRID_SYSTEM,
      CCMC_VARIABLE_NAME_BLOCK_X_MIN_MASK,
      CCMC_VARIABLE_NAME_BLOCK_X_MIN_DESCRIPTION,
      CCMC_VARIABLE_NAME_BLOCK_X_MIN_IS_VECTOR,
      CCMC_VARIABLE_NAME_BLOCK_X_MIN_POSITION_GRID_SYSTEM,
      CCMC_VARIABLE_NAME_BLOCK_X_MIN_DATA_GRID_SYSTEM,
      CCMC_VARIABLE_NAME_BLOCK_X_MAX,
      CCMC_VARIABLE_NAME_BLOCK_X_MAX_DATA_TYPE,
      CCMC_VARIABLE_NAME_BLOCK_X_MAX_CLASSIFICATION,
      CCMC_VARIABLE_NAME_BLOCK_X_MAX_VALID_MIN,
      CCMC_VARIABLE_NAME_BLOCK_X_MAX_VALID_MAX,
      CCMC_VARIABLE_NAME_BLOCK_X_MAX_UNITS,
      CCMC_VARIABLE_NAME_BLOCK_X_MAX_GRID_SYSTEM,
      CCMC_VARIABLE_NAME_BLOCK_X_MAX_MASK,
      CCMC_VARIABLE_NAME_BLOCK_X_MAX_DESCRIPTION,
      CCMC_VARIABLE_NAME_BLOCK_X_MAX_IS_VECTOR,
      CCMC_VARIABLE_NAME_BLOCK_X_MAX_POSITION_GRID_SYSTEM,
      CCMC_VARIABLE_NAME_BLOCK_X_MAX_DATA_GRID_SYSTEM,
      CCMC_VARIABLE_NAME_BLOCK_Y_MIN,
      CCMC_VARIABLE_NAME_BLOCK_Y_MIN_DATA_TYPE,
      CCMC_VARIABLE_NAME_BLOCK_Y_MIN_CLASSIFICATION,
      CCMC_VARIABLE_NAME_BLOCK_Y_MIN_VALID_MIN,
      CCMC_VARIABLE_NAME_BLOCK_Y_MIN_VALID_MAX,
      CCMC_VARIABLE_NAME_BLOCK_Y_MIN_UNITS,
      CCMC_VARIABLE_NAME_BLOCK_Y_MIN_GRID_SYSTEM,
      CCMC_VARIABLE_NAME_BLOCK_Y_MIN_MASK,
      CCMC_VARIABLE_NAME_BLOCK_Y_MIN_DESCRIPTION,
      CCMC_VARIABLE_NAME_BLOCK_Y_MIN_IS_VECTOR,
      CCMC_VARIABLE_NAME_BLOCK_Y_MIN_POSITION_GRID_SYSTEM,
      CCMC_VARIABLE_NAME_BLOCK_Y_MIN_DATA_GRID_SYSTEM,
      CCMC_VARIABLE_NAME_BLOCK_Y_MAX,
      CCMC_VARIABLE_NAME_BLOCK_Y_MAX_DATA_TYPE,
      CCMC_VARIABLE_NAME_BLOCK_Y_MAX_CLASSIFICATION,
      CCMC_VARIABLE_NAME_BLOCK_Y_MAX_VALID_MIN,
      CCMC_VARIABLE_NAME_BLOCK_Y_MAX_VALID_MAX,
      CCMC_VARIABLE_NAME_BLOCK_Y_MAX_UNITS,
      CCMC_VARIABLE_NAME_BLOCK_Y_MAX_GRID_SYSTEM,
      CCMC_VARIABLE_NAME_BLOCK_Y_MAX_MASK,
      CCMC_VARIABLE_NAME_BLOCK_Y_MAX_DESCRIPTION,
      CCMC_VARIABLE_NAME_BLOCK_Y_MAX_IS_VECTOR,
      CCMC_VARIABLE_NAME_BLOCK_Y_MAX_POSITION_GRID_SYSTEM,
      CCMC_VARIABLE_NAME_BLOCK_Y_MAX_DATA_GRID_SYSTEM,
      CCMC_VARIABLE_NAME_BLOCK_Z_MIN,
      CCMC_VARIABLE_NAME_BLOCK_Z_MIN_DATA_TYPE,
      CCMC_VARIABLE_NAME_BLOCK_Z_MIN_CLASSIFICATION,
      CCMC_VARIABLE_NAME_BLOCK_Z_MIN_VALID_MIN,
      CCMC_VARIABLE_NAME_BLOCK_Z_MIN_VALID_MAX,
      CCMC_VARIABLE_NAME_BLOCK_Z_MIN_UNITS,
      CCMC_VARIABLE_NAME_BLOCK_Z_MIN_GRID_SYSTEM,
      CCMC_VARIABLE_NAME_BLOCK_Z_MIN_MASK,
      CCMC_VARIABLE_NAME_BLOCK_Z_MIN_DESCRIPTION,
      CCMC_VARIABLE_NAME_BLOCK_Z_MIN_IS_VECTOR,
      CCMC_VARIABLE_NAME_BLOCK_Z_MIN_POSITION_GRID_SYSTEM,
      CCMC_VARIABLE_NAME_BLOCK_Z_MIN_DATA_GRID_SYSTEM,
      CCMC_VARIABLE_NAME_BLOCK_Z_MAX,
      CCMC_VARIABLE_NAME_BLOCK_Z_MAX_DATA_TYPE,
      CCMC_VARIABLE_NAME_BLOCK_Z_MAX_CLASSIFICATION,
      CCMC_VARIABLE_NAME_BLOCK_Z_MAX_VALID_MIN,
      CCMC_VARIABLE_NAME_BLOCK_Z_MAX_VALID_MAX,
      CCMC_VARIABLE_NAME_BLOCK_Z_MAX_UNITS,
      CCMC_VARIABLE_NAME_BLOCK_Z_MAX_GRID_SYSTEM,
      CCMC_VARIABLE_NAME_BLOCK_Z_MAX_MASK,
      CCMC_VARIABLE_NAME_BLOCK_Z_MAX_DESCRIPTION,
      CCMC_VARIABLE_NAME_BLOCK_Z_MAX_IS_VECTOR,
      CCMC_VARIABLE_NAME_BLOCK_Z_MAX_POSITION_GRID_SYSTEM,
      CCMC_VARIABLE_NAME_BLOCK_Z_MAX_DATA_GRID_SYSTEM,
      CCMC_VARIABLE_NAME_BLOCK_X_CENTER,
      CCMC_VARIABLE_NAME_BLOCK_X_CENTER_DATA_TYPE,
      CCMC_VARIABLE_NAME_BLOCK_X_CENTER_CLASSIFICATION,
      CCMC_VARIABLE_NAME_BLOCK_X_CENTER_VALID_MIN,
      CCMC_VARIABLE_NAME_BLOCK_X_CENTER_VALID_MAX,
      CCMC_VARIABLE_NAME_BLOCK_X_CENTER_UNITS,
      CCMC_VARIABLE_NAME_BLOCK_X_CENTER_GRID_SYSTEM,
      CCMC_VARIABLE_NAME_BLOCK_X_CENTER_MASK,
      CCMC_VARIABLE_NAME_BLOCK_X_CENTER_DESCRIPTION,
      CCMC_VARIABLE_NAME_BLOCK_X_CENTER_IS_VECTOR,
      CCMC_VARIABLE_NAME_BLOCK_X_CENTER_POSITION_GRID_SYSTEM,
      CCMC_VARIABLE_NAME_BLOCK_X_CENTER_DATA_GRID_SYSTEM,
      CCMC_VARIABLE_NAME_BLOCK_Y_CENTER,
      CCMC_VARIABLE_NAME_BLOCK_Y_CENTER_DATA_TYPE,
      CCMC_VARIABLE_NAME_BLOCK_Y_CENTER_CLASSIFICATION,
      CCMC_VARIABLE_NAME_BLOCK_Y_CENTER_VALID_MIN,
      CCMC_VARIABLE_NAME_BLOCK_Y_CENTER_VALID_MAX,
      CCMC_VARIABLE_NAME_BLOCK_Y_CENTER_UNITS,
      CCMC_VARIABLE_NAME_BLOCK_Y_CENTER_GRID_SYSTEM,
      CCMC_VARIABLE_NAME_BLOCK_Y_CENTER_MASK,
      CCMC_VARIABLE_NAME_BLOCK_Y_CENTER_DESCRIPTION,
      CCMC_VARIABLE_NAME_BLOCK_Y_CENTER_IS_VECTOR,
      CCMC_VARIABLE_NAME_BLOCK_Y_CENTER_POSITION_GRID_SYSTEM,
      CCMC_VARIABLE_NAME_BLOCK_Y_CENTER_DATA_GRID_SYSTEM,
      CCMC_VARIABLE_NAME_BLOCK_Z_CENTER,
      CCMC_VARIABLE_NAME_BLOCK_Z_CENTER_DATA_TYPE,
      CCMC_VARIABLE_NAME_BLOCK_Z_CENTER_CLASSIFICATION,
      CCMC_VARIABLE_NAME_BLOCK_Z_CENTER_VALID_MIN,
      CCMC_VARIABLE_NAME_BLOCK_Z_CENTER_VALID_MAX,
      CCMC_VARIABLE_NAME_BLOCK_Z_CENTER_UNITS,
      CCMC_VARIABLE_NAME_BLOCK_Z_CENTER_GRID_SYSTEM,
      CCMC_VARIABLE_NAME_BLOCK_Z_CENTER_MASK,
      CCMC_VARIABLE_NAME_BLOCK_Z_CENTER_DESCRIPTION,
      CCMC_VARIABLE_NAME_BLOCK_Z_CENTER_IS_VECTOR,
      CCMC_VARIABLE_NAME_BLOCK_Z_CENTER_POSITION_GRID_SYSTEM,
      CCMC_VARIABLE_NAME_BLOCK_Z_CENTER_DATA_GRID_SYSTEM,
      CCMC_VARIABLE_NAME_BLOCK_AT_AMR_LEVEL,
      CCMC_VARIABLE_NAME_BLOCK_AT_AMR_LEVEL_DATA_TYPE,
      CCMC_VARIABLE_NAME_BLOCK_AT_AMR_LEVEL_CLASSIFICATION,
      CCMC_VARIABLE_NAME_BLOCK_AT_AMR_LEVEL_VALID_MIN,
      CCMC_VARIABLE_NAME_BLOCK_AT_AMR_LEVEL_VALID_MAX,
      CCMC_VARIABLE_NAME_BLOCK_AT_AMR_LEVEL_UNITS,
      CCMC_VARIABLE_NAME_BLOCK_AT_AMR_LEVEL_GRID_SYSTEM,
      CCMC_VARIABLE_NAME_BLOCK_AT_AMR_LEVEL_MASK,
      CCMC_VARIABLE_NAME_BLOCK_AT_AMR_LEVEL_DESCRIPTION,
      CCMC_VARIABLE_NAME_BLOCK_AT_AMR_LEVEL_IS_VECTOR,
      CCMC_VARIABLE_NAME_BLOCK_AT_AMR_LEVEL_POSITION_GRID_SYSTEM,
      CCMC_VARIABLE_NAME_BLOCK_AT_AMR_LEVEL_DATA_GRID_SYSTEM,
      CCMC_VARIABLE_NAME_BLOCK_PARENT_ID,
      CCMC_VARIABLE_NAME_BLOCK_PARENT_ID_DATA_TYPE,
      CCMC_VARIABLE_NAME_BLOCK_PARENT_ID_CLASSIFICATION,
      CCMC_VARIABLE_NAME_BLOCK_PARENT_ID_VALID_MIN,
      CCMC_VARIABLE_NAME_BLOCK_PARENT_ID_VALID_MAX,
      CCMC_VARIABLE_NAME_BLOCK_PARENT_ID_UNITS,
      CCMC_VARIABLE_NAME_BLOCK_PARENT_ID_GRID_SYSTEM,
      CCMC_VARIABLE_NAME_BLOCK_PARENT_ID_MASK,
      CCMC_VARIABLE_NAME_BLOCK_PARENT_ID_DESCRIPTION,
      CCMC_VARIABLE_NAME_BLOCK_PARENT_ID_IS_VECTOR,
      CCMC_VARIABLE_NAME_BLOCK_PARENT_ID_POSITION_GRID_SYSTEM,
      CCMC_VARIABLE_NAME_BLOCK_PARENT_ID_DATA_GRID_SYSTEM,
      CCMC_VARIABLE_NAME_BLOCK_CHILD_COUNT,
      CCMC_VARIABLE_NAME_BLOCK_CHILD_COUNT_DATA_TYPE,
      CCMC_VARIABLE_NAME_BLOCK_CHILD_COUNT_CLASSIFICATION,
      CCMC_VARIABLE_NAME_BLOCK_CHILD_COUNT_VALID_MIN,
      CCMC_VARIABLE_NAME_BLOCK_CHILD_COUNT_VALID_MAX,
      CCMC_VARIABLE_NAME_BLOCK_CHILD_COUNT_UNITS,
      CCMC_VARIABLE_NAME_BLOCK_CHILD_COUNT_GRID_SYSTEM,
      CCMC_VARIABLE_NAME_BLOCK_CHILD_COUNT_MASK,
      CCMC_VARIABLE_NAME_BLOCK_CHILD_COUNT_DESCRIPTION,
      CCMC_VARIABLE_NAME_BLOCK_CHILD_COUNT_IS_VECTOR,
      CCMC_VARIABLE_NAME_BLOCK_CHILD_COUNT_POSITION_GRID_SYSTEM,
      CCMC_VARIABLE_NAME_BLOCK_CHILD_COUNT_DATA_GRID_SYSTEM,
      CCMC_VARIABLE_NAME_BLOCK_CHILD_ID_1,
      CCMC_VARIABLE_NAME_BLOCK_CHILD_ID_1_DATA_TYPE,
      CCMC_VARIABLE_NAME_BLOCK_CHILD_ID_1_CLASSIFICATION,
      CCMC_VARIABLE_NAME_BLOCK_CHILD_ID_1_VALID_MIN,
      CCMC_VARIABLE_NAME_BLOCK_CHILD_ID_1_VALID_MAX,
      CCMC_VARIABLE_NAME_BLOCK_CHILD_ID_1_UNITS,
      CCMC_VARIABLE_NAME_BLOCK_CHILD_ID_1_GRID_SYSTEM,
      CCMC_VARIABLE_NAME_BLOCK_CHILD_ID_1_MASK,
      CCMC_VARIABLE_NAME_BLOCK_CHILD_ID_1_DESCRIPTION,
      CCMC_VARIABLE_NAME_BLOCK_CHILD_ID_1_IS_VECTOR,
      CCMC_VARIABLE_NAME_BLOCK_CHILD_ID_1_POSITION_GRID_SYSTEM,
      CCMC_VARIABLE_NAME_BLOCK_CHILD_ID_1_DATA_GRID_SYSTEM,
      CCMC_VARIABLE_NAME_BLOCK_CHILD_ID_2,
      CCMC_VARIABLE_NAME_BLOCK_CHILD_ID_2_DATA_TYPE,
      CCMC_VARIABLE_NAME_BLOCK_CHILD_ID_2_CLASSIFICATION,
      CCMC_VARIABLE_NAME_BLOCK_CHILD_ID_2_VALID_MIN,
      CCMC_VARIABLE_NAME_BLOCK_CHILD_ID_2_VALID_MAX,
      CCMC_VARIABLE_NAME_BLOCK_CHILD_ID_1_UNITS,
      CCMC_VARIABLE_NAME_BLOCK_CHILD_ID_1_GRID_SYSTEM,
      CCMC_VARIABLE_NAME_BLOCK_CHILD_ID_1_MASK,
      CCMC_VARIABLE_NAME_BLOCK_CHILD_ID_1_DESCRIPTION,
      CCMC_VARIABLE_NAME_BLOCK_CHILD_ID_1_IS_VECTOR,
      CCMC_VARIABLE_NAME_BLOCK_CHILD_ID_1_POSITION_GRID_SYSTEM,
      CCMC_VARIABLE_NAME_BLOCK_CHILD_ID_1_DATA_GRID_SYSTEM,
      CCMC_VARIABLE_NAME_BLOCK_CHILD_ID_3,
      CCMC_VARIABLE_NAME_BLOCK_CHILD_ID_3_DATA_TYPE,
      CCMC_VARIABLE_NAME_BLOCK_CHILD_ID_3_CLASSIFICATION,
      CCMC_VARIABLE_NAME_BLOCK_CHILD_ID_3_VALID_MIN,
      CCMC_VARIABLE_NAME_BLOCK_CHILD_ID_3_VALID_MAX,
      CCMC_VARIABLE_NAME_BLOCK_CHILD_ID_1_UNITS,
      CCMC_VARIABLE_NAME_BLOCK_CHILD_ID_1_GRID_SYSTEM,
      CCMC_VARIABLE_NAME_BLOCK_CHILD_ID_1_MASK,
      CCMC_VARIABLE_NAME_BLOCK_CHILD_ID_1_DESCRIPTION,
      CCMC_VARIABLE_NAME_BLOCK_CHILD_ID_1_IS_VECTOR,
      CCMC_VARIABLE_NAME_BLOCK_CHILD_ID_1_POSITION_GRID_SYSTEM,
      CCMC_VARIABLE_NAME_BLOCK_CHILD_ID_1_DATA_GRID_SYSTEM,
      CCMC_VARIABLE_NAME_BLOCK_CHILD_ID_4,
      CCMC_VARIABLE_NAME_BLOCK_CHILD_ID_4_DATA_TYPE,
      CCMC_VARIABLE_NAME_BLOCK_CHILD_ID_4_CLASSIFICATION,
      CCMC_VARIABLE_NAME_BLOCK_CHILD_ID_4_VALID_MIN,
      CCMC_VARIABLE_NAME_BLOCK_CHILD_ID_4_VALID_MAX,
      CCMC_VARIABLE_NAME_BLOCK_CHILD_ID_1_UNITS,
      CCMC_VARIABLE_NAME_BLOCK_CHILD_ID_1_GRID_SYSTEM,
      CCMC_VARIABLE_NAME_BLOCK_CHILD_ID_1_MASK,
      CCMC_VARIABLE_NAME_BLOCK_CHILD_ID_1_DESCRIPTION,
      CCMC_VARIABLE_NAME_BLOCK_CHILD_ID_1_IS_VECTOR,
      CCMC_VARIABLE_NAME_BLOCK_CHILD_ID_1_POSITION_GRID_SYSTEM,
      CCMC_VARIABLE_NAME_BLOCK_CHILD_ID_1_DATA_GRID_SYSTEM,
      CCMC_VARIABLE_NAME_BLOCK_CHILD_ID_5,
      CCMC_VARIABLE_NAME_BLOCK_CHILD_ID_5_DATA_TYPE,
      CCMC_VARIABLE_NAME_BLOCK_CHILD_ID_5_CLASSIFICATION,
      CCMC_VARIABLE_NAME_BLOCK_CHILD_ID_5_VALID_MIN,
      CCMC_VARIABLE_NAME_BLOCK_CHILD_ID_5_VALID_MAX,
      CCMC_VARIABLE_NAME_BLOCK_CHILD_ID_1_UNITS,
      CCMC_VARIABLE_NAME_BLOCK_CHILD_ID_1_GRID_SYSTEM,
      CCMC_VARIABLE_NAME_BLOCK_CHILD_ID_1_MASK,
      CCMC_VARIABLE_NAME_BLOCK_CHILD_ID_1_DESCRIPTION,
      CCMC_VARIABLE_NAME_BLOCK_CHILD_ID_1_IS_VECTOR,
      CCMC_VARIABLE_NAME_BLOCK_CHILD_ID_1_POSITION_GRID_SYSTEM,
      CCMC_VARIABLE_NAME_BLOCK_CHILD_ID_1_DATA_GRID_SYSTEM,
      CCMC_VARIABLE_NAME_BLOCK_CHILD_ID_6,
      CCMC_VARIABLE_NAME_BLOCK_CHILD_ID_6_DATA_TYPE,
      CCMC_VARIABLE_NAME_BLOCK_CHILD_ID_6_CLASSIFICATION,
      CCMC_VARIABLE_NAME_BLOCK_CHILD_ID_6_VALID_MIN,
      CCMC_VARIABLE_NAME_BLOCK_CHILD_ID_6_VALID_MAX,
      CCMC_VARIABLE_NAME_BLOCK_CHILD_ID_1_UNITS,
      CCMC_VARIABLE_NAME_BLOCK_CHILD_ID_1_GRID_SYSTEM,
      CCMC_VARIABLE_NAME_BLOCK_CHILD_ID_1_MASK,
      CCMC_VARIABLE_NAME_BLOCK_CHILD_ID_1_DESCRIPTION,
      CCMC_VARIABLE_NAME_BLOCK_CHILD_ID_1_IS_VECTOR,
      CCMC_VARIABLE_NAME_BLOCK_CHILD_ID_1_POSITION_GRID_SYSTEM,
      CCMC_VARIABLE_NAME_BLOCK_CHILD_ID_1_DATA_GRID_SYSTEM,
      CCMC_VARIABLE_NAME_BLOCK_CHILD_ID_7,
      CCMC_VARIABLE_NAME_BLOCK_CHILD_ID_7_DATA_TYPE,
      CCMC_VARIABLE_NAME_BLOCK_CHILD_ID_7_CLASSIFICATION,
      CCMC_VARIABLE_NAME_BLOCK_CHILD_ID_7_VALID_MIN,
      CCMC_VARIABLE_NAME_BLOCK_CHILD_ID_7_VALID_MAX,
      CCMC_VARIABLE_NAME_BLOCK_CHILD_ID_1_UNITS,
      CCMC_VARIABLE_NAME_BLOCK_CHILD_ID_1_GRID_SYSTEM,
      CCMC_VARIABLE_NAME_BLOCK_CHILD_ID_1_MASK,
      CCMC_VARIABLE_NAME_BLOCK_CHILD_ID_1_DESCRIPTION,
      CCMC_VARIABLE_NAME_BLOCK_CHILD_ID_1_IS_VECTOR,
      CCMC_VARIABLE_NAME_BLOCK_CHILD_ID_1_POSITION_GRID_SYSTEM,
      CCMC_VARIABLE_NAME_BLOCK_CHILD_ID_1_DATA_GRID_SYSTEM,
      CCMC_VARIABLE_NAME_BLOCK_CHILD_ID_8,
      CCMC_VARIABLE_NAME_BLOCK_CHILD_ID_8_DATA_TYPE,
      CCMC_VARIABLE_NAME_BLOCK_CHILD_ID_8_CLASSIFICATION,
      CCMC_VARIABLE_NAME_BLOCK_CHILD_ID_8_VALID_MIN,
      CCMC_VARIABLE_NAME_BLOCK_CHILD_ID_8_VALID_MAX,
      CCMC_VARIABLE_NAME_BLOCK_CHILD_ID_8_UNITS,
      CCMC_VARIABLE_NAME_BLOCK_CHILD_ID_8_GRID_SYSTEM,
      CCMC_VARIABLE_NAME_BLOCK_CHILD_ID_8_MASK,
      CCMC_VARIABLE_NAME_BLOCK_CHILD_ID_8_DESCRIPTION,
      CCMC_VARIABLE_NAME_BLOCK_CHILD_ID_8_IS_VECTOR,
      CCMC_VARIABLE_NAME_BLOCK_CHILD_ID_8_POSITION_GRID_SYSTEM,
      CCMC_VARIABLE_NAME_BLOCK_CHILD_ID_8_DATA_GRID_SYSTEM,
      CCMC_VARIABLE_NAME_STATUS,
      CCMC_VARIABLE_NAME_STATUS_DATA_TYPE,
      CCMC_VARIABLE_NAME_STATUS_CLASSIFICATION,
      CCMC_VARIABLE_NAME_STATUS_VALID_MIN,
      CCMC_VARIABLE_NAME_STATUS_VALID_MAX,
      CCMC_VARIABLE_NAME_STATUS_UNITS,
      CCMC_VARIABLE_NAME_STATUS_GRID_SYSTEM,
      CCMC_VARIABLE_NAME_STATUS_MASK,
      CCMC_VARIABLE_NAME_STATUS_DESCRIPTION,
      CCMC_VARIABLE_NAME_STATUS_IS_VECTOR,
      CCMC_VARIABLE_NAME_STATUS_POSITION_GRID_SYSTEM,
      CCMC_VARIABLE_NAME_STATUS_DATA_GRID_SYSTEM
};

/***************************************************************** UCLA_GGCM ***********************************************/

struct registered_ucla_ggcm_variables
{

   char *ccmc_var_name;
   char *ccmc_var_data_type;
   char *ccmc_var_classification;
   float valid_min;
   float valid_max;
   char *units;
   char *grid_system;
   float mask;
   char *description;
   int is_vector_component;
   char *position_grid_system;
   char *data_grid_system;
}static registered_ucla_ggcm_var_list[] =
{

      CCMC_VARIABLE_NAME_X,
      CCMC_VARIABLE_NAME_X_DATA_TYPE,
      CCMC_VARIABLE_NAME_X_CLASSIFICATION,
      CCMC_VARIABLE_NAME_X_VALID_MIN,
      CCMC_VARIABLE_NAME_X_VALID_MAX,
      CCMC_VARIABLE_NAME_X_UNITS,
      CCMC_VARIABLE_NAME_X_GRID_SYSTEM,
      CCMC_VARIABLE_NAME_X_MASK,
      CCMC_VARIABLE_NAME_X_DESCRIPTION,
      CCMC_VARIABLE_NAME_X_IS_VECTOR,
      CCMC_VARIABLE_NAME_X_POSITION_GRID_SYSTEM,
      CCMC_VARIABLE_NAME_X_DATA_GRID_SYSTEM,
      CCMC_VARIABLE_NAME_Y,
      CCMC_VARIABLE_NAME_Y_DATA_TYPE,
      CCMC_VARIABLE_NAME_Y_CLASSIFICATION,
      CCMC_VARIABLE_NAME_Y_VALID_MIN,
      CCMC_VARIABLE_NAME_Y_VALID_MAX,
      CCMC_VARIABLE_NAME_Y_UNITS,
      CCMC_VARIABLE_NAME_Y_GRID_SYSTEM,
      CCMC_VARIABLE_NAME_Y_MASK,
      CCMC_VARIABLE_NAME_Y_DESCRIPTION,
      CCMC_VARIABLE_NAME_Y_IS_VECTOR,
      CCMC_VARIABLE_NAME_Y_POSITION_GRID_SYSTEM,
      CCMC_VARIABLE_NAME_Y_DATA_GRID_SYSTEM,
      CCMC_VARIABLE_NAME_Z,
      CCMC_VARIABLE_NAME_Z_DATA_TYPE,
      CCMC_VARIABLE_NAME_Z_CLASSIFICATION,
      CCMC_VARIABLE_NAME_Z_VALID_MIN,
      CCMC_VARIABLE_NAME_Z_VALID_MAX,
      CCMC_VARIABLE_NAME_Z_UNITS,
      CCMC_VARIABLE_NAME_Z_GRID_SYSTEM,
      CCMC_VARIABLE_NAME_Z_MASK,
      CCMC_VARIABLE_NAME_Z_DESCRIPTION,
      CCMC_VARIABLE_NAME_Z_IS_VECTOR,
      CCMC_VARIABLE_NAME_Z_POSITION_GRID_SYSTEM,
      CCMC_VARIABLE_NAME_Z_DATA_GRID_SYSTEM,
      CCMC_VARIABLE_NAME_X_BX,
      CCMC_VARIABLE_NAME_X_BX_DATA_TYPE,
      CCMC_VARIABLE_NAME_X_BX_CLASSIFICATION,
      CCMC_VARIABLE_NAME_X_BX_VALID_MIN,
      CCMC_VARIABLE_NAME_X_BX_VALID_MAX,
      CCMC_VARIABLE_NAME_X_BX_UNITS,
      CCMC_VARIABLE_NAME_X_BX_GRID_SYSTEM,
      CCMC_VARIABLE_NAME_X_BX_MASK,
      CCMC_VARIABLE_NAME_X_BX_DESCRIPTION,
      CCMC_VARIABLE_NAME_X_BX_IS_VECTOR,
      CCMC_VARIABLE_NAME_X_BX_POSITION_GRID_SYSTEM,
      CCMC_VARIABLE_NAME_X_BX_DATA_GRID_SYSTEM,
      CCMC_VARIABLE_NAME_X_BY,
      CCMC_VARIABLE_NAME_X_BY_DATA_TYPE,
      CCMC_VARIABLE_NAME_X_BY_CLASSIFICATION,
      CCMC_VARIABLE_NAME_X_BY_VALID_MIN,
      CCMC_VARIABLE_NAME_X_BY_VALID_MAX,
      CCMC_VARIABLE_NAME_X_BY_UNITS,
      CCMC_VARIABLE_NAME_X_BY_GRID_SYSTEM,
      CCMC_VARIABLE_NAME_X_BY_MASK,
      CCMC_VARIABLE_NAME_X_BY_DESCRIPTION,
      CCMC_VARIABLE_NAME_X_BY_IS_VECTOR,
      CCMC_VARIABLE_NAME_X_BY_POSITION_GRID_SYSTEM,
      CCMC_VARIABLE_NAME_X_BY_DATA_GRID_SYSTEM,
      CCMC_VARIABLE_NAME_X_BZ,
      CCMC_VARIABLE_NAME_X_BZ_DATA_TYPE,
      CCMC_VARIABLE_NAME_X_BZ_CLASSIFICATION,
      CCMC_VARIABLE_NAME_X_BZ_VALID_MIN,
      CCMC_VARIABLE_NAME_X_BZ_VALID_MAX,
      CCMC_VARIABLE_NAME_X_BZ_UNITS,
      CCMC_VARIABLE_NAME_X_BZ_GRID_SYSTEM,
      CCMC_VARIABLE_NAME_X_BZ_MASK,
      CCMC_VARIABLE_NAME_X_BZ_DESCRIPTION,
      CCMC_VARIABLE_NAME_X_BZ_IS_VECTOR,
      CCMC_VARIABLE_NAME_X_BZ_POSITION_GRID_SYSTEM,
      CCMC_VARIABLE_NAME_X_BZ_DATA_GRID_SYSTEM,
      CCMC_VARIABLE_NAME_Y_BX,
      CCMC_VARIABLE_NAME_Y_BX_DATA_TYPE,
      CCMC_VARIABLE_NAME_Y_BX_CLASSIFICATION,
      CCMC_VARIABLE_NAME_Y_BX_VALID_MIN,
      CCMC_VARIABLE_NAME_Y_BX_VALID_MAX,
      CCMC_VARIABLE_NAME_Y_BX_UNITS,
      CCMC_VARIABLE_NAME_Y_BX_GRID_SYSTEM,
      CCMC_VARIABLE_NAME_Y_BX_MASK,
      CCMC_VARIABLE_NAME_Y_BX_DESCRIPTION,
      CCMC_VARIABLE_NAME_Y_BX_IS_VECTOR,
      CCMC_VARIABLE_NAME_Y_BX_POSITION_GRID_SYSTEM,
      CCMC_VARIABLE_NAME_Y_BX_DATA_GRID_SYSTEM,
      CCMC_VARIABLE_NAME_Y_BY,
      CCMC_VARIABLE_NAME_Y_BY_DATA_TYPE,
      CCMC_VARIABLE_NAME_Y_BY_CLASSIFICATION,
      CCMC_VARIABLE_NAME_Y_BY_VALID_MIN,
      CCMC_VARIABLE_NAME_Y_BY_VALID_MAX,
      CCMC_VARIABLE_NAME_Y_BY_UNITS,
      CCMC_VARIABLE_NAME_Y_BY_GRID_SYSTEM,
      CCMC_VARIABLE_NAME_Y_BY_MASK,
      CCMC_VARIABLE_NAME_Y_BY_DESCRIPTION,
      CCMC_VARIABLE_NAME_Y_BY_IS_VECTOR,
      CCMC_VARIABLE_NAME_Y_BY_POSITION_GRID_SYSTEM,
      CCMC_VARIABLE_NAME_Y_BY_DATA_GRID_SYSTEM,
      CCMC_VARIABLE_NAME_Y_BZ,
      CCMC_VARIABLE_NAME_Y_BZ_DATA_TYPE,
      CCMC_VARIABLE_NAME_Y_BZ_CLASSIFICATION,
      CCMC_VARIABLE_NAME_Y_BZ_VALID_MIN,
      CCMC_VARIABLE_NAME_Y_BZ_VALID_MAX,
      CCMC_VARIABLE_NAME_Y_BZ_UNITS,
      CCMC_VARIABLE_NAME_Y_BZ_GRID_SYSTEM,
      CCMC_VARIABLE_NAME_Y_BZ_MASK,
      CCMC_VARIABLE_NAME_Y_BZ_DESCRIPTION,
      CCMC_VARIABLE_NAME_Y_BZ_IS_VECTOR,
      CCMC_VARIABLE_NAME_Y_BZ_POSITION_GRID_SYSTEM,
      CCMC_VARIABLE_NAME_Y_BZ_DATA_GRID_SYSTEM,
      CCMC_VARIABLE_NAME_Z_BX,
      CCMC_VARIABLE_NAME_Z_BX_DATA_TYPE,
      CCMC_VARIABLE_NAME_Z_BX_CLASSIFICATION,
      CCMC_VARIABLE_NAME_Z_BX_VALID_MIN,
      CCMC_VARIABLE_NAME_Z_BX_VALID_MAX,
      CCMC_VARIABLE_NAME_Z_BX_UNITS,
      CCMC_VARIABLE_NAME_Z_BX_GRID_SYSTEM,
      CCMC_VARIABLE_NAME_Z_BX_MASK,
      CCMC_VARIABLE_NAME_Z_BX_DESCRIPTION,
      CCMC_VARIABLE_NAME_Z_BX_IS_VECTOR,
      CCMC_VARIABLE_NAME_Z_BX_POSITION_GRID_SYSTEM,
      CCMC_VARIABLE_NAME_Z_BX_DATA_GRID_SYSTEM,
      CCMC_VARIABLE_NAME_Z_BY,
      CCMC_VARIABLE_NAME_Z_BY_DATA_TYPE,
      CCMC_VARIABLE_NAME_Z_BY_CLASSIFICATION,
      CCMC_VARIABLE_NAME_Z_BY_VALID_MIN,
      CCMC_VARIABLE_NAME_Z_BY_VALID_MAX,
      CCMC_VARIABLE_NAME_Z_BY_UNITS,
      CCMC_VARIABLE_NAME_Z_BY_GRID_SYSTEM,
      CCMC_VARIABLE_NAME_Z_BY_MASK,
      CCMC_VARIABLE_NAME_Z_BY_DESCRIPTION,
      CCMC_VARIABLE_NAME_Z_BY_IS_VECTOR,
      CCMC_VARIABLE_NAME_Z_BY_POSITION_GRID_SYSTEM,
      CCMC_VARIABLE_NAME_Z_BY_DATA_GRID_SYSTEM,
      CCMC_VARIABLE_NAME_Z_BZ,
      CCMC_VARIABLE_NAME_Z_BZ_DATA_TYPE,
      CCMC_VARIABLE_NAME_Z_BZ_CLASSIFICATION,
      CCMC_VARIABLE_NAME_Z_BZ_VALID_MIN,
      CCMC_VARIABLE_NAME_Z_BZ_VALID_MAX,
      CCMC_VARIABLE_NAME_Z_BZ_UNITS,
      CCMC_VARIABLE_NAME_Z_BZ_GRID_SYSTEM,
      CCMC_VARIABLE_NAME_Z_BZ_MASK,
      CCMC_VARIABLE_NAME_Z_BZ_DESCRIPTION,
      CCMC_VARIABLE_NAME_Z_BZ_IS_VECTOR,
      CCMC_VARIABLE_NAME_Z_BZ_POSITION_GRID_SYSTEM,
      CCMC_VARIABLE_NAME_Z_BZ_DATA_GRID_SYSTEM,
      CCMC_VARIABLE_NAME_BX,
      CCMC_VARIABLE_NAME_BX_DATA_TYPE,
      CCMC_VARIABLE_NAME_BX_CLASSIFICATION,
      CCMC_VARIABLE_NAME_BX_VALID_MIN,
      CCMC_VARIABLE_NAME_BX_VALID_MAX,
      CCMC_VARIABLE_NAME_BX_UNITS,
      CCMC_VARIABLE_NAME_BX_GRID_SYSTEM,
      CCMC_VARIABLE_NAME_BX_MASK,
      CCMC_VARIABLE_NAME_BX_DESCRIPTION,
      CCMC_VARIABLE_NAME_BX_IS_VECTOR,
      CCMC_VARIABLE_NAME_BX_POSITION_GRID_SYSTEM,
      CCMC_VARIABLE_NAME_BX_DATA_GRID_SYSTEM,
      CCMC_VARIABLE_NAME_BY,
      CCMC_VARIABLE_NAME_BY_DATA_TYPE,
      CCMC_VARIABLE_NAME_BY_CLASSIFICATION,
      CCMC_VARIABLE_NAME_BY_VALID_MIN,
      CCMC_VARIABLE_NAME_BY_VALID_MAX,
      CCMC_VARIABLE_NAME_BY_UNITS,
      CCMC_VARIABLE_NAME_BY_GRID_SYSTEM,
      CCMC_VARIABLE_NAME_BY_MASK,
      CCMC_VARIABLE_NAME_BY_DESCRIPTION,
      CCMC_VARIABLE_NAME_BY_IS_VECTOR,
      CCMC_VARIABLE_NAME_BY_POSITION_GRID_SYSTEM,
      CCMC_VARIABLE_NAME_BY_DATA_GRID_SYSTEM,
      CCMC_VARIABLE_NAME_BZ,
      CCMC_VARIABLE_NAME_BZ_DATA_TYPE,
      CCMC_VARIABLE_NAME_BZ_CLASSIFICATION,
      CCMC_VARIABLE_NAME_BZ_VALID_MIN,
      CCMC_VARIABLE_NAME_BZ_VALID_MAX,
      CCMC_VARIABLE_NAME_BZ_UNITS,
      CCMC_VARIABLE_NAME_BZ_GRID_SYSTEM,
      CCMC_VARIABLE_NAME_BZ_MASK,
      CCMC_VARIABLE_NAME_BZ_DESCRIPTION,
      CCMC_VARIABLE_NAME_BZ_IS_VECTOR,
      CCMC_VARIABLE_NAME_BZ_POSITION_GRID_SYSTEM,
      CCMC_VARIABLE_NAME_BZ_DATA_GRID_SYSTEM,
      CCMC_VARIABLE_NAME_BX1,
      CCMC_VARIABLE_NAME_BX1_DATA_TYPE,
      CCMC_VARIABLE_NAME_BX1_CLASSIFICATION,
      CCMC_VARIABLE_NAME_BX1_VALID_MIN,
      CCMC_VARIABLE_NAME_BX1_VALID_MAX,
      CCMC_VARIABLE_NAME_BX1_UNITS,
      CCMC_VARIABLE_NAME_BX1_GRID_SYSTEM,
      CCMC_VARIABLE_NAME_BX1_MASK,
      CCMC_VARIABLE_NAME_BX1_DESCRIPTION,
      CCMC_VARIABLE_NAME_BX1_IS_VECTOR,
      CCMC_VARIABLE_NAME_BX1_POSITION_GRID_SYSTEM,
      CCMC_VARIABLE_NAME_BX1_DATA_GRID_SYSTEM,
      CCMC_VARIABLE_NAME_BY1,
      CCMC_VARIABLE_NAME_BY1_DATA_TYPE,
      CCMC_VARIABLE_NAME_BY1_CLASSIFICATION,
      CCMC_VARIABLE_NAME_BY1_VALID_MIN,
      CCMC_VARIABLE_NAME_BY1_VALID_MAX,
      CCMC_VARIABLE_NAME_BY1_UNITS,
      CCMC_VARIABLE_NAME_BY1_GRID_SYSTEM,
      CCMC_VARIABLE_NAME_BY1_MASK,
      CCMC_VARIABLE_NAME_BY1_DESCRIPTION,
      CCMC_VARIABLE_NAME_BY1_IS_VECTOR,
      CCMC_VARIABLE_NAME_BY1_POSITION_GRID_SYSTEM,
      CCMC_VARIABLE_NAME_BY1_DATA_GRID_SYSTEM,
      CCMC_VARIABLE_NAME_BZ1,
      CCMC_VARIABLE_NAME_BZ1_DATA_TYPE,
      CCMC_VARIABLE_NAME_BZ1_CLASSIFICATION,
      CCMC_VARIABLE_NAME_BZ1_VALID_MIN,
      CCMC_VARIABLE_NAME_BZ1_VALID_MAX,
      CCMC_VARIABLE_NAME_BZ1_UNITS,
      CCMC_VARIABLE_NAME_BZ1_GRID_SYSTEM,
      CCMC_VARIABLE_NAME_BZ1_MASK,
      CCMC_VARIABLE_NAME_BZ1_DESCRIPTION,
      CCMC_VARIABLE_NAME_BZ1_IS_VECTOR,
      CCMC_VARIABLE_NAME_BZ1_POSITION_GRID_SYSTEM,
      CCMC_VARIABLE_NAME_BZ1_DATA_GRID_SYSTEM,
      CCMC_VARIABLE_NAME_UX,
      CCMC_VARIABLE_NAME_UX_DATA_TYPE,
      CCMC_VARIABLE_NAME_UX_CLASSIFICATION,
      CCMC_VARIABLE_NAME_UX_VALID_MIN,
      CCMC_VARIABLE_NAME_UX_VALID_MAX,
      CCMC_VARIABLE_NAME_UX_UNITS,
      CCMC_VARIABLE_NAME_UX_GRID_SYSTEM,
      CCMC_VARIABLE_NAME_UX_MASK,
      CCMC_VARIABLE_NAME_UX_DESCRIPTION,
      CCMC_VARIABLE_NAME_UX_IS_VECTOR,
      CCMC_VARIABLE_NAME_UX_POSITION_GRID_SYSTEM,
      CCMC_VARIABLE_NAME_UX_DATA_GRID_SYSTEM,
      CCMC_VARIABLE_NAME_UY,
      CCMC_VARIABLE_NAME_UY_DATA_TYPE,
      CCMC_VARIABLE_NAME_UY_CLASSIFICATION,
      CCMC_VARIABLE_NAME_UY_VALID_MIN,
      CCMC_VARIABLE_NAME_UY_VALID_MAX,
      CCMC_VARIABLE_NAME_UY_UNITS,
      CCMC_VARIABLE_NAME_UY_GRID_SYSTEM,
      CCMC_VARIABLE_NAME_UY_MASK,
      CCMC_VARIABLE_NAME_UY_DESCRIPTION,
      CCMC_VARIABLE_NAME_UY_IS_VECTOR,
      CCMC_VARIABLE_NAME_UY_POSITION_GRID_SYSTEM,
      CCMC_VARIABLE_NAME_UY_DATA_GRID_SYSTEM,
      CCMC_VARIABLE_NAME_UZ,
      CCMC_VARIABLE_NAME_UZ_DATA_TYPE,
      CCMC_VARIABLE_NAME_UZ_CLASSIFICATION,
      CCMC_VARIABLE_NAME_UZ_VALID_MIN,
      CCMC_VARIABLE_NAME_UZ_VALID_MAX,
      CCMC_VARIABLE_NAME_UZ_UNITS,
      CCMC_VARIABLE_NAME_UZ_GRID_SYSTEM,
      CCMC_VARIABLE_NAME_UZ_MASK,
      CCMC_VARIABLE_NAME_UZ_DESCRIPTION,
      CCMC_VARIABLE_NAME_UZ_IS_VECTOR,
      CCMC_VARIABLE_NAME_UZ_POSITION_GRID_SYSTEM,
      CCMC_VARIABLE_NAME_UZ_DATA_GRID_SYSTEM,
      CCMC_VARIABLE_NAME_JX,
      CCMC_VARIABLE_NAME_JX_DATA_TYPE,
      CCMC_VARIABLE_NAME_JX_CLASSIFICATION,
      CCMC_VARIABLE_NAME_JX_VALID_MIN,
      CCMC_VARIABLE_NAME_JX_VALID_MAX,
      CCMC_VARIABLE_NAME_JX_UNITS,
      CCMC_VARIABLE_NAME_JX_GRID_SYSTEM,
      CCMC_VARIABLE_NAME_JX_MASK,
      CCMC_VARIABLE_NAME_JX_DESCRIPTION,
      CCMC_VARIABLE_NAME_JX_IS_VECTOR,
      CCMC_VARIABLE_NAME_JX_POSITION_GRID_SYSTEM,
      CCMC_VARIABLE_NAME_JZ_DATA_GRID_SYSTEM,
      CCMC_VARIABLE_NAME_JY,
      CCMC_VARIABLE_NAME_JY_DATA_TYPE,
      CCMC_VARIABLE_NAME_JY_CLASSIFICATION,
      CCMC_VARIABLE_NAME_JY_VALID_MIN,
      CCMC_VARIABLE_NAME_JY_VALID_MAX,
      CCMC_VARIABLE_NAME_JY_UNITS,
      CCMC_VARIABLE_NAME_JY_GRID_SYSTEM,
      CCMC_VARIABLE_NAME_JY_MASK,
      CCMC_VARIABLE_NAME_JY_DESCRIPTION,
      CCMC_VARIABLE_NAME_JY_IS_VECTOR,
      CCMC_VARIABLE_NAME_JY_POSITION_GRID_SYSTEM,
      CCMC_VARIABLE_NAME_JY_DATA_GRID_SYSTEM,
      CCMC_VARIABLE_NAME_JZ,
      CCMC_VARIABLE_NAME_JZ_DATA_TYPE,
      CCMC_VARIABLE_NAME_JZ_CLASSIFICATION,
      CCMC_VARIABLE_NAME_JZ_VALID_MIN,
      CCMC_VARIABLE_NAME_JZ_VALID_MAX,
      CCMC_VARIABLE_NAME_JZ_UNITS,
      CCMC_VARIABLE_NAME_JZ_GRID_SYSTEM,
      CCMC_VARIABLE_NAME_JZ_MASK,
      CCMC_VARIABLE_NAME_JZ_DESCRIPTION,
      CCMC_VARIABLE_NAME_JZ_IS_VECTOR,
      CCMC_VARIABLE_NAME_JZ_POSITION_GRID_SYSTEM,
      CCMC_VARIABLE_NAME_JZ_DATA_GRID_SYSTEM,
      CCMC_VARIABLE_NAME_RHO,
      CCMC_VARIABLE_NAME_RHO_DATA_TYPE,
      CCMC_VARIABLE_NAME_RHO_CLASSIFICATION,
      CCMC_VARIABLE_NAME_RHO_VALID_MIN,
      CCMC_VARIABLE_NAME_RHO_VALID_MAX,
      CCMC_VARIABLE_NAME_RHO_UNITS,
      CCMC_VARIABLE_NAME_RHO_GRID_SYSTEM,
      CCMC_VARIABLE_NAME_RHO_MASK,
      CCMC_VARIABLE_NAME_RHO_DESCRIPTION,
      CCMC_VARIABLE_NAME_RHO_IS_VECTOR,
      CCMC_VARIABLE_NAME_RHO_POSITION_GRID_SYSTEM,
      CCMC_VARIABLE_NAME_RHO_DATA_GRID_SYSTEM,
      CCMC_VARIABLE_NAME_RESIS,
      CCMC_VARIABLE_NAME_RESIS_DATA_TYPE,
      CCMC_VARIABLE_NAME_RESIS_CLASSIFICATION,
      CCMC_VARIABLE_NAME_RESIS_VALID_MIN,
      CCMC_VARIABLE_NAME_RESIS_VALID_MAX,
      CCMC_VARIABLE_NAME_RESIS_UNITS,
      CCMC_VARIABLE_NAME_RESIS_GRID_SYSTEM,
      CCMC_VARIABLE_NAME_RESIS_MASK,
      CCMC_VARIABLE_NAME_RESIS_DESCRIPTION,
      CCMC_VARIABLE_NAME_RESIS_IS_VECTOR,
      CCMC_VARIABLE_NAME_RESIS_POSITION_GRID_SYSTEM,
      CCMC_VARIABLE_NAME_RESIS_DATA_GRID_SYSTEM,
      CCMC_VARIABLE_NAME_P,
      CCMC_VARIABLE_NAME_P_DATA_TYPE,
      CCMC_VARIABLE_NAME_P_CLASSIFICATION,
      CCMC_VARIABLE_NAME_P_VALID_MIN,
      CCMC_VARIABLE_NAME_P_VALID_MAX,
      CCMC_VARIABLE_NAME_P_UNITS,
      CCMC_VARIABLE_NAME_P_GRID_SYSTEM,
      CCMC_VARIABLE_NAME_P_MASK,
      CCMC_VARIABLE_NAME_P_DESCRIPTION,
      CCMC_VARIABLE_NAME_P_IS_VECTOR,
      CCMC_VARIABLE_NAME_P_POSITION_GRID_SYSTEM,
      CCMC_VARIABLE_NAME_P_DATA_GRID_SYSTEM };

/***************************************************************** CTIP ***********************************************/

struct registered_ctip_variables
{

   char *ccmc_var_name;
   char *ccmc_var_data_type;
   char *ccmc_var_classification;
   float valid_min;
   float valid_max;
   char *units;
   char *grid_system;
   float mask;
   char *description;
   int is_vector_component;
   char *position_grid_system;
   char *data_grid_system;
}static registered_ctip_var_list[] =
{

      CCMC_VARIABLE_NAME_X,
      CCMC_VARIABLE_NAME_X_DATA_TYPE,
      CCMC_VARIABLE_NAME_X_CLASSIFICATION,
      CCMC_VARIABLE_NAME_X_VALID_MIN,
      CCMC_VARIABLE_NAME_X_VALID_MAX,
      CCMC_VARIABLE_NAME_X_UNITS,
      CCMC_VARIABLE_NAME_X_GRID_SYSTEM,
      CCMC_VARIABLE_NAME_X_MASK,
      CCMC_VARIABLE_NAME_X_DESCRIPTION,
      CCMC_VARIABLE_NAME_X_IS_VECTOR,
      CCMC_VARIABLE_NAME_X_POSITION_GRID_SYSTEM,
      CCMC_VARIABLE_NAME_X_DATA_GRID_SYSTEM,
      CCMC_VARIABLE_NAME_Y,
      CCMC_VARIABLE_NAME_Y_DATA_TYPE,
      CCMC_VARIABLE_NAME_Y_CLASSIFICATION,
      CCMC_VARIABLE_NAME_Y_VALID_MIN,
      CCMC_VARIABLE_NAME_Y_VALID_MAX,
      CCMC_VARIABLE_NAME_Y_UNITS,
      CCMC_VARIABLE_NAME_Y_GRID_SYSTEM,
      CCMC_VARIABLE_NAME_Y_MASK,
      CCMC_VARIABLE_NAME_Y_DESCRIPTION,
      CCMC_VARIABLE_NAME_Y_IS_VECTOR,
      CCMC_VARIABLE_NAME_Y_POSITION_GRID_SYSTEM,
      CCMC_VARIABLE_NAME_Y_DATA_GRID_SYSTEM,
      CCMC_VARIABLE_NAME_Z,
      CCMC_VARIABLE_NAME_Z_DATA_TYPE,
      CCMC_VARIABLE_NAME_Z_CLASSIFICATION,
      CCMC_VARIABLE_NAME_Z_VALID_MIN,
      CCMC_VARIABLE_NAME_Z_VALID_MAX,
      CCMC_VARIABLE_NAME_Z_UNITS,
      CCMC_VARIABLE_NAME_Z_GRID_SYSTEM,
      CCMC_VARIABLE_NAME_Z_MASK,
      CCMC_VARIABLE_NAME_Z_DESCRIPTION,
      CCMC_VARIABLE_NAME_Z_IS_VECTOR,
      CCMC_VARIABLE_NAME_Z_POSITION_GRID_SYSTEM,
      CCMC_VARIABLE_NAME_Z_DATA_GRID_SYSTEM,
      CCMC_VARIABLE_NAME_H,
      CCMC_VARIABLE_NAME_H_DATA_TYPE,
      CCMC_VARIABLE_NAME_H_CLASSIFICATION,
      CCMC_VARIABLE_NAME_H_VALID_MIN,
      CCMC_VARIABLE_NAME_H_VALID_MAX,
      CCMC_VARIABLE_NAME_H_UNITS,
      CCMC_VARIABLE_NAME_H_GRID_SYSTEM,
      CCMC_VARIABLE_NAME_H_MASK,
      CCMC_VARIABLE_NAME_H_DESCRIPTION,
      CCMC_VARIABLE_NAME_H_IS_VECTOR,
      CCMC_VARIABLE_NAME_H_POSITION_GRID_SYSTEM,
      CCMC_VARIABLE_NAME_H_DATA_GRID_SYSTEM,
      CCMC_VARIABLE_NAME_Vn_X,
      CCMC_VARIABLE_NAME_Vn_X_DATA_TYPE,
      CCMC_VARIABLE_NAME_Vn_X_CLASSIFICATION,
      CCMC_VARIABLE_NAME_Vn_X_VALID_MIN,
      CCMC_VARIABLE_NAME_Vn_X_VALID_MAX,
      CCMC_VARIABLE_NAME_Vn_X_UNITS,
      CCMC_VARIABLE_NAME_Vn_X_GRID_SYSTEM,
      CCMC_VARIABLE_NAME_Vn_X_MASK,
      CCMC_VARIABLE_NAME_Vn_X_DESCRIPTION,
      CCMC_VARIABLE_NAME_Vn_X_IS_VECTOR,
      CCMC_VARIABLE_NAME_Vn_X_POSITION_GRID_SYSTEM,
      CCMC_VARIABLE_NAME_Vn_X_DATA_GRID_SYSTEM,
      CCMC_VARIABLE_NAME_Vn_Y,
      CCMC_VARIABLE_NAME_Vn_Y_DATA_TYPE,
      CCMC_VARIABLE_NAME_Vn_Y_CLASSIFICATION,
      CCMC_VARIABLE_NAME_Vn_Y_VALID_MIN,
      CCMC_VARIABLE_NAME_Vn_Y_VALID_MAX,
      CCMC_VARIABLE_NAME_Vn_Y_UNITS,
      CCMC_VARIABLE_NAME_Vn_Y_GRID_SYSTEM,
      CCMC_VARIABLE_NAME_Vn_Y_MASK,
      CCMC_VARIABLE_NAME_Vn_Y_DESCRIPTION,
      CCMC_VARIABLE_NAME_Vn_Y_IS_VECTOR,
      CCMC_VARIABLE_NAME_Vn_Y_POSITION_GRID_SYSTEM,
      CCMC_VARIABLE_NAME_Vn_Y_DATA_GRID_SYSTEM,
      CCMC_VARIABLE_NAME_Vn_Z,
      CCMC_VARIABLE_NAME_Vn_Z_DATA_TYPE,
      CCMC_VARIABLE_NAME_Vn_Z_CLASSIFICATION,
      CCMC_VARIABLE_NAME_Vn_Z_VALID_MIN,
      CCMC_VARIABLE_NAME_Vn_Z_VALID_MAX,
      CCMC_VARIABLE_NAME_Vn_Z_UNITS,
      CCMC_VARIABLE_NAME_Vn_Z_GRID_SYSTEM,
      CCMC_VARIABLE_NAME_Vn_Z_MASK,
      CCMC_VARIABLE_NAME_Vn_Z_DESCRIPTION,
      CCMC_VARIABLE_NAME_Vn_Z_IS_VECTOR,
      CCMC_VARIABLE_NAME_Vn_Z_POSITION_GRID_SYSTEM,
      CCMC_VARIABLE_NAME_Vn_Z_DATA_GRID_SYSTEM,
      CCMC_VARIABLE_NAME_Vi_X,
      CCMC_VARIABLE_NAME_Vi_X_DATA_TYPE,
      CCMC_VARIABLE_NAME_Vi_X_CLASSIFICATION,
      CCMC_VARIABLE_NAME_Vi_X_VALID_MIN,
      CCMC_VARIABLE_NAME_Vi_X_VALID_MAX,
      CCMC_VARIABLE_NAME_Vi_X_UNITS,
      CCMC_VARIABLE_NAME_Vi_X_GRID_SYSTEM,
      CCMC_VARIABLE_NAME_Vi_X_MASK,
      CCMC_VARIABLE_NAME_Vi_X_DESCRIPTION,
      CCMC_VARIABLE_NAME_Vi_X_IS_VECTOR,
      CCMC_VARIABLE_NAME_Vi_X_POSITION_GRID_SYSTEM,
      CCMC_VARIABLE_NAME_Vi_X_DATA_GRID_SYSTEM,
      CCMC_VARIABLE_NAME_Vi_Y,
      CCMC_VARIABLE_NAME_Vi_Y_DATA_TYPE,
      CCMC_VARIABLE_NAME_Vi_Y_CLASSIFICATION,
      CCMC_VARIABLE_NAME_Vi_Y_VALID_MIN,
      CCMC_VARIABLE_NAME_Vi_Y_VALID_MAX,
      CCMC_VARIABLE_NAME_Vi_Y_UNITS,
      CCMC_VARIABLE_NAME_Vi_Y_GRID_SYSTEM,
      CCMC_VARIABLE_NAME_Vi_Y_MASK,
      CCMC_VARIABLE_NAME_Vi_Y_DESCRIPTION,
      CCMC_VARIABLE_NAME_Vi_Y_IS_VECTOR,
      CCMC_VARIABLE_NAME_Vi_Y_POSITION_GRID_SYSTEM,
      CCMC_VARIABLE_NAME_Vi_Y_DATA_GRID_SYSTEM,
      CCMC_VARIABLE_NAME_N_E,
      CCMC_VARIABLE_NAME_N_E_DATA_TYPE,
      CCMC_VARIABLE_NAME_N_E_CLASSIFICATION,
      CCMC_VARIABLE_NAME_N_E_VALID_MIN,
      CCMC_VARIABLE_NAME_N_E_VALID_MAX,
      CCMC_VARIABLE_NAME_N_E_UNITS,
      CCMC_VARIABLE_NAME_N_E_GRID_SYSTEM,
      CCMC_VARIABLE_NAME_N_E_MASK,
      CCMC_VARIABLE_NAME_N_E_DESCRIPTION,
      CCMC_VARIABLE_NAME_N_E_IS_VECTOR,
      CCMC_VARIABLE_NAME_N_E_POSITION_GRID_SYSTEM,
      CCMC_VARIABLE_NAME_N_E_DATA_GRID_SYSTEM,
      CCMC_VARIABLE_NAME_Tn,
      CCMC_VARIABLE_NAME_Tn_DATA_TYPE,
      CCMC_VARIABLE_NAME_Tn_CLASSIFICATION,
      CCMC_VARIABLE_NAME_Tn_VALID_MIN,
      CCMC_VARIABLE_NAME_Tn_VALID_MAX,
      CCMC_VARIABLE_NAME_Tn_UNITS,
      CCMC_VARIABLE_NAME_Tn_GRID_SYSTEM,
      CCMC_VARIABLE_NAME_Tn_MASK,
      CCMC_VARIABLE_NAME_Tn_DESCRIPTION,
      CCMC_VARIABLE_NAME_Tn_IS_VECTOR,
      CCMC_VARIABLE_NAME_Tn_POSITION_GRID_SYSTEM,
      CCMC_VARIABLE_NAME_Tn_DATA_GRID_SYSTEM,
      CCMC_VARIABLE_NAME_RMT,
      CCMC_VARIABLE_NAME_RMT_DATA_TYPE,
      CCMC_VARIABLE_NAME_RMT_CLASSIFICATION,
      CCMC_VARIABLE_NAME_RMT_VALID_MIN,
      CCMC_VARIABLE_NAME_RMT_VALID_MAX,
      CCMC_VARIABLE_NAME_RMT_UNITS,
      CCMC_VARIABLE_NAME_RMT_GRID_SYSTEM,
      CCMC_VARIABLE_NAME_RMT_MASK,
      CCMC_VARIABLE_NAME_RMT_DESCRIPTION,
      CCMC_VARIABLE_NAME_RMT_IS_VECTOR,
      CCMC_VARIABLE_NAME_RMT_POSITION_GRID_SYSTEM,
      CCMC_VARIABLE_NAME_RMT_DATA_GRID_SYSTEM,
      CCMC_VARIABLE_NAME_N_O,
      CCMC_VARIABLE_NAME_N_O_DATA_TYPE,
      CCMC_VARIABLE_NAME_N_O_CLASSIFICATION,
      CCMC_VARIABLE_NAME_N_O_VALID_MIN,
      CCMC_VARIABLE_NAME_N_O_VALID_MAX,
      CCMC_VARIABLE_NAME_N_O_UNITS,
      CCMC_VARIABLE_NAME_N_O_GRID_SYSTEM,
      CCMC_VARIABLE_NAME_N_O_MASK,
      CCMC_VARIABLE_NAME_N_O_DESCRIPTION,
      CCMC_VARIABLE_NAME_N_O_IS_VECTOR,
      CCMC_VARIABLE_NAME_N_O_POSITION_GRID_SYSTEM,
      CCMC_VARIABLE_NAME_N_O_DATA_GRID_SYSTEM,
      CCMC_VARIABLE_NAME_N_N2,
      CCMC_VARIABLE_NAME_N_N2_DATA_TYPE,
      CCMC_VARIABLE_NAME_N_N2_CLASSIFICATION,
      CCMC_VARIABLE_NAME_N_N2_VALID_MIN,
      CCMC_VARIABLE_NAME_N_N2_VALID_MAX,
      CCMC_VARIABLE_NAME_N_N2_UNITS,
      CCMC_VARIABLE_NAME_N_N2_GRID_SYSTEM,
      CCMC_VARIABLE_NAME_N_N2_MASK,
      CCMC_VARIABLE_NAME_N_N2_DESCRIPTION,
      CCMC_VARIABLE_NAME_N_N2_IS_VECTOR,
      CCMC_VARIABLE_NAME_N_N2_POSITION_GRID_SYSTEM,
      CCMC_VARIABLE_NAME_N_N2_DATA_GRID_SYSTEM,
      CCMC_VARIABLE_NAME_N_O2,
      CCMC_VARIABLE_NAME_N_O2_DATA_TYPE,
      CCMC_VARIABLE_NAME_N_O2_CLASSIFICATION,
      CCMC_VARIABLE_NAME_N_O2_VALID_MIN,
      CCMC_VARIABLE_NAME_N_O2_VALID_MAX,
      CCMC_VARIABLE_NAME_N_O2_UNITS,
      CCMC_VARIABLE_NAME_N_O2_GRID_SYSTEM,
      CCMC_VARIABLE_NAME_N_O2_MASK,
      CCMC_VARIABLE_NAME_N_O2_DESCRIPTION,
      CCMC_VARIABLE_NAME_N_O2_IS_VECTOR,
      CCMC_VARIABLE_NAME_N_O2_POSITION_GRID_SYSTEM,
      CCMC_VARIABLE_NAME_N_O2_DATA_GRID_SYSTEM,
      CCMC_VARIABLE_NAME_SIGMA_P,
      CCMC_VARIABLE_NAME_SIGMA_P_DATA_TYPE,
      CCMC_VARIABLE_NAME_SIGMA_P_CLASSIFICATION,
      CCMC_VARIABLE_NAME_SIGMA_P_VALID_MIN,
      CCMC_VARIABLE_NAME_SIGMA_P_VALID_MAX,
      CCMC_VARIABLE_NAME_SIGMA_P_UNITS,
      CCMC_VARIABLE_NAME_SIGMA_P_GRID_SYSTEM,
      CCMC_VARIABLE_NAME_SIGMA_P_MASK,
      CCMC_VARIABLE_NAME_SIGMA_P_DESCRIPTION,
      CCMC_VARIABLE_NAME_SIGMA_P_IS_VECTOR,
      CCMC_VARIABLE_NAME_SIGMA_P_POSITION_GRID_SYSTEM,
      CCMC_VARIABLE_NAME_SIGMA_P_DATA_GRID_SYSTEM,
      CCMC_VARIABLE_NAME_SIGMA_H,
      CCMC_VARIABLE_NAME_SIGMA_H_DATA_TYPE,
      CCMC_VARIABLE_NAME_SIGMA_H_CLASSIFICATION,
      CCMC_VARIABLE_NAME_SIGMA_H_VALID_MIN,
      CCMC_VARIABLE_NAME_SIGMA_H_VALID_MAX,
      CCMC_VARIABLE_NAME_SIGMA_H_UNITS,
      CCMC_VARIABLE_NAME_SIGMA_H_GRID_SYSTEM,
      CCMC_VARIABLE_NAME_SIGMA_H_MASK,
      CCMC_VARIABLE_NAME_SIGMA_H_DESCRIPTION,
      CCMC_VARIABLE_NAME_SIGMA_H_IS_VECTOR,
      CCMC_VARIABLE_NAME_SIGMA_H_POSITION_GRID_SYSTEM,
      CCMC_VARIABLE_NAME_SIGMA_H_DATA_GRID_SYSTEM };

/***************************************************************** ENLIL ***********************************************/

struct registered_enlil_variables
{

   char *ccmc_var_name;
   char *ccmc_var_data_type;
   char *ccmc_var_classification;
   float valid_min;
   float valid_max;
   char *units;
   char *grid_system;
   float mask;
   char *description;
   int is_vector_component;
   char *position_grid_system;
   char *data_grid_system;
}static registered_enlil_var_list[] =
{

      CCMC_VARIABLE_NAME_R,
      CCMC_VARIABLE_NAME_R_DATA_TYPE,
      CCMC_VARIABLE_NAME_R_CLASSIFICATION,
      CCMC_VARIABLE_NAME_R_VALID_MIN,
      CCMC_VARIABLE_NAME_R_VALID_MAX,
      CCMC_VARIABLE_NAME_R_UNITS,
      CCMC_VARIABLE_NAME_R_GRID_SYSTEM,
      CCMC_VARIABLE_NAME_R_MASK,
      CCMC_VARIABLE_NAME_R_DESCRIPTION,
      CCMC_VARIABLE_NAME_R_IS_VECTOR,
      CCMC_VARIABLE_NAME_R_POSITION_GRID_SYSTEM,
      CCMC_VARIABLE_NAME_R_DATA_GRID_SYSTEM,
      CCMC_VARIABLE_NAME_R1,
      CCMC_VARIABLE_NAME_R1_DATA_TYPE,
      CCMC_VARIABLE_NAME_R1_CLASSIFICATION,
      CCMC_VARIABLE_NAME_R1_VALID_MIN,
      CCMC_VARIABLE_NAME_R1_VALID_MAX,
      CCMC_VARIABLE_NAME_R1_UNITS,
      CCMC_VARIABLE_NAME_R1_GRID_SYSTEM,
      CCMC_VARIABLE_NAME_R1_MASK,
      CCMC_VARIABLE_NAME_R1_DESCRIPTION,
      CCMC_VARIABLE_NAME_R1_IS_VECTOR,
      CCMC_VARIABLE_NAME_R1_POSITION_GRID_SYSTEM,
      CCMC_VARIABLE_NAME_R1_DATA_GRID_SYSTEM,
      CCMC_VARIABLE_NAME_R2,
      CCMC_VARIABLE_NAME_R2_DATA_TYPE,
      CCMC_VARIABLE_NAME_R2_CLASSIFICATION,
      CCMC_VARIABLE_NAME_R2_VALID_MIN,
      CCMC_VARIABLE_NAME_R2_VALID_MAX,
      CCMC_VARIABLE_NAME_R2_UNITS,
      CCMC_VARIABLE_NAME_R2_GRID_SYSTEM,
      CCMC_VARIABLE_NAME_R2_MASK,
      CCMC_VARIABLE_NAME_R2_DESCRIPTION,
      CCMC_VARIABLE_NAME_R2_IS_VECTOR,
      CCMC_VARIABLE_NAME_R2_POSITION_GRID_SYSTEM,
      CCMC_VARIABLE_NAME_R2_DATA_GRID_SYSTEM,
      CCMC_VARIABLE_NAME_R3,
      CCMC_VARIABLE_NAME_R3_DATA_TYPE,
      CCMC_VARIABLE_NAME_R3_CLASSIFICATION,
      CCMC_VARIABLE_NAME_R3_VALID_MIN,
      CCMC_VARIABLE_NAME_R3_VALID_MAX,
      CCMC_VARIABLE_NAME_R3_UNITS,
      CCMC_VARIABLE_NAME_R3_GRID_SYSTEM,
      CCMC_VARIABLE_NAME_R3_MASK,
      CCMC_VARIABLE_NAME_R3_DESCRIPTION,
      CCMC_VARIABLE_NAME_R3_IS_VECTOR,
      CCMC_VARIABLE_NAME_R3_POSITION_GRID_SYSTEM,
      CCMC_VARIABLE_NAME_R3_DATA_GRID_SYSTEM,
      CCMC_VARIABLE_NAME_R4,
      CCMC_VARIABLE_NAME_R4_DATA_TYPE,
      CCMC_VARIABLE_NAME_R4_CLASSIFICATION,
      CCMC_VARIABLE_NAME_R4_VALID_MIN,
      CCMC_VARIABLE_NAME_R4_VALID_MAX,
      CCMC_VARIABLE_NAME_R4_UNITS,
      CCMC_VARIABLE_NAME_R4_GRID_SYSTEM,
      CCMC_VARIABLE_NAME_R4_MASK,
      CCMC_VARIABLE_NAME_R4_DESCRIPTION,
      CCMC_VARIABLE_NAME_R4_IS_VECTOR,
      CCMC_VARIABLE_NAME_R4_POSITION_GRID_SYSTEM,
      CCMC_VARIABLE_NAME_R4_DATA_GRID_SYSTEM,
      CCMC_VARIABLE_NAME_R5,
      CCMC_VARIABLE_NAME_R5_DATA_TYPE,
      CCMC_VARIABLE_NAME_R5_CLASSIFICATION,
      CCMC_VARIABLE_NAME_R5_VALID_MIN,
      CCMC_VARIABLE_NAME_R5_VALID_MAX,
      CCMC_VARIABLE_NAME_R5_UNITS,
      CCMC_VARIABLE_NAME_R5_GRID_SYSTEM,
      CCMC_VARIABLE_NAME_R5_MASK,
      CCMC_VARIABLE_NAME_R5_DESCRIPTION,
      CCMC_VARIABLE_NAME_R5_IS_VECTOR,
      CCMC_VARIABLE_NAME_R5_POSITION_GRID_SYSTEM,
      CCMC_VARIABLE_NAME_R5_DATA_GRID_SYSTEM,
      CCMC_VARIABLE_NAME_PHI,
      CCMC_VARIABLE_NAME_PHI_DATA_TYPE,
      CCMC_VARIABLE_NAME_PHI_CLASSIFICATION,
      CCMC_VARIABLE_NAME_PHI_VALID_MIN,
      CCMC_VARIABLE_NAME_PHI_VALID_MAX,
      CCMC_VARIABLE_NAME_PHI_UNITS,
      CCMC_VARIABLE_NAME_PHI_GRID_SYSTEM,
      CCMC_VARIABLE_NAME_PHI_MASK,
      CCMC_VARIABLE_NAME_PHI_DESCRIPTION,
      CCMC_VARIABLE_NAME_PHI_IS_VECTOR,
      CCMC_VARIABLE_NAME_PHI_POSITION_GRID_SYSTEM,
      CCMC_VARIABLE_NAME_PHI_DATA_GRID_SYSTEM,
      CCMC_VARIABLE_NAME_PHI1,
      CCMC_VARIABLE_NAME_PHI1_DATA_TYPE,
      CCMC_VARIABLE_NAME_PHI1_CLASSIFICATION,
      CCMC_VARIABLE_NAME_PHI1_VALID_MIN,
      CCMC_VARIABLE_NAME_PHI1_VALID_MAX,
      CCMC_VARIABLE_NAME_PHI1_UNITS,
      CCMC_VARIABLE_NAME_PHI1_GRID_SYSTEM,
      CCMC_VARIABLE_NAME_PHI1_MASK,
      CCMC_VARIABLE_NAME_PHI1_DESCRIPTION,
      CCMC_VARIABLE_NAME_PHI1_IS_VECTOR,
      CCMC_VARIABLE_NAME_PHI1_POSITION_GRID_SYSTEM,
      CCMC_VARIABLE_NAME_PHI1_DATA_GRID_SYSTEM,
      CCMC_VARIABLE_NAME_PHI2,
      CCMC_VARIABLE_NAME_PHI2_DATA_TYPE,
      CCMC_VARIABLE_NAME_PHI2_CLASSIFICATION,
      CCMC_VARIABLE_NAME_PHI2_VALID_MIN,
      CCMC_VARIABLE_NAME_PHI2_VALID_MAX,
      CCMC_VARIABLE_NAME_PHI2_UNITS,
      CCMC_VARIABLE_NAME_PHI2_GRID_SYSTEM,
      CCMC_VARIABLE_NAME_PHI2_MASK,
      CCMC_VARIABLE_NAME_PHI2_DESCRIPTION,
      CCMC_VARIABLE_NAME_PHI2_IS_VECTOR,
      CCMC_VARIABLE_NAME_PHI2_POSITION_GRID_SYSTEM,
      CCMC_VARIABLE_NAME_PHI2_DATA_GRID_SYSTEM,
      CCMC_VARIABLE_NAME_PHI3,
      CCMC_VARIABLE_NAME_PHI3_DATA_TYPE,
      CCMC_VARIABLE_NAME_PHI3_CLASSIFICATION,
      CCMC_VARIABLE_NAME_PHI3_VALID_MIN,
      CCMC_VARIABLE_NAME_PHI3_VALID_MAX,
      CCMC_VARIABLE_NAME_PHI3_UNITS,
      CCMC_VARIABLE_NAME_PHI3_GRID_SYSTEM,
      CCMC_VARIABLE_NAME_PHI3_MASK,
      CCMC_VARIABLE_NAME_PHI3_DESCRIPTION,
      CCMC_VARIABLE_NAME_PHI3_IS_VECTOR,
      CCMC_VARIABLE_NAME_PHI3_POSITION_GRID_SYSTEM,
      CCMC_VARIABLE_NAME_PHI3_DATA_GRID_SYSTEM,
      CCMC_VARIABLE_NAME_PHI4,
      CCMC_VARIABLE_NAME_PHI4_DATA_TYPE,
      CCMC_VARIABLE_NAME_PHI4_CLASSIFICATION,
      CCMC_VARIABLE_NAME_PHI4_VALID_MIN,
      CCMC_VARIABLE_NAME_PHI4_VALID_MAX,
      CCMC_VARIABLE_NAME_PHI4_UNITS,
      CCMC_VARIABLE_NAME_PHI4_GRID_SYSTEM,
      CCMC_VARIABLE_NAME_PHI4_MASK,
      CCMC_VARIABLE_NAME_PHI4_DESCRIPTION,
      CCMC_VARIABLE_NAME_PHI4_IS_VECTOR,
      CCMC_VARIABLE_NAME_PHI4_POSITION_GRID_SYSTEM,
      CCMC_VARIABLE_NAME_PHI4_DATA_GRID_SYSTEM,
      CCMC_VARIABLE_NAME_PHI5,
      CCMC_VARIABLE_NAME_PHI5_DATA_TYPE,
      CCMC_VARIABLE_NAME_PHI5_CLASSIFICATION,
      CCMC_VARIABLE_NAME_PHI5_VALID_MIN,
      CCMC_VARIABLE_NAME_PHI5_VALID_MAX,
      CCMC_VARIABLE_NAME_PHI5_UNITS,
      CCMC_VARIABLE_NAME_PHI5_GRID_SYSTEM,
      CCMC_VARIABLE_NAME_PHI5_MASK,
      CCMC_VARIABLE_NAME_PHI5_DESCRIPTION,
      CCMC_VARIABLE_NAME_PHI5_IS_VECTOR,
      CCMC_VARIABLE_NAME_PHI5_POSITION_GRID_SYSTEM,
      CCMC_VARIABLE_NAME_PHI5_DATA_GRID_SYSTEM,
      CCMC_VARIABLE_NAME_THETA,
      CCMC_VARIABLE_NAME_THETA_DATA_TYPE,
      CCMC_VARIABLE_NAME_THETA_CLASSIFICATION,
      CCMC_VARIABLE_NAME_THETA_VALID_MIN,
      CCMC_VARIABLE_NAME_THETA_VALID_MAX,
      CCMC_VARIABLE_NAME_THETA_UNITS,
      CCMC_VARIABLE_NAME_THETA_GRID_SYSTEM,
      CCMC_VARIABLE_NAME_THETA_MASK,
      CCMC_VARIABLE_NAME_THETA_DESCRIPTION,
      CCMC_VARIABLE_NAME_THETA_IS_VECTOR,
      CCMC_VARIABLE_NAME_THETA_POSITION_GRID_SYSTEM,
      CCMC_VARIABLE_NAME_THETA_DATA_GRID_SYSTEM,
      CCMC_VARIABLE_NAME_THETA1,
      CCMC_VARIABLE_NAME_THETA1_DATA_TYPE,
      CCMC_VARIABLE_NAME_THETA1_CLASSIFICATION,
      CCMC_VARIABLE_NAME_THETA1_VALID_MIN,
      CCMC_VARIABLE_NAME_THETA1_VALID_MAX,
      CCMC_VARIABLE_NAME_THETA1_UNITS,
      CCMC_VARIABLE_NAME_THETA1_GRID_SYSTEM,
      CCMC_VARIABLE_NAME_THETA1_MASK,
      CCMC_VARIABLE_NAME_THETA1_DESCRIPTION,
      CCMC_VARIABLE_NAME_THETA1_IS_VECTOR,
      CCMC_VARIABLE_NAME_THETA1_POSITION_GRID_SYSTEM,
      CCMC_VARIABLE_NAME_THETA1_DATA_GRID_SYSTEM,
      CCMC_VARIABLE_NAME_THETA2,
      CCMC_VARIABLE_NAME_THETA2_DATA_TYPE,
      CCMC_VARIABLE_NAME_THETA2_CLASSIFICATION,
      CCMC_VARIABLE_NAME_THETA2_VALID_MIN,
      CCMC_VARIABLE_NAME_THETA2_VALID_MAX,
      CCMC_VARIABLE_NAME_THETA2_UNITS,
      CCMC_VARIABLE_NAME_THETA2_GRID_SYSTEM,
      CCMC_VARIABLE_NAME_THETA2_MASK,
      CCMC_VARIABLE_NAME_THETA2_DESCRIPTION,
      CCMC_VARIABLE_NAME_THETA2_IS_VECTOR,
      CCMC_VARIABLE_NAME_THETA2_POSITION_GRID_SYSTEM,
      CCMC_VARIABLE_NAME_THETA2_DATA_GRID_SYSTEM,
      CCMC_VARIABLE_NAME_THETA3,
      CCMC_VARIABLE_NAME_THETA3_DATA_TYPE,
      CCMC_VARIABLE_NAME_THETA3_CLASSIFICATION,
      CCMC_VARIABLE_NAME_THETA3_VALID_MIN,
      CCMC_VARIABLE_NAME_THETA3_VALID_MAX,
      CCMC_VARIABLE_NAME_THETA3_UNITS,
      CCMC_VARIABLE_NAME_THETA3_GRID_SYSTEM,
      CCMC_VARIABLE_NAME_THETA3_MASK,
      CCMC_VARIABLE_NAME_THETA3_DESCRIPTION,
      CCMC_VARIABLE_NAME_THETA3_IS_VECTOR,
      CCMC_VARIABLE_NAME_THETA3_POSITION_GRID_SYSTEM,
      CCMC_VARIABLE_NAME_THETA3_DATA_GRID_SYSTEM,
      CCMC_VARIABLE_NAME_THETA4,
      CCMC_VARIABLE_NAME_THETA4_DATA_TYPE,
      CCMC_VARIABLE_NAME_THETA4_CLASSIFICATION,
      CCMC_VARIABLE_NAME_THETA4_VALID_MIN,
      CCMC_VARIABLE_NAME_THETA4_VALID_MAX,
      CCMC_VARIABLE_NAME_THETA4_UNITS,
      CCMC_VARIABLE_NAME_THETA4_GRID_SYSTEM,
      CCMC_VARIABLE_NAME_THETA4_MASK,
      CCMC_VARIABLE_NAME_THETA4_DESCRIPTION,
      CCMC_VARIABLE_NAME_THETA4_IS_VECTOR,
      CCMC_VARIABLE_NAME_THETA4_POSITION_GRID_SYSTEM,
      CCMC_VARIABLE_NAME_THETA4_DATA_GRID_SYSTEM,
      CCMC_VARIABLE_NAME_THETA5,
      CCMC_VARIABLE_NAME_THETA5_DATA_TYPE,
      CCMC_VARIABLE_NAME_THETA5_CLASSIFICATION,
      CCMC_VARIABLE_NAME_THETA5_VALID_MIN,
      CCMC_VARIABLE_NAME_THETA5_VALID_MAX,
      CCMC_VARIABLE_NAME_THETA5_UNITS,
      CCMC_VARIABLE_NAME_THETA5_GRID_SYSTEM,
      CCMC_VARIABLE_NAME_THETA5_MASK,
      CCMC_VARIABLE_NAME_THETA5_DESCRIPTION,
      CCMC_VARIABLE_NAME_THETA5_IS_VECTOR,
      CCMC_VARIABLE_NAME_THETA5_POSITION_GRID_SYSTEM,
      CCMC_VARIABLE_NAME_THETA5_DATA_GRID_SYSTEM,
      CCMC_VARIABLE_NAME_RHO,
      CCMC_VARIABLE_NAME_RHO_DATA_TYPE,
      CCMC_VARIABLE_NAME_RHO_CLASSIFICATION,
      CCMC_VARIABLE_NAME_RHO_VALID_MIN,
      CCMC_VARIABLE_NAME_RHO_VALID_MAX,
      CCMC_VARIABLE_NAME_RHO_UNITS,
      CCMC_VARIABLE_NAME_RHO_GRID_SYSTEM,
      CCMC_VARIABLE_NAME_RHO_MASK,
      CCMC_VARIABLE_NAME_RHO_DESCRIPTION,
      CCMC_VARIABLE_NAME_RHO_IS_VECTOR,
      CCMC_VARIABLE_NAME_RHO_POSITION_GRID_SYSTEM,
      CCMC_VARIABLE_NAME_RHO_DATA_GRID_SYSTEM,
      CCMC_VARIABLE_NAME_RHO_1,
      CCMC_VARIABLE_NAME_RHO_1_DATA_TYPE,
      CCMC_VARIABLE_NAME_RHO_1_CLASSIFICATION,
      CCMC_VARIABLE_NAME_RHO_1_VALID_MIN,
      CCMC_VARIABLE_NAME_RHO_1_VALID_MAX,
      CCMC_VARIABLE_NAME_RHO_1_UNITS,
      CCMC_VARIABLE_NAME_RHO_1_GRID_SYSTEM,
      CCMC_VARIABLE_NAME_RHO_1_MASK,
      CCMC_VARIABLE_NAME_RHO_1_DESCRIPTION,
      CCMC_VARIABLE_NAME_RHO_1_IS_VECTOR,
      CCMC_VARIABLE_NAME_RHO_1_POSITION_GRID_SYSTEM,
      CCMC_VARIABLE_NAME_RHO_1_DATA_GRID_SYSTEM,
      CCMC_VARIABLE_NAME_RHO_2,
      CCMC_VARIABLE_NAME_RHO_2_DATA_TYPE,
      CCMC_VARIABLE_NAME_RHO_2_CLASSIFICATION,
      CCMC_VARIABLE_NAME_RHO_2_VALID_MIN,
      CCMC_VARIABLE_NAME_RHO_2_VALID_MAX,
      CCMC_VARIABLE_NAME_RHO_2_UNITS,
      CCMC_VARIABLE_NAME_RHO_2_GRID_SYSTEM,
      CCMC_VARIABLE_NAME_RHO_2_MASK,
      CCMC_VARIABLE_NAME_RHO_2_DESCRIPTION,
      CCMC_VARIABLE_NAME_RHO_2_IS_VECTOR,
      CCMC_VARIABLE_NAME_RHO_2_POSITION_GRID_SYSTEM,
      CCMC_VARIABLE_NAME_RHO_2_DATA_GRID_SYSTEM,
      CCMC_VARIABLE_NAME_RHO_3,
      CCMC_VARIABLE_NAME_RHO_3_DATA_TYPE,
      CCMC_VARIABLE_NAME_RHO_3_CLASSIFICATION,
      CCMC_VARIABLE_NAME_RHO_3_VALID_MIN,
      CCMC_VARIABLE_NAME_RHO_3_VALID_MAX,
      CCMC_VARIABLE_NAME_RHO_3_UNITS,
      CCMC_VARIABLE_NAME_RHO_3_GRID_SYSTEM,
      CCMC_VARIABLE_NAME_RHO_3_MASK,
      CCMC_VARIABLE_NAME_RHO_3_DESCRIPTION,
      CCMC_VARIABLE_NAME_RHO_3_IS_VECTOR,
      CCMC_VARIABLE_NAME_RHO_3_POSITION_GRID_SYSTEM,
      CCMC_VARIABLE_NAME_RHO_3_DATA_GRID_SYSTEM,
      CCMC_VARIABLE_NAME_RHO_4,
      CCMC_VARIABLE_NAME_RHO_4_DATA_TYPE,
      CCMC_VARIABLE_NAME_RHO_4_CLASSIFICATION,
      CCMC_VARIABLE_NAME_RHO_4_VALID_MIN,
      CCMC_VARIABLE_NAME_RHO_4_VALID_MAX,
      CCMC_VARIABLE_NAME_RHO_4_UNITS,
      CCMC_VARIABLE_NAME_RHO_4_GRID_SYSTEM,
      CCMC_VARIABLE_NAME_RHO_4_MASK,
      CCMC_VARIABLE_NAME_RHO_4_DESCRIPTION,
      CCMC_VARIABLE_NAME_RHO_4_IS_VECTOR,
      CCMC_VARIABLE_NAME_RHO_4_POSITION_GRID_SYSTEM,
      CCMC_VARIABLE_NAME_RHO_4_DATA_GRID_SYSTEM,
      CCMC_VARIABLE_NAME_T,
      CCMC_VARIABLE_NAME_T_DATA_TYPE,
      CCMC_VARIABLE_NAME_T_CLASSIFICATION,
      CCMC_VARIABLE_NAME_T_VALID_MIN,
      CCMC_VARIABLE_NAME_T_VALID_MAX,
      CCMC_VARIABLE_NAME_T_UNITS,
      CCMC_VARIABLE_NAME_T_GRID_SYSTEM,
      CCMC_VARIABLE_NAME_T_MASK,
      CCMC_VARIABLE_NAME_T_DESCRIPTION,
      CCMC_VARIABLE_NAME_T_IS_VECTOR,
      CCMC_VARIABLE_NAME_T_POSITION_GRID_SYSTEM,
      CCMC_VARIABLE_NAME_T_DATA_GRID_SYSTEM,
      CCMC_VARIABLE_NAME_T_1,
      CCMC_VARIABLE_NAME_T_1_DATA_TYPE,
      CCMC_VARIABLE_NAME_T_1_CLASSIFICATION,
      CCMC_VARIABLE_NAME_T_1_VALID_MIN,
      CCMC_VARIABLE_NAME_T_1_VALID_MAX,
      CCMC_VARIABLE_NAME_T_1_UNITS,
      CCMC_VARIABLE_NAME_T_1_GRID_SYSTEM,
      CCMC_VARIABLE_NAME_T_1_MASK,
      CCMC_VARIABLE_NAME_T_1_DESCRIPTION,
      CCMC_VARIABLE_NAME_T_1_IS_VECTOR,
      CCMC_VARIABLE_NAME_T_1_POSITION_GRID_SYSTEM,
      CCMC_VARIABLE_NAME_T_1_DATA_GRID_SYSTEM,
      CCMC_VARIABLE_NAME_T_2,
      CCMC_VARIABLE_NAME_T_2_DATA_TYPE,
      CCMC_VARIABLE_NAME_T_2_CLASSIFICATION,
      CCMC_VARIABLE_NAME_T_2_VALID_MIN,
      CCMC_VARIABLE_NAME_T_2_VALID_MAX,
      CCMC_VARIABLE_NAME_T_2_UNITS,
      CCMC_VARIABLE_NAME_T_2_GRID_SYSTEM,
      CCMC_VARIABLE_NAME_T_2_MASK,
      CCMC_VARIABLE_NAME_T_2_DESCRIPTION,
      CCMC_VARIABLE_NAME_T_2_IS_VECTOR,
      CCMC_VARIABLE_NAME_T_2_POSITION_GRID_SYSTEM,
      CCMC_VARIABLE_NAME_T_2_DATA_GRID_SYSTEM,
      CCMC_VARIABLE_NAME_T_3,
      CCMC_VARIABLE_NAME_T_3_DATA_TYPE,
      CCMC_VARIABLE_NAME_T_3_CLASSIFICATION,
      CCMC_VARIABLE_NAME_T_3_VALID_MIN,
      CCMC_VARIABLE_NAME_T_3_VALID_MAX,
      CCMC_VARIABLE_NAME_T_3_UNITS,
      CCMC_VARIABLE_NAME_T_3_GRID_SYSTEM,
      CCMC_VARIABLE_NAME_T_3_MASK,
      CCMC_VARIABLE_NAME_T_3_DESCRIPTION,
      CCMC_VARIABLE_NAME_T_3_IS_VECTOR,
      CCMC_VARIABLE_NAME_T_3_POSITION_GRID_SYSTEM,
      CCMC_VARIABLE_NAME_T_3_DATA_GRID_SYSTEM,
      CCMC_VARIABLE_NAME_T_4,
      CCMC_VARIABLE_NAME_T_4_DATA_TYPE,
      CCMC_VARIABLE_NAME_T_4_CLASSIFICATION,
      CCMC_VARIABLE_NAME_T_4_VALID_MIN,
      CCMC_VARIABLE_NAME_T_4_VALID_MAX,
      CCMC_VARIABLE_NAME_T_4_UNITS,
      CCMC_VARIABLE_NAME_T_4_GRID_SYSTEM,
      CCMC_VARIABLE_NAME_T_4_MASK,
      CCMC_VARIABLE_NAME_T_4_DESCRIPTION,
      CCMC_VARIABLE_NAME_T_4_IS_VECTOR,
      CCMC_VARIABLE_NAME_T_4_POSITION_GRID_SYSTEM,
      CCMC_VARIABLE_NAME_T_4_DATA_GRID_SYSTEM,
      CCMC_VARIABLE_NAME_UR,
      CCMC_VARIABLE_NAME_UR_DATA_TYPE,
      CCMC_VARIABLE_NAME_UR_CLASSIFICATION,
      CCMC_VARIABLE_NAME_UR_VALID_MIN,
      CCMC_VARIABLE_NAME_UR_VALID_MAX,
      CCMC_VARIABLE_NAME_UR_UNITS,
      CCMC_VARIABLE_NAME_UR_GRID_SYSTEM,
      CCMC_VARIABLE_NAME_UR_MASK,
      CCMC_VARIABLE_NAME_UR_DESCRIPTION,
      CCMC_VARIABLE_NAME_UR_IS_VECTOR,
      CCMC_VARIABLE_NAME_UR_POSITION_GRID_SYSTEM,
      CCMC_VARIABLE_NAME_UR_DATA_GRID_SYSTEM,
      CCMC_VARIABLE_NAME_UR_1,
      CCMC_VARIABLE_NAME_UR_1_DATA_TYPE,
      CCMC_VARIABLE_NAME_UR_1_CLASSIFICATION,
      CCMC_VARIABLE_NAME_UR_1_VALID_MIN,
      CCMC_VARIABLE_NAME_UR_1_VALID_MAX,
      CCMC_VARIABLE_NAME_UR_1_UNITS,
      CCMC_VARIABLE_NAME_UR_1_GRID_SYSTEM,
      CCMC_VARIABLE_NAME_UR_1_MASK,
      CCMC_VARIABLE_NAME_UR_1_DESCRIPTION,
      CCMC_VARIABLE_NAME_UR_1_IS_VECTOR,
      CCMC_VARIABLE_NAME_UR_1_POSITION_GRID_SYSTEM,
      CCMC_VARIABLE_NAME_UR_1_DATA_GRID_SYSTEM,
      CCMC_VARIABLE_NAME_UR_2,
      CCMC_VARIABLE_NAME_UR_2_DATA_TYPE,
      CCMC_VARIABLE_NAME_UR_2_CLASSIFICATION,
      CCMC_VARIABLE_NAME_UR_2_VALID_MIN,
      CCMC_VARIABLE_NAME_UR_2_VALID_MAX,
      CCMC_VARIABLE_NAME_UR_2_UNITS,
      CCMC_VARIABLE_NAME_UR_2_GRID_SYSTEM,
      CCMC_VARIABLE_NAME_UR_2_MASK,
      CCMC_VARIABLE_NAME_UR_2_DESCRIPTION,
      CCMC_VARIABLE_NAME_UR_2_IS_VECTOR,
      CCMC_VARIABLE_NAME_UR_2_POSITION_GRID_SYSTEM,
      CCMC_VARIABLE_NAME_UR_2_DATA_GRID_SYSTEM,
      CCMC_VARIABLE_NAME_UR_3,
      CCMC_VARIABLE_NAME_UR_3_DATA_TYPE,
      CCMC_VARIABLE_NAME_UR_3_CLASSIFICATION,
      CCMC_VARIABLE_NAME_UR_3_VALID_MIN,
      CCMC_VARIABLE_NAME_UR_3_VALID_MAX,
      CCMC_VARIABLE_NAME_UR_3_UNITS,
      CCMC_VARIABLE_NAME_UR_3_GRID_SYSTEM,
      CCMC_VARIABLE_NAME_UR_3_MASK,
      CCMC_VARIABLE_NAME_UR_3_DESCRIPTION,
      CCMC_VARIABLE_NAME_UR_3_IS_VECTOR,
      CCMC_VARIABLE_NAME_UR_3_POSITION_GRID_SYSTEM,
      CCMC_VARIABLE_NAME_UR_3_DATA_GRID_SYSTEM,
      CCMC_VARIABLE_NAME_UR_4,
      CCMC_VARIABLE_NAME_UR_4_DATA_TYPE,
      CCMC_VARIABLE_NAME_UR_4_CLASSIFICATION,
      CCMC_VARIABLE_NAME_UR_4_VALID_MIN,
      CCMC_VARIABLE_NAME_UR_4_VALID_MAX,
      CCMC_VARIABLE_NAME_UR_4_UNITS,
      CCMC_VARIABLE_NAME_UR_4_GRID_SYSTEM,
      CCMC_VARIABLE_NAME_UR_4_MASK,
      CCMC_VARIABLE_NAME_UR_4_DESCRIPTION,
      CCMC_VARIABLE_NAME_UR_4_IS_VECTOR,
      CCMC_VARIABLE_NAME_UR_4_POSITION_GRID_SYSTEM,
      CCMC_VARIABLE_NAME_UR_4_DATA_GRID_SYSTEM,
      CCMC_VARIABLE_NAME_UPHI,
      CCMC_VARIABLE_NAME_UPHI_DATA_TYPE,
      CCMC_VARIABLE_NAME_UPHI_CLASSIFICATION,
      CCMC_VARIABLE_NAME_UPHI_VALID_MIN,
      CCMC_VARIABLE_NAME_UPHI_VALID_MAX,
      CCMC_VARIABLE_NAME_UPHI_UNITS,
      CCMC_VARIABLE_NAME_UPHI_GRID_SYSTEM,
      CCMC_VARIABLE_NAME_UPHI_MASK,
      CCMC_VARIABLE_NAME_UPHI_DESCRIPTION,
      CCMC_VARIABLE_NAME_UPHI_IS_VECTOR,
      CCMC_VARIABLE_NAME_UPHI_POSITION_GRID_SYSTEM,
      CCMC_VARIABLE_NAME_UPHI_DATA_GRID_SYSTEM,
      CCMC_VARIABLE_NAME_UPHI_1,
      CCMC_VARIABLE_NAME_UPHI_1_DATA_TYPE,
      CCMC_VARIABLE_NAME_UPHI_1_CLASSIFICATION,
      CCMC_VARIABLE_NAME_UPHI_1_VALID_MIN,
      CCMC_VARIABLE_NAME_UPHI_1_VALID_MAX,
      CCMC_VARIABLE_NAME_UPHI_1_UNITS,
      CCMC_VARIABLE_NAME_UPHI_1_GRID_SYSTEM,
      CCMC_VARIABLE_NAME_UPHI_1_MASK,
      CCMC_VARIABLE_NAME_UPHI_1_DESCRIPTION,
      CCMC_VARIABLE_NAME_UPHI_1_IS_VECTOR,
      CCMC_VARIABLE_NAME_UPHI_1_POSITION_GRID_SYSTEM,
      CCMC_VARIABLE_NAME_UPHI_1_DATA_GRID_SYSTEM,
      CCMC_VARIABLE_NAME_UPHI_2,
      CCMC_VARIABLE_NAME_UPHI_2_DATA_TYPE,
      CCMC_VARIABLE_NAME_UPHI_2_CLASSIFICATION,
      CCMC_VARIABLE_NAME_UPHI_2_VALID_MIN,
      CCMC_VARIABLE_NAME_UPHI_2_VALID_MAX,
      CCMC_VARIABLE_NAME_UPHI_2_UNITS,
      CCMC_VARIABLE_NAME_UPHI_2_GRID_SYSTEM,
      CCMC_VARIABLE_NAME_UPHI_2_MASK,
      CCMC_VARIABLE_NAME_UPHI_2_DESCRIPTION,
      CCMC_VARIABLE_NAME_UPHI_2_IS_VECTOR,
      CCMC_VARIABLE_NAME_UPHI_2_POSITION_GRID_SYSTEM,
      CCMC_VARIABLE_NAME_UPHI_2_DATA_GRID_SYSTEM,
      CCMC_VARIABLE_NAME_UPHI_3,
      CCMC_VARIABLE_NAME_UPHI_3_DATA_TYPE,
      CCMC_VARIABLE_NAME_UPHI_3_CLASSIFICATION,
      CCMC_VARIABLE_NAME_UPHI_3_VALID_MIN,
      CCMC_VARIABLE_NAME_UPHI_3_VALID_MAX,
      CCMC_VARIABLE_NAME_UPHI_3_UNITS,
      CCMC_VARIABLE_NAME_UPHI_3_GRID_SYSTEM,
      CCMC_VARIABLE_NAME_UPHI_3_MASK,
      CCMC_VARIABLE_NAME_UPHI_3_DESCRIPTION,
      CCMC_VARIABLE_NAME_UPHI_3_IS_VECTOR,
      CCMC_VARIABLE_NAME_UPHI_3_POSITION_GRID_SYSTEM,
      CCMC_VARIABLE_NAME_UPHI_3_DATA_GRID_SYSTEM,
      CCMC_VARIABLE_NAME_UPHI_4,
      CCMC_VARIABLE_NAME_UPHI_4_DATA_TYPE,
      CCMC_VARIABLE_NAME_UPHI_4_CLASSIFICATION,
      CCMC_VARIABLE_NAME_UPHI_4_VALID_MIN,
      CCMC_VARIABLE_NAME_UPHI_4_VALID_MAX,
      CCMC_VARIABLE_NAME_UPHI_4_UNITS,
      CCMC_VARIABLE_NAME_UPHI_4_GRID_SYSTEM,
      CCMC_VARIABLE_NAME_UPHI_4_MASK,
      CCMC_VARIABLE_NAME_UPHI_4_DESCRIPTION,
      CCMC_VARIABLE_NAME_UPHI_4_IS_VECTOR,
      CCMC_VARIABLE_NAME_UPHI_4_POSITION_GRID_SYSTEM,
      CCMC_VARIABLE_NAME_UPHI_4_DATA_GRID_SYSTEM,
      CCMC_VARIABLE_NAME_UTHETA,
      CCMC_VARIABLE_NAME_UTHETA_DATA_TYPE,
      CCMC_VARIABLE_NAME_UTHETA_CLASSIFICATION,
      CCMC_VARIABLE_NAME_UTHETA_VALID_MIN,
      CCMC_VARIABLE_NAME_UTHETA_VALID_MAX,
      CCMC_VARIABLE_NAME_UTHETA_UNITS,
      CCMC_VARIABLE_NAME_UTHETA_GRID_SYSTEM,
      CCMC_VARIABLE_NAME_UTHETA_MASK,
      CCMC_VARIABLE_NAME_UTHETA_DESCRIPTION,
      CCMC_VARIABLE_NAME_UTHETA_IS_VECTOR,
      CCMC_VARIABLE_NAME_UTHETA_POSITION_GRID_SYSTEM,
      CCMC_VARIABLE_NAME_UTHETA_DATA_GRID_SYSTEM,
      CCMC_VARIABLE_NAME_UTHETA_1,
      CCMC_VARIABLE_NAME_UTHETA_1_DATA_TYPE,
      CCMC_VARIABLE_NAME_UTHETA_1_CLASSIFICATION,
      CCMC_VARIABLE_NAME_UTHETA_1_VALID_MIN,
      CCMC_VARIABLE_NAME_UTHETA_1_VALID_MAX,
      CCMC_VARIABLE_NAME_UTHETA_1_UNITS,
      CCMC_VARIABLE_NAME_UTHETA_1_GRID_SYSTEM,
      CCMC_VARIABLE_NAME_UTHETA_1_MASK,
      CCMC_VARIABLE_NAME_UTHETA_1_DESCRIPTION,
      CCMC_VARIABLE_NAME_UTHETA_1_IS_VECTOR,
      CCMC_VARIABLE_NAME_UTHETA_1_POSITION_GRID_SYSTEM,
      CCMC_VARIABLE_NAME_UTHETA_1_DATA_GRID_SYSTEM,
      CCMC_VARIABLE_NAME_UTHETA_2,
      CCMC_VARIABLE_NAME_UTHETA_2_DATA_TYPE,
      CCMC_VARIABLE_NAME_UTHETA_2_CLASSIFICATION,
      CCMC_VARIABLE_NAME_UTHETA_2_VALID_MIN,
      CCMC_VARIABLE_NAME_UTHETA_2_VALID_MAX,
      CCMC_VARIABLE_NAME_UTHETA_2_UNITS,
      CCMC_VARIABLE_NAME_UTHETA_2_GRID_SYSTEM,
      CCMC_VARIABLE_NAME_UTHETA_2_MASK,
      CCMC_VARIABLE_NAME_UTHETA_2_DESCRIPTION,
      CCMC_VARIABLE_NAME_UTHETA_2_IS_VECTOR,
      CCMC_VARIABLE_NAME_UTHETA_2_POSITION_GRID_SYSTEM,
      CCMC_VARIABLE_NAME_UTHETA_2_DATA_GRID_SYSTEM,
      CCMC_VARIABLE_NAME_UTHETA_3,
      CCMC_VARIABLE_NAME_UTHETA_3_DATA_TYPE,
      CCMC_VARIABLE_NAME_UTHETA_3_CLASSIFICATION,
      CCMC_VARIABLE_NAME_UTHETA_3_VALID_MIN,
      CCMC_VARIABLE_NAME_UTHETA_3_VALID_MAX,
      CCMC_VARIABLE_NAME_UTHETA_3_UNITS,
      CCMC_VARIABLE_NAME_UTHETA_3_GRID_SYSTEM,
      CCMC_VARIABLE_NAME_UTHETA_3_MASK,
      CCMC_VARIABLE_NAME_UTHETA_3_DESCRIPTION,
      CCMC_VARIABLE_NAME_UTHETA_3_IS_VECTOR,
      CCMC_VARIABLE_NAME_UTHETA_3_POSITION_GRID_SYSTEM,
      CCMC_VARIABLE_NAME_UTHETA_3_DATA_GRID_SYSTEM,
      CCMC_VARIABLE_NAME_UTHETA_4,
      CCMC_VARIABLE_NAME_UTHETA_4_DATA_TYPE,
      CCMC_VARIABLE_NAME_UTHETA_4_CLASSIFICATION,
      CCMC_VARIABLE_NAME_UTHETA_4_VALID_MIN,
      CCMC_VARIABLE_NAME_UTHETA_4_VALID_MAX,
      CCMC_VARIABLE_NAME_UTHETA_4_UNITS,
      CCMC_VARIABLE_NAME_UTHETA_4_GRID_SYSTEM,
      CCMC_VARIABLE_NAME_UTHETA_4_MASK,
      CCMC_VARIABLE_NAME_UTHETA_4_DESCRIPTION,
      CCMC_VARIABLE_NAME_UTHETA_4_IS_VECTOR,
      CCMC_VARIABLE_NAME_UTHETA_4_POSITION_GRID_SYSTEM,
      CCMC_VARIABLE_NAME_UTHETA_4_DATA_GRID_SYSTEM,
      CCMC_VARIABLE_NAME_BR,
      CCMC_VARIABLE_NAME_BR_DATA_TYPE,
      CCMC_VARIABLE_NAME_BR_CLASSIFICATION,
      CCMC_VARIABLE_NAME_BR_VALID_MIN,
      CCMC_VARIABLE_NAME_BR_VALID_MAX,
      CCMC_VARIABLE_NAME_BR_UNITS,
      CCMC_VARIABLE_NAME_BR_GRID_SYSTEM,
      CCMC_VARIABLE_NAME_BR_MASK,
      CCMC_VARIABLE_NAME_BR_DESCRIPTION,
      CCMC_VARIABLE_NAME_BR_IS_VECTOR,
      CCMC_VARIABLE_NAME_BR_POSITION_GRID_SYSTEM,
      CCMC_VARIABLE_NAME_BR_DATA_GRID_SYSTEM,
      CCMC_VARIABLE_NAME_BR_1,
      CCMC_VARIABLE_NAME_BR_1_DATA_TYPE,
      CCMC_VARIABLE_NAME_BR_1_CLASSIFICATION,
      CCMC_VARIABLE_NAME_BR_1_VALID_MIN,
      CCMC_VARIABLE_NAME_BR_1_VALID_MAX,
      CCMC_VARIABLE_NAME_BR_1_UNITS,
      CCMC_VARIABLE_NAME_BR_1_GRID_SYSTEM,
      CCMC_VARIABLE_NAME_BR_1_MASK,
      CCMC_VARIABLE_NAME_BR_1_DESCRIPTION,
      CCMC_VARIABLE_NAME_BR_1_IS_VECTOR,
      CCMC_VARIABLE_NAME_BR_1_POSITION_GRID_SYSTEM,
      CCMC_VARIABLE_NAME_BR_1_DATA_GRID_SYSTEM,
      CCMC_VARIABLE_NAME_BR_2,
      CCMC_VARIABLE_NAME_BR_2_DATA_TYPE,
      CCMC_VARIABLE_NAME_BR_2_CLASSIFICATION,
      CCMC_VARIABLE_NAME_BR_2_VALID_MIN,
      CCMC_VARIABLE_NAME_BR_2_VALID_MAX,
      CCMC_VARIABLE_NAME_BR_2_UNITS,
      CCMC_VARIABLE_NAME_BR_2_GRID_SYSTEM,
      CCMC_VARIABLE_NAME_BR_2_MASK,
      CCMC_VARIABLE_NAME_BR_2_DESCRIPTION,
      CCMC_VARIABLE_NAME_BR_2_IS_VECTOR,
      CCMC_VARIABLE_NAME_BR_2_POSITION_GRID_SYSTEM,
      CCMC_VARIABLE_NAME_BR_2_DATA_GRID_SYSTEM,
      CCMC_VARIABLE_NAME_BR_3,
      CCMC_VARIABLE_NAME_BR_3_DATA_TYPE,
      CCMC_VARIABLE_NAME_BR_3_CLASSIFICATION,
      CCMC_VARIABLE_NAME_BR_3_VALID_MIN,
      CCMC_VARIABLE_NAME_BR_3_VALID_MAX,
      CCMC_VARIABLE_NAME_BR_3_UNITS,
      CCMC_VARIABLE_NAME_BR_3_GRID_SYSTEM,
      CCMC_VARIABLE_NAME_BR_3_MASK,
      CCMC_VARIABLE_NAME_BR_3_DESCRIPTION,
      CCMC_VARIABLE_NAME_BR_3_IS_VECTOR,
      CCMC_VARIABLE_NAME_BR_3_POSITION_GRID_SYSTEM,
      CCMC_VARIABLE_NAME_BR_3_DATA_GRID_SYSTEM,
      CCMC_VARIABLE_NAME_BR_4,
      CCMC_VARIABLE_NAME_BR_4_DATA_TYPE,
      CCMC_VARIABLE_NAME_BR_4_CLASSIFICATION,
      CCMC_VARIABLE_NAME_BR_4_VALID_MIN,
      CCMC_VARIABLE_NAME_BR_4_VALID_MAX,
      CCMC_VARIABLE_NAME_BR_4_UNITS,
      CCMC_VARIABLE_NAME_BR_4_GRID_SYSTEM,
      CCMC_VARIABLE_NAME_BR_4_MASK,
      CCMC_VARIABLE_NAME_BR_4_DESCRIPTION,
      CCMC_VARIABLE_NAME_BR_4_IS_VECTOR,
      CCMC_VARIABLE_NAME_BR_4_POSITION_GRID_SYSTEM,
      CCMC_VARIABLE_NAME_BR_4_DATA_GRID_SYSTEM,
      CCMC_VARIABLE_NAME_BPHI,
      CCMC_VARIABLE_NAME_BPHI_DATA_TYPE,
      CCMC_VARIABLE_NAME_BPHI_CLASSIFICATION,
      CCMC_VARIABLE_NAME_BPHI_VALID_MIN,
      CCMC_VARIABLE_NAME_BPHI_VALID_MAX,
      CCMC_VARIABLE_NAME_BPHI_UNITS,
      CCMC_VARIABLE_NAME_BPHI_GRID_SYSTEM,
      CCMC_VARIABLE_NAME_BPHI_MASK,
      CCMC_VARIABLE_NAME_BPHI_DESCRIPTION,
      CCMC_VARIABLE_NAME_BPHI_IS_VECTOR,
      CCMC_VARIABLE_NAME_BPHI_POSITION_GRID_SYSTEM,
      CCMC_VARIABLE_NAME_BPHI_DATA_GRID_SYSTEM,
      CCMC_VARIABLE_NAME_BPHI_1,
      CCMC_VARIABLE_NAME_BPHI_1_DATA_TYPE,
      CCMC_VARIABLE_NAME_BPHI_1_CLASSIFICATION,
      CCMC_VARIABLE_NAME_BPHI_1_VALID_MIN,
      CCMC_VARIABLE_NAME_BPHI_1_VALID_MAX,
      CCMC_VARIABLE_NAME_BPHI_1_UNITS,
      CCMC_VARIABLE_NAME_BPHI_1_GRID_SYSTEM,
      CCMC_VARIABLE_NAME_BPHI_1_MASK,
      CCMC_VARIABLE_NAME_BPHI_1_DESCRIPTION,
      CCMC_VARIABLE_NAME_BPHI_1_IS_VECTOR,
      CCMC_VARIABLE_NAME_BPHI_1_POSITION_GRID_SYSTEM,
      CCMC_VARIABLE_NAME_BPHI_1_DATA_GRID_SYSTEM,
      CCMC_VARIABLE_NAME_BPHI_2,
      CCMC_VARIABLE_NAME_BPHI_2_DATA_TYPE,
      CCMC_VARIABLE_NAME_BPHI_2_CLASSIFICATION,
      CCMC_VARIABLE_NAME_BPHI_2_VALID_MIN,
      CCMC_VARIABLE_NAME_BPHI_2_VALID_MAX,
      CCMC_VARIABLE_NAME_BPHI_2_UNITS,
      CCMC_VARIABLE_NAME_BPHI_2_GRID_SYSTEM,
      CCMC_VARIABLE_NAME_BPHI_2_MASK,
      CCMC_VARIABLE_NAME_BPHI_2_DESCRIPTION,
      CCMC_VARIABLE_NAME_BPHI_2_IS_VECTOR,
      CCMC_VARIABLE_NAME_BPHI_2_POSITION_GRID_SYSTEM,
      CCMC_VARIABLE_NAME_BPHI_2_DATA_GRID_SYSTEM,
      CCMC_VARIABLE_NAME_BPHI_3,
      CCMC_VARIABLE_NAME_BPHI_3_DATA_TYPE,
      CCMC_VARIABLE_NAME_BPHI_3_CLASSIFICATION,
      CCMC_VARIABLE_NAME_BPHI_3_VALID_MIN,
      CCMC_VARIABLE_NAME_BPHI_3_VALID_MAX,
      CCMC_VARIABLE_NAME_BPHI_3_UNITS,
      CCMC_VARIABLE_NAME_BPHI_3_GRID_SYSTEM,
      CCMC_VARIABLE_NAME_BPHI_3_MASK,
      CCMC_VARIABLE_NAME_BPHI_3_DESCRIPTION,
      CCMC_VARIABLE_NAME_BPHI_3_IS_VECTOR,
      CCMC_VARIABLE_NAME_BPHI_3_POSITION_GRID_SYSTEM,
      CCMC_VARIABLE_NAME_BPHI_3_DATA_GRID_SYSTEM,
      CCMC_VARIABLE_NAME_BPHI_4,
      CCMC_VARIABLE_NAME_BPHI_4_DATA_TYPE,
      CCMC_VARIABLE_NAME_BPHI_4_CLASSIFICATION,
      CCMC_VARIABLE_NAME_BPHI_4_VALID_MIN,
      CCMC_VARIABLE_NAME_BPHI_4_VALID_MAX,
      CCMC_VARIABLE_NAME_BPHI_4_UNITS,
      CCMC_VARIABLE_NAME_BPHI_4_GRID_SYSTEM,
      CCMC_VARIABLE_NAME_BPHI_4_MASK,
      CCMC_VARIABLE_NAME_BPHI_4_DESCRIPTION,
      CCMC_VARIABLE_NAME_BPHI_4_IS_VECTOR,
      CCMC_VARIABLE_NAME_BPHI_4_POSITION_GRID_SYSTEM,
      CCMC_VARIABLE_NAME_BPHI_4_DATA_GRID_SYSTEM,
      CCMC_VARIABLE_NAME_BTHETA,
      CCMC_VARIABLE_NAME_BTHETA_DATA_TYPE,
      CCMC_VARIABLE_NAME_BTHETA_CLASSIFICATION,
      CCMC_VARIABLE_NAME_BTHETA_VALID_MIN,
      CCMC_VARIABLE_NAME_BTHETA_VALID_MAX,
      CCMC_VARIABLE_NAME_BTHETA_UNITS,
      CCMC_VARIABLE_NAME_BTHETA_GRID_SYSTEM,
      CCMC_VARIABLE_NAME_BTHETA_MASK,
      CCMC_VARIABLE_NAME_BTHETA_DESCRIPTION,
      CCMC_VARIABLE_NAME_BTHETA_IS_VECTOR,
      CCMC_VARIABLE_NAME_BTHETA_POSITION_GRID_SYSTEM,
      CCMC_VARIABLE_NAME_BTHETA_DATA_GRID_SYSTEM,
      CCMC_VARIABLE_NAME_BTHETA_1,
      CCMC_VARIABLE_NAME_BTHETA_1_DATA_TYPE,
      CCMC_VARIABLE_NAME_BTHETA_1_CLASSIFICATION,
      CCMC_VARIABLE_NAME_BTHETA_1_VALID_MIN,
      CCMC_VARIABLE_NAME_BTHETA_1_VALID_MAX,
      CCMC_VARIABLE_NAME_BTHETA_1_UNITS,
      CCMC_VARIABLE_NAME_BTHETA_1_GRID_SYSTEM,
      CCMC_VARIABLE_NAME_BTHETA_1_MASK,
      CCMC_VARIABLE_NAME_BTHETA_1_DESCRIPTION,
      CCMC_VARIABLE_NAME_BTHETA_1_IS_VECTOR,
      CCMC_VARIABLE_NAME_BTHETA_1_POSITION_GRID_SYSTEM,
      CCMC_VARIABLE_NAME_BTHETA_1_DATA_GRID_SYSTEM,
      CCMC_VARIABLE_NAME_BTHETA_2,
      CCMC_VARIABLE_NAME_BTHETA_2_DATA_TYPE,
      CCMC_VARIABLE_NAME_BTHETA_2_CLASSIFICATION,
      CCMC_VARIABLE_NAME_BTHETA_2_VALID_MIN,
      CCMC_VARIABLE_NAME_BTHETA_2_VALID_MAX,
      CCMC_VARIABLE_NAME_BTHETA_2_UNITS,
      CCMC_VARIABLE_NAME_BTHETA_2_GRID_SYSTEM,
      CCMC_VARIABLE_NAME_BTHETA_2_MASK,
      CCMC_VARIABLE_NAME_BTHETA_2_DESCRIPTION,
      CCMC_VARIABLE_NAME_BTHETA_2_IS_VECTOR,
      CCMC_VARIABLE_NAME_BTHETA_2_POSITION_GRID_SYSTEM,
      CCMC_VARIABLE_NAME_BTHETA_2_DATA_GRID_SYSTEM,
      CCMC_VARIABLE_NAME_BTHETA_3,
      CCMC_VARIABLE_NAME_BTHETA_3_DATA_TYPE,
      CCMC_VARIABLE_NAME_BTHETA_3_CLASSIFICATION,
      CCMC_VARIABLE_NAME_BTHETA_3_VALID_MIN,
      CCMC_VARIABLE_NAME_BTHETA_3_VALID_MAX,
      CCMC_VARIABLE_NAME_BTHETA_3_UNITS,
      CCMC_VARIABLE_NAME_BTHETA_3_GRID_SYSTEM,
      CCMC_VARIABLE_NAME_BTHETA_3_MASK,
      CCMC_VARIABLE_NAME_BTHETA_3_DESCRIPTION,
      CCMC_VARIABLE_NAME_BTHETA_3_IS_VECTOR,
      CCMC_VARIABLE_NAME_BTHETA_3_POSITION_GRID_SYSTEM,
      CCMC_VARIABLE_NAME_BTHETA_3_DATA_GRID_SYSTEM,
      CCMC_VARIABLE_NAME_BTHETA_4,
      CCMC_VARIABLE_NAME_BTHETA_4_DATA_TYPE,
      CCMC_VARIABLE_NAME_BTHETA_4_CLASSIFICATION,
      CCMC_VARIABLE_NAME_BTHETA_4_VALID_MIN,
      CCMC_VARIABLE_NAME_BTHETA_4_VALID_MAX,
      CCMC_VARIABLE_NAME_BTHETA_4_UNITS,
      CCMC_VARIABLE_NAME_BTHETA_4_GRID_SYSTEM,
      CCMC_VARIABLE_NAME_BTHETA_4_MASK,
      CCMC_VARIABLE_NAME_BTHETA_4_DESCRIPTION,
      CCMC_VARIABLE_NAME_BTHETA_4_IS_VECTOR,
      CCMC_VARIABLE_NAME_BTHETA_4_POSITION_GRID_SYSTEM,
      CCMC_VARIABLE_NAME_BTHETA_4_DATA_GRID_SYSTEM,
      CCMC_VARIABLE_NAME_DP,
      CCMC_VARIABLE_NAME_DP_DATA_TYPE,
      CCMC_VARIABLE_NAME_DP_CLASSIFICATION,
      CCMC_VARIABLE_NAME_DP_VALID_MIN,
      CCMC_VARIABLE_NAME_DP_VALID_MAX,
      CCMC_VARIABLE_NAME_DP_UNITS,
      CCMC_VARIABLE_NAME_DP_GRID_SYSTEM,
      CCMC_VARIABLE_NAME_DP_MASK,
      CCMC_VARIABLE_NAME_DP_DESCRIPTION,
      CCMC_VARIABLE_NAME_DP_IS_VECTOR,
      CCMC_VARIABLE_NAME_DP_POSITION_GRID_SYSTEM,
      CCMC_VARIABLE_NAME_DP_DATA_GRID_SYSTEM,
      CCMC_VARIABLE_NAME_DP_1,
      CCMC_VARIABLE_NAME_DP_1_DATA_TYPE,
      CCMC_VARIABLE_NAME_DP_1_CLASSIFICATION,
      CCMC_VARIABLE_NAME_DP_1_VALID_MIN,
      CCMC_VARIABLE_NAME_DP_1_VALID_MAX,
      CCMC_VARIABLE_NAME_DP_1_UNITS,
      CCMC_VARIABLE_NAME_DP_1_GRID_SYSTEM,
      CCMC_VARIABLE_NAME_DP_1_MASK,
      CCMC_VARIABLE_NAME_DP_1_DESCRIPTION,
      CCMC_VARIABLE_NAME_DP_1_IS_VECTOR,
      CCMC_VARIABLE_NAME_DP_1_POSITION_GRID_SYSTEM,
      CCMC_VARIABLE_NAME_DP_1_DATA_GRID_SYSTEM,
      CCMC_VARIABLE_NAME_DP_2,
      CCMC_VARIABLE_NAME_DP_2_DATA_TYPE,
      CCMC_VARIABLE_NAME_DP_2_CLASSIFICATION,
      CCMC_VARIABLE_NAME_DP_2_VALID_MIN,
      CCMC_VARIABLE_NAME_DP_2_VALID_MAX,
      CCMC_VARIABLE_NAME_DP_2_UNITS,
      CCMC_VARIABLE_NAME_DP_2_GRID_SYSTEM,
      CCMC_VARIABLE_NAME_DP_2_MASK,
      CCMC_VARIABLE_NAME_DP_2_DESCRIPTION,
      CCMC_VARIABLE_NAME_DP_2_IS_VECTOR,
      CCMC_VARIABLE_NAME_DP_2_POSITION_GRID_SYSTEM,
      CCMC_VARIABLE_NAME_DP_2_DATA_GRID_SYSTEM,
      CCMC_VARIABLE_NAME_DP_3,
      CCMC_VARIABLE_NAME_DP_3_DATA_TYPE,
      CCMC_VARIABLE_NAME_DP_3_CLASSIFICATION,
      CCMC_VARIABLE_NAME_DP_3_VALID_MIN,
      CCMC_VARIABLE_NAME_DP_3_VALID_MAX,
      CCMC_VARIABLE_NAME_DP_3_UNITS,
      CCMC_VARIABLE_NAME_DP_3_GRID_SYSTEM,
      CCMC_VARIABLE_NAME_DP_3_MASK,
      CCMC_VARIABLE_NAME_DP_3_DESCRIPTION,
      CCMC_VARIABLE_NAME_DP_3_IS_VECTOR,
      CCMC_VARIABLE_NAME_DP_3_POSITION_GRID_SYSTEM,
      CCMC_VARIABLE_NAME_DP_3_DATA_GRID_SYSTEM,
      CCMC_VARIABLE_NAME_DP_4,
      CCMC_VARIABLE_NAME_DP_4_DATA_TYPE,
      CCMC_VARIABLE_NAME_DP_4_CLASSIFICATION,
      CCMC_VARIABLE_NAME_DP_4_VALID_MIN,
      CCMC_VARIABLE_NAME_DP_4_VALID_MAX,
      CCMC_VARIABLE_NAME_DP_4_UNITS,
      CCMC_VARIABLE_NAME_DP_4_GRID_SYSTEM,
      CCMC_VARIABLE_NAME_DP_4_MASK,
      CCMC_VARIABLE_NAME_DP_4_DESCRIPTION,
      CCMC_VARIABLE_NAME_DP_4_IS_VECTOR,
      CCMC_VARIABLE_NAME_DP_4_POSITION_GRID_SYSTEM,
      CCMC_VARIABLE_NAME_DP_4_DATA_GRID_SYSTEM,
      CCMC_VARIABLE_NAME_BP,
      CCMC_VARIABLE_NAME_BP_DATA_TYPE,
      CCMC_VARIABLE_NAME_BP_CLASSIFICATION,
      CCMC_VARIABLE_NAME_BP_VALID_MIN,
      CCMC_VARIABLE_NAME_BP_VALID_MAX,
      CCMC_VARIABLE_NAME_BP_UNITS,
      CCMC_VARIABLE_NAME_BP_GRID_SYSTEM,
      CCMC_VARIABLE_NAME_BP_MASK,
      CCMC_VARIABLE_NAME_BP_DESCRIPTION,
      CCMC_VARIABLE_NAME_BP_IS_VECTOR,
      CCMC_VARIABLE_NAME_BP_POSITION_GRID_SYSTEM,
      CCMC_VARIABLE_NAME_BP_DATA_GRID_SYSTEM,
      CCMC_VARIABLE_NAME_BP_1,
      CCMC_VARIABLE_NAME_BP_1_DATA_TYPE,
      CCMC_VARIABLE_NAME_BP_1_CLASSIFICATION,
      CCMC_VARIABLE_NAME_BP_1_VALID_MIN,
      CCMC_VARIABLE_NAME_BP_1_VALID_MAX,
      CCMC_VARIABLE_NAME_BP_1_UNITS,
      CCMC_VARIABLE_NAME_BP_1_GRID_SYSTEM,
      CCMC_VARIABLE_NAME_BP_1_MASK,
      CCMC_VARIABLE_NAME_BP_1_DESCRIPTION,
      CCMC_VARIABLE_NAME_BP_1_IS_VECTOR,
      CCMC_VARIABLE_NAME_BP_1_POSITION_GRID_SYSTEM,
      CCMC_VARIABLE_NAME_BP_1_DATA_GRID_SYSTEM,
      CCMC_VARIABLE_NAME_BP_2,
      CCMC_VARIABLE_NAME_BP_2_DATA_TYPE,
      CCMC_VARIABLE_NAME_BP_2_CLASSIFICATION,
      CCMC_VARIABLE_NAME_BP_2_VALID_MIN,
      CCMC_VARIABLE_NAME_BP_2_VALID_MAX,
      CCMC_VARIABLE_NAME_BP_2_UNITS,
      CCMC_VARIABLE_NAME_BP_2_GRID_SYSTEM,
      CCMC_VARIABLE_NAME_BP_2_MASK,
      CCMC_VARIABLE_NAME_BP_2_DESCRIPTION,
      CCMC_VARIABLE_NAME_BP_2_IS_VECTOR,
      CCMC_VARIABLE_NAME_BP_2_POSITION_GRID_SYSTEM,
      CCMC_VARIABLE_NAME_BP_2_DATA_GRID_SYSTEM,
      CCMC_VARIABLE_NAME_BP_3,
      CCMC_VARIABLE_NAME_BP_3_DATA_TYPE,
      CCMC_VARIABLE_NAME_BP_3_CLASSIFICATION,
      CCMC_VARIABLE_NAME_BP_3_VALID_MIN,
      CCMC_VARIABLE_NAME_BP_3_VALID_MAX,
      CCMC_VARIABLE_NAME_BP_3_UNITS,
      CCMC_VARIABLE_NAME_BP_3_GRID_SYSTEM,
      CCMC_VARIABLE_NAME_BP_3_MASK,
      CCMC_VARIABLE_NAME_BP_3_DESCRIPTION,
      CCMC_VARIABLE_NAME_BP_3_IS_VECTOR,
      CCMC_VARIABLE_NAME_BP_3_POSITION_GRID_SYSTEM,
      CCMC_VARIABLE_NAME_BP_3_DATA_GRID_SYSTEM,
      CCMC_VARIABLE_NAME_BP_4,
      CCMC_VARIABLE_NAME_BP_4_DATA_TYPE,
      CCMC_VARIABLE_NAME_BP_4_CLASSIFICATION,
      CCMC_VARIABLE_NAME_BP_4_VALID_MIN,
      CCMC_VARIABLE_NAME_BP_4_VALID_MAX,
      CCMC_VARIABLE_NAME_BP_4_UNITS,
      CCMC_VARIABLE_NAME_BP_4_GRID_SYSTEM,
      CCMC_VARIABLE_NAME_BP_4_MASK,
      CCMC_VARIABLE_NAME_BP_4_DESCRIPTION,
      CCMC_VARIABLE_NAME_BP_4_IS_VECTOR,
      CCMC_VARIABLE_NAME_BP_4_POSITION_GRID_SYSTEM,
      CCMC_VARIABLE_NAME_BP_4_DATA_GRID_SYSTEM,
      CCMC_VARIABLE_NAME_B1R,
      CCMC_VARIABLE_NAME_B1R_DATA_TYPE,
      CCMC_VARIABLE_NAME_B1R_CLASSIFICATION,
      CCMC_VARIABLE_NAME_B1R_VALID_MIN,
      CCMC_VARIABLE_NAME_B1R_VALID_MAX,
      CCMC_VARIABLE_NAME_B1R_UNITS,
      CCMC_VARIABLE_NAME_B1R_GRID_SYSTEM,
      CCMC_VARIABLE_NAME_B1R_MASK,
      CCMC_VARIABLE_NAME_B1R_DESCRIPTION,
      CCMC_VARIABLE_NAME_B1R_IS_VECTOR,
      CCMC_VARIABLE_NAME_B1R_POSITION_GRID_SYSTEM,
      CCMC_VARIABLE_NAME_B1R_DATA_GRID_SYSTEM,
      CCMC_VARIABLE_NAME_B1PHI,
      CCMC_VARIABLE_NAME_B1PHI_DATA_TYPE,
      CCMC_VARIABLE_NAME_B1PHI_CLASSIFICATION,
      CCMC_VARIABLE_NAME_B1PHI_VALID_MIN,
      CCMC_VARIABLE_NAME_B1PHI_VALID_MAX,
      CCMC_VARIABLE_NAME_B1PHI_UNITS,
      CCMC_VARIABLE_NAME_B1PHI_GRID_SYSTEM,
      CCMC_VARIABLE_NAME_B1PHI_MASK,
      CCMC_VARIABLE_NAME_B1PHI_DESCRIPTION,
      CCMC_VARIABLE_NAME_B1PHI_IS_VECTOR,
      CCMC_VARIABLE_NAME_B1PHI_POSITION_GRID_SYSTEM,
      CCMC_VARIABLE_NAME_B1PHI_DATA_GRID_SYSTEM,
      CCMC_VARIABLE_NAME_B1THETA,
      CCMC_VARIABLE_NAME_B1THETA_DATA_TYPE,
      CCMC_VARIABLE_NAME_B1THETA_CLASSIFICATION,
      CCMC_VARIABLE_NAME_B1THETA_VALID_MIN,
      CCMC_VARIABLE_NAME_B1THETA_VALID_MAX,
      CCMC_VARIABLE_NAME_B1THETA_UNITS,
      CCMC_VARIABLE_NAME_B1THETA_GRID_SYSTEM,
      CCMC_VARIABLE_NAME_B1THETA_MASK,
      CCMC_VARIABLE_NAME_B1THETA_DESCRIPTION,
      CCMC_VARIABLE_NAME_B1THETA_IS_VECTOR,
      CCMC_VARIABLE_NAME_B1THETA_POSITION_GRID_SYSTEM,
      CCMC_VARIABLE_NAME_B1THETA_DATA_GRID_SYSTEM,
      CCMC_VARIABLE_NAME_TIME_1,
      CCMC_VARIABLE_NAME_TIME_1_DATA_TYPE,
      CCMC_VARIABLE_NAME_TIME_1_CLASSIFICATION,
      CCMC_VARIABLE_NAME_TIME_1_VALID_MIN,
      CCMC_VARIABLE_NAME_TIME_1_VALID_MAX,
      CCMC_VARIABLE_NAME_TIME_1_UNITS,
      CCMC_VARIABLE_NAME_TIME_1_GRID_SYSTEM,
      CCMC_VARIABLE_NAME_TIME_1_MASK,
      CCMC_VARIABLE_NAME_TIME_1_DESCRIPTION,
      CCMC_VARIABLE_NAME_TIME_1_IS_VECTOR,
      CCMC_VARIABLE_NAME_TIME_1_POSITION_GRID_SYSTEM,
      CCMC_VARIABLE_NAME_TIME_1_DATA_GRID_SYSTEM,
      CCMC_VARIABLE_NAME_TIME_2,
      CCMC_VARIABLE_NAME_TIME_2_DATA_TYPE,
      CCMC_VARIABLE_NAME_TIME_2_CLASSIFICATION,
      CCMC_VARIABLE_NAME_TIME_2_VALID_MIN,
      CCMC_VARIABLE_NAME_TIME_2_VALID_MAX,
      CCMC_VARIABLE_NAME_TIME_2_UNITS,
      CCMC_VARIABLE_NAME_TIME_2_GRID_SYSTEM,
      CCMC_VARIABLE_NAME_TIME_2_MASK,
      CCMC_VARIABLE_NAME_TIME_2_DESCRIPTION,
      CCMC_VARIABLE_NAME_TIME_2_IS_VECTOR,
      CCMC_VARIABLE_NAME_TIME_2_POSITION_GRID_SYSTEM,
      CCMC_VARIABLE_NAME_TIME_2_DATA_GRID_SYSTEM,
      CCMC_VARIABLE_NAME_TIME_3,
      CCMC_VARIABLE_NAME_TIME_3_DATA_TYPE,
      CCMC_VARIABLE_NAME_TIME_3_CLASSIFICATION,
      CCMC_VARIABLE_NAME_TIME_3_VALID_MIN,
      CCMC_VARIABLE_NAME_TIME_3_VALID_MAX,
      CCMC_VARIABLE_NAME_TIME_3_UNITS,
      CCMC_VARIABLE_NAME_TIME_3_GRID_SYSTEM,
      CCMC_VARIABLE_NAME_TIME_3_MASK,
      CCMC_VARIABLE_NAME_TIME_3_DESCRIPTION,
      CCMC_VARIABLE_NAME_TIME_3_IS_VECTOR,
      CCMC_VARIABLE_NAME_TIME_3_POSITION_GRID_SYSTEM,
      CCMC_VARIABLE_NAME_TIME_3_DATA_GRID_SYSTEM,
      CCMC_VARIABLE_NAME_TIME_4,
      CCMC_VARIABLE_NAME_TIME_4_DATA_TYPE,
      CCMC_VARIABLE_NAME_TIME_4_CLASSIFICATION,
      CCMC_VARIABLE_NAME_TIME_4_VALID_MIN,
      CCMC_VARIABLE_NAME_TIME_4_VALID_MAX,
      CCMC_VARIABLE_NAME_TIME_4_UNITS,
      CCMC_VARIABLE_NAME_TIME_4_GRID_SYSTEM,
      CCMC_VARIABLE_NAME_TIME_4_MASK,
      CCMC_VARIABLE_NAME_TIME_4_DESCRIPTION,
      CCMC_VARIABLE_NAME_TIME_4_IS_VECTOR,
      CCMC_VARIABLE_NAME_TIME_4_POSITION_GRID_SYSTEM,
      CCMC_VARIABLE_NAME_TIME_4_DATA_GRID_SYSTEM,
      CCMC_VARIABLE_NAME_TIME_STEP_1,
      CCMC_VARIABLE_NAME_TIME_STEP_1_DATA_TYPE,
      CCMC_VARIABLE_NAME_TIME_STEP_1_CLASSIFICATION,
      CCMC_VARIABLE_NAME_TIME_STEP_1_VALID_MIN,
      CCMC_VARIABLE_NAME_TIME_STEP_1_VALID_MAX,
      CCMC_VARIABLE_NAME_TIME_STEP_1_UNITS,
      CCMC_VARIABLE_NAME_TIME_STEP_1_GRID_SYSTEM,
      CCMC_VARIABLE_NAME_TIME_STEP_1_MASK,
      CCMC_VARIABLE_NAME_TIME_STEP_1_DESCRIPTION,
      CCMC_VARIABLE_NAME_TIME_STEP_1_IS_VECTOR,
      CCMC_VARIABLE_NAME_TIME_STEP_1_POSITION_GRID_SYSTEM,
      CCMC_VARIABLE_NAME_TIME_STEP_1_DATA_GRID_SYSTEM,
      CCMC_VARIABLE_NAME_TIME_STEP_2,
      CCMC_VARIABLE_NAME_TIME_STEP_2_DATA_TYPE,
      CCMC_VARIABLE_NAME_TIME_STEP_2_CLASSIFICATION,
      CCMC_VARIABLE_NAME_TIME_STEP_2_VALID_MIN,
      CCMC_VARIABLE_NAME_TIME_STEP_2_VALID_MAX,
      CCMC_VARIABLE_NAME_TIME_STEP_2_UNITS,
      CCMC_VARIABLE_NAME_TIME_STEP_2_GRID_SYSTEM,
      CCMC_VARIABLE_NAME_TIME_STEP_2_MASK,
      CCMC_VARIABLE_NAME_TIME_STEP_2_DESCRIPTION,
      CCMC_VARIABLE_NAME_TIME_STEP_2_IS_VECTOR,
      CCMC_VARIABLE_NAME_TIME_STEP_2_POSITION_GRID_SYSTEM,
      CCMC_VARIABLE_NAME_TIME_STEP_2_DATA_GRID_SYSTEM,
      CCMC_VARIABLE_NAME_TIME_STEP_3,
      CCMC_VARIABLE_NAME_TIME_STEP_3_DATA_TYPE,
      CCMC_VARIABLE_NAME_TIME_STEP_3_CLASSIFICATION,
      CCMC_VARIABLE_NAME_TIME_STEP_3_VALID_MIN,
      CCMC_VARIABLE_NAME_TIME_STEP_3_VALID_MAX,
      CCMC_VARIABLE_NAME_TIME_STEP_3_UNITS,
      CCMC_VARIABLE_NAME_TIME_STEP_3_GRID_SYSTEM,
      CCMC_VARIABLE_NAME_TIME_STEP_3_MASK,
      CCMC_VARIABLE_NAME_TIME_STEP_3_DESCRIPTION,
      CCMC_VARIABLE_NAME_TIME_STEP_3_IS_VECTOR,
      CCMC_VARIABLE_NAME_TIME_STEP_3_POSITION_GRID_SYSTEM,
      CCMC_VARIABLE_NAME_TIME_STEP_3_DATA_GRID_SYSTEM,
      CCMC_VARIABLE_NAME_TIME_STEP_4,
      CCMC_VARIABLE_NAME_TIME_STEP_4_DATA_TYPE,
      CCMC_VARIABLE_NAME_TIME_STEP_4_CLASSIFICATION,
      CCMC_VARIABLE_NAME_TIME_STEP_4_VALID_MIN,
      CCMC_VARIABLE_NAME_TIME_STEP_4_VALID_MAX,
      CCMC_VARIABLE_NAME_TIME_STEP_4_UNITS,
      CCMC_VARIABLE_NAME_TIME_STEP_4_GRID_SYSTEM,
      CCMC_VARIABLE_NAME_TIME_STEP_4_MASK,
      CCMC_VARIABLE_NAME_TIME_STEP_4_DESCRIPTION,
      CCMC_VARIABLE_NAME_TIME_STEP_4_IS_VECTOR,
      CCMC_VARIABLE_NAME_TIME_STEP_4_POSITION_GRID_SYSTEM,
      CCMC_VARIABLE_NAME_TIME_STEP_4_DATA_GRID_SYSTEM,
      CCMC_VARIABLE_NAME_NUMERICAL_STEP_1,
      CCMC_VARIABLE_NAME_NUMERICAL_STEP_1_DATA_TYPE,
      CCMC_VARIABLE_NAME_NUMERICAL_STEP_1_CLASSIFICATION,
      CCMC_VARIABLE_NAME_NUMERICAL_STEP_1_VALID_MIN,
      CCMC_VARIABLE_NAME_NUMERICAL_STEP_1_VALID_MAX,
      CCMC_VARIABLE_NAME_NUMERICAL_STEP_1_UNITS,
      CCMC_VARIABLE_NAME_NUMERICAL_STEP_1_GRID_SYSTEM,
      CCMC_VARIABLE_NAME_NUMERICAL_STEP_1_MASK,
      CCMC_VARIABLE_NAME_NUMERICAL_STEP_1_DESCRIPTION,
      CCMC_VARIABLE_NAME_NUMERICAL_STEP_1_IS_VECTOR,
      CCMC_VARIABLE_NAME_NUMERICAL_STEP_1_POSITION_GRID_SYSTEM,
      CCMC_VARIABLE_NAME_NUMERICAL_STEP_1_DATA_GRID_SYSTEM,
      CCMC_VARIABLE_NAME_NUMERICAL_STEP_2,
      CCMC_VARIABLE_NAME_NUMERICAL_STEP_2_DATA_TYPE,
      CCMC_VARIABLE_NAME_NUMERICAL_STEP_2_CLASSIFICATION,
      CCMC_VARIABLE_NAME_NUMERICAL_STEP_2_VALID_MIN,
      CCMC_VARIABLE_NAME_NUMERICAL_STEP_2_VALID_MAX,
      CCMC_VARIABLE_NAME_NUMERICAL_STEP_2_UNITS,
      CCMC_VARIABLE_NAME_NUMERICAL_STEP_2_GRID_SYSTEM,
      CCMC_VARIABLE_NAME_NUMERICAL_STEP_2_MASK,
      CCMC_VARIABLE_NAME_NUMERICAL_STEP_2_DESCRIPTION,
      CCMC_VARIABLE_NAME_NUMERICAL_STEP_2_IS_VECTOR,
      CCMC_VARIABLE_NAME_NUMERICAL_STEP_2_POSITION_GRID_SYSTEM,
      CCMC_VARIABLE_NAME_NUMERICAL_STEP_2_DATA_GRID_SYSTEM,
      CCMC_VARIABLE_NAME_NUMERICAL_STEP_3,
      CCMC_VARIABLE_NAME_NUMERICAL_STEP_3_DATA_TYPE,
      CCMC_VARIABLE_NAME_NUMERICAL_STEP_3_CLASSIFICATION,
      CCMC_VARIABLE_NAME_NUMERICAL_STEP_3_VALID_MIN,
      CCMC_VARIABLE_NAME_NUMERICAL_STEP_3_VALID_MAX,
      CCMC_VARIABLE_NAME_NUMERICAL_STEP_3_UNITS,
      CCMC_VARIABLE_NAME_NUMERICAL_STEP_3_GRID_SYSTEM,
      CCMC_VARIABLE_NAME_NUMERICAL_STEP_3_MASK,
      CCMC_VARIABLE_NAME_NUMERICAL_STEP_3_DESCRIPTION,
      CCMC_VARIABLE_NAME_NUMERICAL_STEP_3_IS_VECTOR,
      CCMC_VARIABLE_NAME_NUMERICAL_STEP_3_POSITION_GRID_SYSTEM,
      CCMC_VARIABLE_NAME_NUMERICAL_STEP_3_DATA_GRID_SYSTEM,
      CCMC_VARIABLE_NAME_NUMERICAL_STEP_4,
      CCMC_VARIABLE_NAME_NUMERICAL_STEP_4_DATA_TYPE,
      CCMC_VARIABLE_NAME_NUMERICAL_STEP_4_CLASSIFICATION,
      CCMC_VARIABLE_NAME_NUMERICAL_STEP_4_VALID_MIN,
      CCMC_VARIABLE_NAME_NUMERICAL_STEP_4_VALID_MAX,
      CCMC_VARIABLE_NAME_NUMERICAL_STEP_4_UNITS,
      CCMC_VARIABLE_NAME_NUMERICAL_STEP_4_GRID_SYSTEM,
      CCMC_VARIABLE_NAME_NUMERICAL_STEP_4_MASK,
      CCMC_VARIABLE_NAME_NUMERICAL_STEP_4_DESCRIPTION,
      CCMC_VARIABLE_NAME_NUMERICAL_STEP_4_IS_VECTOR,
      CCMC_VARIABLE_NAME_NUMERICAL_STEP_4_POSITION_GRID_SYSTEM,
      CCMC_VARIABLE_NAME_NUMERICAL_STEP_4_DATA_GRID_SYSTEM };

/***************************************************************** KPVT ***********************************************/

struct registered_kpvt_variables
{

   char *ccmc_var_name;
   char *ccmc_var_data_type;
   char *ccmc_var_classification;
   float valid_min;
   float valid_max;
   char *units;
   char *grid_system;
   float mask;
   char *description;
   int is_vector_component;
   char *position_grid_system;
   char *data_grid_system;
}static registered_kpvt_var_list[] =
{

      CCMC_VARIABLE_NAME_C_LON,
      CCMC_VARIABLE_NAME_C_LON_DATA_TYPE,
      CCMC_VARIABLE_NAME_C_LON_CLASSIFICATION,
      CCMC_VARIABLE_NAME_C_LON_VALID_MIN,
      CCMC_VARIABLE_NAME_C_LON_VALID_MAX,
      CCMC_VARIABLE_NAME_C_LON_UNITS,
      CCMC_VARIABLE_NAME_C_LON_GRID_SYSTEM,
      CCMC_VARIABLE_NAME_C_LON_MASK,
      CCMC_VARIABLE_NAME_C_LON_DESCRIPTION,
      CCMC_VARIABLE_NAME_C_LON_IS_VECTOR,
      CCMC_VARIABLE_NAME_C_LON_POSITION_GRID_SYSTEM,
      CCMC_VARIABLE_NAME_C_LON_DATA_GRID_SYSTEM,
      CCMC_VARIABLE_NAME_C_SINE_LAT,
      CCMC_VARIABLE_NAME_C_SINE_LAT_DATA_TYPE,
      CCMC_VARIABLE_NAME_C_SINE_LAT_CLASSIFICATION,
      CCMC_VARIABLE_NAME_C_SINE_LAT_VALID_MIN,
      CCMC_VARIABLE_NAME_C_SINE_LAT_VALID_MAX,
      CCMC_VARIABLE_NAME_C_SINE_LAT_UNITS,
      CCMC_VARIABLE_NAME_C_SINE_LAT_GRID_SYSTEM,
      CCMC_VARIABLE_NAME_C_SINE_LAT_MASK,
      CCMC_VARIABLE_NAME_C_SINE_LAT_DESCRIPTION,
      CCMC_VARIABLE_NAME_C_SINE_LAT_IS_VECTOR,
      CCMC_VARIABLE_NAME_C_SINE_LAT_POSITION_GRID_SYSTEM,
      CCMC_VARIABLE_NAME_C_SINE_LAT_DATA_GRID_SYSTEM,
      CCMC_VARIABLE_NAME_NET_FLUX,
      CCMC_VARIABLE_NAME_NET_FLUX_DATA_TYPE,
      CCMC_VARIABLE_NAME_NET_FLUX_CLASSIFICATION,
      CCMC_VARIABLE_NAME_NET_FLUX_VALID_MIN,
      CCMC_VARIABLE_NAME_NET_FLUX_VALID_MAX,
      CCMC_VARIABLE_NAME_NET_FLUX_UNITS,
      CCMC_VARIABLE_NAME_NET_FLUX_GRID_SYSTEM,
      CCMC_VARIABLE_NAME_NET_FLUX_MASK,
      CCMC_VARIABLE_NAME_NET_FLUX_DESCRIPTION,
      CCMC_VARIABLE_NAME_NET_FLUX_IS_VECTOR,
      CCMC_VARIABLE_NAME_NET_FLUX_POSITION_GRID_SYSTEM,
      CCMC_VARIABLE_NAME_NET_FLUX_DATA_GRID_SYSTEM,
      CCMC_VARIABLE_NAME_TOTAL_FLUX,
      CCMC_VARIABLE_NAME_TOTAL_FLUX_DATA_TYPE,
      CCMC_VARIABLE_NAME_TOTAL_FLUX_CLASSIFICATION,
      CCMC_VARIABLE_NAME_TOTAL_FLUX_VALID_MIN,
      CCMC_VARIABLE_NAME_TOTAL_FLUX_VALID_MAX,
      CCMC_VARIABLE_NAME_TOTAL_FLUX_UNITS,
      CCMC_VARIABLE_NAME_TOTAL_FLUX_GRID_SYSTEM,
      CCMC_VARIABLE_NAME_TOTAL_FLUX_MASK,
      CCMC_VARIABLE_NAME_TOTAL_FLUX_DESCRIPTION,
      CCMC_VARIABLE_NAME_TOTAL_FLUX_IS_VECTOR,
      CCMC_VARIABLE_NAME_TOTAL_FLUX_POSITION_GRID_SYSTEM,
      CCMC_VARIABLE_NAME_TOTAL_FLUX_DATA_GRID_SYSTEM,
      CCMC_VARIABLE_NAME_WEIGHTS,
      CCMC_VARIABLE_NAME_WEIGHTS_DATA_TYPE,
      CCMC_VARIABLE_NAME_WEIGHTS_CLASSIFICATION,
      CCMC_VARIABLE_NAME_WEIGHTS_VALID_MIN,
      CCMC_VARIABLE_NAME_WEIGHTS_VALID_MAX,
      CCMC_VARIABLE_NAME_WEIGHTS_UNITS,
      CCMC_VARIABLE_NAME_WEIGHTS_GRID_SYSTEM,
      CCMC_VARIABLE_NAME_WEIGHTS_MASK,
      CCMC_VARIABLE_NAME_WEIGHTS_DESCRIPTION,
      CCMC_VARIABLE_NAME_WEIGHTS_IS_VECTOR,
      CCMC_VARIABLE_NAME_WEIGHTS_POSITION_GRID_SYSTEM,
      CCMC_VARIABLE_NAME_WEIGHTS_DATA_GRID_SYSTEM };

/***************************************************************** MSFC_TVM ***********************************************/

struct registered_msfc_tvm_variables
{

   char *ccmc_var_name;
   char *ccmc_var_data_type;
   char *ccmc_var_classification;
   float valid_min;
   float valid_max;
   char *units;
   char *grid_system;
   float mask;
   char *description;
   int is_vector_component;
   char *position_grid_system;
   char *data_grid_system;
}static registered_msfc_tvm_var_list[] =
{

      CCMC_VARIABLE_NAME_LON,
      CCMC_VARIABLE_NAME_LON_DATA_TYPE,
      CCMC_VARIABLE_NAME_LON_CLASSIFICATION,
      CCMC_VARIABLE_NAME_LON_VALID_MIN,
      CCMC_VARIABLE_NAME_LON_VALID_MAX,
      CCMC_VARIABLE_NAME_LON_UNITS,
      CCMC_VARIABLE_NAME_LON_GRID_SYSTEM,
      CCMC_VARIABLE_NAME_LON_MASK,
      CCMC_VARIABLE_NAME_LON_DESCRIPTION,
      CCMC_VARIABLE_NAME_LON_IS_VECTOR,
      CCMC_VARIABLE_NAME_LON_POSITION_GRID_SYSTEM,
      CCMC_VARIABLE_NAME_LON_DATA_GRID_SYSTEM,
      CCMC_VARIABLE_NAME_LAT,
      CCMC_VARIABLE_NAME_LAT_DATA_TYPE,
      CCMC_VARIABLE_NAME_LAT_CLASSIFICATION,
      CCMC_VARIABLE_NAME_LAT_VALID_MIN,
      CCMC_VARIABLE_NAME_LAT_VALID_MAX,
      CCMC_VARIABLE_NAME_LAT_UNITS,
      CCMC_VARIABLE_NAME_LAT_GRID_SYSTEM,
      CCMC_VARIABLE_NAME_LAT_MASK,
      CCMC_VARIABLE_NAME_LAT_DESCRIPTION,
      CCMC_VARIABLE_NAME_LAT_IS_VECTOR,
      CCMC_VARIABLE_NAME_LAT_POSITION_GRID_SYSTEM,
      CCMC_VARIABLE_NAME_LAT_DATA_GRID_SYSTEM,
      CCMC_VARIABLE_NAME_B_L,
      CCMC_VARIABLE_NAME_B_L_DATA_TYPE,
      CCMC_VARIABLE_NAME_B_L_CLASSIFICATION,
      CCMC_VARIABLE_NAME_B_L_VALID_MIN,
      CCMC_VARIABLE_NAME_B_L_VALID_MAX,
      CCMC_VARIABLE_NAME_B_L_UNITS,
      CCMC_VARIABLE_NAME_B_L_GRID_SYSTEM,
      CCMC_VARIABLE_NAME_B_L_MASK,
      CCMC_VARIABLE_NAME_B_L_DESCRIPTION,
      CCMC_VARIABLE_NAME_B_L_IS_VECTOR,
      CCMC_VARIABLE_NAME_B_L_POSITION_GRID_SYSTEM,
      CCMC_VARIABLE_NAME_B_L_DATA_GRID_SYSTEM,
      CCMC_VARIABLE_NAME_B_T,
      CCMC_VARIABLE_NAME_B_T_DATA_TYPE,
      CCMC_VARIABLE_NAME_B_T_CLASSIFICATION,
      CCMC_VARIABLE_NAME_B_T_VALID_MIN,
      CCMC_VARIABLE_NAME_B_T_VALID_MAX,
      CCMC_VARIABLE_NAME_B_T_UNITS,
      CCMC_VARIABLE_NAME_B_T_GRID_SYSTEM,
      CCMC_VARIABLE_NAME_B_T_MASK,
      CCMC_VARIABLE_NAME_B_T_DESCRIPTION,
      CCMC_VARIABLE_NAME_B_T_IS_VECTOR,
      CCMC_VARIABLE_NAME_B_T_POSITION_GRID_SYSTEM,
      CCMC_VARIABLE_NAME_B_T_DATA_GRID_SYSTEM,
      CCMC_VARIABLE_NAME_RAZ,
      CCMC_VARIABLE_NAME_RAZ_DATA_TYPE,
      CCMC_VARIABLE_NAME_RAZ_CLASSIFICATION,
      CCMC_VARIABLE_NAME_RAZ_VALID_MIN,
      CCMC_VARIABLE_NAME_RAZ_VALID_MAX,
      CCMC_VARIABLE_NAME_RAZ_UNITS,
      CCMC_VARIABLE_NAME_RAZ_GRID_SYSTEM,
      CCMC_VARIABLE_NAME_RAZ_MASK,
      CCMC_VARIABLE_NAME_RAZ_DESCRIPTION,
      CCMC_VARIABLE_NAME_RAZ_IS_VECTOR,
      CCMC_VARIABLE_NAME_RAZ_POSITION_GRID_SYSTEM,
      CCMC_VARIABLE_NAME_RAZ_DATA_GRID_SYSTEM };

/***************************************************************** MAS ***********************************************/

struct registered_mas_variables
{

   char *ccmc_var_name;
   char *ccmc_var_data_type;
   char *ccmc_var_classification;
   float valid_min;
   float valid_max;
   char *units;
   char *grid_system;
   float mask;
   char *description;
   int is_vector_component;
   char *position_grid_system;
   char *data_grid_system;
}static registered_mas_var_list[] =
{

      CCMC_VARIABLE_NAME_R,
      CCMC_VARIABLE_NAME_R_DATA_TYPE,
      CCMC_VARIABLE_NAME_R_CLASSIFICATION,
      CCMC_VARIABLE_NAME_R_VALID_MIN,
      CCMC_VARIABLE_NAME_R_VALID_MAX,
      CCMC_VARIABLE_NAME_R_UNITS,
      CCMC_VARIABLE_NAME_R_GRID_SYSTEM,
      CCMC_VARIABLE_NAME_R_MASK,
      CCMC_VARIABLE_NAME_R_DESCRIPTION,
      CCMC_VARIABLE_NAME_R_IS_VECTOR,
      CCMC_VARIABLE_NAME_R_POSITION_GRID_SYSTEM,
      CCMC_VARIABLE_NAME_R_DATA_GRID_SYSTEM,
      CCMC_VARIABLE_NAME_PHI,
      CCMC_VARIABLE_NAME_PHI_DATA_TYPE,
      CCMC_VARIABLE_NAME_PHI_CLASSIFICATION,
      CCMC_VARIABLE_NAME_PHI_VALID_MIN,
      CCMC_VARIABLE_NAME_PHI_VALID_MAX,
      CCMC_VARIABLE_NAME_PHI_UNITS,
      CCMC_VARIABLE_NAME_PHI_GRID_SYSTEM,
      CCMC_VARIABLE_NAME_PHI_MASK,
      CCMC_VARIABLE_NAME_PHI_DESCRIPTION,
      CCMC_VARIABLE_NAME_PHI_IS_VECTOR,
      CCMC_VARIABLE_NAME_PHI_POSITION_GRID_SYSTEM,
      CCMC_VARIABLE_NAME_PHI_DATA_GRID_SYSTEM,
      CCMC_VARIABLE_NAME_THETA,
      CCMC_VARIABLE_NAME_THETA_DATA_TYPE,
      CCMC_VARIABLE_NAME_THETA_CLASSIFICATION,
      CCMC_VARIABLE_NAME_THETA_VALID_MIN,
      CCMC_VARIABLE_NAME_THETA_VALID_MAX,
      CCMC_VARIABLE_NAME_THETA_UNITS,
      CCMC_VARIABLE_NAME_THETA_GRID_SYSTEM,
      CCMC_VARIABLE_NAME_THETA_MASK,
      CCMC_VARIABLE_NAME_THETA_DESCRIPTION,
      CCMC_VARIABLE_NAME_THETA_IS_VECTOR,
      CCMC_VARIABLE_NAME_THETA_POSITION_GRID_SYSTEM,
      CCMC_VARIABLE_NAME_THETA_DATA_GRID_SYSTEM,
      CCMC_VARIABLE_NAME_R1,
      CCMC_VARIABLE_NAME_R1_DATA_TYPE,
      CCMC_VARIABLE_NAME_R1_CLASSIFICATION,
      CCMC_VARIABLE_NAME_R1_VALID_MIN,
      CCMC_VARIABLE_NAME_R1_VALID_MAX,
      CCMC_VARIABLE_NAME_R1_UNITS,
      CCMC_VARIABLE_NAME_R1_GRID_SYSTEM,
      CCMC_VARIABLE_NAME_R1_MASK,
      CCMC_VARIABLE_NAME_R1_DESCRIPTION,
      CCMC_VARIABLE_NAME_R1_IS_VECTOR,
      CCMC_VARIABLE_NAME_R1_POSITION_GRID_SYSTEM,
      CCMC_VARIABLE_NAME_R1_DATA_GRID_SYSTEM,
      CCMC_VARIABLE_NAME_PHI1,
      CCMC_VARIABLE_NAME_PHI1_DATA_TYPE,
      CCMC_VARIABLE_NAME_PHI1_CLASSIFICATION,
      CCMC_VARIABLE_NAME_PHI1_VALID_MIN,
      CCMC_VARIABLE_NAME_PHI1_VALID_MAX,
      CCMC_VARIABLE_NAME_PHI1_UNITS,
      CCMC_VARIABLE_NAME_PHI1_GRID_SYSTEM,
      CCMC_VARIABLE_NAME_PHI1_MASK,
      CCMC_VARIABLE_NAME_PHI1_DESCRIPTION,
      CCMC_VARIABLE_NAME_PHI1_IS_VECTOR,
      CCMC_VARIABLE_NAME_PHI1_POSITION_GRID_SYSTEM,
      CCMC_VARIABLE_NAME_PHI1_DATA_GRID_SYSTEM,
      CCMC_VARIABLE_NAME_THETA1,
      CCMC_VARIABLE_NAME_THETA1_DATA_TYPE,
      CCMC_VARIABLE_NAME_THETA1_CLASSIFICATION,
      CCMC_VARIABLE_NAME_THETA1_VALID_MIN,
      CCMC_VARIABLE_NAME_THETA1_VALID_MAX,
      CCMC_VARIABLE_NAME_THETA1_UNITS,
      CCMC_VARIABLE_NAME_THETA1_GRID_SYSTEM,
      CCMC_VARIABLE_NAME_THETA1_MASK,
      CCMC_VARIABLE_NAME_THETA1_DESCRIPTION,
      CCMC_VARIABLE_NAME_THETA1_IS_VECTOR,
      CCMC_VARIABLE_NAME_THETA1_POSITION_GRID_SYSTEM,
      CCMC_VARIABLE_NAME_THETA1_DATA_GRID_SYSTEM,
      CCMC_VARIABLE_NAME_T,
      CCMC_VARIABLE_NAME_T_DATA_TYPE,
      CCMC_VARIABLE_NAME_T_CLASSIFICATION,
      CCMC_VARIABLE_NAME_T_VALID_MIN,
      CCMC_VARIABLE_NAME_T_VALID_MAX,
      CCMC_VARIABLE_NAME_T_UNITS,
      CCMC_VARIABLE_NAME_T_GRID_SYSTEM,
      CCMC_VARIABLE_NAME_T_MASK,
      CCMC_VARIABLE_NAME_T_DESCRIPTION,
      CCMC_VARIABLE_NAME_T_IS_VECTOR,
      CCMC_VARIABLE_NAME_T_POSITION_GRID_SYSTEM,
      CCMC_VARIABLE_NAME_T_DATA_GRID_SYSTEM,
      CCMC_VARIABLE_NAME_P,
      CCMC_VARIABLE_NAME_P_DATA_TYPE,
      CCMC_VARIABLE_NAME_P_CLASSIFICATION,
      CCMC_VARIABLE_NAME_P_VALID_MIN,
      CCMC_VARIABLE_NAME_P_VALID_MAX,
      CCMC_VARIABLE_NAME_P_UNITS,
      CCMC_VARIABLE_NAME_P_GRID_SYSTEM,
      CCMC_VARIABLE_NAME_P_MASK,
      CCMC_VARIABLE_NAME_P_DESCRIPTION,
      CCMC_VARIABLE_NAME_P_IS_VECTOR,
      CCMC_VARIABLE_NAME_P_POSITION_GRID_SYSTEM,
      CCMC_VARIABLE_NAME_P_DATA_GRID_SYSTEM,
      CCMC_VARIABLE_NAME_RHO,
      CCMC_VARIABLE_NAME_RHO_DATA_TYPE,
      CCMC_VARIABLE_NAME_RHO_CLASSIFICATION,
      CCMC_VARIABLE_NAME_RHO_VALID_MIN,
      CCMC_VARIABLE_NAME_RHO_VALID_MAX,
      CCMC_VARIABLE_NAME_RHO_UNITS,
      CCMC_VARIABLE_NAME_RHO_GRID_SYSTEM,
      CCMC_VARIABLE_NAME_RHO_MASK,
      CCMC_VARIABLE_NAME_RHO_DESCRIPTION,
      CCMC_VARIABLE_NAME_RHO_IS_VECTOR,
      CCMC_VARIABLE_NAME_RHO_POSITION_GRID_SYSTEM,
      CCMC_VARIABLE_NAME_RHO_DATA_GRID_SYSTEM,
      CCMC_VARIABLE_NAME_BR,
      CCMC_VARIABLE_NAME_BR_DATA_TYPE,
      CCMC_VARIABLE_NAME_BR_CLASSIFICATION,
      CCMC_VARIABLE_NAME_BR_VALID_MIN,
      CCMC_VARIABLE_NAME_BR_VALID_MAX,
      CCMC_VARIABLE_NAME_BR_UNITS,
      CCMC_VARIABLE_NAME_BR_GRID_SYSTEM,
      CCMC_VARIABLE_NAME_BR_MASK,
      CCMC_VARIABLE_NAME_BR_DESCRIPTION,
      CCMC_VARIABLE_NAME_BR_IS_VECTOR,
      CCMC_VARIABLE_NAME_BR_POSITION_GRID_SYSTEM,
      CCMC_VARIABLE_NAME_BR_DATA_GRID_SYSTEM,
      CCMC_VARIABLE_NAME_BPHI,
      CCMC_VARIABLE_NAME_BPHI_DATA_TYPE,
      CCMC_VARIABLE_NAME_BPHI_CLASSIFICATION,
      CCMC_VARIABLE_NAME_BPHI_VALID_MIN,
      CCMC_VARIABLE_NAME_BPHI_VALID_MAX,
      CCMC_VARIABLE_NAME_BPHI_UNITS,
      CCMC_VARIABLE_NAME_BPHI_GRID_SYSTEM,
      CCMC_VARIABLE_NAME_BPHI_MASK,
      CCMC_VARIABLE_NAME_BPHI_DESCRIPTION,
      CCMC_VARIABLE_NAME_BPHI_IS_VECTOR,
      CCMC_VARIABLE_NAME_BPHI_POSITION_GRID_SYSTEM,
      CCMC_VARIABLE_NAME_BPHI_DATA_GRID_SYSTEM,
      CCMC_VARIABLE_NAME_BTHETA,
      CCMC_VARIABLE_NAME_BTHETA_DATA_TYPE,
      CCMC_VARIABLE_NAME_BTHETA_CLASSIFICATION,
      CCMC_VARIABLE_NAME_BTHETA_VALID_MIN,
      CCMC_VARIABLE_NAME_BTHETA_VALID_MAX,
      CCMC_VARIABLE_NAME_BTHETA_UNITS,
      CCMC_VARIABLE_NAME_BTHETA_GRID_SYSTEM,
      CCMC_VARIABLE_NAME_BTHETA_MASK,
      CCMC_VARIABLE_NAME_BTHETA_DESCRIPTION,
      CCMC_VARIABLE_NAME_BTHETA_IS_VECTOR,
      CCMC_VARIABLE_NAME_BTHETA_POSITION_GRID_SYSTEM,
      CCMC_VARIABLE_NAME_BTHETA_DATA_GRID_SYSTEM,
      CCMC_VARIABLE_NAME_UR,
      CCMC_VARIABLE_NAME_UR_DATA_TYPE,
      CCMC_VARIABLE_NAME_UR_CLASSIFICATION,
      CCMC_VARIABLE_NAME_UR_VALID_MIN,
      CCMC_VARIABLE_NAME_UR_VALID_MAX,
      CCMC_VARIABLE_NAME_UR_UNITS,
      CCMC_VARIABLE_NAME_UR_GRID_SYSTEM,
      CCMC_VARIABLE_NAME_UR_MASK,
      CCMC_VARIABLE_NAME_UR_DESCRIPTION,
      CCMC_VARIABLE_NAME_UR_IS_VECTOR,
      CCMC_VARIABLE_NAME_UR_POSITION_GRID_SYSTEM,
      CCMC_VARIABLE_NAME_UR_DATA_GRID_SYSTEM,
      CCMC_VARIABLE_NAME_UPHI,
      CCMC_VARIABLE_NAME_UPHI_DATA_TYPE,
      CCMC_VARIABLE_NAME_UPHI_CLASSIFICATION,
      CCMC_VARIABLE_NAME_UPHI_VALID_MIN,
      CCMC_VARIABLE_NAME_UPHI_VALID_MAX,
      CCMC_VARIABLE_NAME_UPHI_UNITS,
      CCMC_VARIABLE_NAME_UPHI_GRID_SYSTEM,
      CCMC_VARIABLE_NAME_UPHI_MASK,
      CCMC_VARIABLE_NAME_UPHI_DESCRIPTION,
      CCMC_VARIABLE_NAME_UPHI_IS_VECTOR,
      CCMC_VARIABLE_NAME_UPHI_POSITION_GRID_SYSTEM,
      CCMC_VARIABLE_NAME_UPHI_DATA_GRID_SYSTEM,
      CCMC_VARIABLE_NAME_UTHETA,
      CCMC_VARIABLE_NAME_UTHETA_DATA_TYPE,
      CCMC_VARIABLE_NAME_UTHETA_CLASSIFICATION,
      CCMC_VARIABLE_NAME_UTHETA_VALID_MIN,
      CCMC_VARIABLE_NAME_UTHETA_VALID_MAX,
      CCMC_VARIABLE_NAME_UTHETA_UNITS,
      CCMC_VARIABLE_NAME_UTHETA_GRID_SYSTEM,
      CCMC_VARIABLE_NAME_UTHETA_MASK,
      CCMC_VARIABLE_NAME_UTHETA_DESCRIPTION,
      CCMC_VARIABLE_NAME_UTHETA_IS_VECTOR,
      CCMC_VARIABLE_NAME_UTHETA_POSITION_GRID_SYSTEM,
      CCMC_VARIABLE_NAME_UTHETA_DATA_GRID_SYSTEM,
      CCMC_VARIABLE_NAME_JR,
      CCMC_VARIABLE_NAME_JR_DATA_TYPE,
      CCMC_VARIABLE_NAME_JR_CLASSIFICATION,
      CCMC_VARIABLE_NAME_JR_VALID_MIN,
      CCMC_VARIABLE_NAME_JR_VALID_MAX,
      CCMC_VARIABLE_NAME_JR_UNITS,
      CCMC_VARIABLE_NAME_JR_GRID_SYSTEM,
      CCMC_VARIABLE_NAME_JR_MASK,
      CCMC_VARIABLE_NAME_JR_DESCRIPTION,
      CCMC_VARIABLE_NAME_JR_IS_VECTOR,
      CCMC_VARIABLE_NAME_JR_POSITION_GRID_SYSTEM,
      CCMC_VARIABLE_NAME_JR_DATA_GRID_SYSTEM,
      CCMC_VARIABLE_NAME_JPHI,
      CCMC_VARIABLE_NAME_JPHI_DATA_TYPE,
      CCMC_VARIABLE_NAME_JPHI_CLASSIFICATION,
      CCMC_VARIABLE_NAME_JPHI_VALID_MIN,
      CCMC_VARIABLE_NAME_JPHI_VALID_MAX,
      CCMC_VARIABLE_NAME_JPHI_UNITS,
      CCMC_VARIABLE_NAME_JPHI_GRID_SYSTEM,
      CCMC_VARIABLE_NAME_JPHI_MASK,
      CCMC_VARIABLE_NAME_JPHI_DESCRIPTION,
      CCMC_VARIABLE_NAME_JPHI_IS_VECTOR,
      CCMC_VARIABLE_NAME_JPHI_POSITION_GRID_SYSTEM,
      CCMC_VARIABLE_NAME_JPHI_DATA_GRID_SYSTEM,
      CCMC_VARIABLE_NAME_JTHETA,
      CCMC_VARIABLE_NAME_JTHETA_DATA_TYPE,
      CCMC_VARIABLE_NAME_JTHETA_CLASSIFICATION,
      CCMC_VARIABLE_NAME_JTHETA_VALID_MIN,
      CCMC_VARIABLE_NAME_JTHETA_VALID_MAX,
      CCMC_VARIABLE_NAME_JTHETA_UNITS,
      CCMC_VARIABLE_NAME_JTHETA_GRID_SYSTEM,
      CCMC_VARIABLE_NAME_JTHETA_MASK,
      CCMC_VARIABLE_NAME_JTHETA_DESCRIPTION,
      CCMC_VARIABLE_NAME_JTHETA_IS_VECTOR,
      CCMC_VARIABLE_NAME_JTHETA_POSITION_GRID_SYSTEM,
      CCMC_VARIABLE_NAME_JTHETA_DATA_GRID_SYSTEM };

#endif /* CCMC_REGISTERED_VARIABLES_H_ */


