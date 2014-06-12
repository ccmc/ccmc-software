/*******************************************************************************
 *                                                                             *
 *    NAME:          open_cdf.c                                                *
 *                                                                             *
 *    VERSION:       1.0                                                       *
 *                                                                             *
 *    AUTHOR:        Marlo M. H. Maddox                                        *
 *                   NASA-GSFC-AETD-ISD-ADMA(587)-CCMC                         *
 *                   Marlo.Maddox@nasa.gov                                     *
 *                   (301) 286 - 5202                                          *
 *                                                                             *
 *    PURPOSE:       Performs all of the background low-level cdf functions    *
 *                   required to to open a cdf file including aquiring all the *
 *                   necessary variable numbers.  Variable name validation is  *
 *                   perfomered and any variables that are specified as        *
 *                   arguments to open_cdf() are loaded into main memory       *
 *                                                                             *
 *    INPUTS:        void open_cdf( char *cdf_name, int number_of_arguments,   *
 *                   ... ) the open_cdf function is called for each cdf file   *
 *                   that data is to be extracted from and has two required    *
 *                   arguments followed by the optional variable name arguments*
 *                   that will be read into main memory.                       *
 *                                                                             *
 *                   cdf_name - character string that references the           *
 *                   path/filename of the cdf file that contains data to be    *
 *                   interpolated NOTE: strlen( cdf_name ) < CDF_PATHNAME_LEN  *
 *                   as defined in cdf.h - for CDF2.7 = 128, for CDF 3.0 = 512 *
 *                                                                             *
 *                   number_of_arguments - the number of variables n to be read*
 *                   into main memory.  Should be O if no variables are to be  *
 *                   stored.                                                   *
 *                                                                             *
 *                   optional argument 1..n - n character strings              *
 *                   ( as specified above ), that reference the model variables*
 *                   you would like to read into main memory                   *
 *                                                                             *
 *                   Example 1: will read bx, by, bz into main memory - NOTE:  *
 *                   the entire array will be read into main memory            *
 *                                                                             *
 *                   open_cdf( cdf_name, 3, "bx", "by", "bz" );                *
 *                                                                             *
 *    OUTPUTS:                                                                 *
 *                                                                             *
 *    FUNCTIONS:                                                               *
 *                                                                             *
 *    VARIABLES:                                                               *
 *                                                                             *
 *    MODIFICATION                                                             *
 *    HISTORY:                                                                 *
 *                                                                             *
 *      06.28.05     Maddox, Marlo                                             *
 *                   bug in cdf info model_name char array has 50 elements -   *
 *                   needed to add null terminator to end of string            *
 *                                                                             *
 *      08.08.05     Maddox, Marlo                                             *
 *                   Adding CTIP model                                         *
 *                   added external declaration for:                           *
 *                   load_ucla_ggcm_variable_into_main_memory - was missing    *
 *                   added [model_name]_cdf_interface_variables.h include      *
 *                   definition for variable name verifycation                 *
 *                                                                             *
 *     04.24.06      Maddox, Marlo                                             *
 *                   Adding tracking mechanism to determine if a new file has  *
 *                   been opened.  If so, the interpolator needs to relocate   *
 *                   position in new file even if the actual position hasn't   *
 *                   changed.  This is because the interpolator remembers the  *
 *                   last position used and if it is the same, it reuses the   *
 *                   previous block search info.  However, with AMR, if a new  *
 *                   file is ingested the previous block information is invalid*
 *                                                                             *
 *     08.01.2006    Marlo Maddox                                              *
 *                   adding ENLIL                                              *
 *                                                                             *
 *     09.08.2006    Marlo Maddox                                              *
 *                   added unistd.h to check if cdf file exists                *
 *                   before passing to cdf open                                *
 *                                                                             *
 *     10.04.2006    Marlo Maddox                                              *
 *                   adding list_structure_toolkit functionallity to keep track*
 *                   of what variables have been loadded into main memory.  We *
 *                   also keep track of the file name that was loaded into     *
 *                   memory so multiple calls to open will not cause program to*
 *                   load all of the inital info for that particular file as is*
 *                   the case on the first open call.  Subsequent calls to open*
 *                   with new main mem arguments will automatically check if   *
 *                   variable already exists in mem and if not, variable will  *
 *                   be loaded                                                 *
 *                                                                             *
 *                   Changed function type for load_*_cdf_variable_into_main_..*
 *                   from void to int.  value retuned to load_status variable  *
 *                                                                             *
 *                   inserting all loaded files into loaded_variable_listPtr   *
 *                   and each open file into open_filePtr                      *
 *                                                                             *
 *     11.14.2006    Marlo Maddox                                              *
 *                   The model_name char * variable was once static as it      *
 *                   should be but was changed to auto???  Switched back to    *
 *                   static so the open_cdf routine can retain the model_name  *
 *                   value between multiple calls to open.  Since open doesn't *
 *                   reinitialize an already open file, we can't expect open to*
 *                   ping the file on each open to get the model name.  We'll  *
 *                   just keep the memory location static to retain the        *
 *                   model_name value                                          *
 *                                                                             *
 *     11.30.2006    Marlo Maddox                                              *
 *                   Adding kpvt - kitt peak overservatory data for the vacuum *
 *                   telescope                                                 *
 *                                                                             *
 *     12.07.2006    Maddox, Marlo                                             *
 *                   Adding msfc_tvm...                                        *
 *                                                                             *
 *     02.23.2007    Maddox, Marlo                                             *
 *                   Adding mas...                                             *
 *                                                                             *
 *     04.09.2008    Maddox, Marlo                                             *
 *                   Moved the declaration of new_cdf_file = 1; inside the     *
 *                   block that makes sure the current cdf file name is not    *
 *                   already open.                                             *
 *                                                                             *
 *    04.15.2008     Maddox, Marlo                                             *
 *                   Added allocate_deafults_done & full_bats_grid_in_memory   *
 *                   Variables to address multi-open/close with batsrus        *
 *                   specifically.                                             *
 *                                                                             *
 *                   Also added is_var_in_memory() check before loading a new  *
 *                   variable in main memory                                   *
 *                                                                             *
 *    07.29.2008     Maddox, Marlo                                             *
 *                   Adding eta variable for ucla_ggcm/open_ggcm               *
 *                                                                             *
 *    01.06.2010     Peter MacNeice                                            *
 *                   Adding magic model                                        *
 *                                                                             *
 *    03.10.2010     Peter MacNeice                                            *
 *                   Adding adapt3d model                                      *
 ******************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <math.h>
#include <stdarg.h>
#include <unistd.h>
#include "cdf.h"
#include "cdf_interface_variables.h"
#include "registered_ccmc_variable_names.h"
#include "list_structure_toolkit.h"

#define DEBUG_FLAG 0
/* #define DEBUG_FLAG 1 */

