Anchor: direct reports of each VP, carrying `manager_id AS vp_id`.
---
Recursive step: `JOIN employees e ON e.manager_id = org.id`, keep `org.vp_id`.
