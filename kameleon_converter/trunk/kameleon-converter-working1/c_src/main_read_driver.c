/*******************************************************************************
 *                                                                             *
 *    NAME:          main_read_driver.c                                        *
 *                                                                             *
 *    VERSION:       5.0                                                       *
 *                                                                             *
 *    AUTHOR:        Marlo M. H. Maddox                                        *
 *                   NASA-GSFC-AETD-ISD-ADMA(587)                              *
 *                   Marlo.Maddox@nasa.gov                                     *
 *                   (301) 286 - 5202                                          *
 *                                                                             *
 *    PURPOSE:       Main read driver code for model output conversion software*
 *                   Selects specific read routine based on -m|--model option  *
 *                   specified by user                                         *
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
 *    2004 March 23rd         Maddox, Marlo                                    *
 *                            added ucla_ggcm model                            *
 *    2004 September 9th      Maddox, Marlo                                    *
 *                            passing minmax_flag through and database_file    *
 *                            was passed in August...forgot to update mod hist *
 *    2006 January 4th        Maddox, Marlo                                    *
 *                            added string.h                                   *
 *                                                                             *
 *                            fixed the verbose print out for                  *
 *                            main_write_driver() routine                      *
 *                                                                             *
 *    2006 February 2nd       Maddox, Marlo                                    *
 *                            addidng open_ggcm alias and new enlil model      *
 *                            functionality                                    *
 *    2006 November 6th       Maddox, Marlo added #ifndef to read_enlil        *
 *                            case block.  If KAMELEON was compiled without    *
 *                            libnetcdf.a, the program will still execute      *
 *                            but print a warning if -m ENLIL is used          *
 *    2006 November 29th      Maddox, Marlo                                    *
 *                            adding kptv for the kitt peak vacuum telescope   *
 *                            observation data                                 *
 *    2006 December 4th       Maddox, Marlo                                    *
 *                            Adding msfc_tvm for Marshalls' Tower Vector      *
 *                            Magnetograph telescope observations...active     *
 *                            regions                                          *
 *    2006 December 14th      Maddox, Marlo                                    *
 *                            Adding MAS                                       *
 *    2008 February 21        Maddox, Marlo                                    *
 *                            Changing the #ifdef HDF to                       *
 *                            #ifdef KAMELEON_WITH_HDF                         *
 *                            to make sure the macro isn't confilting with     *
 *                            external macros defined by other librariers      *
 *    2008 July 24th          Maddox, Marlo                                    *
 *                            Returning read_successfull_flag to calling       *
 *                            function.                                        *
 *                                                                             *
 *                            Adding EXIT_SUCESS & EXIT_FAILURE return values  *
 ******************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "ccmc_registered_models.h"

int input_filter(
      char *model_name,
      char *input_file_name,
      int verbose_flag,
      char *data_format,
      char *output_directory,
      char *original_working_directory,
      char *aux_file,
      char *database_file,
      int write_database_info_flag,
      int minmax_flag)
{

   extern int read_batsrus_variable(FILE *, float *, int, int, double *);
   extern int read_ucla_ggcm(char *, char *, int, int);
   extern int read_batsrus(char *, int, int);
   extern int read_gumics(char *, int, int);
   extern int read_ctip(char *, char *, int, int);
#ifdef NETCDF
   extern int read_enlil(char *, char *, int, int);
#endif
#ifdef FITS
   extern int read_kpvt(char *, char *, int, int);
   extern int read_msfc_tvm(char *, char *, int, int);
#endif
#ifdef KAMELEON_WITH_HDF
   extern int read_mas(char *, char *, int, int);
#endif

   extern int main_write_driver(
         char *,
         char *,
         char *,
         int,
         int,
         char *,
         char *,
         int);

   /****************************************************************************
    *                                                                          *
    *  build a key value reference table for models to use in switch statement *
    *                                                                          *
    *    batsrus        = 1                                                    *
    *    ucla_ggcm      = 2 -\                                                 *
    *    open_ggcm      = 2   ----> both strings will point to same case       *
    *    ctip           = 3                                                    *
    *    enlil          = 4                                                    *
    *    kpvt           = 5                                                    *
    *    msfc           = 6                                                    *
    *    mas            = 7                                                    *
    *    gumics         = 8                                                                      *
    ***************************************************************************/

   int model_key;
   int return_status;
   int read_successfull_flag = 0;
   int write_successfull_flag = 0;

   if (strcmp(model_name, BATSRUS) == 0)
   {
      model_key = 1;
   }
   else if (strcmp(model_name, UCLA_GGCM) == 0)
   {
      model_key = 2;
   }
   else if (strcmp(model_name, OPEN_GGCM) == 0)
   {
      model_key = 2;
   }
   else if (strcmp(model_name, CTIP) == 0)
   {
      model_key = 3;
   }
   else if (strcmp(model_name, ENLIL) == 0)
   {
      model_key = 4;
   }
   else if (strcmp(model_name, KPVT) == 0)
   {
      model_key = 5;
   }
   else if (strcmp(model_name, MSFC_TVM) == 0)
   {
      model_key = 6;
   }
   else if (strcmp(model_name, MAS) == 0)
   {
      model_key = 7;
   }
   else if (strcmp(model_name, GUMICS) == 0)
   {
      model_key = 8;
   }

   if (verbose_flag)
   {
      printf(
            "\nselecting %s model read routine for input file %s\n",
            model_name,
            input_file_name);
   }

   switch (model_key)
   {
      case 1:
         if (verbose_flag)
         {
            printf("\ncalling read %s routine\n", model_name);
         }

         if (read_batsrus(input_file_name, verbose_flag, minmax_flag) )
         {
            read_successfull_flag = 1;
         }
         else
         {
            printf(
                  "!!!ERROR reading ---> %s ...ignoring file \n",
                  input_file_name);
         }

         break;
      case 2:
         if (verbose_flag)
         {
            printf("\ncalling read %s routine\n", model_name);
         }

         if (read_ucla_ggcm(
               input_file_name,
               aux_file,
               verbose_flag,
               minmax_flag) )
         {
            read_successfull_flag = 1;
         }
         else
         {
            printf(
                  "!!!ERROR reading ---> %s ...ignoring file \n",
                  input_file_name);
         }

         break;
      case 3:
         if (verbose_flag)
         {
            printf("\ncalling read %s routine\n", model_name);
         }

         if (read_ctip(input_file_name, aux_file, verbose_flag, minmax_flag)
               != EXIT_FAILURE)
         {
            read_successfull_flag = 1;
         }
         else
         {
            printf(
                  "!!!ERROR reading ---> %s ...ignoring file \n",
                  input_file_name);
         }

         break;
      case 4:
         if (verbose_flag)
         {
            printf("\ncalling read %s routine\n", model_name);
         }

#ifdef NETCDF

         if (read_enlil(input_file_name, aux_file, verbose_flag, minmax_flag)
               != EXIT_FAILURE)
         {
            read_successfull_flag = 1;
         }
         else
         {
            printf(
                  "!!!ERROR reading ---> %s ...ignoring file \n",
                  input_file_name);
         }

#endif

#ifndef NETCDF
         printf("\n***WARNING***\t%s\tKAMELEON Converter was not compiled with "
               "the libnetcdf.a library.  Will NOT be able to read ENLIL .nc "
               "files. See README for more details...\n\n", __FILE__ );
#endif

         break;
      case 5:
         if (verbose_flag)
         {
            printf("\ncalling read %s routine\n", model_name);
         }

#ifdef FITS

         if (read_kpvt(input_file_name, aux_file, verbose_flag, minmax_flag)
               != EXIT_FAILURE)
         {
            read_successfull_flag = 1;
         }
         else
         {
            printf(
                  "!!!ERROR reading ---> %s ...ignoring file \n",
                  input_file_name);
         }

#endif

#ifndef FITS
         printf("\n***WARNING***\t%s\tKAMELEON Converter was not compiled with "
               "the libcfitsio.a library.  Will NOT be able to read .fts files. "
               "See README for more details...\n\n", __FILE__ );
#endif

         break;
      case 6:
         if (verbose_flag)
         {
            printf("\ncalling read %s routine\n", model_name);
         }

#ifdef FITS

         if (read_msfc_tvm(input_file_name, aux_file, verbose_flag, minmax_flag)
               != EXIT_FAILURE)
         {
            read_successfull_flag = 1;
         }
         else
         {
            printf(
                  "!!!ERROR reading ---> %s ...ignoring file \n",
                  input_file_name);
         }

#endif

#ifndef FITS
         printf("\n***WARNING***\t%s\tKAMELEON Converter was not compiled with "
               "the libcfitsio.a library.  Will NOT be able to read .fts files. "
               "See README for more details...\n\n", __FILE__ );
#endif

         break;
      case 7:
         if (verbose_flag)
         {
            printf("\ncalling read %s routine\n", model_name);
         }
#ifdef KAMELEON_WITH_HDF

         fprintf( stderr, "%s *WARNING: %s stores each variable in a seperate "
               "file ie bp001.hdf.  Kameleon will automatically attempt to "
               "identify MAS output files of the form Snnn.hdf were S can be a "
               "1,2, or 3 character variable identifier and nnn is a zero padded"
               " 3 digit step number.  If the file name deviates from this "
               "naming convention, KAMELEON may yield unexpected results or"
               " merely ignore the file...\n", __FILE__, model_name);

         if (read_mas(input_file_name, aux_file, verbose_flag, minmax_flag)
               != EXIT_FAILURE)
         {
            read_successfull_flag = 1;
         }
         else
         {
            printf("!!! skipping ---> %s ... \n", input_file_name);
         }

#endif

#ifndef KAMELEON_WITH_HDF
         printf("\n***WARNING***\t%s\tKAMELEON Converter was not compiled with "
               "the libcfitsio.a library.  Will NOT be able to read .fts files."
               " See README for more details...\n\n", __FILE__ );
#endif

         break;
      case 8:
         if (verbose_flag)
         {
            printf("\ncalling read %s routine\n", model_name);
         }
         printf("Success: %i failure: %i\n",EXIT_SUCCESS,EXIT_FAILURE);
         if (read_gumics(input_file_name, verbose_flag, minmax_flag) )
         {
            read_successfull_flag = 1;
         }
         else
         {
            printf("!!! skipping ---> %s ... \n", input_file_name);
         }

         break;

      default:
         printf("no read routine available for %s\n", model_name);
   }

   /* check to see if read was successfully before calling main_write_driver */

   if (read_successfull_flag)
   {

      /* call main write driver */

      if (verbose_flag)
      {
         printf(
               "\ncalling main_write_driver( %s, %s, %s, %d,  %d, %s, %s, %d )\n",
               data_format,
               input_file_name,
               model_name,
               model_key,
               verbose_flag,
               output_directory,
               original_working_directory,
               write_database_info_flag);
      }

      write_successfull_flag = main_write_driver(
            data_format,
            input_file_name,
            model_name,
            model_key,
            verbose_flag,
            output_directory,
            original_working_directory,
            write_database_info_flag);

      /** check return status of main_write_driver() **/

      if( write_successfull_flag == EXIT_FAILURE )
      {
         /** Return Control To Caller With EXIT_FAILURE Status **/

         printf("\n***WARNING***\t%s\tKAMELEON Converter was not able to convert input file %s...skipping...\n\n", __FILE__, input_file_name );

         return EXIT_FAILURE;
      }
      else
      {
         /* Do Nothing, Let Control Go To End Of Function Were EXIT_SUCCESS will
          * be returned **/
      }

   }
   else
   {
      /* If read was unsucessfull print error and then return control to calling
       * function **/

      printf("\n***WARNING***\t%s\tKAMELEON Converter was NOT able to read input file %s...skipping...\n\n", __FILE__, input_file_name );

      return EXIT_FAILURE;

   }


   return EXIT_SUCCESS;

}

