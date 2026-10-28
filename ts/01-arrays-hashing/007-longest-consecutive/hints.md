Put everything in a `Set`.
---
`x` starts a run iff `!set.has(x - 1)`. From each start, walk `x+1, x+2, ...` while in the set.
---
Every element is visited by at most one walk, so the total work is O(n).
