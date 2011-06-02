/*******************************************************************************
 *                                                                             *
 *    NAME:       ccmc_structure_manager.h                                     *
 *                                                                             *
 *    VERSION:    3.2                                                          *
 *                                                                             *
 *    AUTHOR:     Marlo M. H. Maddox                                           *
 *                NASA-GSFC-AETD-ISD-ADMA(587)                                 *
 *                Marlo.Maddox@nasa.gov                                        *
 *                (301) 286 - 5202                                             *
 *                                                                             *
 *    PURPOSE:    Dynamically allocate an array of ccmc_variable_structure     *
 *                ccmc_model_attribute_struct structures based on the int      *
 *                number_of_attributes arguments supplied by calling           *
 *                program.  Return a pointer to the newly created array of     *
 *                structures.                                                  *
 *                                                                             *
 *    INPUTS:     NONE                                                         *
 *                                                                             *
 *    OUTPUTS:    NONE                                                         *
 *                                                                             *
 *    FUNCTIONS:  NONE                                                         *
 *                                                                             *
 *    VARIABLES:                                                               *
 *                                                                             *
 *                                                                             *
 *    MODIFICATION                                                             *
 *    HISTORY:                                                                 *
 *                                                                             *
 ******************************************************************************/

/*** standard structure for capturing global and variable metadata and non-variant data
 to be used as model specific global metadata ***/

struct ccmc_attribute_structure
{
   char *attribute_name;
   char *attribute_type;
   char *attribute_data_type;

   union attribute_val
   {
      int int_value;
      float float_value;
      double double_value;
      char *string_value;
   } Attribute_value;

}*baselined_ccmc_attribute_structure_list;

/*** standard variable structure to hold all information pertaining to any model variable ***/

struct ccmc_variable_structure
{
   char *variable_name;
   char *variable_data_type;
   int variable_size;
   char *data_classification; /** ie. position or data **/

   union variable_val
   {
      int *int_value;
      float *float_value;
      double *double_value;
   } Variable_values;

   /** ccmc_variable_attributes, overwite any variable attribute values from ccmc_attribute_structure **/

   float valid_min;
   float valid_max;
   char *units;
   char *grid_system;
   float mask;
   char *description;
   int is_vector_component;
   char *position_grid_system;
   char *data_grid_system;
   float actual_min;
   float actual_max;

}*baselined_ccmc_variable_structure_list;

/**** keep track of how many attribute & variable structures where created ***/

int put_attribute_call_count;
int put_variable_call_count;

/*** define the function available to manipulate the structures **/

int find_struct_index_for_name(
      char *,
      int,
      void *,
      int);

/*** interface routines for ccmc_attribute_structures ***/

struct ccmc_attribute_structure *init_ccmc_attribute_structure(
      int);

int put_ccmc_attribute(
      char *,
      char *,
      char *,
      void *,
      struct ccmc_attribute_structure *);
int free_ccmc_attribute_structure(
      struct ccmc_attribute_structure *,
      int);
int print_ccmc_attribute_structure(
      struct ccmc_attribute_structure *,
      int);
int update_ccmc_attribute_value(
      char *,
      void *,
      struct ccmc_attribute_structure *);

/*
 int put_ccmc_attribute( char *, char *, char *, void * );
 int put_ccmc_attribute( char *, char *, char *, void *, struct ccmc_attribute_structure * );
 int free_ccmc_attribute_structure( int );
 int print_ccmc_attribute_structure( int );
 */

/*** interface routines for ccmc_variable_structures ***/

struct ccmc_variable_structure *init_ccmc_variable_structure(
      int);
int put_ccmc_variable(
      char *,
      char *,
      int,
      char *,
      void *,
      float,
      float,
      char *,
      char *,
      float,
      char *,
      int,
      char *,
      char *,
      float,
      float,
      struct ccmc_variable_structure *);

int free_ccmc_variable_structure(
      struct ccmc_variable_structure *,
      int);

int print_ccmc_variable_structure(
      struct ccmc_variable_structure *,
      int);
int update_ccmc_variable_value(
      char *,
      int,
      void *,
      struct ccmc_variable_structure *);
int update_ccmc_variable_attrubute_value(
      char *,
      char *,
      void *,
      struct ccmc_variable_structure *);
/*
 int init_ccmc_variable_structure( int );
 int put_ccmc_variable( char *, char *,int, char *, void *, float, float, char *, char *, float, char *, int, char *, char *, float, float );
 int free_ccmc_variable_structure( int );
 int print_ccmc_variable_structure( int );
 */

