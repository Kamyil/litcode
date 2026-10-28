Step 1: count with a `Map<number, number>`.
---
Step 2: `buckets[freq]` is a list of values with that frequency. `buckets` has length `n + 1`.
---
Step 3: walk buckets from high to low frequency, collecting until you have `k` values.
