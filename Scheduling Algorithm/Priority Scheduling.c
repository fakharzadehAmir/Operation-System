#include <stdio.h>

struct process {
    int pid;
    int bt, wt, tt, priority;
};

int main() {
    int size;
    printf("number of processes: ");
    scanf("%d", &size);

    struct process proc[size];

    for(int i = 0; i < size; i++) {
        proc[i].pid = i + 1;
        printf("Enter burst time and priority of process %d (separated by a space): ", i + 1);
        scanf("%d %d", &proc[i].bt, &proc[i].priority);
    }

    for(int i = 0; i < size - 1; i++) {
        int highestPriority = i;
        for(int j = i + 1; j < size; j++) {
            if(proc[j].priority < proc[highestPriority].priority)
                highestPriority = j;
        }

        struct process temp = proc[i];
        proc[i] = proc[highestPriority];
        proc[highestPriority] = temp;
    }

    int totalTime = 0;
    proc[0].wt = 0;
    for(int i = 1; i < size; i++) {
        proc[i].wt = proc[i - 1].wt + proc[i - 1].bt;
    }

    for(int i = 0; i < size; i++) {
        proc[i].tt = proc[i].wt + proc[i].bt;
        totalTime += proc[i].tt;
    }

    float avgWt = 0, avgTt = 0;
    for(int i = 0; i < size; i++) {
        avgWt += proc[i].wt;
        avgTt += proc[i].tt;
    }
    avgWt /= size;
    avgTt /= size;

    printf("\nProcess\tPriority\tBurst Time\tWaiting Time\tTurnaround Time\n");
    for(int i = 0; i < size; i++) {
        printf("%d\t\t%d\t\t%d\t\t%d\t\t%d\n", proc[i].pid, proc[i].priority, proc[i].bt, proc[i].wt, proc[i].tt);
    }

    printf("\nAverage Waiting Time: %.2f\n", avgWt);
    printf("Average Turnaround Time: %.2f\n", avgTt);

    return 0;
}
