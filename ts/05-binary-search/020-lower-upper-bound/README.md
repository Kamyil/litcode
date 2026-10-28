# 020 · Lower Bound / Upper Bound

> **Difficulty:** Easy · **Topics:** binary search, invariants · **Target:** O(log n)

Binary search is easy to describe and famously easy to get wrong (off-by-one, infinite loops,
overflow). Implement the two primitives everything else is built from, on a **sorted** array:

- `lowerBound(nums, x)`: the first index `i` with `nums[i] >= x` (or `nums.length`).
- `upperBound(nums, x)`: the first index `i` with `nums[i] > x` (or `nums.length`).
- `search(nums, x)`: index of `x` or `-1` (use `lowerBound`).
- `countInRange(nums, lo, hi)`: how many elements satisfy `lo <= v <= hi`.

## Examples

```
nums = [1, 2, 2, 2, 5, 7]
lowerBound(nums, 2) -> 1      upperBound(nums, 2) -> 4
lowerBound(nums, 3) -> 4      upperBound(nums, 9) -> 6
countInRange(nums, 2, 5) -> 4
```

## Think about

- Write down your **loop invariant** before coding. A clean one: the answer is always in
  `[lo, hi]` with half-open `hi = nums.length`. Loop `while (lo < hi)`.
- This is exactly what a B-tree does inside one page: binary search over sorted keys to find
  the child pointer (Postgres: `_bt_binsrch`). `countInRange` = an index range scan.
- In C, `(lo + hi) / 2` can overflow `int`. JS numbers won't, but know the `lo + (hi - lo) / 2` idiom.
