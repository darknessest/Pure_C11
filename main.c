#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <limits.h>

#define V 11

void dijkstra(const int graph[V][V], int src, int dst);
bool hamilton(const int graph[V][V], int src);
void UI(const int graph[V][V]);

const char places[V][25] = {"nanputuo temple", "library", "qinye canteen", "song'en bld",
                            "furong lake", "qunxian(xicun) gate", "yanwu stadium",
                            "shangxian stadium", "science and art center", "furong tunnel",
                            "baicheng xiaomen"};
// nanputuo temple,
// library,
// qinye canteen,
// song'en bld,
// furong lake,
// qunxian(xicun) gate
// yanwu stadium,
// shangxian stadium
// science and art center
// furong tunnel,
// baicheng xiaomen,

// 胡里山炮台、曾厝埯
/*
Nanputuo - library, qinye canteen
Library - Nanputuo, qinye canteen, song'en bld, qunxian(xicun) gate
qinye - library, nanputuo,song'en bld, furong lake, furong tunnel, science and art center
song'en - library, Nanputuo, qinye canteen, yanwu stadium, furong lake, science and art center
furong lake - song'en, qinye, science and art center, furong tunnel, baicheng xiaomen,
qunxian(xicun) gate - library, yanwu stadium, science and art center,
yanwu stadium - library, song'en, science and art center, furong lake, shangxian stadium
shangxian stadium - furong tunnel, science and art center, baicheng xiaomen, yanwu stadium,
science and art center - song'en bld, furong lake, qunxian(xicun) gate, yanwu stadium, shangxian stadium, furong tunnel, baicheng xiaomen
furong tunnel - qinye, science and art center, baicheng xiaomen, shangxian stadium

baicheng xiaomen - qinye, furong lake, furong tunnel, shangxian stadium, science and art center,
*/

