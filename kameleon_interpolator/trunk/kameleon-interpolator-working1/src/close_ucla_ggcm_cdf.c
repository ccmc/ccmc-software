/*******************************************************************************
 *                                                                             *
 *    NAME:       close_ucla_ggcm_cdf.c                                        *
 *                                                                             *
 *    VERSION:    1.0                                                          *
 *                                                                             *
 *    AUTHOR:     Marlo M. H. Maddox                                           *
 *                NASA-GSFC-AETD-ISD-ADMA(587)-CCMC                            *
 *                Marlo.Maddox@nasa.gov                                        *
 *                (301) 286 - 5202                                             *
 *                                                                             *
 *    PURPOSE:    Performs necessary clean up from opening a ucla-ggcm/openggcm*
 *                file                                                         *
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
#include "ucla_ggcm_cdf_interface_variables.h"

void close_ucla_ggcm_cdf(
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
   free(x_bx_pos);
   free(y_bx_pos);
   free(z_bx_pos);
   free(x_by_pos);
   free(y_by_pos);
   free(z_by_pos);
   free(x_bz_pos);
   free(y_bz_pos);
   free(z_bz_pos);
   free(bx);
   free(by);
   free(bz);
   free(bx1);
   free(by1);
   free(bz1);
   free(ux);
   free(uy);
   free(uz);
   free(jx);
   free(jy);
   free(jz);
   free(rho);
   free(p);
   free(eta);

   return;

}

