#include <stdio.h>

typedef  struct {
    int age;
    int score;
    char name[50];
} Person;

void findMin(Person person[], int size) {
    int index = 0, minScore = person[0].score;
    for(int i = 1; i < size; i++) {
        if (person[i].score < minScore) {
            minScore = person[i].score;
            index = i;
        }
    }
    for(int idx = index; idx < size - 1; idx++) {
        person[idx] = person[idx + 1];
    }
}

float average(Person person[], int size, int idx) {
    return (person[idx].score + person[size - 1 - idx].score)/2;
}

void sortCouples(Person person[], int size, int couples[]) {
    for(int i = 0; i < size/2 - 1; i++) {
        for(int j = i + 1; j < size/2; j++) {
            int i1 = couples[i], i2 = couples[j];
            if (average(person, size, i1) < average(person, size, i2)) {
                couples[i] = i2;
                couples[j] = i1;
            }
        }
    }
}


int main() {
    int size;
    scanf("%d",&size);
    Person persons[size];
    for(int i = 0; i < size; i++) {
        scanf("%s %d %d", persons[i].name, &persons[i].age, &persons[i].score);
    }
    if (size % 2 == 1) {
        findMin(persons, size);
        size--;
    }
    int couples[(size/2)];
    for(int i = 0; i < size/2; i++) {
        couples[i] = i;
    }
    sortCouples(persons, size, couples);
    for(int i = 0; i < size/2; i++) {
        printf("%s %d %d, %s %d %d\n", persons[couples[i]].name, persons[couples[i]].age,
               persons[couples[i]].score, persons[size - 1 - couples[i]].name, persons[size - 1 - couples[i]].age,
               persons[size - 1 - couples[i]].score);
    }

    return 0;
}
