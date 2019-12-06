//
// Created by Alexey on 27/11/2019.
//
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "fa_linked_list.h"

struct hierarchy {
  bool isIndex;
  int *indexed;
  char file_id[3];
  float size_left;
};

void init(struct hierarchy *region, file **files_list) {
    for (int i = 0; i < 500; ++i) {
        region[i].size_left = BLOCKSIZE;
        region[i].file_id[0] = -1;
        region[i].isIndex = false;
        region[i].indexed = NULL;
    }

    *files_list = malloc(sizeof(file));
    (*files_list)->file_locations = malloc(sizeof(int)*10);
    (*files_list)->size = 1 + rand()%9 + rand()%10*0.1;
    sprintf((*files_list)->name, "%d", 1);
    (*files_list)->next = NULL;

    char temp[3];
    for (int i = 1; i < 50; ++i) {
        sprintf(temp, "%d", i + 1);
        push(*files_list, temp, 1 + rand()%9 + rand()%10*0.1, NULL);
    }
}
void menu(const int flag) {
    if (flag == 0)
        printf("1) Sequential Allocation\n"
               "2) Chained Allocation\n"
               "3) Indexed Allocation\n");
    else
        printf("\n");

    if (flag < 10)
        printf("4) Delete All Odd Files and Insert ABCDE\n");

    printf("5) Allocate Space for New Files\n");

    printf("0) Exit\n"
           "Select: ");
}
void deleteOdd(struct hierarchy *region, file **files_list) {
    char temp[8];
    for (int i = 0; i <= 50; i++) {
        if (i%2 == 1) {
            sprintf(temp, "%d", i);
            remove_by_name(files_list, temp);

            for (int j = 0; j < 500; ++j) {
                if (strcmp(region[j].file_id, temp) == 0) {
                    region[j].isIndex = false;
                    memset(region[j].file_id, 0, sizeof(region[j].file_id));
                    region[j].size_left = BLOCKSIZE;
                    region[j].isIndex = false;
                }
            }
            memset(&temp[0], 0, sizeof(temp));
        }
    }
}
void insertABCDE(file **files_list) {
    push(*files_list, "A", 7.0, NULL);
    push(*files_list, "B", 5.0, NULL);
    push(*files_list, "C", 2.0, NULL);
    push(*files_list, "D", 9.0, NULL);
    push(*files_list, "E", 3.5, NULL);
}

void printAlloc(struct hierarchy *region) {
    /*
     ---- : empty
     F(n) : Full and has block of file 'n'
     x.x(n) : block still has x.x free space and it belongs to  file 'n'
     */
    for (int j = 0; j < 10; ++j)
        printf("%d\t", j);
    printf("\n");

    for (int i = 1; i <= 500; ++i) {
        if (region[i - 1].isIndex)
            printf("Ind(%s)\t", region[i - 1].file_id);
        else if (region[i - 1].size_left == 0.0)
            printf("F(%s)\t", region[i - 1].file_id);
        else if (region[i - 1].size_left == BLOCKSIZE)
            printf("-----\t");

        else
            printf("%.1f(%s)\t", region[i - 1].size_left, region[i - 1].file_id);
        if (i%10 == 0)
            printf("\n");
    }
}
void seqAlloc(struct hierarchy *region, file **files_list) {
    int j = 0, i = 0;
    int n = 0;
    float leftover = 0;
    file *iterator = *files_list;

    while (iterator) {
        n = iterator->size/BLOCKSIZE;
        leftover = iterator->size - n*BLOCKSIZE*1.0;

        for (i = 0; i < n; ++i) {
            strcpy(region[j + i].file_id, iterator->name);
            region[j + i].size_left = 0;
        }
        if (leftover > 0.0) {
            strcpy(region[j + i].file_id, iterator->name);
            region[j + i].size_left = BLOCKSIZE - leftover;
        }

        j += i + 1; // how many blocks to skip, actually should be done in a more intelligent way

        if (iterator->next)
            iterator = iterator->next;
        else return;
    }
}
void seqAddToRegion(struct hierarchy *region, file **files_list) {

    int n = 0, i = 0, j = 0;
    float leftover = 0;

    // ABCDE are located consecutively,
    // so we just need to find A and iterate till the end of the list
    file *iterator = find(files_list, "A");

    while (iterator) {
        n = iterator->size/BLOCKSIZE;
        leftover = iterator->size - (float) n*BLOCKSIZE;
        int length = (leftover > 0.0 ? n + 1 : n);

        // find n (or n+1) consecutive blocks to fit a new file
        for (int counter = 0, k = 0; k < 500; ++k) {
            // if succefully found a first long enough region
            if (counter == length) {
                j = k - length;
                break;
            }

            if (region[k].size_left == BLOCKSIZE)
                ++counter;
            else
                counter = 0;
        }

        for (i = 0; i < n; ++i) {
            strcpy(region[j + i].file_id, iterator->name);
            region[j + i].size_left = 0;
        }
        if (leftover > 0.0) {
            strcpy(region[j + i].file_id, iterator->name);
            region[j + i].size_left = (float) BLOCKSIZE - leftover;
        }

        if (iterator->next)
            iterator = iterator->next;
        else return;
    }

}

