/*******************************************************************************
*                                                                              *
*     NAME:          list_structure_toolkit.h                                  *
*                                                                              *
*     VERSION:       1.0                                                       *
*                                                                              *
*     AUTHOR:        Marlo M. H. Maddox                                        *
*                    NASA-GSFC-AETD-ISD-ADMA(587)-CCMC                         *
*                    Marlo.Maddox@nasa.gov                                     *
*                    (301) 286 - 5202                                          *
*                                                                              *
*     PURPOSE:       Definitions for generic tree structure tools & routines   *
*                                                                              *
*     INPUTS:                                                                  *
*                                                                              *
*     OUTPUTS:       used by list_structure_toolkit.c ...                      *
*                                                                              *
*     FUNCTIONS:                                                               *
*                                                                              *
*     VARIABLES:                                                               *
*                                                                              *
*                                                                              *
*     MODIFICATION                                                             *
*     HISTORY:                                                                 *
*                                                                              *
*     10.03.2006     Maddox, Marlo                                             *
*                    Initial dev                                               *
*                                                                              *
*******************************************************************************/

#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#define MAX_STRING_LEN 500

/** generic structure to store info in a tree  **/

struct list_struct
{
    char string1[MAX_STRING_LEN];
    struct list_struct *NextPtr;
};

/* typedef struct cdf_file_struct *ListNode; only was reserving space for pointer */

typedef struct list_struct ListNode;
typedef struct list_struct *ListNodePtr;

/***** tree structure toolkit functions ******/

void insert_list_node( ListNodePtr *, char * );
void print_list_pre_order( ListNodePtr );
void print_list_post_order( ListNodePtr );
int is_list_empty( ListNodePtr );
void delete_list_node(ListNodePtr *, char * );
int node_exists( ListNodePtr *, char * );

void destroy_list( ListNodePtr *);

ListNodePtr loaded_variable_listPtr;
ListNodePtr open_filePtr;


