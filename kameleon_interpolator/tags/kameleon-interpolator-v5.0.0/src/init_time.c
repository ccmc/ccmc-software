/*******************************************************************************
 *                                                                             *
 *    NAME:          init_time.c                                               *
 *                                                                             *
 *    VERSION:       3.3                                                       *
 *                                                                             *
 *    AUTHOR:        Marlo M. H. Maddox                                        *
 *                   NASA-GSFC-AETD-ISD-ADMA(587)-CCMC                         *
 *                   Marlo.Maddox@nasa.gov                                     *
 *                   (301) 286 - 5202                                          *
 *                                                                             *
 *                                                                             *
 *    PURPOSE:       Initializes tree structure used by time_interpolate code. *
 *                   A given output durectory is specified containing CCMC CDF *
 *                   files converted using the ccmc_converter tool.  Time and  *
 *                   name information for each CCMC CDF file is inserted into a*
 *                   tree structure that is created in this program.  When     *
 *                   subsequent calls to time_interpolate are made given a     *
 *                   particular time,  the data within the tree structure is   *
 *                   used to find the two files whose time steps are <= & >=   *
 *                   the given target time and ultimatley used in the time     *
 *                   interpolation                                             *
 *                                                                             *
 *    INPUTS:        char cdf_data_path - path of directory containing CCMC CDF*
 *                   data                                                      *
 *                                                                             *
 *                   double start_time  - variable defined in calling progran. *
 *                   value will be set to earliest time in data range pass in  *
 *                   address of variable                                       *
 *                                                                             *
 *                   double end_time  - variable defined in calling progran.   *
 *                   value will be set to latest time in data range pass in    *
 *                   address of variable                                       *
 *                                                                             *
 *    OUTPUTS:       ...                                                       *
 *                                                                             *
 *    FUNCTIONS:     ...                                                       *
 *                                                                             *
 *    VARIABLES:     ...                                                       *
 *                                                                             *
 *                                                                             *
 *    MODIFICATION                                                             *
 *    HISTORY:                                                                 *
 *                                                                             *
 *    11.08.2004     Maddox, Marlo                                             *
 *                   Initial Release                                           *
 *                                                                             *
 *    01.06.2005     Maddox, Marlo                                             *
 *                   Generalized code from inital particle tracing app for     *
 *                   general ccmc-lib use                                      *
 *                                                                             *
 *    03.03.2005     Maddox, Marlo                                             *
 *                   Integration and testing for use with ucla_ggcm data       *
 *                                                                             *
 *    07.20.2006     Maddox, Marlo                                             *
 *                   added check for directories with < 2 valid input files    *
 *                   program will now throw an error and exit if this is the   *
 *                   case Added #include "cdf_interface_variables.h" so program*
 *                   can use the cdf_missing_value if returned from            *
 *                   float_attr_get                                            *
 *                                                                             *
 ******************************************************************************/

#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <dirent.h>
#include <sys/param.h>
#include "cdf.h"
#include "cdf_interface_variables.h"
#include "tree_structure_toolkit.h"

#define VERBOSE_FLAG     0

long init_time(
      char *path,
      double *start_time,
      double *end_time)
{

   int access_return_status;
   int read_input_directory(
         char *,
         int);

   int verbose_flag= VERBOSE_FLAG, return_status = 0;

   /* check directory path for exisistence & READ permission */

   if (verbose_flag)
   {
      printf(
            "INSIDE init_time(): path = %s\n",
            path);
   }

   access_return_status = access(
         path,
         R_OK);

   if (verbose_flag)
   {
      printf(
            "access( %s, R_OK ) = %d\n",
            path,
            access_return_status);
   }

   if (access_return_status != 0)
   {
      printf(
            "\nERROR: will not be able to read output from -->%s<-- \n",
            path);
      exit(1);
   }
   else
   {
      if (verbose_flag)
      {
         printf(
               "\ninput directory %s READ OK...\n",
               path);
      }
   }

   /* start reading contents of the directory */

   return_status = read_input_directory(
         path,
         verbose_flag);

   if (return_status <= 1)
   {
      printf(
            "\n%s - ERROR:\tNot enough time steps in directory  %s\nUse regular "
            "interpolator for single files...\nEXITING PROGRAM.\n",
            __FILE__, path ); exit( EXIT_FAILURE );}

         start_time[0] = findmin( rootPtr );

         end_time[0] = findmax( rootPtr );

      /*** lets keep a copy of these for ourselves,  use later on inside time_interpolate for bounds checking ***/
      stored_start_time = start_time[0];
      stored_end_time = end_time[0];

      return 0;

   }

