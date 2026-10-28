# 256 · Lab: Locks, Deadlocks, Job Queues and Safe Migrations

> **Difficulty:** Hard · **Topics:** row locks, FOR UPDATE / SKIP LOCKED, deadlock detection, pg_locks, lock queues, DDL lock levels · **Mode:** two (or three) psql panes

Answers: `lc solution 256`.

## Setup

```sql
DROP TABLE IF EXISTS accounts, jobs;
CREATE TABLE accounts (id int PRIMARY KEY, balance int NOT NULL);
INSERT INTO accounts VALUES (1, 100), (2, 100);
CREATE TABLE jobs (id serial PRIMARY KEY, payload text, status text NOT NULL DEFAULT 'queued', worker text);
INSERT INTO jobs (payload) SELECT 'job ' || g FROM generate_series(1, 10) g;
```

## 1. Seeing who waits for whom

| Pane A | Pane B |
|---|---|
| `BEGIN; UPDATE accounts SET balance = balance - 10 WHERE id = 1;` | |
| | `UPDATE accounts SET balance = balance + 10 WHERE id = 1;` (hangs) |

In a third pane (or Pane A, it's still usable):

```sql
SELECT pid, state, wait_event_type, wait_event, left(query, 60) AS query,
       pg_blocking_pids(pid) AS blocked_by
FROM pg_stat_activity WHERE datname = 'lc' AND pid <> pg_backend_pid();
SELECT locktype, relation::regclass, mode, granted, pid FROM pg_locks WHERE NOT granted;
```

Then `COMMIT;` in A.

- **Q1.** What lock was B waiting for (`wait_event_type`)? Do plain `SELECT`s on `accounts` block during this? Why not?

## 2. A deadlock on purpose

| step | Pane A | Pane B |
|---|---|---|
| 1 | `BEGIN; UPDATE accounts SET balance = balance - 10 WHERE id = 1;` | |
| 2 | | `BEGIN; UPDATE accounts SET balance = balance - 10 WHERE id = 2;` |
| 3 | `UPDATE accounts SET balance = balance + 10 WHERE id = 2;` (waits) | |
| 4 | | `UPDATE accounts SET balance = balance + 10 WHERE id = 1;` |

Wait ~1 s (`deadlock_timeout` is 1s in this container).

- **Q2.** What happened to each session? How does Postgres detect deadlocks (connect to TS 038!)?
- **Q3.** How do you prevent this class of deadlock in application code?

## 3. A job queue with SKIP LOCKED

Pane A, then Pane B (while A's transaction is still open):

```sql
BEGIN;
SELECT id, payload FROM jobs WHERE status = 'queued' ORDER BY id LIMIT 3 FOR UPDATE SKIP LOCKED;
-- ...look at which ids you got, then in each pane:
COMMIT;
```

The realistic worker loop is one statement: claim a job atomically and mark it.

```sql
WITH next AS (
  SELECT id FROM jobs WHERE status = 'queued' ORDER BY id LIMIT 1 FOR UPDATE SKIP LOCKED
)
UPDATE jobs j SET status = 'running', worker = 'w' || pg_backend_pid()
FROM next WHERE j.id = next.id
RETURNING j.*;
```

- **Q4.** Did the two workers get the same jobs? What would happen with plain `FOR UPDATE` (no SKIP LOCKED)? With no locking at all?
- **Q5.** What does `NOWAIT` do instead?

## 4. The migration that took production down

| Pane A (a slow analytics query) | Pane B (a migration) | Pane C (the app) |
|---|---|---|
| `BEGIN; SELECT count(*) FROM accounts; SELECT pg_sleep(60);` | | |
| | `ALTER TABLE accounts ADD COLUMN note text;` (hangs) | |
| | | `SELECT * FROM accounts;` (hangs too!) |

Cancel with Ctrl-C in B, then C returns.

- **Q6.** Why does a plain `SELECT` in C block, when A only holds a read lock? Explain the **lock queue**.
- **Q7.** How do you run DDL safely on a busy table? (Two settings + one habit.)

## 5. Lock levels cheat sheet

```sql
BEGIN; CREATE INDEX ON accounts (balance);
SELECT relation::regclass, mode FROM pg_locks WHERE pid = pg_backend_pid() AND relation = 'accounts'::regclass;
ROLLBACK;
```

- **Q8.** What lock does `CREATE INDEX` take, and what does it block? What does `CREATE INDEX CONCURRENTLY` do
  differently, and what are its caveats?

## Cleanup

```sql
DROP TABLE accounts, jobs;
```
