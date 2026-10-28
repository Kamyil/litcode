# 252 · Lab: B-tree Indexes, Selectivity, Composite & Covering Indexes

> **Difficulty:** Medium · **Topics:** selectivity, index vs bitmap vs seq scan, column order, INCLUDE, index-only scans, partial indexes · **Mode:** psql

Prereq: lab 251 (index on `orders(customer_id)` exists). Answers: `lc solution 252`.

## 1. Selectivity decides the plan

```sql
CREATE INDEX orders_created_at_idx ON orders (created_at);

EXPLAIN ANALYZE SELECT * FROM orders WHERE created_at >= '2025-12-31';  -- ~0.1% of rows
EXPLAIN ANALYZE SELECT * FROM orders WHERE created_at >= '2025-10-01';  -- ~8%
EXPLAIN ANALYZE SELECT * FROM orders WHERE created_at >= '2024-01-01';  -- ~67%
```

- **Q1.** Which scan type does each use? At what selectivity does an index stop being worth it, roughly, and why?
- **Q2.** `orders.created_at` is almost perfectly correlated with physical row order. Check
  `SELECT attname, correlation FROM pg_stats WHERE tablename = 'orders';`. Why does correlation make index range scans cheaper?
  Try the same three queries on `customer_id BETWEEN ...` ranges, which are uncorrelated.

## 2. Composite indexes: column order matters

```sql
CREATE INDEX orders_status_created_idx ON orders (status, created_at);

EXPLAIN ANALYZE SELECT * FROM orders WHERE status = 'refunded' AND created_at >= '2025-06-01';
EXPLAIN ANALYZE SELECT * FROM orders WHERE created_at >= '2025-12-01';
EXPLAIN ANALYZE SELECT * FROM orders WHERE status = 'refunded';
```

- **Q3.** Which of these can use `(status, created_at)` efficiently, and which can't? State the rule.
- **Q4.** Equality columns first, range column last: why is `(status, created_at)` better than
  `(created_at, status)` for the first query?

## 3. Index-only scans and the visibility map

```sql
EXPLAIN (ANALYZE, BUFFERS) SELECT customer_id, created_at FROM orders WHERE customer_id = 7;

CREATE INDEX orders_cust_created_incl_idx ON orders (customer_id, created_at) INCLUDE (total);
EXPLAIN (ANALYZE, BUFFERS) SELECT customer_id, created_at, total FROM orders WHERE customer_id = 7;
```

- **Q5.** Do you see `Index Only Scan`? What does `Heap Fetches: N` mean?
- **Q6.** Run `VACUUM orders;` and repeat. What changed and why? (Keyword: visibility map.)
- **Q7.** What's the difference between a key column and an `INCLUDE` column?

## 4. Partial indexes for skewed data

Only 8% of orders are `pending`, and the app's dashboard constantly asks for them:

```sql
CREATE INDEX orders_pending_idx ON orders (created_at) WHERE status = 'pending';
EXPLAIN ANALYZE SELECT * FROM orders WHERE status = 'pending' ORDER BY created_at LIMIT 20;
SELECT indexrelname, pg_size_pretty(pg_relation_size(indexrelid)) FROM pg_stat_user_indexes WHERE relname = 'orders';
```

- **Q8.** Compare sizes. When does a partial index get used (what must the query's `WHERE` look like)?
- **Q9.** Why does `ORDER BY created_at LIMIT 20` not need a Sort node here?

## 5. The cost of indexes

```sql
CREATE TABLE orders_copy (LIKE orders);   -- columns only: no primary key, no indexes
\timing on
INSERT INTO orders_copy SELECT * FROM orders WHERE id <= 200000;   -- (a) no indexes
TRUNCATE orders_copy;
ALTER TABLE orders_copy ADD PRIMARY KEY (id);
CREATE INDEX ON orders_copy (customer_id);
CREATE INDEX ON orders_copy (created_at);
CREATE INDEX ON orders_copy (status, created_at);
CREATE INDEX ON orders_copy (customer_id, created_at) INCLUDE (total);
INSERT INTO orders_copy SELECT * FROM orders WHERE id <= 200000;   -- (b) five indexes
```

- **Q10.** How much slower was the insert with all the indexes? Name three other costs of an extra index.

## Cleanup

```sql
DROP TABLE orders_copy;
DROP INDEX orders_status_created_idx, orders_cust_created_incl_idx, orders_pending_idx;
-- keep orders_customer_id_idx and orders_created_at_idx
```
