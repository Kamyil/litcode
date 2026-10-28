First get distinct `(customer_id, month)` pairs.
---
`grp = (extract(year FROM month) * 12 + extract(month FROM month)) - row_number() OVER (PARTITION BY customer_id ORDER BY month)`.
---
Group by `(customer_id, grp)` to get islands, then pick the longest per customer with `DISTINCT ON` or `row_number()`.
