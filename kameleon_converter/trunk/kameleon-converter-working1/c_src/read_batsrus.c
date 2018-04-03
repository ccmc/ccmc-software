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

#define DEBUG_FLAG 0
/* debug level for setup_parent() */
#define DEBUG 0

/* wrap those repeated switch statements into function and subroutine */
int get_child_id(int iblock, int ichild)
    {
    int child_id;
    switch (ichild)
	{
    case 0:
	child_id = child_id_1[iblock];
	break;
    case 1:
	child_id = child_id_2[iblock];
	break;
    case 2:
	child_id = child_id_3[iblock];
	break;
    case 3:
	child_id = child_id_4[iblock];
	break;
    case 4:
	child_id = child_id_5[iblock];
	break;
    case 5:
	child_id = child_id_6[iblock];
	break;
    case 6:
	child_id = child_id_7[iblock];
	break;
    case 7:
	child_id = child_id_8[iblock];
	break;
    default:
	fprintf(stderr, "get_child_id: invalid ichild: %i\n", ichild);
	child_id = -1;
	break;
	}
    return (child_id);
    }

void put_child_id(int child_id, int iblock, int ichild)
    {
    switch (ichild)
	{
    case 0:
	child_id_1[iblock] = child_id;
	break;
    case 1:
	child_id_2[iblock] = child_id;
	break;
    case 2:
	child_id_3[iblock] = child_id;
	break;
    case 3:
	child_id_4[iblock] = child_id;
	break;
    case 4:
	child_id_5[iblock] = child_id;
	break;
    case 5:
	child_id_6[iblock] = child_id;
	break;
    case 6:
	child_id_7[iblock] = child_id;
	break;
    case 7:
	child_id_8[iblock] = child_id;
	break;
    default:
	fprintf(stderr, "put_child_id: invalid ichild: %i\n", ichild);
	break;
	}
    }

/* needed for faster setup of octree grid streucture  in build_grid_description() */
int climb_octree(int root, float x, float y, float z, int max_level)
    {
    int ix, iy, iz;
    int debug = 0;
    int child_id;

    /* if (octree_blocklist[root].child_count != 8) {return(root);}
     // new: provide for termination of climb at predefined refinement level
     // (parent block search)
     // max_level<0 will allow infinite number or climbs! */
    if (max_level == 0 || child_count[root] == 0)
	{
	return (root);
	}

    ix = x > block_x_center[root];
    iy = y > block_y_center[root];
    iz = z > block_z_center[root];
    /*ix=x > octree_blocklist[root].XCenter;
     iy=y > octree_blocklist[root].YCenter;
     iz=z > octree_blocklist[root].ZCenter; */

    if (debug)
	{
	fprintf(
		stderr,
		"Climbing tree at root: %i \nXMIN: %f XMAX: %f X: %f IX: %i\nYMIN: %f YMAX: %f Y: %f IY: %i\nZMIN: %f ZMAX: %f Z: %f IZ: %i\n",
		root,
		block_x_min[root],
		block_x_max[root],
		x,
		ix,
		block_y_min[root],
		block_y_max[root],
		y,
		iy,
		block_z_min[root],
		block_z_max[root],
		z,
		iz
	/*                octree_blocklist[root].XMIN,octree_blocklist[root].XMAX,x,ix,
	 octree_blocklist[root].YMIN,octree_blocklist[root].YMAX,y,iy,
	 octree_blocklist[root].ZMIN,octree_blocklist[root].ZMAX,z,iz */
	);
	}
    /* recursion? */
    child_id = get_child_id(root, ix + 2 * iy + 4 * iz);
    /*   child_id=octree_blocklist[root]child_IDs[ix+2*iy+4*iz]; */
    if (child_id < 0)
	{
	/* child_id=-1: end here */
	return (child_id);
	}
    else
	{
	/* child_id >=0: continue climb */
	return (climb_octree(child_id, x, y, z, max_level - 1));
	}
    }

