/*******************************************************************************
 *                                                                             *
 *    NAME:       main_conversion_driver.c                                     *
 *                                                                             *
 *    VERSION:    5.0                                                          *
 *                                                                             *
 *    AUTHOR:     Marlo M. H. Maddox                                           *
 *                NASA-GSFC-AETD-ISD-ADMA(587)                                 *
 *                Marlo.Maddox@nasa.gov                                        *
 *                (301) 286 - 5202                                             *
 *                                                                             *
 *    PURPOSE:    Main driver code for model output conversion software.       *
 *                Performs initial command line argument processing and calling*
 *                of support libraries for specific conversions.               *
 *                                                                             *
 *    INPUTS:     Command Line Arguments:                                      *
 *                                int argc                                     *
 *                                char **argv                                  *
 *                                                                             *
 *    OUTPUTS:    Converted Model Output files                                 *
 *                                                                             *
 *    FUNCTIONS:  *                                                            *
 *                                                                             *
 *    VARIABLES:  *                                                            *
 *                                                                             *
 *                                                                             *
 *    MODIFICATION                                                             *
 *    HISTORY:                                                                 *
 *                                                                             *
 *    2003 Sept   11th     Maddox, Marlo                                       *
 *                         initial development started                         *
 *                                                                             *
 *    2004 March  23rd     Maddox, Marlo                                       *
 *                         added -aux_file flag for auxiliary input files      *
 *                                                                             *
 *    2004 July   30th     Maddox, Marlo                                       *
 *                         chaging to argtable2 from regular argtable          *
 *                                                                             *
 *    2004 August 31st     Maddox, Marlo                                       *
 *                         added -d --database flag to specify location of     *
 *                         database file & pass name to main_read_driver       *
 *                                                                             *
 *    2004 Sept.  9th      Maddox, Marlo                                       *
 *                         added flag to turn off actual min/max calculations  *
 *                         for each variable                                   *
 *                                                                             *
 *    2005 Feb.   16th     Maddox, Marlo                                       *
 *                         Refining rotutines: removing argtable1 references,  *
 *                         cleaning validate_input & read_input_directory      *
 *                         memory elements...                                  *
 *                                                                             *
 *    2005 Feb.   28th     Maddox, Marlo                                       *
 *                         made input_file_array_base_name_local_copy array in *
 *                         read_input_directory function static                *
 *                                                                             *
 *    2005 March 22nd      Maddox, Marlo                                       *
 *                         increased MAX_FILE_ARGS from 200 to 2500            *
 *                         added a database_file_copy variable to pass to      *
 *                         read_database routine...also added function         *
 *                         declaration read_database()                         *
 *                                                                             *
 *    2005 August 16th     Maddox, Marlo                                       *
 *                         add flag to set compression algorithm               *
 *                                                                             *
 *    2006 January 4th     Maddox, Marlo                                       *
 *                         added string.h                                      *
 *                                                                             *
 *    2006 January 6th     Maddox, Marlo                                       *
 *                         Changed getwd to getcwd and associated variables... *
 *                                                                             *
 *    2006 February 2nd    Maddox, Marlo                                       *
 *                         adding open_ggcm alias for ucla_ggcm and new enlil  *
 *                         functionality                                       *
 *                                                                             *
 *    2006 November 29th   Maddox, Marlo                                       *
 *                         Adding kptv for kitt peak vacuum telescope          *
 *                         observations                                        *
 *                                                                             *
 *    2006 Devember 4th    Maddox, Marlo                                       *
 *                         Adding msfc_tvm for Marshalls' Tower Vector         *
 *                         Magnetograph telescope observations...active regions*
 *                                                                             *
 *    2006 December 14th   Maddox, Marlo                                       *
 *                         Adding MAS                                          *
 *                                                                             *
 *    2007 May 17th        Maddox, Marlo                                       *
 *                         adding kameleon_version_info.h header               *
 *                                                                             *
 *                         also adding filter for input files that have .cdf   *
 *                         extension filter was added to:                      *
 *                         validate_input_files()                              *
 *                         read_input_directory()                              *
 *    2008 July 24th       Maddox, Marlo                                       *
 *                         Added EXIT_FAILURE & EXIT_SUCCESS to qualifying     *
 *                         return and exit calls                               *
 *                                                                             *
 ******************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <dirent.h>
#include <sys/param.h>

#include "argtable2.h"

#include "ccmc_registered_models.h"
#include "ccmc_registered_formats.h"
#include "ccmc_registered_compression_algorithms.h"

#include "kameleon_version_info.h"

#define HELP_FLAG    "-help"
#define VERSION_FLAG    "-version"

#define DEFAULT_IN_DIR    "./"
#define DEFAULT_OUT_DIR    "./"
#define DEFAULT_AUX_FILE "NONE"
#define DEFAULT_DATABASE_FILE "NONE"
#define DEFAULT_COMPRESSION "NO_COMPRESSION"

#define BACK_SLASH    "/"

#define MAX_FILE_ARGS        2500
#define MAX_ARG_LENGTH       500
#define MAX_LINE_LENGTH      2500
#define MAX_STRING_LENGTH    10
#define MAX_ARG_ERROR_STORE  50

char *version_num= KAMELEON_VERSION;

char *original_working_directoryPtr;
static char temp_original_working_directory[MAX_ARG_LENGTH];
int buf_size= MAX_ARG_LENGTH;

int input_filter_return_status;
int total_input_file_count = 0;
int successfull_conversion_count = 0;

extern int errno;

int main(
      int argc,
      char **argv)
{

   int check_registry(
         char **registry_array,
         const char **name_to_check,
         int verbose_flag,
         int total,
         char *registry_thing);
   int read_input_directory(
         const char **model_name,
         const char **input_directory,
         int verbose_flag,
         const char **data_format,
         const char **output_directory,
         const char **aux_file,
         const char **database_file,
         int write_database_info_flag,
         int minmax_flag);
   int validate_input_files(
         const char **model_name,
         const char **input_directory,
         int verbose_flag,
         const char **input_file_array_base_name,
         const char **input_file_array_full_path,
         int file_count,
         const char **data_format,
         const char **output_directory,
         const char **aux_file,
         const char **database_file,
         int write_database_info_flag,
         int minmax_flag);
   int validate_aux_file(
         const char **aux_file,
         const char **model_name,
         int verbose_flag);

   extern int read_database(
         int,
         char *);

   int i, return_status;
   int valid_model_flag;
   int valid_format_flag;
   int aux_file_set = 0;
   int valid_aux_file = 0;
   int number_arg_errors;
   int write_database_info_flag = 1; /**  default value is on - means database
    info values assumed read and will
    write to formated output **/
   int minmax_off_flag = 0; /** defulat value off - means actual min/max values
    will be calculated by default **/

   char *format_string = "format";
   char *model_string = "model";
   char *compression_string = "compression";

   /* command line variables */

   static char input_directory[MAX_ARG_LENGTH];
   static char output_directory[MAX_ARG_LENGTH];
   static char aux_file[MAX_ARG_LENGTH];
   static char input_file[MAX_FILE_ARGS][MAX_ARG_LENGTH];
   static char model_name[MAX_STRING_LENGTH];
   static char data_format[MAX_STRING_LENGTH];
   static int verbose_flag;
   static int version_flag;
   static int help_flag;

   /* make a local database_file variable */

   char *database_file_copy;
   int original_database_file_string_length = 0;

   /* define valid command line arguments using argtable2 */

   struct arg_lit *version =
         arg_lit0( NULL, "version", "print software version number");
   struct arg_lit *help = arg_lit0(
         "h",
         "help",
         "print this help");
   struct arg_lit *verbose = arg_lit0(
         "vV",
         "verbose",
         "turn verbose output on");
   struct arg_lit *minmax_off =
         arg_lit0( NULL, "nominmax", "turn OFF actual min/max calculations "
         "for each variable");
   struct arg_str *format = arg_str1(
         "f",
         "format",
         "string",
         "data format to convert to");
   struct arg_str *model = arg_str1(
         "m",
         "model",
         "string",
         "model name");
   struct arg_str *auxfile = arg_str0(
         "a",
         "aux_file",
         "string",
         "auxiliary input file ie. a grid file seperate from general"
            "data files");
   struct arg_str *database_file = arg_str0(
         "d",
         "database",
         "string",
         "ccmc DatabaseInfo file location - for CCMC use only");
   struct arg_str *indir = arg_str0(
         "i",
         "indir",
         "string",
         "directory containing input files to be converted");
   struct arg_str *outdir = arg_str0(
         "o",
         "outdir",
         "string",
         "output directory where converted files will be placed default is ./");
   struct arg_file *file =
         arg_filen( NULL, NULL, "<file>", 0, MAX_FILE_ARGS, "input files");
   struct arg_str *compression= arg_str0(
         "c",
         "compress",
         "string",
         "use specified compression algorithm");
   struct arg_end *end = arg_end( MAX_ARG_ERROR_STORE);

   void* argtable2[] =
   { version, help, verbose, minmax_off, format, model, auxfile, database_file,
         indir, outdir, file, compression, end };

   const size_t narg2 = sizeof(argtable2)/sizeof(argtable2[0]);

   /* check to see if artable was constructed successfully */

   if (arg_nullcheck(argtable2) != 0)
   {
      printf("ERROR: memory not allocated for argtable structure!\n");
   }

   /* set any default values prior to parsing, if command line values overide
    * defaults */

   indir -> sval[0] = DEFAULT_IN_DIR;
   outdir -> sval[0] = DEFAULT_OUT_DIR;
   auxfile -> sval[0] = DEFAULT_AUX_FILE;
   database_file -> sval[0] = DEFAULT_DATABASE_FILE;
   compression -> sval[0] = DEFAULT_COMPRESSION;

   /* parse arguments ( actually sets values too ) */

   number_arg_errors = arg_parse(
         argc,
         argv,
         argtable2);

   /* use set values */
   verbose_flag = verbose -> count;

   /* set minmax_off flag = 1 if minmax -> count > 0 */

   minmax_off_flag = minmax_off -> count;

   /* why does this printf statment break on sun solaris??? */

   /*printf("DEBUG\tmodel = %s\n", model -> sval[0] );
    printf("DEBUG\tvebose count = %d\n", verbose -> count );
    */

   /* check for help flag */

   if (help -> count > 0)
   {
      printf("This program converts model output into a standardized format\n");
      printf(
            "Usage: %s ",
            argv[0]);
      arg_print_syntaxv( stdout, argtable2, "\n");
      arg_print_glossary( stdout, argtable2, " %-25s %s\n");
      arg_freetable(
            argtable2,
            narg2);
      exit( 0);
   }

   /* check for version flag */

   if (version -> count > 0)
   {
      printf(
            "THIS IS KAMELEON SOFTWARE VERSION %s\n",
            version_num);
      arg_freetable(
            argtable2,
            narg2);
      exit( 0);
   }

   /* display any errors if present */

   if (number_arg_errors > 0)
   {
      arg_print_errors( stdout, end, argv[0]);
      printf(
            "\nTry '%s --help' for more information.\n",
            argv[0]);
      arg_freetable(
            argtable2,
            narg2);
      exit( 1);
   }

   /* store directory from which program was launched */

   original_working_directoryPtr = getcwd(
         temp_original_working_directory,
         buf_size);

   /* check -model argument for validity */

   check_registry(
         registered_models,
         model -> sval,
         verbose_flag,
         NUMBER_REGISTERED_MODELS,
         model_string);

   /* check -format argument for validity */

   check_registry(
         registered_formats,
         format -> sval,
         verbose_flag,
         NUMBER_REGISTERED_FORMATS,
         format_string);

   /* check -compression argument for validity if argument is present*/

   if (compression -> count > 0)
   {
      check_registry(
            registered_compression_algorithms,
            compression -> sval,
            verbose_flag,
            NUMBER_REGISTERED_COMPRESSION_ALGORITHMS,
            compression_string);
   }

   /* set the current_compression char string which is defined in the
    * ccmc_registered_compression_algorithm.h file **/

   strcpy(
         current_compression,
         compression -> sval[0]);

   /* check -outdir argument for validity */

   if (access(
         outdir -> sval[0],
         W_OK) )
   {
      printf(
            "\nERROR: will not be able to write output to %s\n",
            outdir -> sval[0]);
      exit(1);
   }
   else
   {
      if (verbose_flag)
      {
         printf(
               "\nsetting output directory to %s\n",
               outdir -> sval[0]);
      }
   }

   /* check -indir argument for validity */

   if (access(
         indir -> sval[0],
         R_OK) )
   {
      printf(
            "\nERROR: will not be able to read input from %s\n",
            indir -> sval[0]);
      exit(1);
   }
   else
   {
      if (verbose_flag)
      {
         printf(
               "setting input directory to %s\n",
               indir -> sval[0]);
      }
   }

   /* check -aux_file argument for validity if specified and check dependencies
    * if not specified */

   /* if aux_file has been explicitly set by user, NOT = NONE */

   if (strcmp(
         auxfile -> sval[0],
         DEFAULT_AUX_FILE) )
   {

      aux_file_set = 1; /* set aux file set flag to true */

      if (access(
            auxfile -> sval[0],
            R_OK) )
      {
         printf(
               "\nERROR: will not be able to read auxiliary input file %s\n",
               auxfile -> sval[0]);
         exit(1);
      }
      else
      {
         if (verbose_flag)
         {
            printf(
                  "setting auxiliary file/directory to %s\n",
                  auxfile -> sval[0]);
         }
      }
   }
   else /* NO auxiliary file was specified, check model dependency */
   {
      if (verbose_flag)
      {
         printf("NO auxiliary file specified - checking model dependencies...\n");
      }

      /* check to se if model specified by user requires the aux file to be set */
      valid_aux_file = validate_aux_file(
            auxfile -> sval,
            model -> sval,
            verbose_flag);

      if ( valid_aux_file )
      {
         arg_freetable(
               argtable2,
               narg2);
         exit( 1);
      }
   }

   /* check -database argument for validity if specified */

   if (strcmp(
         database_file -> sval[0],
         DEFAULT_DATABASE_FILE) )
   {
      /* DatabaseInfo file has been specified, check validity */

      if (access(
            database_file -> sval[0],
            R_OK) )
      {
         printf(
               "\nERROR: will not be able to read input from %s\n",
               database_file -> sval[0]);
         exit(1);
      }
      else
      {
         if (verbose_flag)
         {
            printf(
                  "setting DatabaseInfo file to %s\n",
                  database_file -> sval[0]);
         }
         original_database_file_string_length
               = strlen(database_file -> sval[0]);
         database_file_copy
               = ( char * ) malloc(original_database_file_string_length + 1);
         if (database_file_copy != NULL)
         {
            strcpy(
                  database_file_copy,
                  database_file -> sval[0]);
         }

      }
   }
   else
   {

      /*** set write_database_flag = 0 and pass to write_routine ***/

      write_database_info_flag = 0;

      if (verbose_flag)
      {
         printf(
               "setting DatabaseInfo file to %s\n",
               database_file -> sval[0]);
      }
   }

   /************* if database file was specified, call external read_database
    * function in read_database.c file **********/

   if (write_database_info_flag)
   {
      /*return_status = read_database( verbose_flag, database_file -> sval[0] );*/
      return_status = read_database(
            verbose_flag,
            database_file_copy);
   }

   /*************************** identify input files **************************/

   if (file -> count == 0) /* no input files specified */
   {
      if (verbose_flag)
      {
         printf("\nthere are No individually specified user input files\n");
      }
      read_input_directory(
            model -> sval,
            indir -> sval,
            verbose_flag,
            format -> sval,
            outdir -> sval,
            auxfile -> sval,
            database_file -> sval,
            write_database_info_flag,
            minmax_off_flag);
   }
   else /* input files specified */
   {
      if (verbose_flag)
      {
         printf("\nuser input files have been specified\n");
      }
      validate_input_files(
            model -> sval,
            indir -> sval,
            verbose_flag,
            file -> basename,
            file -> filename,
            file -> count,
            format -> sval,
            outdir -> sval,
            auxfile -> sval,
            database_file -> sval,
            write_database_info_flag,
            minmax_off_flag);
   }


   /*********************** print conversion statistics ***********************/

   if( verbose_flag )
   {
      printf("**************************************************************\n");
      printf("Kameleon Converter Version %s\n", version_num);
      printf("Conversion Statistics:\n");
      printf("Total Number Of Input Files Processed:\t\t%d\n", total_input_file_count );
      printf("Total Number Of Files Created By Kameleon:\t%d\n", successfull_conversion_count );
   }



   return EXIT_SUCCESS;
} /** END OF MAIN **/

