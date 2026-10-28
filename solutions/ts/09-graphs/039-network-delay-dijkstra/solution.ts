import { Heap } from "../../08-heaps/033-binary-heap/solution";

function dijkstra(n: number, times: [number, number, number][], src: number) {
  const adj: [number, number][][] = Array.from({ length: n + 1 }, () => []);
  for (const [u, v, w] of times) adj[u].push([v, w]);
  const dist = new Array<number>(n + 1).fill(Infinity);
  const prev = new Array<number>(n + 1).fill(-1);
  dist[src] = 0;
  const heap = new Heap<[number, number]>((a, b) => a[0] - b[0]);
  heap.push([0, src]);
  while (heap.size) {
    const [d, u] = heap.pop()!;
    if (d > dist[u]) continue; // stale entry
    for (const [v, w] of adj[u]) {
      if (d + w < dist[v]) {
        dist[v] = d + w;
        prev[v] = u;
        heap.push([dist[v], v]);
      }
    }
  }
  return { dist, prev };
}

export function networkDelayTime(times: [number, number, number][], n: number, k: number): number {
  const { dist } = dijkstra(n, times, k);
  const worst = Math.max(...dist.slice(1));
  return worst === Infinity ? -1 : worst;
}

export function shortestPath(n: number, times: [number, number, number][], src: number, dst: number): number[] {
  const { dist, prev } = dijkstra(n, times, src);
  if (dist[dst] === Infinity) return [];
  const path: number[] = [];
  for (let v = dst; v !== -1; v = prev[v]) path.push(v);
  return path.reverse();
}
