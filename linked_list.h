//
// Created by Alexey on 11/12/2019.
//

#ifndef PURE_C11_LINKED_LIST_H
#define PURE_C11_LINKED_LIST_H

#endif //PURE_C11_LINKED_LIST_H

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>

struct node {
  int data;
  struct node *next;
};

struct node *head = NULL;

struct node *current = NULL;

//display the list
void printList() {
    struct node *ptr = head;
    printf("\n[ ");

    //start from the beginning
    while (ptr != NULL) {
        printf("(%d,%d) ", ptr->data);
        ptr = ptr->next;
    }

    printf(" ]");
}

//insert link at the first location
void insertFirst(int data) {
    //create a link
    struct node *link = (struct node *) malloc(sizeof(struct node));

    link->data = data;

    //point it to old first node
    link->next = head;

    //point first to new first node
    head = link;
}

//pop first item
struct node *pop() {

    //save reference to first link
    struct node *tempLink = head;

    //mark next to first link as first
    head = head->next;

    //return the deleted link
    return tempLink;
}

//is list empty
bool isEmpty() {
    return head == NULL;
}

int length() {
    int length = 0;
    struct node *current;

    for (current = head; current != NULL; current = current->next) {
        length++;
    }

    return length;
}