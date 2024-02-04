#include <stdio.h>
#include <unistd.h>
#include <pthread.h>
#include <semaphore.h>
#include <string.h>

#define NUMBER_OF_RESOURCES 5
#define NUMBER_OF_THREADS 10

typedef struct ResourceManager{

    int resources[NUMBER_OF_RESOURCES];
    sem_t availableResources;
    pthread_mutex_t poolMutex;

} ResourceManager;

struct ResourceManager rm;


void* useResources(void* args) {
    int* thread_id = (int *) args;
    sem_wait(&rm.availableResources);
    pthread_mutex_lock(&rm.poolMutex);
    int resource_idx = -1;
    for(int i = 0; i < NUMBER_OF_RESOURCES; i++) {
        if (rm.resources[i] < 0) {
            rm.resources[i] = *thread_id;
            resource_idx = i;
            break;
        }
    }
    pthread_mutex_unlock(&rm.poolMutex);

    if (resource_idx != -1) {
        printf("The %d is performing work with resource %d\n", *thread_id, resource_idx);

// -------- allocate resources ---------
        sleep(9/10);
// -----------------------------------------

        pthread_mutex_lock(&rm.poolMutex);
        rm.resources[resource_idx] = -1;
        pthread_mutex_unlock(&rm.poolMutex);
    }

    sem_post(&rm.availableResources);
    return NULL;
}

int main() {
    sem_init(&rm.availableResources,0, NUMBER_OF_RESOURCES);
    pthread_mutex_init(&rm.poolMutex, NULL);

    for(int i = 0; i < NUMBER_OF_RESOURCES; i++)
        rm.resources[i] = -1;

    pthread_t thread_array[NUMBER_OF_THREADS];
    int thread_id[NUMBER_OF_THREADS];
    for(int i = 0; i < NUMBER_OF_THREADS; i++) {
        thread_id[i] = i + 1;
        pthread_create(&thread_array[i - 1], NULL, useResources, (void*) &thread_id[i]);
    }

    for(int i = 0; i < NUMBER_OF_THREADS; i++) {
        pthread_join(thread_array[i], NULL);

    }


    pthread_mutex_destroy(&rm.poolMutex);
    sem_destroy(&rm.availableResources);
    return 0;
}
