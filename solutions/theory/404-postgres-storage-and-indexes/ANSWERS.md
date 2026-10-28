# 404 · Answers

**1.** 8 KB. `ctid = (page number, line pointer)`, the tuple's physical address. It changes on every UPDATE
(a new version is written elsewhere), and after VACUUM FULL / CLUSTER rewrites, so never store it.

**2.** Values over ~2 KB (`TOAST_TUPLE_THRESHOLD`) are first compressed (pglz/lz4) and, if still too
big, moved out-of-line into the table's TOAST table in ~2 KB chunks. The main tuple keeps an 18-byte
pointer. Benefit: queries that don't select that column never read it, and updates of other
columns don't copy it (the TOAST pointer is reused).

**3.** Advantages: secondary indexes point directly at the heap (ctid) instead of storing the primary key and
needing a second lookup; inserts are cheap (any page with space); no page splits in the table itself.
Disadvantages: no physical ordering, so PK range scans are random I/O unless the data is correlated, and
updates create new versions that need index entries (unless HOT). `CLUSTER` rewrites the table once in index
order, but new inserts and updates don't maintain that order, so it decays.

**4.** B-tree: equality and range, sorting, the default. Hash: equality only, possibly smaller for long
keys. GIN: "contains" queries: jsonb `@>`, arrays, full-text, trigrams. GiST: geometric/range overlap,
nearest-neighbour (PostGIS, range types, exclusion constraints). SP-GiST: space-partitioned trees: quadtrees,
radix trees for text/IP prefixes. BRIN: huge, naturally ordered tables (timestamps in append-only logs).

**5.** No. Visibility (xmin/xmax) lives only in the heap tuple. Index scans must check the heap, except
index-only scans on all-visible pages (visibility map). That's why `count(*)` can't simply count index
entries. It must check visibility, so it's a (parallel) seq scan or an index-only scan with heap fetches.

**6.** (a) yes, leading column. (b) not efficiently, no leading column. (c) yes, equality on the first column,
then the index is ordered by the second. (d) yes, as a range on `last_name` (with C collation or
`text_pattern_ops`). (e) yes, B-trees scan backwards. (f) no, mixed directions don't match. You'd need an
index on `(last_name, first_name DESC)`.

**7.** Index scan: few rows, or ordered output wanted. Bitmap heap scan: a medium number of rows, or combining
several indexes (AND/OR); it reads each heap page once in physical order. Index-only scan: all needed columns
are in the index and pages are all-visible. Seq scan: a large fraction of the table, or tiny tables.

**8.** The whole table is a few pages. Reading them sequentially costs less than descending the index (2–3
pages) plus random heap access. The planner compares costs, and at that size seq scan wins or ties.

**9.** A primary key is UNIQUE + NOT NULL (one per table) and is implemented with a unique index. A UNIQUE
constraint is also implemented as a unique index, but as a constraint it can be referenced by foreign keys
and can be `DEFERRABLE`. A plain unique index can be partial or on expressions. NULLs are distinct by
default, so multiple NULLs are allowed. `UNIQUE NULLS NOT DISTINCT` treats NULLs as equal (only one allowed).

**10.** Postgres indexes the referenced side (it must be unique) but leaves the referencing side up to
you (not every workload needs it, and indexes cost writes). Without it, every `DELETE`/`UPDATE` of a parent row
seq-scans the child table to check for referencing rows (or to cascade), so deleting a customer scans 1M
orders, and joins from parent to children can't use an index. Index FK columns that you join on or whose parent rows you delete.

**11.** Random UUIDv4: inserts land on random leaf pages, so they cause page splits everywhere, a ~50–70% fill
factor (bigger index), poor cache locality (the whole index must stay hot), and more WAL (full-page writes
on many pages). bigint identity / UUIDv7 (time-ordered): inserts go to the rightmost leaf, giving dense pages and a
small hot set. UUIDs are also 16 bytes vs 8.

**12.** Index pages full of dead or deleted entries, or half-empty after splits and deletes, so the index is much bigger
than needed. Detect it with `pgstattuple`/`pgstatindex` (avg leaf density) or by comparing size with a fresh
build. Fix with `REINDEX INDEX CONCURRENTLY` (PG12+), or create a new index concurrently and drop the old one.

**13.** Stale statistics (run ANALYZE), correlated columns (`CREATE STATISTICS`), skewed data beyond the
MCV list (raise the statistics target), functions or expressions on columns (expression index + ANALYZE),
a generic plan with parameters for a prepared statement (`plan_cache_mode`), joins whose selectivity the
planner can't know, or bad `n_distinct` estimates (`ALTER TABLE ... ALTER COLUMN ... SET (n_distinct = ...)`).

**14.** Index-only scans skip the heap only for pages marked all-visible in the visibility map, and VACUUM sets
those bits. Without regular vacuuming, "index-only" scans still fetch most rows from the heap.
