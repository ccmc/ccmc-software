/*******************************************************************************
 *                                                                             *
 *    NAME:          cdf_status_handler.c                                      *
 *                                                                             *
 *    VERSION:       3.2                                                       *
 *                                                                             *
 *    AUTHOR:        Marlo M. H. Maddox                                        *
 *                   NASA-GSFC-AETD-ISD-ADMA(587)                              *
 *                   Marlo.Maddox@nasa.gov                                     *
 *                   (301) 286 - 5202                                          *
 *                                                                             *
 *    PURPOSE:       Capture return status of important CDF library            *
 *                   routines.                                                 *
 *                                                                             *
 *    INPUTS:        CDF status value                                          *
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
 *    02.06.2007    Maddox, Marlo                                              *
 *                  Instead of dying whenever kameleon tries to re-create an   *
 *                  existing cdf, lets just return control back to the the     *
 *                  calling routine so we can skip the file and continue       *
 *                  processing.  Done by checking for CDF_EXISTS               *
 *                                                                             *
 *    07.24.2008    Maddox, Marlo                                              *
 *                  Added EXIT_FAILURE & EXIT_SUCCESS to qualifying            *
 *                  return and exit calls                                      *
 *                                                                             *
 ******************************************************************************/

#include <stdio.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "cdf.h"

/*******************************************************************************
 *                                                                             *
 *                               STATUS HANDLER                                *
 *                                                                             *
 ******************************************************************************/

int StatusHandler(
      CDFstatus status)
/*CDFstatus status;*/
{

   char message[CDF_ERRTEXT_LEN+1];

   if (status < CDF_WARN)
   {
      printf("AN error has occurred, halting...\n");
      CDFerror(status, message);
      printf(
            "%s\n",
            message);

      /* Instead of dying whenever kameleon tries to re-create an existing cdf,
       * lets just return control back to the the calling routine so we can
       * skip the file and continue processing */

      if (status == CDF_EXISTS)
      {
         printf("...Kameleon refusing to overwrite existing file. Skipping...\n");

         return EXIT_FAILURE;
      }
      else
      {
         exit(EXIT_FAILURE);
      }

   }
   else if (status < CDF_OK)
   {
      printf("Warning, function may not have completed as expected...\n");
      CDFerror(status, message);
      printf(
            "%s\n",
            message);
   }
   else if (status > CDF_OK)
   {
      printf("Function Completed successfully, but be advised that...\n");
      CDFerror(status, message);
      printf(
            "%s\n",
            message);
   }

   return EXIT_SUCCESS;

}