/* setup_paret() is called from build_grid_description() */
int setup_parent(
	int iblock,
	int ilev,
	float XMIN,
	float YMIN,
	float ZMIN,
	int N_blks,
	int N_parents_max,
	int *N_parents)
    {

    float xc, yc, zc, dx_root, dy_root, dz_root;
    int root, level_factor, ix, iy, iz, ixyz, jlev;
    xc = block_x_center[iblock]; /* octree_blocklist[iblock].XCenter; */
    yc = block_y_center[iblock]; /* octree_blocklist[iblock].YCenter;*/
    zc = block_z_center[iblock]; /* octree_blocklist[iblock].ZCenter;*/

    /* get size of root block */
    /*   root = block_at_amr_level[0];
    if (DEBUG >= 3)
	{
	fprintf(stderr, "Entering Setup_Parent(): root block: %i\n", root);
	}
    dx_root = block_x_max[root] - block_x_min[root];
    dy_root = block_y_max[root] - block_y_min[root];
    dz_root = block_z_max[root] - block_z_min[root];
    ix = floor((xc - XMIN) / dx_root);
    iy = floor((yc - YMIN) / dy_root);
    iz = floor((zc - ZMIN) / dz_root);
    ixyz = floor(
	    0.001 + ix + special_parameter_P1
		    * (iy + special_parameter_P2 * iz));
    root = block_at_amr_level[ixyz];
*/
    root=-1;
    int iroot=0;
    while (root < 0 && iroot < number_of_blocks_at_amr_level[0]){
      int root_tmp;
      root_tmp=block_at_amr_level[iroot];
      if (   block_x_min[root_tmp] < xc &&  block_x_max[root_tmp] > xc
	     && block_y_min[root_tmp] < yc &&  block_y_max[root_tmp] > yc
	     && block_z_min[root_tmp] < zc &&  block_z_max[root_tmp] > zc){
	root=root_tmp;
	iroot=number_of_blocks_at_amr_level[0];
      }
      iroot++;
    }

    jlev = 0;
    if (DEBUG >= 2)
	{
	fprintf(
		stderr,
		"Setup Parent: xc %f yc %f zc %f\np1: %f p2: %f p3: %f\n",
		xc,
		yc,
		zc,
		special_parameter_P1,
		special_parameter_P2,
		special_parameter_P3);
	fprintf(
		stderr,
		"Setup Parent: Root dx %f dy %f dz %f\n",
		dx_root,
		dy_root,
		dz_root);
	fprintf(
		stderr,
		"Setup Parent: ix %i iy %i iz %i ixyz: %i\n",
		ix,
		iy,
		iz,
		ixyz);
	}
    while (jlev < ilev)
	{
	int ichild, ix_c, iy_c, iz_c;
	jlev++;
	ichild = climb_octree(root, xc, yc, zc, 1); /* only go one level */
	if (DEBUG >= 1)
	    {
	    fprintf(
		    stderr,
		    "climb_octree(root=%i,xc=%f,yc=%f,zc=%f,1)\n",
		    root,
		    xc,
		    yc,
		    zc);
	    fprintf(stderr, "jlev: %i child: %i\n", jlev, ichild);
	    }
	if (ichild < 0 || ichild == root)
	    {
	    /* create child block here and add to parent we came from
	     // the child block IDs will NOT be consecutive for a given parent */
	    ichild = N_blks + (*N_parents);
	    if (DEBUG >= 1)
		{
		fprintf(
			stderr,
			"Setup_Parent: jlev: %i N_blks %i N_parents %i ",
			jlev,
			N_blks,
			(*N_parents));
		}
	    /* test available memory allocation */
	    if ((*N_parents) >= N_parents_max)
		{
		fprintf(
			stderr,
			"setup_parent: running out of memory - returning");
		return (-2);
		}
	    block_at_amr_level[jlev * N_blks
		    + number_of_parent_blocks_at_amr_level[jlev]] = ichild;
	    number_of_parent_blocks_at_amr_level[jlev]
		    = number_of_parent_blocks_at_amr_level[jlev] + 1;
	    N_parents[0] = N_parents[0] + 1;
	    ix_c = xc > block_x_center[root]; /*xc>octree_blocklist[root].XCenter; */
	    iy_c = yc > block_y_center[root]; /*yc>octree_blocklist[root].YCenter; */
	    iz_c = zc > block_z_center[root]; /*zc>octree_blocklist[root].ZCenter; */

	    /*	    if (get_child_id(root, ix_c + 2 * iy_c + 4 * iz_c) == -1)
		    { */
		put_child_id(ichild, root, ix_c + 2 * iy_c + 4 * iz_c);
		child_count[root]++;
		/*		}*/
	    block_amr_levels[ichild] = jlev;
	    parent_id[ichild] = root;
	    /*
	     octree_blocklist[root].child_IDs[ix_c+2*iy_c+4*iz_c]=ichild;
	     octree_blocklist[root].child_count++;
	     octree_blocklist[ichild].refinement_level=jlev;
	     octree_blocklist[ichild].parent_ID=root; */

	    if (ix_c == 0)
		{
		block_x_min[ichild] = block_x_min[root];
		block_x_max[ichild] = block_x_center[root];
		/*    octree_blocklist[ichild].XMIN=octree_blocklist[root].XMIN;
		 octree_blocklist[ichild].XMAX=octree_blocklist[root].XCenter; */
		}
	    else
		{
		block_x_min[ichild] = block_x_center[root];
		block_x_max[ichild] = block_x_max[root];
		/*    octree_blocklist[ichild].XMIN=octree_blocklist[root].XCenter;
		 octree_blocklist[ichild].XMAX=octree_blocklist[root].XMAX; */
		}
	    if (iy_c == 0)
		{
		block_y_min[ichild] = block_y_min[root];
		block_y_max[ichild] = block_y_center[root];
		/*    octree_blocklist[ichild].YMIN=octree_blocklist[root].YMIN;
		 octree_blocklist[ichild].YMAX=octree_blocklist[root].YCenter; */
		}
	    else
		{
		block_y_min[ichild] = block_y_center[root];
		block_y_max[ichild] = block_y_max[root];
		/*    octree_blocklist[ichild].YMIN=octree_blocklist[root].YCenter;
		 octree_blocklist[ichild].YMAX=octree_blocklist[root].YMAX; */
		}
	    if (iz_c == 0)
		{
		block_z_min[ichild] = block_z_min[root];
		block_z_max[ichild] = block_z_center[root];
		/*octree_blocklist[ichild].ZMIN=octree_blocklist[root].ZMIN;
		 octree_blocklist[ichild].ZMAX=octree_blocklist[root].ZCenter; */
		}
	    else
		{
		block_z_min[ichild] = block_z_center[root];
		block_z_max[ichild] = block_z_max[root];
		/* octree_blocklist[ichild].ZMIN=octree_blocklist[root].ZCenter;
		 octree_blocklist[ichild].ZMAX=octree_blocklist[root].ZMAX; */
		}
	    block_x_center[ichild]
		    = (block_x_min[ichild] + block_x_max[ichild]) / 2.;
	    block_y_center[ichild]
		    = (block_y_min[ichild] + block_y_max[ichild]) / 2.;
	    block_z_center[ichild]
		    = (block_z_min[ichild] + block_z_max[ichild]) / 2.;
	    /*      octree_blocklist[ichild].XCenter=
	     (octree_blocklist[ichild].XMIN+octree_blocklist[ichild].XMAX)/2.;
	     octree_blocklist[ichild].YCenter=
	     (octree_blocklist[ichild].YMIN+octree_blocklist[ichild].YMAX)/2.;
	     octree_blocklist[ichild].ZCenter=
	     (octree_blocklist[ichild].ZMIN+octree_blocklist[ichild].ZMAX)/2.; */
	    if (DEBUG >= 1)
		{
		fprintf(
			stderr,
			"Setup_Parent: ilev: %i Root block: %i\nNew child block: jlev: %i ID: %i\n",
			ilev,
			root,
			jlev,
			ichild);
		}
	    }
	root = ichild;
	}
    if (DEBUG >= 3)
	{
	fprintf(stderr, "Exiting Setup_Parent(): Returning root: %i\n", root);
	}
    return (root);
    }

