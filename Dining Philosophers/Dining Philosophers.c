#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>


pthread_mutex_t chopsticks[5];

void *eating(void *philosopherId) {
    int i = *(int*)philosopherId;
    printf("philosopher %d is thinking!\n", i+1);
    sleep(1);
    if (i % 2 == 0) {
        pthread_mutex_lock(&chopsticks[i]);    
        pthread_mutex_lock(&chopsticks[(i + 1) % 5]);    
    } else {
        pthread_mutex_lock(&chopsticks[(i + 1) % 5]);    
        pthread_mutex_lock(&chopsticks[i]);    
    }
    sleep(1);
    printf("philosopher %d is eating using chopstick[%d] and chopstick[%d]!\n", i+1, i+1, ((i+1)%5)+1);
    sleep(1);
    if (i % 2 == 0) {
        pthread_mutex_unlock(&chopsticks[(i + 1) % 5]);            
        pthread_mutex_unlock(&chopsticks[i]);    
    } else {
        pthread_mutex_unlock(&chopsticks[i]);    
        pthread_mutex_unlock(&chopsticks[(i + 1) % 5]);    
    }
    printf("philosopher %d finished eating!\n", i+1);


    return NULL;
}

int main() {
    pthread_t threads[5];
    int philosopher_numbers[5];

    for(int i = 0; i < 5; i++) {
        pthread_mutex_init(&chopsticks[i], NULL);
        philosopher_numbers[i] = i;
    }

    for(int i = 0; i < 5; i++) {
        pthread_create(&threads[i], NULL, eating, (void *)&philosopher_numbers[i]);
    }

    for(int i = 0; i < 5; i++) {
        pthread_join(threads[i], NULL);
    }

    for(int i = 0; i < 5; i++)
        pthread_mutex_destroy(&chopsticks[i]);
    return 0;
}