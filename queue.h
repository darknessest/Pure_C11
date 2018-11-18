#include <stdlib.h>
#include <stdbool.h>
#include <stdio.h>
#define TYPE int    //bool if all arcs are equal

struct QNode {
  TYPE key;
  struct QNode *next;
};

typedef struct Queue {
  struct QNode *front, *rear;
} queue;

struct QNode *newNode(TYPE k) {
    struct QNode *temp = (struct QNode *) malloc(sizeof(struct QNode));
    temp->key = k;
    temp->next = NULL;
    return temp;
}

queue *createQueue() {
    queue *q = (queue *) malloc(sizeof(queue));
    q->front = q->rear = NULL;
    return q;
}

void enQueue(queue *q, TYPE k) {
    struct QNode *temp = newNode(k);

    if (q->rear == NULL) {
        q->front = q->rear = temp;
        return;
    }

    q->rear->next = temp;
    q->rear = temp;
}

struct QNode *deQueue(queue *q) {
    // If queue is empty, return NULL.
    if (q->front == NULL)
        return NULL;

    struct QNode *temp = q->front;
    q->front = q->front->next;

    if (q->front == NULL)
        q->rear = NULL;
    return temp;
}