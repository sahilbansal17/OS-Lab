#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

// child and parent both fork each other

int main (int argc, char ** argv) {

	printf("Hello World (pid:%d)\n\n", (int) getpid());
	int rc = fork();

	if (rc < 0) {
		fprintf(stderr, "fork failed\n");
		exit(1);
	}
	else if (rc == 0) {
		int wait_time;
		// printf("Hello, I am child (pid: %d)\n", (int) getpid());

		int child_fork = fork();
		if (child_fork < 0) {
			fprintf(stderr, "fork failed\n");
			exit(1);
		}
		else if (child_fork == 0) {
			sleep(5);
			printf("Hello, I am child (pid: %d)\n", (int) getpid());
		}
		else {
			int rc_wait = wait(NULL);
			sleep(5);
			printf("Hello, I am parent of %d (pid:%d)\n", child_fork, (int) getpid());
		}
	}
	else {
		int rc_wait = wait(NULL);
		// printf("hello, I am parent of %d (rc_wait:%d) (pid:%d)\n", rc, rc_wait, (int) getpid());

		int parent_fork = fork();
		if (parent_fork < 0) {
			fprintf(stderr, "fork failed\n");
		}
		else if (parent_fork == 0) {
			sleep(5);
			printf("Hello, I am child (pid: %d)\n", (int) getpid());
		}
		else {
			int rc_wait = wait(NULL);
			sleep(5);
			printf("Hello, I am parent of %d (pid:%d)\n", parent_fork, (int) getpid());
		}
	}

	return 0;
}