# 251 · Answers

**Q1.** `cost=startup..total`. Startup = cost before the first row can be returned (e.g. a Sort
must consume all input first, so its startup cost is high). Total = cost to return all rows. The unit
is arbitrary "cost units", anchored by `seq_page_cost = 1.0` (one sequential page read).

**Q2.** From statistics gathered by `ANALYZE` (autovacuum runs it): `pg_stats` has per-column
`n_distinct`, most-common values + frequencies, and a histogram. For `customer_id = 42` the estimate is
roughly `reltuples / n_distinct`. But `n_distinct` is itself *estimated from a sample* (30k rows by
default), and for a column with ~200k distinct values it comes out far too low (a few thousand), hence ~200
instead of the real ~5. Statistics are approximations. Lab 257 shows how to improve them.

**Q3.** Off by ~40× here (200 estimated vs ~5 actual). Harmless for this query, but bad estimates matter because the planner picks join algorithms, join
order and scan types from them. An underestimate of 5 rows when there are 500k makes it choose a nested
loop that runs 500k index probes. *Most* slow-query investigations start with "estimate vs actual
differ by orders of magnitude at node X".

**Q4.** `hit` = page was found in Postgres's shared buffers; `read` = had to ask the OS (which may
still serve it from its page cache, so "read" isn't necessarily disk). Second run: mostly hits, because
the first run loaded the pages. Note: big seq scans use a small **ring buffer** (256 KB) so they don't
evict the whole cache — another reason you may still see reads.

**Q5.** About 18k pages for 1M rows (~55 rows per page). A seq scan touches every page: hit + read ≈ relpages.

**Q6.** `relpages + reltuples * (0.01 + 0.0025)` — e.g. 18,090 + 1,000,000 × 0.0125 = 30,590. It
should match `EXPLAIN`'s total cost exactly (with parallelism off; a parallel plan divides the CPU part across workers).

**Q7.** That a random page read costs 4× a sequential one, which was true for spinning disks (and is
discounted for caching). On SSDs random and sequential reads are close, so 1.1 makes the planner
pick index scans more readily. Wrong settings here are a classic cause of "Postgres ignores my index".

**Q8.** `Index Scan using orders_customer_id_idx`. A handful of buffers (3–4 index pages from root to
leaf, plus ~1 heap page per matching row) versus ~18,000 for the seq scan.

**Q9.** With ~150 matching rows scattered across many heap pages, Postgres may use a **Bitmap Index
Scan** to collect all matching TIDs first, sort them by page, then a **Bitmap Heap Scan** reads each
heap page once in physical order. It sits between "few rows → index scan" and "many rows → seq scan".

**Q10.** A bounded heap of size N (here 10): stream all rows, keep only the best 10 seen so far.
O(n log k) time and O(k) memory instead of O(n log n) and memory for all rows.

**Q11.** The table is big enough for a **parallel** plan: workers each scan part of the table and
pre-aggregate (Partial HashAggregate), `Gather` collects their partial results, and a Finalize
aggregate combines them. Controlled by `max_parallel_workers_per_gather` and the table size.
