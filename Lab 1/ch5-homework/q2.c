#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <fcntl.h>
#include <sys/wait.h>
int main (int argc, char ** argv) {

	int fd = open("testfile.txt", O_WRONLY | O_TRUNC);	
	int rc = fork();

	
	if (write(fd, "This will be output to testfile.txt\n", 36) != 36) {
    	write(2, "There was an error writing to testfile.txt\n", 43);
    	return -1;
	}

	if (rc < 0) {
		fprintf(stderr, "fork failed\n");
		exit(1);
	}
	else if (rc == 0) {
		printf("File pointer accessed by child: %d\n", fd);
		
		if (write(fd, "This will be output to testfile.txt\n", 36) != 36) {
        	write(2, "There was an error writing to testfile.txt\n", 43);
        	return -1;
    	}
	}
	else {
		printf("File pointer accessed by parent: %d\n", fd);

		if (write(fd, "This will be output to testfile.txt\n", 36) != 36) {
        	write(2, "There was an error writing to testfile.txt\n", 43);
        	return -1;
    	}

	}


	if (write(fd, "This will be output to testfile.txt\n", 36) != 36) {
    	write(2, "There was an error writing to testfile.txt\n", 43);
    	return -1;
	}

	return 0;
}