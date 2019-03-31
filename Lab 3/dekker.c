#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
//#include <sys/types.h>
//#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/wait.h>

#define FIRST 1
#define SECOND 2
#define KRED  "\x1B[31m"
#define KGRN  "\x1B[32m"
#define KNRM  "\x1B[0m"

int shmID, // identifier for shared memory to store processNumber
    shrID; // identifier for shared memory to store shared value
int p1ID, p2ID, favor_ID;
key_t key1 = 54991, key2 = 784519, key3 = 12345, key4 = 54321, key5 = 11111;
int *processNumber, *shared;
int *p1_wants_to_enter, *p2_wants_to_enter;
int *favoured;

int main() {
    shmID = shmget(key1, sizeof(int), IPC_CREAT | 0660); // flag
    if (shmID < 0) {
        printf("Shared memory 1 could not be allocated. Quitting.\n");
        exit(1);
    }

    shrID = shmget(key2, sizeof(int), IPC_CREAT | 0660); // flag
    if (shrID < 0) {
        printf("Shared memory 2 could not be allocated. Quitting.\n");
        exit(1);
    }

    p1ID = shmget(key3, sizeof(int), IPC_CREAT | 0660);
    if (p1ID < 0) {
        printf("Shared memory 3 could not be allocated. Quitting.\n");
    }

    p2ID = shmget(key4, sizeof(int), IPC_CREAT | 0660);
    if (p2ID < 0) {
        printf("Shared memory 4 could not be allocated. Quitting.\n");
    }

    favor_ID = shmget(key5, sizeof(int), IPC_CREAT | 0660);
    if (favor_ID < 0) {
        printf("Shared memory 4 could not be allocated. Quitting.\n");
    }

    // Initialization is done in parent process
    // shmat : attach shared memory segment. Returns (void *)-1 on failure else
    // Attaches shared segment identified with shmID to an address
    // processNumber in process address space

    processNumber = (int *)shmat(shmID, NULL, 0);
    if (processNumber == (int *)-1) {
        printf("Failed to attach processNumber to first process. Quitting.\n");
        exit(-1);
    }

    shared = (int *)shmat(shrID, NULL, 0);
    if (shared == (int *)-1) {
        printf("Failed to attach shared variable to first process. Quitting.\n");
        exit(-1);
    }

    p1_wants_to_enter = shmat(p1ID, NULL, 0);
    if (p1_wants_to_enter == (int *)-1) {
        printf("Failed to attach p1_wants_to_enter variable to first process. Quitting\n");
        exit(-1);
    }
    p2_wants_to_enter = shmat(p2ID, NULL, 0);
    if (p2_wants_to_enter == (int *)-1) {
        printf("Failed to attach p2_wants_to_enter variable to first process. Quitting\n");
        exit(-1);
    }

    favoured = shmat(favor_ID, NULL, 0);
    if (favoured == (int *)-1) {
        printf("Failed to attach favoured variable to first process. Quitting\n");
        exit(-1);
    }

    *shared = 0;
    *processNumber = FIRST;
    *p1_wants_to_enter = 0;
    *p2_wants_to_enter = 0;
    *favoured = 1;

    // first child process
    if (fork() == 0) {
        while (1) {
            *p1_wants_to_enter = 1;
            while (*p2_wants_to_enter == 1) {
                // printf("1: p1_wants_to_enter:%d p2_wants_to_enter:%d\n", *p1_wants_to_enter, *p2_wants_to_enter);
                if (*favoured == 2) {
                    *p1_wants_to_enter = 0;
                    while (*favoured == 2)
                        ;

                    // printf("1 wait for %f seconds\n", k);
                    *p1_wants_to_enter = 1;
                }
            }

            // Critical Section Begin
            *shared = *shared + 1;

            sleep(1);
            printf("%sFirst: Critical Section. %d %s\n", KRED, *shared, KNRM);

            *favoured = 2;
            *p1_wants_to_enter = 0;
        }
    } 
    else {
        // second child process
        if (fork() == 0) {
            while (1) {
                *p2_wants_to_enter = 1;
                while (*p1_wants_to_enter == 1) {
                    if (*favoured == 1) {
                        *p2_wants_to_enter = 0;
                        while (*favoured == 1)
                            ;
                        *p2_wants_to_enter = 1;
                    }
                }

                // Critical Section Begin
                *shared = *shared + 1;
                int sleep_time = 1 + (rand() % 3);
                sleep(sleep_time);
                printf("%sSecond: Critical Section. %d %s\n", KGRN, *shared, KNRM);
                *favoured = 1;
                *p2_wants_to_enter = 0;
            }
        }
        else {
            wait(NULL);
            wait(NULL);
        }
    }
}