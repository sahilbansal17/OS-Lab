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

#define N 10
int choosing[10], tickets[10];

// producer
void *process(void *process_id) {
	int x = (int) process_id;	

	int done = 0;
	while (!done) {
		choosing[x] = 1;
		int max_ticket = tickets[0];
		for (int i = 1; i < N; i ++) {
			if (tickets[i] > max_ticket) {
				max_ticket = tickets[i];
			}
		}
		tickets[x] = max_ticket + 1;
		choosing[x] = 0;

		for (int i = 0; i < N; i ++) {
			if (i == x) {
				continue;
			}
			while (choosing[i] != 0) ;
			while (tickets[i]  != 0 && tickets[i] < tickets[x]) ;
			if (tickets[i] == tickets[x] && i < x) {
				while(tickets[i] != 0) ;
			}
		}
		// critical section
		printf("Process %d in the critical section\n", x);
		tickets[x] = 0;
		int sleep_time = 1 + (rand() % 10);
		sleep(sleep_time);
	}
}

int main(int argc, char **argv) {
	
	// intialize choosing and tickets
	for (int i = 0; i < N; i ++) {
		choosing[i] = 0;
		tickets[i] = 0;
	}
	// threads declaration
	pthread_t p[N];

	// create threads 
	for (int i = 0; i < N; i ++) {
		pthread_create(&p[i], NULL, process, (void *)i);
	}
	
	// wait for the threads to finish
	for (int i = 0; i < N; i ++) {
		pthread_join(p[i], NULL);	
	}

	return 0;
}