#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>

#include "ccmc_structure_manager.h"
#include "cdf.h"

int main()
{
   extern int write_structures_2_cdf(
         char *,
         char *,
         int,
         int,
         char *,
         char *,
         char *);

   char *input_file_name = "magick_input_files";
   char *data_format = "cdf";

   int file_name_length = 18;
   int verbose_flag = 1;

   char *output_directory = "./";
   char *model_name = "magick";
   char *original_working_directory = "./";

   int stat, i;

   float x[100];
   float y[100];
   float z[100];

   float dummy_var[10000];

   char *x_name = "x";
   char *y_name = "y";
   char *z_name = "z";

   char *x_units = "inches";
   char *y_units = "inches";
   char *z_units = "inches";

   int grid_system_count = 1;

   char *readme =
         "This file was created using calls to kameleon converter internals...";

   char *dummy_var_units = "gallons";
   char
         *dummy_var_desription =
               "Dummy variable created to show how kameleons structure manager can be used externally";

   char *kameleon_version_num = "KAMELEON CONVERTER LIBRARY PROTOTYPE";

   /**** initialize our attribute structure that will contain all of the global metadata for this file ***/

   baselined_ccmc_attribute_structure_list = init_ccmc_attribute_structure( 3);

   /**** initialize our variable structure that will contain all of the variables and variable metadata for this file ***/

   baselined_ccmc_variable_structure_list = init_ccmc_variable_structure( 3);

   /** now we can actually put attributes into our newly created baselined_ccmc_attribute_structure_list **/
   /** NOTE:  Keep track of the size of our created structure above, we only specifid 3 attributes so we
    * only put 3 ***/

   printf("INFO:\tcalling put_ccmc_attribute() for grid_system_count\n");

   put_ccmc_attribute(
         "grid_system_count",
         "global",
         "int",
         &grid_system_count,
         baselined_ccmc_attribute_structure_list);

   printf("INFO:\tcalling put_ccmc_attribute() for README\n");

   put_ccmc_attribute(
         "README",
         "global",
         "char",
         "",
         baselined_ccmc_attribute_structure_list);

   printf("INFO:\tcalling put_ccmc_attribute() for kameleon_version\n");

   put_ccmc_attribute(
         "kameleon_version",
         "global",
         "char",
         "This IS THE INITIAL VERSION VALUE THAT WILL BE UPDATED",
         baselined_ccmc_attribute_structure_list);

   printf("INFO:\tcalling write_structures_2_cdf\n");

   printf("INFO\tCONTENTS OF baselined_ccmc_attribute_structure_list\n\n\n");

   print_ccmc_attribute_structure(
         baselined_ccmc_attribute_structure_list,
         3);

   /** an example of how to update an already existing atribute **/

   printf("INFO:\tcalling update_ccmc_attribute_value() for kameleon_version\n");

   stat = update_ccmc_attribute_value(
         "kameleon_version",
         kameleon_version_num,
         baselined_ccmc_attribute_structure_list);

   stat = update_ccmc_attribute_value(
         "README",
         readme,
         baselined_ccmc_attribute_structure_list);

   printf("INFO\tCONTENTS OF baselined_ccmc_attribute_structure_list\n\n\n");

   print_ccmc_attribute_structure(
         baselined_ccmc_attribute_structure_list,
         3);

   /** lets generate dummy data **/

   for (i=0; i < 100; i++)
   {
      x[i]=y[i]=z[i] = i;
   }

   for (i=0; i < 10000; i++)
   {
      dummy_var[i] = i;
   }

   /*** put our variables and variable metadata into the variable structure **/
   /*
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
    */

   /* Each Variable and subsequent variable structure contains not only the variable
    * name and values but also the variable metadta.  Calls to put include the specification of all
    * varaible attributes as listed below with data types listed above
    *
    *
    * name
    * data type
    * variable size
    * variable classification ie position or data
    * array of data values
    * valid min
    * valid max
    * units
    * grid_system
    * mask
    * description
    * is_vector_component
    * position_grid_system,
    * data_grid_system,
    * actual_min,
    * actual_max,
    * structure to write to
    */

   /** put our dummy x position variable **/

   printf("INFO:\tcalling put_ccmc_variable() for x\n");

   put_ccmc_variable(
         x_name,
         "float",
         100,
         "position",
         &x,
         -100.0,
         100.0,
         x_units,
         "GRID TYPE GOES HERE ie GSM, GSE...",
         -99999.0,
         "Varible Descriptioin Goes Here",
         0,
         "If Variable is only a special grid for positions only, we would specify that here",
         "If Variable is only a special grid for data only, we would specify that here",
         -100.0,
         100.0,
         baselined_ccmc_variable_structure_list);

   printf("INFO\tCONTENTS OF baselined_ccmc_attribute_structure_list\n\n\n");

   print_ccmc_attribute_structure(
         baselined_ccmc_attribute_structure_list,
         3);

   /** put our dummy y position variable **/

   printf("INFO:\tcalling put_ccmc_variable() for y\n");

   put_ccmc_variable(
         y_name,
         "float",
         100,
         "position",
         &y,
         -100,
         100,
         y_units,
         "GRID TYPE GOES HERE ie GSM, GSE...",
         -99999,
         "Varible Descriptioin Goes Here",
         0,
         "If Variable is only a special grid for positions only, we would specify that here",
         "If Variable is only a special grid for data only, we would specify that here",
         -100,
         100,
         baselined_ccmc_variable_structure_list);

   printf("INFO\tCONTENTS OF baselined_ccmc_attribute_structure_list\n\n\n");

   print_ccmc_attribute_structure(
         baselined_ccmc_attribute_structure_list,
         3);

   /** put our dummy z position variable **/

   printf("INFO:\tcalling put_ccmc_variable() for z\n");

   put_ccmc_variable(
         z_name,
         "float",
         100,
         "position",
         &z,
         -100,
         100,
         z_units,
         "GRID TYPE GOES HERE ie GSM, GSE...",
         -99999,
         "Varible Descriptioin Goes Here",
         0,
         "If Variable is only a special grid for positions only, we would specify that here",
         "If Variable is only a special grid for data only, we would specify that here",
         -100,
         100,
         baselined_ccmc_variable_structure_list);

   printf("INFO\tCONTENTS OF baselined_ccmc_attribute_structure_list\n\n\n");

   print_ccmc_attribute_structure(
         baselined_ccmc_attribute_structure_list,
         3);

   /** put our dummy dummy_var position variable **/

   printf("INFO:\tcalling put_ccmc_variable() for dummy_variable\n");

   put_ccmc_variable(
         "dummy_variable",
         "float",
         10000,
         "data",
         &dummy_var,
         -10000,
         10000,
         dummy_var_units,
         "GRID TYPE GOES HERE ie GSM, GSE...",
         -99999,
         dummy_var_desription,
         0,
         "If Variable is only a special grid for positions only, we would specify that here",
         "If Variable is only a special grid for data only, we would specify that here",
         -10000,
         10000,
         baselined_ccmc_variable_structure_list);

   /** showing how to update individual variable attribute values **/

   printf("INFO:\tcalling update_ccmc_variable_attribute_value() for z description\n");

   stat = update_ccmc_variable_attribute_value(
         "z",
         "description",
         "Lets Change Z's description from the defaults",
         baselined_ccmc_variable_structure_list);

   printf("INFO:\tcalling write_structures_2_cdf\n");

   printf("INFO\tCONTENTS OF baselined_ccmc_attribute_structure_list\n\n\n");

   print_ccmc_attribute_structure(
         baselined_ccmc_attribute_structure_list,
         3);

   write_structures_2_cdf(
         input_file_name,
         data_format,
         file_name_length,
         verbose_flag,
         output_directory,
         model_name,
         original_working_directory);

}
