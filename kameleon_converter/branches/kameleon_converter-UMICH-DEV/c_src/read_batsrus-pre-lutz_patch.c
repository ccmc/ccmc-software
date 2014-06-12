/*******************************************************************************
 *                                                                             *
 *    NAME:          read_batsrus.c                                            *
 *                                                                             *
 *    VERSION:       2.3                                                       *
 *                                                                             *
 *    AUTHOR:        Marlo M. H. Maddox                                        *
 *                   NASA-GSFC-AETD-ISD-ADMA(587)                              *
 *                   Marlo.Maddox@nasa.gov                                     *
 *                   (301) 286 - 5202                                          *
 *                                                                             *
 *    PURPOSE:       Custom read routine for BATSRUS model output.  All        *
 *                   available BATSRUS data is read into main memory were it   *
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
 *                                                                             *
 *    2003 September 11th     Maddox, Marlo                                    *
 *                            initial development started                      *
 *                                                                             *
 *    2003 December  8th      Maddox, Marlo                                    *
 *                            Testing of block tree implementation revealed 1+ *
 *                            GB  files created to many parents.  Changed test *
 *                            parameters in parent testing loop.  Changed right*
 *                            side compare from mim/max to center.             *
 *                                                                             *
 *    2004 September 9th      Maddox, Marlo                                    *
 *                            adding actual min/max calculations for each      *
 *                            variable                                         *
 *                                                                             *
 *    2005 March 4th          Maddox, Marlo changed function close to fclose   *
 *                            for each opened file                             *
 *                                                                             *
 *    2005 March 18th,19th    Rastaetter, Lutz                                 *
 *                            Added byte swap and double precision             *
 *                            functionality to enable BATSRUS read rotine to   *
 *                            run on both BIG & LITTLE ENDIAN Machines as well *
 *                            read functionality for BATS Framework double     *
 *                            precision .out files                             *
 *                                                                             *
 *    2005 May 3rd            Maddox, Marlo                                    *
 *                            Made endian_flip an external function in         *
 *                            endian_flip.c                                    *
 *                                                                             *
 *    2006 January 4th        Maddox, Marlo                                    *
 *                            linear min/max search for b1x was missing...added*
 *                                                                             *
 *    2006 June 6th           Maddox, Marlo                                    *
 *                            Adding functionality to check for NX, NY,NZ since*
 *                            new version of SWMF-BATSRUS does not have these  *
 *                            in the .out files.  Check .out first and then    *
 *                            database info, if not found throw an error and   *
 *                            don't convert.                                   *
 *                                                                             *
 * *  2006 June 12th          Maddox, Marlo                                    *
 *                            Adding functionality to check for P1, P2,P3 since*
 *                            new version of SWMF-BATSRUS does not have these  *
 *                            in the .out files.  Check .out first and then    *
 *                            database info, if not found throw an error and   *
 *                            don't convert.                                   *
 *                                                                             *
 *    2006 October 31         Maddox, Marlo                                    *
 *                            Changing MAX_FIRST_RECORD from 100 -> 200        *
 *                                                                             *
 ******************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "read_batsrus.h"
#include "endian_flip.h"
#include "strsplit.h"
#include "read_database.h"

#define MAX_FIRST_RECORD        200

#define DEBUG_FLAG 0

int read_record_1(int verbose_flag);
int read_record_2(int verbose_flag);
int read_record_3(int verbose_flag);
int read_record_4(int verbose_flag);
int read_record_5(int verbose_flag);
int read_record_6(int verbose_flag);
int read_record_7(int verbose_flag);
int build_grid_description(int verbose_flag);
int endian_flip(char *array, long unit_len, long n_units);
int four_bytes=4, eight_bytes=8, one_element=1;
float *special_parameter_array;

extern int errno;

/*******************************************************************************
 *                                       MAIN                                  *
 ******************************************************************************/

int read_batsrus(char *input_file_name, int verbose_flag, int minmax_flag)
{

   extern int linear_minmax_search(float *, int, float *, float *);

   int status_code;

   if (verbose_flag)
   {
      printf("...reading %s\n", input_file_name);
   }

   if ( (input_filePtr = fopen(input_file_name, "rb") ) == NULL)
   {
      printf("ERROR Opening File \n");
      perror(input_file_name);
      /* should return control if open was unsucessful */
      return 0;
   }
   else
   {
      /*************** Read Record 1  *********************/

      status_code = read_record_1(verbose_flag);

      if ( !status_code)
      {
         return 0;
      }

      /* if first read record fails, chances are this file is not a valid
       BATRUS output file return control to main read routine */

      /*** 06/08/2006 lets check the return status for each record ****/

      /*************** Read Record 2  *********************/

      status_code = read_record_2(verbose_flag);

      if ( !status_code)
      {
         return 0;
      }

      /*************** Read Record 3  *********************/

      status_code = read_record_3(verbose_flag);

      if ( !status_code)
      {
         return 0;
      }

      /*************** Read Record 4  *********************/

      status_code = read_record_4(verbose_flag);

      if ( !status_code)
      {
         return 0;
      }

      /*************** Read Record 5  *********************/

      status_code = read_record_5(verbose_flag);

      if ( !status_code)
      {
         return 0;
      }

      if ( DEBUG_FLAG)
         printf(
               "DEBUG\tnumber_of_cells = x_dimension_size * y_dimension_size * "
                  "z_dimension_size\n\t = %d * %d * %d = %d\n",
               x_dimension_size,
               y_dimension_size,
               z_dimension_size,
               number_of_cells);

      number_of_cells = x_dimension_size * y_dimension_size * z_dimension_size;

      if ( DEBUG_FLAG)
         printf(
               "DEBUG\tnumber_of_cells = x_dimension_size * y_dimension_size * "
                  "z_dimension_size\n\t = %d * %d * %d = %d\n",
               x_dimension_size,
               y_dimension_size,
               z_dimension_size,
               number_of_cells);

      /*************** Read Record 6  *********************/
      status_code = read_record_6(verbose_flag);

      if ( !status_code)
      {
         return 0;
      }

      /*************** Read Record 7  *********************/
      status_code = read_record_7(verbose_flag);

      if ( !status_code)
      {
         return 0;
      }

   }

   fclose(input_filePtr);

   number_of_blocks = number_of_cells / (special_parameter_NX
         * special_parameter_NY * special_parameter_NZ );

   status_code = build_grid_description(verbose_flag);

   /*********************  calcluate actual min/max values for each ariable
    * unless -nominmax flag was specified *************/

   /**** add more error handling for each linear_minmax_search function
    * call ***/

   /*** if -nominmax option was NOT specified ***/

   if ( !minmax_flag)
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

      linear_minmax_search(
            x_position_arrayPtr,
            number_of_cells,
            &x_position_actual_min,
            &x_position_actual_max);
      if (verbose_flag)
      {
         printf(
               "%-25s%-25g%-25g\n",
               "x_position",
               x_position_actual_min,
               x_position_actual_max);
      }

      linear_minmax_search(
            y_position_arrayPtr,
            number_of_cells,
            &y_position_actual_min,
            &y_position_actual_max);
      if (verbose_flag)
      {
         printf(
               "%-25s%-25g%-25g\n",
               "y_position",
               y_position_actual_min,
               y_position_actual_max);
      }

      linear_minmax_search(
            z_position_arrayPtr,
            number_of_cells,
            &z_position_actual_min,
            &z_position_actual_max);
      if (verbose_flag)
      {
         printf(
               "%-25s%-25g%-25g\n",
               "z_position",
               z_position_actual_min,
               z_position_actual_max);
      }

      linear_minmax_search(
            rho_arrayPtr,
            number_of_cells,
            &rho_actual_min,
            &rho_actual_max);
      if (verbose_flag)
      {
         printf("%-25s%-25g%-25g\n", "rho", rho_actual_min, rho_actual_max);
      }

      linear_minmax_search(
            ux_arrayPtr,
            number_of_cells,
            &ux_actual_min,
            &ux_actual_max);
      if (verbose_flag)
      {
         printf("%-25s%-25g%-25g\n", "ux", ux_actual_min, ux_actual_max);
      }

      linear_minmax_search(
            uy_arrayPtr,
            number_of_cells,
            &uy_actual_min,
            &uy_actual_max);
      if (verbose_flag)
      {
         printf("%-25s%-25g%-25g\n", "uy", uy_actual_min, uy_actual_max);
      }

      linear_minmax_search(
            uz_arrayPtr,
            number_of_cells,
            &uz_actual_min,
            &uz_actual_max);
      if (verbose_flag)
      {
         printf("%-25s%-25g%-25g\n", "uz", uz_actual_min, uz_actual_max);
      }

      linear_minmax_search(
            bx_arrayPtr,
            number_of_cells,
            &bx_actual_min,
            &bx_actual_max);
      if (verbose_flag)
      {
         printf("%-25s%-25g%-25g\n", "bx", bx_actual_min, bx_actual_max);
      }

      linear_minmax_search(
            by_arrayPtr,
            number_of_cells,
            &by_actual_min,
            &by_actual_max);
      if (verbose_flag)
      {
         printf("%-25s%-25g%-25g\n", "by", by_actual_min, by_actual_max);
      }

      linear_minmax_search(
            bz_arrayPtr,
            number_of_cells,
            &bz_actual_min,
            &bz_actual_max);
      if (verbose_flag)
      {
         printf("%-25s%-25g%-25g\n", "bz", bz_actual_min, bz_actual_max);
      }

      linear_minmax_search(
            b1x_arrayPtr,
            number_of_cells,
            &b1x_actual_min,
            &b1x_actual_max);
      if (verbose_flag)
      {
         printf("%-25s%-25g%-25g\n", "b1x", b1x_actual_min, b1x_actual_max);
      }

      linear_minmax_search(
            b1y_arrayPtr,
            number_of_cells,
            &b1y_actual_min,
            &b1y_actual_max);
      if (verbose_flag)
      {
         printf("%-25s%-25g%-25g\n", "b1y", b1y_actual_min, b1y_actual_max);
      }

      linear_minmax_search(
            b1z_arrayPtr,
            number_of_cells,
            &b1z_actual_min,
            &b1z_actual_max);
      if (verbose_flag)
      {
         printf("%-25s%-25g%-25g\n", "b1z", b1z_actual_min, b1z_actual_max);
      }

      linear_minmax_search(
            p_arrayPtr,
            number_of_cells,
            &p_actual_min,
            &p_actual_max);
      if (verbose_flag)
      {
         printf("%-25s%-25g%-25g\n", "p", p_actual_min, p_actual_max);
      }

      linear_minmax_search(
            e_arrayPtr,
            number_of_cells,
            &e_actual_min,
            &e_actual_max);
      if (verbose_flag)
      {
         printf("%-25s%-25g%-25g\n", "e", e_actual_min, e_actual_max);
      }

      linear_minmax_search(
            jx_arrayPtr,
            number_of_cells,
            &jx_actual_min,
            &jx_actual_max);
      if (verbose_flag)
      {
         printf("%-25s%-25g%-25g\n", "jx", jx_actual_min, jx_actual_max);
      }

      linear_minmax_search(
            jy_arrayPtr,
            number_of_cells,
            &jy_actual_min,
            &jy_actual_max);
      if (verbose_flag)
      {
         printf("%-25s%-25g%-25g\n", "jy", jy_actual_min, jy_actual_max);
      }

      linear_minmax_search(
            jz_arrayPtr,
            number_of_cells,
            &jz_actual_min,
            &jz_actual_max);
      if (verbose_flag)
      {
         printf("%-25s%-25g%-25g\n", "jz", jz_actual_min, jz_actual_max);
      }

   }

   /* return 1 if all reads were successfull. otherwise return 0 */

   return status_code;
}

