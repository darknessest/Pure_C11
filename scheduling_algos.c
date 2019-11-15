#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

typedef struct process {
  int arrival_t;
  int burst_t;
  int remaining_t;
  int waiting_t;
} process;

int cmp_by_burst(const void *a, const void *b) {
    process *ia = (process *) a;
    process *ib = (process *) b;
    return (ia->burst_t - ib->burst_t);
}
int cmp_by_wt(const void *a, const void *b) {
    process *ia = (process *) a;
    process *ib = (process *) b;
    return (ia->waiting_t - ib->waiting_t);
}

void gen_data(const int size, process list[]) {
    srand(time(NULL));

    printf("Generated processes:\n"
           "No. | Arrival | Burst\n");
    for (int i = 0; i < size; ++i) {
        list[i].arrival_t = i + 1;
        list[i].burst_t = 2 + rand()%(size);
        list[i].remaining_t = list[i].burst_t;
        list[i].waiting_t = 0;
        printf("%d\t\t%d\t\t%d\n", i + 1, list[i].arrival_t, list[i].burst_t);
    }
}

void RR(const int size, const int time_quantum, process arr[]) {
    int count, time, remain = size, flag = 0;
    int sumwt = 0, sumtat = 0;

    process *list;
    list = (process *) malloc(sizeof(process)*(size + 1));
    memcpy(list, arr, size*sizeof(process));

    printf("\n\n*** RoundRobin ***\nProcess | Turnaround Time | Waiting Time\n");

    for (time = 0, count = 0; remain != 0;) {
        if (list[count].remaining_t <= time_quantum && list[count].remaining_t > 0) {
            time += list[count].remaining_t;
            list[count].remaining_t = 0;
            flag = 1;
        } else if (list[count].remaining_t > 0) {
            list[count].remaining_t -= time_quantum;
            time += time_quantum;
        }
        if (list[count].remaining_t == 0 && flag == 1) {
            remain--;
            printf("P[%d]\t\t\t%d\t\t\t\t%d\n",
                   count + 1,
                   time - list[count].arrival_t,
                   time - list[count].arrival_t - list[count].burst_t);
            sumwt += time - list[count].arrival_t - list[count].burst_t;
            sumtat += time - list[count].arrival_t;
            flag = 0;
        }
        if (count == size - 1)
            count = 0;
        else if (list[count + 1].arrival_t <= time)
            count++;
        else
            count = 0;
    }

    printf("\nAverage Waiting Time = %.4f\n", (float) sumwt/(float) size);
    printf("Avg Turnaround Time  = %.4f\n\n", (float) sumtat/(float) size);
}

void FCFS(const int size, process arr[]) {
    process *list;
    list = (process *) malloc(sizeof(process)*(size + 1));
    memcpy(list, arr, size*sizeof(process));

    int tat[size], sumwt = 0, sumtat = 0;

    list[0].waiting_t = 0;    //waiting time for first process is 0

    //calculating waiting time
    for (int i = 1; i < size; i++) {
        list[i].waiting_t = 0;
        for (int j = 0; j < i; j++)
            list[i].waiting_t += list[j].burst_t;
    }
    printf("\n\n*** FCFS ***\nProcess | Turnaround Time | Waiting Time\n");
//    printf("\nProcess\t\tWaiting Time\tTurnaround Time\n");

    //calculating turnaround time
    for (int i = 0; i < size; i++) {
        tat[i] = list[i].burst_t + list[i].waiting_t;
        sumwt += list[i].waiting_t;
        sumtat += tat[i];
        printf("P[%d]\t\t\t%d\t\t\t\t%d\n", i + 1, tat[i], list[i].waiting_t);
    }

    printf("\nAverage Waiting Time = %.4f\n", (float) sumwt/(float) size);
    printf("Avg Turnaround Time  = %.4f\n\n", (float) sumtat/(float) size);
}

void SRT(const int size, process arr[]) {
    int endTime = 0, smallest = size, sumwt = 0, sumtat = 0;

    process *list;
    list = (process *) malloc(sizeof(process)*(size + 1));
    memcpy(list, arr, size*sizeof(process));

    for (int i = 0; i < size; ++i)
        endTime += list[i].burst_t;

    list[smallest].remaining_t = 9999;

    for (int time = 0; time < endTime; ++time) {
        // if process finished, switch to infinite process
        if (list[smallest].remaining_t == 0)
            smallest = size;
        // find min remain time(>0) in range of arrival time
        for (int j = 0; j < time; ++j) {
            // finding min in a range of visible valid processes
            if (list[j].remaining_t > 0 && list[j].remaining_t < list[smallest].remaining_t)
                smallest = j;
        }
        // decreasing time in a exectuing process (smallest)
        --list[smallest].remaining_t;

        // adding wating time to visible processes in a queue
        for (int k = 0; k < time; ++k) {
            if (k != smallest && list[k].remaining_t > 0)
                ++list[k].waiting_t;
        }
    }

    qsort(list, size, sizeof(process), cmp_by_wt);

    printf("\n\n*** SRT ***\nProcess | Turnaround Time | Waiting Time\n");
    for (int l = 0; l < size; ++l) {
        printf("P[%d]\t\t\t%d\t\t\t\t%d\n", list[l].arrival_t, list[l].waiting_t + list[l].burst_t, list[l].waiting_t);
        sumwt += list[l].waiting_t;
        sumtat += list[l].waiting_t + list[l].burst_t;
    }

    printf("\nAverage Waiting Time = %.4f\n", sumwt*1.0/size);
    printf("Avg Turnaround Time  = %.4f\n\n", sumtat*1.0/size);
}

