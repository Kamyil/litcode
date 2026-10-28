# 406 · Quiz: SQL Semantics & Gotchas

> **Difficulty:** Medium · **Topics:** NULL logic, aggregation rules, evaluation order, joins, types · **Mode:** predict, then run in `lc sql psql`, then `lc solution 406`

Predict each result **before** running it.

**1.** `SELECT NULL = NULL, NULL IS NULL, NULL <> 1, NULL IS DISTINCT FROM 1;`

**2.** `SELECT count(*), count(referred_by), count(DISTINCT referred_by) FROM customers;` — what does each count?

**3.** `SELECT avg(x) FROM (VALUES (1), (NULL), (3)) t(x);` — 1.33 or 2?

**4.** Why does this fail, and how do you fix it?
```sql
SELECT customer_id, count(*) AS n FROM orders WHERE n > 100 GROUP BY customer_id;
```
Write down the **logical** order of evaluation of a SELECT's clauses.

**5.** `SELECT 1 WHERE 1 NOT IN (2, 3, NULL);` returns how many rows? And `1 IN (1, NULL)`?

**6.** A `LEFT JOIN` returns fewer rows than the left table had. Impossible? Look:
```sql
SELECT c.id, o.id FROM customers c LEFT JOIN orders o ON o.customer_id = c.id WHERE o.status = 'paid';
```
What happened, and where should the condition go?

**7.** `SELECT 5 / 2, 5 / 2.0, 5::numeric / 2, round(2.5), round(3.5), round(2.5::float8);`

**8.** `SELECT '2025-03-30 02:30'::timestamptz` in `Europe/Warsaw` — what's special? `timestamp` vs `timestamptz`: which one stores the time zone?

**9.** `UNION` vs `UNION ALL`: difference and performance implication?

**10.** `SELECT DISTINCT ON (customer_id) * FROM orders ORDER BY customer_id, created_at DESC;` — what does it return? Why must `ORDER BY` start with the DISTINCT ON expressions?

**11.** Is the order of rows from `SELECT * FROM t` guaranteed? What about inside a CTE, or after `GROUP BY`?

**12.** `WHERE created_at BETWEEN '2025-01-01' AND '2025-01-31'` on a `timestamptz` column — what bug lurks?

**13.** `COALESCE(a, b)` vs `NULLIF(a, b)` — one example each where they're the right tool.

**14.** What's the difference between `WHERE` and `HAVING` and `FILTER (WHERE ...)`, and `QUALIFY` (not in Postgres — what do you use instead)?

**15.** `SELECT count(*) FROM orders o JOIN order_items i ON i.order_id = o.id` vs `count(DISTINCT o.id)` — explain "fan-out" and how it silently breaks `sum(o.total)`.
