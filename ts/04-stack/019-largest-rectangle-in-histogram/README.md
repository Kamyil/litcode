# 019 · Largest Rectangle in Histogram

> **Difficulty:** Hard · **Topics:** monotonic stack · **Target:** O(n)

Bars of width 1 with heights `heights`. Return the area of the largest rectangle that fits
entirely inside the histogram.

```
heights = [2, 1, 5, 6, 2, 3]  ->  10   (bars 5 and 6, height 5, width 2)
```

## Think about

- For each bar, the best rectangle *using that bar's full height* extends left and right
  until a shorter bar. Monotonic increasing stack finds both boundaries.
- When you pop bar `j` because bar `i` is shorter: right boundary is `i`, left boundary is
  the new stack top. Width = `i - newTop - 1`.
- A sentinel bar of height 0 at the end flushes the stack.
