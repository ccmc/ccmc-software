/*******************************************************************************
 *                                                                             *
 *    NAME:          read_ucla_ggcm.c                                          *
 *                                                                             *
 *    VERSION:       3                                                         *
 *                                                                             *
 *    AUTHOR:        Marlo M. H. Maddox                                        *
 *                   NASA-GSFC-AETD-ISD-ADMA(587)                              *
 *                   Marlo.Maddox@nasa.gov                                     *
 *                   (301) 286 - 5202                                          *
 *                                                                             *
 *    PURPOSE:       Custom read routine for UCLA-GGCM model output.  All      *
 *                   available UCLA-GGCM data is read into main memory were it *
 *                   can then be written into a standard format with a specific*
 *                   write module.                                             *
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
 *    03.22.2003     Maddox, Marlo                                             *
 *                   initial development started                               *
 *    09.09.2004     Maddox, Marlo                                             *
 *                   adding actual min/max calculations for each variable      *
 *    02.28.2005     Maddox, Marlo                                             *
 *                   trying to get a handle on scoping & memory issues code    *
 *                   used to read/validate *.h files because valid_grid_file & *
 *                   valid_data_file variables where being overwriten...fixed  *
 *                   by changing size of first_line_in_grid_file &             *
 *                   first_line_in_data_file from 25 -> 50.  The *.h files had *
 *                   lines > 25 ie. 44 thus overwriting the int variables      *
 *                   valid_*_file that are declared after first_line_in_*_file *
 *                   arrays safeguard was already in place with                *
 *                   max_compare_len = 50 which is retained for use in fgets() *
 *                   calls                                                     *
 *                                                                             *
 *    03.04.2005     Maddox, Marlo                                             *
 *                   added fclose() for open filePtr located inside            *
 *                   validate_ucla_ggcm_input_files() function                 *
 *    08.30.2005     Maddox, Marlo                                             *
 *                   Turned the file rewind flag ON and added rewinds before   *
 *                   every getf* call for starndard/stagared grid and data     *
 *                   testing to see if nan's disapear.  Fixed gridx[0] nan's   *
 *                   by restricting grid read to once per excecution.  This    *
 *                   is more efficient anyway since the grid files are static  *
 *                   The nan issue arisses when multiple input files are       *
 *                   read in ie. all 3df files.  The fourth 3df file read in   *
 *                   will produce a nan value for the first variable read in   *
 *                   - bx[0] hence the below band-aid. AHHHHHHHHHH             *
 *                                                                             *
 *                   NOTE: the file_rewind_flag causes the converter to run    *
 *                   ~4x slower.  If variables are being read inorder          *
 *                   the rewind isn't needed.                                  *
 *                                                                             *
 *    09.01.2005     Maddox, Marlo                                             *
 *                   Added a band-aid to force bx[0] from nan to bx[1]         *
 *                   this is a bug in which gridx[0] or bx[0] was being        *
 *                   replaced with nan from rdn2 call.  AHHHHHHHHHHHH          *
 *                                                                             *
 *                   Found BUG.  Added static keyword to declaration for       *
 *                   ucla_grid_file_len, ucla_data_file_len,                   *
 *                   f77_open_file_status_len, f77_open_file_format_len;       *
 *                   the values were being overwritten after the first call    *
 *                   so subsequent f77_file_opens were not receiving the       *
 *                   correct arguments for formatting thus the improper values *
 *                   at element zero of the next variable                      *
 *                   bandaid should no longer be needed.                       *
 *                                                                             *
 *                   Turned file rewind flag back off                          *
 *                                                                             *
 *    03.02.2007     Maddox, Marlo                                             *
 *                   Rewind flag issues has resurfaced.  Instead of having the *
 *                   flag either on or off, check the value set by the fortran *
 *                   read that specifies the size of the read value.  If the   *
 *                   value is -1 try to rewind the file and read again.        *
 *                                                                             *
 *                                                                             *
 *    06.26.2007     Maddox, Marlo                                             *
 *                   Had to add ifdef g77 for the f77_file_close, f77_file_open*
 *                   and f77_file_rewind                                       *
 *    06.10.2008     Rastaetter, Lutz                                          *
 *                   Read grid information from ASCII files with higher        *
 *                   precision.                                                *
 ******************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <dirent.h>
#include <sys/param.h>
#include "read_ucla_ggcm.h"

#define FORTRAN_FILE_OPEN_FORMAT    "formatted"
#define FORTRAN_FILE_OPEN_STATUS    "old"
#define FORTRAN_FILE_OPEN_UNIT_N    10

#define VALID_GRID_TEXT            "FIELD-1D-1"
#define VALID_DATA_TEXT            "FIELD-3D-1"

/** restricts how much data is read in from first line to validate input **/

#define MAX_FIRST_LINE_LEN      50
#define MAX_COMPARE_LEN        50
#define MAX_FILE_NAME_LEN        500
#define MAX_COPY_LEN            450
#define MAX_STATUS_LEN        25

int read_standard_grid(int, char *);
int read_staggered_grids(int, char *);
int allocate_space_for_data(int);
int read_3df_data(int, char *);
int validate_ucla_ggcm_input_files(char *, char *, int);

#ifdef g77
extern void f77_file_open__(
      int *,
      char *,
      int *,
      char *,
      int *,
      char *,
      int *,
      int *);
extern void f77_file_rewind__(int *, int *);
extern void f77_file_close__(int *, int *);
#else
extern void f77_file_open_( int *, char *, int *, char *, int *, char *, int *, int * );
extern void f77_file_rewind_( int *, int * );
extern void f77_file_close_( int *, int * );
#endif

extern void getf11_(int *, float [], int *, char *, char *, int *, int *);
extern void getf31_(
      int *,
      float [],
      int *,
      int *,
      int *,
      char *,
      char *,
      int *,
      int *);

/******************************************************************************/
/* if reading variables out of order, a file rewind is required NOTE: rewinds
 * before each read is 4x slower */
int file_rewind_flag = 0; /****************************************************/
/******************************************************************************/

int nx = 0, ny = 0, nz = 0;
/* pass these values to modified getf* routines so c and fortran arrays only
 * compare relevant elements */
int five_char = 5;
int three_char = 3;
int two_char = 2;

int i, return_status=123;

int f77_open_file_unit_n= FORTRAN_FILE_OPEN_UNIT_N;
char *f77_open_file_status= FORTRAN_FILE_OPEN_STATUS;
char *f77_open_file_format= FORTRAN_FILE_OPEN_FORMAT;

/************* main driver routine *******************************************/

