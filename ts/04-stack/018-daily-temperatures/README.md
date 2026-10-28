# 018 · Daily Temperatures

> **Difficulty:** Medium · **Topics:** monotonic stack · **Target:** O(n)

For each day, return how many days you have to wait until a warmer temperature. `0` if
there is none.

## Examples

```
[73, 74, 75, 71, 69, 72, 76, 73]  ->  [1, 1, 4, 2, 1, 1, 0, 0]
[30, 40, 50, 60]                  ->  [1, 1, 1, 0]
```

## Think about

- **Monotonic stack**: keep indices of days still waiting for a warmer day; their
  temperatures are decreasing from bottom to top. A new warmer day resolves several of them.
- This pattern = "next greater element". It shows up in 019, stock span, and trapping rain water.
