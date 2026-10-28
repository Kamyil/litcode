SELECT c.id AS customer_id, c.name, o.id AS order_id, o.created_at, o.total
FROM customers c
LEFT JOIN LATERAL (
    SELECT id, created_at, total
    FROM orders
    WHERE customer_id = c.id
    ORDER BY created_at DESC
    LIMIT 2
) o ON true
WHERE c.id <= 15
ORDER BY c.id, o.created_at DESC;
