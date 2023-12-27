#include<stdio.h>

struct process
{
    int pid;
    int bt, wt, tt;
};

int main() {
    int size;
    printf("number of processes: ");
    scanf("%d", &size);
    struct process myProcesses[size];

    for(int i = 0; i < size; i++) {
        myProcesses[i].pid = i + 1;
        printf("Enter burst time of process %d: ", i + 1);
        scanf("%d", &myProcesses[i].bt);
    }

    myProcesses[0].wt = 0;
    for(int i = 1; i < size; i++) {
        myProcesses[i].wt = myProcesses[i - 1].wt + myProcesses[i - 1].bt;
    }

    for(int i = 0; i < size; i++) {
        myProcesses[i].tt = myProcesses[i].wt + myProcesses[i].bt;
    }

    int totalWt = 0, totalTt = 0;

    printf("\nProcess\tBurst Time\tWaiting Time\tTurnaround Time\n");
    for(int i = 0; i < size; i++) {
        totalWt += myProcesses[i].wt;
        totalTt += myProcesses[i].tt;
        printf("%d\t\t%d\t\t%d\t\t%d\n", myProcesses[i].pid, myProcesses[i].bt, myProcesses[i].wt, myProcesses[i].tt);
    }

    float avgWt = (float) totalWt / size;
    float avgTt = (float) totalTt / size;

    printf("\nAverage waiting time: %.2f\n", avgWt);
    printf("Average turnaround time: %.2f\n", avgTt);

    return 0;
}
