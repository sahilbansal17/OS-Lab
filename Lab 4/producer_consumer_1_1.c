// one producer and one consumer problem using semaphores
#include <pthread.h>
#include <assert.h>
#include <sched.h>
#include <semaphore.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

#define MAX 1 // maximum size of the buffer
int buffer[MAX]; // buffer to be used by the producer and consumer
int filled = 0; // points to the next empty slot in the buffer
int used = 0; // points to the next unused slot in the buffer

sem_t empty; // empty spaces
sem_t full; // filled spaces

// produce an item
void produce() {
	int value;
	printf("Please enter the value to produce: ");
	scanf("%d", &value);
	buffer[filled] = value;
	filled = (filled + 1) % MAX;
}

// consume an item
int consume() {
	int tmp = buffer[used];
	used = (used + 1) % MAX;
	return tmp;
}

// producer
void *producer(void *arg) {
	sem_wait(&empty); // wait if buffer is filled, i.e. no empty spaces
	produce(); // produce an item
	sem_post(&full);
}

// consumer
void *consumer(void *arg) {
	int i, tmp = 0;
	sem_wait(&full); // wait if buffer is empty, i.e. no filled spaces
	tmp = consume(); // consume an item
	sem_post(&empty);
	printf("The value consumed is: %d\n", tmp);
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