void open_cdf(
      char *cdf_name,
      int number_of_arguments,
      ...)
{

   char * get_cdf_info(
         int);
   int valid_input_variable(
         char *);
   extern void get_batsrus_cdf_info(
         int);
   extern int load_batsrus_cdf_variable_into_main_memory(
         char *);
   extern int load_ucla_ggcm_cdf_variable_into_main_memory(
         char *);
   extern int load_ctip_cdf_variable_into_main_memory(
         char *);
   extern int load_enlil_cdf_variable_into_main_memory(
         char *);
   extern int load_kpvt_cdf_variable_into_main_memory(
         char *);
//   extern int load_magic_cdf_variable_into_main_memory(
//         char *);
   extern int load_adapt3d_cdf_variable_into_main_memory(
         char *);
   extern int load_msfc_tvm_cdf_variable_into_main_memory(
         char *);
   extern int load_mas_cdf_variable_into_main_memory(
         char *);

   extern void insert_list_node(
         ListNodePtr *,
         char *);
   extern int node_exists(
         ListNodePtr *,
         char *);

   extern CDFid current_file_id;
   CDFstatus status;

   int main_memory_flag;

   int string_len;
   int load_status, current_file_already_loaded_flag;

   static char *model_name;
   /*char *model_name;*/

   char *current_input_variable;

   char *batsrus = "batsrus";
   char *ucla_ggcm = "ucla_ggcm";
   char *open_ggcm = "open_ggcm";
   char *ctip = "ctip";
   char *enlil = "enlil";
   char *kpvt = "kpvt";
   char *magic = "magnetogram";
   char *magnetogram = "magnetogram";
   char *adapt3d = "ADAPT3D";
   char *msfc_tvm = "msfc_tvm";
   char *mas = "mas";

   va_list argptr; /* create a pointer to the variable list of arguments */

   /**** CDF_PATHNAME_LEN restricts the length of the
    * CDFname argument must <= CDF_PATHNAME_LEN ****/

   string_len = strlen(cdf_name);

   if (string_len >= CDF_PATHNAME_LEN)
   {
      printf(
            "\n\n!!ERROR!! String length of pathname:\n\n\t%s\n\n\t"
               "Exceeds ------>%d<------ characters\n\nCurrent Version of CDF "
               "library restricts the length of path & file name arguments to "
               "CDFopen.\nSimply reduce data directory depth enough to ensure "
               "length of full path and file name conforms to restrictions.\n\n",
            cdf_name,
            CDF_PATHNAME_LEN);
      exit( 1);
   }

   /**** lets see if the file even exists first ****/

   /**** also, if the file is already open, we don't even need to call the CDFopen ****/

   if ( !access(
         cdf_name,
         R_OK) )
   {
      /*** specified file is read OK ****/

      if (node_exists(
            &open_filePtr,
            cdf_name) == 0)
      {

         /*** specified file is NOT currently open ****/

         status = CDFopen( cdf_name, &current_file_id );
         if (status != CDF_OK)
         {
            StatusHandler(status);
            printf(
                  "ERROR: cannot open %s\nIs %s a valid KAMELEON Generated CDF File?",
                  cdf_name,
                  cdf_name);
            exit( EXIT_FAILURE);
         }

         /*** every time the open_cdf function is called, set new_cdf_file flag
          * = 1.  new_cdf_file variable is defined in cdf_interface_variables.h file ***/

         new_cdf_file = 1;

         /*** also if this is a new file, then allocate_defaults_done should be initialized to 0 ***/

         allocate_deafults_done = 0;

         /*** as well as full_bats_grid_in_memory **/

         full_bats_grid_in_memory = 0;

      }
      else
      {
         printf(
               "WARNING: %s already open\n",
               cdf_name);
      }
   }
   else
   {
      printf(
            "ERROR: cannot read \t%s\n",
            cdf_name);
      perror(cdf_name);
      exit( EXIT_FAILURE);
   }

   main_memory_flag = number_of_arguments;

   /*printf("DEBUG\tmain_mem flag is set to %d\n", main_memory_flag );*/

   /** only call get info if the currently open cdf files info is not already loaded **/

   if (node_exists(
         &open_filePtr,
         cdf_name) == 0)
   {

      /*** specified file is NOT currently open ****/

      if ( DEBUG_FLAG)
         printf(
               "new file being loaded = %s\n",
               cdf_name);

      model_name = get_cdf_info(main_memory_flag);

      /** keep track of what files we currently have info in memory for **/
      insert_list_node(
            &open_filePtr,
            cdf_name);

   }

   if ( DEBUG_FLAG)
      printf(
            "DEBUG\tvalue of model_name variable = %s\n",
            model_name);

   /****************************************************************************
    *                                                                          *
    * If the model name = batsrus and the file is alread open but hasn't yet   *
    * been opened with main memory, all of the unique batsrus grid info won't  *
    * be in main memory as only the space required for a single block will have*
    * been allocated - NOT the entire dynamic grid as if get_batsrus_cdf_info()*
    * would have been called with main memory flag. And since we only call     *
    * get_cdf_info() once for a single file, subsequent calls to open_cdf with *
    * batsrus files after an initial open bats cdf call with out main memory is*
    * a problem.  To handle this, we check a flag:                             *
    *                                                                          *
    *          full_grid_in_memory                                             *
    *                                                                          *
    * If full_grid_in_memory <=0 && main_memory_flag > 0 we need to call       *
    * get_batsrus_cdf_info(main_memory_flag);                                  *
    * with main memory flag = 1 to load the full batsrus grid                  *
    ***************************************************************************/

   if ( (!strcmp(
         model_name,
         batsrus) ) && full_bats_grid_in_memory <= 0 && main_memory_flag > 0)
   {
      get_batsrus_cdf_info(main_memory_flag);

      full_bats_grid_in_memory = 1;

   }

   /* make argptr point to first unamed argument
    * ( an input variable to read into main memory ) */

   va_start( argptr, number_of_arguments );

   /*dummy_input_variable_hack = va_arg( argptr, char * );*/

   /* check for additional arguments to open_cdf.  Each variable argument will
    * be read into main memory */
            printf("start loading data into memory? \n");
            printf("number_of_arguments = %d \n",number_of_arguments);
   while (number_of_arguments-- > 0)
   {

      /*fprintf(stderr, "DEBUG\tcurrent number_of_arguments = %d\n", number_of_arguments );*/

      current_input_variable = va_arg( argptr, char * );

      /*fprintf(stderr, "DEBUG\tthe current argument is --->%s<---\n", current_input_variable );*/

      /*fprintf( stderr, "DEBUG\tsizeof( %s ) = %d\n", current_input_variable, sizeof( current_input_variable ) );*/

      /*** don't process invalid input variables ***/

      if (valid_input_variable(current_input_variable)
            && is_var_in_memory(current_input_variable) == 0)
      {
         if ( DEBUG_FLAG)
            fprintf(stderr, "DEBUG\t calling load for %s\n", current_input_variable);

         if ( !strcmp(
               model_name,
               batsrus) )
         {
            load_status
                  = load_batsrus_cdf_variable_into_main_memory(current_input_variable);
         }
         else if ( !strcmp(
               model_name,
               ucla_ggcm) )
         {
            load_status
                  = load_ucla_ggcm_cdf_variable_into_main_memory(current_input_variable);
         }
         else if ( !strcmp(
               model_name,
               open_ggcm) )
         {
            load_status
                  = load_ucla_ggcm_cdf_variable_into_main_memory(current_input_variable);
         }
         else if ( !strcmp(
               model_name,
               ctip) )
         {
            load_status
                  = load_ctip_cdf_variable_into_main_memory(current_input_variable);
         }
         else if ( !strcmp(
               model_name,
               enlil) )
         {
            load_status
                  = load_enlil_cdf_variable_into_main_memory(current_input_variable);
         }
         else if ( !strcmp(
               model_name,
               kpvt) )
         {
            load_status
                  = load_kpvt_cdf_variable_into_main_memory(current_input_variable);
         }
//         else if ( !strcmp(
//               model_name,
//               magic) )
//         {
//            load_status
//                  = load_magic_cdf_variable_into_main_memory(current_input_variable);
//         }
//         else if ( !strcmp(
//               model_name,
//               magnetogram) )
//         {
//            load_status
//                  = load_magic_cdf_variable_into_main_memory(current_input_variable);
//         }
         else if ( !strcmp(
               model_name,
               adapt3d) )
         {
            load_status
                  = load_adapt3d_cdf_variable_into_main_memory(current_input_variable);
         }
         else if ( !strcmp(
               model_name,
               msfc_tvm) )
         {
            load_status
                  = load_msfc_tvm_cdf_variable_into_main_memory(current_input_variable);
         }
         else if ( !strcmp(
               model_name,
               mas) )
         {
            load_status
                  = load_mas_cdf_variable_into_main_memory(current_input_variable);
         }

         /** if variable was loaded sucessfully, add variable name to our list_structure **/

         if (load_status == EXIT_SUCCESS)
            insert_list_node(
                  &loaded_variable_listPtr,
                  current_input_variable);
      }
      else if (is_var_in_memory(current_input_variable) == 1)
      {
         fprintf( stderr, "WARNING:\tcurrent input variable ---->%s<---- already"
         " in memory.  Ignoring.\n", current_input_variable);
      }
      else
      {
         fprintf( stderr, "ERROR:\tcurrent input variable ---->%s<---- could "
         "not be validated.  Ignoring.\n", current_input_variable);
      }

   }

   va_end(argptr);

   if( DEBUG_FLAG ) printf("%s\tEXITING OPEN...\n", __FILE__);

   return;

}

