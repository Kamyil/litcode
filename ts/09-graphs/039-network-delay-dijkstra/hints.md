`dist = Array(n + 1).fill(Infinity); dist[k] = 0;` heap of `[distance, node]`.
---
Pop `[d, u]`; if `d > dist[u]` it's stale, skip. Otherwise relax each edge `u -> v`: if
`d + w < dist[v]`, update and push.
---
For the path, keep `prev[v] = u` whenever you relax, then walk back from `dst`.
