export function findOrder(n: number, prereqs: [number, number][]): number[] {
  const adj: number[][] = Array.from({ length: n }, () => []);
  const indeg = new Array<number>(n).fill(0);
  for (const [a, b] of prereqs) {
    adj[b].push(a);
    indeg[a]++;
  }
  const order: number[] = [];
  for (let c = 0; c < n; c++) if (indeg[c] === 0) order.push(c);
  // `order` doubles as the BFS queue
  for (let i = 0; i < order.length; i++) {
    for (const next of adj[order[i]]) if (--indeg[next] === 0) order.push(next);
  }
  return order.length === n ? order : [];
}

export function canFinish(n: number, prereqs: [number, number][]): boolean {
  return findOrder(n, prereqs).length === n;
}
