# 048 · Coin Change

> **Difficulty:** Medium · **Topics:** unbounded knapsack DP · **Target:** O(amount · coins)

- `coinChange(coins, amount)`: fewest coins that sum to `amount` (unlimited coins of each
  denomination), or `-1` if impossible.
- `coinChangeWays(coins, amount)`: number of **combinations** (order doesn't matter) that
  sum to `amount`.

## Examples

```
coinChange([1, 2, 5], 11)       -> 3   (5 + 5 + 1)
coinChange([2], 3)              -> -1
coinChangeWays([1, 2, 5], 5)    -> 4   (5, 2+2+1, 2+1+1+1, 1+1+1+1+1)
```

## Think about

- Why does greedy (always take the biggest coin) fail? Find a counterexample with `[1, 3, 4]`.
- Ways: loop **coins outside, amounts inside** to count combinations. Swap the loops and you
  count permutations instead. Make sure you can explain why.
