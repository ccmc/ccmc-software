/*******************************************************************************
 *                                                                             *
 *    NAME:        ccmc_registered_formats.h                                   *
 *                                                                             *
 *    VERSION:     1.0                                                         *
 *                                                                             *
 *    AUTHOR:     Marlo M. H. Maddox                                           *
 *                NASA-GSFC-AETD-ISD-ADMA(587)                                 *
 *                Marlo.Maddox@nasa.gov                                        *
 *                (301) 286 - 5202                                             *
 *                                                                             *
 *    PURPOSE:    centralized location for registerd/supported CCMC            *
 *                data format names. To be used for data format                *
 *                standarization through conversion software                   *
 *                                                                             *
 *    INPUTS:     NONE                                                         *
 *                                                                             *
 *    OUTPUTS:    NONE                                                         *
 *                                                                             *
 *    FUNCTIONS:  NONE                                                         *
 *                                                                             *
 *    VARIABLES:  - MACRO data format names registered/supported within the    *
 *                CCMC                                                         *
 *                                                                             *
 *    MODIFICATION                                                             *
 *    HISTORY:                                                                 *
 *                                                                             *
 *    09.25.2003  Maddox, Marlo                                                *
 *                initial development started                                  *
 *    03.26.2004  Maddox, Marlo                                                *
 *                made array declarations static                               *
 *                                                                             *
 ******************************************************************************/

#define NUMBER_REGISTERED_FORMATS 2 /*** update total number of registered models ***/

/******************* define registered model names ******************************/

#define CDF    "cdf"
#define HDF5    "hdf5"

/********************* load all regustered model names into registered_model array **********/

static char *registered_formats[] =
{ CDF, HDF5 };
