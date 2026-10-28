# 006 · Product of Array Except Self

> **Difficulty:** Medium · **Topics:** prefix/suffix arrays · **Target:** O(n) time, O(1) extra space

Return an array `out` where `out[i]` is the product of every element of `nums` except
`nums[i]`. **Do not use division.**

## Examples

```
[1, 2, 3, 4]       ->  [24, 12, 8, 6]
[-1, 1, 0, -3, 3]  ->  [0, 0, 9, 0, 0]
```

## Think about

- `out[i] = (product of everything left of i) * (product of everything right of i)`.
- Prefix sums/products are one of the most reused tricks: range-sum queries, running
  totals, and SQL window functions (`sum(x) OVER (ORDER BY ...)`) are the same idea.
- "O(1) extra space" means the output array doesn't count.
