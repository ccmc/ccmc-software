/*******************************************************************************
 *                                                                             *
 *      NAME:           read_mas.c                                             *
 *                                                                             *
 *      VERSION:        0.1                                                    *
 *                                                                             *
 *      AUTHOR:         Marlo M. H. Maddox                                     *
 *                      NASA-GSFC-AETD-ISD-ADMA(587)/CCMC(612.3)               *
 *                      Marlo.Maddox@nasa.gov                                  *
 *                      (301) 286 - 5202                                       *
 *                                                                             *
 *      PURPOSE:        Custom read routine for MAS model output.              *
 *                                                                             *
 *      INPUTS:         char *input_file_name, char *aux_file,                 *
 *                      int verbose_flag, int minmax_flag                      *
 *                                                                             *
 *      OUTPUTS:        mas variable values stored in main memory              *
 *                                                                             *
 *      FUNCTIONS:      a few                                                  *
 *      VARIABLES:      a few more                                             *
 *                                                                             *
 *      MODIFICATION                                                           *
 *      HISTORY:                                                               *
 *                                                                             *
 *      2006 December 14th      Maddox, Marlo                                  *
 *                              Initial Development Started                    *
 *      2007 February 12th      Maddox, Marlo                                  *
 *                              Passing variable pointer to read_3d_mas_file   *
 *                              function not working as expected...tweaking    *
 *      2007 February 26th      Maddox, Marlo                                  *
 *                              Had r and theta reversed or dim0 and dim2...   *
 *                              changing where applicable                      *
 *      2007 April 6th          Maddox, Marlo                                  *
 *                              adding support for offset position arrays      *
 *                                                                             *
 *      2008 February 21        Maddox, Marlo                                  *
 *                              Changing the #ifdef HDF to                     *
 *                              #ifdef KAMELEON_WITH_HDF                       *
 *                              to make sure the macro isn't confilting with   *
 *                              external macros defined by other librariers    *
 *                                                                             *
 *                              ...also for some reason MAX_NC_NAME and        *
 *                              MAX_VAR_DIMS cannot be found in the hdf        *
 *                              include directory.  When using HDF4 and netCDF *
 *                              at the same time, there are a buch of config   *
 *                              issues that may arise.  Lets deal with it here *
 *                              by setting these values ourselves if there not *
 *                              defined - these are specified in the           *
 *                              read_3d_mas_file() routine                     *
 *                                                                             *
 *     2011 June 6		Maddox, Marlo				       *
 *     				Updateing for new phi+1 grid		       *
 *     									       *
 *                                                                             *
 ******************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <unistd.h>
#include <sys/types.h>
#include <dirent.h>
#include <sys/param.h>

#include "read_mas.h"
#include "current_mas_step.h"

#define MAX_STEPS 100
#define MAX_FILE_NAME_LENGTH 2500
#define MAX_BASE_FILE_NAME_LENGTH 25
#define MAX_FILE_NAME_FIELD_LENGTH 5

#define DEBUG_FLAG 0

#ifdef KAMELEON_WITH_HDF
#include "mfhdf.h"
#endif

int auto_read_mas(char *, char *, char *, int, char *, int);
int read_3d_mas_file(char *, char *, int);
extern int linear_minmax_search(float *, int, float *, float *);

static int *flag_t, *flag_p, *flag_rho, *flag_vr, *flag_vp, *flag_vt, *flag_jr,
      *flag_jp, *flag_jt, *flag_br, *flag_bp, *flag_bt;

/* use these to determine if we need to read position values from the file */
static int *flag_pos1, *flag_pos2, *flag_pos3, *flag_pos1_plus1,
      *flag_pos2_plus1, *flag_pos3_plus1;

static int is_first_call=1; /** is this the first call to read_mas **/
static int current_step;
static int *step_done_array= NULL;

/*******************************************************************************
 *                     READ MAS MAIN                                           *
 ******************************************************************************/

/** NOTE: fakeDim2 is r, fakeDim1 is theta, and fakeDim0 is phi **/

