# 023 · Median of Two Sorted Arrays

> **Difficulty:** Hard · **Topics:** binary search on partitions · **Target:** O(log(min(m, n)))

Given two sorted arrays `a` and `b`, return the median of their union. The O(m + n) merge is
easy; the interview answer is logarithmic.

## Examples

```
a = [1, 3],    b = [2]     ->  2
a = [1, 2],    b = [3, 4]  ->  2.5
```

## Think about

- Cut `a` at `i` and `b` at `j` such that the left parts together hold half the elements
  (`i + j = (m + n + 1) / 2`). The cut is correct iff `a[i-1] <= b[j]` and `b[j-1] <= a[i]`.
- Binary search `i` over the **shorter** array. Use `-Infinity` / `+Infinity` for out-of-range.
