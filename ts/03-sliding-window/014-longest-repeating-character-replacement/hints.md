Count letters inside the window (`Int32Array(26)`), and track `maxCount` = highest count seen.
---
If `(right - left + 1) - maxCount > k`, shrink by one from the left (decrement its count).
---
The answer only grows when `maxCount` grows, so a stale `maxCount` just keeps the window from
shrinking below the best size found so far — it never reports a size that wasn't achieved.
