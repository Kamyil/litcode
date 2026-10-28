# 009 · 3Sum

> **Difficulty:** Medium · **Topics:** sorting, two pointers · **Target:** O(n²)

Return all **unique** triplets `[a, b, c]` from `nums` with `a + b + c === 0`. Each triplet
must be sorted ascending; the list of triplets may be in any order (tests sort it).

## Examples

```
[-1, 0, 1, 2, -1, -4]  ->  [[-1, -1, 2], [-1, 0, 1]]
[0, 1, 1]              ->  []
[0, 0, 0, 0]           ->  [[0, 0, 0]]
```

## Think about

- Sort first. Then for each `i`, the problem becomes **two-sum on a sorted array**, which
  two pointers solve in O(n) and O(1) space.
- The hard part is skipping duplicates without a Set. Where do you skip?
