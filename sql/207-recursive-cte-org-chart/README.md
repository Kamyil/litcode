# 207 · Recursive Cte Org Chart

> **Difficulty:** Medium · **Topics:** WITH RECURSIVE, hierarchies, path arrays

For every **VP**, the total number of people in their organization (all levels below
them, not counting the VP) and the total salary of that organization.

Columns: `vp_id, department, headcount, total_salary` — ordered by `vp_id`.

Tables: `customers`, `products`, `orders`, `order_items`, `employees`, `events`, `bookings` (see `sql/schema/01-schema.sql`, or `\d+ orders` in psql).

## Workflow

```
lc sql up               # once: start + seed postgres
$EDITOR answer.sql      # write ONE read-only query (all output of the file is compared)
lc sql run 207          # see your output
lc test 207             # compare with the expected result
lc sql explain 207      # EXPLAIN ANALYZE your query
```

## Think about

- A recursive CTE = anchor query `UNION ALL` recursive part that references the CTE.
  It runs until the recursive part returns no rows (it's iteration, not recursion).
- Carry extra columns through the recursion: here the VP's id ("root") so you can group by it.
- Cycles in data (A manages B manages A) make it loop forever. Postgres 14+ has
  `CYCLE id SET is_cycle USING path`. Older trick: carry an `ARRAY[id]` path and stop if `id = ANY(path)`.
- Alternative designs for trees in SQL: `ltree` extension, nested sets, closure tables.
  Know at least the names and trade-offs.
