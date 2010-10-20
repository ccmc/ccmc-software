/*******************************************************************************
 *                                                                             *
 *       NAME:          hunt.c                                                 *
 *                                                                             *
 *       VERSION:       1.0                                                    *
 *                                                                             *
 *       AUTHOR:        Marlo M. H. Maddox                                     *
 *                      NASA-GSFC-AETD-ISD-ADMA(587)-CCMC                      *
 *                      Marlo.Maddox@nasa.gov                                  *
 *                      (301) 286 - 5202                                       *
 *                                                                             *
 *       PURPOSE:       Numerical Recipes style hunt function.  Finds the      *
 *                      (closest) index  of a value in the supplied array.     *
 *                                                                             *
 *       INPUTS:        (1) array to search (2) array size (3) search value (4)*
 *                       return index val                                      *
 *                                                                             *
 *       OUTPUTS:       array index for found/closest value                    *
 *                                                                             *
 *       FUNCTIONS:                                                            *
 *                                                                             *
 *       VARIABLES:                                                            *
 *                                                                             *
 *                                                                             *
 *       MODIFICATION                                                          *
 *       HISTORY:                                                              *
 *                                                                             *
 *                                                                             *
 *       ??.??.????     Rastaetter, Lutz                                       *
 *                      Initial development started                            *
 *                                                                             *
 *       07.28.2005     Maddox, Marlo                                          *
 *                      Hunt should return -1 for index jlo if x < xx[0]       *
 *                      meaning x is in acsending order or if x > xx[0]        *
 *                      meaning x is in descending order                       *
 ******************************************************************************/

/*** add official header sooner or later            ***/
/*** Hunt Routine Supplied by Dr. Lutz Rastaetter   ***/
/*** 7/28/2005 Update - Marlo Maddox - hunt should return -1 for index jlo if x < xx[0] meaning x is in acsending order or if
 x > xx[0] meaning x is in descending order

 ***/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void hunt(
      float *xx,
      int n,
      float x,
      int *jlo)
{
   int jm, jhi, inc, ascnd, n1;

   n1=n-1;
   ascnd=(xx[n1] >= xx[0]);

   /*** section added 7/28/2005 per Lutz's request ***/
   if (ascnd)
   {
      if (x < xx[0])
      {
         *jlo = -1;
         return;
      }
   }
   else
   {
      if (x > xx[0])
      {
         *jlo = -1;
         return;
      }
   }
   /*** end added section ***/

   if (*jlo < 0 || *jlo >= n)
   {
      /* no useful input -> bisection */
      *jlo=0;
      jhi= n;
   }
   else
   {
      inc=1; /* hunting increment */

      if ( (x >= xx[*jlo] ) == ascnd) /* hunt up */
      {
         if (*jlo == n1)
         {
            return;
         }
         jhi = (*jlo)+1;

         while ( (x > xx[jhi] ) == ascnd) /* not done yet */
         {
            inc+=inc; /* double increment */
            jhi=(*jlo)+inc;

            if (jhi >n1)
            {
               jhi = n; /* Done hunting since off end of table */
               break;
            } /* try again */
         } /* done hunting, value bracketed */
      }
      else
      {
         if (*jlo == 0)
         { /* hunt down */
            return;
         }

         jhi = (*jlo)--;

         while ( (x < xx[*jlo] ) == ascnd)
         { /* not done yet */
            jhi=(*jlo);
            inc*=2;

            if (inc >= jhi) /* off end of table */
            {
               *jlo=0;
               break;
            }
            else
            {
               *jlo = jhi-inc;
            }
         } /* try again */
      } /* hunt done */
   } /* hunt done, begin final bisection */

   while (jhi-(*jlo) != 1)
   {
      jm=(jhi+(*jlo))/2;

      if ( (x > xx[jm] ) == ascnd)
      {
         *jlo = jm;
      }
      else
      {
         jhi = jm;
      }
   }
}

