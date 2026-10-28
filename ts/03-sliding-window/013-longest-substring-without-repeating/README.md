# 013 · Longest Substring Without Repeating Characters

> **Difficulty:** Medium · **Topics:** sliding window, hash map · **Target:** O(n)

Return the length of the longest substring of `s` with no repeated characters.

## Examples

```
"abcabcbb"  ->  3   ("abc")
"bbbbb"     ->  1
"pwwkew"    ->  3   ("wke"; "pwke" is a subsequence, not a substring)
```

## Think about

- The canonical **variable-size sliding window**: grow `right` every step, shrink `left` only
  when the window becomes invalid. Each index enters and leaves once -> O(n).
- Optimization: store the *last index* of each char so `left` can jump instead of crawl.
