/*******************************************************************************
 *                                                                             *
 *    NAME:          close_cdf.c                                               *
 *                                                                             *
 *    VERSION:       1.0                                                       *
 *                                                                             *
 *    AUTHOR:        Marlo M. H. Maddox                                        *
 *                   NASA-GSFC-AETD-ISD-ADMA(587)-CCMC                         *
 *                   Marlo.Maddox@nasa.gov                                     *
 *                   (301) 286 - 5202                                          *
 *                                                                             *
 *    PURPOSE:       Calls cleanup routine for specific model output type.     *
 *                                                                             *
 *    INPUTS:        *                                                         *
 *                                                                             *
 *    OUTPUTS:       *                                                         *
 *                                                                             *
 *    FUNCTIONS:     *                                                         *
 *                                                                             *
 *    VARIABLES:     *                                                         *
 *                                                                             *
 *                                                                             *
 *    MODIFICATION                                                             *
 *    HISTORY:                                                                 *
 *                                                                             *
 *    10.4.2006      Maddox, Marlo     Destroy our new open_file list structure*
 *                                     pointed to by open_filePtr as well as   *
 *                                     the loaded_variable_listPtr structure   *
 *                                     that holds all of the loaded variables  *
 *                                                                             *
 *    10.11.2006     Maddox, Marlo     Adding close logic for open_ggcm name   *
 *                                                                             *
 *    02.29.2008     Maddox, Marlo     Adding close logic for MAS              *
 *                                                                             *
 ******************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <math.h>
#include <stdarg.h>
#include "cdf.h"
#include "cdf_interface_variables.h"
#include "list_structure_toolkit.h"

#define DEBUG_FLAG 0

void close_cdf(void)
{

   CDFstatus status;
   extern void destroy_list(ListNodePtr *);

   char *batsrus = "batsrus";
   char *ucla_ggcm = "ucla_ggcm";
   char *open_ggcm = "open_ggcm";
   char *ctip = "ctip";
   char *enlil = "enlil";
   char *mas = "mas";
/*pmn    char *magic = "magic"; */
   char *magic = "magnetogram";
   char *adapt3d = "ADAPT3D";

   int model_name_attr_num;
   char *model_name_attr = "model_name";
   static char model_name[50];
   int gattr_str_len[1];

   extern void close_batsrus_cdf(void);
   extern void close_ucla_ggcm_cdf(void);
   extern void close_ctip_cdf(void);
   extern void close_enlil_cdf(void);
   extern void close_mas_cdf(void);
//   extern void close_magic_cdf(void);
   extern void close_adapt3d_cdf(void);

   if( DEBUG_FLAG)
   {
      printf("***inside close_cdf()\n");
   }

   status
         = CDFlib( GET_, ATTR_NUMBER_, model_name_attr, &model_name_attr_num, NULL_);

   if (status != CDF_OK)
   {
      StatusHandler(status);

      /*** there doesn't seem to be a valid cdf currently open.  Lets Just return
       * control back to the calling program before we do double_free's or something **/

      return;

   }

   /********* get value for model_name_attr **********/
   status = CDFlib(
   SELECT_, ATTR_, model_name_attr_num,
   SELECT_, gENTRY_, 0,
   GET_, gENTRY_DATA_, &model_name,
   GET_, gENTRY_NUMELEMS_, gattr_str_len,
   NULL_);
   if (status != CDF_OK)
      StatusHandler(status);

   model_name[ gattr_str_len[0] ] = '\0';

   /*printf("DEBUG\textracted model name attribute --->%s<---\n", model_name );*/

   /********** use model name attribute to determine which get_*_cdf_info routine to call ****************/

   if ( !strcmp(batsrus, model_name) )
   {
      if(DEBUG_FLAG) { printf("DEBUG\tCDF files contains %s model data\nDEBUG\t"
            "calling close_batsrus_cdf\n", model_name );}
      close_batsrus_cdf();

      /*** reset allocate_deafults_done back to zero **/

         allocate_deafults_done = 0;

   }
   else if ( !strcmp(ucla_ggcm, model_name) )
   {
      if(DEBUG_FLAG) { printf("DEBUG\tCDF files contains %s model data\nDEBUG\t"
            "calling close_ucla_ggcm_cdf\n", model_name );}
      close_ucla_ggcm_cdf();
   }
   else if ( !strcmp(open_ggcm, model_name) )
   {
      if(DEBUG_FLAG) { printf("DEBUG\tCDF files contains %s model data\nDEBUG\t"
            "calling close_ucla_ggcm_cdf\n", model_name );}
      close_ucla_ggcm_cdf();
   }
   else if ( !strcmp(ctip, model_name) )
   {
      if(DEBUG_FLAG) { printf("DEBUG\tCDF files contains %s model data\nDEBUG\t"
            "calling close_ctip_cdf\n", model_name );}
      close_ctip_cdf();
   }
   else if ( !strcmp(enlil, model_name) )
   {
      if(DEBUG_FLAG) { printf("DEBUG\tCDF files contains %s model data\nDEBUG\t"
            "calling close_enlil_cdf\n", model_name );}
      close_enlil_cdf();
   }
   else if ( !strcmp(mas, model_name) )
   {
      if(DEBUG_FLAG) { printf("DEBUG\tCDF files contains %s model data\nDEBUG\t"
            "calling close_mas_cdf\n", model_name );}
      close_mas_cdf();
   }
//   else if ( !strcmp(magic, model_name) )
//   {
//      if(DEBUG_FLAG) { printf("DEBUG\tCDF files contains %s model data\nDEBUG\t"
//            "calling close_magic_cdf\n", model_name );}
//      close_magic_cdf();
//   }
   else if ( !strcmp(adapt3d, model_name) )
   {
      if(DEBUG_FLAG) { printf("DEBUG\tCDF files contains %s model data\nDEBUG\t"
            "calling close_adapt3d_cdf\n", model_name );}
      close_adapt3d_cdf();
   }
   else
   {
      printf(
            "ERROR\tCDF FILE may not have closed properly...Kameleon doesn't "
            "have a close module for %s\n",
            model_name);
   }

   /** destroy stuctures before returning control to calling program **/
   destroy_list( &loaded_variable_listPtr);
   destroy_list( &open_filePtr);

   return;

}

