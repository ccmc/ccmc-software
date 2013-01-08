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

	printf("creating kameleon object\n");
	int kameleon = Kameleon_create();
	printf("created kameleon object\n");
	Kameleon_delete(kameleon);
	printf("deleted kameleon\n");
	return 0;
}