int findFreeLocation(const struct hierarchy *region) {
    int location = rand()%500;
    // so it won't get into an infinite loop
    for (int i = 0; i < 1000; ++i) {
        if (region[location].size_left == BLOCKSIZE)
            return location;

        location = rand()%500;
    }
    return -1;
}

void chainedAlloc(struct hierarchy *region, file **files_list) {
    int i = 0;
    int n = 0;
    float leftover = 0;
    file *iterator = *files_list;
    int rfl = 0;    // random free location

    while (iterator) {
        n = iterator->size/BLOCKSIZE;
        leftover = iterator->size - n*BLOCKSIZE*1.0;

        // actually, here should check whether rfl is valid (enough free space)
        // but in this this case it would lower readability
        rfl = findFreeLocation(region);

        for (i = 0; i < n; ++i) {
            iterator->file_locations[i] = rfl;
            strcpy(region[rfl].file_id, iterator->name);
            region[rfl].size_left = 0;
            rfl = findFreeLocation(region);
        }
        if (leftover > 0.0) {
            iterator->file_locations[i] = rfl;
            strcpy(region[rfl].file_id, iterator->name);
            region[rfl].size_left = BLOCKSIZE - leftover;
        }

        if (iterator->next)
            iterator = iterator->next;
        else return;
    }
}
void chainAddToRegion(struct hierarchy *region, file **files_list) {
    int i = 0;
    int n = 0;
    float leftover = 0;
    int rfl = 0;    // random free location

    // ABCDE are located consecutively,
    // so we just need to find A and iterate till the end of the list
    file *iterator = find(files_list, "A");

    while (iterator) {
        n = iterator->size/BLOCKSIZE;
        leftover = iterator->size - n*BLOCKSIZE*1.0;

        rfl = findFreeLocation(region);

        region[rfl].isIndex = true;
        for (i = 0; i < n; ++i) {
            iterator->file_locations[i] = rfl;
            strcpy(region[rfl].file_id, iterator->name);
            region[rfl].isIndex = region[rfl].isIndex == true ? true : false;
            region[rfl].size_left = 0;
            rfl = findFreeLocation(region);
        }
        if (leftover > 0.0) {
            iterator->file_locations[i] = rfl;
            strcpy(region[rfl].file_id, iterator->name);
            region[rfl].isIndex = false;
            region[rfl].size_left = BLOCKSIZE - leftover;
        }

        if (iterator->next)
            iterator = iterator->next;
        else return;
    }
}

