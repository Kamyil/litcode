# 002 · Contains Duplicate

> **Difficulty:** Easy · **Topics:** hash set, sorting · **Target:** O(n) time

Return `true` if any value appears at least twice in `nums`, otherwise `false`.

Then implement a second version, `containsNearbyDuplicate(nums, k)`: return `true` if there
are two **distinct indices** `i` and `j` with `nums[i] === nums[j]` and `|i - j| <= k`.

## Examples

```
containsDuplicate([1, 2, 3, 1])            -> true
containsDuplicate([1, 2, 3, 4])            -> false
containsNearbyDuplicate([1, 2, 3, 1], 3)   -> true
containsNearbyDuplicate([1, 2, 3, 1, 2, 3], 2) -> false
```

## Think about

- Three approaches for part one: O(n²) compare, O(n log n) sort, O(n) set. What does each cost
  in **memory**? When would you pick the sort version? (Hint: memory-constrained, or data
  already sorted — this is exactly the trade-off Postgres makes between hash and sort plans.)
- Part two is a *sliding window* over a set of size `k`.
