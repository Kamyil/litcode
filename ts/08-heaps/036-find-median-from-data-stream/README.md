# 036 · Find Median from Data Stream

> **Difficulty:** Hard · **Topics:** two heaps · **Target:** O(log n) add, O(1) median

Design `MedianFinder` with `addNum(x)` and `findMedian()` (average of the two middle values
when the count is even).

## Think about

- Two heaps: a **max-heap** of the lower half and a **min-heap** of the upper half, sizes
  differing by at most one. The median lives at the tops.
- Follow-ups: values only in `[0, 100]`? (counting array). 99% of values in `[0, 100]`?
- Postgres's `percentile_cont(0.5) WITHIN GROUP (ORDER BY x)` sorts everything instead — fine
  for a one-shot aggregate, but why wouldn't it work for a *streaming* median?
