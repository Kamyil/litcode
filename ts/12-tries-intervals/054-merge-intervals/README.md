# 054 · Merge & Insert Intervals

> **Difficulty:** Medium · **Topics:** sorting, sweep · **Target:** O(n log n) / O(n)

- `merge(intervals)`: merge all overlapping `[start, end]` intervals (touching ones like
  `[1,4]` and `[4,5]` count as overlapping). Return them sorted by start.
- `insert(intervals, newInterval)`: `intervals` is already sorted and non-overlapping;
  insert `newInterval`, merging as needed, in **O(n)**.

## Think about

- Sort by start; then each interval either extends the last merged one or starts a new one.
- Postgres has this built in: range types (`tstzrange`, `int4range`), the `&&` overlap
  operator, `range_agg()` to merge, and **exclusion constraints** with a GiST index to forbid
  overlapping bookings: `EXCLUDE USING gist (room WITH =, during WITH &&)`. (SQL track 2xx.)
