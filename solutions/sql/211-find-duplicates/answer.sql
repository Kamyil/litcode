SELECT id, email, keep_id
FROM (
    SELECT id, email,
           row_number() OVER w AS rn,
           first_value(id) OVER w AS keep_id
    FROM customers
    WINDOW w AS (PARTITION BY lower(trim(email)) ORDER BY id)
) d
WHERE rn > 1
ORDER BY id;
