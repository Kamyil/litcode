# 406 · Answers

**1.** `NULL, true, NULL, true`. Comparisons with NULL yield NULL ("unknown"). `IS [NOT] DISTINCT FROM` is
the NULL-safe comparison.

**2.** All rows; rows where `referred_by` is not NULL; distinct non-NULL referrers. Aggregates other than
`count(*)` ignore NULLs.

**3.** 2. `avg` ignores NULLs: (1 + 3) / 2. Wrap with `COALESCE(x, 0)` if NULL should count as zero.

**4.** `WHERE` runs before grouping, so the aggregate alias `n` doesn't exist yet. Use
`HAVING count(*) > 100`. Logical order: FROM/JOIN → WHERE → GROUP BY → aggregates → HAVING → window functions →
SELECT list (aliases) → DISTINCT → ORDER BY (can use aliases) → LIMIT/OFFSET.

**5.** Zero rows: `1 <> 2 AND 1 <> 3 AND 1 <> NULL` = `true AND true AND NULL` = NULL. `1 IN (1, NULL)` is true
(`1 = 1 OR ...`). `2 IN (1, NULL)` would be NULL, not false.

**6.** The `WHERE o.status = 'paid'` is applied after the join. For customers with no orders, `o.status` is
NULL, so the condition is not true and those rows are removed: the LEFT JOIN has become an INNER JOIN. Put the
condition in the `ON` clause (`ON o.customer_id = c.id AND o.status = 'paid'`) to keep all customers.

**7.** `2` (integer division), `2.5000000000000000`, `2.5000000000000000`, `3` (numeric rounds half away
from zero), `4`, `2` (float8 rounds half to even, following the platform's rint). Integer division and
float rounding are classic bugs, so use `numeric` for money.

**8.** 02:30 doesn't exist on that date (DST jump from 02:00 to 03:00). Postgres interprets it as 03:30
CEST (it shifts forward). `timestamptz` does **not** store a zone: it stores a UTC instant and converts
to/from the session `TimeZone` on input and output. `timestamp` (without time zone) stores a wall-clock reading with no
zone at all. Use `timestamptz` for real events.

**9.** `UNION` removes duplicates (a sort or hash over the whole result), `UNION ALL` just appends. Use
`UNION ALL` unless you need dedup. It's cheaper and can stream.

**10.** One row per customer: the first row of each group according to `ORDER BY`, i.e. each customer's
latest order. Postgres needs the ORDER BY to group equal DISTINCT ON values together and to define which row
is "first". Ordering by other expressions first would make the result ill-defined.

**11.** Never guaranteed without `ORDER BY`, not from a table, not from a CTE or subquery, not after `GROUP BY`
(hash aggregation returns arbitrary order), not across parallel plans. Even "it always came back sorted"
changes with plans, synchronized seq scans (`synchronize_seqscans`), or updates moving rows.

**12.** `'2025-01-31'` means `2025-01-31 00:00:00`, so everything on Jan 31 after midnight is excluded. Use a
half-open range: `created_at >= '2025-01-01' AND created_at < '2025-02-01'`.

**13.** `COALESCE(nickname, name)` returns the first non-NULL (defaults, display fallbacks). `NULLIF(denominator, 0)`
avoids division by zero: `x / NULLIF(y, 0)` yields NULL instead of an error. It also turns sentinel
values (`''`, `-1`) into NULL.

**14.** `WHERE` filters rows before grouping. `HAVING` filters groups after aggregation. `FILTER (WHERE ...)`
restricts which rows feed one particular aggregate (conditional aggregation). `QUALIFY` (Snowflake,
BigQuery, DuckDB) filters on window function results. In Postgres, wrap the query in a subquery or CTE and filter
the window column in the outer `WHERE`.

**15.** Joining one order to its N items repeats the order row N times ("fan-out"). `count(*)` counts items, not
orders, and `sum(o.total)` adds each order's total N times, which silently inflates revenue. Fix: aggregate the child table first
(subquery or CTE grouped by `order_id`) and then join, or use `count(DISTINCT ...)` (doesn't fix sums of
non-unique values). One of the most common real-world reporting bugs.
