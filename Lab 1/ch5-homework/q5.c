#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>

int main (int argc, char **argv) {

	int rc = fork();
	if (rc < 0) {
		fprintf(stderr, "Fork failed\n");
		exit(1);
	}
	else if (rc == 0) {
		int rc_wait = wait(NULL);
		printf("This is the child process.\n");
		printf("The wait function in the child returned: %d\n", rc_wait);
	}
	else {
		int rc_wait = wait(NULL);
		printf("The wait function in the parent returned: %d\n", rc_wait);
	}
	return 0;
}