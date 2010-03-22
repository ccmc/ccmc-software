/*
 * FileReaderCompatibility.c
 *
 *  Created on: Mar 16, 2010
 *      Author: dberrios
 */

#include <ccmc/wrappers/c/FileReader_c.h>
#include <stdio.h>
#include <string.h>

int main(int argc, char * argv[])
{
	if (argc < 2)
	{
		printf("FileReaderCompatibility <filename>\n");
		return 1;
	}
	int id = 0;

	FileReader_create(id);
	FileReader_open(id, argv[1]);

	FileReader_close(id);
	FileReader_delete(id);
	return 0;

}


