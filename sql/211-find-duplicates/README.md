# 211 · Find Duplicates

> **Difficulty:** Medium · **Topics:** normalization, window functions, data cleanup

Some customers signed up twice with the same email differing only in **letter case and
surrounding whitespace** (e.g. `user97@...` and `User97@... `).

List the rows you would **delete** to keep only the **oldest account** (lowest `id`) per
normalized email (`lower(trim(email))`).

Columns: `id, email, keep_id` (the id that survives) — ordered by `id`.

Tables: `customers`, `products`, `orders`, `order_items`, `employees`, `events`, `bookings` (see `sql/schema/01-schema.sql`, or `\d+ orders` in psql).

## Workflow

```
lc sql up               # once: start + seed postgres
$EDITOR answer.sql      # write ONE read-only query (all output of the file is compared)
lc sql run 211          # see your output
lc test 211             # compare with the expected result
lc sql explain 211      # EXPLAIN ANALYZE your query
```

## Think about

- `row_number() OVER (PARTITION BY lower(trim(email)) ORDER BY id)` numbers each group;
  everything with `rn > 1` is a duplicate. `first_value(id) OVER (...)` gives the survivor.
- The real fix: a unique **expression index** `CREATE UNIQUE INDEX ON customers (lower(trim(email)))`
  — or the `citext` type. You can't create it until the duplicates are gone!
- In production you'd also have to re-point foreign keys (`orders.customer_id`) before deleting.
