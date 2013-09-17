/*******************************************************************************
 *                                                                             *
 *    NAME:          read_ctip.c                                               *
 *                                                                             *
 *    VERSION:       0.1                                                       *
 *                                                                             *
 *    AUTHOR:        Lutz Rastaetter                                           *
 *                   CUA/CCMC/NASA-GSFC (612.3)                                *
 *                   lr@waipio.gsfc.nasa.gov                                   *
 *                   (301) 286 - 1085                                          *
 *                                                                             *
 *    PURPOSE:       Custom read routine for CTIP 3D model output.             *
 *                                                                             *
 *    INPUTS:        char *input_file_name, char *aux_file, int verbose_flag,  *
 *                   int minmax_flag                                           *
 *                                                                             *
 *    OUTPUTS:       calls to ccmc_structure_manager that produce standard     *
 *                   attribute & variable structures that are processed by     *
 *                   write_structure_2_* routine                               *
 *                                                                             *
 *    FUNCTIONS:     a few                                                     *
 *                                                                             *
 *    VARIABLES:     a few more                                                *
 *                                                                             *
 *    MODIFICATION                                                             *
 *    HISTORY:                                                                 *
 *                                                                             *
 *    03.23.2005     Rastaetter, Lutz                                          *
 *                   initial development started                               *
 *                                                                             *
 *    04.04.2005     Maddox, Marlo                                             *
 *                   inserting calls to ccmc_structure_manager to populate     *
 *                   variable and attribute structures                         *
 *                                                                             *
 *    08.11.2005     Maddox, Marlo                                             *
 *                   Changed value in put_ccmc_attribute call for              *
 *                   grid_system_2_dimension_1_size from ctip_NZ to            *
 *                   ctip_number_of_cells which is ctip_NX*ctip_NY*ctip_NZ     *
 *                                                                             *
 *    01.10.2006     Maddox, Marlo                                             *
 *                   During the actual setting of local variable attribute     *
 *                   values, is_vector_component was being set to yes|no vs    *
 *                   1|0 - fixed                                               *
 *                                                                             *
 *                   also had to change is_vector_component from char to int   *
 *                                                                             *
 *   10.05.2006      Maddox, Marlo                                             *
 *                   Fixed return values in read_ctip_header_file() function   *
 *                   initialized error_flag = 0 and function now returns either*
 *                   EXIT_SUCCESS or EXIT_FAILURE                              *
 *                                                                             *
 *                   Fixed bug in actual read_ctip function where strstr was   *
 *                   searching ctip_unit_strarr.sarr[*] for V*_Lon & V*_Lat    *
 *                   when it should have been looking for V*_lon & V*_lat      *
 *                   essentially lowercase l vs uppercase L was causing some of*
 *                   the V components to not be found...                       *
 *                                                                             *
 *   02.06.2007      Maddox, Marlo                                             *
 *                   changing ccmc_metadata_structure.h to                     *
 *                   ccmc_structure_manager.h                                  *
 *                                                                             *
 *   05.17.2007      Maddox, Marlo                                             *
 *                   adding update for kameleon_version gattribute             *
 *                                                                             *
 *   05.22.2007      Maddox, Marlo                                             *
 *                   was referencing wrong attribute_structure list for change *
 *                   above, fixed to local_ccmc_attribute_structure_list       *
 *                                                                             *
 *                   Also added appropriate return variables for               *
 *                   update_ccmc_attribute_value function call                 *
 ******************************************************************************/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>

#include "read_ctip.h"
#include "endian_flip.h"
#include "strsplit.h"

#include "ccmc_attributes.h"
#include "ccmc_registered_variables.h"
#include "ccmc_structure_manager.h"

#include "kameleon_version_info.h"

#define MAX_FIRST_RECORD        100

#define DEBUG_FLAG 0

int read_ctip_header_file(char *ctip_header_file, int verbose_flag);
int read_ctip_record(
      FILE *input_filePtr,
      int verbose_flag,
      long int expected_size,
      char *tmp_double);

/*int build_grid_description( int verbose_flag ); */
float *special_parameter_array;

extern int errno;

int read_ctip_header_file(char *ctip_header_file, int verbose_flag)
{
   FILE *headerfile;
   int error_flag = 0;
   ctip_NX=-1;
   ctip_NY=-1;
   ctip_NZ=-1;
   if ((headerfile=fopen(ctip_header_file, "r")) == NULL)
   {
      printf("Could not open %s\n", ctip_header_file);
      return (EXIT_FAILURE);
   }
   if (fgets(ctip_headerline, CTIP_MAX_HEADER_STRING, headerfile) == NULL)
      error_flag=1;

   /***** IS THIS A DUPLICATE LINE *******/

   if (fgets(ctip_headerline, CTIP_MAX_HEADER_STRING, headerfile) == NULL)
      error_flag=1;

   if (sscanf(ctip_headerline, "%i %i %i", &ctip_NZ, &ctip_NX, &ctip_NY) !=3)
   {
      fprintf(stderr,"Read of grid size parameters was incomplete!\n");
      return (EXIT_FAILURE);
   }
   ctip_number_of_cells=ctip_NX*ctip_NY*ctip_NZ;

   if (verbose_flag)
   {
      printf(
            "CTIP_headerline: %s\nNX: %d NY: %d NZ: %d number_of_cells: %d\n",
            ctip_headerline,
            ctip_NX,
            ctip_NY,
            ctip_NZ,
            ctip_number_of_cells);
   }
   if (fgets(ctip_unit_string, CTIP_MAX_UNITS_STRING, headerfile) == NULL)
      error_flag=1;
   fclose(headerfile);
   if (verbose_flag)
   {
      printf(
            "Read CTIP header: %d %d %d\nVariables+units: %s\nUnits: %s\n",
            ctip_NX,
            ctip_NY,
            ctip_NZ,
            ctip_headerline,
            ctip_unit_string);
   }

   if ( DEBUG_FLAG)
      printf("%s\nverbose_flag = %d\nerror_flag = %d\nEXIT_SUCCESS = %d\n"
         "EXIT_FAILURE = %d\n",
      __FILE__,verbose_flag,error_flag,EXIT_SUCCESS,EXIT_FAILURE);

      if( error_flag )
      {  return EXIT_FAILURE;
   }
   else
   {
      return EXIT_SUCCESS;
   }

}

