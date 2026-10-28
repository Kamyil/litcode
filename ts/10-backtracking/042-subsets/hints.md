`backtrack(start, current)`: record a copy of `current`; for `i` from `start`: push `nums[i]`,
recurse with `i + 1`, pop.
---
With duplicates: sort; inside the loop, `if (i > start && nums[i] === nums[i - 1]) continue`.