char * get_cdf_info(
      int main_memory_flag)
{

   CDFstatus status;

   extern void get_batsrus_cdf_info(
         int);
   extern void get_ucla_ggcm_cdf_info(
         int);
   extern void get_ctip_cdf_info(
         int);
   extern void get_enlil_cdf_info(
         int);
   extern void get_kpvt_cdf_info(
         int);
//   extern void get_magic_cdf_info(
//         int);
   extern void get_adapt3d_cdf_info(
         int);
   extern void get_msfc_tvm_cdf_info(
         int);
   extern void get_mas_cdf_info(
         int);

   char *batsrus = "batsrus";
   char *ucla_ggcm = "ucla_ggcm";
   char *open_ggcm = "open_ggcm";
   char *ctip = "ctip";
   char *enlil = "enlil";
   char *kpvt = "kpvt";
   char *magnetogram = "magnetogram";
   char *magic = "magnetogram";
   char *adapt3d = "ADAPT3D";
   char *msfc_tvm = "msfc_tvm";
   char *mas = "mas";

   long model_name_attr_num;
   char *model_name_attr = "model_name";
   static char model_name[50]; /* cdf likes arrays */
   long gattr_str_len[1];

   status
         = CDFlib( GET_, ATTR_NUMBER_, model_name_attr, &model_name_attr_num, NULL_);
   if (status != CDF_OK)
      StatusHandler(status);

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

      printf("DEBUG\tCDF files contains %s model name\n", model_name );
   /********** use model name attribute to determine which get_*_cdf_info routine to call ****************/

   if ( !strcmp(
         batsrus,
         model_name) )
   {

      /*** lets set full_grid_in_memory flag ***/

      full_bats_grid_in_memory = main_memory_flag;

      /*printf("DEBUG\tCDF files contains %s model data\ncalling get_batsrus_cdf_info\n", model_name );*/
      get_batsrus_cdf_info(main_memory_flag);

   }
   else if ( !strcmp(
         ucla_ggcm,
         model_name) )
   {
      /*printf("DEBUG\tCDF files contains %s model data\n", model_name );*/
      get_ucla_ggcm_cdf_info(main_memory_flag);
   }
   else if ( !strcmp(
         open_ggcm,
         model_name) )
   {
      /*printf("DEBUG\tCDF files contains %s model data\n", model_name );*/
      get_ucla_ggcm_cdf_info(main_memory_flag);
   }
   else if ( !strcmp(
         ctip,
         model_name) )
   {
      /*printf("DEBUG\tCDF files contains %s model data\n", model_name );*/
      get_ctip_cdf_info(main_memory_flag);
   }
   else if ( !strcmp(
         enlil,
         model_name) )
   {
      /*printf("DEBUG\tCDF files contains %s model data\n", model_name );*/
      get_enlil_cdf_info(main_memory_flag);
   }
   else if ( !strcmp(
         kpvt,
         model_name) )
   {
      /*printf("DEBUG\tCDF files contains %s model data\n", model_name );*/
      get_kpvt_cdf_info(main_memory_flag);
   }
//   else if ( !strcmp(
//         magic,
//         model_name) )
//   {
//      /*printf("DEBUG\tCDF files contains %s model data\n", model_name );*/
//      get_magic_cdf_info(main_memory_flag);
//   }
//   else if ( !strcmp( magnetogram, model_name) )
//   {
//      printf("DEBUG\tCDF files contains %s model data\n", model_name );
//      get_magic_cdf_info(main_memory_flag);
//   }
   else if ( !strcmp(
         adapt3d,
         model_name) )
   {
      /*printf("DEBUG\tCDF files contains %s model data\n", model_name );*/
      get_adapt3d_cdf_info(main_memory_flag);
   }
   else if ( !strcmp(
         msfc_tvm,
         model_name) )
   {
      /*printf("DEBUG\tCDF files contains %s model data\n", model_name );*/
      get_msfc_tvm_cdf_info(main_memory_flag);
   }
   else if ( !strcmp(
         mas,
         model_name) )
   {
      /*printf("DEBUG\tCDF files contains %s model data\n", model_name );*/
      get_mas_cdf_info(main_memory_flag);
   }
   else
   {
      printf("ERROR\tCDF files contains unsupported model data\n\n");
   }

   return model_name;

}

