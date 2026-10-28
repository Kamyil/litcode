# 205 · Top N Per Group Window

> **Difficulty:** Medium · **Topics:** window functions, ROW_NUMBER vs RANK vs DENSE_RANK

For each category, the top 3 products by total quantity sold (over all orders, any status).

Columns: `category, product_id, name, units, rnk` where `rnk` is the position within the
category (1, 2, 3) using `ROW_NUMBER` with ties broken by `product_id` — ordered by
`category, rnk`.

Tables: `customers`, `products`, `orders`, `order_items`, `employees`, `events`, `bookings` (see `sql/schema/01-schema.sql`, or `\d+ orders` in psql).

## Workflow

```
lc sql up               # once: start + seed postgres
$EDITOR answer.sql      # write ONE read-only query (all output of the file is compared)
lc sql run 205          # see your output
lc test 205             # compare with the expected result
lc sql explain 205      # EXPLAIN ANALYZE your query
```

## Think about

- Window functions run **after** `GROUP BY`, so you can rank an aggregate:
  `row_number() OVER (PARTITION BY category ORDER BY sum(quantity) DESC)`.
- You can't filter a window result in the same `WHERE` — wrap it in a subquery/CTE.
- `ROW_NUMBER` (1,2,3,4), `RANK` (1,2,2,4), `DENSE_RANK` (1,2,2,3): know which one the
  question wants. "Top 3 products" with ties is ambiguous — ask the interviewer!
