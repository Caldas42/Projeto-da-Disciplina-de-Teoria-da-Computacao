from typing import Dict, List, Tuple, Any, Optional

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
                raise ValueError("Dijkstra simples não suporta arestas com peso negativo.")
            if not visited[v]:
                nd = dist[u] + w
                if nd < dist[v]:
                    dist[v] = nd
                    prev[v] = u

    return dist, prev

def reconstruct_path(prev: Dict[Node, Optional[Node]], source: Node, target: Node) -> List[Node]:
    if target not in prev:
        return []
    path = []
    cur = target
    while cur is not None:
        path.append(cur)
        if cur == source:
            break
        cur = prev[cur]
    path.reverse()
    return path if path and path[0] == source else []

if __name__ == "__main__":
    G: Graph = {
        'A': [('B', 2), ('C', 5)],
        'B': [('C', 1), ('D', 4)],
        'C': [('D', 2)],
        'D': [('E', 1)],
        'E': []
    }

    dist, prev = dijkstra_simple(G, 'A')
    print("Distâncias a partir de A:")
    for v in sorted(G.keys()):
        d = dist[v]
        print(f"  A -> {v}: {d if d != INF else 'inacessível'}")
    print("Caminho A -> E:", reconstruct_path(prev, 'A', 'E'))
