// Reader writer problem using processes through semaphores implemented using shared memory

// Reader's preference solution
#include <pthread.h>
#include <assert.h>
#include <sched.h>
#include <semaphore.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>

#define SHMSZ     27
#define KEY1  12345
#define KEY2  23456
#define KEY3  34567
#define PERMS  0600 

sem_t *resource;
sem_t *rmutex;

// use shared memory to share the variable readcount among different reader processes
int shmid1, shmid2, shmid3;
key_t key;
int *readcount;

int main(int argc, char **argv) {

	if (argc != 3) {
		printf("Usage of the program: ./reader_writer <no. of writers> <no. of readers>\n");
		return 0;
	}

	int num_writers = atoi(argv[1]);
	printf("Number of writers: %d\n", num_writers);

	int num_readers = atoi(argv[2]);
	printf("Number of readers is: %d\n\n", num_readers);

    // get the share id for attaching readcount
    if ((shmid1 = shmget(KEY1, sizeof(int), 0666 | IPC_CREAT)) < 0) {
        perror("shmget1 ");
        exit(1);
    }
    // get the share id for attaching sem_t resource
    if ((shmid2 = shmget(KEY2, sizeof(sem_t), 0666 | IPC_CREAT)) < 0) {
        perror("shmget2 ");
        exit(1);
    }
    // get the share id for attaching sem_t rmutex
    if ((shmid3 = shmget(KEY3, sizeof(sem_t), 0666 | IPC_CREAT)) < 0) {
        perror("shmget3 ");
        exit(1);
    }

    // attach the readcount variable to all the child processes
	if ((readcount = (int *) shmat(shmid1, NULL, 0)) == NULL) {
        perror("shmat");
        exit(1);
    }
	// attach the sem_t resource variable to all the child processes
	if ((resource = (sem_t *) shmat(shmid2, NULL, 0)) == NULL) {
        perror("shmat");
        exit(1);
    }
	// attach the sem_t mutex variable to all the child processes
	if ((rmutex = (sem_t *) shmat(shmid3, NULL, 0)) == NULL) {
        perror("shmat");
        exit(1);
    }

    // set readcount = 0
    *readcount = 0;

    // initialize the semaphore variables
	sem_init(resource, 0, 1);
	sem_init(rmutex, 0, 1);

	int writer_no = 0;
	// create processes for writers
	for (int i = 0; i < num_writers; i ++) {
		writer_no ++;
		int pid = fork();
		if (pid == -1) {
			perror("Fork error");
			return 0;
		}
		if (pid == 0) {
			// child process, i.e. writer
			sem_wait(resource);
	
			// writing
			// sleep for random time
			int sleep_time = 1 + (rand() % 3);
			printf("Writer %d writing for %d seconds.\n\n", writer_no, sleep_time);
			// sleep(sleep_time);

			sem_post(resource);

			// finished writer process, exit
			exit(0);
		}
	}
	
	// create processes for readers
	int reader_no = 0;
	for (int i = 0; i < num_readers; i ++) {
		reader_no ++;
		int pid = fork();
		if (pid == -1) {
			perror("Fork error");
			return 0;
		}
		if (pid == 0) {
			// child process, i.e. reader	
			sem_wait(rmutex);
			(*readcount) ++;
			if (*readcount == 1) {
				sem_wait(resource);
				printf("\n\n------First reader------\n\n");
			}
			sem_post(rmutex);

			// reading process
			// sleep for random time
			int sleep_time = 1 + (rand() % 3);
			printf("\t\tReader %d (readcount: %d) is reading for %d seconds.\n\n", reader_no, *readcount, sleep_time);
			// sleep(sleep_time);

			sem_wait(rmutex);
			(*readcount) --;
			if ((*readcount) == 0) {
				printf("\n\n------Last reader------\n\n");
				sem_post(resource);
			}

			sem_post(rmutex);

			// finished reader process, exit
			exit(0);
		}
	}

	return 0;
} 