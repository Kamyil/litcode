# 202 · Group By Having

> **Difficulty:** Easy · **Topics:** GROUP BY, HAVING, aggregates, FILTER

Per country: the number of customers, the number on the `pro` plan, and the share of
`pro` customers as a percentage rounded to 1 decimal. Only countries with **at least
10,000** customers.

Columns: `country, customers, pro, pro_pct` — ordered by `customers` descending.

Tables: `customers`, `products`, `orders`, `order_items`, `employees`, `events`, `bookings` (see `sql/schema/01-schema.sql`, or `\d+ orders` in psql).

## Workflow

```
lc sql up               # once: start + seed postgres
$EDITOR answer.sql      # write ONE read-only query (all output of the file is compared)
lc sql run 202          # see your output
lc test 202             # compare with the expected result
lc sql explain 202      # EXPLAIN ANALYZE your query
```

## Think about

- `count(*) FILTER (WHERE plan = 'pro')` is the clean, Postgres-native way to do conditional
  aggregates (the portable version is `sum(CASE WHEN ... THEN 1 ELSE 0 END)`).
- `WHERE` filters rows **before** grouping; `HAVING` filters groups **after**. Which one can use an index?
- Integer division: `pro / customers` is 0 for integers. Cast to `numeric` first.
