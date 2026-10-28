# 251 · Lab: Reading EXPLAIN ANALYZE

> **Difficulty:** Easy · **Topics:** plan nodes, cost model, estimates vs actuals, buffers · **Mode:** read & do in psql

`lc tmux 251` opens this file next to two psql panes. Run each step, answer the **Q**s in your
own words (out loud, like in an interview), then compare with `lc solution 251`. Mark done with `lc done 251`.

## 1. The shape of a plan

```sql
EXPLAIN SELECT * FROM orders WHERE customer_id = 42;
SET max_parallel_workers_per_gather = 0;   -- keep plans simple for this lab
EXPLAIN SELECT * FROM orders WHERE customer_id = 42;
```

The first plan is a `Gather` over a `Parallel Seq Scan`; the second a plain
`Seq Scan on orders (cost=0.00..30590.00 rows=200 width=41)` (your numbers may differ slightly).

- **Q1.** What do the two cost numbers mean? What unit are they in?
- **Q2.** Where does `rows=200` come from if Postgres didn't run the query? (`SELECT n_distinct FROM pg_stats WHERE tablename = 'orders' AND attname = 'customer_id';`)

## 2. Estimates vs reality

```sql
EXPLAIN (ANALYZE, BUFFERS) SELECT * FROM orders WHERE customer_id = 42;
```

Now there's `actual time=... rows=... loops=1` and `Buffers: shared hit=... read=...`.

- **Q3.** Is the row estimate close to `actual rows`? Why does a bad estimate matter even when
  this particular query is fast?
- **Q4.** What's the difference between `shared hit` and `shared read`? Run the query twice — what changes and why?
- **Q5.** How many 8 KB pages does the table have? Check: `SELECT relpages, reltuples FROM pg_class WHERE relname = 'orders';`
  Compare with the buffers number from the seq scan.

## 3. Where the cost formula comes from

```sql
SHOW seq_page_cost; SHOW cpu_tuple_cost; SHOW cpu_operator_cost; SHOW random_page_cost;
```

- **Q6.** Compute the seq scan's total cost by hand: `relpages * seq_page_cost + reltuples * (cpu_tuple_cost + cpu_operator_cost)`.
  (The extra `cpu_operator_cost` is for evaluating the `WHERE`.) Does it match `EXPLAIN`?
- **Q7.** `random_page_cost` defaults to 4.0, `seq_page_cost` to 1.0. What does that ratio encode, and why do
  people lower `random_page_cost` to ~1.1 on SSDs?

## 4. Your first index

```sql
CREATE INDEX orders_customer_id_idx ON orders (customer_id);
EXPLAIN (ANALYZE, BUFFERS) SELECT * FROM orders WHERE customer_id = 42;
```

- **Q8.** Which scan type do you get now, and how many buffers does it touch vs the seq scan?
- **Q9.** Try a power user: `WHERE customer_id = 7` (they have ~150 orders). Plan type? Why might it
  switch to a **Bitmap Heap Scan**?

## 5. Nodes you'll meet everywhere

Run and identify each node type, then explain what it does in one sentence:

```sql
EXPLAIN ANALYZE SELECT status, count(*) FROM orders GROUP BY status;                       -- ?
EXPLAIN ANALYZE SELECT * FROM orders ORDER BY total DESC LIMIT 10;                          -- ?
EXPLAIN ANALYZE SELECT * FROM orders o JOIN customers c ON c.id = o.customer_id WHERE c.id < 100; -- ?
```

- **Q10.** The `ORDER BY ... LIMIT 10` plan says `Sort Method: top-N heapsort`. What algorithm is
  that and why is it so much cheaper than sorting 1M rows? (You built it: TS 034 / sys 104.)
- **Q11.** Why does the `GROUP BY` plan probably show `Gather` and `Partial HashAggregate`?

## 6. Readable output tools

- `EXPLAIN (ANALYZE, BUFFERS, FORMAT JSON)` — paste into https://explain.dalibo.com or https://explain.depesz.com.
- `\timing on` in psql shows client-side time.
- `SELECT query, calls, mean_exec_time, rows FROM pg_stat_statements ORDER BY total_exec_time DESC LIMIT 5;`
  — the "which queries are slow in production" view (enabled in this container).

## Cleanup

`RESET max_parallel_workers_per_gather;` — and keep `orders_customer_id_idx` — later labs assume it exists.
