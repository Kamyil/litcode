Fill a `(m+1) x (n+1)` table. Then reduce to one row: you need `dp[i-1][j-1]`, so save it in a
variable before overwriting `row[j]`.