int read_record_1(verbose_flag)
{

   int record_size, a, n=0;

   /*** get the number of bytes in record 1 ( these are 1 byte caracters that
    * specify the units for model variables)*/

   fread( &byte_value, four_bytes, one_element, input_filePtr);
   record_size = byte_value;

   if (ferror(input_filePtr) || feof(input_filePtr) )
   {
      printf("ERROR with fread must have returned non zero status_code for "
         "file stream\n");
      perror("input_filePtr");
      return 0;
   }

   if (verbose_flag)
   {
      printf("Record_1: found record_size: %d MAX: %d\n", record_size,
      MAX_FIRST_RECORD);
   }

   if (verbose_flag)
      printf("testing endianess of input file...\n");

   flip_endian = (record_size > 32000 );

   if (verbose_flag)
      printf("setting flip_endian flag = %d\n", flip_endian);

   if (flip_endian)
      endian_flip((char *)&record_size, four_bytes, one_element);

   if (verbose_flag)
   {
      printf("Record_1: found record_size: %d\n", record_size);
   }

   if (record_size > MAX_FIRST_RECORD)
   {
      return 0;
   }

   /* if first four bytes of file give number > MAX_FIRST_RECORD it's a bogus
    * input file */

   /** we know the record size, now allocate space for the char *units_string
    * variable **/

   unit_string=(char*)malloc( (record_size + 1 ) * sizeof(char));

   n = fread(unit_string, 1, record_size, input_filePtr);

   unit_string[n] = '\0';

   if ( DEBUG_FLAG)
      printf(
            "strlen( unit_string ) = %d\nn = %d\n",
            (int)strlen(unit_string),
            n);

   if (ferror(input_filePtr) || feof(input_filePtr) )
   {
      printf("ERROR with fread must have returned non zero status_code for file "
         "stream\n");
      perror("input_filePtr");
      return 0;
   }

   fread( &byte_value_2, four_bytes, one_element, input_filePtr);

   if (ferror(input_filePtr) || feof(input_filePtr) )
   {
      printf("ERROR with fread must have returned non zero status_code for file "
         "stream\n");
      perror("input_filePtr");
      return 0;
   }

   if (verbose_flag)
      printf("unit string = ---->%s<----\n", unit_string);

   if (byte_value == byte_value_2)
   {
      if (verbose_flag)
      {
         printf("READ Record 1 SUCCESSFUL\n");
      }
      return 1;
   }
   else
   {
      printf("ERROR reading input file\n");
      return 0;
   }

}

int read_record_2(verbose_flag)
{
   int record_size, number_elements;
   double elapsed_time_seconds_double, *tmp_double;

   /*** get the number of bytes in record 2 ( these are five 4 byte integers
    * that specify how current model was run)*/
   fread( &byte_value, four_bytes, one_element, input_filePtr);

   if (flip_endian)
      endian_flip((char*)&byte_value, four_bytes, one_element);

   record_size = byte_value;
   number_elements = record_size / four_bytes;
   is_double = (number_elements == 6);

   fread( &current_iteration_step, four_bytes, one_element, input_filePtr);

   if (flip_endian)
      endian_flip((char*)&current_iteration_step, four_bytes, one_element);

   if (is_double)
   {
      fread(
            &elapsed_time_seconds_double,
            eight_bytes,
            one_element,
            input_filePtr);
      if (flip_endian)
         endian_flip(
               (char *)&elapsed_time_seconds_double,
               eight_bytes,
               one_element);
      elapsed_time_seconds=elapsed_time_seconds_double;
   }
   else
   {
      fread( &elapsed_time_seconds, four_bytes, one_element, input_filePtr);
      if (flip_endian)
         endian_flip((char *)&elapsed_time_seconds, four_bytes, one_element);
   }

   fread( &number_dimensions, four_bytes, one_element, input_filePtr);

   if (flip_endian)
      endian_flip((char *)&number_dimensions, four_bytes, one_element);
   fread( &number_special_parameters, four_bytes, one_element, input_filePtr);
   if (flip_endian)
      endian_flip((char *)&number_special_parameters, four_bytes, one_element);

   fread( &number_plot_variables, four_bytes, one_element, input_filePtr);
   if (flip_endian)
      endian_flip((char *)&number_plot_variables, four_bytes, one_element);

   fread( &byte_value_2, four_bytes, one_element, input_filePtr);
   if (flip_endian)
      endian_flip((char *)&byte_value_2, four_bytes, one_element);

   if (byte_value == byte_value_2)
   {
      if (verbose_flag)
      {
         printf("Number of plot variables: %d \n", number_plot_variables);
         printf(
               "Number of special parameters: %d \n",
               number_special_parameters);
         printf("READ Record 2 SUCCESSFUL\n");
      }
      return 1;
   }
   else
   {
      printf("ERROR reading input file\n");
      return 0;
   }
}

int read_record_3(verbose_flag)
{

   int record_size, number_elements;

   /*** get the number of bytes in record 3 ( there are three 4byte integers
    * for x,y,&z dimension sizes) */

   fread( &byte_value, four_bytes, one_element, input_filePtr);
   if (flip_endian)
      endian_flip((char *)&byte_value, four_bytes, one_element);
   record_size = byte_value;
   number_elements = record_size / four_bytes;

   if (verbose_flag)
      printf("Read record_3: Flip_endian: %d Is double precision: %d Number "
         "elements: %d\n", flip_endian, is_double, number_elements);

   fread( &x_dimension_size, four_bytes, one_element, input_filePtr);
   if (flip_endian)
      endian_flip((char *)&x_dimension_size, four_bytes, one_element);
   fread( &y_dimension_size, four_bytes, one_element, input_filePtr);
   if (flip_endian)
      endian_flip((char *)&y_dimension_size, four_bytes, one_element);
   fread( &z_dimension_size, four_bytes, one_element, input_filePtr);
   if (flip_endian)
      endian_flip((char *)&z_dimension_size, four_bytes, one_element);
   if (verbose_flag)
      printf(
            "Read record_3: Dimension sizes: %d %d %d\n",
            x_dimension_size,
            y_dimension_size,
            z_dimension_size);

   fread( &byte_value_2, four_bytes, one_element, input_filePtr);

   if (flip_endian)
      endian_flip((char *)&byte_value_2, four_bytes, one_element);

   if (byte_value == byte_value_2)
   {
      if (verbose_flag)
      {
         printf("READ Record 3 SUCCESSFUL\n");
      }
      return 1;
   }
   else
   {
      printf("ERROR reading input file\n");
      return 0;
   }
}

