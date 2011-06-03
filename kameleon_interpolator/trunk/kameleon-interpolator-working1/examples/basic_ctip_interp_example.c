#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "kameleon.h"

int main(int argc, char *argv[])
{

    /*char *cdf_name = "/export/home/mmaddox/test_data/test_cdf_data/3d__ful_1_20000101_000000_0000.out.cdf";*/

    /*char *cdf_name = "3d__ful_1_n200344.out.cdf";*/

    char cdf_name[250];
    float x, y, z, interp_value;

    char *variable_name = "Rmt";
    char *variable_name1 = "Tn";
    char *variable_name2 = "sigmaP";
    char *variable_name3 = "N_e";
    char *variable_name4 = "N_O";
    char *variable_name5 = "N_O2";
    char *variable_name6 = "N_N2";
    char *variable_name7 = "Vn_x";
    char *variable_name8 = "Vn_y";
    char *variable_name9 = "Vn_z";
    char *variable_name10 = "Vi_x";
    char *variable_name11 = "Vi_y";
    char *variable_name12 = "H";

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

    interp_value = interpolate_ctip_cdf(variable_name, x, y, z, 0, 0);
    printf("_______________________________________________________________________________________________\n");
    printf("%s [ %f, %f, %f ]\t%g %s\n", variable_name, x, y, z, interp_value,
            get_units(variable_name) );

    interp_value = interpolate_ctip_cdf(variable_name1, x, y, z, 0, 0);
    printf("_______________________________________________________________________________________________\n");
    printf("%s [ %f, %f, %f ]\t%g %s\n", variable_name1, x, y, z, interp_value,
            get_units(variable_name1) );

    interp_value = interpolate_ctip_cdf(variable_name2, x, y, z, 0, 0);
    printf("_______________________________________________________________________________________________\n");
    printf("%s [ %f, %f, %f ]\t%g %s\n", variable_name2, x, y, z, interp_value,
            get_units(variable_name2) );

    interp_value = interpolate_ctip_cdf(variable_name3, x, y, z, 0, 0);
    printf("_______________________________________________________________________________________________\n");
    printf("%s [ %f, %f, %f ]\t%g %s\n", variable_name3, x, y, z, interp_value,
            get_units(variable_name3) );

    interp_value = interpolate_ctip_cdf(variable_name4, x, y, z, 0, 0);
    printf("_______________________________________________________________________________________________\n");
    printf("%s [ %f, %f, %f ]\t%g %s\n", variable_name4, x, y, z, interp_value,
            get_units(variable_name4) );

    interp_value = interpolate_ctip_cdf(variable_name5, x, y, z, 0, 0);
    printf("_______________________________________________________________________________________________\n");
    printf("%s [ %f, %f, %f ]\t%g %s\n", variable_name5, x, y, z, interp_value,
            get_units(variable_name5) );

    interp_value = interpolate_ctip_cdf(variable_name6, x, y, z, 0, 0);
    printf("_______________________________________________________________________________________________\n");
    printf("%s [ %f, %f, %f ]\t%g %s\n", variable_name6, x, y, z, interp_value,
            get_units(variable_name6) );

    interp_value = interpolate_ctip_cdf(variable_name7, x, y, z, 0, 0);
    printf("_______________________________________________________________________________________________\n");
    printf("%s [ %f, %f, %f ]\t%g %s\n", variable_name7, x, y, z, interp_value,
            get_units(variable_name7) );

    interp_value = interpolate_ctip_cdf(variable_name8, x, y, z, 0, 0);
    printf("_______________________________________________________________________________________________\n");
    printf("%s [ %f, %f, %f ]\t%g %s\n", variable_name8, x, y, z, interp_value,
            get_units(variable_name8) );

    interp_value = interpolate_ctip_cdf(variable_name9, x, y, z, 0, 0);
    printf("_______________________________________________________________________________________________\n");
    printf("%s [ %f, %f, %f ]\t%g %s\n", variable_name9, x, y, z, interp_value,
            get_units(variable_name9) );

    interp_value = interpolate_ctip_cdf(variable_name10, x, y, z, 0, 0);
    printf("_______________________________________________________________________________________________\n");
    printf("%s [ %f, %f, %f ]\t%g %s\n", variable_name10, x, y, z,
            interp_value, get_units(variable_name10) );

    interp_value = interpolate_ctip_cdf(variable_name11, x, y, z, 0, 0);
    printf("_______________________________________________________________________________________________\n");
    printf("%s [ %f, %f, %f ]\t%g %s\n", variable_name11, x, y, z,
            interp_value, get_units(variable_name11) );

    interp_value = interpolate_ctip_cdf(variable_name12, x, y, z, 0, 0);
    printf("_______________________________________________________________________________________________\n");
    printf("%s [ %f, %f, %f ]\t%g %s\n", variable_name12, x, y, z,
            interp_value, get_units(variable_name12) );

    printf("DEBUG\t about to close cdf\n");

    close_cdf();

    return 1;

}

