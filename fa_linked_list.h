//
// Created by Alexey on 27/11/2019.
//
#include <stdbool.h>
#include <math.h>       // gcc name.c -o name -lm

#define BLOCKSIZE 2.0

typedef struct File {
  char name[3];
  float size;
  // file is divided between blocks
  // this array holds addressed of each block
  int *file_locations;
  struct File *next;
} file;

void print_list(file *head, const int state) {
    file *current = head;

    while (current != NULL) {
        printf("%s.txt\t\t%.1fk\t", current->name, current->size);
        if (state == 1)
            for (int i = 0; i < ceil(current->size/BLOCKSIZE); ++i)
                printf("%d <- ", current->file_locations[i]);
        if (state == 2)
            for (int i = 0; i <= ceil(current->size/BLOCKSIZE); ++i)
                printf("%d <- ", current->file_locations[i]);

        printf("\n");
        current = current->next;
    }
}

void push(file *head, const char *name, const float size, const int *location) {

    file *current = head;
    while (current->next != NULL) {
        current = current->next;
    }

    /* now we can add a new element */
    current->next = malloc(sizeof(file));
    strcpy(current->next->name, name);
    current->next->size = size;
    current->next->file_locations = malloc(sizeof(int)*(int) (size/BLOCKSIZE + 1));

    current->next->next = NULL;
}

file *pop(file **head) {
    int retval = -1;
    file *next_node = NULL;
    file *ret_node = NULL;
    if (*head == NULL) {
        return NULL;
    }

    next_node = (*head)->next;
    memcpy(ret_node, next_node, sizeof(file));
    free(*head);
    *head = next_node;

    return ret_node;
}

int remove_by_name(file **head, const char *name) {
    // deleting head
    if (strcmp((*head)->name, name) == 0) {
        (*head) = (*head)->next;
        return 0;
    }

    file *prev = *head;
    file *cur = *head;

    while (cur != NULL) {
        if (strcmp(cur->name, name) == 0)
            break;
        if (cur->next == NULL)
            return 404;
        prev = cur;
        cur = cur->next;
    }

    if (cur->next != NULL)
        prev->next = cur->next;

    free(cur);
    return 0;
}
file *find(file **head, const char *name) {
    if (*head == NULL)
        return NULL;

    file *cur = *head;
    file *ret = malloc(sizeof(file));

    //navigate through list

    while (cur != NULL) {
        if (strcmp(cur->name, name) == 0)
            break;
        if (cur->next == NULL)
            return NULL;

        cur = cur->next;
    }

    memcpy(ret, cur, sizeof(file));

    //if data found, return the current Link
    return ret;
}