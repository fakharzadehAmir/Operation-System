#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/shm.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <unistd.h>
#include <semaphore.h>
#include "sharedmemory.h"

int main() {
    int shared_memory_file;
    SharedStackMemory *shared_stack_mem;
    sem_t *empty_semaphore, *full_semaphore;
    printf("Consumer started:\n");

    empty_semaphore = sem_open(SEM_EMPTY_NAME, O_RDWR, 0666, 0);
    full_semaphore = sem_open(SEM_FULL_NAME, O_RDWR, 0666, 0);

    if (empty_semaphore == SEM_FAILED || full_semaphore == SEM_FAILED) {
        perror("Opening semaphores: Fault");
        return EXIT_FAILURE;
    }

    shared_memory_file = shm_open(SHARED_MEM_NAME, O_RDWR, 0666);
    if (shared_memory_file < 0) {
        perror("Opening shared memory: Fault");
        return EXIT_FAILURE;
    }

    shared_stack_mem = (SharedStackMemory *)mmap(0, sizeof(SharedStackMemory), PROT_WRITE | PROT_READ, MAP_SHARED, shared_memory_file, 0);

    if (shared_stack_mem == MAP_FAILED) {
        perror("Mapping shared memory: Fault");
        return EXIT_FAILURE;
    }
    
    int num;
    for(int i = 0; i < STACK_SIZE; i++) {
        sem_wait(full_semaphore);

        if (shared_stack_mem->stack_pointer >= 0) {
            int consumedValue = shared_stack_mem->data[shared_stack_mem->stack_pointer];
            shared_stack_mem->stack_pointer--;
            printf("Consumed ====> %d\n", consumedValue);
        }

        sem_post(empty_semaphore);
        sleep(3);
    }

    munmap(shared_stack_mem, sizeof(SharedStackMemory));
    close(shared_memory_file);
    sem_close(empty_semaphore);
    sem_close(full_semaphore);
    printf("Consumer finished.\n");

    return EXIT_SUCCESS;
}
