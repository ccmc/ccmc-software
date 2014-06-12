/*******************************************************************************
 *                                                                             *
 *    NAME:          get_current_CDFid.c                                                *
 *                                                                             *
 *    VERSION:       1.0                                                       *
 *                                                                             *
 *    AUTHOR:        David Berrios                                             *
 *                                                                             *
 *    PURPOSE:       Returns the CDFid of the currently selected file          *
 *                                                                             *
 *    INPUTS:        													       *
 *                                                                             *
 *    OUTPUTS:                                                                 *
 *                                                                             *
 *    FUNCTIONS:                                                               *
 *                                                                             *
 *    VARIABLES:                                                               *
 *                                                                             *
 *    MODIFICATION                                                             *
 *    HISTORY:                                                                 *
 *                                                                             *
 *      5/29/2009     David Berrios                                            *
 *                                                                             *
 ******************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <math.h>
#include <stdarg.h>
#include <unistd.h>
#include "cdf.h"
#include "cdf_interface_variables.h"
#include "registered_ccmc_variable_names.h"
#include "list_structure_toolkit.h"

#define DEBUG_FLAG 0

CDFid get_current_CDFid()
{
	extern CDFid current_file_id;
	return current_file_id;
}
