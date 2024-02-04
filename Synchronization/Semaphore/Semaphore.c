#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>

pthread_mutex_t m1;
pthread_mutex_t m2;
sem_t sem1;
sem_t sem2;

void* p1(void* arg) {
    char* itemsToPrint = (char *) arg;
    pthread_mutex_lock(&m1);
    sem_wait(&sem2);
    printf("%c\n", *itemsToPrint);
    sem_post(&sem1);
    pthread_mutex_unlock(&m1);
    return NULL;
}

void* p2(void* arg) {
    char* itemsToPrint = (char *) arg;
    pthread_mutex_lock(&m2);
    sem_wait(&sem1);
    printf("%c\n", *itemsToPrint);
    sem_post(&sem2);
    pthread_mutex_unlock(&m2);
    return NULL;
}

int main() {
    pthread_t thread1, thread2;

    pthread_mutex_init(&m1,NULL);
    pthread_mutex_init(&m2,NULL);

    sem_init(&sem1, 0, 1);
    sem_init(&sem2, 0, 0);

    char* str1 = "FEG";
    char* str2 = "ACB";

    for(int i = 0; i < 3; i++) {
        pthread_create(&thread1, NULL, p1, &str1[i]);
        pthread_create(&thread2, NULL, p2, &str2[i]);
    }

    pthread_join(thread1, NULL);
    pthread_join(thread2, NULL);

    pthread_mutex_destroy(&m1);
    pthread_mutex_destroy(&m2);

    sem_destroy(&sem1);
    sem_destroy(&sem2);
    return 0;
}

