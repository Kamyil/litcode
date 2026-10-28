SELECT p.category,
       count(DISTINCT o.id) AS orders,
       sum(oi.quantity * oi.unit_price) AS revenue
FROM orders o
JOIN order_items oi ON oi.order_id = o.id
JOIN products p ON p.id = oi.product_id
WHERE o.status IN ('paid', 'shipped')
  AND o.created_at >= '2025-01-01' AND o.created_at < '2026-01-01'
GROUP BY p.category
ORDER BY revenue DESC;
