/*******************************************************************************
 *                                                                             *
 *    NAME:          endian_flip.c                                                *
 *                                                                             *
 *    VERSION:       0.1                                                       *
 *                                                                             *
 *    AUTHOR:        Lutz Rastaetter                                           *
 *                   CUA/CCMC/NASA-GSFC (612.3)                                *
 *                   lr@waipio.gsfc.nasa.gov                                   *
 *                   (301) 286 - 1085                                          *
 *                                                                             *
 *    PURPOSE:       Routine that converts to/from big <--> little endian      *
 *                                                                             *
 *    INPUTS:        *                                                         *
 *                                                                             *
 *    OUTPUTS:       *                                                         *
 *                                                                             *
 *    FUNCTIONS:     *                                                         *
 *                                                                             *
 *    VARIABLES:     *                                                         *
 *                                                                             *
 *    MODIFICATION                                                             *
 *    HISTORY:                                                                 *
 *                                                                             *
 *    ??.??.????     Rastaetter, Lutz                                          *
 *                   initial development started                               *
 *                                                                             *
 ******************************************************************************/

int endian_flip(char *array, long unit_len, long n_units)
{

   long i, ii, j, ij, ij1, i_unit, i_char;

   /* work through the array elements */
   for (i=0L, ii=0L; i<n_units; i++, ii+=unit_len)
   {
      /*ii=i*unit_len;*//*------------^*/

      /* swap bytes within the element */
      for (j=0L, ij=ii, ij1=ii+unit_len-1; j<unit_len/2L; j++, ij++, ij1--)
      {
         char cdum;
         ij = ii+j;
         ij1 = ii+unit_len-j-1;
         cdum =array[ij1];
         array[ij1]=array[ij];
         array[ij]=cdum;
      }
   }

   return (0);

}
