# 213 · Pivot Conditional Aggregation

> **Difficulty:** Easy · **Topics:** pivoting, FILTER, generate_series for missing rows

Order counts per **year** (rows) by **status** (columns).

Columns: `year, pending, paid, shipped, cancelled, refunded, total` — ordered by `year`.
`year` is an integer.

Tables: `customers`, `products`, `orders`, `order_items`, `employees`, `events`, `bookings` (see `sql/schema/01-schema.sql`, or `\d+ orders` in psql).

## Workflow

```
lc sql up               # once: start + seed postgres
$EDITOR answer.sql      # write ONE read-only query (all output of the file is compared)
lc sql run 213          # see your output
lc test 213             # compare with the expected result
lc sql explain 213      # EXPLAIN ANALYZE your query
```

## Think about

- SQL has no dynamic `PIVOT` in Postgres; you write one aggregate per column with `FILTER`.
  (`crosstab()` from `tablefunc` exists but is clunky.)
- If a year had no orders at all, it would be missing. The fix is to `LEFT JOIN` from
  `generate_series(2023, 2025)` — remember it for time-series charts with gaps.
