/*******************************************************************************
 *                                                                             *
 *       NAME:       time_interpolate.c                                        *
 *                                                                             *
 *       VERSION:    3.3                                                       *
 *                                                                             *
 *       AUTHOR:     Marlo M. H. Maddox                                        *
 *                   NASA-GSFC-AETD-ISD-ADMA(587)-CCMC                         *
 *                   Marlo.Maddox@nasa.gov                                     *
 *                   (301) 286 - 5202                                          *
 *                                                                             *
 *       PURPOSE:    interpolation wrapper to interpolate data at a given point*
 *                   for a given time.  Uses a tree data structure to find     *
 *                   files whose times are <= & >= a given target time. Values *
 *                   for know times are extracted from data and target time    *
 *                   values are interpolated using knwon values.               *
 *                                                                             *
 *       INPUTS:     variable_name, target_time, x, y, z                       *
 *                                                                             *
 *       OUTPUTS:    ...                                                       *
 *                                                                             *
 *       FUNCTIONS:  ...                                                       *
 *                                                                             *
 *       VARIABLES:  ...                                                       *
 *                                                                             *
 *                                                                             *
 *       MODIFICATION                                                          *
 *       HISTORY:                                                              *
 *                                                                             *
 *       11.08.2004  Maddox, Marlo                                             *
 *                   initial release                                           *
 *                                                                             *
 *       03.03.2005  Maddox, Marlo                                             *
 *                   UCLA-GGCM support added to code.  Model name is           *
 *                   extracted on first call for correct interp call           *
 *                                                                             *
 *       04.26.2005  Maddox, Marlo                                             *
 *                   in corral time step function, left/right found            *
 *                   flag variable                                             *
 *                                                                             *
 ******************************************************************************/

#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "tree_structure_toolkit.h"
#include "cdf.h"

#define DEBUG_FLAG 0

float time_interpolate(
      char *variable_name,
      double time,
      float x,
      float y,
      float z)
{

   extern TreeNodePtr rootPtr;

   extern void open_cdf(
         char *,
         int number_of_arguments,
         ...);
   extern void close_cdf(
         void);
   void corral_time_step(
         TreeNodePtr,
         double,
         char *,
         char *,
         double *,
         double *,
         double *);
   extern float interpolate_batsrus_cdf(
         char *,
         float,
         float,
         float,
         int,
         int,
         ...);
   extern float interpolate_ucla_ggcm_cdf(
         char *,
         float,
         float,
         float,
         int,
         int,
         ...);
   extern void in_order(
         TreeNodePtr);
   extern int StatusHandler(
         CDFstatus);

   /****** variables for corraling node < & > given time step ******/
   char less_than_file_name[MAX_STRING_LEN]; /* MAX_STRING_LEN is defined in tree_structure_toolkit.h */
   char greater_than_file_name[MAX_STRING_LEN];
   double time_0, time_1, delta_t;

   /***** variables for linear interpolation *****/

   double variable_delta;
   double variable_1, variable_0;

   float time_interpolated_value;

   static int call_count = 0;

   char *batsrus = "batsrus";
   char *ucla_ggcm = "ucla_ggcm";

   int model_name_attr_num;
   char *model_name_attr = "model_name";
   static char model_name[50];

   int debug_flag= DEBUG_FLAG;

   CDFstatus status;

   /********** debugging output
    printf("DEBUG\trootPtr->file_name = %s\nrootPtr->epoch_time = %f\n", rootPtr->file_name, rootPtr->epoch_time );

    printf("DEBUG\t[2]\taddress of rootPtr = %d\n", &rootPtr );
    printf("DEBUG\t[2]\taddress of rootPtr->LPtr = %d\n", &rootPtr->LPtr );
    printf("DEBUG\t[2]\taddress of rootPtr->RPtr = %d\n", &rootPtr->RPtr );

    pre_order( rootPtr );

    ****************************/

   /*printf("%f\t%f %f %f\n", local_time, local_x, local_y, local_z );*/
   /*printf("%f\t%f %f %f\n", time, x, y, z );*/

   /******* until I can make the corral out of bounds proof, lets intercept stray values here ****/

   if (time < stored_start_time)
   {
      time = stored_start_time + 1000;
      if (debug_flag)
      {
         printf(
               "!!WARNING: current time %f is less than stored simulation "
               "start_time %f.  Replacing non-existing time with start_time...\n",
               time,
               stored_start_time);
      }
   }
   else if (time > stored_end_time)
   {
      time = stored_end_time - 1000;
      if (debug_flag)
      {
         printf(
               "!!WARNING: current time %f is greater than stored simulation "
               "end_time %f.  Replacing non-existing time with end_time...\n",
               time,
               stored_end_time);
      }
   }

   corral_time_step(
         rootPtr,
         time,
         less_than_file_name,
         greater_than_file_name,
         &time_0,
         &time_1,
         &delta_t);

   /************** DEBUGING OUTPUT
    printf("DEBUG\tPOST corall diagnostics\n");
    printf("left_file_name & time = %s\t%f\n", less_than_file_name, time_0 );
    printf("greater_file_name & time = %s\t%f\n", greater_than_file_name, time_1 );
    printf("with delta_t = %f\n", delta_t );
    ********************************/

   open_cdf(
         less_than_file_name,
         0);

   /**** get model name on fist call to function & assume homogenous data for remainder of execution ****/

   if ( !call_count)
   {

      status
            = CDFlib( GET_, ATTR_NUMBER_, model_name_attr, &model_name_attr_num, NULL_);
      if (status != CDF_OK)
         StatusHandler(status);

      /********* get value for model_name_attr **********/
      status = CDFlib(
      SELECT_, ATTR_, model_name_attr_num,
      SELECT_, gENTRY_, 0,
      GET_, gENTRY_DATA_, &model_name,
      NULL_);
      if (status != CDF_OK)
         StatusHandler(status);
   }

   if ( !strcmp(
         batsrus,
         model_name) )
   {
      variable_0 = interpolate_batsrus_cdf(
            variable_name,
            x,
            y,
            z,
            0,
            0);
   }
   else if ( !strcmp(
         ucla_ggcm,
         model_name) )
   {
      variable_0 = interpolate_ucla_ggcm_cdf(
            variable_name,
            x,
            y,
            z,
            0,
            0);
   }

   close_cdf();

   open_cdf(
         greater_than_file_name,
         0);

   if ( !strcmp(
         batsrus,
         model_name) )
   {
      variable_1 = interpolate_batsrus_cdf(
            variable_name,
            x,
            y,
            z,
            0,
            0);
   }
   else if ( !strcmp(
         ucla_ggcm,
         model_name) )
   {
      variable_1 = interpolate_ucla_ggcm_cdf(
            variable_name,
            x,
            y,
            z,
            0,
            0);
   }

   close_cdf();

   variable_delta = variable_1 - variable_0;

   time_interpolated_value = (variable_delta / delta_t ) * (time - time_1 )
         + variable_1;

   call_count++;

   return (time_interpolated_value );

}

