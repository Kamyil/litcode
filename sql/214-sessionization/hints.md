`CASE WHEN lag(occurred_at) OVER w IS NULL OR occurred_at - lag(occurred_at) OVER w > interval '30 minutes' THEN 1 ELSE 0 END`.
---
Then `count(DISTINCT session_no)` per customer, or `sum(flag)`.
