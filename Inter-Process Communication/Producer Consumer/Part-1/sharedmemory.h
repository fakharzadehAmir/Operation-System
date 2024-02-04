#pragma once
#define SHARED_MEM_NAME "my_shared_memory"
#define SEM_EMPTY_NAME "empty"
#define SEM_FULL_NAME "full"
#define STACK_SIZE 10
#define STACK_CAPACITY 10
#define CONSUMER_COUNT 5
#define PRODUCER_COUNT 2
#define SHARED_MEM_SIZE 1024


typedef struct {
    int data[STACK_CAPACITY];
    int stack_pointer;
} SharedStackMemory;