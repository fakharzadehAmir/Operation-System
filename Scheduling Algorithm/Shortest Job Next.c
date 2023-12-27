#include <stdio.h>

struct process {
    int pid;
    int bt, wt, tt;
};

int main() {
    int size;
    printf("number of processes: ");
    scanf("%d", &size);

    struct process proc[size];

    for(int i = 0; i < size; i++) {
        proc[i].pid = i + 1;
        printf("Enter burst time of process %d: ", i + 1);
        scanf("%d", &proc[i].bt);
    }

    for (int i = 0; i < size - 1; i++) {
        int minIndex = i;
        for (int j = i + 1; j < size; j++) {
            if (proc[j].bt < proc[minIndex].bt)
                minIndex = j;
        }

        struct process temp = proc[i];
        proc[i] = proc[minIndex];
        proc[minIndex] = temp;
    }

    int total_wt = 0, total_tt = 0;
    proc[0].wt = 0; 

    for(int i = 1; i < size; i++) {
        proc[i].wt = proc[i - 1].wt + proc[i - 1].bt;
        total_wt += proc[i].wt;
    }

    for(int i = 0; i < size; i++) {
        proc[i].tt = proc[i].wt + proc[i].bt;
        total_tt += proc[i].tt;
    }

    float avg_wt = (float)total_wt / size;
    float avg_tt = (float)total_tt / size;

    printf("\nProcess\tBurst Time\tWaiting Time\tTurnaround Time\n");
    for(int i = 0; i < size; i++) {
        printf("%d\t\t%d\t\t%d\t\t%d\n", proc[i].pid, proc[i].bt, proc[i].wt, proc[i].tt);
    }

    printf("\nAverage Waiting Time: %.2f\n", avg_wt);
    printf("Average Turnaround Time: %.2f\n", avg_tt);

    return 0;
}
