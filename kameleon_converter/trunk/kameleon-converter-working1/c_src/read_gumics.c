/*******************************************************************************
 *                                                                             *
 *    NAME:          read_gumics.c                                             *
 *                                                                             *
 *    VERSION:       2.3                                                       *
 *                                                                             *
 *    AUTHOR:        Lutz Rastaetter                                           *
 *                   Space Weather laboratory (674)                            *
 *                   Lutz.Rastaetter@nasa.gov                                  *
 *                   (301) 286 - 1085                                          *
 *                                                                             *
 *    PURPOSE:       Custom read routine for GUMICS TecPlot model output.      *
 *                   All available GUMICS data is read into main memory were it   *
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
 *    MODIFICATION iHISTORY                                                    *
 *    read_batsrus.                                                            *
 *    04.24.2012              Lutz Rastaetter                                  *
 *                            Adding blocks to lowest refinement level to make *
 *                          P1*P2*P3 blocks if there are more than zero blocks *
 *    2018/03/30    Lutz Rastaetter - read_gumics.c derived from read_batsrus.c*       
 ******************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "read_batsrus.h" /* yes, we need teh variables defined here! */
#include "endian_flip.h"
#include "strsplit.h"
#include "read_database.h"
#include "cdf.h"

#define max(a,b) ( (a)>(b) ? (a) : (b) )
#define min(a,b) ( (a)<(b) ? (a) : (b) )

#define MAX_FIRST_RECORD        65535  /** Changed from 200 -> 32767  **/

#define DEBUG_FLAG 0
/* debug level for setup_parent() */
#define DEBUG 0

extern int get_child_id(int iblock, int ichild);
extern void put_child_id(int child_id, int iblock, int ichild);
extern int climb_octree(int root, float x, float y, float z, int max_level);
extern int setup_parent(int iblock, int ilev,
			float XMIN, float YMIN, float ZMIN,
			int N_blks, int N_parents_max, int *N_parents);
extern int unique(
	float *input_array,
	int input_array_len,
	int stride,
	int **output_index_array);

float *special_parameter_array;

extern int errno;
#include "cxform.h"

/*******************************************************************************
 *                                       MAIN                                  *
 ******************************************************************************/

int read_gumics(char *input_file_name, int verbose_flag, int minmax_flag)
{

  extern int linear_minmax_search(float *, int, float *, float *);
  
  int status_code;
  
  if (verbose_flag)
    {
      printf("...reading %s\n", input_file_name);
    }
  
  if ((input_filePtr = fopen(input_file_name, "r")) == NULL)
    {
      printf("ERROR Opening File \n");
      perror(input_file_name);
      /* should return control if open was unsucessful */
      return EXIT_FAILURE;
    }
  else
    {
      status_code = read_gumics_data(verbose_flag);
      
      if (!status_code)
	{
	  return EXIT_FAILURE;
	}
      
      if (DEBUG_FLAG)
	printf(
	       "DEBUG\tnumber_of_cells = x_dimension_size * y_dimension_size * "
	       "z_dimension_size\n\t = %d * %d * %d = %d\n",
	       x_dimension_size,
	       y_dimension_size,
	       z_dimension_size,
	       number_of_cells);
      
      number_of_cells = x_dimension_size * y_dimension_size
	* z_dimension_size;
      
      if (DEBUG_FLAG)
	printf(
	       "DEBUG\tnumber_of_cells = x_dimension_size * y_dimension_size * "
	       "z_dimension_size\n\t = %d * %d * %d = %d\n",
	       x_dimension_size,
	       y_dimension_size,
	       z_dimension_size,
	       number_of_cells);
      
      fclose(input_filePtr);

    /* auto-detect NX, NY, NZ: from Lutz Rastaetter's 3DView/lib/setup_octree.c */
      if (special_parameter_NX <= 1)
	{
	  int icell;
	  icell = 1;
	  while (x_position_arrayPtr[0] != x_position_arrayPtr[icell])
	    {
	      if (icell > number_of_cells / 4)
		{
		  return (EXIT_FAILURE);
		}
	      else
		{
		  icell++;
		}
	    }
	  special_parameter_NX = icell;
	}
      if (special_parameter_NY <= 1)
	{
	  int icell;
	  icell = floor(special_parameter_NX);
	while (y_position_arrayPtr[0] != y_position_arrayPtr[icell])
	  {
	    if (icell > number_of_cells / (2 * special_parameter_NX))
	      {
		return (EXIT_FAILURE);
	      }
	    else
	      {
		icell += floor(special_parameter_NX);
	      }
	  }
	special_parameter_NY = icell / special_parameter_NX;
	}

      if (special_parameter_NZ <= 1)
	{
	  int icell, nzb, *uniq_z, n_uniq = 1, i_uniq = 0, nv;
	  float tmp_z;
	  nv = special_parameter_NX * special_parameter_NY;
	  nzb = number_of_cells / nv;
	  n_uniq = unique(x_position_arrayPtr, number_of_cells, nv, &uniq_z);
	  special_parameter_NZ = nzb;
	  if (DEBUG)
	    {
	      for (i_uniq = 0; i_uniq < 20; i_uniq++)
		{
		  fprintf(stderr, "unique[%i]=%i\n", i_uniq, uniq_z[i_uniq]);
		}
	    }
	  for (i_uniq = 0; i_uniq < n_uniq - 1; i_uniq++)
	    {
	      if (special_parameter_NZ > (uniq_z[i_uniq + 1] - uniq_z[i_uniq]))
		{
		  special_parameter_NZ = (uniq_z[i_uniq + 1] - uniq_z[i_uniq]);
		}
	    }
	  free(uniq_z);
	}

      if (verbose_flag)
	{
	  printf(
		 "Read_gumics after read: Special parameters: NX: %f NY: %f NZ: %f\n",
		 special_parameter_NX,
		 special_parameter_NY,
		 special_parameter_NZ);
	}
      
      number_of_blocks = number_of_cells / (special_parameter_NX
					    * special_parameter_NY * special_parameter_NZ);

      fprintf(
	      stderr,
	      "NX %f NY %f NZ %f\nP1: %f P2: %f p3: %f\n",
	      special_parameter_NX,
	      special_parameter_NY,
	      special_parameter_NZ,
	      special_parameter_P1,
	      special_parameter_P2,
	      special_parameter_P3);
      
      status_code = build_gumics_grid_description(verbose_flag);
      printf("status: %i\n",status_code);

    /*********************  calcluate actual min/max values for each ariable
     * unless -nominmax flag was specified *************/

    /**** add more error handling for each linear_minmax_search function
     * call ***/

    /*** if -nominmax option was NOT specified ***/

      if (!minmax_flag)
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
	  
	  /*
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

	*/

	/* only calculate if optional_status_variable_present = 1 */

	  if (optional_status_variable_present)
	    {
	      
	      linear_minmax_search(
				   status_arrayPtr,
				   number_of_cells,
				   &status_actual_min,
				   &status_actual_max);
	      if (verbose_flag)
		{
		  printf(
			 "%-25s%-25g%-25g\n",
			 "status",
			 status_actual_min,
			 status_actual_max);
		}
	      
	    }

	}

    /* return 1 if all reads were succesful. otherwise return 0 */

    return status_code;
    
    }
}

