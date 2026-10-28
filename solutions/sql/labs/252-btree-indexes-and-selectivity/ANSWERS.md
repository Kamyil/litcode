# 252 · Answers

**Q1.** ~0.1%: Index Scan. ~8%: Bitmap Heap Scan (or index scan thanks to correlation). ~67%: Seq Scan.
Rule of thumb: once you need more than a few percent of the table, reading it sequentially is
cheaper than random page access. The real crossover depends on correlation, `random_page_cost` and
how much is cached. There's no fixed number. Postgres compares estimated costs.

**Q2.** `correlation` ≈ 1.0 for `created_at` (and `id`) and ≈ 0 for `customer_id`. With high
correlation, consecutive index entries point to the same or neighboring heap pages, so a range scan
reads few pages, mostly sequentially. Uncorrelated ranges touch a different heap page for nearly
every row, so the planner switches to bitmap or seq scans much sooner.

**Q3.** Query 1: yes, it seeks to `('refunded', '2025-06-01')` and scans a contiguous range.
Query 3: yes, a leading-column prefix (`status = 'refunded'`). Query 2: not efficiently, because
`created_at` isn't a prefix, so matching entries are spread across every status group. (Postgres 18
added "skip scan" for some of these cases; on 17 you'd scan the whole index or the table.) Rule: a
B-tree on (a, b, c) serves conditions on a prefix: `a`, `a,b`, `a,b,c`, with equality on all but the last used column.

**Q4.** With `(status, created_at)` all matches sit in one contiguous index range. With
`(created_at, status)`, the range `created_at >= ...` includes all statuses, and `status` is checked
per entry (an index *filter*, not a seek), so far more index entries are read.

**Q5.** An Index Only Scan answers from the index without visiting the heap, but only for pages the
**visibility map** marks *all-visible*. For other pages it must check the heap tuple's visibility (MVCC
info lives in the heap, not the index): those are the `Heap Fetches`.

**Q6.** After `VACUUM`, the visibility map bits are set, so `Heap Fetches: 0` and far fewer buffers.
Lesson: index-only scans need a well-vacuumed table. A heavily updated table rarely gets them.

**Q7.** Key columns are part of the B-tree ordering and can be searched or sorted on. `INCLUDE`
columns are only stored in leaf entries as payload. They make index-only scans possible without
widening the key (and can't be used for seeking or ordering). Also, unique indexes enforce uniqueness only on key columns.

**Q8.** The partial index is ~8% of the size of a full one, and it's cheaper to maintain (inserts of
non-pending rows don't touch it). It's used only when the planner can prove the query's `WHERE`
implies the index predicate. Here it's literally `status = 'pending'`. A parameterized `status = $1`
generic plan can't use it.

**Q9.** The index is already ordered by `created_at`, so the scan returns rows in order and stops
after 20 (a `Limit` over an `Index Scan`). B-trees give you sorted output for free.

**Q10.** Typically 2–4× slower with 5 extra indexes. Every index must be updated on every INSERT,
and on every UPDATE that isn't HOT. Other costs: disk space and cache memory (indexes compete for
shared buffers), more WAL (replication lag, backups), longer VACUUM, and more work for the planner.
Unused indexes are pure cost, so find them with `pg_stat_user_indexes.idx_scan = 0`.
