#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <semaphore.h>
#include <fcntl.h>
#include <sys/shm.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include "sharedmemory.h"

int main() {
    int shared_memory_file;
    SharedStackMemory *shared_stack_mem;
    sem_t *empty_semaphore, *full_semaphore;
    printf("Producer started:\n");

    empty_semaphore = sem_open(SEM_EMPTY_NAME, O_CREAT, 0666, STACK_CAPACITY);
    full_semaphore = sem_open(SEM_FULL_NAME, O_CREAT, 0666, 0);
    srand(time(NULL));

    shared_memory_file = shm_open(SHARED_MEM_NAME, O_CREAT | O_RDWR, 0666);
    if (shared_memory_file < 0) {
        perror("Opening shared memory: Fault");
        return EXIT_FAILURE;
    }

    ftruncate(shared_memory_file, sizeof(SharedStackMemory));
    shared_stack_mem = (SharedStackMemory *)mmap(0, sizeof(SharedStackMemory), PROT_WRITE, MAP_SHARED, shared_memory_file, 0);

    if (shared_stack_mem == MAP_FAILED) {
        perror("Mapping shared memory: Fault");
        return EXIT_FAILURE;
    }

    shared_stack_mem->stack_pointer = -1;
    int num;
    for(int i = 0; i < STACK_SIZE; i++) {
        sem_wait(empty_semaphore);
        num = rand() % 20 + 1;

        if (shared_stack_mem->stack_pointer < STACK_CAPACITY - 1) {
            shared_stack_mem->stack_pointer++;
            shared_stack_mem->data[shared_stack_mem->stack_pointer] = num;
            printf("Pushed ====> %d\n", num);
        }

        sem_post(full_semaphore);
        sleep(1);
    }
    sleep(30);

    munmap(shared_stack_mem, sizeof(SharedStackMemory));
    close(shared_memory_file);
    printf("Producer finished.\n");

    return EXIT_SUCCESS;
}