/*******************************************************************************
 *                                       MAIN                                  *
 ******************************************************************************/
int read_ctip(
      char *input_file_name,
      char *aux_file,
      int verbose_flag,
      int minmax_flag)
{

   extern int update_ccmc_variable_attribute_value(
         char *,
         char *,
         void *,
         struct ccmc_variable_structure *);
   extern int linear_minmax_search(float *, int, float *, float *);

   /* This is the second option for creating standard attribute & variable structures.  */
   /* The user supplies the structures that are then baselined before writing        */
   /* to cdf/hdf5, etc....  Therofore, let the user instantiate his/her own copy of    */
   /* both ccmc_attribute & ccmc_variable structure lists                                */

   static struct ccmc_attribute_structure *local_ccmc_attribute_structure_list;
   static struct ccmc_variable_structure *local_ccmc_variable_structure_list;

   int status_code, i, j, k, nvar, nvar2;
   char *tmp_2d_array, *tmp_4d_array;
   float *data_ptr;
   int number_dims=3, number_grids=2;
   int found, number_variable_attributes=15;
   float mask;
   int is_vector_component;
   char name[20], units[30], si_units[30], units_idl[50], si_units_idl[50],
         description[500], alt_name[20], name_idl[50], grid_system[100],
         data_grid_system[100], position_grid_system[100];
   float valid_min, valid_max, actual_min, actual_max, unit_conversion_factor;
   float PI=2.*asin(1.);

   /*** mmaddox m***/
   float zero = 0.0, one_eighty = 180.0, three_sixty = 360.0, ctip_missing;
   int stat;
   char *kameleon_version_num= KAMELEON_VERSION;

   /** we also need local_valid_min for z since were using z to represent
    * pressure level, leave the deafualt value for z valid_max @ 100000 **/

   float local_valid_min = 1.0;

   strarr ctip_unit_strarr;

   ctip_unit_strarr.len=0;
   ctip_unit_strarr.sarr=NULL;

   if (verbose_flag)
   {
      printf("...reading %s\n", input_file_name);
   }

   if (read_ctip_header_file(aux_file, verbose_flag) == EXIT_FAILURE)
   {
      exit(EXIT_FAILURE);
   }

   if ( DEBUG_FLAG)
      printf("DEBUG\tback from read_ctip_header\n");

   if ( (ctip_input_filePtr = fopen(input_file_name, "rb") ) == NULL)
   {
      printf("ERROR Opening File \n");
      perror(input_file_name);
      /* should return control if open was unsucessful */
      return 0;
   }

   /****************************************************************************
    *                                CREATE THE GLOBAL ATTRIBUTES              *
    ***************************************************************************/

   /****************************************************************************
    *    initialize the local_ccmc_attribute_structure_list to hold n number of*
    *    cccmc_attribute_structures where n = NUMBER_GLOBAL_ATTR =             *
    *    NUMBER_CCMC_ATTR + 5                                                  *
    *    as defined in ccmc_attributes.h                                       *
    *                                                                          *
    *   There will be a traditional ctip_attributes.h file that will contain   *
    *   NUMBER_CTIP_ATTRIBUTES which would be 5.  And an array of structures   *
    *   containing ctip attribute name and data type values for normal         *
    *   automation                                                             *
    ***************************************************************************/

   /**** this should actualy be +6 because we need to specify grid_2_type ***/

   local_ccmc_attribute_structure_list
         = init_ccmc_attribute_structure( NUMBER_CCMC_ATTR + 5);

   /* next use the put_ccmc_attribute() routine supplied via the
    * ccmc_metadata_structure.c code this routine will create all the ccmc
    * attributes listed in ccmc_attributes.h file via the structure list to
    * ensure uniformity the arguments to put_ccmc_attribute are char
    * *attribute_name, attribute classification ie global || model, char
    * *data_type, value "" ( since we will set the actual values later using the
    * update_ccmc_attribute_value() function), and the particular
    * attribute_structure to add these to which could be one or more
    * local_structs or the predefined baselined_attribute_structure_list */

   /*** for each name-data type pair in the ccmc_attribute_name_type_list, call
    * put_ccmc_attribute with attr name & data type since we are writing from
    * the ccmc_attribute_name_type_list, the attribute_type = global ***/

   /* this is just to demonstrate how the default ccmc attributes and values
    * would normally be created. In this instance they could have been created
    * and populated at the same time but for demo purposes we'll create and
    * populate with defaults and then update specific attribute values by name*/

   for (i=0; i < NUMBER_CCMC_ATTR; i++)
   {
      put_ccmc_attribute(
            ccmc_attribute_name_type_list[i].ccmc_attr_name,
            "global",
            ccmc_attribute_name_type_list[i].ccmc_attr_data_type,
            "",
            local_ccmc_attribute_structure_list);
   }

   if ( DEBUG_FLAG)
      printf("DEBUG\t just put default ccmc attributes "
         "from ccmc_atribute_name_type_list...\n");

   /**** these would normally be defined in the ctip_attributes.h file and
    * exported through ctip_attributes_name_type_list but for this example
    * we'll put these in manually since we already have the names and values
    * here ***/

   put_ccmc_attribute(
         "grid_system_2_number_of_dimensions",
         "model",
         "int",
         &number_dims,
         local_ccmc_attribute_structure_list);
   put_ccmc_attribute(
         "grid_system_2_dimension_1_size",
         "model",
         "int",
         &ctip_number_of_cells,
         local_ccmc_attribute_structure_list);
   put_ccmc_attribute(
         "grid_system_2_dimension_2_size",
         "model",
         "int",
         &ctip_NX,
         local_ccmc_attribute_structure_list);
   put_ccmc_attribute(
         "grid_system_2_dimension_3_size",
         "model",
         "int",
         &ctip_NY,
         local_ccmc_attribute_structure_list);
   put_ccmc_attribute(
         "grid_system_2",
         "model",
         "char",
         "H, X, Y",
         local_ccmc_attribute_structure_list);

   /*** !!!!!!!!! add grid_2_type here !!!!!!!!!! ***/

   /*** now lets update the attribute values with values we have using
    * update_ccmc_attribute_value***/
   /*** this updates by attribute name and specified structure */

   stat = update_ccmc_attribute_value("README",
   CTIP_MODEL_DESCRIPTION, local_ccmc_attribute_structure_list);
   stat = update_ccmc_attribute_value("README_visualization",
   CTIP_COORDINATE_DISCRIPTION, local_ccmc_attribute_structure_list);
   stat = update_ccmc_attribute_value(
         "model_name",
         "ctip",
         local_ccmc_attribute_structure_list);
   stat = update_ccmc_attribute_value("model_type",
   CTIP_DISCIPLINE, local_ccmc_attribute_structure_list);
   stat = update_ccmc_attribute_value(
         "original_output_file_name",
         input_file_name,
         local_ccmc_attribute_structure_list);
   stat = update_ccmc_attribute_value("terms_of_usage",
   CCMC_USE_POLICY, local_ccmc_attribute_structure_list);
   stat = update_ccmc_attribute_value(
         "grid_system_count",
         &number_grids,
         local_ccmc_attribute_structure_list);
   stat = update_ccmc_attribute_value(
         "grid_system_1_number_of_dimensions",
         &number_dims,
         local_ccmc_attribute_structure_list);
   stat = update_ccmc_attribute_value(
         "grid_system_1_dimension_1_size",
         &ctip_NZ,
         local_ccmc_attribute_structure_list);
   stat = update_ccmc_attribute_value(
         "grid_system_1_dimension_2_size",
         &ctip_NX,
         local_ccmc_attribute_structure_list);
   stat = update_ccmc_attribute_value(
         "grid_system_1_dimension_3_size",
         &ctip_NY,
         local_ccmc_attribute_structure_list);
   stat = update_ccmc_attribute_value(
         "grid_system_1",
         "Z, X, Y",
         local_ccmc_attribute_structure_list);
   stat = update_ccmc_attribute_value(
         "output_type",
         "3D spherical",
         local_ccmc_attribute_structure_list);
   stat = update_ccmc_attribute_value(
         "standard_grid_target",
         "geo",
         local_ccmc_attribute_structure_list);
   stat = update_ccmc_attribute_value(
         "grid_1_type",
         "spherical, rtp",
         local_ccmc_attribute_structure_list);
   stat = update_ccmc_attribute_value(
         "kameleon_version",
         kameleon_version_num,
         local_ccmc_attribute_structure_list);

   /* allocate all arrays */
   ctip_number_of_cells=ctip_NX*ctip_NY*ctip_NZ;

   if ( (data_ptr=(float*)malloc(ctip_number_of_cells*sizeof(float)))==NULL)
   {
      fprintf(stderr,"Read_CTIP: allocation of generic data array failed\n");
      return (1);
   }

   strsplit(ctip_unit_string, &ctip_unit_strarr, ",");
   nvar =ctip_unit_strarr.len;
   tmp_2d_array=(char*)malloc(ctip_NX*ctip_NY*sizeof(double));
   tmp_4d_array=(char*)malloc(ctip_NX*ctip_NY*ctip_NZ*sizeof(double)*nvar);

   printf("number_of_cells: %d NVAR: %d\n", ctip_number_of_cells, nvar);

   /*************** Read Record 1  *********************/
   if (read_ctip_record(ctip_input_filePtr, verbose_flag, ctip_number_of_cells
         *nvar, tmp_4d_array) == EXIT_FAILURE)
   {
      printf("Read_CTIP: Error reading record 1\n");
      /* we don't need to die...just return control to calling program - main
       * read driver */
      /*exit (EXIT_FAILURE);*/
      return ( EXIT_FAILURE );

   }

   if (read_ctip_record(
         ctip_input_filePtr,
         verbose_flag,
         ctip_NX*ctip_NY,
         tmp_2d_array) == EXIT_FAILURE)
   {
      printf("Read_CTIP: Error reading record 2\n");
      exit(EXIT_FAILURE);
   }
   status_code=1;

   /* 3D temporary array */
   data_ptr=(float*)malloc(ctip_number_of_cells*sizeof(float));

   CTIP_MISSING=-pow(256., 5);

   /* coordinate systems */
   strcpy(grid_system, "IP colat lon");
   strcpy(position_grid_system, "IP colat lon");
   strcpy(data_grid_system, "Height colat lon");

   /* Z, X, Y */
   ctip_ip_arrayPtr=(float*)malloc(ctip_NZ*sizeof(float));
   ctip_colat_arrayPtr=(float*)malloc(ctip_NX*sizeof(float));
   ctip_lon_arrayPtr=(float*)malloc(ctip_NY*sizeof(float));
   for (i=0; i<ctip_NZ; i++)
   {
      ctip_ip_arrayPtr[i]=1+i;
   }
   for (i=0; i<ctip_NX; i++)
   {
      ctip_colat_arrayPtr[i]=(i*180.)/(ctip_NX-1);
   }
   for (i=0; i<ctip_NY; i++)
   {
      ctip_lon_arrayPtr[i]=(i*360.)/ctip_NY;
   }

   /****************************************************************************
    *                        CREATE THE VARIABLES & VARIABLE ATTRIBUTES        *
    ***************************************************************************/

   /****************************************************************************
    *    initialize the local_ccmc_variable_structure_list to hold n number of *
    *    cccmc_variable_structures where n = NUMBER_REGISTERED_CTIP_VARIABLES  *
    *    defined in ccmc_registered_variables.h                                *
    ***************************************************************************/

   local_ccmc_variable_structure_list
         = init_ccmc_variable_structure( NUMBER_REGISTERED_CTIP_VARIABLES);

   /* next use the put_ccmc_variable() routine supplied via the
    * ccmc_metadata_structure.c code */
   /* this routine will create all the registered ccmc variables listed in
    * ccmc_registered_variables.h file via the structure list to ensure
    * uniformity the arguments to put_ccmc_variable are:

    char *var_name, char *var_data_type,int var_size, char *data_classification
    ie position||data,  void *var_values, float valid_min, float valid_max,
    char *units, char *grid_system, float mask, char *description, int
    is_vector_component, char *position_grid_system, char *data_grid_system,
    float actual_min, float actual_max, struct ccmc_variable_structure
    *current_var_struct

    ...we will set the actual values later using the
    update_ccmc_variable_attribute_value() & update_ccmc_variable_value functions

    for each variable, create structure entry using defaults defined in
    ccmc_registered_variable.h and exported using registered_ctip_var_list ***/

   /*** this is just to demonstrate how the default ccmc variable attributes and
    *  values would normally be created. In this instance they could have been
    * created and populated at the same time */
   /*   but for demo purposes we'll create and populate with defaults and then
    * update specific variable attribute values and data values later by name...*/

   /**** Put default variable names and variable attribute values into the
    * ccmc_baselined_variable_structure_list ***/

   for (i = 0; i< NUMBER_REGISTERED_CTIP_VARIABLES; i++)
   {
      put_ccmc_variable(
            registered_ctip_var_list[i].ccmc_var_name,
            registered_ctip_var_list[i].ccmc_var_data_type,
            0,
            registered_ctip_var_list[i].ccmc_var_classification,
            "",
            registered_ctip_var_list[i].valid_min,
            registered_ctip_var_list[i].valid_max,
            registered_ctip_var_list[i].units,
            registered_ctip_var_list[i].grid_system,
            registered_ctip_var_list[i].mask,
            registered_ctip_var_list[i].description,
            registered_ctip_var_list[i].is_vector_component,
            registered_ctip_var_list[i].position_grid_system,
            registered_ctip_var_list[i].data_grid_system,
            zero,
            zero,
            local_ccmc_variable_structure_list);
   }

   if (verbose_flag)
   {
      printf("registered variable names and default variable attribute values "
         "succeffully added to baselined_ccmc_variable_structure_list...\n");
   }

   /****************************************************************************
    *   write the actual variable values into the                              *
    * baselined_ccmc_variable_structure_list                                   *
    ***************************************************************************/

   /*** write the original model variable values ***/

   if (verbose_flag)
   {
      printf("\nwritting original model output variables values into "
         "local_ccmc_variable_structure_list...\n");
   }

   /*************************************/
   /* Z-coordinate: IP - pressure level */
   /*************************************/

   /*** update each variable size as the defualts are set to zero, this reallocs
    * space for the actual values to be stored *****/

   stat = update_ccmc_variable_attribute_value(
         "z",
         "variable_size",
         &ctip_NZ,
         local_ccmc_variable_structure_list);
   stat = update_ccmc_variable_value(
         "z",
         ctip_NZ,
         ctip_ip_arrayPtr,
         local_ccmc_variable_structure_list);

   /*** lets update description since default in .h is just description for
    * regular z coordinate ***/

   strcpy(
         description,
         "Pressure level. Highest pressure at bottom. Physical height varies "
            "over time in the lagrangian grid.");

   stat = update_ccmc_variable_attribute_value(
         "z",
         "description",
         description,
         local_ccmc_variable_structure_list);

   /*** change default z units from R to '[ ]' since pressure level does not
    * have units ***/

   stat = update_ccmc_variable_attribute_value(
         "z",
         "units",
         "[ ]",
         local_ccmc_variable_structure_list);

   /** now valid/actual min max values **/

   linear_minmax_search(ctip_ip_arrayPtr, ctip_NZ, &actual_min, &actual_max);

   /** we change the default value for z from -10000 to 1 since z references
    * pressure level in this implementation **/

   stat = update_ccmc_variable_attribute_value(
         "z",
         "valid_min",
         &local_valid_min,
         local_ccmc_variable_structure_list);

   stat = update_ccmc_variable_attribute_value(
         "z",
         "actual_min",
         &actual_min,
         local_ccmc_variable_structure_list);
   stat = update_ccmc_variable_attribute_value(
         "z",
         "actual_max",
         &actual_max,
         local_ccmc_variable_structure_list);

   /*****************************/
   /* X coordinate: co-latitude */
   /*****************************/

   stat = update_ccmc_variable_attribute_value(
         "x",
         "variable_size",
         &ctip_NX,
         local_ccmc_variable_structure_list);
   stat = update_ccmc_variable_value(
         "x",
         ctip_NX,
         ctip_colat_arrayPtr,
         local_ccmc_variable_structure_list);

   /*** lets update description since default in .h is just description for
    * regular z coordinate ***/

   strcpy(description, "Co-Latitude in geographic coordinates. North=0.");

   stat = update_ccmc_variable_attribute_value(
         "x",
         "description",
         description,
         local_ccmc_variable_structure_list);

   linear_minmax_search(ctip_colat_arrayPtr, ctip_NX, &actual_min, &actual_max);

   stat = update_ccmc_variable_attribute_value(
         "x",
         "actual_min",
         &actual_min,
         local_ccmc_variable_structure_list);
   stat = update_ccmc_variable_attribute_value(
         "x",
         "actual_max",
         &actual_max,
         local_ccmc_variable_structure_list);
   stat = update_ccmc_variable_attribute_value(
         "x",
         "valid_min",
         &zero,
         local_ccmc_variable_structure_list);

   ctip_missing = CTIP_MISSING;

   stat = update_ccmc_variable_attribute_value(
         "x",
         "valid_max",
         &one_eighty,
         local_ccmc_variable_structure_list);
   stat = update_ccmc_variable_attribute_value(
         "x",
         "units",
         "deg",
         local_ccmc_variable_structure_list);
   stat = update_ccmc_variable_attribute_value(
         "x",
         "mask",
         &ctip_missing,
         local_ccmc_variable_structure_list);

   /***************************/
   /* Y coordinate: longitude */
   /***************************/

   stat = update_ccmc_variable_attribute_value(
         "y",
         "variable_size",
         &ctip_NY,
         local_ccmc_variable_structure_list);
   stat = update_ccmc_variable_value(
         "y",
         ctip_NX,
         ctip_lon_arrayPtr,
         local_ccmc_variable_structure_list);

   strcpy(description, "Longitude in geographic coordinates.");

   stat = update_ccmc_variable_attribute_value(
         "y",
         "description",
         description,
         local_ccmc_variable_structure_list);

   linear_minmax_search(ctip_lon_arrayPtr, ctip_NY, &actual_min, &actual_max);

   stat = update_ccmc_variable_attribute_value(
         "y",
         "actual_min",
         &actual_min,
         local_ccmc_variable_structure_list);
   stat = update_ccmc_variable_attribute_value(
         "y",
         "actual_max",
         &actual_max,
         local_ccmc_variable_structure_list);
   stat = update_ccmc_variable_attribute_value(
         "y",
         "valid_min",
         &zero,
         local_ccmc_variable_structure_list);
   stat = update_ccmc_variable_attribute_value(
         "y",
         "valid_max",
         &three_sixty,
         local_ccmc_variable_structure_list);
   stat = update_ccmc_variable_attribute_value(
         "y",
         "units",
         "deg",
         local_ccmc_variable_structure_list);
   stat = update_ccmc_variable_attribute_value(
         "y",
         "mask",
         &ctip_missing,
         local_ccmc_variable_structure_list);

   /*********************/
   /* now the variables */
   /*********************/
   /*    printf("NVAR: %ld\n",nvar);*/

   for (i=0, k=3; i<nvar; i++, k++)
   {
      strarr tmp_strarr;
      tmp_strarr.len=0;
      tmp_strarr.sarr=NULL;

      found=0;
      strsplit(ctip_unit_strarr.sarr[i], &tmp_strarr, " ");
      strcpy(alt_name, tmp_strarr.sarr[0]+1);
      strncpy(units, tmp_strarr.sarr[1], strlen(tmp_strarr.sarr[1])-1);

      if (strstr(ctip_unit_strarr.sarr[i], "Height") != NULL)
      {
         found=1;
         valid_min=0.;
         valid_max=1e6;
         strcpy(name, "H"); /* this is the variable name */
         strcpy(units, "km");
         strcpy(si_units, "m");
         unit_conversion_factor=1000.;

         /* code to render unit in IDL - sometimes identical to units */
         strcpy(units_idl, "km");

         /* code to render unit in IDL - sometimes identical to si_units */
         strcpy(si_units_idl, "m");
         is_vector_component = 0;
         mask=CTIP_MISSING;

         /* alternative variable name - code to be rendered in IDL */
         strcpy(name_idl, "Height");
         strcpy(
               description,
               "Height above ground. This height describes points on the "
                  "pressure levels over time in the Lagrangian grid.");
      }
      if (strstr(ctip_unit_strarr.sarr[i], "Vn_lon") != NULL)
      {
         found=1;
         valid_min=0.;
         valid_max=1e6;
         strcpy(units, "m/s");
         strcpy(si_units, "m/s");
         unit_conversion_factor=1.;
         strcpy(units_idl, "m/s");
         strcpy(si_units_idl, "m/s");
         mask=CTIP_MISSING;
         strcpy(
               description,
               "Neutral velocity - longitudinal / zonal direction.");
         is_vector_component = 1;
         strcpy(name, "Vn_y");
         strcpy(name_idl, "Vn!Dlon!N");
      }
      if (strstr(ctip_unit_strarr.sarr[i], "Vn_lat") != NULL)
      {
         found=1;
         valid_min=-1.e6;
         valid_max= 1.e6;
         strcpy(units, "m/s");
         mask=CTIP_MISSING;
         strcpy(
               description,
               "Neutral velocity - latitudinal / meridional component");
         is_vector_component = 1;
         strcpy(si_units, "m/s");
         unit_conversion_factor=1.;
         strcpy(units_idl, "m/s");
         strcpy(si_units_idl, "m/s");
         strcpy(name, "Vn_x"); /**** made x lower case ***/
         strcpy(name_idl, "Vn!Dlat!N");
      }
      if (strstr(ctip_unit_strarr.sarr[i], "Vn_z")!= NULL)
      {
         found=1;
         valid_min=-1.e6;
         valid_max= 1.e6;
         strcpy(units, "m/s");
         mask=CTIP_MISSING;
         strcpy(description, "Neutral velocity - vertical direction");
         is_vector_component = 1;
         strcpy(si_units, "m/s");
         unit_conversion_factor=1.;
         strcpy(units_idl, "m/s");
         strcpy(si_units_idl, "m/s");
         strcpy(name, "Vn_z"); /* made Z lower case */
         strcpy(name_idl, "Vn!Dz!N");
      }
      if (strstr(ctip_unit_strarr.sarr[i], "Vi_lon")!= NULL)
      {
         found=1;
         valid_min=-1.e6;
         valid_max= 1.e6;
         strcpy(units, "m/s");
         mask=CTIP_MISSING;
         strcpy(description, "Plasma velocity - longitudinal / zonal component");
         is_vector_component = 1;
         strcpy(si_units, "m/s");
         unit_conversion_factor=1.;
         strcpy(units_idl, "m/s");
         strcpy(si_units_idl, "m/s");
         strcpy(name, "Vi_y");
         strcpy(name_idl, "Vi!DLon!N");
      }
      if (strstr(ctip_unit_strarr.sarr[i], "Vi_lat")!= NULL)
      {
         found=1;
         valid_min=-1.e6;
         valid_max= 1.e6;
         strcpy(units, "m/s");
         mask=CTIP_MISSING;
         strcpy(
               description,
               "Plasma velocity - latitudinal / meridional component");
         is_vector_component = 1;
         strcpy(si_units, "m/s");
         unit_conversion_factor=1.;
         strcpy(units_idl, "m/s");
         strcpy(si_units_idl, "m/s");
         strcpy(name, "Vi_x");
         strcpy(name_idl, "Vi!DLat!N");
      }
      /* Vi_z is not present - set to 0 to complete vector */

      if (strstr(ctip_unit_strarr.sarr[i], "N_e")!= NULL)
      {
         found=1;
         valid_min= 0;
         valid_max= 1.e30;
         strcpy(units, "m^-3");
         mask=CTIP_MISSING;
         strcpy(description, "Number density of electrons");
         is_vector_component = 0;
         strcpy(si_units, "m^-3");
         unit_conversion_factor=1.;
         strcpy(units_idl, "m^-3");
         strcpy(si_units_idl, "m!U-3!N");
         strcpy(name, "N_e");
         strcpy(name_idl, "N!De!N");
      }
      if (strstr(ctip_unit_strarr.sarr[i], "T_n")!= NULL)
      {
         found=1;
         valid_min= 0.;
         valid_max= 1.e6;
         strcpy(units, "K");
         mask=CTIP_MISSING;
         strcpy(description, "Neutral temperature");
         is_vector_component = 0;
         strcpy(si_units, "K");
         unit_conversion_factor=1.;
         strcpy(units_idl, "K");
         strcpy(si_units_idl, "K");
         strcpy(name, "Tn");
         strcpy(name_idl, "T!DN!N");
      }
      if (strstr(ctip_unit_strarr.sarr[i], "Rmt")!= NULL)
      {
         found=1;
         valid_min= 0.;
         valid_max= 1.e6;
         strcpy(units, "amu");
         mask=CTIP_MISSING;
         strcpy(description, "mean molecular mass");
         is_vector_component = 0;
         strcpy(si_units, "amu");
         unit_conversion_factor=1.;
         strcpy(units_idl, "amu");
         strcpy(si_units_idl, "amu");
         strcpy(name, "Rmt");
         strcpy(name_idl, "Rmt");
      }
      if (strstr(ctip_unit_strarr.sarr[i], "N_O")!= NULL)
      {
         found=1;
         valid_min= 0.;
         valid_max= 1.e30;
         strcpy(units, "m^-3");
         mask=CTIP_MISSING;
         strcpy(description, "Number density of oxygen ions");
         is_vector_component = 0;
         strcpy(si_units, "m^-3");
         unit_conversion_factor=1.;
         strcpy(units_idl, "m^-3");
         strcpy(si_units_idl, "m!U-3!N");
         strcpy(name, "N_O");
         strcpy(name_idl, "N!DO!N");
      }
      if (strstr(ctip_unit_strarr.sarr[i], "N_N2")!= NULL)
      {
         found=1;
         valid_min= 0.;
         valid_max= 1.e30;
         strcpy(units, "m/s");
         mask=CTIP_MISSING;
         strcpy(description, "Number density of molecular nitrogen");
         is_vector_component = 0;
         strcpy(si_units, "m^-3");
         unit_conversion_factor=1.;
         strcpy(units_idl, "m^-3");
         strcpy(si_units_idl, "m!U-3!N");
         strcpy(name, "N_N2");
         strcpy(name_idl, "N!DN2!N");
      }
      if (strstr(ctip_unit_strarr.sarr[i], "N_O2")!= NULL)
      {
         found=1;
         valid_min= 0.;
         valid_max= 1.e30;
         strcpy(units, "m/s");
         mask=CTIP_MISSING;
         strcpy(description, "Number density of molecular oxygen");
         is_vector_component = 0;
         strcpy(si_units, "m^-3");
         unit_conversion_factor=1.;
         strcpy(units_idl, "m^-3");
         strcpy(si_units_idl, "m!U-3!N");
         strcpy(name, "N_O2");
         strcpy(name_idl, "N!DO!N");
      }
      if (strstr(ctip_unit_strarr.sarr[i], "sigmaP")!= NULL)
      {
         found=1;
         valid_min= 0.;
         valid_max= 1.e6;
         mask=CTIP_MISSING;
         strcpy(description, "Pedersen conductivity");
         is_vector_component = 0;
         unit_conversion_factor=1.;
         strcpy(units, "mho/m");
         strcpy(si_units, "mho/m");
         strcpy(units_idl, "mho/m");
         strcpy(si_units_idl, "(!4X!Xm)!U-1!N");
         strcpy(name, "sigmaP");
         strcpy(name_idl, "!4r!X!DP!N");
      }
      if (strstr(ctip_unit_strarr.sarr[i], "sigmaH") != NULL)
      {
         found=1;
         valid_min= 0.;
         valid_max= 1.e6;
         mask=CTIP_MISSING;
         strcpy(description, "Hall conductivity");
         is_vector_component = 0;
         unit_conversion_factor=1.;
         strcpy(units, "mho/m");
         strcpy(si_units, "mho/m");
         strcpy(units_idl, "mho/m");
         strcpy(si_units_idl, "(!4X!Xm)!U-1!N");
         strcpy(name, "sigmaH");
         strcpy(name_idl, "!4r!X!DH!N");
      }
      if (found)
      {

         if (!ctip_is_double)
            for (j=0; j<ctip_number_of_cells; j++)
            {
               data_ptr[j]=((float*)tmp_4d_array)[ctip_number_of_cells*i+j];
            }
         if (ctip_is_double)
            for (j=0; j<ctip_number_of_cells; j++)
            {
               data_ptr[j]=((double*)tmp_4d_array)[ctip_number_of_cells*i+j];
            }

         /**************************** put to local structures ***************/

         stat = update_ccmc_variable_attribute_value(
               name,
               "variable_size",
               &ctip_number_of_cells,
               local_ccmc_variable_structure_list);
         stat = update_ccmc_variable_value(
               name,
               ctip_number_of_cells,
               data_ptr,
               local_ccmc_variable_structure_list);

         linear_minmax_search(
               data_ptr,
               ctip_number_of_cells,
               &actual_min,
               &actual_max);

         stat = update_ccmc_variable_attribute_value(
               name,
               "actual_min",
               &actual_min,
               local_ccmc_variable_structure_list);
         stat = update_ccmc_variable_attribute_value(
               name,
               "actual_max",
               &actual_max,
               local_ccmc_variable_structure_list);
         stat = update_ccmc_variable_attribute_value(
               name,
               "valid_min",
               &valid_min,
               local_ccmc_variable_structure_list);
         stat = update_ccmc_variable_attribute_value(
               name,
               "valid_max",
               &valid_max,
               local_ccmc_variable_structure_list);
         stat = update_ccmc_variable_attribute_value(
               name,
               "units",
               units,
               local_ccmc_variable_structure_list);
         stat = update_ccmc_variable_attribute_value(
               name,
               "grid_system",
               grid_system,
               local_ccmc_variable_structure_list);
         stat = update_ccmc_variable_attribute_value(
               name,
               "mask",
               &ctip_missing,
               local_ccmc_variable_structure_list);
         stat = update_ccmc_variable_attribute_value(
               name,
               "description",
               description,
               local_ccmc_variable_structure_list);
         stat = update_ccmc_variable_attribute_value(
               name,
               "is_vector_component",
               &is_vector_component,
               local_ccmc_variable_structure_list);
         stat = update_ccmc_variable_attribute_value(
               name,
               "position_grid_system",
               position_grid_system,
               local_ccmc_variable_structure_list);
         stat = update_ccmc_variable_attribute_value(
               name,
               "data_grid_system",
               data_grid_system,
               local_ccmc_variable_structure_list);

         if (verbose_flag)
            printf("Finished pushing data for variable %s into structure\n"
               "Variable min: %f max: %f\n", name, actual_min, actual_max);
      }
      printf(
            "Read_CTIP: finished copying data into variable %s\n",
            tmp_strarr.sarr[0]+1);
      free_strarr(&tmp_strarr);
   }

   /********** lets go ahead and baseline the local struct and proceed to the
    * write_structures_2_cdf stage for testing ********/

   baselined_ccmc_attribute_structure_list
         = local_ccmc_attribute_structure_list;
   baselined_ccmc_variable_structure_list = local_ccmc_variable_structure_list;

   if ( DEBUG_FLAG)
   {
      printf("\t\t\t\t\t\t\t\t\tDEBUGGING INFO\n\n\n\n\n\n******************** "
         "baselined \n");
      print_ccmc_attribute_structure(
            baselined_ccmc_attribute_structure_list,
            put_attribute_call_count);

      printf("\n\n\n\n\n\n******************** local \n");
      print_ccmc_attribute_structure(
            local_ccmc_attribute_structure_list,
            put_attribute_call_count);
   }
   /* process 2D information */

   /*    fclose( ctip_input_filePtr ); */
   if (verbose_flag)
   {
      printf("Read_CTIP: finished copying data into variables...\n");
   }
   status_code=1;
   printf("Success: %d Failure: %d Status: %d\n", 1, 0, status_code);
   /*  the main_read_driver EXIT_FAILURE || EXIT_SUCCESS */
   /*  return status_code; */
   return EXIT_SUCCESS;
}