int read_ucla_ggcm(
      char *ucla_data_file,
      char *ucla_grid_file,
      int verbose_flag,
      int minmax_flag)
{

   int read_staggered_grid(int, char *);
   int linear_minmax_search(float *, int, float *, float *);

   int i, j, k, q;
   int string_len;
   static int local_call_count = 0;

   /**** THE PAIN WAS HERE ALL ALONG STATIC ***/
   static int ucla_grid_file_len, ucla_data_file_len, f77_open_file_status_len,
         f77_open_file_format_len;

   char ucla_grid_file_2[MAX_FILE_NAME_LEN];
   char ucla_data_file_2[MAX_FILE_NAME_LEN];

   char f77_open_file_status_2[MAX_STATUS_LEN];
   char f77_open_file_format_2[MAX_STATUS_LEN];


   /* Lutz - add grid read from gridx.txt, gridy.txt, gridz.txt */
   /* read gridx,gridy,gridz files */
   FILE *grid_file;
   int use_gridx=1;
   float tmp_x1,tmp_x3;
   char dum_string[500];
   int have_ucla_grid=0;

   if ( (grid_file = fopen("gridx.txt","r") ) == NULL){
     printf("Could no open gridx.txt\n");
     use_gridx=0;
   } /* i.e., use the grid file provided */
   if (use_gridx) {
     int i;
     printf("Reading gridx.txt\n");
     fscanf(grid_file,"%[^\n]",dum_string);
     for (i=0;i<MAX_GRID_SIZE_X;i++){
       float x1,x2,x3;
       if (fscanf(grid_file,"%f %f %f ",&x1,&x2,&x3) !=3 ){
	 fprintf(stderr,"End of file reached: gridx.txt %i\n",i);
	 break;
       }
       gridx[i]=x1;
       gridx_bx[i]=x3;
       gridx_by[i]=x1;
       gridx_bz[i]=x1;
     }
     fclose(grid_file);
     NX=i;
/* gridy.txt: y, y_by1 */
        if ( (grid_file = fopen("gridy.txt","r") ) == NULL){
            printf("Could no open gridy.txt\n");
	    exit(1);
        } else {
            printf("Reading gridy.txt\n");
            fscanf(grid_file,"%[^\n]",dum_string);
            printf("1st line: %s\n",dum_string);
            for (i=0;i<MAX_GRID_SIZE_Y;i++){
                float x1,x2,x3;
                if (fscanf(grid_file,"%f %f %f ",&x1,&x2,&x3) !=3 ){
		  fprintf(stderr,"End of file reached: gridy.txt: %i\n",i);
		  break;
                }
		gridy[i]=x1;
		gridy_bx[i]=x1;
		gridy_by[i]=x3;
		gridy_bz[i]=x1;
            }
            fclose(grid_file);
	    NY=i;
        }
/* gridz.txt: z, z_bz1 */
        if ( (grid_file = fopen("gridz.txt","r") ) == NULL){
            printf("Could no open gridz.txt\n");
        } else {
            printf("Reading gridz.txt\n");
            fscanf(grid_file,"%[^\n]",dum_string);
            printf("1st line: %s\n",dum_string);
            for (i=0;i<MAX_GRID_SIZE_Z;i++){
                float x1,x2,x3;
                if (fscanf(grid_file,"%f %f %f ",&x1,&x2,&x3) !=3 ){
                    printf("End of file reached: gridz.txt: %i\n",i);
		    break;
                }
                gridz[i]=x1;
                gridz_bx[i]=x1;
                gridz_by[i]=x1;
                gridz_bz[i]=x3;
                fprintf(stderr,"Z: %f  Z_Bz1: %f\n",gridz[i],gridz_bz[i]);
            }
            fclose(grid_file);
	    NZ=i;
        }
	dimension_size=NX*NY*NZ;
	fprintf(stderr,"read_ucla_ggcm_v3: NX=%i NY=%i NZ=%i dimension_size=%i\n",
		NX,NY,NZ,dimension_size);
	/* erase setting for grid file since we successflly read grid information already */
	ucla_grid_file[0]='\0';
	have_ucla_grid=1;
   }


   strncpy(ucla_data_file_2, ucla_data_file, MAX_COPY_LEN);
   strncpy(f77_open_file_status_2, f77_open_file_status, MAX_STATUS_LEN);
   strncpy(f77_open_file_format_2, f77_open_file_format, MAX_STATUS_LEN);

   ucla_data_file_2[strlen(ucla_data_file)] = ' ';
   f77_open_file_status_2[strlen( f77_open_file_status ) ] = ' ';
   f77_open_file_format_2[strlen( f77_open_file_format ) ] = ' ';

   return_status = validate_ucla_ggcm_input_files(
         ucla_grid_file,
         ucla_data_file,
         verbose_flag);

   /****
    * if both files were valid continue on , else return control to
    * main_read_driver with status 0 so it can find another data file if
    * available NOTE: control will not even make it back here if grid file was
    * invalid. validate function would have exited program there *****/

   if ( !return_status)
   {
      return 0;
   }

   /******* -i flag to converter that allows a directory of data to be specified
    * breaks after 4 iterations and inserts a nan value for gridx[0] so lets
    * just read the grid stuff once... if the local_call_count > 0 skip the
    * read_*grid stuff **********************************/

   if ( have_ucla_grid == 0){ 
/* the following code section is not used if gridx.txt, gridy.txt and gridz.txt 
   were read above */
   if ( !local_call_count)
   {

      strncpy(ucla_grid_file_2, ucla_grid_file, MAX_COPY_LEN);

      ucla_grid_file_2[strlen(ucla_grid_file)] = ' ';

      /**** validate grid and 3df file before proceeding, if grid file is bad we
       * need to exit program. If 3df file is bad try the next 3df file ****/

      /**** only continue if both files are valid *********/

      /*  open grid file using fortran file open in external f77_file_open_ */

      if (verbose_flag)
      {
         printf("\nopening ucla_ggcm grid file:\t%s\n\n", ucla_grid_file);
      }

      /* get string lenghts to pass on to fortran routine */

      ucla_grid_file_len = strlen(ucla_grid_file);
      f77_open_file_status_len = strlen(f77_open_file_status);
      f77_open_file_format_len = strlen(f77_open_file_format);

#ifdef g77
      f77_file_open__(
            &f77_open_file_unit_n,
            &ucla_grid_file_2[0],
            &ucla_grid_file_len,
            &f77_open_file_status_2[0],
            &f77_open_file_status_len,
            &f77_open_file_format_2[0],
            &f77_open_file_format_len,
            &return_status);
#else
      f77_file_open_( &f77_open_file_unit_n, &ucla_grid_file_2[0], &ucla_grid_file_len, &f77_open_file_status_2[0], &f77_open_file_status_len, &f77_open_file_format_2[0], &f77_open_file_format_len, &return_status );
#endif

      if (return_status)
      {
         printf("ERROR\tfortran open statement failed for %s\n", ucla_grid_file);
      } /* ERROR HANDLER */

      /* read regular grid positions:gridx, gridy, gridz using fortran getf11() */

      return_status = read_standard_grid(verbose_flag, ucla_grid_file);

      if (return_status)
      {
         if (verbose_flag)
         {
            printf("\nstandard grid positions [ gridx, gridy, gridz ] read "
               "successfully.\n\n");
         }
         local_call_count++;
      }
      else
      {
         /**** ERROR HANDLER *******/
         printf("ERROR:\tread_standard_grid FAILED !\n");
      }

      /******** read staggered grid positions: gx-bx, gy-bx, gz-bx, gx-by, gy-by,
       * gz-by, gx-bz, gy-bz, gz-bz using fortran getf11() *************/

      return_status = read_staggered_grid(verbose_flag, ucla_grid_file);

      if (return_status)
      {
         if (verbose_flag)
         {
            printf("\nstaggered grid positions [ gx-bx, gy-bx, gz-bx ] , "
               "[ gx-by, gy-by, gz-by ] , [ gx-bz, gy-bz, gz-bz ] read "
               "successfully....\n\n");
         }
      }
      else
      {
         /**** ERROR HANDLER *******/
	printf("ERROR:\tread_staggered_grid FAILED !\n");
	exit (1);
      }

      /*  close grid file using fortran file close in external f77_file_close_ */

#ifdef g77
      f77_file_close__( &f77_open_file_unit_n, &return_status);
#else
      f77_file_close_( &f77_open_file_unit_n, &return_status );
#endif

      if (return_status)
      {
         printf(
               "ERROR\tfortran close statement failed for %s\n",
               ucla_grid_file);
      } /* ERROR HANDLER */

     } /*** end of if local_call_count > 0 ****/
   } /*** end of grid reading section - turned of if we read ascii files */

   /*************** allocate space for data arrays ***************/

   /* no need to test return status, function will never return control if an
    * error occurs instead will exit in function */

   return_status = allocate_space_for_data(dimension_size);

   /*  open 3df data file using fortran file open in external f77_file_open_ */

   f77_open_file_unit_n = 10; /* reset f77_open_file_unit_n */

   ucla_data_file_len = strlen(ucla_data_file);

#ifdef g77
   f77_file_open__(
         &f77_open_file_unit_n,
         &ucla_data_file_2[0],
         &ucla_data_file_len,
         &f77_open_file_status_2[0],
         &f77_open_file_status_len,
         &f77_open_file_format_2[0],
         &f77_open_file_format_len,
         &return_status);
#else
   f77_file_open_( &f77_open_file_unit_n, &ucla_data_file_2[0],
         &ucla_data_file_len, &f77_open_file_status_2[0],
         &f77_open_file_status_len, &f77_open_file_format_2[0],
         &f77_open_file_format_len, &return_status );
#endif

   if (return_status)
   {
      printf("ERROR\tfortran open statement failed for %s\n", ucla_data_file);
   } /* ERROR HANDLER */

   nx = 0, ny = 0, nz = 0; /* reset nx, ny, nz */

   /************** read 3df data **********************************/

   return_status = read_3df_data(verbose_flag, ucla_data_file);

   if (return_status)
   {
      if (verbose_flag)
      {
         printf("\ndata values read successfully....\n\n");
      }
   }
   else
   {
      /**** ERROR HANDLER *******/
      printf("ERROR:\tread_3df_data FAILED !\n");
   }

   /* reset for debugging */
   return_status=123;
   f77_open_file_unit_n = 10;

#ifdef g77
   f77_file_close__( &f77_open_file_unit_n, &return_status);
#else
   f77_file_close_( &f77_open_file_unit_n, &return_status );
#endif

   if (return_status)
   {
      /* ERROR HANDLER */
      printf("ERROR\tfortran close statement failed for %s\n", ucla_data_file);
   }

   /*********************  clacluate actual min/max values for each ariable
    * unless -nominmax flag was specified *************/

   /**** add more error handling for each linear_minmax_search function call */

   if ( !minmax_flag) /*** if -nominmax option was NOT specified ***/
   {

      if (verbose_flag)
      {
         printf("\ncalculating actual minimum & maximum values for each "
            "variable ...\n");
      }

      if (verbose_flag)
      {
         printf("%-25s%-25s%-25s\n", "", "min", "max");
      }

      linear_minmax_search(gridx, NX, &gridx_actual_min, &gridx_actual_max);
      if (verbose_flag)
      {
         printf(
               "%-25s%-25g%-25g\n",
               "gridx",
               gridx_actual_min,
               gridx_actual_max);
      }

      linear_minmax_search(gridy, NY, &gridy_actual_min, &gridy_actual_max);
      if (verbose_flag)
      {
         printf(
               "%-25s%-25g%-25g\n",
               "gridy",
               gridy_actual_min,
               gridy_actual_max);
      }

      linear_minmax_search(gridz, NZ, &gridz_actual_min, &gridz_actual_max);
      if (verbose_flag)
      {
         printf(
               "%-25s%-25g%-25g\n",
               "gridz",
               gridz_actual_min,
               gridz_actual_max);
      }

      linear_minmax_search(
            gridx_bx,
            NX,
            &gridx_bx_actual_min,
            &gridx_bx_actual_max);
      if (verbose_flag)
      {
         printf(
               "%-25s%-25g%-25g\n",
               "gridx_bx",
               gridx_bx_actual_min,
               gridx_bx_actual_max);
      }

      linear_minmax_search(
            gridy_bx,
            NY,
            &gridy_bx_actual_min,
            &gridy_bx_actual_max);
      if (verbose_flag)
      {
         printf(
               "%-25s%-25g%-25g\n",
               "gridy_bx",
               gridy_bx_actual_min,
               gridy_bx_actual_max);
      }

      linear_minmax_search(
            gridz_bx,
            NZ,
            &gridz_bx_actual_min,
            &gridz_bx_actual_max);
      if (verbose_flag)
      {
         printf(
               "%-25s%-25g%-25g\n",
               "gridz_bx",
               gridz_bx_actual_min,
               gridz_bx_actual_max);
      }

      linear_minmax_search(
            gridx_by,
            NX,
            &gridx_by_actual_min,
            &gridx_by_actual_max);
      if (verbose_flag)
      {
         printf(
               "%-25s%-25g%-25g\n",
               "gridx_by",
               gridx_by_actual_min,
               gridx_by_actual_max);
      }

      linear_minmax_search(
            gridy_by,
            NY,
            &gridy_by_actual_min,
            &gridy_by_actual_max);
      if (verbose_flag)
      {
         printf(
               "%-25s%-25g%-25g\n",
               "gridy_by",
               gridy_by_actual_min,
               gridy_by_actual_max);
      }

      linear_minmax_search(
            gridz_by,
            NZ,
            &gridz_by_actual_min,
            &gridz_by_actual_max);
      if (verbose_flag)
      {
         printf(
               "%-25s%-25g%-25g\n",
               "gridz_by",
               gridz_by_actual_min,
               gridz_by_actual_max);
      }

      linear_minmax_search(
            gridx_bz,
            NX,
            &gridx_bz_actual_min,
            &gridx_bz_actual_max);
      if (verbose_flag)
      {
         printf(
               "%-25s%-25g%-25g\n",
               "gridx_bz",
               gridx_bz_actual_min,
               gridx_bz_actual_max);
      }

      linear_minmax_search(
            gridy_bz,
            NY,
            &gridy_bz_actual_min,
            &gridy_bz_actual_max);
      if (verbose_flag)
      {
         printf(
               "%-25s%-25g%-25g\n",
               "gridy_bz",
               gridy_bz_actual_min,
               gridy_bz_actual_max);
      }

      linear_minmax_search(
            gridz_bz,
            NZ,
            &gridz_bz_actual_min,
            &gridz_bz_actual_max);
      if (verbose_flag)
      {
         printf(
               "%-25s%-25g%-25g\n",
               "gridz_bz",
               gridz_bz_actual_min,
               gridz_bz_actual_max);
      }

      linear_minmax_search(
            density_rr,
            dimension_size,
            &density_rr_actual_min,
            &density_rr_actual_max);
      if (verbose_flag)
      {
         printf(
               "%-25s%-25g%-25g\n",
               "density_rr",
               density_rr_actual_min,
               density_rr_actual_max);
      }

      linear_minmax_search(
            pressure_pp,
            dimension_size,
            &pressure_pp_actual_min,
            &pressure_pp_actual_max);
      if (verbose_flag)
      {
         printf(
               "%-25s%-25g%-25g\n",
               "pressure_pp",
               pressure_pp_actual_min,
               pressure_pp_actual_max);
      }

      linear_minmax_search(
            velocity_vx,
            dimension_size,
            &velocity_vx_actual_min,
            &velocity_vx_actual_max);
      if (verbose_flag)
      {
         printf(
               "%-25s%-25g%-25g\n",
               "velocity_vx",
               velocity_vx_actual_min,
               velocity_vx_actual_max);
      }

      linear_minmax_search(
            velocity_vy,
            dimension_size,
            &velocity_vy_actual_min,
            &velocity_vy_actual_max);
      if (verbose_flag)
      {
         printf(
               "%-25s%-25g%-25g\n",
               "velocity_vy",
               velocity_vy_actual_min,
               velocity_vy_actual_max);
      }

      linear_minmax_search(
            velocity_vz,
            dimension_size,
            &velocity_vz_actual_min,
            &velocity_vz_actual_max);
      if (verbose_flag)
      {
         printf(
               "%-25s%-25g%-25g\n",
               "velocity_vz",
               velocity_vz_actual_min,
               velocity_vz_actual_max);
      }

      linear_minmax_search(
            magnetic_field_bx,
            dimension_size,
            &magnetic_field_bx_actual_min,
            &magnetic_field_bx_actual_max);
      if (verbose_flag)
      {
         printf(
               "%-25s%-25g%-25g\n",
               "magnetic_field_bx",
               magnetic_field_bx_actual_min,
               magnetic_field_bx_actual_max);
      }

      /*** band-aid bx[0] gets set to nan from rdn2 WTF! - its ugly but it will
       * have to do for now gridx was affected prior performing file rewinds to
       * be the culprit until we restricted grid reads to one, this only
       * happens at iteration/file read number 4+ ***/

      if ( isnan( magnetic_field_bx_actual_min ) || isnan( magnetic_field_bx_actual_max ))
      {
         printf("\n!! BUG WARNING !! magnetic field bx has an nan value "
            "somewhere replacing with neighboring value.\nre-doing min/max "
            "search...\n\n");

         magnetic_field_bx[0] = magnetic_field_bx[1];

         linear_minmax_search(
               magnetic_field_bx,
               dimension_size,
               &magnetic_field_bx_actual_min,
               &magnetic_field_bx_actual_max);
         if (verbose_flag)
         {
            printf(
                  "%-25s%-25g%-25g\n",
                  "magnetic_field_bx",
                  magnetic_field_bx_actual_min,
                  magnetic_field_bx_actual_max);
         }
      }

      linear_minmax_search(
            magnetic_field_by,
            dimension_size,
            &magnetic_field_by_actual_min,
            &magnetic_field_by_actual_max);
      if (verbose_flag)
      {
         printf(
               "%-25s%-25g%-25g\n",
               "magnetic_field_by",
               magnetic_field_by_actual_min,
               magnetic_field_by_actual_max);
      }

      linear_minmax_search(
            magnetic_field_bz,
            dimension_size,
            &magnetic_field_bz_actual_min,
            &magnetic_field_bz_actual_max);
      if (verbose_flag)
      {
         printf(
               "%-25s%-25g%-25g\n",
               "magnetic_field_bz",
               magnetic_field_bz_actual_min,
               magnetic_field_bz_actual_max);
      }

      linear_minmax_search(
            magnetic_field_bx1,
            dimension_size,
            &magnetic_field_bx1_actual_min,
            &magnetic_field_bx1_actual_max);
      if (verbose_flag)
      {
         printf(
               "%-25s%-25g%-25g\n",
               "magnetic_field_bx1",
               magnetic_field_bx1_actual_min,
               magnetic_field_bx1_actual_max);
      }

      linear_minmax_search(
            magnetic_field_by1,
            dimension_size,
            &magnetic_field_by1_actual_min,
            &magnetic_field_by1_actual_max);
      if (verbose_flag)
      {
         printf(
               "%-25s%-25g%-25g\n",
               "magnetic_field_by1",
               magnetic_field_by1_actual_min,
               magnetic_field_by1_actual_max);
      }

      linear_minmax_search(
            magnetic_field_bz1,
            dimension_size,
            &magnetic_field_bz1_actual_min,
            &magnetic_field_bz1_actual_max);
      if (verbose_flag)
      {
         printf(
               "%-25s%-25g%-25g\n",
               "magnetic_field_bz1",
               magnetic_field_bz1_actual_min,
               magnetic_field_bz1_actual_max);
      }

      linear_minmax_search(
            current_density_xjx,
            dimension_size,
            &current_density_xjx_actual_min,
            &current_density_xjx_actual_max);
      if (verbose_flag)
      {
         printf(
               "%-25s%-25g%-25g\n",
               "current_density_xjx",
               current_density_xjx_actual_min,
               current_density_xjx_actual_max);
      }

      linear_minmax_search(
            current_density_xjy,
            dimension_size,
            &current_density_xjy_actual_min,
            &current_density_xjy_actual_max);
      if (verbose_flag)
      {
         printf(
               "%-25s%-25g%-25g\n",
               "current_density_xjz",
               current_density_xjy_actual_min,
               current_density_xjy_actual_max);
      }

      linear_minmax_search(
            current_density_xjz,
            dimension_size,
            &current_density_xjz_actual_min,
            &current_density_xjz_actual_max);
      if (verbose_flag)
      {
         printf(
               "%-25s%-25g%-25g\n",
               "current_density_xjz",
               current_density_xjz_actual_min,
               current_density_xjz_actual_max);
      }

      /*  this array was empty, why is it defined in read_ucla_ggcm.h
       linear_minmax_search(
       resitivity_resis,
       dimension_size,
       &resitivity_resis_actual_min,
       &resitivity_resis_actual_max);
       if (verbose_flag)
       {
       printf(
       "%-25s%-25g%-25g\n",
       "resitivity_resis",
       resitivity_resis_actual_min,
       resitivity_resis_actual_max);
       }
       */

   }

   return 1;

}

