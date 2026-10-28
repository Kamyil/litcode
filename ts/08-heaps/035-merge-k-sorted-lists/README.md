# 035 · Merge K Sorted Lists

> **Difficulty:** Hard · **Topics:** heap, k-way merge · **Target:** O(N log k)

Merge `k` sorted linked lists into one sorted list (`N` = total nodes). Splice the existing
nodes.

## Think about

- A heap holding one node per list. Pop the smallest, push its `next`. This **k-way merge**
  is exactly how a database merges sorted runs that didn't fit in memory (sys/110, and in
  Postgres `tuplesort.c`'s "merge" phase — look for `mergeruns`). When `work_mem` is too small
  for an in-memory sort, `EXPLAIN ANALYZE` reports `Sort Method: external merge  Disk: ...`.
- Alternative: divide and conquer — merge pairs of lists (reuse 025). Same complexity.