// Short Proccess Next
void SPN(const int size, process arr[]) {
    int total = 0;
    float avg_wt;

    process *list;
    list = (process *) malloc(sizeof(process)*(size + 1));
    memcpy(list, arr, size*sizeof(process));

    qsort(list, size, sizeof(process), cmp_by_burst);
    // waiting time for first process will be zero
    list[0].waiting_t = 0;

    // calculate waiting time
    for (int i = 1; i < size; i++) {
        list[i].waiting_t = 0;
        for (int j = 0; j < i; j++)
            list[i].waiting_t += list[j].burst_t;

        total += list[i].waiting_t;
    }

    // average waiting time
    avg_wt = (float) total/(float) size;
    total = 0;

    printf("\n\n*** SPN ***\nProcess | Turnaround Time | Waiting Time\n");
    for (int i = 0; i < size; i++) {
        // calculate turnaround time
//        tat[i] = list[i].burst_t + list[i].waiting_t;
        total += list[i].burst_t + list[i].waiting_t;
        printf("P[%d]\t\t\t%d\t\t\t\t%d\n",
               list[i].arrival_t,
               list[i].burst_t + list[i].waiting_t,
               list[i].waiting_t);
    }
    printf("\nAverage Waiting Time = %.4f\n", avg_wt);
    printf("Avg Turnaround Time  = %.4f\n\n", total*1.0/size);
}

void HRRN(const int size, const process arr[]) {
    // assuming that list of processes is sorted by arrival time

    process *list;
    list = (process *) malloc(sizeof(process)*(size + 1));
    memcpy(list, arr, size*sizeof(process));

    int endTime = 0;
    int sumwt = 0, sumtat = 0;
    int i;
    // Variable for Completion status
    // Pending: rt > 0
    // Completed: rt = 0

    for (i = 0; i < size; i++)
        endTime += list[i].burst_t;

    printf("\n\n*** HRRN ***\nProcess | Turnaround Time | Waiting Time | Normalized TaT\n");
    for (int time = list[0].arrival_t; time < endTime;) {
        float hrr = -9999;          // Set lower limit to response ratio
        float temp;                 // Response Ratio Variable
        int loc = 0;                // Variable to store next processs selected

        for (i = 0; i < size; i++) {

            // checking if process has arrived and is incomplete
            if (list[i].arrival_t <= time && list[i].remaining_t != 0) {

                // calculating Response Ratio
                temp = (list[i].burst_t + (time - list[i].arrival_t))/list[i].burst_t;

                // checking for Highest Response Ratio
                if (hrr < temp) {
                    // Storing Response Ratio
                    hrr = temp;
                    // Storing Location
                    loc = i;
                }
            }
        }

        // Updating time value
        time += list[loc].burst_t;

        // Calculation of waiting time
        list[loc].waiting_t = time - list[loc].arrival_t - list[loc].burst_t;


        // Sum Turn Around Time for average
        sumtat += time - list[loc].arrival_t;
        // turnaround time: time - list[loc].arrival_t
        // Normalized Turn Around Time : ((time - list[loc].arrival_t)/list[loc].burst_t);

        // Updating Completion Status
        list[loc].remaining_t = 0;

        // Sum Waiting Time for average
        sumwt += list[loc].waiting_t;
        printf("P[%d]\t\t\t%d\t\t\t\t%d\t\t\t%.4f\n",
               list[loc].arrival_t,
               time - list[loc].arrival_t,
               list[loc].waiting_t,
               ((time - list[loc].arrival_t)*1.0/list[loc].burst_t));
    }

    printf("\nAverage Waiting Time = %.4f\n", sumwt*1.0/size);
    printf("Avg Turnaround Time  = %.4f\n\n", sumtat*1.0/size);
}

int main() {
    int n, time_quantum = 1, choice;
    process *list;

    printf("Enter Number of Processes: ");
    scanf("%d", &n);

    list = (process *) malloc(sizeof(process)*(n + 1));
    gen_data(n, list);

    printf("Data has been generated\n\n");
    while (1) {
        printf(
            "1. Round Robin\n"
            "2. FCFS\n"
            "3. SRT\n"
            "4. SPN\n"
            "5. HRRN\n"
            "0. Exit\n"
            "Select: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1:
                RR(n, time_quantum, list);
                break;
            case 2:
                FCFS(n, list);
                break;
            case 3:
                SRT(n, list);
                break;
            case 4:
                SPN(n, list);
                break;
            case 5:
                HRRN(n, list);
                break;
            case 0:
                exit(0);
            default:
                break;
        }
    }
}
