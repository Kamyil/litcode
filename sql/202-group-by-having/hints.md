`HAVING count(*) >= 10000`.
---
`round(100.0 * count(*) FILTER (WHERE plan = 'pro') / count(*), 1)`.
