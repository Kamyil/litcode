WITH cohort AS (
    SELECT id AS customer_id, date_trunc('month', created_at)::date AS cohort
    FROM customers
    WHERE created_at >= '2024-01-01' AND created_at < '2025-01-01'
), activity AS (
    SELECT DISTINCT c.customer_id, c.cohort,
           (extract(year FROM o.created_at) * 12 + extract(month FROM o.created_at))
             - (extract(year FROM c.cohort) * 12 + extract(month FROM c.cohort)) AS month_offset
    FROM cohort c
    JOIN orders o ON o.customer_id = c.customer_id
), sizes AS (
    SELECT cohort, count(*) AS size FROM cohort GROUP BY cohort
)
SELECT s.cohort,
       s.size,
       round(100.0 * count(DISTINCT a.customer_id) FILTER (WHERE a.month_offset = 1) / s.size, 1) AS m1_pct,
       round(100.0 * count(DISTINCT a.customer_id) FILTER (WHERE a.month_offset = 2) / s.size, 1) AS m2_pct,
       round(100.0 * count(DISTINCT a.customer_id) FILTER (WHERE a.month_offset = 3) / s.size, 1) AS m3_pct
FROM sizes s
LEFT JOIN activity a ON a.cohort = s.cohort
GROUP BY s.cohort, s.size
ORDER BY s.cohort;