/******************* validate_ucla_ggcm_input_files ***************************/

int validate_ucla_ggcm_input_files(
      char *ucla_grid_file,
      char *ucla_data_file,
      int verbose_flag)
{

   FILE *filePtr;

   /** data read from first line is stored into these arrays,
    * ensure max_compare_len argument to fgets is less than or equal to
    * MAX_FIRST_LINE_LEN **/
   char first_line_in_grid_file[MAX_FIRST_LINE_LEN];
   char first_line_in_data_file[MAX_FIRST_LINE_LEN];

   int valid_grid_file = 0;
   int valid_data_file = 0;

   /** restrict how much data is read in from first line of file, data can be
    * validated by reading the first 11 or so characters **/
   int max_compare_len= MAX_COMPARE_LEN;
   int string_len;

   char *valid_grid_text= VALID_GRID_TEXT;
   char *valid_data_text= VALID_DATA_TEXT;

   if (verbose_flag)
   {
      printf("validating input files...\n");
   }

   /******* first test grid file for validity, exit if invalid **********/
   if (strlen(ucla_grid_file) > 0){
    if ( (filePtr = fopen(ucla_grid_file, "r") ) == NULL)
    {
      fprintf( stderr, "ERROR opening grid file: %s \nEXITING "
      "PROGRAM.", ucla_grid_file);
      exit(1);
    }
    else
    {
      fgets(first_line_in_grid_file, max_compare_len, filePtr);

      /* strcmp brakes on empty strings and binary files with segmaentation
       * faults, try to intercept these */

      if ( !strlen(first_line_in_grid_file) )
      {
         return 0;
      }

      string_len = strlen(first_line_in_grid_file);
      first_line_in_grid_file[string_len] = '\0';

      if (verbose_flag)
      {
         printf(
               "first line in specified grid file = %s\n",
               first_line_in_grid_file);
      }

      if (strstr(first_line_in_grid_file, valid_grid_text) != NULL)
      {
         valid_grid_file = 1;
      }
      else
      {
         fprintf( stderr, "\n!!! ERROR: specified grid file ---> %s <--- is "
         "NOT a valid UCLA GGCM grid file!  YOU MUST SPECIFY A VALID GRID"
         " FILE\nEXITING PROGRAM.", ucla_grid_file);
         exit(1);
      }

    }

    fclose(filePtr);
   } else { valid_grid_file = 1; } /* we didn't get a file to test */

   if (verbose_flag)
   {
      printf("valid_grid_file flag being set = %d\n", valid_grid_file);
   }

   /******* now test data file for validity return control to read_ucla function
    * if data file is invalid with return status of 0 *******/

   if ( (filePtr = fopen(ucla_data_file, "r") ) == NULL)
   {
      fprintf( stderr, "ERROR opening data file: %s \nEXITING "
      "PROGRAM.", ucla_data_file);
      exit(1);
   }
   else
   {
      fgets(first_line_in_data_file, max_compare_len, filePtr);

      /* strcmp brakes on empty strings and binary files with segmaentation
       * faults, try to intercept these */

      if ( !strlen(first_line_in_data_file) )
      {
         return 0;
      }

      string_len = strlen(first_line_in_data_file);
      first_line_in_data_file[string_len] = '\0';

      if (verbose_flag)
      {
         printf(
               "first line in specified data file = %s\n",
               first_line_in_data_file);
      }

      if (strstr(first_line_in_data_file, valid_data_text) != NULL)
      {
         valid_data_file = 1;
      }
      else
      {
         /* datafile is not valid do something */
         if (verbose_flag)
         {
            printf("ERROR reading data_file %s\n", ucla_data_file);
         }
      }

   }

   fclose(filePtr);

   if (verbose_flag)
   {
      printf("valid_data_file flag being set = %d\n", valid_data_file);
   }

   if (valid_grid_file && valid_data_file)
   {
      return 1;
   }
   else
   {
      return 0;
   }

}

