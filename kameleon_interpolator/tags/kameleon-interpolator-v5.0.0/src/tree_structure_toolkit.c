/*******************************************************************************
 *                                                                             *
 *       NAME:          tree_structure_toolkit.c                               *
 *                                                                             *
 *       VERSION:       1.0                                                    *
 *                                                                             *
 *       AUTHOR:        Marlo M. H. Maddox                                     *
 *                      NASA-GSFC-AETD-ISD-ADMA(587)-CCMC                      *
 *                      Marlo.Maddox@nasa.gov                                  *
 *                      (301) 286 - 5202                                       *
 *                                                                             *
 *       PURPOSE:       Routines for creating, accessing, and updating tree    *
 *                      data structure containing file_name & time information *
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
 *       11.08.2004     Maddox, Marlo                                          *
 *                      Initial release                                        *
 *                                                                             *
 ******************************************************************************/

#include <string.h>
#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <dirent.h>
#include <sys/param.h>
#include "cdf.h"
#include "tree_structure_toolkit.h"

#define MAX_STRING_LEN 500

/******************** INSERT TREE NODE FUNCTION *******************************/
void insert_node(
      TreeNodePtr *tPtr,
      char *current_file_name,
      double current_epoch_time)
{

   if ( *tPtr == NULL)
   {

      *tPtr = ( struct cdf_file_struct * ) malloc(sizeof(TreeNode));

      if (*tPtr!=NULL)
      {

         strcpy(
               (*tPtr)->file_name,
               current_file_name);

         (*tPtr)->epoch_time = current_epoch_time;
         (*tPtr)->LPtr = NULL;
         (*tPtr)->RPtr = NULL;

      }
      else
      {
         printf(
               "ERROR! %s\t%f NOT INSERTED... ZERO MEMORY!\n",
               current_file_name,
               current_epoch_time);
      }
   }
   else if (current_epoch_time<(*tPtr)->epoch_time)
   {

      insert_node(
            &((*tPtr)->LPtr),
            current_file_name,
            current_epoch_time);
   }
   else if (current_epoch_time > (*tPtr)->epoch_time)
   {

      insert_node(
            &((*tPtr)->RPtr),
            current_file_name,
            current_epoch_time);
   }
   else
   {
      printf(
            "epoch time %f for %s is a DUPLICATE of existing tree node...\n",
            current_epoch_time,
            current_file_name);
   }

   return;

}

/***************************** print tree using in-order notation *************/

void in_order(
      TreeNodePtr tPtr)
{
   if (tPtr!=NULL)
   {

      /*printf("DEBUG\t&tPtr->LPtr = %d\n", tPtr->LPtr );*/

      in_order(tPtr->LPtr);
      printf(
            "%s\t%f\n",
            tPtr->file_name,
            tPtr-> epoch_time);

      /*printf("DEBUG\t&tPtr->RPtr = %d\n", tPtr->RPtr );*/

      in_order(tPtr->RPtr);
   }
}

/***************************** print tree using pre-order notation ************/

void pre_order(
      TreeNodePtr tPtr)
{
   if (tPtr!=NULL)
   {
      printf(
            "%s\t%f\n",
            tPtr->file_name,
            tPtr-> epoch_time);
      pre_order(tPtr->LPtr);
      pre_order(tPtr->RPtr);
   }
}

/***************************** print tree using post-order notation ***********/

void post_order(
      TreeNodePtr tPtr)
{
   if (tPtr!=NULL)
   {
      post_order(tPtr->LPtr);
      post_order(tPtr->RPtr);
      printf(
            "%s\t%f\n",
            tPtr->file_name,
            tPtr-> epoch_time);
   }
}

/*************************** is tree empty function ***************************/

int isEmpty(
      TreeNodePtr tPtr)
{
   return tPtr==NULL;
}

/************************** DESTROY tree function  ****************************/

TreeNodePtr destroy(
      TreeNodePtr *tPtr)
{

   if (*tPtr!=NULL)
   {
      (*tPtr)->LPtr=NULL;
      (*tPtr)->RPtr=NULL;
      /*(*tPtr)->epoch_time=NULL;*/
      free(*tPtr);
      delete_node(
            &*tPtr,
            ( *tPtr ) -> epoch_time);
   }
   return *tPtr;
}

/********************** find min value function *******************************/

double findmin(
      TreeNodePtr tPtr)
{

   static double min_epoch_time;

   if (tPtr != NULL)
   {

      min_epoch_time = tPtr->epoch_time;

      if (tPtr->LPtr != NULL)
      {
         findmin(tPtr->LPtr);
      }

   }

   return min_epoch_time;

}

/*********************************** find max value function ******************/

double findmax(
      TreeNodePtr tPtr)
{

   static double max_epoch_time;

   if (tPtr != NULL)
   {

      max_epoch_time = tPtr->epoch_time;

      if (tPtr->RPtr != NULL)
      {
         findmax(tPtr->RPtr);
      }

   }

   return max_epoch_time;

}

/******************************** delete node from tree ***********************/
void delete_node(
      TreeNodePtr *tPtr,
      double current_epoch_time)
{

   TreeNodePtr p, q;
   q=p=*tPtr;
   if (p!=NULL)
   {
      if (p -> epoch_time > current_epoch_time)
      {
         delete_node(
               &(*tPtr)->LPtr,
               current_epoch_time);
      }
      else if (p->epoch_time < current_epoch_time)
      {
         delete_node(
               &(*tPtr) -> RPtr,
               current_epoch_time);
      }
      else if (p->epoch_time==current_epoch_time)
      {
         if (p->LPtr==NULL&&p->RPtr==NULL)
         {
            (*tPtr)=NULL;
            free(p);
         }
         else if (p->LPtr==NULL)
         {
            (*tPtr)=p->RPtr;
            free(p);
         }
         else if (p->RPtr==NULL)
         {
            (*tPtr)=p->LPtr;
            free(p);
         }
         else
         {
            (*tPtr)=p=p->RPtr;

            while (p->LPtr!=NULL)
               p=p->LPtr;
            p->LPtr=q->LPtr;
            free(q);
         }
      }
      else if (p->epoch_time!=current_epoch_time)
         printf("NO SUCH VALUE IN CURRENT TREE!\n\n");
   }
}

