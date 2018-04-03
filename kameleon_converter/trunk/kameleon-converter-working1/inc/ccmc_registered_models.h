/*******************************************************************************
 *    NAME:       ccmc_registered_models.h                                     *
 *                                                                             *
 *    VERSION:    5.0                                                          *
 *                                                                             *
 *    AUTHOR:     Marlo M. H. Maddox                                           *
 *                NASA-GSFC-AETD-ISD-ADMA(587)                                 *
 *                Marlo.Maddox@nasa.gov                                        *
 *                (301) 286 - 5202                                             *
 *                                                                             *
 *    PURPOSE:    centralized location for registerd CCMC model names. To be   *
 *                used for data format standarization through conversion       *
 *                software                                                     *
 *                                                                             *
 *    INPUTS:     NONE                                                         *
 *                                                                             *
 *    OUTPUTS:    NONE                                                         *
 *                                                                             *
 *    FUNCTIONS:  NONE                                                         *
 *                                                                             *
 *    VARIABLES:  - MACRO model names registered within the CCMC               *
 *                                                                             *
 *    MODIFICATION                                                             *
 *    HISTORY:                                                                 *
 *                                                                             *
 *    09.25.2003  Maddox, Marlo                                                *
 *                initial development started                                  *
 *                                                                             *
 *    03.23.2004  Maddox, Marlo                                                *
 *                added ucla_ggcm model to registry                            *
 *                                                                             *
 *    03.26.2004  Maddox, Marlo                                                *
 *                made array declarations static                               *
 *                                                                             *
 *    04.05.2005  Maddox, Marlo                                                *
 *                added CTIP                                                   *
 *                                                                             *
 *    02.02.2006  Maddox, Marlo                                                *
 *                adding open_ggcm aka ucla_ggcm and new enlil model           *
 *    11.29.2006  Maddox, Marlo                                                *
 *                adding kptv for the kitt peak vacuum telescope observation   *
 *                data                                                         *
 *                                                                             *
 *    12.04.2006  Maddox, Marlo                                                *
 *                Adding msfc_tvm for Marshalls' Tower Vector Magnetograph     *
 *                telescope observations...active regions                      *
 *                                                                             *
 *    12.14.2006  Maddox, Marlo                                                *
 *                Adding MAS                                                   *
 *                                                                             *
 ******************************************************************************/

#define NUMBER_REGISTERED_MODELS     9   /*** update total number of registered models ***/

/******************* define registered model names ******************************/

#define BATSRUS      "batsrus"
#define UCLA_GGCM    "ucla_ggcm"
#define OPEN_GGCM    "open_ggcm"
#define CTIP         "ctip"
#define ENLIL        "enlil"
#define KPVT         "kpvt"
#define MSFC_TVM     "msfc_tvm"
#define MAS          "mas"
#define GUMICS       "gumics"

/********************* load all regustered model names into registered_model array **********/

static char *registered_models[] =
  { BATSRUS, UCLA_GGCM, OPEN_GGCM, CTIP, ENLIL, KPVT, MSFC_TVM, MAS, GUMICS };