/*******************************************************************************
 *****************     read gridx, gridy, gridz through fortran getf11()  ******
 ******************************************************************************/

int read_standard_grid(int verbose_flag, char *ucla_grid_file)
{

   /* terminate string again, for some reason second calls to string names
    * ( defined in read_ucla.h ) behave funny */

   gridx_name[7] = '\0';

   if (verbose_flag)
   {
      printf("reading %s...\n", gridx_name);
   }

   getf11_(
         &f77_open_file_unit_n,
         &gridx[0],
         &nx,
         &gridx_name[0],
         &l2[0],
         &it,
         &five_char);

   if (nx == -1)
   {
      printf("%s\tWARNING: EOF or file read error encountered. Rewinding file "
         "and retrying...",
      __FILE__ );

#ifdef g77
            f77_file_rewind__( &f77_open_file_unit_n, &return_status );
#else f77_file_rewind_( &f77_open_file_unit_n, &return_status );
#endif

      if( return_status )
      {  printf("ERROR\tfortran rewind statement failed for "
               "%s\n", ucla_grid_file );} /* ERROR HANDLER */

      /** try to read the file again **/

      getf11_( &f77_open_file_unit_n, &gridx[0], &nx, &gridx_name[0],
            &l2[0], &it, &five_char );
      if( nx != -1 ) printf("retry successful.\n");
   }

   /* terminate string again, for some reason second calls to string names
    * ( defined in read_ucla.h ) behave funny */

   gridy_name[7] = '\0';

   if( verbose_flag )
   {  printf("reading %s...\n", gridy_name );}

   getf11_( &f77_open_file_unit_n, &gridy[0], &ny, &gridy_name[0], &l2[0], &it,
         &five_char );

   if( ny == -1 )
   {
      printf("%s\tWARNING: EOF or file read error encountered. Rewinding file "
            "and retrying...", __FILE__ );

#ifdef g77
      f77_file_rewind__( &f77_open_file_unit_n, &return_status );
#else
      f77_file_rewind_( &f77_open_file_unit_n, &return_status );
#endif

      if( return_status )
      {  printf("ERROR\tfortran rewind statement failed for "
               "%s\n", ucla_grid_file );} /* ERROR HANDLER */

      /** try to read the file again **/

      getf11_( &f77_open_file_unit_n, &gridy[0], &ny, &gridy_name[0], &l2[0],
            &it, &five_char );
      if( ny != -1 ) printf("retry successful.\n");
   }

   /* terminate string again, for some reason second calls to string names
    * ( defined in read_ucla.h ) behave funny */

   gridz_name[7] = '\0';

   if( verbose_flag )
   {  printf("reading %s...\n", gridz_name );}

   getf11_( &f77_open_file_unit_n, &gridz[0], &nz, &gridz_name[0], &l2[0], &it,
         &five_char );

   if( nz == -1 )
   {
      printf("%s\tWARNING: EOF or file read error encountered. Rewinding file "
            "and retrying...", __FILE__ );

#ifdef g77
      f77_file_rewind__( &f77_open_file_unit_n, &return_status );
#else
      f77_file_rewind_( &f77_open_file_unit_n, &return_status );
#endif

      if( return_status )
      {  printf("ERROR\tfortran rewind statement failed for "
               "%s\n", ucla_grid_file );} /* ERROR HANDLER */

      /** try to read the file again **/

      getf11_( &f77_open_file_unit_n, &gridz[0], &nz, &gridz_name[0], &l2[0],
            &it, &five_char );
      if( nz != -1 ) printf("retry successful.\n");
   }

   /* store main dimensions size information before getting staggered stuff */

   NX = nx;
   NY = ny;
   NZ = nz;
   dimension_size = NX * NY * NZ;

   if( nx> 0 && ny> 0 && nz> 0 )
   {
      /* nx|ny|nz will be equal to -1 if there was a problem reading */
      return 1;
   }
   else return -1;

}

