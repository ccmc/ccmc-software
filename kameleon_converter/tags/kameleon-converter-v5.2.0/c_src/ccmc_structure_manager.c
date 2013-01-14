/*******************************************************************************
 *                                                                             *
 *    NAME:          ccmc_structure_manager.c                                  *
 *                                                                             *
 *    VERSION:       4.0                                                       *
 *                                                                             *
 *    AUTHOR:        Marlo M. H. Maddox                                        *
 *                   NASA-GSFC-AETD-ISD-ADMA(587)                              *
 *                   Marlo.Maddox@nasa.gov                                     *
 *                   (301) 286 - 5202                                          *
 *                                                                             *
 *    PURPOSE:       Dynamically allocate an array of ccmc_variable_structure  *
 *                   ccmc_model_attribute_struct structures based on the int   *
 *                   number_of_attributes arguments supplied by calling        *
 *                   program.  Return a pointer to the newly created array of  *
 *                   structures.                                               *
 *                                                                             *
 *    INPUTS:        *                                                         *
 *                                                                             *
 *    OUTPUTS:       a pointer to an array of ccmc_model_attribute_struct      *
 *                   structures                                                *
 *                                                                             *
 *    FUNCTIONS:     *                                                         *
 *                                                                             *
 *    VARIABLES:     *                                                         *
 *                                                                             *
 *                                                                             *
 *    MODIFICATION                                                             *
 *    HISTORY:                                                                 *
 *    2006  April 1st      Maddox, Marlo                                       *
 *                         Initial Development                                 *
 *                                                                             *
 *                                                                             *
 *    2006  October 17th   Maddox, Marlo                                       *
 *                         Adding double type logic for                        *
 *                         update_ccmc_attribute_value                         *
 *                                                                             *
 *    2007  February 6th   Maddox, Marlo                                       *
 *                         changing ccmc_metadata_structure.h to               *
 *                         ccmc_structure_manager.h                            *
 ******************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "ccmc_structure_manager.h"

#define DEBUG_FLAG 0

struct ccmc_attribute_structure *init_ccmc_attribute_structure(
      int number_of_attributes)
/*int init_ccmc_attribute_structure( int number_of_attributes )*/
{

   put_attribute_call_count = 0;

   return ( ( struct ccmc_attribute_structure *) calloc(
         number_of_attributes,
         sizeof(struct ccmc_attribute_structure)) );

}

struct ccmc_variable_structure *init_ccmc_variable_structure(
      int number_of_variables)
/*int init_ccmc_variable_structure( int number_of_variables )*/
{

   put_variable_call_count = 0;

   return ( ( struct ccmc_variable_structure *) calloc(
         number_of_variables,
         sizeof(struct ccmc_variable_structure)) );

}

int put_ccmc_attribute(
      char *attr_name,
      char *attr_type,
      char *attr_data_type,
      void *attr_value,
      struct ccmc_attribute_structure *current_attr_struct)
/*int put_ccmc_attribute( char *attr_name, char *attr_type, char *attr_data_type, void *attr_value )*/
{

   int valid_attr_type(char *);
   int valid_attr_data_type(char *);

   /*** verify that attr_type & attr_data_types are supported ***/
   if ( !valid_attr_type(attr_type) )
   {
      printf(
            "ERROR:\t%s attribute type is invalid.  Suppoted type: global, "
            "model, variable\n",
            attr_type);
      return 1;
   } /*** failure ***/
   if ( !valid_attr_data_type(attr_data_type) )
   {
      printf(
            "ERROR:\t%s attribute data type is invalid.  Suppoted data types: "
            "int, float, double, char\n",
            attr_data_type);
      return 1;
   } /*** failure ***/

   /*** allocate memory in current structure for submitted char elements, add
    * values ***/

   current_attr_struct[put_attribute_call_count].attribute_name
         = ( char * ) malloc(strlen(attr_name) + 1);
   strcpy(
         current_attr_struct[put_attribute_call_count].attribute_name,
         attr_name);

   current_attr_struct[put_attribute_call_count].attribute_type
         = ( char * ) malloc(strlen(attr_type) + 1);
   strcpy(
         current_attr_struct[put_attribute_call_count].attribute_type,
         attr_type);

   current_attr_struct[put_attribute_call_count].attribute_data_type
         = ( char * ) malloc(strlen(attr_data_type) + 1);
   strcpy(
         current_attr_struct[put_attribute_call_count].attribute_data_type,
         attr_data_type);

   /*** if attribute value is a string, dynamically allocate space based on
    * string length and strcpy value into structure ***/

   if ( !strcmp(attr_data_type, "char") )
   {
      current_attr_struct[put_attribute_call_count].Attribute_value.string_value
            = ( char * ) malloc(strlen( (char *) attr_value ) + 1);
      strcpy(
            current_attr_struct[put_attribute_call_count].Attribute_value.string_value,
            ( char * ) attr_value );
      put_attribute_call_count++;
      return 0;
   }
   else if ( !strcmp(attr_data_type, "int") )
   {
      current_attr_struct[put_attribute_call_count].Attribute_value.int_value
            = *((int*)attr_value);
      put_attribute_call_count++;
      return 0;
   }
   else if ( !strcmp(attr_data_type, "float") )
   {
      current_attr_struct[put_attribute_call_count].Attribute_value.float_value
            = *((float*)attr_value);
      put_attribute_call_count++;
      return 0;
   }
   else if ( !strcmp(attr_data_type, "double") )
   {
      current_attr_struct[put_attribute_call_count].Attribute_value.double_value
            = *((double*) attr_value);
      put_attribute_call_count++;
      return 0;
   }
   else
   {
      return 1; /*** error ***/
   }

}

