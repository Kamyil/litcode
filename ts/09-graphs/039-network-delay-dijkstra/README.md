# 039 · Network Delay Time (Dijkstra)

> **Difficulty:** Medium · **Topics:** weighted shortest path, heap · **Target:** O(E log V)

`times[i] = [u, v, w]`: a directed edge from node `u` to `v` taking `w` ms (`w >= 0`). Nodes are
`1..n`. A signal is sent from node `k`. Return the time until **all** nodes have received it,
or `-1` if some node is unreachable.

Also implement `shortestPath(n, times, src, dst)`: return the actual path as a list of nodes
(e.g. `[2, 1, 4]`), or `[]` if unreachable. If several shortest paths exist, any is fine.

Import your heap from `../../08-heaps/033-binary-heap/solution`.

## Think about

- Dijkstra = BFS where the queue is a priority queue keyed by distance. With a binary heap
  you can't decrease-key cheaply, so push duplicates and skip stale entries on pop.
- Why does it break with negative edges? (Bellman-Ford handles them, O(VE).)
- The Postgres planner's join search is also a shortest-path-ish search over a huge space —
  for many tables it switches from exhaustive DP to a genetic algorithm (`geqo_threshold`).
