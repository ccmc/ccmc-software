/*
 * FileReaderCompatibility.c
 *
 *  Created on: Mar 16, 2010
 *      Author: dberrios
 */

#include <ccmc/wrappers/c/GeneralFileReader_c.h>
#include <stdio.h>
#include <string.h>

int main(int argc, char * argv[])
{
	if (argc < 2)
	{
		printf("GeneralFileReaderCompatibility <filename>\n");
		return 1;
	}
	int id = 0;

	GeneralFileReader_create(id);
	GeneralFileReader_open(id, argv[1]);

	GeneralFileReader_close(id);
	GeneralFileReader_delete(id);
	return 0;

}


