`a = 1, b = 1` (ways to reach step 0 and step 1). Repeat `n - 1` times: `[a, b] = [b, a + b]`.
---
minCost: `dp[i] = cost[i] + min(dp[i-1], dp[i-2])`; answer `min(dp[n-1], dp[n-2])`.
