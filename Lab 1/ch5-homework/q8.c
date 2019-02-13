#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <fcntl.h>
#include <sys/wait.h>

int main (int argc, char **argv) {
	int rc1 = fork(), rc2 = fork();
	if (rc < 0) {
		fprintf(stderr, "Fork failed\n");
		exit(1);
	}
	
	if (rc1 == 0) {
		// first child process
		
	}
	if (rc2 == 0) {
		// second child process
		
	}
	return 0;
}