Sort. Fix `i`; set `l = i + 1`, `r = n - 1`; move `l` up if the sum is too small, `r` down if too big.
---
Skip duplicate anchors: `if (i > 0 && nums[i] === nums[i - 1]) continue`.
---
After recording a triplet, advance `l` past all equal values (and `r` likewise).
---
Early exit: once `nums[i] > 0`, no triplet starting there can sum to zero.
