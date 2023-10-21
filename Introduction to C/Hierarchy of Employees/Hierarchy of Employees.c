#include <stdio.h>
#include "string.h"
typedef struct{
    char name[(int) 1e2];
} Person;

Person employee[(int) 1e5];

void Add(char newName[], int position, int size) {
    for(int i = size; i > position; i--) {
        strcpy(employee[i].name, employee[i - 1].name);
    }
    strcpy(employee[position].name, newName);
}

void Swap(char* Person1, char* Person2, int size){
    int index1, index2;
    for(int idx = 0; idx < size; idx++) {
        if (strcmp(Person1, employee[idx].name) == 0)
            index1 = idx;
        else if (strcmp(Person2, employee[idx].name) == 0)
            index2 = idx;
    }
    Person tmp;
    tmp = employee[index1];
    employee[index1] = employee[index2];
    employee[index2] = tmp;
}

void Delete(char* Person, int size) {
    int index;
    for(int idx = 0; idx < size; idx++) {
        if (strcmp(Person, employee[idx].name) == 0) {
            index = idx;
            break;
        }
    }
    for (int i = index + 1; i < size; i++) {
        strcpy(employee[i-1].name, employee[i].name);

    }
}
int main() {
    int size, numberInstruction;
    scanf("%d", &size);
    for(int idx = 0 ; idx < size; idx++) {
        scanf("%s",employee[idx].name);
    }
    scanf("%d", &numberInstruction);
    for(int i = 0; i < numberInstruction; i++) {
        char instruction[5];
        scanf("%s", instruction);
        if (strcmp(instruction, "add") == 0) {
            char newName[100];
            int newPosition;
            scanf("%s\n%d", newName, &newPosition);
            Add(newName, newPosition, size);
            size++;
        }
        else if (strcmp(instruction, "swap") == 0) {
            char Person1[100], Person2[100];
            scanf("%s\n%s", Person1, Person2);

            Swap(Person1, Person2, size);
        }
        else if (strcmp(instruction, "delete") == 0)
        {
            char Person[100];
            scanf("%s", Person);
            Delete(Person, size);
            size--;
        }

    }

    for(int i = 0; i < size; i++) {
        printf("%s ", employee[i].name);
    }


    return 0;
}
