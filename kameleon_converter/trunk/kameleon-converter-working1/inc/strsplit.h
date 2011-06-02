/*******************************************************************************
 *                                                                             *
 *    NAME:          strsplit.h                                                *
 *                                                                             *
 *    VERSION:       0.1                                                       *
 *                                                                             *
 *    AUTHOR:        Lutz Rastaetter                                           *
 *                   CUA/CCMC/NASA-GSFC (612.3)                                *
 *                   lr@waipio.gsfc.nasa.gov                                   *
 *                   (301) 286 - 1085                                          *
 *                                                                             *
 *    PURPOSE:       String Spliting Data Structure and routine definitions    *
 *                                                                             *
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

typedef struct
{
   long len;
   char **sarr;
} strarr;

int free_strarr(
      strarr *);
int strsplit(
      char *,
      strarr *,
      char *);
