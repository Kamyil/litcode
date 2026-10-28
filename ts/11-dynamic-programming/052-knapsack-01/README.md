# 052 · 0/1 Knapsack & Partition Equal Subset Sum

> **Difficulty:** Medium · **Topics:** knapsack DP · **Target:** O(n · capacity)

- `knapsack(weights, values, capacity)`: each item at most once; maximize total value with
  total weight `<= capacity`.
- `canPartition(nums)`: can `nums` be split into two subsets with equal sums?

## Think about

- `dp[c]` = best value with capacity `c`. For 0/1 (each item once), iterate `c` **downwards**
  so an item isn't reused in the same round. Upwards = unbounded knapsack (048!).
- `canPartition` is knapsack in disguise: can you reach exactly `sum / 2`? (boolean DP / bitset)
- Knapsack is NP-hard, yet this is "polynomial"? It's *pseudo-polynomial*: O(n · capacity),
  exponential in the number of **bits** of capacity. Good trivia for interviews.
