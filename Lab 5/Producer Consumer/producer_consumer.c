// multiple producer and multiple consumers problem using semaphores with threads
#include <pthread.h>
#include <assert.h>
#include <sched.h>
#include <semaphore.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <sys/ipc.h>
#include <sys/shm.h>

int BUFFER_SIZE; // maximum size of the buffer
int *buffer; // buffer to be used by the producer and consumer
int *filled; // points to the next empty slot in the buffer
int *used; // points to the next unused slot in the buffer
int *val; // the value which is produced by the producer

sem_t *empty; // empty spaces
sem_t *full; // filled spaces
sem_t *mutex; // to guarantee mutual exclusion among multiple producers and consumers

int shmid1, shmid2, shmid3, shmid4;
int shmsid1, shmsid2, shmsid3;

#define KEY1 1234567
#define KEY2 2345671
#define KEY3 3456712
#define KEY4 4567123
#define KEY5 5671234
#define KEY6 6712345
#define KEY7 7123456

int main(int argc, char **argv) {

	if (argc != 4) {
		printf("Usage of the program: ./producer_consumer <no. of producers> <no. of consumers> <buffer size>\n");
		return 0;
	}

	int num_producers = atoi(argv[1]);
	printf("Number of producers: %d\n", num_producers);

	int num_consumers = atoi(argv[2]);
	printf("Number of consumers is: %d\n", num_consumers);

	BUFFER_SIZE = atoi(argv[3]);
	printf("The size of the buffer is: %d\n\n", BUFFER_SIZE);

	// get the share ids
    if ((shmid1 = shmget(KEY1, sizeof(int), 0666 | IPC_CREAT)) < 0) {
        perror("shmget1 ");
        exit(1);
    }
    if ((shmid2 = shmget(KEY2, sizeof(int), 0666 | IPC_CREAT)) < 0) {
        perror("shmget2 ");
        exit(1);
    }
    if ((shmid3 = shmget(KEY3, sizeof(int), 0666 | IPC_CREAT)) < 0) {
        perror("shmget3 ");
        exit(1);
    }
    if ((shmid4 = shmget(KEY4, sizeof(int), 0666 | IPC_CREAT)) < 0) {
        perror("shmget4 ");
        exit(1);
    }
    if ((shmsid1 = shmget(KEY5, sizeof(sem_t), 0666 | IPC_CREAT)) < 0) {
        perror("shmget5 ");
        exit(1);
    }
    if ((shmsid2 = shmget(KEY6, sizeof(sem_t), 0666 | IPC_CREAT)) < 0) {
        perror("shmget6 ");
        exit(1);
    }
    if ((shmsid3 = shmget(KEY7, sizeof(sem_t), 0666 | IPC_CREAT)) < 0) {
        perror("shmget7 ");
        exit(1);
    }

    // attach the shared variables to all the processes
    if ((buffer = (int *) shmat(shmid1, NULL, 0)) == NULL) {
        perror("shmat1 ");
        exit(1);
    }
    if ((filled = (int *) shmat(shmid2, NULL, 0)) == NULL) {
        perror("shmat1 ");
        exit(1);
    }
    if ((used = (int *) shmat(shmid3, NULL, 0)) == NULL) {
        perror("shmat1 ");
        exit(1);
    }
    if ((val = (int *) shmat(shmid4, NULL, 0)) == NULL) {
        perror("shmat1 ");
        exit(1);
    }    
    if ((empty = (sem_t *) shmat(shmsid1, NULL, 0)) == NULL) {
        perror("shmat1 ");
        exit(1);
    }    
    if ((full = (sem_t *) shmat(shmsid2, NULL, 0)) == NULL) {
        perror("shmat1 ");
        exit(1);
    }    
    if ((mutex = (sem_t *) shmat(shmsid3, NULL, 0)) == NULL) {
        perror("shmat1 ");
        exit(1);
    }

	// initialize the shared variables
	sem_init(empty, 0, BUFFER_SIZE);
	sem_init(full, 0, 0);
	sem_init(mutex, 0, 1); // intialize with 1, because binary semaphore

	*filled = 0;
	*used = 0;
	*val = 0;
	// buffer = (int *) malloc (BUFFER_SIZE * sizeof(int));

	// create processes for producers
	int producer_no = 0;
	for (int i = 0; i < num_producers; i ++) {
		producer_no ++;

		int pid = fork();
		if (pid == -1) {
			perror("Fork error: ");
			return 0;
		}
		if (pid == 0) {
			while (1) {
				sem_wait(empty); // wait if buffer is filled, i.e. no empty spaces
				sem_wait(mutex); // wait for lock to be free
				*val = (*val) + 1;
				// produce an item
				*(buffer + *filled) = (*val);
				printf("The value PRODUCED is: %d\n", *val);
				*filled = (*filled + 1) % BUFFER_SIZE;
				// sleep for random time
				int sleep_time = 1 + (rand() % 3);
				printf("Producer %d waiting for %d seconds.\n\n", producer_no + 1, sleep_time);
				// sleep(sleep_time);

				sem_post(mutex); // release the lock	
				sem_post(full);
			}
		}
	}
	
	// create processes for consumers
	int consumer_no = 0;
	for (int i = 0; i < num_consumers; i ++) {
		consumer_no ++;
		int pid = fork();
		if (pid == -1) {
			perror("Fork error: ");
			return 0;
		}
		
		if (pid == 0) {
			int tmp = 0;
			while (1) {
				sem_wait(full); // wait if buffer is empty, i.e. no filled spaces
				sem_wait(mutex); // wait for lock to be free

				// consume an item
				tmp = *(buffer + *used)	;
				*used = (*used + 1) % BUFFER_SIZE;
				printf("\t\t\t\tThe value consumed is: %d\n", tmp);

				// sleep for random time
				int sleep_time = 1 + (rand() % 3);
				printf("\t\t\t\tConsumer %d waiting for %d seconds.\n\n", consumer_no + 1, sleep_time);
				// sleep(sleep_time);

				sem_post(mutex); // release the lock
				sem_post(empty);
			}
		}
	}

	return 0;
}