// one producer and one consumer problem using semaphores with threads
#include <pthread.h>
#include <assert.h>
#include <sched.h>
#include <semaphore.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

#define MAX 5 // maximum size of the buffer
#define loops 10 // maximum no of times producer and consumer process run
int buffer[MAX]; // buffer to be used by the producer and consumer
int filled = 0; // points to the next empty slot in the buffer
int used = 0; // points to the next unused slot in the buffer

sem_t empty; // empty spaces
sem_t full; // filled spaces

// produce an item
void produce(int value) {
	// int value;
	// printf("Please enter the value to produce: \n");
	// scanf("%d", &value);
	buffer[filled] = value;
	printf("The value PRODUCED is: %d\n", value);
	filled = (filled + 1) % MAX;
}

// consume an item
int consume() {
	int tmp = buffer[used];
	used = (used + 1) % MAX;
	printf("The value consumed is: %d\n", tmp);
	return tmp;
}

// producer
void *producer(void *arg) {
	for (int i = 0; i < loops; i ++) {		
		sem_wait(&empty); // wait if buffer is filled, i.e. no empty spaces
		produce(i); // produce an item
		sem_post(&full);
	}
}

// consumer
void *consumer(void *arg) {
	int i, tmp = 0;
	for (int i = 0; i < loops; i ++) {
		sem_wait(&full); // wait if buffer is empty, i.e. no filled spaces
		tmp = consume(); // consume an item
		sem_post(&empty);
	}
}

int main () {

	// initialize the semaphore variables
	sem_init(&empty, 0, MAX);
	sem_init(&full, 0, 0);

	// threads for producer and consumer
	pthread_t p, c;
	pthread_create(&p, NULL, producer, NULL);
	pthread_create(&c, NULL, consumer, NULL);

	// wait for the threads to finish
	pthread_join(p, NULL);
	pthread_join(c, NULL);

	return 0;
}