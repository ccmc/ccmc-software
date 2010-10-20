/*******************************************************************************
 *                                                                             *
 *       NAME:          f2c_close_cdf.c                                        *
 *                                                                             *
 *       VERSION:       1.0                                                    *
 *                                                                             *
 *       AUTHOR:        Marlo M. H. Maddox                                     *
 *                      NASA-GSFC-AETD-ISD-ADMA(587)-CCMC                      *
 *                      Marlo.Maddox@nasa.gov                                  *
 *                      (301) 286 - 5202                                       *
 *                                                                             *
 *       PURPOSE:       Fortran wrapper to close_cdf routine                   *
 *                                                                             *
 *       INPUTS:        Path/Filename String Generated In Fortran              *
 *                                                                             *
 *       OUTPUTS:       ...                                                    *
 *                                                                             *
 *       FUNCTIONS:     ...                                                    *
 *                                                                             *
 *       VARIABLES:     ...                                                    *
 *                                                                             *
 *                                                                             *
 *       MODIFICATION                                                          *
 *       HISTORY:                                                              *
 *                                                                             *
 *       08.28.2007     Maddox, Marlo                                          *
 *                      added -fno-second-underscore to Makefile to avoid      *
 *                      having to implement pre-processor macros to choose     *
 *                      the correct function name.                             *
 ******************************************************************************/

#include <stdio.h>
#include <stdlib.h>

void f2c_close_cdf_(
      void)
{

   extern void close_cdf(
         void);

   close_cdf();

   return;

}