int valid_input_variable(
      char *current_input_variable_name)
{

   /************** big or **************/

   if ( !strcmp(
         current_input_variable_name,
         br_name) || !strcmp(
         current_input_variable_name,
         bphi_name) || !strcmp(
         current_input_variable_name,
         btheta_name) || !strcmp(
         current_input_variable_name,
         bx_name) || !strcmp(
         current_input_variable_name,
         by_name) || !strcmp(
         current_input_variable_name,
         bz_name) || !strcmp(
         current_input_variable_name,
         b1x_name) || !strcmp(
         current_input_variable_name,
         b1y_name) || !strcmp(
         current_input_variable_name,
         b1z_name) || !strcmp(
         current_input_variable_name,
         bx1_name) || !strcmp(
         current_input_variable_name,
         by1_name) || !strcmp(
         current_input_variable_name,
         bz1_name) || !strcmp(
         current_input_variable_name,
         ur_name) || !strcmp(
         current_input_variable_name,
         uphi_name) || !strcmp(
         current_input_variable_name,
         utheta_name) || !strcmp(
         current_input_variable_name,
         ux_name) || !strcmp(
         current_input_variable_name,
         uy_name) || !strcmp(
         current_input_variable_name,
         uz_name) || !strcmp(
         current_input_variable_name,
         jr_name) || !strcmp(
         current_input_variable_name,
         jphi_name) || !strcmp(
         current_input_variable_name,
         jtheta_name) || !strcmp(
         current_input_variable_name,
         jx_name) || !strcmp(
         current_input_variable_name,
         jy_name) || !strcmp(
         current_input_variable_name,
         jz_name) || !strcmp(
         current_input_variable_name,
         rho_name) || !strcmp(
         current_input_variable_name,
         p_name) || !strcmp(
         current_input_variable_name,
         temp_name) || !strcmp(
         current_input_variable_name,
         e_name) || !strcmp(
         current_input_variable_name,
         dp_name) || !strcmp(
         current_input_variable_name,
         bp_name) || !strcmp(
         current_input_variable_name,
         Vn_x_name) || !strcmp(
         current_input_variable_name,
         Vn_y_name) || !strcmp(
         current_input_variable_name,
         Vn_z_name) || !strcmp(
         current_input_variable_name,
         Vi_x_name) || !strcmp(
         current_input_variable_name,
         Vi_y_name) || !strcmp(
         current_input_variable_name,
         N_e_name) || !strcmp(
         current_input_variable_name,
         N_O_name) || !strcmp(
         current_input_variable_name,
         N_O2_name) || !strcmp(
         current_input_variable_name,
         N_N2_name) || !strcmp(
         current_input_variable_name,
         Rmt_name) || !strcmp(
         current_input_variable_name,
         T_name) || !strcmp(
         current_input_variable_name,
         Tn_name) || !strcmp(
         current_input_variable_name,
         sigmaP_name) || !strcmp(
         current_input_variable_name,
         "H") || !strcmp(
         current_input_variable_name,
         sigmaH_name) || !strcmp(
         current_input_variable_name,
         net_flux_name) || !strcmp(
         current_input_variable_name,
         total_flux_name) || !strcmp(
         current_input_variable_name,
         weights_name) || !strcmp(
         current_input_variable_name,
         b_t_name) || !strcmp(
         current_input_variable_name,
         b_l_name) || !strcmp(
         current_input_variable_name,
         raz_name) || !strcmp(
         current_input_variable_name,
         eta_name) )
   {
      return 1;
   } /** abother big or to test for validity of ENLIL input variables  **/
   else if ( !strcmp(
         current_input_variable_name,
         rho_1_name) || !strcmp(
         current_input_variable_name,
         rho_2_name) || !strcmp(
         current_input_variable_name,
         rho_3_name) || !strcmp(
         current_input_variable_name,
         rho_4_name) || !strcmp(
         current_input_variable_name,
         T_1_name) || !strcmp(
         current_input_variable_name,
         T_2_name) || !strcmp(
         current_input_variable_name,
         T_3_name) || !strcmp(
         current_input_variable_name,
         T_4_name) || !strcmp(
         current_input_variable_name,
         ur_1_name) || !strcmp(
         current_input_variable_name,
         ur_2_name) || !strcmp(
         current_input_variable_name,
         ur_3_name) || !strcmp(
         current_input_variable_name,
         ur_4_name) || !strcmp(
         current_input_variable_name,
         uphi_1_name) || !strcmp(
         current_input_variable_name,
         uphi_2_name) || !strcmp(
         current_input_variable_name,
         uphi_3_name) || !strcmp(
         current_input_variable_name,
         uphi_4_name) || !strcmp(
         current_input_variable_name,
         btheta_1_name) || !strcmp(
         current_input_variable_name,
         btheta_2_name) || !strcmp(
         current_input_variable_name,
         btheta_3_name) || !strcmp(
         current_input_variable_name,
         btheta_4_name) || !strcmp(
         current_input_variable_name,
         bp_1_name) || !strcmp(
         current_input_variable_name,
         bp_2_name) || !strcmp(
         current_input_variable_name,
         bp_3_name) || !strcmp(
         current_input_variable_name,
         bp_4_name) || !strcmp(
         current_input_variable_name,
         b1r_name) || !strcmp(
         current_input_variable_name,
         b1phi_name)|| !strcmp(
         current_input_variable_name,
         b1theta_name) || !strcmp(
         current_input_variable_name,
         time_1_name) || !strcmp(
         current_input_variable_name,
         time_2_name) || !strcmp(
         current_input_variable_name,
         time_3_name) || !strcmp(
         current_input_variable_name,
         time_4_name) || !strcmp(
         current_input_variable_name,
         time_step_1_name) || !strcmp(
         current_input_variable_name,
         time_step_2_name) || !strcmp(
         current_input_variable_name,
         time_step_3_name) || !strcmp(
         current_input_variable_name,
         time_step_4_name) || !strcmp(
         current_input_variable_name,
         numerical_step_1_name) || !strcmp(
         current_input_variable_name,
         numerical_step_2_name) || !strcmp(
         current_input_variable_name,
         numerical_step_3_name) || !strcmp(
         current_input_variable_name,
         numerical_step_4_name) )
   {
      return 1;
   } /** abother big or to test for validity of MAGIC input variables  **/
   else if ( !strcmp(
         current_input_variable_name,
         x_name) || !strcmp(
         current_input_variable_name,
         y_name) || !strcmp(
         current_input_variable_name,
         z_name) || !strcmp(
         current_input_variable_name,
         time_range_name) || !strcmp(
         current_input_variable_name,
         longitude_name) || !strcmp(
         current_input_variable_name,
         latitude_name) || !strcmp(
         current_input_variable_name,
         b_x_name) || !strcmp(
         current_input_variable_name,
         b_y_name) || !strcmp(
         current_input_variable_name,
         b_z_name) || !strcmp(
         current_input_variable_name,
         v_x_name) || !strcmp(
         current_input_variable_name,
         v_y_name) || !strcmp(
         current_input_variable_name,
         v_z_name) || !strcmp(
         current_input_variable_name,
         b_x_BSpline_Coeff_name) || !strcmp(
         current_input_variable_name,
         b_y_BSpline_Coeff_name) || !strcmp(
         current_input_variable_name,
         b_z_BSpline_Coeff_name) || !strcmp(
         current_input_variable_name,
         v_x_BSpline_Coeff_name) || !strcmp(
         current_input_variable_name,
         v_y_BSpline_Coeff_name) || !strcmp(
         current_input_variable_name,
         v_z_BSpline_Coeff_name) )
   {
      return 1;
   } /** abother big or to test for validity of ADAPT3D input variables  **/
   else if ( !strcmp(
         current_input_variable_name,
         coord_name) || !strcmp(
         current_input_variable_name,
         unkno_name) || !strcmp(
         current_input_variable_name,
         intmat_name) || !strcmp(
         current_input_variable_name,
         bconi_name) )
   {
      return 1;
   }
   else
   {
      return 0;
   }

}

