WITH months AS (
    SELECT DISTINCT customer_id, date_trunc('month', created_at)::date AS month
    FROM orders
    WHERE customer_id <= 50
), islands AS (
    SELECT customer_id, month,
           (extract(year FROM month) * 12 + extract(month FROM month))
               - row_number() OVER (PARTITION BY customer_id ORDER BY month) AS grp
    FROM months
), streaks AS (
    SELECT customer_id, count(*) AS streak_months, min(month) AS streak_start
    FROM islands
    GROUP BY customer_id, grp
), best AS (
    SELECT DISTINCT ON (customer_id) customer_id, streak_months, streak_start
    FROM streaks
    ORDER BY customer_id, streak_months DESC, streak_start
)
SELECT customer_id, streak_months, streak_start
FROM best
ORDER BY streak_months DESC, customer_id
LIMIT 10;
