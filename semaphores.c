//
// Created by Alexey on 11/12/2019.
//
#include <unistd.h>
#include "linked_list.h"
#include <time.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <signal.h>

union _semun {
  int val;                    /* value for SETVAL */
  struct semid_ds *buf;       /* buffer for IPC_STAT, IPC_SET */
  unsigned short int *array;  /* array for GETALL, SETALL */
  struct seminfo *__buf;      /* buffer for IPC_INFO */
};

int my_semaphore = -1, status;

void start_process();

// **********************************************

void accessSema(increment) {
    struct sembuf sops;

    sops.sem_num = 0;
    sops.sem_op = increment;
    sops.sem_flg = SEM_UNDO;

    if (semop(my_semaphore, &sops, 1) < 0)
        perror("my_semaphore");

    return;
}

// **********************************************

void free_resources() {
    union _semun semarg;

    if (my_semaphore >= 0)
        if (semctl(my_semaphore, 0, IPC_RMID, semarg) < 0) {
            perror("free_resources: my_semaphore");
        }

}

// **********************************************

void writer() {
    printf("Hi, from Writer (%d)", getpid());

    while (1) {
//        accessSema(-1);

        //CRITICAL SECTION BEGIN
        if (isEmpty()) {
            for (int i = 0; i < 10; ++i)
                insertFirst(rand()%10);
        }
        //CRITICAL SECTION END

        accessSema(1); // Note I don't have to just add 1
        // Go collect the child body

    }
}

void reader() {
    printf("Hi, from Reader (%d)\n", getpid());
    while (1) {
        while (!isEmpty()) {
            printList();
            printf("%d ", pop());
            usleep(1000000);     // give sime pause for visualization
        }
        printf("\n");
        accessSema(-1);
        wait(&status);
    }
}

int main(int argc, char *argv[]) {
    union _semun semarg;
    int i, pid;
    srand((time)NULL);

    for (int j = 0; j < 10; ++j) {
        insertFirst(j+rand()%10);
    }
    // ------------------------------

// Get semaphore for display critical region

    my_semaphore = semget(IPC_PRIVATE, 1, 0666);
    if (my_semaphore < 0) {
        free_resources();
        perror("semget(my_semaphore)");
        exit(-1);
    }
    printf("created my_semaphore %d\n", my_semaphore);

// Set a single value to zero --- waiting on this will suspend
// a task until this gets incremented

    semarg.val = 0;
    if (semctl(my_semaphore, 0, SETVAL, semarg) < 0) {
        perror("semctl_1(my_semaphore)");
        free_resources();
        exit(-1);
    }
    // -------------------------------

    pid = fork();
    if (pid < 0) {
        perror("fork err");
        free_resources();
        exit(-1);
    }
    if (pid == 0) {
        // TODO: start writer in child(?)
        writer();
        start_process();
        return 0;
    }
    reader();
    // TODO: start reader in father(?)

    printf("Creating child: %d\n", pid);

    // Let the process go for 5 seconds
    sleep(5);
    printf("parent done waiting, now set child in action\n");

    accessSema(6); // Note I don't have to just add 1
    // Go collect the child body

    pid = wait(&status);
    printf("---- %d ----\n", pid);
    printf("All done waiting for child\n");

    free_resources();

}//End of main

void start_process() {
    printf("Child calling accessSema(-6)\n");

    accessSema(-6); // I can wait for more than 1
    printf("Child returns from accessSema   #%d\n", status);
    sleep(4);
    printf("Child terminating\n");
}