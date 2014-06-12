/*******************************************************************************
 *                                                                             *
 *    NAME:          cdf_status_handler.c                                      *
 *                                                                             *
 *    VERSION:       1.0                                                       *
 *                                                                             *
 *    AUTHOR:        Marlo M. H. Maddox                                        *
 *                   NASA-GSFC-AETD-ISD-ADMA(587)-CCMC                         *
 *                   Marlo.Maddox@nasa.gov                                     *
 *                   (301) 286 - 5202                                          *
 *                                                                             *
 *    PURPOSE:       Central Location to process cdf status values returned    *
 *                   from cdf calls                                            *
 *                                                                             *
 *    INPUTS:        CDFstatus status                                          *
 *                                                                             *
 *    OUTPUTS:       CDFstatus status Value                                    *
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
#include "cdf_interface_variables.h"
#include "cdf.h"

/*******************************************************************************
 *                                                                             *
 *                               STATUS HANDLER                                *
 *                                                                             *
 *******************************************************************************/

int StatusHandler(
      CDFstatus status)
{

   char message[CDF_ERRTEXT_LEN+1];

   if (status < CDF_WARN)
   {
      printf("AN error has occurred, halting...\n");
      CDFerror (status, message);
      printf(
            "%s\n",
            message);
      /*exit (status);*/
   }
   else if (status < CDF_OK)
   {
      printf("Warning, function may not have completed as expected...\n");
      CDFerror (status, message);
      printf(
            "%s\n",
            message);
   }
   else if (status > CDF_OK)
   {
      printf("Function Completed successfully, but be advised that...\n");
      CDFerror (status, message);
      printf(
            "%s\n",
            message);
   }

   /* modified originally void functionwith simple return; */

   return status;

}

