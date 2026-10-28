# 005 · Top K Frequent Elements

> **Difficulty:** Medium · **Topics:** counting, bucket sort, heap · **Target:** O(n)

Return the `k` most frequent elements of `nums`, ordered from most to least frequent.
The answer is guaranteed to be unique (no ties at the cut-off, and the tests only use
inputs where the order is unambiguous).

## Examples

```
nums = [1,1,1,2,2,3], k = 2  ->  [1, 2]
nums = [1],           k = 1  ->  [1]
```

## Think about

- Sorting the counts is O(n log n). A min-heap of size k is O(n log k).
- **Bucket sort** is O(n): a frequency is at most `n`, so index buckets by frequency.
- Postgres connection: `SELECT x, count(*) FROM t GROUP BY x ORDER BY 2 DESC LIMIT k`
  uses a HashAggregate, then a *top-N heapsort* — the planner literally does the heap trick.
