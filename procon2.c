#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#define N_COUNTER 4 // the size of a shared buffer
#define MILLI 1000  // time scale

void mywrite(int n);
int myread();
sem_t semWrite, semRead; // POSIX semaphore

int queue[N_COUNTER]; // shared buffer
int wptr; // write pointer for queue[]
int rptr; // read pointer for queue[]
int turn;
int interested[2];

void lock(int threadId) {
    interested[threadId] = 1;
    turn = 1 - threadId;
    while (interested[1 - threadId] == 1 && turn == 1 - threadId) {
        // Busy waiting
    }
}

void unlock(int threadId) {
    interested[threadId] = 0;
}

// producer thread function
void* producer(void* arg) {
    for (int i = 0; i < 10; i++) {
        lock(0);
        mywrite(i); /**** write i into the shared memory ****/
        printf("producer : wrote %d\n", i);
        unlock(0);

        // sleep m miliseconds
        int m = rand() % 10;
        usleep(MILLI * m * 10); // m*10
    }
    return NULL;
}

// consumer thread function
void* consumer(void* arg) {
    for (int i = 0; i < 10; i++) {
        lock(1);
        int n = myread(); /**** read a value from the shared memory ****/
        printf("\tconsumer : read %d\n", i);
        unlock(1);

        // sleep m miliseconds
        int m = rand() % 10;
        usleep(MILLI * m * 10); // m*10
    }
    return NULL;
}

// write n into the shared memory
void mywrite(int n) {
    sem_wait(&semWrite); // wait for available write slot

    queue[wptr] = n; // write the value into the shared buffer
    wptr = (wptr + 1) % N_COUNTER; // update the write pointer

    sem_post(&semRead); // signal the availability of data for reading
}

// read a value from the shared memory
int myread() {
    sem_wait(&semRead); // wait for available data to read

    int n = queue[rptr]; // read the value from the shared buffer
    rptr = (rptr + 1) % N_COUNTER; // update the read pointer

    sem_post(&semWrite); // signal the availability of write slot

    return n;
}

int main() {
    pthread_t t[2]; // thread structure
    srand(time(NULL));

    // init semaphore
    sem_init(&semWrite, 0, N_COUNTER); // initialize semWrite with the maximum number of write slots
    sem_init(&semRead, 0, 0); // initialize semRead with 0 read slots

    // create the threads for the producer and consumer
    pthread_create(&t[0], NULL, producer, NULL);
    pthread_create(&t[1], NULL, consumer, NULL);

    for (int i = 0; i < 2; i++)
        pthread_join(t[i], NULL); // wait for the threads

    //destroy the semaphores
    sem_destroy(&semWrite);
    sem_destroy(&semRead);

    return 0;
}
