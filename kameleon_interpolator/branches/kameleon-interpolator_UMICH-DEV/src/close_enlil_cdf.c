/*******************************************************************************
 *                                                                             *
 *    NAME:          close_enlil_cdf.c                                         *
 *                                                                             *
 *    VERSION:       1.0                                                       *
 *                                                                             *
 *    AUTHOR:        Marlo M. H. Maddox                                        *
 *                   NASA-GSFC-AETD-ISD-ADMA(587)-CCMC                         *
 *                   Marlo.Maddox@nasa.gov                                     *
 *                   (301) 286 - 5202                                          *
 *                                                                             *
 *    PURPOSE:       Performs necessary clean up from opening a enlil file     *
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
#include "enlil_cdf_interface_variables.h"

void close_enlil_cdf(
      void)
{

   CDFstatus status;
   extern CDFid current_file_id;

   status = CDFclose ( current_file_id );
   if (status != CDF_OK)
      StatusHandler(status);

   free(x_pos);
   free(y_pos);
   free(z_pos);
   free(bx);
   free(by);
   free(bz);
   free(b1x);
   free(b1y);
   free(b1z);
   free(ux);
   free(uy);
   free(uz);
   free(rho);
   free(dp);
   free(T);
   free(bp);

   return;

}

