# 209 · Percentiles And Distribution

> **Difficulty:** Medium · **Topics:** ordered-set aggregates, percentile_cont, width_bucket

Per order status: number of orders, average total, median total and 95th percentile
total (continuous percentile), all rounded to 2 decimals.

Columns: `status, orders, avg_total, median_total, p95_total` — ordered by `orders` descending.

Tables: `customers`, `products`, `orders`, `order_items`, `employees`, `events`, `bookings` (see `sql/schema/01-schema.sql`, or `\d+ orders` in psql).

## Workflow

```
lc sql up               # once: start + seed postgres
$EDITOR answer.sql      # write ONE read-only query (all output of the file is compared)
lc sql run 209          # see your output
lc test 209             # compare with the expected result
lc sql explain 209      # EXPLAIN ANALYZE your query
```

## Think about

- `percentile_cont(0.5) WITHIN GROUP (ORDER BY total)` interpolates; `percentile_disc`
  returns an actual value from the set. Means lie with skewed data — medians and p95/p99
  are what you report for latencies and money.
- These aggregates need to sort each group: on big data they're expensive. Approximate
  alternatives: t-digest / HyperLogLog extensions.
- Bonus (not graded): a histogram of totals with `width_bucket(total, 0, 2000, 20)`.