int check_registry(
      char **registry_array,
      const char **name_to_check,
      int verbose_flag,
      int total,
      char *registry_thing)
{
   int i, valid_flag=0;

   if (verbose_flag)
   {
      printf(
            "\nchecking for supported %ss...\n",
            registry_thing);
   }

   for (i = 0; i < total; i++)
   {
      if (verbose_flag)
      {
         printf(
               "(%d) %-10s\tis a supported %s\n",
               i+1,
               registry_array[i],
               registry_thing);
      }

      if ( !strcmp(
            registry_array[i],
            name_to_check[0]) )
      {
         valid_flag = 1;
      }
   }

   if (valid_flag)
   {
      if (verbose_flag)
      {
         printf(
               "\nconfiguring software for %s %s\n",
               name_to_check[0],
               registry_thing);
      }

      return EXIT_SUCCESS;
   }
   else
   {
      printf(
            "ERROR: %s is NOT a supported %s\n",
            name_to_check[0],
            registry_thing);
      exit(EXIT_FAILURE);
   }

   /* program should never get here */

}

int read_input_directory(
      const char **model_name,
      const char **input_directory,
      int verbose_flag,
      const char **data_format,
      const char **output_directory,
      const char **aux_file,
      const char **database_file,
      int write_database_info_flag,
      int minmax_flag)
{

   extern int input_filter(
         char *model_name,
         char *input_file_name,
         int verbose_flag,
         char *data_format,
         char *output_directory,
         char *original_working_directoryPtr,
         char *aux_file,
         char *database_file,
         int write_database_info_flag,
         int minmax_flag);

   DIR *directory_pointer;
   struct dirent *directory_struct;

   /** need to retain values between calls to input_filter() and beyond **/
   static int number_directory_entries=0;
   static int i=0;

   int longest_string_length = 0;

   char *current_working_directory_tempPtr;

   char current_working_directory_temp[MAX_ARG_LENGTH];
   char current_working_directory[MAX_ARG_LENGTH];
   static char full_path_filename[MAX_ARG_LENGTH];
   char filename[MAX_ARG_LENGTH];

   int buf_size= MAX_ARG_LENGTH;

   char *dot = ".";
   char *dotdot = "..";
   char *root = "/";

   char *cdf_file_extension = ".cdf";

   static char model_name_local_copy[MAX_ARG_LENGTH];
   char input_directory_local_copy[MAX_ARG_LENGTH];
   static char data_format_local_copy[MAX_ARG_LENGTH];
   static char output_directory_local_copy[MAX_ARG_LENGTH];
   static char aux_file_local_copy[MAX_ARG_LENGTH];
   static char database_file_local_copy[MAX_ARG_LENGTH];

   static char
         input_file_array_base_name_local_copy[MAX_ARG_LENGTH][MAX_ARG_LENGTH];
   char input_file_array_full_path_local_copy[MAX_ARG_LENGTH][MAX_ARG_LENGTH];

   strcpy(
         model_name_local_copy,
         model_name[0]);
   strcpy(
         input_directory_local_copy,
         input_directory[0]);
   strcpy(
         data_format_local_copy,
         data_format[0]);
   strcpy(
         output_directory_local_copy,
         output_directory[0]);
   strcpy(
         aux_file_local_copy,
         aux_file[0]);
   strcpy(
         database_file_local_copy,
         database_file[0]);

   /** read contents of specified directory & place filtered results in
    * input_file_array_base_name_local_copy **/

   if ( (directory_pointer = opendir(input_directory_local_copy) ) == NULL)
   {
      perror(input_directory_local_copy);
      exit(EXIT_FAILURE);
   }
   else
   {
      /** for each directory entry **/

      while ( (directory_struct = readdir(directory_pointer)) != NULL)
      {
         chdir(input_directory_local_copy);

         /* do not process . and .. directory entries */

         if (strcmp(
               directory_struct -> d_name,
               dot) && strcmp(
               directory_struct -> d_name,
               dotdot) )
         {
            /*printf("processing directory entry number %d - %s\n",
             * i, directory_struct -> d_name );*/
            strcpy(
                  input_file_array_base_name_local_copy[ i ],
                  directory_struct -> d_name);
            number_directory_entries++;
            i++;
         }
      }
   }

   closedir(directory_pointer);

   /* process directory entry contents stored in input_file_array_local_copy */

   for (i = 0; i < number_directory_entries; i++)
   {
      chdir(input_directory_local_copy);
      current_working_directory_tempPtr = getcwd(
            current_working_directory_temp,
            buf_size);

      strcpy(
            full_path_filename,
            current_working_directory_temp);

      if (verbose_flag)
      {
         printf("\n\n-----------------------------------------------------------"
            "----------------------------------\n\n");
      }
      if (verbose_flag)
      {
         printf(
               "current working directory -> %s\n",
               current_working_directory_temp);
      }

      if (strcmp(
            input_directory_local_copy,
            root) != 0)
      {
         strcat(
               full_path_filename,
               BACK_SLASH);
      } /* add a backslash to path if cwd is not root / */

      strcpy(
            filename,
            input_file_array_base_name_local_copy[ i ]);

      strcat(
            full_path_filename,
            filename);

      /*

       printf("DEBUG\tafter strcpy & strcat\nfilename = %s\nfull_path_file_name "
       "= %s\n", filename, full_path_filename );
       */

      if (verbose_flag)
      {
         printf(
               "checking read permision for (%d)\t%s\n",
               i,
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
      else if (strstr(
            full_path_filename,
            cdf_file_extension) != NULL)
      {
         printf(
               "*WARNING: %s\t Current input file --->%s<--- has suffix "
                  "--->%s<---...ignoring\n",
__FILE__         , full_path_filename,
         cdf_file_extension );
      }
      else /* file is readable call read routine for current file */
      {
         if( verbose_flag )
         {  printf("\n*calling input_filter( %s , %s, %d, %s, %s, %s, %s, %s, "
            "%d, %d ); \n", model_name_local_copy, full_path_filename,
            verbose_flag, data_format_local_copy, output_directory_local_copy,
            temp_original_working_directory, aux_file_local_copy,
            database_file_local_copy, write_database_info_flag, minmax_flag );}

         /*  change dir back to original */
         chdir( original_working_directoryPtr );

         /** keep track of the total number of reads **/

         total_input_file_count++;

         input_filter_return_status = input_filter( model_name_local_copy, full_path_filename, verbose_flag,
         data_format_local_copy, output_directory_local_copy,
         temp_original_working_directory, aux_file_local_copy,
         database_file_local_copy, write_database_info_flag, minmax_flag );

         if( input_filter_return_status == EXIT_SUCCESS )
         {
            successfull_conversion_count++;
         }

      }

   }

   return EXIT_SUCCESS;

}

int validate_input_files(
      const char **model_name,
      const char **input_directory,
      int verbose_flag,
      const char **input_file_array_base_name,
      const char **input_file_array_full_path,
      int file_count,
      const char **data_format,
      const char **output_directory,
      const char **aux_file,
      const char **database_file,
      int write_database_info_flag,
      int minmax_flag)
{

   extern int input_filter(
         char *model_name,
         char *input_file_name,
         int verbose_flag,
         char *data_format,
         char *output_directory,
         char *original_working_directoryPtr,
         char *aux_file,
         char *database_file,
         int write_database_info_flag,
         int minmax_flag);

   int i;

   char *root = "/";
   char *cdf_file_extension = ".cdf";

   char current_working_directory_temp[MAX_ARG_LENGTH];

   /* reserve space for local copy of arguments initially passed to function */

   static char model_name_local_copy[MAX_ARG_LENGTH];
   char input_directory_local_copy[MAX_ARG_LENGTH];
   char input_file_array_base_name_local_copy[file_count][MAX_ARG_LENGTH];
   char input_file_array_full_path_local_copy[file_count][MAX_ARG_LENGTH];
   static char data_format_local_copy[MAX_ARG_LENGTH];
   char output_directory_local_copy[MAX_ARG_LENGTH];
   static char aux_file_local_copy[MAX_ARG_LENGTH];
   static char database_file_local_copy[MAX_ARG_LENGTH];

   /***** make local copies of function argument values to be safe *****/

   strcpy(
         model_name_local_copy,
         model_name[0]);
   strcpy(
         input_directory_local_copy,
         input_directory[0]);
   strcpy(
         data_format_local_copy,
         data_format[0]);
   strcpy(
         output_directory_local_copy,
         output_directory[0]);
   strcpy(
         aux_file_local_copy,
         aux_file[0]);
   strcpy(
         database_file_local_copy,
         database_file[0]);

   /* build array copies that contain list of command line input file_names */

   for (i = 0; i < file_count; i++)
   {
      strcpy(
            input_file_array_base_name_local_copy[ i ],
            input_file_array_base_name[i]);
      strcpy(
            input_file_array_full_path_local_copy[ i ],
            input_file_array_full_path[i]);
      /*
       printf("DEBUG\t input_file_array_base_name_local_copy[ %d ] = "
       "%s\ninput_file_array_full_path_local_copy[ %d ] = %s\n", i,
       input_file_array_base_name_local_copy[ i ], i,
       input_file_array_full_path_local_copy[ i ] );
       */
   }

   for (i = 0; i < file_count; i++)
   {
      if (file_count > 0) /* if command line input files actually exist */
      {

         /** if file is a .cdf file, lets save ourselves the waisted time and
          * filter out here **/

         if (strstr(
               input_file_array_full_path_local_copy[i],
               cdf_file_extension) != NULL)
         {
            printf(
                  "*WARNING: %s\t Current input file --->%s<--- has suffix "
                     "--->%s<---...ignoring\n",
__FILE__            ,
            input_file_array_full_path_local_copy[i],
            cdf_file_extension );
         }
         else
         {
            /* check if each element of input_file_array to see if it exists and
             * is readable and call the read routine for each */

            if(verbose_flag)
            {  printf("original input (%d)\t%s\n", i + 1 ,
               input_file_array_base_name_local_copy[i] );}

            if( ! access( input_file_array_full_path_local_copy[i], R_OK) )
            {
               if( verbose_flag )
               {  printf("***** calling input_filter( %s, %s, %d, %s, %s, %s, "
                  "%s, %s, %d, %d ); *****\n", model_name_local_copy,
                  input_file_array_full_path_local_copy[i], verbose_flag,
                  data_format_local_copy, output_directory_local_copy,
                  original_working_directoryPtr, aux_file_local_copy,
                  database_file_local_copy, write_database_info_flag,
                  minmax_flag );
               }

               /** keep track of the total number of reads **/

               total_input_file_count++;

               input_filter_return_status = input_filter( model_name_local_copy,
               input_file_array_full_path_local_copy[i], verbose_flag,
               data_format_local_copy, output_directory_local_copy,
               original_working_directoryPtr, aux_file_local_copy,
               database_file_local_copy, write_database_info_flag,
               minmax_flag );

               if( input_filter_return_status == EXIT_SUCCESS )
               {
                  successfull_conversion_count++;
               }

            }
            else
            {
               printf("ERROR: cannot read \t%s\n",
               input_file_array_full_path_local_copy[i] );
               perror( input_file_array_full_path_local_copy[i] );
            }

         }

      }

      /** keep track of each sucessfull read **/

   }

   return EXIT_FAILURE;

}

int validate_aux_file(
      const char **aux_file,
      const char **model_name,
      int verbose_flag)
{

   /** NOTE:  add smart verification of aux file **/

   /****************************************************************************
    ***    Current Models That Require an Auxiliary File Separate from       ***
    ***    the data files.  ie. UCLA-GGCM has 3df data files and a general   ***
    ***    grid file used by all data files.                                 ***
    ***                                                                      ***
    *** ( 1 ) ucla_ggcm - requires *.grid file in addition to 3df data files ***
    ***                                                                      ***
    *** ( 2 ) open_ggcm - requires *.grid file in addition to 3df data files ***
    ***                                                                      ***
    *** ( 3 ) ctip - requires .h header file in addition to data files       ***
    ***                                                                      ***
    *** ( 4 ) ENLIL - NOTE that enlil has additional ev files aside from     ***
    ***               the time files.  They are not required per se but      ***
    ***               we'll look for them automatically inside the input     ***
    ***               directory                                              ***
    ***                                                                      ***
    *** ( 5 ) MAS - NOTE that MAS stores each variable in a sepeartae *.hdf  ***
    ***             file. Kameleon will look for the files inside the input  ***
    ***             or specified auxdirectory                                ***
    ***                                                                      ***
    ***************************************************************************/

   /** next line essentially makes sure model name == to any names listed in
    * logical test **/

   /* if true the user's specified model does not require an auxiliary file
    * return 1 */

   if (strcmp(
         model_name[0],
         "ucla_ggcm") && strcmp(
         model_name[0],
         "ctip") && strcmp(
         model_name[0],
         "open_ggcm") && strcmp(
         model_name[0],
         "enlil") && strcmp(
         model_name[0],
         "msfc_tvm") && strcmp(
         model_name[0],
         "mas"))
   {
      return EXIT_SUCCESS;
   }
   else /* user's specified model requires the auxiliary file retrun 0 */
   {

      fprintf( stderr, "\nERROR:\tA Valid Auxiliary File/Directory Must Be "
      "Specified for the %s model. ie.\n", model_name[0]);

      fprintf( stderr, "%-20s%-20s\n", "UCLA_GGCM/OpenGGCM", ".grid file");
      fprintf( stderr, "%-20s%-20s\n", "CTIP", "3d_ctip.h file");
      fprintf( stderr, "%-20s%-20s\n", "ENLIL", "DIRECTORY containing ev*.nc "
      "files");
      fprintf( stderr, "%-20s%-20s\n", "MSFC_TVM", "DIRECTORY containing "
      "*bl.fits, *bt.fits, & *raz.fits files");
      fprintf( stderr, "%-20s%-20s\n", "MAS", "DIRECTORY containing *.hdf files"
      "since each variable is stored in its own seperate file");

      return EXIT_FAILURE;
   }

}
