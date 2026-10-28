`dp = Array(capacity + 1).fill(0)`; for each item `(w, v)`: for `c` from capacity down to `w`:
`dp[c] = max(dp[c], dp[c - w] + v)`.
---
canPartition: odd sum -> false. `reach[0] = true`; for each x: for `s` from target down to x:
`reach[s] ||= reach[s - x]`.
