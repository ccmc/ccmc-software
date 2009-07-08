/*
 * Kameleon_compatiblity_test.c
 *
 *  Created on: Jul 8, 2009
 *      Author: dberrios
 */

#include <ccmc/Kameleon_compatibility.h>

int main( int argc, char * argv[])
{
	int k0 = 0;
	Kameleon_create(k0);
	int status = Kameleon_delete(k0);
	printf("kdelete: %d\n", status);
	return 0;
}
