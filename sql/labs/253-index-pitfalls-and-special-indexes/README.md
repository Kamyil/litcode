# 253 · Lab: Why Isn't My Index Used? (+ GIN, trigram, expression indexes)

> **Difficulty:** Medium · **Topics:** sargability, expression indexes, LIKE, GIN for jsonb, pg_trgm · **Mode:** psql

"I added an index and Postgres ignores it" is the most common database interview question
there is. Each step shows one cause. Answers: `lc solution 253`.

## 1. Functions on the column

```sql
CREATE INDEX customers_created_at_idx ON customers (created_at);
EXPLAIN ANALYZE SELECT * FROM customers WHERE date(created_at) = '2024-05-01';
EXPLAIN ANALYZE SELECT * FROM customers WHERE created_at >= '2024-05-01' AND created_at < '2024-05-02';
```

- **Q1.** Why can't the first query use the index? Rewrite rule?

```sql
EXPLAIN ANALYZE SELECT * FROM customers WHERE lower(email) = 'user97@proton.me';
CREATE INDEX customers_lower_email_idx ON customers (lower(email));
ANALYZE customers;
EXPLAIN ANALYZE SELECT * FROM customers WHERE lower(email) = 'user97@proton.me';
```

- **Q2.** What's an expression index, and why did we run `ANALYZE` after creating it?

## 2. Type mismatches

```sql
EXPLAIN SELECT * FROM orders WHERE customer_id = 42;        -- int = int
EXPLAIN SELECT * FROM orders WHERE customer_id = 42.0;      -- int = numeric ?
EXPLAIN SELECT * FROM orders WHERE customer_id::text = '42';
```

- **Q3.** Which ones use `orders_customer_id_idx` and why not the others? Where do these sneak in
  from in real apps?

## 3. LIKE and collations

```sql
CREATE INDEX products_name_idx ON products (name);
EXPLAIN ANALYZE SELECT * FROM products WHERE name LIKE 'Ultra Lamp%';
SELECT datcollate FROM pg_database WHERE datname = current_database();
```

- **Q4.** If the database collation isn't `C`, a plain B-tree can't serve `LIKE 'prefix%'`. Why? Fix:

```sql
CREATE INDEX products_name_pattern_idx ON products (name text_pattern_ops);
EXPLAIN ANALYZE SELECT * FROM products WHERE name LIKE 'Ultra Lamp%';
```

## 4. Leading wildcards: trigram GIN

```sql
EXPLAIN ANALYZE SELECT * FROM products WHERE name ILIKE '%lamp 12%';
CREATE INDEX products_name_trgm_idx ON products USING gin (name gin_trgm_ops);
EXPLAIN ANALYZE SELECT * FROM products WHERE name ILIKE '%lamp 12%';
SELECT show_trgm('lamp');
SELECT name, similarity(name, 'Ultra Lampp 123') AS sim FROM products ORDER BY name <-> 'Ultra Lampp 123' LIMIT 5;
```

- **Q5.** How does a trigram index answer a `%...%` search? What is GIN, structurally?
- **Q6.** The last query uses `<->` (distance). Which index type supports *ordering* by it, GIN or GiST?

## 5. JSONB: containment vs extraction

```sql
CREATE TABLE ev AS SELECT * FROM events;   -- private copy
EXPLAIN ANALYZE SELECT count(*) FROM ev WHERE payload @> '{"device": "tablet"}';
CREATE INDEX ev_payload_gin ON ev USING gin (payload jsonb_path_ops);
EXPLAIN ANALYZE SELECT count(*) FROM ev WHERE payload @> '{"device": "tablet"}';
EXPLAIN ANALYZE SELECT count(*) FROM ev WHERE payload->>'device' = 'tablet';
CREATE INDEX ev_device_idx ON ev ((payload->>'device'));
ANALYZE ev;
EXPLAIN ANALYZE SELECT count(*) FROM ev WHERE payload->>'device' = 'tablet';
SELECT indexrelname, pg_size_pretty(pg_relation_size(indexrelid)) FROM pg_stat_user_indexes WHERE relname = 'ev';
```

- **Q7.** Which operator does each index serve? Compare sizes. Is either worth it for a value that
  matches ~14% of rows?

## 6. OR across columns, and low-cardinality columns

```sql
EXPLAIN ANALYZE SELECT * FROM orders WHERE customer_id = 42 OR created_at >= '2025-12-31';
CREATE INDEX orders_status_idx ON orders (status);
EXPLAIN ANALYZE SELECT * FROM orders WHERE status = 'shipped';
```

- **Q8.** How does Postgres combine two indexes for an `OR`? (Look for `BitmapOr`.)
- **Q9.** Why is `orders_status_idx` useless for `status = 'shipped'` but fine for `status = 'refunded'`?

## Cleanup

```sql
DROP TABLE ev;
DROP INDEX customers_created_at_idx, customers_lower_email_idx, products_name_idx,
           products_name_pattern_idx, products_name_trgm_idx, orders_status_idx;
```
