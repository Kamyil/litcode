# 215 · Cohort Retention

> **Difficulty:** Hard · **Topics:** cohorts, date arithmetic, conditional aggregation

Monthly signup cohorts for **2024** (by `customers.created_at`). For each cohort: its
size, and the percentage of the cohort that placed at least one order (any status) in
month 1, 2 and 3 **after** the signup month (month 0 = the signup month itself).

Columns: `cohort` (date, first of month), `size`, `m1_pct, m2_pct, m3_pct` (rounded to 1
decimal) — ordered by `cohort`.

Tables: `customers`, `products`, `orders`, `order_items`, `employees`, `events`, `bookings` (see `sql/schema/01-schema.sql`, or `\d+ orders` in psql).

## Workflow

```
lc sql up               # once: start + seed postgres
$EDITOR answer.sql      # write ONE read-only query (all output of the file is compared)
lc sql run 215          # see your output
lc test 215             # compare with the expected result
lc sql explain 215      # EXPLAIN ANALYZE your query
```

## Think about

- Month offset: `(extract(year FROM age(order_month, cohort)) * 12 + extract(month FROM age(order_month, cohort)))`,
  or simpler with integer month numbers: `(y2 * 12 + m2) - (y1 * 12 + m1)`.
- Count **distinct customers** per (cohort, offset), not orders.
- This is a staple of product-analytics interviews; window functions aren't even needed.
