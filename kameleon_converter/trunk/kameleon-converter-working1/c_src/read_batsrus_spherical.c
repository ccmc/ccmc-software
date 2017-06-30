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
 *    2008 June 30            Rastaetter, Lutz                                 *
 *                            determination of NX,NY,NZ in block and of        *
 *                            special parameter P1, P2, P3, also               *
 *                            implemeted fast and more robust setup of grid    *
 *                            Added macros min(),max() and utilities unique(), *
 *                            get_child_id(),put_child_id(), setup_parent()    *
 *                            and climb_octree().                              *
 *                                                                             *
 *    2008 July 18            Rastaetter, Lutz                                 *
 *                            Added dipole tile calculation functionality      *
 *                            using cxform created by E. Santiago, R. Boller   *
 *                            http://nssdcftp.gsfc.nasa.gov/selected_software/ *
 *                            coordinate_transform/                            *
 *                                                                             *
 *    2008 August 08          Maddox, Marlo                                    *
 *                            Changed MAX_FIRST_RECORD from 200 -> 32767       *
 *
 *    2009 April 09           Lutz Rastaetter                                  *
 *                            Changed MAX_FIRST_RECORD to 65535                *
 *                            flip endianness when the record_size<0 is found  *
 *                 							       *
 *    08.30.2011   	      Maddox, Marlo				       *
 *    		   	      Adding new "status" variable to batsrus          *
 *    04.24.2012              Lutz Rastaetter                                  *
 *                            Adding blocks to lowest refinement level to make *
 *                          P1*P2*P3 blocks if there are more than zero blocks *
 ******************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "read_batsrus.h"
#include "endian_flip.h"
#include "strsplit.h"
#include "read_database.h"
#include "cdf.h"

#define max(a,b) ( (a)>(b) ? (a) : (b) )
#define min(a,b) ( (a)<(b) ? (a) : (b) )

#define MAX_FIRST_RECORD        65535  /** Changed from 200 -> 32767  **/

#define DEBUG_FLAG 1
/* debug level for setup_parent() */
#define DEBUG 1


int build_spherical_grid_description(int verbose_flag);
/********************** record 6 ***************/
float *r_position_arrayPtr;
float *th_position_arrayPtr;
float *phi_position_arrayPtr;

extern int errno;
#include "cxform.h"

/*******************************************************************************
 *                                       MAIN                                  *
 ******************************************************************************/

int calculate_spherical_coordinates(int verbose_flag){

	int status_code;

	printf("  calculating spherical coordinates!\n");

    if ((r_position_arrayPtr = calloc(number_of_cells, sizeof(float))) == NULL)
	{
	printf("MALLOC for r_position_arrayPtr array FAILED\n");
	exit(EXIT_FAILURE);
	}
	else{
	printf("  MALLOC for r_position_arrayPtr array succesfull!\n");
	}

    if ((th_position_arrayPtr = calloc(number_of_cells, sizeof(float))) == NULL)
	{
	printf("MALLOC for th_position_arrayPtr array FAILED\n");
	exit(EXIT_FAILURE);
	}
	else{
	printf("  MALLOC for th_position_arrayPtr array succesfull!\n");
	}
    if ((phi_position_arrayPtr = calloc(number_of_cells, sizeof(float))) == NULL)
	{
	printf("MALLOC for phi_position_arrayPtr array FAILED\n");
	exit(EXIT_FAILURE);
	}
	else{
	printf("  MALLOC for phi_position_arrayPtr array succesfull!\n");
	}
	
	printf("  copying x position..\n");
	int i;
	for (i = 0; i < number_of_cells; i++)
	{
      r_position_arrayPtr[i] = sqrt(pow(x_position_arrayPtr[i],2) + pow(y_position_arrayPtr[i],2) + pow(z_position_arrayPtr[i],2));
      th_position_arrayPtr[i] = acos(z_position_arrayPtr[i]/r_position_arrayPtr[i]);
      phi_position_arrayPtr[i] = atan2(y_position_arrayPtr[i], x_position_arrayPtr[i]);
	}
	fprintf(stderr, "  maths:\n   sqrt(4)=%f\n   2^2=%f\n   acos(-1)=%f\n   atan2(1,0)=%f\n  log10(100)=%f\n", sqrt(4), pow(2,2), acos(-1), atan2(1,0), log10(100));


	printf("WARNING: Attempting hack to place spherical coordinates in X,Y,Z arrays\n");
	fprintf(stderr, "log10(r)\ttheta\tphi\n");
	for (i = 0; i < number_of_cells; i++)
	{
		x_position_arrayPtr[i] = log10(r_position_arrayPtr[i]);
		y_position_arrayPtr[i] = phi_position_arrayPtr[i];
		z_position_arrayPtr[i] = acos(-1)/2 - th_position_arrayPtr[i];
		
		if (i <= 100)
		{
			fprintf(stderr, "%f\t%f\t%f\n", x_position_arrayPtr[i], y_position_arrayPtr[i], z_position_arrayPtr[i]);
		}
	}
	status_code = 1; /* set to 0 to halt, set to 1 to proceed */
	return status_code;

}

