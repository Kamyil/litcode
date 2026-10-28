# 257 · Lab: Join Algorithms, work_mem, and Planner Statistics

> **Difficulty:** Hard · **Topics:** nested loop / hash / merge join, external sorts, hash batches, ANALYZE, extended statistics · **Mode:** psql

Answers: `lc solution 257`. Assumes the indexes from 251/252 (`orders(customer_id)`, `orders(created_at)`).

## 1. Three join algorithms

```sql
-- few outer rows + index on the inner side
EXPLAIN ANALYZE SELECT * FROM customers c JOIN orders o ON o.customer_id = c.id WHERE c.id < 20;
-- big join, no useful order
EXPLAIN ANALYZE SELECT c.country, count(*) FROM customers c JOIN orders o ON o.customer_id = c.id GROUP BY c.country;
-- both inputs already sorted on the join key
EXPLAIN ANALYZE SELECT count(*) FROM orders o JOIN order_items oi ON oi.order_id = o.id WHERE o.id < 200000;
```

- **Q1.** Identify Nested Loop, Hash Join and Merge Join. For each: how does it work, what's its cost,
  and when does the planner choose it? (You've written all three ideas: two loops, a hash map, merging sorted lists.)
- **Q2.** Force the "wrong" algorithm and compare times:
  `SET enable_hashjoin = off;` re-run the second query; `RESET enable_hashjoin;`. Why do these `enable_*`
  settings exist, and why shouldn't you use them in production?

## 2. work_mem: sorts and hashes that spill to disk

```sql
SET work_mem = '64kB';
EXPLAIN (ANALYZE, BUFFERS) SELECT * FROM orders ORDER BY total;
SET work_mem = '256MB';
EXPLAIN (ANALYZE, BUFFERS) SELECT * FROM orders ORDER BY total;
RESET work_mem;
```

- **Q3.** Compare `Sort Method:` lines (`external merge Disk: ...` vs `quicksort Memory: ...`) and the
  times. This is exactly sys/110. How many runs would 64 kB produce, roughly?

```sql
SET work_mem = '64kB';
EXPLAIN ANALYZE SELECT c.country, count(*) FROM customers c JOIN orders o ON o.customer_id = c.id GROUP BY c.country;
RESET work_mem;
```

- **Q4.** Find `Batches:` on the Hash node. What does a hash join do when the hash table doesn't fit in `work_mem`?
- **Q5.** `work_mem` is per *operation*, per backend. Why is setting it to 1 GB globally on a server with
  200 connections dangerous? What's a safer approach?

## 3. When statistics lie

```sql
CREATE TABLE addr AS
SELECT g AS id,
       (ARRAY['Warsaw','Krakow','Berlin','Munich','Paris','Lyon'])[1 + g % 6] AS city,
       (ARRAY['PL','PL','DE','DE','FR','FR'])[1 + g % 6] AS country
FROM generate_series(1, 300000) g;
ANALYZE addr;
EXPLAIN ANALYZE SELECT * FROM addr WHERE city = 'Warsaw' AND country = 'PL';
```

- **Q6.** Compare estimated vs actual rows. Why is the estimate ~3x too low? (Hint: the planner assumes the two conditions are independent.)

```sql
CREATE STATISTICS addr_city_country (dependencies, ndistinct, mcv) ON city, country FROM addr;
ANALYZE addr;
EXPLAIN ANALYZE SELECT * FROM addr WHERE city = 'Warsaw' AND country = 'PL';
EXPLAIN ANALYZE SELECT city, country, count(*) FROM addr GROUP BY 1, 2;
```

- **Q7.** What did extended statistics fix? Where can a 3x misestimate turn into a 1000x slower query?

## 4. Stale statistics

```sql
CREATE TABLE fresh (id int, v int) WITH (autovacuum_enabled = off);
INSERT INTO fresh SELECT g, g % 10 FROM generate_series(1, 1000) g;
ANALYZE fresh;
INSERT INTO fresh SELECT g, 42 FROM generate_series(1, 500000) g;
EXPLAIN ANALYZE SELECT * FROM fresh WHERE v = 42;
ANALYZE fresh;
EXPLAIN ANALYZE SELECT * FROM fresh WHERE v = 42;
```

- **Q8.** What did the planner believe before the second `ANALYZE`? When should you run `ANALYZE` manually?
- **Q9.** `SHOW default_statistics_target;`: what does it control, and when would you raise it for one column?

## Cleanup

```sql
DROP TABLE addr, fresh;
```