int read_record_4(verbose_flag)
{

   int i, record_size, number_elements;
   double *dtmp;

   if ( DEBUG_FLAG)
      printf(
            "DEBUG\t from read_record_4 - Dimension sizes: %d %d %d\n",
            x_dimension_size,
            y_dimension_size,
            z_dimension_size);

   /*** get the number of bytes in record 4 ( there are ten 4byte integers for x,y,&z dimension sizes) */

   fread( &byte_value, four_bytes, one_element, input_filePtr);
   if (flip_endian)
      endian_flip((char *)&byte_value, four_bytes, one_element);
   record_size = byte_value;
   number_elements = record_size / ( (1+is_double)*four_bytes );

   if ( DEBUG_FLAG)
      printf(
            "DEBUG\t from read_record_4\trecord_size = %d\tnnumber_of_elements "
               "= %d\n",
            record_size,
            number_elements);

   if (number_elements != number_special_parameters)
   {
      fprintf(stderr,"Number of parameters read (%d) does not match record "
      "length (%d)!\n", number_elements,number_special_parameters);
      exit(EXIT_FAILURE);
   }

   if (verbose_flag)
      printf("Read_record_4: number_elements: %d\n", number_elements);

   if (number_elements != number_special_parameters)
   {
      exit(EXIT_FAILURE);
   }

   special_parameter_array=(float*)malloc(number_special_parameters
         *sizeof(float));

   /*if (is_double) <--DUPLICATE */

   if (is_double)
   {
      dtmp=(double*)malloc(number_special_parameters*sizeof(double));
      fread(dtmp, eight_bytes, number_special_parameters, input_filePtr);

      if (flip_endian)
         endian_flip((char *)dtmp, eight_bytes, number_special_parameters);

      for (i=0L; i<number_special_parameters; i++)
         special_parameter_array[i]=dtmp[i];
      free(dtmp);
   }
   else
   {
      fread(
            special_parameter_array,
            four_bytes,
            number_special_parameters,
            input_filePtr);
      if (flip_endian)
         endian_flip(
               (char *)special_parameter_array,
               four_bytes,
               number_special_parameters);
   }

   fread( &byte_value_2, four_bytes, one_element, input_filePtr);
   if (flip_endian)
      endian_flip((char *)&byte_value_2, four_bytes, one_element);

   if (byte_value == byte_value_2)
   {
      if (verbose_flag)
      {
         printf("READ Record 4 SUCCESSFUL\n");
      }
      return 1;
   }
   else
   {
      printf("ERROR reading input file\n");
      return 0;
   }
}

int read_record_5(verbose_flag)
{
   int i, i2, record_size, number_of_elements;

   /* flags to determine if special grid parameters exist in the .out
    * file. If not, code checks to see if the the read_database_info function
    * has already retireved the values ***/

   int NX_set = 0, NY_set = 0, NZ_set = 0;
   int P1_set = 0, P2_set = 0, P3_set = 0;

   strarr tmp_strarr;

   /* initialize - or we'll get a SEGMENTATION FAULT/BUS ERROR */
   tmp_strarr.len=0;
   tmp_strarr.sarr=NULL;

   /*** get the number of bytes in record 5 ( these are 1 byte caracters that
    * specify the models variable names)*/
   fread( &byte_value, four_bytes, one_element, input_filePtr);
   if (flip_endian)
      endian_flip((char *)&byte_value, four_bytes, one_element);
   record_size = byte_value;

   /** we know the record size, now allocate space for the char
    * *variable_name_string variable **/

   variable_name_string=(char*)malloc( (record_size + 1 ) * sizeof(char));

   number_of_elements =fread(
         variable_name_string,
         1,
         record_size,
         input_filePtr);

   if ( DEBUG_FLAG)
      printf(
            "DEBUG\tnumber_of_elements ( bytes read from last fread ) = %d\n",
            number_of_elements);

   variable_name_string[number_of_elements] = '\0';
   if (verbose_flag)
      printf(
            "Read_record_5: variable string length %d record size: %d\n",
            number_of_elements,
            record_size);

   if (verbose_flag)
      printf(
            "Read_record_5: Variables available: --->%s<---\n",
            variable_name_string);

   strsplit(variable_name_string, &tmp_strarr, " ");

   if (verbose_flag)
   {
      printf("Variable name string array length: %ld\n", tmp_strarr.len);
      printf(
            "Variable name string array elements: %s %s\n",
            tmp_strarr.sarr[0],
            tmp_strarr.sarr[1]);
   }

   if (tmp_strarr.len < (number_special_parameters))
      exit(EXIT_FAILURE);

   /* do this in case the special parameters do not always come in the same
    * order or not at all */

   for (i=0, i2=tmp_strarr.len-number_special_parameters; i
         <number_special_parameters; i++, i2++)
   {
      if (verbose_flag)
         printf(
               "parsing special parameters: %d variable: %s\n",
               i,
               tmp_strarr.sarr[i2]);

      if (strcmp(tmp_strarr.sarr[i2], "g") == 0)
         special_parameter_g =special_parameter_array[i];
      if (strcmp(tmp_strarr.sarr[i2], "c") == 0)
         special_parameter_c =special_parameter_array[i];
      if (strcmp(tmp_strarr.sarr[i2], "th") == 0)
         special_parameter_th=special_parameter_array[i];
      if (strcmp(tmp_strarr.sarr[i2], "p1") == 0)
      {
         special_parameter_P1=special_parameter_array[i];
         P1_set = 1;
      }
      if (strcmp(tmp_strarr.sarr[i2], "p2") == 0)
      {
         special_parameter_P2=special_parameter_array[i];
         P2_set = 1;
      }
      if (strcmp(tmp_strarr.sarr[i2], "p3") == 0)
      {
         special_parameter_P3=special_parameter_array[i];
         P3_set = 1;
      }
      if (strcmp(tmp_strarr.sarr[i2], "NX") == 0)
      {
         special_parameter_NX=special_parameter_array[i];
         NX_set = 1;
      }
      if (strcmp(tmp_strarr.sarr[i2], "NY") == 0)
      {
         special_parameter_NY=special_parameter_array[i];
         NY_set = 1;
      }
      if (strcmp(tmp_strarr.sarr[i2], "NZ") == 0)
      {
         special_parameter_NZ=special_parameter_array[i];
         NZ_set = 1;
      }
      if (strcmp(tmp_strarr.sarr[i2], "R") == 0)
         special_parameter_R =special_parameter_array[i];
   }

   /* inspect for sanity */

   /****** NX, NY, & NZ is crucial for block count calculations, if not set see
    * if database info has them ********/

   /*if .out doesn't contain NX, check see if DatabaseInfo file set the value */

   if ( !NX_set && batsrus_special_parameter_NX_exists_in_databaseinfo)
   {
      special_parameter_NX = atof(batsrus_special_parameter_NX);
      if (verbose_flag)
      {
         printf(
               "setting special parameter NX from DatabaseInfo file:\t%f\n",
               special_parameter_NX);
      }
   }
   else if ( !NX_set && !batsrus_special_parameter_NX_exists_in_databaseinfo)
   {
      fprintf( stderr, "!!!! %s\tERROR: special parameter NX could not be "
      "located.  Set key value in DatabaseInfo file to proceed...\nie. 6 "
      "# batsrus_special_parameter_NX\n", __FILE__ );
      return 0;
   }
   if( ! NY_set && batsrus_special_parameter_NY_exists_in_databaseinfo )
   {special_parameter_NY  = atof( batsrus_special_parameter_NY );

      if( verbose_flag )
      {
         printf("setting special parameter NY from DatabaseInfo file:\t%f\n",
               special_parameter_NY );
      }
   }
   else if( ! NY_set && ! batsrus_special_parameter_NY_exists_in_databaseinfo )
   {
      fprintf( stderr, "!!!! %s\tERROR: special parameter NY could not be "
            "located.  Set key value in DatabaseInfo file to proceed...\nie. "
            "6 # batsrus_special_parameter_NY\n", __FILE__ );
      return 0;
   }
   if( ! NZ_set && batsrus_special_parameter_NZ_exists_in_databaseinfo )
   {
      special_parameter_NZ = atof( batsrus_special_parameter_NZ );
      if( verbose_flag )
      {
         printf("setting special parameter NZ from DatabaseInfo file:\t%f\n",
               special_parameter_NZ );
      }
   }
   else if( ! NZ_set && ! batsrus_special_parameter_NZ_exists_in_databaseinfo )
   {
      fprintf( stderr, "!!!! %s\tERROR: special parameter NZ could not be "
            "located.  Set key value in DatabaseInfo file to proceed...\nie. 6 "
            "# batsrus_special_parameter_NZ\n", __FILE__ );
      return 0;
   }

   /****** P1, P2, & P3 is crucial for block count calculations, if not set see
    * if database info has them ********/

   /* if .out doesn't contain P1, check see if DatabaseInfo file set the value */

   if( ! P1_set && batsrus_special_parameter_P1_exists_in_databaseinfo )
   {
      special_parameter_P1 = atof( batsrus_special_parameter_P1 );
      if( verbose_flag )
      {
         printf("setting special parameter P1 from DatabaseInfo file:\t%f\n",
               special_parameter_P1 );}
   }
   else if( ! P1_set && ! batsrus_special_parameter_P1_exists_in_databaseinfo )
   {
      fprintf( stderr, "!!!! %s\tERROR: special parameter P1 could not be l"
            "ocated.  Set key value in DatabaseInfo file to proceed...\nie. 6 "
            "# batsrus_special_parameter_P1\n", __FILE__ );
      return 0;
   }
   if( ! P2_set && batsrus_special_parameter_P2_exists_in_databaseinfo )
   {
      special_parameter_P2 = atof( batsrus_special_parameter_P2 );
      if( verbose_flag )
      {  printf("setting special parameter P2 from DatabaseInfo file:\t%f\n",
            special_parameter_P2 );}
   }
   else if( ! P2_set && ! batsrus_special_parameter_P2_exists_in_databaseinfo )
   {
      fprintf( stderr, "!!!! %s\tERROR: special parameter P2 could not be "
            "located.  Set key value in DatabaseInfo file to proceed...\nie. 6 "
            "# batsrus_special_parameter_P2\n", __FILE__ );
      return 0;
   }
   if( ! P3_set && batsrus_special_parameter_P3_exists_in_databaseinfo )
   {
      special_parameter_P3 = atof( batsrus_special_parameter_P3 );
      if( verbose_flag )
      {  printf("setting special parameter P3 from DatabaseInfo file:\t%f\n",
            special_parameter_P3 );}
   }
   else if( ! P3_set && ! batsrus_special_parameter_P3_exists_in_databaseinfo )
   {
      fprintf( stderr, "!!!! %s\tERROR: special parameter P3 could not be "
            "located.  Set key value in DatabaseInfo file to proceed...\nie. 6 "
            "# batsrus_special_parameter_P3\n", __FILE__ );
      return 0;
   }

   if (verbose_flag)
   {  printf("Special parameters: NX: %f R: %f P3: %f\n", special_parameter_NX,
         special_parameter_R,special_parameter_P3);}
   free_strarr(&tmp_strarr);

   fread( &byte_value_2, four_bytes, one_element , input_filePtr);
   if (flip_endian) endian_flip((char *)&byte_value_2,four_bytes,one_element);

   if( byte_value == byte_value_2)
   {
      if( verbose_flag )
      {  printf("READ Record 5 SUCCESSFUL\n");}
      return 1;
   }
   else
   {
      printf("ERROR reading input file\n");
      exit(EXIT_FAILURE);
   }
}

