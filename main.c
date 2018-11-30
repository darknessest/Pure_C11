//5_1
#include "pq.h"
#include <limits.h>
#include <stdbool.h>

// Number of vertices in the graph
#define SIZE 50 //size of graph

void primMST(int graph[SIZE][SIZE], const int x) {
    int finalMST[SIZE];
    int key_weights[SIZE];
    bool inMST[SIZE];

    // Initialization
    for (int i = 0; i < SIZE; i++)
        key_weights[i] = INT_MAX, inMST[i] = false;

    key_weights[x] = 0;
    finalMST[x] = -1;   // First node is always root of MST
    Node *pq = newNode(x, 0);   //Priority queue
    pair temp;

    while (!isEmpty(&pq)) {

        temp = peek(&pq);
        pop(&pq);
        inMST[temp.vertex] = true;

        for (int v = 0; v < SIZE; v++) {
            if (inMST[v] == false && graph[temp.vertex][v] < key_weights[v] && graph[temp.vertex][v]) {
                finalMST[v] = temp.vertex;
                key_weights[v] = graph[temp.vertex][v];
                //pushing into priority queue
                push(&pq, v, graph[temp.vertex][v]);
            }
        }
    }

    // print the constructed MST
    int sum = 0;
    for (int i = 0; i < SIZE; ++i) {
        if (i == x)
            continue;
        printf("%d - %d\tw:%d\n", finalMST[i], i, graph[finalMST[i]][i]);
        sum += graph[finalMST[i]][i];
    }
    printf("\nsum: %d", sum);
}

int main() {
    int graph[SIZE][SIZE];
    //Initialization
    for (int i = 0; i < SIZE; ++i) {
        for (int j = i; j < SIZE; ++j) {
            if (i == j)
                graph[i][j] = 0;
            else {
                graph[i][j] = (rand()%2 == 0 ? (rand() + 1)%10 : 0);
                graph[j][i] = graph[i][j];
            }
        }
    }
    //Printing graph
//    for (int k = 0; k < SIZE; ++k) {
//        printf("[");
//        for (int i = 0; i < SIZE; ++i) {
//            printf("%d", graph[k][i]);
//            if (i != SIZE)
//                printf(",");
//        }
//        printf("],\n");
//    }
    primMST(graph, 8);

    return 0;
}
