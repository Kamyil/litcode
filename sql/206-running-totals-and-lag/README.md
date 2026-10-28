# 206 · Running Totals And Lag

> **Difficulty:** Medium · **Topics:** window frames, LAG, date_trunc

Monthly revenue (sum of `orders.total` for `paid` + `shipped` orders) for **2024**, with
a running year-to-date total and the month-over-month change in percent.

Columns: `month` (a `date`, first day of month), `revenue`, `ytd`, `mom_pct` (rounded to
2 decimals, `NULL` for January) — ordered by `month`.

Tables: `customers`, `products`, `orders`, `order_items`, `employees`, `events`, `bookings` (see `sql/schema/01-schema.sql`, or `\d+ orders` in psql).

## Workflow

```
lc sql up               # once: start + seed postgres
$EDITOR answer.sql      # write ONE read-only query (all output of the file is compared)
lc sql run 206          # see your output
lc test 206             # compare with the expected result
lc sql explain 206      # EXPLAIN ANALYZE your query
```

## Think about

- `sum(revenue) OVER (ORDER BY month)` is a running total. The default frame is
  `RANGE BETWEEN UNBOUNDED PRECEDING AND CURRENT ROW` — with duplicate `ORDER BY` values,
  `RANGE` includes all peers. `ROWS` doesn't. Know the difference.
- `lag(revenue) OVER (ORDER BY month)` gives the previous row.
- `date_trunc('month', created_at)` returns a `timestamptz` in the **session time zone**.
  Month boundaries depend on `TimeZone`! (Here the container runs in UTC.)
