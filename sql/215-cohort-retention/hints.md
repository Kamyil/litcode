CTE 1: customer → cohort month. CTE 2: distinct (customer, order month) with the offset.
---
`count(DISTINCT customer_id) FILTER (WHERE offset = 1)` etc., divided by the cohort size.
