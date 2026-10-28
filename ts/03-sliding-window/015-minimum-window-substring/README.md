# 015 · Minimum Window Substring

> **Difficulty:** Hard · **Topics:** sliding window, counting · **Target:** O(|s| + |t|)

Return the smallest substring of `s` that contains every character of `t` (with
multiplicity). Return `""` if none exists. The answer is unique in the tests.

## Examples

```
s = "ADOBECODEBANC", t = "ABC"  ->  "BANC"
s = "a",             t = "aa"   ->  ""
```

## Think about

- Grow right until the window is valid, then shrink left while it stays valid, recording the
  best. Track validity with a single integer `missing` instead of comparing whole maps.
