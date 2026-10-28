# 258 · Answers

**Q1.** The B-tree is tens of MB (an entry per row). BRIN is a few dozen **kB**. BRIN (Block Range INdex)
stores a summary, the min and max of the column, for each range of 128 heap pages (`pages_per_range`). A
query skips every range whose [min, max] can't match and scans the remaining ranges' pages entirely
(a lossy Bitmap Heap Scan with `Rows Removed by Index Recheck`). It reads more heap pages than a
B-tree, but it's ~1000× smaller and nearly free to maintain.

**Q2.** If values are spread randomly, every block range's [min, max] covers almost the whole domain, so
nothing can be skipped and BRIN degenerates into a full scan. BRIN only works when the physical order
follows the column: append-only logs, time series, sequential ids. Updates and out-of-order inserts erode it.

**Q3.** The generated events start on 2025-01-05 and run into June 2025, and there's no June partition,
so June rows land in `DEFAULT`. When you later `CREATE TABLE ev_2025_06 PARTITION OF ...`, Postgres must
scan the default partition (under a lock) to verify none of its rows belong in the new partition, and it fails if
some do. You'd have to move them first. Many teams avoid DEFAULT partitions and pre-create future
partitions instead (`pg_partman` automates this).

**Q4.** The date-range query scans only `ev_2025_03`, pruned at **plan time** from constants. The
`customer_id` query scans all partitions. The prepared statement with a parameter may use a generic plan:
the partitions are then pruned at **execution time** (`Subplans Removed: N`) once `$1` is known. Run-time pruning
also happens for values coming from a nested loop's outer side.

**Q5.** Partitioning only helps queries that filter on the **partition key**. Choose it from the dominant
access pattern (time for logs and events, tenant for multi-tenant apps). Queries without the key pay a
small extra cost: scanning N partitions plus planning overhead.

**Q6.** The DELETE took seconds, wrote WAL for every row, and left ~20% of the table as dead tuples
for VACUUM to clean (bloat, and index entries to remove). DETACH + DROP is a metadata change: milliseconds,
almost no WAL, no vacuum debt. That's the strongest reason to partition time-series data.

**Q7.** (1) Primary keys and unique constraints must include the partition key, so a global uniqueness
constraint on `id` alone is impossible. (2) Thousands of partitions slow planning and use more memory
and locks. (3) Foreign keys referencing partitioned tables have restrictions (supported since PG12, with caveats).
(4) Rows that change partition-key values move between partitions (a delete plus an insert under the hood).
(5) Operational overhead: creating future partitions, monitoring the default partition.
