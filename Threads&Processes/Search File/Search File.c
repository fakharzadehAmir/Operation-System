#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <pthread.h>

#define MAX_THREADS 256
#define MAX_PATH_LEN 256

typedef struct {
   char path[MAX_PATH_LEN];
   char fileName[MAX_PATH_LEN];
} Directory;

int threadCount = 0;
pthread_t threads[MAX_THREADS];
Directory dirs[MAX_THREADS];

pthread_mutex_t lock;

void *ThreadFunc(void *args) {
   Directory *directory = (Directory *)args;
   DIR *dir = opendir(directory->path);
   struct dirent *entry;
   while ((entry = readdir(dir)) != NULL) {
       if (entry->d_type == DT_DIR && strcmp(entry->d_name, ".") != 0 && strcmp(entry->d_name, "..") != 0) {
           pthread_mutex_lock(&lock);
           threadCount++;
           int currentIndex = threadCount;
           pthread_mutex_unlock(&lock);
           snprintf(dirs[currentIndex].path, MAX_PATH_LEN, "%s/%s", directory->path, entry->d_name);
           strcpy(dirs[currentIndex].fileName, directory->fileName);

           if (pthread_create(&threads[currentIndex], NULL, ThreadFunc, &dirs[currentIndex]) != 0) {
               perror("Failed to create thread");
               continue;
           }
       } else if (entry->d_type == DT_REG && strcmp(entry->d_name, directory->fileName) == 0) {
           printf("[+] Found:\n%s/%s\n\n", directory->path, entry->d_name);
       }
   }

   closedir(dir);
}

int main() {
   pthread_mutex_init(&lock, NULL);
   char *root;
   root = malloc(sizeof(char)*256);
   scanf("search %s %s", root, dirs[0].fileName);

   snprintf(dirs[0].path, MAX_PATH_LEN, "../HW2/Q1/%s", root);
   free(root);
   if (pthread_create(&threads[0], NULL, ThreadFunc, &dirs[0]) != 0) {
       perror("Failed to create thread");
       return 1;
   }

   pthread_join(threads[0], NULL);

   for (int i = 1; i < threadCount; i++) {
       pthread_join(threads[i], NULL);
   }

   pthread_mutex_destroy(&lock);
   return 0;
}
