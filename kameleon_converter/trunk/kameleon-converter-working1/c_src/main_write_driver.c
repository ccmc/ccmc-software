/*******************************************************************************
 *                                                                             *
 *    NAME:          main_write_driver.c                                       *
 *                                                                             *
 *    VERSION:       4.4.1                                                     *
 *                                                                             *
 *    AUTHOR:        Marlo M. H. Maddox                                        *
 *                   NASA-GSFC-AETD-ISD-ADMA(587)                              *
 *                   Marlo.Maddox@nasa.gov                                     *
 *                   (301) 286 - 5202                                          *
 *                                                                             *
 *    PURPOSE:       Main write driver code for model output conversion        *
 *                   software Selects specific write routine based on          *
 *                   -f|--format option and -m|--model option specified by user*
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
 *    2003 September 11th     Maddox, Marlo                                    *
 *                            initial development started                      *
 *    2003 December  10th     Maddox, Marlo                                    *
 *                            changing everything to cdf z variables to account*
 *                            for new block/grid info with diff dimmension size*
 *                                                                             *
 *    2004 March     26th     Maddox, Marlo                                    *
 *                            Decomposing main_write_driver to create meta_data*
 *                            package for current model then call external     *
 *                            specialized model-format write routine ie.       *
 *                            write_batsrus_cdf or write_ucla_cdf to do the    *
 *                            actual write                                     *
 *                                                                             *
 *    2005 March     4th      Maddox, Marlo                                    *
 *                            made global/variable_metadata arrays static      *
 *                                                                             *
 *                                                                             *
 *    2005 April     5th      Maddox, Marlo                                    *
 *                            Redesign.  Changing to single format write using *
 *                            attribute and variable structures that can be    *
 *                            provided directly from read routine and validated*
 *                            or created from write_*_*_structure() developed  *
 *                            from former write_*_cdf() routines.              *
 *                                                                             *
 *    2006 April    18th      Maddox, Marlo                                    *
 *                            Creating  write_enlil_structure routine          *
 *                            NOTE:  This is one of two ways to populate the   *
 *                            structures. The other way is directly with the   *
 *                            read_<model> routines.                           *
 *                                                                             *
 *    2006 November 29th      Maddox, Marlo                                    *
 *                            Creating write_kpvt_structure routine            *
 *                                                                             *
 *    2006 Deember  4th       Maddox, Marlo                                    *
 *                            Creating write_msfc_tvm_structure routine        *
 *                                                                             *
 *    2007 February 6th       Maddox, Marlo                                    *
 *                            Adding MAS Write                                 *
 *                                                                             *
 *    2007 February 16th      Maddox, Marlo                                    *
 *                            adding file_name_length = strlen(input_file_name)*
 *                            again directly after write_mas_structure() call  *
 *                            as the function modifies the contents of the     *
 *                            original input_file_name string to reflect a     *
 *                            generic name mas_merged_step_* vs the variable   *
 *                            specific name ie bp001.hdf                       *
 *                                                                             *
 ******************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

int main_write_driver(
      char *data_format,
      char *input_file_name,
      char *model_name,
      int model_key,
      int verbose_flag,
      char *output_directory,
      char *original_working_directory,
      int write_database_info_flag)
{

   extern int write_batsrus_structure(
         char *,
         char *,
         int,
         int,
         char *,
         char *,
         char *,
         int);
   extern int write_ucla_ggcm_structure(
         char *,
         char *,
         int,
         int,
         char *,
         char *,
         char *,
         int);
   extern int write_enlil_structure(
         char *,
         char *,
         int,
         int,
         char *,
         char *,
         char *,
         int);
   extern int write_kpvt_structure(
         char *,
         char *,
         int,
         int,
         char *,
         char *,
         char *,
         int);
   extern int write_msfc_tvm_structure(
         char *,
         char *,
         int,
         int,
         char *,
         char *,
         char *,
         int);
   extern int write_mas_structure(
         char *,
         char *,
         int,
         int,
         char *,
         char *,
         char *,
         int);
   extern int write_gumics_structure(
         char *,
         char *,
         int,
         int,
         char *,
         char *,
         char *,
         int);

   extern int write_structures_2_cdf(
         char *,
         char *,
         int,
         int,
         char *,
         char *,
         char *);

   int i, j, k, array_size, format_key, file_name_length;

   int write_structures_2_cdf_return_status;

   /****************************************************************************
    *    build a key value table of the models to use in switch statement      *
    *                                                                          *
    *    batsrus                 = 1                                           *
    *    OpenGGCM/UCLA-GGCM      = 2                                           *
    *    CTIP                    = 3                                           *
    *    ENLIL                   = 4                                           *
    *    KPVT                    = 5                                           *
    *    MSFC_TVM                = 6                                           *
    *    MAS                     = 7                                           *
    *                                                                          *
    ***************************************************************************/

   /** Intructs the main_write_driver of which write_*_2_structure function to
    * call **/

   file_name_length = strlen(input_file_name);

   switch (model_key)

   /* NOTE: this switch is still using model key created in main_read_driver.c */
   {
      case 1: /*************** BATSRUS MODEL *********************/

         /*** write all current batsrus data from main memory into standardized
          * variable & attribute structures  ***/

         if (verbose_flag)
         {
            printf(
                  "calling write_%s_striucture routine for %s\n",
                  model_name,
                  model_name);
         }

         write_batsrus_structure(
               input_file_name,
               data_format,
               file_name_length,
               verbose_flag,
               output_directory,
               model_name,
               original_working_directory,
               write_database_info_flag);
         break;

      case 2: /*************************** UCLA-GGCM MODEL ********************/

         /*** write all current ucla_ggcm/OpenGGCM data from main memory into
          * standardized variable & attribute structures  ***/

         if (verbose_flag)
         {
            printf(
                  "calling write_%s_structure routine for %s\n",
                  model_name,
                  model_name);
         }

         write_ucla_ggcm_structure(
               input_file_name,
               data_format,
               file_name_length,
               verbose_flag,
               output_directory,
               model_name,
               original_working_directory,
               write_database_info_flag);
         break;

      case 3: /************************* CTIP MODEL ***************************/

         /*** write all current ucla_ggcm/OpenGGCM data from main memory into
          * standardized variable & attribute structures  ***/

         if (verbose_flag)
         {
            printf(
                  "calling write_%s_striucture routine for %s\n",
                  model_name,
                  model_name);
         }

         if (verbose_flag)
            printf("DEBUG\tINFO:\tWe would normally call a specific "
                  "write_structure routine but since the ctip read has already provided structures, lets go straight to cdf...\n");

         break;

      case 4: /*************************** ENLIL MODEL ************************/

         /*** write all current ENLIL data from main memory into standardized
          * variable & attribute structures  ***/

         if (verbose_flag)
         {
            printf(
                  "calling write_%s_striucture routine for %s\n",
                  model_name,
                  model_name);
         }

         write_enlil_structure(
               input_file_name,
               data_format,
               file_name_length,
               verbose_flag,
               output_directory,
               model_name,
               original_working_directory,
               write_database_info_flag);
         break;

      case 5: /******************** KPVT DATA *********************************/

         /*** write all current KPVT data from main memory into standardized
          * variable & attribute structures  ***/

         if (verbose_flag)
         {
            printf(
                  "calling write_%s_striucture routine for %s\n",
                  model_name,
                  model_name);
         }

         write_kpvt_structure(
               input_file_name,
               data_format,
               file_name_length,
               verbose_flag,
               output_directory,
               model_name,
               original_working_directory,
               write_database_info_flag);
         break;

      case 6: /***************** MSFC_TVM DATA ********************************/

         /*** write all current MSFC_TVM data from main memory into standardized
          * variable & attribute structures  ***/

         if (verbose_flag)
         {
            printf(
                  "calling write_%s_striucture routine for %s\n",
                  model_name,
                  model_name);
         }

         write_msfc_tvm_structure(
               input_file_name,
               data_format,
               file_name_length,
               verbose_flag,
               output_directory,
               model_name,
               original_working_directory,
               write_database_info_flag);
         break;

      case 7: /************************* MAS **********************************/

         /*** write all current MAS data from main memory into standardized
          * variable & attribute structures  ***/

         if (verbose_flag)
         {
            printf(
                  "calling write_%s_striucture routine for %s\n",
                  model_name,
                  model_name);
         }

         write_mas_structure(
               input_file_name,
               data_format,
               file_name_length,
               verbose_flag,
               output_directory,
               model_name,
               original_working_directory,
               write_database_info_flag);
         break;

      case 8: /************************* GUMICS **********************************/

         /*** write all current GUMICS data from main memory into standardized
          * variable & attribute structures  ***/

         if (verbose_flag)
         {
            printf(
                  "calling write_%s_structure routine for %s\n",
                  model_name,
                  model_name);
         }

         write_gumics_structure(
               input_file_name,
               data_format,
               file_name_length,
               verbose_flag,
               output_directory,
               model_name,
               original_working_directory,
               write_database_info_flag);
         break;

      default:
         printf(
               "ERROR: NO wriite structure routine available for %s\n",
               model_name);
         break;
   }

   /****************************************************************************
    *    build a key value table of the formats to use in switch statement     *
    *                                                                          *
    *    CDF        = 1                                                        *
    *    HDF5       = -                                                       *
    ***************************************************************************/

   if (strcmp(data_format, "cdf") == 0)
   {
      format_key = 1;
   }

   switch (format_key)
   /** determine which data format to write structure data to ***/
   {
      case 1: /* CDF FORMAT */

         if (verbose_flag)
         {
            printf(
                  "calling write_striucture_2_cdf routine for %s\n",
                  model_name);
         }

         write_structures_2_cdf_return_status = write_structures_2_cdf(
               input_file_name,
               data_format,
               file_name_length,
               verbose_flag,
               output_directory,
               model_name,
               original_working_directory);
         break;

      default:
         printf(
               "ERROR NO write structure routine available for %s data format\n",
               data_format);
   }

   if( write_structures_2_cdf_return_status == EXIT_FAILURE )
   {
      return EXIT_FAILURE;
   }
   else
   {
      return EXIT_SUCCESS;
   }
}