int read_mas(
      char *input_file_name,
      char *aux_file,
      int verbose_flag,
      int minmax_flag)
{

   /* If the Compiler macro KAMELEON_WITH_HDF is not defined, that means the makefile
    * determined that required hdf libraries were not available.  Instead of
    * shutting down the entire converter Lets just disable any read routines
    * that depend on the HDF library */

#ifndef KAMELEON_WITH_HDF
   printf("\n\n!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!\n");
   printf(
         "!! WARNING: from %s line [%d]. Conversion Software was not compiled and linked with the required HDF libraries ( libmfhdf.a  and it's dependencies: libdf.a, libsz.a, libjpeg.a, and libz.a ).  No .hdf HDF files will be ingested... \n",
         __FILE__, __LINE__ );
         return EXIT_FAILURE;
#else

         int i, status, number_of_elements1, number_of_elements2,
         number_of_elements3         , number_of_elements4, number_of_elements5, number_of_elements6,
         number_of_elements7         ;

         static int call_count=0;

         char current_variable_string[MAX_FILE_NAME_FIELD_LENGTH];

         char *hdf_suffix = ".hdf";
         char *dot = ".";
         char *dotdot = "..";
         char *slash = "/";

         char base_file_name[MAX_FILE_NAME_LENGTH],
         base_file_name_no_extension[MAX_BASE_FILE_NAME_LENGTH],
         extension[MAX_BASE_FILE_NAME_LENGTH], *string_token;
         char file_name_variable_field[MAX_FILE_NAME_FIELD_LENGTH];
         char file_name_step_number_field[MAX_FILE_NAME_FIELD_LENGTH];

         /********** some directory processing specific variables ***/

         DIR *directory_pointer;
         struct dirent *directory_struct;
         /** need to retain values between calls to input_filter() and beyond **/
         static int number_directory_entries=0;
         char *current_working_directory_tempPtr;

         char current_working_directory_temp[MAX_FILE_NAME_LENGTH];
         char current_working_directory[MAX_FILE_NAME_LENGTH];
         static char full_path_filename[MAX_FILE_NAME_LENGTH];
         char filename[MAX_FILE_NAME_LENGTH];

         if ( DEBUG_FLAG)
         printf("\n\n\nDEBUG\tEntering read_mas()\n");

         /**** if this is the first call, we need to initialize our step_done_array
          * and clear all elemets bitwise to zero ****/

         if (is_first_call)
         {
            if ( DEBUG_FLAG)
            printf("\nDEBUG\tFIRST CALL is %d ...initializing memory\n",
                  is_first_call);

            step_done_array = calloc( MAX_STEPS, sizeof( int ));
            flag_t = calloc( MAX_STEPS, sizeof( int ));
            flag_p = calloc( MAX_STEPS, sizeof( int ));
            flag_rho = calloc( MAX_STEPS, sizeof( int ));
            flag_vr = calloc( MAX_STEPS, sizeof( int ));
            flag_vp = calloc( MAX_STEPS, sizeof( int ));
            flag_vt = calloc( MAX_STEPS, sizeof( int ));
            flag_br = calloc( MAX_STEPS, sizeof( int ));
            flag_bp = calloc( MAX_STEPS, sizeof( int ));
            flag_bt = calloc( MAX_STEPS, sizeof( int ));
            flag_jr = calloc( MAX_STEPS, sizeof( int ));
            flag_jp = calloc( MAX_STEPS, sizeof( int ));
            flag_jt = calloc( MAX_STEPS, sizeof( int ));
            flag_pos1 = calloc( MAX_STEPS, sizeof( int ));
            flag_pos2 = calloc( MAX_STEPS, sizeof( int ));
            flag_pos3 = calloc( MAX_STEPS, sizeof( int ));
            flag_pos1_plus1 = calloc( MAX_STEPS, sizeof( int ));
            flag_pos2_plus1 = calloc( MAX_STEPS, sizeof( int ));
            flag_pos3_plus1 = calloc( MAX_STEPS, sizeof( int ));

         }
         if (step_done_array == NULL)
         {
            printf("%s ERROR: CALLOC FAILED for step_done_array\n",
                  __FILE__ ); return EXIT_FAILURE;}

         /*** add checks for the rest before deployment ***/

         /****************************************************************************
          *
          * lets go ahead and rely on the hdf file naming convention and only proceed
          * if the current file name ends with .hdf *
          *
          ***************************************************************************/

         if( strstr( input_file_name, hdf_suffix ) == NULL )
         {
            printf("*WARNING: %s\t Current input file --->%s<--- does not match the standard suffix --->%s<---...ignoring\n", __FILE__, input_file_name, hdf_suffix );
            return EXIT_FAILURE;
         }

         /********** extract base name from file name using subsequent calls to strtok **********/

         strcpy( base_file_name, input_file_name );
         base_file_name[ strlen( input_file_name ) ] = '\0';

         string_token = strtok( base_file_name, slash );

         while( string_token != NULL)
         {
            strcpy( base_file_name , string_token );
            if( verbose_flag )
            {  printf("extracting file name from path ...base_file_name = %s\n", base_file_name );}
            string_token = strtok( NULL, slash );
         }

         /***** base_file_name should now contain base file name without path ****/

         if( verbose_flag )
         {  printf("resulting base_file_name = %s\n", base_file_name );}

         /* we now want to sepearte the current file name from it's extension */

         strcpy( base_file_name_no_extension, base_file_name );
         base_file_name_no_extension[ strlen( base_file_name ) ] = '\0';

         /* extract left hand string from .hdf extension using subsequent calls to
          * strtok */

         string_token = strtok( base_file_name_no_extension, dot );
         strcpy( base_file_name_no_extension , string_token );

         while( string_token != NULL )
         {
            strcpy( extension , string_token );
            if( verbose_flag )
            {  printf("extracting extension ...extension = %s\n", extension );}
            string_token = strtok( NULL, dot );
         }

         /************ base_file_name_no_extension should now contain base file
          * name without the .hdf extension ***********/

         /* if( verbose_flag )
          * {
          * printf("resulting base_file_name_no_extension = ->%s<- of length %d\n",
          * base_file_name_no_extension, strlen( base_file_name_no_extension ) );
          * } */

         if( verbose_flag )
         {  printf("resulting extension = %s\n", extension );}

         /** if the extension isn't hdf, we have to assume the file name isn't of
          * the expected form **/

         if( strncmp( extension, "hdf", 3 ) !=0 )
         {
            fprintf( stderr, "%s *WARNING: %s doesn't have the expected namming format. Ignoring...\n", __FILE__, base_file_name );
            return EXIT_FAILURE;
         }

         /**** based on the length of the base file name without the extension,
          * copy the variable name field into the current_variable_string **/

         if( strlen( base_file_name_no_extension ) == 6 ) /** ie rho001 **/
         {
            strncpy( file_name_variable_field, base_file_name_no_extension, 3 );

            /** the above string copy didn't seem to terminate as expected, lets force here **/

            file_name_variable_field[3] = '\0';

            if( verbose_flag )
            {  printf("extracting variable name field from base_file_name_no_extension %s = %s\n", base_file_name_no_extension, file_name_variable_field );}

            string_token = strtok( base_file_name_no_extension, file_name_variable_field );

            while( string_token != NULL)
            {
               strcpy( file_name_step_number_field , string_token );
               if( verbose_flag )
               {  printf("extracting step number field from base_file_name_no_extension %s = %s\n", base_file_name_no_extension, file_name_step_number_field );}
               string_token = strtok( NULL, file_name_variable_field );
            }
         }
         else if( strlen( base_file_name_no_extension ) == 5 ) /** ie bp001 **/
         {
            printf("DEBUG\t strlen == 5 seperate first two characters...\n");

            strncpy( file_name_variable_field, base_file_name_no_extension, 2 );

            /** the above string copy didn't seem to terminate as expected, lets
             * force here **/

            file_name_variable_field[2] = '\0';

            if( verbose_flag )
            {  printf("extracting variable name field from base_file_name_no_extension %s = %s\n", base_file_name_no_extension, file_name_variable_field );}

            string_token = strtok( base_file_name_no_extension, file_name_variable_field );

            while( string_token != NULL)
            {
               strcpy( file_name_step_number_field , string_token );
               if( verbose_flag )
               {  printf("extracting step number field from base_file_name_no_extension %s = %s\n", base_file_name_no_extension, file_name_step_number_field );}
               string_token = strtok( NULL, file_name_variable_field );
            }
         }
         else if( strlen( base_file_name_no_extension ) == 4 ) /** ie t001 **/
         {
            strncpy( file_name_variable_field, base_file_name_no_extension, 1 );

            /** the above string copy didn't seem to terminate as expected, lets force here **/

            file_name_variable_field[1] = '\0';

            if( verbose_flag )
            {  printf("extracting variable name field from base_file_name_no_extension %s = %s\n", base_file_name_no_extension, file_name_variable_field );}

            string_token = strtok( base_file_name_no_extension, file_name_variable_field );

            while( string_token != NULL)
            {
               strcpy( file_name_step_number_field , string_token );
               if( verbose_flag )
               {  printf("extracting step number field from base_file_name_no_extension %s = %s\n", base_file_name_no_extension, file_name_step_number_field );}
               string_token = strtok( NULL, file_name_variable_field );
            }
         }
         else
         {
            fprintf( stderr, "%s *WARNING: %s doesn't have the expected namming format. Ignoring...\n", __FILE__, base_file_name );
            return EXIT_FAILURE;
         }

         /**************** we should now have seprate file name field elements
          * identifying the variable name and step number: file_name_variable_field
          * & file_name_step_number_field ***/

         current_step = atoi( file_name_step_number_field );

         /** set the current_mas_step variable defined in read_mas.h, this will be
          * used to recontruct a name for the cdf file **/

         current_mas_step = current_step;

         if( DEBUG_FLAG )
         {  printf("DEBUG:\tcurrent file to process = %s\n\tcurrent variable = ->%s<-\n\tcurrent step number = %d\n\twill now search for and process all files with step %s\n", input_file_name, file_name_variable_field, current_step, file_name_step_number_field );}

         /**** we should now see if the current variable and step has been
          * processed. On first match, process current file and all registered
          * variables for current step. *****/

         if( strcmp( file_name_variable_field, "bp" ) == 0 )
         {
            printf("DEBUG\t\"bp\" block\n");

            if( flag_bp[current_step] == current_step )
            {
               if( DEBUG_FLAG ) printf("%s already processed, ignoring %s\n", file_name_variable_field, input_file_name );
               return EXIT_FAILURE;
            }

            status = auto_read_mas( input_file_name, file_name_variable_field, file_name_step_number_field, current_step, aux_file, verbose_flag );

         }
         else if( strcmp( file_name_variable_field, "br" ) == 0 )
         {
            printf("DEBUG\t\"br\" block\n");

            if( flag_br[current_step] == current_step )
            {
               if( DEBUG_FLAG ) printf("%s already processed, ignoring %s\n", file_name_variable_field, input_file_name );
               return EXIT_FAILURE;
            }

            status = auto_read_mas( input_file_name, file_name_variable_field, file_name_step_number_field, current_step, aux_file, verbose_flag );

         }
         else if( strcmp( file_name_variable_field, "bt" ) == 0 )
         {
            printf("DEBUG\t\"bt\" block\n");

            if( flag_bt[current_step] == current_step )
            {
               if( DEBUG_FLAG ) printf("%s already processed, ignoring %s\n", file_name_variable_field, input_file_name );
               return EXIT_FAILURE;
            }

            status = auto_read_mas( input_file_name, file_name_variable_field, file_name_step_number_field, current_step, aux_file, verbose_flag );

         }
         else if( strcmp( file_name_variable_field, "vp" ) == 0 )
         {
            printf("DEBUG\t\"vp\" block\n");

            if( flag_vp[current_step] == current_step )
            {
               if( DEBUG_FLAG ) printf("%s already processed, ignoring %s\n", file_name_variable_field, input_file_name );
               return EXIT_FAILURE;
            }

            status = auto_read_mas( input_file_name, file_name_variable_field, file_name_step_number_field, current_step, aux_file, verbose_flag );

         }
         else if( strcmp( file_name_variable_field, "vr" ) == 0 )
         {
            printf("DEBUG\t\"vr\" block\n");

            if( flag_vr[current_step] == current_step )
            {
               if( DEBUG_FLAG ) printf("%s already processed, ignoring %s\n", file_name_variable_field, input_file_name );
               return EXIT_FAILURE;
            }

            status = auto_read_mas( input_file_name, file_name_variable_field, file_name_step_number_field, current_step, aux_file, verbose_flag );

         }
         else if( strcmp( file_name_variable_field, "vt" ) == 0 )
         {
            printf("DEBUG\t\"vt\" block\n");

            if( flag_vt[current_step] == current_step )
            {
               if( DEBUG_FLAG ) printf("%s already processed, ignoring %s\n", file_name_variable_field, input_file_name );
               return EXIT_FAILURE;
            }

            status = auto_read_mas( input_file_name, file_name_variable_field, file_name_step_number_field, current_step, aux_file, verbose_flag );

         }
         else if( strcmp( file_name_variable_field, "jp" ) == 0 )
         {
            printf("DEBUG\t\"jp\" block\n");

            if( flag_jp[current_step] == current_step )
            {
               if( DEBUG_FLAG ) printf("%s already processed, ignoring %s\n", file_name_variable_field, input_file_name );
               return EXIT_FAILURE;
            }

            status = auto_read_mas( input_file_name, file_name_variable_field, file_name_step_number_field, current_step, aux_file, verbose_flag );

         }
         else if( strcmp( file_name_variable_field, "jr" ) == 0 )
         {
            printf("DEBUG\t\"jr\" block\n");

            if( flag_jr[current_step] == current_step )
            {
               if( DEBUG_FLAG ) printf("%s already processed, ignoring %s\n", file_name_variable_field, input_file_name );
               return EXIT_FAILURE;
            }

            status = auto_read_mas( input_file_name, file_name_variable_field, file_name_step_number_field, current_step, aux_file, verbose_flag );

         }
         else if( strcmp( file_name_variable_field, "jt" ) == 0 )
         {
            printf("DEBUG\t\"jt\" block\n");

            if( flag_jt[current_step] == current_step )
            {
               if( DEBUG_FLAG ) printf("%s already processed, ignoring %s\n", file_name_variable_field, input_file_name );
               return EXIT_FAILURE;
            }

            status = auto_read_mas( input_file_name, file_name_variable_field, file_name_step_number_field, current_step, aux_file, verbose_flag );

         }
         else if( strcmp( file_name_variable_field, "rho" ) == 0 )
         {
            printf("DEBUG\t\"rho\" block\n");

            if( flag_rho[current_step] == current_step )
            {
               if( DEBUG_FLAG ) printf("%s already processed, ignoring %s\n", file_name_variable_field, input_file_name );
               return EXIT_FAILURE;
            }

            status = auto_read_mas( input_file_name, file_name_variable_field, file_name_step_number_field, current_step, aux_file, verbose_flag );

         }
         else if( strcmp( file_name_variable_field, "p" ) == 0 )
         {
            printf("DEBUG\t\"p\" block\n");

            if( flag_p[current_step] == current_step )
            {
               if( DEBUG_FLAG ) printf("%s already processed, ignoring %s\n", file_name_variable_field, input_file_name );
               return EXIT_FAILURE;
            }

            status = auto_read_mas( input_file_name, file_name_variable_field, file_name_step_number_field, current_step, aux_file, verbose_flag );

         }
         else if( strcmp( file_name_variable_field, "t" ) == 0 )
         {
            printf("DEBUG\t\"t\" block\n");

            if( flag_t[current_step] == current_step )
            {
               if( DEBUG_FLAG ) printf("%s already processed, ignoring %s\n", file_name_variable_field, input_file_name );
               return EXIT_FAILURE;
            }

            status = auto_read_mas( input_file_name, file_name_variable_field, file_name_step_number_field, current_step, aux_file, verbose_flag );

         }
         else /*** variable is not recognized ***/
         {
            printf("%s WARNING: Variable %s not recognized. Ignoring %s\n", __FILE__, file_name_variable_field, input_file_name );
            return EXIT_FAILURE;
         }

         /* calcluate actual min/max values for each ariable unless -nominmax f
          * lag was specified *************/

         /* add more error handling for each linear_minmax_search function call */

         number_of_elements1 = fakedim0_plus1 * fakedim1_plus1 * fakedim2_plus1; /** for variables rho, t, p **/
         number_of_elements2 = fakedim0_plus1 * fakedim1_plus1 * fakedim2; /** vr, jr **/
         number_of_elements3 = fakedim0_plus1 * fakedim1 * fakedim2_plus1; /** j_theta, v_theata **/
         number_of_elements4 = fakedim0_plus1 * fakedim1 * fakedim2; /** b_phi **/

         number_of_elements5 = fakedim0 * fakedim1_plus1 * fakedim2_plus1; /** v_phi, and j_phi **/
         number_of_elements6 = fakedim0 * fakedim1_plus1 * fakedim2; /** b_theta **/
         number_of_elements7 = fakedim0 * fakedim1 * fakedim2_plus1; /** br **/


         printf("total number_of_elements1 = %d * %d * %d = %d\t\tfor variables rho, t, p \n", 		fakedim0_plus1, fakedim1_plus1,	fakedim2_plus1, number_of_elements1 );
         printf("total number_of_elements2 = %d * %d * %d = %d\t\tfor variables vr, jr,\n", 		fakedim0_plus1, fakedim1_plus1, fakedim2, 	number_of_elements2 );
         printf("total number_of_elements3 = %d * %d * %d = %d\t\tfor variables j_theta, v_theata\n", 	fakedim0_plus1, fakedim1, 	fakedim2_plus1, number_of_elements3 );
         printf("total number_of_elements4 = %d * %d * %d = %d\t\tfor variables b_phi\n", 		fakedim0_plus1, fakedim1, 	fakedim1, 	number_of_elements4 );

         printf("total number_of_elements5 = %d * %d * %d = %d\t\tfor variables v_phi, and j_phi \n", 	fakedim0, 	fakedim1_plus1, fakedim2_plus1, number_of_elements5 );
         printf("total number_of_elements6 = %d * %d * %d = %d\t\tfor variables b_theta\n", 		fakedim0, 	fakedim1_plus1, fakedim2, 	number_of_elements6 );
         printf("total number_of_elements7 = %d * %d * %d = %d\t\tfor variables br\n", 			fakedim0, 	fakedim1, 	fakedim2_plus1, number_of_elements7 );

         if( DEBUG_FLAG )
         {
            for( i=0; i< fakedim0; i++ ) printf("phi[%d] = %f\n", i, dim0[i] );
            for( i=0; i< fakedim1; i++ ) printf("theta[%d] = %f\n", i, dim1[i] );
            for( i=0; i< fakedim2; i++ ) printf("r[%d] = %f\n", i, dim2[i] );
            for( i=0; i< fakedim0_plus1; i++ ) printf("phi12[%d] = %f\n", i, dim0_plus1[i] );
            for( i=0; i< fakedim1_plus1; i++ ) printf("theta1[%d] = %f\n", i, dim1_plus1[i] );
            for( i=0; i< fakedim2_plus1; i++ ) printf("r1[%d] = %f\n", i, dim2_plus1[i] );
         }

         if( ! minmax_flag ) /*** if -nominmax option was NOT specified ***/
         {

            if( verbose_flag )
            {  printf("\ncalculating actual minimum & maximum values for each variable ...\n");}

            if( verbose_flag )
            {  printf("%-25s%-25s%-25s\n", "", "min", "max");}

            linear_minmax_search( dim0, fakedim0, &phi_actual_min, &phi_actual_max );
            if( verbose_flag )
            {  printf("%-25s%-25g%-25g\n", "phi", phi_actual_min, phi_actual_max );}

            linear_minmax_search( dim1, fakedim1, &theta_actual_min, &theta_actual_max );
            if( verbose_flag )
            {  printf("%-25s%-25g%-25g\n", "theta", theta_actual_min, theta_actual_max );}

            linear_minmax_search( dim2, fakedim2, &r_actual_min, &r_actual_max );
            if( verbose_flag )
            {  printf("%-25s%-25g%-25g\n", "r", r_actual_min, r_actual_max );}


            linear_minmax_search( dim0_plus1, fakedim0_plus1, &phi_plus1_actual_min, &phi_plus1_actual_max );
                        if( verbose_flag )
                        {  printf("%-25s%-25g%-25g\n", "phi2", phi_plus1_actual_min, phi_plus1_actual_max );}


            linear_minmax_search( dim1_plus1, fakedim1_plus1, &theta_plus1_actual_min, &theta_plus1_actual_max );
            if( verbose_flag )
            {  printf("%-25s%-25g%-25g\n", "theta2", theta_plus1_actual_min, theta_plus1_actual_max );}

            linear_minmax_search( dim2_plus1, fakedim2_plus1, &r_plus1_actual_min, &r_plus1_actual_max );
            if( verbose_flag )
            {  printf("%-25s%-25g%-25g\n", "r2", r_plus1_actual_min, r_plus1_actual_max );}

            linear_minmax_search( t, number_of_elements1, &t_actual_min, &t_actual_max );
            if( verbose_flag )
            {  printf("%-25s%-25g%-25g\n", "t", t_actual_min, t_actual_max );}

            linear_minmax_search( p, number_of_elements1, &p_actual_min, &p_actual_max );
            if( verbose_flag )
            {  printf("%-25s%-25g%-25g\n", "p", p_actual_min, p_actual_max );}

            linear_minmax_search( rho, number_of_elements1, &rho_actual_min, &rho_actual_max );
            if( verbose_flag )
            {  printf("%-25s%-25g%-25g\n", "rho", rho_actual_min, rho_actual_max );}

            linear_minmax_search( vr, number_of_elements2, &vr_actual_min, &vr_actual_max );
            if( verbose_flag )
            {  printf("%-25s%-25g%-25g\n", "vr", vr_actual_min, vr_actual_max );}

            linear_minmax_search( vp, number_of_elements1, &vp_actual_min, &vp_actual_max );
            if( verbose_flag )
            {  printf("%-25s%-25g%-25g\n", "vp", vp_actual_min, vp_actual_max );}

            linear_minmax_search( vt, number_of_elements3, &vt_actual_min, &vt_actual_max );
            if( verbose_flag )
            {  printf("%-25s%-25g%-25g\n", "vt", vt_actual_min, vt_actual_max );}

            linear_minmax_search( br, number_of_elements3, &br_actual_min, &br_actual_max );
            if( verbose_flag )
            {  printf("%-25s%-25g%-25g\n", "br", br_actual_min, br_actual_max );}

            linear_minmax_search( bp, number_of_elements4, &bp_actual_min, &bp_actual_max );
            if( verbose_flag )
            {  printf("%-25s%-25g%-25g\n", "bp", bp_actual_min, bp_actual_max );}

            linear_minmax_search( bt, number_of_elements2, &bt_actual_min, &bt_actual_max );
            if( verbose_flag )
            {  printf("%-25s%-25g%-25g\n", "bt", bt_actual_min, bt_actual_max );}

            linear_minmax_search( jr, number_of_elements2, &jr_actual_min, &jr_actual_max );
            if( verbose_flag )
            {  printf("%-25s%-25g%-25g\n", "jr", jr_actual_min, jr_actual_max );}

            linear_minmax_search( jp, number_of_elements1, &jp_actual_min, &jp_actual_max );
            if( verbose_flag )
            {  printf("%-25s%-25g%-25g\n", "jp", jp_actual_min, jp_actual_max );}

            linear_minmax_search( jt, number_of_elements3, &jt_actual_min, &jt_actual_max );
            if( verbose_flag )
            {  printf("%-25s%-25g%-25g\n", "jt", jt_actual_min, jt_actual_max );}

         }

#endif

return   EXIT_SUCCESS;

}

