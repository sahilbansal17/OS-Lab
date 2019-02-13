#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <fcntl.h>
#include <sys/wait.h>

int main (int argc, char **argv) {
	int rc = fork();
	if (rc < 0) {
		fprintf(stderr, "Fork failed\n");
		exit(1);
	}
	else if (rc == 0) {
		close(STDOUT_FILENO);
		// open("./p4.output", O_CREAT|O_WRONLY|O_TRUNC, S_IRWXU);
		printf("this shouldn't print out\n");
	}
	else {
		int rc_wait = wait(NULL);
		printf("Will parent print something?\n");
	}
	return 0;
}