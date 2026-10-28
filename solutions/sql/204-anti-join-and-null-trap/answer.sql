SELECT c.id, c.email, c.created_at
FROM customers c
WHERE c.country = 'JP'
  AND NOT EXISTS (SELECT 1 FROM orders o WHERE o.customer_id = c.id)
ORDER BY c.id;