int read_gumics_data( verbose_flag)
    {
#define number_of_characters 256
      static char *line1_1,*line1_2,*line1_3,*line1_4,*line1_5,*line1_6;
      static char *line2_1,*line2_2,*line2_3;
      static char *line3_1,*line3_2,*line3_3,*line3_4,*line3_5,*line3_6,*line3_7;
      line1_1=(char*)malloc(number_of_characters);
      line1_2=(char*)malloc(number_of_characters);
      line1_3=(char*)malloc(number_of_characters);
      line1_4=(char*)malloc(number_of_characters);
      line1_5=(char*)malloc(number_of_characters);
      line1_6=(char*)malloc(number_of_characters);
      line2_1=(char*)malloc(number_of_characters);
      line2_2=(char*)malloc(number_of_characters);
      line2_3=(char*)malloc(number_of_characters);
      line3_1=(char*)malloc(number_of_characters);
      line3_2=(char*)malloc(number_of_characters);
      line3_3=(char*)malloc(number_of_characters);
      line3_4=(char*)malloc(number_of_characters);
      line3_5=(char*)malloc(number_of_characters);
      line3_6=(char*)malloc(number_of_characters);
      line3_7=(char*)malloc(number_of_characters);

      char line[number_of_characters];
      int record_size, a, n = 0;
      /* block stucture parameters */
      int NX_set = 1, NY_set = 1, NZ_set = 1;
      int P1_set = 0, P2_set = 0, P3_set = 0;
      int th_set = 0;
      int n_blks=0,n_vertices;
      
      special_parameter_NX=2;
      special_parameter_NY=2;
      special_parameter_NZ=2;
      special_parameter_R=3.7;
      special_parameter_th=0.; /* don't know from this file */
      special_parameter_P1=0; /* don't know from this file */
      special_parameter_P2=0; /* don't know from this file */
      special_parameter_P3=0; /* don't know from this file */
            
    /*** get the number of bytes in record 1 ( these are 1 byte caracters that
     * specify the units for model variables)*/
      fgets(line,number_of_characters,input_filePtr);
      printf("Line 1:|%s|\n",line);
      record_size=sscanf(line,"%s %s %s %s %s %s",line1_1,line1_2,line1_3,line1_4,line1_5,line1_6);
      printf("read line 1: %s %s %s %s %s %s\n",line1_1,line1_2,line1_3,line1_4,line1_5,line1_6);
      if (record_size != 6 ){
	fprintf(stderr,"Line 1 records read: %i expected: 6\n",record_size);
		/*of the GUMICS input file may have exceeded string length.\n"); */
	return 0;
      }

      /*    record_size=fscanf(input_filePtr,"%s",line2); */
      
      fgets(line,number_of_characters,input_filePtr);
      printf("Line 2:|%s|\n",line);
      
      record_size=sscanf(line,"%s %s %s",line2_1,line2_2,line2_3);
      
      if (record_size != 3){
	fprintf(stderr,"Line 2 of the GUMICS input file may have exceeded string length.\n");
	return 0;
      }
      printf("read line 2: %s %s %s\n",line2_1,line2_2,line2_3);
      
      fgets(line,number_of_characters,input_filePtr);
      record_size=sscanf(line,"%s %s %s %s %s %s %s",line3_1,line3_2,line3_3,line3_4,line3_5,line3_6,line3_7);
      if (record_size != 7){
	fprintf(stderr,"Line 3 of the GUMICS input file may have exceeded string length.\n");
	return 0;
      }
    
      int error;
      if (strcmp(line1_6,"") == 0){
	printf("read_GUMICS: line1 \"%s %s\" does not have the expected elements\n",line1_5,line1_6);
	return(0);
      }
/* read number after "T = " in
TITLE = "mstate19991231_220030.hc, t = 30"
*/
      if (sscanf(line1_6,"%f",&elapsed_time_seconds) != 1){
	printf("read error for GUMICS elapsed time\n");
	return(0);
      }
      printf("elapsed time: %f\n",elapsed_time_seconds);
      /* 
split variable string into comma-separated components:
VARIABLES = "X","Y","Z","rho","rhoUx","rhoUy","rhoUz","U","P","Bx","By","Bz","Bx1","By1","Bz1"
 */

      strarr variable_string_array;
    /* initialize - or we'll get a SEGMENTATION FAULT/BUS ERROR */
      variable_string_array.len=0;
      variable_string_array.sarr=NULL;
      printf("line2_3: |%s|\n",line2_3);
      error=strsplit(line2_3,&variable_string_array,",");
      unit_string=(char*)malloc(number_of_characters);
      strcpy(unit_string,"\n");
      
      /* read the value after N= in
ZONE T="Whole grid", N=1057318, E=952376, ET=brick, F=FEPOINT
    */
    printf("line3_4: |%s|\n",line3_4);
    printf("line3_5: |%s|\n",line3_5);

    if (sscanf(line3_4+2,"%i",&n_vertices) != 1){
      printf("read error for GUMICS grid size\n");
      return(0);
    }
    if (sscanf(line3_5+2,"%i",&n_blks) != 1){
      printf("read error for GUMICS grid size\n");
      return(0);
    }

    /* dimension of final data arrays */
    x_dimension_size=n_blks*special_parameter_NX*special_parameter_NY*special_parameter_NZ;

    /* dimension of data in TecPlot file */
    int n_variables=variable_string_array.len;
    float *data_buffer;
    int *index_buffer;
    data_buffer =(float*) malloc(n_variables*n_vertices*sizeof(float));
    index_buffer=(int*) malloc(8*n_blks*sizeof(int)); /* indices for corners of brick */
    /*    unit_string = (char*) malloc((record_size + 1) * sizeof(char));

    n = fread(unit_string, 1, record_size, input_filePtr);

    unit_string[n] = '\0';
    */

    if (DEBUG_FLAG)
	printf(
		"strlen( unit_string ) = %d\nn = %d\n",
		(int) strlen(unit_string),
		n);

    if (ferror(input_filePtr) || feof(input_filePtr))
	{
	printf(
		"ERROR with fread must have returned non zero status_code for file "
		    "stream\n");
	perror("input_filePtr");
	return 0;
	}
    if (x_dimension_size < 0 ){
      printf("read_gumics: reading of grid cell failed\n");
      return(EXIT_FAILURE);
    }
    number_of_cells=x_dimension_size;
    printf("number of cells: %i variables: %i\n",number_of_cells,variable_string_array.len);
    /* allocate field arrays */
    int rho_index,ux_index,uy_index,uz_index;
    float *variable_pointer[variable_string_array.len];
    int i;

    for (i=0;i<variable_string_array.len;i++){
      if (strcmp(variable_string_array.sarr[i],"\"X\"") == 0){
	strcat(unit_string," m");
	if ((x_position_arrayPtr = (float *) calloc(number_of_cells, sizeof(float))) == NULL){
	  printf("MALLOC for rho array FAILED\n");
	  exit(EXIT_FAILURE);
	}
	variable_pointer[i]=x_position_arrayPtr;
      }
      if (strcmp(variable_string_array.sarr[i],"\"Y\"") == 0){
	strcat(unit_string," m");
	if ((y_position_arrayPtr = (float *) calloc(number_of_cells, sizeof(float))) == NULL) {
	  printf("MALLOC for rho array FAILED\n");
	  exit(EXIT_FAILURE);
	}
	variable_pointer[i]=y_position_arrayPtr;
      }
      if (strcmp(variable_string_array.sarr[i],"\"Z\"") == 0){
	strcat(unit_string," m");
	if ((z_position_arrayPtr = (float *) calloc(number_of_cells, sizeof(float))) == NULL) {
	  printf("MALLOC for rho array FAILED\n");
	  exit(EXIT_FAILURE);
	}
	variable_pointer[i]=z_position_arrayPtr;
      }
      if (strcmp(variable_string_array.sarr[i],"\"rho\"") == 0){
	strcat(unit_string,"amu/cm^3");
	if ((rho_arrayPtr = (float *) calloc(number_of_cells, sizeof(float))) == NULL) {
	  printf("MALLOC for rho array FAILED\n");
	  exit(EXIT_FAILURE);
	}
	variable_pointer[i]=rho_arrayPtr;
	rho_index=i;
      }
      if (strcmp(variable_string_array.sarr[i],"\"rhoUx\"") == 0){
	strcat(unit_string," km/s");
	if ((ux_arrayPtr = (float *) calloc(number_of_cells, sizeof(float))) == NULL) {
	  printf("MALLOC for rho array FAILED\n");
	  exit(EXIT_FAILURE);
	}
	variable_pointer[i]=ux_arrayPtr;
	ux_index=i;	
      }
      if (strcmp(variable_string_array.sarr[i],"\"rhoUy\"") == 0){
	strcat(unit_string," km/s");
	if ((uy_arrayPtr = (float *) calloc(number_of_cells, sizeof(float))) == NULL) {
	  printf("MALLOC for rho array FAILED\n");
	  exit(EXIT_FAILURE);
	}
	variable_pointer[i]=uy_arrayPtr;
	uy_index=i;	
      }
      if (strcmp(variable_string_array.sarr[i],"\"rhoUz\"") == 0){
	strcat(unit_string," km/s");
	if ((uz_arrayPtr = (float *) calloc(number_of_cells, sizeof(float))) == NULL) {
	  printf("MALLOC for rho array FAILED\n");
	  exit(EXIT_FAILURE);
	}
	variable_pointer[i]=uz_arrayPtr;
	uz_index=i;	
      }
      if (strcmp(variable_string_array.sarr[i],"\"U\"") == 0){
	strcat(unit_string," fJ/cm^3");
	if ((e_arrayPtr = (float *) calloc(number_of_cells, sizeof(float))) == NULL) {
	  printf("MALLOC for rho array FAILED\n");
	  exit(EXIT_FAILURE);
	}
	variable_pointer[i]=e_arrayPtr;
      }
      if (strcmp(variable_string_array.sarr[i],"\"P\"") == 0){
	strcat(unit_string," nPa");
	if ((p_arrayPtr = (float *) calloc(number_of_cells, sizeof(float))) == NULL) {
	  printf("MALLOC for rho array FAILED\n");
	  exit(EXIT_FAILURE);
	}
	variable_pointer[i]=p_arrayPtr;
      }
      if (strcmp(variable_string_array.sarr[i],"\"Bx\"") == 0){
	strcat(unit_string," nT");
	if ((bx_arrayPtr = (float *) calloc(number_of_cells, sizeof(float))) == NULL) {
	  printf("MALLOC for rho array FAILED\n");
	  exit(EXIT_FAILURE);
	}
	variable_pointer[i]=bx_arrayPtr;
      }
      if (strcmp(variable_string_array.sarr[i],"\"By\"") == 0){
	strcat(unit_string," nT");
	if ((by_arrayPtr = (float *) calloc(number_of_cells, sizeof(float))) == NULL) {
	  printf("MALLOC for rho array FAILED\n");
	  exit(EXIT_FAILURE);
	}
	variable_pointer[i]=by_arrayPtr;
      }
      if (strcmp(variable_string_array.sarr[i],"\"Bz\"") == 0){
	strcat(unit_string," nT");
	if ((bz_arrayPtr = (float *) calloc(number_of_cells, sizeof(float))) == NULL) {
	  printf("MALLOC for rho array FAILED\n");
	  exit(EXIT_FAILURE);
	}
	variable_pointer[i]=bz_arrayPtr;
      }
      if (strcmp(variable_string_array.sarr[i],"\"Bx1\"") == 0){
	strcat(unit_string," nT");
	if ((b1x_arrayPtr = (float *) calloc(number_of_cells, sizeof(float))) == NULL) {
	  printf("MALLOC for rho array FAILED\n");
	  exit(EXIT_FAILURE);
	}
	variable_pointer[i]=b1x_arrayPtr;
      }
      if (strcmp(variable_string_array.sarr[i],"\"By1\"") == 0){
	strcat(unit_string," nT");
	if ((b1y_arrayPtr = (float *) calloc(number_of_cells, sizeof(float))) == NULL) {
	  printf("MALLOC for rho array FAILED\n");
	  exit(EXIT_FAILURE);
	}
	variable_pointer[i]=b1y_arrayPtr;
      }
      if (strcmp(variable_string_array.sarr[i],"\"Bz1\"") == 0){
	strcat(unit_string," nT");
	if ((b1z_arrayPtr = (float *) calloc(number_of_cells, sizeof(float))) == NULL) {
	  printf("MALLOC for rho array FAILED\n");
	  exit(EXIT_FAILURE);
	}
	variable_pointer[i]=b1z_arrayPtr;
      }
    }

    if (verbose_flag)
      printf("unit string = ---->%s<----\n", unit_string);

    /* read data */

    if (verbose_flag)
      {
	printf("READ Record 1 SUCCESSFUL\n");
      }
    
    y_dimension_size=1;
    z_dimension_size=1;
    if (verbose_flag)
      {
	printf("READ Data with (%d,%d) elements\n",variable_string_array.len,x_dimension_size);
      }
    
    if (!th_set && batsrus_special_parameter_th_exists_in_databaseinfo)
      {
	fprintf(stderr, "!!!! %s\tWARNING: special parameter th could not be "
		"located.  Will attempt to compute parameter\n", __FILE__);
	if (strcmp(dipole_update, "yes") != 0)
	  {
	    special_parameter_th = -atof(batsrus_special_parameter_th);
	  }
	else
	  {
	    /*get cxform to compute it  */
	    int return_status = 0;
	    double cxform_et, cdf_epoch_start_time_value,
	      cdf_epoch_dipole_time_value, cxform_et_offset_to_cdf_epoch;
	    double v_in[3], v_out[3];
	    v_in[0] = 0.;
	    v_in[0] = 0.;
	    v_in[0] = 1.;
	    
	    cdf_epoch_start_time_value = parseEPOCH3(start_time_cdf_epoch3);
	    
	    /*** so elapsed_time_seconds won't be in milliseconds ***/
	    
	    cdf_epoch_dipole_time_value = cdf_epoch_start_time_value
	      + (elapsed_time_seconds * 1000);
	    cxform_et_offset_to_cdf_epoch = computeEPOCH(
							 2000,
							 1,
							 1,
							 12,
							 0,
							 0,
							 0);
	    cxform_et = (cdf_epoch_dipole_time_value
			 - cxform_et_offset_to_cdf_epoch) / 1000.;
	    return_status = cxform("SM", "GSE", cxform_et, v_in, v_out);
	    special_parameter_th = atan2(v_out[2], v_out[0]); /* *180./3.14159264; */
	  }
      }
    
    /****** P1, P2, & P3 is crucial for block count calculations, if not set see
     * if database info has them ********/

    /* if .out doesn't contain P1, check see if DatabaseInfo file set the value */
/*P1_set=1;
special_parameter_P1=2;
P2_set=1;
special_parameter_P2=1;
P3_set=1;
special_parameter_P3=1;
*/
    if (!P1_set && batsrus_special_parameter_P1_exists_in_databaseinfo)
      {
	special_parameter_P1 = atof(batsrus_special_parameter_P1);
	if (verbose_flag)
	    {
	      printf(
		    "setting special parameter P1 from DatabaseInfo file:\t%f\n",
		    special_parameter_P1);
	    }
      }
    else
	if (!P1_set && !batsrus_special_parameter_P1_exists_in_databaseinfo)
	  {
	    fprintf(
		    stderr,
		    "!!!! %s\tWARNING: special parameter P1 could not be "
		    "located.  Will attempt to compute parameter\n"
		    "# batsrus_special_parameter_P1 later ...\n",
		    __FILE__);
	    /*      fprintf( stderr, "!!!! %s\tERROR: special parameter P1 could not be l"
		    "ocated.  Set key value in DatabaseInfo file to proceed...\nie. 6 "
		    "# batsrus_special_parameter_P1\n", __FILE__ );
		    return 0; */
	  }
    if (!P2_set && batsrus_special_parameter_P2_exists_in_databaseinfo)
      {
	special_parameter_P2 = atof(batsrus_special_parameter_P2);
	if (verbose_flag)
	  {
	    printf(
		   "setting special parameter P2 from DatabaseInfo file:\t%f\n",
		   special_parameter_P2);
	  }
      }
    else
      if (!P2_set && !batsrus_special_parameter_P2_exists_in_databaseinfo)
	{
	  fprintf(
		  stderr,
		  "!!!! %s\tWARNING: special parameter P2 could not be "
		  "located.  Will attempt to compute parameter\n"
		  "# batsrus_special_parameter_P2 later ...\n",
		  __FILE__);
	  /*      fprintf( stderr, "!!!! %s\tERROR: special parameter P2 could not be "
		  "located.  Set key value in DatabaseInfo file to proceed...\nie. 6 "
		  "# batsrus_special_parameter_P2\n", __FILE__ );
		  return 0; */
	}
    if (!P3_set && batsrus_special_parameter_P3_exists_in_databaseinfo)
      {
	special_parameter_P3 = atof(batsrus_special_parameter_P3);
	if (verbose_flag)
	  {
	    printf(
		   "setting special parameter P3 from DatabaseInfo file:\t%f\n",
		   special_parameter_P3);
	  }
      }
    else
      if (!P3_set && !batsrus_special_parameter_P3_exists_in_databaseinfo)
	{
	  fprintf(
		  stderr,
		  "!!!! %s\tWARNING: special parameter P3 could not be "
		  "located.  Will attempt to compute parameter\n"
		  "# batsrus_special_parameter_P3 later ...\n",
		  __FILE__);
	  /*      fprintf( stderr, "!!!! %s\tERROR: special parameter P3 could not be "
		  "located.  Set key value in DatabaseInfo file to proceed...\nie. 6 "
		  "# batsrus_special_parameter_P3\n", __FILE__ );
		  return 0; */
	}
    if (verbose_flag)
      {
	printf(
	       "Read Record 5: Special parameters: NX: %f R: %f P3: %f\n",
	       special_parameter_NX,
	       special_parameter_R,
	       special_parameter_P3);
      }
    

    /* read data */
    /* read vertex data */
    fgets(line,number_of_characters,input_filePtr);
    if (n_variables == 15){
      int j=0;
      float d0,d1,d2,d3,d4,d5,d6,d7,d8,d9,d10,d11,d12,d13,d14,d15;
      for (i=0;i<n_vertices;i++){
      /*      for (i=0;i<1;i++){ */
	fgets(line,number_of_characters,input_filePtr);
	/*	printf("line: |%s|\n",line); */
	if (sscanf(line," %f %f %f %f %f %f %f %f %f %f %f %f %f %f %f"
		   ,&d0,&d1,&d2,&d3,&d4,&d5,&d6,&d7,&d8,&d9,&d10,&d11,&d12,&d13,&d14)!= 15){
	  printf("(Scan error: line does not have 15 float values\nLine: |%s|\n",line); 
	  return 0;
	}
	/*	printf ("%f %f\n",data_vector[0],data_vector[14]); */
	data_buffer[15*i]=d0;
	data_buffer[15*i+1]=d1;
	data_buffer[15*i+2]=d2;
	data_buffer[15*i+3]=d3;
	data_buffer[15*i+4]=d4;
	data_buffer[15*i+5]=d5;
	data_buffer[15*i+6]=d6;
	data_buffer[15*i+7]=d7;
	data_buffer[15*i+8]=d8;
	data_buffer[15*i+9]=d9;
	data_buffer[15*i+10]=d10;
	data_buffer[15*i+11]=d11;
	data_buffer[15*i+12]=d12;
	data_buffer[15*i+13]=d13;
	data_buffer[15*i+14]=d14;
      }
    }
    printf("first line data:\n");
    for (i=0;i<15;i++){printf("%g ",data_buffer[i]);}
    printf("\n");
    printf("last line data (vertex: %i):\n",n_vertices-1);
    for (i=0;i<15;i++){printf("%g ",data_buffer[15*(n_vertices-1)+i]);}
    printf("\n");

    /* read index array */
    int i0,i1,i2,i3,i4,i5,i6,i7;
    fgets(line,number_of_characters,input_filePtr);
    for (i=0;i<n_blks;i++){
      int j=0;
      /*      printf("%s\n",line); */
      fgets(line,number_of_characters,input_filePtr);
      if (sscanf(line,"%i %i %i %i %i %i %i %i",&i0,&i1,&i2,&i3,&i4,&i5,&i6,&i7) != 8){
	return 0;};
      /*      printf ("%i %i\n",index_vector[0],index_vector[7]); */
      index_buffer[8*i]=i0-1;
      index_buffer[8*i+1]=i1-1;
      index_buffer[8*i+2]=i2-1;
      index_buffer[8*i+3]=i3-1;
      index_buffer[8*i+4]=i4-1;
      index_buffer[8*i+5]=i5-1;
      index_buffer[8*i+6]=i6-1;
      index_buffer[8*i+7]=i7-1;
    }
    /* assign to block vertices */

    printf("n_blks: %i n_variables: %i\n",n_blks,n_variables);
    printf("first block indices:\n");
    for (i=0;i<8;i++){printf("%i ",index_buffer[i]);}
    printf("\n");
    printf("last block indices:\n");
    for (i=(n_blks-1)*8;i<n_blks*8;i++){printf("%i ",index_buffer[i]);}
    printf("\n");

    printf("first line data:\n");
    for (i=0;i<15;i++){printf("%g ",data_buffer[i]);}
    printf("\n");
    printf("last line data (vertex: %i):\n",n_vertices-1);
    for (i=0;i<15;i++){printf("%g ",data_buffer[15*(n_vertices-1)+i]);}
    printf("\n");

    int cell_remap[]={4,5,7,6,0,1,3,2};
    if (n_variables == 15){
      int i;
      for (i=0;i<n_blks;i++){
	float single_value,rho_tmp;
	int ivar,j;
	for (j=0;j<8;j++){
	  int index,index_blk;
	  float R_E=6371200.;
	  float m_p_m3_cm3=1.6726e-21;
	  float to_nano=1e9;
	  float to_kilo=1e-3;
	  /*	  float to_femto=1e15; */
	  index=index_buffer[cell_remap[j] + 8*i];
	  index_blk=i*8+j;
	  if (i == 0){printf("index: %i index_blk: %i x: %g\n",index,index_blk,data_buffer[15*i]);}
	  x_position_arrayPtr[index_blk]= data_buffer[n_variables*index]/R_E;
	  y_position_arrayPtr[index_blk]= data_buffer[n_variables*index+1]/R_E;
	  z_position_arrayPtr[index_blk]= data_buffer[n_variables*index+2]/R_E;
	  rho_arrayPtr[index_blk]= data_buffer[n_variables*index+3]/m_p_m3_cm3;
	  /*	  ux_arrayPtr[index_blk]= data_buffer[n_variables*index+4];
	  uy_arrayPtr[index_blk]= data_buffer[n_variables*index+5];
	  uz_arrayPtr[index_blk]= data_buffer[n_variables*index+6];*/
	  ux_arrayPtr[index_blk]= data_buffer[n_variables*index+4]/data_buffer[n_variables*index+3]*to_kilo;
	  uy_arrayPtr[index_blk]= data_buffer[n_variables*index+5]/data_buffer[n_variables*index+3]*to_kilo;
	  uz_arrayPtr[index_blk]= data_buffer[n_variables*index+6]/data_buffer[n_variables*index+3]*to_kilo;
	  e_arrayPtr[index_blk]= data_buffer[n_variables*index+7]*to_nano;
	  p_arrayPtr[index_blk]= data_buffer[n_variables*index+8]*to_nano;
	  bx_arrayPtr[index_blk]= data_buffer[n_variables*index+9]*to_nano;
	  by_arrayPtr[index_blk]= data_buffer[n_variables*index+10]*to_nano;
	  bz_arrayPtr[index_blk]= data_buffer[n_variables*index+11]*to_nano;
	  b1x_arrayPtr[index_blk]= data_buffer[n_variables*index+12]*to_nano;
	  b1y_arrayPtr[index_blk]= data_buffer[n_variables*index+13]*to_nano;
	  b1z_arrayPtr[index_blk]= data_buffer[n_variables*index+14]*to_nano;
	}
      }
    }
    printf("First block x,y,z,rho:\n");
    for (i=0;i<16;i++){
      printf("%g %g %g %g %g %g %g %g %g %g %g %g %g %g %g\n",x_position_arrayPtr[i],y_position_arrayPtr[i],z_position_arrayPtr[i],rho_arrayPtr[i],ux_arrayPtr[i],uy_arrayPtr[i],uz_arrayPtr[i],e_arrayPtr[i],p_arrayPtr[i],bx_arrayPtr[i],by_arrayPtr[i],bz_arrayPtr[i],b1x_arrayPtr[i],b1y_arrayPtr[i],b1z_arrayPtr[i]);
    }
    printf("Last block rho:\n");
    for (i=8*n_blks-8;i<8*n_blks;i++){
      printf("%g\n",rho_arrayPtr[i]);
    }

    optional_status_variable_present=0;
    printf("read_GUMICS: read %i lines with %i variables\n",number_of_cells,variable_string_array.len);

    free(line1_1);
    free(line1_2);
    free(line1_3);
    free(line1_4);
    free(line1_5);
    free(line1_6);

    free(line2_1);
    free(line2_2);
    free(line2_3);

    free(line3_1);
    free(line3_2);
    free(line3_3);
    free(line3_4);
    free(line3_5);
    free(line3_6);
    free(line3_7);

    free(data_buffer);
    free(index_buffer);
    
    return 1;

    }



