# 208 · Gaps And Islands

> **Difficulty:** Hard · **Topics:** gaps & islands, row_number difference trick

For the 50 power users (`customer_id <= 50`), find each customer's **longest streak of
consecutive calendar months** with at least one order (any status).

Columns: `customer_id, streak_months, streak_start` (`date`, first month of the longest
streak; if several streaks tie, the earliest) — ordered by `streak_months` descending,
then `customer_id`. Return only the top 10.

Tables: `customers`, `products`, `orders`, `order_items`, `employees`, `events`, `bookings` (see `sql/schema/01-schema.sql`, or `\d+ orders` in psql).

## Workflow

```
lc sql up               # once: start + seed postgres
$EDITOR answer.sql      # write ONE read-only query (all output of the file is compared)
lc sql run 208          # see your output
lc test 208             # compare with the expected result
lc sql explain 208      # EXPLAIN ANALYZE your query
```

## Think about

- The classic trick: for consecutive values, `value - row_number()` is constant within an
  "island". With months: `month - (row_number() * interval '1 month')` or convert months
  to integers (`year * 12 + month`) and subtract `row_number()`.
- This pattern solves "consecutive login days", "uninterrupted uptime", "seat ranges" etc.
  A favorite in SQL interviews.
