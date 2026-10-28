Make `a` the shorter array. Search `i` in `[0, m]`, derive `j = half - i`.
---
`aLeft = i > 0 ? a[i-1] : -Inf`, `aRight = i < m ? a[i] : Inf`, same for b.
If `aLeft > bRight`, move `i` left; if `bLeft > aRight`, move `i` right; else found.
---
Median: odd total -> `max(aLeft, bLeft)`; even -> average of that and `min(aRight, bRight)`.