int main() {
    int G[V][V] = {
        {0, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0},
        {1, 0, 1, 1, 0, 1, 1, 0, 0, 0, 0},
        {1, 1, 0, 1, 1, 0, 0, 0, 1, 1, 1},
        {1, 1, 1, 0, 1, 1, 1, 0, 1, 0, 0},
        {0, 0, 1, 1, 0, 0, 1, 0, 1, 1, 1},
        {0, 1, 0, 1, 0, 0, 1, 0, 1, 0, 0},
        {0, 1, 0, 1, 1, 1, 0, 1, 1, 0, 0},
        {0, 0, 0, 0, 0, 0, 1, 0, 1, 1, 1},
        {0, 0, 1, 1, 1, 1, 1, 1, 0, 1, 1},
        {0, 0, 1, 0, 1, 0, 0, 1, 1, 0, 1},
        {0, 0, 1, 0, 1, 0, 0, 1, 1, 1, 0}
    };

    for (int i = 0; i < V; ++i) {
        for (int j = 0; j < V; ++j) {
            printf("%d ", G[i][j]);
        }
        printf("\n");
    }
    UI(G);
}
//TODO add clear screen feature
void UI(const int graph[V][V]) {
    short user_input = 0, src = 0, dst = 0;
    bool loop = 0;

    printf("Hello!\n");
    SCREEN_1:
    printf("Please, choose the place where you are right now (starting point):\n");
    for (int i = 0; i < V; ++i)
        printf("%d. %s\n", i + 1, places[i]);

    printf("0. Exit\n");
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
        case 6:
            dst = 6;
            break;
        case 7:
            dst = 7;
            break;
        case 8:
            dst = 8;
            break;
        case 9:
            dst = 9;
            break;
        case 10:
            dst = 10;
            break;
        case 11:
            dst = 11;
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
           "99. Go back\n"
           "0. Exit\n");
    scanf("%hd", &user_input);
    switch (user_input) {
        case 0:
            printf("\nThank you! Goodbye!\n");
            exit(0);
        case 1:
            loop = 1;
            goto SCREEN_4;
        case 2:
            loop = 0;
            break;
        case 99:
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
    printf("Please, choose the place where you want to go (finishing point):\n");
    for (int i = 0; i < V; ++i)
        printf("%d. %s\n", i + 1, places[i]);

    printf("99. Go back\n"
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
        case 6:
            dst = 6;
            break;
        case 7:
            dst = 7;
            break;
        case 8:
            dst = 8;
            break;
        case 9:
            dst = 9;
            break;
        case 10:
            dst = 10;
            break;
        case 11:
            dst = 11;
            break;

        case 99:
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
    SCREEN_4:
    if (loop == 1) {
        hamilton(graph, src);
    } else {
        dijkstra(graph, src - 1, dst - 1);
    }

}
//----------------------------------------------------------------------------
int minDistance(const int dist[], const bool sptSet[]) {
    int min = INT_MAX, min_index = 0;

    for (int v = 0; v < V; v++)
        if (sptSet[v] == false &&
            dist[v] <= min)
            min = dist[v], min_index = v;

    return min_index;
}
void printPath(const int parent[], const int x) {
    if (parent[x] == -1)
        return;
    if (parent[x] <= V)
        printPath(parent, parent[x]);

    printf("->%d", x);
}
void printDijkstraSolutions(const int *dist, const int src, const int dst, const int *parent) {
    printf("From-To\t Distance\tPath");

    printf("\n%d -> %d \t\t %d\t\t%d ", src, dst, dist[dst], src);
    printPath(parent, dst);
}
void dijkstra(const int graph[V][V], const int src, const int dst) {
    int dist[V];
    bool sptSet[V];
    int parent[V];

    for (int i = 0; i < V; i++) {
        if (i == src)
            parent[i] = -1;
        dist[i] = INT_MAX;
        sptSet[i] = false;
    }
    dist[src] = 0;
    for (int count = 0; count < V - 1; count++) {
        int u = minDistance(dist, sptSet);
        sptSet[u] = true;
        for (int v = 0; v < V; v++) {
            if (!sptSet[v] && graph[u][v] &&
                dist[u] + graph[u][v] < dist[v]) {
                parent[v] = u;
                dist[v] = dist[u] + graph[u][v];
            }
        }
    }

    printDijkstraSolutions(dist, src, dst, parent);
}
//----------------------------------------------------------------------------
void printHamCycle(int *path, const int src, const int G[V][V]) {
    double sum = 0;

    printf("Solution Exists:"
           " Following is one Hamiltonian Cycle \n");
    for (int i = src, temp = src; i < V; i++) {
        printf(" %d ", path[i] + 1);
        sum += G[temp][path[i]];
        temp = path[i];
    }
    for (int i = 0, temp = 0; i <= src; i++) {
        printf(" %d ", path[i] + 1);
        sum += G[temp][path[i]];
        temp = path[i];
    }
    printf("\nDistance: %lf", sum);
}
bool isSafe(const int v, const int graph[V][V], const int path[], const int pos){
//checking wheter vertex can be used
    //wheter vertex is adjacent to prev vertex
    if (graph[path[pos - 1]][v] == 0)
        return false;

    //wheter vertex is already included
    for (int i = 0; i < pos; i++)
        if (path[i] == v)
            return false;

    return true;
}
bool hamCycleUtil(const int graph[V][V], int path[], int pos) {
    //if all vertices are included in Hamiltonian Cycle
    if (pos == V) {
        //if there is an edge from the last included vertex to the
        //first vertex
        if (graph[path[pos - 1]][path[0]] == 1)
            return true;
        else
            return false;
    }

    // Try different vertices as a next candidate in Hamiltonian Cycle.
    // We don't try for 0 as we included 0 as starting point in in hamilton()
    for (int v = 1; v < V; v++) {
        if (isSafe(v, graph, path, pos)) {
            path[pos] = v;
            //continue to construct the path
            if (hamCycleUtil(graph, path, pos + 1) == true)
                return true;
            //removing vertex if useless
            path[pos] = -1;
        }
    }
    //if there's no path
    return false;
}
bool hamilton(const int graph[V][V], const int src) {
    int *path = malloc(sizeof(int)*V);
    for (int i = 0; i < V; i++)
        path[i] = -1;

    //graph is undirected so the cycle can be started from any point
    path[0] = 0;
    if (hamCycleUtil(graph, path, 1) == false) {
        printf("\nSorry, currently there's no route for round trip");
        return false;
    }

    printHamCycle(path, src, graph);
    return true;
}