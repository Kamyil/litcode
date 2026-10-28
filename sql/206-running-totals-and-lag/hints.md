Aggregate per month in a CTE, then apply windows in the outer query.
---
`round(100.0 * (revenue - lag(revenue) OVER w) / lag(revenue) OVER w, 2)` with `WINDOW w AS (ORDER BY month)`.
