# 050 · Longest Common Subsequence

> **Difficulty:** Medium · **Topics:** 2-D DP · **Target:** O(m · n) time, O(min(m, n)) space

- `longestCommonSubsequence(a, b)`: length of the LCS.
- `lcsString(a, b)`: one actual LCS string (tests validate it).

```
a = "abcde", b = "ace"  ->  3 ("ace")
```

## Think about

- `dp[i][j]` = LCS of `a[0..i)` and `b[0..j)`. If `a[i-1] === b[j-1]`: `dp[i-1][j-1] + 1`,
  else `max(dp[i-1][j], dp[i][j-1])`.
- Space: each row only depends on the previous row — two rows (or one row + a temp) suffice.
  But reconstruction needs the full table (or Hirschberg's algorithm).
- This is the algorithm behind `diff` and `git diff`.