/******** read staggered grid positions: gx-bx, gy-bx, gz-bx, gx-by, gy-by,
 * gz-by, gx-bz, gy-bz, gz-bz *************/

int read_staggered_grid(int verbose_flag, char *ucla_grid_file)
{
   int return_code = 1;

   /* terminate string again, for some reason second calls to string names
    * ( defined in read_ucla.h ) behave funny */

   gridx_bx_name[7] = '\0';

   if (verbose_flag)
   {
      printf("reading\t%s...\n", gridx_bx_name);
   }

   getf11_(
         &f77_open_file_unit_n,
         &gridx_bx[0],
         &nx,
         &gridx_bx_name[0],
         &l2[0],
         &it,
         &five_char);
   /*getf11_( &f77_open_file_unit_n, &gridx_bx[0], &nx, &gridx_bx_name, &l2[0],
    * &it, &five_char );*/

   if (nx == -1)
   {
      printf("%s\tWARNING: EOF or file read error encountered. Rewinding file "
         "and retrying...",
      __FILE__ );
#ifdef g77
            f77_file_rewind__( &f77_open_file_unit_n, &return_status );
#else
            f77_file_rewind_( &f77_open_file_unit_n, &return_status );
#endif

      if( return_status )
      {  printf("ERROR\tfortran rewind statement failed for "
               "%s\n", ucla_grid_file );} /* ERROR HANDLER */

      /** try to read the file again **/
      getf11_( &f77_open_file_unit_n, &gridx_bx[0], &nx, &gridx_bx_name[0],
            &l2[0], &it, &five_char );
      if( nx != -1 ) printf("retry successful.\n");

   }

   /* terminate string again, for some reason second calls to string names
    * ( defined in read_ucla.h ) behave funny */

   gridy_bx_name[7] = '\0';

   if( verbose_flag )
   {  printf("reading\t%s...\n", gridy_bx_name );}

   getf11_( &f77_open_file_unit_n, &gridy_bx[0], &ny, &gridy_bx_name[0],
         &l2[0], &it, &five_char );

   if( ny == -1 )
   {
      printf("%s\tWARNING: EOF or file read error encountered. Rewinding file "
            "and retrying...", __FILE__ );
#ifdef g77
      f77_file_rewind__( &f77_open_file_unit_n, &return_status );
#else
      f77_file_rewind_( &f77_open_file_unit_n, &return_status );
#endif

      if( return_status )
      {  printf("ERROR\tfortran rewind statement failed for "
               "%s\n", ucla_grid_file );} /* ERROR HANDLER */

      /** try to read the file again **/
      getf11_( &f77_open_file_unit_n, &gridy_bx[0], &ny, &gridy_bx_name[0],
            &l2[0], &it, &five_char );
      if( ny != -1 ) printf("retry successful.\n");
   }

   gridz_bx_name[7] = '\0'; /* terminate string again, for some reason second
    calls to string names ( defined in read_ucla.h ) behave funny */

   if( verbose_flag )
   {  printf("reading\t%s...\n", gridz_bx_name );}

   getf11_( &f77_open_file_unit_n, &gridz_bx[0], &nz, &gridz_bx_name[0], &l2[0],
         &it, &five_char );

   if( nz == -1 )
   {
      printf("%s\tWARNING: EOF or file read error encountered. Rewinding file "
            "and retrying...", __FILE__ );
#ifdef g77
      f77_file_rewind__( &f77_open_file_unit_n, &return_status );
#else
      f77_file_rewind_( &f77_open_file_unit_n, &return_status );
#endif

      if( return_status )
      {  printf("ERROR\tfortran rewind statement failed for "
               "%s\n", ucla_grid_file );} /* ERROR HANDLER */

      /** try to read the file again **/
      getf11_( &f77_open_file_unit_n, &gridz_bx[0], &nz, &gridz_bx_name[0],
            &l2[0], &it, &five_char );
      if( nz != -1 ) printf("retry successful.\n");
   }

   /* terminate string again, for some reason second calls to string names
    * ( defined in read_ucla.h ) behave funny */

   gridx_by_name[7] = '\0';

   if( verbose_flag )
   {  printf("reading\t%s...\n", gridx_by_name );}

   getf11_( &f77_open_file_unit_n, &gridx_by[0], &nx, &gridx_by_name[0], &l2[0],
         &it, &five_char );

   if( nx == -1 )
   {
      printf("%s\tWARNING: EOF or file read error encountered. Rewinding file "
            "and retrying...", __FILE__ );
#ifdef g77
      f77_file_rewind__( &f77_open_file_unit_n, &return_status );
#else
      f77_file_rewind_( &f77_open_file_unit_n, &return_status );
#endif

      if( return_status )
      {  printf("ERROR\tfortran rewind statement failed for "
               "%s\n", ucla_grid_file );} /* ERROR HANDLER */

      /** try to read the file again **/
      getf11_( &f77_open_file_unit_n, &gridx_by[0], &nx, &gridx_by_name[0],
            &l2[0], &it, &five_char );
      if( nx != -1 ) printf("retry successful.\n");
   }

   /* terminate string again, for some reason second calls to string names
    * ( defined in read_ucla.h ) behave funny */

   gridy_by_name[7] = '\0';

   if( verbose_flag )
   {  printf("reading\t%s...\n", gridy_by_name );}

   getf11_( &f77_open_file_unit_n, &gridy_by[0], &ny, &gridy_by_name[0], &l2[0],
         &it, &five_char );

   if( ny == -1 )
   {
      printf("%s\tWARNING: EOF or file read error encountered. Rewinding file "
            "and retrying...", __FILE__ );
#ifdef g77
      f77_file_rewind__( &f77_open_file_unit_n, &return_status );
#else
      f77_file_rewind_( &f77_open_file_unit_n, &return_status );
#endif

      if( return_status )
      {  printf("ERROR\tfortran rewind statement failed for "
               "%s\n", ucla_grid_file );} /* ERROR HANDLER */

      /** try to read the file again **/
      getf11_( &f77_open_file_unit_n, &gridy_by[0], &ny, &gridy_by_name[0],
            &l2[0], &it, &five_char );
      if( ny != -1 ) printf("retry successful.\n");
   }

   /* terminate string again, for some reason second calls to string names
    * ( defined in read_ucla.h ) behave funny */
   gridz_by_name[7] = '\0';

   if( verbose_flag )
   {  printf("reading\t%s...\n", gridz_by_name );}

   getf11_( &f77_open_file_unit_n, &gridz_by[0], &nz, &gridz_by_name[0], &l2[0],
         &it, &five_char );

   if( nz == -1 )
   {
      printf("%s\tWARNING: EOF or file read error encountered. Rewinding file "
            "and retrying...", __FILE__ );
#ifdef g77
      f77_file_rewind__( &f77_open_file_unit_n, &return_status );
#else
      f77_file_rewind_( &f77_open_file_unit_n, &return_status );
#endif

      if( return_status )
      {  printf("ERROR\tfortran rewind statement failed for "
               "%s\n", ucla_grid_file );} /* ERROR HANDLER */

      /** try to read the file again **/
      getf11_( &f77_open_file_unit_n, &gridz_by[0], &nz, &gridz_by_name[0],
            &l2[0], &it, &five_char );
      if( nz != -1 ) printf("retry successful.\n");
   }

   /* terminate string again, for some reason second calls to string names
    * ( defined in read_ucla.h ) behave funny */

   gridx_bz_name[7] = '\0';

   if( verbose_flag )
   {  printf("reading\t%s...\n", gridx_bz_name );}

   getf11_( &f77_open_file_unit_n, &gridx_bz[0], &nx, &gridx_bz_name[0], &l2[0],
         &it, &five_char );

   if( nx == -1 )
   {
      printf("%s\tWARNING: EOF or file read error encountered. Rewinding file "
            "and retrying...", __FILE__ );
#ifdef g77
      f77_file_rewind__( &f77_open_file_unit_n, &return_status );
#else
      f77_file_rewind_( &f77_open_file_unit_n, &return_status );
#endif

      if( return_status )
      {  printf("ERROR\tfortran rewind statement failed for"
               " %s\n", ucla_grid_file );} /* ERROR HANDLER */

      /** try to read the file again **/
      getf11_( &f77_open_file_unit_n, &gridx_bz[0], &nx, &gridx_bz_name[0],
            &l2[0], &it, &five_char );
      if( nx != -1 ) printf("retry successful.\n");
   }

   /* terminate string again, for some reason second calls to string names
    * ( defined in read_ucla.h ) behave funny */

   gridy_bz_name[7] = '\0';

   if( verbose_flag )
   {  printf("reading\t%s...\n", gridy_bz_name );}

   getf11_( &f77_open_file_unit_n, &gridy_bz[0], &ny, &gridy_bz_name[0], &l2[0],
         &it, &five_char );

   if( ny == -1 )
   {
      printf("%s\tWARNING: EOF or file read error encountered. Rewinding file"
            " and retrying...", __FILE__ );
#ifdef g77
      f77_file_rewind__( &f77_open_file_unit_n, &return_status );
#else
      f77_file_rewind_( &f77_open_file_unit_n, &return_status );
#endif

      if( return_status )
      {  printf("ERROR\tfortran rewind statement failed for "
               "%s\n", ucla_grid_file );} /* ERROR HANDLER */

      /** try to read the file again **/
      getf11_( &f77_open_file_unit_n, &gridy_bz[0], &ny, &gridy_bz_name[0],
            &l2[0], &it, &five_char );
      if( ny != -1 ) printf("retry successful.\n");
   }

   /* terminate string again, for some reason second calls to string names
    * ( defined in read_ucla.h ) behave funny */

   gridz_bz_name[7] = '\0';

   if( verbose_flag )
   {  printf("reading\t%s...\n", gridz_bz_name );}

   getf11_( &f77_open_file_unit_n, &gridz_bz[0], &nz, &gridz_bz_name[0], &l2[0],
         &it, &five_char );

   if( nz == -1 )
   {
      printf("%s\tWARNING: EOF or file read error encountered. Rewinding file "
            "and retrying...", __FILE__ );
#ifdef g77
      f77_file_rewind__( &f77_open_file_unit_n, &return_status );
#else
      f77_file_rewind_( &f77_open_file_unit_n, &return_status );
#endif

      if( return_status )
      {  printf("ERROR\tfortran rewind statement failed for "
               "%s\n", ucla_grid_file );} /* ERROR HANDLER */

      /** try to read the file again **/
      getf11_( &f77_open_file_unit_n, &gridz_bz[0], &nz, &gridz_bz_name[0],
            &l2[0], &it, &five_char );
      if( nz != -1 ) printf("retry successful.\n");
   }

   if( nx < 0 || ny < 0 || nz < 0 )
   {
      /* nx|ny|nz will be equal to -1 if there was a problem reading */
      return_code = -1;
   }

   return return_code;

}