int read_record_6(verbose_flag)
{
   long int i;
   long int record_size, number_elements;

   if (is_double)
   {
      if ( (tmp_double=calloc( 3*number_of_cells, sizeof(double)) ) == NULL)
      {
         printf("MALLOC for x_position array FAILED\n");
         exit(EXIT_FAILURE);
      }
   }

   if ( (x_position_arrayPtr = calloc(number_of_cells, sizeof(float)) ) == NULL)
   {
      printf("MALLOC for x_position array FAILED\n");
      exit(EXIT_FAILURE);
   }
   if ( (y_position_arrayPtr = calloc(number_of_cells, sizeof(float)) ) == NULL)
   {
      printf("MALLOC for y_position array FAILED\n");
      exit(EXIT_FAILURE);
   }
   if ( (z_position_arrayPtr = calloc(number_of_cells, sizeof(float)) ) == NULL)
   {
      printf("MALLOC for z_position array FAILED\n");
      exit(EXIT_FAILURE);
   }

   /*** get the number of bytes in record 6 ( the number of bytes in record
    * 6 = number_of_cells * 3dimensions * 4bytes ) */

   fread( &byte_value, four_bytes, one_element, input_filePtr);
   if (flip_endian)
      endian_flip((char *)&byte_value, four_bytes, one_element);
   record_size = byte_value;

   if (is_double)
   {
      number_elements = record_size /eight_bytes;
   }
   else
   {
      number_elements = record_size /four_bytes;
   }

   if ( DEBUG_FLAG)
      printf("DEBUG\t 3 * number_of_cells = 3 * %d = %d\n", number_of_cells, 3
            * number_of_cells);

   if (number_elements != (3*number_of_cells))
   {
      printf("Record 6 inconsistent with expectation\n");
      printf(
            "Record_size: %ld Number_elements: %ld Cells %d\n",
            record_size,
            number_elements,
            3*number_of_cells);
      exit(EXIT_FAILURE);
   }

   if (is_double)
   {
      fread(tmp_double, eight_bytes, 3*number_of_cells, input_filePtr);
      if (flip_endian)
         endian_flip((char *)tmp_double, eight_bytes, 3*number_of_cells);

      for (i=0L; i<number_of_cells; i++)
      {
         x_position_arrayPtr[i]=tmp_double[i];
         y_position_arrayPtr[i]=tmp_double[i+number_of_cells];
         z_position_arrayPtr[i]=tmp_double[i+2*number_of_cells];
      }

   }
   else
   {
      fread(x_position_arrayPtr, four_bytes, number_of_cells, input_filePtr);
      if (flip_endian)
         endian_flip((char*)x_position_arrayPtr, four_bytes, number_of_cells);
      fread(y_position_arrayPtr, four_bytes, number_of_cells, input_filePtr);
      if (flip_endian)
         endian_flip((char*)y_position_arrayPtr, four_bytes, number_of_cells);
      fread(z_position_arrayPtr, four_bytes, number_of_cells, input_filePtr);
      if (flip_endian)
         endian_flip((char*)z_position_arrayPtr, four_bytes, number_of_cells);
   }

   fread( &byte_value_2, four_bytes, one_element, input_filePtr);
   if (flip_endian)
      endian_flip((char *)&byte_value_2, four_bytes, one_element);

   if (byte_value == byte_value_2)
   {
      if (verbose_flag)
      {
         printf("READ Record 6 SUCCESSFUL\n");
      }
      return 1;
   }
   else
   {
      printf("ERROR reading input file\n");
      return 0;
   }
}

