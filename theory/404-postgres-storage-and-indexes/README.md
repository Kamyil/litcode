# 404 · Quiz: Postgres Storage & Indexes

> **Difficulty:** Medium · **Topics:** pages, tuples, TOAST, index types, planner basics · **Mode:** `lc solution 404`

Best after labs 251–253 and sys/108–109.

**1.** What's the default page size? What is a `ctid`, and why can't it be used as a permanent row ID?

**2.** A row has a 5 MB `text` column. Where is it stored? What's TOAST and when does it kick in?

**3.** Why does Postgres store table data in a **heap** (unordered) rather than clustered by primary key
like MySQL/InnoDB? Name one advantage and one disadvantage. What does the `CLUSTER` command do — and does it last?

**4.** Name the index types: B-tree, Hash, GIN, GiST, SP-GiST, BRIN. One typical use case for each.

**5.** Does an index entry in Postgres know whether the row it points to is visible to your transaction?
Consequence for `SELECT count(*)`?

**6.** You have an index on `(last_name, first_name)`. Which queries can use it efficiently?
(a) `WHERE last_name = 'Kowalski'` (b) `WHERE first_name = 'Jan'` (c) `WHERE last_name = 'K' ORDER BY first_name`
(d) `WHERE last_name LIKE 'Kow%'` (e) `ORDER BY last_name DESC, first_name DESC LIMIT 10` (f) `ORDER BY last_name, first_name DESC`

**7.** Index scan vs bitmap heap scan vs index-only scan vs seq scan: when is each chosen?

**8.** Why might Postgres not use your index on a 1,000-row table even for an exact match?

**9.** What's the difference between `UNIQUE` constraint, unique index, and primary key? Can a unique index
contain multiple NULLs? (And what does `NULLS NOT DISTINCT` change, PG15+?)

**10.** Why are foreign key columns on the **referencing** side not indexed automatically, and when do you need that index?
(Hint: `DELETE FROM customers WHERE id = 5` with 1M orders.)

**11.** Random UUIDv4 primary keys vs bigint identity vs UUIDv7: effect on B-tree inserts, index size, cache hit rate?

**12.** What's index bloat? How do you detect and fix it without downtime?

**13.** `EXPLAIN` estimates 10 rows, `EXPLAIN ANALYZE` shows 500,000. List possible causes and fixes.

**14.** What does `VACUUM` have to do with index-only scans?
