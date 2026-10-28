Hours at speed `k` = sum of `Math.ceil(p / k)`.
---
Search `lo = 1, hi = max(piles)`. If `hours(mid) <= h`, `hi = mid` else `lo = mid + 1`.
