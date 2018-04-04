/*******************************************************************************
 *                                                                             *
 *       NAME:          cdf_interface_variables.h                              *
 *                                                                             *
 *       VERSION:       1.0                                                    *
 *                                                                             *
 *       AUTHOR:        Marlo M. H. Maddox                                     *
 *                      NASA-GSFC-AETD-ISD-ADMA(587)-CCMC                      *
 *                      Marlo.Maddox@nasa.gov                                  *
 *                      (301) 286 - 5202                                       *
 *                                                                             *
 *       PURPOSE:       Definitions all globaly used cdf variables             *
 *                                                                             *
 *       INPUTS:                                                               *
 *                                                                             *
 *       OUTPUTS:                                                              *
 *                                                                             *
 *       FUNCTIONS:     cdf_status_handler.c, close_batsrus_cdf.c, close_cdf.c *
 *                      close_ctip_cdf.c ...and a bunch of other ones too      *
 *                      ...see doxygen for details...                          *
 *                                                                             *
 *        VARIABLES:                                                           *
 *                                                                             *
 *                                                                             *
 *       MODIFICATION                                                          *
 *       HISTORY:                                                              *
 *                                                                             *
 *                                                                             *
 *       10.13.2006     Maddox, Marlo                                          *
 *                      Removed bug introduced in this version                 *
 *                      non-initialized variable should not be                 *
 *                      static...                                              *
 *                                                                             *
 *       04.15.2008     Maddox, Marlo Fixing Multiopen Batsrus Bug.  Had to Add*
 *                      full_bats_grid_in_memory variable to help us determine *
 *                      if multi open's with bats require the entire amr grid  *
 *                      to be loaded                                           *
 *                                                                             *
 *                      Also had to add an allocate_defaults_done flag to      *
 *                      determine if muli-open batsrus needs to call           *
 *                      allocate_defaults_done NOTE: when                      *
 *                      allocate_defaults_done was static, the variable wasn't *
 *                      being reset properly.                                  *
 ******************************************************************************/

/** prevent the contents of the header from being scanned multiple times and
 * also solve the problem of multiple inclusion **/

#ifndef CDF_INTERFACE_VARIABLES_
#define CDF_INTERFACE_VARIABLES_

#include "cdf.h"

int StatusHandler(
      CDFstatus);

CDFid current_file_id;

int new_cdf_file;
int allocate_defaults_done;

int full_bats_grid_in_memory;

static float cdf_missing_value = -.01e-32;

#endif /*CDF_INTERFACE_VARIABLES_*/

