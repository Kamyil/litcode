# 256 · Answers

**Q1.** `wait_event_type = Lock`, `wait_event = transactionid`: B waits for A's **transaction** to end,
because row locks are recorded in the tuple itself (`xmax` + infomask), not in `pg_locks`, and a waiter
queues on the locker's transaction id. Plain SELECTs don't block: under MVCC readers read the
last committed version and never take row locks. Readers don't block writers, and writers don't block readers.

**Q2.** After `deadlock_timeout`, the waiting backend runs the deadlock detector, which builds the
**wait-for graph** of lock waits and searches it for a cycle (a cycle-detection DFS, the same idea as course
schedule / topological sort). One transaction is aborted with `ERROR: deadlock detected` (SQLSTATE
`40P01`) and the server log shows both queries. The other proceeds once the victim's locks are released.

**Q3.** Lock rows in a **consistent global order** (e.g. always lower id first:
`SELECT ... WHERE id IN (1, 2) ORDER BY id FOR UPDATE`), keep transactions short, and retry on 40P01.
Deadlocks are an application bug, not a database malfunction.

**Q4.** No. Each worker skips rows locked by the other, so they get disjoint jobs and work in
parallel. Plain `FOR UPDATE`: worker 2 blocks until worker 1 commits, then (READ COMMITTED) re-checks the
rows, finds them `done`, and gets nothing for those (serialized workers). No locking: both read the
same `queued` rows and process jobs twice. SKIP LOCKED turns a table into a decent queue for moderate
throughput (the basis of tools like `pgmq`, Oban, good_job, graphile-worker).

**Q5.** `NOWAIT` errors immediately (`55P03 lock_not_available`) instead of waiting. Use it when the
caller would rather fail fast than queue up.

**Q6.** `ALTER TABLE ... ADD COLUMN` needs an **ACCESS EXCLUSIVE** lock, which conflicts with A's
ACCESS SHARE. B waits in the lock queue, and C's ACCESS SHARE request conflicts with B's *queued* exclusive
request. Postgres grants locks in FIFO order to avoid starving B, so C queues behind B. One long query plus
one DDL statement blocks **every** new query on the table, and connection pools fill up within seconds.

**Q7.** `SET lock_timeout = '2s'` before DDL (fail fast instead of blocking everyone, then retry in a
loop), and `statement_timeout` for the operation itself. Habit: check for long-running transactions first
(`pg_stat_activity`). Plus: prefer operations that are metadata-only (adding a nullable column or one with a
constant default is instant since PG11), use `CREATE INDEX CONCURRENTLY`, and add constraints as
`NOT VALID` then `VALIDATE CONSTRAINT` (which takes a weaker lock).

**Q8.** `CREATE INDEX` takes a **SHARE** lock: reads continue, but INSERT/UPDATE/DELETE block for the
whole build. `CONCURRENTLY` takes SHARE UPDATE EXCLUSIVE (writes continue). It scans the table twice and
waits for all transactions that could see old snapshots, so it takes longer. It can't run inside a
transaction block, and if it fails it leaves an **INVALID** index behind that you must drop and
retry (`\d` shows INVALID). `REINDEX CONCURRENTLY` (PG12+) works the same way.
