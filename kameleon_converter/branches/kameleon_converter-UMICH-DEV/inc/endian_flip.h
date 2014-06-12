/*******************************************************************************
 *                                                                             *
 *    NAME:          endian_flip.h                                             *
 *                                                                             *
 *    VERSION:       0.1                                                       *
 *                                                                             *
 *    AUTHOR:        Lutz Rastaetter                                           *
 *                   CUA/CCMC/NASA-GSFC (612.3)                                *
 *                   lr@waipio.gsfc.nasa.gov                                   *
 *                   (301) 286 - 1085                                          *
 *                                                                             *
 *    PURPOSE:       endian_flip Routine definition.  endian_flip converts     *
 *                   to/from big <--> little endian                            *
 *                                                                             *
 *    INPUTS:        *                                                         *
 *                                                                             *
 *    OUTPUTS:       *                                                         *
 *                                                                             *
 *    FUNCTIONS:     *                                                         *
 *                                                                             *
 *    VARIABLES:     *                                                         *
 *                                                                             *
 *    MODIFICATION                                                             *
 *    HISTORY:                                                                 *
 *                                                                             *
 *    ??.??.????     Rastaetter, Lutz                                          *
 *                   initial development started                               *
 *                                                                             *
 ******************************************************************************/

#ifndef _ENDIAN_FLIP_
int endian_flip(char *array,long unit_len,long n_units);
#define _ENDIAN_FLIP_
#endif
