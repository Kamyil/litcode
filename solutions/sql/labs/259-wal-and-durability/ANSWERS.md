# 259 · Answers

**Q1.** A Log Sequence Number is a byte position in the (conceptually infinite) WAL stream, shown as
`X/YYYYYYYY`. WAL includes record headers, per-tuple overhead, full-page images (after a checkpoint, the
first change to each page logs the whole 8 KB page), and index changes. So for a fresh table it's often
larger than the data itself.

**Q2.** Every change is first described in the WAL (in memory, then written sequentially). At COMMIT,
Postgres flushes (`fsync`) the WAL up to the commit record before reporting success. The changed data
pages stay in shared buffers ("dirty") and are written to the table files later, lazily. After a crash,
recovery replays the WAL from the last checkpoint, redoing changes that hadn't reached the data files yet.
Sequential WAL fsyncs are cheap compared to random writes of data pages, and that's the whole trick.

**Q3.** Often 5–20× faster on fsync-bound hardware. Risk with `synchronous_commit = off`: a crash can
lose the last few hundred milliseconds of *acknowledged* commits (up to 3 × `wal_writer_delay`), but
the database stays **consistent**: no corruption, and transactions are lost whole. `fsync = off` is
different: a crash can leave data files and WAL inconsistent, i.e. **corruption**. Never use it for data you
care about. `synchronous_commit = off` can be set per transaction, which is fine for e.g. analytics events.

**Q4.** The fsync at commit is the expensive part (a disk round-trip). One transaction with 2000 rows
needs one WAL flush. 2000 transactions need 2000 (minus whatever group commit merges). Use batched inserts
or `COPY` for bulk loads.

**Q5.** After a checkpoint, the first modification of each page writes a **full-page image** into the
WAL. Later modifications of the same page (until the next checkpoint) log only the small change. They
exist because of **torn pages**: the OS and disk write in 4 KB (or smaller) units, so a crash mid-write
could leave an 8 KB page half old, half new. Redo can't repair that from a tiny delta, but it can
restore the full image and replay on top. More checkpoints means more full-page images, so more WAL.

**Q6.** A checkpoint writes all dirty buffers to the data files, fsyncs them, and records a redo
point: WAL before it is no longer needed for crash recovery (it can be recycled or archived).
Frequent checkpoints mean shorter crash recovery, but more I/O spikes and more full-page-image WAL.
Infrequent ones (`max_wal_size` large, `checkpoint_timeout` longer) mean smoother performance and less WAL, but
longer recovery after a crash. `checkpoint_completion_target` (0.9) spreads the writes out.

**Q7.** Replication: the primary streams WAL records to replicas, which replay them continuously (a
physical, byte-identical copy). PITR: take a base backup plus archive every WAL segment. To recover,
restore the backup and replay WAL up to a chosen timestamp or LSN (`recovery_target_time`). Logical
replication decodes WAL into row changes (`wal_level = logical`) for selective or cross-version replication.

**Q8.** Single server, defaults (`fsync = on`, `synchronous_commit = on`): yes, safe. The commit
record was fsynced before the OK. Failover is different: with asynchronous replication (the default)
the replica may lag, so a commit acknowledged by the primary can be **lost** after failover. To prevent that,
use synchronous replication (`synchronous_standby_names` + `synchronous_commit = on`/`remote_apply`),
which adds latency to every commit. Classic CAP/latency trade-off question.
