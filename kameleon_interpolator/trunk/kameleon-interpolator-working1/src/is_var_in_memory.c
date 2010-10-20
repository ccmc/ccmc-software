/*******************************************************************************
 *                                                                             *
 *    NAME:          is_var_in_memory.c                                        *
 *                                                                             *
 *    VERSION:       1.0                                                       *
 *                                                                             *
 *    AUTHOR:        Marlo M. H. Maddox                                        *
 *                   NASA-GSFC-AETD-ISD-ADMA(587)-CCMC                         *
 *                   Marlo.Maddox@nasa.gov                                     *
 *                   (301) 286 - 5202                                          *
 *                                                                             *
 *    PURPOSE:       Variables loaded into main memory using the open_cdf call *
 *                   have their names stored in a list structure pointed to by *
 *                   loaded_variable_listPtr.  This routine searches the list  *
 *                   structure for the existence of a specified variable name. *
 *                   If the name is not in the list, the variable is not       *
 *                   currently in main memory and 0 is returned.  Otherwise a  *
 *                   value of 1 is returned indicating that the variables was  *
 *                   inserted in the list and thus has already been loaded into*
 *                   main memory                                               *
 *                                                                             *
 *    INPUTS:        char *variable_name to inquire about                      *
 *                                                                             *
 *    OUTPUTS:       0 || 1                                                    *
 *                                                                             *
 *    FUNCTIONS:                                                               *
 *                                                                             *
 *    VARIABLES:                                                               *
 *                                                                             *
 *    MODIFICATION                                                             *
 *    HISTORY:                                                                 *
 *                                                                             *
 *    08.05.2006    Marlo Maddox - Initial Development Started                 *
 *                                                                             *
 ******************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "list_structure_toolkit.h"

#define DEBUG_FLAG 0

int is_var_in_memory(char *variable_name)
{
    /** we have a default structure defined in the list_structure_toolkit,
     * lets search it for variable_name **/

    return node_exists( &loaded_variable_listPtr, variable_name);
}

