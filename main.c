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
// 1.nanputuo temple,
// 2.library,
// 3.qinye canteen,
// 4.song'en bld,
// 5.furong lake,
// 6.qunxian(xicun) gate
// 7.yanwu stadium,
// 8.shangxian stadium
// 9.science and art center
// 10.furong tunnel,
// 11.baicheng xiaomen,

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
        {0, 300, 500, 450, 0, 0, 0, 0, 0, 0, 0},
        {300, 0, 350, 150, 0, 500, 450, 0, 0, 0, 0},
        {500, 350, 0, 450, 250, 0, 0, 0, 300, 550, 600},
        {450, 150, 450, 0, 50, 450, 350, 0, 200, 0, 0},
        {0, 0, 250, 50, 0, 0, 350, 0, 90, 750, 800},
        {0, 500, 0, 450, 0, 0, 450, 0, 350, 0, 0},
        {0, 450, 0, 350, 350, 450, 0, 200, 400, 0, 0},
        {0, 0, 0, 0, 0, 0, 200, 0, 200, 750, 700},
        {0, 0, 300, 200, 90, 350, 400, 200, 0, 750, 700},
        {0, 0, 550, 0, 750, 0, 0, 750, 750, 0, 550},
        {0, 0, 600, 0, 800, 0, 0, 700, 700, 550, 0}
    };
//
//    for (int i = 0; i < V; ++i) {
//        for (int j = 0; j < V; ++j) {
//            printf("%d\t", G[i][j]);
//        }
//        printf("\n");
//    }
    UI(G);
}

void UI(const int graph[V][V]) {
    short user_input = 0, src = 0, dst = 0;
    bool loop = 0;

    printf("Welcome!\n");
    SCREEN_1:
    printf("Choose the place where you are right now (starting point):\n");
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
        case 6:
            src = 6;
            break;
        case 7:
            src = 7;
            break;
        case 8:
            src = 8;
            break;
        case 9:
            src = 9;
            break;
        case 10:
            src = 10;
            break;
        case 11:
            src = 11;
            break;
        default:
            printf("Please, input valid number!\n"
                   "Press enter to continue...");
            getchar();
            getchar();
            system("clear");
            goto SCREEN_1;
    }

    SCREEN_2:
    //clear screen
    system("clear");
    printf("Starting point: %s\nDo you want to make a round trip ?\n"
           "1. Yes\n"
           "2. No\n"
           "99. Go back\n"
           "0. Exit\n", places[src-1]);
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
            system("clear");
            goto SCREEN_1;
        default:
            printf("Please, input valid number!\n"
                   "Press enter to continue...");
            getchar();
            getchar();
            system("clear");
            goto SCREEN_2;
    }

    SCREEN_3:
    //clear screen
    system("clear");
    printf("Starting point: %s\nPlease, choose the place where you want to go (finishing point):\n", places[src-1]);
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
            system("clear");
            goto SCREEN_2;
        default:
            printf("Please, input valid number!\n"
                   "Press enter to continue...");
            getchar();
            getchar();
//            system("clear");
            system("clear");
            goto SCREEN_3;
    }
    SCREEN_4:
    system("clear");
    if (loop == 1) {
        hamilton(graph, src - 1);
    } else {
        dijkstra(graph, (src - 1), (dst - 1));
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
void printDijkstraPath(const int *parent, const int x) {
    if (parent[x] == -1)
        return;
    if (parent[x] <= V)
        printDijkstraPath(parent, parent[x]);

    printf("-> %d.%s ", x + 1, places[x]);
}
void printDijkstraSolution(const int *dist, const int src, const int dst, const int *parent) {
    printf("From-To: %s -> %s\n", places[src], places[dst]);
    printf("Distance: %d m\n", dist[dst]);
    printf("Path: %d.%s ", src + 1, places[src]);
    printDijkstraPath(parent, dst);
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

    printDijkstraSolution(dist, src, dst, parent);
}
//----------------------------------------------------------------------------
void printHamCycle(int *path, const int src, const int graph[V][V]) {
    int total_distance = 0;
    int temp_src = 0;
    printf("Solution Exists:"
           " Following is one Hamiltonian Cycle \n");
    for (int j = 0; j < V; ++j)
        if (src == path[j])
            temp_src = j;

    for (int i = temp_src, temp = temp_src; i < V; i++) {
        printf(" %d.%s ", path[i] + 1, places[path[i]]);
        total_distance += graph[temp][path[i]];
        temp = path[i];
    }
    for (int i = 0, temp = 0; i <= temp_src; i++) {
        printf(" %d.%s", path[i] + 1, places[path[i]]);
        total_distance += graph[temp][path[i]];
        temp = path[i];
    }
    printf("\nDistance: %d m", total_distance);
}
bool isSafe(const int v, const int graph[V][V], const int path[], const int pos) {
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
        if (graph[path[pos - 1]][path[0]] != 0)
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