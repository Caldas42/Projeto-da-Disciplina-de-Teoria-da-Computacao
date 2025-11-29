#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

#define INF (INT_MAX / 4)

void dijkstra(int n, int *graph, int src, int *dist, int *prev);
void print_path(int *prev, int src, int target);

int main(void) {
    int n = 5;
    int *graph = (int*)malloc(n * n * sizeof(int));
    if (!graph) { perror("malloc"); return 1; }

    for (int i = 0; i < n * n; ++i) graph[i] = -1;

    graph[0*n + 1] = 2;
    graph[0*n + 2] = 5;
    graph[1*n + 2] = 1;
    graph[1*n + 3] = 4;
    graph[2*n + 3] = 2;
    graph[3*n + 4] = 1;

    int *dist = (int*)malloc(n * sizeof(int));
    int *prev = (int*)malloc(n * sizeof(int));
    if (!dist || !prev) { perror("malloc"); return 1; }

    dijkstra(n, graph, 0, dist, prev);

    printf("Distâncias a partir de 0 (A):\n");
    for (int i = 0; i < n; ++i) {
        if (dist[i] == INF) printf("  0 -> %d: inacessível\n", i);
        else printf("  0 -> %d: %d\n", i, dist[i]);
    }

    printf("\nCaminho 0 -> 4: ");
    if (dist[4] == INF) printf("inacessível\n");
    else {
        print_path(prev, 0, 4);
        printf("\n");
    }

    free(graph);
    free(dist);
    free(prev);
    return 0;
}

void dijkstra(int n, int *graph, int src, int *dist, int *prev) {
    int *visited = (int*)calloc(n, sizeof(int));
    if (!visited) { perror("calloc"); exit(1); }

    for (int i = 0; i < n; ++i) {
        dist[i] = INF;
        prev[i] = -1;
    }
    dist[src] = 0;

    for (int iter = 0; iter < n; ++iter) {
        int u = -1;
        int best = INF;
        for (int i = 0; i < n; ++i) {
            if (!visited[i] && dist[i] < best) {
                best = dist[i];
                u = i;
            }
        }
        if (u == -1) break;
        visited[u] = 1;

        for (int v = 0; v < n; ++v) {
            int w = graph[u * n + v];
            if (w >= 0 && !visited[v]) {
                long alt = (long)dist[u] + w;
                if (alt < dist[v]) {
                    dist[v] = (int)alt;
                    prev[v] = u;
                }
            }
        }
    }

    free(visited);
}

void print_path(int *prev, int src, int target) {
    if (target == src) {
        printf("%d", src);
    } else if (prev[target] == -1) {
        printf("inacessível");
    } else {
        print_path(prev, src, prev[target]);
        printf(" -> %d", target);
    }
}