void indexedAlloc(struct hierarchy *region, file **files_list) {
    int i = 0;
    int n = 0;
    float leftover = 0;
    file *iterator = *files_list;
    int rfl = 0;    // random free location

    while (iterator) {
        n = iterator->size/BLOCKSIZE;
        leftover = iterator->size - n*BLOCKSIZE*1.0;

        rfl = findFreeLocation(region);

        // first block in a region only holds a pointer to a table of blocks
        iterator->file_locations[0] = rfl;
        strcpy(region[rfl].file_id, iterator->name);

        region[rfl].isIndex = true;
        region[rfl].indexed = iterator->file_locations;

        for (i = 1; i <= n; ++i) {
            rfl = findFreeLocation(region);
            char *name_to_copy = iterator->name;
            printf("name to be copied: %s\n", name_to_copy);
            strcpy(region[rfl].file_id, name_to_copy);
            iterator->file_locations[i] = rfl;

            region[rfl].isIndex = false;
            region[rfl].size_left = 0.0;

        }
        if (leftover > 0.0) {
            rfl = findFreeLocation(region);
            char *name_to_copy = iterator->name;
            printf("name to be copied: %s\n", name_to_copy);
            strcpy(region[rfl].file_id, name_to_copy);
            iterator->file_locations[i] = rfl;

            region[rfl].isIndex = false;
            region[rfl].size_left = BLOCKSIZE - leftover;
        }

        if (iterator->next)
            iterator = iterator->next;
        else return;
    }
}
void indAddToRegion(struct hierarchy *region, file **files_list) {
    int i = 0;
    int n = 0;
    float leftover = 0;
    int rfl = 0;    // random free location

    // ABCDE are located consecutively,
    // so we just need to find A and iterate till the end of the list
    file *iterator = find(files_list, "A");

    while (iterator) {
        n = iterator->size/BLOCKSIZE;
        leftover = iterator->size - n*BLOCKSIZE*1.0;

        rfl = findFreeLocation(region);

        // first block in a region only holds a pointer to a table of blocks
        iterator->file_locations[0] = rfl;
        strcpy(region[rfl].file_id, iterator->name);

        region[rfl].isIndex = true;
        region[rfl].indexed = iterator->file_locations;

        for (i = 1; i <= n; ++i) {
            rfl = findFreeLocation(region);
            iterator->file_locations[i] = rfl;
            region[rfl].isIndex = false;
            region[rfl].size_left = 0.0;
            strcpy(region[rfl].file_id, iterator->name);

        }
        if (leftover > 0.0) {
            rfl = findFreeLocation(region);
            iterator->file_locations[i] = rfl;
            strcpy(region[rfl].file_id, iterator->name);
            region[rfl].isIndex = false;
            region[rfl].size_left = BLOCKSIZE - leftover;
        }

        if (iterator->next)
            iterator = iterator->next;
        else return;
    }
}

int main() {
    int choice = 0;
    int flag = 0;
    struct hierarchy region[500];

    file *files_list = NULL;
    srand(time(NULL));
    init(region, &files_list);
    printf("Data has been generated\n");

    while (1) {
        menu(flag);
        scanf("%d", &choice);

        switch (choice) {
            case 1:
                seqAlloc(region, &files_list);
                printAlloc(region);
                print_list(files_list, 0);
                flag = 1;
                break;
            case 2:
                chainedAlloc(region, &files_list);
                printAlloc(region);
                print_list(files_list, 1);
                flag = 2;
                break;
            case 3:
                indexedAlloc(region, &files_list);
                printAlloc(region);
                print_list(files_list, 2);
                flag = 3;
                break;
            case 4:
                deleteOdd(region, &files_list);
                insertABCDE(&files_list);
                print_list(files_list, flag - 1);
                printf("\nABCDE has been inserted!\n");
                flag += 10;
                break;
            case 5:
                if (flag == 11) {
                    seqAddToRegion(region, &files_list);
                    printAlloc(region);
                    print_list(files_list, 0);
                }
                if (flag == 12) {
                    chainAddToRegion(region, &files_list);
                    printAlloc(region);
                    print_list(files_list, 1);
                }
                if (flag == 13) {
                    indAddToRegion(region, &files_list);
                    printAlloc(region);
                    print_list(files_list, 2);
                }

            case 0:
                printf("Exiting\n");
                exit(0);
            default:
                printf("Enter correct number\n");
                break;
        }
    }
}