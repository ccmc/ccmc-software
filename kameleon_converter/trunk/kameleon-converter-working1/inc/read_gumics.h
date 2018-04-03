/*******************************************************************************
 *                                                                             *
 *    NAME:         read_gumics.h                                             *
 *                                                                             *
 *    VERSION:      1.0                                                        *
 *                                                                             *
 *    AUTHOR:       Lutz Rastaetter                                            *
 *                  Space Weather Laboratory (674)                             *
 *                  Lutz.Rastaetter@nasa.gov                                   *
 *                  (301) 286 - 1085                                           *
 *                                                                             *
 *    PURPOSE:                                                                 *
 *                                                                             *
 *    INPUTS:       *                                                          *
 *                                                                             *
 *    OUTPUTS:      *                                                          *
 *                                                                             *
 *    FUNCTIONS:    *                                                          *
 *                                                                             *
 *    VARIABLES:    GUMICS-specific attributes                                 *
 *                                                                             *
 *                                                                             *
 *    MODIFICATION                                                             *
 *    HISTORY:                                                                 *
 *                                                                             *
 *    2018/03/30     Lutz Rastaetter                                           *
 *                   define GUMICS-specific attributes only                    *
 *                                                                             *
 *                                                                             *
 *                                                                             *
 ******************************************************************************/

#define GUMICS_MODEL_DESCRIPTION "MHD Model of the Earth's magnetosphere using cell-adaptive grid (single-cell blocks) with a coupled ionosphere electrodynamics solver - Pekka Janhunen, Ilja Honkonen -  Finnish Meteorological Institute (FMI) www.fmi.fi."

#define GUMICS_COORDINATE_DISCRIPTION "Geocentric Solar Ecliptic (GSE) - X,Y,Z"

#define GUMICS_DISCIPLINE "Magnetosphere"

/* we will reuse variables defined in read_batsrus.h that are not GUMICS-specific */

