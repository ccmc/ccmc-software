/*******************************************************************************
 *                                                                             *
 *       NAME:          list_structure_toolkit.c                               *
 *                                                                             *
 *       VERSION:       1.0                                                    *
 *                                                                             *
 *       AUTHOR:        Marlo M. H. Maddox                                     *
 *                      NASA-GSFC-AETD-ISD-ADMA(587)-CCMC                      *
 *                      Marlo.Maddox@nasa.gov                                  *
 *                      (301) 286 - 5202                                       *
 *                                                                             *
 *       PURPOSE:       Routines for creating, accessing, and updating tree    *
 *                      data structure containing generic information          *
 *                                                                             *
 *       INPUTS:        ...                                                    *
 *                                                                             *
 *       OUTPUTS:       ...                                                    *
 *                                                                             *
 *       FUNCTIONS:     ...                                                    *
 *                                                                             *
 *       VARIABLES:     ...                                                    *
 *                                                                             *
 *                                                                             *
 *       MODIFICATION                                                          *
 *       HISTORY:                                                              *
 *                                                                             *
 *       12.03.2006     Maddox, Marlo                                          *
 *                      Initial Development                                    *
 ******************************************************************************/

#include <string.h>
#include <stdio.h>
#include <math.h>
#include <stdlib.h>

#include "list_structure_toolkit.h"

#define DEBUG_FLAG 0

/******************** INSERT TREE NODE FUNCTION **************************/
void insert_list_node(
      ListNodePtr *tPtr,
      char *current_string)
{

   /** lets check the length of the current_string **/

   if (strlen(current_string) > MAX_STRING_LEN)
   {
      fprintf(stderr, "\n%s\tWARNING: Length of %s > %d !!! Only storing first "
            "%d characters in list...\n", __FILE__, current_string, MAX_STRING_LEN, MAX_STRING_LEN - 1);

   }

   if ( *tPtr == NULL)
   {

      *tPtr = ( struct list_struct * ) malloc(sizeof(ListNode));

      if (*tPtr!=NULL)
      {

         strncpy(
               (*tPtr)->string1,
               current_string,
               MAX_STRING_LEN -1);

         (*tPtr)->NextPtr = NULL;

      }
      else
      {
         printf(
               "%s\tERROR: %s NOT INSERTED... ZERO MEMORY!\n",
               __FILE__, current_string );
            }
         }
         else if( strncmp( current_string, (*tPtr)->string1, MAX_STRING_LEN -1 ) !=0 )
         {
            insert_list_node( &((*tPtr)->NextPtr), current_string );
         }
         else
         {
            if( DEBUG_FLAG )
            {  printf("%s is a DUPLICATE of existing list node...\n", current_string );}
         }

         return;

      }

      /***************************** print tree using pre-order notation *******************/

void print_list_pre_order(
      ListNodePtr tPtr)
{
   if (tPtr!=NULL)
   {
      fprintf(stderr, "%s\n", tPtr->string1);
      print_list_pre_order(tPtr->NextPtr);
   }
}

/***************************** print tree using post-order notation *******************/

void print_list_post_order(
      ListNodePtr tPtr)
{
   if (tPtr!=NULL)
   {
      print_list_post_order(tPtr->NextPtr);
      fprintf(stderr,"%s\n", tPtr->string1);
   }
}

/*************************** is tree empty function *********************************/

int is_list_empty(
      ListNodePtr tPtr)
{
   return tPtr==NULL;
}

/************************** DESTROY tree function  *******************************/

void destroy_list(
      ListNodePtr *tPtr)
{

   ListNodePtr p, tmp;

   if ( *tPtr == NULL)
   {
      /* there is nothing to destroy */

      return;
   }

   p = (*tPtr)->NextPtr;

   (*tPtr)->NextPtr = NULL;

   while (p != NULL)
   {
      tmp = p->NextPtr;
      free(p);
      p =tmp;
   }

   delete_list_node(
         &*tPtr,
         ( *tPtr ) -> string1);

   return;

}

/************************************ delete node from tree ***********************/
void delete_list_node(
      ListNodePtr *tPtr,
      char *current_string)
{

   ListNodePtr p, q;
   q=p=*tPtr;

   if (p!=NULL)
   {
      if (strncmp(
            current_string,
            (*tPtr)->string1,
            MAX_STRING_LEN -1) !=0)
      {
         delete_list_node(
               &(*tPtr)->NextPtr,
               current_string);
      }
      else if (strncmp(
            current_string,
            (*tPtr)->string1,
            MAX_STRING_LEN -1) == 0)
      {
         if (p->NextPtr==NULL)
         {
            (*tPtr)=NULL;
            free(p);
         }
         else
         {
            (*tPtr)=p=p->NextPtr;
            /*while(p->NextPtr!=NULL) p=p->NextPtr;*//*p->NextPtr=q->NextPtr;*/free(p);
         }
      }

   }
}

/******************** Search for existing TREE NODE FUNCTION **************************/
int node_exists(
      ListNodePtr *tPtr,
      char *current_string)
{

   static int found_flag = 0;

   if ( *tPtr == NULL)
   {

      if ( DEBUG_FLAG)
      {
         fprintf(stderr, "%s\tINFO: %s does not exists in tree.\n", __FILE__, current_string);
      }

      found_flag = 0;
      return found_flag;

   }
   else if (strncmp(
         current_string,
         (*tPtr)->string1,
         MAX_STRING_LEN -1) !=0)
   {
      node_exists(
            &((*tPtr)->NextPtr),
            current_string);
   }
   else if (strncmp(
         current_string,
         (*tPtr)->string1,
         MAX_STRING_LEN -1) ==0)
   {
      if ( DEBUG_FLAG)
      {
         fprintf(stderr, "%s\tINFO: %s FOUND in tree.\n", __FILE__, current_string);
      }
      found_flag = 1;
      return found_flag;

   }

   return found_flag;

}

