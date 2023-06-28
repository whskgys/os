#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>
#include <stdlib.h>

#define N_COUNTER 4 // the size of a shared buffer
#define MILLI 1000  // time scale

void mywrite (int n);
int myread();

pthread_mutex_t critical_section; // POSIX mutex
sem_t semWrite, semRead; // POSIX semaphore
int queue[N_COUNTER]; // shared buffer
int wptr; // write pointer for queue[]
int rptr; // read pointer for queue[]

// producer thread function
void* producer(void* arg) {
  for(int i=0; i<10; i++) {
    mywrite(i); /**** write i into the shared memory ****/
    printf("producer : wrote %d\n", i);

    // sleep m miliseconds
    int m = rand()%10;
    usleep(MILLI*m*10); // m*10
  }
  return NULL;
}

// consumer thread function
void* consumer(void* arg) {
  for(int i=0; i<10; i++) {
    int n = myread(); /**** read a value from the shared memory ****/
    printf("\tconsumer : read %d\n", i);

    // sleep m miliseconds
    int m = rand()%10;
    usleep(MILLI*m*10); // m*10
  }
  return NULL;
}

// write n into the shared memory
void mywrite(int n) {
    sem_wait(&semWrite); // wait for available write slot
   pthread_mutex_lock(&critical_section); // acquire the mutex

    queue[wptr] = n; // write the value into the shared buffer
    wptr = (wptr + 1) % N_COUNTER; // update the write pointer

    pthread_mutex_unlock(&critical_section); // release the mutex

    sem_post(&semRead); // signal the availability of data for reading
}

// write a value from the shared memory
int myread() {
    sem_wait(&semRead); // wait for available data to read

    pthread_mutex_lock(&critical_section); // acquire the mutex

    int n = queue[rptr]; // read the value from the shared buffer
    rptr = (rptr + 1) % N_COUNTER; // update the read pointer

    pthread_mutex_unlock(&critical_section); // release the mutex

    sem_post(&semWrite); // signal the availability of write slot

  return n;
}

int main() {
  pthread_t t[2]; // thread structure
  srand(time(NULL));

  pthread_mutex_init(&critical_section, NULL); // init mutex

  // init semaphore
  sem_init(&semWrite, 0, N_COUNTER); // initialize semWrite with the maximum number of write slots
  sem_init(&semRead, 0, 0); // initialize semRead with 0 read slots

  // create the threads for the producer and consumer
  pthread_create(&t[0], NULL, producer, NULL);
  pthread_create(&t[1], NULL, consumer, NULL);

  for(int i=0; i<2; i++)
    pthread_join(t[i],NULL); // wait for the threads

  //destroy the semaphores
  sem_destroy(&semWrite);
  sem_destroy(&semRead);


  pthread_mutex_destroy(&critical_section); // destroy mutex
  return 0;
}
