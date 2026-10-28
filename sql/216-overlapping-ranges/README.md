# 216 · Overlapping Ranges

> **Difficulty:** Medium · **Topics:** range types, &&, self-joins, exclusion constraints

The hotel's `bookings` table has double bookings. For each room, the number of
**overlapping pairs** of bookings, and the total overlapping time in hours.

Columns: `room, overlapping_pairs, overlap_hours` (numeric, rounded to 1 decimal) — only
rooms with at least one overlap, ordered by `overlapping_pairs` descending, then `room`.

Tables: `customers`, `products`, `orders`, `order_items`, `employees`, `events`, `bookings` (see `sql/schema/01-schema.sql`, or `\d+ orders` in psql).

## Workflow

```
lc sql up               # once: start + seed postgres
$EDITOR answer.sql      # write ONE read-only query (all output of the file is compared)
lc sql run 216          # see your output
lc test 216             # compare with the expected result
lc sql explain 216      # EXPLAIN ANALYZE your query
```

## Think about

- `a.during && b.during` = overlap; `a.during * b.during` = the intersection range;
  `upper(r) - lower(r)` its length. Join each pair once: `a.id < b.id`.
- Prevention instead of detection (try it in psql after deleting overlaps in a copy):
  `ALTER TABLE bookings ADD EXCLUDE USING gist (room WITH =, during WITH &&);`
  (needs `btree_gist` for the `=` on text — already installed). Exclusion constraints are a
  Postgres superpower that comes up when interviewers ask "how do you prevent double booking?"