int read_ctip_record(
      FILE *input_filePtr,
      int verbose_flag,
      long int expected_size,
      char *tmp_array)
{
   /* expected_size: number of numbers expected - float or double */
   int record_size, a, n=0, element_size;

   /* get the number of bytes in record 1 ( we expect a array of expected_size
    * with either float or double ) */
   fread( &ctip_byte_value, 4, 1, input_filePtr);
   record_size = ctip_byte_value;

   if (ferror(input_filePtr) || feof(input_filePtr) )
   {
      printf("ERROR with fread must have returned non zero status_code for file "
         "stream\n");
      perror("input_filePtr");
      return EXIT_FAILURE;
   }
   ctip_flip_endian=( (record_size != (sizeof(float)*expected_size) )
         && (record_size != (sizeof(double)*expected_size) ) );
   if (ctip_flip_endian)
      endian_flip((char *)&record_size, 4, 1);

   if (verbose_flag)
   {
      printf(
            "CTIP Record: found record_size: %d Expected: %ld\n",
            record_size,
            sizeof(float)*expected_size);
   }

   if ( (record_size != sizeof(float)*expected_size) && (record_size
         != sizeof(double)*expected_size ))
   {
      printf("Read_CTIP_record: record size not as expected!\n");
      /* mmadddox - we don't need to die, pass error back to calling function */
      /*exit(EXIT_FAILURE);*/
      return ( EXIT_FAILURE );
   }
   ctip_is_double=(record_size == sizeof(double)*expected_size );

   /* sizeof float or double */
   element_size=(1-ctip_is_double)*sizeof(float)+ctip_is_double*sizeof(double);
   if (verbose_flag)
   {
      printf(
            "CTIP_is_double: %d CTIP_flip_endian: %d Record_size: %d "
               "element_size: %d\n",
            ctip_is_double,
            ctip_flip_endian,
            record_size,
            element_size);
   }

   n = fread(tmp_array, 1, record_size, input_filePtr);
   if (n != record_size)
   {
      printf("CTIP_read_record: number of bytes read does not agree with record "
         "size \n");
      return (EXIT_FAILURE);
   }

   if (ctip_flip_endian)
      endian_flip(tmp_array, element_size, expected_size);

   if (ferror(input_filePtr) || feof(input_filePtr) )
   {
      printf("ERROR with fread must have returned non zero status_code for file "
         "stream\n");
      perror("input_filePtr");
      return EXIT_FAILURE;
   }

   fread( &ctip_byte_value_2, 4, 1, input_filePtr);

   if (ferror(input_filePtr) || feof(input_filePtr) )
   {
      printf("ERROR with fread must have returned non zero status_code for file "
         "stream\n");
      perror("input_filePtr");
      return EXIT_FAILURE;
   }

   if (ctip_byte_value == ctip_byte_value_2)
   {
      if (verbose_flag)
      {
         printf("Read_CTIP_Record SUCCESSFUL\n");
      }
      return EXIT_SUCCESS;
   }
   else
   {
      printf("ERROR reading input file\n");
      return EXIT_FAILURE;
   }

}

int build_ctip_grid_description(verbose_flag)
{

   int debug_flag = 0;

   int i, j, element_count, block_count, current_amr_level, parent_level;

   return 1;

}
