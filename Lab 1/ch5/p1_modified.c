#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main (int argc, char ** argv) {

	printf("Hello World (pid:%d)\n", (int) getpid());
	int rc = fork();

	if (rc < 0) {
		fprintf(stderr, "fork failed\n");
		exit(1);
	}
	else if (rc == 0) {
		int wait_time;
		printf("Hello, I am child (pid: %d)\n", (int) getpid());
		printf("Multiplication table of 3 is:\n");
		for (int i = 1; i <= 10; i ++) {
			printf("3 * %d = %d\n", i, 3*i);
			wait_time = rand() % 5 + 1;
			sleep(wait_time);
			printf("Child waited for %d seconds\n", wait_time);
		}
		printf("\n");
	}
	else {
		// int rc_wait = wait(NULL);
		int wait_time;
		printf("Hello, I am parent of %d (pid: %d)\n", rc, (int) getpid());
		printf("Multiplication table of 2 is:\n");
		for (int i = 1; i <= 10; i ++) {
			printf("2 * %d = %d\n", i, 2*i);
			wait_time = rand() % 5 + 1;
			sleep(wait_time);
			printf("Parent waited for %d seconds\n", wait_time);
		}
		printf("\n");
	}

	return 0;
}