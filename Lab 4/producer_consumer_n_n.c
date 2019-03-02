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

int BUFFER_SIZE; // maximum size of the buffer
int *buffer; // buffer to be used by the producer and consumer
int filled = 0; // points to the next empty slot in the buffer
int used = 0; // points to the next unused slot in the buffer
int val = 0; // the value which is produced by the producer

sem_t empty; // empty spaces
sem_t full; // filled spaces
sem_t mutex; // to guarantee mutual exclusion among multiple producers and consumers

// produce an item
void produce(int value) {
	// int value;
	// printf("Please enter the value to produce: \n");
	// scanf("%d", &value);
	buffer[filled] = value;
	printf("The value PRODUCED is: %d\n", value);
	filled = (filled + 1) % BUFFER_SIZE;
}

// consume an item
int consume() {
	int tmp = buffer[used];
	used = (used + 1) % BUFFER_SIZE;
	printf("\t\t\t\tThe value consumed is: %d\n", tmp);
	return tmp;
}

// producer
void *producer(void *producer_id) {
	int id = (int) producer_id;	

	sem_wait(&empty); // wait if buffer is filled, i.e. no empty spaces
	sem_wait(&mutex); // wait for lock to be free
	val = (val + 1);
	produce(val); // produce an item

	// sleep for random time
	int sleep_time = 1 + (rand() % 3);
	printf("Producer %d waiting for %d seconds.\n\n", id + 1, sleep_time);
	sleep(sleep_time);

	sem_post(&mutex); // release the lock	
	sem_post(&full);
}

// consumer
void *consumer(void *consumer_id) {
	int id = (int) consumer_id;
	int i, tmp = 0;

	sem_wait(&full); // wait if buffer is empty, i.e. no filled spaces
	sem_wait(&mutex); // wait for lock to be free
	tmp = consume(); // consume an item

	// sleep for random time
	int sleep_time = 1 + (rand() % 3);
	printf("\t\t\t\tConsumer %d waiting for %d seconds.\n\n", id + 1, sleep_time);
	sleep(sleep_time);

	sem_post(&mutex); // release the lock
	sem_post(&empty);
}

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

	buffer = (int *) malloc (BUFFER_SIZE * sizeof(int));

	// initialize the semaphore variables
	sem_init(&empty, 0, BUFFER_SIZE);
	sem_init(&full, 0, 0);
	sem_init(&mutex, 0, 1); // intialize with 1, because binary semaphore

	// threads for producer and consumer
	pthread_t p[num_producers], c[num_consumers];

	// create threads for producers
	for (int i = 0; i < num_producers; i ++) {
		pthread_create(&p[i], NULL, producer, (void *)i);
	}
	
	// create threads for consumers
	for (int i = 0; i < num_consumers; i ++) {
		pthread_create(&c[i], NULL, consumer, (void *)i);
	}

	// wait for the threads to finish
	
	// wait for producer threads
	for (int i = 0; i < num_producers; i ++) {
		pthread_join(p[i], NULL);	
	}

	// wait for consumer threads
	for (int i = 0; i < num_consumers; i ++) {
		pthread_join(c[i], NULL);
	}

	return 0;
}