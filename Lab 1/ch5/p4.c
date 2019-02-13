#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <fcntl.h>
#include <sys/wait.h>

int main (int argc, char **argv) {
	// printf("Hello World (pid:%d)\n", (int) getpid());
	int rc = fork();
	if (rc < 0) {
		fprintf(stderr, "Fork failed\n");
		exit(1);
	}
	else if (rc == 0) {
		close(STDOUT_FILENO);
		open("./p4.output", O_CREAT|O_WRONLY|O_TRUNC, S_IRWXU);
		// printf("Hello, I am child (pid:%d)\n", (int) getpid());
		char *myargs[3];
		myargs[0] = strdup("wc");
		myargs[1] = strdup("p4.c");
		myargs[2] = NULL;
		execvp(myargs[0], myargs);
		printf("this shouldn't print out");
	}
	else {
		int rc_wait = wait(NULL);
		// printf("Hello, I am parent of %d (rc_wait:%d) (pid:%d)\n", rc, rc_wait, (int) getpid());
	}
	return 0;
}