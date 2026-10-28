`dp[a]` = fewest coins for amount `a`; `dp[0] = 0`, everything else `Infinity`.
`dp[a] = min over coins c <= a of dp[a - c] + 1`.
---
Ways: `ways[0] = 1`; `for c of coins: for a from c to amount: ways[a] += ways[a - c]`.
