# 007 · Longest Consecutive Sequence

> **Difficulty:** Medium · **Topics:** hash set · **Target:** O(n) time

Given an unsorted array, return the length of the longest run of consecutive integers
(`[100, 4, 200, 1, 3, 2]` contains `1, 2, 3, 4` -> `4`). Must be O(n): sorting is not allowed
in the intended solution (the test uses a large input to nudge you).

## Examples

```
[100, 4, 200, 1, 3, 2]          ->  4
[0, 3, 7, 2, 5, 8, 4, 6, 0, 1]  ->  9
```

## Think about

- Only start counting from a number that is the **start** of a run. How do you know it is
  a start? Why does that make the whole thing O(n) and not O(n²)?