int update_ccmc_attribute_value(
      char *attr_name,
      void *new_attr_value,
      struct ccmc_attribute_structure *current_attr_struct)
{

   /*
    * find_struct_index_for_name can search both attribute & variable
    * structures.  It just needs to know how to type cast the structure.  Call
    * find_struct_index_for_name with last argument 1 or 2...1 for attr 2 for
    * var
    *
    */

   int struct_index;

   if ( DEBUG_FLAG)
      printf("DEBUG\tinside update_ccmc_attr...");

   /*** lets try to screen out NULL pointers from the begining, otherwise
    * we will have problems ***/

   if (new_attr_value == NULL)
   {

      if ( DEBUG_FLAG)
         printf("ERROR\tnew_attr_value == NULL...skipping current attribute\n");

      return EXIT_FAILURE;
   }

   /*** find index of structure containing attr_name ***/

   struct_index = find_struct_index_for_name(
         attr_name,
         put_attribute_call_count,
         current_attr_struct,
         1);

   /*** struct_index value, if -1 no element was found for given name.
    * Return control to calling program ***/

   if (struct_index == -1)
      return EXIT_FAILURE;

   if ( DEBUG_FLAG)
      printf(
            "HI.  struct index found = %d\nwe were looking for %s and found %s\n",
            struct_index,
            attr_name,
            current_attr_struct[struct_index].attribute_name);

   /* since we filter the data types when they are initially added, we will
    * assume all existing data types are valid and we just update using the
    * correct type block
    */

   /**** if found attribute structure has a value of type char, realloc and
    * strcpy new value ****/

   if ( !strcmp(current_attr_struct[struct_index].attribute_data_type, "char") )
   {

      if ( DEBUG_FLAG)
         printf("inside char logic\n");

      current_attr_struct[struct_index].Attribute_value.string_value = realloc(
            current_attr_struct[struct_index].Attribute_value.string_value,
            sizeof(char) * strlen( (char *) new_attr_value ) +1);

      if ( DEBUG_FLAG)
         printf("size realloced\n");

      strcpy(
            current_attr_struct[struct_index].Attribute_value.string_value,
            ( char *) new_attr_value );

      if ( DEBUG_FLAG)
         printf("finished char logic\n");

   }
   else if ( !strcmp(
         current_attr_struct[struct_index].attribute_data_type,
         "int") )
   {

      current_attr_struct[struct_index].Attribute_value.int_value
            = *(( int *) new_attr_value);

   }
   else if ( !strcmp(
         current_attr_struct[struct_index].attribute_data_type,
         "float") )
   {

      current_attr_struct[struct_index].Attribute_value.float_value
            = *(( float *) new_attr_value);

   }
   else if ( !strcmp(
         current_attr_struct[struct_index].attribute_data_type,
         "double") )
   {

      current_attr_struct[struct_index].Attribute_value.double_value
            = *(( double *) new_attr_value);

   }

   return EXIT_SUCCESS;

}

