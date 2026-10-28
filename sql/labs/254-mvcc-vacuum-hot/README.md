# 254 · Lab: MVCC, Dead Tuples, VACUUM and HOT Updates

> **Difficulty:** Medium · **Topics:** xmin/xmax, ctid, tuple versions, bloat, VACUUM vs VACUUM FULL, HOT, fillfactor, pageinspect · **Mode:** psql (two panes for step 3)

Postgres never updates a row in place. An `UPDATE` writes a **new version** of the row and marks
the old one dead. This lab makes that visible. Connect it to sys/108: you're looking at the same
slotted page layout. Answers: `lc solution 254`.

## Setup

```sql
CREATE TABLE acct (id int PRIMARY KEY, owner text, balance int) WITH (autovacuum_enabled = off);
INSERT INTO acct SELECT g, 'owner ' || g, 100 FROM generate_series(1, 5) g;
```

## 1. Hidden system columns

```sql
SELECT ctid, xmin, xmax, * FROM acct;
SELECT txid_current();
UPDATE acct SET balance = balance + 1 WHERE id = 1;
SELECT ctid, xmin, xmax, * FROM acct;
```

- **Q1.** What are `ctid`, `xmin`, `xmax`? Why did row 1's `ctid` change after the update?

## 2. Look inside the page

```sql
SELECT lp, lp_off, lp_len, t_xmin, t_xmax, t_ctid, t_infomask2 & 16384 > 0 AS hot_updated
FROM heap_page_items(get_raw_page('acct', 0));
SELECT * FROM page_header(get_raw_page('acct', 0));
```

- **Q2.** How many line pointers are there for 5 logical rows, and why? Find the old version of row 1.
  What does its `t_ctid` point to?
- **Q3.** Map `page_header`'s `lower` and `upper` to what you implemented in sys/108.

## 3. Snapshots: who sees what (two panes)

Pane A:
```sql
BEGIN ISOLATION LEVEL REPEATABLE READ;
SELECT balance FROM acct WHERE id = 2;   -- 100
```
Pane B:
```sql
UPDATE acct SET balance = 999 WHERE id = 2;   -- autocommits
SELECT ctid, xmin, xmax, balance FROM acct WHERE id = 2;
```
Pane A:
```sql
SELECT balance FROM acct WHERE id = 2;   -- still 100?!
COMMIT;
SELECT balance FROM acct WHERE id = 2;
```

- **Q4.** Both versions of row 2 exist on the page at once. How does Pane A's snapshot decide which
  version is visible to it? Why can't VACUUM remove the old version while Pane A's transaction is open?

## 4. Dead tuples and VACUUM

```sql
UPDATE acct SET balance = balance + 1;  -- run this line 20 times (psql: \watch 0.1, then Ctrl-C)
SELECT n_live_tup, n_dead_tup, n_tup_upd, n_tup_hot_upd FROM pg_stat_user_tables WHERE relname = 'acct';
SELECT count(*) FROM heap_page_items(get_raw_page('acct', 0));
VACUUM VERBOSE acct;
SELECT count(*) AS line_pointers, count(*) FILTER (WHERE lp_len = 0) AS unused FROM heap_page_items(get_raw_page('acct', 0));
```

- **Q5.** What did VACUUM do to the dead tuples and their line pointers? Did the file shrink? (`SELECT pg_relation_size('acct');`)
- **Q6.** `VACUUM` vs `VACUUM FULL`: what does each do, what lock does each take, and when would you use FULL
  (or `pg_repack`) in production?

## 5. HOT updates and fillfactor

Notice `n_tup_hot_upd` above. Now on a bigger table:

```sql
CREATE TABLE hot_test (id int PRIMARY KEY, indexed_col int, plain_col int) WITH (fillfactor = 100);
CREATE INDEX ON hot_test (indexed_col);
INSERT INTO hot_test SELECT g, g, g FROM generate_series(1, 100000) g;
UPDATE hot_test SET plain_col = plain_col + 1;
UPDATE hot_test SET indexed_col = indexed_col + 1;
SELECT n_tup_upd, n_tup_hot_upd FROM pg_stat_user_tables WHERE relname = 'hot_test';

TRUNCATE hot_test;
ALTER TABLE hot_test SET (fillfactor = 70);
INSERT INTO hot_test SELECT g, g, g FROM generate_series(1, 100000) g;
SELECT pg_stat_reset_single_table_counters('hot_test'::regclass);
UPDATE hot_test SET plain_col = plain_col + 1;
SELECT n_tup_upd, n_tup_hot_upd FROM pg_stat_user_tables WHERE relname = 'hot_test';
```

(Stats may lag by a moment; re-run the SELECT if counters look stale.)

- **Q7.** What two conditions must hold for an update to be HOT? Why did updating `indexed_col` never produce HOT updates?
- **Q8.** Why does `fillfactor = 70` increase the HOT ratio, and what does it cost?

## 6. Transaction ID wraparound (read)

`xmin` is a 32-bit counter. Read about freezing:
`SELECT relname, age(relfrozenxid) FROM pg_class WHERE relkind = 'r' ORDER BY 2 DESC LIMIT 5;`

- **Q9.** What happens if a table is never vacuumed for ~2 billion transactions? What does VACUUM's "freezing" do?

## Cleanup

```sql
DROP TABLE acct, hot_test;
```
