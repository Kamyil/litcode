Part one: a `Set` — if `set.has(x)` before you add it, you found a duplicate.
---
Part two: keep a `Set` holding only the last `k` elements. When the window grows past `k`,
delete `nums[i - k - 1]`... careful with the off-by-one: the window is indices `[i-k, i-1]`.
