#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "kameleon.h"

int main(int argc, char *argv[])
{

    char variable[10];
    char cdf_name[200];
    char *units;

    float X, Y, Z;

    int count = 0;
    float n;
    float missing = -10000.0;
    float delta_x, delta_y, delta_z;

    float interp_value1, sample_factor, x2;

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

    open_cdf(cdf_name, 1, variable);

    /************************** example for new get_units() function ***************************
     **	takes variable name as argument and return a pointer to the string of units	**
     *******************************************************************************************/

    units = get_units(variable);
    printf("units for %s = %s\n", variable, units);

    for (n = X; n <= x2; n+=sample_factor)
    {

        interp_value1 = interpolate_ucla_ggcm_cdf(variable, n, Y, Z, 1, 1,
                missing, &delta_x, &delta_y, &delta_z);
        printf("\t%s [ %f, %f, %f ]\t%g %s\tdelta %f %s\n", variable, n, Y, Z,
                interp_value1, get_units(variable), delta_x, get_units("x") );
        count++;
    }

    close_cdf();

    return 1;

}

