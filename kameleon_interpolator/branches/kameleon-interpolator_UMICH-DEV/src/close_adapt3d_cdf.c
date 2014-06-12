/*******************************************************************************
 *                                                                             *
 *    NAME:          close_adapt3d_cdf.c                                       *
 *                                                                             *
 *    VERSION:       1.0                                                       *
 *                                                                             *
 *    AUTHOR:        Peter MacNeice                                            *
 *                   Marlo M. H. Maddox                                        *
 *                   NASA-GSFC-AETD-ISD-ADMA(587)-CCMC                         *
 *                   Marlo.Maddox@nasa.gov                                     *
 *                   (301) 286 - 5202                                          *
 *                                                                             *
 *    PURPOSE:       Performs necessary clean up from opening a ADAPT3D file   *
 *                                                                             *
 *    INPUTS:                                                                  *
 *                                                                             *
 *    OUTPUTS:                                                                 *
 *                                                                             *
 *    FUNCTIONS:                                                               *
 *                                                                             *
 *    VARIABLES:                                                               *
 *                                                                             *
 *                                                                             *
 *    MODIFICATION                                                             *
 *    HISTORY:                                                                 *
 *                                                                             *
 *                                                                             *
 ******************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <math.h>
#include <stdarg.h>
#include "cdf_interface_variables.h"
#include "cdf.h"
#include "adapt3d_cdf_interface_variables.h"

void close_adapt3d_cdf(
      void)
{

   CDFstatus status;
   extern CDFid current_file_id;

   status = CDFclose ( current_file_id );
   if (status != CDF_OK)
      StatusHandler(status);

   free(unkno);
   free(coord);
   free(rho);
   free(bx);
   free(by);
   free(bz);
   free(ux);
   free(uy);
   free(uz);
   free(p);
   free(temp);
   free(intmat);
   free(bconi);
   free(indx);
   free(esup1);
   free(esup2);

   return;

}

