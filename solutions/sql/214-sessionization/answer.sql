WITH flagged AS (
    SELECT customer_id,
           CASE WHEN lag(occurred_at) OVER w IS NULL
                  OR occurred_at - lag(occurred_at) OVER w > interval '30 minutes'
                THEN 1 ELSE 0 END AS new_session
    FROM events
    WHERE customer_id <= 20
    WINDOW w AS (PARTITION BY customer_id ORDER BY occurred_at, id)
)
SELECT customer_id,
       sum(new_session) AS sessions,
       count(*) AS events,
       round(count(*)::numeric / sum(new_session), 2) AS avg_events_per_session
FROM flagged
GROUP BY customer_id
ORDER BY customer_id;
