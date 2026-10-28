# 210 · Jsonb Querying

> **Difficulty:** Medium · **Topics:** jsonb operators, containment, jsonb_array_elements_text

For `add_to_cart` events: the number of events and the average `payload->>'ms'` per
device, plus the number of **distinct customers** per device.

Columns: `device, events, avg_ms` (rounded to 1 decimal), `customers` — ordered by `events` descending.

(Not graded, but do it: the 5 most common product **tags** across all products, using
`jsonb_array_elements_text(attrs->'tags')`.)

Tables: `customers`, `products`, `orders`, `order_items`, `employees`, `events`, `bookings` (see `sql/schema/01-schema.sql`, or `\d+ orders` in psql).

## Workflow

```
lc sql up               # once: start + seed postgres
$EDITOR answer.sql      # write ONE read-only query (all output of the file is compared)
lc sql run 210          # see your output
lc test 210             # compare with the expected result
lc sql explain 210      # EXPLAIN ANALYZE your query
```

## Think about

- `->` returns `jsonb`, `->>` returns `text`. Cast text to numbers: `(payload->>'ms')::int`.
- Containment `payload @> '{"device": "mobile"}'` can use a **GIN** index (`jsonb_path_ops`
  is smaller and faster for `@>`), while `payload->>'device' = 'mobile'` needs an
  **expression** index. Lab 253 has you measure both.
- When should something be a real column instead of a JSON key? (When you filter, join,
  or constrain on it — the planner has no statistics on JSON keys by default.)
