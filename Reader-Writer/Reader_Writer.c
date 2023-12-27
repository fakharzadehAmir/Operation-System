#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>

#define BUFFER_SIZE 100
#define NUM_WRITERS 3
#define NUM_READERS 10

int buffer[BUFFER_SIZE];
int readerCount = 0;
int bufferCount = 0;

sem_t rw_mutex;
sem_t mutex;

void* writer(void* args) {
    int pid = *(int *) args;
    while (1) {
        sem_wait(&rw_mutex);
        if (bufferCount >= BUFFER_SIZE) {
            sem_post(&rw_mutex);
            break;
        }
        int newElement = rand(); 
        buffer[bufferCount] = newElement;
        bufferCount++;
        printf("Writer %d wrote %d to buffer position %d\n", pid, newElement, bufferCount);
        sem_post(&rw_mutex);
        sleep(1);

    }
    return NULL;
}


void* reader(void* args) {
    int pid = *(int *) args;
    while (1) {
        sem_wait(&mutex);
        readerCount++;
        if (readerCount == 1)
            sem_wait(&rw_mutex);
        sem_post(&mutex);

        int readElement = buffer[bufferCount - 1];
        printf("Reader %d reads %d from buffer position %d, readers: %d\n", pid, readElement, bufferCount, readerCount);

        sem_wait(&mutex);
        readerCount--;
        if (readerCount == 0)
            sem_post(&rw_mutex);
        sem_post(&mutex);
        usleep(1000000);

    }
    return NULL;
}

int main() {
    pthread_t writers[NUM_WRITERS], readers[NUM_READERS];
    int writer_ids[NUM_WRITERS], reader_ids[NUM_READERS];

    sem_init(&mutex, 0, 1);
    sem_init(&rw_mutex, 0, 1);

    for (int i = 0; i < NUM_WRITERS; i++) {
        writer_ids[i] = i;
        pthread_create(&writers[i], NULL, writer, &writer_ids[i]);

    }

    for (int i = 0; i < NUM_READERS; i++) {
        reader_ids[i] = i;
        pthread_create(&readers[i], NULL, reader, &reader_ids[i]);

    }

    for (int i = 0; i < NUM_WRITERS; i++) {
        pthread_join(writers[i], NULL);
    }
    for (int i = 0; i < NUM_READERS; i++) {
        pthread_join(readers[i], NULL);
    }

    sem_destroy(&mutex);
    sem_destroy(&rw_mutex);

    return 0;
}