int read_record_7(verbose_flag)
{

   int read_batsrus_variable(FILE *, float *, int, int, double *);

   int i, read_failed_flag=0;
   int record_size, number_elements;
   strarr tmp_strarr;

   /* initialize - or we'll get a SEGMENTATION FAULT/BUS ERROR */
   tmp_strarr.len=0;
   tmp_strarr.sarr=NULL;

   /* get array of variable names to correctly distribute data */
   strsplit(variable_name_string, &tmp_strarr, " ");

   if (verbose_flag)
   {
      printf("Variable name string array length: %ld\n", tmp_strarr.len);
      printf(
            "Variable name string array elements 0 & 1: %s %s\n",
            tmp_strarr.sarr[0],
            tmp_strarr.sarr[1]);
   }

   if (tmp_strarr.len < (number_special_parameters))
      exit(EXIT_FAILURE);

   if ( (rho_arrayPtr = (float *) calloc(number_of_cells, sizeof(float)) )
         == NULL)
   {
      printf("MALLOC for rho array FAILED\n");
      exit(EXIT_FAILURE);
   }
   if ( (ux_arrayPtr = (float *) calloc(number_of_cells, sizeof(float)) )
         == NULL)
   {
      printf("MALLOC for ux array FAILED\n");
      exit(EXIT_FAILURE);
   }
   if ( (uy_arrayPtr = (float *) calloc(number_of_cells, sizeof(float)) )
         == NULL)
   {
      printf("MALLOC for uy array FAILED\n");
      exit(EXIT_FAILURE);
   }
   if ( (uz_arrayPtr = (float *) calloc(number_of_cells, sizeof(float)) )
         == NULL)
   {
      printf("MALLOC for uz array FAILED\n");
      exit(EXIT_FAILURE);
   }
   if ( (bx_arrayPtr = (float *) calloc(number_of_cells, sizeof(float)) )
         == NULL)
   {
      printf("MALLOC for bx array FAILED\n");
      exit(EXIT_FAILURE);
   }
   if ( (by_arrayPtr = (float *) calloc(number_of_cells, sizeof(float)) )
         == NULL)
   {
      printf("MALLOC for by array FAILED\n");
      exit(EXIT_FAILURE);
   }
   if ( (bz_arrayPtr = (float *) calloc(number_of_cells, sizeof(float)) )
         == NULL)
   {
      printf("MALLOC for bz array FAILED\n");
      exit(EXIT_FAILURE);
   }
   if ( (b1x_arrayPtr = (float *) calloc(number_of_cells, sizeof(float)) )
         == NULL)
   {
      printf("MALLOC for blx array FAILED\n");
      exit(EXIT_FAILURE);
   }
   if ( (b1y_arrayPtr = (float *) calloc(number_of_cells, sizeof(float)) )
         == NULL)
   {
      printf("MALLOC for bly array FAILED\n");
      exit(EXIT_FAILURE);
   }
   if ( (b1z_arrayPtr = (float *) calloc(number_of_cells, sizeof(float)) )
         == NULL)
   {
      printf("MALLOC for blz array FAILED\n");
      exit(EXIT_FAILURE);
   }
   if ( (p_arrayPtr = (float *) calloc(number_of_cells, sizeof(float)) )
         == NULL)
   {
      printf("MALLOC for p array FAILED\n");
      exit(EXIT_FAILURE);
   }
   if ( (e_arrayPtr = (float *) calloc(number_of_cells, sizeof(float)) )
         == NULL)
   {
      printf("MALLOC for e array FAILED\n");
      exit(EXIT_FAILURE);
   }
   if ( (jx_arrayPtr = (float *) calloc(number_of_cells, sizeof(float)) )
         == NULL)
   {
      printf("MALLOC for jx array FAILED\n");
      exit(EXIT_FAILURE);
   }
   if ( (jy_arrayPtr = (float *) calloc(number_of_cells, sizeof(float)) )
         == NULL)
   {
      printf("MALLOC for jy array FAILED\n");
      exit(EXIT_FAILURE);
   }
   if ( (jz_arrayPtr = (float *) calloc(number_of_cells, sizeof(float)) )
         == NULL)
   {
      printf("MALLOC for jz array FAILED\n");
      exit(EXIT_FAILURE);
   }

   /* read data: start at variable name 3 (0: 'X', 1: 'Y', 2: 'Z' have been
    * read before) */

   for (i=3L; i<tmp_strarr.len-number_special_parameters; i++)
   {
      if (verbose_flag)
      {
         printf("reading variable %d of %ld: %s\n", i+1, tmp_strarr.len
               -number_special_parameters, tmp_strarr.sarr[i]);
      }

      /******************** get all rho variables ****************************/

      if (strcmp(tmp_strarr.sarr[i], "rho") == 0)
      {
         read_failed_flag+=read_batsrus_variable(
               input_filePtr,
               rho_arrayPtr,
               flip_endian,
               is_double,
               tmp_double);
      }

      /******************** get all ux variables ****************************/

      if (strcmp(tmp_strarr.sarr[i], "ux") == 0)
      {
         read_failed_flag+=read_batsrus_variable(
               input_filePtr,
               ux_arrayPtr,
               flip_endian,
               is_double,
               tmp_double);
      }

      /******************** get all uy variables ****************************/

      if (strcmp(tmp_strarr.sarr[i], "uy") == 0)
      {
         read_failed_flag+=read_batsrus_variable(
               input_filePtr,
               uy_arrayPtr,
               flip_endian,
               is_double,
               tmp_double);
      }

      /******************** get all uz variables ****************************/

      if (strcmp(tmp_strarr.sarr[i], "uz") == 0)
      {
         read_failed_flag+=read_batsrus_variable(
               input_filePtr,
               uz_arrayPtr,
               flip_endian,
               is_double,
               tmp_double);
      }

      /******************** get all bx variables ****************************/

      if (strcmp(tmp_strarr.sarr[i], "bx") == 0)
      {
         read_failed_flag+=read_batsrus_variable(
               input_filePtr,
               bx_arrayPtr,
               flip_endian,
               is_double,
               tmp_double);
      }

      /******************** get all by variables ****************************/

      if (strcmp(tmp_strarr.sarr[i], "by") == 0)
      {
         read_failed_flag+=read_batsrus_variable(
               input_filePtr,
               by_arrayPtr,
               flip_endian,
               is_double,
               tmp_double);
      }

      /******************** get all bz variables ****************************/

      if (strcmp(tmp_strarr.sarr[i], "bz") == 0)
      {
         read_failed_flag+=read_batsrus_variable(
               input_filePtr,
               bz_arrayPtr,
               flip_endian,
               is_double,
               tmp_double);
      }

      /******************** get all b1x variables ****************************/

      if (strcmp(tmp_strarr.sarr[i], "b1x") == 0)
      {
         read_failed_flag+=read_batsrus_variable(
               input_filePtr,
               b1x_arrayPtr,
               flip_endian,
               is_double,
               tmp_double);
      }

      /******************** get all b1y variables ****************************/

      if (strcmp(tmp_strarr.sarr[i], "b1y") == 0)
      {
         read_failed_flag+=read_batsrus_variable(
               input_filePtr,
               b1y_arrayPtr,
               flip_endian,
               is_double,
               tmp_double);
      }

      /******************** get all b1z variables ****************************/

      if (strcmp(tmp_strarr.sarr[i], "b1z") == 0)
      {
         read_failed_flag+=read_batsrus_variable(
               input_filePtr,
               b1z_arrayPtr,
               flip_endian,
               is_double,
               tmp_double);
      }

      /******************** get all jx variables ****************************/

      if (strcmp(tmp_strarr.sarr[i], "jx") == 0)
      {
         read_failed_flag+=read_batsrus_variable(
               input_filePtr,
               jx_arrayPtr,
               flip_endian,
               is_double,
               tmp_double);
      }

      /******************** get all jy variables ****************************/

      if (strcmp(tmp_strarr.sarr[i], "jy") == 0)
      {
         read_failed_flag+=read_batsrus_variable(
               input_filePtr,
               jy_arrayPtr,
               flip_endian,
               is_double,
               tmp_double);
      }

      /******************** get all jz variables ****************************/

      if (strcmp(tmp_strarr.sarr[i], "jz") == 0)
      {
         read_failed_flag+=read_batsrus_variable(
               input_filePtr,
               jz_arrayPtr,
               flip_endian,
               is_double,
               tmp_double);
      }

      /******************** get all e variables ****************************/

      if (strcmp(tmp_strarr.sarr[i], "e") == 0)
      {
         read_failed_flag+=read_batsrus_variable(
               input_filePtr,
               e_arrayPtr,
               flip_endian,
               is_double,
               tmp_double);
      }

      /******************** get all p variables ****************************/

      if (strcmp(tmp_strarr.sarr[i], "p") == 0)
      {
         read_failed_flag+=read_batsrus_variable(
               input_filePtr,
               p_arrayPtr,
               flip_endian,
               is_double,
               tmp_double);
      }
   }

   if (is_double)
   {
      free(tmp_double);
   }

   free_strarr(&tmp_strarr);

   if (read_failed_flag == 0)
   {
      if (verbose_flag)
      {
         printf("READ Record 7 SUCCESSFUL\n");
      }
      return 1;
   }
   else
   {
      printf("ERROR reading input file\n");
      return 0;
   }

}

