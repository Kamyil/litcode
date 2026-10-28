# 044 · Combination Sum

> **Difficulty:** Medium · **Topics:** backtracking with pruning

- `combinationSum(candidates, target)`: distinct positive candidates, each may be used
  **unlimited** times. Return all unique combinations that sum to `target`.
- `combinationSum2(candidates, target)`: candidates may contain duplicates, each element used
  **at most once**. No duplicate combinations.

Order doesn't matter (tests normalize).

## Examples

```
combinationSum([2,3,6,7], 7)          -> [[2,2,3],[7]]
combinationSum2([10,1,2,7,6,1,5], 8)  -> [[1,1,6],[1,2,5],[1,7],[2,6]]
```

## Think about

- Sort candidates so you can **prune**: once `candidate > remaining`, every later one is too big — `break`.
- The difference between the two variants is one character: recurse with `i` (reuse allowed)
  vs `i + 1`.
