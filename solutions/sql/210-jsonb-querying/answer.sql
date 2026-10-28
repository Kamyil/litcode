SELECT payload->>'device' AS device,
       count(*) AS events,
       round(avg((payload->>'ms')::int), 1) AS avg_ms,
       count(DISTINCT customer_id) AS customers
FROM events
WHERE type = 'add_to_cart'
GROUP BY payload->>'device'
ORDER BY events DESC;
