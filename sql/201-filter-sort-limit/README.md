# 201 · Filter Sort Limit

> **Difficulty:** Easy · **Topics:** WHERE, ORDER BY, LIMIT, ties

The 10 most expensive **books** that are in stock (`stock > 0`) and not discontinued
(`attrs` has no `"discontinued": true`).

Columns: `id, name, price` — ordered by `price` descending, then `id` ascending (a
deterministic tie-breaker — always add one when you `LIMIT`).

Tables: `customers`, `products`, `orders`, `order_items`, `employees`, `events`, `bookings` (see `sql/schema/01-schema.sql`, or `\d+ orders` in psql).

## Workflow

```
lc sql up               # once: start + seed postgres
$EDITOR answer.sql      # write ONE read-only query (all output of the file is compared)
lc sql run 201          # see your output
lc test 201             # compare with the expected result
lc sql explain 201      # EXPLAIN ANALYZE your query
```

## Think about

- Without the `id` tie-breaker, rows with equal prices can come back in any order and
  `LIMIT` picks an arbitrary subset. Postgres gives **no** ordering guarantee without `ORDER BY`.
- `attrs->>'discontinued'` is `NULL` for most rows. What does `WHERE attrs->>'discontinued' <> 'true'`
  return for them? (Nothing! `NULL <> 'true'` is `NULL`, not true.) Use `IS DISTINCT FROM`
  or `NOT (attrs @> '{"discontinued": true}')`.
