//4.2
#include "queue.h"
#include <limits.h>

#define SIZE 10
//#define TYPE int   //bool if all arcs are equal

typedef struct Pair {
  int x;
  int y;
} pair;//additional

struct AdjListNode {
  int dest;
  TYPE val;
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
}//works
void destroy_graph(Graph *G) {
}
struct AdjListNode *newAdjListNode(const int dest) {
    struct AdjListNode *newNode =
        (struct AdjListNode *) malloc(sizeof(struct AdjListNode));
    newNode->dest = dest;
    newNode->next = NULL;
    return newNode;
}//works
void PutVex(Graph *G, const int src, const int dest, const TYPE val) {
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
}//works
pair LocateVex(const Graph *G, const int val) {
    for (int v = 0; v < G->V; ++v) {
        struct AdjListNode *iter = G->array[v].head;
        while (iter) {
            if (iter->val == val)
                return (pair) {v, iter->dest};
            iter = iter->next;
        }
    }
    return (pair) {INT_MIN, INT_MIN};
}//works
TYPE GetVex(const Graph *G, const pair address) {
    //return value
    if (address.x > G->V)
        return INT_MIN;
    struct AdjListNode *iter = G->array[address.x].head;
    while (iter) {
        if (iter->dest == address.y)
            return iter->val;
        iter = iter->next;
    }

    return INT_MIN;
}//works
int FirstAdjVex(const Graph *G, const int v) {
    if (G->array[v].head)
        return G->array[v].head->dest;
    return -1;
}//works
int NextAdjVex(const Graph *G, const int v, const int w) {
    if (G->array[v].head) {
        struct AdjListNode *iter = G->array[v].head;
        while (iter && iter->dest != w)
            iter = iter->next;
        if (iter->dest == w && iter->next)
            return iter->next->dest;
    }
    return -1;
}//works
void InsertVex(Graph *G, const int v) {
    if (G->V < (v + 1)) {
        G->array = realloc(G->array, (v + 1)*sizeof(struct AdjList));
        for (int i = G->V; i < v + 1; ++i)
            G->array[i].head = NULL;
        G->V = (v + 1);
    }
}//works
void DeleteVex(Graph *G, const int v) {
    if (G->V >= v) {
        struct AdjListNode *current = G->array[v].head;
        struct AdjListNode *next;

        while (current != NULL) {
            next = current->next;
            free(current);
            current = next;
        }
        G->array[v].head = NULL;
    }
}//works
void InsertArc(Graph *G, const int v, const int w) {
    if (G->V >= v) {
        PutVex(G, v, w, 1);
    } else {
        InsertVex(G, v);
        PutVex(G, v, w, 1);
    }
}//works
void DeleteArc(Graph *G, const int v, const int w) {
    if (G->V >= v) {
        struct AdjListNode *prev = G->array[v].head;
        if (prev->dest != w) {
            struct AdjListNode *temp;
            while (prev->next && prev->next->dest != w)
                prev = prev->next;
            if (prev->dest != w)
                return;//wasn't found
            temp = prev->next;
            prev->next = temp->next;
            free(temp);
        } else {
            G->array[v].head = prev->next;
            free(prev);
        }
    }
}//works

//TODO BFS and DFS
void BFS(Graph *G, int s) { //BFS search
    // Mark all the vertices as not visited
    bool visited[G->V];
    for (int i = 0; i < G->V; i++)
        visited[i] = false;

    // Create a queue for BFS
    queue *Q = createQueue();

    // Mark the current node as visited and enqueue it
    visited[s] = true;

    enQueue(Q, s);
    // 'i' will be used to get all adjacent
    // vertices of a vertex
    struct AdjListNode *iter;
    while (Q->rear) {
        // Dequeue a vertex from queue and print it
        s = deQueue(Q)->key;
        printf("%d ", s);
//        s = Q.front();
//        cout << s << " ";
//        Q.pop_front();

        // Get all adjacent vertices of the dequeued
        // vertex s. If a adjacent has not been visited,
        // then mark it visited and enqueue it
        iter = G->array[s].head;
        while (iter) {
            if (!visited[iter->dest]) {
                visited[iter->dest] = true;
                enQueue(Q, iter->dest);
            }
            iter = iter->next;
        }

    }
}
//void DFSUtil(int v, bool visited[]) {
//    // Mark the current node as visited and
//    // print it
//    visited[v] = true;
//    cout << v << " ";
//
//    // Recur for all the vertices adjacent
//    // to this vertex
//    list<int>::iterator i;
//    for (i = adj[v].begin(); i != adj[v].end(); ++i)
//        if (!visited[*i])
//            DFSUtil(*i, visited);
//}

// DFS traversal of the vertices reachable from v.
// It uses recursive DFSUtil()
//void Graph::DFS(int v) {
//    // Mark all the vertices as not visited
//    bool *visited = new
//    bool[V];
//    for (int i = 0; i < V; i++)
//        visited[i] = false;
//
//    // Call the recursive helper function
//    // to print DFS traversal
//    DFSUtil(v, visited);
//}

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
}//works

int main() {
//    // 1 - directed; 0 - undirected
//    Graph *A = createGraph(SIZE, 0);
//    for (int i = 0; i < 20; ++i) {
//        PutVex(A, rand()%10, rand()%10, 1 + rand()%10);
//    }
//    printGraph(A);
//    printf("----------------------------------");
//    pair p = LocateVex(A, 1);
//    printf("\nLocating value 1: %d - %d\n", p.x, p.y);
//    printf("\nValue between 9 - 3: %d\n", GetVex(A, (pair) {9, 3}));
//    printf("\nFirst vertex adjacent to 7: %d\n", FirstAdjVex(A, 7));
//    printf("\nNext vertex adjacent to  2 - 7: %d\n", NextAdjVex(A, 2, 7));
////    InsertVex(A, 10); //[0-10] insert 10th
//    DeleteVex(A, 3);
//    InsertArc(A, 12, 1);
//    DeleteArc(A, 12, 7);
//    printf("----------------------------------\n");
//    printGraph(A);

    Graph *ss = createGraph(4, 0);
    PutVex(ss, 0, 1, 1);
    PutVex(ss, 0, 2, 1);
    PutVex(ss, 1, 2, 1);
    PutVex(ss, 2, 0, 1);
    PutVex(ss, 2, 3, 1);
    PutVex(ss, 3, 3, 1);
    printGraph(ss);
    BFS(ss, 2);//should be 2 0 3 1
    return 0;
}