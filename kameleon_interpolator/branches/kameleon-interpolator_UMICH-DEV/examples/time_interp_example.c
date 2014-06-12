/***************** to compile just:

 cc time_interp_example.c lib/libinterp.a ${CDF_LIB}/libcdf.a -lm
 
 were ${CDF_LIB} is the location of your local cdf library installation
 
 to run
 
 ./a.out cdf_data_directory_path variable_name X Y Z sample_time_interval
 
 were sample_time_interval is the number of miliseconds to add to start_time until 
 the end_time is reached.

 ****************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "kameleon.h"

int main(int argc, char *argv[])
{

    long status;

    char data_path[750];
    char variable[10];

    float X, Y, Z;

    double time, start_time, end_time;
    float sample_time_interval;

    float time_interpolated_value;

    if (argc != 7)
    {
        printf("ERROR:this example program requires 6 arguments - (1)full path of data directory (2) variable name (3) x position (4) y position (5) z position (6) time step in milliseconds\n");
        exit( EXIT_FAILURE);
    }

    strcpy(data_path, argv[1]);
    strcpy(variable, argv[2]);

    X = atof(argv[3]);
    Y = atof(argv[4]);
    Z = atof(argv[5]);

    sample_time_interval = atof(argv[6]);

    status = init_time(data_path, &start_time, &end_time);

    if (status == EXIT_FAILURE)
    {
        exit( EXIT_FAILURE);
    }

    printf("Simulation start_time:\t%f msec\n", start_time);
    printf("Simulation end_time:\t%f msec\n", end_time);

    for (time = start_time; time <= end_time; time+=sample_time_interval)
    {
        time_interpolated_value = time_interpolate(variable, time, X, Y, Z);
        printf("%s [ %f, %f, %f ] @ %f milliseconds\t%f\n", variable, X, Y, Z,
                time, time_interpolated_value);
    }

    return 1;

}

