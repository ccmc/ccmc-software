/*******************************************************************************
 *                                                                             *
 *    NAME:          linear_minmax_serch.c                                     *
 *                                                                             *
 *    VERSION:       2.3                                                       *
 *                                                                             *
 *    AUTHOR:        Marlo M. H. Maddox                                        *
 *                   NASA-GSFC-AETD-ISD-ADMA(587)                              *
 *                   Marlo.Maddox@nasa.gov                                     *
 *                   (301) 286 - 5202                                          *
 *                                                                             *
 *    PURPOSE:       Stand alone array search function that searches for min & *
 *                   max values in an array                                    *
 *                                                                             *
 *    INPUTS:        pointer to array that is to be searched & address of      *
 *                   variables to store min/max values                         *
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
 *    2004 September 9th      Marlo maddox                                     *
 *                            Initial code development                         *
 *                                                                             *
 *    2006 October 19th       Marlo Maddox                                     *
 *                            Made a duplicate function that searches array of *
 *                            type int we'll call it                           *
 *                            linear_min_max_search_int()                      *
 ******************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

int linear_minmax_search(
      float *array_to_search,
      int number_of_elements,
      float *min_value,
      float *max_value)
{

   int i;

   if (array_to_search == NULL){return;}

   *min_value = array_to_search[0];
   *max_value = array_to_search[0];

   for (i = 0; i < number_of_elements; i++)
   {
      if (array_to_search[i] <= *min_value)
      {
         *min_value = array_to_search[i];
      }
      if (array_to_search[i] >= *max_value)
      {
         *max_value = array_to_search[i];
      }
   }

   return 1;

}

int linear_minmax_search_int(
      int *array_to_search,
      int number_of_elements,
      int *min_value,
      int *max_value)
{

   int i;

   *min_value = array_to_search[0];
   *max_value = array_to_search[0];

   for (i = 0; i < number_of_elements; i++)
   {
      if (array_to_search[i] <= *min_value)
      {
         *min_value = array_to_search[i];
      }
      if (array_to_search[i] >= *max_value)
      {
         *max_value = array_to_search[i];
      }
   }

   return 1;

}
