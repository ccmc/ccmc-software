#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "kameleon.h"

int main(int argc, char *argv[])
{

    char cdf_name[250];

    /***************** declared local pointers to store returned attribute value pointers ************/

    double *double_attribute_valuePtr;

    if (argc != 2)
    {
        printf("ERROR:this example program requires 1 argument - full path and file name of data file\n");
        exit( EXIT_FAILURE);
    }

    strcpy(cdf_name, argv[1]);

    open_cdf(cdf_name, 0);

    printf("Open %s complete...\n", cdf_name);

    /*** get the GLOBAL ATTRIBUTE value ***/

    if (attribute_exists("time_refdate_mjd") )
        double_attribute_valuePtr = gattribute_get("time_refdate_mjd");
    if (attribute_exists("time_refdate_mjd") )
    {
        printf("\n\ntime_refdate_mjd attribute = %f\n",
                *double_attribute_valuePtr);
    }

    return 1;

}