int put_ccmc_variable(
      char *var_name,
      char *var_data_type,
      int var_size,
      char *data_classification,
      void *var_values,
      float valid_min,
      float valid_max,
      char *units,
      char *grid_system,
      float mask,
      char *description,
      int is_vector_component,
      char *position_grid_system,
      char *data_grid_system,
      float actual_min,
      float actual_max,
      struct ccmc_variable_structure *current_var_struct)
/*
 int put_ccmc_variable(  char *var_name, char *var_data_type,int var_size, char *data_classification,
 void *var_values, float valid_min, float valid_max, char *units, char *grid_system,
 float mask, char *description, int is_vector_component, char *position_grid_system,
 char *data_grid_system, float actual_min, float actual_max )*/
{

   int valid_data_classification(char *);
   int valid_var_data_type(char *);

   /*** verify that attr_type & attr_data_types are supported ***/
   if ( !valid_data_classification(data_classification) )
   {
      printf(
            "ERROR:\t%s data classification is invalid.  Suppoted data "
            "classification types: data, position\n",
            data_classification);
      return 1;
   } /*** failure ***/
   if ( !valid_var_data_type(var_data_type) )
   {
      printf(
            "ERROR:\t%s variable data type is invalid.  Suppoted data types: "
            "int, float, double\n",
            var_data_type);
      return 1;
   } /*** failure ***/

   /*** allocate memory in current structure for submitted char elements, add
    * values for char, int, float elements***/

   if ( DEBUG_FLAG)
      printf(
            "DEBUG\tinside put_ccmc_variable\tprocessing %s\ntype = %s\nsize "
            "= %d\nmin=%f\nmax=%f\n",
            var_name,
            var_data_type,
            var_size,
            actual_min,
            actual_max);
   /*
    printf("var_values[0] = %f\n", ((float *)var_values)[0]  );
    printf("var_values[1] = %f\n", ((float *)var_values)[1]  );
    printf("var_values[2] = %f\n", ((float *)var_values)[2]  );
    */
   /***** char values ******/

   if ( DEBUG_FLAG)
      printf("DEBUG\tallocating and inserting values into current_var_struct\n");

   current_var_struct[put_variable_call_count].variable_name
         = ( char * ) malloc(strlen(var_name) + 1);
   strcpy(current_var_struct[put_variable_call_count].variable_name, var_name);

   current_var_struct[put_variable_call_count].variable_data_type
         = ( char * ) malloc(strlen(var_data_type) + 1);
   strcpy(
         current_var_struct[put_variable_call_count].variable_data_type,
         var_data_type);

   current_var_struct[put_variable_call_count].data_classification
         = ( char * ) malloc(strlen(data_classification) + 1);
   strcpy(
         current_var_struct[put_variable_call_count].data_classification,
         data_classification);

   current_var_struct[put_variable_call_count].units
         = ( char * ) malloc(strlen(units) + 1);
   strcpy(current_var_struct[put_variable_call_count].units, units);

   current_var_struct[put_variable_call_count].grid_system
         = ( char * ) malloc(strlen(grid_system) + 1);
   strcpy(current_var_struct[put_variable_call_count].grid_system, grid_system);

   current_var_struct[put_variable_call_count].description
         = ( char * ) malloc(strlen(description) + 1);

   strcpy(current_var_struct[put_variable_call_count].description, description);

   current_var_struct[put_variable_call_count].position_grid_system
         = ( char * ) malloc(strlen(position_grid_system) + 1);
   strcpy(
         current_var_struct[put_variable_call_count].position_grid_system,
         position_grid_system);

   current_var_struct[put_variable_call_count].data_grid_system
         = ( char * ) malloc(strlen(data_grid_system) + 1);
   strcpy(
         current_var_struct[put_variable_call_count].data_grid_system,
         data_grid_system);

   /***** float and int values ******/

   current_var_struct[put_variable_call_count].variable_size = var_size;
   current_var_struct[put_variable_call_count].valid_min = valid_min;
   current_var_struct[put_variable_call_count].valid_max = valid_max;
   current_var_struct[put_variable_call_count].mask = mask;
   current_var_struct[put_variable_call_count].is_vector_component
         = is_vector_component;
   current_var_struct[put_variable_call_count].actual_min = actual_min;
   current_var_struct[put_variable_call_count].actual_max = actual_max;

   if ( DEBUG_FLAG)
      printf("DEBUG\tdone allocating and inserting values into current_var_struct\n");
   if ( DEBUG_FLAG)
      printf("DEBUG\tsetting actuall array values current_var_struct\n");

   /***** now set the actual array values based on var_data_type  ****/

   if ( !strcmp(var_data_type, "double") )
   {
      int i;
      current_var_struct[put_variable_call_count].Variable_values.double_value
            = ( double * ) malloc(sizeof(double) * var_size);
      /*memcpy( current_var_struct[put_variable_call_count].Variable_values.double_value, ( double * ) var_values, var_size );*/

      for (i=0; i< var_size; i++)
      {
         current_var_struct[put_variable_call_count].Variable_values.double_value[i]
               = (( double * )var_values)[i];
      }

      /*current_var_struct[put_variable_call_count].Variable_values.double_value = ( double * ) var_values;*/
      put_variable_call_count++;
      return 0;
   }
   else if ( !strcmp(var_data_type, "float") )
   {
      int i;
      current_var_struct[put_variable_call_count].Variable_values.float_value
            = ( float * ) malloc(sizeof(float) * var_size);
      /*memcpy( current_var_struct[put_variable_call_count].Variable_values.float_value, ( float * ) var_values, var_size );*/

      for (i=0; i< var_size; i++)
      {
         current_var_struct[put_variable_call_count].Variable_values.float_value[i]
               = (( float * )var_values)[i];
      }

      /*current_var_struct[put_variable_call_count].Variable_values.float_value = ( float * ) var_values;*/
      put_variable_call_count++;
      return 0;
   }
   else if ( !strcmp(var_data_type, "int") )
   {
      int i;
      current_var_struct[put_variable_call_count].Variable_values.int_value
            = ( int * ) malloc(sizeof(int) * var_size);
      /*memcpy( current_var_struct[put_variable_call_count].Variable_values.int_value, ( int * ) var_values, var_size );*/

      for (i=0; i< var_size; i++)
      {
         current_var_struct[put_variable_call_count].Variable_values.int_value[i]
               = (( int * )var_values)[i];
      }

      /*current_var_struct[put_variable_call_count].Variable_values.int_value = ( int * ) var_values;*/
      put_variable_call_count++;
      return 0;
   }
   else
   {
      return 1; /*** error ***/
   }

}

