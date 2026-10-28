WITH units AS (
    SELECT p.category, p.id AS product_id, p.name, sum(oi.quantity) AS units
    FROM order_items oi
    JOIN products p ON p.id = oi.product_id
    GROUP BY p.category, p.id, p.name
), ranked AS (
    SELECT *, row_number() OVER (PARTITION BY category ORDER BY units DESC, product_id) AS rnk
    FROM units
)
SELECT category, product_id, name, units, rnk
FROM ranked
WHERE rnk <= 3
ORDER BY category, rnk;
