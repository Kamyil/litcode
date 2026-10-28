`used = Array(n).fill(false)`; at each depth try every unused index.
---
Unique: sort, and skip `i` if `nums[i] === nums[i-1] && !used[i-1]` (use equal values left to right only).
---
nextPermutation: find largest `i` with `a[i] < a[i+1]`. If none, reverse all. Else find largest
`j > i` with `a[j] > a[i]`, swap, reverse `a[i+1..]`.
