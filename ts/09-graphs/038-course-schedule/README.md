# 038 · Course Schedule (topological sort)

> **Difficulty:** Medium · **Topics:** directed graph, cycle detection, topo sort · **Target:** O(V + E)

`n` courses `0..n-1`. `prereqs[i] = [a, b]` means you must take `b` before `a`.

- `canFinish(n, prereqs)`: is it possible to take all courses?
- `findOrder(n, prereqs)`: any valid order, or `[]` if impossible. (Tests validate the order.)

## Think about

- Impossible iff the graph has a cycle.
- **Kahn's algorithm** (BFS from in-degree-0 nodes) vs **DFS with three colors**
  (white/gray/black). Know both; Kahn's is easier to get right under pressure.
- Real-world topo sorts: build systems, `pg_dump` ordering objects by dependency
  (`pg_depend`), and Postgres's deadlock detector looks for cycles in the wait-for graph
  (`src/backend/storage/lmgr/deadlock.c`).
