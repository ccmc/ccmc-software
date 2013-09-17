/*******************************************************************************
 *                                                                             *
 *    NAME:          close_ctip_cdf.c                                          *
 *                                                                             *
 *    VERSION:       1.0                                                       *
 *                                                                             *
 *    AUTHOR:        Marlo M. H. Maddox                                        *
 *                   NASA-GSFC-AETD-ISD-ADMA(587)-CCMC                         *
 *                   Marlo.Maddox@nasa.gov                                     *
 *                   (301) 286 - 5202                                          *
 *                                                                             *
 *    PURPOSE:       Performs necessary clean up from opening a ctip file      *
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
#include "ctip_cdf_interface_variables.h"

void close_ctip_cdf(
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
   free(H_pos);
   free(Vn_x);
   free(Vn_y);
   free(Vn_z);
   free(Vi_x);
   free(Vi_y);
   free(N_e);
   free(N_O);
   free(N_O2);
   free(N_N2);
   free(Rmt);
   free(Tn);
   free(sigmaP);
   free(sigmaH);

   return;

}

