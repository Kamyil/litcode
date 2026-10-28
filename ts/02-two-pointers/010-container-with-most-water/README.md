# 010 · Container With Most Water

> **Difficulty:** Medium · **Topics:** two pointers, greedy proof · **Target:** O(n)

`height[i]` is the height of a vertical line at x = i. Pick two lines that, together with
the x-axis, hold the most water. Return that area: `min(h[i], h[j]) * (j - i)`.

## Examples

```
[1, 8, 6, 2, 5, 4, 8, 3, 7]  ->  49   (lines at 1 and 8: min(8,7) * 7)
[1, 1]                       ->  1
```

## Think about

- Start with the widest container. Which pointer should move, and **why is it safe** to
  discard the other option? Be ready to argue this in an interview — it is the whole question.
