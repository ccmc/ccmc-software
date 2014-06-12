/*******************************************************************************
 *                                                                             *
 *       NAME:          tree_structure_toolkit.h                               *
 *                                                                             *
 *       VERSION:       1.0                                                    *
 *                                                                             *
 *       AUTHOR:        Marlo M. H. Maddox                                     *
 *                      NASA-GSFC-AETD-ISD-ADMA(587)-CCMC                      *
 *                      Marlo.Maddox@nasa.gov                                  *
 *                      (301) 286 - 5202                                       *
 *                                                                             *
 *       PURPOSE:       Definitions for tree structure tools & routines        *
 *                                                                             *
 *       INPUTS:                                                               *
 *                                                                             *
 *       OUTPUTS:       used by tree_structure_toolkit.c, init_time.c,         *
 *                      interpolate.c                                          *
 *                                                                             *
 *       FUNCTIONS:                                                            *
 *                                                                             *
 *        VARIABLES:                                                           *
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
#include <stdlib.h>

#define MAX_STRING_LEN 500

/** structure for each cdf file to be placed as a node on tree **/

struct cdf_file_struct
{
   char file_name[MAX_STRING_LEN];
   /*char *file_name;*/
   double epoch_time;
   struct cdf_file_struct *LPtr;
   struct cdf_file_struct *RPtr;

};

/* typedef struct cdf_file_struct *TreeNode; only was reserving space for pointer */
typedef struct cdf_file_struct TreeNode;
typedef struct cdf_file_struct *TreeNodePtr;

/***** tree structure toolkit functions ******/

void insert_node(
      TreeNodePtr *,
      char *,
      double);
void in_order(
      TreeNodePtr);
void pre_order(
      TreeNodePtr);
void post_order(
      TreeNodePtr);
int isEmpty(
      TreeNodePtr);
double findmin(
      TreeNodePtr);
double findmax(
      TreeNodePtr);
void delete_node(
      TreeNodePtr *,
      double);

TreeNodePtr destroy(
      TreeNodePtr *);

/*TreeNodePtr rootPt; declare in init_time*/

/**** tree pointer ******/
TreeNodePtr rootPtr;

/*** lets keep a tab on the start_time & end_time for each simulation ***/

double stored_start_time;
double stored_end_time;

