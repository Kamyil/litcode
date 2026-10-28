# 047 · House Robber I & II

> **Difficulty:** Medium · **Topics:** 1-D DP · **Target:** O(n), O(1) space

- `rob(nums)`: houses in a line; you can't rob two adjacent houses. Maximize the loot.
- `robCircle(nums)`: houses in a circle (first and last are adjacent).

## Examples

```
rob([2, 7, 9, 3, 1])     -> 12   (2 + 9 + 1)
robCircle([2, 3, 2])     -> 3
robCircle([1, 2, 3, 1])  -> 4
```

## Think about

- State: `best(i)` = max loot from the first `i` houses. Choice at house `i`: skip it
  (`best(i-1)`) or rob it (`best(i-2) + nums[i]`).
- Circle: you can't use both ends, so it's `max(rob(without first), rob(without last))`.
  Reducing a new problem to a solved one is a big interview skill.
