#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <limits.h>

#define V 9

void dijkstra(int graph[V][V], int src, int dst);
//TODO make dijkstra
void hamilton(int graph[V][V], int src);
//TODO make hamilton cycle algo
void UI();

int main() {
    UI();

}

void UI() {
    short user_input = 0, src = 0, dst = 0;
    bool loop = 0;

    printf("Hello!\n");
    SCREEN_1:
    printf("Please, choose the place where you are right now (starting point):\n"
           "1. \n"
           "2. \n"
           "3. \n"
           "4. \n"
           "5. \n"
           "0. Exit\n");
    //TODO add real locations
    scanf("%hd", &user_input);

    //starting location
    switch (user_input) {
        case 0:
            printf("\nThank you! Goodbye!\n");
            exit(0);

        case 1:
            src = 1;
            break;
        case 2:
            src = 2;
            break;
        case 3:
            src = 3;
            break;
        case 4:
            src = 4;
            break;
        case 5:
            src = 5;
            break;
        default:
            printf("Please, input valid number!\n"
                   "Press enter to continue...");
            getchar();
            getchar();
//            system("clear");
            goto SCREEN_1;
    }

    SCREEN_2:
    //clear screen
    printf("\nDo you want to make a round trip ?\n"
           "1. Yes\n"
           "2. No\n"
           "8. Go back\n"
           "0. Exit\n");
    scanf("%hd", &user_input);
    switch (user_input) {
        case 0:
            printf("\nThank you! Goodbye!\n");
            exit(0);
        case 1:
            loop = 0;
            break;
        case 2:
            loop = 1;
            break;
        case 8:
            //clear screen;
            goto SCREEN_1;
        default:
            printf("Please, input valid number!\n"
                   "Press enter to continue...");
            getchar();
            getchar();
//            system("clear");
            goto SCREEN_2;
    }

    SCREEN_3:
    //clear screen
    printf("Please, choose the place where you want to go (finishing point):\n"
           "1. \n"
           "2. \n"
           "3. \n"
           "4. \n"
           "5. \n"
           "8. Go back\n"
           "0. Exit\n");
    scanf("%hd", &user_input);

    //starting location
    switch (user_input) {
        case 0:
            printf("\nThank you! Goodbye!\n");
            exit(0);

        case 1:
            dst = 1;
            break;
        case 2:
            dst = 2;
            break;
        case 3:
            dst = 3;
            break;
        case 4:
            dst = 4;
            break;
        case 5:
            dst = 5;
            break;
        case 8:
            //clear screen;
            goto SCREEN_2;
        default:
            printf("Please, input valid number!\n"
                   "Press enter to continue...");
            getchar();
            getchar();
//            system("clear");
            goto SCREEN_3;
    }
    if (loop == 1) {
//        hamilton();
    } else {
//        dijkstra();
    }

}

int minDistance(const int dist[], const bool sptSet[]) {
    int min = INT_MAX, min_index;

    for (int v = 0; v < V; v++)
        if (sptSet[v] == false &&
            dist[v] <= min)
            min = dist[v], min_index = v;

    return min_index;
}

void printPath(int parent[], int j) {

    // Base Case : If j is source
    if (parent[j] == -1)
        return;

    printPath(parent, parent[j]);

    printf("%d ", j);
}

int printSolution(int dist[], int n,
                  int parent[]) {
    int src = 0;
    printf("Vertex\t Distance\tPath");
    for (int i = 1; i < V; i++) {
        printf("\n%d -> %d \t\t %d\t\t%d ",
               src, i, dist[i], src);
        printPath(parent, i);
    }
}

void dijkstra(int graph[V][V], const int src, const int dst) {
    int dist[V];

    bool sptSet[V];

    int parent[V];

    for (int i = 0; i < V; i++) {
        parent[0] = -1;
        dist[i] = INT_MAX;
        sptSet[i] = false;
    }

    dist[src] = 0;

    // Find shortest path
    // for all vertices
    for (int count = 0; count < V - 1; count++) {

        int u = minDistance(dist, sptSet);
        sptSet[u] = true;

        for (int v = 0; v < V; v++)
            if (!sptSet[v] && graph[u][v] &&
                dist[u] + graph[u][v] < dist[v]) {
                parent[v] = u;
                dist[v] = dist[u] + graph[u][v];
            }
    }

    // print the constructed
    // distance array
    printSolution(dist, V, parent);
}