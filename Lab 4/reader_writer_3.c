// multiple readers and multiple writers problem using semaphores with threads
// fair solution

#include <pthread.h>
#include <assert.h>
#include <sched.h>
#include <semaphore.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

sem_t resourceAccess;
sem_t readCountAccess;
sem_t serviceQueue;
int readcount = 0;

// reader
void *reader(void *reader_id) {
	int id = (int) reader_id;		
	sem_wait(&serviceQueue);
	sem_wait(&readCountAccess);
	
	if (readcount == 0) {
		sem_wait(&resourceAccess);
		printf("\n\n------First reader------\n\n");
	}
	readcount++;

	sem_post(&serviceQueue);
	sem_post(&readCountAccess);

	// reading process
	// sleep for random time
	int sleep_time = 1 + (rand() % 3);
	printf("\t\tReader %d is reading for %d seconds.\n\n", id + 1, sleep_time);
	sleep(sleep_time);

	sem_wait(&readCountAccess);
	readcount --;
	if (readcount == 0) {
		printf("\n\n------Last reader------\n\n");
		sem_post(&resourceAccess);
	}

	sem_post(&readCountAccess); 
}

// writer
void *writer(void *writer_id) {
	int id = (int) writer_id;

	sem_wait(&serviceQueue);
	sem_wait(&resourceAccess);

	sem_post(&serviceQueue);

	// writing
	// sleep for random time
	int sleep_time = 1 + (rand() % 3);
	printf("Writer %d writing for %d seconds.\n\n", id + 1, sleep_time);
	sleep(sleep_time);

	sem_post(&resourceAccess);
}

int main(int argc, char **argv) {

	if (argc != 3) {
		printf("Usage of the program: ./reader_writer <no. of writers> <no. of readers>\n");
		return 0;
	}

	int num_writers = atoi(argv[1]);
	printf("Number of writers: %d\n", num_writers);

	int num_readers = atoi(argv[2]);
	printf("Number of readers is: %d\n\n", num_readers);

	// initialize the semaphore variables
	sem_init(&resourceAccess, 0, 1);
	sem_init(&readCountAccess, 0, 1);
	sem_init(&serviceQueue, 0, 1);
	
	// threads for readers and writers
	pthread_t r[num_readers], w[num_writers];
	
	// create threads for writers
	for (int i = 0; i < num_writers; i ++) {
		pthread_create(&w[i], NULL, writer, (void *)i);
	}
	
	// create threads for readers
	for (int i = 0; i < num_readers; i ++) {
		pthread_create(&r[i], NULL, reader, (void *)i);
	}

	// wait for the threads to finish
	
	// wait for writer threads
	for (int i = 0; i < num_writers; i ++) {
		pthread_join(w[i], NULL);	
	}

	// wait for reader threads
	for (int i = 0; i < num_readers; i ++) {
		pthread_join(r[i], NULL);
	}

	return 0;
}