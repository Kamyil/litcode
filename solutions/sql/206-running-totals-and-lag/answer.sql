WITH monthly AS (
    SELECT date_trunc('month', created_at)::date AS month, sum(total) AS revenue
    FROM orders
    WHERE status IN ('paid', 'shipped')
      AND created_at >= '2024-01-01' AND created_at < '2025-01-01'
    GROUP BY 1
)
SELECT month,
       revenue,
       sum(revenue) OVER w AS ytd,
       round(100.0 * (revenue - lag(revenue) OVER w) / lag(revenue) OVER w, 2) AS mom_pct
FROM monthly
WINDOW w AS (ORDER BY month)
ORDER BY month;
