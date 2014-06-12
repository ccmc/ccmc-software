#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "kameleon.h"

int main(int argc, char *argv[])
{

    float *var_arrayPtr;
    float *var_arrayPtr2;
    int array_size;
    int array_size2;

    /*char *cdf_name = "/export/home/mmaddox/test_data/test_cdf_data/3d__ful_1_20000101_000000_0000.out.cdf";*/

    /*char *cdf_name = "3d__ful_1_n200344.out.cdf";*/
    int i;
    char cdf_name[250];
    float x, y, z, interp_value;

    char *variable_name = "rho";
    char *variable_name1 = "b1x";
    char *variable_name2 = "b1y";
    char *variable_name3 = "b1z";
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
    char *variable_name14 = "e";

    if (argc != 5)
    {
        printf("ERROR:this example program requires 4 arguments - (1)full path and file name of data file (2) x position (3) y position (4) z position \n");
        exit( EXIT_FAILURE);
    }

    strcpy(cdf_name, argv[1]);

    x = atof(argv[2]);
    y = atof(argv[3]);
    z = atof(argv[4]);

    /*
     fprintf(stderr, "Trying var_get\n");
     var_arrayPtr = var_get( cdf_name, "x" , &array_size);
     var_arrayPtr2 = var_get( cdf_name, "jx" , &array_size2);

     if( var_arrayPtr != NULL )
     {
     
     printf("back from var_get\nvalue of var_arrayPtr[1] = %f\nsize of var_array = %d\n", (float) var_arrayPtr[1], array_size );
     
     for( i=0;i<array_size;i+=100)
     {
     
     printf("x[%d] = %f\n", i, var_arrayPtr[i] );
     printf("jx[%d] = %f\n", i, var_arrayPtr2[i] );

     }
     }
     */

    printf("\ncalling open_cdf 1\n");
    open_cdf(cdf_name, 5, variable_name, variable_name1, variable_name2,
            variable_name3, variable_name4);
    printf("\nopen_cdf 1 complete\n");

    interp_value = interpolate_batsrus_cdf(variable_name, x, y, z, 1, 0);

    printf("%s [ %f, %f, %f ]\t%g %s\n", variable_name, x, y, z, interp_value,
            get_units(variable_name) );

    interp_value = interpolate_batsrus_cdf(variable_name1, x, y, z, 1, 0);

    printf("%s [ %f, %f, %f ]\t%g %s\n", variable_name1, x, y, z, interp_value,
            get_units(variable_name1) );

    interp_value = interpolate_batsrus_cdf(variable_name2, x, y, z, 1, 0);

    printf("%s [ %f, %f, %f ]\t%g %s\n", variable_name2, x, y, z, interp_value,
            get_units(variable_name2) );

    interp_value = interpolate_batsrus_cdf(variable_name3, x, y, z, 1, 0);

    printf("%s [ %f, %f, %f ]\t%g %s\n", variable_name3, x, y, z, interp_value,
            get_units(variable_name3) );

    interp_value = interpolate_batsrus_cdf(variable_name4, x, y, z, 1, 0);

    printf("%s [ %f, %f, %f ]\t%g %s\n", variable_name4, x, y, z, interp_value,
            get_units(variable_name4) );

    printf("\ncalling open_cdf 2\n");
    open_cdf(cdf_name, 5, variable_name5, variable_name6, variable_name7,
            variable_name8, variable_name9);
    printf("\nopen_cdf 2complete\n");

    interp_value = interpolate_batsrus_cdf(variable_name5, x, y, z, 1, 0);

    printf("%s [ %f, %f, %f ]\t%g %s\n", variable_name5, x, y, z, interp_value,
            get_units(variable_name5) );

    interp_value = interpolate_batsrus_cdf(variable_name6, x, y, z, 1, 0);

    printf("%s [ %f, %f, %f ]\t%g %s\n", variable_name6, x, y, z, interp_value,
            get_units(variable_name6) );

    interp_value = interpolate_batsrus_cdf(variable_name7, x, y, z, 1, 0);

    printf("%s [ %f, %f, %f ]\t%g %s\n", variable_name7, x, y, z, interp_value,
            get_units(variable_name7) );

    interp_value = interpolate_batsrus_cdf(variable_name8, x, y, z, 1, 0);

    printf("%s [ %f, %f, %f ]\t%g %s\n", variable_name8, x, y, z, interp_value,
            get_units(variable_name8) );

    interp_value = interpolate_batsrus_cdf(variable_name9, x, y, z, 1, 0);

    printf("%s [ %f, %f, %f ]\t%g %s\n", variable_name9, x, y, z, interp_value,
            get_units(variable_name9) );

    printf("\ncalling open_cdf 3\n");
    open_cdf(cdf_name, 5, variable_name10, variable_name11, variable_name12,
            variable_name13, variable_name14);
    printf("\nopen_cdf 3complete\n");

    interp_value = interpolate_batsrus_cdf(variable_name10, x, y, z, 1, 0);

    printf("%s [ %f, %f, %f ]\t%g %s\n", variable_name10, x, y, z,
            interp_value, get_units(variable_name10) );

    interp_value = interpolate_batsrus_cdf(variable_name11, x, y, z, 1, 0);

    printf("%s [ %f, %f, %f ]\t%g %s\n", variable_name11, x, y, z,
            interp_value, get_units(variable_name11) );

    interp_value = interpolate_batsrus_cdf(variable_name12, x, y, z, 1, 0);

    printf("%s [ %f, %f, %f ]\t%g %s\n", variable_name12, x, y, z,
            interp_value, get_units(variable_name12) );

    interp_value = interpolate_batsrus_cdf(variable_name13, x, y, z, 1, 0);

    printf("%s [ %f, %f, %f ]\t%g %s\n", variable_name13, x, y, z,
            interp_value, get_units(variable_name13) );

    interp_value = interpolate_batsrus_cdf(variable_name14, x, y, z, 1, 0);

    printf("%s [ %f, %f, %f ]\t%g %s\n", variable_name14, x, y, z,
            interp_value, get_units(variable_name14) );

    printf("\n******************* open and interp test complete\ntrying more interpolation");

    interp_value = interpolate_batsrus_cdf(variable_name, x, y, z, 1, 0);

    printf("%s [ %f, %f, %f ]\t%g %s\n", variable_name, x, y, z, interp_value,
            get_units(variable_name) );

    interp_value = interpolate_batsrus_cdf(variable_name1, x, y, z, 1, 0);

    printf("%s [ %f, %f, %f ]\t%g %s\n", variable_name1, x, y, z, interp_value,
            get_units(variable_name1) );

    interp_value = interpolate_batsrus_cdf(variable_name2, x, y, z, 1, 0);

    printf("%s [ %f, %f, %f ]\t%g %s\n", variable_name2, x, y, z, interp_value,
            get_units(variable_name2) );

    interp_value = interpolate_batsrus_cdf(variable_name3, x, y, z, 1, 0);

    printf("%s [ %f, %f, %f ]\t%g %s\n", variable_name3, x, y, z, interp_value,
            get_units(variable_name3) );

    interp_value = interpolate_batsrus_cdf(variable_name4, x, y, z, 1, 0);

    printf("%s [ %f, %f, %f ]\t%g %s\n", variable_name4, x, y, z, interp_value,
            get_units(variable_name4) );

    interp_value = interpolate_batsrus_cdf(variable_name5, x, y, z, 1, 0);

    printf("%s [ %f, %f, %f ]\t%g %s\n", variable_name5, x, y, z, interp_value,
            get_units(variable_name5) );

    interp_value = interpolate_batsrus_cdf(variable_name6, x, y, z, 1, 0);

    printf("%s [ %f, %f, %f ]\t%g %s\n", variable_name6, x, y, z, interp_value,
            get_units(variable_name6) );

    interp_value = interpolate_batsrus_cdf(variable_name7, x, y, z, 1, 0);

    printf("%s [ %f, %f, %f ]\t%g %s\n", variable_name7, x, y, z, interp_value,
            get_units(variable_name7) );

    interp_value = interpolate_batsrus_cdf(variable_name8, x, y, z, 1, 0);

    printf("%s [ %f, %f, %f ]\t%g %s\n", variable_name8, x, y, z, interp_value,
            get_units(variable_name8) );

    interp_value = interpolate_batsrus_cdf(variable_name9, x, y, z, 1, 0);

    printf("%s [ %f, %f, %f ]\t%g %s\n", variable_name9, x, y, z, interp_value,
            get_units(variable_name9) );

    interp_value = interpolate_batsrus_cdf(variable_name10, x, y, z, 1, 0);

    printf("%s [ %f, %f, %f ]\t%g %s\n", variable_name10, x, y, z,
            interp_value, get_units(variable_name10) );

    interp_value = interpolate_batsrus_cdf(variable_name11, x, y, z, 1, 0);

    printf("%s [ %f, %f, %f ]\t%g %s\n", variable_name11, x, y, z,
            interp_value, get_units(variable_name11) );

    interp_value = interpolate_batsrus_cdf(variable_name12, x, y, z, 1, 0);

    printf("%s [ %f, %f, %f ]\t%g %s\n", variable_name12, x, y, z,
            interp_value, get_units(variable_name12) );

    interp_value = interpolate_batsrus_cdf(variable_name13, x, y, z, 1, 0);

    printf("%s [ %f, %f, %f ]\t%g %s\n", variable_name13, x, y, z,
            interp_value, get_units(variable_name13) );

    interp_value = interpolate_batsrus_cdf(variable_name14, x, y, z, 1, 0);

    printf("%s [ %f, %f, %f ]\t%g %s\n", variable_name14, x, y, z,
            interp_value, get_units(variable_name14) );

    close_cdf();

    open_cdf(cdf_name, 15, variable_name, variable_name1, variable_name2,
            variable_name3, variable_name4, variable_name5, variable_name6,
            variable_name7, variable_name8, variable_name9, variable_name10,
            variable_name11, variable_name12, variable_name13, variable_name14);
    printf("\nopen_cdf complete\n");
    open_cdf(cdf_name, 15, variable_name, variable_name1, variable_name2,
            variable_name3, variable_name4, variable_name5, variable_name6,
            variable_name7, variable_name8, variable_name9, variable_name10,
            variable_name11, variable_name12, variable_name13, variable_name14);
    printf("\nopen_cdf complete\n");

    interp_value = interpolate_batsrus_cdf(variable_name, x, y, z, 1, 0);

    printf("%s [ %f, %f, %f ]\t%g %s\n", variable_name, x, y, z, interp_value,
            get_units(variable_name) );

    interp_value = interpolate_batsrus_cdf(variable_name1, x, y, z, 1, 0);

    printf("%s [ %f, %f, %f ]\t%g %s\n", variable_name1, x, y, z, interp_value,
            get_units(variable_name1) );

    interp_value = interpolate_batsrus_cdf(variable_name2, x, y, z, 1, 0);

    printf("%s [ %f, %f, %f ]\t%g %s\n", variable_name2, x, y, z, interp_value,
            get_units(variable_name2) );

    interp_value = interpolate_batsrus_cdf(variable_name3, x, y, z, 1, 0);

    printf("%s [ %f, %f, %f ]\t%g %s\n", variable_name3, x, y, z, interp_value,
            get_units(variable_name3) );

    interp_value = interpolate_batsrus_cdf(variable_name4, x, y, z, 1, 0);

    printf("%s [ %f, %f, %f ]\t%g %s\n", variable_name4, x, y, z, interp_value,
            get_units(variable_name4) );

    interp_value = interpolate_batsrus_cdf(variable_name5, x, y, z, 1, 0);

    printf("%s [ %f, %f, %f ]\t%g %s\n", variable_name5, x, y, z, interp_value,
            get_units(variable_name5) );

    interp_value = interpolate_batsrus_cdf(variable_name6, x, y, z, 1, 0);

    printf("%s [ %f, %f, %f ]\t%g %s\n", variable_name6, x, y, z, interp_value,
            get_units(variable_name6) );

    interp_value = interpolate_batsrus_cdf(variable_name7, x, y, z, 1, 0);

    printf("%s [ %f, %f, %f ]\t%g %s\n", variable_name7, x, y, z, interp_value,
            get_units(variable_name7) );

    interp_value = interpolate_batsrus_cdf(variable_name8, x, y, z, 1, 0);

    printf("%s [ %f, %f, %f ]\t%g %s\n", variable_name8, x, y, z, interp_value,
            get_units(variable_name8) );

    interp_value = interpolate_batsrus_cdf(variable_name9, x, y, z, 1, 0);

    printf("%s [ %f, %f, %f ]\t%g %s\n", variable_name9, x, y, z, interp_value,
            get_units(variable_name9) );

    interp_value = interpolate_batsrus_cdf(variable_name10, x, y, z, 1, 0);

    printf("%s [ %f, %f, %f ]\t%g %s\n", variable_name10, x, y, z,
            interp_value, get_units(variable_name10) );

    interp_value = interpolate_batsrus_cdf(variable_name11, x, y, z, 1, 0);

    printf("%s [ %f, %f, %f ]\t%g %s\n", variable_name11, x, y, z,
            interp_value, get_units(variable_name11) );

    interp_value = interpolate_batsrus_cdf(variable_name12, x, y, z, 1, 0);

    printf("%s [ %f, %f, %f ]\t%g %s\n", variable_name12, x, y, z,
            interp_value, get_units(variable_name12) );

    interp_value = interpolate_batsrus_cdf(variable_name13, x, y, z, 1, 0);

    printf("%s [ %f, %f, %f ]\t%g %s\n", variable_name13, x, y, z,
            interp_value, get_units(variable_name13) );

    interp_value = interpolate_batsrus_cdf(variable_name14, x, y, z, 1, 0);

    printf("%s [ %f, %f, %f ]\t%g %s\n", variable_name14, x, y, z,
            interp_value, get_units(variable_name14) );

    close_cdf();

    open_cdf(cdf_name, 15, variable_name, variable_name1, variable_name2,
            variable_name3, variable_name4, variable_name5, variable_name6,
            variable_name7, variable_name8, variable_name9, variable_name10,
            variable_name11, variable_name12, variable_name13, variable_name14);

    printf("\nopen_cdf complete\n");

    interp_value = interpolate_batsrus_cdf(variable_name, x, y, z, 1, 0);

    printf("%s [ %f, %f, %f ]\t%g %s\n", variable_name, x, y, z, interp_value,
            get_units(variable_name) );

    interp_value = interpolate_batsrus_cdf(variable_name1, x, y, z, 1, 0);

    printf("%s [ %f, %f, %f ]\t%g %s\n", variable_name1, x, y, z, interp_value,
            get_units(variable_name1) );

    interp_value = interpolate_batsrus_cdf(variable_name2, x, y, z, 1, 0);

    printf("%s [ %f, %f, %f ]\t%g %s\n", variable_name2, x, y, z, interp_value,
            get_units(variable_name2) );

    interp_value = interpolate_batsrus_cdf(variable_name3, x, y, z, 1, 0);

    printf("%s [ %f, %f, %f ]\t%g %s\n", variable_name3, x, y, z, interp_value,
            get_units(variable_name3) );

    interp_value = interpolate_batsrus_cdf(variable_name4, x, y, z, 1, 0);

    printf("%s [ %f, %f, %f ]\t%g %s\n", variable_name4, x, y, z, interp_value,
            get_units(variable_name4) );

    interp_value = interpolate_batsrus_cdf(variable_name5, x, y, z, 1, 0);

    printf("%s [ %f, %f, %f ]\t%g %s\n", variable_name5, x, y, z, interp_value,
            get_units(variable_name5) );

    interp_value = interpolate_batsrus_cdf(variable_name6, x, y, z, 1, 0);

    printf("%s [ %f, %f, %f ]\t%g %s\n", variable_name6, x, y, z, interp_value,
            get_units(variable_name6) );

    interp_value = interpolate_batsrus_cdf(variable_name7, x, y, z, 1, 0);

    printf("%s [ %f, %f, %f ]\t%g %s\n", variable_name7, x, y, z, interp_value,
            get_units(variable_name7) );

    interp_value = interpolate_batsrus_cdf(variable_name8, x, y, z, 1, 0);

    printf("%s [ %f, %f, %f ]\t%g %s\n", variable_name8, x, y, z, interp_value,
            get_units(variable_name8) );

    interp_value = interpolate_batsrus_cdf(variable_name9, x, y, z, 1, 0);

    printf("%s [ %f, %f, %f ]\t%g %s\n", variable_name9, x, y, z, interp_value,
            get_units(variable_name9) );

    interp_value = interpolate_batsrus_cdf(variable_name10, x, y, z, 1, 0);

    printf("%s [ %f, %f, %f ]\t%g %s\n", variable_name10, x, y, z,
            interp_value, get_units(variable_name10) );

    interp_value = interpolate_batsrus_cdf(variable_name11, x, y, z, 1, 0);

    printf("%s [ %f, %f, %f ]\t%g %s\n", variable_name11, x, y, z,
            interp_value, get_units(variable_name11) );

    interp_value = interpolate_batsrus_cdf(variable_name12, x, y, z, 1, 0);

    printf("%s [ %f, %f, %f ]\t%g %s\n", variable_name12, x, y, z,
            interp_value, get_units(variable_name12) );

    interp_value = interpolate_batsrus_cdf(variable_name13, x, y, z, 1, 0);

    printf("%s [ %f, %f, %f ]\t%g %s\n", variable_name13, x, y, z,
            interp_value, get_units(variable_name13) );

    interp_value = interpolate_batsrus_cdf(variable_name14, x, y, z, 1, 0);

    printf("%s [ %f, %f, %f ]\t%g %s\n", variable_name14, x, y, z,
            interp_value, get_units(variable_name14) );

    close_cdf();

    return 1;

}
