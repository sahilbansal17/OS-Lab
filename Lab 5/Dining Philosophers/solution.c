#include <pthread.h>
#include <semaphore.h>
#include <stdio.h>
#include <stdlib.h>

int N; // number of philosophers
#define MAX 100
#define KRED  "\x1B[31m"
#define KGRN  "\x1B[32m"
#define KNRM  "\x1B[0m"

sem_t chopstick[MAX];


// dining philosopher's problem
// solution 1: each philosophper tries to acquire two chopsticks
void *philosopher_solution_1(void* id) {
	int no = (int) id;
	// acquire both the chopsticks
	sem_wait(&chopstick[no]);
	// sleep(1);
	printf("Philosopher %d ---- picks up left chopstick %d\n", no, no);
	sem_wait(&chopstick[(no + 1)%N]);
	// sleep(1);
	printf("Philosopher %d ---- picks up right chopstick %d\n", no, (no + 1)%N);
	printf("\t\t\t%sPhilosopher %d ---- begins eating%s\n", KGRN, no, KNRM);
	sleep(1);
	printf("\t\t\t%sPhilosopher %d ---- ends eating%s\n", KRED, no, KNRM);
	// release both the chopsticks
	sem_post(&chopstick[no]);
	sem_post(&chopstick[(no + 1)%N]);
}

// solution 2:
// each philosopher tries to acquire first left chopstick and then right chopstick
void *philosopher_solution_2(void* id) {
	int no = (int) id;

	// acquire the left chopstick
	sem_wait(&chopstick[no]);
	sleep(1);
	printf("Philosopher %d ---- picks up left chopstick %d\n", no, no);
	
	// acquire the right chopstick
	sem_wait(&chopstick[(no + 1)%N]);
	
	printf("Philosopher %d ---- picks up right chopstick %d\n", no, (no + 1)%N);
	printf("\t\t\t%sPhilosopher %d ---- begins eating%s\n", KGRN, no, KNRM);
	sleep(1);
	printf("\t\t\t%sPhilosopher %d ---- ends eating%s\n", KRED, no, KNRM);
	
	// release both the chopsticks
	sem_post(&chopstick[no]);
	sem_post(&chopstick[(no + 1)%N]);

}

// solution 3:
// restrict no of philosophers eating at any time to 4
sem_t pcount;
void *philosopher_solution_3(void* id) {
	int no = (int) id;

	sem_wait(&pcount);
	// acquire both the chopsticks
	sem_wait(&chopstick[no]);
	printf("Philosopher %d ---- picks up left chopstick %d\n", no, no);
	sem_wait(&chopstick[(no + 1) % N]);
	printf("Philosopher %d ---- picks up right chopstick %d\n", no, (no + 1)%N);
	printf("\t\t\t%sPhilosopher %d ---- begins eating%s\n", KGRN, no, KNRM);
	sleep(1);
	printf("\t\t\t%sPhilosopher %d ---- ends eating%s\n", KRED, no, KNRM);
	// release both the chopsticks
	sem_post(&chopstick[no]);
	sem_post(&chopstick[(no + 1)%N]);

	sem_post(&pcount);
}

// solution 4
// chopsticks are numbered, each philosopher tries to acquire lower numbered first
void *philosopher_solution_4(void* id) {
	int no = (int) id;

	int left = no;
	int right = (no + 1) % N;
	if (left < right) {
		sem_wait(&chopstick[left]);
		printf("Philosopher %d ---- picks up left chopstick %d\n", no, no);
		sem_wait(&chopstick[right]);
		printf("Philosopher %d ---- picks up right chopstick %d\n", no, (no + 1)%N);
		printf("\t\t\t%sPhilosopher %d ---- begins eating%s\n", KGRN, no, KNRM);
		sleep(1);
		printf("\t\t\t%sPhilosopher %d ---- ends eating%s\n", KRED, no, KNRM);
		// release both the chopsticks
		sem_post(&chopstick[right]);
		sem_post(&chopstick[left]);
	}
	else {
		sem_wait(&chopstick[right]);
		printf("Philosopher %d ---- picks up right chopstick %d\n", no, (no + 1)%N);
		sem_wait(&chopstick[left]);
		printf("Philosopher %d ---- picks up left chopstick %d\n", no, no);
		printf("\t\t\t%sPhilosopher %d ---- begins eating%s\n", KGRN, no, KNRM);
		sleep(1);
		printf("\t\t\t%sPhilosopher %d ---- ends eating%s\n", KRED, no, KNRM);
		// release both the chopsticks
		sem_post(&chopstick[left]);
		sem_post(&chopstick[right]);
	}
}

