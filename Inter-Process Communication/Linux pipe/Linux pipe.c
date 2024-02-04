#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <string.h>

#define MAX_ARGS 10
#define MAX_INPUT_SIZE 100

void runCommand(char **arguments, int inputFd, int outputFd) {
    pid_t processId = fork();
    if (processId == 0) {
        if (inputFd != 0) {
            dup2(inputFd, 0);
            close(inputFd);
        }

        if (outputFd != 1) {
            dup2(outputFd, 1);
            close(outputFd);
        }

        fprintf(stderr, "Output: ");
        for (int i = 0; arguments[i] != NULL; i++) {
            fprintf(stderr, "%s ", arguments[i]);
        }
        fprintf(stderr, "\n");

        execvp(arguments[0], arguments);
        perror("execvp");
        exit(EXIT_FAILURE);
    } else if (processId < 0) {
        perror("fork");
        exit(EXIT_FAILURE);
    }

    waitpid(processId, NULL, 0);
}

void runPipeline(char ***commands, int numCommands) {
    int index, inputFd, pipeDescriptors[numCommands - 1][2];

    for (index = 0; index < numCommands - 1; index++) {
        if (pipe(pipeDescriptors[index]) == -1) {
            perror("pipe");
            exit(EXIT_FAILURE);
        }
    }

    for (index = 0; index < numCommands; index++) {
        int outputFd = (index == numCommands - 1) ? 1 : pipeDescriptors[index][1];

        runCommand(commands[index], inputFd, outputFd);

        if (index > 0) {
            close(pipeDescriptors[index - 1][1]);
        }

        if (index < numCommands - 1) {
            inputFd = pipeDescriptors[index][0];
        }
    }

    for (index = 0; index < numCommands - 1; index++) {
        close(pipeDescriptors[index][0]);
    }
}

int main() {
    char userInput[MAX_INPUT_SIZE];
    printf("Input: ");
    fgets(userInput, sizeof(userInput), stdin);

    userInput[strcspn(userInput, "\n")] = '\0';

    char *token, *subtoken, *saveptr1, *saveptr2, ***commands; 
    int numCommands;

    while (1) {
        token = strtok_r(userInput, "|", &saveptr1);
        if (token == NULL)
            break;
        numCommands++;
    }

    memcpy(userInput, saveptr1, MAX_INPUT_SIZE);

    commands = (char ***)malloc(numCommands * sizeof(char **));

    int index = 0;
    while (1) {
        token = strtok_r(userInput, "|", &saveptr1);
        if (token == NULL) 
            break;
        int argCount = 0;
        commands[index] = (char **)malloc(MAX_ARGS * sizeof(char *));

        while (1) {
            subtoken = strtok_r(token, " ", &saveptr2);
            if (subtoken == NULL) 
                break;
            commands[index][argCount++] = subtoken;
        }
        commands[index][argCount] = NULL; 
        index++;
    }
    runPipeline(commands, numCommands);
    for (int j = 0; j < numCommands; j++) 
        free(commands[j]);
    free(commands);

    return 0;
}
