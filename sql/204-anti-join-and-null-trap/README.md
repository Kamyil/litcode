# 204 · Anti Join And Null Trap

> **Difficulty:** Medium · **Topics:** LEFT JOIN, NOT EXISTS, NOT IN + NULL

Customers who have **never** placed an order, in the `JP` market.

Columns: `id, email, created_at` — ordered by `id`. (Expect a few dozen rows.)

Then, in a psql pane (not graded), compare these three forms and explain the difference:

```sql
SELECT count(*) FROM customers c WHERE NOT EXISTS (SELECT 1 FROM orders o WHERE o.customer_id = c.id);
SELECT count(*) FROM customers c LEFT JOIN orders o ON o.customer_id = c.id WHERE o.id IS NULL;
SELECT count(*) FROM customers c WHERE c.id NOT IN (SELECT referred_by FROM customers);
```

The last one returns **0** even though most customers referred nobody. Why?

Tables: `customers`, `products`, `orders`, `order_items`, `employees`, `events`, `bookings` (see `sql/schema/01-schema.sql`, or `\d+ orders` in psql).

## Workflow

```
lc sql up               # once: start + seed postgres
$EDITOR answer.sql      # write ONE read-only query (all output of the file is compared)
lc sql run 204          # see your output
lc test 204             # compare with the expected result
lc sql explain 204      # EXPLAIN ANALYZE your query
```

## Think about

- `x NOT IN (list)` is `x <> a AND x <> b AND ...`. If the list contains a `NULL`, one term is
  `NULL`, so the whole predicate is never true. **Never use `NOT IN` with a subquery on a
  nullable column.** Use `NOT EXISTS`.
- Postgres plans `NOT EXISTS` as a **Hash Anti Join**. `EXPLAIN` it and look.
