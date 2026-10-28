SELECT country,
       count(*) AS customers,
       count(*) FILTER (WHERE plan = 'pro') AS pro,
       round(100.0 * count(*) FILTER (WHERE plan = 'pro') / count(*), 1) AS pro_pct
FROM customers
GROUP BY country
HAVING count(*) >= 10000
ORDER BY customers DESC;
