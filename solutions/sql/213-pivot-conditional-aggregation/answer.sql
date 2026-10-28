SELECT extract(year FROM created_at)::int AS year,
       count(*) FILTER (WHERE status = 'pending') AS pending,
       count(*) FILTER (WHERE status = 'paid') AS paid,
       count(*) FILTER (WHERE status = 'shipped') AS shipped,
       count(*) FILTER (WHERE status = 'cancelled') AS cancelled,
       count(*) FILTER (WHERE status = 'refunded') AS refunded,
       count(*) AS total
FROM orders
GROUP BY 1
ORDER BY 1;