int update_ccmc_variable_value(
      char *var_name,
      int var_size,
      void *new_var_value,
      struct ccmc_variable_structure *current_var_struct)
/*int put_ccmc_attribute( char *attr_name, char *attr_type, char *attr_data_type, void *attr_value )*/
{

   /********** find_struct_index_for_name can search both attribute & variable structures.  It just needs to know how
    to type cast the structure.  Call find_struct_index_for_name with last argument 1 or 2...1 for attr 2 for var
    ***********/

   int struct_index;

   /*printf("DEBUG\tinside update_ccmc_var_value...\n");*/

   /*** find index of structure containing attr_name ***/

   /********************************************************* 1||2 see above **/

   struct_index = find_struct_index_for_name(
         var_name,
         put_variable_call_count,
         current_var_struct,
         2);

   /*printf("HI.  struct index found = %d\nwe were looking for %s and found %s\n", struct_index, var_name, current_var_struct[struct_index].variable_name );*/

   /*** struct_index value, if -1 no element was found for given name.  Return control to calling program ***/

   if (struct_index == -1)
      return 1;

   /***** now set the actual array values based on var_data_type  ****/

   if ( !strcmp(current_var_struct[struct_index].variable_data_type, "double") )
   {
      int i;
      current_var_struct[struct_index].Variable_values.double_value = realloc(
            current_var_struct[struct_index].Variable_values.double_value,
            sizeof(double) * var_size);
      /*memcpy( current_var_struct[put_variable_call_count].Variable_values.double_value, ( double * ) var_values, var_size );*/

      for (i=0; i< var_size; i++)
      {
         current_var_struct[struct_index].Variable_values.double_value[i] = (( double * )new_var_value)[i];
      }

      return 0;
   }
   else if ( !strcmp(
         current_var_struct[struct_index].variable_data_type,
         "float") )
   {
      int i;
      /*printf("should be inside float data type processing\n");*/
      current_var_struct[struct_index].Variable_values.float_value = realloc(
            current_var_struct[struct_index].Variable_values.float_value,
            sizeof(float) * var_size);
      /*memcpy( current_var_struct[put_variable_call_count].Variable_values.double_value, ( double * ) var_values, var_size );*/

      for (i=0; i< var_size; i++)
      {
         current_var_struct[struct_index].Variable_values.float_value[i] = (( float * )new_var_value)[i];
         /*printf("DEBUG\t value[ %d ]\t%f\n", i, ((float* )new_var_value)[i]);*/
      }

      return 0;
   }
   else if ( !strcmp(current_var_struct[struct_index].variable_data_type, "int") )
   {
      int i;

      current_var_struct[struct_index].Variable_values.int_value = realloc(
            current_var_struct[struct_index].Variable_values.int_value,
            sizeof(int) * var_size);

      for (i=0; i< var_size; i++)
      {
         current_var_struct[struct_index].Variable_values.int_value[i] = (( int * )new_var_value)[i];
      }

      return 0;
   }
   else
   {
      return 1; /*** error ***/
   }

}

