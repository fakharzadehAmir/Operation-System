#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <math.h>
#include <pthread.h>
#include <string.h>

int main()
{
    int pipes[3][2];
    pthread_mutex_t mtx;
    FILE *filePtr;
    int in, out, count, numElements, numSetCount;
    srand(time(NULL));

    pipe(pipes[0]);
    pipe(pipes[1]);
    pipe(pipes[2]);
    pthread_mutex_init(&mtx, NULL);

    filePtr = fopen("input.txt", "r");
    if (filePtr == NULL)
    {
        perror("Error opening");
        exit(EXIT_FAILURE);
    }

    char *line = NULL;
    size_t len = 0;
    ssize_t readBytes;

    if ((readBytes = getline(&line, &len, filePtr)) == -1)
    {
        perror("Error reading");
        exit(EXIT_FAILURE);
    }

    numElements = atoi(line);

    int processID1 = fork();
    if (processID1 > 0)
    {
        int temp;
        for (int i = 0; i < numElements; i++)
        {
            int num;
            readBytes = getline(&line, &len, filePtr);
            num = atoi(line);
            printf("input = %d\n", num);
            write(pipes[1][1], &num, sizeof(int));
            write(pipes[2][1], &num, sizeof(int));  
        }

        fclose(filePtr);

        float minResult, avgResult;
        read(pipes[0][0], &minResult, sizeof(float));
        read(pipes[0][0], &avgResult, sizeof(float));

        printf("Mean = %.2f, Minimum = %.2f\n", avgResult, minResult);

        exit(0);
    }

    int processID2 = fork();
    if (processID2 > 0)
    {
        int tmp, i;
        float min = 1e8;
        while (i < numElements)
        {
            read(pipes[1][0], &tmp, sizeof(int));
            printf("first_read =======> %d\n", tmp);
            if (min > tmp)
                min = tmp;
            i++;
        }
        write(pipes[0][1], &min, sizeof(int));
        exit(0);
    }

    int processID3 = fork();
    if (processID3 > 0)
    {
        int sum, tmp, i;

        while (i < numElements)
        {
            read(pipes[2][0], &tmp, sizeof(int));
            sum += tmp;
            printf("second_read =======> %d\n", tmp);
            i++;
        }
        float avg = (float)sum / numElements;

        write(pipes[0][1], &avg, sizeof(float));

        exit(0);
    }

    wait(NULL);

    pthread_mutex_destroy(&mtx);

    return 0;
}