/* utility function neeeded to determine special_parameter_NZ */
/* added by Lutz Rastaetter, June 9, 2008 */
int unique(
	float *input_array,
	int input_array_len,
	int stride,
	int **output_index_array)
    {
    /* built after IDL's uniq() function */
    /* return position indices of values of input_array that differ from
     previous element in the input array */
    int test_index = 0, n_unique = 1;

    if (input_array_len <= 0)
	{
	fprintf(stderr, "%s: input array length negative or zero\n", __FILE__);
	return (0);
	}
    if (stride <= 0)
	{
	stride = 1;
	}

    if ((*output_index_array = (int*) calloc(
	    (stride > 1) + (input_array_len / stride),
	    sizeof(int))) == NULL)
	{
	fprintf(stderr, "%s: memory allocation failed\n", __FILE__);
	return (-1);
	}
    test_index = stride;
    while (test_index < input_array_len)
	{
	if (input_array[test_index] != input_array[test_index - stride])
	    {
	    (*output_index_array)[n_unique] = test_index / stride;
	    n_unique++;
	    }
	test_index += stride;
	}
    return (n_unique);
    }

int read_record_1(int verbose_flag);
int read_record_2(int verbose_flag);
int read_record_3(int verbose_flag);
int read_record_4(int verbose_flag);
int read_record_5(int verbose_flag);
int read_record_6(int verbose_flag);
int read_record_7(int verbose_flag);
int build_grid_description(int verbose_flag);
int endian_flip(char *array, long unit_len, long n_units);
int four_bytes = 4, eight_bytes = 8, one_element = 1;
float *special_parameter_array;

extern int errno;
#include "cxform.h"

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

	/*************** Read Record 6  *********************/
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

    status_code = build_grid_description(verbose_flag);

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