int update_ccmc_variable_attribute_value(
      char *var_name,
      char *attr_name,
      void *new_attr_value,
      struct ccmc_variable_structure *current_var_struct)
{

   /*
    * find_struct_index_for_name can search both attribute & variable
    * structures.  It just needs to know how to type cast the structure.  Call
    * find_struct_index_for_name with last argument 1 or 2...1 for attr 2 for
    * var
    */

   int struct_index;

   /*printf("DEBUG\tinside update_ccmc_var_value...\nname = %s\nattr_name = %s\nnew_value = %d\n", var_name, attr_name, (int *) new_attr_value );*/

   /*** find index of variable structure containing var_name ***/

   /********************************************************* 1||2 see above **/

   struct_index = find_struct_index_for_name(
         var_name,
         put_variable_call_count,
         current_var_struct,
         2);

   /*** struct_index value, if -1 no element was found for given name.
    * Return control to calling program ***/

   if (struct_index == -1)
      return 1;

   /*printf("HI.  struct index found = %d\nwe were looking for %s and found %s\n", struct_index, var_name, current_var_struct[struct_index].variable_name );*/

   /** copmare attr_name with each attr_name in variable structure, if match is
    * found update attr_value with new_attr_value **/

   /** put variable size structure element before actual variable attributes -
    * this will be updated before writing actual variable values to baselined
    * structure **/

   if ( !strcmp(attr_name, "variable_size") )
   {
      /*    int dumy_size = *(( int * )new_attr_value);*/

      /*    printf("DEBUG\t dummy_size = %d\n", dumy_size );*/
      /*    printf("DEBUG\t should be setting variable size to %d\n", (int *)new_attr_value );*/
      current_var_struct[struct_index].variable_size
            = *(( int * )new_attr_value);
      /*    printf("DEBUG\t should have set variable size to %d\n", current_var_struct[struct_index].variable_size );*/
   }
   else if ( !strcmp(attr_name, "valid_min") )
   {
      current_var_struct[struct_index].valid_min
            = *(( float * ) new_attr_value);
   }
   else if ( !strcmp(attr_name, "valid_max") )
   {
      current_var_struct[struct_index].valid_max
            = *(( float * ) new_attr_value);
   }
   else if ( !strcmp(attr_name, "units") )
   {
      current_var_struct[struct_index].units = realloc(
            current_var_struct[struct_index].units,
            sizeof(char) * strlen( (char *) new_attr_value ) +1);
      strcpy(current_var_struct[struct_index].units, ( char *) new_attr_value );
   }
   else if ( !strcmp(attr_name, "grid_system") )
   {
      current_var_struct[struct_index].grid_system = realloc(
            current_var_struct[struct_index].grid_system,
            sizeof(char) * strlen( (char *) new_attr_value ) +1);
      strcpy(
            current_var_struct[struct_index].grid_system,
            ( char *) new_attr_value );
   }
   else if ( !strcmp(attr_name, "mask") )
   {
      current_var_struct[struct_index].mask = *(( float * ) new_attr_value);
   }
   else if ( !strcmp(attr_name, "description") )
   {
      current_var_struct[struct_index].description = realloc(
            current_var_struct[struct_index].description,
            sizeof(char) * strlen( (char *) new_attr_value ) +1);
      strcpy(
            current_var_struct[struct_index].description,
            ( char *) new_attr_value );
   }
   else if ( !strcmp(attr_name, "is_vector_component") )
   {
      current_var_struct[struct_index].is_vector_component
            = *(( int * ) new_attr_value);
   }
   else if ( !strcmp(attr_name, "position_grid_system") )
   {
      current_var_struct[struct_index].position_grid_system = realloc(
            current_var_struct[struct_index].position_grid_system,
            sizeof(char) * strlen( (char *) new_attr_value ) +1);
      strcpy(
            current_var_struct[struct_index].position_grid_system,
            ( char *) new_attr_value );
   }
   else if ( !strcmp(attr_name, "data_grid_system") )
   {
      current_var_struct[struct_index].data_grid_system = realloc(
            current_var_struct[struct_index].data_grid_system,
            sizeof(char) * strlen( (char *) new_attr_value ) +1);
      strcpy(
            current_var_struct[struct_index].data_grid_system,
            ( char *) new_attr_value );
   }
   else if ( !strcmp(attr_name, "actual_min") )
   {
      current_var_struct[struct_index].actual_min
            = *(( float * ) new_attr_value);
   }
   else if ( !strcmp(attr_name, "actual_max") )
   {
      current_var_struct[struct_index].actual_max
            = *(( float * ) new_attr_value);
   }
   else
   {
      printf("ERROR\t%s is not a registered variable attribute!!!\n", attr_name);
      return 1;
   }

