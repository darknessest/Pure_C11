//5_2
#include "queue.h"

#define SIZE 4

struct AdjListNode {
  int dest;
  int val;
  struct AdjListNode *next;
};//additional
struct AdjList {
  struct AdjListNode *head;
};//additional
typedef struct graph {
  int V;    //number vertexes
  bool isDirected;
  struct AdjList *array;
} Graph;

Graph *createGraph(const int V, const bool isDirected) {
    Graph *graph = (Graph *) malloc(sizeof(Graph));
    graph->V = V;
    graph->isDirected = isDirected;
    // Create an array of adjacency lists.  Size of
    // array will be V
    graph->array = (struct AdjList *) malloc(V*sizeof(struct AdjList));

    // Initialize each adjacency list as empty by
    // making head as NULL
    for (int i = 0; i < V; ++i)
        graph->array[i].head = NULL;

    return graph;
}
struct AdjListNode *newAdjListNode(const int dest) {
    struct AdjListNode *newNode =
        (struct AdjListNode *) malloc(sizeof(struct AdjListNode));
    newNode->dest = dest;
    newNode->next = NULL;
    return newNode;
}
void PutVex(Graph *G, const int src, const int dest, const int val) {
    // Add an edge from src to dest.  A new node is
    // added to the adjacency list of src.  The node
    // is added at the begining
    struct AdjListNode *newNode = newAdjListNode(dest);
    newNode->next = G->array[src].head;
    G->array[src].head = newNode;
    G->array[src].head->val = val;

    // graph is undirected -> dest to src also
    if (G->isDirected == 1) {
        newNode = newAdjListNode(src);
        newNode->next = G->array[dest].head;
        G->array[dest].head = newNode;
        G->array[dest].head->val = val;
    }
}

void printGraph(const Graph *graph) {
    for (int v = 0; v < graph->V; ++v) {
        struct AdjListNode *iter = graph->array[v].head;
        printf("\nAdjacency list of vertex %d\n head ", v);
        while (iter) {
            printf("-> %d(%d)", iter->dest, iter->val);
            iter = iter->next;
        }
        printf("\n");
    }
}
void findpaths(Graph *g, int src, int dst, int k);



int main() {
    Graph *A;
    // 0 - directed; 1 - undirected
    A = createGraph(SIZE, 0);
    PutVex(A, 0, 3, 1);
    PutVex(A, 0, 1, 1);
    PutVex(A, 0, 2, 1);
    PutVex(A, 1, 3, 1);
    PutVex(A, 2, 0, 1);
    PutVex(A, 2, 1, 1);
    printGraph(A);

    findpaths(A, 2, 3, 4);

}

void printpath(int *path, const int size) {
    for (int i = 0; i < size; i++)
        printf("%d ", path[i]);
    printf("\n");
}

int isNotVisited(const int x, int *path, const int size) {
    //prevents loops
    for (int i = 0; i < size; ++i)
        if (path[i] == x)
            return 0;

    return 1;
}

void findpaths(Graph *g, int src, const int dst, const int k) {
    // creating a queue with paths
    queue *q = createQueue();

    int curr_size = 1;
    int *path = (int *) malloc(sizeof(int)*curr_size);//current path
    if (path == NULL)
        printf("error memalloc\n");

    path[0] = src;

    enQueue(q, path, 1);

    while (q->front) {   // == not empty
        path = realloc(path, sizeof(int)*0);
        curr_size = q->front->size;//length of path currently in use
        path = realloc(path, (curr_size)*sizeof(int));
        for (int i = 0, tempval = 0; i < curr_size; ++i) {
            //memcpy
            tempval = q->front->key[i];
            path[i] = tempval;
        }
        deQueue(q);

        int last = path[curr_size - 1];
        struct AdjListNode *temp = g->array[last].head;

        if (last == dst && curr_size == k) {
            // if last vertex is the desired destination
            // then print the path
            printf("\nThe first path from %d to %d of length %d is: ", src, dst, k);
            printpath(path, curr_size);
            return;
        } else {
            //BFS
            ++curr_size;
            while (temp) {
                if (isNotVisited(temp->dest, path, curr_size)) {
                    //new available path
                    int *newpath = malloc(sizeof(int)*curr_size);
                    for (int i = 0, tempval = 0; i < curr_size - 1; ++i) {
                        //memcpy
                        tempval = path[i];
                        newpath[i] = tempval;
                    }

                    newpath[curr_size - 1] = temp->dest;
                    enQueue(q, newpath, curr_size);
                }
                temp = temp->next;
            }
        }
    }
    printf("\nThe first path from %d to %d of length %d doesn't exist\n", src, dst, k);
}
