# 254 · Answers

**Q1.** `ctid` = physical location `(page, line pointer)`. `xmin` = id of the transaction that created
this version. `xmax` = id of the transaction that deleted or updated it (0 if still current). An UPDATE
inserts a new tuple version at a new slot and sets the old version's `xmax`, so the visible row's ctid changes.

**Q2.** Six line pointers: five current versions plus the dead old version of row 1 (until vacuum).
The old version has `t_xmax` = the updating transaction, and its `t_ctid` points to the new version's
ctid. That's the update chain (HOT chains are followed this way).

**Q3.** Same thing: `lower` = end of the line pointer array (24-byte header + 4 bytes per item in real
Postgres), `upper` = start of tuple space. Tuples grow down from `special` (= page size for heap pages).

**Q4.** A snapshot records which transactions were committed when it was taken (xmin/xmax/in-progress
list). A tuple version is visible if its `xmin` committed before the snapshot and its `xmax` is either
empty or not committed per the snapshot. Pane A's snapshot predates B's update, so for A the new
version's xmin is "in the future" and the old version's xmax is "not yet committed". VACUUM can only
remove versions that are dead to **every** running snapshot. One long-running transaction (or an idle
`BEGIN` someone forgot) holds back cleanup for the whole database. That's the #1 cause of bloat.
Monitor `backend_xmin` in `pg_stat_activity`.

**Q5.** VACUUM removes dead tuple versions, marks their line pointers unused (reusable), defragments
the page and updates the free space map and visibility map. The file usually does **not** shrink
(only empty pages at the very end can be truncated). Space is reused for future inserts and updates.

**Q6.** `VACUUM`: in place, concurrent with reads and writes (ShareUpdateExclusive lock), makes space
reusable. `VACUUM FULL`: rewrites the whole table into a new file, returning space to the OS, but takes
an **ACCESS EXCLUSIVE** lock (blocks even SELECTs) for the duration and needs extra disk. In
production use `pg_repack` (online rewrite) after a bloat incident. Regular autovacuum tuning should
keep you from needing either.

**Q7.** HOT (Heap-Only Tuple) requires (1) no indexed column changed, and (2) the new version fits on
the **same page**. Then no index entries are added. Indexes keep pointing to the chain's root line
pointer. Changing `indexed_col` changes an index key, so every index must get a new entry and HOT is impossible.

**Q8.** With fillfactor 100 pages are packed full, so new versions usually don't fit on the same page
and the update is non-HOT. Fillfactor 70 leaves 30% free per page for new versions, so most updates
become HOT, which means less index bloat and less WAL. The cost: the table is ~40% bigger, with more pages to scan.

**Q9.** Comparisons of 32-bit xids wrap around, so old rows would suddenly look like they were created
"in the future" and become invisible (data loss). Postgres prevents it by **freezing**: VACUUM marks
old tuples as frozen (visible to everyone, regardless of xid). Autovacuum forces anti-wraparound
vacuums, and if they can't run, Postgres eventually stops accepting writes to protect the data. Long
transactions and disabled autovacuum are how teams get there.
