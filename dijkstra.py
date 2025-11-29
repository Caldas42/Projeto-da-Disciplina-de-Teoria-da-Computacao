from typing import Dict, List, Tuple, Any, Optional
import random
import time
import statistics

Node = Any
Edge = Tuple[Node, float]
Graph = Dict[Node, List[Edge]]
INF = float('inf')

def dijkstra_simple(graph: Graph, source: Node) -> Tuple[Dict[Node, float], Dict[Node, Optional[Node]]]:
    if source not in graph:
        raise KeyError("Fonte (source) não está presente no grafo.")

    nodes = list(graph.keys())
    dist: Dict[Node, float] = {v: INF for v in nodes}
    prev: Dict[Node, Optional[Node]] = {v: None for v in nodes}
    visited: Dict[Node, bool] = {v: False for v in nodes}

    dist[source] = 0.0

    for _ in range(len(nodes)):
        u = None
        best = INF
        for v in nodes:
            if not visited[v] and dist[v] < best:
                best = dist[v]
                u = v
        if u is None:
            break

        visited[u] = True

        for v, w in graph[u]:
            if w < 0:
                raise ValueError("Dijkstra simples não suporta pesos negativos.")
            if not visited[v]:
                nd = dist[u] + w
                if nd < dist[v]:
                    dist[v] = nd
                    prev[v] = u

    return dist, prev

def gerar_grafo(n: int, densidade: float = 0.3) -> Graph:
    graph: Graph = {}
    nodes = [f"V{i}" for i in range(n)]

    for u in nodes:
        graph[u] = []
        for v in nodes:
            if u != v and random.random() < densidade:
                peso = random.randint(1, 10)
                graph[u].append((v, peso))

    return graph

def medir_tempo(graph: Graph, source: Node, repeticoes: int = 20) -> List[float]:
    tempos = []
    for _ in range(repeticoes):
        t0 = time.perf_counter()
        dijkstra_simple(graph, source)
        t1 = time.perf_counter()
        tempos.append(t1 - t0)
    return tempos

if __name__ == "__main__":
    random.seed(0)

    tamanhos = {
        "pequeno": 200,
        "medio": 400,
        "grande": 1000
    }

    repeticoes = 20 

    for nome, n in tamanhos.items():
        print(f"\nRodando grafo {nome} (n = {n}) {repeticoes} vezes...")

        G = gerar_grafo(n)
        source = "V0"

        tempos = medir_tempo(G, source, repeticoes)

        media = statistics.mean(tempos)
        stdev = statistics.stdev(tempos) if len(tempos) > 1 else 0.0

        print(f"  Tempo médio : {media:.6f} s")
        print(f"  Desvio-padrão: {stdev:.6f} s")