   return 0;

}

int valid_attr_type(char *attr_type)
{

   /** these are the only valid attribute types currently supported **/

   char *attr_type1 = "global";
   char *attr_type2 = "model";
   char *attr_type3 = "variable";

   if ( !strcmp(attr_type1, attr_type) || !strcmp(attr_type2, attr_type)
         || !strcmp(attr_type3, attr_type) )
   {
      return 1;
   }
   else
   {
      return 0;
   }

}

int valid_attr_data_type(char *attr_data_type)
{

   /** these are the only valid attribute types currently supported **/

   char *attr_data_type1 = "int";
   char *attr_data_type2 = "float";
   char *attr_data_type3 = "double";
   char *attr_data_type4 = "char";

   if ( !strcmp(attr_data_type1, attr_data_type) || !strcmp(
         attr_data_type2,
         attr_data_type) || !strcmp(attr_data_type3, attr_data_type)
         || !strcmp(attr_data_type4, attr_data_type) )
   {
      return 1;
   }
   else
   {
      return 0;
   }

}

int valid_data_classification(char *data_classification)
{

   /** these are the only valid data_classification types currently supported **/

   char *data_classification_type1 = "position";
   char *data_classification_type2 = "data";
   char *data_classification_type3 = "grid";

   if ( !strcmp(data_classification_type1, data_classification) || !strcmp(
         data_classification_type2,
         data_classification) || !strcmp(
         data_classification_type3,
         data_classification) )
   {
      return 1;
   }
   else
   {
      return 0;
   }

}

int valid_var_data_type(char *var_data_type)
{

   /** these are the only valid attribute types currently supported **/

   char *var_data_type1 = "int";
   char *var_data_type2 = "float";
   char *var_data_type3 = "double";

   if ( !strcmp(var_data_type1, var_data_type) || !strcmp(
         var_data_type2,
         var_data_type) || !strcmp(var_data_type3, var_data_type) )
   {
      return 1;
   }
   else
   {
      return 0;
   }

}

int free_ccmc_attribute_structure(
      struct ccmc_attribute_structure *current_attr_struct,
      int number_of_attributes)
/*int free_ccmc_attribute_structure( int number_of_attributes )*/
{

   int i;

   for (i=0; i< number_of_attributes; i++)
   {

      /** The Attribute_value.string+value union member needs to be realesed if
       * type is char otherwise int, float, double do not need managing...I think ***/
      if ( !strcmp(current_attr_struct[i].attribute_data_type, "char") )
      {
         free(current_attr_struct[i].Attribute_value.string_value);
      }

      free(current_attr_struct[i].attribute_name);
      free(current_attr_struct[i].attribute_type);
      free(current_attr_struct[i].attribute_data_type);

   }

   free(current_attr_struct);

   /*** reset call count so array of structures begins at 0, not at the last
    * count of any previous init & set process ***/
   put_attribute_call_count = 0;

   return EXIT_SUCCESS;

}