int auto_read_mas(
      char *input_file_name,
      char *variable_name,
      char *step_number_field,
      int step_number,
      char *aux_file,
      int verbose_flag)
{

   char current_base_name[MAX_FILE_NAME_LENGTH];
   char current_file_name[MAX_FILE_NAME_LENGTH];
   int string_len, char_location;
   char *slash = "/";
   char *hdf_extension = ".hdf";

   char *slash_location_pointer;

   int read_status;

   /** build the current file name on the fly using the aux_file argument which
    * is the full directory path **/

   /** first check to see if the aux_file string contains a trailing forward
    * slash character **/

   string_len = strlen(aux_file);

   if ( DEBUG_FLAG)
      printf("last char of %s = %c\n", aux_file, aux_file[string_len - 1]);

   /** if there is NOT a trailing slash on the end of the path, add it and move
    * the null terminator one space to the left **/

   /* char_location = strrchr( aux_file, slash ); */
   /*slash_location_pointer = strrchr( aux_file, slash );

    if( slash_location_pointer++ == NULL ) printf("the slash_location_pointer is
    the last character...\n");
    */

   strcpy(current_base_name, aux_file);

   /** last character in path is NOT a trailing backslash **/

   if (current_base_name[string_len - 1] != '/')
   {
      if ( DEBUG_FLAG)
         printf(
               "current_base_name->%s does NOT have trailing backslash.  Adding...\nchar_location %d != string_len - 1 %d\n",
               current_base_name,
               char_location,
               string_len);

      strcat(current_base_name, slash);

   }

   if ( DEBUG_FLAG)
      printf("current_base_name = %s\n", current_base_name);

   /*** for any step number passed here to auto_read_mas, read all existing
    * variables for given step number ***/

   if (flag_bp[step_number] != step_number)
   {
      /*** set/reset current_file_name ***/

      strcpy(current_file_name, current_base_name);

      /** now add the variable name string to the path **/

      strcat(current_file_name, "bp");

      /** now add the current step number string to the path **/

      strcat(current_file_name, step_number_field);

      /** now add the file name extension string to the path **/

      strcat(current_file_name, hdf_extension);

      if (verbose_flag)
         printf("searching for %s\n", current_file_name);

      /*** verify file existence and access, then call read_3d_mas_file() ***/

      if (access(current_file_name, R_OK) )
      {
         printf(
               "\nERROR: will not be able to read input from %s\n",
               current_file_name);
         return EXIT_FAILURE;
      }
      else
      {
         read_status = read_3d_mas_file(current_file_name, "bp", verbose_flag);

         if (read_status == EXIT_SUCCESS)
         {
            flag_bp[step_number] = step_number;
         }
      }

   }
   if (flag_bt[step_number] != step_number)
   {
      /*** set/reset current_file_name ***/

      strcpy(current_file_name, current_base_name);

      /** now add the variable name string to the path **/

      strcat(current_file_name, "bt");

      /** now add the current step number string to the path **/

      strcat(current_file_name, step_number_field);

      /** now add the file name extension string to the path **/

      strcat(current_file_name, hdf_extension);

      if (verbose_flag)
         printf("searching for %s\n", current_file_name);

      /*** verify file existence and access, then call read_3d_mas_file() ***/

      if (access(current_file_name, R_OK) )
      {
         printf(
               "\nERROR: will not be able to read input from %s\n",
               current_file_name);
         return EXIT_FAILURE;
      }
      else
      {
         read_status = read_3d_mas_file(current_file_name, "bt", verbose_flag);

         if (read_status == EXIT_SUCCESS)
         {
            flag_bt[step_number] = step_number;
         }
      }

   }
   if (flag_br[step_number] != step_number)
   {
      /*** set/reset current_file_name ***/

      strcpy(current_file_name, current_base_name);

      /** now add the variable name string to the path **/

      strcat(current_file_name, "br");

      /** now add the current step number string to the path **/

      strcat(current_file_name, step_number_field);

      /** now add the file name extension string to the path **/

      strcat(current_file_name, hdf_extension);

      if (verbose_flag)
         printf("searching for %s\n", current_file_name);

      /*** verify file existence and access, then call read_3d_mas_file() ***/

      if (access(current_file_name, R_OK) )
      {
         printf(
               "\nERROR: will not be able to read input from %s\n",
               current_file_name);
         return EXIT_FAILURE;
      }
      else
      {
         read_status = read_3d_mas_file(current_file_name, "br", verbose_flag);

         if (read_status == EXIT_SUCCESS)
         {
            flag_br[step_number] = step_number;
         }
      }
   }
   if (flag_jp[step_number] != step_number)
   {
      /*** set/reset current_file_name ***/

      strcpy(current_file_name, current_base_name);

      /** now add the variable name string to the path **/

      strcat(current_file_name, "jp");

      /** now add the current step number string to the path **/

      strcat(current_file_name, step_number_field);

      /** now add the file name extension string to the path **/

      strcat(current_file_name, hdf_extension);

      if (verbose_flag)
         printf("searching for %s\n", current_file_name);

      /*** verify file existence and access, then call read_3d_mas_file() ***/

      if (access(current_file_name, R_OK) )
      {
         printf(
               "\nERROR: will not be able to read input from %s\n",
               current_file_name);
         return EXIT_FAILURE;
      }
      else
      {
         read_status = read_3d_mas_file(current_file_name, "jp", verbose_flag);

         if (read_status == EXIT_SUCCESS)
         {
            flag_jp[step_number] = step_number;
         }
      }
   }
   if (flag_jt[step_number] != step_number)
   {
      /*** set/reset current_file_name ***/

      strcpy(current_file_name, current_base_name);

      /** now add the variable name string to the path **/

      strcat(current_file_name, "jt");

      /** now add the current step number string to the path **/

      strcat(current_file_name, step_number_field);

      /** now add the file name extension string to the path **/

      strcat(current_file_name, hdf_extension);

      if (verbose_flag)
         printf("searching for %s\n", current_file_name);

      /*** verify file existence and access, then call read_3d_mas_file() ***/

      if (access(current_file_name, R_OK) )
      {
         printf(
               "\nERROR: will not be able to read input from %s\n",
               current_file_name);
         return EXIT_FAILURE;
      }
      else
      {
         read_status = read_3d_mas_file(current_file_name, "jt", verbose_flag);

         if (read_status == EXIT_SUCCESS)
         {
            flag_jt[step_number] = step_number;
         }
      }
   }
   if (flag_jr[step_number] != step_number)
   {
      /*** set/reset current_file_name ***/

      strcpy(current_file_name, current_base_name);

      /** now add the variable name string to the path **/

      strcat(current_file_name, "jr");

      /** now add the current step number string to the path **/

      strcat(current_file_name, step_number_field);

      /** now add the file name extension string to the path **/

      strcat(current_file_name, hdf_extension);

      if (verbose_flag)
         printf("searching for %s\n", current_file_name);

      /*** verify file existence and access, then call read_3d_mas_file() ***/

      if (access(current_file_name, R_OK) )
      {
         printf(
               "\nERROR: will not be able to read input from %s\n",
               current_file_name);
         return EXIT_FAILURE;
      }
      else
      {
         read_status = read_3d_mas_file(current_file_name, "jr", verbose_flag);

         if (read_status == EXIT_SUCCESS)
         {
            flag_jr[step_number] = step_number;
         }
      }
   }
   if (flag_vp[step_number] != step_number)
   {
      /*** set/reset current_file_name ***/

      strcpy(current_file_name, current_base_name);

      /** now add the variable name string to the path **/

      strcat(current_file_name, "vp");

      /** now add the current step number string to the path **/

      strcat(current_file_name, step_number_field);

      /** now add the file name extension string to the path **/

      strcat(current_file_name, hdf_extension);

      if (verbose_flag)
         printf("searching for %s\n", current_file_name);

      /*** verify file existence and access, then call read_3d_mas_file() ***/

      if (access(current_file_name, R_OK) )
      {
         printf(
               "\nERROR: will not be able to read input from %s\n",
               current_file_name);
         return EXIT_FAILURE;
      }
      else
      {
         read_status = read_3d_mas_file(current_file_name, "vp", verbose_flag);

         if (read_status == EXIT_SUCCESS)
         {
            flag_vp[step_number] = step_number;
         }
      }
   }
   if (flag_vt[step_number] != step_number)
   {
      /*** set/reset current_file_name ***/

      strcpy(current_file_name, current_base_name);

      /** now add the variable name string to the path **/

      strcat(current_file_name, "vt");

      /** now add the current step number string to the path **/

      strcat(current_file_name, step_number_field);

      /** now add the file name extension string to the path **/

      strcat(current_file_name, hdf_extension);

      if (verbose_flag)
         printf("searching for %s\n", current_file_name);

      /*** verify file existence and access, then call read_3d_mas_file() ***/

      if (access(current_file_name, R_OK) )
      {
         printf(
               "\nERROR: will not be able to read input from %s\n",
               current_file_name);
         return EXIT_FAILURE;
      }
      else
      {
         read_status = read_3d_mas_file(current_file_name, "vt", verbose_flag);

         if (read_status == EXIT_SUCCESS)
         {
            flag_vt[step_number] = step_number;
         }
      }
   }
   if (flag_vr[step_number] != step_number)
   {
      /*** set/reset current_file_name ***/

      strcpy(current_file_name, current_base_name);

      /** now add the variable name string to the path **/

      strcat(current_file_name, "vr");

      /** now add the current step number string to the path **/

      strcat(current_file_name, step_number_field);

      /** now add the file name extension string to the path **/

      strcat(current_file_name, hdf_extension);

      if (verbose_flag)
         printf("searching for %s\n", current_file_name);

      /*** verify file existence and access, then call read_3d_mas_file() ***/

      if (access(current_file_name, R_OK) )
      {
         printf(
               "\nERROR: will not be able to read input from %s\n",
               current_file_name);
         return EXIT_FAILURE;
      }
      else
      {
         read_status = read_3d_mas_file(current_file_name, "vr", verbose_flag);

         if (read_status == EXIT_SUCCESS)
         {
            flag_vr[step_number] = step_number;
         }
      }
   }
   if (flag_rho[step_number] != step_number)
   {
      /*** set/reset current_file_name ***/

      strcpy(current_file_name, current_base_name);

      /** now add the variable name string to the path **/

      strcat(current_file_name, "rho");

      /** now add the current step number string to the path **/

      strcat(current_file_name, step_number_field);

      /** now add the file name extension string to the path **/

      strcat(current_file_name, hdf_extension);

      if (verbose_flag)
         printf("searching for %s\n", current_file_name);

      /*** verify file existence and access, then call read_3d_mas_file() ***/

      if (access(current_file_name, R_OK) )
      {
         printf(
               "\nERROR: will not be able to read input from %s\n",
               current_file_name);
         return EXIT_FAILURE;
      }
      else
      {
         read_status = read_3d_mas_file(current_file_name, "rho", verbose_flag);

         if (read_status == EXIT_SUCCESS)
         {
            flag_rho[step_number] = step_number;
         }
      }
   }
   if (flag_p[step_number] != step_number)
   {
      /*** set/reset current_file_name ***/

      strcpy(current_file_name, current_base_name);

      /** now add the variable name string to the path **/

      strcat(current_file_name, "p");

      /** now add the current step number string to the path **/

      strcat(current_file_name, step_number_field);

      /** now add the file name extension string to the path **/

      strcat(current_file_name, hdf_extension);

      if (verbose_flag)
         printf("searching for %s\n", current_file_name);

      /*** verify file existence and access, then call read_3d_mas_file() ***/

      if (access(current_file_name, R_OK) )
      {
         printf(
               "\nERROR: will not be able to read input from %s\n",
               current_file_name);
         return EXIT_FAILURE;
      }
      else
      {
         read_status = read_3d_mas_file(current_file_name, "p", verbose_flag);

         if (read_status == EXIT_SUCCESS)
         {
            flag_p[step_number] = step_number;
         }
      }
   }
   if (flag_t[step_number] != step_number)
   {
      /*** set/reset current_file_name ***/

      strcpy(current_file_name, current_base_name);

      /** now add the variable name string to the path **/

      strcat(current_file_name, "t");

      /** now add the current step number string to the path **/

      strcat(current_file_name, step_number_field);

      /** now add the file name extension string to the path **/

      strcat(current_file_name, hdf_extension);

      if (verbose_flag)
         printf("searching for %s\n", current_file_name);

      /*** verify file existence and access, then call read_3d_mas_file() ***/

      if (access(current_file_name, R_OK) )
      {
         printf(
               "\nERROR: will not be able to read input from %s\n",
               current_file_name);
         return EXIT_FAILURE;
      }
      else
      {
         read_status = read_3d_mas_file(current_file_name, "t", verbose_flag);

         if (read_status == EXIT_SUCCESS)
         {
            flag_t[step_number] = step_number;
         }
      }
   }

   /*** if we reach here we have to assume that all files for the current step
    * have been processed, set cooresponding array element accordingly ***/
   step_done_array[step_number] = 1;

   return 1;

}