int read_record_1( verbose_flag)
    {

    int record_size, a, n = 0;

    /*** get the number of bytes in record 1 ( these are 1 byte caracters that
     * specify the units for model variables)*/

    fread(&byte_value, four_bytes, one_element, input_filePtr);
    record_size = byte_value;

    if (ferror(input_filePtr) || feof(input_filePtr))
	{
	printf("ERROR with fread must have returned non zero status_code for "
	    "file stream\n");
	perror("input_filePtr");
	return 0;
	}

    if (verbose_flag)
	{
	printf(
		"Record_1: found record_size: %d MAX: %d\n",
		record_size,
		MAX_FIRST_RECORD);
	}

    if (verbose_flag)
	printf("testing endianess of input file...\n");

    flip_endian = ((record_size < 0) || (record_size > MAX_FIRST_RECORD));

    if (verbose_flag)
	printf("setting flip_endian flag = %d\n", flip_endian);

    if (flip_endian)
	endian_flip((char *) &record_size, four_bytes, one_element);

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

    unit_string = (char*) malloc((record_size + 1) * sizeof(char));

    n = fread(unit_string, 1, record_size, input_filePtr);

    unit_string[n] = '\0';

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

    fread(&byte_value_2, four_bytes, one_element, input_filePtr);

    if (ferror(input_filePtr) || feof(input_filePtr))
	{
	printf(
		"ERROR with fread must have returned non zero status_code for file "
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

int read_record_2( verbose_flag)
    {
    int record_size, number_elements;
    double elapsed_time_seconds_double, *tmp_double;

    /*** get the number of bytes in record 2 ( these are five 4 byte integers
     * that specify how current model was run)*/
    fread(&byte_value, four_bytes, one_element, input_filePtr);

    if (flip_endian)
	endian_flip((char*) &byte_value, four_bytes, one_element);

    record_size = byte_value;
    number_elements = record_size / four_bytes;
    is_double = (number_elements == 6);

    fread(&current_iteration_step, four_bytes, one_element, input_filePtr);

    if (flip_endian)
	endian_flip((char*) &current_iteration_step, four_bytes, one_element);

    if (is_double)
	{
	fread(
		&elapsed_time_seconds_double,
		eight_bytes,
		one_element,
		input_filePtr);
	if (flip_endian)
	    endian_flip(
		    (char *) &elapsed_time_seconds_double,
		    eight_bytes,
		    one_element);
	elapsed_time_seconds = elapsed_time_seconds_double;
	}
    else
	{
	fread(&elapsed_time_seconds, four_bytes, one_element, input_filePtr);
	if (flip_endian)
	    endian_flip((char *) &elapsed_time_seconds, four_bytes, one_element);
	}

    fread(&number_dimensions, four_bytes, one_element, input_filePtr);

    if (flip_endian)
	endian_flip((char *) &number_dimensions, four_bytes, one_element);
    fread(&number_special_parameters, four_bytes, one_element, input_filePtr);
    if (flip_endian)
	endian_flip(
		(char *) &number_special_parameters,
		four_bytes,
		one_element);

    fread(&number_plot_variables, four_bytes, one_element, input_filePtr);
    if (flip_endian)
	endian_flip((char *) &number_plot_variables, four_bytes, one_element);

    fread(&byte_value_2, four_bytes, one_element, input_filePtr);
    if (flip_endian)
	endian_flip((char *) &byte_value_2, four_bytes, one_element);

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

int read_record_3( verbose_flag)
    {

    int record_size, number_elements;

    /*** get the number of bytes in record 3 ( there are three 4byte integers
     * for x,y,&z dimension sizes) */

    fread(&byte_value, four_bytes, one_element, input_filePtr);
    if (flip_endian)
	endian_flip((char *) &byte_value, four_bytes, one_element);
    record_size = byte_value;
    number_elements = record_size / four_bytes;

    if (verbose_flag)
	printf("Read record_3: Flip_endian: %d Is double precision: %d Number "
	    "elements: %d\n", flip_endian, is_double, number_elements);

    fread(&x_dimension_size, four_bytes, one_element, input_filePtr);
    if (flip_endian)
	endian_flip((char *) &x_dimension_size, four_bytes, one_element);
    fread(&y_dimension_size, four_bytes, one_element, input_filePtr);
    if (flip_endian)
	endian_flip((char *) &y_dimension_size, four_bytes, one_element);
    fread(&z_dimension_size, four_bytes, one_element, input_filePtr);
    if (flip_endian)
	endian_flip((char *) &z_dimension_size, four_bytes, one_element);
    if (verbose_flag)
	printf(
		"Read record_3: Dimension sizes: %d %d %d\n",
		x_dimension_size,
		y_dimension_size,
		z_dimension_size);

    fread(&byte_value_2, four_bytes, one_element, input_filePtr);

    if (flip_endian)
	endian_flip((char *) &byte_value_2, four_bytes, one_element);

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

int read_record_4( verbose_flag)
    {

    int i, record_size, number_elements;
    double *dtmp;

    if (DEBUG_FLAG)
	printf(
		"DEBUG\t from read_record_4 - Dimension sizes: %d %d %d\n",
		x_dimension_size,
		y_dimension_size,
		z_dimension_size);

    /*** get the number of bytes in record 4 ( there are ten 4byte integers for x,y,&z dimension sizes) */

    fread(&byte_value, four_bytes, one_element, input_filePtr);
    if (flip_endian)
	endian_flip((char *) &byte_value, four_bytes, one_element);
    record_size = byte_value;
    number_elements = record_size / ((1 + is_double) * four_bytes);

    if (DEBUG_FLAG)
	printf(
		"DEBUG\t from read_record_4\trecord_size = %d\tnnumber_of_elements "
		    "= %d\n",
		record_size,
		number_elements);

    if (number_elements != number_special_parameters)
	{
	fprintf(stderr, "Number of parameters read (%d) does not match record "
	    "length (%d)!\n", number_elements, number_special_parameters);
	exit(EXIT_FAILURE);
	}

    if (verbose_flag)
	printf("Read_record_4: number_elements: %d\n", number_elements);

    if (number_elements != number_special_parameters)
	{
	exit(EXIT_FAILURE);
	}

    special_parameter_array = (float*) malloc(
	    number_special_parameters * sizeof(float));

    /*if (is_double) <--DUPLICATE */

    if (is_double)
	{
	dtmp = (double*) malloc(number_special_parameters * sizeof(double));
	fread(dtmp, eight_bytes, number_special_parameters, input_filePtr);

	if (flip_endian)
	    endian_flip((char *) dtmp, eight_bytes, number_special_parameters);

	for (i = 0L; i < number_special_parameters; i++)
	    special_parameter_array[i] = dtmp[i];
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
		    (char *) special_parameter_array,
		    four_bytes,
		    number_special_parameters);
	}

    fread(&byte_value_2, four_bytes, one_element, input_filePtr);
    if (flip_endian)
	endian_flip((char *) &byte_value_2, four_bytes, one_element);

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

int read_record_5( verbose_flag)
    {
    int i, i2, record_size, number_of_elements;

    /* flags to determine if special grid parameters exist in the .out
     * file. If not, code checks to see if the read_database_info function
     * has already retrieved the values ***/

    int NX_set = 0, NY_set = 0, NZ_set = 0;
    int P1_set = 0, P2_set = 0, P3_set = 0;
    int th_set = 0;

    strarr tmp_strarr;

    /* initialize - or we'll get a SEGMENTATION FAULT/BUS ERROR */
    tmp_strarr.len = 0;
    tmp_strarr.sarr = NULL;

    /*** get the number of bytes in record 5 ( these are 1 byte characters that
     * specify the models variable names)*/
    fread(&byte_value, four_bytes, one_element, input_filePtr);
    if (flip_endian)
	endian_flip((char *) &byte_value, four_bytes, one_element);
    record_size = byte_value;

    /** we know the record size, now allocate space for the char
     * *variable_name_string variable **/

    variable_name_string = (char*) malloc((record_size + 1) * sizeof(char));

    number_of_elements = fread(
	    variable_name_string,
	    1,
	    record_size,
	    input_filePtr);

    if (DEBUG_FLAG)
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

    for (i = 0, i2 = tmp_strarr.len - number_special_parameters; i
	    < number_special_parameters; i++, i2++)
	{
	if (verbose_flag)
	    printf(
		    "parsing special parameters: %d variable: %s\n",
		    i,
		    tmp_strarr.sarr[i2]);

	if (strcmp(tmp_strarr.sarr[i2], "g") == 0)
	    special_parameter_g = special_parameter_array[i];
	if (strcmp(tmp_strarr.sarr[i2], "c") == 0)
	    special_parameter_c = special_parameter_array[i];
	if (strcmp(tmp_strarr.sarr[i2], "th") == 0)
	    {
	    special_parameter_th = special_parameter_array[i];
	    th_set = 1;
	    }
	if (strcmp(tmp_strarr.sarr[i2], "p1") == 0)
	    {
	    special_parameter_P1 = special_parameter_array[i];
	    P1_set = 1;
	    }
	if (strcmp(tmp_strarr.sarr[i2], "p2") == 0)
	    {
	    special_parameter_P2 = special_parameter_array[i];
	    P2_set = 1;
	    }
	if (strcmp(tmp_strarr.sarr[i2], "p3") == 0)
	    {
	    special_parameter_P3 = special_parameter_array[i];
	    P3_set = 1;
	    }
	if (strcmp(tmp_strarr.sarr[i2], "NX") == 0)
	    {
	    special_parameter_NX = special_parameter_array[i];
	    NX_set = 1;
	    }
	if (strcmp(tmp_strarr.sarr[i2], "NY") == 0)
	    {
	    special_parameter_NY = special_parameter_array[i];
	    NY_set = 1;
	    }
	if (strcmp(tmp_strarr.sarr[i2], "NZ") == 0)
	    {
	    special_parameter_NZ = special_parameter_array[i];
	    NZ_set = 1;
	    }
	if (strcmp(tmp_strarr.sarr[i2], "R") == 0 || strcmp(
		tmp_strarr.sarr[i2],
		"rbody") == 0)
	    special_parameter_R = special_parameter_array[i];
	}

    /* inspect for sanity */

    /****** NX, NY, & NZ is crucial for block count calculations, if not set see
     * if database info has them ********/

    /*if .out doesn't contain NX, check see if DatabaseInfo file set the value */

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
	    return_status = cxform("SM", "GSM", cxform_et, v_in, v_out);
	    special_parameter_th = atan2(v_out[2], v_out[0]); /* *180./3.14159264; */
	    }
	}

    if (!NX_set && batsrus_special_parameter_NX_exists_in_databaseinfo)
	{
	special_parameter_NX = atof(batsrus_special_parameter_NX);
	if (verbose_flag)
	    {
	    printf(
		    "setting special parameter NX from DatabaseInfo file:\t%f\n",
		    special_parameter_NX);
	    }
	}
    else
	if (!NX_set && !batsrus_special_parameter_NX_exists_in_databaseinfo)
	    {
	    fprintf(
		    stderr,
		    "!!!! %s\tWARNING: special parameter NX could not be "
			"located.  Will attempt to compute parameter\n"
			"# batsrus_special_parameter_NX later...\n",
		    __FILE__);
	    /*      fprintf( stderr, "!!!! %s\tERROR: special parameter NX could not be "
	     "located.  Set key value in DatabaseInfo file to proceed...\nie. 6 "
	     "# batsrus_special_parameter_NX\n", __FILE__ );
	     return 0; */
	    }
    if (!NY_set && batsrus_special_parameter_NY_exists_in_databaseinfo)
	{
	special_parameter_NY = atof(batsrus_special_parameter_NY);

	if (verbose_flag)
	    {
	    printf(
		    "setting special parameter NY from DatabaseInfo file:\t%f\n",
		    special_parameter_NY);
	    }
	}
    else
	if (!NY_set && !batsrus_special_parameter_NY_exists_in_databaseinfo)
	    {
	    fprintf(
		    stderr,
		    "!!!! %s\tWARNING: special parameter NY could not be "
			"located.  Will attempt to compute parameter\n"
			"# batsrus_special_parameter_NY later ...\n",
		    __FILE__);
	    /*      fprintf( stderr, "!!!! %s\tERROR: special parameter NY could not be "
	     "located.  Set key value in DatabaseInfo file to proceed...\nie. "
	     "6 # batsrus_special_parameter_NY\n", __FILE__ );
	     return 0; */
	    }
    if (!NZ_set && batsrus_special_parameter_NZ_exists_in_databaseinfo)
	{
	special_parameter_NZ = atof(batsrus_special_parameter_NZ);
	if (verbose_flag)
	    {
	    printf(
		    "setting special parameter NZ from DatabaseInfo file:\t%f\n",
		    special_parameter_NZ);
	    }
	}
    else
	if (!NZ_set && !batsrus_special_parameter_NZ_exists_in_databaseinfo)
	    {
	    fprintf(
		    stderr,
		    "!!!! %s\tWARNING: special parameter NZ could not be "
			"located.  Will attempt to compute parameter\n"
			"# batsrus_special_parameter_NZ later ...\n",
		    __FILE__);
	    /*      fprintf( stderr, "!!!! %s\tERROR: special parameter NZ could not be "
	     "located.  Set key value in DatabaseInfo file to proceed...\nie. 6 "
	     "# batsrus_special_parameter_NZ\n", __FILE__ );
	     return 0; */
	    }

    /****** P1, P2, & P3 is crucial for block count calculations, if not set see
     * if database info has them ********/

    /* if .out doesn't contain P1, check see if DatabaseInfo file set the value */

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
    free_strarr(&tmp_strarr);

    fread(&byte_value_2, four_bytes, one_element, input_filePtr);
    if (flip_endian)
	endian_flip((char *) &byte_value_2, four_bytes, one_element);

    if (byte_value == byte_value_2)
	{
	if (verbose_flag)
	    {
	    printf("READ Record 5 SUCCESSFUL\n");
	    }
	return 1;
	}
    else
	{
	printf("ERROR reading input file\n");
	exit(EXIT_FAILURE);
	}
    }

int read_record_6( verbose_flag)
    {
    long int i;
    long int record_size, number_elements;

    if (is_double)
	{
	if ((tmp_double = calloc(3 * number_of_cells, sizeof(double))) == NULL)
	    {
	    printf("MALLOC for x_position array FAILED\n");
	    exit(EXIT_FAILURE);
	    }
	}

    if ((x_position_arrayPtr = calloc(number_of_cells, sizeof(float))) == NULL)
	{
	printf("MALLOC for x_position array FAILED\n");
	exit(EXIT_FAILURE);
	}
    if ((y_position_arrayPtr = calloc(number_of_cells, sizeof(float))) == NULL)
	{
	printf("MALLOC for y_position array FAILED\n");
	exit(EXIT_FAILURE);
	}
    if ((z_position_arrayPtr = calloc(number_of_cells, sizeof(float))) == NULL)
	{
	printf("MALLOC for z_position array FAILED\n");
	exit(EXIT_FAILURE);
	}

    /*** get the number of bytes in record 6 ( the number of bytes in record
     * 6 = number_of_cells * 3dimensions * 4bytes ) */

    fread(&byte_value, four_bytes, one_element, input_filePtr);
    if (flip_endian)
	endian_flip((char *) &byte_value, four_bytes, one_element);
    record_size = byte_value;

    if (is_double)
	{
	number_elements = record_size / eight_bytes;
	}
    else
	{
	number_elements = record_size / four_bytes;
	}

    if (DEBUG_FLAG)
	printf(
		"DEBUG\t 3 * number_of_cells = 3 * %d = %d\n",
		number_of_cells,
		3 * number_of_cells);

    if (number_elements != (3 * number_of_cells))
	{
	printf("Record 6 inconsistent with expectation\n");
	printf(
		"Record_size: %ld Number_elements: %ld Cells %d\n",
		record_size,
		number_elements,
		3 * number_of_cells);
	exit(EXIT_FAILURE);
	}

    if (is_double)
	{
	fread(tmp_double, eight_bytes, 3 * number_of_cells, input_filePtr);
	if (flip_endian)
	    endian_flip((char *) tmp_double, eight_bytes, 3 * number_of_cells);

	for (i = 0L; i < number_of_cells; i++)
	    {
	    x_position_arrayPtr[i] = tmp_double[i];
	    y_position_arrayPtr[i] = tmp_double[i + number_of_cells];
	    z_position_arrayPtr[i] = tmp_double[i + 2 * number_of_cells];
	    }

	}
    else
	{
	fread(x_position_arrayPtr, four_bytes, number_of_cells, input_filePtr);
	if (flip_endian)
	    endian_flip(
		    (char*) x_position_arrayPtr,
		    four_bytes,
		    number_of_cells);
	fread(y_position_arrayPtr, four_bytes, number_of_cells, input_filePtr);
	if (flip_endian)
	    endian_flip(
		    (char*) y_position_arrayPtr,
		    four_bytes,
		    number_of_cells);
	fread(z_position_arrayPtr, four_bytes, number_of_cells, input_filePtr);
	if (flip_endian)
	    endian_flip(
		    (char*) z_position_arrayPtr,
		    four_bytes,
		    number_of_cells);
	}

    fread(&byte_value_2, four_bytes, one_element, input_filePtr);
    if (flip_endian)
	endian_flip((char *) &byte_value_2, four_bytes, one_element);

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

int read_record_7( verbose_flag)
    {

    int read_batsrus_variable(FILE *, float *, int, int, double *);

    int i, read_failed_flag = 0;
    int record_size, number_elements;
    strarr tmp_strarr;

    /* initialize - or we'll get a SEGMENTATION FAULT/BUS ERROR */
    tmp_strarr.len = 0;
    tmp_strarr.sarr = NULL;

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

    if ((rho_arrayPtr = (float *) calloc(number_of_cells, sizeof(float)))
	    == NULL)
	{
	printf("MALLOC for rho array FAILED\n");
	exit(EXIT_FAILURE);
	}
    if ((ux_arrayPtr = (float *) calloc(number_of_cells, sizeof(float)))
	    == NULL)
	{
	printf("MALLOC for ux array FAILED\n");
	exit(EXIT_FAILURE);
	}
    if ((uy_arrayPtr = (float *) calloc(number_of_cells, sizeof(float)))
	    == NULL)
	{
	printf("MALLOC for uy array FAILED\n");
	exit(EXIT_FAILURE);
	}
    if ((uz_arrayPtr = (float *) calloc(number_of_cells, sizeof(float)))
	    == NULL)
	{
	printf("MALLOC for uz array FAILED\n");
	exit(EXIT_FAILURE);
	}
    if ((bx_arrayPtr = (float *) calloc(number_of_cells, sizeof(float)))
	    == NULL)
	{
	printf("MALLOC for bx array FAILED\n");
	exit(EXIT_FAILURE);
	}
    if ((by_arrayPtr = (float *) calloc(number_of_cells, sizeof(float)))
	    == NULL)
	{
	printf("MALLOC for by array FAILED\n");
	exit(EXIT_FAILURE);
	}
    if ((bz_arrayPtr = (float *) calloc(number_of_cells, sizeof(float)))
	    == NULL)
	{
	printf("MALLOC for bz array FAILED\n");
	exit(EXIT_FAILURE);
	}
    if ((b1x_arrayPtr = (float *) calloc(number_of_cells, sizeof(float)))
	    == NULL)
	{
	printf("MALLOC for blx array FAILED\n");
	exit(EXIT_FAILURE);
	}
    if ((b1y_arrayPtr = (float *) calloc(number_of_cells, sizeof(float)))
	    == NULL)
	{
	printf("MALLOC for bly array FAILED\n");
	exit(EXIT_FAILURE);
	}
    if ((b1z_arrayPtr = (float *) calloc(number_of_cells, sizeof(float)))
	    == NULL)
	{
	printf("MALLOC for blz array FAILED\n");
	exit(EXIT_FAILURE);
	}
    if ((p_arrayPtr = (float *) calloc(number_of_cells, sizeof(float))) == NULL)
	{
	printf("MALLOC for p array FAILED\n");
	exit(EXIT_FAILURE);
	}
    if ((e_arrayPtr = (float *) calloc(number_of_cells, sizeof(float))) == NULL)
	{
	printf("MALLOC for e array FAILED\n");
	exit(EXIT_FAILURE);
	}
    if ((jx_arrayPtr = (float *) calloc(number_of_cells, sizeof(float)))
	    == NULL)
	{
	printf("MALLOC for jx array FAILED\n");
	exit(EXIT_FAILURE);
	}
    if ((jy_arrayPtr = (float *) calloc(number_of_cells, sizeof(float)))
	    == NULL)
	{
	printf("MALLOC for jy array FAILED\n");
	exit(EXIT_FAILURE);
	}
    if ((jz_arrayPtr = (float *) calloc(number_of_cells, sizeof(float)))
	    == NULL)
	{
	printf("MALLOC for jz array FAILED\n");
	exit(EXIT_FAILURE);
	}

    /* NEW STATUS VARIABLE setup */

    if ((status_arrayPtr = (float *) calloc(number_of_cells, sizeof(float)))
	    == NULL)
	{
	printf("MALLOC for status array FAILED\n");
	exit(EXIT_FAILURE);
	}

    /* read data: start at variable name 3 (0: 'X', 1: 'Y', 2: 'Z' have been
     * read before) */

    for (i = 3L; i < tmp_strarr.len - number_special_parameters; i++)
	{
	if (verbose_flag)
	    {
	    printf(
		    "reading variable %d of %ld: %s\n",
		    i + 1,
		    tmp_strarr.len - number_special_parameters,
		    tmp_strarr.sarr[i]);
	    }

	/******************** get all rho variables ****************************/

	if (strcmp(tmp_strarr.sarr[i], "rho") == 0)
	    {
	    read_failed_flag += read_batsrus_variable(
		    input_filePtr,
		    rho_arrayPtr,
		    flip_endian,
		    is_double,
		    tmp_double);
	    }

	/******************** get all ux variables ****************************/

	if (strcmp(tmp_strarr.sarr[i], "ux") == 0)
	    {
	    read_failed_flag += read_batsrus_variable(
		    input_filePtr,
		    ux_arrayPtr,
		    flip_endian,
		    is_double,
		    tmp_double);
	    }

	/******************** get all uy variables ****************************/

	if (strcmp(tmp_strarr.sarr[i], "uy") == 0)
	    {
	    read_failed_flag += read_batsrus_variable(
		    input_filePtr,
		    uy_arrayPtr,
		    flip_endian,
		    is_double,
		    tmp_double);
	    }

	/******************** get all uz variables ****************************/

	if (strcmp(tmp_strarr.sarr[i], "uz") == 0)
	    {
	    read_failed_flag += read_batsrus_variable(
		    input_filePtr,
		    uz_arrayPtr,
		    flip_endian,
		    is_double,
		    tmp_double);
	    }

	/******************** get all bx variables ****************************/

	if (strcmp(tmp_strarr.sarr[i], "bx") == 0)
	    {
	    read_failed_flag += read_batsrus_variable(
		    input_filePtr,
		    bx_arrayPtr,
		    flip_endian,
		    is_double,
		    tmp_double);
	    }

	/******************** get all by variables ****************************/

	if (strcmp(tmp_strarr.sarr[i], "by") == 0)
	    {
	    read_failed_flag += read_batsrus_variable(
		    input_filePtr,
		    by_arrayPtr,
		    flip_endian,
		    is_double,
		    tmp_double);
	    }

	/******************** get all bz variables ****************************/

	if (strcmp(tmp_strarr.sarr[i], "bz") == 0)
	    {
	    read_failed_flag += read_batsrus_variable(
		    input_filePtr,
		    bz_arrayPtr,
		    flip_endian,
		    is_double,
		    tmp_double);
	    }

	/******************** get all b1x variables ****************************/

	if (strcmp(tmp_strarr.sarr[i], "b1x") == 0)
	    {
	    read_failed_flag += read_batsrus_variable(
		    input_filePtr,
		    b1x_arrayPtr,
		    flip_endian,
		    is_double,
		    tmp_double);
	    }

	/******************** get all b1y variables ****************************/

	if (strcmp(tmp_strarr.sarr[i], "b1y") == 0)
	    {
	    read_failed_flag += read_batsrus_variable(
		    input_filePtr,
		    b1y_arrayPtr,
		    flip_endian,
		    is_double,
		    tmp_double);
	    }

	/******************** get all b1z variables ****************************/

	if (strcmp(tmp_strarr.sarr[i], "b1z") == 0)
	    {
	    read_failed_flag += read_batsrus_variable(
		    input_filePtr,
		    b1z_arrayPtr,
		    flip_endian,
		    is_double,
		    tmp_double);
	    }

	/******************** get all jx variables ****************************/

	if (strcmp(tmp_strarr.sarr[i], "jx") == 0)
	    {
	    read_failed_flag += read_batsrus_variable(
		    input_filePtr,
		    jx_arrayPtr,
		    flip_endian,
		    is_double,
		    tmp_double);
	    }

	/******************** get all jy variables ****************************/

	if (strcmp(tmp_strarr.sarr[i], "jy") == 0)
	    {
	    read_failed_flag += read_batsrus_variable(
		    input_filePtr,
		    jy_arrayPtr,
		    flip_endian,
		    is_double,
		    tmp_double);
	    }

	/******************** get all jz variables ****************************/

	if (strcmp(tmp_strarr.sarr[i], "jz") == 0)
	    {
	    read_failed_flag += read_batsrus_variable(
		    input_filePtr,
		    jz_arrayPtr,
		    flip_endian,
		    is_double,
		    tmp_double);
	    }

	/******************** get all e variables ****************************/

	if (strcmp(tmp_strarr.sarr[i], "e") == 0)
	    {
	    read_failed_flag += read_batsrus_variable(
		    input_filePtr,
		    e_arrayPtr,
		    flip_endian,
		    is_double,
		    tmp_double);
	    }

	/******************** get all p variables ****************************/

	if (strcmp(tmp_strarr.sarr[i], "p") == 0)
	    {
	    read_failed_flag += read_batsrus_variable(
		    input_filePtr,
		    p_arrayPtr,
		    flip_endian,
		    is_double,
		    tmp_double);
	    }

	/******************** get all status variables ****************************/

	if (strcmp(tmp_strarr.sarr[i], "status") == 0)
	    {
	    read_failed_flag += read_batsrus_variable(
		    input_filePtr,
		    status_arrayPtr,
		    flip_endian,
		    is_double,
		    tmp_double);

	    optional_status_variable_present = 1;
	    }
	else
	    {
	    optional_status_variable_present = 0;
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

int build_grid_description( verbose_flag)
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

    int ilev=0,ilev_start=0; 

    block_space = (8 * number_of_blocks) / 7; /* extra space needed to store
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

    for (block_count = 0; block_count < number_of_blocks; block_count++)
	{

	block_amr_levels[block_count] = floor(
		(log(
			(global_x_max - global_x_min) / (special_parameter_P1
				* NX * cell_delta_x[block_count]))) / log(2.0) + 0.5);
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
		number_of_blocks / 7);
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

/******************* Lutz's read_batsrus_variable *****************************/

int read_batsrus_variable(
	FILE *input_filePtr,
	float *variable_array,
	int flip_endian,
	int is_double,
	double *tmp_double)
    {
    int byte_value, record_size, number_elements, four_bytes = 4, eight_bytes =
	    8, one_element = 1;
    long int i, read_failed_flag = 0;

    fread(&byte_value, four_bytes, one_element, input_filePtr);
    if (flip_endian)
	endian_flip((char*) &byte_value, four_bytes, one_element);
    record_size = byte_value;
    if (is_double)
	{
	number_elements = record_size / eight_bytes;
	fread(tmp_double, eight_bytes, number_elements, input_filePtr);
	if (flip_endian)
	    endian_flip((char *) tmp_double, eight_bytes, number_elements);
	for (i = 0L; i < number_elements; i++)
	    variable_array[i] = tmp_double[i];
	}
    else
	{
	number_elements = record_size / four_bytes;
	fread(variable_array, four_bytes, number_elements, input_filePtr);
	if (flip_endian)
	    endian_flip((char*) variable_array, four_bytes, number_elements);
	}
    fread(&byte_value_2, four_bytes, one_element, input_filePtr);
    if (flip_endian)
	endian_flip((char*) &byte_value_2, four_bytes, one_element);
    if ((byte_value != byte_value_2) || (number_elements != number_of_cells))
	{
	read_failed_flag = 1;
	}

    return (read_failed_flag);
    }
