#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#include "kameleon.h"

int main(int argc, char *argv[])
{

    char variable[15];
    char cdf_name[200];
    char *units;

    double X, Y, Z;

    int count = 0;
    double n;

    float missing = -10000.0;
    double delta_x, delta_y, delta_z;
    double interp_value1, sample_factor, x2;

    int i, j;

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
    x2 = atof(argv[7]);

    /*
     printf("DEBUG\nposition1 = [ %f, %f, %f ]\nposition2 = [ %f, %f, %f ]\n", X, Y, Z, x2, Y, Z );
     printf("Debug\nSamlpe Factor = %f\n", sample_factor );
     */

    open_cdf(cdf_name, 0);

    /************************** example for get_units() function ***************************
     **	takes variable name as argument and return a pointer to the string of units	**
     ****************************************************************************************/

    for (i = 89; i < 90; i++)
    {
        for (j = 0; j < 360; j++)
        {

            float sin_lat = (-1.0) + ( (2.0 / 180.0) * ( (i + 1 ) - .5 ) );
            interp_value1 = interpolate_kpvt_cdf(variable, sin_lat, j+1, Z, 0,
                    0);

            printf("%f\t%f\t%f\n", i+1.0, j+1.0, interp_value1);
        }
    }

    /*
     close_cdf();
     */
    return 1;

}

