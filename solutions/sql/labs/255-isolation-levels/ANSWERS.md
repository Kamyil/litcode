# 255 · Answers

**Q1.** A1: 100, A2: 50. In READ COMMITTED **each statement** takes a new snapshot, so it sees
everything committed before that statement started. B's change was committed at B1 (autocommit), so
A2 sees it. That's a non-repeatable read. Dirty reads never happen in Postgres: even
`READ UNCOMMITTED` behaves like READ COMMITTED, because MVCC never shows uncommitted versions to other transactions.

**Q2.** 120: A's +10 is lost. Both read 100, computed in the app, and wrote absolute values. B's UPDATE
waited for A's row lock, then (in READ COMMITTED) re-checked the row and overwrote it. Fixes:
1. **Atomic update**: `SET balance = balance + 10` (the database does the read-modify-write under the row lock). Best option when possible.
2. **Pessimistic lock**: `SELECT ... FOR UPDATE` at the start, so the second transaction waits until the first commits, then reads 110.
3. **REPEATABLE READ / SERIALIZABLE** with retry. Or **optimistic locking** in the app: a `version` column
   and `UPDATE ... WHERE id = 1 AND version = 7`, checking the affected row count (what ORMs do).

**Q3.** `ERROR: could not serialize access due to concurrent update` (SQLSTATE `40001`). B's snapshot
is older than A's committed change to the same row, and Postgres refuses to overwrite a version B never
saw. The application must **roll back and retry the whole transaction** (re-reading the data). Any code
using RR/SERIALIZABLE needs a retry loop.

**Q4.** No. Postgres's REPEATABLE READ is **snapshot isolation**: one snapshot for the whole
transaction, so phantoms can't appear in reads. Postgres is stricter than the standard's minimum. (It
still allows write skew; see Q5.)

**Q5.** Both doctors are off call, so the invariant is broken. Each transaction read a consistent
snapshot (2 on call), and they updated **different** rows, so there was no write-write conflict to block on or detect.
This is **write skew**, the classic anomaly snapshot isolation permits.

**Q6.** The second to commit (usually B, at B2 or B3) fails with `40001 could not serialize access due to
read/write dependencies among transactions`. Postgres's **Serializable Snapshot Isolation (SSI)** tracks
read sets with lightweight SIRead predicate locks (they block nobody) and detects dangerous structures
(two consecutive rw-antidependencies forming a cycle), then aborts one transaction. It can produce
false positives, so retry. Non-SERIALIZABLE fixes: `SELECT ... FOR UPDATE` on **all** rows the decision
depends on (`SELECT * FROM doctors WHERE on_call FOR UPDATE`), or materialize the conflict (lock a
common row like the shift/ward row), or enforce it with a constraint/trigger.

**Q7.** REPEATABLE READ (one snapshot for all 10 queries, readers never block writers under MVCC).
`SERIALIZABLE READ ONLY DEFERRABLE` waits until it can get a snapshot guaranteed free of serialization
anomalies, then runs with no SSI overhead and can never fail with 40001. It's ideal for long reports and `pg_dump`-like jobs.
