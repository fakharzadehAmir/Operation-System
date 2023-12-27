#include <stdio.h>

struct process {
    int pid;
    int bt, wt, tt, rt;
};

int main() {
    int size, quantum;
    printf("number of processes: ");
    scanf("%d", &size);

    struct process proc[size];

    for(int i = 0; i < size; i++) {
        proc[i].pid = i + 1;
        printf("Enter burst time of process %d: ", i + 1);
        scanf("%d", &proc[i].bt);
        proc[i].rt = proc[i].bt;
    }

    printf("Enter the time quantum: ");
    scanf("%d", &quantum);

    int totalWt = 0, totalTt = 0, currentTime = 0;
    int completed = 0;

    while (completed != size) {
        for (int i = 0; i < size; i++) {
            if (proc[i].rt > 0) {
                if (proc[i].rt <= quantum) {
                    currentTime += proc[i].rt;
                    proc[i].wt = currentTime - proc[i].bt;
                    proc[i].tt = currentTime;
                    proc[i].rt = 0;
                    completed++;
                } else {
                    currentTime += quantum;
                    proc[i].rt -= quantum;
                }
            }
        }
    }

    for (int i = 0; i < size; i++) {
        totalWt += proc[i].wt;
        totalTt += proc[i].tt;
    }

    float avgWt = (float) totalWt / size;
    float avgTt = (float) totalTt / size;

    printf("\nProcess\tBurst Time\tWaiting Time\tTurnaround Time\n");
    for (int i = 0; i < size; i++) {
        printf("%d\t\t%d\t\t%d\t\t%d\n", proc[i].pid, proc[i].bt, proc[i].wt, proc[i].tt);
    }

    printf("\nAverage Waiting Time: %.2f\n", avgWt);
    printf("Average Turnaround Time: %.2f\n", avgTt);

    return 0;
}
