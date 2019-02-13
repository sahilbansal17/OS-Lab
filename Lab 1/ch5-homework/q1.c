#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <fcntl.h>
#include <sys/wait.h>
int main (int argc, char ** argv) {

	int x = 100;
	int rc = fork();

	printf("The value of x just after fork call: %d\n", x);
	if (rc < 0) {
		fprintf(stderr, "fork failed\n");
		exit(1);
	}
	else if (rc == 0) {
		x++ ;
		printf("The value of x in the child is: %d\n", x);
	}
	else {
		x-- ;
		printf("The value of x in the parent is: %d\n", x);
	}

	return 0;
}