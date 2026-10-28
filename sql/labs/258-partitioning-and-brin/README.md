# 258 · Lab: Partitioning and BRIN Indexes for Time-Series Data

> **Difficulty:** Medium · **Topics:** declarative partitioning, partition pruning, BRIN vs B-tree, dropping old data · **Mode:** psql

`events` is append-only and time-ordered — the classic case for both techniques. Answers: `lc solution 258`.

## 1. BRIN: a tiny index for naturally ordered data

```sql
CREATE TABLE ev_brin AS SELECT * FROM events ORDER BY occurred_at;
SELECT correlation FROM pg_stats WHERE tablename = 'ev_brin' AND attname = 'occurred_at'; -- after ANALYZE
ANALYZE ev_brin;

CREATE INDEX ev_brin_btree ON ev_brin (occurred_at);
CREATE INDEX ev_brin_brin  ON ev_brin USING brin (occurred_at);
SELECT indexrelname, pg_size_pretty(pg_relation_size(indexrelid)) FROM pg_stat_user_indexes WHERE relname = 'ev_brin';

EXPLAIN (ANALYZE, BUFFERS) SELECT count(*) FROM ev_brin WHERE occurred_at BETWEEN '2025-03-01' AND '2025-03-02';
DROP INDEX ev_brin_btree;
EXPLAIN (ANALYZE, BUFFERS) SELECT count(*) FROM ev_brin WHERE occurred_at BETWEEN '2025-03-01' AND '2025-03-02';
```

- **Q1.** How big is each index? How does BRIN work (what does it store per block range)?
- **Q2.** Why does BRIN need physical correlation? What happens on randomly ordered data? Try it:
  `CREATE TABLE ev_rand AS SELECT * FROM events ORDER BY random();` + a BRIN index + the same query.

## 2. Range partitioning by month

```sql
CREATE TABLE ev_part (LIKE events) PARTITION BY RANGE (occurred_at);
CREATE TABLE ev_2025_01 PARTITION OF ev_part FOR VALUES FROM ('2025-01-01') TO ('2025-02-01');
CREATE TABLE ev_2025_02 PARTITION OF ev_part FOR VALUES FROM ('2025-02-01') TO ('2025-03-01');
CREATE TABLE ev_2025_03 PARTITION OF ev_part FOR VALUES FROM ('2025-03-01') TO ('2025-04-01');
CREATE TABLE ev_2025_04 PARTITION OF ev_part FOR VALUES FROM ('2025-04-01') TO ('2025-05-01');
CREATE TABLE ev_2025_05 PARTITION OF ev_part FOR VALUES FROM ('2025-05-01') TO ('2025-06-01');
CREATE TABLE ev_default PARTITION OF ev_part DEFAULT;
INSERT INTO ev_part SELECT * FROM events;
ANALYZE ev_part;
SELECT tableoid::regclass, count(*) FROM ev_part GROUP BY 1 ORDER BY 1;
```

- **Q3.** Why are there rows in `ev_default`? What's the risk of a default partition when you later add `ev_2025_06`?

```sql
EXPLAIN ANALYZE SELECT count(*) FROM ev_part WHERE occurred_at >= '2025-03-10' AND occurred_at < '2025-03-20';
EXPLAIN ANALYZE SELECT count(*) FROM ev_part WHERE customer_id = 42;
PREPARE q(timestamptz) AS SELECT count(*) FROM ev_part WHERE occurred_at >= $1;
EXPLAIN ANALYZE EXECUTE q('2025-05-15');
```

- **Q4.** Which partitions are scanned in each? What's the difference between plan-time and run-time
  pruning (look for `Subplans Removed`)?
- **Q5.** The `customer_id` query scans everything. What's the partition key design lesson?

## 3. Retention: the killer feature

```sql
\timing on
DELETE FROM ev_brin WHERE occurred_at < '2025-02-01';   -- row by row
ALTER TABLE ev_part DETACH PARTITION ev_2025_01;         -- metadata only
DROP TABLE ev_2025_01;
```

- **Q6.** Compare. What does DELETE leave behind (think lab 254) that DROP doesn't?
- **Q7.** Name two downsides/limits of partitioning (unique constraints, planning time, too many partitions...).

## Cleanup

```sql
DROP TABLE ev_brin, ev_part;
DROP TABLE IF EXISTS ev_rand;
```
