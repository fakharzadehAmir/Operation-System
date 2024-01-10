#include <stdio.h>
#include <stdbool.h>

int numberOfResources;
int numberOfCustomers;

void printMatrix(int matrix[numberOfCustomers][numberOfResources], int rows, int cols) {
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            printf("%d ", matrix[i][j]);
        }
        printf("\n");
    }
}

bool isSafe(int available[], int max[][numberOfResources], int allocation[][numberOfResources], int need[][numberOfResources]) {
    int work[numberOfResources];
    for (int i = 0; i < numberOfResources; i++) {
        work[i] = available[i];
    }

    bool finish[numberOfCustomers];
    for (int i = 0; i < numberOfCustomers; i++) {
        finish[i] = false;
    }

    while (1) {
        bool found = false;
        for (int i = 0; i < numberOfCustomers; i++) {
            if (!finish[i]) {
                bool possible = true;
                for (int j = 0; j < numberOfResources; j++) {
                    if (need[i][j] > work[j]) {
                        possible = false;
                        break;
                    }
                }

                if (possible) {
                    for (int j = 0; j < numberOfResources; j++) {
                        work[j] += allocation[i][j];
                    }
                    finish[i] = true;
                    found = true;
                }
            }
        }

        if (!found) {
            for (int i = 0; i < numberOfCustomers; i++) {
                if (!finish[i]) {
                    return false;
                }
            }
            return true;  
        }
    }
}

int main() {
    printf("Enter the number of resources: ");
    scanf("%d", &numberOfResources);
    
    printf("Enter the number of customers: ");
    scanf("%d", &numberOfCustomers);

    int available[numberOfResources];
    printf("Enter the available resources: ");
    for (int i = 0; i < numberOfResources; i++) {
        scanf("%d", &available[i]);
    }

    int maximum[numberOfCustomers][numberOfResources];
    printf("Enter the maximum resource requirement for each customer:\n");
    for (int i = 0; i < numberOfCustomers; i++) {
        for (int j = 0; j < numberOfResources; j++) {
            scanf("%d", &maximum[i][j]);
        }
    }

    int allocation[numberOfCustomers][numberOfResources];
    printf("Enter the allocated resources for each customer:\n");
    for (int i = 0; i < numberOfCustomers; i++) {
        for (int j = 0; j < numberOfResources; j++) {
            scanf("%d", &allocation[i][j]);
        }
    }

    int need[numberOfCustomers][numberOfResources];
    for (int i = 0; i < numberOfCustomers; i++) {
        for (int j = 0; j < numberOfResources; j++) {
            need[i][j] = maximum[i][j] - allocation[i][j];
        }
    }
    printf("need matrix: \n");
    printMatrix(need, numberOfCustomers, numberOfResources);


    printf("The system is currently in a %s state.\n", isSafe(available, maximum, allocation, need) ? "safe" : "not safe");

    return 0;
}
