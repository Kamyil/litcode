# 259 · Lab: WAL, Checkpoints and Durability

> **Difficulty:** Medium · **Topics:** write-ahead log, LSNs, fsync, synchronous_commit, checkpoints, full-page writes · **Mode:** psql

How does Postgres survive a power cut halfway through a transaction? Answers: `lc solution 259`.

## 1. Watching the WAL grow

```sql
SELECT pg_current_wal_lsn();
CREATE TABLE w (id int, v text);
SELECT pg_current_wal_lsn() AS before \gset
INSERT INTO w SELECT g, repeat('x', 100) FROM generate_series(1, 100000) g;
SELECT pg_size_pretty(pg_wal_lsn_diff(pg_current_wal_lsn(), :'before')) AS wal_written,
       pg_size_pretty(pg_relation_size('w')) AS table_size;
```

- **Q1.** What is an LSN? Why is more WAL written than the table's size?
- **Q2.** Explain write-ahead logging in three sentences: what gets written where, and in which order, at COMMIT?

## 2. The cost of durability

```sql
\timing on
CREATE TABLE tiny (id serial, t timestamptz DEFAULT now());
-- 2000 single-row transactions, each COMMIT waits for fsync:
DO $$ BEGIN FOR i IN 1..2000 LOOP INSERT INTO tiny DEFAULT VALUES; COMMIT; END LOOP; END $$;
SET synchronous_commit = off;
DO $$ BEGIN FOR i IN 1..2000 LOOP INSERT INTO tiny DEFAULT VALUES; COMMIT; END LOOP; END $$;
RESET synchronous_commit;
```

- **Q3.** How much faster was it with `synchronous_commit = off`? What exactly do you risk (and what
  do you *not* risk)? Compare with `fsync = off`.
- **Q4.** Why is batching many rows into one transaction so much faster than many tiny transactions?

## 3. Checkpoints and full-page writes

```sql
SELECT * FROM pg_stat_bgwriter;   -- PG17: see also pg_stat_checkpointer
SELECT * FROM pg_stat_checkpointer;
SHOW checkpoint_timeout; SHOW max_wal_size; SHOW full_page_writes;
CHECKPOINT;
SELECT pg_current_wal_lsn() AS before \gset
UPDATE w SET v = 'y' WHERE id % 100 = 0;   -- touches ~1000 rows spread over all pages
SELECT pg_size_pretty(pg_wal_lsn_diff(pg_current_wal_lsn(), :'before'));
SELECT pg_current_wal_lsn() AS before \gset
UPDATE w SET v = 'z' WHERE id % 100 = 0;   -- same rows again, no checkpoint in between
SELECT pg_size_pretty(pg_wal_lsn_diff(pg_current_wal_lsn(), :'before'));
```

- **Q5.** Why did the first update after `CHECKPOINT` write much more WAL than the second identical one?
  What problem do full-page writes solve?
- **Q6.** What does a checkpoint do, and what's the trade-off in making them more or less frequent?

## 4. Putting it together (talk it through)

- **Q7.** Streaming replication and point-in-time recovery are both "just WAL". Explain how.
- **Q8.** A client gets `COMMIT` OK, then the server loses power. With default settings, is the data
  safe? What if the commit was acknowledged by a primary but the replica it failed over to hadn't
  received the WAL yet?

## Cleanup

```sql
DROP TABLE w, tiny;
```
