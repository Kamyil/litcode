-- Deterministic seed data (setseed) so everyone gets identical results.
-- Sizes: 200k customers, 10k products, 1M orders, ~3M order items, 1M events, 2k employees, 5k bookings.
SET client_min_messages = warning;
\o /dev/null
SELECT setseed(0.42);
\o

-- customers: skewed countries and plans; ~1% have a mixed-case duplicate email (exercise 211)
INSERT INTO customers (id, email, name, country, plan, referred_by, created_at)
SELECT g,
       'user' || g || '@' || (ARRAY['gmail.com', 'proton.me', 'example.org', 'fastmail.com'])[1 + g % 4],
       (ARRAY['Anna', 'Jan', 'Kasia', 'Piotr', 'Ola', 'Marek', 'Eva', 'Liam', 'Noah', 'Mia', 'Yuki', 'Ravi'])[1 + g % 12]
           || ' ' || (ARRAY['Nowak', 'Kowalski', 'Smith', 'Müller', 'Garcia', 'Rossi', 'Tanaka', 'Singh'])[1 + (g / 12) % 8],
       CASE WHEN r < 0.30 THEN 'PL' WHEN r < 0.50 THEN 'DE' WHEN r < 0.62 THEN 'US' WHEN r < 0.72 THEN 'GB'
            WHEN r < 0.80 THEN 'FR' WHEN r < 0.86 THEN 'ES' WHEN r < 0.91 THEN 'IT' WHEN r < 0.95 THEN 'NL'
            WHEN r < 0.98 THEN 'SE' ELSE 'JP' END,
       CASE WHEN r2 < 0.80 THEN 'free' WHEN r2 < 0.98 THEN 'pro' ELSE 'enterprise' END,
       CASE WHEN g > 1000 AND g % 7 = 0 THEN (g * 31) % (g - 1) + 1 END,
       timestamptz '2023-01-01' + (g / 200000.0) * interval '1095 days' + random() * interval '12 hours'
FROM (SELECT g, random() AS r, random() AS r2 FROM generate_series(1, 200000) g) s;

INSERT INTO customers (id, email, name, country, plan, created_at)
SELECT 200000 + row_number() OVER (ORDER BY c.id),
       upper(left(c.email, 1)) || substr(c.email, 2) || CASE WHEN c.id % 3 = 0 THEN ' ' ELSE '' END,
       c.name, c.country, 'free', c.created_at + interval '30 days'
FROM customers c
WHERE c.id % 97 = 0;

-- products
INSERT INTO products (id, name, category, price, stock, attrs)
SELECT g,
       (ARRAY['Ultra', 'Mini', 'Pro', 'Eco', 'Smart', 'Classic', 'Max', 'Lite'])[1 + g % 8] || ' '
           || (ARRAY['Widget', 'Gadget', 'Book', 'Lamp', 'Chair', 'Mug', 'Shirt', 'Phone', 'Kettle', 'Desk'])[1 + (g / 8) % 10]
           || ' ' || g,
       (ARRAY['books', 'electronics', 'home', 'garden', 'toys', 'clothing', 'sports', 'beauty'])[1 + (g * 7) % 8],
       round((5 + random() * 495)::numeric, 2),
       (random() * 500)::int,
       jsonb_build_object(
           'color', (ARRAY['red', 'green', 'blue', 'black', 'white'])[1 + g % 5],
           'weight_g', 100 + (g * 37) % 5000,
           'tags', (ARRAY['["new", "sale"]', '["eco"]', '["sale"]', '["premium", "new"]', '[]'])[1 + g % 5]::jsonb)
           || CASE WHEN g % 10 = 0 THEN '{"discontinued": true}'::jsonb ELSE '{}'::jsonb END
FROM generate_series(1, 10000) g;

-- orders: created_at increases with id (good for BRIN), skewed statuses, a few heavy customers
INSERT INTO orders (id, customer_id, status, created_at, shipped_at)
SELECT g,
       -- 50 power users with ~170 orders each, pausing for ~a month every few months (gaps & islands)
       CASE WHEN g % 100 = 0 AND (g / 50000 + (g / 100) % 50) % (3 + ((g / 100) % 50) % 4) <> 0 THEN 1 + (g / 100) % 50
            ELSE 1 + floor(random() * 200000)::int END,
       st,
       ts,
       CASE WHEN st = 'shipped' THEN ts + (1 + floor(random() * 10)) * interval '1 day' END
