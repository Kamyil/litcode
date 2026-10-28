SELECT status,
       count(*) AS orders,
       round(avg(total), 2) AS avg_total,
       round((percentile_cont(0.5) WITHIN GROUP (ORDER BY total))::numeric, 2) AS median_total,
       round((percentile_cont(0.95) WITHIN GROUP (ORDER BY total))::numeric, 2) AS p95_total
FROM orders
GROUP BY status
ORDER BY orders DESC;
