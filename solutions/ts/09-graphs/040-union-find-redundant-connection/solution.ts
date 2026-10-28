export class UnionFind {
  count: number;
  private parent: Int32Array;
  private size: Int32Array;

  constructor(n: number) {
    this.count = n;
    this.parent = new Int32Array(n).map((_, i) => i);
    this.size = new Int32Array(n).fill(1);
  }

  find(x: number): number {
    let root = x;
    while (this.parent[root] !== root) root = this.parent[root];
    while (this.parent[x] !== root) {
      const next = this.parent[x];
      this.parent[x] = root;
      x = next;
    }
    return root;
  }

  union(a: number, b: number): boolean {
    let ra = this.find(a);
    let rb = this.find(b);
    if (ra === rb) return false;
    if (this.size[ra] < this.size[rb]) [ra, rb] = [rb, ra];
    this.parent[rb] = ra;
    this.size[ra] += this.size[rb];
    this.count--;
    return true;
  }

  connected(a: number, b: number): boolean {
    return this.find(a) === this.find(b);
  }
}

export function countComponents(n: number, edges: [number, number][]): number {
  const uf = new UnionFind(n);
  for (const [a, b] of edges) uf.union(a, b);
  return uf.count;
}

export function findRedundantConnection(edges: [number, number][]): [number, number] {
  const uf = new UnionFind(edges.length + 1);
  for (const [a, b] of edges) if (!uf.union(a, b)) return [a, b];
  throw new Error("no redundant edge");
}
