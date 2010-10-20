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

    char *variable_name = "rho";
    char *variable_name1 = "bx1";
    char *variable_name2 = "by1";
    char *variable_name3 = "bz1";
    char *variable_name4 = "bx";
    char *variable_name5 = "by";
    char *variable_name6 = "bz";
    char *variable_name7 = "ux";
    char *variable_name8 = "uy";
    char *variable_name9 = "uz";
    char *variable_name10 = "jx";
    char *variable_name11 = "jy";
    char *variable_name12 = "jz";
    char *variable_name13 = "p";

    if (argc != 5)
    {
        printf("ERROR:this example program requires 4 arguments - (1)full path and file name of data file (2) x position (3) y position (4) z position \n");
        exit( EXIT_FAILURE);
    }

    strcpy(cdf_name, argv[1]);

    x = atof(argv[2]);
    y = atof(argv[3]);
    z = atof(argv[4]);

    open_cdf(cdf_name, 14, variable_name, variable_name1, variable_name2,
            variable_name3, variable_name4, variable_name5, variable_name6,
            variable_name7, variable_name8, variable_name9, variable_name10,
            variable_name11, variable_name12, variable_name13);

    printf("\nopen_cdf complete\n");

    interp_value = interpolate_ucla_ggcm_cdf(variable_name, x, y, z, 1, 0);

    printf("%s [ %f, %f, %f ]\t%g %s\n", variable_name, x, y, z, interp_value,
            get_units(variable_name) );

    interp_value = interpolate_ucla_ggcm_cdf(variable_name1, x, y, z, 1, 0);

    printf("%s [ %f, %f, %f ]\t%g %s\n", variable_name1, x, y, z, interp_value,
            get_units(variable_name1) );

    interp_value = interpolate_ucla_ggcm_cdf(variable_name2, x, y, z, 1, 0);

    printf("%s [ %f, %f, %f ]\t%g %s\n", variable_name2, x, y, z, interp_value,
            get_units(variable_name2) );

    interp_value = interpolate_ucla_ggcm_cdf(variable_name3, x, y, z, 1, 0);

    printf("%s [ %f, %f, %f ]\t%g %s\n", variable_name3, x, y, z, interp_value,
            get_units(variable_name3) );

    interp_value = interpolate_ucla_ggcm_cdf(variable_name4, x, y, z, 1, 0);

    printf("%s [ %f, %f, %f ]\t%g %s\n", variable_name4, x, y, z, interp_value,
            get_units(variable_name4) );

    interp_value = interpolate_ucla_ggcm_cdf(variable_name5, x, y, z, 1, 0);

    printf("%s [ %f, %f, %f ]\t%g %s\n", variable_name5, x, y, z, interp_value,
            get_units(variable_name5) );

    interp_value = interpolate_ucla_ggcm_cdf(variable_name6, x, y, z, 1, 0);

    printf("%s [ %f, %f, %f ]\t%g %s\n", variable_name6, x, y, z, interp_value,
            get_units(variable_name6) );

    interp_value = interpolate_ucla_ggcm_cdf(variable_name7, x, y, z, 1, 0);

    printf("%s [ %f, %f, %f ]\t%g %s\n", variable_name7, x, y, z, interp_value,
            get_units(variable_name7) );

    interp_value = interpolate_ucla_ggcm_cdf(variable_name8, x, y, z, 1, 0);

    printf("%s [ %f, %f, %f ]\t%g %s\n", variable_name8, x, y, z, interp_value,
            get_units(variable_name8) );

    interp_value = interpolate_ucla_ggcm_cdf(variable_name9, x, y, z, 1, 0);

    printf("%s [ %f, %f, %f ]\t%g %s\n", variable_name9, x, y, z, interp_value,
            get_units(variable_name9) );

    interp_value = interpolate_ucla_ggcm_cdf(variable_name10, x, y, z, 1, 0);

    printf("%s [ %f, %f, %f ]\t%g %s\n", variable_name10, x, y, z,
            interp_value, get_units(variable_name10) );

    interp_value = interpolate_ucla_ggcm_cdf(variable_name11, x, y, z, 1, 0);

    printf("%s [ %f, %f, %f ]\t%g %s\n", variable_name11, x, y, z,
            interp_value, get_units(variable_name11) );

    interp_value = interpolate_ucla_ggcm_cdf(variable_name12, x, y, z, 1, 0);

    printf("%s [ %f, %f, %f ]\t%g %s\n", variable_name12, x, y, z,
            interp_value, get_units(variable_name12) );

    interp_value = interpolate_ucla_ggcm_cdf(variable_name13, x, y, z, 1, 0);

    printf("%s [ %f, %f, %f ]\t%g %s\n", variable_name13, x, y, z,
            interp_value, get_units(variable_name13) );

    close_cdf();

    open_cdf(cdf_name, 14, variable_name, variable_name1, variable_name2,
            variable_name3, variable_name4, variable_name5, variable_name6,
            variable_name7, variable_name8, variable_name9, variable_name10,
            variable_name11, variable_name12, variable_name13);

    printf("\nopen_cdf complete\n");

    interp_value = interpolate_ucla_ggcm_cdf(variable_name, x, y, z, 1, 0);

    printf("%s [ %f, %f, %f ]\t%g %s\n", variable_name, x, y, z, interp_value,
            get_units(variable_name) );

    interp_value = interpolate_ucla_ggcm_cdf(variable_name1, x, y, z, 1, 0);

    printf("%s [ %f, %f, %f ]\t%g %s\n", variable_name1, x, y, z, interp_value,
            get_units(variable_name1) );

    interp_value = interpolate_ucla_ggcm_cdf(variable_name2, x, y, z, 1, 0);

    printf("%s [ %f, %f, %f ]\t%g %s\n", variable_name2, x, y, z, interp_value,
            get_units(variable_name2) );

    interp_value = interpolate_ucla_ggcm_cdf(variable_name3, x, y, z, 1, 0);

    printf("%s [ %f, %f, %f ]\t%g %s\n", variable_name3, x, y, z, interp_value,
            get_units(variable_name3) );

    interp_value = interpolate_ucla_ggcm_cdf(variable_name4, x, y, z, 1, 0);

    printf("%s [ %f, %f, %f ]\t%g %s\n", variable_name4, x, y, z, interp_value,
            get_units(variable_name4) );

    interp_value = interpolate_ucla_ggcm_cdf(variable_name5, x, y, z, 1, 0);

    printf("%s [ %f, %f, %f ]\t%g %s\n", variable_name5, x, y, z, interp_value,
            get_units(variable_name5) );

    interp_value = interpolate_ucla_ggcm_cdf(variable_name6, x, y, z, 1, 0);

    printf("%s [ %f, %f, %f ]\t%g %s\n", variable_name6, x, y, z, interp_value,
            get_units(variable_name6) );

    interp_value = interpolate_ucla_ggcm_cdf(variable_name7, x, y, z, 1, 0);

    printf("%s [ %f, %f, %f ]\t%g %s\n", variable_name7, x, y, z, interp_value,
            get_units(variable_name7) );

    interp_value = interpolate_ucla_ggcm_cdf(variable_name8, x, y, z, 1, 0);

    printf("%s [ %f, %f, %f ]\t%g %s\n", variable_name8, x, y, z, interp_value,
            get_units(variable_name8) );

    interp_value = interpolate_ucla_ggcm_cdf(variable_name9, x, y, z, 1, 0);

    printf("%s [ %f, %f, %f ]\t%g %s\n", variable_name9, x, y, z, interp_value,
            get_units(variable_name9) );

    interp_value = interpolate_ucla_ggcm_cdf(variable_name10, x, y, z, 1, 0);

    printf("%s [ %f, %f, %f ]\t%g %s\n", variable_name10, x, y, z,
            interp_value, get_units(variable_name10) );

    interp_value = interpolate_ucla_ggcm_cdf(variable_name11, x, y, z, 1, 0);

    printf("%s [ %f, %f, %f ]\t%g %s\n", variable_name11, x, y, z,
            interp_value, get_units(variable_name11) );

    interp_value = interpolate_ucla_ggcm_cdf(variable_name12, x, y, z, 1, 0);

    printf("%s [ %f, %f, %f ]\t%g %s\n", variable_name12, x, y, z,
            interp_value, get_units(variable_name12) );

    interp_value = interpolate_ucla_ggcm_cdf(variable_name13, x, y, z, 1, 0);

    printf("%s [ %f, %f, %f ]\t%g %s\n", variable_name13, x, y, z,
            interp_value, get_units(variable_name13) );

    close_cdf();

    open_cdf(cdf_name, 14, variable_name, variable_name1, variable_name2,
            variable_name3, variable_name4, variable_name5, variable_name6,
            variable_name7, variable_name8, variable_name9, variable_name10,
            variable_name11, variable_name12, variable_name13);

    printf("\nopen_cdf complete\n");

    interp_value = interpolate_ucla_ggcm_cdf(variable_name, x, y, z, 1, 0);

    printf("%s [ %f, %f, %f ]\t%g %s\n", variable_name, x, y, z, interp_value,
            get_units(variable_name) );

    interp_value = interpolate_ucla_ggcm_cdf(variable_name1, x, y, z, 1, 0);

    printf("%s [ %f, %f, %f ]\t%g %s\n", variable_name1, x, y, z, interp_value,
            get_units(variable_name1) );

    interp_value = interpolate_ucla_ggcm_cdf(variable_name2, x, y, z, 1, 0);

    printf("%s [ %f, %f, %f ]\t%g %s\n", variable_name2, x, y, z, interp_value,
            get_units(variable_name2) );

    interp_value = interpolate_ucla_ggcm_cdf(variable_name3, x, y, z, 1, 0);

    printf("%s [ %f, %f, %f ]\t%g %s\n", variable_name3, x, y, z, interp_value,
            get_units(variable_name3) );

    interp_value = interpolate_ucla_ggcm_cdf(variable_name4, x, y, z, 1, 0);

    printf("%s [ %f, %f, %f ]\t%g %s\n", variable_name4, x, y, z, interp_value,
            get_units(variable_name4) );

    interp_value = interpolate_ucla_ggcm_cdf(variable_name5, x, y, z, 1, 0);

    printf("%s [ %f, %f, %f ]\t%g %s\n", variable_name5, x, y, z, interp_value,
            get_units(variable_name5) );

    interp_value = interpolate_ucla_ggcm_cdf(variable_name6, x, y, z, 1, 0);

    printf("%s [ %f, %f, %f ]\t%g %s\n", variable_name6, x, y, z, interp_value,
            get_units(variable_name6) );

    interp_value = interpolate_ucla_ggcm_cdf(variable_name7, x, y, z, 1, 0);

    printf("%s [ %f, %f, %f ]\t%g %s\n", variable_name7, x, y, z, interp_value,
            get_units(variable_name7) );

    interp_value = interpolate_ucla_ggcm_cdf(variable_name8, x, y, z, 1, 0);

    printf("%s [ %f, %f, %f ]\t%g %s\n", variable_name8, x, y, z, interp_value,
            get_units(variable_name8) );

    interp_value = interpolate_ucla_ggcm_cdf(variable_name9, x, y, z, 1, 0);

    printf("%s [ %f, %f, %f ]\t%g %s\n", variable_name9, x, y, z, interp_value,
            get_units(variable_name9) );

    interp_value = interpolate_ucla_ggcm_cdf(variable_name10, x, y, z, 1, 0);

    printf("%s [ %f, %f, %f ]\t%g %s\n", variable_name10, x, y, z,
            interp_value, get_units(variable_name10) );

    interp_value = interpolate_ucla_ggcm_cdf(variable_name11, x, y, z, 1, 0);

    printf("%s [ %f, %f, %f ]\t%g %s\n", variable_name11, x, y, z,
            interp_value, get_units(variable_name11) );

    interp_value = interpolate_ucla_ggcm_cdf(variable_name12, x, y, z, 1, 0);

    printf("%s [ %f, %f, %f ]\t%g %s\n", variable_name12, x, y, z,
            interp_value, get_units(variable_name12) );

    interp_value = interpolate_ucla_ggcm_cdf(variable_name13, x, y, z, 1, 0);

    printf("%s [ %f, %f, %f ]\t%g %s\n", variable_name13, x, y, z,
            interp_value, get_units(variable_name13) );

    close_cdf();

    return 1;

}

