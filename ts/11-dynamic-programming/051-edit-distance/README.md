# 051 · Edit Distance (Levenshtein)

> **Difficulty:** Hard · **Topics:** 2-D DP · **Target:** O(m · n)

Return the minimum number of single-character **inserts, deletes and replaces** to turn
`a` into `b`.

```
"horse" -> "ros"            3   (horse -> rorse -> rose -> ros)
"intention" -> "execution"  5
```

## Think about

- `dp[i][j]` = distance between `a[0..i)` and `b[0..j)`. Base cases: `dp[i][0] = i`, `dp[0][j] = j`.
- Match: `dp[i-1][j-1]`. Otherwise `1 + min(replace dp[i-1][j-1], delete dp[i-1][j], insert dp[i][j-1])`.
- Postgres ships this: `CREATE EXTENSION fuzzystrmatch; SELECT levenshtein('horse', 'ros');`.
  For fuzzy search at scale you'd use `pg_trgm` with a GIN index instead — why? (Levenshtein
  can't use an index; trigram similarity can.)
