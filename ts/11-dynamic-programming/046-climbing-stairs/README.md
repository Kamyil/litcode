# 046 · Climbing Stairs (intro to DP)

> **Difficulty:** Easy · **Topics:** recursion -> memoization -> tabulation -> O(1) space

You climb `n` steps, taking 1 or 2 steps at a time. In how many distinct ways can you reach
the top? Then `minCostClimbingStairs(cost)`: step `i` costs `cost[i]` to leave; start at step
0 or 1; reach the top (index `n`) at minimum cost.

Write `climbStairs` **four times** in your head (or in the file, as helpers):
1. naive recursion (watch it die at n = 45),
2. memoized recursion (top-down DP),
3. table (bottom-up DP),
4. two variables (O(1) space).

That progression *is* dynamic programming. Every DP problem after this is "find the recurrence".

## Think about

- `ways(n) = ways(n-1) + ways(n-2)` — Fibonacci. Why is the naive version O(2ⁿ)?
- Results exceed 2^53 around n = 78. The tests stop at n = 70; for bigger n you'd need `BigInt`.