/****************** allocate space for data arrays ****************************/

int allocate_space_for_data(int dimension_size)
{

   /* dynamically allocate space for data arrays using
    * dimension_size = NX * NY * NZ */

   if ( (density_rr = calloc(dimension_size, sizeof(float)) ) == NULL)
   {
      printf(
            "ERROR:\tCALLOC FAILED for density_rr array of size %d\n",
            dimension_size);
      exit( EXIT_FAILURE);
   }
   if ( (pressure_pp = calloc(dimension_size, sizeof(float)) ) == NULL)
   {
      printf(
            "ERROR:\tCALLOC FAILED for pressure_pp array of size %d\n",
            dimension_size);
      exit( EXIT_FAILURE);
   }
   if ( (velocity_vx = calloc(dimension_size, sizeof(float)) ) == NULL)
   {
      printf(
            "ERROR:\tCALLOC FAILED for velocity_vx array of size %d\n",
            dimension_size);
      exit( EXIT_FAILURE);
   }
   if ( (velocity_vy = calloc(dimension_size, sizeof(float)) ) == NULL)
   {
      printf(
            "ERROR:\tCALLOC FAILED for velocity_vy array of size %d\n",
            dimension_size);
      exit( EXIT_FAILURE);
   }
   if ( (velocity_vz = calloc(dimension_size, sizeof(float)) ) == NULL)
   {
      printf(
            "ERROR:\tCALLOC FAILED for velocity_vz array of size %d\n",
            dimension_size);
      exit( EXIT_FAILURE);
   }
   if ( (magnetic_field_bx = calloc(dimension_size, sizeof(float)) ) == NULL)
   {
      printf(
            "ERROR:\tCALLOC FAILED for magnetic_field_bx array of size %d\n",
            dimension_size);
      exit( EXIT_FAILURE);
   }
   if ( (magnetic_field_by = calloc(dimension_size, sizeof(float)) ) == NULL)
   {
      printf(
            "ERROR:\tCALLOC FAILED for magnetic_field_by array of size %d\n",
            dimension_size);
      exit( EXIT_FAILURE);
   }
   if ( (magnetic_field_bz = calloc(dimension_size, sizeof(float)) ) == NULL)
   {
      printf(
            "ERROR:\tCALLOC FAILED for magnetic_field_bz array of size %d\n",
            dimension_size);
      exit( EXIT_FAILURE);
   }
   if ( (magnetic_field_bx1 = calloc(dimension_size, sizeof(float)) ) == NULL)
   {
      printf(
            "ERROR:\tCALLOC FAILED for magnetic_field_bx1 array of size %d\n",
            dimension_size);
      exit( EXIT_FAILURE);
   }
   if ( (magnetic_field_by1 = calloc(dimension_size, sizeof(float)) ) == NULL)
   {
      printf(
            "ERROR:\tCALLOC FAILED for magnetic_field_by1 array of size %d\n",
            dimension_size);
      exit( EXIT_FAILURE);
   }
   if ( (magnetic_field_bz1 = calloc(dimension_size, sizeof(float)) ) == NULL)
   {
      printf(
            "ERROR:\tCALLOC FAILED for magnetic_field_bz1 array of size %d\n",
            dimension_size);
      exit( EXIT_FAILURE);
   }
   if ( (current_density_xjx = calloc(dimension_size, sizeof(float)) ) == NULL)
   {
      printf(
            "ERROR:\tCALLOC FAILED for current_density_xjx array of size %d\n",
            dimension_size);
      exit( EXIT_FAILURE);
   }
   if ( (current_density_xjy = calloc(dimension_size, sizeof(float)) ) == NULL)
   {
      printf(
            "ERROR:\tCALLOC FAILED for current_density_xjy array of size %d\n",
            dimension_size);
      exit( EXIT_FAILURE);
   }
   if ( (current_density_xjz = calloc(dimension_size, sizeof(float)) ) == NULL)
   {
      printf(
            "ERROR:\tCALLOC FAILED for current_density_xjz array of size %d\n",
            dimension_size);
      exit( EXIT_FAILURE);
   }
   if ( (resitivity_resis = calloc(dimension_size, sizeof(float)) ) == NULL)
   {
      printf(
            "ERROR:\tCALLOC FAILED for resitivity_resis array of size %d\n",
            dimension_size);
      exit( EXIT_FAILURE);
   }

   return 1;

}

/******************** read 3df data *******************************************/