// solution 5:
// all odd numbered philosopher try to pick left chopstick and then right,
// even numbered philosopher try to pick right chopstick and then left
void *philosopher_solution_5(void* id) {
	int no = (int) id;

	int left = no;
	int right = (no + 1) % N;

	if (no & 1) {
		// odd philospher
		sem_wait(&chopstick[left]);
		printf("Philosopher %d ---- picks up left chopstick %d\n", no, no);
		sem_wait(&chopstick[right]);
		printf("Philosopher %d ---- picks up right chopstick %d\n", no, (no + 1)%N);
		printf("\t\t\t%sPhilosopher %d ---- begins eating%s\n", KGRN, no, KNRM);
		sleep(1);
		printf("\t\t\t%sPhilosopher %d ---- ends eating%s\n", KRED, no, KNRM);
		// release both the chopsticks
		sem_post(&chopstick[right]);
		sem_post(&chopstick[left]);
	}
	else {
		// even philosopher
		sem_wait(&chopstick[right]);
		printf("Philosopher %d ---- picks up right chopstick %d\n", no, (no + 1)%N);
		sem_wait(&chopstick[left]);
		printf("Philosopher %d ---- picks up left chopstick %d\n", no, no);
		printf("\t\t\t%sPhilosopher %d ---- begins eating%s\n", KGRN, no, KNRM);
		sleep(1);
		printf("\t\t\t%sPhilosopher %d ---- ends eating%s\n", KRED, no, KNRM);
		// release both the chopsticks
		sem_post(&chopstick[left]);
		sem_post(&chopstick[right]);
	}
}

// solution 6:
// solution using arbitrator (philosopher needs to request an arbitrator for chopstick).
sem_t mutex;
void *philosopher_solution_6(void* id) {
	int no = (int) id;
	// request permission from arbitrator
	sem_wait(&mutex);
	// acquire both the chopsticks
	sem_wait(&chopstick[no]);
	// sleep(1);
	printf("Philosopher %d ---- picks up left chopstick %d\n", no, no);
	sem_wait(&chopstick[(no + 1)%N]);
	// sleep(1);
	printf("Philosopher %d ---- picks up right chopstick %d\n", no, (no + 1)%N);
	printf("\t\t\t%sPhilosopher %d ---- begins eating%s\n", KGRN, no, KNRM);
	sleep(1);
	printf("\t\t\t%sPhilosopher %d ---- ends eating%s\n", KRED, no, KNRM);
	// release both the chopsticks
	sem_post(&chopstick[no]);
	sem_post(&chopstick[(no + 1)%N]);
	sem_post(&mutex);
}

// solution 7:
// chandy mishra solution
void *philosopher_solution_7(void* id) {

}

int main (int argc, char **argv) {

	if (argc != 3) {
		printf("Usage of the program: ./dining_philosopher <solution no.> <no. of philosophers>\n");
		return 0;
	}

	int sol_no = atoi(argv[1]);
	N = atoi(argv[2]);

	pthread_t p[N]; // philosopher threads

	// initialize semaphore for the chopsticks
	for (int i = 0; i < N; i ++) {
		int retvalue = sem_init(&chopstick[i], 0, 1); // intialize with 1 becuase binary semaphore
		if (retvalue == -1) {
			perror("Semaphore initialization failed: ");
			return 0;
		}
	}

	sem_init(&pcount, 0, 4); // restricting to 4 philosphers
	sem_init(&mutex, 0, 1);
	// create threads for philosophers

	switch(sol_no) {
		case 1:
				for (int i = 0; i < N; i ++) {
					int retvalue = pthread_create(&p[i], NULL, philosopher_solution_1, (void *)i);
					if (retvalue != 0) {
						perror("pthread creation error: ");
						return 0;
					}
				}
				break;
		case 2:
				for (int i = 0; i < N; i ++) {
					int retvalue = pthread_create(&p[i], NULL, philosopher_solution_2, (void *)i);
					if (retvalue != 0) {
						perror("pthread creation error: ");
						return 0;
					}
				}
				break;
		case 3: 
				for (int i = 0; i < N; i ++) {
					int retvalue = pthread_create(&p[i], NULL, philosopher_solution_3, (void *)i);
					if (retvalue != 0) {
						perror("pthread creation error: ");
						return 0;
					}
				}
				break;
		case 4:
				for (int i = 0; i < N; i ++) {
					int retvalue = pthread_create(&p[i], NULL, philosopher_solution_4, (void *)i);
					if (retvalue != 0) {
						perror("pthread creation error: ");
						return 0;
					}
				}
				break;
		case 5:
				for (int i = 0; i < N; i ++) {
					int retvalue = pthread_create(&p[i], NULL, philosopher_solution_5, (void *)i);
					if (retvalue != 0) {
						perror("pthread creation error: ");
						return 0;
					}
				}
				break;
		case 6:
				for (int i = 0; i < N; i ++) {
					int retvalue = pthread_create(&p[i], NULL, philosopher_solution_6, (void *)i);
					if (retvalue != 0) {
						perror("pthread creation error: ");
						return 0;
					}
				}
				break;
		case 7:
				for (int i = 0; i < N; i ++) {
					int retvalue = pthread_create(&p[i], NULL, philosopher_solution_7, (void *)i);
					if (retvalue != 0) {
						perror("pthread creation error: ");
						return 0;
					}
				}
				break;
	}

	/*/
	for (int i = 0; i < N; i ++) {
		int retvalue = pthread_create(&p[i], NULL, philosopher, (void *)i);
		if (retvalue != 0) {
			perror("pthread creation error: ");
			return 0;
		}
	}
	//*/

	// wait for threads to finish
	for (int i = 0; i < N; i ++) {
		int retvalue = pthread_join(p[i], NULL);
		if (retvalue != 0) {
			perror("pthread join error: ");
			return 0;
		}
	}

	// destroy the semaphores
	for (int i = 0; i < N; i ++) {
		int retvalue = sem_destroy(&chopstick[i]);
		if (retvalue == -1) {
			perror("Semaphore destroy error: ");
			return 0;
		}
	}
	return 0;
}