FROM (
    SELECT g,
           timestamptz '2023-01-01' + (g / 1000000.0) * interval '1095 days' + random() * interval '6 hours' AS ts,
           CASE WHEN r < 0.70 THEN 'shipped' WHEN r < 0.85 THEN 'paid' WHEN r < 0.93 THEN 'pending'
                WHEN r < 0.98 THEN 'cancelled' ELSE 'refunded' END AS st
    FROM (SELECT g, random() AS r FROM generate_series(1, 1000000) g) x
) s;

-- order_items: 1..5 distinct products per order, deterministic product choice
INSERT INTO order_items (order_id, product_id, quantity, unit_price)
SELECT o.id, p.id, 1 + floor(random() * 5)::int, p.price
FROM orders o
CROSS JOIN LATERAL generate_series(1, 1 + (o.id % 5)::int) k
JOIN products p ON p.id = ((o.id * 7919 + k * 104729) % 10000) + 1;

UPDATE orders o SET total = s.total
FROM (SELECT order_id, sum(quantity * unit_price) AS total FROM order_items GROUP BY order_id) s
WHERE s.order_id = o.id;

-- employees: a 5-level hierarchy (CEO -> VPs -> directors -> managers -> engineers/staff)
INSERT INTO employees (id, name, manager_id, department, title, salary, hired_at)
SELECT g,
       'Employee ' || g,
       CASE WHEN g = 1 THEN NULL
            WHEN g <= 6 THEN 1
            WHEN g <= 36 THEN 2 + (g - 7) % 5
            WHEN g <= 236 THEN 7 + (g - 37) % 30
            ELSE 37 + (g - 237) % 200 END,
       CASE WHEN g = 1 THEN 'exec'
            ELSE (ARRAY['engineering', 'sales', 'marketing', 'finance', 'support'])[1 + (
                CASE WHEN g <= 6 THEN g - 2
                     WHEN g <= 36 THEN (g - 7) % 5
                     WHEN g <= 236 THEN ((g - 37) % 30) % 5
                     ELSE (((g - 237) % 200) % 30) % 5 END)] END,
       CASE WHEN g = 1 THEN 'CEO' WHEN g <= 6 THEN 'VP' WHEN g <= 36 THEN 'Director'
            WHEN g <= 236 THEN 'Manager' ELSE 'Staff' END,
       CASE WHEN g = 1 THEN 400000 WHEN g <= 6 THEN 250000 WHEN g <= 36 THEN 180000
            WHEN g <= 236 THEN 120000 ELSE 60000 + (g * 7919) % 50000 END,
       date '2015-01-01' + (g * 13) % 3650
FROM generate_series(1, 2000) g;

-- events: clickstream in bursts ("sessions" of 1..9 events a few minutes apart, days between sessions).
-- ids follow time order (good for BRIN, lab 258).
INSERT INTO events (id, customer_id, type, payload, occurred_at)
SELECT row_number() OVER (ORDER BY ts, c), c, type, payload, ts
FROM (
    SELECT c, ts,
           (ARRAY['page_view', 'page_view', 'page_view', 'search', 'add_to_cart', 'checkout', 'login'])[1 + floor(random() * 7)::int] AS type,
           jsonb_build_object(
               'device', (ARRAY['mobile', 'desktop', 'tablet'])[1 + ((c + s) % 10 > 5)::int + ((c + s) % 10 > 8)::int],
               'path', '/p/' || (1 + (c * 31 + s * 7 + e) % 10000),
               'ms', (random() * 3000)::int) AS payload
    FROM generate_series(1, 5000) c
    CROSS JOIN generate_series(1, 40) s
    CROSS JOIN LATERAL generate_series(1, 1 + (c + s) % 9) e
    CROSS JOIN LATERAL (
        SELECT timestamptz '2025-01-01'
               + (s * 4) * interval '1 day' + (c % 1440) * interval '1 minute'
               + e * interval '6 minutes' + random() * interval '5 minutes'
               + CASE WHEN e > 5 AND (c + s) % 4 = 0 THEN interval '45 minutes' ELSE interval '0' END AS ts
    ) t
) x;

-- bookings: rooms with some overlapping (double-booked) reservations
INSERT INTO bookings (id, room, guest, during)
SELECT g,
       'room-' || (1 + g % 40),
       'guest-' || g,
       tstzrange(t, t + (1 + g % 4) * interval '1 day')
FROM (SELECT g, timestamptz '2025-06-01' + ((g / 40) * 3 + (g % 7 = 0)::int * 2) * interval '1 day' AS t
      FROM generate_series(1, 5000) g) s;

VACUUM ANALYZE;
