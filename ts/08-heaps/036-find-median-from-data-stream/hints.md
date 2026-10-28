Always push into `low` (max-heap) first, then move `low`'s top into `high` (min-heap).
---
If `high.size > low.size`, move `high`'s top back into `low`. Now `low.size` is equal or one bigger.
---
Median = odd count ? `low.peek()` : `(low.peek() + high.peek()) / 2`.
