#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <unistd.h>
#include <pthread.h>
#include <semaphore.h>

#define N 10 // size of the buffer

sem_t fill_count;
sem_t empty_count;
sem_t lock, mutex;
int current_size = 0;
int producer_index = 0;
int consumer_index = 0;
int value = 0;
int buffer[N];

void* producer() {
	// printf("Producer started running...\n");
	int a, b, c, d;
	while (1) {
		sem_getvalue(&fill_count, &a);
		sem_getvalue(&empty_count, &b);
		sem_getvalue(&lock, &c);
		sem_getvalue(&mutex, &d);
		// printf("/ fill_count: %d, empty_count: %d, lock: %d, mutex: %d\n", a, b, c, d); 
		sem_wait(&empty_count);
		sem_wait(&mutex);
		value ++;
		buffer[producer_index] = value;
		producer_index = (producer_index + 1) % N;
		printf("The item produced is: %d\n", value);
		current_size ++; // increase the current no. of items in the buffer
		sem_getvalue(&lock, &c);
		if (current_size == 2) {
			sem_post(&lock);
		}
		sem_post(&fill_count);
		sem_getvalue(&fill_count, &a);
		sem_getvalue(&empty_count, &b);
		sem_getvalue(&lock, &c);
		sem_getvalue(&mutex, &d);
		// printf("/ fill_count: %d, empty_count: %d, lock: %d, mutex: %d\n", a, b, c, d); 
		sem_post(&mutex);	
	}
}

void* consumer1() {
	// printf("Consumer1 started running...\n");
	int a, b, c, d;
	while (1) {
		sem_getvalue(&fill_count, &a);
		sem_getvalue(&empty_count, &b);
		sem_getvalue(&lock, &c);
		sem_getvalue(&mutex, &d);
		// printf("\t\t\t/ fill_count: %d, empty_count: %d, lock: %d, mutex: %d\n", a, b, c, d);
		sem_wait(&fill_count);
		sem_wait(&mutex);
		if (current_size == 1) {
			sem_wait(&lock); // cannot get the lock if C2 has entered just before, lock released when buffer size=2
			int value = buffer[consumer_index];
			consumer_index = (consumer_index + 1) % N;
			printf("\t\t\tThe item consumed is: %d\n", value);
			current_size --;
			sem_post(&lock);
			sem_post(&empty_count);
			sem_post(&mutex);
			continue;	
		}
		int value = buffer[consumer_index];
		consumer_index = (consumer_index + 1) % N;
		printf("\t\t\tThe item consumed is: %d\n", value);
		current_size --; // decrease the current no. of items in the buffer by 1
		sem_post(&empty_count);
		sem_getvalue(&fill_count, &a);
		sem_getvalue(&empty_count, &b);
		sem_getvalue(&lock, &c);
		sem_getvalue(&mutex, &d);
		// printf("\t\t\t/ fill_count: %d, empty_count: %d, lock: %d, mutex: %d\n", a, b, c, d);
		sem_post(&mutex);
	}
}

void* consumer2() {
	
	// printf("Consumer2 started running...\n");
	int a, b, c, d;
	while (1) {
		sem_getvalue(&fill_count, &a);
		sem_getvalue(&empty_count, &b);
		sem_getvalue(&lock, &c);
		sem_getvalue(&mutex, &d);
		// printf("\t\t\t\t\t\t/ fill_count: %d, empty_count: %d, lock: %d, mutex: %d\n", a, b, c, d);
		sem_wait(&fill_count);
		if (current_size == 1) {
			sem_wait(&lock); // cannot get the lock if size was 1 and it got into critical section the previous time
			sem_post(&fill_count);
			continue;		
		}
		sem_wait(&fill_count); // size >= 2, decrease fill_count once more
		sem_wait(&mutex);
		int value1 = buffer[consumer_index];
		consumer_index = (consumer_index + 1) % N;
		int value2 = buffer[consumer_index];
		consumer_index = (consumer_index + 1) % N;
		printf("\t\t\t\t\t\tThe items consumed are: %d, %d\n", value1, value2);
		current_size -= 2; // decrease the current no. of items in the buffer by 2
		sem_post(&empty_count);
		sem_post(&empty_count);
		sem_getvalue(&fill_count, &a);
		sem_getvalue(&empty_count, &b);
		sem_getvalue(&lock, &c);
		sem_getvalue(&mutex, &d);
		// printf("\t\t\t\t\t\t/ fill_count: %d, empty_count: %d, lock: %d, mutex: %d\n", a, b, c, d);
		sem_post(&mutex);
	}
}

int main () {
	
	int num_producers = 1, num_consumers = 2;

	pthread_t p, c[2];
	
	// initialize the semaphores
	sem_init(&fill_count, 0, 0);
	sem_init(&empty_count, 0, N);
	sem_init(&lock, 0, 1);
	sem_init(&mutex, 0, 1);

	// producer thread
	int p1 = pthread_create(&p, NULL, producer, NULL);
	if (p1 != 0) {
		perror("Unable to create producer...\n");
		exit(-1);
	}

	// consumer threads
	int c1 = pthread_create(&c[0], NULL, consumer1, NULL);
	if (c1 != 0) {
		perror("Unable to create consumer 1...\n");
		exit(-1);
	}
	
	int c2 = pthread_create(&c[1], NULL, consumer2, NULL);
	if (c2 != 0) {
		perror("Unable to create consumer 2...\n");
		exit(-1);
	}

	// wait for producer thread to finish
	pthread_join(p, NULL);
	pthread_join(c[0], NULL);
	pthread_join(c[1], NULL);
	return 0;

}
