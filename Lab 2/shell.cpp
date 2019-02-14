#include <iostream>
#include <string>

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

using namespace std;
#define MAX_LENGTH 80
int main() {

    char line[MAX_LENGTH], directory[MAX_LENGTH], command[MAX_LENGTH]; 
    char **arguments, **args;

    arguments = (char **) malloc (100* sizeof(char *));
    int count;
    getcwd(directory,sizeof(directory));

  	while (1) {
    	printf("%s>> ", directory);

    	gets(line);
    	printf("%s\n", line);
    	if(!strcmp(line,"exit")){
    		exit(0);
    	}

    	int child = fork();
    	if (child == 0) {

			char *token = strtok(line, " ");

			strcpy(command, token);
			printf("Running the command: %s\n", command);

			int i = 0;
			while (token != NULL) {
				// printf("%s\n", token);
				token = strtok(NULL, " ");
				arguments[i] = (char *) malloc (MAX_LENGTH);
				// printf("%s", token);
				if (arguments[i] == NULL) {
					break;
				}
				strcpy(arguments[i], token);
				i ++;
			}    		

			// args = (char **) malloc (i * sizeof(char *));

			for(int j = 0; j < i; j ++) {
				// printf("%s", arguments[i]);
				args[i] = (char *) malloc (MAX_LENGTH);
				strcpy(args[i], arguments[i]);
			}
    		execvp(command, arguments);	
    		exit(0);
    	}

    	wait(NULL);
    	printf("\n");
    
    }

    return 0;
}