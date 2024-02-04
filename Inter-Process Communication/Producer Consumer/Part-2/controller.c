#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/shm.h>
#include <sys/mman.h>
#include <sys/wait.h>
#include <signal.h>
#include "sharedmemory.h"
#include <fcntl.h>
#include <sys/stat.h>
#include <semaphore.h>
#include <time.h>

#define MAX_CONSUMERS 10
#define CONSUMER_NAME_SIZE 20
#define MIN_WANTED_STACK_SIZE 20
#define MAX_WANTED_STACK_SIZE 30

int main() {
    int shared_memory_file, process_id, num_consumers;
    SharedStackMemory *shared_stack_mem;
    sem_t *empty_semaphore, *full_semaphore;
    pid_t consumers[MAX_CONSUMERS];
    char consumer_name[CONSUMER_NAME_SIZE];
    
    shared_memory_file = shm_open(SHARED_MEM_NAME, O_RDWR, 0666);
    if (shared_memory_file < 0) {
        perror("Opening shared memory: Fault");
        return EXIT_FAILURE;
    }

    shared_stack_mem = (SharedStackMemory *)mmap(0, sizeof(SharedStackMemory), PROT_READ, MAP_SHARED, shared_memory_file, 0);
    if (shared_stack_mem == MAP_FAILED) {
        perror("Mapping shared memory: Fault");
        return EXIT_FAILURE;
    }

    empty_semaphore = sem_open(SEM_EMPTY_NAME, O_CREAT, 0666, STACK_CAPACITY);
    full_semaphore = sem_open(SEM_FULL_NAME, O_CREAT, 0666, 0);
    if (empty_semaphore == SEM_FAILED || full_semaphore == SEM_FAILED) {
        perror("Opening semaphores: Fault");
        return EXIT_FAILURE;
    }

    for (;;) {
        int stack_size = shared_stack_mem->stack_pointer + 1;
        
        if (stack_size > MAX_WANTED_STACK_SIZE && num_consumers < MAX_CONSUMERS || num_consumers==0) {
            process_id = fork();
            if (process_id == 0) {
                snprintf(consumer_name, CONSUMER_NAME_SIZE, "consumer%d", num_consumers + 1);
                execl("./consumer", "consumer", NULL);
                exit(EXIT_FAILURE);
            } else if (process_id > 0) {
                consumers[num_consumers++] = process_id;
            } else {
                perror("Failed to fork");
            }
        }

        if (stack_size < MIN_WANTED_STACK_SIZE && num_consumers > 1) {
            process_id = consumers[--num_consumers];
            kill(process_id, SIGTERM);
            printf("Consumer removed, PID: %d\n", process_id);
            
        }
        printf("Current stack size: %d, Number of consumers: %d\n", stack_size, num_consumers);
        sleep(5);
    }
    
    return 0;
}