# 212 · Lateral Latest Per Customer

> **Difficulty:** Medium · **Topics:** LATERAL joins, DISTINCT ON

For customers with `id <= 15`, their **two most recent orders** (any status). Customers
without orders must still appear once, with `NULL`s.

Columns: `customer_id, name, order_id, created_at, total` — ordered by `customer_id`,
then `created_at` descending.

Tables: `customers`, `products`, `orders`, `order_items`, `employees`, `events`, `bookings` (see `sql/schema/01-schema.sql`, or `\d+ orders` in psql).

## Workflow

```
lc sql up               # once: start + seed postgres
$EDITOR answer.sql      # write ONE read-only query (all output of the file is compared)
lc sql run 212          # see your output
lc test 212             # compare with the expected result
lc sql explain 212      # EXPLAIN ANALYZE your query
```

## Think about

- `LEFT JOIN LATERAL (SELECT ... WHERE o.customer_id = c.id ORDER BY created_at DESC LIMIT 2) o ON true`
  runs the subquery per outer row — with an index on `(customer_id, created_at DESC)`, each is a
  tiny index scan. This is the fastest top-N-per-group for "few groups, many rows".
- `DISTINCT ON (customer_id)` handles top-1 per group elegantly (Postgres-specific).
- Compare with the window-function approach (205): it must compute `row_number()` over all rows first.
