# 022 · Koko Eating Bananas (binary search on the answer)

> **Difficulty:** Medium · **Topics:** binary search on answer space · **Target:** O(n log max)

`piles[i]` bananas in pile `i`. Koko eats at speed `k` bananas/hour: each hour she picks one
pile and eats `k` from it (if the pile has fewer, she finishes it and waits out the hour).
Return the minimum integer `k` that lets her finish everything within `h` hours.

## Examples

```
piles = [3, 6, 7, 11],      h = 8  ->  4
piles = [30, 11, 23, 4, 20], h = 5 ->  30
piles = [30, 11, 23, 4, 20], h = 6 ->  23
```

## Think about

- You aren't searching an array — you're searching the **answer space** `[1, max(piles)]`.
  The predicate "can finish at speed k" is monotonic: false...false, true...true. Find the
  first true = `lowerBound` on a predicate.
- Many "minimize the maximum" problems (ship capacity, split array) are this same pattern.
