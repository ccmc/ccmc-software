#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "kameleon.h"

int main(int argc, char *argv[])
{

    /*char *cdf_name = "/export/home/mmaddox/test_data/test_cdf_data/3d__ful_1_20000101_000000_0000.out.cdf";*/

    /*char *cdf_name = "3d__ful_1_n200344.out.cdf";*/

    char cdf_name[250];
    double x, y, z, interp_value;

    char *variable_name = "rho";
    char *variable_name1 = "T";
    char *variable_name2 = "ux";
    char *variable_name3 = "uy";
    char *variable_name4 = "uz";
    char *variable_name5 = "bx";
    char *variable_name6 = "by";
    char *variable_name7 = "bz";
    char *variable_name8 = "bp";

    if (argc != 5)
    {
        printf("ERROR:this example program requires 4 arguments - (1)full path and file name of data file (2) x position (3) y position (4) z position \n");
        exit( EXIT_FAILURE);
    }

    strcpy(cdf_name, argv[1]);

    x = atof(argv[2]);
    y = atof(argv[3]);
    z = atof(argv[4]);

    /*printf("EXAMPLE 1: all variables no memory\n"); */

    printf("\ncalling open_cdf\n");

    open_cdf(cdf_name, 0);

    printf("\nopen_cdf complete\n\n\n");

    interp_value = interpolate_enlil_cdf(variable_name, x, y, z, 0, 0);
    printf("_______________________________________________________________________________________________\n");
    printf("%s [ %f, %f, %f ]\t%g %s\n", variable_name, x, y, z, interp_value,
            get_units(variable_name) );

    interp_value = interpolate_enlil_cdf(variable_name1, x, y, z, 0, 0);
    printf("_______________________________________________________________________________________________\n");
    printf("%s [ %f, %f, %f ]\t%g %s\n", variable_name1, x, y, z, interp_value,
            get_units(variable_name1) );

    interp_value = interpolate_enlil_cdf(variable_name2, x, y, z, 0, 0);
    printf("_______________________________________________________________________________________________\n");
    printf("%s [ %f, %f, %f ]\t%g %s\n", variable_name2, x, y, z, interp_value,
            get_units(variable_name2) );

    interp_value = interpolate_enlil_cdf(variable_name3, x, y, z, 0, 0);
    printf("_______________________________________________________________________________________________\n");
    printf("%s [ %f, %f, %f ]\t%g %s\n", variable_name3, x, y, z, interp_value,
            get_units(variable_name3) );

    interp_value = interpolate_enlil_cdf(variable_name4, x, y, z, 0, 0);
    printf("_______________________________________________________________________________________________\n");
    printf("%s [ %f, %f, %f ]\t%g %s\n", variable_name4, x, y, z, interp_value,
            get_units(variable_name4) );

    interp_value = interpolate_enlil_cdf(variable_name5, x, y, z, 0, 0);
    printf("_______________________________________________________________________________________________\n");
    printf("%s [ %f, %f, %f ]\t%g %s\n", variable_name5, x, y, z, interp_value,
            get_units(variable_name5) );

    interp_value = interpolate_enlil_cdf(variable_name6, x, y, z, 0, 0);
    printf("_______________________________________________________________________________________________\n");
    printf("%s [ %f, %f, %f ]\t%g %s\n", variable_name6, x, y, z, interp_value,
            get_units(variable_name6) );

    interp_value = interpolate_enlil_cdf(variable_name7, x, y, z, 0, 0);
    printf("_______________________________________________________________________________________________\n");
    printf("%s [ %f, %f, %f ]\t%g %s\n", variable_name7, x, y, z, interp_value,
            get_units(variable_name7) );

    interp_value = interpolate_enlil_cdf(variable_name8, x, y, z, 0, 0);
    printf("_______________________________________________________________________________________________\n");
    printf("%s [ %f, %f, %f ]\t%g %s\n", variable_name8, x, y, z, interp_value,
            get_units(variable_name8) );

    printf("DEBUG\t about to close cdf\n");

    close_cdf();

    return 1;

}

