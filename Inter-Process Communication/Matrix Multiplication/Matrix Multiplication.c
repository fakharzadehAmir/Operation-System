#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <unistd.h>
#include <sys/wait.h>
#include <time.h>

#define SHARED_MEM_SIZE 1024
#define SHARED_MEM_NAME "my_shared_memory"

int main() {
    int rows = 10, cols1 = 9, cols2 = 8;
    int shared_mem_fd, matrix1[rows][cols1], matrix2[cols1][cols2], (*result_matrix)[cols2];
    pid_t child_processes[rows];

    shared_mem_fd = shm_open(SHARED_MEM_NAME, O_CREAT | O_RDWR, 0666);
    if (shared_mem_fd < 0) {
        perror("Failed to open");
        return EXIT_FAILURE;
    }
    printf("Shared memory opened\n");

    ftruncate(shared_mem_fd, SHARED_MEM_SIZE);
    printf("Shared memory size set to %d bytes\n", SHARED_MEM_SIZE);
    srand(time(NULL)); 

    printf("Matrix1:\n");
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols1; j++) {
            matrix1[i][j] = rand() % 10 + 1;
            printf("[%d]\t", matrix1[i][j]);
        }
        printf("\n");
    }

    printf("\nMatrix2:\n");
    for (int i = 0; i < cols1; i++) {
        for (int j = 0; j < cols2; j++) {
            matrix2[i][j] = rand() % 10 + 2;
            printf("[%d]\t", matrix2[i][j]);
        }
        printf("\n");
    }

    result_matrix = (int (*)[cols2])mmap(0, SHARED_MEM_SIZE, PROT_WRITE, MAP_SHARED, shared_mem_fd, 0);
    printf("Shared memory mapped\n");

    printf("matrix multiplication started\n");
    int row = 0;
    while (row < rows) {
        child_processes[row] = fork();
        if (child_processes[row] == 0) { 
            printf("Child with pid %d ->\n", row);
            int col = 0;
            while (col < cols2) {
                int sum = 0;
                for (int p = 0; p < cols1; p++) {
                    sum += matrix1[row][p] * matrix2[p][col];
                }
                result_matrix[row][col] = sum;
                printf("%d:\t[%d][%d] = %d\n", row, row, col, sum);
                col++;
            }
            exit(0);
        }
        row++;
    }

    int num_processes = rows;
    while (num_processes > 0) {
        wait(NULL);
        num_processes--;
    }
    printf("Matrix multiplication completed\n");
    printf("Result matrix is:\n");
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols2; j++) {
            printf("%d ", result_matrix[i][j]);
        }
        printf("\n");
    }

    munmap(result_matrix, SHARED_MEM_SIZE);
    close(shared_mem_fd);
    shm_unlink(SHARED_MEM_NAME);
    printf("Resources cleaned up\n");

    return EXIT_SUCCESS;
}
