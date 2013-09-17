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

    if (attribute_exists("README") )
        char_attribute_valuePtr = gattribute_get("README");
    if (attribute_exists("README") )
    {
        printf("\n\nREADME attribute = %s\n", char_attribute_valuePtr);
    }

    /*** get the grid_system_count global attribute value ***/

    if (attribute_exists("grid_system_count") )
        int_attribute_valuePtr = gattribute_get("grid_system_count");
    if (attribute_exists("grid_system_count") )
    {
        printf("\ngrid_system_count attribute = %d\n\n",
                *int_attribute_valuePtr);
    }

    /*** test for the existence of gattributes ***/

    printf("attribute_exists( \"model_name\" ) = %d\n",
            attribute_exists("model_name") );
    printf("attribute_exists( \"model_type\" ) = %d\n",
            attribute_exists("model_type") );
    printf("attribute_exists( \"elapsed_time_in_seconds\") = %d\n",
            attribute_exists("elapsed_time_in_seconds") );
    printf("attribute_exists( \"horse_power\") = %d\n",
            attribute_exists("horse_power") );
    printf("attribute_exists( \"units\") = %d\n", attribute_exists("units") );

    /*** get the elapsed_time_in_seconds global attribute value ***/

    if (attribute_exists("elapsed_time_in_seconds") )
        float_attribute_valuePtr = gattribute_get("elapsed_time_in_seconds");
    if (attribute_exists("elapsed_time_in_seconds") )
    {
        printf("\nelapsed_time_in_seconds attribute = %f\n",
                *float_attribute_valuePtr);
    }

    /*** get the description variable ATTRIBUTE value for x ***/

    if (var_exists("x") && attribute_exists("description") )
        char_attribute_valuePtr = vattribute_get("x", "description");
    if (var_exists("x") && attribute_exists("description") )
    {
        printf("\ndescription attribute = %s\n", char_attribute_valuePtr);
    }

    /*** get the is_vector_component variable attribute value for bx ***/

    if (var_exists("bx") && attribute_exists("is_vector_component") )
        int_attribute_valuePtr = vattribute_get("bx", "is_vector_component");
    if (var_exists("bx") && attribute_exists("is_vector_component") )
    {
        printf("\nis_vector_component attribute = %d\n",
                *int_attribute_valuePtr);
    }

    /*** get the valid_min variable attribute value for rho***/

    if (var_exists("rho") && attribute_exists("valid_min") )
        float_attribute_valuePtr = vattribute_get("rho", "valid_min");
    if (var_exists("rho") && attribute_exists("valid_min") )
    {
        printf("\nvalid_min attribute = %f\n", *float_attribute_valuePtr);
    }

    /*** get the units variable attribute value for rho***/

    if (var_exists("rho") && attribute_exists("units") )
        char_attribute_valuePtr = vattribute_get("rho", "units");
    if (var_exists("rho") && attribute_exists("units") )
    {
        printf("\nunits attribute = %s\n", char_attribute_valuePtr);
    }

    printf("DEBUG\tlegacy stuff\n\n\n\n");

    /********** legacy routines... before generic *attribute_get routines were available ***/

    /** char * get_units( "variable_name" ); **/

    if (var_exists("x") && attribute_exists("units") )
        printf(
                "\nAN example of using the get_units() routine:  units for X = %s\n",
                get_units("x") );

    /*** float gattribute_float_get( "global_attribute_name" ); **/

    if (attribute_exists("elapsed_time_in_seconds") )
        printf(
                "\nAN example of using the gattribute_float_get() routine:  elapsed_time_in_seconds %f\n",
                gattribute_float_get("elapsed_time_in_seconds") );

    /*** char * gattribute_char_get( "global_attribute_name" ); **/

    if (attribute_exists("model_name") )
        printf(
                "\nAN example of using the gattribute_char_get() routine, model_name = %s\n",
                gattribute_char_get("model_name") );

    return 1;
}

