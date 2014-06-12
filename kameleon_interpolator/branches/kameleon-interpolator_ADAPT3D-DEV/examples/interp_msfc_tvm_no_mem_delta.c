#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "kameleon.h"

int main(int argc, char *argv[])
{

    char variable[15];
    char cdf_name[200];
    char *units;

    float X, Y, Z;

    int count = 0;
    int i, j;
    float n;

    float missing = -10000.0;

    float delta_x, delta_y, delta_z;

    float interp_value1, sample_factor, y2;

    float lon, lon_delta;

    if (argc != 8)
    {
        printf("ERROR:this example program requires 7 arguments - (1)full path and file name of data file (2) VARIABLE NAME (3) x1 position (4) y1 position (5) z1 position (6) delta x (7) x2 \n");
        exit( EXIT_FAILURE);
    }

    strcpy(cdf_name, argv[1]);
    strcpy(variable, argv[2]);

    X = atof(argv[3]);
    Y = atof(argv[4]);
    Z = atof(argv[5]);

    sample_factor = atof(argv[6]);
    y2 = atof(argv[7]);

    printf("DEBUG\nargv[0] --->%s<---\n", argv[0]);
    printf("DEBUG\nargv[1] --->%s<---\n", argv[1]);

    printf("DEBUG\ninput file name --->%s<---\n", cdf_name);
    printf("DEBUG\nvariable name --->%s<---\n", variable);
    printf("DEBUG\nposition1 = [ %f, %f, %f ]\nposition2 = [ %f, %f, %f ]\n",
            X, Y, Z, X, y2, Z);
    printf("Samlpe Factor = %f\n", sample_factor);

    open_cdf(cdf_name, 0);

    /************************** example for new get_units() function ***************************
     **	takes variable name as argument and return a pointer to the string of units	**
     *******************************************************************************************/

    units = get_units(variable);
    printf("units for %s = %s\n", variable, units);

    for (n = Y; n <= y2; n+=sample_factor)
    {

        interp_value1 = interpolate_msfc_tvm_cdf(variable, X, n, Z, 0, 1,
                missing, &delta_x, &delta_y, &delta_z);
        /*** normal print **/
        /*
         printf("\t%s [ %f, %f, %f ]\t%g %s\tdelta %f %s\n",  variable, X, n, Z, interp_value1, get_units( variable ), delta_y, get_units( "b_l" ) );
         */
        /* plot print */
        printf("%f\t%f\t%f\n", X, n, interp_value1);
        count++;
    }

    /*
     lon = -0.18698;
     
     for( j = 0; j < 328; j++ )
     {
     
     
     interp_value1 = interpolate_msfc_tvm_cdf( variable, 0.155, lon, Z, 0, 0 );
     
     lon = lon + .01;
     
     printf("%f\t%f\t%f\n", 0.155, lon, interp_value1 );
     }
     
     */

    /*
     close_cdf();
     */
    return 1;

}

