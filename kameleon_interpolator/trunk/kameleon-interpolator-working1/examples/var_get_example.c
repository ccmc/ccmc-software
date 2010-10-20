#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "kameleon.h"

#define DEBUG 0

int main(int argc, char *argv[])
{

    float *var_arrayPtr, *var_arrayPtr2;
    int array_size;

    int i;
    char cdf_name[250];
    char variable_name[50];

    if (argc != 3)
    {
        printf("ERROR:this example program requires 2 arguments - (1)full path and file name of data file (2) variable_name \n");
        exit( EXIT_FAILURE);
    }

    strcpy(cdf_name, argv[1]);
    strcpy(variable_name, argv[2]);

    fprintf( stderr, "\n%s\tINFO: Reading variable %s from %s ... \n", __FILE__, variable_name, cdf_name);

    var_arrayPtr = var_get(cdf_name, variable_name, &array_size);

    if (var_arrayPtr != NULL)
    {

        printf("Complete.  number of values read = %d\n", array_size);
        printf("Displaying every 10th value\n");

        for (i=0; i<array_size; i+=10)
        {

            printf("%s[%d] = %f\n", variable_name, i, var_arrayPtr[i]);

        }
        /*
         return EXIT_SUCCESS;
         */
    }

    /** now lets try a case where a cdf file has already be opened **/

    open_cdf(cdf_name, 0);

    fprintf( stderr, "\n%s\tINFO: Reading variable %s from %s ... \n", __FILE__, variable_name, cdf_name);

    var_arrayPtr2 = var_get(cdf_name, variable_name, &array_size);

    if (var_arrayPtr != NULL)
    {

        printf("Complete.  number of values read = %d\n", array_size);
        printf("Displaying every 10th value\n");

        for (i=0; i<array_size; i+=10)
        {

            printf("%s[%d] = %f\n", variable_name, i, var_arrayPtr2[i]);

        }

        /*
         return EXIT_SUCCESS;
         */
    }

    /** now lets try a case where a cdf file has already be opened with main memory **/

    open_cdf(cdf_name, 1, "rho");

    fprintf( stderr, "\n%s\tINFO: Reading variable %s from %s ... \n", __FILE__, variable_name, cdf_name);

    var_arrayPtr2 = var_get(cdf_name, variable_name, &array_size);

    if (var_arrayPtr != NULL)
    {

        printf("Complete.  number of values read = %d\n", array_size);
        printf("Displaying every 10th value\n");

        for (i=0; i<array_size; i+=10)
        {

            printf("%s[%d] = %f\n", variable_name, i, var_arrayPtr2[i]);

        }

        return EXIT_SUCCESS;

    }

    return EXIT_FAILURE;

}