int build_grid_description(verbose_flag)
{

   int debug_flag = 0;

   int i, j, element_count, block_count, current_amr_level, parent_level;

   int block_index_1, block_index_2;

   int parent_index, test_parent;

   int ix, iy, iz;

   int child_id_key, number_of_parents = 0;

   float eps = 1.e-5;

   float level_factor;

   float cell_delta_x[number_of_blocks];
   float cell_delta_y[number_of_blocks];
   float cell_delta_z[number_of_blocks];

   int NX = special_parameter_NX;
   int NY = special_parameter_NY;
   int NZ = special_parameter_NZ;

   int elements_per_block = special_parameter_NX * special_parameter_NY
         * special_parameter_NZ;

   int block_space;

   block_space = ( 8 * number_of_blocks ) / 7; /* extra space needed to store
   virtual/parent blocks that are generated in program */

   if (verbose_flag)
   {
      printf("\nbuilding grid description..........\n");
      printf("number of original blocks =\t%d\n", number_of_blocks);
      printf("original blocks plus virtual =\t%d\n", block_space);
   }
   /* dynamic memory allocation for block_*_min/max, block_*_center, and b
    * lock_amr_levels arrays */

   /* create extra block space for virtual parent blocks
    * ( 8 * number_of_blocks ) / 7  */

   if ( (block_x_min = (float *) calloc(block_space, sizeof(float)) ) == NULL)
   {
      printf("MALLOC for block_x_min array FAILED\n");
      exit(EXIT_FAILURE);
   }
   if ( (block_x_max = (float *) calloc(block_space, sizeof(float)) ) == NULL)
   {
      printf("MALLOC for block_x_max array FAILED\n");
      exit(EXIT_FAILURE);
   }
   if ( (block_y_min = (float *) calloc(block_space, sizeof(float)) ) == NULL)
   {
      printf("MALLOC for block_y_min array FAILED\n");
      exit(EXIT_FAILURE);
   }
   if ( (block_y_max = (float *) calloc(block_space, sizeof(float)) ) == NULL)
   {
      printf("MALLOC for block_x_max array FAILED\n");
      exit(EXIT_FAILURE);
   }
   if ( (block_z_min = (float *) calloc(block_space, sizeof(float)) ) == NULL)
   {
      printf("MALLOC for block_z_min array FAILED\n");
      exit(EXIT_FAILURE);
   }
   if ( (block_z_max = (float *) calloc(block_space, sizeof(float)) ) == NULL)
   {
      printf("MALLOC for block_z_max array FAILED\n");
      exit(EXIT_FAILURE);
   }

   if ( (block_x_center = (float *) calloc(block_space, sizeof(float)) )
         == NULL)
   {
      printf("MALLOC for block_x_center array FAILED\n");
      exit(EXIT_FAILURE);
   }
   if ( (block_y_center = (float *) calloc(block_space, sizeof(float)) )
         == NULL)
   {
      printf("MALLOC for block_y_center array FAILED\n");
      exit(EXIT_FAILURE);
   }
   if ( (block_z_center = (float *) calloc(block_space, sizeof(float)) )
         == NULL)
   {
      printf("MALLOC for block_z_center array FAILED\n");
      exit(EXIT_FAILURE);
   }

   if ( (block_amr_levels = (float *) calloc(block_space, sizeof(float)) )
         == NULL)
   {
      printf("MALLOC for block_amr_levels array FAILED\n");
      exit(EXIT_FAILURE);
   }

   /* allocate memory for parent_id & child_count, child_id_1 ... child_id_8 */

   if ( (parent_id = (int *) calloc(block_space, sizeof(int)) ) == NULL)
   {
      printf("MALLOC for  parent_id array FAILED\n");
      exit(EXIT_FAILURE);
   }
   if ( (child_count = (int *) calloc(block_space, sizeof(int)) ) == NULL)
   {
      printf("MALLOC for  child_count array FAILED\n");
      exit(EXIT_FAILURE);
   }
   if ( (child_id_1 = (int *) calloc(block_space, sizeof(int)) ) == NULL)
   {
      printf("MALLOC for  child_id_1 array FAILED\n");
      exit(EXIT_FAILURE);
   }
   if ( (child_id_2 = (int *) calloc(block_space, sizeof(int)) ) == NULL)
   {
      printf("MALLOC for  child_id_2 array FAILED\n");
      exit(EXIT_FAILURE);
   }
   if ( (child_id_3 = (int *) calloc(block_space, sizeof(int)) ) == NULL)
   {
      printf("MALLOC for  child_id_3 array FAILED\n");
      exit(EXIT_FAILURE);
   }
   if ( (child_id_4 = (int *) calloc(block_space, sizeof(int)) ) == NULL)
   {
      printf("MALLOC for  child_id_4 array FAILED\n");
      exit(EXIT_FAILURE);
   }
   if ( (child_id_5 = (int *) calloc(block_space, sizeof(int)) ) == NULL)
   {
      printf("MALLOC for  child_id_5 array FAILED\n");
      exit(EXIT_FAILURE);
   }
   if ( (child_id_6 = (int *) calloc(block_space, sizeof(int)) ) == NULL)
   {
      printf("MALLOC for  child_id_6 array FAILED\n");
      exit(EXIT_FAILURE);
   }
   if ( (child_id_7 = (int *) calloc(block_space, sizeof(int)) ) == NULL)
   {
      printf("MALLOC for  child_id_7 array FAILED\n");
      exit(EXIT_FAILURE);
   }
   if ( (child_id_8 = (int *) calloc(block_space, sizeof(int)) ) == NULL)
   {
      printf("MALLOC for  child_id_8 array FAILED\n");
      exit(EXIT_FAILURE);
   }

   /* calculate the smallest cell size for every block...index of array
    * coresponds to block id number */

   for (block_count = 0; block_count < number_of_blocks; block_count++)
   {
      cell_delta_x[block_count] =
         x_position_arrayPtr[ ( block_count * NX * NY * NZ ) + 1 ] -
         x_position_arrayPtr[ ( block_count * NX * NY * NZ ) ];
      cell_delta_y[block_count] =
         y_position_arrayPtr[ ( block_count * NX * NY * NZ ) + NX ] -
         y_position_arrayPtr[ ( block_count * NX * NY * NZ ) ];
      cell_delta_z[block_count] =
         z_position_arrayPtr[ ( block_count * NX * NY * NZ ) + ( NX * NY ) ] -
         z_position_arrayPtr[ ( block_count * NX * NY * NZ ) ];
   }

   /* initially set smallest_cell_size in x to an actual value */

   smallest_cell_size = cell_delta_x[0];

   /* find the smallest cell size */
   for (block_count = 0; block_count < number_of_blocks; block_count++)
   {
      if (cell_delta_x[block_count] < smallest_cell_size)
      {
         smallest_cell_size = cell_delta_x[block_count];
      }
   }

   if (debug_flag)
   {
      printf(
            "DEBUG\tsmallest cell size = %f\nnumber of cells = %d\nnumber of "
            "blocks = %d\n",
            smallest_cell_size,
            number_of_cells,
            number_of_blocks);
   }

   element_count = 0; /* set element count to zero but do not reset inside block
                         traversal loop */

   /* find and populate min and max values for each block in x,y,z */

   /* for every block */

   for (block_count = 0; block_count < number_of_blocks; block_count++)
   {

      /*initialize each block_* array element to first element for cooresponding
       * block in *_position_array */

      block_x_min[ block_count ] = x_position_arrayPtr[ element_count ];
      block_x_max[ block_count ] = x_position_arrayPtr[ element_count ];
      block_y_min[ block_count ] = y_position_arrayPtr[ element_count ];
      block_y_max[ block_count ] = y_position_arrayPtr[ element_count ];
      block_z_min[ block_count ] = z_position_arrayPtr[ element_count ];
      block_z_max[ block_count ] = z_position_arrayPtr[ element_count ];

      /* for every element in current block */

      for (j = 0; j < elements_per_block; j++)
      {
         if (block_x_min[ block_count ] > x_position_arrayPtr[ element_count ])
         {
            block_x_min[ block_count ] = x_position_arrayPtr[ element_count ];
         }
         if (block_x_max[ block_count ] < x_position_arrayPtr[ element_count ])
         {
            block_x_max[ block_count ] = x_position_arrayPtr[ element_count ];
         }
         if (block_y_min[ block_count ] > y_position_arrayPtr[ element_count ])
         {
            block_y_min[ block_count ] = y_position_arrayPtr[ element_count ];
         }
         if (block_y_max[ block_count ] < y_position_arrayPtr[ element_count ])
         {
            block_y_max[ block_count ] = y_position_arrayPtr[ element_count ];
         }
         if (block_z_min[ block_count ] > z_position_arrayPtr[ element_count ])
         {
            block_z_min[ block_count ] = z_position_arrayPtr[ element_count ];
         }
         if (block_z_max[ block_count ] < z_position_arrayPtr[ element_count ])
         {
            block_z_max[ block_count ] = z_position_arrayPtr[ element_count ];
         }

         element_count++;
      }

   }

   /* massage all blocks x,y,z min and max arrays to use the edge of the cells
    * instead of the middle of the cells */
   for (block_count = 0; block_count < number_of_blocks; block_count++)
   {
      block_x_min[ block_count ] = block_x_min[ block_count ]
            - (cell_delta_x[block_count]/2 );
      block_x_max[ block_count ] = block_x_max[ block_count ]
            + (cell_delta_x[block_count]/2 );
      block_y_min[ block_count ] = block_y_min[ block_count ]
            - (cell_delta_y[block_count]/2 );
      block_y_max[ block_count ] = block_y_max[ block_count ]
            + (cell_delta_y[block_count]/2 );
      block_z_min[ block_count ] = block_z_min[ block_count ]
            - (cell_delta_z[block_count]/2 );
      block_z_max[ block_count ] = block_z_max[ block_count ]
            + (cell_delta_z[block_count]/2 );
   }

   /* find all global min and max */

   /* initialize all global* variables to first corresponding element in positon
    *  array */

   global_x_min = block_x_min[ 0 ];
   global_x_max = block_x_min[ 0 ];
   global_y_min = block_y_min[ 0 ];
   global_y_max = block_y_min[ 0 ];
   global_z_min = block_z_min[ 0 ];
   global_z_max = block_z_min[ 0 ];

   for (i = 0; i < number_of_blocks; i++)
   {

      if (global_x_min > block_x_min[ i ])
      {
         global_x_min = block_x_min[ i ];
      }
      if (global_x_max < block_x_max[ i ])
      {
         global_x_max = block_x_max[ i ];
      }
      if (global_y_min > block_y_min[ i ])
      {
         global_y_min = block_y_min[ i ];
      }
      if (global_y_max < block_y_max[ i ])
      {
         global_y_max = block_y_max[ i ];
      }
      if (global_z_min > block_z_min[ i ])
      {
         global_z_min = block_z_min[ i ];
      }
      if (global_z_max < block_z_max[ i ])
      {
         global_z_max = block_z_max[ i ];
      }

   }

   if (verbose_flag)
   {
      printf(
            "\nXMIN =\t%f\nXMAX =\t%f\nYMIN =\t%f\nYMAX =\t%f\nZMIN =\t%f\nZMAX "
            "=\t%f\n",
            global_x_min,
            global_x_max,
            global_y_min,
            global_x_max,
            global_z_min,
            global_z_max);
   }

   /* calculate the maximum AMR level */

   max_amr_level = log( (global_x_max - global_x_min )/ (special_parameter_P1
         * NX * smallest_cell_size ));
   max_amr_level = max_amr_level / (log(2.0) );
   max_amr_level = floor(max_amr_level + 1.5) ; /* add 1.5 to the real amr
   level, see Lutz ... subtract 1 before returning program control to main*/

   if (verbose_flag)
   {
      printf("Calculated max_amr_level = %f\n", max_amr_level);
   }

   if (debug_flag)
   {
      printf(
            "size of block_at_amr_level = number_of_blocks %d * max_amr_level "
            "%f = %f\n",
            number_of_blocks,
            max_amr_level,
            number_of_blocks * max_amr_level);
   }

   /* allocate space for arrays block_at_amr_level number_of_blocks_at_amr_level
    * and number_parents_at_amr_level */
   if ( (block_at_amr_level = (int *) calloc(
         number_of_blocks * max_amr_level,
         sizeof(int)) ) == NULL)
   {
      printf("MALLOC for  block_at_amr_level array FAILED\n");
      exit(EXIT_FAILURE);
   }
   if ( (number_of_blocks_at_amr_level = (int *) calloc(
         max_amr_level,
         sizeof(int)) ) == NULL)
   {
      printf("MALLOC for  number_of_blocks_at_amr_level array FAILED\n");
      exit(EXIT_FAILURE);
   }
   if ( (number_of_parent_blocks_at_amr_level = (int *) calloc(
         max_amr_level,
         sizeof(int)) ) == NULL)
   {
      printf("MALLOC for  number_of_parent_blocks_at_amr_level array FAILED\n");
      exit(EXIT_FAILURE);
   }

   /* calculate and populate amr level for every block */

   /* for every block */

   for (block_count = 0; block_count < number_of_blocks; block_count++)
   {

      block_amr_levels[ block_count ] = ceil( (log( (global_x_max
            - global_x_min )/ (special_parameter_P1 * NX
            * cell_delta_x[block_count] )) ) / log( 2.0) );

   }

   /* calculate and populate value for block_at_amr_level,
    * number_of_blocks_at_amr_level, and parent_id arrays for every AMR level
    * in block_amr_levels array */

   /* for every block */

   for (block_count = 0; block_count < number_of_blocks; block_count++)
   {
      current_amr_level = block_amr_levels[ block_count ];

      block_at_amr_level[ current_amr_level * number_of_blocks +
      number_of_blocks_at_amr_level[ current_amr_level ] ] = block_count;

      parent_id[ block_count ] = -1;

      if (debug_flag)
      {
         printf(
               "\nDEBUG\tblock_at_amr_level[ %d ] = %d\n",
               current_amr_level * number_of_blocks
                     + number_of_blocks_at_amr_level[ current_amr_level ],
               block_count);
         printf(
               "DEBUG\tblock_at_amr_level[ %d ] = %d\n",
               current_amr_level * number_of_blocks
                     + number_of_blocks_at_amr_level[ current_amr_level ],
               block_at_amr_level[ current_amr_level * number_of_blocks +
               number_of_blocks_at_amr_level[ current_amr_level ] ]);
      }

      number_of_blocks_at_amr_level[ current_amr_level ]++;

   }

   /* calculate and populate values for center of every block */

   /* for every block */

   for (block_count =0; block_count < number_of_blocks; block_count++)
   {
      block_x_center[ block_count ] = ( 0.5 * (block_x_max[ block_count ]
            - block_x_min[ block_count ] ) ) + block_x_min[ block_count ];
      block_y_center[ block_count ] = ( 0.5 * (block_y_max[ block_count ]
            - block_y_min[ block_count ] ) ) + block_y_min[ block_count ];
      block_z_center[ block_count ] = ( 0.5 * (block_z_max[ block_count ]
            - block_z_min[ block_count ] ) ) + block_z_min[ block_count ];
   }

   /* initialize values of child_ids, parent_id, and child count arrays */

   for (block_count = 0; block_count < number_of_blocks; block_count++)
   {
      child_id_1[ block_count ] = -1;
      child_id_2[ block_count ] = -1;
      child_id_3[ block_count ] = -1;
      child_id_4[ block_count ] = -1;
      child_id_5[ block_count ] = -1;
      child_id_6[ block_count ] = -1;
      child_id_7[ block_count ] = -1;
      child_id_8[ block_count ] = -1;
      parent_id[block_count] = -1;
      child_count[ block_count ] = 0;
   }

   if (verbose_flag)
   {
      printf("Number of blocks:\t%d\n", number_of_blocks);

      for (i = 0; i < max_amr_level; i++)
      {
         printf(
               "\nnumber_of_blocks_at_amr_level[%d] = \t%d\n",
               i,
               number_of_blocks_at_amr_level[i]);
      }
   }

   /* begin calculating virtual parent blocks starting from blocks with the
    * finest resolution down to the root blocks */

   if (verbose_flag)
   {
      printf("\n**\tBEGINING CALCULATIONS FOR VIRTUAL PARENT BLOCKS\t**\n");
   }

   for (current_amr_level = max_amr_level - 1, level_factor = pow(
         2.0,
         current_amr_level); current_amr_level > 0;
         current_amr_level--,
         level_factor/=2)
   { /* [1] begin parent block calculation for loop */

      parent_level = current_amr_level - 1;
      number_of_blocks_at_amr_level[ current_amr_level ]
            += number_of_parent_blocks_at_amr_level[ current_amr_level ];

      if (debug_flag)
      {
         printf(
               "\n\n\nDEBUG\tcurrent_amr_level = %d\tlevel_factor = "
                  "%f\tparent_level = %d\tnumber_of_blocks_at_amr_level[%d] = "
                  "%d\n\n\n",
               current_amr_level,
               level_factor,
               parent_level,
               current_amr_level,
               number_of_blocks_at_amr_level[ current_amr_level ]);
      }

      if (number_of_blocks_at_amr_level[ current_amr_level ] > 0)
      { /* [2] begin if # blocks at amr level > 0 loop */

         /* parse existing blocks and group them together in newly defined
          * virtual parent blocks */

         for (block_index_1 = 0; block_index_1
               < number_of_blocks_at_amr_level[ current_amr_level ]; block_index_1++)
         { /* [3] begin for every block at each amr level loop */

            if (debug_flag)
            {
               printf(
                     "\n**DEBUG\tProcessing block %d for amr level %d\tactual "
                        "block is ?\n",
                     block_index_1,
                     current_amr_level);
            }

            block_count = block_at_amr_level[ number_of_blocks * ( current_amr_level ) + block_index_1 ];
            parent_index = -1; /* reset */
            block_index_2 = 0; /* index of parent block already established */

            if (debug_flag)
            {
               printf("DEBUG\tblock_count = block_at_amr_level[ number_of_blocks"
                  " * ( current_amr_level ) + block_index_1 ]\n");
               printf("DEBUG\tblock_count = block_at_amr_level"
                  "[        %d        *       %d              +     %d       "
                  " ]\n", number_of_blocks, current_amr_level, block_index_1);
               printf(
                     "DEBUG\tblock_count = block_at_amr_level[ %d ] = %d\n",
                     number_of_blocks * (current_amr_level ) + block_index_1,
                     block_at_amr_level[ number_of_blocks * ( current_amr_level ) + block_index_1 ]);
               printf("DEBUG\tsetting block_count = %d\n", block_count);
            }

            /* loop #[4] searches for an existing parent block for the current
             * block being processed, if no parent is found, one is created in
             * loop #[6] */

            /* virtual parent blocks may have been created for earlier blocks */

            while ( (parent_index == -1) && (block_index_2
                  < number_of_parent_blocks_at_amr_level[ parent_level ] ))
            { /* [4] begin while loop */

               /* test existing parents (which are appended to existing
                * unrefined blocks at the parent level)*/

               test_parent = block_at_amr_level[ number_of_blocks * ( parent_level )
               + number_of_blocks_at_amr_level[ parent_level ]
               + block_index_2 ];

               if ( (block_x_min[test_parent] - eps )
                     <= block_x_center[block_count]
                     && (block_x_max[test_parent] + eps )
                           >= block_x_center[block_count]
                     && (block_y_min[test_parent] - eps )
                           <= block_y_center[block_count]
                     && (block_y_max[test_parent] + eps )
                           >= block_y_center[block_count]
                     && (block_z_min[test_parent] - eps )
                           <= block_z_center[block_count]
                     && (block_z_max[test_parent] + eps )
                           >= block_z_center[block_count])
               { /* [5] begin if test structure */

                  if (debug_flag)
                  {
                     printf("DEBUG\t************ PARENT FOUND ************\n");
                  }

                  parent_index = test_parent;

                  /* position in parent block */

                  ix = block_x_center[ block_count ]
                        > block_x_center[parent_index];
                  iy = block_y_center[ block_count ]
                        > block_y_center[parent_index];
                  iz = block_z_center[ block_count ]
                        > block_z_center[parent_index];

                  /*printf("found position in parent block %d %d %d\n",ix,iy,iz);*/

                  parent_id[ block_count ] = parent_index;

                  if (debug_flag)
                  {
                     printf(
                           "DEBUG\tSetting parent_id[ %d ] = %d\n",
                           block_count,
                           parent_index);
                  }

                  child_id_key = iz*4+2*iy+ix;

                  if (debug_flag)
                  {
                     printf(
                           "DEBUG\tSetting child_id_%d[%d] = %d t\n",
                           child_id_key,
                           parent_index,
                           block_count);
                  }

                  switch (child_id_key)
                  {
                     case 0:
                        child_id_1[ parent_index ] = block_count;
                        break;
                     case 1:
                        child_id_2[ parent_index ] = block_count;
                        break;
                     case 2:
                        child_id_3[ parent_index ] = block_count;
                        break;
                     case 3:
                        child_id_4[ parent_index ] = block_count;
                        break;
                     case 4:
                        child_id_5[ parent_index ] = block_count;
                        break;
                     case 5:
                        child_id_6[ parent_index ] = block_count;
                        break;
                     case 6:
                        child_id_7[ parent_index ] = block_count;
                        break;
                     case 7:
                        child_id_8[ parent_index ] = block_count;
                        break;

                     default:
                        /* do something constructive */
                        printf("\n\nERROR cannot select appropriate child_id "
                           "array for child id %d\n\n", child_id_key);
                        return 0;
                        break;

                  }

                  child_count[ parent_index ]++;

               } /* [5]  end if test structure */

               block_index_2++;

            } /* [4] end while loop */

            /* loop #[6] creates a parent for the current block being processed
             * because no parent block was found in loop #[4] */

            if (parent_index == -1)
            { /* [6] begin if loop nested loop number 6 */

               /* create new virtual block */

               parent_index = number_of_blocks + number_of_parents;

               if (debug_flag)
               {
                  printf(
                        "\n!!!!!DEBUG\tNO parent found...creating parent whith "
                           "index:\t%d\n",
                        parent_index);
               }

               number_of_parents++;

               if (number_of_parents > number_of_blocks / 7)
               { /* [7] begin error check statement */
                  printf(
                        "number_of_parents %d > %d and therofore is too large!\n",
                        number_of_parents,
                        number_of_blocks / 7);
                  return 0;
               } /* [7] end error check statemen */

               block_at_amr_level[ number_of_blocks * ( parent_level ) +
               number_of_blocks_at_amr_level[ parent_level ]
               + number_of_parent_blocks_at_amr_level[ parent_level ] ]
                     = parent_index;

               number_of_parent_blocks_at_amr_level[ parent_level ]
                     = number_of_parent_blocks_at_amr_level[ parent_level ] + 1;

               ix
                     = ( int )floor( (block_x_center[ block_count ]
                           - global_x_min ) * special_parameter_P1
                           * level_factor / (global_x_max - global_x_min )) % 2;

               if (ix == 0)
               { /* [8] */

                  block_x_min[ parent_index ] = block_x_min[ block_count ]
                        - (current_amr_level <= 1 ) * eps;
                  block_x_max[ parent_index ] = 2 * block_x_max[ block_count ]
                        - block_x_min[ block_count ] + (current_amr_level <= 1 )
                        * eps;

               }
               else
               {
                  block_x_min[ parent_index ] = 2 * block_x_min[ block_count ]
                        - block_x_max[ block_count ] - (current_amr_level <= 1 )
                        * eps;
                  block_x_max[ parent_index ] = block_x_max[ block_count ]
                        + (current_amr_level <= 1 ) * eps;
               } /* [8] */

               iy
                     = ( int )floor( (block_y_center[ block_count ]
                           - global_y_min ) * special_parameter_P2
                           * level_factor / (global_y_max - global_y_min )) % 2;

               if (iy == 0)
               { /* [9] */

                  block_y_min[ parent_index ] = block_y_min[ block_count ]
                        - (current_amr_level <= 1 ) * eps;
                  block_y_max[ parent_index ] = 2 * block_y_max[ block_count ]
                        - block_y_min[ block_count ] + (current_amr_level <= 1 )
                        * eps;

               }
               else
               {
                  block_y_min[ parent_index ] = 2 * block_y_min[ block_count ]
                        - block_y_max[ block_count ] - (current_amr_level <= 1 )
                        * eps;
                  block_y_max[ parent_index ] = block_y_max[ block_count ]
                        + (current_amr_level <= 1 ) * eps;
               } /* [9] */

               iz
                     = ( int )floor( (block_z_center[ block_count ]
                           - global_z_min ) * special_parameter_P3
                           * level_factor / (global_z_max - global_z_min )) % 2;

               if (iz == 0)
               { /* [10] */

                  block_z_min[ parent_index ] = block_z_min[ block_count ]
                        - (current_amr_level <= 1 ) * eps;
                  block_z_max[ parent_index ] = 2 * block_z_max[ block_count ]
                        - block_z_min[ block_count ] + (current_amr_level <= 1 )
                        * eps;

               }
               else
               {
                  block_z_min[ parent_index ] = 2 * block_z_min[ block_count ]
                        - block_z_max[ block_count ] - (current_amr_level <= 1 )
                        * eps;
                  block_z_max[ parent_index ] = block_z_max[ block_count ]
                        + (current_amr_level <= 1 ) * eps;
               } /* [10] */

               block_x_center[parent_index] = 0.5 * (block_x_min[parent_index]
                     + block_x_max[parent_index] );
               block_y_center[parent_index] = 0.5 * (block_y_min[parent_index]
                     + block_y_max[parent_index] );
               block_z_center[parent_index] = 0.5 * (block_z_min[parent_index]
                     + block_z_max[parent_index] );

               /*child_count[parent_index]++;*/
               child_count[parent_index] = 1;

               child_id_key = ix+2*iy+4*iz;

               switch (child_id_key)
               {
                  case 0:
                     child_id_1[ parent_index ] = block_count;
                     break;
                  case 1:
                     child_id_2[ parent_index ] = block_count;
                     break;
                  case 2:
                     child_id_3[ parent_index ] = block_count;
                     break;
                  case 3:
                     child_id_4[ parent_index ] = block_count;
                     break;
                  case 4:
                     child_id_5[ parent_index ] = block_count;
                     break;
                  case 5:
                     child_id_6[ parent_index ] = block_count;
                     break;
                  case 6:
                     child_id_7[ parent_index ] = block_count;
                     break;
                  case 7:
                     child_id_8[ parent_index ] = block_count;
                     break;

                  default:
                     /* do something constructive */
                     printf(
                           "\n\nERROR cannot select appropriate child_id array "
                              "for child id %d\n\n",
                           child_id_key);
                     return 0;
                     break;

               }

               parent_id[block_count] = parent_index;

            } /* [6] end nested if loop number 6 */

         } /* [3] end for every block at each amr level loop */

      } /* [2] end if # blocks at amr level > 0 loop */

   } /* [1] end parent block calculation for loop */

   number_of_blocks_at_amr_level[0] += number_of_parent_blocks_at_amr_level[0];

   if (verbose_flag)
   {
      printf("\nGRID STRUCTURE SUCCESSFULLY GENERATED\n\nsummary:\n");
      printf("Number of parents:\t%d\n", number_of_parents);

      for (i = 0; i < max_amr_level; i++)
      {
         printf(
               "\nnumber_of_blocks_at_amr_level[%d] = \t%d\t"
                  "number_of_parent_blocks_at_amr_level[%d] = \t%d\n",
               i,
               number_of_blocks_at_amr_level[i],
               i,
               number_of_parent_blocks_at_amr_level[i]);
      }
   }

   /* remove 1 from calculated max_amr_level to get to actual max_amr_level */
   max_amr_level--;

   return 1;

}