int read_batsrus_spherical(char *input_file_name, int verbose_flag, int minmax_flag)
    {

    extern int linear_minmax_search(float *, int, float *, float *);

    int status_code;

    if (verbose_flag)
	{
	printf("...reading %s\n", input_file_name);
	}

    if ((input_filePtr = fopen(input_file_name, "rb")) == NULL)
	{
	printf("ERROR Opening File \n");
	perror(input_file_name);
	/* should return control if open was unsucessful */
	return EXIT_FAILURE;
	}
    else
	{
	/*************** Read Record 1  *********************/

	status_code = read_record_1(verbose_flag);

	if (!status_code)
	    {
	    return EXIT_FAILURE;
	    }

	/* if first read record fails, chances are this file is not a valid
	 BATRUS output file return control to main read routine */

	/*** 06/08/2006 lets check the return status for each record ****/

	/*************** Read Record 2  *********************/

	status_code = read_record_2(verbose_flag);

	if (!status_code)
	    {
	    return EXIT_FAILURE;
	    }

	/*************** Read Record 3  *********************/

	status_code = read_record_3(verbose_flag);

	if (!status_code)
	    {
	    return EXIT_FAILURE;
	    }

	/*************** Read Record 4  *********************/

	status_code = read_record_4(verbose_flag);

	if (!status_code)
	    {
	    return EXIT_FAILURE;
	    }

	/*************** Read Record 5  *********************/

	status_code = read_record_5(verbose_flag);

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

	/*************** Read Record 6  - sets the x,y,z_position_arrayPtr *********************/ 
	status_code = read_record_6(verbose_flag);

	if (!status_code)
	    {
	    return EXIT_FAILURE;
	    }


	/*************** Read Record 7  *********************/
	status_code = read_record_7(verbose_flag);

	if (!status_code)
	    {
	    return EXIT_FAILURE;
	    }

	}

    fclose(input_filePtr);

	status_code = calculate_spherical_coordinates(verbose_flag);

	if (!status_code)
	    {
	    return EXIT_FAILURE;
	    }


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
	fprintf(stderr,
		"Read_batsrus after read: Special parameters: NX: %f NY: %f NZ: %f\n",
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

    status_code = build_spherical_grid_description(verbose_flag);

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

    /* return 1 if all reads were succesfull. otherwise return 0 */

    return status_code;
    }

int build_spherical_grid_description( verbose_flag)
    {

    int debug_flag = 1;

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

    int ilev=0,ilev_start=0; 

    block_space = (8 * number_of_blocks) / 7; /* extra space needed to store
     virtual/parent blocks that are generated in program */

    if (verbose_flag)
	{
	fprintf(stderr, "\nbuilding spherical grid description..........\n");
	fprintf(stderr, "number of original blocks =\t%d\n", number_of_blocks);
	fprintf(stderr, "original blocks plus virtual =\t%d\n", block_space);
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

    if (debug_flag)
    {
		printf(
		"DEBUG\tinitial smallest cell size = %f\n", smallest_cell_size);
    }

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
	if (smallest_cell_size < 0){
	printf(
		"Error: \tsmallest cell size = %f\n  Exiting!\n",
		smallest_cell_size);
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
	fprintf(stderr,
		"\nXMIN =\t%f\nXMAX =\t%f\nYMIN =\t%f\nYMAX =\t%f\nZMIN =\t%f\nZMAX "
		    "=\t%f\n",
		global_x_min,
		global_x_max,
		global_y_min,
		global_y_max,
		global_z_min,
		global_z_max);
	}

    /* calculate the maximum AMR level */

    /* auto-detect P1,P2,P3 - from Lutz Rastaetter's 3DView/lib/setup_octree.c */
    if (verbose_flag)
    {
        fprintf(stderr, "P1,P2,P3: %d, %d, %d", P1,P2,P3);
    }
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
		    p1_tmp = (global_x_max - global_x_min) / (block_x_max[ib]
			    - block_x_min[ib]);
		    p2_tmp = (global_y_max - global_y_min) / (block_y_max[ib]
			    - block_y_min[ib]);
		    p3_tmp = (global_z_max - global_z_min) / (block_z_max[ib]
			    - block_z_min[ib]);
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
	    (global_x_max - global_x_min) / (special_parameter_P1 * NX
		    * smallest_cell_size));
    max_amr_level = max_amr_level / (log(2.0));
    max_amr_level = floor(max_amr_level + 1.5); /* add 1.5 to the real amr
     level, see Lutz ... subtract 1 before returning program control to main*/

    if (verbose_flag)
	{
	fprintf(stderr, "Calculated max_amr_level = %f\n", max_amr_level);
	fprintf(stderr, "  global_x_max = %f\n  global_x_min = %f\n  special_parameter_P1 = %f\n  smallest_cell_size = %f\n", global_x_max, global_x_min, special_parameter_P1, smallest_cell_size);
	}

    if (debug_flag)
	{
	fprintf(stderr, 
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
	fprintf(stderr, "MALLOC for  block_at_amr_level array FAILED\n");
	exit(EXIT_FAILURE);
	}
    if ((number_of_blocks_at_amr_level = (int *) calloc(
	    max_amr_level,
	    sizeof(int))) == NULL)
	{
	fprintf(stderr,"MALLOC for  number_of_blocks_at_amr_level array FAILED\n");
	exit(EXIT_FAILURE);
	}
    if ((number_of_parent_blocks_at_amr_level = (int *) calloc(
	    max_amr_level,
	    sizeof(int))) == NULL)
	{
	fprintf(stderr,
		"MALLOC for  number_of_parent_blocks_at_amr_level array FAILED\n");
	exit(EXIT_FAILURE);
	}

    /* calculate and populate amr level for every block */

    /* for every block */

	float temp_amr_level;
	float temp_cell_ratio;

	

    for (block_count = 0; block_count < number_of_blocks; block_count++)
	{

		temp_cell_ratio = (global_x_max - global_x_min) / (special_parameter_P1
										* NX * cell_delta_x[block_count]);
		

		temp_amr_level = ceil( log(temp_cell_ratio) / log(2.0) + 0.5 ) - 1;
		if (temp_amr_level >= max_amr_level)
		{
			fprintf(stderr, "WARNING! amr level too high!, exiting");

		}


		block_amr_levels[block_count] = temp_amr_level;

/*		if (debug_flag)
		{
			fprintf(stderr, "global_x_max: %f, global_x_min: %f, cell_delta_x: %f  ratio: %f amr_refinement_factor:%f  ", 
				global_x_max, global_x_min, cell_delta_x[block_count],temp_cell_ratio, temp_amr_level);

			fprintf(stderr, "block_amr_levels[%d]=%f\n", block_count, block_amr_levels[block_count]);
		}*/
	}
	

    /* calculate and populate value for block_at_amr_level,
     * number_of_blocks_at_amr_level, and parent_id arrays for every AMR level
     * in block_amr_levels array */

    /* for every block */
     fprintf(stderr, "adding number of blocks to each amr_level\n");
     int block_at_amr_level_index;

    for (block_count = 0; block_count < number_of_blocks; block_count++)
	{
		current_amr_level = block_amr_levels[block_count];
		
		block_at_amr_level_index = current_amr_level * number_of_blocks + number_of_blocks_at_amr_level[current_amr_level];
		 
/*		if (block_at_amr_level_index >= number_of_blocks * max_amr_level)
		{
			fprintf(stderr, "WARNING! block_at_amr_level_index > %d", number_of_blocks * max_amr_level);

		}*/
		

/*		if (debug_flag)
		    {
		    fprintf(stderr,
			    "\nDEBUG\tblock_at_amr_level[ %d ] = %d\n",
			    current_amr_level * number_of_blocks
				    + number_of_blocks_at_amr_level[current_amr_level],
			    block_count);
		    fprintf(stderr,
			    "DEBUG\tblock_at_amr_level[ %d ] = %d\n",
			    current_amr_level * number_of_blocks
				    + number_of_blocks_at_amr_level[current_amr_level],
			    block_at_amr_level[current_amr_level * number_of_blocks
				    + number_of_blocks_at_amr_level[current_amr_level]]);
		     }*/

		block_at_amr_level[block_at_amr_level_index] = block_count;
		parent_id[block_count] = -1;
		number_of_blocks_at_amr_level[current_amr_level]++;

	}

	if (debug_flag)
	{
		fprintf(stderr, "added number of blocks to each amr_level\n");
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
    fprintf(stderr, "initialize values of child_ids, parent_id, and child count arrays \n");

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
	fprintf(stderr,"Number of blocks:\t%d\n", number_of_blocks);

	for (i = 0; i < max_amr_level; i++)
	    {
	    fprintf(stderr,
		    "\nnumber_of_blocks_at_amr_level[%d] = \t%d\n",
		    i,
		    number_of_blocks_at_amr_level[i]);
	    }
	}
	fprintf(stderr, "\t\t\tP1: %i P2: %i P3: %i\n", P1, P2, P3);

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
		  fprintf(stderr,"Setting up new block %i at AMR level 0: %i\n",iblk,number_of_blocks_at_amr_level[0]);
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
			number_of_blocks / 7,
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
			    "Child IDs assigned: %i %i %i %i %i %i %i %i\nChiuld count: %i\n",
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
		number_of_blocks / 7);
	}

    if (verbose_flag)
	{
	fprintf(stderr, "\nGRID STRUCTURE SUCCESSFULLY GENERATED\n\nsummary:\n");
	fprintf(stderr, "Number of parents:\t%d\n", number_of_parents);

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
