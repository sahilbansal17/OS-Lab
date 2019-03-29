// open any device file in binary and read only mode

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

int main () {

	char *file_name;
	file_name = (char *) malloc (10);
	strcpy(file_name, "/dev/sdb");

	printf("%s\n", file_name);

	FILE *fptr;
	fptr = fopen(file_name, "rb");

	if (!fptr) {
		perror("File read");
		exit(0);
	}

	char *block = malloc(512);

	fread(block, 512, 1, fptr);
	printf("%s", block);

	return 0;
}