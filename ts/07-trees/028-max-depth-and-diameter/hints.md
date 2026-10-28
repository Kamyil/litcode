`maxDepth = node ? 1 + max(maxDepth(left), maxDepth(right)) : 0`.
---
Diameter: inside a height() helper, `best = max(best, hl + hr)` before returning `1 + max(hl, hr)`.
---
Iterative: BFS level by level and count levels, or a stack of `[node, depth]` pairs.
