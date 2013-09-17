#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "kameleon.h"

int main(int argc, char *argv[])
{

    char cdf_name[250];

    /***************** declared local pointers to store returned attribute value pointers ************/

    char *char_attribute_valuePtr;
    int *int_attribute_valuePtr;
    float *float_attribute_valuePtr;

    if (argc != 2)
    {
        printf("ERROR:this example program requires 1 argument - full path and file name of data file\n");
        exit( EXIT_FAILURE);
    }

    strcpy(cdf_name, argv[1]);

    open_cdf(cdf_name, 0);

    printf("Open %s complete...\n", cdf_name);

    /*** get the README GLOBAL ATTRIBUTE value ***/

    char_attribute_valuePtr = gattribute_get("README");

    if (char_attribute_valuePtr != NULL)
    {
        printf("\n\nREADME attribute = %s\n", char_attribute_valuePtr);
    }

    /*** get the grid_system_count global attribute value ***/

    int_attribute_valuePtr = gattribute_get("grid_system_count");

    if (int_attribute_valuePtr != NULL)
    {
        printf("\ngrid_system_count attribute = %d\n", *int_attribute_valuePtr);
    }

    /*** get the global_x_min global attribute value ***/

    float_attribute_valuePtr = gattribute_get("elapsed_time_in_seconds");

    if (float_attribute_valuePtr != NULL)
    {
        printf("\nelapsed_time_in_seconds attribute = %f\n",
                *float_attribute_valuePtr);
    }

    /*** get the description variable ATTRIBUTE value for x ***/

    char_attribute_valuePtr = vattribute_get("x", "description");

    if (char_attribute_valuePtr != NULL)
    {
        printf("\ndescription attribute = %s\n", char_attribute_valuePtr);
    }

    /*** get the is_vector_component variable attribute value for bx ***/

    int_attribute_valuePtr = vattribute_get("bx", "is_vector_component");

    if (int_attribute_valuePtr != NULL)
    {
        printf("\nis_vector_component attribute = %d\n",
                *int_attribute_valuePtr);
    }

    /*** get the valid_min variable attribute value for rho***/

    float_attribute_valuePtr = vattribute_get("rho", "valid_min");

    if (float_attribute_valuePtr != NULL)
    {
        printf("\nvalid_min attribute = %f\n", *float_attribute_valuePtr);
    }

    /*** get the units variable attribute value for rho***/

    char_attribute_valuePtr = vattribute_get("rho", "units");

    if (char_attribute_valuePtr != NULL)
    {
        printf("\nunits attribute = %s\n", char_attribute_valuePtr);
    }

    printf("DEBUG\tlegacy stuff\n\n\n\n");

    /********** legacy routines... before generic *attribute_get routines were available ***/

    /** char * get_units( "variable_name" ); **/

    printf(
            "\nAN example of using the get_units() routine:  units for X = %s\n",
            get_units("x") );

    /*** float gattribute_float_get( "global_attribute_name" ); **/

    printf(
            "\nAN example of using the gattribute_float_get() routine:  elapsed_time_in_seconds %f\n",
            gattribute_float_get("elapsed_time_in_seconds") );

    /*** char * gattribute_char_get( "global_attribute_name" ); **/

    printf(
            "\nAN example of using the gattribute_char_get() routine, model_name = %s\n",
            gattribute_char_get("model_name") );

    return 1;
}
