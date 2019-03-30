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

    // Create First (child) process
    if (fork() == 0) {
        // This is the code first process executes

        for (int i = 0; i < 20; i++) {
            *p1_wants_to_enter = 1;
            *favoured = 2;
            while (*p2_wants_to_enter && *favoured == 2) ;


            // Wait

            // Critical Section Begin
            int sleep_time = 1 + (rand() % 3);
            sleep(sleep_time);
            *shared = *shared + 1;
            printf("[%d] First: Critical Section (%2d).\n", i + 1, *shared);

            *p1_wants_to_enter = 0;
        }
    }
    else {
        // Create second (child) process
        if (fork() == 0) {
            // This is the code second process executes

            for (int i = 0; i < 20; i++) {
                *p2_wants_to_enter = 1;
                *favoured = 1;
                while (*p1_wants_to_enter && *favoured == 1);
                // Wait

                // Critical Section Begin
                *shared = *shared + 1;
                int sleep_time = 1 + (rand() % 3);
                sleep(sleep_time);
                printf("[%d] Second: Critical Section (%2d).\n", i + 1, *shared);
                *p2_wants_to_enter = 0;
            }

        } else {
            wait(NULL);
            wait(NULL);
        }
    }
}