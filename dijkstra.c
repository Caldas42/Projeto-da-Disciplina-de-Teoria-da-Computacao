#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <time.h>
#include <math.h>

#define INF (INT_MAX / 4)

int *gerar_grafo(int n, double density);
double get_time_seconds(void);
void medir_tempo(int n, int *graph, int repeticoes, double *out_times);
void estatistica(int repeticoes, const double *times, double *mean_out, double *stdev_out);
void dijkstra(int n, int *graph, int src, int *dist, int *prev);

int main(void) {
    srand(0);

    const int repeticoes = 20;
    struct { const char *name; int n; double density; } casos[] = {
        {"pequeno", 200,  0.3},
        {"medio",  400, 0.3},
        {"grande", 1000, 0.3}
    };
    const int ncasos = sizeof(casos) / sizeof(casos[0]);

    for (int ci = 0; ci < ncasos; ++ci) {
        const char *nome = casos[ci].name;
        int n = casos[ci].n;
        double density = casos[ci].density;

        printf("\nRodando grafo %s (n = %d, density = %.2f) %d vezes...\n", nome, n, density, repeticoes);

        int *G = gerar_grafo(n, density);
        if (!G) { fprintf(stderr, "Erro ao gerar grafo %s\n", nome); return 1; }

        double *tempos = (double*)malloc(repeticoes * sizeof(double));
        if (!tempos) { perror("malloc"); free(G); return 1; }

        medir_tempo(n, G, repeticoes, tempos);

        double media = 0.0, stdev = 0.0;
        estatistica(repeticoes, tempos, &media, &stdev);

        printf("  Tempo medio : %.6f s\n", media);
        printf("  Desvio-padrao: %.6f s\n", stdev);

        free(tempos);
        free(G);
    }

    printf("\n");
    return 0;
}

int *gerar_grafo(int n, double density) {
    int *graph = (int*)malloc(n * n * sizeof(int));
    if (!graph) { perror("malloc"); return NULL; }

    for (int i = 0; i < n * n; ++i) graph[i] = -1;

    for (int u = 0; u < n; ++u) {
        for (int v = 0; v < n; ++v) {
            if (u == v) continue;
            double r = rand() / (RAND_MAX + 1.0);
            if (r < density) {
                int peso = (rand() % 10) + 1;
                graph[u * n + v] = peso;
            }
        }
    }

    return graph;
}

double get_time_seconds(void) {
    return (double)clock() / CLOCKS_PER_SEC;
}

void medir_tempo(int n, int *graph, int repeticoes, double *out_times) {
    int *dist = (int*)malloc(n * sizeof(int));
    int *prev = (int*)malloc(n * sizeof(int));
    if (!dist || !prev) { perror("malloc"); exit(1); }

    for (int i = 0; i < repeticoes; ++i) {
        double t0 = get_time_seconds();
        dijkstra(n, graph, 0, dist, prev);
        double t1 = get_time_seconds();
        out_times[i] = t1 - t0;
    }

    free(dist);
    free(prev);
}

void estatistica(int repeticoes, const double *times, double *mean_out, double *stdev_out) {
    double sum = 0.0;
    for (int i = 0; i < repeticoes; ++i) sum += times[i];
    double mean = sum / (double)repeticoes;

    double s2 = 0.0;
    for (int i = 0; i < repeticoes; ++i) {
        double d = times[i] - mean;
        s2 += d * d;
    }
    double stdev = (repeticoes > 1) ? sqrt(s2 / (repeticoes - 1)) : 0.0;

    *mean_out = mean;
    *stdev_out = stdev;
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