int read_input_directory(
      char *input_directory,
      int verbose_flag)
{

   extern void open_cdf(
         char *,
         int number_of_arguments,
         ...);
   extern char * gattribute_char_get(
         char *);
   extern float gattribute_float_get(
         char *);
   extern void insert_node(
         TreeNodePtr *,
         char *,
         double);
   extern void in_order(
         TreeNodePtr);
   extern void close_cdf(
         void);

   DIR *directory_pointer;
   struct dirent *directory_struct;

   int number_directory_entries=0;

   char current_working_directory_temp[MAX_STRING_LEN];
   int buf_size= MAX_STRING_LEN;

   /**** allocate  full_path_filename some memory space ****/
   /*char *full_path_filename, full_path_filename_2[MAX_STRING_LEN];*/
   char full_path_filename[MAX_STRING_LEN];
   char *full_path_filenamePtr;

   char *filename;

   char *dot = ".";
   char *dotdot = "..";
   char *root = "/";
   char *back_slash= "/";
   char *cdf = "cdf";

   /******************* time manipulation variables **********/

   char *start_time;
   float elapsed_time;
   double start_time_epoch;
   double current_file_time_epoch;

   if ( (directory_pointer = opendir(input_directory) ) == NULL)
   {
      perror(input_directory);
      exit(1);
   }
   else
   {

      while ( (directory_struct = readdir(directory_pointer)) != NULL)
      {

         char *string_token;
         char string_buffer[MAX_STRING_LEN];

         int valid_file = 0;

         chdir(input_directory);

         /* do not process . and .. directory entries or file ! *.cdf */
         if (strcmp(
               directory_struct -> d_name,
               dot) && strcmp(
               directory_struct -> d_name,
               dotdot) )
         {

            /*** isolate file name extensions - way of validating file otherwise cdf reads fail on open ***/

            strcpy(
                  string_buffer,
                  directory_struct -> d_name);

            if (verbose_flag)
            {
               printf(
                     "\nprocessing %s ...\n",
                     string_buffer);
            }

            string_token = strtok(
                  string_buffer,
                  dot);

            /*printf("generating string tokens ... %s\n", string_token );*/

            while ( (string_token = strtok( NULL, dot) ) != NULL)
            {

               /*printf("string_token is now %s\n", string_token );*/

               if ( !strcmp(
                     string_token,
                     cdf) )
               {
                  valid_file = 1;
               }

            }

            if (valid_file)
            {

               number_directory_entries++;
               if (verbose_flag)
               {
                  printf(
                        "Directory entry (%d)\t%s\n",
                        number_directory_entries,
                        directory_struct -> d_name);
               }

               /* check each element of directory_entries to see if it exists and is readable and call the read routine for each */

               /*full_path_filename = getwd( current_working_directory_temp );*/
               full_path_filenamePtr = getcwd(
                     current_working_directory_temp,
                     buf_size);
               strcpy(
                     full_path_filename,
                     current_working_directory_temp);

               if (strcmp(
                     input_directory,
                     root) != 0)
               {
                  strcat(
                        full_path_filename,
                        back_slash);
               } /* add a backslash to path if cwd is not root / */

               filename = directory_struct -> d_name;

               strcat(
                     full_path_filename,
                     filename);

               if (verbose_flag)
               {
                  printf(
                        "checking read permision for (%d)\t%s\n",
                        number_directory_entries,
                        full_path_filename);
               }

               if (access(
                     full_path_filename,
                     R_OK) )
               {
                  printf(
                        "*unreadable*\t%s\n",
                        full_path_filename);
                  perror(full_path_filename);
               }
               else /* file is readable call read routine for current file */
               {
                  if (verbose_flag)
                  {
                     printf(
                           "inserting tree node %s ...\n\n",
                           full_path_filename);
                  }

                  /*printf("DEBUG\t calling open_cdf( %s, 0 )\n", full_path_filename );*/

                  open_cdf(
                        full_path_filename,
                        0);

                  /*printf("DEBUG\topen_cdf( %s, 0 ) successfully\n", full_path_filename );*/

                  start_time = gattribute_char_get("start_time");

                  if (start_time == NULL)
                  {
                     printf(
                           "%s - ERROR: Could find essential start_time "
                           "attribute for time interpolation\n",
                           __FILE__ ); return EXIT_FAILURE;}

                        elapsed_time = gattribute_float_get( "elapsed_time_in_seconds" );

                     if (elapsed_time == cdf_missing_value )
                     {  printf("%s - ERROR: Could find essential start_time "
                           "attribute for time interpolation\n", __FILE__ ); return EXIT_FAILURE;}

                     start_time_epoch = parseEPOCH3( start_time );
                     current_file_time_epoch = start_time_epoch + ( elapsed_time * 1000.0 ); /* because cdf epoch is in milli seconds */
                     start_time[25] = '\0';

                     insert_node( &rootPtr, full_path_filename, current_file_time_epoch );

                     close_cdf();

                  }

               }
               else
               {
                  if( verbose_flag )
                  {
                     printf("File extension %s couldn't be verified.  Ignoring "
                           "file %s ....\n", string_token, directory_struct -> d_name );
                  }
               }

            }
            else
            {
               if(verbose_flag)
               {  printf("Not using %s found in %s\n", directory_struct -> d_name, input_directory );}
            }
         }
      }

      closedir(directory_pointer);

      /*********** DEBUGGING/VERBOSE OUTPUT *
       printf("\nDEBUG\tprinting tree using in-order notation\n");
       in_order( rootPtr );
       printf("\nDEBUG\tprinting tree using pre-order notation\n");
       pre_order( rootPtr );
       printf("\nDEBUG\tprinting tree using post-order notation\n");
       post_order( rootPtr );
       */
      /*
       printf("DEBUG\t[1]\taddress of rootPtr = %d\n", &rootPtr );
       printf("DEBUG\t[1]\taddress of rootPtr->LPtr = %d\n", &rootPtr->LPtr );
       printf("DEBUG\t[1]\taddress of rootPtr->RPtr = %d\n", &rootPtr->RPtr );
       */

      /** lets return the number of directory entries to check if >1 **/

      return number_directory_entries;

   }

