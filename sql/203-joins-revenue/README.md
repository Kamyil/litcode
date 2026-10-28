# 203 · Joins Revenue

> **Difficulty:** Easy · **Topics:** INNER JOIN, multi-table aggregation

Revenue per product category in **2025**, counting only orders with status `paid` or
`shipped`. Revenue = `sum(quantity * unit_price)`.

Columns: `category, orders, revenue` where `orders` is the number of **distinct** orders
that contained at least one product of that category — ordered by `revenue` descending.

Tables: `customers`, `products`, `orders`, `order_items`, `employees`, `events`, `bookings` (see `sql/schema/01-schema.sql`, or `\d+ orders` in psql).

## Workflow

```
lc sql up               # once: start + seed postgres
$EDITOR answer.sql      # write ONE read-only query (all output of the file is compared)
lc sql run 203          # see your output
lc test 203             # compare with the expected result
lc sql explain 203      # EXPLAIN ANALYZE your query
```

## Think about

- Filter on a timestamp range with a half-open interval:
  `created_at >= '2025-01-01' AND created_at < '2026-01-01'`. Avoid
  `extract(year FROM created_at) = 2025` — it can't use an index on `created_at`
  (no index exists yet, but build the habit; see lab 253).
- Joining `orders` to `order_items` multiplies rows. That's why `count(DISTINCT o.id)` is needed.
