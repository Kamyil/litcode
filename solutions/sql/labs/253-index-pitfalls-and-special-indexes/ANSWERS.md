# 253 · Answers

**Q1.** The index stores `created_at` values. `date(created_at)` is a different expression, and
Postgres can't invert arbitrary functions to turn it into a range on the column. Rewrite rule:
keep the column bare on one side (*sargable* predicates): `col >= a AND col < b`. Otherwise
create an index on exactly the expression used.

**Q2.** An index on the result of an expression (`lower(email)`). Queries must use the same expression
to match. `ANALYZE` gathers statistics on the expression's values, which the planner otherwise
doesn't have (it would guess the selectivity).

**Q3.** `= 42` uses it. `= 42.0` compares `int` with `numeric`, so Postgres casts the *column* to
numeric (`customer_id::numeric = 42.0`) and the btree on int can't be used. `customer_id::text`
is a function on the column, so no index either. Real-world sources: ORMs binding parameters with the
wrong type, JSON values coming in as strings, comparing `varchar` to `text[]` etc. Look for
`Filter: ((customer_id)::numeric = 42.0)` in plans.

**Q4.** In linguistic collations (e.g. `en_US.UTF-8`) sort order isn't byte order, and characters can
compare equal or sort in surprising ways, so "all strings starting with X" isn't guaranteed to be a
contiguous index range. `text_pattern_ops` builds the B-tree with byte-wise (C) comparison, which
supports prefix `LIKE` (but not normal `<`/`ORDER BY` in the linguistic collation). If the
database uses `C` collation, the plain index works too. (The official docker image uses `en_US.utf8`.)

**Q5.** `pg_trgm` splits strings into 3-character pieces (`"  l"`, `" la"`, `lam`, `amp`, `mp `).
The GIN index maps each trigram to the list of rows containing it. A `%lamp 12%` search
extracts the pattern's trigrams, intersects the posting lists, and then rechecks the candidate rows.
GIN = Generalized **Inverted** Index: key → sorted posting list (or posting tree) of TIDs, the same
structure as a search engine's inverted index. It's great for "contains" queries on arrays, jsonb,
full text and trigrams, and slower to update (it uses a pending list, `fastupdate`).

**Q6.** GiST (`gist_trgm_ops`) supports KNN ordering (`ORDER BY name <-> 'x' LIMIT n`) because it's a
tree with bounding information. GIN can't produce ordered results. GIN is usually faster for plain
filtering, while GiST is smaller and supports distance ordering.

**Q7.** `jsonb_path_ops` GIN serves `@>` (and jsonpath `@?`/`@@`), but not `->>` equality. The
expression B-tree serves `payload->>'device' = ...` only. The expression index is much smaller. For a
value matching ~14% of rows, a bitmap scan can still beat a seq scan on a wide table, but for `count(*)`
the gain is small. Indexes on low-selectivity values rarely pay off. The GIN index shines for
selective, varied keys (`@> '{"path": "/p/123"}'`).

**Q8.** Two Bitmap Index Scans (one per index), combined with `BitmapOr` (a union of TID bitmaps),
then one Bitmap Heap Scan. `BitmapAnd` similarly intersects for `AND` across separate indexes. That's
why several single-column indexes can sometimes replace a composite one (less efficiently).

**Q9.** `shipped` is 70% of the table: an index scan would be slower than a seq scan, so the planner
ignores the index (statistics know the most-common-value frequencies). `refunded` (2%) is selective
enough. For skewed columns, a **partial index** on the rare values is the better design (lab 252).