/****** corral_time_step using recursuve function find tree nodes with < > times for any given time given time *******/

void corral_time_step(
      TreeNodePtr tPtr,
      double time,
      char f0[],
      char f1[],
      double *t0,
      double *t1,
      double *delta_t)
{

   void corral_time_step(
         TreeNodePtr,
         double,
         char [],
         char [],
         double *,
         double *,
         double *);

   /*extern TreeNodePtr rootPtr;*/

   static int found_left_flag = 0, found_right_flag = 0; /** lets retain values during a recursion cycle, reset after entire tree has been traversed **/

   TreeNodePtr current_node, left_node, right_node;

   double local_time = time;

   current_node = tPtr;

   /*if( tPtr != NULL )*/

   if (tPtr != NULL)
   {

      current_node = tPtr;
      left_node = tPtr->LPtr;
      right_node = tPtr->RPtr;

      if (current_node->epoch_time <= local_time)
      {
         t0[0] = current_node->epoch_time;
         strcpy(
               f0,
               current_node->file_name);
         f0[ strlen( current_node->file_name) ] ='\0';
         found_left_flag++;

         if (tPtr->RPtr != NULL)
         {
            corral_time_step(
                  tPtr->RPtr,
                  local_time,
                  f0,
                  f1,
                  t0,
                  t1,
                  delta_t);
         }

      }
      else /* current nodes time must be >= time */
      {
         t1[0] = current_node->epoch_time;
         strcpy(
               f1,
               current_node->file_name);
         f1[strlen( current_node->file_name)]='\0';
         found_right_flag++;

         if (tPtr->LPtr != NULL)
         {
            corral_time_step(
                  tPtr->LPtr,
                  local_time,
                  f0,
                  f1,
                  t0,
                  t1,
                  delta_t);
         }

      }
   }
   /*
    if( found_left_flag == 0 )
    {
    t0[0] = t1[0];
    strcpy( f0, f1 );

    printf("DEBUG\tCOULDN't find time = %f\t\tso no left file found, using %s\n", time, f0 );

    }
    if( found_right_flag == 0 )
    {
    t1[0] = t0[0];
    strcpy( f1, f0 );

    printf("DEBUG\tCOULDN't find time = %f\t\tso no right file found, using %s\n", time, f1 );

    }
    */

   delta_t[0] = t1[0] - t0[0];

   return;

}

