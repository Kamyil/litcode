Two rolling variables: `prev2`, `prev1`. `cur = max(prev1, prev2 + x)`.
---
Circle: handle length 1 separately, then `max(rob(nums.slice(1)), rob(nums.slice(0, -1)))`.
