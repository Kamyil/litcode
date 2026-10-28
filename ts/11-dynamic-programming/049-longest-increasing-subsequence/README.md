# 049 · Longest Increasing Subsequence

> **Difficulty:** Medium · **Topics:** DP, patience sorting + binary search · **Target:** O(n log n)

Return the length of the longest **strictly** increasing subsequence.

```
[10, 9, 2, 5, 3, 7, 101, 18]  ->  4   ([2, 3, 7, 101])
```

Then `lisSequence(nums)`: return one actual LIS (any one, the test validates it).

## Think about

- O(n²): `dp[i]` = LIS ending at `i` = `1 + max(dp[j])` over `j < i` with `nums[j] < nums[i]`.
- O(n log n): keep `tails[k]` = smallest possible tail of an increasing subsequence of length
  `k + 1`. For each `x`, `lowerBound(tails, x)` (from 020!) tells you where it goes.
- `tails` is **not** an LIS itself — a common misconception. To reconstruct, store parent pointers.
