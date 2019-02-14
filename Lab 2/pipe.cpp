#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>

#define MAX_LENGTH 200

int main(int argc,char **argv) {
	
	if(argc != 2){
		printf("Usage: ./pipe file_name\n");
		exit(0);
	}

	char* msg;

	// open the file
	FILE *f = fopen(argv[1], "r");

	if (f == NULL) {
		printf("Error in opening the file\n");
		exit(0);
	}
	
	// calculate the length of the input file
	fseek(f, 0, SEEK_END);
	long length = ftell (f);
	fseek(f, 0, SEEK_SET);
	
	// allocate memory to the message 
	msg = malloc(length);
	
	// read message from the file
	if (msg) {
		fread (msg, 1, length, f);
	}
	
	// close the file
	fclose(f);
	
	char input[MAX_LENGTH];
	int p[2], pipe_2[2], pid1, pid2, pid3;
	
	if (pipe(p) < 0){
		printf("Error in pipe command\n");
		exit(1);
	}

	if (pipe(pipe_2) < 0) {
		printf("Error in pipe command\n");
		exit(1);
	}
	
	pid1 = fork();
	
	if (pid1 < 0) {
		printf("Error in fork call\n");
		exit(1);
	}
	
	if (pid1 > 0) {
		// first process reads the data
		printf("\nThe file contains the content as shown:\n%s\n\n", msg);
		write(p[1], msg, strlen(msg) + 1);
		wait(0);

		read(pipe_2[0], input, MAX_LENGTH);
		printf("The word count as read by the 1st process: %s\n\n", input);
		close(pipe_2[0]);

		// read(p[0], input, MAX_LENGTH);
		// printf("The word count as read by the 1st process: %s\n", input);
		// close(p[0]);
	}
	else {
		pid2 = fork();
		
		if (pid2 > 0) {

			// fourth process counts the no of words and passes it to the 1st process

			wait(0);
			read(p[0], input, MAX_LENGTH);
			int count = 0;
			for(int i = 0; i < strlen(input); i ++){
				if(input[i+1]==' '||input[i+1]=='\n') count++;
			}

			char count_res[MAX_LENGTH];
			// itoa(count, count_res, MAX_LENGTH);
			snprintf(count_res, MAX_LENGTH, "%d", count);
			write(pipe_2[1], count_res, strlen(count_res) + 1);
			close(pipe_2[1]);

			printf("Fourth process counts the no. of words in the file and passes it to the 1st process.\n\n");
			// close(p[0]);

			close(p[1]);
		}
		else {
			pid3 = fork();
			
			if(pid3 > 0) {
				wait(0);
				read(p[0], input, MAX_LENGTH);
				for(int i = 1; i < strlen(input); i += 2){
					if(input[i] >='a' && input[i] <= 'z'){
						input[i] = input[i] -'a' + 'A';
					}
					else if(input[i] >= 'A' && input[i] <= 'Z'){
						input[i] = input[i] - 'A' + 'a';
					}
				}
				printf("Third process changes the odd indices, the string is: \n%s\n\n", input);
				close(p[0]);
				write(p[1], input, strlen(input) + 1);
				close(p[1]);
			}
			
			else {
				read(p[0], input, MAX_LENGTH);
				for(int i = 0; i < strlen(input); i += 2){
					if(input[i] >= 'a' && input[i] <= 'z'){
						input[i] = input[i] -'a' + 'A';
					}
					else if(input[i] >= 'A' && input[i] <= 'Z'){
						input[i] = input[i] - 'A' + 'a';
					}
				}
				printf("Second process changes the even indices, the string is: \n%s\n\n", input);
				close(p[0]);
				write(p[1], input, strlen(input) + 1);
				close(p[1]);
			}
		}

	}
	return 0;
}