int read_3d_mas_file(
      char *current_file_name,
      char *current_variable_name,
      int verbose_flag)
{

#ifdef KAMELEON_WITH_HDF

   /*** FOR some reason MAX_NC_NAME and MAX_VAR_DIMS cannot be found in the hdf
    *** include directory.  When using HDF4 and netCDF at the same time, there are
    *** a buch of config issues that may arise.  Lets deal with it here by setting
    *** these values ourselves if there not defined  */

#ifndef MAX_VAR_DIMS
#define MAX_VAR_DIMS 32
#endif

#ifndef MAX_NC_NAME
#define MAX_NC_NAME 256
#endif

   /*
    *  sd_id = SD interface identifier,
    * sds_is = Data Set Identifier,
    * sds_index = Specific Data Set in File
    *
    */

   int32 sd_id, sds_id, sds_index;
   intn status;
   int32 n_datasets, n_file_attrs, index;
   int32 dim_sizes[MAX_VAR_DIMS];
   int32 rank, data_type, n_attrs;
   int32 start_1_dim[1], edges_1_dim[1];
   int32 start_3_dim[3], edges_3_dim[3];

   char name[MAX_NC_NAME];

   float32 *buffer;
   float32 *pos_1;
   float32 *pos_2;
   float32 *pos_3;
   float32 *data_1;
   float32 data_2[84][80][64];

   float32 ***threeD_data_array;

   int i, j, k, iphi, itheta, ir;

   /** verify file is hdf file **/

   status = Hishdf(current_file_name);

   if (status == FAIL)
   {
      printf("%s ERROR\tUnable to verify %s as HDF file\n",
            __FILE__, current_file_name );
      return( EXIT_FAILURE );
   }

   /** open the fileand initialize theSDinterface **/

   sd_id = SDstart( current_file_name, DFACC_READ );

   /** get info about the file **/

   status = SDfileinfo( sd_id, &n_datasets, &n_file_attrs );

   /*** loop through each sds_id **/

   for( index = 0; index < n_datasets; index++ )
   {

      /*** if this is the first call, read the position data ***/

      sds_id = SDselect( sd_id, index );
      status = SDgetinfo( sds_id, name, &rank, dim_sizes, &data_type, &n_attrs );
      /*
       printf("name = %s\n", name);
       printf("rank = %d\n", rank);
       printf("dimension sizes are : ");
       for(i=0;i<rank;i++) printf("%d ", dim_sizes[i] );
       printf("\n");
       printf("data type is %d\n", data_type );
       printf("nunmber of attributes is %d\n", n_attrs);
       */

      /** if the file contains the variable rho OR t OR p use the grid arrays contained
       * in this file for the plus1/offset positions **/

      if( rank == 1 && ( strcmp( current_variable_name, "rho") == 0 || strcmp( current_variable_name, "t") == 0 || strcmp( current_variable_name, "p") == 0) )
      {

         /*********** POSITION 1 phi_plus1 *******************************************/

         /** first index name matches and this is the first call **/

         if( strcmp( name, dim0_name ) == 0 && flag_pos1_plus1[current_step] == 0 )
         {
            if( verbose_flag ) printf("processing [PHI_PLUS_1] 1 dimensional dataset %s of size %d\n", name, dim_sizes[0] );

            start_1_dim[0] = 0;
            edges_1_dim[0] = dim_sizes[0];

            dim0_plus1 = (float *) malloc( dim_sizes[0] * sizeof( float ) );

            /** set fakedim0 defined in read_mas.h **/

            fakedim0_plus1 = dim_sizes[0];

            if( dim0_plus1 == NULL )
            {
               printf("%s ERROR:\tmalloc failed for %s buffer\n", name, __FILE__ );
               return( EXIT_FAILURE );
            }

            status = SDreaddata( sds_id, start_1_dim, NULL, edges_1_dim, (VOIDP)dim0_plus1 );

            flag_pos1_plus1[current_step] = 1;
         }

         /*********** POSITION 2 theta_plus1 *******************************************/

         /* first index name matches and this is the first call */

         if( strcmp( name, dim1_name ) == 0 && flag_pos2_plus1[current_step] == 0 )
         {
            if( verbose_flag ) printf("processing [THETA_PLUS_1]1 dimensional dataset %s of size %d\n", name, dim_sizes[0] );

            start_1_dim[0] = 0;
            edges_1_dim[0] = dim_sizes[0];

            dim1_plus1 = (float *) malloc( dim_sizes[0] * sizeof( float ) );

            /** set fakedim1 defined in read_mas.h **/

            fakedim1_plus1 = dim_sizes[0];

            if( dim1_plus1 == NULL )
            {
               printf("%s ERROR:\tmalloc failed for %s buffer\n", name, __FILE__ );
               return( EXIT_FAILURE );
            }

            status = SDreaddata( sds_id, start_1_dim, NULL, edges_1_dim, (VOIDP)dim1_plus1 );

            flag_pos2_plus1[current_step] = 1;
         }

         /*********** POSITION 3 r_plus1 *******************************************/

         /** first index name matches and this is the first call **/

         if( strcmp( name, dim2_name ) == 0 && flag_pos3_plus1[current_step] == 0 )
         {
            if( verbose_flag ) printf("processing [R_PLUS_1]1 dimensional dataset %s of size %d\n", name, dim_sizes[0] );

            start_1_dim[0] = 0;
            edges_1_dim[0] = dim_sizes[0];

            dim2_plus1 = (float *) malloc( dim_sizes[0] * sizeof( float ) );

            /** set fakedim0 defined in read_mas.h **/

            fakedim2_plus1 = dim_sizes[0];

            if( dim2_plus1 == NULL )
            {
               printf("%s ERROR:\tmalloc failed for %s buffer\n", name, __FILE__ );
               return( EXIT_FAILURE );
            }

            status = SDreaddata( sds_id, start_1_dim, NULL, edges_1_dim, (VOIDP)dim2_plus1 );

            flag_pos3_plus1[current_step] = 1;
         }

      } /*** end of       if( rank == 1 && ( strcmp( current_variable_name, "rho") == 0 || strcmp( current_variable_name, "t") == 0 || strcmp( current_variable_name, "p") == 0) )
 block ***/
      else if ( rank == 1 && strcmp( current_variable_name, "br") == 0
      ) /** if the file contains the variable br use the dim0 and dim1 arrays contained
       * in this file for the normal phi and theta positions **/
      {

         /*********** POSITION 1 phi *******************************************/


	  /** first index name matches and this is the first call **/

	  if( strcmp( name, dim0_name ) == 0 && flag_pos1[current_step] == 0 )
	  {
	    if( verbose_flag ) printf("processing [PHI] 1 dimensional dataset %s of size %d\n", name, dim_sizes[0] );

	    start_1_dim[0] = 0;
	    edges_1_dim[0] = dim_sizes[0];

	    dim0 = (float *) malloc( dim_sizes[0] * sizeof( float ) );

	    /** set fakedim0 defined in read_mas.h **/

	    fakedim0 = dim_sizes[0];

		if( dim0 == NULL )
		{
		    printf("%s ERROR:\tmalloc failed for %s buffer\n", name, __FILE__ );
		    return( EXIT_FAILURE );
		}

	    status = SDreaddata( sds_id, start_1_dim, NULL, edges_1_dim, (VOIDP)dim0 );

	    flag_pos1[current_step] = 1;
	 }

         /*********** POSITION 2 *******************************************/

         /** first index name matches and this is the first call **/

         if( strcmp( name, dim1_name ) == 0 && flag_pos2[current_step] == 0 )
         {
            if( verbose_flag ) printf("processing [THETA] 1 dimensional dataset %s of size %d\n", name, dim_sizes[0] );

            start_1_dim[0] = 0;
            edges_1_dim[0] = dim_sizes[0];

            dim1 = (float *) malloc( dim_sizes[0] * sizeof( float ) );

            /** set fakedim0 defined in read_mas.h **/

            fakedim1 = dim_sizes[0];

            if( dim1 == NULL )
            {
               printf("%s ERROR:\tmalloc failed for %s buffer\n", name, __FILE__ );
               return( EXIT_FAILURE );
            }

            status = SDreaddata( sds_id, start_1_dim, NULL, edges_1_dim, (VOIDP)dim1 );

            flag_pos2[current_step] = 1;
         }

         /*********** POSITION 3 *******************************************/

         /* Position 3 ( dim2 ) in this br file cooresponds to r_plus1 which
          * was loaded in the block that processes rho,t, and p files
          */



      }
      else if( rank == 1 && ( strcmp( current_variable_name, "vr") == 0 || strcmp( current_variable_name, "bp") == 0 || strcmp( current_variable_name, "bt") == 0 || strcmp( current_variable_name, "jr") == 0) )
      /** if the file contains the variable bp OR vr OR bt OR jr use the dim2 array contained
             * in this file for the normal r positions **/
            {

          /*********** POSITION 3 r *******************************************/


      	  /** first index name matches and this is the first call **/

      	  if( strcmp( name, dim2_name ) == 0 && flag_pos3[current_step] == 0 )
      	  {
      	    if( verbose_flag ) printf("processing [R] 1 dimensional dataset %s of size %d\n", name, dim_sizes[0] );

      	    start_1_dim[0] = 0;
      	    edges_1_dim[0] = dim_sizes[0];

      	    dim2 = (float *) malloc( dim_sizes[0] * sizeof( float ) );

      	    /** set fakedim0 defined in read_mas.h **/

      	    fakedim2 = dim_sizes[0];

      		if( dim2 == NULL )
      		{
      		    printf("%s ERROR:\tmalloc failed for %s buffer\n", name, __FILE__ );
      		    return( EXIT_FAILURE );
      		}

      	    status = SDreaddata( sds_id, start_1_dim, NULL, edges_1_dim, (VOIDP)dim2 );

      	    flag_pos3[current_step] = 1;
      	 }





      }
      else if( rank == 3 )
      {

         int nphi = dim_sizes[0];
         int ntheta = dim_sizes[1];
         int nr = dim_sizes[2];

         float local_buffer[ nphi ][ ntheta ][ nr ];
         float local_buffer_edges[ nr ][ ntheta ][ nphi ];

         /*
          float local_buffer_reversed[nr][ntheta][dim1];
          float local_buffer_reversed_edges[nr][ntheta][dim1];
          */

         start_3_dim[0] = 0;
         start_3_dim[1] = 0;
         start_3_dim[2] = 0;

         edges_3_dim[0] = nphi;
         edges_3_dim[1] = ntheta;
         edges_3_dim[2] = nr;

         if( verbose_flag ) printf("processing 3 dimensional dataset %s of size %d * %d * %d \n", name, nphi, ntheta, nr );

         /** read data into static 3d buffer **/

         printf("DEBUG\treading HDF data into array shape %d*%d*%d\t with edges %d, %d, %d\n", nphi, ntheta, nr, edges_3_dim[0], edges_3_dim[1], edges_3_dim[2] );

         status = SDreaddata( sds_id, start_3_dim, NULL, edges_3_dim, (VOIDP)local_buffer );

         /*
          * printf("DEBUG\treading HDF data into array shape %d*%d*%d\t with
          * edges %d, %d, %d\n", nphi, ntheta, nr, edges_3_dim[0],
          * edges_3_dim[1], edges_3_dim[2] );
          */
         /*
          status = SDreaddata( sds_id, start_3_dim, NULL, edges_3_dim,
          (VOIDP)local_buffer_reversed );
          */
         /*
          edges_3_dim[0] = nphi;
          edges_3_dim[1] = ntheta;
          edges_3_dim[2] = nr;
          */
         /*
          printf("DEBUG\treading HDF data into array shape %d*%d*%d\t with
          edges %d, %d, %d\n", nr, ntheta, nphi, edges_3_dim[0],
          edges_3_dim[1], edges_3_dim[2] );

          status = SDreaddata( sds_id, start_3_dim, NULL, edges_3_dim,
          (VOIDP)local_buffer_edges );


          printf("DEBUG\treading HDF data into array shape %d*%d*%d\t with
          edges %d, %d, %d\n", nphi, ntheta, nr, edges_3_dim[0],
          edges_3_dim[1], edges_3_dim[2] );
          */
         /*    status = SDreaddata( sds_id, start_3_dim, NULL, edges_3_dim,
          * (VOIDP)local_buffer_reversed_edges );
          */

         /** dynamically allocate space for current_variable_buffer which
          * points to a specific variable pointer defined in read_mas.h **/

         if( strcmp( current_variable_name, "t") == 0 )
         {
            printf("DEBUG\tallocating space for t\n");
            t = (float *) malloc( nr * ntheta * nphi *sizeof( float ) );
         }
         else if( strcmp( current_variable_name, "p") == 0 )
         {
            printf("DEBUG\tallocating space for p\n");
            p = (float *) malloc( nr * ntheta * nphi *sizeof( float ) );
         }
         else if( strcmp( current_variable_name, "rho") == 0 )
         {
            printf("DEBUG\tallocating space for rho\n");
            rho = (float *) malloc( nr * ntheta * nphi *sizeof( float ) );
         }
         else if( strcmp( current_variable_name, "vr") == 0 )
         {
            printf("DEBUG\tallocating space for vr\n");
            vr = (float *) malloc( nr * ntheta * nphi *sizeof( float ) );
         }
         else if( strcmp( current_variable_name, "vp") == 0 )
         {
            printf("DEBUG\tallocating space for vp\n");
            vp = (float *) malloc( nr * ntheta * nphi *sizeof( float ) );
         }
         else if( strcmp( current_variable_name, "vt") == 0 )
         {
            printf("DEBUG\tallocating space for vt\n");
            vt = (float *) malloc( nr * ntheta * nphi *sizeof( float ) );
         }
         else if( strcmp( current_variable_name, "jr") == 0 )
         {
            printf("DEBUG\tallocating space for jr\n");
            jr = (float *) malloc( nr * ntheta * nphi *sizeof( float ) );
         }
         else if( strcmp( current_variable_name, "jp") == 0 )
         {
            printf("DEBUG\tallocating space for jp\n");
            jp = (float *) malloc( nr * ntheta * nphi *sizeof( float ) );
         }
         else if( strcmp( current_variable_name, "jt") == 0 )
         {
            printf("DEBUG\tallocating space for jt\n");
            jt = (float *) malloc( nr * ntheta * nphi *sizeof( float ) );
         }
         else if( strcmp( current_variable_name, "br") == 0 )
         {
            printf("DEBUG\tallocating space for br\n");
            br = (float *) malloc( nr * ntheta * nphi *sizeof( float ) );
         }
         else if( strcmp( current_variable_name, "bp") == 0 )
         {
            printf("DEBUG\tallocating space for bp\n");
            bp = (float *) malloc( nr * ntheta * nphi *sizeof( float ) );

            for( ir= 0; ir < nr; ir++ )
            {
               for( itheta = 0; itheta < ntheta; itheta++ )
               {
                  for( iphi = 0; iphi < nphi; iphi++ )
                  {/*
                   printf("\n    local_buffer   [%d][%d][%d]        \t= %f\n", iphi,itheta,ir, local_buffer[iphi][itheta][ir] );
                   printf("\n    local_buffer_e [%d][%d][%d]        \t= %f\n", iphi,itheta,ir, local_buffer_edges[iphi][itheta][ir] );
                   printf("\n    local_buffer_rev        [%d][%d][%d]        \t= %f\n", iphi,itheta,ir, local_buffer_reversed[iphi][itheta][ir] );
                   printf("\n    local_buffer_re_edges    [%d][%d][%d]        \t= %f\n", iphi,itheta,ir, local_buffer_reversed_edges[iphi][itheta][ir] );
                   */
                  }
               }
            }

            for( iphi = 0; iphi < nphi; iphi++ )
            {

               if( DEBUG_FLAG ) printf("\nlocal_buffer    [%d][0][0]\t= %f\n", iphi, local_buffer[iphi][0][0] );

               /*printf("\nlocal_buffer    [0][0][%d]\t= %f\n", iphi, local_buffer[0][0][iphi] );

                * printf("\nlocal_buffer_e  [%d][0][0]\t= %f\n", iphi, local_buffer_edges[iphi][0][0] );

                printf("\nlocal_buffer_r  [%d][0][0]\t= %f\n", iphi, local_buffer_reversed[iphi][0][0] );
                printf("\nlocal_buffer_re [%d][0][0]\t= %f\n", iphi, local_buffer_reversed_edges[iphi][0][0] );
                */
            }

            for( itheta = 0; itheta < ntheta; itheta++ )
            {

               if( DEBUG_FLAG ) printf("\nlocal_buffer    [0][%d][0]\t= %f\n", itheta, local_buffer[0][itheta][0] );
               /*printf("\nlocal_buffer_e  [0][%d][0]\t= %f\n", itheta, local_buffer_edges[0][itheta][0] );

                printf("\nlocal_buffer_r  [0][%d][0]\t= %f\n", itheta, local_buffer_reversed[0][itheta][0] );
                printf("\nlocal_buffer_re [0][%d][0]\t= %f\n", itheta, local_buffer_reversed_edges[0][itheta][0] );
                */
            }

            for( ir = 0; ir < nr; ir++ )
            {

               if( DEBUG_FLAG ) printf("\nlocal_buffer    [0][0][%d]\t= %f\n", ir, local_buffer[0][0][ir] );

               /*printf("\nlocal_buffer    [%d][0][0]\t= %f\n", ir, local_buffer[ir][0][0] );
                */
               /*printf("\nlocal_buffer_e  [0][0][%d]\t= %f\n", ir, local_buffer_edges[0][0][ir] );

                printf("\nlocal_buffer_r  [0][0][%d]\t= %f\n", ir, local_buffer_reversed[0][0][ir] );
                printf("\nlocal_buffer_re [0][0][%d]\t= %f\n", ir, local_buffer_reversed_edges[0][0][ir] );
                */
            }

            if( DEBUG_FLAG )  printf("were am i seg faulting...asks bp\n");

         }
         else if( strcmp( current_variable_name, "bt") == 0 )
         {
            printf("DEBUG\tallocating space for bt\n");
            bt = (float *) malloc( nr * ntheta * nphi *sizeof( float ) );
         }
         else
         {
            printf("%s\tERROR: %s is an unrecognized variable, unable to process\n", __FILE__, current_variable_name );
            return EXIT_FAILURE;
         }

         /************ DEBUG PRINTOUT OF ARRAY

          for( iphi= 0; iphi < nr; iphi++ )
          {
          for( itheta = 0; itheta < ntheta; itheta++ )
          {
          for( ir = 0; ir < nr; ir++ )
          {
          printf("\n    local_buffer         [%d][%d][%d]        \t= %f\n", iphi,itheta,ir, local_buffer[iphi][itheta][ir] );
          printf("\n    local_buffer_reversed[%d][%d][%d]        \t= %f\n", iphi,itheta,ir, local_buffer_reversed[iphi][itheta][ir] );

          }
          }
          }

          **/

         /*
          for( iphi = 0; iphi < nr; iphi++ )
          {

          printf("\nlocal_buffer[%d][0][0]\t= %f\n", iphi, local_buffer[iphi][0][0] );

          }

          for( itheta = 0; itheta < ntheta; itheta++ )
          {

          printf("\nlocal_buffer[0][%d][0]\t= %f\n", itheta, local_buffer[0][itheta][0] );

          }

          for( ir = 0; ir < nr; ir++ )
          {

          printf("\nlocal_buffer[0][0][%d]\t= %f\n", ir, local_buffer[0][0][ir] );

          }
          */

         for( ir= 0; ir < nr; ir++ )
         {
            for( itheta = 0; itheta < ntheta; itheta++ )
            {
               for( iphi = 0; iphi < nphi; iphi++ )
               {

                  if( strcmp( current_variable_name, "t") == 0 )
                  {
                     /*t[ iphi + ( itheta * nr ) +  ( ir * nr * ntheta ) ] = local_buffer_reversed[ir][itheta][iphi];*//* old wrong way */
                     /*t[ iphi + ( itheta * nr ) +  ( ir * nr * ntheta ) ] = local_buffer[iphi][itheta][ir];*//** test way 032307 **/
                     t[ iphi + ( itheta * nphi ) + ( ir * nphi * ntheta ) ] = local_buffer[iphi][itheta][ir];
                  }
                  else if( strcmp( current_variable_name, "p") == 0 )
                  {
                     p[ iphi + ( itheta * nphi ) + ( ir * nphi * ntheta ) ] = local_buffer[iphi][itheta][ir];
                  }
                  else if( strcmp( current_variable_name, "rho") == 0 )
                  {
                     rho[ iphi + ( itheta * nphi ) + ( ir * nphi * ntheta ) ] = local_buffer[iphi][itheta][ir];
                  }
                  else if( strcmp( current_variable_name, "vr") == 0 )
                  {
                     vr[ iphi + ( itheta * nphi ) + ( ir * nphi * ntheta ) ] = local_buffer[iphi][itheta][ir];
                  }
                  else if( strcmp( current_variable_name, "vp") == 0 )
                  {
                     vp[ iphi + ( itheta * nphi ) + ( ir * nphi * ntheta ) ] = local_buffer[iphi][itheta][ir];
                  }
                  else if( strcmp( current_variable_name, "vt") == 0 )
                  {
                     vt[ iphi + ( itheta * nphi ) + ( ir * nphi * ntheta ) ] = local_buffer[iphi][itheta][ir];
                  }
                  else if( strcmp( current_variable_name, "jr") == 0 )
                  {
                     jr[ iphi + ( itheta * nphi ) + ( ir * nphi * ntheta ) ] = local_buffer[iphi][itheta][ir];
                  }
                  else if( strcmp( current_variable_name, "jp") == 0 )
                  {
                     jp[ iphi + ( itheta * nphi ) + ( ir * nphi * ntheta ) ] = local_buffer[iphi][itheta][ir];
                  }
                  else if( strcmp( current_variable_name, "jt") == 0 )
                  {
                     jt[ iphi + ( itheta * nphi ) + ( ir * nphi * ntheta ) ] = local_buffer[iphi][itheta][ir];
                  }
                  else if( strcmp( current_variable_name, "br") == 0 )
                  {
                     /*br[ ir + ( itheta * nr ) +  ( iphi * nr * ntheta )  ] = local_buffer[iphi][itheta][ir];*//* 84*80*64 */
                     br[ iphi + ( itheta * nphi ) + ( ir * nphi * ntheta ) ] = local_buffer[iphi][itheta][ir];/* 64*80*84 */

                  }
                  else if( strcmp( current_variable_name, "bp") == 0 )
                  {
                     /*printf("WTF bp \n");*/

                     bp[ iphi + ( itheta * nphi ) + ( ir * nphi * ntheta ) ] = local_buffer[iphi][itheta][ir];
                     /*
                      printf("\nlocal_buffer [%d][%d][%d]\t= %f\n", iphi,itheta,ir, local_buffer[iphi][itheta][ir] );
                      printf("bp            [%d]\t= %f\n",  iphi + ( itheta * nr ) +  ( ir * nr * ntheta ), bp[ iphi + ( itheta * nr ) +  ( ir * nr * ntheta ) ] );
                      */
                  }
                  else if( strcmp( current_variable_name, "bt") == 0 )
                  {
                     bt[ iphi + ( itheta * nphi ) + ( ir * nphi * ntheta ) ] = local_buffer[iphi][itheta][ir];
                  }
                  else
                  {
                     printf("%s\tERROR: %s is an unrecognized variable, unable to process\n", __FILE__, current_variable_name );
                     return EXIT_FAILURE;
                  }
               }

            }

         }

      } /*** end of if( rank == 3 ) block ***/

      /** close currently open dataset **/

      status = SDendaccess( sds_id );

   }

   /** close interface to currently open file **/

   status = SDend( sd_id );

   /** was this the first call? if so set flag accordingly **/

   if( is_first_call ) is_first_call = 0;

   return EXIT_SUCCESS;

#endif

   return EXIT_SUCCESS;

}

