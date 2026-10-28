# 405 · Answers

**1.** **Atomicity**: all or nothing. The commit record in the WAL flips a transaction from invisible to
visible, and aborted versions are never seen (clog/pg_xact). **Consistency**: constraints, foreign keys,
triggers keep invariants. **Isolation**: MVCC snapshots plus locks plus SSI. **Durability**: WAL fsync at
commit, then crash recovery.

**2.** Every write creates a new row version tagged with the transaction ids that created and expired it.
Readers use a snapshot to choose which version to see, so they never wait for writers, and writers never
wait for readers. Writers only block other writers of the same row. The price: dead versions accumulate and
must be cleaned by VACUUM (bloat, vacuum tuning, wraparound maintenance), and visibility checks cost CPU.

**3.** READ UNCOMMITTED behaves as READ COMMITTED (no dirty reads). READ COMMITTED: non-repeatable reads,
phantoms, lost updates (with read-modify-write in the app), write skew. REPEATABLE READ (snapshot isolation):
no non-repeatable reads, no phantoms (stricter than the standard), lost updates turn into serialization errors,
write skew still possible. SERIALIZABLE (SSI): none, behaves as if serial, at the cost of possible 40001 aborts.

**4.** Two doctors on call, each checks "someone else is on call" and both go off call; or two bookings
each check "room free" then insert. Prevent: SERIALIZABLE (with retry), explicit `SELECT ... FOR UPDATE`
on the rows the decision depends on, locking a shared "parent" row, or a constraint (exclusion constraint for
bookings, unique index).

**5.** It holds its snapshot, so VACUUM can't remove dead tuples anywhere newer than it, causing database-wide
bloat and eventually wraparound risk. It holds its locks, so DDL queues behind it and everything queues behind the
DDL. It wastes a connection slot. Fix: `idle_in_transaction_session_timeout`.

**6.** `FOR UPDATE` blocks all other row locks, including the `FOR KEY SHARE` that foreign key checks take.
`FOR NO KEY UPDATE` (what plain UPDATEs not touching key columns take) still allows `FOR KEY SHARE`, so
inserting child rows referencing the parent isn't blocked by an update of non-key columns of the parent.
`FOR SHARE` lets multiple readers lock a row against modification. The weaker modes reduce contention, especially around FKs.

**7.** After `deadlock_timeout` a waiting backend builds the wait-for graph and looks for a cycle. If it
finds one, it aborts one participant with `40P01 deadlock_detected`. The application retries the
transaction, and long-term fixes the lock ordering.

**8.** Adding a column with a constant (non-volatile) default is metadata-only since PG11 (the default is
stored in the catalog), so it's instant but still needs a brief ACCESS EXCLUSIVE lock (use `lock_timeout`!).
Changing a column's type to an incompatible binary representation rewrites the whole table and all its
indexes under ACCESS EXCLUSIVE, which means downtime for big tables. The online alternative: add a new column, backfill in
batches, swap.

**9.** Row locks protect individual tuples (stored in the tuple header). Table locks (ACCESS SHARE ... ACCESS
EXCLUSIVE) protect the table's existence and structure, taken implicitly by every statement. Advisory locks are
application-defined locks on arbitrary numbers that Postgres doesn't interpret: e.g. ensuring only one
instance of a cron job or migration runs (`pg_try_advisory_lock(42)`), or per-tenant serialization.

**10.** Pessimistic: lock first, then read and modify: `BEGIN; SELECT balance FROM accounts WHERE id = 1 FOR UPDATE; UPDATE ...; COMMIT;`.
Optimistic: read without locking, then write conditionally and detect conflicts:
`UPDATE accounts SET balance = 90, version = version + 1 WHERE id = 1 AND version = 7;` and retry if 0 rows were updated.
Optimistic wins with low contention, pessimistic with high contention.

**11.** SSI aborts transactions when it detects a dangerous pattern, and that's normal operation, not a
failure. Retry on `40001 serialization_failure` and `40P01 deadlock_detected`. Retry the **whole**
transaction including its reads, ideally with backoff.

**12.** Each Postgres connection is a separate OS process (several MB of memory, plus snapshot-computation
costs that grow with the connection count). Thousands of app connections overwhelm it. PgBouncer multiplexes many
client connections onto a few server connections. In transaction mode a client gets a server connection only for
a transaction, so session state breaks: `SET` without `LOCAL`, session advisory locks, `LISTEN`, temp tables,
and (in older PgBouncer) prepared statements. PgBouncer 1.21+ supports protocol-level prepared statements.

**13.** Async: the commit returns after the local WAL flush. Lowest latency, but a failover can lose recently
committed transactions (replication lag). Sync: the commit waits until a standby confirms (flush or apply). No
data loss on failover, at the cost of a network round-trip per commit, and if the sync standby is down,
commits block (unless you configure several candidates or quorum `ANY 1 (...)`).

**14.** xids are 32-bit and compared modulo 2³², so each transaction sees ~2 billion xids as "past" and ~2
billion as "future". Without freezing, old rows would flip to "future" and vanish. VACUUM freezes tuples
older than `vacuum_freeze_min_age`, and autovacuum forces anti-wraparound vacuums at
`autovacuum_freeze_max_age`. If it's blocked (long transactions, disabled autovacuum), Postgres eventually
refuses new xids to protect data. Monitor `age(datfrozenxid)`.
