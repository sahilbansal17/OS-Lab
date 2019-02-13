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
		char *myargs[3];
		myargs[0] = strdup("/bin/ls");
		myargs[1] = NULL;
		myargs[2] = NULL;
		execvp(myargs[0], myargs);
	}
	else {
		int rc_wait = wait(NULL);
	}
	return 0;
}