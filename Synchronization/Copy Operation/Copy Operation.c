#include <stdio.h>
#include <unistd.h>
#include <limits.h>
#include <pthread.h>
#include <semaphore.h>
#include <string.h>

char buffer[256];
int finish = 0;
pthread_mutex_t mutex, finish_mutex;
pthread_cond_t cond;

void* publisher(void* args){
    char* readerFile = (char *) args;
    FILE* file = fopen(readerFile, "rb");
    if (file == NULL) {
        perror("Error opening reader file");
        return NULL;
    }

    char new_text[256];
    while (fgets(new_text, sizeof(new_text), file)) {
        pthread_mutex_lock(&mutex);
        while (strlen(buffer) != 0) {
            pthread_cond_wait(&cond, &mutex);
        }
        strcpy(buffer, new_text);
        pthread_cond_signal(&cond);
        pthread_mutex_unlock(&mutex);
    }
    pthread_mutex_lock(&finish_mutex);
    finish = 1;
    pthread_cond_signal(&cond);
    pthread_mutex_unlock(&finish_mutex);
    fclose(file);
    return NULL;
}

void* consumer(void* args) {
    char* writerFile = (char *) args;
    FILE* file = fopen(writerFile, "wb");
    if (file == NULL) {
        perror("Error opening writer file");
        return NULL;
    }

    while (1) {
        pthread_mutex_lock(&mutex);
        while (strlen(buffer) == 0) {
            pthread_mutex_lock(&finish_mutex);
            if (finish) {
                pthread_mutex_unlock(&finish_mutex);
                break;
            }
            pthread_mutex_unlock(&finish_mutex);
            pthread_cond_wait(&cond, &mutex);
        }
        if (finish && strlen(buffer) == 0) {
            pthread_mutex_unlock(&mutex);
            break;
        }
        fprintf(file, "%s", buffer);
        buffer[0] = '\0';
        pthread_cond_signal(&cond);
        pthread_mutex_unlock(&mutex);
    }
    fclose(file);
    return NULL;
}

int main() {
    char cwd[PATH_MAX];
    getcwd(cwd, sizeof(cwd));
    char readerFile[256];
    char writerFile[256];
    char readerPath[PATH_MAX * 2];
    char writerPath[PATH_MAX * 2];
    printf("Enter reader file: ");
    scanf("%s", readerFile);
    printf("Enter writer file: ");
    scanf("%s", writerFile);

    snprintf(readerPath, PATH_MAX * 2, "%s/%s", cwd, readerFile);
    snprintf(writerPath, PATH_MAX * 2, "%s/%s", cwd, writerFile);
    pthread_t pub_thread, con_thread;

    pthread_mutex_init(&mutex, NULL);
    pthread_mutex_init(&finish_mutex, NULL);
    pthread_cond_init(&cond, NULL);

    pthread_create(&pub_thread, NULL, publisher, (void*)readerPath);
    pthread_create(&con_thread, NULL, consumer, (void*)writerPath);

    pthread_join(pub_thread, NULL);
    pthread_join(con_thread, NULL);

    pthread_mutex_destroy(&mutex);
    pthread_mutex_destroy(&finish_mutex);
    pthread_cond_destroy(&cond);
    return 0;
}

