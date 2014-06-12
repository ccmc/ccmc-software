/*******************************************************************************
 *                                                                             *
 *       NAME:          close_mas_cdf.c                                        *
 *                                                                             *
 *       VERSION:       1.0                                                    *
 *                                                                             *
 *       AUTHOR:        Marlo M. H. Maddox                                     *
 *                      NASA-GSFC-AETD-ISD-ADMA(587)-CCMC                      *
 *                      Marlo.Maddox@nasa.gov                                  *
 *                      (301) 286 - 5202                                       *
 *                                                                             *
 *       PURPOSE:       Performs necessary clean up from opening a enlil file  *
 *                                                                             *
 *       INPUTS:                                                               *
 *                                                                             *
 *       OUTPUTS:                                                              *
 *                                                                             *
 *       FUNCTIONS:                                                            *
 *                                                                             *
 *       VARIABLES:                                                            *
 *                                                                             *
 *                                                                             *
 *       MODIFICATION                                                          *
 *       HISTORY:                                                              *
 *                                                                             *
 *       02.29.2008     Maddox, Marlo                                          *
                        Initial Dev Started                                    *
 ******************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <math.h>
#include <stdarg.h>
#include "cdf_interface_variables.h"
#include "cdf.h"
#include "mas_cdf_interface_variables.h"

void close_mas_cdf(
      void)
{

   CDFstatus status;
   extern CDFid current_file_id;

   status = CDFclose ( current_file_id );
   if (status != CDF_OK)
      StatusHandler(status);

   free(r_pos);
   free(phi_pos);
   free(theta_pos);
   free(r1_pos);
   free(theta1_pos);
   free(br);
   free(bphi);
   free(btheta);
   free(ur);
   free(uphi);
   free(utheta);
   free(jr);
   free(jphi);
   free(jtheta);
   free(rho);
   free(p);
   free(T);

   return;

}

