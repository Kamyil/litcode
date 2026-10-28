# 257 · Answers

**Q1.**
- **Nested Loop**: for each outer row, look up matching inner rows (ideally an index scan). Cost ≈
  outer_rows × inner_lookup. Chosen when the outer side is small and the inner side has an index. It's the
  only algorithm for non-equality joins (`<`, `&&`, `LIKE`...).
- **Hash Join**: build a hash table on the smaller input (by join key), then probe it with each row
  of the larger input. O(n + m), needs memory (`work_mem` × `hash_mem_multiplier`), equality joins only.
  Chosen for large unsorted inputs.
- **Merge Join**: both inputs sorted by the join key (from an index or an explicit sort), then walk them
  together like merging two sorted lists. Great when the inputs are already ordered (here `orders.id` and
  `order_items (order_id, ...)` PK), and it handles huge inputs without a big hash table.

**Q2.** Without hash joins it typically falls back to a merge join with big sorts, several times slower.
The `enable_*` flags don't truly disable anything. They add a huge cost penalty, so they're for
*diagnosing* planner choices ("is the alternative actually faster?"). In production, fix the root cause
(statistics, indexes, cost settings) instead. Global flags affect every query, and data changes over time.

**Q3.** 64 kB holds a few hundred rows, so on the order of thousands of runs, merged in multiple passes.
`external merge Disk: ~60MB` vs `quicksort Memory: ~100MB`, and the in-memory sort is several
times faster. (Postgres limits the merge fan-in by memory, so with tiny `work_mem` it needs multiple merge passes.)

**Q4.** The hash join partitions both inputs into **batches** by hash value and writes all but the first
batch to temp files, then processes one batch at a time (a "Grace hash join"). `Batches: 1` means
everything fit in memory. Many batches mean extra I/O. A badly underestimated build side can also cause
batch explosions.

**Q5.** One query can use several multiples of `work_mem` (each sort and hash node, times parallel
workers), so 200 connections × several × 1 GB can exceed RAM and trigger the OOM killer. Keep the global
setting modest (e.g. 16–64 MB) and raise it per session or role for heavy jobs:
`SET work_mem = '512MB'` inside the reporting transaction, or `ALTER ROLE analytics SET work_mem = ...`.
Use `log_temp_files` to find spilling queries.

**Q6.** The planner multiplies the selectivities: P(city = Warsaw) × P(country = PL) = 1/6 × 1/3 = 1/18
of the rows, ≈ 16.7k. In reality city implies country, so it's 1/6 ≈ 50k, 3× more. It assumes columns are independent.

**Q7.** With `dependencies`/`mcv` statistics the estimate becomes ≈ 50k, and `ndistinct` fixes the
GROUP BY estimate (6 groups, not 18). A 3× error at one node compounds across joins: underestimating
a join input by 1000× makes the planner pick a nested loop that executes the inner side a million times instead
of a hash join. Misestimates cause most catastrophic plans.

**Q8.** It believed the table had ~1,000 rows with `v` in 0..9, so `v = 42` would match ~0 rows. Fine
here, but in a join it would choose nested loops over 500k rows. Autovacuum's analyze threshold
eventually catches up. Run `ANALYZE` manually after bulk loads, after big deletes or updates, and after
creating expression indexes or extended statistics, before running heavy queries.

**Q9.** How many values `ANALYZE` samples and keeps per column (MCV list size and histogram buckets,
default 100, sampling 300 × target rows). Raise it for a column with skewed, high-cardinality data where
estimates are poor: `ALTER TABLE t ALTER COLUMN c SET STATISTICS 1000; ANALYZE t;`.