int build_gumics_grid_description( verbose_flag)
    {

    int debug_flag = 0;

    int i, j, element_count, block_count, current_amr_level, parent_level;

    int block_index_1, block_index_2;

    int parent_index, test_parent;

    int ix, iy, iz;

    int child_id_key, number_of_parents = 0, number_of_parents_old = 0;

    float eps = 1.e-5;

    float level_factor;

    float cell_delta_x[number_of_blocks];
    float cell_delta_y[number_of_blocks];
    float cell_delta_z[number_of_blocks];

    int NX = special_parameter_NX;
    int NY = special_parameter_NY;
    int NZ = special_parameter_NZ;
    int P1 = special_parameter_P1;
    int P2 = special_parameter_P2;
    int P3 = special_parameter_P3;

    int elements_per_block = special_parameter_NX * special_parameter_NY
	    * special_parameter_NZ;

    int block_space;
    float cell_min_dx=10000.;

    int ilev=0,ilev_start=0; 

    /* calculate the smallest cell size for every block...index of array
     * corresponds to block id number */

    for (block_count = 0; block_count < number_of_blocks; block_count++)
	{
	cell_delta_x[block_count] = x_position_arrayPtr[(block_count * NX * NY
		* NZ) + 1] - x_position_arrayPtr[(block_count * NX * NY * NZ)];
	cell_delta_y[block_count] = y_position_arrayPtr[(block_count * NX * NY
		* NZ) + NX] - y_position_arrayPtr[(block_count * NX * NY * NZ)];
	cell_delta_z[block_count] = z_position_arrayPtr[(block_count * NX * NY
		* NZ) + (NX * NY)] - z_position_arrayPtr[(block_count * NX * NY
		* NZ)];
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

    /*    if (debug_flag)
	  {*/
    printf(
	   "DEBUG\tsmallest cell size = %f\nnumber of cells = %d\nnumber of "
	   "blocks = %d\n",
	   smallest_cell_size,
	   number_of_cells,
	   number_of_blocks);
	/*	} */

    printf("R: %f\n",special_parameter_R);

    /*    block_space = (8 * number_of_blocks) / 7; *//* extra space needed to store
     virtual/parent blocks that are generated in program */
    /* GUMICS: some blocks are incomplete 
       estimate number by dividing exclusion sphere surface by square of smallest cell size */
    block_space = (8 * number_of_blocks) / 7;
    printf("block space (BATSRUS): %i\n",block_space);
    block_space+= 4*floor((4*3.141592654*special_parameter_R*special_parameter_R)/(smallest_cell_size*smallest_cell_size)) ; /* extra space needed to store */
    printf("block space (GUMICS): %i\n",block_space);

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

    
    if ((block_x_min = (float *) calloc(block_space, sizeof(float))) == NULL)
        {
	  printf("MALLOC for block_x_min array FAILED\n");
	  exit(EXIT_FAILURE);
	}
    if ((block_x_max = (float *) calloc(block_space, sizeof(float))) == NULL)
	{
	printf("MALLOC for block_x_max array FAILED\n");
	exit(EXIT_FAILURE);
	}
    if ((block_y_min = (float *) calloc(block_space, sizeof(float))) == NULL)
	{
	printf("MALLOC for block_y_min array FAILED\n");
	exit(EXIT_FAILURE);
	}
    if ((block_y_max = (float *) calloc(block_space, sizeof(float))) == NULL)
	{
	printf("MALLOC for block_x_max array FAILED\n");
	exit(EXIT_FAILURE);
	}
    if ((block_z_min = (float *) calloc(block_space, sizeof(float))) == NULL)
	{
	printf("MALLOC for block_z_min array FAILED\n");
	exit(EXIT_FAILURE);
	}
    if ((block_z_max = (float *) calloc(block_space, sizeof(float))) == NULL)
	{
	printf("MALLOC for block_z_max array FAILED\n");
	exit(EXIT_FAILURE);
	}

    if ((block_x_center = (float *) calloc(block_space, sizeof(float))) == NULL)
	{
	printf("MALLOC for block_x_center array FAILED\n");
	exit(EXIT_FAILURE);
	}
    if ((block_y_center = (float *) calloc(block_space, sizeof(float))) == NULL)
	{
	printf("MALLOC for block_y_center array FAILED\n");
	exit(EXIT_FAILURE);
	}
    if ((block_z_center = (float *) calloc(block_space, sizeof(float))) == NULL)
	{
	printf("MALLOC for block_z_center array FAILED\n");
	exit(EXIT_FAILURE);
	}

    if ((block_amr_levels = (float *) calloc(block_space, sizeof(float)))
	    == NULL)
	{
	printf("MALLOC for block_amr_levels array FAILED\n");
	exit(EXIT_FAILURE);
	}

    /* allocate memory for parent_id & child_count, child_id_1 ... child_id_8 */

    if ((parent_id = (int *) calloc(block_space, sizeof(int))) == NULL)
	{
	printf("MALLOC for  parent_id array FAILED\n");
	exit(EXIT_FAILURE);
	}
    if ((child_count = (int *) calloc(block_space, sizeof(int))) == NULL)
	{
	printf("MALLOC for  child_count array FAILED\n");
	exit(EXIT_FAILURE);
	}
    if ((child_id_1 = (int *) calloc(block_space, sizeof(int))) == NULL)
	{
	printf("MALLOC for  child_id_1 array FAILED\n");
	exit(EXIT_FAILURE);
	}
    if ((child_id_2 = (int *) calloc(block_space, sizeof(int))) == NULL)
	{
	printf("MALLOC for  child_id_2 array FAILED\n");
	exit(EXIT_FAILURE);
	}
    if ((child_id_3 = (int *) calloc(block_space, sizeof(int))) == NULL)
	{
	printf("MALLOC for  child_id_3 array FAILED\n");
	exit(EXIT_FAILURE);
	}
    if ((child_id_4 = (int *) calloc(block_space, sizeof(int))) == NULL)
	{
	printf("MALLOC for  child_id_4 array FAILED\n");
	exit(EXIT_FAILURE);
	}
    if ((child_id_5 = (int *) calloc(block_space, sizeof(int))) == NULL)
	{
	printf("MALLOC for  child_id_5 array FAILED\n");
	exit(EXIT_FAILURE);
	}
    if ((child_id_6 = (int *) calloc(block_space, sizeof(int))) == NULL)
	{
	printf("MALLOC for  child_id_6 array FAILED\n");
	exit(EXIT_FAILURE);
	}
    if ((child_id_7 = (int *) calloc(block_space, sizeof(int))) == NULL)
	{
	printf("MALLOC for  child_id_7 array FAILED\n");
	exit(EXIT_FAILURE);
	}
    if ((child_id_8 = (int *) calloc(block_space, sizeof(int))) == NULL)
	{
	printf("MALLOC for  child_id_8 array FAILED\n");
	exit(EXIT_FAILURE);
	}

    element_count = 0; /* set element count to zero but do not reset inside block
     traversal loop */

    /* find and populate min and max values for each block in x,y,z */

    /* for every block */

    for (block_count = 0; block_count < number_of_blocks; block_count++)
	{

	/*initialize each block_* array element to first element for corresponding
	 * block in *_position_array */

	block_x_min[block_count] = x_position_arrayPtr[element_count];
	block_x_max[block_count] = x_position_arrayPtr[element_count];
	block_y_min[block_count] = y_position_arrayPtr[element_count];
	block_y_max[block_count] = y_position_arrayPtr[element_count];
	block_z_min[block_count] = z_position_arrayPtr[element_count];
	block_z_max[block_count] = z_position_arrayPtr[element_count];

	/* for every element in current block */

	for (j = 0; j < elements_per_block; j++)
	    {
	    if (block_x_min[block_count] > x_position_arrayPtr[element_count])
		{
		block_x_min[block_count] = x_position_arrayPtr[element_count];
		}
	    if (block_x_max[block_count] < x_position_arrayPtr[element_count])
		{
		block_x_max[block_count] = x_position_arrayPtr[element_count];
		}
	    if (block_y_min[block_count] > y_position_arrayPtr[element_count])
		{
		block_y_min[block_count] = y_position_arrayPtr[element_count];
		}
	    if (block_y_max[block_count] < y_position_arrayPtr[element_count])
		{
		block_y_max[block_count] = y_position_arrayPtr[element_count];
		}
	    if (block_z_min[block_count] > z_position_arrayPtr[element_count])
		{
		block_z_min[block_count] = z_position_arrayPtr[element_count];
		}
	    if (block_z_max[block_count] < z_position_arrayPtr[element_count])
		{
		block_z_max[block_count] = z_position_arrayPtr[element_count];
		}

	    element_count++;
	    }

	}

    /* massage all blocks x,y,z min and max arrays to use the edge of the cells
     * instead of the middle of the cells */
    /*
    for (block_count = 0; block_count < number_of_blocks; block_count++)
	{
	block_x_min[block_count] = block_x_min[block_count]
		- (cell_delta_x[block_count] / 2);
	block_x_max[block_count] = block_x_max[block_count]
		+ (cell_delta_x[block_count] / 2);
	block_y_min[block_count] = block_y_min[block_count]
		- (cell_delta_y[block_count] / 2);
	block_y_max[block_count] = block_y_max[block_count]
		+ (cell_delta_y[block_count] / 2);
	block_z_min[block_count] = block_z_min[block_count]
		- (cell_delta_z[block_count] / 2);
	block_z_max[block_count] = block_z_max[block_count]
		+ (cell_delta_z[block_count] / 2);
	}
    */
    /* find all global min and max */

    /* initialize all global* variables to first corresponding element in positon
     *  array */

    global_x_min = block_x_min[0];
    global_x_max = block_x_min[0];
    global_y_min = block_y_min[0];
    global_y_max = block_y_min[0];
    global_z_min = block_z_min[0];
    global_z_max = block_z_min[0];

    for (i = 0; i < number_of_blocks; i++)
	{

	if (global_x_min > block_x_min[i])
	    {
	    global_x_min = block_x_min[i];
	    }
	if (global_x_max < block_x_max[i])
	    {
	    global_x_max = block_x_max[i];
	    }
	if (global_y_min > block_y_min[i])
	    {
	    global_y_min = block_y_min[i];
	    }
	if (global_y_max < block_y_max[i])
	    {
	    global_y_max = block_y_max[i];
	    }
	if (global_z_min > block_z_min[i])
	    {
	    global_z_min = block_z_min[i];
	    }
	if (global_z_max < block_z_max[i])
	    {
	    global_z_max = block_z_max[i];
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

    /* auto-detect P1,P2,P3 - from Lutz Rastaetter's 3DView/lib/setup_octree.c */
    if (P1 <= 0 || P2 <= 0 || P3 <= 0)
	{
	float TOLERANCE = 0.001;
	float p1, p2, p3;
	int ib;
	p1 = 100000.;
	p2 = 100000.;
	p3 = 100000.;
	fprintf(
		stderr,
		"Global xmin: %f xmax: %f\n",
		global_x_min,
		global_x_max);
	fprintf(
		stderr,
		"Global ymin: %f ymax: %f\n",
		global_y_min,
		global_y_max);
	fprintf(
		stderr,
		"Global zmin: %f zmax: %f\n",
		global_z_min,
		global_z_max);
	for (ib = 0; ib < number_of_blocks; ib++)
	    {
	    float p1_tmp, p2_tmp, p3_tmp;
	    p1_tmp = floor((global_x_max - global_x_min) / (0.9999*fabs(block_x_max[ib] - block_x_min[ib])));
	    p2_tmp = floor((global_y_max - global_y_min) / (0.9999*fabs(block_y_max[ib] - block_y_min[ib])));
	    p3_tmp = floor((global_z_max - global_z_min) / (0.9999*fabs(block_z_max[ib] - block_z_min[ib])));
	    /*	    if ((ib % 10000) == 0){
	      printf("iblock: %i p1_tmp: %f p2_tmp: %f p3_tmp: %f\n",ib,p1_tmp,p2_tmp,p3_tmp);
	    }
	    */
	    if (p1_tmp < p1)
		{
		p1 = p1_tmp;
		}
	    if (p2_tmp < p2)
		{
		p2 = p2_tmp;
		}
	    if (p3_tmp < p3)
		{
		p3 = p3_tmp;
		}
	    }
	if (fabs(p1 - floor(p1 + 0.5)) > TOLERANCE)
	    {
	    level_factor = 1. / min( ceil(p1)-p1 , p1-floor(p1) );
	    p1 *= level_factor;
	    p2 *= level_factor;
	    p3 *= level_factor;
	    }
	if (fabs(p2 - floor(p2 + 0.5)) > TOLERANCE)
	    {
	    level_factor = 1. / min( ceil(p2)-p2 , p2-floor(p2) );
	    p1 *= level_factor;
	    p2 *= level_factor;
	    p3 *= level_factor;
	    }
	if (fabs(p3 - floor(p3 + 0.5)) > TOLERANCE)
	    {
	    level_factor = 1. / min( ceil(p3)-p3 , p3-floor(p3) );
	    p1 *= level_factor;
	    p2 *= level_factor;
	    p3 *= level_factor;
	    }
	/* reduce p1,p2,p3 by powers of 2 */
	while (((p1 / 2 - floor(p1 / 2 + 0.25)) < 0.4) && ((p2 / 2 - floor(
		p2 / 2 + 0.25)) < 0.4) && ((p3 / 2 - floor(p3 / 2 + 0.25))
		< 0.4))
	    {
	    p1 /= 2;
	    p2 /= 2;
	    p3 /= 2;
	    }
	
	special_parameter_P1 = P1 = p1;
	special_parameter_P2 = P2 = p2;
	special_parameter_P3 = P3 = p3;
	fprintf(stderr, "P1: %i P2: %i P3: %i\n", P1, P2, P3);
	}

    max_amr_level = log(
			(global_x_max - global_x_min) / (special_parameter_P1 * (NX-1) /* SWMF IDL data: NX */
		    * smallest_cell_size));
    max_amr_level = max_amr_level / (log(2.0));
    max_amr_level = floor(max_amr_level + 1.5); /* add 1.5 to the real amr
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
    if ((block_at_amr_level = (int *) calloc(
	    number_of_blocks * max_amr_level,
	    sizeof(int))) == NULL)
	{
	printf("MALLOC for  block_at_amr_level array FAILED\n");
	exit(EXIT_FAILURE);
	}
    /* initialize with -1  - missing block */
    for (i=0;i<number_of_blocks * max_amr_level;i++){
      block_at_amr_level[i]=-1;
    }

    if ((number_of_blocks_at_amr_level = (int *) calloc(
	    max_amr_level,
	    sizeof(int))) == NULL)
	{
	printf("MALLOC for  number_of_blocks_at_amr_level array FAILED\n");
	exit(EXIT_FAILURE);
	}
    if ((number_of_parent_blocks_at_amr_level = (int *) calloc(
	    max_amr_level,
	    sizeof(int))) == NULL)
	{
	printf(
		"MALLOC for  number_of_parent_blocks_at_amr_level array FAILED\n");
	exit(EXIT_FAILURE);
	}

    /* calculate and populate amr level for every block */

    /* for every block */
    /*    printf("Block NX: %i\n",NX); */
    for (block_count = 0; block_count < number_of_blocks; block_count++)
	{

	block_amr_levels[block_count] = floor(
		(log(
			(global_x_max - global_x_min) / (special_parameter_P1
			 * (NX-1) * cell_delta_x[block_count]))) / log(2.0) + 0.5);
	/*		(log(
			(global_x_max - global_x_min) / (special_parameter_P1
			* NX * cell_delta_x[block_count]))) / log(2.0)); */


	}

    /* calculate and populate value for block_at_amr_level,
     * number_of_blocks_at_amr_level, and parent_id arrays for every AMR level
     * in block_amr_levels array */

    /* for every block */

    for (block_count = 0; block_count < number_of_blocks; block_count++)
	{
	current_amr_level = block_amr_levels[block_count];

	block_at_amr_level[current_amr_level * number_of_blocks
		+ number_of_blocks_at_amr_level[current_amr_level]]
		= block_count;

	parent_id[block_count] = -1;

	if (debug_flag)
	    {
	    printf(
		    "\nDEBUG\tblock_at_amr_level[ %d ] = %d\n",
		    current_amr_level * number_of_blocks
			    + number_of_blocks_at_amr_level[current_amr_level],
		    block_count);
	    printf(
		    "DEBUG\tblock_at_amr_level[ %d ] = %d\n",
		    current_amr_level * number_of_blocks
			    + number_of_blocks_at_amr_level[current_amr_level],
		    block_at_amr_level[current_amr_level * number_of_blocks
			    + number_of_blocks_at_amr_level[current_amr_level]]);
	    }

	number_of_blocks_at_amr_level[current_amr_level]++;

	}

    /* calculate and populate values for center of every block */

    /* for every block */

    for (block_count = 0; block_count < number_of_blocks; block_count++)
	{
	block_x_center[block_count] = (0.5 * (block_x_max[block_count]
		- block_x_min[block_count])) + block_x_min[block_count];
	block_y_center[block_count] = (0.5 * (block_y_max[block_count]
		- block_y_min[block_count])) + block_y_min[block_count];
	block_z_center[block_count] = (0.5 * (block_z_max[block_count]
		- block_z_min[block_count])) + block_z_min[block_count];
	}

    /* initialize values of child_ids, parent_id, and child count arrays */

    for (block_count = 0; block_count < block_space; block_count++)
	{
	child_id_1[block_count] = -1;
	child_id_2[block_count] = -1;
	child_id_3[block_count] = -1;
	child_id_4[block_count] = -1;
	child_id_5[block_count] = -1;
	child_id_6[block_count] = -1;
	child_id_7[block_count] = -1;
	child_id_8[block_count] = -1;
	parent_id[block_count] = -1;
	child_count[block_count] = 0;
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

    for (current_amr_level=0;current_amr_level < max_amr_level; current_amr_level++){
       fprintf(stderr,"Read BASTRUS: level: %i number of blocks: %i\n",current_amr_level,number_of_blocks_at_amr_level[current_amr_level]);
    }

    if (number_of_blocks_at_amr_level[0] > 0 && number_of_blocks_at_amr_level[0] < (P1*P2*P3) ){
	float dx,dy,dz; 
	int N_parents_old,num_new_blocks,N_ix,N_iy,N_iz,ix,iy,iz; 
	float XC,YC,ZC;
	int ib,iblk,nblk0;
	ilev=0;
	dx=(global_x_max-global_x_min)/P1;
	dy=(global_y_max-global_y_min)/P2;
	dz=(global_z_max-global_z_min)/P3;
	iblk=number_of_blocks;
	nblk0=number_of_blocks_at_amr_level[0];
	for (iz=0;iz<P3;iz++){
	  ZC=global_z_min+(iz+0.5)*dz;
	  for (iy=0;iy<P2;iy++){
	    YC=global_y_min+(iy+0.5)*dy;
	    for (ix=0;ix<P1;ix++){
	      int block_exists=0;
	      XC=global_x_min+(ix+0.5)*dx;
	      /* test existing blocks; */
	      for (ib=0;ib<nblk0;ib++){
		int test_block;
		test_block=block_at_amr_level[ib];
		if  ( (XC > block_x_min[test_block] && XC < block_x_max[test_block] )
		      && (YC > block_y_min[test_block]  && YC < block_y_max[test_block]  )
		      && (ZC > block_z_min[test_block] && ZC < block_z_max[test_block]  ) )
		  {
		    block_exists=1;
		  }
		if (block_exists == 0){
		  fprintf(stderr,"Settung up new block %i at AMR level 0: %i\n",iblk,number_of_blocks_at_amr_level[0]);
		  /* increment local counter (level = 0) */
		  block_at_amr_level[number_of_blocks_at_amr_level[0]]=iblk;
		  /* set new block number into block_at_AMRlevel array */
		  block_amr_levels[iblk]=ilev;
		  block_x_min[iblk]=global_x_min+ix*dx;
		  block_x_max[iblk]=global_x_min+(ix+1)*dx;
		  block_x_center[iblk]=global_x_min+(ix+0.5)*dx;
		  block_y_min[iblk]=global_y_min+iy*dy;
		  block_y_max[iblk]=global_y_min+(iy+1)*dy;
		  block_y_center[iblk]=global_y_min+(iy+0.5)*dy;
		  block_z_min[iblk]=global_z_min+iz*dz;
		  block_z_max[iblk]=global_z_min+(iz+1)*dz;
		  block_z_center[iblk]=global_z_min+(iz+0.5)*dz;
		  /* increment global counters		  */
		  iblk++;
		  number_of_parents++;
		  number_of_blocks_at_amr_level[0]=number_of_blocks_at_amr_level[0]+1;
		}
	      }
	    }
	  }
	}
	/* we need to set these for any interpolation to succeed; */
	number_of_blocks_at_amr_level[0]=P1*P2*P3;
	number_of_parent_blocks_at_amr_level[0]=P1*P2*P3;
	fprintf(stderr,"Added %i blocks to coarsest refinement level\n",number_of_parents);
	ilev_start=1; 
    }
    
   /* begin calculating virtual parent blocks starting from blocks with the
    * finest resolution down to the root blocks */

    for (
	 current_amr_level = ilev_start, level_factor=pow(2.,ilev_start); 
	 current_amr_level <= max_amr_level - 1;
	 current_amr_level++, level_factor = level_factor*2.)
     {
     /*    for (current_amr_level = 0, level_factor = 1.; current_amr_level
     //	    <= max_amr_level - 1; current_amr_level++, level_factor
     //	    = level_factor * 2.)
     //	{ */
     if (number_of_blocks_at_amr_level[current_amr_level] <= 0)
       { 
	 float dx, dy, dz; /* block size at level current_amr_level */
	 int iblk, N_parents_old, num_new_blocks, N_ix, N_iy, N_iz, ix, iy,
	   iz;
	 dx = (global_x_max - global_x_min) / (level_factor * P1);
	 dy = (global_y_max - global_y_min) / (level_factor * P2);
	 dz = (global_z_max - global_z_min) / (level_factor * P3);
	 /* setup virtual parent blocks */
	 N_ix = level_factor * P1;
	 N_iy = level_factor * P2;
	 N_iz = level_factor * P3;
	 for (iz = 0; iz < N_iz; iz++)
	   {
	     int iz_c;
	     iz_c = iz % 2;
	     for (iy = 0; iy < N_iy; iy++)
	       {
		 int iy_c;
		 iy_c = iy % 2;
		 for (ix = 0; ix < N_ix; ix++)
		   {
		     int ix_c, iv_c, parent_block, ixyz;
		     ix_c = ix % 2;
		     child_id_key = iv_c = ix_c + 2 * iy_c + 4 * iz_c;
		     ixyz = ix + N_ix * (iy + N_iy * iz);
		     iblk = number_of_blocks + number_of_parents + ixyz;
		     block_amr_levels[iblk] = current_amr_level;
		     block_x_min[iblk] = global_x_min + ix * dx;
		     block_x_max[iblk] = global_x_min + (ix + 1) * dx;
		     block_x_center[iblk] = global_x_min + (ix + 0.5) * dx;
		     block_y_min[iblk] = global_y_min + iy * dy;
		     block_y_max[iblk] = global_y_min + (iy + 1) * dy;
		     block_y_center[iblk] = global_y_min + (iy + 0.5) * dy;
		     block_z_min[iblk] = global_z_min + iz * dz;
		     block_z_max[iblk] = global_z_min + (iz + 1) * dz;
		     block_z_center[iblk] = global_z_min + (iz + 0.5) * dz;
		     if (current_amr_level > 0)
		       {
			 int iparent, ip;
			 iparent = ((ix / 2) + (N_ix / 2) * ((iy / 2)
							     + (N_iy / 2) * (iz / 2)));
			 /* update child-parent connections */
			 /*        ip=block_at_AMRlevel[(current_amr_level-1)*N_blks+iparent]; */
			 ip = block_at_amr_level[number_of_blocks
						 * (current_amr_level - 1) + iparent];
			 if (ip < 0){
			   fprintf(stderr,"Read BATSRUS: parent block not set %i %i %i\n",number_of_blocks,current_amr_level,iparent);
			   exit (1);
			 }
			 if (block_amr_levels[ip] != (current_amr_level - 1))
			   {
			     fprintf(
				     stderr,
				     "Setup Octree: ix %i N_ix: %i iy: %i N_iy: %i iz: %i N_iz: %i parent %i in level %i.\n",
				     ix,
				     N_ix,
				     iy,
				     N_iy,
				     iz,
				     N_iz,
				     iparent,
				     current_amr_level - 1);
			     fprintf(
				     stderr,
				     "Setup Octree: parent block %i in wrong refinement level %f!=%i (current_amr_level).\n",
				     ip,
				     block_amr_levels[ip],
				     current_amr_level);
			     exit(1);
			   }
			 if (parent_id[iblk] < 0)
			   {
			     parent_id[iblk] = ip;
			   }
			 /* update children */
			 if (get_child_id(ip, child_id_key) < 0)
			   {
			     put_child_id(iblk, ip, child_id_key);
			     child_count[ip]++;
			   }
			 else
			   {
			     fprintf(
				     stderr,
				     "Block ip=%i:  Child block already assigned: iv_c=%i ix_c=%i iy_c=%i iz_c=%i\n",
				     ip,
				     iv_c,
				     ix_c,
				     iy_c,
				     iz_c);
			   }
		       }
		   }
	       }
	   }
	 /* add blocks to total number of blocks  */
	 num_new_blocks = N_ix * N_iy * N_iz;
	 number_of_parent_blocks_at_amr_level[current_amr_level]
	   = num_new_blocks;
	 fprintf(
		 stderr,
		 "Setup_octree: adding %i blocks to block_at_amrlevel\n %i\n",
		 num_new_blocks,
		 current_amr_level);
	 for (i = 0; i <= num_new_blocks - 1; i++)
	   {
#ifdef DEBUG
	     if (current_amr_level == 7 && i < 10)
	       fprintf(
		       stderr,
		       "current_amr_level: %i i: %i current_amr_level*N_blks+i: %i N_blks+number_of_parents+i: %i\n",
		       current_amr_level,
		       i,
		       current_amr_level * number_of_blocks + i,
		       number_of_blocks + number_of_parents + i);
#endif
	     block_at_amr_level[current_amr_level * number_of_blocks + i]
	       = number_of_blocks + number_of_parents + i;
	   }
	 number_of_parents_old = number_of_parents;
	 number_of_parents += num_new_blocks;
	 number_of_blocks_at_amr_level[current_amr_level] = num_new_blocks;
       }
	else
	    {
	    int iblk;
	    for (iblk = 0; iblk
		    < number_of_blocks_at_amr_level[current_amr_level]; iblk++)
		{
		int ib, parent_block, ichild, ix_c, iy_c, iz_c;
		float xc, yc, zc;
		ib = block_at_amr_level[current_amr_level * number_of_blocks
			+ iblk];
		xc = block_x_center[ib]; /* octree_blocklist[ib].XCenter; */
		yc = block_y_center[ib]; /* octree_blocklist[ib].YCenter; */
		zc = block_z_center[ib]; /* octree_blocklist[ib].ZCenter; */
		parent_block = setup_parent(
			ib,
			current_amr_level - 1,
			global_x_min,
			global_y_min,
			global_z_min,
			number_of_blocks,
			block_space-number_of_blocks,
			&number_of_parents);
		if (parent_block < 0)
		    {
		    /* error encountered */
		    exit(1);
		    }
		/* establish connections */
		ix_c = xc > block_x_center[parent_block]; /*octree_blocklist[parent_block].XCenter; */
		iy_c = yc > block_y_center[parent_block]; /*octree_blocklist[parent_block].YCenter; */
		iz_c = zc > block_z_center[parent_block]; /*octree_blocklist[parent_block].ZCenter; */
		ichild = ix_c + 2 * iy_c + 4 * iz_c;
		/*    if (octree_blocklist[parent_block].child_IDs[ichild] < 0){
		 octree_blocklist[parent_block].child_IDs[ichild]=ib;
		 octree_blocklist[parent_block].child_count++;
		 }
		 octree_blocklist[ib].parent_ID=parent_block; */
		if (get_child_id(parent_block, ichild) < 0)
		    {
		    put_child_id(ib, parent_block, ichild);
		    child_count[parent_block]++;
		    }
		else
		    {
		    fprintf(stderr, "Child_id already assigned: %i\n", ichild);
		    fprintf(
			    stderr,
			    "Ilev: %i Block: %i Parent: %i\n",
			    current_amr_level,
			    ib,
			    parent_block);
		    fprintf(
			    stderr,
			    "Parent XC: %f YC: %f ZC: %f\n",
			    block_x_center[parent_block],
			    block_y_center[parent_block],
			    block_z_center[parent_block]);
		    fprintf(
			    stderr,
			    "Block XC: %f YC: %f ZC: %f\n",
			    block_x_center[ib],
			    block_y_center[ib],
			    block_z_center[ib]);
		    fprintf(stderr, "IX: %i IY: %i IZ: %i\n", ix_c, iy_c, iz_c);
		    fprintf(
			    stderr,
			    "Child IDs assigned: %i %i %i %i %i %i %i %i\nChiuld count: %i",
			    child_id_1[parent_block],
			    child_id_2[parent_block],
			    child_id_3[parent_block],
			    child_id_4[parent_block],
			    child_id_5[parent_block],
			    child_id_6[parent_block],
			    child_id_7[parent_block],
			    child_id_8[parent_block],
			    child_count[parent_block]);
		    exit(1);
		    }
		}
	    }
	fprintf(
		stderr,
		"Setup_Octree::new_algorithm: level: %i of %f level_factor: %f\n",
		current_amr_level,
		max_amr_level - 1,
		level_factor);
	fprintf(
		stderr,
		" N_Parents: Total: %i Max: %i\n",
		number_of_parents,
		block_space-number_of_blocks );
	}

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
   
