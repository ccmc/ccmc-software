/*
 * Kameleon_compatiblity_test.c
 *
 *  Created on: Jul 8, 2009
 *      Author: dberrios
 */

#include <ccmc/wrappers/c/Kameleon_c.h>
#include <stdio.h>
#include <string.h>

int main( int argc, char * argv[])
{
	if (argc != 2)
	{
		printf("Must enter 1 filename1\n");
		return 1;
	}
	int k0 = 0;
	int k1 = 1;
	Kameleon_create(k0);
	Kameleon_create(k1);

	int i0 = 0;
	Kameleon_open(k0,argv[1]);
	Kameleon_open(k1,argv[1]);

	//get model names
	char modelName1[512];
	char modelName2[512];
	Kameleon_get_model_name(k1, modelName1);
	Kameleon_get_model_name(k0, modelName2);

	printf("model1: %s\n",modelName1);
	printf("model2: %s\n",modelName2);

	float c0,c1,c2,dc0,dc1,dc2;
	c0 = -10.f;
	c1 = -2.f;
	c2 = 0.f;
	Kameleon_load_variable(k0,"bx");
	Kameleon_load_variable(k1,"bx");

	float value1 = Kameleon_interpolate(k0,"bx",&c0,&c1,&c2,&dc0,&dc1,&dc2);
	float value2 = Kameleon_interpolate(k1,"bx",&c0,&c1,&c2,&dc0,&dc1,&dc2);
	value1 = Kameleon_interpolate(k0,"bx",&c0,&c1,&c2,&dc0,&dc1,&dc2);
	value2 = Kameleon_interpolate(k1,"bx",&c0,&c1,&c2,&dc0,&dc1,&dc2);
	printf("value1: %f\n",value1);
	printf("value2: %f\n",value2);
	int status = Kameleon_delete(k0);
	printf("kdelete: %d\n", status);
	status = Kameleon_delete(k1);
	printf("kdelete: %d\n", status);

	return 0;
}
