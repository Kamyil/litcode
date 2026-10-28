`lo = 0, hi = n`. `mid = (lo + hi) >>> 1`. If `nums[mid] < x` the answer is right of mid: `lo = mid + 1`; else `hi = mid`.
---
`upperBound` is the same with `<=` instead of `<`.
---
`countInRange = upperBound(hi) - lowerBound(lo)`.
