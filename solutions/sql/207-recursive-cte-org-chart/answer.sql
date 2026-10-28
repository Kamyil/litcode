WITH RECURSIVE org AS (
    SELECT e.id, e.salary, e.manager_id AS vp_id
    FROM employees e
    JOIN employees vp ON vp.id = e.manager_id AND vp.title = 'VP'
    UNION ALL
    SELECT e.id, e.salary, org.vp_id
    FROM employees e
    JOIN org ON e.manager_id = org.id
)
SELECT org.vp_id, vp.department, count(*) AS headcount, sum(org.salary) AS total_salary
FROM org
JOIN employees vp ON vp.id = org.vp_id
GROUP BY org.vp_id, vp.department
ORDER BY org.vp_id;