int read_3df_data(int verbose_flag, char *ucla_data_file)
{

   int return_code = 1;

   /* terminate string again, for some reason second calls to string names
    * ( defined in read_ucla.h ) behave funny */

   magnetic_field_bx_name[7] = '\0';

   if (verbose_flag)
   {
      printf("reading %s...\n", magnetic_field_bx_name);
   }

   getf31_(
         &f77_open_file_unit_n,
         &magnetic_field_bx[0],
         &nx,
         &ny,
         &nz,
         &magnetic_field_bx_name[0],
         &l2[0],
         &it,
         &two_char);
   if (nx < 0 || ny < 0 || nz < 0)
   {
      return_code = -1;
   } /* nx|ny|nz will be equal to -1 if there was a problem reading */

   if (return_code == -1)
   {
      printf("%s\tWARNING: EOF or file read error encountered. Rewinding file "
         "and retrying...",
      __FILE__ );
#ifdef g77
            f77_file_rewind__( &f77_open_file_unit_n, &return_status );
#else
            f77_file_rewind_( &f77_open_file_unit_n, &return_status );
#endif

if      ( return_status )
      {  printf("ERROR\tfortran rewind statement failed for "
               "%s\n", ucla_data_file );} /* ERROR HANDLER */
      getf31_( &f77_open_file_unit_n, &magnetic_field_bx[0], &nx, &ny, &nz,
            &magnetic_field_bx_name[0], &l2[0], &it, &two_char );
      if( nx >= 0 && ny >= 0 && nz >= 0 ) printf("retry successful.\n");
   }

   /* terminate string again, for some reason second calls to string names
    * ( defined in read_ucla.h ) behave funny */

   magnetic_field_by_name[7] = '\0';

   if( verbose_flag )
   {  printf("reading %s...\n", magnetic_field_by_name );};

   getf31_( &f77_open_file_unit_n, &magnetic_field_by[0], &nx, &ny, &nz,
         &magnetic_field_by_name[0], &l2[0], &it, &two_char );
   if( nx < 0 || ny < 0 || nz < 0 )
   {
      /* nx|ny|nz will be equal to -1 if there was a problem reading */
      return_code = -1;
   }

   if( return_code == -1 )
   {
      printf("%s\tWARNING: EOF or file read error encountered. Rewinding file "
            "and retrying...", __FILE__ );
#ifdef g77
      f77_file_rewind__( &f77_open_file_unit_n, &return_status );
#else
      f77_file_rewind_( &f77_open_file_unit_n, &return_status );
#endif

      if( return_status )
      {  printf("ERROR\tfortran rewind statement failed for "
               "%s\n", ucla_data_file );} /* ERROR HANDLER */
      getf31_( &f77_open_file_unit_n, &magnetic_field_by[0], &nx, &ny, &nz,
            &magnetic_field_by_name[0], &l2[0], &it, &two_char );
      if( nx >= 0 && ny >= 0 && nz >= 0 ) printf("retry successful.\n");
   }

   magnetic_field_bz_name[7] = '\0';

   if( verbose_flag )
   {  printf("reading %s...\n", magnetic_field_bz_name );}

   getf31_( &f77_open_file_unit_n, &magnetic_field_bz[0], &nx, &ny, &nz, &magnetic_field_bz_name[0], &l2[0], &it, &two_char );
   if( nx < 0 || ny < 0 || nz < 0 )
   {
      /* nx|ny|nz will be equal to -1 if there was a problem reading */
      return_code = -1;
   }

   if( return_code == -1 )
   {
      printf("%s\tWARNING: EOF or file read error encountered. Rewinding file "
            "and retrying...", __FILE__ );
#ifdef g77
      f77_file_rewind__( &f77_open_file_unit_n, &return_status );
#else
      f77_file_rewind_( &f77_open_file_unit_n, &return_status );
#endif

      if( return_status )
      {  printf("ERROR\tfortran rewind statement failed for "
               "%s\n", ucla_data_file );} /* ERROR HANDLER */
      getf31_( &f77_open_file_unit_n, &magnetic_field_bz[0], &nx, &ny, &nz,
            &magnetic_field_bz_name[0], &l2[0], &it, &two_char );
      if( nx >= 0 && ny >= 0 && nz >= 0 ) printf("retry successful.\n");
   }

   /* terminate string again, for some reason second calls to string names
    * ( defined in read_ucla.h ) behave funny */

   velocity_vx_name[7] = '\0';

   if( verbose_flag )
   {
      printf("reading %s...\n", velocity_vx_name );
   }

   getf31_( &f77_open_file_unit_n, &velocity_vx[0], &nx, &ny, &nz,
         &velocity_vx_name[0], &l2[0], &it, &two_char );
   if( nx < 0 || ny < 0 || nz < 0 )
   {
      /* nx|ny|nz will be equal to -1 if there was a problem reading */
      return_code = -1;
   }

   if( return_code == -1 )
   {
      printf("%s\tWARNING: EOF or file read error encountered. Rewinding file "
            "and retrying...", __FILE__ );
#ifdef g77
      f77_file_rewind__( &f77_open_file_unit_n, &return_status );
#else
      f77_file_rewind_( &f77_open_file_unit_n, &return_status );
#endif

      if( return_status )
      {  printf("ERROR\tfortran rewind statement failed for "
               "%s\n", ucla_data_file );} /* ERROR HANDLER */
      getf31_( &f77_open_file_unit_n, &velocity_vx[0], &nx, &ny, &nz,
            &velocity_vx_name[0], &l2[0], &it, &two_char );
      if( nx >= 0 && ny >= 0 && nz >= 0 ) printf("retry successful.\n");
   }

   /* terminate string again, for some reason second calls to string names
    * ( defined in read_ucla.h ) behave funny */

   velocity_vy_name[7] = '\0';

   if( verbose_flag )
   {  printf("reading %s...\n", velocity_vy_name );}

   getf31_( &f77_open_file_unit_n, &velocity_vy[0], &nx, &ny, &nz,
         &velocity_vy_name[0], &l2[0], &it, &two_char );
   if( nx < 0 || ny < 0 || nz < 0 )
   {
      /* nx|ny|nz will be equal to -1 if there was a problem reading */
      return_code = -1;
   }

   if( return_code == -1 )
   {
      printf("%s\tWARNING: EOF or file read error encountered. Rewinding file "
            "and retrying...", __FILE__ );
#ifdef g77
      f77_file_rewind__( &f77_open_file_unit_n, &return_status );
#else
      f77_file_rewind_( &f77_open_file_unit_n, &return_status );
#endif

      if( return_status )
      {  printf("ERROR\tfortran rewind statement failed for "
               "%s\n", ucla_data_file );} /* ERROR HANDLER */
      getf31_( &f77_open_file_unit_n, &velocity_vy[0], &nx, &ny, &nz,
            &velocity_vy_name[0], &l2[0], &it, &two_char );
      if( nx >= 0 && ny >= 0 && nz >= 0 ) printf("retry successful.\n");
   }

   velocity_vz_name[7] = '\0'; /* terminate string again, for some reason
    second calls to string names ( defined in read_ucla.h ) behave funny */

   if( verbose_flag )
   {  printf("reading %s...\n", velocity_vz_name );}

   getf31_( &f77_open_file_unit_n, &velocity_vz[0], &nx, &ny, &nz,
         &velocity_vz_name[0], &l2[0], &it, &two_char );
   if( nx < 0 || ny < 0 || nz < 0 )
   {  return_code = -1;} /* nx|ny|nz will be equal to -1 if there was a
    problem reading */

   if( return_code == -1 )
   {
      printf("%s\tWARNING: EOF or file read error encountered. Rewinding file "
            "and retrying...", __FILE__ );
#ifdef g77
      f77_file_rewind__( &f77_open_file_unit_n, &return_status );
#else
      f77_file_rewind_( &f77_open_file_unit_n, &return_status );
#endif

      if( return_status )
      {  printf("ERROR\tfortran rewind statement failed for "
               "%s\n", ucla_data_file );} /* ERROR HANDLER */
      getf31_( &f77_open_file_unit_n, &velocity_vz[0], &nx, &ny, &nz,
            &velocity_vz_name[0], &l2[0], &it, &two_char );
      if( nx >= 0 && ny >= 0 && nz >= 0 ) printf("retry successful.\n");
   }

   density_rr_name[7] = '\0';

   if( verbose_flag )
   {  printf("reading %s...\n", density_rr_name );}

   getf31_( &f77_open_file_unit_n, &density_rr[0], &nx, &ny, &nz,
         &density_rr_name[0], &l2[0], &it, &two_char );
   if( nx < 0 || ny < 0 || nz < 0 )
   {  return_code = -1;} /* nx|ny|nz will be equal to -1 if there was a problem reading */

   if( return_code == -1 )
   {
      printf("%s\tWARNING: EOF or file read error encountered. Rewinding file "
            "and retrying...", __FILE__ );
#ifdef g77
      f77_file_rewind__( &f77_open_file_unit_n, &return_status );
#else
      f77_file_rewind_( &f77_open_file_unit_n, &return_status );
#endif

      if( return_status )
      {  printf("ERROR\tfortran rewind statement failed for "
               "%s\n", ucla_data_file );} /* ERROR HANDLER */
      getf31_( &f77_open_file_unit_n, &density_rr[0], &nx, &ny, &nz,
            &density_rr_name[0], &l2[0], &it, &two_char );
      if( nx >= 0 && ny >= 0 && nz >= 0 ) printf("retry successful.\n");
   }

   pressure_pp_name[7] = '\0';

   if( verbose_flag )
   {  printf("reading %s...\n", pressure_pp_name );}

   getf31_( &f77_open_file_unit_n, &pressure_pp[0], &nx, &ny, &nz,
         &pressure_pp_name[0], &l2[0], &it, &two_char );
   if( nx < 0 || ny < 0 || nz < 0 )
   {
      /* nx|ny|nz will be equal to -1 if there was a problem reading */
      return_code = -1;
   }

   if( return_code == -1 )
   {
      printf("%s\tWARNING: EOF or file read error encountered. Rewinding file "
            "and retrying...", __FILE__ );
#ifdef g77
      f77_file_rewind__( &f77_open_file_unit_n, &return_status );
#else
      f77_file_rewind_( &f77_open_file_unit_n, &return_status );
#endif

      if( return_status )
      {  printf("ERROR\tfortran rewind statement failed for "
               "%s\n", ucla_data_file );} /* ERROR HANDLER */
      getf31_( &f77_open_file_unit_n, &pressure_pp[0], &nx, &ny, &nz,
            &pressure_pp_name[0], &l2[0], &it, &two_char );
      if( nx >= 0 && ny >= 0 && nz >= 0 ) printf("retry successful.\n");
   }

   magnetic_field_bx1_name[7] = '\0';

   if( verbose_flag )
   {  printf("reading %s...\n", magnetic_field_bx1_name );}

   getf31_( &f77_open_file_unit_n, &magnetic_field_bx1[0], &nx, &ny, &nz,
         &magnetic_field_bx1_name[0], &l2[0], &it, &two_char );
   if( nx < 0 || ny < 0 || nz < 0 )
   {  return_code = -1;} /* nx|ny|nz will be equal to -1 if there was a problem reading */

   if( return_code == -1 )
   {
      printf("%s\tWARNING: EOF or file read error encountered. Rewinding file "
            "and retrying...", __FILE__ );
#ifdef g77
      f77_file_rewind__( &f77_open_file_unit_n, &return_status );
#else
      f77_file_rewind_( &f77_open_file_unit_n, &return_status );
#endif

      if( return_status )
      {  printf("ERROR\tfortran rewind statement failed for "
               "%s\n", ucla_data_file );} /* ERROR HANDLER */
      getf31_( &f77_open_file_unit_n, &magnetic_field_bx1[0], &nx, &ny, &nz,
            &magnetic_field_bx1_name[0], &l2[0], &it, &two_char );
      if( nx >= 0 && ny >= 0 && nz >= 0 ) printf("retry successful.\n");
   }

   magnetic_field_by1_name[7] = '\0';

   if( verbose_flag )
   {  printf("reading %s...\n", magnetic_field_by1_name );}

   getf31_( &f77_open_file_unit_n, &magnetic_field_by1[0], &nx, &ny, &nz,
         &magnetic_field_by1_name[0], &l2[0], &it, &two_char );
   if( nx < 0 || ny < 0 || nz < 0 )
   {
      /* nx|ny|nz will be equal to -1 if there was a problem reading */
      return_code = -1;
   }

   if( return_code == -1 )
   {
      printf("%s\tWARNING: EOF or file read error encountered. Rewinding file "
            "and retrying...", __FILE__ );
#ifdef g77
      f77_file_rewind__( &f77_open_file_unit_n, &return_status );
#else
      f77_file_rewind_( &f77_open_file_unit_n, &return_status );
#endif

      if( return_status )
      {  printf("ERROR\tfortran rewind statement failed for "
               "%s\n", ucla_data_file );} /* ERROR HANDLER */
      getf31_( &f77_open_file_unit_n, &magnetic_field_by1[0], &nx, &ny, &nz,
            &magnetic_field_by1_name[0], &l2[0], &it, &two_char );
      if( nx >= 0 && ny >= 0 && nz >= 0 ) printf("retry successful.\n");
   }

   magnetic_field_bz1_name[7] = '\0';

   if( verbose_flag )
   {  printf("reading %s...\n", magnetic_field_bz1_name );}

   getf31_( &f77_open_file_unit_n, &magnetic_field_bz1[0], &nx, &ny, &nz,
         &magnetic_field_bz1_name[0], &l2[0], &it, &two_char );
   if( nx < 0 || ny < 0 || nz < 0 )
   {
      /* nx|ny|nz will be equal to -1 if there was a problem reading */
      return_code = -1;
   }

   if( return_code == -1 )
   {
      printf("%s\tWARNING: EOF or file read error encountered. Rewinding file "
            "and retrying...", __FILE__ );
#ifdef g77
      f77_file_rewind__( &f77_open_file_unit_n, &return_status );
#else
      f77_file_rewind_( &f77_open_file_unit_n, &return_status );
#endif

      if( return_status )
      {  printf("ERROR\tfortran rewind statement failed for "
               "%s\n", ucla_data_file );} /* ERROR HANDLER */
      getf31_( &f77_open_file_unit_n, &magnetic_field_bz1[0], &nx, &ny, &nz,
            &magnetic_field_bz1_name[0], &l2[0], &it, &two_char );
      if( nx >= 0 && ny >= 0 && nz >= 0 ) printf("retry successful.\n");
   }

   current_density_xjx_name[7] = '\0';

   if( verbose_flag )
   {  printf("reading %s...\n", current_density_xjx_name );}

   getf31_( &f77_open_file_unit_n, &current_density_xjx[0], &nx, &ny, &nz,
         &current_density_xjx_name[0], &l2[0], &it, &three_char );
   if( nx < 0 || ny < 0 || nz < 0 )
   {
      /* nx|ny|nz will be equal to -1 if there was a problem reading */
      return_code = -1;
   }

   if( return_code == -1 )
   {
      printf("%s\tWARNING: EOF or file read error encountered. Rewinding file "
            "and retrying...", __FILE__ );
#ifdef g77
      f77_file_rewind__( &f77_open_file_unit_n, &return_status );
#else
      f77_file_rewind_( &f77_open_file_unit_n, &return_status );
#endif

      if( return_status )
      {  printf("ERROR\tfortran rewind statement failed for "
               "%s\n", ucla_data_file );} /* ERROR HANDLER */
      getf31_( &f77_open_file_unit_n, &current_density_xjx[0], &nx, &ny,
            &nz, &current_density_xjx_name[0], &l2[0], &it, &three_char );
      if( nx >= 0 && ny >= 0 && nz >= 0 ) printf("retry successful.\n");
   }

   current_density_xjy_name[7] = '\0';

   if( verbose_flag )
   {  printf("reading %s...\n", current_density_xjy_name );}

   getf31_( &f77_open_file_unit_n, &current_density_xjy[0], &nx, &ny, &nz,
         &current_density_xjy_name[0], &l2[0], &it, &three_char );
   if( nx < 0 || ny < 0 || nz < 0 )
   {
      /* nx|ny|nz will be equal to -1 if there was a problem reading */
      return_code = -1;
   }

   if( return_code == -1 )
   {
      printf("%s\tWARNING: EOF or file read error encountered. Rewinding file "
            "and retrying...", __FILE__ );
#ifdef g77
      f77_file_rewind__( &f77_open_file_unit_n, &return_status );
#else
      f77_file_rewind_( &f77_open_file_unit_n, &return_status );
#endif

      if( return_status )
      {  printf("ERROR\tfortran rewind statement failed for "
            "%s\n", ucla_data_file );} /* ERROR HANDLER */
      getf31_( &f77_open_file_unit_n, &current_density_xjy[0], &nx, &ny, &nz,
            &current_density_xjy_name[0], &l2[0], &it, &three_char );
      if( nx >= 0 && ny >= 0 && nz >= 0 ) printf("retry successful.\n");
   }

   current_density_xjz_name[7] = '\0';

   if( verbose_flag )
   {  printf("reading %s...\n", current_density_xjz_name );}

   getf31_( &f77_open_file_unit_n, &current_density_xjz[0], &nx, &ny, &nz,
         &current_density_xjz_name[0], &l2[0], &it, &three_char );
   if( nx < 0 || ny < 0 || nz < 0 )
   {
      /* nx|ny|nz will be equal to -1 if there was a problem reading */
      return_code = -1;
      }

   if( return_code == -1 )
   {
      printf("%s\tWARNING: EOF or file read error encountered. Rewinding file "
            "and retrying...", __FILE__ );
#ifdef g77
      f77_file_rewind__( &f77_open_file_unit_n, &return_status );
#else
      f77_file_rewind_( &f77_open_file_unit_n, &return_status );
#endif

      if( return_status )
      {  printf("ERROR\tfortran rewind statement failed for "
            "%s\n", ucla_data_file );} /* ERROR HANDLER */
      getf31_( &f77_open_file_unit_n, &current_density_xjz[0], &nx, &ny, &nz,
            &current_density_xjz_name[0], &l2[0], &it, &three_char );
      if( nx >= 0 && ny >= 0 && nz >= 0 ) printf("retry successful.\n");
   }

   return return_code;

}