/******************* Lutz's read_batsrus_variable *****************************/

int read_batsrus_variable(
      FILE *input_filePtr,
      float *variable_array,
      int flip_endian,
      int is_double,
      double *tmp_double)
{
   int byte_value, record_size, number_elements, four_bytes=4, eight_bytes=8,
         one_element=1;
   long int i, read_failed_flag=0;

   fread( &byte_value, four_bytes, one_element, input_filePtr);
   if (flip_endian)
      endian_flip((char*)&byte_value, four_bytes, one_element);
   record_size = byte_value;
   if (is_double)
   {
      number_elements = record_size / eight_bytes;
      fread(tmp_double, eight_bytes, number_elements, input_filePtr);
      if (flip_endian)
         endian_flip((char *)tmp_double, eight_bytes, number_elements);
      for (i=0L; i<number_elements; i++)
         variable_array[i]=tmp_double[i];
   }
   else
   {
      number_elements = record_size / four_bytes;
      fread(variable_array, four_bytes, number_elements, input_filePtr);
      if (flip_endian)
         endian_flip((char*)variable_array, four_bytes, number_elements);
   }
   fread( &byte_value_2, four_bytes, one_element, input_filePtr);
   if (flip_endian)
      endian_flip((char*)&byte_value_2, four_bytes, one_element);
   if ( (byte_value != byte_value_2) || (number_elements != number_of_cells))
   {
      read_failed_flag = 1;
   }

   return (read_failed_flag);
}
