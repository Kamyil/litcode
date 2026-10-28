Table of size `(m + 1) x (n + 1)` filled with zeros; fill row by row.
---
Reconstruct from `dp[m][n]`: if chars match, take it and go diagonal; else move toward the larger
neighbor (up or left).
