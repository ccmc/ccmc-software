/*******************************************************************************
 *                                                                             *
 *    NAME:          strsplit.c                                                *
 *                                                                             *
 *    VERSION:       0.1                                                       *
 *                                                                             *
 *    AUTHOR:        Lutz Rastaetter                                           *
 *                   CUA/CCMC/NASA-GSFC (612.3)                                *
 *                   lr@waipio.gsfc.nasa.gov                                   *
 *                   (301) 286 - 1085                                          *
 *                                                                             *
 *    PURPOSE:       String Spliting routine for specified string and delimiter*
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


/****** strsplit - privided by Lutz Rasaeatter *****/

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "strsplit.h"

/* #define DEBUG  */
/*
 typedef struct {
 long len;
 char **sarr;
 } strarr;
 */

int free_strarr(strarr *split_string)
{
   int i;

   if (split_string == NULL)
      return (0);

   for (i=0; i<(*split_string).len; i++)
   {
      if ((*split_string).sarr[i] != NULL)
         free((*split_string).sarr[i]);
   }
   if ((*split_string).sarr != NULL)
   {
      free((*split_string).sarr);
      (*split_string).sarr=NULL;
   }
   (*split_string).len=0;
   return (0);
}

int strsplit(char *string, strarr *split_string, char *separator)
{
   char *str_ptr, *str_copy;
   long split_count=0, splitlen, i;

   /* temporary string */
   str_copy=(char*)malloc(strlen(string)+1);

   /* need to copy string to preserve original */
   strcpy(str_copy, string);
#ifdef DEBUG
   fprintf(stderr,"Strsplit: to split: %s  Separator: |%s|\n",string,separator);
#endif
   /* first pass - determine number of tokens */
   str_ptr=strtok(str_copy, separator);

   while (str_ptr != NULL)
   {
#ifdef DEBUG
      fprintf(stderr,"Strsplit: string token: %s\n",str_ptr);
#endif
      split_count++;
      str_ptr=strtok(NULL,separator);
   }
#ifdef DEBUG
   fprintf(stderr,"Strsplit: split count: %ld\n",split_count);
#endif
   /* clean out old array if one was handed to strsplit */
   if ((split_string[0].sarr != NULL))
   {
      for (i=0; i<split_string[0].len; i++)
      {
         if (split_string[0].sarr[i] == NULL)
         {
            break;
         }
         free(split_string[0].sarr[i]);
      }
      free(split_string[0].sarr);
      split_string[0].len=0;
   }
   /* now we have the structure: long int number zero and a NULL pointer */
#ifdef DEBUG
   fprintf(stderr,"Strsplit: arrlen: %ld\n",split_count);
#endif
   /* fill data into string array */
   split_string[0].len=split_count;
   split_string[0].sarr=(char**)malloc(split_count*sizeof(char*));

   /* 2nd pass - extract substrings */
   strcpy(str_copy, string);
   str_ptr=strtok(str_copy, separator);
   for (i=0; i<split_count; i++)
   {
#ifdef DEBUG
      fprintf(stderr,"Strsplit fill data: %ld %s\n",i,str_ptr);
#endif
      splitlen=strlen(str_ptr);
#ifdef DEBUG
      fprintf(stderr," Token length: %ld\n",splitlen);
#endif
      split_string[0].sarr[i]=(char*)malloc(splitlen+1);
      strcpy(split_string[0].sarr[i], str_ptr);
      str_ptr=strtok(NULL,separator);
   }
#ifdef DEBUG
   fprintf(stderr,"Strsplit: length: %ld  last string of array: %s\n",
         split_string[0].len,split_string[0].sarr[split_string[0].len-1]);
#endif
   free(str_copy);
   return (0);
}