int free_ccmc_variable_structure(
      struct ccmc_variable_structure *current_var_struct,
      int number_of_variables)
/*int free_ccmc_variable_structure( int number_of_variables )*/
{

   int i;

   for (i=0; i< number_of_variables; i++)
   {

      /** The Variable_values.* union member needs to be realesed  ***/

      if ( !strcmp(current_var_struct[i].variable_data_type, "int") )
      {
         free(current_var_struct[i].Variable_values.int_value);
      }
      else if ( !strcmp(current_var_struct[i].variable_data_type, "float") )
      {
         free(current_var_struct[i].Variable_values.float_value);
      }
      else if ( !strcmp(current_var_struct[i].variable_data_type, "double") )
      {
         free(current_var_struct[i].Variable_values.double_value);
      }

      free(current_var_struct[i].variable_name);
      free(current_var_struct[i].variable_data_type);
      free(current_var_struct[i].data_classification);
      free(current_var_struct[i].units);
      free(current_var_struct[i].grid_system);
      free(current_var_struct[i].description);
      free(current_var_struct[i].position_grid_system);
      free(current_var_struct[i].data_grid_system);
   }

   free(current_var_struct);

   /*** reset call count so array of structures begins at 0, not at the last
    * count of any previous init & set process ***/
   put_variable_call_count = 0;

   return EXIT_SUCCESS;

}

int print_ccmc_attribute_structure(
      struct ccmc_attribute_structure *current_attr_struct,
      int number_of_attributes)
{

   int i;

   for (i = 0; i < number_of_attributes; i++)
   {

      printf("\nContents of ccmc_attribute_structure ( %d )\n\n", i);

      printf(
            "current_attr_struct[%d].attribute_name = %s\n",
            i,
            current_attr_struct[i].attribute_name);
      printf(
            "current_attr_struct[%d].attribute_type = %s\n",
            i,
            current_attr_struct[i].attribute_type);
      printf(
            "current_attr_struct[%d].attribute_data_type = %s\n",
            i,
            current_attr_struct[i].attribute_data_type);

      if ( !strcmp(current_attr_struct[i].attribute_data_type, "char") )
      {
         printf(
               "current_attr_struct[%d].Attribute_value.string_value = %s\n",
               i,
               current_attr_struct[i].Attribute_value.string_value);
      }
      else if ( !strcmp(current_attr_struct[i].attribute_data_type, "int") )
      {
         printf(
               "current_attr_struct[%d].Attribute_value.string_value = %d\n",
               i,
               current_attr_struct[i].Attribute_value.int_value);
      }
      else if ( !strcmp(current_attr_struct[i].attribute_data_type, "float") )
      {
         printf(
               "current_attr_struct[%d].Attribute_value.string_value = %f\n",
               i,
               current_attr_struct[i].Attribute_value.float_value);
      }
      else if ( !strcmp(current_attr_struct[i].attribute_data_type, "double") )
      {
         printf(
               "current_attr_struct[%d].Attribute_value.string_value = %f\n",
               i,
               current_attr_struct[i].Attribute_value.double_value);
      }
      else
      {
         printf(
               "ERROR\tdata type %s not supported\n",
               current_attr_struct[i].attribute_data_type);
      }
   }

   return 0;

}

