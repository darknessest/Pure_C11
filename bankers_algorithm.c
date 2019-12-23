#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#define N 100

int main() {
    int curr[N][N];
    int max_claim[N][N];
    int avl[N];
    int alloc[N];
    memset(alloc, 0, N);
    int max_res[N];
    int running[N];

    int i, j, exec, r, p;
    int count = 0;
    bool safe = false;
    srand(time(NULL));

    printf("\nEnter the number of resources: ");
    scanf("%d", &r);

    printf("\nEnter the number of processes: ");
    scanf("%d", &p);
    for (i = 0; i < p; i++) {
        running[i] = 1;
        count++;
    }


    printf("\nGenerating Allocated Resource Table: ");
    for (i = 0; i < p; i++) {
        for (j = 0; j < r; j++) {
//            scanf("%d", &curr[i][j]);
            curr[i][j] = rand()%5;
        }
    }

    printf("\nGenerating Maximum Claim table");
    for (i = 0; i < p; i++) {
        for (j = 0; j < r; j++)
            max_claim[i][j] = curr[i][j] + rand()%5;

    }

    printf("\nGenerating Claim Vector\n");
    for (i = 0; i < p; i++) {
        for (j = 0; j < r; j++)
            max_res[j] += curr[i][j]+ rand()%2;

    }

    printf("\nThe Claim Vector is: ");
    for (i = 0; i < r; i++)
        printf("%d ", max_res[i]);

    printf("\n\nThe Allocated Resource Table:\n");
    for (i = 0; i < p; i++) {
        for (j = 0; j < r; j++)
            printf("\t%d", curr[i][j]);
        printf("\n");
    }

    printf("\nThe Maximum Claim Table:\n");
    for (i = 0; i < p; i++) {
        for (j = 0; j < r; j++)
            printf("\t%d", max_claim[i][j]);
        printf("\n");
    }

    for (i = 0; i < p; i++)
        for (j = 0; j < r; j++)
            alloc[j] += curr[i][j];

    printf("\nAllocated resources: ");
    for (i = 0; i < r; i++)
        printf("%d ", alloc[i]);
    for (i = 0; i < r; i++)
        avl[i] = max_res[i] - alloc[i];

    printf("\nAvailable resources: ");
    for (i = 0; i < r; i++)
        printf("%d ", avl[i]);
    printf("\n");

    while (count != 0) {
        safe = false;
        for (i = 0; i < p; i++) {
            if (running[i]) {
                exec = 1;
                for (j = 0; j < r; j++) {
                    if (max_claim[i][j] - curr[i][j] > avl[j]) {
                        exec = 0;
                        break;
                    }
                }

                if (exec) {
                    printf("\nProcess%d is executing.\n", i + 1);
                    running[i] = 0;
                    count--;
                    safe = true;
                    for (j = 0; j < r; j++)
                        avl[j] += curr[i][j];
                    break;
                }
            }
        }

        if (!safe) {
            printf("\nThe processes are in unsafe state.");
            break;
        }

        if (safe)
            printf("\nThe process is in safe state.");

        printf("\nAvailable vector: ");
        for (i = 0; i < r; i++)
            printf("%d ", avl[i]);
    }

    return 0;
}