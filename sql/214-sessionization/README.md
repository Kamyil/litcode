# 214 · Sessionization

> **Difficulty:** Hard · **Topics:** LAG, running sum of flags, sessionization

Split the events of customers `1..20` into **sessions**: a new session starts when more
than **30 minutes** passed since that customer's previous event (or at their first event).

Columns: `customer_id, sessions, events, avg_events_per_session` (rounded to 2 decimals)
— ordered by `customer_id`.

Tables: `customers`, `products`, `orders`, `order_items`, `employees`, `events`, `bookings` (see `sql/schema/01-schema.sql`, or `\d+ orders` in psql).

## Workflow

```
lc sql up               # once: start + seed postgres
$EDITOR answer.sql      # write ONE read-only query (all output of the file is compared)
lc sql run 214          # see your output
lc test 214             # compare with the expected result
lc sql explain 214      # EXPLAIN ANALYZE your query
```

## Think about

- Step 1: flag rows that start a session: `occurred_at - lag(occurred_at) OVER (...) > interval '30 minutes'`
  (or `lag` is NULL).
- Step 2: a running `sum(flag) OVER (PARTITION BY customer_id ORDER BY occurred_at)` assigns
  a session number to every row. "Running sum of start flags" is a pattern worth memorizing.
