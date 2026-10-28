`go(start, remaining)`: if remaining is 0 record; loop `i` from `start`, break if `c[i] > remaining`.
---
Variant 1 recurses with `go(i, remaining - c[i])`. Variant 2 with `go(i + 1, ...)` plus the
same-level duplicate skip from 042.
