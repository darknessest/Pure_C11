#include <stdlib.h>
#include <stdbool.h>
#include <stdio.h>

#define TYPE int*

struct QNode {
  TYPE key;
  unsigned size;
  struct QNode *next;
};

typedef struct Queue {
  struct QNode *front, *rear;
} queue;

struct QNode *newNode(TYPE k, const int size) {
    struct QNode *temp = (struct QNode *) malloc(sizeof(struct QNode));
    temp->key = malloc(sizeof(int)*(size + 1));

    for (int i = 0, tempval = 0; i < size; i++) {
        //memcpy
        tempval = k[i];
        temp->key[i] = tempval;
    }
    temp->next = NULL;
    return temp;
}

queue *createQueue() {
    queue *q = (queue *) malloc(sizeof(queue));
    q->front = q->rear = NULL;
    return q;
}

void enQueue(queue *q, TYPE k, const int size) {
    struct QNode *temp = newNode(k, size);
    temp->size = (unsigned int) size;
    if (q->rear == NULL) {
        q->front = q->rear = temp;
        return;
    }

    q->rear->next = temp;
    q->rear = temp;
}

struct QNode *deQueue(queue *q) {
    // If queue is empty, return NULL
    if (q->front == NULL)
        return NULL;

    struct QNode *temp = q->front;
    q->front = q->front->next;

    if (q->front == NULL)
        q->rear = NULL;
    return temp;
}