int print_ccmc_variable_structure(
      struct ccmc_variable_structure *current_var_struct,
      int number_of_variables)
{

   int i, j;

   for (i = 0; i < number_of_variables; i++)
   {

      printf("\nContents of ccmc_variable_structure ( %d )\n\n", i);

      printf(
            "current_var_struct[%d].variable_name = %s\n",
            i,
            current_var_struct[i].variable_name);
      printf(
            "current_var_struct[%d].variable_data_type = %s\n",
            i,
            current_var_struct[i].variable_data_type);
      printf(
            "current_var_struct[%d].variable_size = %d\n",
            i,
            current_var_struct[i].variable_size);
      printf(
            "current_var_struct[%d].data_classification = %s\n",
            i,
            current_var_struct[i].data_classification);

      if ( !strcmp(current_var_struct[i].variable_data_type, "int") )
      {
         for (j=0; j<current_var_struct[i].variable_size; j+=1000)
         {
            printf(
                  "%s[%d] = %d\n",
                  current_var_struct[i].variable_name,
                  j,
                  current_var_struct[i].Variable_values.int_value[j]);
         }
      }
      else if ( !strcmp(current_var_struct[i].variable_data_type, "float") )
      {
         for (j=0; j<current_var_struct[i].variable_size; j+=1000)
         {
            printf(
                  "%s[%d] = %f\n",
                  current_var_struct[i].variable_name,
                  j,
                  current_var_struct[i].Variable_values.float_value[j]);
         }
      }
      else if ( !strcmp(current_var_struct[i].variable_data_type, "double") )
      {
         for (j=0; j<current_var_struct[i].variable_size; j++)
         {
            printf(
                  "%s[%d] = %f\n",
                  current_var_struct[i].variable_name,
                  j,
                  current_var_struct[i].Variable_values.double_value[j]);
         }
      }
      else
      {
         printf(
               "ERROR\tdata type %s not supported\n",
               current_var_struct[i].variable_data_type);
      }

      printf(
            "current_var_struct[%d].valid_min = %f\n",
            i,
            current_var_struct[i].valid_min);
      printf(
            "current_var_struct[%d].valid_max = %f\n",
            i,
            current_var_struct[i].valid_max);
      printf(
            "current_var_struct[%d].units = %s\n",
            i,
            current_var_struct[i].units);
      printf(
            "current_var_struct[%d].grid_system = %s\n",
            i,
            current_var_struct[i].grid_system);
      printf(
            "current_var_struct[%d].mask = %f\n",
            i,
            current_var_struct[i].mask);
      printf(
            "current_var_struct[%d].description = %s\n",
            i,
            current_var_struct[i].description);
      printf(
            "current_var_struct[%d].is_vector_component = %d\n",
            i,
            current_var_struct[i].is_vector_component);
      printf(
            "current_var_struct[%d].position_grid_system = %s\n",
            i,
            current_var_struct[i].position_grid_system);
      printf(
            "current_var_struct[%d].data_grid_system = %s\n",
            i,
            current_var_struct[i].data_grid_system);
      printf(
            "current_var_struct[%d].actual_min = %f\n",
            i,
            current_var_struct[i].actual_min);
      printf(
            "current_var_struct[%d].actual_max = %f\n",
            i,
            current_var_struct[i].actual_max);
   }

   return 0;

}

int find_struct_index_for_name(
      char *search_name,
      int struct_count,
      void *current_struct,
      int ccmc_struct_key)
{

   int i;

   /*****
    ccmc_struct_key = 1 for ccmc_attribute_structure
    ccmc_struct_key = 2 for ccmc_variable_structure
    *****/

   struct ccmc_attribute_structure *local_attr_struct;
   struct ccmc_variable_structure *local_var_struct;

   switch (ccmc_struct_key)
   {
      case 1:

         local_attr_struct
               = ( struct ccmc_attribute_structure * ) current_struct;

         /*** for every structure in array ***/

         for (i = 0; i < struct_count; i++)
         {

            /*** if a structures attribute name == what we are looking for... ***/

            /*printf("DEBUG\tcurrent_struct[%d].attribute_name = %s\n", i, local_attr_struct[i].attribute_name);*/

            if ( !strcmp(search_name, local_attr_struct[i].attribute_name) )
            {
               return i;
            }

         }
         break;
      case 2:
         local_var_struct = ( struct ccmc_variable_structure * ) current_struct;

         /*** for every structure in array ***/

         for (i = 0; i < struct_count; i++)
         {

            /*** if a structures variable name == what we are looking for... ***/

            /*printf("DEBUG\tcurrent_struct[%d].variable_name = %s\n", i, local_var_struct[i].variable_name);*/

            if ( !strcmp(search_name, local_var_struct[i].variable_name) )
            {
               return i;
            }

         }
         break;
      default:
         printf(
               "ERROR: Invalid struture key:\tccmc_struct_key = %d...Only 1 & 2 "
               "are defined for ccmc_attribute_structure and "
               "ccmc_variable_structure respectively.\n",
               ccmc_struct_key);
         return -1;
         break;
   }

   /*** if we get here, the given attribute name was not found.  Throw an error
    * and .. return -1 ***/
   printf(
         "ERROR:\t could NOT find structure element ------->%s<------- "
         "skipping....\n",
         search_name);

   return -1;

}
