# 014 · Longest Repeating Character Replacement

> **Difficulty:** Medium · **Topics:** sliding window, counting · **Target:** O(n)

String `s` of uppercase letters. You may replace at most `k` characters with any other
letter. Return the length of the longest substring that can be made of one repeated letter.

## Examples

```
s = "ABAB",    k = 2  ->  4
s = "AABABBA", k = 1  ->  4   ("AABA" -> "AAAA", or "ABBB" -> "BBBB")
```

## Think about

- A window is valid iff `windowLength - countOfMostFrequentChar <= k`.
- Subtle: you never need to *decrease* `maxCount` when shrinking. Why does a stale
  (too high) `maxCount` not produce a wrong answer? This is a classic "explain it" question.
