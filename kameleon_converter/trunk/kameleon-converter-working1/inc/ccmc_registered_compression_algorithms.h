/*******************************************************************************
 *                                                                             *
 *    NAME:       ccmc_registered_compression_algorithms.h                     *
 *                                                                             *
 *    VERSION:    1.0                                                          *
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
 *    VARIABLES:  - MACRO compression algorithm names registered/supported     *
 *                within the CCMC                                              *
 *                                                                             *
 *    MODIFICATION                                                             *
 *    HISTORY:                                                                 *
 *                                                                             *
 *    08.16.2005  Maddox, Marlo                                                *
 *                Initial development started                                  *
 ******************************************************************************/

#define NUMBER_REGISTERED_COMPRESSION_ALGORITHMS    4    /*** update total number of registered compression algorithms ***/

/******************* define registered model names ******************************/

#define RLE     "rle"
#define HUFF    "huff"
#define AHUFF   "ahuff"
#define GZIP    "gzip"

/** I don't feel like passing the command line specified compression algorithm through the entire program lets just set it here **/

char current_compression[25];

/********************* load all regustered compression algorithmnames into registered_model array **********/

static char *registered_compression_algorithms[] =
{ RLE, HUFF, AHUFF, GZIP };
