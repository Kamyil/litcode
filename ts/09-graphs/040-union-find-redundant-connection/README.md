# 040 · Union-Find: Redundant Connection & Connected Components

> **Difficulty:** Medium · **Topics:** disjoint set union · **Target:** ~O(α(n)) per operation

Implement a `UnionFind(n)` class with `find(x)`, `union(a, b)` (returns `false` if already
connected), `connected(a, b)` and a `count` of components. Use **path compression** and
**union by rank/size**.

Then use it:
- `countComponents(n, edges)`: number of connected components in an undirected graph.
- `findRedundantConnection(edges)`: nodes `1..n`, the edges form a tree plus one extra edge.
  Return the extra edge (if multiple answers, the one that appears **last** in the input).

## Think about

- Without the two optimizations, `find` can degrade to O(n). With both: inverse Ackermann —
  effectively constant.
- Kruskal's MST is "sort edges + union-find". Great follow-up question.
